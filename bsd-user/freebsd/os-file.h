/*
 *  FreeBSD file related system call shims and definitions
 *
 *  Copyright (c) 2014 Stacey D. Son
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

#ifndef __FREEBSD_OS_FILE_H_
#define __FREEBSD_OS_FILE_H_

#define _WANT_FREEBSD11_STAT
#define _WANT_FREEBSD11_STATFS
#define _WANT_FREEBSD11_DIRENT
#include <sys/stat.h>
#include <aio.h>
#include <unistd.h>

#include "qemu-os.h"

/*
 * Asynchronous I/O.
 */

/* aio_read(2) */
static abi_long do_freebsd_aio_read(__unused abi_ulong iocb)
{
    qemu_log("qemu: Unsupported syscall aio_read()\n");
    return -TARGET_ENOSYS;
}

/* aio_write(2) */
static abi_long do_freebsd_aio_write(__unused abi_ulong iocb)
{
    qemu_log("qemu: Unsupported syscall aio_write()\n");
    return -TARGET_ENOSYS;
}

/* aio_suspend(2) */
static abi_long do_freebsd_aio_suspend(__unused abi_ulong iocbs,
	__unused int niocb, __unused abi_ulong timeout)
{
    qemu_log("qemu: Unsupported syscall aio_suspend()\n");
    return -TARGET_ENOSYS;
}

/* aio_cancel(2) */
static abi_long do_freebsd_aio_cancel(__unused int fildes,
	__unused abi_ulong iocb)
{
    qemu_log("qemu: Unsupported syscall aio_cancel()\n");
    return -TARGET_ENOSYS;
}

/* aio_error(2) */
static abi_long do_freebsd_aio_error(__unused abi_ulong iocb)
{
    qemu_log("qemu: Unsupported syscall aio_error()\n");
    return -TARGET_ENOSYS;
}

/* aio_waitcomplete(2) */
static abi_long do_freebsd_aio_waitcomplete(__unused abi_ulong iocbp,
	__unused abi_ulong timeout)
{
    qemu_log("qemu: Unsupported syscall aio_waitcomplete()\n");
    return -TARGET_ENOSYS;
}

/* aio_fsync(2) */
static abi_long do_freebsd_aio_fsync(__unused int op,
	__unused abi_ulong iocb)
{
    qemu_log("qemu: Unsupported syscall aio_fsync()\n");
    return -TARGET_ENOSYS;
}

/* aio_mlock(2) */
static abi_long do_freebsd_aio_mlock(__unused abi_ulong iocb)
{
    qemu_log("qemu: Unsupported syscall aio_mlock()\n");
    return -TARGET_ENOSYS;
}

#if defined(__FreeBSD_version) && __FreeBSD_version >= 1000000
/* pipe2(2) */
static abi_long do_bsd_pipe2(void *cpu_env, abi_ulong pipedes, int flags)
{
    int host_pipe[2];
    int host_ret = pipe2(host_pipe, flags); /* XXXss - flags should be
											   translated from target to host. */

    if (is_error(host_ret)) {
		return get_errno(host_ret);
    }
	/*
	 * XXX pipe2() returns it's second FD by copying it back to
	 * userspace and not in a second register like pipe(2):
	 * set_second_rval(cpu_env, host_pipe[1]);
	 *
	 * Copy the FD's back to userspace:
	 */
	if (put_user_s32(host_pipe[0], pipedes) ||
		put_user_s32(host_pipe[1], pipedes + sizeof(host_pipe[0]))) {
		return -TARGET_EFAULT;
	}
    return 0;
}

/* chflagsat(2) */
static inline abi_long do_bsd_chflagsat(int fd, abi_ulong path,
	abi_ulong flags, int atflags)
{
    abi_long ret;
    void *p;

    LOCK_PATH(p, path);
    ret = get_errno(chflagsat(fd, p, flags, atflags)); /* XXX path(p)? */
    UNLOCK_PATH(p, path);

    return ret;
}

#else /* !  __FreeBSD_version > 1000000 */

static abi_long do_bsd_pipe2(__unused void *cpu_env, __unused abi_long arg1,
	__unused int flags)
{

    qemu_log("qemu: Unsupported syscall pipe2()\n");
    return -TARGET_ENOSYS;
}

static inline abi_long do_bsd_chflagsat(__unused int fd,
	__unused abi_ulong path, __unused abi_ulong flags, int atflags)
{

    qemu_log("qemu: Unsupported syscall chflagsat()\n");
    return -TARGET_ENOSYS;
}

#endif /* !  __FreeBSD_version >= 1000000 */
#endif /* __FREEBSD_OS_FILE_H_ */
