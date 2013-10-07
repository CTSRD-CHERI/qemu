/*
 *  OpenBSD stat related system call shims and definitions
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

#ifndef __OPENBSD_STAT_H_
#define __OPENBSD_STAT_H_

/*
 * XXX To support FreeBSD binaries on OpenBSD these syscalls will need
 * to be emulated.
 */

/* stat(2) */
static inline abi_long do_freebsd_stat(abi_long arg1, abi_long arg2)
{

    qemu_log("qemu: Unsupported syscall stat()\n");
    return -TARGET_ENOSYS;
}

/* lstat(2) */
static inline abi_long do_freebsd_lstat(abi_long arg1, abi_long arg2)
{

    qemu_log("qemu: Unsupported syscall lstat()\n");
    return -TARGET_ENOSYS;
}

/* fstat(2) */
static inline abi_long do_freebsd_fstat(abi_long arg1, abi_long arg2)
{

    qemu_log("qemu: Unsupported syscall fstat()\n");
    return -TARGET_ENOSYS;
}

/* fstatat(2) */
static inline abi_long do_freebsd_fstatat(abi_long arg1, abi_long arg2,
        abi_long arg3, abi_long arg4)
{

    qemu_log("qemu: Unsupported syscall fstatat()\n");
    return -TARGET_ENOSYS;
}

/* undocummented nstat(char *path, struct nstat *ub) syscall */
static abi_long do_freebsd_nstat(abi_long arg1, abi_long arg2)
{

    qemu_log("qemu: Unsupported syscall nstat()\n");
    return -TARGET_ENOSYS;
}

/* undocummented nfstat(int fd, struct nstat *sb) syscall */
static abi_long do_freebsd_nfstat(abi_long arg1, abi_long arg2)
{

    qemu_log("qemu: Unsupported syscall nfstat()\n");
    return -TARGET_ENOSYS;
}

/* undocummented nlstat(char *path, struct nstat *ub) syscall */
static abi_long do_freebsd_nlstat(abi_long arg1, abi_long arg2)
{

    qemu_log("qemu: Unsupported syscall nlstat()\n");
    return -TARGET_ENOSYS;
}

/* getfh(2) */
static abi_long do_freebsd_getfh(abi_long arg1, abi_long arg2)
{

    qemu_log("qemu: Unsupported syscall getfh()\n");
    return -TARGET_ENOSYS;
}

/* lgetfh(2) */
static inline abi_long do_freebsd_lgetfh(abi_long arg1, abi_long arg2)
{

    qemu_log("qemu: Unsupported syscall lgetfh()\n");
    return -TARGET_ENOSYS;
}

/* fhopen(2) */
static inline abi_long do_freebsd_fhopen(abi_long arg1, abi_long arg2)
{

    qemu_log("qemu: Unsupported syscall fhopen()\n");
    return -TARGET_ENOSYS;
}

/* fhstat(2) */
static inline abi_long do_freebsd_fhstat(abi_long arg1, abi_long arg2)
{

    qemu_log("qemu: Unsupported syscall fhstat()\n");
    return -TARGET_ENOSYS;
}

/* fhstatfs(2) */
static inline abi_long do_freebsd_fhstatfs(abi_ulong target_fhp_addr,
        abi_ulong target_stfs_addr)
{

    qemu_log("qemu: Unsupported syscall fhstatfs()\n");
    return -TARGET_ENOSYS;
}

/* statfs(2) */
static inline abi_long do_freebsd_statfs(abi_long arg1, abi_long arg2)
{

    qemu_log("qemu: Unsupported syscall statfs()\n");
    return -TARGET_ENOSYS;
}

/* fstatfs(2) */
static inline abi_long do_freebsd_fstatfs(abi_long fd, abi_ulong target_addr)
{

    qemu_log("qemu: Unsupported syscall fstatfs()\n");
    return -TARGET_ENOSYS;
}

/* getfsstat(2) */
static inline abi_long do_freebsd_getfsstat(abi_ulong target_addr,
        abi_long bufsize, abi_long flags)
{

    qemu_log("qemu: Unsupported syscall getfsstat()\n");
    return -TARGET_ENOSYS;
}

/* getdents(2) */
static inline abi_long do_freebsd_getdents(abi_long arg1, abi_ulong arg2,
        abi_long nbytes)
{

    qemu_log("qemu: Unsupported syscall getdents()\n");
    return -TARGET_ENOSYS;
}

/* getdirecentries(2) */
static inline abi_long do_freebsd_getdirentries(abi_long arg1, abi_ulong arg2,
        abi_long nbytes, abi_ulong arg4)
{

    qemu_log("qemu: Unsupported syscall getdirecentries()\n");
    return -TARGET_ENOSYS;
}

/* fcntl(2) */
static inline abi_long do_freebsd_fcntl(abi_long arg1, abi_long arg2,
        abi_ulong arg3)
{

    qemu_log("qemu: Unsupported syscall fcntl()\n");
    return -TARGET_ENOSYS;
}

#endif /* ! __OPENBSD_STAT_H_ */
