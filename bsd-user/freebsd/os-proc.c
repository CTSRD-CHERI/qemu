/*
 *  FreeBSD process related emulation code
 *
 *  Copyright (c) 2013-15 Stacey D. Son
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#define _WANT_FREEBSD11_STAT
#define _WANT_FREEBSD11_STATFS
#define _WANT_FREEBSD11_DIRENT
#include <sys/param.h>
#include <sys/types.h>
#include <sys/queue.h>
#include <sys/sysctl.h>
#include <sys/unistd.h>
#if defined(__FreeBSD_version) && __FreeBSD_version > 900000
#include <sys/socket.h>
struct kinfo_proc;
#include <libprocstat.h>
#endif
#include <string.h>

#include "qemu.h"

/*
 * The maximum number of additional program arguments added by the emulator
 * to argv in an execve(2) or an fexecve(2) call.
 */
#define MAX_ARGV_EXTRA  7

/*
 * Get the filename for the given file descriptor.
 * Note that this may return NULL (fail) if no longer cached in the kernel.
 */
static char *
get_filename_from_fd(pid_t pid, int fd, char *filename, size_t len)
{
    char *ret = NULL;
#if defined(__FreeBSD_version) && __FreeBSD_version > 900000
    unsigned int cnt;
    struct procstat *procstat = NULL;
    struct kinfo_proc *kp = NULL;
    struct filestat_list *head = NULL;
    struct filestat *fst;

    procstat = procstat_open_sysctl();
    if (procstat == NULL)
        goto out;

    kp = procstat_getprocs(procstat, KERN_PROC_PID, pid, &cnt);
    if (kp == NULL)
        goto out;

    head = procstat_getfiles(procstat, kp, 0);
    if (head == NULL)
        goto out;

    STAILQ_FOREACH(fst, head, next) {
        if (fd == fst->fs_fd) {
            if (fst->fs_path != NULL) {
                (void)strlcpy(filename, fst->fs_path, len);
                ret = filename;
            }
            break;
        }
    }

out:
    if (head != NULL)
        procstat_freefiles(procstat, head);
    if (kp != NULL)
        procstat_freeprocs(procstat, kp);
    if (procstat != NULL)
        procstat_close(procstat);
#endif
    return ret;
}

#if defined(__FreeBSD_version) && __FreeBSD_version < 1100000
static int
is_target_shell_script(int fd, char *interp, size_t size, char **interp_args)
{
    char buf[2], *p, *b;
    ssize_t n;

    if (fd < 0) {
        return 0;
    }
    (void)lseek(fd, 0L, SEEK_SET);
    if (read(fd, buf, 2) != 2) {
        return 0;
    }
    if (buf[0] != '#' && buf[1] != '!') {
        return 0;
    }
    if (size == 0) {
        return 0;
    }
    b = interp;
    /* Remove the trailing whitespace after "#!", if any. */
    while (size != 0) {
        n = read(fd, b, 1);
        if (n < 0 || n == 0) {
            return 0;
        }
        if ((*b != ' ') && (*b != '\t')) {
            b++;
            size--;
            break;
        }
    }
    while (size != 0) {
        n = read(fd, b, size);
        if (n < 0 || n == 0) {
            return 0;
        }
        if ((p = memchr(b, '\n', size)) != NULL) {
            int hasargs = 0;
            *p = 0;

            *interp_args = NULL;
            p = interp;
            while (*p) {
                if ((*p == ' ') || (*p == '\t')) {
                    hasargs = 1;
                    *p = 0;
                } else if (hasargs) {
                    *interp_args = p;
                    break;
                }
                ++p;
            }
            return 1;
        }
        b += n;
        size -= n;
    }

    return 0;
}
#endif

/*
 * execve/fexecve
 */
