/*
 *  BSD syscalls
 *
 *  Copyright (c) 2003 - 2008 Fabrice Bellard
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
#include "qemu/osdep.h"
#include "qemu/cutils.h"
#include "qemu/path.h"
#include <sys/syscall.h>
#include <sys/param.h>
#include <sys/sysctl.h>
#include <utime.h>

#include "qemu.h"
#include "qemu-common.h"
#include "user/syscall-trace.h"

#define target_to_host_bitmask(x, tbl) (x)

/* BSD independent syscall shims */
#include "bsd-signal.h"

/* *BSD dependent syscall shims */
#include "os-time.h"
#include "os-signal.h"

/* #define DEBUG */

static abi_ulong target_brk;
static abi_ulong target_original_brk;

/*
 * errno conversion.
 */
abi_long get_errno(abi_long ret)
{
    if (ret == -1)
        /* XXX need to translate host -> target errnos here */
        return -(errno);
    else
        return ret;
}

int is_error(abi_long ret)
{
    return (abi_ulong)ret >= (abi_ulong)(-4096);
}

void target_set_brk(abi_ulong new_brk)
{
    target_original_brk = target_brk = HOST_PAGE_ALIGN(new_brk);
}

/* do_obreak() must return target errnos. */
static abi_long do_obreak(abi_ulong new_brk)
{
    abi_ulong brk_page;
    abi_long mapped_addr;
    int new_alloc_size;

    if (!new_brk)
        return 0;
    if (new_brk < target_original_brk)
        return -TARGET_EINVAL;

    brk_page = HOST_PAGE_ALIGN(target_brk);

    /* If the new brk is less than this, set it and we're done... */
    if (new_brk < brk_page) {
        target_brk = new_brk;
        return 0;
    }

    /* We need to allocate more memory after the brk... */
    new_alloc_size = HOST_PAGE_ALIGN(new_brk - brk_page + 1);
    mapped_addr = get_errno(target_mmap(brk_page, new_alloc_size,
                                        PROT_READ|PROT_WRITE,
                                        MAP_ANON|MAP_FIXED|MAP_PRIVATE, -1, 0));

    if (!is_error(mapped_addr))
        target_brk = new_brk;
    else
        return mapped_addr;

    return 0;
}

/* FIXME
 * lock_iovec()/unlock_iovec() have a return code of 0 for success where
 * other lock functions have a return code of 0 for failure.
 */
static abi_long lock_iovec(int type, struct iovec *vec, abi_ulong target_addr,
                           int count, int copy)
{
    struct target_iovec *target_vec;
    abi_ulong base;
    int i;

    target_vec = lock_user(VERIFY_READ, target_addr, count * sizeof(struct target_iovec), 1);
    if (!target_vec)
        return -TARGET_EFAULT;
    for(i = 0;i < count; i++) {
        base = tswapl(target_vec[i].iov_base);
        vec[i].iov_len = tswapl(target_vec[i].iov_len);
        if (vec[i].iov_len != 0) {
            vec[i].iov_base = lock_user(type, base, vec[i].iov_len, copy);
            /* Don't check lock_user return value. We must call writev even
               if a element has invalid base address. */
        } else {
            /* zero length pointer is ignored */
            vec[i].iov_base = NULL;
        }
    }
    unlock_user (target_vec, target_addr, 0);
    return 0;
}

static abi_long unlock_iovec(struct iovec *vec, abi_ulong target_addr,
                             int count, int copy)
{
    struct target_iovec *target_vec;
    abi_ulong base;
    int i;

    target_vec = lock_user(VERIFY_READ, target_addr, count * sizeof(struct target_iovec), 1);
    if (!target_vec)
        return -TARGET_EFAULT;
    for(i = 0;i < count; i++) {
        if (target_vec[i].iov_base) {
            base = tswapl(target_vec[i].iov_base);
            unlock_user(vec[i].iov_base, base, copy ? vec[i].iov_len : 0);
        }
    }
    unlock_user (target_vec, target_addr, 0);

    return 0;
}

