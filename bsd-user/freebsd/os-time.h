/*
 *  FreeBSD time related system call shims
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

#ifndef __FREEBSD_OS_TIME_H_
#define __FREEBSD_OS_TIME_H_

#include <sys/types.h>
#include <sys/event.h>
#include <sys/select.h>
#include <sys/timex.h>
#include <signal.h>
#include <time.h>

#include "qemu-os.h"

/* nanosleep(2) */
static inline abi_long do_freebsd_nanosleep(abi_long arg1, abi_long arg2)
{
    abi_long ret;
    struct timespec req, rem;

    ret = t2h_freebsd_timespec(&req, arg1);
    if (!is_error(ret)) {
        ret = get_errno(nanosleep(&req, &rem));
        if (!is_error(ret) && arg2) {
            h2t_freebsd_timespec(arg2, &rem);
        }
    }

    return ret;
}

/* clock_gettime(2) */
static inline abi_long do_freebsd_clock_gettime(abi_long arg1, abi_long arg2)
{
    abi_long ret;
    struct timespec ts;

    ret = get_errno(clock_gettime(arg1, &ts));
    if (!is_error(ret)) {
        if (h2t_freebsd_timespec(arg2, &ts)) {
            return -TARGET_EFAULT;
        }
    }

    return ret;
}

/* clock_settime(2) */
static inline abi_long do_freebsd_clock_settime(abi_long arg1, abi_long arg2)
{
    struct timespec ts;

    if (t2h_freebsd_timespec(&ts, arg2) != 0) {
        return -TARGET_EFAULT;
    }

    return get_errno(clock_settime(arg1, &ts));
}

/* clock_getres(2) */
static inline abi_long do_freebsd_clock_getres(abi_long arg1, abi_long arg2)
{
    abi_long ret;
    struct timespec ts;

    ret = get_errno(clock_getres(arg1, &ts));
    if (!is_error(ret)) {
        if (h2t_freebsd_timespec(arg2, &ts)) {
            return -TARGET_EFAULT;
        }
    }

    return ret;
}

/* gettimeofday(2) */
static inline abi_long do_freebsd_gettimeofday(abi_ulong arg1, abi_ulong arg2)
{
    abi_long ret;
    struct timeval tv;
    struct timezone tz, *target_tz; /* XXX */

    if (arg2 != 0) {
        if (!lock_user_struct(VERIFY_READ, target_tz, arg2, 0)) {
            return -TARGET_EFAULT;
        }
        __get_user(tz.tz_minuteswest, &target_tz->tz_minuteswest);
        __get_user(tz.tz_dsttime, &target_tz->tz_dsttime);
        unlock_user_struct(target_tz, arg2, 1);
    }
    ret = get_errno(gettimeofday(&tv, arg2 != 0 ? &tz : NULL));
    if (!is_error(ret)) {
        if (h2t_freebsd_timeval(&tv, arg1)) {
            return -TARGET_EFAULT;
        }
    }

    return ret;
}

/* settimeofday(2) */
static inline abi_long do_freebsd_settimeofday(abi_long arg1, abi_long arg2)
{
    struct timeval tv;
    struct timezone tz, *target_tz; /* XXX */

    if (arg2 != 0) {
        if (!lock_user_struct(VERIFY_READ, target_tz, arg2, 0)) {
            return -TARGET_EFAULT;
        }
        __get_user(tz.tz_minuteswest, &target_tz->tz_minuteswest);
        __get_user(tz.tz_dsttime, &target_tz->tz_dsttime);
        unlock_user_struct(target_tz, arg2, 1);
    }
    if (t2h_freebsd_timeval(&tv, arg1)) {
        return -TARGET_EFAULT;
    }

    return get_errno(settimeofday(&tv, arg2 != 0 ? &tz : NULL));
}

