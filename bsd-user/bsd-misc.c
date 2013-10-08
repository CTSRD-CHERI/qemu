/*
 *  BSD misc system call conversions routines
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
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/uuid.h>

#include "qemu.h"
#include "qemu-bsd.h"

/*
 * BSD uuidgen(2) struct uuid conversion
 */
abi_long host_to_target_uuid(abi_ulong target_addr, struct uuid *host_uuid)
{
    struct target_uuid *target_uuid;

    if (!lock_user_struct(VERIFY_WRITE, target_uuid, target_addr, 0)) {
        return -TARGET_EFAULT;
    }
    __put_user(host_uuid->time_low, &target_uuid->time_low);
    __put_user(host_uuid->time_mid, &target_uuid->time_mid);
    __put_user(host_uuid->time_hi_and_version,
        &target_uuid->time_hi_and_version);
    host_uuid->clock_seq_hi_and_reserved =
        target_uuid->clock_seq_hi_and_reserved;
    host_uuid->clock_seq_low = target_uuid->clock_seq_low;
    memcpy(host_uuid->node, target_uuid->node, TARGET_UUID_NODE_LEN);
    unlock_user_struct(target_uuid, target_addr, 1);
    return 0;
}

abi_long target_to_host_semarray(int semid, unsigned short **host_array,
        abi_ulong target_addr)
{
    abi_long ret;
    int nsems, i;
    unsigned short *array;
    union semun semun;
    struct semid_ds semid_ds;

    semun.buf = &semid_ds;
    ret = semctl(semid, 0, IPC_STAT, semun);
    if (ret == -1) {
        return get_errno(ret);
    }
    nsems = semid_ds.sem_nsems;
    *host_array = (unsigned short *)malloc(nsems * sizeof(unsigned short));
    array = lock_user(VERIFY_READ, target_addr,
        nsems*sizeof(unsigned short), 1);
    if (array == NULL) {
        free(*host_array);
        return -TARGET_EFAULT;
    }
    for (i = 0; i < nsems; i++) {
        (*host_array)[i] = array[i];
    }
    unlock_user(array, target_addr, 0);

    return 0;
}

abi_long host_to_target_semarray(int semid, abi_ulong target_addr,
        unsigned short **host_array)
{
    abi_long ret;
    int nsems, i;
    unsigned short *array;
    union semun semun;
    struct semid_ds semid_ds;

    semun.buf = &semid_ds;

    ret = semctl(semid, 0, IPC_STAT, semun);
    if (ret == -1) {
        free(*host_array);
        return get_errno(ret);
    }

    nsems = semid_ds.sem_nsems;
    array = (unsigned short *)lock_user(VERIFY_WRITE, target_addr,
        nsems*sizeof(unsigned short), 0);
    if (array == NULL) {
        free(*host_array);
        return -TARGET_EFAULT;
    }
    for (i = 0; i < nsems; i++) {
        array[i] = (*host_array)[i];
    }
    free(*host_array);
    unlock_user(array, target_addr, 1);
    return 0;
}

abi_long target_to_host_semid_ds(struct semid_ds *host_sd,
        abi_ulong target_addr)
{
    struct target_semid_ds *target_sd;

    if (!lock_user_struct(VERIFY_READ, target_sd, target_addr, 1)) {
        return -TARGET_EFAULT;
    }
    if (target_to_host_ipc_perm(&(host_sd->sem_perm), (target_addr +
                    offsetof(struct target_semid_ds, sem_perm)))) {
        return -TARGET_EFAULT;
    }
    /* sem_base is not used by kernel for IPC_STAT/IPC_SET */
    /* host_sd->sem_base  = g2h(target_sd->sem_base); */
    host_sd->sem_nsems = tswap16(target_sd->sem_nsems);
    host_sd->sem_otime = tswapal(target_sd->sem_otime);
    host_sd->sem_ctime = tswapal(target_sd->sem_ctime);
    unlock_user_struct(target_sd, target_addr, 0);
    return 0;
}

abi_long host_to_target_semid_ds(abi_ulong target_addr,
        struct semid_ds *host_sd)
{
    struct target_semid_ds *target_sd;

    if (!lock_user_struct(VERIFY_WRITE, target_sd, target_addr, 0)) {
        return -TARGET_EFAULT;
    }
    if (host_to_target_ipc_perm((target_addr +
                    offsetof(struct target_semid_ds, sem_perm)),
                &(host_sd->sem_perm))) {
        return -TARGET_EFAULT;
    }
    /* sem_base is not used by kernel for IPC_STAT/IPC_SET */
    /* target_sd->sem_base = h2g((void *)host_sd->sem_base); */
    target_sd->sem_nsems = tswap16(host_sd->sem_nsems);
    target_sd->sem_otime = tswapal(host_sd->sem_otime);
    target_sd->sem_ctime = tswapal(host_sd->sem_ctime);
    unlock_user_struct(target_sd, target_addr, 1);

    return 0;
}

abi_long target_to_host_msqid_ds(struct msqid_ds *host_md,
        abi_ulong target_addr)
{
    struct target_msqid_ds *target_md;

    if (!lock_user_struct(VERIFY_READ, target_md, target_addr, 1)) {
        return -TARGET_EFAULT;
    }
    if (target_to_host_ipc_perm(&(host_md->msg_perm), target_addr)) {
        return -TARGET_EFAULT;
    }

    memset(host_md, 0, sizeof(struct msqid_ds));
    /* msg_first and msg_last are not used by IPC_SET/IPC_STAT in kernel. */
    host_md->msg_cbytes = tswapal(target_md->msg_cbytes);
    host_md->msg_qnum = tswapal(target_md->msg_qnum);
    host_md->msg_qbytes = tswapal(target_md->msg_qbytes);
    host_md->msg_lspid = tswapal(target_md->msg_lspid);
    host_md->msg_lrpid = tswapal(target_md->msg_lrpid);
    host_md->msg_stime = tswapal(target_md->msg_stime);
    host_md->msg_rtime = tswapal(target_md->msg_rtime);
    host_md->msg_ctime = tswapal(target_md->msg_ctime);
    unlock_user_struct(target_md, target_addr, 0);

    return 0;
}

abi_long host_to_target_msqid_ds(abi_ulong target_addr,
        struct msqid_ds *host_md)
{
    struct target_msqid_ds *target_md;

    if (!lock_user_struct(VERIFY_WRITE, target_md, target_addr, 0)) {
        return -TARGET_EFAULT;
    }
    if (host_to_target_ipc_perm(target_addr, &(host_md->msg_perm))) {
        return -TARGET_EFAULT;
    }

    memset(target_md, 0, sizeof(struct target_msqid_ds));
    /* msg_first and msg_last are not used by IPC_SET/IPC_STAT in kernel. */
    target_md->msg_cbytes = tswapal(host_md->msg_cbytes);
    target_md->msg_qnum = tswapal(host_md->msg_qnum);
    target_md->msg_qbytes = tswapal(host_md->msg_qbytes);
    target_md->msg_lspid = tswapal(host_md->msg_lspid);
    target_md->msg_lrpid = tswapal(host_md->msg_lrpid);
    target_md->msg_stime = tswapal(host_md->msg_stime);
    target_md->msg_rtime = tswapal(host_md->msg_rtime);
    target_md->msg_ctime = tswapal(host_md->msg_ctime);
    unlock_user_struct(target_md, target_addr, 1);

    return 0;
}

