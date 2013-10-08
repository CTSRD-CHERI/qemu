/*
 *  FreeBSD thr emulation support code
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

#include <sys/param.h>
#include <sys/types.h>
#include <sys/errno.h>
#include <sys/mman.h>
#include <sys/thr.h>
#include <sys/umtx.h>
#include <sys/rtprio.h>

#include <machine/atomic.h>

#include <time.h>

#include "qemu.h"
#include "qemu-os.h"
#include "target_arch_cpu.h"
#include "target_arch_thread.h"

// #define DEBUG_UMTX(...)   fprintf(stderr, __VA_ARGS__)
#define DEBUG_UMTX(...)

#define NEW_STACK_SIZE  0x40000

/* sys/_umtx.h */
struct target_umtx {
    abi_ulong   u_owner;    /* Owner of the mutex. */
};

struct target_umutex {
    uint32_t    m_owner;    /* Owner of the mutex */
    uint32_t    m_flags;    /* Flags of the mutex */
    uint32_t    m_ceiling[2];   /* Priority protect ceiling */
    uint32_t    m_spare[4];
};

struct target_ucond {
    uint32_t    c_has_waiters;  /* Has waiters in kernel */
    uint32_t    c_flags;    /* Flags of the condition variable */
    uint32_t    c_clockid;  /* Clock id */
    uint32_t    c_spare[1];
};

struct target_urwlock {
    uint32_t    rw_state;
    uint32_t    rw_flags;
    uint32_t    rw_blocked_readers;
    uint32_t    rw_blocked_writers;
    uint32_t    rw_spare[4];
};

struct target__usem {
    uint32_t    _has_waiters;
    uint32_t    _count;
    uint32_t    _flags;
};

static pthread_mutex_t new_thread_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t *new_freebsd_thread_lock_ptr = &new_thread_lock;

static pthread_mutex_t umtx_wait_lck = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t umtx_wait_cv = PTHREAD_COND_INITIALIZER;
static abi_ulong umtx_wait_addr;

static void rtp_to_schedparam(const struct rtprio *rtp, int *policy,
        struct sched_param *param)
{

    switch (rtp->type) {
    case RTP_PRIO_REALTIME:
        *policy = SCHED_RR;
        param->sched_priority = RTP_PRIO_MAX - rtp->prio;
        break;

    case RTP_PRIO_FIFO:
        *policy = SCHED_FIFO;
        param->sched_priority = RTP_PRIO_MAX - rtp->prio;
        break;

    default:
        *policy = SCHED_OTHER;
        param->sched_priority = 0;
        break;
    }
}

void *new_freebsd_thread_start(void *arg)
{
    new_freebsd_thread_info_t *info = arg;
    CPUArchState *env;
    CPUState *cpu;
    // TaskState *ts;
    long tid;

    env = info->env;
    cpu = ENV_GET_CPU(env);
    thread_cpu = cpu;

    // ts = (TaskState *)env->opaque;
    (void)thr_self(&tid);
    cpu->host_tid = tid;
    // ts->ts_tid = tid;

    /* copy out the TID info */
    if (info->param.child_tid) {
        put_user(tid, info->param.child_tid, abi_long);
    }
    if (info->param.parent_tid) {
        put_user(info->parent_tid, info->param.parent_tid, abi_long);
    }

    /* Set arch dependent registers to start thread. */
    target_thread_set_upcall(env, info->param.start_func, info->param.arg,
        info->param.stack_base, info->param.stack_size);

    /* Enable signals */
    sigprocmask(SIG_SETMASK, &info->sigmask, NULL);
    /* Signal to the parent that we're ready. */
    pthread_mutex_lock(&info->mutex);
    pthread_cond_broadcast(&info->cond);
    pthread_mutex_unlock(&info->mutex);
    /* Wait until the parent has finished initializing the TLS state. */
    pthread_mutex_lock(new_freebsd_thread_lock_ptr);
    pthread_mutex_unlock(new_freebsd_thread_lock_ptr);

    cpu_loop(env);
    /* never exits */

    return NULL;
}

/*
 * FreeBSD user mutex (_umtx) emulation
 */
static int tcmpset_al(abi_ulong *addr, abi_ulong a, abi_ulong b)
{
    abi_ulong current = tswapal(a);
    abi_ulong new = tswapal(b);

#ifdef TARGET_ABI32
    return atomic_cmpset_acq_32(addr, current, new);
#else
    return atomic_cmpset_acq_64(addr, current, new);
#endif
}

static int tcmpset_32(uint32_t *addr, uint32_t a, uint32_t b)
{
    uint32_t current = tswap32(a);
    uint32_t new = tswap32(b);

    return atomic_cmpset_acq_32(addr, current, new);
}

abi_long freebsd_umtx_wait_uint(abi_ulong obj, uint32_t val,
        struct timespec *timeout)
{

    DEBUG_UMTX("<WAIT> %s: _umtx_op(%p, %d, 0x%x, NULL, %p)\n",
            __func__, g2h(obj), UMTX_OP_WAIT_UINT, val, timeout);
    return get_errno(_umtx_op(g2h(obj), UMTX_OP_WAIT_UINT, val, NULL, timeout));
}

