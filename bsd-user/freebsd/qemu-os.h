/*
 *  FreeBSD conversion extern declarations
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

#ifndef _QEMU_OS_H_
#define _QEMU_OS_H_

#include <sys/types.h>
#include <sys/acl.h>
#include <sys/mount.h>
#include <sys/timex.h>
#include <sys/rtprio.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>

#include <time.h>

/* os-time.c */
abi_long t2h_freebsd_timeval(struct timeval *tv, abi_ulong target_tv_addr);
abi_long h2t_freebsd_timeval(struct timeval *tv, abi_ulong target_tv_addr);

abi_long t2h_freebsd_timespec(struct timespec *ts, abi_ulong target_ts_addr);
abi_long h2t_freebsd_timespec(abi_ulong target_ts_addr, struct timespec *ts);

abi_long t2h_freebsd_timex(struct timex *host_tx, abi_ulong target_tx_addr);

abi_long h2t_freebsd_ntptimeval(abi_ulong target_ntv_addr,
        struct ntptimeval *ntv);

abi_ulong copy_from_user_fdset(fd_set *fds, abi_ulong target_fds_addr, int n);
abi_ulong copy_from_user_fdset_ptr(fd_set *fds, fd_set **fds_ptr,
        abi_ulong target_fds_addr, int n);
abi_long copy_to_user_fdset(abi_ulong target_fds_addr, const fd_set *fds,
        int n);

#endif /* !_QEMU_OS_H_ */
