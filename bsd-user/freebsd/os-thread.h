/*
 *  FreeBSD thread and user mutex related system call shims
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
#ifndef __FREEBSD_OS_THREAD_H_
#define __FREEBSD_OS_THREAD_H_

#include <sys/thr.h>
#include <sys/rtprio.h>
#include <sys/umtx.h>

#include "qemu.h"
#include "qemu-os.h"

struct target__umtx_time {
    struct target_freebsd_timespec	_timeout;
    uint32_t	_flags;
    uint32_t	_clockid;
};

static abi_long do_freebsd_thr_create(CPUArchState *env, abi_ulong target_ctx,
        abi_ulong target_id, int flags)
{

    qemu_log("qemu: Unsupported syscall thr_create()\n");
    return -TARGET_ENOSYS;
}

static abi_long do_freebsd_thr_self(abi_ulong target_id)
{
    abi_long ret;
    long tid;

    ret = get_errno(thr_self(&tid));
    if (!is_error(ret)) {
        if (put_user_sal(tid, target_id)) {
            return -TARGET_EFAULT;
        }
    }
    return ret;
}

static abi_long do_freebsd_thr_exit(CPUArchState *cpu_env, abi_ulong tid_addr)
{
    CPUState *cpu = ENV_GET_CPU(cpu_env);
    TaskState *ts;

    /*
     * XXX This probably breaks if a signal arrives.
     * We should disable signals.
     */
    cpu_list_lock();
    /* Remove the CPU from the list. */
    QTAILQ_REMOVE(&cpus, cpu, node);
    cpu_list_unlock();
    if (tid_addr) {
        /* Signal target userland that it can free the stack. */
        if (!put_user_sal(1, tid_addr)) {
            freebsd_umtx_wake(tid_addr, INT_MAX);
        }
    }
    thread_cpu = NULL;
    object_unref(OBJECT(ENV_GET_CPU(cpu_env)));
    ts = cpu->opaque;
    g_free(ts);
    pthread_exit(NULL);
    /* Doesn't return */
    return 0;
}

static abi_long do_freebsd_thr_kill(long id, int sig)
{

    return get_errno(thr_kill(id, target_to_host_signal(sig)));
}

static abi_long do_freebsd_thr_kill2(pid_t pid, long id, int sig)
{

    return get_errno(thr_kill2(pid, id, target_to_host_signal(sig)));
}

static abi_long do_freebsd_thr_suspend(abi_ulong target_ts)
{
    abi_long ret;
    struct timespec ts;

    if (target_ts != 0) {
        if (t2h_freebsd_timespec(&ts, target_ts)) {
            return -TARGET_EFAULT;
        }
        ret = thr_suspend(&ts);
    } else {
        ret = thr_suspend(NULL);
    }
    return ret;
}

static abi_long do_freebsd_thr_wake(long tid)
{

    return get_errno(thr_wake(tid));
}

static abi_long do_freebsd_thr_set_name(long tid, abi_ulong target_name)
{
    abi_long ret;
    void *p;

    p = lock_user_string(target_name);
    if (p == NULL) {
        return -TARGET_EFAULT;
    }
    ret = thr_set_name(tid, p);
    unlock_user(p, target_name, 0);

    return ret;
}

static abi_long do_freebsd_rtprio_thread(int function, lwpid_t lwpid,
        abi_ulong target_addr)
{
    int ret;
    struct rtprio rtp;

    ret = t2h_freebsd_rtprio(&rtp, target_addr);
    if (!is_error(ret)) {
        ret = get_errno(rtprio_thread(function, lwpid, &rtp));
    }
    if (!is_error(ret)) {
        ret = h2t_freebsd_rtprio(target_addr, &rtp);
    }
    return ret;
}

static abi_long do_freebsd_getcontext(void *cpu_env, abi_ulong arg1)
{
    abi_long ret;
    target_ucontext_t *ucp;
    sigset_t sigmask;

    if (arg1 == 0) {
        return -TARGET_EINVAL;
    }
    ret = get_errno(sigprocmask(0, NULL, &sigmask));
    if (!is_error(ret)) {
        ucp = lock_user(VERIFY_WRITE, arg1, sizeof(target_ucontext_t), 0);
        if (ucp == 0) {
            return -TARGET_EFAULT;
        }
        ret = get_mcontext(cpu_env, &ucp->uc_mcontext, TARGET_MC_GET_CLEAR_RET);
        host_to_target_sigset(&ucp->uc_sigmask, &sigmask);
        memset(ucp->__spare__, 0, sizeof(ucp->__spare__));
        unlock_user(ucp, arg1, sizeof(target_ucontext_t));
    }
    return ret;
}

