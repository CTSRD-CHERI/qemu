#ifndef __NETBSD_OS_TIME_H_
#define __NETBSD_OS_TIME_H_

/*
 * XXX To support FreeBSD binaries on NetBSD these syscalls will need to
 * be emulated.
 */
static inline abi_long do_freebsd_nanosleep(abi_long arg1, abi_long arg2)
{

    qemu_log("qemu: Unsupported syscall %s\n", __func__);
    return -TARGET_ENOSYS;
}

static inline abi_long do_freebsd_clock_gettime(abi_long arg1, abi_long arg2)
{

    qemu_log("qemu: Unsupported syscall %s\n", __func__);
    return -TARGET_ENOSYS;
}

static inline abi_long do_freebsd_clock_settime(abi_long arg1, abi_long arg2)
{

    qemu_log("qemu: Unsupported syscall %s\n", __func__);
    return -TARGET_ENOSYS;
}

static inline abi_long do_freebsd_clock_getres(abi_long arg1, abi_long arg2)
{

    qemu_log("qemu: Unsupported syscall %s\n", __func__);
    return -TARGET_ENOSYS;
}

static inline abi_long do_freebsd_gettimeofday(abi_long arg1, abi_long arg2)
{

    qemu_log("qemu: Unsupported syscall %s\n", __func__);
    return -TARGET_ENOSYS;
}

static inline abi_long do_freebsd_settimeofday(abi_long arg1, abi_long arg2)
{

    qemu_log("qemu: Unsupported syscall %s\n", __func__);
    return -TARGET_ENOSYS;
}

static inline abi_long do_freebsd_adjtime(abi_ulong target_delta_addr,
        abi_ulong target_old_addr)
{

    qemu_log("qemu: Unsupported syscall %s\n", __func__);
    return -TARGET_ENOSYS;
}

static abi_long do_freebsd_ntp_adjtime(abi_ulong target_tx_addr)
{

    qemu_log("qemu: Unsupported syscall %s\n", __func__);
    return -TARGET_ENOSYS;
}

static abi_long do_freebsd_ntp_gettime(abi_ulong target_ntv_addr)
{

    qemu_log("qemu: Unsupported syscall %s\n", __func__);
    return -TARGET_ENOSYS;
}

static inline abi_long do_freebsd_utimes(abi_long arg1, abi_long arg2)
{

    qemu_log("qemu: Unsupported syscall %s\n", __func__);
    return -TARGET_ENOSYS;
}

static inline abi_long do_freebsd_lutimes(abi_long arg1, abi_long arg2)
{

    qemu_log("qemu: Unsupported syscall %s\n", __func__);
    return -TARGET_ENOSYS;
}

static inline abi_long do_freebsd_futimes(abi_long arg1, abi_long arg2)
{

    qemu_log("qemu: Unsupported syscall %s\n", __func__);
    return -TARGET_ENOSYS;
}

static inline abi_long do_freebsd_futimesat(abi_long arg1, abi_long arg2,
        abi_long arg3)
{

    qemu_log("qemu: Unsupported syscall %s\n", __func__);
    return -TARGET_ENOSYS;
}

static inline abi_long do_freebsd_ktimer_create(abi_long arg1, abi_long arg2,
        abi_long arg3)
{

    qemu_log("qemu: Unsupported syscall %s\n", __func__);
    return -TARGET_ENOSYS;
}

static inline abi_long do_freebsd_ktimer_delete(abi_long arg1)
{

    qemu_log("qemu: Unsupported syscall %s\n", __func__);
    return -TARGET_ENOSYS;
}

static inline abi_long do_freebsd_ktimer_settime(abi_long arg1, abi_long arg2,
        abi_long arg3, abi_long arg4)
{

    qemu_log("qemu: Unsupported syscall %s\n", __func__);
    return -TARGET_ENOSYS;
}

static inline abi_long do_freebsd_ktimer_gettime(abi_long arg1, abi_long arg2)
{

    qemu_log("qemu: Unsupported syscall %s\n", __func__);
    return -TARGET_ENOSYS;
}

static inline abi_long do_freebsd_ktimer_getoverrun(abi_long arg1)
{

    qemu_log("qemu: Unsupported syscall %s\n", __func__);
    return -TARGET_ENOSYS;
}

static inline abi_long do_freebsd_select(int n, abi_ulong rfd_addr,
        abi_ulong wfd_addr, abi_ulong efd_addr, abi_ulong target_tv_addr)
{

    qemu_log("qemu: Unsupported syscall %s\n", __func__);
    return -TARGET_ENOSYS;
}


static inline abi_long do_freebsd_pselect(int n, abi_ulong rfd_addr,
        abi_ulong wfd_addr, abi_ulong efd_addr, abi_ulong ts_addr,
        abi_ulong set_addr)
{

    qemu_log("qemu: Unsupported syscall %s\n", __func__);
    return -TARGET_ENOSYS;
}

static inline abi_long do_freebsd_kqueue(void)
{

    qemu_log("qemu: Unsupported syscall %s\n", __func__);
    return -TARGET_ENOSYS;
}

static inline abi_long do_freebsd_kevent(abi_long arg1, abi_ulong arg2,
        abi_long arg3, abi_ulong arg4, abi_long arg5, abi_long arg6)
{

    qemu_log("qemu: Unsupported syscall %s\n", __func__);
    return -TARGET_ENOSYS;
}

static inline abi_long do_freebsd_sigtimedwait(abi_ulong arg1, abi_ulong arg2,
        abi_ulong arg3)
{

    qemu_log("qemu: Unsupported syscall %s\n", __func__);
    return -TARGET_ENOSYS;
}

#endif /* ! __NETBSD_OS_TIME_H_ */