/* do_syscall() should always have a single exit point at the end so
   that actions, such as logging of syscall results, can be performed.
   All errnos that do_syscall() returns must be -TARGET_<errcode>. */
abi_long do_freebsd_syscall(void *cpu_env, int num, abi_long arg1,
                            abi_long arg2, abi_long arg3, abi_long arg4,
                            abi_long arg5, abi_long arg6, abi_long arg7,
                            abi_long arg8)
{
    CPUState *cpu = env_cpu(cpu_env);
    abi_long ret;
    void *p;

#ifdef DEBUG
    gemu_log("freebsd syscall %d\n", num);
#endif
    record_syscall_start(cpu, num, arg1, arg2, arg3, arg4, arg5, arg6, 0, 0);

    if(do_strace)
        print_freebsd_syscall(num, arg1, arg2, arg3, arg4, arg5, arg6);

    switch(num) {
    case TARGET_FREEBSD_NR_exit:
#ifdef CONFIG_GPROF
        _mcleanup();
#endif
        gdb_exit(cpu_env, arg1);
        qemu_plugin_atexit_cb();
        /* XXX: should free thread stack and CPU env */
        _exit(arg1);
        ret = 0; /* avoid warning */
        break;
    case TARGET_FREEBSD_NR_read:
        if (!(p = lock_user(VERIFY_WRITE, arg2, arg3, 0)))
            goto efault;
        ret = get_errno(read(arg1, p, arg3));
        unlock_user(p, arg2, ret);
        break;
    case TARGET_FREEBSD_NR_write:
        if (!(p = lock_user(VERIFY_READ, arg2, arg3, 1)))
            goto efault;
        ret = get_errno(write(arg1, p, arg3));
        unlock_user(p, arg2, 0);
        break;
    case TARGET_FREEBSD_NR_writev:
        {
            int count = arg3;
            struct iovec *vec;

            vec = alloca(count * sizeof(struct iovec));
            if (lock_iovec(VERIFY_READ, vec, arg2, count, 1) < 0)
                goto efault;
            ret = get_errno(writev(arg1, vec, count));
            unlock_iovec(vec, arg2, count, 0);
        }
        break;
    case TARGET_FREEBSD_NR_open:
        if (!(p = lock_user_string(arg1)))
            goto efault;
        ret = get_errno(open(path(p),
                             target_to_host_bitmask(arg2, fcntl_flags_tbl),
                             arg3));
        unlock_user(p, arg1, 0);
        break;
    case TARGET_FREEBSD_NR_mmap:
        ret = get_errno(target_mmap(arg1, arg2, arg3,
                                    target_to_host_bitmask(arg4, mmap_flags_tbl),
                                    arg5,
                                    arg6));
        break;
    case TARGET_FREEBSD_NR_mprotect:
        ret = get_errno(target_mprotect(arg1, arg2, arg3));
        break;
    case TARGET_FREEBSD_NR_break:
        ret = do_obreak(arg1);
        break;

        /*
         * time related system calls.
         */
    case TARGET_FREEBSD_NR_nanosleep: /* nanosleep(2) */
        ret = do_freebsd_nanosleep(arg1, arg2);
        break;

    case TARGET_FREEBSD_NR_clock_gettime: /* clock_gettime(2) */
        ret = do_freebsd_clock_gettime(arg1, arg2);
        break;

    case TARGET_FREEBSD_NR_clock_settime: /* clock_settime(2) */
        ret = do_freebsd_clock_settime(arg1, arg2);
        break;

    case TARGET_FREEBSD_NR_clock_getres: /* clock_getres(2) */
        ret = do_freebsd_clock_getres(arg1, arg2);
        break;

    case TARGET_FREEBSD_NR_gettimeofday: /* gettimeofday(2) */
        ret = do_freebsd_gettimeofday(arg1, arg2);
        break;

    case TARGET_FREEBSD_NR_settimeofday: /* settimeofday(2) */
        ret = do_freebsd_settimeofday(arg1, arg2);
        break;

    case TARGET_FREEBSD_NR_adjtime: /* adjtime(2) */
        ret = do_freebsd_adjtime(arg1, arg2);
        break;

    case TARGET_FREEBSD_NR_ntp_adjtime: /* ntp_adjtime(2) */
        ret = do_freebsd_ntp_adjtime(arg1);
        break;

    case TARGET_FREEBSD_NR_ntp_gettime: /* ntp_gettime(2) */
        ret = do_freebsd_ntp_gettime(arg1);
        break;

    case TARGET_FREEBSD_NR_utimes: /* utimes(2) */
        ret = do_freebsd_utimes(arg1, arg2);
        break;

    case TARGET_FREEBSD_NR_lutimes: /* lutimes(2) */
        ret = do_freebsd_lutimes(arg1, arg2);
        break;

    case TARGET_FREEBSD_NR_futimes: /* futimes(2) */
        ret = do_freebsd_futimes(arg1, arg2);
        break;

    case TARGET_FREEBSD_NR_futimesat: /* futimesat(2) */
        ret = do_freebsd_futimesat(arg1, arg2, arg3);
        break;

    case TARGET_FREEBSD_NR_ktimer_create: /* undocumented */
        ret = do_freebsd_ktimer_create(arg1, arg2, arg3);
        break;

    case TARGET_FREEBSD_NR_ktimer_delete: /* undocumented */
        ret = do_freebsd_ktimer_delete(arg1);
        break;

    case TARGET_FREEBSD_NR_ktimer_settime: /* undocumented */
        ret = do_freebsd_ktimer_settime(arg1, arg2, arg3, arg4);
        break;

    case TARGET_FREEBSD_NR_ktimer_gettime: /* undocumented */
        ret = do_freebsd_ktimer_gettime(arg1, arg2);
        break;

    case TARGET_FREEBSD_NR_ktimer_getoverrun: /* undocumented */
        ret = do_freebsd_ktimer_getoverrun(arg1);
        break;

    case TARGET_FREEBSD_NR_select: /* select(2) */
        ret = do_freebsd_select(arg1, arg2, arg3, arg4, arg5);
        break;

    case TARGET_FREEBSD_NR_pselect: /* pselect(2) */
        ret = do_freebsd_pselect(arg1, arg2, arg3, arg4, arg5, arg6);
        break;

    case TARGET_FREEBSD_NR_kqueue: /* kqueue(2) */
        ret = do_freebsd_kqueue();
        break;

    case TARGET_FREEBSD_NR_kevent: /* kevent(2) */
        ret = do_freebsd_kevent(arg1, arg2, arg3, arg4, arg5, arg6);
        break;

    case TARGET_FREEBSD_NR_setitimer: /* setitimer(2) */
        ret = do_freebsd_setitimer(arg1, arg2, arg3);
        break;

    case TARGET_FREEBSD_NR_getitimer: /* getitimer(2) */
        ret = do_freebsd_getitimer(arg1, arg2);
        break;

        /*
         * signal system calls
         */
    case TARGET_FREEBSD_NR_sigtimedwait: /* sigtimedwait(2) */
        ret = do_freebsd_sigtimedwait(arg1, arg2, arg3);
        break;

    case TARGET_FREEBSD_NR_sigaction: /* sigaction(2) */
        ret = do_bsd_sigaction(arg1, arg2, arg3);
        break;

    case TARGET_FREEBSD_NR_sigprocmask: /* sigprocmask(2) */
        ret = do_bsd_sigprocmask(arg1, arg2, arg3);
        break;

    case TARGET_FREEBSD_NR_sigpending: /* sigpending(2) */
        ret = do_bsd_sigpending(arg1);
        break;

    case TARGET_FREEBSD_NR_sigsuspend: /* sigsuspend(2) */
        ret = do_bsd_sigsuspend(arg1, arg2);
        break;

    case TARGET_FREEBSD_NR_sigreturn: /* sigreturn(2) */
        ret = do_bsd_sigreturn(cpu_env, arg1);
        break;

    case TARGET_FREEBSD_NR_sigwait: /* sigwait(2) */
        ret = do_bsd_sigwait(arg1, arg2, arg3);
        break;

    case TARGET_FREEBSD_NR_sigwaitinfo: /* sigwaitinfo(2) */
        ret = do_bsd_sigwaitinfo(arg1, arg2);
        break;

    case TARGET_FREEBSD_NR_sigqueue: /* sigqueue(2) */
        ret = do_bsd_sigqueue(arg1, arg2, arg3);
        break;

    case TARGET_FREEBSD_NR_sigaltstack: /* sigaltstack(2) */
        ret = do_bsd_sigaltstack(cpu_env, arg1, arg2);
        break;

    case TARGET_FREEBSD_NR_kill: /* kill(2) */
        ret = do_bsd_kill(arg1, arg2);
        break;

    case TARGET_FREEBSD_NR_killpg: /* killpg(2) */
        ret = do_bsd_killpg(arg1, arg2);
        break;

    case TARGET_FREEBSD_NR_pdkill: /* pdkill(2) */
        ret = do_freebsd_pdkill(arg1, arg2);
        break;

        /*
         * sys{ctl, arch, call}
         */
    case TARGET_FREEBSD_NR___sysctl: /* sysctl(3) */
        ret = do_freebsd_sysctl(cpu_env, arg1, arg2, arg3, arg4, arg5, arg6);
        break;

    case TARGET_FREEBSD_NR_sysarch: /* sysarch(2) */
        ret = do_freebsd_sysarch(cpu_env, arg1, arg2);
        break;

    case TARGET_FREEBSD_NR_syscall: /* syscall(2) */
    case TARGET_FREEBSD_NR___syscall: /* __syscall(2) */
        ret = do_freebsd_syscall(cpu_env, arg1 & 0xffff, arg2, arg3, arg4,
                arg5, arg6, arg7, arg8, 0);
        break;

    default:
        ret = get_errno(syscall(num, arg1, arg2, arg3, arg4, arg5, arg6, arg7,
                    arg8));
        break;
    }
 fail:
#ifdef DEBUG
    gemu_log(" = %ld\n", ret);
#endif
    if (do_strace)
        print_freebsd_syscall_ret(num, ret);

    record_syscall_return(cpu, num, ret);
    return ret;
 efault:
    ret = -TARGET_EFAULT;
    goto fail;
}

