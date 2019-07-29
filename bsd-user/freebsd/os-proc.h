/*
 *  process related system call shims and definitions
 *
 *  Copyright (c) 2013-14 Stacey D. Son
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

#ifndef __FREEBSD_PROC_H_
#define __FREEBSD_PROC_H_

#include <sys/param.h>
#include <sys/capsicum.h>
#if defined(__FreeBSD_version) && __FreeBSD_version >= 1000000
#include <sys/procctl.h>
#include <sys/signal.h>
#endif
#include <sys/types.h>
#if defined(__FreeBSD_version) && __FreeBSD_version > 900000
#include <sys/procdesc.h>
#endif
#include <sys/wait.h>
#include <unistd.h>

#include "qemu.h"
#include "target_arch_cpu.h"
#include "target_os_user.h"

extern int __setugid(int flag);
extern int pdwait4(int fd, int *status, int options, struct rusage *rusage);

/* execve(2) */
static inline abi_long do_freebsd_execve(abi_ulong path_or_fd, abi_ulong argp,
        abi_ulong envp)
{

    return freebsd_exec_common(path_or_fd, argp, envp, 0);
}

/* fexecve(2) */
static inline abi_long do_freebsd_fexecve(abi_ulong path_or_fd, abi_ulong argp,
        abi_ulong envp)
{

    return freebsd_exec_common(path_or_fd, argp, envp, 1);
}

/* wait4(2) */
static inline abi_long do_freebsd_wait4(abi_long arg1, abi_ulong target_status,
        abi_long arg3, abi_ulong target_rusage)
{
    abi_long ret;
    int status;
    struct rusage rusage, *rusage_ptr = NULL;

    if (target_rusage) {
        rusage_ptr = &rusage;
    }
    ret = get_errno(wait4(arg1, &status, arg3, rusage_ptr));
    if (target_status != 0) {
        status = host_to_target_waitstatus(status);
        if (put_user_s32(status, target_status) != 0) {
            return -TARGET_EFAULT;
        }
    }
    if (target_rusage != 0) {
        host_to_target_rusage(target_rusage, &rusage);
    }
    return ret;
}

#if defined(__FreeBSD_version) && __FreeBSD_version >= 1000000
/* wait6(2) */
static inline abi_long do_freebsd_wait6(void *cpu_env, abi_long idtype, 
    abi_long id1, abi_long id2,
    abi_ulong target_status, abi_long options, abi_ulong target_wrusage,
	abi_ulong target_infop, abi_ulong pad1)
{
    abi_long ret;
    int status;
    struct __wrusage wrusage, *wrusage_ptr = NULL;
    siginfo_t info;
    void *p;

    if (regpairs_aligned(cpu_env) != 0) {
		/* printf("shifting args\n"); */
		/* 64-bit id is aligned, so shift all the arguments over by one */
		id1 = id2;
		id2 = target_status;
		target_status = options;
		options = target_wrusage;
		target_wrusage = target_infop;
		target_infop = pad1;
    }

    if (target_wrusage) {
        wrusage_ptr = &wrusage;
    }
    ret = get_errno(wait6(idtype, target_arg64(id1, id2), &status, options, wrusage_ptr, &info));
    if (target_status != 0) {
        status = host_to_target_waitstatus(status);
        if (put_user_s32(status, target_status) != 0) {
            return -TARGET_EFAULT;
        }
    }
    if (target_wrusage != 0) {
        host_to_target_wrusage(target_wrusage, &wrusage);
    }
    if (target_infop != 0) {
        p = lock_user(VERIFY_WRITE, target_infop, sizeof(target_siginfo_t), 0);
        if (p == NULL) {
            return -TARGET_EFAULT;
        }
        host_to_target_siginfo(p, &info);
        unlock_user(p, target_infop, sizeof(target_siginfo_t));
    }
    return ret;
}

#else /* !  __FreeBSD_version >= 1000000 */

static inline abi_long do_freebsd_wait6( __unused abi_long idtype,
	__unused abi_long id,  __unused abi_ulong target_status,
	__unused abi_long options, __unused abi_ulong target_wrusage,
	__unused abi_ulong target_infop)
{

    qemu_log("qemu: Unsupported syscall wait6()\n");
    return -TARGET_ENOSYS;
}
#endif /* __FreeBSD_version >= 1000000 */

#if defined(__FreeBSD_version) && __FreeBSD_version > 900000
/* setloginclass(2) */
static inline abi_long do_freebsd_setloginclass(abi_ulong arg1)
{
    abi_long ret;
    void *p;

    p = lock_user_string(arg1);
    if (p == NULL) {
        return -TARGET_EFAULT;
    }
    ret = get_errno(setloginclass(p));
    unlock_user(p, arg1, 0);

    return ret;
}

