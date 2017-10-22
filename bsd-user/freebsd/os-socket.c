/*
 *  FreeBSD socket related system call helpers
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

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "qemu.h"
#include "qemu-os.h"

abi_long t2h_freebsd_cmsg(struct msghdr *msgh,
        struct target_msghdr *target_msgh)
{
    struct cmsghdr *cmsg = CMSG_FIRSTHDR(msgh);
    abi_long msg_controllen;
    abi_ulong target_cmsg_addr;
    struct target_cmsghdr *target_cmsg;
    socklen_t space = 0;


    msg_controllen = tswapal(target_msgh->msg_controllen);
    if (msg_controllen < sizeof(struct target_cmsghdr)) {
        goto the_end;
    }
    target_cmsg_addr = tswapal(target_msgh->msg_control);
    target_cmsg = lock_user(VERIFY_READ, target_cmsg_addr, msg_controllen, 1);
    if (target_cmsg == 0) {
        return -TARGET_EFAULT;
    }
    while (cmsg && target_cmsg) {
        void *data = CMSG_DATA(cmsg);
        void *target_data = TARGET_CMSG_DATA(target_cmsg);
        int len = tswapal(target_cmsg->cmsg_len) -
            TARGET_CMSG_ALIGN(sizeof(struct target_cmsghdr));
        space += CMSG_SPACE(len);
        if (space > msgh->msg_controllen) {
            space -= CMSG_SPACE(len);
            gemu_log("Host cmsg overflow\n");
            break;
        }
        cmsg->cmsg_level = tswap32(target_cmsg->cmsg_level);
        cmsg->cmsg_type = tswap32(target_cmsg->cmsg_type);
        cmsg->cmsg_len = CMSG_LEN(len);

        if ((cmsg->cmsg_level == TARGET_SOL_SOCKET) &&
            (cmsg->cmsg_type == SCM_RIGHTS)) {
            int *fd = (int *)data;
            int *target_fd = (int *)target_data;
            int i, numfds = len / sizeof(int);

            for (i = 0; i < numfds; i++) {
                fd[i] = tswap32(target_fd[i]);
            }
        } else if ((cmsg->cmsg_level == TARGET_SOL_SOCKET) &&
            (cmsg->cmsg_type == SCM_TIMESTAMP) &&
            (len == sizeof(struct timeval)))  {
            /* copy struct timeval to host */
            struct timeval *tv = (struct timeval *)data;
            struct target_freebsd_timeval *target_tv =
                (struct target_freebsd_timeval *)target_data;
            __get_user(tv->tv_sec, &target_tv->tv_sec);
            __get_user(tv->tv_usec, &target_tv->tv_usec);
        } else {
            gemu_log("Unsupported ancillary data: %d/%d\n",
                cmsg->cmsg_level, cmsg->cmsg_type);
            memcpy(data, target_data, len);
        }
        cmsg = CMSG_NXTHDR(msgh, cmsg);
        target_cmsg = TARGET_CMSG_NXTHDR(target_msgh, target_cmsg);
    }
    unlock_user(target_cmsg, target_cmsg_addr, 0);

the_end:
    msgh->msg_controllen = space;
    return 0;
}

abi_long h2t_freebsd_cmsg(struct target_msghdr *target_msgh,
        struct msghdr *msgh)
{
    struct cmsghdr *cmsg = CMSG_FIRSTHDR(msgh);
    abi_long msg_controllen;
    abi_ulong target_cmsg_addr;
    struct target_cmsghdr *target_cmsg;
    socklen_t space = 0;

    msg_controllen = tswapal(target_msgh->msg_controllen);
    if (msg_controllen < sizeof(struct target_cmsghdr)) {
        goto the_end;
    }
    target_cmsg_addr = tswapal(target_msgh->msg_control);
    target_cmsg = lock_user(VERIFY_WRITE, target_cmsg_addr,
        msg_controllen, 0);
    if (target_cmsg == 0) {
        return -TARGET_EFAULT;
    }
    while (cmsg && target_cmsg) {
        void *data = CMSG_DATA(cmsg);
        void *target_data = TARGET_CMSG_DATA(target_cmsg);
        int len = cmsg->cmsg_len - CMSG_ALIGN(sizeof(struct cmsghdr));

        space += TARGET_CMSG_SPACE(len);
        if (space > msg_controllen) {
            space -= TARGET_CMSG_SPACE(len);
            gemu_log("Target cmsg overflow\n");
            break;
        }
        target_cmsg->cmsg_level = tswap32(cmsg->cmsg_level);
        target_cmsg->cmsg_type = tswap32(cmsg->cmsg_type);
        target_cmsg->cmsg_len = tswapal(TARGET_CMSG_LEN(len));
        if ((cmsg->cmsg_level == TARGET_SOL_SOCKET) &&
            (cmsg->cmsg_type == SCM_RIGHTS)) {
            int *fd = (int *)data;
            int *target_fd = (int *)target_data;
            int i, numfds = len / sizeof(int);

            for (i = 0; i < numfds; i++) {
                target_fd[i] = tswap32(fd[i]);
            }
        } else if ((cmsg->cmsg_level == TARGET_SOL_SOCKET) &&
            (cmsg->cmsg_type == SCM_TIMESTAMP) &&
            (len == sizeof(struct timeval))) {
            /* copy struct timeval to target */
            struct timeval *tv = (struct timeval *)data;
            struct target_freebsd_timeval *target_tv =
                (struct target_freebsd_timeval *)target_data;
            __put_user(tv->tv_sec, &target_tv->tv_sec);
            __put_user(tv->tv_usec, &target_tv->tv_usec);
        } else {
            gemu_log("Unsupported ancillary data: %d/%d\n",
                cmsg->cmsg_level, cmsg->cmsg_type);
            memcpy(target_data, data, len);
        }
        cmsg = CMSG_NXTHDR(msgh, cmsg);
        target_cmsg = TARGET_CMSG_NXTHDR(target_msgh, target_cmsg);
    }
    unlock_user(target_cmsg, target_cmsg_addr, space);

the_end:
    target_msgh->msg_controllen = tswapal(space);
    return 0;
}

