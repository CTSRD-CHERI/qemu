/*
 * Declarations for cpu physical memory functions
 *
 * Copyright 2011 Red Hat, Inc. and/or its affiliates
 *
 * Authors:
 *  Avi Kivity <avi@redhat.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 2 or
 * later.  See the COPYING file in the top-level directory.
 *
 */

/*
 * This header is for use by exec.c and memory.c ONLY.  Do not include it.
 * The functions declared here will be removed soon.
 */

#ifndef QEMU_EXEC_RAMBLOCK_H
#define QEMU_EXEC_RAMBLOCK_H

#include "cpu-common.h"

struct CheriTagMem; // opaque struct

#ifdef CONFIG_USER_ONLY
/*
 * XXXKW: define TLBENTRYCAP macros for the time Tag Controller implementation
 * bits specific to TLB entries are not separated from SoftMMU implementation.
 */
#define TLBENTRYCAP_MASK (uintptr_t)0x7
    /* Trap if a non-zero tag is read/written. */
#define TLBENTRYCAP_FLAG_TRAP (uintptr_t)0x1
    /* Trap if any tag is read/written */
#define TLBENTRYCAP_FLAG_TRAP_ANY (uintptr_t)0x4
    /* Clear any tag read/written. */
#define TLBENTRYCAP_FLAG_CLEAR (uintptr_t)0x2
    /* This page contains only zero tag bits. */
#define ALL_ZERO_TAGBLK ((void *)(uintptr_t)(~0 & ~TLBENTRYCAP_MASK))

struct RAMBlock {
    /* Instead of mr->size */
    uint64_t size;
    uint8_t *host;

    /* Bitmap of CHERI tag bits */
    struct CheriTagMem *cheri_tags;
};
#else
struct RAMBlock {
    struct rcu_head rcu;
    struct MemoryRegion *mr;
    uint8_t *host;
    uint8_t *colo_cache; /* For colo, VM's ram cache */
    ram_addr_t offset;
    ram_addr_t used_length;
    ram_addr_t max_length;
    void (*resized)(const char*, uint64_t length, void *host);
    uint32_t flags;
    /* Protected by iothread lock.  */
    char idstr[256];
    /* RCU-enabled, writes protected by the ramlist lock */
    QLIST_ENTRY(RAMBlock) next;
    QLIST_HEAD(, RAMBlockNotifier) ramblock_notifiers;
    int fd;
    size_t page_size;
    /* dirty bitmap used during migration */
    unsigned long *bmap;
    /* bitmap of already received pages in postcopy */
    unsigned long *receivedmap;

    /* Bitmap of CHERI tag bits */
    struct CheriTagMem *cheri_tags;

    /*
     * bitmap to track already cleared dirty bitmap.  When the bit is
     * set, it means the corresponding memory chunk needs a log-clear.
     * Set this up to non-NULL to enable the capability to postpone
     * and split clearing of dirty bitmap on the remote node (e.g.,
     * KVM).  The bitmap will be set only when doing global sync.
     *
     * NOTE: this bitmap is different comparing to the other bitmaps
     * in that one bit can represent multiple guest pages (which is
     * decided by the `clear_bmap_shift' variable below).  On
     * destination side, this should always be NULL, and the variable
     * `clear_bmap_shift' is meaningless.
     */
    unsigned long *clear_bmap;
    uint8_t clear_bmap_shift;
};
#endif

#endif