abi_long freebsd_exec_common(abi_ulong path_or_fd, abi_ulong guest_argp,
        abi_ulong guest_envp, int do_fexec)
{
    size_t argc, envc, ii;
    const char **argv; 
    char **envp, **guestargs, **q;
    char filepath[PATH_MAX];
    bool iself, isscript;
#if defined(__FreeBSD_version) && __FreeBSD_version < 1100000
    char shebangpath[PATH_MAX], *shebangargs;
#endif
    abi_ulong gp;
    abi_uintptr_t addr;
    int fd, total_size;
    void *p;
    abi_long ret;

    argv = NULL;
    total_size = 0;

    /*
     * Calculate argc.
     */
    argc = 0;
    for (gp = guest_argp; gp; gp += sizeof(addr)) {
        if (get_user_uintptr(addr, gp)) {
            return -TARGET_EFAULT;
        }
        if (!uintptr_vaddr(addr)) {
            break;
        }
        argc++;
    }

    /*
     * Calculate envc.
     */
    envc = 0;
    for (gp = guest_envp; gp; gp += sizeof(addr)) {
        if (get_user_uintptr(addr, gp)) {
            return -TARGET_EFAULT;
        }
        if (!uintptr_vaddr(addr)) {
            break;
        }
        envc++;
    }

    /*
     * Copy a guest argv and lock its content.
     */
    guestargs = g_new0(char *, argc + 1);
    for (gp = guest_argp, q = guestargs; gp; gp += sizeof(addr), q++) {
        if (get_user_uintptr(addr, gp)) {
            ret = -TARGET_EFAULT;
            goto execve_end;
        }
        if (!uintptr_vaddr(addr)) {
            break;
        }
        *q = lock_user_string(uintptr_vaddr(addr));
        if (*q == NULL) {
            ret = -TARGET_EFAULT;
            goto execve_end;
        }
        total_size += strlen(*q) + 1;
    }
    *q++ = NULL;

    /*
     * Copy a guest envp and lock its content.
     */
    envp = g_new0(char *, envc + 1);
    for (gp = guest_envp, q = envp; gp; gp += sizeof(addr), q++) {
        if (get_user_uintptr(addr, gp)) {
            ret = -TARGET_EFAULT;
            goto execve_end;
        }
        if (!uintptr_vaddr(addr)) {
            break;
        }
        *q = lock_user_string(uintptr_vaddr(addr));
        if (*q == NULL) {
            ret = -TARGET_EFAULT;
            goto execve_end;
        }
        total_size += strlen(*q) + 1;
    }
    *q = NULL;

    /*
     * This case will not be caught by the host's execve() if its
     * page size is bigger than the target's.
     */
    if (total_size > MAX_ARG_PAGES * TARGET_PAGE_SIZE) {
        ret = -TARGET_E2BIG;
        goto execve_end;
    }


    /*
     * Determine a file descriptor and a file path.
     */
    if (do_fexec) {
        fd = (int)path_or_fd;

        if (get_filename_from_fd(getpid(), fd, filepath,
                                 sizeof(filepath)) == NULL) {
            ret = -TARGET_EBADF;
            goto execve_end;
        }
    } else {
        p = lock_user_string(path_or_fd);
        if (p == NULL) {
            ret = -TARGET_EFAULT;
            goto execve_end;
        }
        strncpy(filepath, p, sizeof(filepath));
        unlock_user(p, path_or_fd, 0);

        fd = open(filepath, O_RDONLY | O_CLOEXEC);
        if (fd < 0) {
            ret = -TARGET_EBADF;
            goto execve_end;
        }
    }
    assert(fd >= 0);
    /*
     * Determine if the file is an ELF binary or a script.
     */
    iself = (is_target_elf_binary(fd) == 1);
    isscript = false;
#if defined(__FreeBSD_version) && __FreeBSD_version < 1100000
    /*
     * The FreeBSD host version doesn't have the imgact_binmisc kernel module.
     *
     * In case the file is a script, we must explicitly execute it with the
     * emulator.
     *
     * XXKW: In fact, the imgact_binmisc kernel module was merged into FreeBSD
     * 10.1. Also, it's insufficient to determine if the imgact_binmisc is
     * available based on a FreeBSD version. Instead, the check should verify
     * if the module is loaded.
     */
    if (!iself) {
        isscript = (is_target_shell_script(fd, shebangpath, sizeof(shebangpath),
                                           &shebangargs) != 0);
        /*
         * If shebangargs exists, then it must be a script.
         */
        assert(shebangargs == NULL || isscript);
    }
#endif
    if (!do_fexec) {
        close(fd);
    }

    if (!iself && !isscript) {
        /*
         * The file is not an ELF binary not it's a script on a host without the
         * imgact_binmisc kernel module.
         *
         * Execute a system call with the original guest arguments.
         */
        if (do_fexec) {
            ret = get_errno(fexecve(fd, guestargs, envp));
        } else {
            ret = get_errno(execve(p, guestargs, envp));
        }
        goto execve_end;
    }

    /*
     * The emulator recognizes the file.
     *
     * We are going to execute the file explicitly with the emulator because of
     * two reasons:
     * - fexecve(2) could result in executing the file without the emulator;
     * - A host might not support or might not have loaded the imgact_binmisc
     *   kernel module.
     *
     * XXXKW: The above list might be incomplete. Any other reasons should be
     * appended to it.
     */
    assert(iself || isscript);

    /*
     * Construct argv.
     *
     * For a list of possible argv values, see below execve(2) and fexecve(2)
     * calls.
     */
    argv = g_new0(const char *, argc + MAX_ARGV_EXTRA + 1);
    ii = 0;

    /*
     * Use the user mode to execute the file.
     */
    argv[ii++] = getprogname();

    if (iself) {
        /*
         * Pass the original program name as a QEMU argument.
         */
        argv[ii++] = "-0";
        argv[ii++] = guestargs[0];
    }

    /*
     * Add QEMU arguments required to correctly execute the file.
     */
    argv[ii++] = "-L";
    argv[ii++] = interp_prefix;

#if defined(__FreeBSD_version) && __FreeBSD_version < 1100000
    if (isscript) {
        /*
         * Use an interpreter found in the file with its optional arguments to
         * execute the script.
         */
        argv[ii++] = shebangpath;
        if (shebangargs != NULL) {
            argv[ii++] = shebangargs;
        }
    }
#endif

    /*
     * Add the file path to be executed.
     */
    argv[ii++] = filepath;

    /*
     * Finally, copy all guest arguments except for a command name as it
     * could've been replaced.
     */
    if (argc > 1) {
        (void)memcpy(&argv[ii], &guestargs[1], (argc - 1) * sizeof(*guestargs));
        ii += argc - 1;
    }

    /*
     * Terminate argv with a NULL pointer.
     */
    /* The original command name is replaced with filepath. */
    assert(ii <= argc + MAX_ARGV_EXTRA);
    argv[ii] = NULL;

    if (iself) {
        /*
         * In case of an ELF binary, argv includes:
         * 0: getprogname()
         * 1: -0
         * 2: guestargs[0]
         * 3: -L
         * 4: interp_prefix
         * 5: filepath
         * 6: guestargs[1]
         * 7: guestargs[2]
         * ...
         */
        ret = get_errno(execve(qemu_proc_pathname, (char * const *)argv, envp));
#if defined(__FreeBSD_version) && __FreeBSD_version < 1100000
    } else if (isscript) {
        /*
         * In case of a script, argv includes (shebangsargs is optional):
         * 0: getprogname()
         * 1: -L
         * 2: interp_prefix
         * 3: shebangpath
         * 4: shebangargs
         * 5: filepath
         * 6: guestargs[1]
         * 7: guestargs[2]
         * ...
         */
        ret = get_errno(execve(qemu_proc_pathname, (char * const *)argv, envp));
#endif
    } else {
        /* Unreachable. */
        abort();
    }

execve_end:
    /*
     * Unlock the guest argv.
     */
    for (gp = guest_argp, q = guestargs; *q; gp += sizeof(abi_uintptr_t), q++) {
        if (get_user_uintptr(addr, gp)) {
            break;
        }
        if (!uintptr_vaddr(addr)) {
            break;
        }
        unlock_user(*q, uintptr_vaddr(addr), 0);
    }

    /*
     * Unlock the guest envp.
     */
    for (gp = guest_envp, q = envp; *q; gp += sizeof(abi_uintptr_t), q++) {
        if (get_user_uintptr(addr, gp)) {
            break;
        }
        if (!uintptr_vaddr(addr)) {
            break;
        }
        unlock_user(*q, uintptr_vaddr(addr), 0);
    }

    g_free(guestargs);
    g_free(argv);
    g_free(envp);

    return ret;
}