/* adjtime(2) */
static inline abi_long do_freebsd_adjtime(abi_ulong target_delta_addr,
        abi_ulong target_old_addr)
{
    abi_long ret;
    struct timeval host_delta, host_old;

    ret = t2h_freebsd_timeval(&host_delta, target_delta_addr);
    if (is_error(ret)) {
        return ret;
    }

    if (target_old_addr) {
        ret = get_errno(adjtime(&host_delta, &host_old));
        if (is_error(ret)) {
            return ret;
        }
        ret = h2t_freebsd_timeval(&host_old, target_old_addr);
    } else {
        ret = get_errno(adjtime(&host_delta, NULL));
    }

    return ret;
}

/* ntp_adjtime(2) */
static abi_long do_freebsd_ntp_adjtime(abi_ulong target_tx_addr)
{
    abi_long ret;
    struct timex host_tx;

    ret = t2h_freebsd_timex(&host_tx, target_tx_addr);
    if (ret == 0) {
        ret = get_errno(ntp_adjtime(&host_tx));
    }

    return ret;
}

/* ntp_gettime(2) */
static abi_long do_freebsd_ntp_gettime(abi_ulong target_ntv_addr)
{
    abi_long ret;
    struct ntptimeval host_ntv;

    ret = get_errno(ntp_gettime(&host_ntv));
    if (ret == 0) {
        ret = h2t_freebsd_ntptimeval(target_ntv_addr, &host_ntv);
    }

    return ret;
}


/* utimes(2) */
static inline abi_long do_freebsd_utimes(abi_long arg1, abi_long arg2)
{
    abi_long ret;
    void *p;
    struct timeval *tvp, tv[2];

    if (arg2 != 0) {
        if (t2h_freebsd_timeval(&tv[0], arg2) ||
                t2h_freebsd_timeval(&tv[1], arg2 +
                        sizeof(struct target_freebsd_timeval))) {
            return -TARGET_EFAULT;
        }
        tvp = tv;
    } else {
        tvp = NULL;
    }
    p = lock_user_string(arg1);
    if (p == NULL) {
        return -TARGET_EFAULT;
    }
    ret = get_errno(utimes(p, tvp));
    unlock_user(p, arg1, 0);

    return ret;
}

/* lutimes(2) */
static inline abi_long do_freebsd_lutimes(abi_long arg1, abi_long arg2)
{
    abi_long ret;
    void *p;
    struct timeval *tvp, tv[2];

    if (arg2 != 0) {
        if (t2h_freebsd_timeval(&tv[0], arg2) ||
                t2h_freebsd_timeval(&tv[1], arg2 +
                        sizeof(struct target_freebsd_timeval))) {
            return -TARGET_EFAULT;
        }
        tvp = tv;
    } else {
        tvp = NULL;
    }
    p = lock_user_string(arg1);
    if (p == NULL) {
        return -TARGET_EFAULT;
    }
    ret = get_errno(lutimes(p, tvp));
    unlock_user(p, arg1, 0);

    return ret;
}

/* futimes(2) */
static inline abi_long do_freebsd_futimes(abi_long arg1, abi_long arg2)
{
    struct timeval *tvp, tv[2];

    if (arg2 != 0) {
        if (t2h_freebsd_timeval(&tv[0], arg2) ||
                t2h_freebsd_timeval(&tv[1], arg2 +
                        sizeof(struct target_freebsd_timeval))) {
            return -TARGET_EFAULT;
        }
        tvp = tv;
    } else {
        tvp = NULL;
    }

    return get_errno(futimes(arg1, tvp));
}

/* futimesat(2) */
static inline abi_long do_freebsd_futimesat(abi_long arg1, abi_long arg2,
        abi_long arg3)
{
    abi_long ret;
    void *p;
    struct timeval *tvp, tv[2];

    if (arg3 != 0) {
        if (t2h_freebsd_timeval(&tv[0], arg3) ||
                t2h_freebsd_timeval(&tv[1], arg3 +
                        sizeof(struct target_freebsd_timeval))) {
            return -TARGET_EFAULT;
        }
        tvp = tv;
    } else {
        tvp = NULL;
    }

    p = lock_user_string(arg2);
    if (p == NULL) {
        return -TARGET_EFAULT;
    }
    ret = get_errno(futimesat(arg1, p, tvp));
    unlock_user(p, arg2, 0);

    return ret;
}