abi_long freebsd_umtx_wait_uint_private(abi_ulong obj, uint32_t val,
        struct timespec *timeout)
{

    DEBUG_UMTX("<WAIT> %s: _umtx_op(%p, %d, 0x%x, NULL, %p)\n",
            __func__, g2h(obj), UMTX_OP_WAIT_UINT_PRIVATE, val, timeout);
    return get_errno(_umtx_op(g2h(obj), UMTX_OP_WAIT_UINT_PRIVATE, val, NULL,
                timeout));
}

abi_long freebsd_umtx_wake_private(abi_ulong obj, uint32_t val)
{

    DEBUG_UMTX("<WAKE> %s: _umtx_op(%p, %d, 0x%x, NULL, NULL)\n",
            __func__, g2h(obj), UMTX_OP_WAKE_PRIVATE, val);
    return get_errno(_umtx_op(g2h(obj), UMTX_OP_WAKE_PRIVATE, val, NULL, NULL));
}

#if defined(__FreeBSD_version) && __FreeBSD_version > 900000
#if defined(UMTX_OP_NWAKE_PRIVATE)
abi_long freebsd_umtx_nwake_private(abi_ulong obj, uint32_t val)
{

    DEBUG_UMTX("<NWAKE> %s: _umtx_op(%p, %d, 0x%x, NULL, NULL)\n",
            __func__, g2h(obj), UMTX_OP_NWAKE_PRIVATE, val);
    return get_errno(_umtx_op(g2h(obj), UMTX_OP_NWAKE_PRIVATE, val, NULL,
                NULL));
}
#endif /* UMTX_OP_NWAKE_PRIVATE */

#if defined(UMTX_OP_MUTEX_WAKE2)
abi_long freebsd_umtx_mutex_wake2(abi_ulong target_addr,
        __unused uint32_t flags)
{
    abi_long ret = 0;

    pthread_mutex_lock(&umtx_wait_lck);
    DEBUG_UMTX("<WAKE2 CV> %s: _umtx_op(%p, %d, 0x%x, NULL, NULL)\n",
            __func__, g2h(target_addr), UMTX_OP_MUTEX_WAKE2, flags);
    umtx_wait_addr = target_addr;
    ret = get_errno(pthread_cond_broadcast(&umtx_wait_cv));
    pthread_mutex_unlock(&umtx_wait_lck);

    return ret;
}
#endif /* UMTX_OP_MUTEX_WAKE2 */

#if defined(__FreeBSD_version) && __FreeBSD_version > 1100000
abi_long freebsd_umtx_sem_wait(abi_ulong obj, struct timespec *timeout)
{

    /* XXX Assumes struct _usem is opauque to the user */
    if (!access_ok(VERIFY_WRITE, obj, sizeof(struct target__usem))) {
        return -TARGET_EFAULT;
    }
    return get_errno(_umtx_op(g2h(obj), UMTX_OP_SEM2_WAIT, 0, NULL, timeout));
}

abi_long freebsd_umtx_sem_wake(abi_ulong obj, uint32_t val)
{

    return get_errno(_umtx_op(g2h(obj), UMTX_OP_SEM2_WAKE, val, NULL, NULL));
}

#else /* ! __FreeBSD_version > 1100000 */

abi_long freebsd_umtx_sem_wait(abi_ulong obj, struct timespec *timeout)
{

    /* XXX Assumes struct _usem is opauque to the user */
    if (!access_ok(VERIFY_WRITE, obj, sizeof(struct target__usem))) {
        return -TARGET_EFAULT;
    }
    return get_errno(_umtx_op(g2h(obj), UMTX_OP_SEM_WAIT, 0, NULL, timeout));
}

abi_long freebsd_umtx_sem_wake(abi_ulong obj, uint32_t val)
{

    return get_errno(_umtx_op(g2h(obj), UMTX_OP_SEM_WAKE, val, NULL, NULL));
}
#endif /* ! __FreeBSD_version > 110000 */
#endif /* ! __FreeBSD_version > 900000 */

abi_long t2h_freebsd_rtprio(struct rtprio *host_rtp, abi_ulong target_addr)
{
    struct target_freebsd_rtprio *target_rtp;

    if (!lock_user_struct(VERIFY_READ, target_rtp, target_addr, 1)) {
        return -TARGET_EFAULT;
    }
    __get_user(host_rtp->type, &target_rtp->type);
    __get_user(host_rtp->prio, &target_rtp->prio);
    unlock_user_struct(target_rtp, target_addr, 0);
    return 0;
}

abi_long h2t_freebsd_rtprio(abi_ulong target_addr, struct rtprio *host_rtp)
{
    struct target_freebsd_rtprio *target_rtp;

    if (!lock_user_struct(VERIFY_WRITE, target_rtp, target_addr, 0)) {
        return -TARGET_EFAULT;
    }
    __put_user(host_rtp->type, &target_rtp->type);
    __put_user(host_rtp->prio, &target_rtp->prio);
    unlock_user_struct(target_rtp, target_addr, 1);
    return 0;
}