#if !defined(__FreeBSD_version)
#error __FreeBSD_version not defined!
#endif

#if defined(__FreeBSD_version) && __FreeBSD_version >= 1001510
#include <sys/procctl.h>
#endif

#if defined(__FreeBSD_version) && __FreeBSD_version >= 1001510 && defined(PROC_REAP_ACQUIRE)

static abi_long
t2h_procctl_cmd(int target_cmd, int *host_cmd)
{

    switch(target_cmd) {
    case TARGET_PROC_SPROTECT:
        *host_cmd = PROC_SPROTECT;
        break;

    case TARGET_PROC_REAP_ACQUIRE:
        *host_cmd = PROC_REAP_ACQUIRE;
        break;

    case TARGET_PROC_REAP_RELEASE:
        *host_cmd = PROC_REAP_RELEASE;
        break;

    case TARGET_PROC_REAP_STATUS:
        *host_cmd = PROC_REAP_STATUS;
        break;

    case TARGET_PROC_REAP_KILL:
        *host_cmd = PROC_REAP_KILL;
        break;

    default:
        return (-TARGET_EINVAL);
    }

    return 0;
}

static abi_long
t2h_reaper_kill(abi_ulong target_rk_addr, struct procctl_reaper_kill *host_rk)
{
    struct target_procctl_reaper_kill *target_rk;

    if (!lock_user_struct(VERIFY_READ, target_rk, target_rk_addr, 1))
        return -TARGET_EFAULT;
    __get_user(host_rk->rk_sig, &target_rk->rk_sig);
    __get_user(host_rk->rk_flags, &target_rk->rk_flags);
    __get_user(host_rk->rk_subtree, &target_rk->rk_subtree);
    __get_user(host_rk->rk_killed, &target_rk->rk_killed);
    __get_user(host_rk->rk_fpid, &target_rk->rk_fpid);
    unlock_user_struct(target_rk, target_rk_addr, 0);

    return 0;
}

