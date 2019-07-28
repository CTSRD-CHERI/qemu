/*
 *  socket related system call shims
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
#ifndef __BSD_SOCKET_H_
#define __BSD_SOCKET_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>

#include "qemu-bsd.h"

/* bind(2) */
static inline abi_long do_bsd_bind(int sockfd, abi_ulong target_addr,
        socklen_t addrlen)
{
    abi_long ret;
    void *addr;

    if ((int)addrlen < 0) {
        return -TARGET_EINVAL;
    }

    addr = alloca(addrlen + 1);
    ret = target_to_host_sockaddr(addr, target_addr, addrlen);
    if (is_error(ret)) {
        return ret;
    }

    return get_errno(bind(sockfd, addr, addrlen));
}

/* connect(2) */
static inline abi_long do_bsd_connect(int sockfd, abi_ulong target_addr,
        socklen_t addrlen)
{
    abi_long ret;
    void *addr;

    if ((int)addrlen < 0) {
        return -TARGET_EINVAL;
    }
    addr = alloca(addrlen+1);

    ret = target_to_host_sockaddr(addr, target_addr, addrlen);

    if (is_error(ret)) {
        return ret;
    }

    return get_errno(connect(sockfd, addr, addrlen));
}

/* accept(2) */
static inline abi_long do_bsd_accept(int fd, abi_ulong target_addr,
        abi_ulong target_addrlen_addr)
{
    socklen_t addrlen;
    void *addr;
    abi_long ret;

    if (target_addr == 0) {
        return get_errno(accept(fd, NULL, NULL));
    }
    /* return EINVAL if addrlen pointer is invalid */
    if (get_user_u32(addrlen, target_addrlen_addr)) {
        return -TARGET_EINVAL;
    }
    if ((int)addrlen < 0) {
        return -TARGET_EINVAL;
    }
    if (!access_ok(VERIFY_WRITE, target_addr, addrlen)) {
        return -TARGET_EINVAL;
    }
    addr = alloca(addrlen);

    ret = get_errno(accept(fd, addr, &addrlen));
    if (!is_error(ret)) {
        host_to_target_sockaddr(target_addr, addr, addrlen);
        if (put_user_u32(addrlen, target_addrlen_addr)) {
            ret = -TARGET_EFAULT;
        }
    }
    return ret;
}

/* getpeername(2) */
static inline abi_long do_bsd_getpeername(int fd, abi_ulong target_addr,
        abi_ulong target_addrlen_addr)
{
    socklen_t addrlen;
    void *addr;
    abi_long ret;

    if (get_user_u32(addrlen, target_addrlen_addr)) {
        return -TARGET_EFAULT;
    }
    if ((int)addrlen < 0) {
        return -TARGET_EINVAL;
    }
    if (!access_ok(VERIFY_WRITE, target_addr, addrlen)) {
        return -TARGET_EFAULT;
    }
    addr = alloca(addrlen);
    ret = get_errno(getpeername(fd, addr, &addrlen));
    if (!is_error(ret)) {
        host_to_target_sockaddr(target_addr, addr, addrlen);
        if (put_user_u32(addrlen, target_addrlen_addr)) {
            ret = -TARGET_EFAULT;
        }
    }
    return ret;
}

/* getsockname(2) */
static inline abi_long do_bsd_getsockname(int fd, abi_ulong target_addr,
        abi_ulong target_addrlen_addr)
{
    socklen_t addrlen;
    void *addr;
    abi_long ret;

    if (get_user_u32(addrlen, target_addrlen_addr)) {
        return -TARGET_EFAULT;
    }
    if ((int)addrlen < 0) {
        return -TARGET_EINVAL;
    }
    if (!access_ok(VERIFY_WRITE, target_addr, addrlen)) {
        return -TARGET_EFAULT;
    }
    addr = alloca(addrlen);

    ret = get_errno(getsockname(fd, addr, &addrlen));
    if (!is_error(ret)) {
        host_to_target_sockaddr(target_addr, addr, addrlen);
        if (put_user_u32(addrlen, target_addrlen_addr)) {
            ret = -TARGET_EFAULT;
        }
    }
    return ret;
}

/* socketpair(2) */
static inline abi_long do_bsd_socketpair(int domain, int type, int protocol,
        abi_ulong target_tab_addr)
{
    int tab[2];
    abi_long ret;

    ret = get_errno(socketpair(domain, type, protocol, tab));
    if (!is_error(ret)) {
        if (put_user_s32(tab[0], target_tab_addr) ||
                put_user_s32(tab[1], target_tab_addr + sizeof(tab[0]))) {
            ret = -TARGET_EFAULT;
        }
    }
    return ret;
}

/* sendto(2) */
static inline abi_long do_bsd_sendto(int fd, abi_ulong msg, size_t len,
        int flags, abi_ulong target_addr, socklen_t addrlen)
{
    struct sockaddr *saddr;
    void *host_msg;
    abi_long ret;

    if ((int)addrlen < 0) {
        return -TARGET_EINVAL;
    }
    host_msg = lock_user(VERIFY_READ, msg, len, 1);
    if (!host_msg) {
        return -TARGET_EFAULT;
    }
    if (target_addr) {
        saddr = alloca(addrlen);
        ret = target_to_host_sockaddr(saddr, target_addr, addrlen);
        if (is_error(ret)) {
            unlock_user(host_msg, msg, 0);
            return ret;
        }
        ret = get_errno(sendto(fd, host_msg, len, flags, saddr, addrlen));
    } else {
        ret = get_errno(send(fd, host_msg, len, flags));
    }
    unlock_user(host_msg, msg, 0);
    return ret;
}

/* recvfrom(2) */
static inline abi_long do_bsd_recvfrom(int fd, abi_ulong msg, size_t len,
        int flags, abi_ulong target_addr, abi_ulong target_addrlen)
{
    socklen_t addrlen;
    struct sockaddr *saddr;
    void *host_msg;
    abi_long ret;

    host_msg = lock_user(VERIFY_WRITE, msg, len, 0);
    if (!host_msg) {
        return -TARGET_EFAULT;
    }
    if (target_addr) {
        if (get_user_u32(addrlen, target_addrlen)) {
            ret = -TARGET_EFAULT;
            goto fail;
        }
        if ((int)addrlen < 0) {
            ret = -TARGET_EINVAL;
            goto fail;
        }
        saddr = alloca(addrlen);
        ret = get_errno(recvfrom(fd, host_msg, len, flags, saddr, &addrlen));
    } else {
        saddr = NULL; /* To keep compiler quiet.  */
        ret = get_errno(qemu_recv(fd, host_msg, len, flags));
    }
    if (!is_error(ret)) {
        if (target_addr) {
            host_to_target_sockaddr(target_addr, saddr, addrlen);
            if (put_user_u32(addrlen, target_addrlen)) {
                ret = -TARGET_EFAULT;
                goto fail;
            }
        }
        unlock_user(host_msg, msg, len);
    } else {
fail:
        unlock_user(host_msg, msg, 0);
    }
    return ret;
}

/* socket(2) */
static inline abi_long do_bsd_socket(abi_long domain, abi_long type,
        abi_long protocol)
{

    return get_errno(socket(domain, type, protocol));
}

/* shutdown(2) */
static inline abi_long do_bsd_shutdown(abi_long s, abi_long how)
{

    return get_errno(shutdown(s, how));
}

#endif /* !__BSD_SOCKET_H_ */
