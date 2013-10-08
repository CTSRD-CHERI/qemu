#ifndef __NETBSD_OS_THREAD_H_
#define __NETBSD_OS_THREAD_H_

/*
 * XXX To support FreeBSD binaries on NetBSD these syscalls will need to
 * be emulated.
 */
static abi_long do_freebsd_thr_create(CPUArchState *env, abi_ulong target_ctx,
        abi_ulong target_id, int flags)
{

    qemu_log("qemu: Unsupported syscall thr_create()\n");
    return -TARGET_ENOSYS;
}

static abi_long do_freebsd_thr_create(CPUArchState *env, abi_ulong thread_ctx,
        abi_ulong target_id, int flags)
{

    qemu_log("qemu: Unsupported syscall thr_create()\n");
    return -TARGET_ENOSYS;
}

static abi_long do_freebsd_thr_new(CPUArchState *env,
        abi_ulong target_param_addr, int32_t param_size)
{

    qemu_log("qemu: Unsupported syscall thr_new()\n");
    return -TARGET_ENOSYS;
}

static abi_long do_freebsd_thr_self(abi_ulong target_id)
{

    qemu_log("qemu: Unsupported syscall thr_self()\n");
    return -TARGET_ENOSYS;
}

static abi_long do_freebsd_thr_exit(CPUArchState *cpu_env, abi_ulong tid_addr)
{

    qemu_log("qemu: Unsupported syscall thr_exit()\n");
    return -TARGET_ENOSYS;
}

static abi_long do_freebsd_thr_kill(long id, int sig)
{

    qemu_log("qemu: Unsupported syscall thr_kill()\n");
    return -TARGET_ENOSYS;
}

static abi_long do_freebsd_thr_kill2(pid_t pid, long id, int sig)
{

    qemu_log("qemu: Unsupported syscall thr_kill2()\n");
    return -TARGET_ENOSYS;
}

static abi_long do_freebsd_thr_suspend(abi_ulong target_ts)
{

    qemu_log("qemu: Unsupported syscall thr_suspend()\n");
    return -TARGET_ENOSYS;
}

static abi_long do_freebsd_thr_wake(long tid)
{

    qemu_log("qemu: Unsupported syscall thr_wake()\n");
    return -TARGET_ENOSYS;
}

static abi_long do_freebsd_thr_set_name(long tid, abi_ulong target_name)
{

    qemu_log("qemu: Unsupported syscall thr_set_name()\n");
    return -TARGET_ENOSYS;
}

static abi_long do_freebsd_rtprio_thread(int function, lwpid_t lwpid,
        abi_ulong target_addr)
{

    qemu_log("qemu: Unsupported syscall rtprio_thread()\n");
    return -TARGET_ENOSYS;
}

static abi_long do_freebsd_getcontext(void *cpu_env, abi_ulong arg1)
{

    qemu_log("qemu: Unsupported syscall getcontext()\n");
    return -TARGET_ENOSYS;
}

static abi_long do_freebsd_setcontext(void *cpu_env, abi_ulong arg1)
{

    qemu_log("qemu: Unsupported syscall setcontext()\n");
    return -TARGET_ENOSYS;
}

static abi_long do_freebsd_swapcontext(void *cpu_env, abi_ulong arg1,
        abi_ulong arg2)
{

    qemu_log("qemu: Unsupported syscall swapcontext()\n");
    return -TARGET_ENOSYS;
}

static inline abi_long do_freebsd__umtx_lock(abi_ulong target_addr)
{

    qemu_log("qemu: Unsupported syscall _umtx_lock()\n");
    return -TARGET_ENOSYS;
}

static inline abi_long do_freebsd__umtx_unlock(abi_ulong target_addr)
{

    qemu_log("qemu: Unsupported syscall _umtx_unlock()\n");
    return -TARGET_ENOSYS;
}

static inline abi_long do_freebsd__umtx_op(abi_ulong obj, int op, abi_ulong val,
        abi_ulong uaddr, abi_ulong target_ts)
{

    qemu_log("qemu: Unsupported syscall _umtx_op()\n");
    return -TARGET_ENOSYS;
}

#endif /* ! __NETBSD_OS_THREAD_H_ */