abi_long do_netbsd_syscall(void *cpu_env, int num, abi_long arg1,
                           abi_long arg2, abi_long arg3, abi_long arg4,
                           abi_long arg5, abi_long arg6)
{
    CPUState *cpu = env_cpu(cpu_env);
    abi_long ret;
    void *p;

#ifdef DEBUG
    gemu_log("netbsd syscall %d\n", num);
#endif

    record_syscall_start(cpu, num, arg1, arg2, arg3, arg4, arg5, arg6, 0, 0);

    if(do_strace)
        print_netbsd_syscall(num, arg1, arg2, arg3, arg4, arg5, arg6);

    switch(num) {
    case TARGET_NETBSD_NR_exit:
#ifdef CONFIG_GPROF
        _mcleanup();
#endif
        gdb_exit(cpu_env, arg1);
        qemu_plugin_atexit_cb();
        /* XXX: should free thread stack and CPU env */
        _exit(arg1);
        ret = 0; /* avoid warning */
        break;
    case TARGET_NETBSD_NR_read:
        if (!(p = lock_user(VERIFY_WRITE, arg2, arg3, 0)))
            goto efault;
        ret = get_errno(read(arg1, p, arg3));
        unlock_user(p, arg2, ret);
        break;
    case TARGET_NETBSD_NR_write:
        if (!(p = lock_user(VERIFY_READ, arg2, arg3, 1)))
            goto efault;
        ret = get_errno(write(arg1, p, arg3));
        unlock_user(p, arg2, 0);
        break;
    case TARGET_NETBSD_NR_open:
        if (!(p = lock_user_string(arg1)))
            goto efault;
        ret = get_errno(open(path(p),
                             target_to_host_bitmask(arg2, fcntl_flags_tbl),
                             arg3));
        unlock_user(p, arg1, 0);
        break;
    case TARGET_NETBSD_NR_mmap:
        ret = get_errno(target_mmap(arg1, arg2, arg3,
                                    target_to_host_bitmask(arg4, mmap_flags_tbl),
                                    arg5,
                                    arg6));
        break;
    case TARGET_NETBSD_NR_mprotect:
        ret = get_errno(target_mprotect(arg1, arg2, arg3));
        break;

    case TARGET_NETBSD_NR_syscall:
    case TARGET_NETBSD_NR___syscall:
        ret = do_netbsd_syscall(cpu_env,arg1 & 0xffff,arg2,arg3,arg4,arg5,arg6,0);
        break;
    default:
        ret = syscall(num, arg1, arg2, arg3, arg4, arg5, arg6);
        break;
    }
 fail:
#ifdef DEBUG
    gemu_log(" = %ld\n", ret);
#endif
    if (do_strace)
        print_netbsd_syscall_ret(num, ret);

    record_syscall_return(cpu, num, ret);
    return ret;
 efault:
    ret = -TARGET_EFAULT;
    goto fail;
}

