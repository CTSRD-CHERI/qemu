/*
 *  FreeBSD socket related system call shims
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
#ifndef __FREEBSD_SOCKET_H_
#define __FREEBSD_SOCKET_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>

#include "qemu-os.h"

/* sendmsg(2) */
static inline abi_long do_freebsd_sendmsg(int fd, abi_ulong target_msg,
        int flags)
{
    abi_long ret;
    struct target_msghdr *msgp;
    struct msghdr msg;
    int count;
    struct iovec *vec;
    abi_ulong target_vec;

    if (!lock_user_struct(VERIFY_READ, msgp, target_msg, 1)) {
        return -TARGET_EFAULT;
    }
    if (msgp->msg_name) {
        msg.msg_namelen = tswap32(msgp->msg_namelen);
        msg.msg_name = alloca(msg.msg_namelen);
        ret = target_to_host_sockaddr(msg.msg_name,
            tswapal(msgp->msg_name), msg.msg_namelen);

        if (is_error(ret)) {
            unlock_user_struct(msgp, target_msg, 0);
            return ret;
        }
    } else {
        msg.msg_name = NULL;
        msg.msg_namelen = 0;
    }
    if (tswapal(msgp->msg_controllen) > 0) {
        msg.msg_controllen = 2 * tswapal(msgp->msg_controllen);
        msg.msg_control = alloca(msg.msg_controllen);
        msg.msg_flags = tswap32(msgp->msg_flags);
    } else {
        msg.msg_controllen = 0;
        msg.msg_control = NULL;
        msg.msg_flags = 0;
    }

    count = tswapal(msgp->msg_iovlen);
    target_vec = tswapal(msgp->msg_iov);
    vec = lock_iovec(VERIFY_READ, target_vec, count, 1);
    msg.msg_iovlen = count;
    msg.msg_iov = vec;

    ret = t2h_freebsd_cmsg(&msg, msgp);
    if (!is_error(ret)) {
        ret = get_errno(sendmsg(fd, &msg, flags));
    }
    unlock_iovec(vec, target_vec, count, 0);
    unlock_user_struct(msgp, target_msg, 0);
    return ret;
}

/* recvmsg(2) */
static inline abi_long do_freebsd_recvmsg(int fd, abi_ulong target_msg,
        int flags)
{
    abi_long ret, len;
    struct target_msghdr *msgp;
    struct msghdr msg;
    int count;
    struct iovec *vec;
    abi_ulong target_vec;

    if (!lock_user_struct(VERIFY_WRITE, msgp, target_msg, 0)) {
        return -TARGET_EFAULT;
    }
    if (msgp->msg_name) {
        msg.msg_namelen = tswap32(msgp->msg_namelen);
        msg.msg_name = alloca(msg.msg_namelen);
        ret = target_to_host_sockaddr(msg.msg_name,
            tswapal(msgp->msg_name), msg.msg_namelen);

        if (is_error(ret)) {
            unlock_user_struct(msgp, target_msg, 1);
            return ret;
        }
    } else {
        msg.msg_name = NULL;
        msg.msg_namelen = 0;
    }
    msg.msg_controllen = 2 * tswapal(msgp->msg_controllen);
    msg.msg_control = alloca(msg.msg_controllen);
    msg.msg_flags = tswap32(msgp->msg_flags);

    count = tswapal(msgp->msg_iovlen);
    target_vec = tswapal(msgp->msg_iov);
    vec = lock_iovec(VERIFY_WRITE, target_vec, count, 0);
    msg.msg_iovlen = count;
    msg.msg_iov = vec;

    ret = get_errno(recvmsg(fd, &msg, flags));
    if (!is_error(ret)) {
        len = ret;
        ret = h2t_freebsd_cmsg(msgp, &msg);
        if (!is_error(ret)) {
            msgp->msg_namelen = tswap32(msg.msg_namelen);
            if (msg.msg_name != NULL) {
                ret = host_to_target_sockaddr(tswapal(msgp->msg_name),
                        msg.msg_name, msg.msg_namelen);
                if (is_error(ret)) {
                    goto out;
                }
            }
        }
        ret = len;
    }
out:
    unlock_iovec(vec, target_vec, count, 1);
    unlock_user_struct(msgp, target_msg, 1);
    return ret;
}