/* getloginclass(2) */
static inline abi_long do_freebsd_getloginclass(abi_ulong arg1, abi_ulong arg2)
{
    abi_long ret;
    void *p;

    p = lock_user_string(arg1);
    if (p == NULL) {
        return -TARGET_EFAULT;
    }
    ret = get_errno(getloginclass(p, arg2));
    unlock_user(p, arg1, 0);

    return ret;
}

/* pdwait4(2) */
static inline abi_long do_freebsd_pdwait4(abi_long arg1,
        abi_ulong target_status, abi_long arg3, abi_ulong target_rusage)
{
    abi_long ret;
    int status;
    struct rusage rusage, *rusage_ptr = NULL;

    if (target_rusage) {
        rusage_ptr = &rusage;
    }
    ret = get_errno(pdwait4(arg1, &status, arg3, rusage_ptr));
    if (target_status != 0) {
        status = host_to_target_waitstatus(status);
        if (put_user_s32(status, target_status) != 0) {
            return -TARGET_EFAULT;
        }
    }
    if (target_rusage != 0) {
        host_to_target_rusage(target_rusage, &rusage);
    }
    return ret;
}

/* pdgetpid(2) */
static inline abi_long do_freebsd_pdgetpid(abi_long fd, abi_ulong target_pidp)
{
    abi_long ret;
    pid_t pid;

    ret = get_errno(pdgetpid(fd, &pid));
    if (!is_error(ret)) {
        if (put_user_u32(pid, target_pidp)) {
            return -TARGET_EFAULT;
        }
    }
    return ret;
}

#else

/* setloginclass(2) */
static inline abi_long do_freebsd_setloginclass(abi_ulong arg1)
{

    qemu_log("qemu: Unsupported syscall setloginclass()\n");
    return -TARGET_ENOSYS;
}

/* getloginclass(2) */
static inline abi_long do_freebsd_getloginclass(abi_ulong arg1, abi_ulong arg2)
{

    qemu_log("qemu: Unsupported syscall getloginclass()\n");
    return -TARGET_ENOSYS;
}

/* pdwait4(2) */
static inline abi_long do_freebsd_pdwait4(abi_long arg1,
        abi_ulong target_status, abi_long arg3, abi_ulong target_rusage)
{

    qemu_log("qemu: Unsupported syscall pdwait4()\n");
    return -TARGET_ENOSYS;
}

/* pdgetpid(2) */
static inline abi_long do_freebsd_pdgetpid(abi_long fd, abi_ulong target_pidp)
{

    qemu_log("qemu: Unsupported syscall pdgetpid()\n");
    return -TARGET_ENOSYS;
}
#endif /* !  __FreeBSD_version > 900000 */

/* undocumented __setugid */
static inline abi_long do_freebsd___setugid(abi_long arg1)
{

    return get_errno(__setugid(arg1));
}

/* fork(2) */
static inline abi_long do_freebsd_fork(void *cpu_env)
{
    abi_long ret;
    abi_ulong child_flag;

    fork_start();
    ret = fork();
    if (ret == 0) {
        /* child */
        child_flag = 1;
        target_cpu_clone_regs(cpu_env, 0);
    } else {
        /* parent */
        child_flag = 0;
    }

    /*
     * The fork system call sets a child flag in the second return
     * value: 0 for parent process, 1 for child process.
     */
    set_second_rval(cpu_env, child_flag);

    fork_end(child_flag);

    return ret;
}

/* vfork(2) */
static inline abi_long do_freebsd_vfork(void *cpu_env)
{

    return do_freebsd_fork(cpu_env);
}

/* rfork(2) */
static inline abi_long do_freebsd_rfork(void *cpu_env, abi_long flags)
{
    abi_long ret;
    abi_ulong child_flag;

    fork_start();
    ret = rfork(flags);
    if (ret == 0) {
        /* child */
        child_flag = 1;
        target_cpu_clone_regs(cpu_env, 0);
    } else {
        /* parent */
        child_flag = 0;
    }

    /*
     * The fork system call sets a child flag in the second return
     * value: 0 for parent process, 1 for child process.
     */
    set_second_rval(cpu_env, child_flag);
    fork_end(child_flag);

    return ret;

}

