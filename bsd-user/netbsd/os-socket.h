/*
 *  NetBSD socket related system call shims
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
#ifndef __NETBSD_SOCKET_H_
#define __NETBSD_SOCKET_H_

/*
 * XXX To support FreeBSD binaries on NetBSD these syscalls will need
 * to be emulated.
 */

/* sendmsg(2) */
static inline abi_long do_freebsd_sendmsg(int fd, abi_ulong target_msg,
        int flags)
{

    qemu_log("qemu: Unsupported syscall sendmsg()\n");
    return -TARGET_ENOSYS;
}

/* recvmsg(2) */
static inline abi_long do_freebsd_recvmsg(int fd, abi_ulong target_msg,
        int flags)
{

    qemu_log("qemu: Unsupported syscall recvmsg()\n");
    return -TARGET_ENOSYS;
}

/* setsockopt(2) */
static inline abi_long do_bsd_setsockopt(int sockfd, int level, int optname,
        abi_ulong optval_addr, socklen_t optlen)
{

    qemu_log("qemu: Unsupported syscall setsockopt()\n");
    return -TARGET_ENOSYS;
}

/* getsockopt(2) */
static inline abi_long do_bsd_getsockopt(int sockfd, int level, int optname,
        abi_ulong optval_addr, abi_ulong optlen)
{

    qemu_log("qemu: Unsupported syscall getsockopt()\n");
    return -TARGET_ENOSYS;
}

/* setfib(2) */
static inline abi_long do_freebsd_setfib(abi_long fib)
{

    qemu_log("qemu: Unsupported syscall setfib()\n");
    return -TARGET_ENOSYS;
}

/* sctp_peeloff(2) */
static inline abi_long do_freebsd_sctp_peeloff(abi_long s, abi_ulong id)
{

    qemu_log("qemu: Unsupported syscall sctp_peeloff()\n");
    return -TARGET_ENOSYS;
}

/* sctp_generic_sendmsg(2) */
static inline abi_long do_freebsd_sctp_generic_sendmsg(abi_long s,
        abi_ulong target_msg, abi_long msglen, abi_ulong target_to,
        abi_ulong len, abi_ulong target_sinfo, abi_long flags)
{

    qemu_log("qemu: Unsupported syscall sctp_generic_sendmsg()\n");
    return -TARGET_ENOSYS;
}

/* sctp_generic_recvmsg(2) */
static inline abi_long do_freebsd_sctp_generic_recvmsg(abi_long s,
        abi_ulong target_iov, abi_long iovlen, abi_ulong target_from,
        abi_ulong fromlen, abi_ulong target_sinfo, abi_ulong target_msgflags)
{

    qemu_log("qemu: Unsupported syscall sctp_generic_recvmsg()\n");
    return -TARGET_ENOSYS;
}

#endif /* !__NETBSD_SOCKET_H_ */