abi_long freebsd_lock_umtx(abi_ulong target_addr, abi_long id,
        struct timespec *timeout)
{
    abi_long ret;
    abi_long owner;

    /*
     * XXX Note that memory at umtx_addr can change and so we need to be
     * careful and check for faults.
     */
    for (;;) {
        struct target_umtx *target_umtx;

        if (!lock_user_struct(VERIFY_WRITE, target_umtx, target_addr, 0)) {
            return -TARGET_EFAULT;
        }
        /* Check the simple uncontested case. */
        if (tcmpset_al(&target_umtx->u_owner,
                TARGET_UMTX_UNOWNED, id)) {
            unlock_user_struct(target_umtx, target_addr, 1);
            return 0;
        }
        /* Check to see if the lock is contested but free. */
        __get_user(owner, &target_umtx->u_owner);

        if (TARGET_UMTX_CONTESTED == owner) {
            if (tcmpset_al(&target_umtx->u_owner, TARGET_UMTX_CONTESTED,
                        id | TARGET_UMTX_CONTESTED)) {
                unlock_user_struct(target_umtx, target_addr, 1);
                return 0;
            }
            /* We failed because it changed on us, restart. */
            unlock_user_struct(target_umtx, target_addr, 1);
            continue;
        }

        /* Set the contested bit and sleep. */
        do {
            __get_user(owner, &target_umtx->u_owner);
            if (owner & TARGET_UMTX_CONTESTED) {
                break;
            }
        } while (!tcmpset_al(&target_umtx->u_owner, owner,
                    owner | TARGET_UMTX_CONTESTED));

        __get_user(owner, &target_umtx->u_owner);
        unlock_user_struct(target_umtx, target_addr, 1);

        /* Byte swap, if needed, to match what is stored in user mem. */
        owner = tswapal(owner);
        DEBUG_UMTX("<WAIT> %s: _umtx_op(%p, %d, 0x%llx, NULL, NULL)\n",
                __func__, g2h(target_addr), UMTX_OP_WAIT, (long long)owner);
#ifdef TARGET_ABI32
        ret = get_errno(_umtx_op(g2h(target_addr), UMTX_OP_WAIT_UINT, owner,
            NULL, timeout));
#else
        ret = get_errno(_umtx_op(g2h(target_addr), UMTX_OP_WAIT, owner,
            NULL, timeout));
#endif
        if (is_error(ret)) {
            return ret;
        }
    }
}

abi_long freebsd_unlock_umtx(abi_ulong target_addr, abi_long id)
{
    abi_ulong owner;
    struct target_umtx *target_umtx;

    if (!lock_user_struct(VERIFY_WRITE, target_umtx, target_addr, 0)) {
        return -TARGET_EFAULT;
    }
    __get_user(owner, &target_umtx->u_owner);
    if ((owner & ~TARGET_UMTX_CONTESTED) != id) {
        unlock_user_struct(target_umtx, target_addr, 1);
        return -TARGET_EPERM;
    }
    /* Check the simple uncontested case. */
    if ((owner & ~TARGET_UMTX_CONTESTED) == 0) {
        if (tcmpset_al(&target_umtx->u_owner, owner,
            TARGET_UMTX_UNOWNED)) {
            unlock_user_struct(target_umtx, target_addr, 1);
            return 0;
        }
    }
    /* This is a contested lock. Unlock it. */
    __put_user(TARGET_UMTX_UNOWNED, &target_umtx->u_owner);
    unlock_user_struct(target_umtx, target_addr, 1);

    /* Wake up all those contesting it. */
    DEBUG_UMTX("<WAKE> %s: _umtx_op(%p, %d, 0x%x, NULL, NULL)\n",
            __func__, g2h(target_addr), UMTX_OP_WAKE, 0);
    _umtx_op(g2h(target_addr), UMTX_OP_WAKE, 0, 0, 0);

    return 0;
}

abi_long freebsd_umtx_wait(abi_ulong targ_addr, abi_ulong id,
        struct timespec *ts)
{

    /* We want to check the user memory but not lock it.  We might sleep. */
    if (!access_ok(VERIFY_READ, targ_addr, sizeof(abi_ulong))) {
        return -TARGET_EFAULT;
    }

    DEBUG_UMTX("<WAIT> %s: _umtx_op(%p, %d, 0x%llx, NULL, NULL)\n",
            __func__, g2h(targ_addr), UMTX_OP_WAIT, (long long)id);
#ifdef TARGET_ABI32
    return get_errno(_umtx_op(g2h(targ_addr), UMTX_OP_WAIT_UINT, id, NULL, ts));
#else
    return get_errno(_umtx_op(g2h(targ_addr), UMTX_OP_WAIT, id, NULL, ts));
#endif
}

abi_long freebsd_umtx_wake(abi_ulong target_addr, uint32_t n_wake)
{

    DEBUG_UMTX("<WAKE> %s: _umtx_op(%p, %d, 0x%x, NULL, NULL)\n",
            __func__, g2h(target_addr), UMTX_OP_WAKE, n_wake);
    return get_errno(_umtx_op(g2h(target_addr), UMTX_OP_WAKE, n_wake, NULL, 0));
}