#if defined(__FreeBSD_version) && __FreeBSD_version > 900000
/* pdfork(2) */
static inline abi_long do_freebsd_pdfork(void *cpu_env, abi_ulong target_fdp,
        abi_long flags)
{
    abi_long ret;
    abi_ulong child_flag;
    int fd;

    fork_start();
    ret = pdfork(&fd, flags);
    if (ret == 0) {
        /* child */
        child_flag = 1;
        target_cpu_clone_regs(cpu_env, 0);
    } else {
        /* parent */
        child_flag = 0;
    }
    if (put_user_s32(fd, target_fdp)) {
        return -TARGET_EFAULT;
    }

    /*
     * The fork system call sets a child flag in the second return
     * value: 0 for parent process, 1 for child process.
     */
    set_second_rval(cpu_env, child_flag);
    fork_end(child_flag);

    return ret;
}

#else

/* pdfork(2) */
static inline abi_long do_freebsd_pdfork(void *cpu_env, abi_ulong arg1,
        abi_long arg2)
{

    qemu_log("qemu: Unsupported syscall pdfork()\n");
    return -TARGET_ENOSYS;
}

#endif /* __FreeBSD_version > 900000 */

/* jail(2) */
static inline abi_long do_freebsd_jail(abi_ulong arg1)
{

    qemu_log("qemu: Unsupported syscall jail()\n");
    return -TARGET_ENOSYS;
}

/* jail_attach(2) */
static inline abi_long do_freebsd_jail_attach(abi_long arg1)
{

    qemu_log("qemu: Unsupported syscall jail_attach()\n");
    return -TARGET_ENOSYS;
}

/* jail_remove(2) */
static inline abi_long do_freebsd_jail_remove(abi_long arg1)
{

    qemu_log("qemu: Unsupported syscall jail_remove()\n");
    return -TARGET_ENOSYS;
}

/* jail_get(2) */
static inline abi_long do_freebsd_jail_get(abi_ulong arg1, abi_long arg2,
        abi_long arg3)
{

    qemu_log("qemu: Unsupported syscall jail_get()\n");
    return -TARGET_ENOSYS;
}

/* jail_set(2) */
static inline abi_long do_freebsd_jail_set(abi_ulong arg1, abi_long arg2,
        abi_long arg3)
{

    qemu_log("qemu: Unsupported syscall jail_set()\n");
    return -TARGET_ENOSYS;
}

/* cap_enter(2) */
static inline abi_long do_freebsd_cap_enter(void)
{

    qemu_log("qemu: Unsupported syscall cap_enter()\n");
    return -TARGET_ENOSYS;
}

/* cap_new(2) */
static inline abi_long do_freebsd_cap_new(abi_long arg1, abi_ulong arg2)
{

    qemu_log("qemu: Unsupported syscall cap_new()\n");
    return -TARGET_ENOSYS;
}

/* cap_getrights(2) */
static inline abi_long do_freebsd_cap_getrights(abi_long arg1, abi_ulong arg2)
{

    qemu_log("qemu: Unsupported syscall cap_getrights()\n");
    return -TARGET_ENOSYS;
}

/* cap_getmode(2) */
static inline abi_long do_freebsd_cap_getmode(abi_ulong arg1)
{

    qemu_log("qemu: Unsupported syscall cap_getmode()\n");
    return -TARGET_ENOSYS;
}

/* cap_rights_limit(2) */
static inline abi_long do_freebsd_cap_rights_limit(int fd, abi_ulong raddr)
{
    cap_rights_t rights;
    target_cap_rights_t *target_rights;
    int i, nrights;

    if (fd == STDERR_FILENO || fd == STDOUT_FILENO || fd == STDIN_FILENO)
        return -TARGET_ENOSYS;
    if (lock_user_struct(VERIFY_READ, target_rights, raddr, 1) == NULL)
        return -TARGET_EFAULT;

    memset(&rights, 0, sizeof(rights));
    nrights = MIN(sizeof(target_rights->cr_rights), sizeof(rights.cr_rights));
    for (i = 0; i < nrights; ++i) {
        rights.cr_rights[i] = tswap64(target_rights->cr_rights[i]);
    }
    unlock_user_struct(target_rights, raddr, 1);

    return (get_errno(cap_rights_limit(fd, &rights)));
}

