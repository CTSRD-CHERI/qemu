/*
 *  FreeBSD process related emulation code
 *
 *  Copyright (c) 2013 Stacey D. Son
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

#include <sys/types.h>
#include <sys/queue.h>
#include <sys/sysctl.h>
#include <sys/unistd.h>
#if defined(__FreeBSD_version) && __FreeBSD_version > 900000
#include <libprocstat.h>
#endif
#include <string.h>

#include "qemu.h"

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
    struct kinfo_proc *kipp = NULL;
    struct filestat_list *head = NULL;
    struct filestat *fst;

    procstat = procstat_open_sysctl();
    if (NULL == procstat) {
        goto out;
    }

    kipp = procstat_getprocs(procstat, KERN_PROC_PID, pid, &cnt);
    if (NULL == kipp) {
        goto out;
    }

    head = procstat_getfiles(procstat, kipp, 0);
    if (NULL == head) {
        goto out;
    }

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
    if (head != NULL) {
        procstat_freefiles(procstat, head);
    }
    if (kipp != NULL) {
        procstat_freeprocs(procstat, kipp);
    }
    if (procstat != NULL) {
        procstat_close(procstat);
    }
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
    char **argp, **envp, **qargp, **qarg1, **qarg0, **qargend;
    int argc, envc;
    abi_ulong gp;
    abi_ulong addr;
    char **q;
    int total_size = 0;
    void *p;
    abi_long ret;

    argc = 0;
    for (gp = guest_argp; gp; gp += sizeof(abi_ulong)) {
        if (get_user_ual(addr, gp)) {
            return -TARGET_EFAULT;
        }
        if (!addr) {
            break;
        }
        argc++;
    }
    envc = 0;
    for (gp = guest_envp; gp; gp += sizeof(abi_ulong)) {
        if (get_user_ual(addr, gp)) {
            return -TARGET_EFAULT;
        }
        if (!addr) {
            break;
        }
        envc++;
    }

    qarg0 = argp =  alloca((argc + 7) * sizeof(void *));
    /* save the first agrument for the emulator */
    *argp++ = (char *)getprogname();
    qargp = argp;
    *argp++ = (char *)getprogname();
    qarg1 = argp;
    envp = alloca((envc + 1) * sizeof(void *));
    for (gp = guest_argp, q = argp; gp; gp += sizeof(abi_ulong), q++) {
        if (get_user_ual(addr, gp)) {
            ret = -TARGET_EFAULT;
            goto execve_end;
        }
        if (!addr) {
            break;
        }
        *q = lock_user_string(addr);
        if (*q == NULL) {
            ret = -TARGET_EFAULT;
            goto execve_end;
        }
        total_size += strlen(*q) + 1;
    }
    *q++ = NULL;
    qargend = q;

    for (gp = guest_envp, q = envp; gp; gp += sizeof(abi_ulong), q++) {
        if (get_user_ual(addr, gp)) {
            ret = -TARGET_EFAULT;
            goto execve_end;
        }
        if (!addr) {
            break;
        }
        *q = lock_user_string(addr);
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

    if (do_fexec) {
#if defined(__FreeBSD_version) && __FreeBSD_version < 1100000
        char execpath[PATH_MAX], *scriptargs;
#endif /* __FreeBSD_version < 1100000 */

        if (((int)path_or_fd > 0 &&
            is_target_elf_binary((int)path_or_fd)) == 1) {
            char execpath[PATH_MAX];

            /*
             * The executable is an elf binary for the target
             * arch.  execve() it using the emulator if we can
             * determine the filename path from the fd.
             */
            if (get_filename_from_fd(getpid(), (int)path_or_fd, execpath,
                        sizeof(execpath)) != NULL) {
                *qarg1 = execpath;
#ifndef DONT_INHERIT_INTERP_PREFIX
                memmove(qarg1 + 2, qarg1, (qargend-qarg1) * sizeof(*qarg1));
                *qarg1++ = (char *)"-L";
                *qarg1++ = (char *)interp_prefix;
#endif
                ret = get_errno(execve(qemu_proc_pathname, qargp, envp));
            } else {
                /* Getting the filename path failed. */
                ret = -TARGET_EBADF;
                goto execve_end;
            }
#if defined(__FreeBSD_version) && __FreeBSD_version < 1100000
        } else if (is_target_shell_script((int)path_or_fd, execpath,
                    sizeof(execpath), &scriptargs) != 0) {
            char scriptpath[PATH_MAX];

            /* execve() as a target script using emulator. */
            if (get_filename_from_fd(getpid(), (int)path_or_fd, scriptpath,
                        sizeof(scriptpath)) != NULL) {
                *qargp = execpath;
                *qarg1 = scriptpath;
#ifndef DONT_INHERIT_INTERP_PREFIX
                memmove(qargp + 2, qargp, (qargend-qargp) * sizeof(*qargp));
                qargp[0] = (char *)"-L";
                qargp[1] = (char *)interp_prefix;
                qarg1 += 2;
                qargend += 2;
#endif
                if (scriptargs) {
                    memmove(qarg1 + 1, qarg1, (qargend-qarg1) * sizeof(*qarg1));
                    *qarg1 = scriptargs;
                }
                ret = get_errno(execve(qemu_proc_pathname, qarg0, envp));
            } else {
                ret = -TARGET_EBADF;
                goto execve_end;
            }
#endif
        } else {
            ret = get_errno(fexecve((int)path_or_fd, argp, envp));
        }
    } else {
        int fd;
#if defined(__FreeBSD_version) && __FreeBSD_version < 1100000
        char execpath[PATH_MAX], *scriptargs;
#endif

        p = lock_user_string(path_or_fd);
        if (p == NULL) {
            ret = -TARGET_EFAULT;
            goto execve_end;
        }

        /*
         * Check the header and see if it a target elf binary.  If so
         * then execute using qemu user mode emulator.
         */
        fd = open(p, O_RDONLY | O_CLOEXEC);
        if (fd > 0 && is_target_elf_binary(fd) == 1) {
            close(fd);
            /* execve() as a target binary using emulator. */
            *qarg1 = (char *)p;
#ifndef DONT_INHERIT_INTERP_PREFIX
            memmove(qarg1 + 2, qarg1, (qargend-qarg1) * sizeof(*qarg1));
            *qarg1++ = (char *)"-L";
            *qarg1++ = (char *)interp_prefix;
#endif
            ret = get_errno(execve(qemu_proc_pathname, qargp, envp));
#if defined(__FreeBSD_version) && __FreeBSD_version < 1100000
        } else if (is_target_shell_script(fd, execpath,
                    sizeof(execpath), &scriptargs) != 0) {
            close(fd);
            /* execve() as a target script using emulator. */
            *qargp = execpath;
            *qarg1 = (char *)p;
#ifndef DONT_INHERIT_INTERP_PREFIX
            memmove(qargp + 2, qargp, (qargend-qargp) * sizeof(*qargp));
            qargp[0] = (char *)"-L";
            qargp[1] = (char *)interp_prefix;
            qarg1 += 2;
            qargend += 2;
#endif
            if (scriptargs) {
                memmove(qarg1 + 1, qarg1, (qargend-qarg1) * sizeof(*qarg1));
                *qarg1 = scriptargs;
            }
            ret = get_errno(execve(qemu_proc_pathname, qarg0, envp));
#endif
        } else {
            close(fd);
            /* Execve() as a host native binary. */
            ret = get_errno(execve(p, argp, envp));
        }
        unlock_user(p, path_or_fd, 0);
    }

execve_end:
    for (gp = guest_argp, q = argp; *q; gp += sizeof(abi_ulong), q++) {
        if (get_user_ual(addr, gp) || !addr) {
            break;
        }
        unlock_user(*q, addr, 0);
    }

    for (gp = guest_envp, q = envp; *q; gp += sizeof(abi_ulong), q++) {
        if (get_user_ual(addr, gp) || !addr) {
            break;
        }
        unlock_user(*q, addr, 0);
    }
    return ret;
}