/* setsockopt(2) */
static inline abi_long do_bsd_setsockopt(int sockfd, int level, int optname,
        abi_ulong optval_addr, socklen_t optlen)
{
    abi_long ret;
    int val;
    struct ip_mreqn *ip_mreq;
    void *p;

    switch (level) {
    case IPPROTO_TCP:
        /* TCP options all take an 'int' value. */
        if (optlen < sizeof(uint32_t)) {
            return -TARGET_EINVAL;
        }
        if (get_user_u32(val, optval_addr)) {
            return -TARGET_EFAULT;
        }
        ret = get_errno(setsockopt(sockfd, level, optname, &val, sizeof(val)));
        break;

    case IPPROTO_IP:
        switch (optname) {
        case IP_OPTIONS:
            p = lock_user(VERIFY_READ, optval_addr, optlen, 0);
            if (p == NULL)
                return -TARGET_EFAULT;
            ret = get_errno(setsockopt(sockfd, level, optname, p, optlen));
            unlock_user(p, optval_addr, 0);
            break;
        case IP_HDRINCL:/* int; header is included with data */
        case IP_TOS:    /* int; IP type of service and preced. */
        case IP_TTL:    /* int; IP time to live */
        case IP_RECVOPTS: /* bool; receive all IP opts w/dgram */
        case IP_RECVRETOPTS: /* bool; receive IP opts for response */
        case IP_RECVDSTADDR: /* bool; receive IP dst addr w/dgram */
        case IP_MULTICAST_IF:/* u_char; set/get IP multicast i/f  */
        case IP_MULTICAST_TTL:/* u_char; set/get IP multicast ttl */
        case IP_MULTICAST_LOOP:/*u_char;set/get IP multicast loopback */
        case IP_PORTRANGE: /* int; range to choose for unspec port */
        case IP_RECVIF: /* bool; receive reception if w/dgram */
        case IP_IPSEC_POLICY:   /* int; set/get security policy */
#if defined(__FreeBSD_version) && __FreeBSD_version <= 1100000
        case IP_FAITH:  /* bool; accept FAITH'ed connections */
#endif
        case IP_RECVTTL: /* bool; receive reception TTL w/dgram */
            val = 0;
            if (optlen >= sizeof(uint32_t)) {
                if (get_user_u32(val, optval_addr)) {
                    return -TARGET_EFAULT;
                }
            } else if (optlen >= 1) {
                if (get_user_u8(val, optval_addr)) {
                    return -TARGET_EFAULT;
                }
            }
            ret = get_errno(setsockopt(sockfd, level, optname, &val,
                        sizeof(val)));
            break;

        case IP_ADD_MEMBERSHIP: /*ip_mreq; add an IP group membership */
        case IP_DROP_MEMBERSHIP:/*ip_mreq; drop an IP group membership*/
            if (optlen < sizeof(struct target_ip_mreq) ||
                    optlen > sizeof(struct target_ip_mreqn)) {
                return -TARGET_EINVAL;
            }
            ip_mreq = (struct ip_mreqn *) alloca(optlen);
            target_to_host_ip_mreq(ip_mreq, optval_addr, optlen);
            ret = get_errno(setsockopt(sockfd, level, optname, ip_mreq,
                        optlen));
            break;

        default:
            goto unimplemented;
        }
        break;

    case IPPROTO_IPV6:
        switch (optname) {
        case IPV6_UNICAST_HOPS:     /* int; IP6 hops */
        case IPV6_MULTICAST_IF:     /* u_int; set/get IP6 multicast i/f  */
        case IPV6_MULTICAST_HOPS:   /* int; set/get IP6 multicast hops */
        case IPV6_MULTICAST_LOOP:   /* u_int; set/get IP6 multicast loopback */
        case IPV6_PORTRANGE:        /* int; range to choose for unspec port */
        case IPV6_CHECKSUM:         /* int; checksum offset for raw socket */
        case IPV6_V6ONLY:           /* bool; make AF_INET6 sockets v6 only */
        case IPV6_RECVPKTINFO:      /* bool; recv if, dst addr */
        case IPV6_RECVHOPLIMIT:     /* bool; recv hop limit */
        case IPV6_RECVRTHDR:        /* bool; recv routing header */
        case IPV6_RECVHOPOPTS:      /* bool; recv hop-by-hop option */
        case IPV6_RECVDSTOPTS:      /* bool; recv dst option after rthdr */
        case IPV6_USE_MIN_MTU:      /* bool; send packets at the minimum MTU */
        case IPV6_RECVPATHMTU:      /* bool; notify an according MTU */
        case IPV6_HOPLIMIT:         /* int; send hop limit */
        case IPV6_RECVTCLASS:       /* bool; recv traffic class values */
        case IPV6_AUTOFLOWLABEL:    /* bool; attach flowlabel automagically */
        case IPV6_TCLASS:           /* int; send traffic class value */
        case IPV6_DONTFRAG:         /* bool; disable IPv6 fragmentation */
        case IPV6_PREFER_TEMPADDR:  /* int; prefer temporary addresses */
        case IPV6_BINDANY:          /* bool: allow bind to any address */
#ifdef IPV6_BINDMULTI
        case IPV6_BINDMULTI:        /* bool; allow multibind to same addr/port*/
#endif /* IPV6_BINDMULTI */
#ifdef IPV6_RSS_LISTEN_BUCKET
        case IPV6_RSS_LISTEN_BUCKET: /* int; set RSS listen bucket */
#endif /* IPV6_RSS_LISTEN_BUCKET */
#ifdef IPV6_FLOWID
        case IPV6_FLOWID:           /* int; flowid of given socket */
#endif /* IPV6_FLOWID */
#ifdef IPV6_FLOWTYPE
        case IPV6_FLOWTYPE:         /* int; flowtype of given socket */
#endif /* IPV6_FLOWTYPE */
#ifdef IPV6_RSSBUCKETID
        case IPV6_RSSBUCKETID:      /* int; RSS bucket ID of given socket */
#endif /* IPV6_RSSBUCKETID */
            val = 0;
            if (optlen >= sizeof(uint32_t)) {
                if (get_user_u32(val, optval_addr)) {
                    return -TARGET_EFAULT;
                }
            } else if (optlen >= 1) {
                if (get_user_u8(val, optval_addr)) {
                    return -TARGET_EFAULT;
                }
            }
            ret = get_errno(setsockopt(sockfd, level, optname, &val,
                        sizeof(val)));
            break;

        case IPV6_JOIN_GROUP:   /* ipv6_mreq; join a group membership */
        case IPV6_LEAVE_GROUP:  /* ipv6_mreq; leave a group membership */
        case ICMP6_FILTER:      /* icmp6_filter; icmp6 filter */
        case IPV6_IPSEC_POLICY: /* struct; get/set security policy */
        case IPV6_FW_ADD:       /* add a firewall rule to chain */
        case IPV6_FW_DEL:       /* delete a firewall rule from chain */
        case IPV6_FW_FLUSH:     /* flush firewall rule chain */
        case IPV6_FW_ZERO:      /* clear single/all firewall counter(s) */
        case IPV6_FW_GET:       /* get entire firewall rule chain */
        case IPV6_RTHDRDSTOPTS: /* ip6_dest; send dst option before rthdr */
        case IPV6_PATHMTU:      /* mtuinfo; get the current path MTU */
        case IPV6_PKTINFO:      /* in6_pktinfo; send if, src addr */
        case IPV6_NEXTHOP:      /* sockaddr; next hop addr */
        case IPV6_HOPOPTS:      /* ip6_hbh; send hop-by-hop option */
        case IPV6_DSTOPTS:      /* ip6_dest; send dst option befor rthdr */
        case IPV6_RTHDR:        /* ip6_rthdr; send routing header */
        case IPV6_MSFILTER:     /* struct __msfilterreq; */
        default:
            goto unimplemented;
        }
        break;

    case TARGET_SOL_SOCKET:
        switch (optname) {
        /* Options with 'int' argument.  */
        case TARGET_SO_DEBUG:
            optname = SO_DEBUG;
            break;

        case TARGET_SO_REUSEADDR:
            optname = SO_REUSEADDR;
            break;

        case TARGET_SO_REUSEPORT:
            optname = SO_REUSEADDR;
            break;

        case TARGET_SO_KEEPALIVE:
            optname = SO_KEEPALIVE;
            break;

        case TARGET_SO_DONTROUTE:
            optname = SO_DONTROUTE;
            break;

        case TARGET_SO_LINGER:
            optname = SO_LINGER;
            break;

        case TARGET_SO_BROADCAST:
            optname = SO_BROADCAST;
            break;

        case TARGET_SO_OOBINLINE:
            optname = SO_OOBINLINE;
            break;

        case TARGET_SO_SNDBUF:
            optname = SO_SNDBUF;
            break;

        case TARGET_SO_RCVBUF:
            optname = SO_RCVBUF;
            break;

        case TARGET_SO_SNDLOWAT:
            optname = SO_RCVLOWAT;
            break;

        case TARGET_SO_RCVLOWAT:
            optname = SO_RCVLOWAT;
            break;

        case TARGET_SO_SNDTIMEO:
            optname = SO_SNDTIMEO;
            break;

        case TARGET_SO_RCVTIMEO:
            optname = SO_RCVTIMEO;
            break;

        case TARGET_SO_ACCEPTFILTER:
            goto unimplemented;

        case TARGET_SO_NOSIGPIPE:
            optname = SO_NOSIGPIPE;
            break;

        case TARGET_SO_TIMESTAMP:
            optname = SO_TIMESTAMP;
            break;

        case TARGET_SO_BINTIME:
            optname = SO_BINTIME;
            break;

        case TARGET_SO_ERROR:
            optname = SO_ERROR;
            break;

        case TARGET_SO_SETFIB:
            optname = SO_ERROR;
            break;

#ifdef SO_USER_COOKIE
        case TARGET_SO_USER_COOKIE:
            optname = SO_USER_COOKIE;
            break;
#endif
        default:
            goto unimplemented;
        }
        if (optlen < sizeof(uint32_t)) {
            return -TARGET_EINVAL;
        }
        if (get_user_u32(val, optval_addr)) {
            return -TARGET_EFAULT;
        }
        ret = get_errno(setsockopt(sockfd, SOL_SOCKET, optname, &val,
                    sizeof(val)));
        break;
    default:
unimplemented:
    gemu_log("Unsupported setsockopt level=%d optname=%d\n",
        level, optname);
    ret = -TARGET_ENOPROTOOPT;
    }

    return ret;
}