/* cap_ioctls_limit(2) */
static inline abi_long do_freebsd_cap_ioctls_limit(int fd, abi_ulong cmd_addr,
	abi_ulong ncmds)
{
    abi_ulong i, *target_cmds;
    unsigned long *cmds;
    abi_long ret;

    if (fd == STDERR_FILENO || fd == STDOUT_FILENO || fd == STDIN_FILENO)
        return -TARGET_ENOSYS;
    target_cmds = lock_user(VERIFY_READ, cmd_addr, sizeof(*target_cmds) * ncmds,
        1);
    if (target_cmds == NULL)
        return -TARGET_EFAULT;

    cmds = malloc(sizeof(*cmds) * ncmds);
    if (cmds == NULL)
        return -TARGET_ENOMEM;
    for (i = 0; i < ncmds; ++i) {
        cmds[i] = tswapl(target_cmds[i]);
    }
    unlock_user(target_cmds, cmd_addr, sizeof(*target_cmds) * ncmds);
    ret = cap_ioctls_limit(fd, cmds, ncmds);
    free(cmds);
	return (get_errno(ret));
}

/* cap_ioctls_get(2) */
static inline abi_long do_freebsd_cap_ioctls_get(int fd, abi_ulong cmd_addr,
	abi_ulong ncmds)
{
    abi_ulong *target_cmds;
    unsigned long *cmds;
    ssize_t i, ret;

    cmds = malloc(sizeof(*cmds) * ncmds);
    if (cmds == NULL)
        return -TARGET_ENOMEM;
    ret = cap_ioctls_get(fd, cmds, ncmds);
    /* cap_ioctls_get will not modify buffer if all allowed */
    if (ret == CAP_IOCTLS_ALL) {
        free(cmds);
        return CAP_IOCTLS_ALL;
    } else if (ret < 0) {
        free(cmds);
        return (get_errno(ret));
    }

    target_cmds = lock_user(VERIFY_WRITE, cmd_addr,
        sizeof(*target_cmds) * ncmds, 1);
    if (target_cmds == NULL)
        return -TARGET_EFAULT;
    for (i = 0; i < ret; ++i) {
        target_cmds[i] = tswapl(cmds[i]);
    }
    free(cmds);
    unlock_user(target_cmds, cmd_addr, sizeof(*target_cmds) * ncmds);
    return (ret);
}

/* cap_fcntls_limit(2) */
static inline abi_long do_freebsd_cap_fcntls_limit(int fd, abi_ulong rights)
{
    uint32_t host_rights;

    if (fd == STDERR_FILENO || fd == STDOUT_FILENO || fd == STDIN_FILENO)
        return -TARGET_ENOSYS;
    host_rights = tswap32(rights);
    return (get_errno(cap_fcntls_limit(fd, host_rights)));
}

/* cap_fcntls_get(2) */
static inline abi_long do_freebsd_cap_fcntls_get(int fd, abi_ulong raddr)
{
    uint32_t host_rights;
    int ret;

    ret = cap_fcntls_get(fd, &host_rights);
    if (ret < 0)
        return (get_errno(ret));

    put_user_u32(host_rights, raddr);
    return (0);
}

/* audit(2) */
static inline abi_long do_freebsd_audit(abi_ulong arg1, abi_ulong arg2)
{

    qemu_log("qemu: Unsupported syscall audit()\n");
    return -TARGET_ENOSYS;
}

/* auditon(2) */
static inline abi_long do_freebsd_auditon(abi_long arg1, abi_ulong arg2,
        abi_ulong arg3)
{

    qemu_log("qemu: Unsupported syscall auditon()\n");
    return -TARGET_ENOSYS;
}

/* getaudit(2) */
static inline abi_long do_freebsd_getaudit(abi_ulong arg1)
{

    qemu_log("qemu: Unsupported syscall getaudit()\n");
    return -TARGET_ENOSYS;
}

/* setaudit(2) */
static inline abi_long do_freebsd_setaudit(abi_ulong arg1)
{

    qemu_log("qemu: Unsupported syscall setaudit()\n");
    return -TARGET_ENOSYS;
}

/* getaudit_addr(2) */
static inline abi_long do_freebsd_getaudit_addr(abi_ulong arg1,
        abi_ulong arg2)
{

    qemu_log("qemu: Unsupported syscall getaudit_addr()\n");
    return -TARGET_ENOSYS;
}

/* setaudit_addr(2) */
static inline abi_long do_freebsd_setaudit_addr(abi_ulong arg1,
        abi_ulong arg2)
{

    qemu_log("qemu: Unsupported syscall setaudit_addr()\n");
    return -TARGET_ENOSYS;
}

/* auditctl(2) */
static inline abi_long do_freebsd_auditctl(abi_ulong arg1)
{

    qemu_log("qemu: Unsupported syscall auditctl()\n");
    return -TARGET_ENOSYS;
}

#endif /* ! __FREEBSD_PROC_H_ */
