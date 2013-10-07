#ifndef __NETBSD_PROC_H_
#define __NETBSD_PROC_H_

/*
 * XXX To support FreeBSD binaries on NetBSD these syscalls will need
 * to be emulated.
 */

/* execve(2) */
static inline abi_long do_freebsd_execve(abi_ulong path_or_fd, abi_ulong argp,
        abi_ulong envp)
{

    qemu_log("qemu: Unsupported syscall execve()\n");
    return -TARGET_ENOSYS;
}

/* fexecve(2) */
static inline abi_long do_freebsd_fexecve(abi_ulong path_or_fd, abi_ulong argp,
        abi_ulong envp)
{

    qemu_log("qemu: Unsupported syscall fexecve()\n");
    return -TARGET_ENOSYS;
}

/* wait4(2) */
static inline abi_long do_freebsd_wait4(abi_long arg1, abi_ulong target_status,
        abi_long arg3, abi_ulong target_rusage)
{

    qemu_log("qemu: Unsupported syscall wait4()\n");
    return -TARGET_ENOSYS;
}

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

/* undocumented __setugid */
static inline abi_long do_freebsd___setugid(abi_long arg1)
{

    qemu_log("qemu: Unsupported syscall __setugid()\n");
    return -TARGET_ENOSYS;
}

/* fork(2) */
static inline abi_long do_freebsd_fork(void *cpu_env)
{

    qemu_log("qemu: Unsupported syscall fork()\n");
    return -TARGET_ENOSYS;
}

/* vfork(2) */
static inline abi_long do_freebsd_vfork(void *cpu_env)
{

    qemu_log("qemu: Unsupported syscall vfork()\n");
    return -TARGET_ENOSYS;
}

/* rfork(2) */
static inline abi_long do_freebsd_rfork(void *cpu_env, abi_long flags)
{

    qemu_log("qemu: Unsupported syscall rfork()\n");
    return -TARGET_ENOSYS;
}

/* pdfork(2) */
static inline abi_long do_freebsd_pdfork(void *cpu_env, abi_ulong arg1,
        abi_long arg2)
{

    qemu_log("qemu: Unsupported syscall pdfork()\n");
    return -TARGET_ENOSYS
}

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

#endif /* ! __NETBSD_PROC_H_ */
