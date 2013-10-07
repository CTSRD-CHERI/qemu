/*
 *  memory management system call shims and definitions
 *
 *  Copyright (c) 2013-15 Stacey D. Son
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

/*--
 * Copyright (c) 1982, 1986, 1993
 *      The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef _BSD_MMAN_H_
#define _BSD_MMAN_H_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <fcntl.h>

#include "qemu-bsd.h"

extern struct bsd_shm_regions bsd_shm_regions[];
extern abi_ulong bsd_target_brk;
extern abi_ulong bsd_target_original_brk;

/* mmap(2) */
static inline abi_long do_bsd_mmap(void *cpu_env, abi_long arg1, abi_long arg2,
    abi_long arg3, abi_long arg4, abi_long arg5, abi_long arg6, abi_long arg7,
    abi_long arg8)
{

    if (regpairs_aligned(cpu_env) != 0) {
       arg6 = arg7;
       arg7 = arg8;
    }
    return get_errno(target_mmap(arg1, arg2, arg3,
                target_to_host_bitmask(arg4, mmap_flags_tbl), arg5,
		target_offset64(arg6, arg7)));
}

/* munmap(2) */
static inline abi_long do_bsd_munmap(abi_long arg1, abi_long arg2)
{

    return get_errno(target_munmap(arg1, arg2));
}

/* mprotect(2) */
static inline abi_long do_bsd_mprotect(abi_long arg1, abi_long arg2,
        abi_long arg3)
{

    return get_errno(target_mprotect(arg1, arg2, arg3));
}

/* msync(2) */
static inline abi_long do_bsd_msync(abi_long arg1, abi_long arg2, abi_long arg3)
{

    return get_errno(msync(g2h(arg1), arg2, arg3));
}

/* mlock(2) */
static inline abi_long do_bsd_mlock(abi_long arg1, abi_long arg2)
{

    return get_errno(mlock(g2h(arg1), arg2));
}

/* munlock(2) */
static inline abi_long do_bsd_munlock(abi_long arg1, abi_long arg2)
{

    return get_errno(munlock(g2h(arg1), arg2));
}

/* mlockall(2) */
static inline abi_long do_bsd_mlockall(abi_long arg1)
{

    return get_errno(mlockall(arg1));
}

/* munlockall(2) */
static inline abi_long do_bsd_munlockall(void)
{

    return get_errno(munlockall());
}

/* madvise(2) */
static inline abi_long do_bsd_madvise(abi_long arg1, abi_long arg2,
        abi_long arg3)
{
    /*
     * A straight passthrough may not be safe because qemu sometimes
     * turns private file-backed mapping into anonymous mappings. This
     * will break MADV_DONTNEED.  This is a hint, so ignoring and returing
     * success is ok.
     */
    return get_errno(0);
}

/* minherit(2) */
static inline abi_long do_bsd_minherit(abi_long addr, abi_long len,
        abi_long inherit)
{

    return get_errno(minherit(g2h(addr), len, inherit));
}

/* mincore(2) */
static inline abi_long do_bsd_mincore(abi_ulong target_addr, abi_ulong len,
        abi_ulong target_vec)
{
    abi_long ret;
    void *p, *a;

    a = lock_user(VERIFY_WRITE, target_addr, len, 0);
    if (a == NULL) {
        return -TARGET_EFAULT;
    }
    p = lock_user_string(target_vec);
    if (p == NULL) {
        unlock_user(a, target_addr, 0);
        return -TARGET_EFAULT;
    }
    ret = get_errno(mincore(a, len, p));
    unlock_user(p, target_vec, ret);
    unlock_user(a, target_addr, 0);

    return ret;
}

/* break() XXX this needs some more work. */
static inline abi_long do_obreak(abi_ulong new_brk)
{
    abi_ulong brk_page;
    abi_long mapped_addr;
    int new_alloc_size;

    return -TARGET_EINVAL;

    if (!new_brk) {
        return 0;
    }
    if (new_brk < bsd_target_original_brk) {
        return -TARGET_EINVAL;
    }

    brk_page = HOST_PAGE_ALIGN(bsd_target_brk);

    /* If the new brk is less than this, set it and we're done... */
    if (new_brk < brk_page) {
        bsd_target_brk = new_brk;
        return 0;
    }

    /* We need to allocate more memory after the brk... */
    new_alloc_size = HOST_PAGE_ALIGN(new_brk - brk_page + 1);
    mapped_addr = get_errno(target_mmap(brk_page, new_alloc_size,
                PROT_READ|PROT_WRITE, MAP_ANON|MAP_FIXED|MAP_PRIVATE, -1, 0));

    if (!is_error(mapped_addr)) {
        bsd_target_brk = new_brk;
    } else {
        return mapped_addr;
    }

    return 0;
}

/* shm_open(2) */
static inline abi_long do_bsd_shm_open(abi_ulong arg1, abi_long arg2,
        abi_long arg3)
{
    int ret;
    void *p;

    p = lock_user_string(arg1);
    if (p == NULL) {
        return -TARGET_EFAULT;
    }
    ret = get_errno(shm_open(path(p),
                target_to_host_bitmask(arg2, fcntl_flags_tbl), arg3));
    unlock_user(p, arg1, 0);

    return ret;
}

