/*
 *  FreeBSD time related system call helpers
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

#include <errno.h>
#include <time.h>
#include <sys/timex.h>
#include <sys/select.h>

#include "qemu.h"
#include "qemu-os.h"

/*
 * FreeBSD time conversion functions
 */
abi_long t2h_freebsd_timeval(struct timeval *tv, abi_ulong target_tv_addr)
{
    struct target_freebsd_timeval *target_tv;

    if (!lock_user_struct(VERIFY_READ, target_tv, target_tv_addr, 0)) {
        return -TARGET_EFAULT;
    }
    __get_user(tv->tv_sec, &target_tv->tv_sec);
    __get_user(tv->tv_usec, &target_tv->tv_usec);
    unlock_user_struct(target_tv, target_tv_addr, 1);

    return 0;
}

abi_long h2t_freebsd_timeval(struct timeval *tv, abi_ulong target_tv_addr)
{
    struct target_freebsd_timeval *target_tv;

    if (!lock_user_struct(VERIFY_WRITE, target_tv, target_tv_addr, 0)) {
        return -TARGET_EFAULT;
    }
    __put_user(tv->tv_sec, &target_tv->tv_sec);
    __put_user(tv->tv_usec, &target_tv->tv_usec);
    unlock_user_struct(target_tv, target_tv_addr, 1);

    return 0;
}

abi_long t2h_freebsd_timespec(struct timespec *ts, abi_ulong target_ts_addr)
{
    struct target_freebsd_timespec *target_ts;

    if (!lock_user_struct(VERIFY_READ, target_ts, target_ts_addr, 0)) {
        return -TARGET_EFAULT;
    }
    __get_user(ts->tv_sec, &target_ts->tv_sec);
    __get_user(ts->tv_nsec, &target_ts->tv_nsec);
    unlock_user_struct(target_ts, target_ts_addr, 1);

    return 0;
}

abi_long h2t_freebsd_timespec(abi_ulong target_ts_addr, struct timespec *ts)
{
    struct target_freebsd_timespec *target_ts;

    if (!lock_user_struct(VERIFY_WRITE, target_ts, target_ts_addr, 0)) {
        return -TARGET_EFAULT;
    }
    __put_user(ts->tv_sec, &target_ts->tv_sec);
    __put_user(ts->tv_nsec, &target_ts->tv_nsec);
    unlock_user_struct(target_ts, target_ts_addr, 1);

    return 0;
}

abi_long t2h_freebsd_timex(struct timex *host_tx, abi_ulong target_tx_addr)
{
    struct target_freebsd_timex *target_tx;

    if (!lock_user_struct(VERIFY_READ, target_tx, target_tx_addr, 0)) {
        return -TARGET_EFAULT;
    }
    __get_user(host_tx->modes, &target_tx->modes);
    __get_user(host_tx->offset, &target_tx->offset);
    __get_user(host_tx->freq, &target_tx->freq);
    __get_user(host_tx->maxerror, &target_tx->maxerror);
    __get_user(host_tx->esterror, &target_tx->esterror);
    __get_user(host_tx->status, &target_tx->status);
    __get_user(host_tx->constant, &target_tx->constant);
    __get_user(host_tx->precision, &target_tx->precision);
    __get_user(host_tx->ppsfreq, &target_tx->ppsfreq);
    __get_user(host_tx->jitter, &target_tx->jitter);
    __get_user(host_tx->shift, &target_tx->shift);
    __get_user(host_tx->stabil, &target_tx->stabil);
    __get_user(host_tx->jitcnt, &target_tx->jitcnt);
    __get_user(host_tx->calcnt, &target_tx->calcnt);
    __get_user(host_tx->errcnt, &target_tx->errcnt);
    __get_user(host_tx->stbcnt, &target_tx->stbcnt);
    unlock_user_struct(target_tx, target_tx_addr, 1);

    return 0;
}

abi_long h2t_freebsd_ntptimeval(abi_ulong target_ntv_addr,
        struct ntptimeval *ntv)
{
    struct target_freebsd_ntptimeval *target_ntv;

    if (!lock_user_struct(VERIFY_WRITE, target_ntv, target_ntv_addr, 0)) {
        return -TARGET_EFAULT;
    }
    __put_user(ntv->time.tv_sec, &target_ntv->time.tv_sec);
    __put_user(ntv->time.tv_nsec, &target_ntv->time.tv_nsec);
    __put_user(ntv->maxerror, &target_ntv->maxerror);
    __put_user(ntv->esterror, &target_ntv->esterror);
    __put_user(ntv->tai, &target_ntv->tai);
    __put_user(ntv->time_state, &target_ntv->time_state);

    return 0;
}

/*
 * select(2) fdset copy functions
 */
abi_ulong copy_from_user_fdset(fd_set *fds, abi_ulong target_fds_addr, int n)
{
    int i, nw, j, k;
    abi_ulong b, *target_fds;

    nw = (n + TARGET_ABI_BITS - 1) / TARGET_ABI_BITS;
    target_fds = lock_user(VERIFY_READ, target_fds_addr,
            sizeof(abi_ulong) * nw, 1);
    if (target_fds == NULL) {
        return -TARGET_EFAULT;
    }
    FD_ZERO(fds);
    k = 0;
    for (i = 0; i < nw; i++) {
        /* grab the abi_ulong */
        __get_user(b, &target_fds[i]);
        for (j = 0; j < TARGET_ABI_BITS; j++) {
            /* check the bit inside the abi_ulong */
            if ((b >> j) & 1) {
                FD_SET(k, fds);
            }
            k++;
        }
    }
    unlock_user(target_fds, target_fds_addr, 0);

    return 0;
}

abi_ulong copy_from_user_fdset_ptr(fd_set *fds, fd_set **fds_ptr,
        abi_ulong target_fds_addr, int n)
{

    if (target_fds_addr) {
        if (copy_from_user_fdset(fds, target_fds_addr, n)) {
            return -TARGET_EFAULT;
        }
        *fds_ptr = fds;
    } else {
        *fds_ptr = NULL;
    }

    return 0;
}

abi_long copy_to_user_fdset(abi_ulong target_fds_addr, const fd_set *fds, int n)
{
    int i, nw, j, k;
    abi_long v;
    abi_ulong *target_fds;

    nw = (n + TARGET_ABI_BITS - 1) / TARGET_ABI_BITS;
    target_fds = lock_user(VERIFY_WRITE, target_fds_addr,
            sizeof(abi_ulong) * nw, 0);
    if (target_fds == NULL) {
        return -TARGET_EFAULT;
    }
    k = 0;
    for (i = 0; i < nw; i++) {
        v = 0;
        for (j = 0; j < TARGET_ABI_BITS; j++) {
            v |= ((FD_ISSET(k, fds) != 0) << j);
            k++;
        }
        __put_user(v, &target_fds[i]);
    }
    unlock_user(target_fds, target_fds_addr, sizeof(abi_ulong) * nw);

    return 0;
}