/* getsockopt(2) */
static inline abi_long do_bsd_getsockopt(int sockfd, int level, int optname,
        abi_ulong optval_addr, abi_ulong optlen)
{
    abi_long ret;
    int len, val;
    socklen_t lv;
    void *p;

    switch (level) {
    case TARGET_SOL_SOCKET:
        level = SOL_SOCKET;
        switch (optname) {

        /* These don't just return a single integer */
        case TARGET_SO_LINGER:
        case TARGET_SO_RCVTIMEO:
        case TARGET_SO_SNDTIMEO:
        case TARGET_SO_ACCEPTFILTER:
            goto unimplemented;

        /* Options with 'int' argument.  */
        case TARGET_SO_DEBUG:
            optname = SO_DEBUG;
            goto int_case;

        case TARGET_SO_REUSEADDR:
            optname = SO_REUSEADDR;
            goto int_case;

        case TARGET_SO_REUSEPORT:
            optname = SO_REUSEPORT;
            goto int_case;

        case TARGET_SO_TYPE:
            optname = SO_TYPE;
            goto int_case;

        case TARGET_SO_ERROR:
            optname = SO_ERROR;
            goto int_case;

        case TARGET_SO_DONTROUTE:
            optname = SO_DONTROUTE;
            goto int_case;

        case TARGET_SO_BROADCAST:
            optname = SO_BROADCAST;
            goto int_case;

        case TARGET_SO_SNDBUF:
            optname = SO_SNDBUF;
            goto int_case;

        case TARGET_SO_RCVBUF:
            optname = SO_RCVBUF;
            goto int_case;

        case TARGET_SO_KEEPALIVE:
            optname = SO_KEEPALIVE;
            goto int_case;

        case TARGET_SO_OOBINLINE:
            optname = SO_OOBINLINE;
            goto int_case;

        case TARGET_SO_TIMESTAMP:
            optname = SO_TIMESTAMP;
            goto int_case;

        case TARGET_SO_RCVLOWAT:
            optname = SO_RCVLOWAT;
            goto int_case;

        case TARGET_SO_LISTENINCQLEN:
            optname = SO_LISTENINCQLEN;
            goto int_case;

        default:
int_case:
            if (get_user_u32(len, optlen)) {
                return -TARGET_EFAULT;
            }
            if (len < 0) {
                return -TARGET_EINVAL;
            }
            lv = sizeof(lv);
            ret = get_errno(getsockopt(sockfd, level, optname, &val, &lv));
            if (ret < 0) {
                return ret;
            }
            if (len > lv) {
                len = lv;
            }
            if (len == 4) {
                if (put_user_u32(val, optval_addr)) {
                    return -TARGET_EFAULT;
                }
            } else {
                if (put_user_u8(val, optval_addr)) {
                    return -TARGET_EFAULT;
                }
            }
            if (put_user_u32(len, optlen)) {
                return -TARGET_EFAULT;
            }
            break;

        }
        break;

    case IPPROTO_TCP:
        /* TCP options all take an 'int' value. */
        goto int_case;

    case IPPROTO_IP:
        switch (optname) {
        case IP_OPTIONS:
            if (get_user_u32(len, optlen))
                return -TARGET_EFAULT;
            lv = (socklen_t)len;
            p = lock_user(VERIFY_WRITE, optval_addr, len, 0);
            if (p == NULL)
                return -TARGET_EFAULT;
            ret = get_errno(getsockopt(sockfd, level, optname, p, &lv));
            unlock_user(p, optval_addr, len);
            if (put_user_u32(lv, optlen))
                return -TARGET_EFAULT;
            break;
        case IP_HDRINCL:
        case IP_TOS:
        case IP_TTL:
        case IP_RECVOPTS:
        case IP_RECVRETOPTS:
        case IP_RECVDSTADDR:

        case IP_RETOPTS:
#if defined(__FreeBSD_version) && __FreeBSD_version > 900000 && defined(IP_RECVTOS)
        case IP_RECVTOS:
#endif
        case IP_MULTICAST_TTL:
        case IP_MULTICAST_LOOP:
        case IP_PORTRANGE:
        case IP_IPSEC_POLICY:
#if defined(__FreeBSD_version) && __FreeBSD_version <= 1100000
        case IP_FAITH:
#endif
        case IP_ONESBCAST:
        case IP_BINDANY:
            if (get_user_u32(len, optlen)) {
                return -TARGET_EFAULT;
            }
            if (len < 0) {
                return -TARGET_EINVAL;
            }
            lv = sizeof(lv);
            ret = get_errno(getsockopt(sockfd, level, optname,
                &val, &lv));
            if (ret < 0) {
                return ret;
            }
            if (len < sizeof(int) && len > 0 && val >= 0 &&
                val < 255) {
                len = 1;
                if (put_user_u32(len, optlen) ||
                        put_user_u8(val, optval_addr)) {
                    return -TARGET_EFAULT;
                }
            } else {
                if (len > sizeof(int)) {
                    len = sizeof(int);
                }
                if (put_user_u32(len, optlen) ||
                        put_user_u32(val, optval_addr)) {
                    return -TARGET_EFAULT;
                }
            }
            break;

        default:
            goto unimplemented;
        }
        break;

    case IPPROTO_IPV6:
        switch (optname) {
        case IPV6_UNICAST_HOPS:     /* int; IP6 hops */
        case IPV6_MULTICAST_IF:     /* u_int; set/get IP6 multicast i/f  */
        case IPV6_MULTICAST_HOPS:   /* int; set/get IP6 multicast hops */
        case IPV6_MULTICAST_LOOP:   /* u_int; set/get IP6 multicast loopback */
        case IPV6_PORTRANGE:        /* int; range to choose for unspec port */
        case IPV6_CHECKSUM:         /* int; checksum offset for raw socket */
        case IPV6_V6ONLY:           /* bool; make AF_INET6 sockets v6 only */
        case IPV6_RECVPKTINFO:      /* bool; recv if, dst addr */
        case IPV6_RECVHOPLIMIT:     /* bool; recv hop limit */
        case IPV6_RECVRTHDR:        /* bool; recv routing header */
        case IPV6_RECVHOPOPTS:      /* bool; recv hop-by-hop option */
        case IPV6_RECVDSTOPTS:      /* bool; recv dst option after rthdr */
        case IPV6_USE_MIN_MTU:      /* bool; send packets at the minimum MTU */
        case IPV6_RECVPATHMTU:      /* bool; notify an according MTU */
        case IPV6_HOPLIMIT:         /* int; send hop limit */
        case IPV6_RECVTCLASS:       /* bool; recv traffic class values */
        case IPV6_AUTOFLOWLABEL:    /* bool; attach flowlabel automagically */
        case IPV6_TCLASS:           /* int; send traffic class value */
        case IPV6_DONTFRAG:         /* bool; disable IPv6 fragmentation */
        case IPV6_PREFER_TEMPADDR:  /* int; prefer temporary addresses */
        case IPV6_BINDANY:          /* bool: allow bind to any address */
#ifdef IPV6_BINDMULTI
        case IPV6_BINDMULTI:        /* bool; allow multibind to same addr/port*/
#endif /* IPV6_BINDMULTI */
#ifdef IPV6_RSS_LISTEN_BUCKET
        case IPV6_RSS_LISTEN_BUCKET: /* int; set RSS listen bucket */
#endif /* IPV6_RSS_LISTEN_BUCKET */
#ifdef IPV6_FLOWID
        case IPV6_FLOWID:           /* int; flowid of given socket */
#endif /* IPV6_FLOWID */
#ifdef IPV6_FLOWTYPE
        case IPV6_FLOWTYPE:         /* int; flowtype of given socket */
#endif /* IPV6_FLOWTYPE */
#ifdef IPV6_RSSBUCKETID
        case IPV6_RSSBUCKETID:      /* int; RSS bucket ID of given socket */
#endif /* IPV6_RSSBUCKETID */
            if (get_user_u32(len, optlen)) {
                return -TARGET_EFAULT;
            }
            if (len < 0) {
                return -TARGET_EINVAL;
            }
            lv = sizeof(lv);
            ret = get_errno(getsockopt(sockfd, level, optname,
                &val, &lv));
            if (ret < 0) {
                return ret;
            }
            if (len < sizeof(int) && len > 0 && val >= 0 &&
                val < 255) {
                len = 1;
                if (put_user_u32(len, optlen) ||
                        put_user_u8(val, optval_addr)) {
                    return -TARGET_EFAULT;
                }
            } else {
                if (len > sizeof(int)) {
                    len = sizeof(int);
                }
                if (put_user_u32(len, optlen) ||
                        put_user_u32(val, optval_addr)) {
                    return -TARGET_EFAULT;
                }
            }
            break;

        case IPV6_JOIN_GROUP:   /* ipv6_mreq; join a group membership */
        case IPV6_LEAVE_GROUP:  /* ipv6_mreq; leave a group membership */
        case ICMP6_FILTER:      /* icmp6_filter; icmp6 filter */
        case IPV6_IPSEC_POLICY: /* struct; get/set security policy */
        case IPV6_FW_ADD:       /* add a firewall rule to chain */
        case IPV6_FW_DEL:       /* delete a firewall rule from chain */
        case IPV6_FW_FLUSH:     /* flush firewall rule chain */
        case IPV6_FW_ZERO:      /* clear single/all firewall counter(s) */
        case IPV6_FW_GET:       /* get entire firewall rule chain */
        case IPV6_RTHDRDSTOPTS: /* ip6_dest; send dst option before rthdr */
        case IPV6_PATHMTU:      /* mtuinfo; get the current path MTU */
        case IPV6_PKTINFO:      /* in6_pktinfo; send if, src addr */
        case IPV6_NEXTHOP:      /* sockaddr; next hop addr */
        case IPV6_HOPOPTS:      /* ip6_hbh; send hop-by-hop option */
        case IPV6_DSTOPTS:      /* ip6_dest; send dst option befor rthdr */
        case IPV6_RTHDR:        /* ip6_rthdr; send routing header */
        case IPV6_MSFILTER:     /* struct __msfilterreq; */
        default:
            goto unimplemented;
        }
        break;

    default:
unimplemented:
        gemu_log("getsockopt level=%d optname=%d not yet supported\n",
            level, optname);
        ret = -TARGET_EOPNOTSUPP;
        break;
    }
    return ret;
}