static abi_long
h2t_reaper_status(struct procctl_reaper_status *host_rs,
        abi_ulong target_rs_addr)
{
    struct target_procctl_reaper_status *target_rs;

    if (!lock_user_struct(VERIFY_WRITE, target_rs, target_rs_addr, 0))
        return -TARGET_EFAULT;
    __put_user(host_rs->rs_flags, &target_rs->rs_flags);
    __put_user(host_rs->rs_children, &target_rs->rs_children);
    __put_user(host_rs->rs_descendants, &target_rs->rs_descendants);
    __put_user(host_rs->rs_reaper, &target_rs->rs_reaper);
    __put_user(host_rs->rs_pid, &target_rs->rs_pid);
    unlock_user_struct(target_rs, target_rs_addr, 1);

    return 0;
}

static abi_long
h2t_reaper_kill(struct procctl_reaper_kill *host_rk, abi_ulong target_rk_addr)
{
    struct target_procctl_reaper_kill *target_rk;

    if (!lock_user_struct(VERIFY_WRITE, target_rk, target_rk_addr, 0))
        return -TARGET_EFAULT;
    __put_user(host_rk->rk_sig, &target_rk->rk_sig);
    __put_user(host_rk->rk_flags, &target_rk->rk_flags);
    __put_user(host_rk->rk_subtree, &target_rk->rk_subtree);
    __put_user(host_rk->rk_killed, &target_rk->rk_killed);
    __put_user(host_rk->rk_fpid, &target_rk->rk_fpid);
    unlock_user_struct(target_rk, target_rk_addr, 1);

    return 0;
}

static abi_long
h2t_procctl_reaper_pidinfo(struct procctl_reaper_pidinfo *host_pi,
        abi_ulong target_pi_addr)
{
    struct target_procctl_reaper_pidinfo *target_pi;

    if (!lock_user_struct(VERIFY_WRITE, target_pi, target_pi_addr, 0))
        return -TARGET_EFAULT;
    __put_user(host_pi->pi_pid, &target_pi->pi_pid);
    __put_user(host_pi->pi_subtree, &target_pi->pi_subtree);
    __put_user(host_pi->pi_flags, &target_pi->pi_flags);
    unlock_user_struct(target_pi, target_pi_addr, 1);

    return 0;
}