abi_long freebsd_umtx_mutex_wake(abi_ulong obj, abi_long val)
{

    DEBUG_UMTX("<WAKE> %s: _umtx_op(%p, %d, 0x%llx, NULL, NULL)\n",
            __func__, g2h(obj), UMTX_OP_WAKE, (long long)val);
    return get_errno(_umtx_op(g2h(obj), UMTX_OP_MUTEX_WAKE, val, NULL, NULL));
}

abi_long freebsd_lock_umutex(abi_ulong target_addr, uint32_t id,
        struct timespec *ts, int mode)
{
    uint32_t owner, flags;
    int ret = 0;

    for (;;) {
        struct target_umutex *target_umutex;

        pthread_mutex_lock(&umtx_wait_lck);

        if (!lock_user_struct(VERIFY_WRITE, target_umutex, target_addr, 0)) {
            pthread_mutex_unlock(&umtx_wait_lck);
            return -TARGET_EFAULT;
        }

        __get_user(owner, &target_umutex->m_owner);

        if (TARGET_UMUTEX_WAIT == mode) {
            if (TARGET_UMUTEX_UNOWNED == owner ||
                    TARGET_UMUTEX_CONTESTED == owner) {
                unlock_user_struct(target_umutex, target_addr, 1);
                pthread_mutex_unlock(&umtx_wait_lck);
                return 0;
            }
        } else {
            if (tcmpset_32(&target_umutex->m_owner, TARGET_UMUTEX_UNOWNED,
                        id)) {
                /* The acquired succeeded. */
                unlock_user_struct(target_umutex, target_addr, 1);
                pthread_mutex_unlock(&umtx_wait_lck);
                return 0;
            }

            /* If no one owns it but it is contested try to acquire it. */
            if (TARGET_UMUTEX_CONTESTED == owner) {
                if (tcmpset_32(&target_umutex->m_owner, TARGET_UMUTEX_CONTESTED,
                            id | TARGET_UMUTEX_CONTESTED)) {
                    unlock_user_struct(target_umutex, target_addr, 1);
                    pthread_mutex_unlock(&umtx_wait_lck);
                    return 0;
                }
                /* The lock changed so restart. */
                unlock_user_struct(target_umutex, target_addr, 1);
                pthread_mutex_unlock(&umtx_wait_lck);
                continue;
            }
        }

        __get_user(flags, &target_umutex->m_flags);
        if ((flags & TARGET_UMUTEX_ERROR_CHECK) != 0 &&
                (owner & ~TARGET_UMUTEX_CONTESTED) == id) {
            unlock_user_struct(target_umutex, target_addr, 1);
            pthread_mutex_unlock(&umtx_wait_lck);
            return -TARGET_EDEADLK;
        }

        if (TARGET_UMUTEX_TRY == mode) {
            unlock_user_struct(target_umutex, target_addr, 1);
            pthread_mutex_unlock(&umtx_wait_lck);
            return -TARGET_EBUSY;
        }

        /*
         * If we caught a signal, we have retried and now
         * exit immediately.
         */
        if (is_error(ret)) {
            unlock_user_struct(target_umutex, target_addr, 1);
            pthread_mutex_unlock(&umtx_wait_lck);
            return ret;
        }

        /* Set the contested bit and sleep. */
        if (!tcmpset_32(&target_umutex->m_owner, owner,
                    owner | TARGET_UMUTEX_CONTESTED)) {
            unlock_user_struct(target_umutex, target_addr, 1);
            pthread_mutex_unlock(&umtx_wait_lck);
            continue;
        }

        DEBUG_UMTX("<WAIT CV> %s: _umtx_op(%p, %d, 0x%x, NULL, NULL)\n",
                __func__, g2h(target_addr), UMTX_OP_WAIT_UINT,
                tswap32(target_umutex->m_owner));
        unlock_user_struct(target_umutex, target_addr, 1);

again:
        if (ts == NULL) {
            ret = get_errno(pthread_cond_wait(&umtx_wait_cv,
                        &umtx_wait_lck));
        } else {
            ret = get_errno(pthread_cond_timedwait(&umtx_wait_cv,
                        &umtx_wait_lck, ts));
        }
        if (ret != 0) {
            pthread_mutex_unlock(&umtx_wait_lck);
            break;
        }
        if (target_addr != umtx_wait_addr) {
            goto again;
        }
        pthread_mutex_unlock(&umtx_wait_lck);
    }

    return ret;
}