/* shm_unlink(2) */
static inline abi_long do_bsd_shm_unlink(abi_ulong arg1)
{
    int ret;
    void *p;

    p = lock_user_string(arg1);
    if (p == NULL) {
        return -TARGET_EFAULT;
    }
    ret = get_errno(shm_unlink(p)); /* XXX path(p)? */
    unlock_user(p, arg1, 0);

    return ret;
}

/* shmget(2) */
static inline abi_long do_bsd_shmget(abi_long arg1, abi_ulong arg2,
        abi_long arg3)
{

    return get_errno(shmget(arg1, arg2, arg3));
}

/* shmctl(2) */
static inline abi_long do_bsd_shmctl(abi_long shmid, abi_long cmd,
        abi_ulong buff)
{
    struct shmid_ds dsarg;
    abi_long ret = -TARGET_EINVAL;

    cmd &= 0xff;

    switch (cmd) {
    case IPC_STAT:
    case IPC_SET:
        if (target_to_host_shmid_ds(&dsarg, buff)) {
            return -TARGET_EFAULT;
        }
        ret = get_errno(shmctl(shmid, cmd, &dsarg));
        if (host_to_target_shmid_ds(buff, &dsarg)) {
            return -TARGET_EFAULT;
        }
        break;

    case IPC_RMID:
        ret = get_errno(shmctl(shmid, cmd, NULL));
        break;

    default:
        ret = -TARGET_EINVAL;
        break;
    }

    return ret;
}

/* shmat(2) */
static inline abi_long do_bsd_shmat(int shmid, abi_ulong shmaddr, int shmflg)
{
    abi_ulong raddr;
    abi_long ret;
    void *host_raddr;
    struct shmid_ds shm_info;
    int i;

    /* Find out the length of the shared memory segment. */
    ret = get_errno(shmctl(shmid, IPC_STAT, &shm_info));
    if (is_error(ret)) {
        /* Can't get the length */
        return ret;
    }

    mmap_lock();

    if (shmaddr) {
        host_raddr = shmat(shmid, (void *)g2h(shmaddr), shmflg);
    } else {
        abi_ulong mmap_start;

        mmap_start = mmap_find_vma(0, shm_info.shm_segsz);

        if (mmap_start == -1) {
            errno = ENOMEM;
            host_raddr = (void *)-1;
        } else {
            host_raddr = shmat(shmid, g2h(mmap_start),
                shmflg /* | SHM_REMAP */);
        }
    }

    if (host_raddr == (void *)-1) {
        mmap_unlock();
        return get_errno((long)host_raddr);
    }
    raddr = h2g((unsigned long)host_raddr);

    page_set_flags(raddr, raddr + shm_info.shm_segsz,
        PAGE_VALID | PAGE_READ | ((shmflg & SHM_RDONLY) ? 0 : PAGE_WRITE));

    for (i = 0; i < N_BSD_SHM_REGIONS; i++) {
        if (bsd_shm_regions[i].start == 0) {
            bsd_shm_regions[i].start = raddr;
            bsd_shm_regions[i].size = shm_info.shm_segsz;
            break;
        }
    }

    mmap_unlock();
    return raddr;
}

/* shmdt(2) */
static inline abi_long do_bsd_shmdt(abi_ulong shmaddr)
{
    int i;

    for (i = 0; i < N_BSD_SHM_REGIONS; ++i) {
        if (bsd_shm_regions[i].start == shmaddr) {
            bsd_shm_regions[i].start = 0;
            page_set_flags(shmaddr,
                shmaddr + bsd_shm_regions[i].size, 0);
            break;
        }
    }

    return get_errno(shmdt(g2h(shmaddr)));
}


static inline abi_long do_bsd_vadvise(void)
{
    /* See sys_ovadvise() in vm_unix.c */
    qemu_log("qemu: Unsupported syscall vadvise()\n");
    return -TARGET_ENOSYS;
}

static inline abi_long do_bsd_sbrk(void)
{
    /* see sys_sbrk() in vm_mmap.c */
    qemu_log("qemu: Unsupported syscall sbrk()\n");
    return -TARGET_ENOSYS;
}

static inline abi_long do_bsd_sstk(void)
{
    /* see sys_sstk() in vm_mmap.c */
    qemu_log("qemu: Unsupported syscall sstk()\n");
    return -TARGET_ENOSYS;
}

/*
 * undocumented freebsd6_mmap(caddr_t addr, size_t len, int prot, int
 * flags, int fd, int pad, off_t pos) system call.
 */
static inline abi_long do_bsd_freebsd6_mmap(abi_long arg1, abi_long arg2,
        abi_long arg3, abi_long arg4, abi_long arg5, abi_long arg6,
        abi_long arg7)
{

#if TARGET_LONG_BITS == 32
    qemu_log("qemu: Unsupported syscall freebsd6_mmap()\n");
    return -TARGET_ENOSYS;
#else
    /* XXX the pad argument is ignored */
    return get_errno(target_mmap(arg1, arg2, arg3,
                target_to_host_bitmask(arg4, mmap_flags_tbl), arg5, arg7));
#endif
}

#endif /* !_BSD_MMAN_H_ */
