/*
 *  FreeBSD extend attributes and ACL conversions
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
#ifndef _ACL_PRIVATE
#define _ACL_PRIVATE
#endif
#include <sys/acl.h>

#include "qemu.h"
#include "qemu-os.h"

/*
 * FreeBSD ACL conversion.
 */
abi_long t2h_freebsd_acl(struct acl *host_acl, abi_ulong target_addr)
{
    uint32_t i;
    struct target_freebsd_acl *target_acl;

    if (!lock_user_struct(VERIFY_READ, target_acl, target_addr, 1)) {
        return -TARGET_EFAULT;
    }
    __get_user(host_acl->acl_maxcnt, &target_acl->acl_maxcnt);
    __get_user(host_acl->acl_cnt, &target_acl->acl_cnt);

    for (i = 0; i < host_acl->acl_maxcnt; i++) {
        __get_user(host_acl->acl_entry[i].ae_tag,
            &target_acl->acl_entry[i].ae_tag);
        __get_user(host_acl->acl_entry[i].ae_id,
            &target_acl->acl_entry[i].ae_id);
        __get_user(host_acl->acl_entry[i].ae_perm,
            &target_acl->acl_entry[i].ae_perm);
        __get_user(host_acl->acl_entry[i].ae_entry_type,
            &target_acl->acl_entry[i].ae_entry_type);
        __get_user(host_acl->acl_entry[i].ae_flags,
            &target_acl->acl_entry[i].ae_flags);
    }

    unlock_user_struct(target_acl, target_addr, 0);
    return 0;
}

abi_long h2t_freebsd_acl(abi_ulong target_addr, struct acl *host_acl)
{
    uint32_t i;
    struct target_freebsd_acl *target_acl;

    if (!lock_user_struct(VERIFY_WRITE, target_acl, target_addr, 0)) {
        return -TARGET_EFAULT;
    }

    __put_user(host_acl->acl_maxcnt, &target_acl->acl_maxcnt);
    __put_user(host_acl->acl_cnt, &target_acl->acl_cnt);

    for (i = 0; i < host_acl->acl_maxcnt; i++) {
        __put_user(host_acl->acl_entry[i].ae_tag,
            &target_acl->acl_entry[i].ae_tag);
        __put_user(host_acl->acl_entry[i].ae_id,
            &target_acl->acl_entry[i].ae_id);
        __put_user(host_acl->acl_entry[i].ae_perm,
            &target_acl->acl_entry[i].ae_perm);
        __put_user(host_acl->acl_entry[i].ae_entry_type,
            &target_acl->acl_entry[i].ae_entry_type);
        __put_user(host_acl->acl_entry[i].ae_flags,
            &target_acl->acl_entry[i].ae_flags);
    }

    unlock_user_struct(target_acl, target_addr, 1);
    return 0;
}

abi_long t2h_freebsd_acl_type(acl_type_t *host_type, abi_long target_type)
{

    switch (target_type) {
    case TARGET_FREEBSD_ACL_TYPE_ACCESS_OLD:
        *host_type = ACL_TYPE_ACCESS_OLD;
        break;

    case TARGET_FREEBSD_ACL_TYPE_DEFAULT_OLD:
        *host_type = ACL_TYPE_DEFAULT_OLD;
        break;

    case TARGET_FREEBSD_ACL_TYPE_ACCESS:
        *host_type = ACL_TYPE_ACCESS;
        break;

    case TARGET_FREEBSD_ACL_TYPE_DEFAULT:
        *host_type = ACL_TYPE_ACCESS;
        break;

    case TARGET_FREEBSD_ACL_TYPE_NFS4:
        *host_type = ACL_TYPE_NFS4;
        break;

    default:
        return -TARGET_EINVAL;
    }
    return 0;
}