abi_long do_openbsd_syscall(void *cpu_env, int num, abi_long arg1,
                            abi_long arg2, abi_long arg3, abi_long arg4,
                            abi_long arg5, abi_long arg6)
{
    CPUState *cpu = env_cpu(cpu_env);
    abi_long ret;
    void *p;

#ifdef DEBUG
    gemu_log("openbsd syscall %d\n", num);
#endif

    record_syscall_start(cpu, num, arg1, arg2, arg3, arg4, arg5, arg6, 0, 0);

    if(do_strace)
        print_openbsd_syscall(num, arg1, arg2, arg3, arg4, arg5, arg6);

    switch(num) {
    case TARGET_OPENBSD_NR_exit:
#ifdef CONFIG_GPROF
        _mcleanup();
#endif
        gdb_exit(cpu_env, arg1);
        qemu_plugin_atexit_cb();
        /* XXX: should free thread stack and CPU env */
        _exit(arg1);
        ret = 0; /* avoid warning */
        break;
    case TARGET_OPENBSD_NR_read:
        if (!(p = lock_user(VERIFY_WRITE, arg2, arg3, 0)))
            goto efault;
        ret = get_errno(read(arg1, p, arg3));
        unlock_user(p, arg2, ret);
        break;
    case TARGET_OPENBSD_NR_write:
        if (!(p = lock_user(VERIFY_READ, arg2, arg3, 1)))
            goto efault;
        ret = get_errno(write(arg1, p, arg3));
        unlock_user(p, arg2, 0);
        break;
    case TARGET_OPENBSD_NR_open:
        if (!(p = lock_user_string(arg1)))
            goto efault;
        ret = get_errno(open(path(p),
                             target_to_host_bitmask(arg2, fcntl_flags_tbl),
                             arg3));
        unlock_user(p, arg1, 0);
        break;
    case TARGET_OPENBSD_NR_mmap:
        ret = get_errno(target_mmap(arg1, arg2, arg3,
                                    target_to_host_bitmask(arg4, mmap_flags_tbl),
                                    arg5,
                                    arg6));
        break;
    case TARGET_OPENBSD_NR_mprotect:
        ret = get_errno(target_mprotect(arg1, arg2, arg3));
        break;

    case TARGET_OPENBSD_NR_syscall:
    case TARGET_OPENBSD_NR___syscall:
        ret = do_openbsd_syscall(cpu_env,arg1 & 0xffff,arg2,arg3,arg4,arg5,arg6,0);
        break;
    default:
        ret = syscall(num, arg1, arg2, arg3, arg4, arg5, arg6);
        break;
    }
 fail:
#ifdef DEBUG
    gemu_log(" = %ld\n", ret);
#endif
    if (do_strace)
        print_openbsd_syscall_ret(num, ret);

    record_syscall_return(cpu, num, ret);
    return ret;
 efault:
    ret = -TARGET_EFAULT;
    goto fail;
}

void syscall_init(void)
{
}
