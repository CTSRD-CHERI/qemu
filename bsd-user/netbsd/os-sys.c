/*
 *  NetBSD sysctl() and sysarch() system call emulation
 *
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

#include <sys/types.h>
#include <sys/param.h>
#include <sys/sysctl.h>
#include <string.h>

#include "qemu.h"

#include "target_arch_sysarch.h"
#include "target_os_vmparam.h"


/* This must be emulated to support FreeBSD target binaries on NetBSD host. */

abi_long do_freebsd_sysctl(CPUArchState *env, abi_ulong namep, int32_t namelen,
        abi_ulong oldp, abi_ulong oldlenp, abi_ulong newp, abi_ulong newlen)
{

    qemu_log("qemu: Unsupported syscall __sysctl()\n");
    return -TARGET_ENOSYS;
}

/* sysarch() is architecture dependent. */
abi_long do_freebsd_sysarch(void *cpu_env, abi_long arg1, abi_long arg2)
{

    qemu_log("qemu: Unsupported syscall sysarch()\n");
    return -TARGET_ENOSYS;
}