/*
 * undocumented ktimer_create(clockid_t clock_id,  struct sigevent *evp,
 * int *timerid) syscall
 */
static inline abi_long do_freebsd_ktimer_create(abi_long arg1, abi_long arg2,
        abi_long arg3)
{

    qemu_log("qemu: Unsupported syscall ktimer_create()\n");
    return -TARGET_ENOSYS;
}

/* undocumented ktimer_delete(int timerid) syscall */
static inline abi_long do_freebsd_ktimer_delete(abi_long arg1)
{

    qemu_log("qemu: Unsupported syscall ktimer_delete()\n");
    return -TARGET_ENOSYS;
}

/*
 * undocumented ktimer_settime(int timerid, int flags,
 * const struct itimerspec *value, struct itimerspec *ovalue) syscall
 */
static inline abi_long do_freebsd_ktimer_settime(abi_long arg1, abi_long arg2,
        abi_long arg3, abi_long arg4)
{

    qemu_log("qemu: Unsupported syscall ktimer_settime()\n");
    return -TARGET_ENOSYS;
}

/*
 * undocumented ktimer_gettime(int timerid, struct itimerspec *value)
 * syscall
 */
static inline abi_long do_freebsd_ktimer_gettime(abi_long arg1, abi_long arg2)
{

    qemu_log("qemu: Unsupported syscall ktimer_gettime()\n");
    return -TARGET_ENOSYS;
}

/*
 * undocumented ktimer_getoverrun(int timerid) syscall
 */
static inline abi_long do_freebsd_ktimer_getoverrun(abi_long arg1)
{

    qemu_log("qemu: Unsupported syscall ktimer_getoverrun()\n");
    return -TARGET_ENOSYS;
}

/* select(2) */
static inline abi_long do_freebsd_select(CPUArchState *env, int n,
	abi_ulong rfd_addr, abi_ulong wfd_addr, abi_ulong efd_addr,
	abi_ulong target_tv_addr)
{
    CPUState *cpu = ENV_GET_CPU(env);
    TaskState *ts = (TaskState *)cpu->opaque;
    fd_set rfds, wfds, efds;
    fd_set *rfds_ptr, *wfds_ptr, *efds_ptr;
    struct timeval tv, tv2;
    struct timespec tspec, *ts_ptr;
    abi_long ret, error;
    sigset_t mask, omask;


    ret = copy_from_user_fdset_ptr(&rfds, &rfds_ptr, rfd_addr, n);
    if (ret != 0) {
        return ret;
    }
    ret = copy_from_user_fdset_ptr(&wfds, &wfds_ptr, wfd_addr, n);
    if (ret != 0) {
        return ret;
    }
    ret = copy_from_user_fdset_ptr(&efds, &efds_ptr, efd_addr, n);
    if (ret != 0) {
        return ret;
    }

    if (target_tv_addr != 0) {
        if (t2h_freebsd_timeval(&tv, target_tv_addr)) {
            return -TARGET_EFAULT;
        }
        tspec.tv_sec = tv.tv_sec;
        tspec.tv_nsec = tv.tv_usec * 1000;
        ts_ptr = &tspec;
    } else {
        ts_ptr = NULL;
    }

    sigfillset(&mask);
    sigprocmask(SIG_BLOCK, &mask, &omask);
	if (ts->signal_pending) {
        sigprocmask(SIG_SETMASK, &omask, NULL);

        /* We have a signal pending so just poll select() and return. */
        tv2.tv_sec = tv2.tv_usec = 0;
        ret = get_errno(select(n, rfds_ptr, wfds_ptr, efds_ptr, &tv2));
	} else {
        ret = get_errno(pselect(n, rfds_ptr, wfds_ptr, efds_ptr, ts_ptr,
                    &omask));
        sigprocmask(SIG_SETMASK, &omask, NULL);
	}

    if (!is_error(ret)) {
        if (rfd_addr != 0) {
            error = copy_to_user_fdset(rfd_addr, &rfds, n);
            if (error != 0) {
                return error;
            }
        }
        if (wfd_addr != 0) {
            error = copy_to_user_fdset(wfd_addr, &wfds, n);
            if (error != 0) {
                return error;
            }
        }
        if (efd_addr != 0) {
            error = copy_to_user_fdset(efd_addr, &efds, n);
            if (error != 0) {
                return error;
            }
        }
        if (target_tv_addr != 0) {
            error = h2t_freebsd_timeval(&tv, target_tv_addr);
            if (is_error(error)) {
                return error;
            }
        }
    }
    return ret;
}

