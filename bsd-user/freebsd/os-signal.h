/*
 *  FreeBSD signal system call shims
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

#ifndef __FREEBSD_OS_SIGNAL_H_
#define __FREEBSD_OS_SIGNAL_H_

#if defined(__FreeBSD_version) && __FreeBSD_version > 900000
#include <sys/procdesc.h>

/* pdkill(2) */
static inline abi_long do_freebsd_pdkill(abi_long arg1, abi_long arg2)
{

    return get_errno(pdkill(arg1, arg2));
}

#else

static inline abi_long do_freebsd_pdkill(abi_long arg1, abi_long arg2)
{

    qemu_log("qemu: Unsupported syscall pdkill()\n");
    return -TARGET_ENOSYS;
}
#endif /* ! __FreeBSD_version > 900000 */

#endif /* ! __FREEBSD_OS_SIGNAL_H_ */