abi_long
do_freebsd_procctl(void *cpu_env, int idtype, abi_ulong arg2, abi_ulong arg3,
       abi_ulong arg4, abi_ulong arg5, abi_ulong arg6)
{
    abi_long error = 0, target_rp_pids;
    void *data;
    int host_cmd, flags;
    uint32_t u, target_rp_count;
    union {
        struct procctl_reaper_status rs;
        struct procctl_reaper_pids rp;
        struct procctl_reaper_kill rk;
    } host;
    struct target_procctl_reaper_pids *target_rp;
    id_t id; /* 64-bit */
    int target_cmd;
    abi_ulong target_arg;

#if TARGET_ABI_BITS == 32
    /* See if we need to align the register pairs. */
    if (regpairs_aligned(cpu_env)) {
        id = (id_t)target_arg64(arg3, arg4);
        target_cmd = (int)arg5;
        target_arg = arg6;
    } else {
        id = (id_t)target_arg64(arg2, arg3);
        target_cmd = (int)arg4;
        target_arg = arg5;
    }
#else
    id = (id_t)arg2;
    target_cmd = (int)arg3;
    target_arg = arg4;
#endif

    error = t2h_procctl_cmd(target_cmd, &host_cmd);
    if (error)
        return error;

    switch (host_cmd) {
    case PROC_SPROTECT:
        data = &flags;
        break;

    case PROC_REAP_ACQUIRE:
    case PROC_REAP_RELEASE:
        if (target_arg == 0)
            data = NULL;
        else
            error = -TARGET_EINVAL;
        break;

    case PROC_REAP_STATUS:
        data = &host.rs;
        break;

    case PROC_REAP_GETPIDS:
        if (!lock_user_struct(VERIFY_READ, target_rp, target_arg, 1)) {
            return -TARGET_EFAULT;
        }
        __get_user(target_rp_count, &target_rp->rp_count);
        __get_user(target_rp_pids, &target_rp->rp_pids);
        unlock_user_struct(target_rp, target_arg, 0);
        host.rp.rp_count = target_rp_count;
        /* XXX we should check target_rc_count to see if it is reasonable. */
        host.rp.rp_pids = alloca(target_rp_count *
                sizeof(struct procctl_reaper_pidinfo));
        if (host.rp.rp_pids == NULL)
            error = -TARGET_ENOMEM;
        else
            data = &host.rp;
        break;

    case PROC_REAP_KILL:
        error = t2h_reaper_kill(target_arg, &host.rk);
        break;
    }

    if (error)
        return error;

    error = get_errno(procctl(idtype, id, host_cmd, data));

    if (error)
        return error;

    switch(host_cmd) {
    case PROC_SPROTECT:
        if (put_user_s32(flags, target_arg))
            return -TARGET_EFAULT;
        break;

    case PROC_REAP_STATUS:
        error = h2t_reaper_status(&host.rs, target_arg);
        break;

    case PROC_REAP_GETPIDS:
        /* copyout reaper pidinfo */
        for (u = 0; u < target_rp_count; u++) {
            error = h2t_procctl_reaper_pidinfo(&host.rp.rp_pids[u],
                    target_rp_pids +
                    (u * sizeof(struct target_procctl_reaper_pidinfo)));
            if (error)
                break;
        }
        break;

    case PROC_REAP_KILL:
        error = h2t_reaper_kill(&host.rk, target_arg);
        break;
    }

    return error;
}

#else /* ! __FreeBSD_version >= 1100000 */

abi_long
do_freebsd_procctl(__unused void *cpu_env, __unused int idtype,
        __unused abi_ulong arg2, __unused abi_ulong arg3,
        __unused abi_ulong arg4, __unused abi_ulong arg5,
        __unused abi_ulong arg6)
{

    qemu_log("qemu: Unsupported syscall procctl()\n");
    return -TARGET_ENOSYS;
}


#endif /* ! __FreeBSD_version >= 1100000 */