abi_long freebsd_unlock_umutex(abi_ulong target_addr, uint32_t id)
{
    struct target_umutex *target_umutex;
    uint32_t owner;


    if (!lock_user_struct(VERIFY_WRITE, target_umutex, target_addr, 0)) {
        return -TARGET_EFAULT;
    }
    /* Make sure we own this mutex. */
    __get_user(owner, &target_umutex->m_owner);
    if ((owner & ~TARGET_UMUTEX_CONTESTED) != id) {
        unlock_user_struct(target_umutex, target_addr, 1);
        return -TARGET_EPERM;
    }
    if ((owner & TARGET_UMUTEX_CONTESTED) == 0) {
        if (tcmpset_32(&target_umutex->m_owner, owner, TARGET_UMTX_UNOWNED)) {
            unlock_user_struct(target_umutex, target_addr, 1);
            return 0;
        }
    }
    /* This is a contested lock. Unlock it. */
    __put_user(TARGET_UMUTEX_UNOWNED, &target_umutex->m_owner);
    unlock_user_struct(target_umutex, target_addr, 1);

    /* And wake up all those contesting it. */
    DEBUG_UMTX("<WAKE> %s: _umtx_op(%p, %d, 0x%x, NULL, NULL)\n",
            __func__, g2h(target_addr), UMTX_OP_WAKE, 0);
    return get_errno(_umtx_op(g2h(target_addr), UMTX_OP_WAKE, 0, 0, 0));
}

/*
 * _cv_mutex is keeps other threads from doing a signal or broadcast until
 * the thread is actually asleep and ready.  This is a global mutex for all
 * condition vars so I am sure performance may be a problem if there are lots
 * of CVs.
 */
static struct umutex _cv_mutex = { 0, 0, { 0, 0 }, { 0, 0, 0, 0 } };


/*
 * wflags CVWAIT_CHECK_UNPARKING, CVWAIT_ABSTIME, CVWAIT_CLOCKID
 */
abi_long freebsd_cv_wait(abi_ulong target_ucond_addr,
        abi_ulong target_umtx_addr, struct timespec *ts, int wflags)
{
    abi_long ret;
    long tid;

    if (!access_ok(VERIFY_WRITE, target_ucond_addr,
                sizeof(struct target_ucond))) {
        return -TARGET_EFAULT;
    }

    /* Check the clock ID if needed. */
    if ((wflags & TARGET_CVWAIT_CLOCKID) != 0) {
        struct target_ucond *target_ucond;
        uint32_t clockid;

        if (!lock_user_struct(VERIFY_WRITE, target_ucond, target_ucond_addr,
                    0)) {
            return -TARGET_EFAULT;
        }
        __get_user(clockid, &target_ucond->c_clockid);
        unlock_user_struct(target_ucond, target_ucond_addr, 1);
        if (clockid >= CLOCK_THREAD_CPUTIME_ID) {
            /* Only HW clock id will work. */
            return -TARGET_EINVAL;
        }
    }

    thr_self(&tid);

    /* Lock the _cv_mutex so we can safely unlock the user mutex */
    _umtx_op(&_cv_mutex, UMTX_OP_MUTEX_LOCK, 0, NULL, NULL);

    /* unlock the user mutex */
    ret = freebsd_unlock_umutex(target_umtx_addr, tid);
    if (is_error(ret)) {
        _umtx_op(&_cv_mutex, UMTX_OP_MUTEX_UNLOCK, 0, NULL, NULL);
        return ret;
    }

    /* UMTX_OP_CV_WAIT unlocks _cv_mutex */
    DEBUG_UMTX("<WAIT> %s: _umtx_op(%p, %d, 0x%x, %p, NULL)\n",
            __func__, g2h(target_ucond_addr), UMTX_OP_CV_WAIT, wflags,
            &_cv_mutex);
    ret = get_errno(_umtx_op(g2h(target_ucond_addr), UMTX_OP_CV_WAIT, wflags,
                &_cv_mutex, ts));

    return ret;
}

abi_long freebsd_cv_signal(abi_ulong target_ucond_addr)
{
    abi_long ret;

    if (!access_ok(VERIFY_WRITE, target_ucond_addr,
                sizeof(struct target_ucond))) {
        return -TARGET_EFAULT;
    }

    /* Lock the _cv_mutex to prevent a race in do_cv_wait(). */
    _umtx_op(&_cv_mutex, UMTX_OP_MUTEX_LOCK, 0, NULL, NULL);
    DEBUG_UMTX("<WAKE> %s: _umtx_op(%p, %d, 0x%x, NULL, NULL)\n",
            __func__, g2h(target_ucond_addr), UMTX_OP_CV_SIGNAL, 0);
    ret = get_errno(_umtx_op(g2h(target_ucond_addr), UMTX_OP_CV_SIGNAL, 0,
        NULL, NULL));
    _umtx_op(&_cv_mutex, UMTX_OP_MUTEX_UNLOCK, 0, NULL, NULL);

    return ret;
}

abi_long freebsd_cv_broadcast(abi_ulong target_ucond_addr)
{
    int ret;

    if (!access_ok(VERIFY_WRITE, target_ucond_addr,
                sizeof(struct target_ucond))) {
        return -TARGET_EFAULT;
    }

    /* Lock the _cv_mutex to prevent a race in do_cv_wait(). */
    _umtx_op(&_cv_mutex, UMTX_OP_MUTEX_LOCK, 0, NULL, NULL);
    DEBUG_UMTX("<WAKE> %s: _umtx_op(%p, %d, 0x%x, NULL, NULL)\n",
            __func__, g2h(target_ucond_addr), UMTX_OP_CV_BROADCAST, 0);
    ret = get_errno(_umtx_op(g2h(target_ucond_addr), UMTX_OP_CV_BROADCAST, 0,
                NULL, NULL));
    _umtx_op(&_cv_mutex, UMTX_OP_MUTEX_UNLOCK, 0, NULL, NULL);

    return ret;
}

