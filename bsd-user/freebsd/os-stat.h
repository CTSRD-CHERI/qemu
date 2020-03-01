/*
 *  stat related system call shims and definitions
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

#ifndef __FREEBSD_STAT_H_
#define __FREEBSD_STAT_H_

#include <sys/types.h>
#include <sys/ucred.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <dirent.h>

#include "qemu-os.h"

#if defined(__FreeBSD_version) && __FreeBSD_version < 1200031
/* undocumented nstat system calls */
int nstat(const char *path, struct stat *sb);
int nlstat(const char *path, struct stat *sb);
int nfstat(int fd, struct stat *sb);
#endif

/* stat(2) */
static inline abi_long do_freebsd_stat(abi_long arg1, abi_long arg2)
{
    abi_long ret;
    void *p;
    struct stat st;

    LOCK_PATH(p, arg1);
    ret = get_errno(stat(path(p), &st));
    UNLOCK_PATH(p, arg1);
    if (!is_error(ret)) {
        ret = h2t_freebsd_stat(arg2, &st);
    }
    return ret;
}

/* lstat(2) */
static inline abi_long do_freebsd_lstat(abi_long arg1, abi_long arg2)
{
    abi_long ret;
    void *p;
    struct stat st;

    LOCK_PATH(p, arg1);
    ret = get_errno(lstat(path(p), &st));
    UNLOCK_PATH(p, arg1);
    if (!is_error(ret)) {
        ret = h2t_freebsd_stat(arg2, &st);
    }
    return ret;
}

/* fstat(2) */
static inline abi_long do_freebsd_fstat(abi_long arg1, abi_long arg2)
{
    abi_long ret;
    struct stat st;

    ret = get_errno(fstat(arg1, &st));
    if (!is_error(ret))  {
        ret = h2t_freebsd_stat(arg2, &st);
    }
    return ret;
}

/* fstatat(2) */
static inline abi_long do_freebsd_fstatat(abi_long arg1, abi_long arg2,
        abi_long arg3, abi_long arg4)
{
    abi_long ret;
    void *p;
    struct stat st;

    LOCK_PATH(p, arg2);
    ret = get_errno(fstatat(arg1, p, &st, arg4));
    UNLOCK_PATH(p, arg2);
    if (!is_error(ret) && arg3) {
        ret = h2t_freebsd_stat(arg3, &st);
    }
    return ret;
}

#if defined(__FreeBSD_version) && __FreeBSD_version < 1200031
/* undocummented nstat(char *path, struct nstat *ub) syscall */
static abi_long do_freebsd_nstat(abi_long arg1, abi_long arg2)
{
    abi_long ret;
    void *p;
    struct stat st;

    LOCK_PATH(p, arg1);
    ret = get_errno(nstat(path(p), &st));
    UNLOCK_PATH(p, arg1);
    if (!is_error(ret)) {
        ret = h2t_freebsd_nstat(arg2, &st);
    }
    return ret;
}

/* undocummented nfstat(int fd, struct nstat *sb) syscall */
static abi_long do_freebsd_nfstat(abi_long arg1, abi_long arg2)
{
    abi_long ret;
    struct stat st;

    ret = get_errno(nfstat(arg1, &st));
    if (!is_error(ret))  {
        ret = h2t_freebsd_nstat(arg2, &st);
    }
    return ret;
}

/* undocummented nlstat(char *path, struct nstat *ub) syscall */
static abi_long do_freebsd_nlstat(abi_long arg1, abi_long arg2)
{
    abi_long ret;
    void *p;
    struct stat st;

    LOCK_PATH(p, arg1);
    ret = get_errno(nlstat(path(p), &st));
    UNLOCK_PATH(p, arg1);
    if (!is_error(ret)) {
        ret = h2t_freebsd_nstat(arg2, &st);
    }
    return ret;
}
#endif

/* getfh(2) */
static abi_long do_freebsd_getfh(abi_long arg1, abi_long arg2)
{
    abi_long ret;
    void *p;
    fhandle_t host_fh;

    LOCK_PATH(p, arg1);
    ret = get_errno(getfh(path(p), &host_fh));
    UNLOCK_PATH(p, arg1);
    if (is_error(ret)) {
        return ret;
    }
    return h2t_freebsd_fhandle(arg2, &host_fh);
}