/* pselect(2) */
static inline abi_long do_freebsd_pselect(int n, abi_ulong rfd_addr,
        abi_ulong wfd_addr, abi_ulong efd_addr, abi_ulong ts_addr,
        abi_ulong set_addr)
{
    fd_set rfds, wfds, efds;
    fd_set *rfds_ptr, *wfds_ptr, *efds_ptr;
    sigset_t set, *set_ptr;
    struct timespec ts, *ts_ptr;
    void *p;
    abi_long ret;

    ret = copy_from_user_fdset_ptr(&rfds, &rfds_ptr, rfd_addr, n);
    if (is_error(ret)) {
        return ret;
    }
    ret = copy_from_user_fdset_ptr(&wfds, &wfds_ptr, wfd_addr, n);
    if (is_error(ret)) {
        return ret;
    }
    ret = copy_from_user_fdset_ptr(&efds, &efds_ptr, efd_addr, n);
    if (is_error(ret)) {
        return ret;
    }

    /* Unlike select(), pselect() uses struct timespec instead of timeval */
    if (ts_addr) {
        if (t2h_freebsd_timespec(&ts, ts_addr)) {
            return -TARGET_EFAULT;
        }
        ts_ptr = &ts;
    } else {
        ts_ptr = NULL;
    }

    if (set_addr != 0) {
        p = lock_user(VERIFY_READ, set_addr, sizeof(target_sigset_t), 1);
        if (p == NULL) {
            return -TARGET_EFAULT;
        }
        target_to_host_sigset(&set, p);
        unlock_user(p, set_addr, 0);
        set_ptr = &set;
    } else {
        set_ptr = NULL;
    }

    ret = get_errno(pselect(n, rfds_ptr, wfds_ptr, efds_ptr, ts_ptr, set_ptr));

    if (!is_error(ret)) {
        if (rfd_addr != 0) {
            ret = copy_to_user_fdset(rfd_addr, &rfds, n);
            if (is_error(ret)) {
                return ret;
            }
        }
        if (wfd_addr != 0) {
            ret = copy_to_user_fdset(wfd_addr, &wfds, n);
            if (is_error(ret)) {
                return ret;
            }
        }
        if (efd_addr != 0) {
            ret = copy_to_user_fdset(efd_addr, &efds, n);
            if (is_error(ret)) {
                return ret;
            }
        }
    }
    return ret;
}