abi_long freebsd_rw_rdlock(abi_ulong target_addr, long fflag,
        struct timespec *ts)
{
    struct target_urwlock *target_urwlock;
    uint32_t flags, wrflags;
    uint32_t state;
    uint32_t blocked_readers;
    abi_long ret;

    if (!lock_user_struct(VERIFY_WRITE, target_urwlock, target_addr, 0)) {
        return -TARGET_EFAULT;
    }

    __get_user(flags, &target_urwlock->rw_flags);
    wrflags = TARGET_URWLOCK_WRITE_OWNER;
    if (!(fflag & TARGET_URWLOCK_PREFER_READER) &&
            !(flags & TARGET_URWLOCK_PREFER_READER)) {
        wrflags |= TARGET_URWLOCK_WRITE_WAITERS;
    }
    for (;;) {
        __get_user(state, &target_urwlock->rw_state);
        /* try to lock it */
        while (!(state & wrflags)) {
            if (TARGET_URWLOCK_READER_COUNT(state) ==
                TARGET_URWLOCK_MAX_READERS) {
                unlock_user_struct(target_urwlock,
                    target_addr, 1);
                return -TARGET_EAGAIN;
            }
            if (tcmpset_32(&target_urwlock->rw_state, state,
                (state + 1))) {
                /* The acquired succeeded. */
                unlock_user_struct(target_urwlock,
                    target_addr, 1);
                return 0;
            }
            __get_user(state, &target_urwlock->rw_state);
        }
        /* set read contention bit */
        if (!tcmpset_32(&target_urwlock->rw_state, state,
            state | TARGET_URWLOCK_READ_WAITERS)) {
            /* The state has changed.  Start over. */
            continue;
        }

        /* contention bit is set, increase read waiter count */
        __get_user(blocked_readers, &target_urwlock->rw_blocked_readers);
        while (!tcmpset_32(&target_urwlock->rw_blocked_readers,
                    blocked_readers, blocked_readers + 1)) {
            __get_user(blocked_readers, &target_urwlock->rw_blocked_readers);
        }

        while (state & wrflags) {
            /* sleep/wait */
            unlock_user_struct(target_urwlock, target_addr, 1);
            DEBUG_UMTX("<WAIT> %s: _umtx_op(%p, %d, 0x%x (0x%x), NULL, NULL)\n",
                    __func__, &target_urwlock->rw_state,
                    UMTX_OP_WAIT_UINT, tswap32(state),
                    target_urwlock->rw_state);
            ret = get_errno(_umtx_op(&target_urwlock->rw_state,
                        UMTX_OP_WAIT_UINT, tswap32(state), NULL, ts));
            if (is_error(ret)) {
                return ret;
            }
            if (!lock_user_struct(VERIFY_WRITE, target_urwlock,
                        target_addr, 0)) {
                return -TARGET_EFAULT;
            }
            __get_user(state, &target_urwlock->rw_state);
        }

        /* decrease read waiter count */
        __get_user(blocked_readers, &target_urwlock->rw_blocked_readers);
        while (!tcmpset_32(&target_urwlock->rw_blocked_readers,
                    blocked_readers, (blocked_readers - 1))) {
            __get_user(blocked_readers, &target_urwlock->rw_blocked_readers);
        }
        if (blocked_readers == 1) {
            /* clear read contention bit */
            __get_user(state, &target_urwlock->rw_state);
            while (!tcmpset_32(&target_urwlock->rw_state, state,
                state & ~TARGET_URWLOCK_READ_WAITERS)) {
                __get_user(state, &target_urwlock->rw_state);
            }
        }
    }
}

abi_long freebsd_rw_wrlock(abi_ulong target_addr, long fflag,
        struct timespec *ts)
{
    struct target_urwlock *target_urwlock;
    uint32_t blocked_readers, blocked_writers;
    uint32_t state;
    abi_long ret;