/* lgetfh(2) */
static inline abi_long do_freebsd_lgetfh(abi_long arg1, abi_long arg2)
{
    abi_long ret;
    void *p;
    fhandle_t host_fh;

    LOCK_PATH(p, arg1);
    ret = get_errno(lgetfh(path(p), &host_fh));
    UNLOCK_PATH(p, arg1);
    if (is_error(ret)) {
        return ret;
    }
    return h2t_freebsd_fhandle(arg2, &host_fh);
}

/* fhopen(2) */
static inline abi_long do_freebsd_fhopen(abi_long arg1, abi_long arg2)
{
    abi_long ret;
    fhandle_t host_fh;

    ret = t2h_freebsd_fhandle(&host_fh, arg1);
    if (is_error(ret)) {
        return ret;
    }

    return get_errno(fhopen(&host_fh, arg2));
}

/* fhstat(2) */
static inline abi_long do_freebsd_fhstat(abi_long arg1, abi_long arg2)
{
    abi_long ret;
    fhandle_t host_fh;
    struct stat host_sb;

    ret = t2h_freebsd_fhandle(&host_fh, arg1);
    if (is_error(ret)) {
        return ret;
    }
    ret = get_errno(fhstat(&host_fh, &host_sb));
    if (is_error(ret)) {
        return ret;
    }
    return h2t_freebsd_stat(arg2, &host_sb);
}

/* fhstatfs(2) */
static inline abi_long do_freebsd_fhstatfs(abi_ulong target_fhp_addr,
        abi_ulong target_stfs_addr)
{
    abi_long ret;
    fhandle_t host_fh;
    struct statfs host_stfs;

    ret = t2h_freebsd_fhandle(&host_fh, target_fhp_addr);
    if (is_error(ret)) {
        return ret;
    }
    ret = get_errno(fhstatfs(&host_fh, &host_stfs));
    if (is_error(ret)) {
        return ret;
    }
    return h2t_freebsd_statfs(target_stfs_addr, &host_stfs);
}

/* statfs(2) */
static inline abi_long do_freebsd_statfs(abi_long arg1, abi_long arg2)
{
    abi_long ret;
    void *p;
    struct statfs host_stfs;

    LOCK_PATH(p, arg1);
    ret = get_errno(statfs(path(p), &host_stfs));
    UNLOCK_PATH(p, arg1);
    if (is_error(ret)) {
        return ret;
    }

    return h2t_freebsd_statfs(arg2, &host_stfs);
}

/* fstatfs(2) */
static inline abi_long do_freebsd_fstatfs(abi_long fd, abi_ulong target_addr)
{
    abi_long ret;
    struct statfs host_stfs;

    ret = get_errno(fstatfs(fd, &host_stfs));
    if (is_error(ret)) {
        return ret;
    }

    return h2t_freebsd_statfs(target_addr, &host_stfs);
}

/* getfsstat(2) */
static inline abi_long do_freebsd_getfsstat(abi_ulong target_addr,
        abi_long bufsize, abi_long flags)
{
    abi_long ret;
    struct statfs *host_stfs;
    int count;
    long host_bufsize;

    count = bufsize / sizeof(struct target_freebsd_statfs);

    /* if user buffer is NULL then return number of mounted FS's */
    if (target_addr == 0 || count == 0) {
        return get_errno(getfsstat(NULL, 0, flags));
    }

    /* XXX check count to be reasonable */
    host_bufsize = sizeof(struct statfs) * count;
    host_stfs = alloca(host_bufsize);
    if (!host_stfs) {
        return -TARGET_EINVAL;
    }

    ret = count = get_errno(getfsstat(host_stfs, host_bufsize, flags));
    if (is_error(ret)) {
        return ret;
    }

    while (count--) {
        if (h2t_freebsd_statfs((target_addr +
                        (count * sizeof(struct target_freebsd_statfs))),
                    &host_stfs[count])) {
            return -TARGET_EFAULT;
        }
    }
    return ret;
}

/* getdents(2) */
static inline abi_long do_freebsd_getdents(abi_long arg1, abi_ulong arg2,
        abi_long nbytes)
{
    abi_long ret;
    struct dirent *dirp;

    dirp = lock_user(VERIFY_WRITE, arg2, nbytes, 0);
    if (dirp == NULL) {
        return -TARGET_EFAULT;
    }
    ret = get_errno(getdents(arg1, (char *)dirp, nbytes));
    if (!is_error(ret)) {
        struct dirent *de;
        int len = ret;
        int reclen;

        de = dirp;
        while (len > 0) {
            reclen = de->d_reclen;
            if (reclen > len) {
                return -TARGET_EFAULT;
            }
            de->d_reclen = tswap16(reclen);
            de->d_fileno = tswap32(de->d_fileno);
            len -= reclen;
        }
    }
    return ret;
}

