/*
 *  miscellaneous BSD system call shims
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

#ifndef __BSD_MISC_H_
#define __BSD_MISC_H_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/uuid.h>

#include "qemu-bsd.h"

/* quotactl(2) */
static inline abi_long do_bsd_quotactl(abi_ulong path, abi_long cmd,
        abi_ulong target_addr)
{

    qemu_log("qemu: Unsupported syscall quotactl()\n");
    return -TARGET_ENOSYS;
}

/* reboot(2) */
static inline abi_long do_bsd_reboot(abi_long how)
{

    qemu_log("qemu: Unsupported syscall reboot()\n");
    return -TARGET_ENOSYS;
}

/* uuidgen(2) */
static inline abi_long do_bsd_uuidgen(abi_ulong target_addr, int count)
{
    int i;
    abi_long ret;
    struct uuid *host_uuid;

    if (count < 1 || count > 2048) {
        return -TARGET_EINVAL;
    }

    host_uuid = (struct uuid *)g_malloc(count * sizeof(struct uuid));

    if (host_uuid == NULL) {
        return -TARGET_ENOMEM;
    }

    ret = get_errno(uuidgen(host_uuid, count));
    if (is_error(ret)) {
        goto out;
    }
    for (i = 0; i < count; i++) {
        ret = host_to_target_uuid(target_addr +
            (abi_ulong)(sizeof(struct target_uuid) * i), &host_uuid[i]);
        if (is_error(ret)) {
            goto out;
        }
    }

out:
    g_free(host_uuid);
    return ret;
}


/*
 * System V Semaphores
 */

/* semget(2) */
static inline abi_long do_bsd_semget(abi_long key, int nsems,
        int target_flags)
{

    return get_errno(semget(key, nsems,
                target_to_host_bitmask(target_flags, ipc_flags_tbl)));
}

/* semop(2) */
static inline abi_long do_bsd_semop(int semid, abi_long ptr, unsigned nsops)
{
    struct sembuf sops[nsops];
    struct target_sembuf *target_sembuf;
    int i;

    target_sembuf = lock_user(VERIFY_READ, ptr,
            nsops * sizeof(struct target_sembuf), 1);
    if (target_sembuf == NULL) {
        return -TARGET_EFAULT;
    }
    for (i = 0; i < nsops; i++) {
        __get_user(sops[i].sem_num, &target_sembuf[i].sem_num);
        __get_user(sops[i].sem_op, &target_sembuf[i].sem_op);
        __get_user(sops[i].sem_flg, &target_sembuf[i].sem_flg);
    }
    unlock_user(target_sembuf, ptr, 0);

    return semop(semid, sops, nsops);
}

/* __semctl(2) */
static inline abi_long do_bsd___semctl(int semid, int semnum, int target_cmd,
        union target_semun target_su)
{
    union semun arg;
    struct semid_ds dsarg;
    unsigned short *array = NULL;
    int host_cmd;
    abi_long ret = 0;
    abi_long err;
    abi_ulong target_addr;

    switch (target_cmd) {
    case TARGET_GETVAL:
        host_cmd = GETVAL;
        break;

    case TARGET_SETVAL:
        host_cmd = SETVAL;
        break;

    case TARGET_GETALL:
        host_cmd = GETALL;
        break;

    case TARGET_SETALL:
        host_cmd = SETALL;
        break;

    case TARGET_IPC_STAT:
        host_cmd = IPC_STAT;
        break;

    case TARGET_IPC_SET:
        host_cmd = IPC_SET;
        break;

    case TARGET_IPC_RMID:
        host_cmd = IPC_RMID;
        break;

    case TARGET_GETPID:
        host_cmd = GETPID;
        break;

    case TARGET_GETNCNT:
        host_cmd = GETNCNT;
        break;

    case TARGET_GETZCNT:
        host_cmd = GETZCNT;
        break;

    default:
        return -TARGET_EINVAL;
    }