static abi_long do_freebsd_setcontext(void *cpu_env, abi_ulong arg1)
{
    abi_long ret;
    target_ucontext_t *ucp;
    sigset_t sigmask;
    if (arg1 == 0) {
        return -TARGET_EINVAL;
    }
    ucp = lock_user(VERIFY_READ, arg1, sizeof(target_ucontext_t), 1);
    if (ucp == 0) {
        return -TARGET_EFAULT;
    }
    ret = set_mcontext(cpu_env, &ucp->uc_mcontext, 0);
    target_to_host_sigset(&sigmask, &ucp->uc_sigmask);
    unlock_user(ucp, arg1, sizeof(target_ucontext_t));
    if (!is_error(ret)) {
        (void)sigprocmask(SIG_SETMASK, &sigmask, NULL);
    }
    return ret;
}

/* swapcontext(2) */
static abi_long do_freebsd_swapcontext(void *cpu_env, abi_ulong arg1,
        abi_ulong arg2)
{
    abi_long ret;
    target_ucontext_t *ucp;
    sigset_t sigmask;

    if (arg1 == 0 || arg2 == 0) {
        return -TARGET_EINVAL;
    }
    /* Save current context in arg1. */
    ret = get_errno(sigprocmask(0, NULL,  &sigmask));
    if (!is_error(ret)) {
        ucp = lock_user(VERIFY_WRITE, arg1, sizeof(target_ucontext_t), 0);
        if (ucp == 0) {
            return -TARGET_EFAULT;
        }
        ret = get_mcontext(cpu_env, &ucp->uc_mcontext, TARGET_MC_GET_CLEAR_RET);
        host_to_target_sigset(&ucp->uc_sigmask, &sigmask);
        memset(ucp->__spare__, 0, sizeof(ucp->__spare__));
        unlock_user(ucp, arg1, sizeof(target_ucontext_t));
    }
    if (is_error(ret)) {
            return ret;
    }

    /* Restore the context in arg2 to the current context. */
    ucp = lock_user(VERIFY_READ, arg2, sizeof(target_ucontext_t), 1);
    if (ucp == 0) {
        return -TARGET_EFAULT;
    }
    ret = set_mcontext(cpu_env, &ucp->uc_mcontext, 0);
    target_to_host_sigset(&sigmask, &ucp->uc_sigmask);
    unlock_user(ucp, arg2, sizeof(target_ucontext_t));
    if (!is_error(ret)) {
        (void)sigprocmask(SIG_SETMASK, &sigmask, NULL);
    }
    return ret;
}


/* undocumented _umtx_lock() */
static inline abi_long do_freebsd__umtx_lock(abi_ulong target_addr)
{
    abi_long ret;
    long tid;

    ret = get_errno(thr_self(&tid));
    if (is_error(ret)) {
        return ret;
    }
    return freebsd_lock_umtx(target_addr, tid, 0, NULL);
}

/* undocumented _umtx_unlock() */
static inline abi_long do_freebsd__umtx_unlock(abi_ulong target_addr)
{
    abi_long ret;
    long tid;

    ret = get_errno(thr_self(&tid));
    if (is_error(ret)) {
        return ret;
    }
    return freebsd_unlock_umtx(target_addr, tid);
}

/* undocumented _umtx_op(void *obj, int op, u_long val, void *uaddr,
                           void *target_time); */
