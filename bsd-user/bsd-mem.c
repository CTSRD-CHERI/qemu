/*
 *  memory management system conversion routines
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

#include <sys/ipc.h>
#include <sys/shm.h>

#include "qemu.h"
#include "qemu-bsd.h"

struct bsd_shm_regions bsd_shm_regions[N_BSD_SHM_REGIONS];

abi_ulong bsd_target_brk;
abi_ulong bsd_target_original_brk;

void target_set_brk(abi_ulong new_brk)
{

    bsd_target_original_brk = bsd_target_brk = HOST_PAGE_ALIGN(new_brk);
}

abi_long target_to_host_ipc_perm(struct ipc_perm *host_ip,
        abi_ulong target_addr)
{
    struct target_ipc_perm *target_ip;

    if (!lock_user_struct(VERIFY_READ, target_ip, target_addr, 1)) {
        return -TARGET_EFAULT;
    }
    __get_user(host_ip->cuid, &target_ip->cuid);
    __get_user(host_ip->cgid, &target_ip->cgid);
    __get_user(host_ip->uid, &target_ip->uid);
    __get_user(host_ip->gid, &target_ip->gid);
    __get_user(host_ip->mode, &target_ip->mode);
    __get_user(host_ip->seq, &target_ip->seq);
    __get_user(host_ip->key, &target_ip->key);
    unlock_user_struct(target_ip, target_addr, 0);

    return 0;
}

abi_long host_to_target_ipc_perm(abi_ulong target_addr,
        struct ipc_perm *host_ip)
{
    struct target_ipc_perm *target_ip;

    if (!lock_user_struct(VERIFY_WRITE, target_ip, target_addr, 0)) {
        return -TARGET_EFAULT;
    }
    __put_user(host_ip->cuid, &target_ip->cuid);
    __put_user(host_ip->cgid, &target_ip->cgid);
    __put_user(host_ip->uid, &target_ip->uid);
    __put_user(host_ip->gid, &target_ip->gid);
    __put_user(host_ip->mode, &target_ip->mode);
    __put_user(host_ip->seq, &target_ip->seq);
    __put_user(host_ip->key, &target_ip->key);
    unlock_user_struct(target_ip, target_addr, 1);

    return 0;
}

abi_long target_to_host_shmid_ds(struct shmid_ds *host_sd,
        abi_ulong target_addr)
{
    struct target_shmid_ds *target_sd;

    if (!lock_user_struct(VERIFY_READ, target_sd, target_addr, 1)) {
        return -TARGET_EFAULT;
    }
    if (target_to_host_ipc_perm(&(host_sd->shm_perm), target_addr)) {
        return -TARGET_EFAULT;
    }
    __get_user(host_sd->shm_segsz, &target_sd->shm_segsz);
    __get_user(host_sd->shm_lpid, &target_sd->shm_lpid);
    __get_user(host_sd->shm_cpid, &target_sd->shm_cpid);
    __get_user(host_sd->shm_nattch, &target_sd->shm_nattch);
    __get_user(host_sd->shm_atime, &target_sd->shm_atime);
    __get_user(host_sd->shm_dtime, &target_sd->shm_dtime);
    __get_user(host_sd->shm_ctime, &target_sd->shm_ctime);
    unlock_user_struct(target_sd, target_addr, 0);

    return 0;
}

abi_long host_to_target_shmid_ds(abi_ulong target_addr,
        struct shmid_ds *host_sd)
{
    struct target_shmid_ds *target_sd;

    if (!lock_user_struct(VERIFY_WRITE, target_sd, target_addr, 0)) {
        return -TARGET_EFAULT;
    }
    if (host_to_target_ipc_perm(target_addr, &(host_sd->shm_perm))) {
        return -TARGET_EFAULT;
    }
    __put_user(host_sd->shm_segsz, &target_sd->shm_segsz);
    __put_user(host_sd->shm_lpid, &target_sd->shm_lpid);
    __put_user(host_sd->shm_cpid, &target_sd->shm_cpid);
    __put_user(host_sd->shm_nattch, &target_sd->shm_nattch);
    __put_user(host_sd->shm_atime, &target_sd->shm_atime);
    __put_user(host_sd->shm_dtime, &target_sd->shm_dtime);
    __put_user(host_sd->shm_ctime, &target_sd->shm_ctime);
    unlock_user_struct(target_sd, target_addr, 1);

    return 0;
}