    switch (host_cmd) {
    case GETVAL:
    case SETVAL:
        arg.val = tswap32(target_su.val);
        ret = get_errno(semctl(semid, semnum, host_cmd, arg));
        target_su.val = tswap32(arg.val);
        break;

    case GETALL:
    case SETALL:
        if (get_user_ual(target_addr, (abi_ulong)target_su.array)) {
            return -TARGET_EFAULT;
        }
        err = target_to_host_semarray(semid, &array, target_addr);
        if (is_error(err)) {
            return err;
        }
        arg.array = array;
        ret = get_errno(semctl(semid, semnum, host_cmd, arg));
        err = host_to_target_semarray(semid, target_addr, &array);
        if (is_error(err)) {
            return err;
        }
        break;

    case IPC_STAT:
    case IPC_SET:
        if (get_user_ual(target_addr, (abi_ulong)target_su.buf)) {
            return -TARGET_EFAULT;
        }
        err = target_to_host_semid_ds(&dsarg, target_addr);
        if (is_error(err)) {
            return err;
        }
        arg.buf = &dsarg;
        ret = get_errno(semctl(semid, semnum, host_cmd, arg));
        err = host_to_target_semid_ds(target_addr, &dsarg);
        if (is_error(err)) {
            return err;
        }
        break;

    case IPC_RMID:
    case GETPID:
    case GETNCNT:
    case GETZCNT:
        ret = get_errno(semctl(semid, semnum, host_cmd, NULL));
        break;

    default:
        ret = -TARGET_EINVAL;
        break;
    }
    return ret;
}

/* msgctl(2) */
static inline abi_long do_bsd_msgctl(int msgid, int target_cmd, abi_long ptr)
{
    struct msqid_ds dsarg;
    abi_long ret = -TARGET_EINVAL;
    int host_cmd;

    switch (target_cmd) {
    case TARGET_IPC_STAT:
        host_cmd = IPC_STAT;
        break;

    case TARGET_IPC_SET:
        host_cmd = IPC_SET;
        break;

    case TARGET_IPC_RMID:
        host_cmd = IPC_RMID;
        break;

    default:
        return -TARGET_EINVAL;
    }

    switch (host_cmd) {
    case IPC_STAT:
    case IPC_SET:
        if (target_to_host_msqid_ds(&dsarg, ptr)) {
            return -TARGET_EFAULT;
        }
        ret = get_errno(msgctl(msgid, host_cmd, &dsarg));
        if (host_to_target_msqid_ds(ptr, &dsarg)) {
            return -TARGET_EFAULT;
        }
        break;

    case IPC_RMID:
        ret = get_errno(msgctl(msgid, host_cmd, NULL));
        break;

    default:
        ret = -TARGET_EINVAL;
        break;
    }
    return ret;
}

struct kern_mymsg {
    long mtype;
    char mtext[1];
};

/* msgsnd(2) */
static inline abi_long do_bsd_msgsnd(int msqid, abi_long msgp,
        unsigned int msgsz, int msgflg)
{
    struct target_msgbuf *target_mb;
    struct kern_mymsg *host_mb;
    abi_long ret;

    if (!lock_user_struct(VERIFY_READ, target_mb, msgp, 0)) {
        return -TARGET_EFAULT;
    }
    host_mb = g_malloc(msgsz+sizeof(long));
    host_mb->mtype = (abi_long) tswapal(target_mb->mtype);
    memcpy(host_mb->mtext, target_mb->mtext, msgsz);
    ret = get_errno(msgsnd(msqid, host_mb, msgsz, msgflg));
    g_free(host_mb);
    unlock_user_struct(target_mb, msgp, 0);

    return ret;
}

/* msgrcv(2) */
static inline abi_long do_bsd_msgrcv(int msqid, abi_long msgp,
        unsigned int msgsz, abi_long msgtyp, int msgflg)
{
    struct target_msgbuf *target_mb = NULL;
    char *target_mtext;
    struct kern_mymsg *host_mb;
    abi_long ret = 0;

    if (!lock_user_struct(VERIFY_WRITE, target_mb, msgp, 0)) {
        return -TARGET_EFAULT;
    }
    host_mb = g_malloc(msgsz+sizeof(long));
    ret = get_errno(msgrcv(msqid, host_mb, msgsz, tswapal(msgtyp), msgflg));
    if (ret > 0) {
        abi_ulong target_mtext_addr = msgp + sizeof(abi_ulong);
        target_mtext = lock_user(VERIFY_WRITE, target_mtext_addr, ret, 0);
        if (target_mtext == NULL) {
            ret = -TARGET_EFAULT;
            goto end;
        }
        memcpy(target_mb->mtext, host_mb->mtext, ret);
        unlock_user(target_mtext, target_mtext_addr, ret);
    }
    target_mb->mtype = tswapal(host_mb->mtype);
end:
    if (target_mb != NULL) {
        unlock_user_struct(target_mb, msgp, 1);
    }
    g_free(host_mb);
    return ret;
}

/* getdtablesize(2) */
static inline abi_long do_bsd_getdtablesize(void)
{

    return get_errno(getdtablesize());
}

#endif /* ! __BSD_MISC_H_ */
