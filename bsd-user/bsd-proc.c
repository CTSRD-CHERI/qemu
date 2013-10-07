/*
 *  BSD process related system call helpers
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

#include <sys/param.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/wait.h>

#include "qemu.h"
#include "qemu-bsd.h"

/*
 * resource/rusage conversion
 */
int target_to_host_resource(int code)
{

    switch (code) {
    case TARGET_RLIMIT_AS:
        return RLIMIT_AS;

    case TARGET_RLIMIT_CORE:
        return RLIMIT_CORE;

    case TARGET_RLIMIT_CPU:
        return RLIMIT_CPU;

    case TARGET_RLIMIT_DATA:
        return RLIMIT_DATA;

    case TARGET_RLIMIT_FSIZE:
        return RLIMIT_FSIZE;

    case TARGET_RLIMIT_MEMLOCK:
        return RLIMIT_MEMLOCK;

    case TARGET_RLIMIT_NOFILE:
        return RLIMIT_NOFILE;

    case TARGET_RLIMIT_NPROC:
        return RLIMIT_NPROC;

    case TARGET_RLIMIT_RSS:
        return RLIMIT_RSS;

    case TARGET_RLIMIT_SBSIZE:
        return RLIMIT_SBSIZE;

    case TARGET_RLIMIT_STACK:
        return RLIMIT_STACK;

    case TARGET_RLIMIT_SWAP:
        return RLIMIT_SWAP;

    case TARGET_RLIMIT_NPTS:
        return RLIMIT_NPTS;

    default:
        return code;
    }
}

rlim_t target_to_host_rlim(abi_ulong target_rlim)
{
    abi_ulong target_rlim_swap;
    rlim_t result;

    target_rlim_swap = tswapal(target_rlim);
    if (target_rlim_swap == TARGET_RLIM_INFINITY) {
        return RLIM_INFINITY;
    }

    result = target_rlim_swap;
    if (target_rlim_swap != (rlim_t)result) {
        return RLIM_INFINITY;
    }

    return result;
}

abi_ulong host_to_target_rlim(rlim_t rlim)
{
    abi_ulong target_rlim_swap;
    abi_ulong result;

    if (rlim == RLIM_INFINITY || rlim != (abi_long)rlim) {
        target_rlim_swap = TARGET_RLIM_INFINITY;
    } else {
        target_rlim_swap = rlim;
    }
    result = tswapal(target_rlim_swap);

    return result;
}

void h2t_rusage(const struct rusage *rusage,
	struct target_freebsd_rusage *target_rusage)
{
    __put_user(rusage->ru_utime.tv_sec, &target_rusage->ru_utime.tv_sec);
    __put_user(rusage->ru_utime.tv_usec, &target_rusage->ru_utime.tv_usec);

    __put_user(rusage->ru_stime.tv_sec, &target_rusage->ru_stime.tv_sec);
    __put_user(rusage->ru_stime.tv_usec, &target_rusage->ru_stime.tv_usec);

    __put_user(rusage->ru_maxrss, &target_rusage->ru_maxrss);
    __put_user(rusage->ru_idrss, &target_rusage->ru_idrss);
    __put_user(rusage->ru_idrss, &target_rusage->ru_idrss);
    __put_user(rusage->ru_isrss, &target_rusage->ru_isrss);
    __put_user(rusage->ru_minflt, &target_rusage->ru_minflt);
    __put_user(rusage->ru_majflt, &target_rusage->ru_majflt);
    __put_user(rusage->ru_nswap, &target_rusage->ru_nswap);
    __put_user(rusage->ru_inblock, &target_rusage->ru_inblock);
    __put_user(rusage->ru_oublock, &target_rusage->ru_oublock);
    __put_user(rusage->ru_msgsnd, &target_rusage->ru_msgsnd);
    __put_user(rusage->ru_msgrcv, &target_rusage->ru_msgrcv);
    __put_user(rusage->ru_nsignals, &target_rusage->ru_nsignals);
    __put_user(rusage->ru_nvcsw, &target_rusage->ru_nvcsw);
    __put_user(rusage->ru_nivcsw, &target_rusage->ru_nivcsw);
}

abi_long host_to_target_rusage(abi_ulong target_addr,
        const struct rusage *rusage)
{
    struct target_freebsd_rusage *target_rusage;

    if (!lock_user_struct(VERIFY_WRITE, target_rusage, target_addr, 0)) {
        return -TARGET_EFAULT;
    }
    h2t_rusage(rusage, target_rusage);
    unlock_user_struct(target_rusage, target_addr, 1);

    return 0;
}

#if defined(__FreeBSD_version) && __FreeBSD_version >= 1000000
abi_long host_to_target_wrusage(abi_ulong target_addr,
	const struct __wrusage *wrusage)
{
    struct target_freebsd__wrusage *target_wrusage;

    if (!lock_user_struct(VERIFY_WRITE, target_wrusage, target_addr, 0)) {
        return -TARGET_EFAULT;
    }
    h2t_rusage(&wrusage->wru_self, &target_wrusage->wru_self);
    h2t_rusage(&wrusage->wru_children, &target_wrusage->wru_children);
    unlock_user_struct(target_wrusage, target_addr, 1);

    return 0;
}
#endif /* __FreeBSD_version >= 1000000 */

/*
 * wait status conversion.
 *
 * Map host to target signal numbers for the wait family of syscalls.
 * Assume all other status bits are the same.
 */
int host_to_target_waitstatus(int status)
{
    if (WIFSIGNALED(status)) {
        return host_to_target_signal(WTERMSIG(status)) | (status & ~0x7f);
    }
    if (WIFSTOPPED(status)) {
        return (host_to_target_signal(WSTOPSIG(status)) << 8) | (status & 0xff);
    }
    return status;
}