    if (!lock_user_struct(VERIFY_WRITE, target_urwlock, target_addr, 0)) {
        return -TARGET_EFAULT;
    }
    blocked_readers = 0;
    for (;;) {
        __get_user(state, &target_urwlock->rw_state);
        while (!(state & TARGET_URWLOCK_WRITE_OWNER) &&
            TARGET_URWLOCK_READER_COUNT(state) == 0) {
            if (tcmpset_32(&target_urwlock->rw_state, state,
                        state | TARGET_URWLOCK_WRITE_OWNER)) {
                unlock_user_struct(target_urwlock, target_addr, 1);
                return 0;
            }
            __get_user(state, &target_urwlock->rw_state);
        }

        if (!(state & (TARGET_URWLOCK_WRITE_OWNER |
                        TARGET_URWLOCK_WRITE_WAITERS)) &&
                blocked_readers != 0) {
            DEBUG_UMTX("<WAKE> %s: _umtx_op(%p, %d, 0x%x, NULL, NULL)\n",
                __func__, &target_urwlock->rw_state, UMTX_OP_WAKE,
                tswap32(state));
            ret = get_errno(_umtx_op(&target_urwlock->rw_state,
                UMTX_OP_WAKE, INT_MAX, NULL, NULL));
            return ret;
        }
        /* re-read the state */
        __get_user(state, &target_urwlock->rw_state);

        /* and set TARGET_URWLOCK_WRITE_WAITERS */
        while (((state & TARGET_URWLOCK_WRITE_OWNER) ||
                    TARGET_URWLOCK_READER_COUNT(state) != 0) &&
                (state & TARGET_URWLOCK_WRITE_WAITERS) == 0) {
            if (tcmpset_32(&target_urwlock->rw_state, state,
                        state | TARGET_URWLOCK_WRITE_WAITERS)) {
                break;
            }
            __get_user(state, &target_urwlock->rw_state);
        }

        /* contention bit is set, increase write waiter count */
        __get_user(blocked_writers, &target_urwlock->rw_blocked_writers);
        while (!tcmpset_32(&target_urwlock->rw_blocked_writers,
                    blocked_writers, blocked_writers + 1)) {
            __get_user(blocked_writers, &target_urwlock->rw_blocked_writers);
        }

        /* sleep */
        while ((state & TARGET_URWLOCK_WRITE_OWNER) ||
                (TARGET_URWLOCK_READER_COUNT(state) != 0)) {
            unlock_user_struct(target_urwlock, target_addr, 1);
            DEBUG_UMTX("<WAIT> %s: _umtx_op(%p, %d, 0x%x(0x%x), NULL, NULL)\n",
                    __func__, &target_urwlock->rw_blocked_writers,
                    UMTX_OP_WAIT_UINT, tswap32(state),
                    target_urwlock->rw_state);
            ret = get_errno(_umtx_op(&target_urwlock->rw_state,
                        UMTX_OP_WAIT_UINT, tswap32(state), NULL, ts));
            if (is_error(ret)) {
                return ret;
            }
            if (!lock_user_struct(VERIFY_WRITE, target_urwlock, target_addr,
                        0)) {
                return -TARGET_EFAULT;
            }
            __get_user(state, &target_urwlock->rw_state);
        }

        /* decrease the write waiter count */
        __get_user(blocked_writers, &target_urwlock->rw_blocked_writers);
        while (!tcmpset_32(&target_urwlock->rw_blocked_writers,
                    blocked_writers, (blocked_writers - 1))) {
            __get_user(blocked_writers, &target_urwlock->rw_blocked_writers);
        }
        if (blocked_writers == 1) {
            /* clear write contention bit */
            __get_user(state, &target_urwlock->rw_state);
            while (!tcmpset_32(&target_urwlock->rw_state, state,
                        state & ~TARGET_URWLOCK_WRITE_WAITERS)) {
                __get_user(state, &target_urwlock->rw_state);
            }
            __get_user(blocked_readers, &target_urwlock->rw_blocked_readers);
        } else {
            blocked_readers = 0;
        }
    }
}

abi_long freebsd_rw_unlock(abi_ulong target_addr)
{
    struct target_urwlock *target_urwlock;
    uint32_t flags, state, count = 0;

    if (!lock_user_struct(VERIFY_WRITE, target_urwlock, target_addr, 0)) {
        return -TARGET_EFAULT;
    }

    __get_user(flags, &target_urwlock->rw_flags);
    __get_user(state, &target_urwlock->rw_state);

    if (state & TARGET_URWLOCK_WRITE_OWNER) {
        for (;;) {
            if (!tcmpset_32(&target_urwlock->rw_state, state,
                state & ~TARGET_URWLOCK_WRITE_OWNER)) {
                __get_user(state, &target_urwlock->rw_state);
                if (!(state & TARGET_URWLOCK_WRITE_OWNER)) {
                    unlock_user_struct(target_urwlock,
                        target_addr, 1);
                    return -TARGET_EPERM;
                }
            } else {
                break;
            }
        }
    } else if (TARGET_URWLOCK_READER_COUNT(state) != 0) {
        /* decrement reader count */
        for (;;) {
            if (!tcmpset_32(&target_urwlock->rw_state, state, (state  - 1))) {
                if (TARGET_URWLOCK_READER_COUNT(state) == 0) {
                    unlock_user_struct(target_urwlock,
                        target_addr, 1);
                        return -TARGET_EPERM;
                 }
            } else {
                break;
            }
        }
    } else {
        unlock_user_struct(target_urwlock, target_addr, 1);
        return -TARGET_EPERM;
    }

    if (!(flags & TARGET_URWLOCK_PREFER_READER)) {
        if (state & TARGET_URWLOCK_WRITE_WAITERS) {
            count = 1;
        } else if (state & TARGET_URWLOCK_READ_WAITERS) {
            count = INT_MAX;
        }
    } else {
        if (state & TARGET_URWLOCK_READ_WAITERS) {
            count = INT_MAX;
        } else if (state & TARGET_URWLOCK_WRITE_WAITERS) {
            count = 1;
        }
    }

    unlock_user_struct(target_urwlock, target_addr, 1);
    if (count != 0) {
        DEBUG_UMTX("<WAKE> %s: _umtx_op(%p, %d, 0x%x, NULL, NULL)\n",
                __func__, &target_urwlock->rw_state, UMTX_OP_WAKE, count);
        return get_errno(_umtx_op(&target_urwlock->rw_state, UMTX_OP_WAKE,
                    count, NULL, NULL));
    } else {
        return 0;
    }
}