#if defined(__FreeBSD_version) && __FreeBSD_version >= 1100000
/* ppoll(2) */
static abi_long do_freebsd_ppoll(abi_long arg1, abi_long arg2, abi_ulong arg3,
		abi_ulong arg4)
{
    abi_long ret;
    nfds_t i, nfds = arg2;
    struct pollfd *pfd;
    struct target_pollfd *target_pfd;
    struct timespec ts, *ts_ptr;
    sigset_t set, *set_ptr;
    void *p;

    target_pfd = lock_user(VERIFY_WRITE, arg1,
            sizeof(struct target_pollfd) * nfds, 1);
	if (!target_pfd)
        return -TARGET_EFAULT;
    pfd = alloca(sizeof(struct pollfd) * nfds);
    for (i = 0; i < nfds; i++) {
        pfd[i].fd = tswap32(target_pfd[i].fd);
        pfd[i].events = tswap16(target_pfd[i].events);
    }

    /* Unlike poll(), ppoll() uses struct timespec. */
    if (arg3) {
        if (t2h_freebsd_timespec(&ts, arg3))
            return -TARGET_EFAULT;
        ts_ptr = &ts;
    } else {
        ts_ptr = NULL;
    }

    if (arg4 != 0) {
        p = lock_user(VERIFY_READ, arg4, sizeof(target_sigset_t), 1);
        if (p == NULL)
            return -TARGET_EFAULT;
        target_to_host_sigset(&set, p);
        unlock_user(p, arg4, 0);
        set_ptr = &set;
    } else {
        set_ptr = NULL;
    }

    ret = get_errno(ppoll(pfd, nfds, ts_ptr, set_ptr));
    if (!is_error(ret)) {
        for (i = 0; i < nfds; i++)
            target_pfd[i].revents = tswap16(pfd[i].revents);
    }
    unlock_user(target_pfd, arg1, sizeof(struct target_pollfd) * nfds);

    return ret;
}
#endif /* __FreeBSD_version >= 1100000 */

/* kqueue(2) */
static inline abi_long do_freebsd_kqueue(void)
{

    return get_errno(kqueue());
}

/* kevent(2) */
static inline abi_long do_freebsd_kevent(abi_long arg1, abi_ulong arg2,
        abi_long arg3, abi_ulong arg4, abi_long arg5, abi_long arg6)
{
    abi_long ret;
    struct kevent *changelist = NULL, *eventlist = NULL;
    struct target_freebsd_kevent *target_changelist, *target_eventlist;
    struct timespec ts;
    int i;

    if (arg3 != 0) {
        target_changelist = lock_user(VERIFY_READ, arg2,
                sizeof(struct target_freebsd_kevent) * arg3, 1);
        if (target_changelist == NULL) {
            return -TARGET_EFAULT;
        }

        changelist = alloca(sizeof(struct kevent) * arg3);
        for (i = 0; i < arg3; i++) {
            __get_user(changelist[i].ident, &target_changelist[i].ident);
            __get_user(changelist[i].filter, &target_changelist[i].filter);
            __get_user(changelist[i].flags, &target_changelist[i].flags);
            __get_user(changelist[i].fflags, &target_changelist[i].fflags);
            __get_user(changelist[i].data, &target_changelist[i].data);
            /* __get_user(changelist[i].udata, &target_changelist[i].udata); */
#if TARGET_ABI_BITS == 32
            changelist[i].udata = (void *)(uintptr_t)target_changelist[i].udata;
            tswap32s((uint32_t *)&changelist[i].udata);
#else
            changelist[i].udata = (void *)(uintptr_t)target_changelist[i].udata;
            tswap64s((uint64_t *)&changelist[i].udata);
#endif
        }
        unlock_user(target_changelist, arg2, 0);
    }

    if (arg5 != 0) {
        eventlist = alloca(sizeof(struct kevent) * arg5);
    }
    if (arg6 != 0) {
        if (t2h_freebsd_timespec(&ts, arg6)) {
            return -TARGET_EFAULT;
        }
    }
    ret = get_errno(kevent(arg1, changelist, arg3, eventlist, arg5,
                arg6 != 0 ? &ts : NULL));
    if (!is_error(ret)) {
        target_eventlist = lock_user(VERIFY_WRITE, arg4,
                sizeof(struct target_freebsd_kevent) * arg5, 0);
        if (target_eventlist == NULL) {
                return -TARGET_EFAULT;
        }
        for (i = 0; i < arg5; i++) {
            __put_user(eventlist[i].ident, &target_eventlist[i].ident);
            __put_user(eventlist[i].filter, &target_eventlist[i].filter);
            __put_user(eventlist[i].flags, &target_eventlist[i].flags);
            __put_user(eventlist[i].fflags, &target_eventlist[i].fflags);
            __put_user(eventlist[i].data, &target_eventlist[i].data);
            /* __put_user(eventlist[i].udata, &target_eventlist[i].udata);*/
#if TARGET_ABI_BITS == 32
            tswap32s((uint32_t *)&eventlist[i].data);
            target_eventlist[i].data = (uintptr_t)eventlist[i].data;
#else
            tswap64s((uint64_t *)&eventlist[i].data);
            target_eventlist[i].data = (uintptr_t)eventlist[i].data;
#endif
        }
        unlock_user(target_eventlist, arg4,
                sizeof(struct target_freebsd_kevent) * arg5);
    }
    return ret;
}