/* getdirecentries(2) */
static inline abi_long do_freebsd_getdirentries(abi_long arg1, abi_ulong arg2,
        abi_long nbytes, abi_ulong arg4)
{
    abi_long ret;
    struct dirent *dirp;
    long basep;

    dirp = lock_user(VERIFY_WRITE, arg2, nbytes, 0);
    if (dirp == NULL) {
        return -TARGET_EFAULT;
    }
    ret = get_errno(getdirentries(arg1, (char *)dirp, nbytes, &basep));
    if (!is_error(ret)) {
        struct dirent *de;
        int len = ret;
        int reclen;

        de = dirp;
        while (len > 0) {
            reclen = de->d_reclen;
            if (reclen > len) {
                return -TARGET_EFAULT;
            }
            de->d_reclen = tswap16(reclen);
            de->d_fileno = tswap32(de->d_fileno);
            len -= reclen;
            de = (struct dirent *)((void *)de + reclen);
        }
    }
    unlock_user(dirp, arg2, ret);
    if (arg4) {
        if (put_user(basep, arg4, abi_ulong)) {
            return -TARGET_EFAULT;
        }
    }
    return ret;
}

/* fcntl(2) */
static inline abi_long do_freebsd_fcntl(abi_long arg1, abi_long arg2,
        abi_ulong arg3)
{
    abi_long ret;
    int host_cmd;
    struct flock fl;
    struct target_freebsd_flock *target_fl;

    host_cmd = target_to_host_fcntl_cmd(arg2);
    if (host_cmd < 0) {
        return host_cmd;
    }
    switch (arg2) {
    case TARGET_F_GETLK:
        if (!lock_user_struct(VERIFY_READ, target_fl, arg3, 1)) {
            return -TARGET_EFAULT;
        }
        __get_user(fl.l_type, &target_fl->l_type);
        __get_user(fl.l_whence, &target_fl->l_whence);
        __get_user(fl.l_start, &target_fl->l_start);
        __get_user(fl.l_len, &target_fl->l_len);
        __get_user(fl.l_pid, &target_fl->l_pid);
        __get_user(fl.l_sysid, &target_fl->l_sysid);
        unlock_user_struct(target_fl, arg3, 0);
        ret = get_errno(fcntl(arg1, host_cmd, &fl));
        if (!is_error(ret)) {
            if (!lock_user_struct(VERIFY_WRITE, target_fl, arg3, 0)) {
                return -TARGET_EFAULT;
            }
            __put_user(fl.l_type, &target_fl->l_type);
            __put_user(fl.l_whence, &target_fl->l_whence);
            __put_user(fl.l_start, &target_fl->l_start);
            __put_user(fl.l_len, &target_fl->l_len);
            __put_user(fl.l_pid, &target_fl->l_pid);
            __put_user(fl.l_sysid, &target_fl->l_sysid);
            unlock_user_struct(target_fl, arg3, 1);
        }
        break;

    case TARGET_F_SETLK:
    case TARGET_F_SETLKW:
        if (!lock_user_struct(VERIFY_READ, target_fl, arg3, 1)) {
            return -TARGET_EFAULT;
        }
        __get_user(fl.l_type, &target_fl->l_type);
        __get_user(fl.l_whence, &target_fl->l_whence);
        __get_user(fl.l_start, &target_fl->l_start);
        __get_user(fl.l_len, &target_fl->l_len);
        __get_user(fl.l_pid, &target_fl->l_pid);
        __get_user(fl.l_sysid, &target_fl->l_sysid);
        unlock_user_struct(target_fl, arg3, 0);
        ret = get_errno(fcntl(arg1, host_cmd, &fl));
        break;

    case TARGET_F_DUPFD:
    case TARGET_F_DUP2FD:
    case TARGET_F_GETOWN:
    case TARGET_F_SETOWN:
    case TARGET_F_GETFD:
    case TARGET_F_SETFD:
    case TARGET_F_GETFL:
    case TARGET_F_SETFL:
    case TARGET_F_READAHEAD:
    case TARGET_F_RDAHEAD:
    default:
        ret = get_errno(fcntl(arg1, host_cmd, arg3));
        break;
    }
    return ret;
}

#endif /* ! __FREEBSD_STAT_H_ */