static inline abi_long do_freebsd__umtx_op(abi_ulong obj, int op, abi_ulong val,
        abi_ulong uaddr, abi_ulong target_time)
{
    abi_long ret;
    struct timespec ts;
    struct _umtx_time ut;
    long tid;

    switch (op) {
    case TARGET_UMTX_OP_LOCK:
        ret = get_errno(thr_self(&tid));
        if (is_error(ret)) {
            return ret;
        }
        if (target_time != 0) {
	    ut._clockid = CLOCK_REALTIME;
	    ut._flags = 0;
            if (t2h_freebsd_timespec(&ut._timeout, target_time +
		offsetof(struct target__umtx_time, _timeout))) {
                return -TARGET_EFAULT;
            }
            ret = freebsd_lock_umtx(obj, tid, sizeof(ut), &ut);
        } else {
            ret = freebsd_lock_umtx(obj, tid, 0, NULL);
        }
        break;

    case TARGET_UMTX_OP_UNLOCK:
        ret = get_errno(thr_self(&tid));
        if (is_error(ret)) {
            return ret;
        }
        ret = freebsd_unlock_umtx(obj, tid);
        break;

    case TARGET_UMTX_OP_WAIT:
        /* args: obj *, val, (void *)sizeof(ut), ut * */
        if (target_time != 0) {
	    ut._clockid = CLOCK_REALTIME;
	    ut._flags = 0;
            if (t2h_freebsd_timespec(&ut._timeout, target_time +
		offsetof(struct target__umtx_time, _timeout))) {
                return -TARGET_EFAULT;
            }
            ret = freebsd_umtx_wait(obj, tswapal(val), sizeof(ut), &ut);
        } else {
            ret = freebsd_umtx_wait(obj, tswapal(val), 0, NULL);
        }
        break;

    case TARGET_UMTX_OP_WAKE:
        /* args: obj *, nr_wakeup */
        ret = freebsd_umtx_wake(obj, val);
        break;

    case TARGET_UMTX_OP_MUTEX_LOCK:
        ret = get_errno(thr_self(&tid));
        if (is_error(ret)) {
            return ret;
        }
        if (target_time) {
            if (t2h_freebsd_timespec(&ts, target_time)) {
                return -TARGET_EFAULT;
            }
            ret = freebsd_lock_umutex(obj, tid, &ts, 0);
        } else {
            ret = freebsd_lock_umutex(obj, tid, NULL, 0);
        }
        break;

    case TARGET_UMTX_OP_MUTEX_UNLOCK:
        ret = get_errno(thr_self(&tid));
        if (is_error(ret)) {
            return ret;
        }
        ret = freebsd_unlock_umutex(obj, tid);
        break;

    case TARGET_UMTX_OP_MUTEX_TRYLOCK:
        ret = get_errno(thr_self(&tid));
        if (is_error(ret)) {
            return ret;
        }
        ret = freebsd_lock_umutex(obj, tid, NULL, TARGET_UMUTEX_TRY);
        break;

    case TARGET_UMTX_OP_MUTEX_WAIT:
        ret = get_errno(thr_self(&tid));
        if (is_error(ret)) {
            return ret;
        }
        if (target_time != 0) {
            if (t2h_freebsd_timespec(&ts, target_time)) {
                return -TARGET_EFAULT;
            }
            ret = freebsd_lock_umutex(obj, tid, &ts, TARGET_UMUTEX_WAIT);
        } else {
            ret = freebsd_lock_umutex(obj, tid, NULL, TARGET_UMUTEX_WAIT);
        }
        break;

    case TARGET_UMTX_OP_MUTEX_WAKE:
        /* Don't need to do access_ok(). */
        ret = freebsd_umtx_mutex_wake(obj, val);
        break;

    case TARGET_UMTX_OP_SET_CEILING:
        ret = 0; /* XXX quietly ignore these things for now */
        break;

    case TARGET_UMTX_OP_CV_WAIT:
        /*
         * Initialization of the struct conv is done by
         * bzero'ing everything in userland.
         */
        if (target_time != 0) {
            if (t2h_freebsd_timespec(&ts, target_time)) {
                return -TARGET_EFAULT;
            }
            ret = freebsd_cv_wait(obj, uaddr, &ts, val);
        } else {
            ret = freebsd_cv_wait(obj, uaddr, NULL, val);
        }
        break;

    case TARGET_UMTX_OP_CV_SIGNAL:
        /*
         * XXX
         * User code may check if c_has_waiters is zero.  Other
         * than that it is assume that user code doesn't do
         * much with the struct conv fields and is pretty
         * much opauque to userland.
         */
        ret = freebsd_cv_signal(obj);
        break;

    case TARGET_UMTX_OP_CV_BROADCAST:
        /*
         * XXX
         * User code may check if c_has_waiters is zero.  Other
         * than that it is assume that user code doesn't do
         * much with the struct conv fields and is pretty
         * much opauque to userland.
         */
        ret = freebsd_cv_broadcast(obj);
        break;

    case TARGET_UMTX_OP_WAIT_UINT:
        if (!access_ok(VERIFY_READ, obj, sizeof(abi_ulong))) {
            return -TARGET_EFAULT;
        }
        /* args: obj *, val, (void *)sizeof(ut), ut * */
        if (target_time != 0) {
	    ut._clockid = CLOCK_REALTIME;
	    ut._flags = 0;
            if (t2h_freebsd_timespec(&ut._timeout, target_time +
		offsetof(struct target__umtx_time, _timeout))) {
                return -TARGET_EFAULT;
            }
            ret = freebsd_umtx_wait_uint(obj, tswap32((uint32_t)val),
			sizeof(ut), &ut);
        } else {
            ret = freebsd_umtx_wait_uint(obj, tswap32((uint32_t)val), 0, NULL);
        }
        break;

    case TARGET_UMTX_OP_WAIT_UINT_PRIVATE:
        if (!access_ok(VERIFY_READ, obj, sizeof(abi_ulong))) {
            return -TARGET_EFAULT;
        }
        /* args: obj *, val, (void *)sizeof(ut), ut * */
        if (target_time != 0) {
	    ut._clockid = CLOCK_REALTIME;
	    ut._flags = 0;
            if (t2h_freebsd_timespec(&ut._timeout, target_time +
		offsetof(struct target__umtx_time, _timeout))) {
                return -TARGET_EFAULT;
            }
            ret = freebsd_umtx_wait_uint_private(obj, tswap32((uint32_t)val),
			sizeof(ut), &ut);
        } else {
            ret = freebsd_umtx_wait_uint_private(obj, tswap32((uint32_t)val),
			0, NULL);
        }
        break;

    case TARGET_UMTX_OP_WAKE_PRIVATE:
        /* Don't need to do access_ok(). */
        ret = freebsd_umtx_wake_private(obj, val);
        break;

    case TARGET_UMTX_OP_RW_RDLOCK:
        if (target_time != 0) {
	    ut._clockid = CLOCK_REALTIME;
	    ut._flags = 0;
            if (t2h_freebsd_timespec(&ut._timeout, target_time +
		offsetof(struct target__umtx_time, _timeout))) {
                return -TARGET_EFAULT;
            }
            ret = freebsd_rw_rdlock(obj, val, sizeof(ut), &ut);
        } else {
            ret = freebsd_rw_rdlock(obj, val, 0, NULL);
        }
        break;

    case TARGET_UMTX_OP_RW_WRLOCK:
        if (target_time != 0) {
	    ut._clockid = CLOCK_REALTIME;
	    ut._flags = 0;
            if (t2h_freebsd_timespec(&ut._timeout, target_time +
		offsetof(struct target__umtx_time, _timeout))) {
                return -TARGET_EFAULT;
            }
            ret = freebsd_rw_wrlock(obj, val, sizeof(ut), &ut);
        } else {
            ret = freebsd_rw_wrlock(obj, val, 0, NULL);
        }
        break;

    case TARGET_UMTX_OP_RW_UNLOCK:
        ret = freebsd_rw_unlock(obj);
        break;

#if defined(__FreeBSD_version) && __FreeBSD_version > 900000
#ifdef UMTX_OP_MUTEX_WAKE2
    case TARGET_UMTX_OP_MUTEX_WAKE2:
        ret = freebsd_umtx_mutex_wake2(obj, val);
        break;
#endif /* UMTX_OP_MUTEX_WAKE2 */

#ifdef UMTX_OP_NWAKE_PRIVATE
    case TARGET_UMTX_OP_NWAKE_PRIVATE:
        ret = freebsd_umtx_nwake_private(obj, val);
        break;
#endif /* UMTX_OP_NWAKE_PRIVATE */

#if __FreeBSD_version > 1100000
    case TARGET_UMTX_OP_SEM2_WAIT:
        /* args: obj *, val, (void *)sizeof(ut), ut * */
        if (target_time != 0) {
	    ut._clockid = CLOCK_REALTIME;
	    ut._flags = 0;
            if (t2h_freebsd_timespec(&ut._timeout, target_time +
		offsetof(struct target__umtx_time, _timeout))) {
                return -TARGET_EFAULT;
            }
            ret = freebsd_umtx_sem2_wait(obj, sizeof(ut), &ut);
        } else {
            ret = freebsd_umtx_sem2_wait(obj, 0, NULL);
        }
	break;

    case TARGET_UMTX_OP_SEM2_WAKE:
        /* Don't need to do access_ok(). */
        ret = freebsd_umtx_sem2_wake(obj, val);
        break;
#endif
    case TARGET_UMTX_OP_SEM_WAIT:
        /* args: obj *, val, (void *)sizeof(ut), ut * */
        if (target_time != 0) {
	    ut._clockid = CLOCK_REALTIME;
	    ut._flags = 0;
            if (t2h_freebsd_timespec(&ut._timeout, target_time +
		offsetof(struct target__umtx_time, _timeout))) {
                return -TARGET_EFAULT;
            }
            ret = freebsd_umtx_sem_wait(obj, sizeof(ut), &ut);
        } else {
            ret = freebsd_umtx_sem_wait(obj, 0, NULL);
        }
        break;

    case TARGET_UMTX_OP_SEM_WAKE:
        /* Don't need to do access_ok(). */
        ret = freebsd_umtx_sem_wake(obj, val);
        break;
#endif
    default:
        return -TARGET_EINVAL;
    }
    return ret;
}

#endif /* !__FREEBSD_OS_THREAD_H_ */