/* sigtimedwait(2) */
static inline abi_long do_freebsd_sigtimedwait(abi_ulong arg1, abi_ulong arg2,
        abi_ulong arg3)
{
    abi_long ret;
    void *p;
    sigset_t set;
    struct timespec uts, *puts;
    siginfo_t uinfo;

    p = lock_user(VERIFY_READ, arg1, sizeof(target_sigset_t), 1);
    if (p == NULL) {
        return -TARGET_EFAULT;
    }
    target_to_host_sigset(&set, p);
    unlock_user(p, arg1, 0);
    if (arg3) {
        puts = &uts;
        t2h_freebsd_timespec(puts, arg3);
    } else {
        puts = NULL;
    }
    ret = get_errno(sigtimedwait(&set, &uinfo, puts));
    if (!is_error(ret) && arg2) {
        p = lock_user(VERIFY_WRITE, arg2, sizeof(target_siginfo_t), 0);
        if (p == NULL) {
            return -TARGET_EFAULT;
        }
        host_to_target_siginfo(p, &uinfo);
        unlock_user(p, arg2, sizeof(target_siginfo_t));
    }
    return ret;
}

/* setitimer(2) */
static inline abi_long do_freebsd_setitimer(int arg1, abi_ulong arg2, abi_ulong arg3)
{
   abi_long ret = 0;
   struct itimerval value, ovalue, *pvalue;

   if (arg2) {
       pvalue = &value;
       if (t2h_freebsd_timeval(&pvalue->it_interval, arg2) ||
           t2h_freebsd_timeval(&pvalue->it_value, arg2 + sizeof(struct target_freebsd_timeval))) {
             return -TARGET_EFAULT;
       } 
   } else {
       pvalue = NULL;
   }
   ret = get_errno(setitimer(arg1, pvalue, &ovalue));
   if (!is_error(ret) && arg3) {
       if (h2t_freebsd_timeval(&ovalue.it_interval, arg3)
          || h2t_freebsd_timeval(&ovalue.it_value, arg3 + sizeof(struct target_freebsd_timeval))) {
             return -TARGET_EFAULT;
       }
   }
   return ret;
}

/* getitimer(2) */
static inline abi_long do_freebsd_getitimer(int arg1, abi_ulong arg2)
{
   abi_long ret = 0;
   struct itimerval value;

   ret = get_errno(getitimer(arg1, &value));
   if (!is_error(ret) && arg2) {
       if (h2t_freebsd_timeval(&value.it_interval, arg2) ||
           h2t_freebsd_timeval(&value.it_value, arg2 + sizeof(struct target_freebsd_timeval))) {
            return -TARGET_EFAULT;
       }
   }
   return ret;
}