/* setfib(2) */
static inline abi_long do_freebsd_setfib(abi_long fib)
{

    return get_errno(setfib(fib));
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

/* sendfile(2) */
static inline abi_long do_freebsd_sendfile(abi_long fd, abi_long s,
        abi_ulong arg3, abi_ulong arg4, abi_ulong nbytes, abi_ulong target_hdtr,
        abi_ulong target_sbytes, abi_long flags)
{

    qemu_log("qemu: Unsupported syscall sendfile()\n");
    return -TARGET_ENOSYS;
}

#if defined(__FreeBSD_version) && __FreeBSD_version >= 1000000
/* bindat(2) */
static inline abi_long do_freebsd_bindat(int fd, int sockfd,
	abi_ulong target_addr, socklen_t addrlen)
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

    return get_errno(bindat(fd, sockfd, addr, addrlen));
}

/* connectat(2) */
static inline abi_long do_freebsd_connectat(int fd, int sockfd,
	abi_ulong target_addr, socklen_t addrlen)
{
    abi_long ret;
    void *addr;

    if ((int)addrlen < 0) {
        return -TARGET_EINVAL;
    }
    addr = alloca(addrlen);

    ret = target_to_host_sockaddr(addr, target_addr, addrlen);

    if (is_error(ret)) {
        return ret;
    }

    return get_errno(connectat(fd, sockfd, addr, addrlen));
}