abi_long do_freebsd_thr_new(CPUArchState *env,
        abi_ulong target_param_addr, int32_t param_size)
{
    new_freebsd_thread_info_t info;
    pthread_attr_t attr;
    TaskState *ts;
    CPUArchState *new_env;
    CPUState *new_cpu;
    struct target_freebsd_thr_param *target_param;
    abi_ulong target_rtp_addr;
    struct target_freebsd_rtprio *target_rtp;
    struct rtprio *rtp_ptr, rtp;
    CPUState *cpu = ENV_GET_CPU(env);
    TaskState *parent_ts = (TaskState *)cpu->opaque;
    sigset_t sigmask;
    struct sched_param sched_param;
    int sched_policy;
    int ret = 0;

    memset(&info, 0, sizeof(info));

    if (!lock_user_struct(VERIFY_READ, target_param, target_param_addr, 1)) {
        return -TARGET_EFAULT;
    }
    info.param.start_func = tswapal(target_param->start_func);
    info.param.arg = tswapal(target_param->arg);
    info.param.stack_base = tswapal(target_param->stack_base);
    info.param.stack_size = tswapal(target_param->stack_size);
    info.param.tls_base = tswapal(target_param->tls_base);
    info.param.tls_size = tswapal(target_param->tls_size);
    info.param.child_tid = tswapal(target_param->child_tid);
    info.param.parent_tid = tswapal(target_param->parent_tid);
    info.param.flags = tswap32(target_param->flags);
    target_rtp_addr = info.param.rtp = tswapal(target_param->rtp);
    unlock_user(target_param, target_param_addr, 0);

    thr_self(&info.parent_tid);

    if (target_rtp_addr) {
        if (!lock_user_struct(VERIFY_READ, target_rtp, target_rtp_addr, 1)) {
            return -TARGET_EFAULT;
        }
        rtp.type = tswap16(target_rtp->type);
        rtp.prio = tswap16(target_rtp->prio);
        unlock_user(target_rtp, target_rtp_addr, 0);
        rtp_ptr = &rtp;
    } else {
        rtp_ptr = NULL;
    }

    /* Create a new CPU instance. */
    ts = g_malloc0(sizeof(TaskState));
    init_task_state(ts);
    new_env = cpu_copy(env);
    //target_cpu_reset(new_env); /* XXX called in cpu_copy()? */

    /* init regs that differ from the parent thread. */
    target_cpu_clone_regs(new_env, info.param.stack_base);
    new_cpu = ENV_GET_CPU(new_env);
    new_cpu->opaque = ts;
    ts->bprm = parent_ts->bprm;
    ts->info = parent_ts->info;

    target_cpu_set_tls(new_env, info.param.tls_base);

    /* Grab a mutex so that thread setup appears atomic. */
    pthread_mutex_lock(new_freebsd_thread_lock_ptr);

    pthread_mutex_init(&info.mutex, NULL);
    pthread_mutex_lock(&info.mutex);
    pthread_cond_init(&info.cond, NULL);
    info.env = new_env;

    /* XXX check return values... */
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, NEW_STACK_SIZE);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (rtp_ptr) {
        rtp_to_schedparam(&rtp, &sched_policy, &sched_param);
        pthread_attr_setschedpolicy(&attr, sched_policy);
        pthread_attr_setschedparam(&attr, &sched_param);
    }

    /*
     * It is not safe to deliver signals until the child has finished
     * initializing, so temporarily block all signals.
     */
    sigfillset(&sigmask);
    sigprocmask(SIG_BLOCK, &sigmask, &info.sigmask);

    ret = pthread_create(&info.thread, &attr, new_freebsd_thread_start, &info);
    /* XXX Free new CPU state if thread creation fails. */

    sigprocmask(SIG_SETMASK, &info.sigmask, NULL);
    pthread_attr_destroy(&attr);
    if (ret == 0) {
        /* Wait for the child to initialize. */
        pthread_cond_wait(&info.cond, &info.mutex);
    } else {
        /* Creation of new thread failed. */
        ret = -host_to_target_errno(errno);
    }

    pthread_mutex_unlock(&info.mutex);
    pthread_cond_destroy(&info.cond);
    pthread_mutex_destroy(&info.mutex);
    pthread_mutex_unlock(new_freebsd_thread_lock_ptr);

    return ret;
}