#if defined(__FreeBSD_version) && __FreeBSD_version >= 902503
/* clock_getcpuclockid2(id_t, int, clockid_t *)  Not documented. */
static inline abi_long do_freebsd_clock_getcpuclockid2(abi_ulong arg1,
        abi_ulong arg2, abi_ulong arg3, abi_ulong arg4)
{
    abi_long ret = 0;
    id_t id;    /* 64-bit value */
    int which;
    abi_ulong target_clk_id_addr;
    clockid_t clk_id;

#if TARGET_ABI_BITS == 32
    id = (id_t)target_arg64(arg1, arg2);
    which = (int)arg3;
    target_clk_id_addr = arg4;
#else
    id = (id_t)arg1;
    which = (int)arg2;
    target_clk_id_addr = arg3;
#endif

    if (target_clk_id_addr == 0)
        return -TARGET_EINVAL;

    switch(which) {
    case TARGET_CPUCLOCK_WHICH_PID:
        ret = get_errno(clock_getcpuclockid2(id, CPUCLOCK_WHICH_PID, &clk_id));
        break;

    case TARGET_CPUCLOCK_WHICH_TID:
        ret = get_errno(clock_getcpuclockid2(id, CPUCLOCK_WHICH_TID, &clk_id));
        break;

    default:
        ret = -TARGET_EINVAL;
        break;
    }

    if (!ret && put_user_s32(clk_id, target_clk_id_addr))
        ret = -TARGET_EFAULT;

    return ret;
}

#else /* ! __FreeBSD_version >= 902503 */

static inline abi_long do_freebsd_clock_getcpuclockid2(abi_ulong arg1 __unused,
        abi_ulong arg2 __unused, abi_ulong arg3 __unused,
        abi_ulong arg4 __unused)
{

    qemu_log("qemu: Unsupported syscall clock_getcpuclockid2()\n");
    return -TARGET_ENOSYS;
}
#endif /* ! __FreeBSD_version >= 902503 */

#if defined(__FreeBSD_version) && ((__FreeBSD__ == 10 && __FreeBSD_version >= 1003000) || __FreeBSD_version >= 1100056)

static inline abi_long do_freebsd_futimens(abi_ulong arg1,
        abi_ulong arg2)
{
    struct timespec *tvp, tv[2];

    if (arg2 != 0) {
        if (t2h_freebsd_timespec(&tv[0], arg2) || 
                t2h_freebsd_timespec(&tv[1], arg2 +
                        sizeof(struct target_freebsd_timeval))) {
            return -TARGET_EFAULT;
        }
        tvp = tv;
    } else {
        tvp = NULL;
    }

    return get_errno(futimens(arg1, tvp));
}
static inline abi_long do_freebsd_utimensat(abi_ulong arg1,
        abi_ulong arg2, abi_ulong arg3, abi_ulong arg4)
{
    abi_long ret = 0;
    void *p;
    struct timespec *tvp, tv[2];

    if (arg3 != 0) {
        if (t2h_freebsd_timespec(&tv[0], arg3) ||
                t2h_freebsd_timespec(&tv[1], arg3 +
                        sizeof(struct target_freebsd_timeval))) {
            return -TARGET_EFAULT;
        }
        tvp = tv;
    } else {
        tvp = NULL;
    }

    p = lock_user_string(arg2);
    if (p == NULL) {
        return -TARGET_EFAULT;
    }
    ret = get_errno(utimensat(arg1, p, tvp,
	target_to_host_bitmask(arg4, fcntl_flags_tbl)));
    unlock_user(p, arg2, 0);
    return ret;
}
#else /* ! __FreeBSD_version >= 1100000 */
static inline abi_long do_freebsd_futimens(abi_ulong arg1,
        abi_ulong arg2)
{
    qemu_log("qemu: Unsupported syscall futimens\n");
    return -TARGET_ENOSYS;
}
static inline abi_long do_freebsd_utimensat(abi_ulong arg1,
        abi_ulong arg2, abi_ulong arg3, abi_ulong arg4)
{
    qemu_log("qemu: Unsupported syscall utimensat()\n");
    return -TARGET_ENOSYS;
}
#endif /* ! __FreeBSD_version >= 1100000 */
#endif /* __FREEBSD_OS_TIME_H_ */