/* accept4(2) */
static inline abi_long do_freebsd_accept4(int fd, abi_ulong target_addr,
        abi_ulong target_addrlen_addr, int flags)
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

    ret = get_errno(accept4(fd, addr, &addrlen, flags));
    if (!is_error(ret)) {
        host_to_target_sockaddr(target_addr, addr, addrlen);
        if (put_user_u32(addrlen, target_addrlen_addr)) {
            ret = -TARGET_EFAULT;
        }
    }
    return ret;
}

#else /* ! __FreeBSD_version >= 1000000 */

/* bindat(2) */
static inline abi_long do_freebsd_bindat(__unused int fd, __unused int sockfd,
	__unused abi_ulong target_addr, __unused socklen_t addrlen)
{

    qemu_log("qemu: Unsupported syscall bindat()\n");
    return -TARGET_ENOSYS;
}

/* connectat(2) */
static inline abi_long do_freebsd_connectat(__unused int fd,
	__unused int sockfd, __unused abi_ulong target_addr,
	__unused socklen_t addrlen)
{

    qemu_log("qemu: Unsupported syscall connectat()\n");
    return -TARGET_ENOSYS;
}

/* accept4(2) */
static inline abi_long do_freebsd_accept4(__unused int fd,
	__unused abi_ulong target_addr, __unused abi_ulong target_addrlen_addr,
	__unused int flags)
{

    qemu_log("qemu: Unsupported syscall accept4()\n");
    return -TARGET_ENOSYS;
}
#endif /* ! __FreeBSD_version >= 1000000 */
#endif /* !__FREEBSD_SOCKET_H_ */
