/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2015-2016 Stacey Son <sson@FreeBSD.org>
 * Copyright (c) 2016-2018 Alfredo Mazzinghi <am2419@cl.cam.ac.uk>
 * Copyright (c) 2016-2018 Alex Richardson <Alexander.Richardson@cl.cam.ac.uk>
 * All rights reserved.
 *
 * This software was developed by SRI International and the University of
 * Cambridge Computer Laboratory under DARPA/AFRL contract FA8750-10-C-0237
 * ("CTSRD"), as part of the DARPA CRASH research programme.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#include "cheri_tagmem.h"
#include "exec/exec-all.h"
#include "exec/log.h"
#include "exec/ramblock.h"
#include "cheri_tagmem.h"
#include "cheri-helper-utils.h"
// XXX: use hbitmap? Or a different data structure?
#include "qemu/bitmap.h"
#include "glib/ghash.h"

#if defined(TARGET_MIPS)
#include "cheri_utils.h"
#include "internal.h"
#endif

#include "cheri-archspecific.h"

#if !defined(TARGET_CHERI)
#error "Should only be included for TARGET_CHERI"
#endif

// XXX: use secure/target_tlb_bit0/target_tlb_bit1 for cheri TLB permissions?

/*
 * Tagged Memory Emulation
 *
 * CHERI requires a 1-bit tag for every capability-aligned,
 * capability-sized word in physical memory.  This allows capabilities
 * to be safely loaded and stored in meory without loss of integrity.
 *
 * For emulation purposes the tag is stored in a two-level array containing
 * fixed size bitmaps. To reduce the amount of memory needed the tag flag array
 * is allocated sparsely, 4K tags at at time, and on demand.
 * This 4K number is arbitary and depending on the workload other sizes may be
 * better.
 *
 * Note: We also support an mode where we use one byte per tag. This makes it
 * easy to set or unset a tag without the need of locking or atomics.
 * This requires eight times the memory.
 *
 * As tag accesses are not atomic with regard to data writes/reads spurious
 * invalid capabilities could be created in a threaded context.
 * Therefore, we don't use atomic bitwise RMW operations and the one byte per
 * tag variant actually performs slightly worse due to increased memory usage.
 *
 * XXX Should consider adding a reference count per tag block so that
 * blocks can be deallocated when no longer used maybe.
 *
 * FIXME: rewrite using somethign more like the upcoming MTE changes (https://github.com/rth7680/qemu/commits/tgt-arm-mte-user)
 */

// Report if spin locks are being held too long.
//#define INSTRUMENT_SPIN_LOCKS
#define INSTRUMENT_SPIN_REPORT 10000

#if defined(CHERI_MAGIC128) || defined(CHERI_128)
#define CAP_TAG_SHFT        4           // 5 for 256-bit caps, 4 for 128-bit
#else /* !(CHERI_MAGIC128 || CHERI_128) */
#define CAP_TAG_SHFT        5           // 5 for 256-bit caps, 4 for 128-bit
#endif /* !(CHERI_MAGIC128 || CHERI_128) */
#define CAP_SIZE            (1 << CAP_TAG_SHFT)
#define CAP_MASK            ((1 << CAP_TAG_SHFT) - 1)
#define CAP_TAGBLK_SHFT     12          // 2^12 or 4096 tags per block
#define CAP_TAGBLK_MSK      ((1 << CAP_TAGBLK_SHFT) - 1)
#define CAP_TAGBLK_SIZE       (1 << CAP_TAGBLK_SHFT)
#define CAP_TAGBLK_IDX(tag_idx) ((tag_idx) & CAP_TAGBLK_MSK)
#define TAGS_PER_PAGE        (TARGET_PAGE_SIZE / CHERI_CAP_SIZE)
#ifdef CHERI_MAGIC128
// "Magic" table to store the additional 128 metadata bits for the magic128
// configuration of QEMU
static GHashTable *magic128_table;
struct Magic128Data {
    uint64_t tps;    // type, permissions, sealed
    uint64_t length; // length of capability
};
#endif

#ifndef CAP_TAG_GET_MANY_SHFT
#error "Define a CAP_TAG_GET_MANY_SHFT in appropriate cheri-archspecific.h"
#endif

_Static_assert(CAP_TAG_GET_MANY_SHFT <= 3, "");

// Pack/unpack to make get/set multiple tags atomic (assuming the host word size
// is large enough) lock_tags are indexed as bytes, and so are always little
// endian. Bitmaps are indexed as longs, and so are host endian.
#if (CAP_TAG_GET_MANY_SHFT == 3)
#define byte_unpack_he byte_unpack_64
#define byte_pack_he byte_pack_64
#ifdef HOST_WORDS_BIGENDIAN
#define byte_unpack_le byte_unpack_swap_64
#define byte_pack_le byte_pack_swap_64
#else
#define byte_unpack_le byte_unpack_64
#define byte_pack_le byte_pack_64
#endif
#define packed_t uint64_t
#else
#define byte_unpack_he byte_unpack_32
#define byte_pack_he byte_pack_32
#ifdef HOST_WORDS_BIGENDIAN
#define byte_unpack_le byte_unpack_swap_32
#define byte_pack_le byte_pack_swap_32
#else
#define byte_unpack_le byte_unpack_32
#define byte_pack_le byte_pack_32
#endif
#define packed_t uint32_t
#endif

#define CAP_TAG_GET_MANY_MASK ((1 << (1ULL << CAP_TAG_GET_MANY_SHFT)) - 1ULL)

static inline size_t num_tagblocks(RAMBlock* ram)
{
    uint64_t memory_size = memory_region_size(ram->mr);
    size_t result = DIV_ROUND_UP(memory_size, CHERI_CAP_SIZE * CAP_TAGBLK_SIZE);
    assert(result == (memory_size >> CAP_TAG_SHFT) >> CAP_TAGBLK_SHFT);
    return result;
}

// Use one bit per tag (otherwise a byte):
#define TAGMEM_USE_BITMAP 1
#define TAGMEM_BITMAP_SCALE (TAGMEM_USE_BITMAP ? 1 : 8)

_Static_assert(CAP_TAG_GET_MANY_SHFT < 3 || sizeof(long) >= 8 ||
                   TAGMEM_USE_BITMAP,
               "Otherwise settags cannot be atomic");

// A CHERI tag with a readers-writer lock.
// Write preferring as if one core is spinning on a read waiting for an update,
// it is preferable for the writer to take priority.
// This lock should never be held for more than the duration of an instruction.
typedef struct lock_tag {
    uint8_t as_int;
} lock_tag;

#define LOCKTAG_MASK_TAG (1 << 0)
#define LOCKTAG_MASK_WRITE_LOCKED (1 << 1)
#define LOCKTAG_MASK_WRITE_WAITING (1 << 2)
#define LOCKTAG_MASK_READERS (0b11111 < 3)
#define LOCKTAG_MASK_READER_INC (1 << 3)

#ifdef INSTRUMENT_SPIN_LOCKS

uint32_t read_spins_max = 0;
uint32_t write_spins_max = 0;

static void spins_report(const char *info, uint32_t new, uint32_t *max_ptr)
{
    uint32_t max = *max_ptr;
    if (new > max) {
        qatomic_cmpxchg(max_ptr, max, new);
        printf("New maximum spins for %s: %d\n", info, new);
    }
    if (new > INSTRUMENT_SPIN_REPORT) {
        printf("Large number of spins for %s: %d\n", info, new);
    }
}

#else

#define spins_report(...)

#endif

// Acquire / release. All acquires (read and write) return the tag.
// read acquire and read release can optionally set the tag.

static bool lock_tag_read_tag_and_acquire_lock(lock_tag *lock)
{
    lock_tag old = *lock;

    int32_t spins = 0;
    do {
        if (!(old.as_int &
              (LOCKTAG_MASK_WRITE_WAITING | LOCKTAG_MASK_WRITE_LOCKED))) {
            lock_tag new = old;
            // (TODO: a static assert that we don't have more than 31 host
            // threads) Overflow precluded by number of threads
            new.as_int += LOCKTAG_MASK_READER_INC;
            lock_tag got = {.as_int = qatomic_cmpxchg(&lock->as_int, old.as_int,
                                                      new.as_int)};
            if (got.as_int == old.as_int)
                break;
            old = got;
        } else {
            old = *lock;
        }
        spins++;
    } while (true);
    spins_report("read acquire", spins, &read_spins_max);
    return old.as_int & LOCKTAG_MASK_TAG;
}

static bool lock_tag_release_read(lock_tag *lock)
{
    lock_tag tag = {
        .as_int = qatomic_fetch_sub(&lock->as_int, LOCKTAG_MASK_READER_INC)};
    return tag.as_int & LOCKTAG_MASK_TAG;
}

static bool lock_tag_write_tag_and_acquire_lock_impl(lock_tag *lock,
                                                     bool set_tag, bool tag)
{
    lock_tag old = *lock;
    uint32_t spins = 0;
    do {
        uint8_t read_and_wait =
            LOCKTAG_MASK_READERS | LOCKTAG_MASK_WRITE_WAITING;
        if ((old.as_int & LOCKTAG_MASK_WRITE_LOCKED) ||
            ((old.as_int & read_and_wait) == read_and_wait)) {
            old = *lock;
        } else {
            lock_tag new = old;
            if (old.as_int & LOCKTAG_MASK_READERS) {
                new.as_int |= LOCKTAG_MASK_WRITE_WAITING;
            } else {
                new.as_int |= LOCKTAG_MASK_WRITE_LOCKED;
                if (set_tag)
                    new.as_int = (new.as_int & ~LOCKTAG_MASK_TAG) | tag;
            }
            lock_tag got = {.as_int = qatomic_cmpxchg(&lock->as_int, old.as_int,
                                                      new.as_int)};
            if ((got.as_int == old.as_int) &&
                !(old.as_int & LOCKTAG_MASK_READERS))
                break;
            old = got;
        }
        spins++;
    } while (true);
    spins_report("write acquire", spins, &read_spins_max);
    return old.as_int & LOCKTAG_MASK_TAG;
}

static bool lock_tag_write_tag_and_acquire_lock(lock_tag *lock, bool tag)
{
    return lock_tag_write_tag_and_acquire_lock_impl(lock, true, tag);
}

static bool lock_tag_write_acquire_lock(lock_tag *lock)
{
    return lock_tag_write_tag_and_acquire_lock_impl(lock, false, false);
}

static void lock_tag_release_write(lock_tag *lock)
{
    qatomic_fetch_and(&lock->as_int, LOCKTAG_MASK_TAG);
}

static void lock_tag_write_tag_and_release(lock_tag *lock, bool tag)
{
    qatomic_set(&lock->as_int, tag);
}

// These two don't take locks outs, so use with caution
static bool lock_tag_read(lock_tag *lock)
{
#ifdef CONFIG_DEBUG_TCG
    assert(!parallel_cpus ||
           (lock->as_int & (LOCKTAG_MASK_READERS | LOCKTAG_MASK_WRITE_LOCKED)));
#endif
    return lock->as_int & LOCKTAG_MASK_TAG;
}

static bool lock_tag_write(lock_tag *lock, bool tag, bool check_locked)
{
#ifdef CONFIG_DEBUG_TCG
    if (check_locked)
        assert(!parallel_cpus || (lock->as_int & LOCKTAG_MASK_WRITE_LOCKED));
#endif
    bool old = lock->as_int & LOCKTAG_MASK_TAG;
    lock->as_int = (lock->as_int & ~LOCKTAG_MASK_TAG) | tag;
    return old;
}

typedef struct CheriTagBlock {
    // It would be silly to use locks for non-mttcg. So support both formats and
    // Use one or the other depending on qemu_tcg_mttcg_enabled()
    // It looks like single stepping can be turned on/off, no probably best
    // not to use parallel_cpus.
    union {
        DECLARE_BITMAP(tag_bitmap, CAP_TAGBLK_SIZE *TAGMEM_BITMAP_SCALE);
        lock_tag locked_tags[CAP_TAGBLK_SIZE];
    };
} CheriTagBlock;

#define ALL_ZERO_TAGBLK ((void*)(uintptr_t)1)

static CheriTagBlock *cheri_tag_new_tagblk(RAMBlock *ram, uint64_t tagidx)
{
    CheriTagBlock *tagblk, *old;

    size_t size = qemu_tcg_mttcg_enabled()
                      ? sizeof(((CheriTagBlock *)0)->locked_tags)
                      : sizeof(((CheriTagBlock *)0)->tag_bitmap);

    tagblk = g_malloc0(size);
    if (tagblk == NULL) {
        error_report("Can't allocate tag block.");
        exit(1);
    }

    CheriTagBlock **tagmem = (CheriTagBlock **)ram->cheri_tags;
    size_t tagblock_index = (tagidx >> CAP_TAGBLK_SHFT);
    /* Possible race here so use atomic compare and swap. */
    cheri_debug_assert(tagblock_index < num_tagblocks(ram) &&
                       "Tag index out of bounds");
    old = qatomic_cmpxchg(&tagmem[tagblock_index], NULL, tagblk);
    if (old != NULL) {
        /* Lost the race, free. */
        g_free(tagblk);
        return old;
    } else {
        return tagblk;
    }
}

static inline QEMU_ALWAYS_INLINE CheriTagBlock *cheri_tag_block(size_t tag_index,
                                                                RAMBlock *ram)
{
    const size_t tagbock_index = tag_index >> CAP_TAGBLK_SHFT;
    cheri_debug_assert(ram->cheri_tags);
    cheri_debug_assert(tagbock_index < num_tagblocks(ram));
    CheriTagBlock **tagmem = (CheriTagBlock **)ram->cheri_tags;
    return tagmem[tagbock_index];
}

static inline QEMU_ALWAYS_INLINE bool tagblock_get_tag(CheriTagBlock *block,
                                                       size_t block_index)
{
    return block
               ? test_bit(block_index * TAGMEM_BITMAP_SCALE, block->tag_bitmap)
               : false;
}

static inline QEMU_ALWAYS_INLINE bool
tagblock_get_locktag(CheriTagBlock *block, size_t block_index,
                     tag_reader_lock_t *lock)
{
    if (!block) {
        if (lock)
            *lock = TAG_LOCK_NONE;
        return false;
    }

    lock_tag *locktag = &block->locked_tags[block_index];
    if (lock) {
        *lock = (tag_reader_lock_t)locktag;
        return lock_tag_read_tag_and_acquire_lock(locktag);
    } else {
        return lock_tag_read(locktag);
    }
}

// Given that many should always be well aligned, might as well use larger
// operations than on individual bits. This also makes it easier to do an atomic
// version if desired
static inline QEMU_ALWAYS_INLINE int tagblock_get_tag_many(CheriTagBlock *block,
                                                           size_t block_index)
{
    if (!block)
        return 0;

    block_index *= TAGMEM_BITMAP_SCALE;

    long result = block->tag_bitmap[BIT_WORD(block_index)] >>
                  (block_index & (BITS_PER_LONG - 1));

#if !TAGMEM_USE_BITMAP
    result = byte_pack_he(result);
#endif

    return result & CAP_TAG_GET_MANY_MASK;
}

static inline QEMU_ALWAYS_INLINE int
tagblock_get_locktag_many(CheriTagBlock *block, size_t block_index,
                          bool take_lock)
{
    if (!block)
        return 0;

    if (!take_lock) {
        // If we don't need the lock we can just read a whole bunch of these at
        // once
        packed_t raw = *(packed_t *)&block->locked_tags[block_index].as_int;
        return byte_pack_le(raw);
    } else {
        // Otherwise acquire the locks in order
        lock_tag *start = &block->locked_tags[block_index];
        int result = 0;
        for (int ndx = 0; ndx != (1 << CAP_TAG_GET_MANY_SHFT); ndx++) {
            result |= lock_tag_read_tag_and_acquire_lock(start + ndx)
                          ? (1 << ndx)
                          : 0;
        }
        return result;
    }
}

static inline QEMU_ALWAYS_INLINE bool tagblock_set_tag(CheriTagBlock *block,
                                                       size_t block_index)
{
    bool old_bit =
        test_bit(block_index * TAGMEM_BITMAP_SCALE, block->tag_bitmap) ? 1 : 0;
    set_bit(block_index * TAGMEM_BITMAP_SCALE, block->tag_bitmap);
    return old_bit;
}

static inline QEMU_ALWAYS_INLINE bool
tagblock_set_locktag(CheriTagBlock *block, size_t block_index,
                     tag_writer_lock_t *lock, bool lock_only)
{
    lock_tag *lockTag = &block->locked_tags[block_index];
    if (lock) {
        *lock = (tag_writer_lock_t)(lockTag);
        return lock_tag_write_tag_and_acquire_lock_impl(lockTag, !lock_only, 1);
    } else {
        return lock_tag_write(&block->locked_tags[block_index], 1, true);
    }
}

static inline QEMU_ALWAYS_INLINE void
tagblock_set_tag_many(CheriTagBlock *block, size_t block_index, uint8_t tags)
{
    block_index *= TAGMEM_BITMAP_SCALE;

    size_t hi = BIT_WORD(block_index);
    size_t lo = block_index & (BITS_PER_LONG - 1);

    long tags_shifted = tags;
#if !TAGMEM_USE_BITMAP
    tags_shifted = byte_unpack_he(tags);
#endif

    tags_shifted = (tags & CAP_TAG_GET_MANY_MASK) << lo;

    long result = block->tag_bitmap[hi];
    result = (result & ~(CAP_TAG_GET_MANY_MASK << lo)) | tags_shifted;
    block->tag_bitmap[BIT_WORD(block_index)] = result;
}

static inline QEMU_ALWAYS_INLINE void
tagblock_set_locktag_many(CheriTagBlock *block, size_t block_index,
                          uint8_t tags, bool take_lock)
{
    if (!take_lock) {
        packed_t unpacked = byte_unpack_le(tags);
        *(packed_t *)&block->locked_tags[block_index].as_int = unpacked;
    } else {
        lock_tag *start = &block->locked_tags[block_index];
        lock_tag *end = start + (1 << CAP_TAG_GET_MANY_SHFT);
        for (; start != end; start++) {
            lock_tag_write_tag_and_acquire_lock(start, tags & 1);
            tags = tags >> 1;
        }
    }
}

static inline QEMU_ALWAYS_INLINE void tagblock_clear_tag(CheriTagBlock *block,
                                                         size_t block_index)
{
    clear_bit(block_index * TAGMEM_BITMAP_SCALE, block->tag_bitmap);
}

static inline QEMU_ALWAYS_INLINE void
tagblock_clear_locktag(CheriTagBlock *block, size_t block_index,
                       tag_writer_lock_t *lock, bool do_assert)
{
    lock_tag *lockTag = &block->locked_tags[block_index];
    if (lock) {
        *lock = (tag_writer_lock_t)lockTag;
        lock_tag_write_tag_and_acquire_lock(lockTag, 0);
    } else {
        lock_tag_write(lockTag, 0, do_assert);
    }
}

static inline QEMU_ALWAYS_INLINE bool tag_bit_get(size_t index, RAMBlock *ram,
                                                  tag_reader_lock_t *lock)
{
    CheriTagBlock *block = cheri_tag_block(index, ram);
    size_t block_index = CAP_TAGBLK_IDX(index);
    if (qemu_tcg_mttcg_enabled()) {
        return tagblock_get_locktag(block, block_index, lock);
    } else {
        if (lock)
            *lock = TAG_LOCK_NONE;
        return tagblock_get_tag(block, block_index);
    }
}

static inline QEMU_ALWAYS_INLINE bool tag_bit_set(size_t index, RAMBlock *ram,
                                                  bool *allocated,
                                                  tag_writer_lock_t *lock,
                                                  bool lock_only)
{
    CheriTagBlock *block = cheri_tag_block(index, ram);
    if (!block) {
        block = cheri_tag_new_tagblk(ram, index);
        *allocated = true;
    }

    size_t block_index = CAP_TAGBLK_IDX(index);

    if (qemu_tcg_mttcg_enabled()) {
        return tagblock_set_locktag(block, block_index, lock, lock_only);
    } else {
        if (lock)
            *lock = TAG_LOCK_NONE;
        return tagblock_set_tag(block, block_index);
    }
}
static inline QEMU_ALWAYS_INLINE void tag_bit_clear(size_t index, RAMBlock *ram,
                                                    tag_writer_lock_t *lock)
{
    CheriTagBlock *block = cheri_tag_block(index, ram);
    size_t block_index = CAP_TAGBLK_IDX(index);

    if (block) {
        if (qemu_tcg_mttcg_enabled()) {
            tagblock_clear_locktag(block, block_index, lock, true);
        } else {
            tagblock_clear_tag(block, block_index);
        }
    }
}

static inline QEMU_ALWAYS_INLINE void tag_release_write_lock(size_t index,
                                                             RAMBlock *ram)
{
    CheriTagBlock *block = cheri_tag_block(index, ram);

    if (block) {
        lock_tag_release_write(&block->locked_tags[CAP_TAGBLK_IDX(index)]);
    }
}

// static inline QEMU_ALWAYS_INLINE void
// tag_bit_range_clear(RAMBlock *ram, size_t start, size_t count)
//{
//    cheri_debug_assert(start + count < num_tagblocks(ram) * CAP_TAGBLK_SIZE);
//    for (size_t i = start; i < start + count; i++) {
//        // TODO: use memset/bitmap_clear_range for individual tag blocks
//        tag_bit_clear(i, ram);
//    }
//}

void cheri_tag_init(MemoryRegion *mr, uint64_t memory_size)
{
    assert(memory_region_is_ram(mr));
    assert(memory_region_size(mr) == memory_size &&
           "Incorrect tag mem size passed?");

    size_t cheri_ntagblks = num_tagblocks(mr->ram_block);
    mr->ram_block->cheri_tags =
        g_malloc0(cheri_ntagblks * sizeof(CheriTagBlock *));
    if (mr->ram_block->cheri_tags == NULL) {
        error_report("%s: Can't allocated tag memory", __func__);
        exit(-1);
    }
#ifdef CHERI_MAGIC128
    if (!magic128_table) {
        magic128_table = g_hash_table_new(g_int64_hash, NULL);
    }
#endif
}

static inline hwaddr v2p_addr(CPUArchState *env, target_ulong vaddr,
                              MMUAccessType rw, int reg, uintptr_t pc,
                              int *prot)
{
    hwaddr paddr = 0;

#ifdef TARGET_MIPS
    paddr = cpu_mips_translate_address_c2(env, vaddr, rw, reg, prot, pc);
#elif defined(TARGET_RISCV)
    paddr = cpu_riscv_translate_address_tagmem(env, vaddr, rw, reg, prot, pc);
#elif defined(TARGET_AARCH64)
    paddr = cpu_arm_translate_address_tagmem(env, vaddr, rw, reg, prot, pc);
#else
#error "TODO: IMPLEMENT cpu_translate_address_tagmem"
#endif
    assert(paddr != -1LL && "Should have raised an MMU fault on failure");
    return paddr;
}

static inline void check_tagmem_writable(CPUArchState *env, target_ulong vaddr,
                                         hwaddr paddr, ram_addr_t ram_offset,
                                         MemoryRegion *mr, uintptr_t pc)
{
    if (memory_region_is_rom(mr) || memory_region_is_romd(mr)) {
        error_report(
            "QEMU ERROR: attempting change tag bit on read-only memory:");
        error_report("%s: vaddr=0x%jx -> %s+0x%jx (paddr=0x%jx)", __func__,
                     (uintmax_t)vaddr, memory_region_name(mr),
                     (uintmax_t)ram_offset, (uintmax_t)paddr);
        cpu_transaction_failed(env_cpu(env), paddr, vaddr, CHERI_CAP_SIZE,
                               MMU_DATA_STORE, // TODO: MMU_DATA_CAP_STORE
                               cpu_mmu_index(env, false),
                               MEMTXATTRS_UNSPECIFIED, MEMTX_ERROR, pc);
    }
}

static inline RAMBlock *p2r_addr(CPUArchState *env, hwaddr addr, ram_addr_t* offset, MemoryRegion** mrp)
{
    hwaddr hoffset, len;
    MemoryRegion *mr;
    CPUState *cs = env_cpu(env);

    mr = address_space_translate(cs->as, addr, &hoffset, &len, false, MEMTXATTRS_UNSPECIFIED);
    *offset = hoffset;
    if (mrp)
        *mrp = mr;

    // ROM/ROMD regions can have "RAM" addresses, but for our purposes we want
    // them to not have tags and so return RAM_ADDR_INVALID.
    if (memory_region_is_rom(mr) || memory_region_is_romd(mr)) {
        return NULL;
    }
    return mr->ram_block;
}

static inline RAMBlock *v2r_addr(CPUArchState *env, target_ulong vaddr,
                                 hwaddr *ret_paddr, ram_addr_t *offset,
                                 MMUAccessType rw, int reg, uintptr_t pc)
{
    int prot;
    MemoryRegion* mr = NULL;
    hwaddr paddr = v2p_addr(env, vaddr, rw, reg, pc, &prot);
    if (ret_paddr)
        *ret_paddr = paddr;
    RAMBlock *block = p2r_addr(env, paddr, offset, &mr);
    if (rw == MMU_DATA_CAP_STORE || rw == MMU_DATA_STORE)
        check_tagmem_writable(env, vaddr, paddr, *offset, mr, pc);
    return block;
}

void *cheri_tagmem_for_addr(RAMBlock *ram, ram_addr_t ram_offset, size_t size)
{
#if !TAGMEM_USE_BITMAP
    return NULL; /* Not implemented */
#else
    if (!ram || !ram->cheri_tags)
        return ALL_ZERO_TAGBLK;

    uint64_t tag = ram_offset >> CAP_TAG_SHFT;
#ifndef TARGET_AARCH64
    // AArch64 seems to use different sizes. Might be worth looking into.
    cheri_debug_assert(size == TARGET_PAGE_SIZE && "Unexpected size");
#endif
    CheriTagBlock *tagblk = cheri_tag_block(tag, ram);
    if (tagblk != NULL) {
        const size_t tagblk_index = CAP_TAGBLK_IDX(tag);
        if (qemu_tcg_mttcg_enabled()) {
            return tagblk->locked_tags + tagblk_index;
        } else {
            return tagblk->tag_bitmap + BIT_WORD(tagblk_index);
        }
    }
    // No tags allocated yet
    return ALL_ZERO_TAGBLK;
#endif
}

static inline CPUIOTLBEntry *
get_iotlb_entry_fast(CPUArchState *env, target_ulong vaddr, int mmu_idx)
{
    /* XXXAR: see mte_helper.c */
    /*
     * Find the iotlbentry for ptr.  This *must* be present in the TLB
     * because we just found the mapping.
     * TODO: Perhaps there should be a cputlb helper that returns a
     * matching tlb entry + iotlb entry.
     */
#ifdef CONFIG_DEBUG_TCG
    CPUTLBEntry *entry = tlb_entry(env, mmu_idx, vaddr);
    g_assert(tlb_hit(tlb_addr_write(entry), vaddr));
#endif
    return &env_tlb(env)->d[mmu_idx].iotlb[tlb_index(env, mmu_idx, vaddr)];
}

typedef struct TagOffset {
    target_ulong value;
} TagOffset;

static QEMU_ALWAYS_INLINE TagOffset addr_to_tag_offset(target_ulong addr)
{
    return (struct TagOffset){.value = addr >> CAP_TAG_SHFT};
}

static QEMU_ALWAYS_INLINE target_ulong tag_offset_to_addr(TagOffset offset)
{
    return offset.value << CAP_TAG_SHFT;
}

static void cheri_tag_invalidate_one(CPUArchState *env, target_ulong vaddr,
                                     uintptr_t pc, tag_writer_lock_t *lock,
                                     bool lock_only);

static void invalidate_from_locktag(lock_tag *lock)
{
    if (lock != TAG_LOCK_NONE) {
        assert(lock != TAG_LOCK_ERROR);
        lock_tag_write_tag_and_release(lock, 0);
    }
}

void cheri_tag_invalidate_aligned_impl(CPUArchState *env, target_ulong vaddr,
                                       uintptr_t pc, tag_writer_lock_t *lock,
                                       bool lock_only)
{
    cheri_debug_assert(QEMU_IS_ALIGNED(vaddr, CHERI_CAP_SIZE));
    if (lock && *lock) {
        invalidate_from_locktag((lock_tag *)*lock);
        return;
    }
    cheri_tag_invalidate_one(env, vaddr, pc, lock, lock_only);
}

void cheri_tag_invalidate_impl(CPUArchState *env, target_ulong vaddr,
                               int32_t size, uintptr_t pc,
                               tag_writer_lock_t *first,
                               tag_writer_lock_t *second, bool lock_only)
{
    cheri_debug_assert(size > 0);

    if (first && *first) {
        invalidate_from_locktag((lock_tag *)*first);
        if (second && *second)
            invalidate_from_locktag((lock_tag *)*second);
        return;
    }

    target_ulong first_addr = vaddr;
    target_ulong last_addr = (vaddr + size - 1);
    TagOffset tag_start = addr_to_tag_offset(first_addr);
    TagOffset tag_end = addr_to_tag_offset(last_addr);
    if (likely(tag_start.value == tag_end.value)) {
        // Common case, only one tag (i.e. an aligned store)
        cheri_tag_invalidate_one(env, vaddr, pc, first, lock_only);
        return;
    }
    // Unaligned store -> can cross a capabiblity alignment boundary and
    // therefore invalidate two tags. It can also cross pages
    size_t ntags = tag_end.value - tag_start.value + 1;
    assert(ntags == 2 && "Should invalidate at most two tag bits here");
    // Note: we always do a new page lookup here since unaligned stores
    // should be rare and the probing should be fast if it's in the QEMU TLB
#if defined(TARGET_ALIGNED_ONLY)
#if defined(CHERI_UNALIGNED)
    if (unlikely((first_addr & TARGET_PAGE_MASK) !=
                 (last_addr & TARGET_PAGE_MASK))) {
        warn_report("Got unaligned %d-byte store across page "
                    "boundary at 0x" TARGET_FMT_lx "\r\n",
                    size, vaddr);
    }
#else
    qemu_log_flush();
    error_report("FATAL: %s: " TARGET_FMT_lx
                 "+%d crosses a page boundary\r",
                 __func__, vaddr, size);
    char buffer[256];
    FILE *f = fmemopen(buffer, sizeof(buffer), "w");
    fprintf(f, "Probably caused by guest instruction: ");
    target_disas(f, env_cpu(env), cpu_get_current_pc(env, pc, false),
                 /* Only one instr*/ -1);
    fprintf(f, "\r");
    fclose(f);
    buffer[sizeof(buffer) - 1] = '\0';
    error_report("%s", buffer);
    exit(1);
#endif
#endif
    for (target_ulong addr = tag_offset_to_addr(tag_start);
         addr <= tag_offset_to_addr(tag_end); addr += CHERI_CAP_SIZE) {
        cheri_tag_invalidate_one(env, addr, pc, first, lock_only);
        first = second;
    }
}

static void cheri_tag_invalidate_one(CPUArchState *env, target_ulong vaddr,
                                     uintptr_t pc, tag_writer_lock_t *lock,
                                     bool lock_only)
{

    if (lock)
        *lock = TAG_LOCK_NONE;

    /*
     * When resolving this address in the TLB, treat it like a data store
     * (MMU_DATA_STORE) rather than a capability store (MMU_DATA_CAP_STORE),
     * so that we don't require that the SC inhibit be clear.
     */
    const int mmu_idx = cpu_mmu_index(env, false);
    void *host_addr;

    // We need to use this version to suppress exceptions if we only want the
    // lock This is required as the data access might throw an exception with a
    // higher priority. It should be the case that the data access will throw an
    // MMU exception as well, But if it doesn't, I am returning another special
    // value to catch that happening.

    int access_flags = probe_access_flags(env, vaddr, MMU_DATA_STORE, mmu_idx,
                                          lock_only, &host_addr, pc);

    if (access_flags & TLB_INVALID_MASK) {
        if (lock)
            *lock = TAG_LOCK_ERROR;
        return;
    }
    // Only RAM and ROM regions are backed by host addresses so if
    // probe_write() returns NULL we know that we can't write the tagmem.
    if (unlikely(!host_addr))
        return;

    CPUIOTLBEntry *iotlbentry = get_iotlb_entry_fast(env, vaddr, mmu_idx);
    // If the host_addr was not NULL, the tagmem value should also not be NULL!
    cheri_debug_assert(iotlbentry->tagmem != NULL);

    if (iotlbentry->tagmem == ALL_ZERO_TAGBLK) {
        // All tags for this page are zero -> no need to invalidate. We also
        // couldn't invalidate if we wanted to since ALL_ZERO_TAGBLK is not a
        // valid pointer but a magic constant.
        return;
    }

    // We cached the tagblock address in the iotlb -> find find the tag
    // bit: The iotlb->tagmem pointer always points to the tag memory
    // for the start of the page so we can simply add the index for the
    // page offset.
    target_ulong page_offset = vaddr & ~TARGET_PAGE_MASK;
    TagOffset tag_offset = addr_to_tag_offset(page_offset);

    bool old_value;

#ifdef CONFIG_DEBUG_TCG
    ram_addr_t offset;
    RAMBlock *block = qemu_ram_block_from_host(host_addr, false, &offset);
    uint64_t tag = offset >> CAP_TAG_SHFT;
    CheriTagBlock *tagblk = cheri_tag_block(tag, block);
    const size_t tagblk_index = CAP_TAGBLK_IDX(tag);
#endif

    if (qemu_tcg_mttcg_enabled()) {
        lock_tag *lockTag = (lock_tag *)iotlbentry->tagmem + tag_offset.value;
#ifdef CONFIG_DEBUG_TCG
        lock_tag *lockTagFullLookup = tagblk->locked_tags + tagblk_index;
        assert(lockTag == lockTagFullLookup);
#endif
        if (lock) {
            *lock = (tag_writer_lock_t)(lockTag);
            old_value = lock_tag_write_tag_and_acquire_lock_impl(lockTag,
                                                                 !lock_only, 0);
        } else {
            old_value = lock_tag_write(lockTag, 0, true);
        }
    } else {
#if TAGMEM_USE_BITMAP
        old_value = test_bit(tag_offset.value, iotlbentry->tagmem) ? 1 : 0;
        clear_bit(tag_offset.value, iotlbentry->tagmem);
#else
#error "!TAGMEM_USE_BITMAP case not implemented"
#endif
#ifdef CONFIG_DEBUG_TCG
        unsigned long *p_old = tagblk->tag_bitmap + BIT_WORD(tagblk_index);
        // warn_report("tag bit old: %p, tag bit new: %p\r", p_old, p_new);
#if TAGMEM_USE_BITMAP
        unsigned long *p_new =
            (unsigned long *)iotlbentry->tagmem + BIT_WORD(tag_offset.value);
#else
#error "!TAGMEM_USE_BITMAP case not implemented"
#endif
        assert(p_old == p_new);
#endif
    }

    if (qemu_log_instr_enabled(env)) {
        qemu_log_instr_extra(
            env,
            "    Cap Tag Write [" TARGET_FMT_lx "/" RAM_ADDR_FMT "] %d -> 0\n",
            vaddr, qemu_ram_addr_from_host(host_addr), old_value);
    }
}

void cheri_tag_phys_invalidate(CPUArchState *env, RAMBlock *ram,
                               ram_addr_t ram_offset, ram_addr_t len,
                               const target_ulong *vaddr)
{
    // Ignore tag clearing requests for ROM, etc.
    if (!ram->cheri_tags) {
        return;
    }
    cheri_debug_assert(!memory_region_is_rom(ram->mr) &&
                       !memory_region_is_romd(ram->mr));

    ram_addr_t endaddr = (uint64_t)(ram_offset + len);
    ram_addr_t startaddr = QEMU_ALIGN_DOWN(ram_offset, CAP_SIZE);

    for(ram_addr_t addr = startaddr; addr < endaddr; addr += CAP_SIZE) {
        uint64_t tag = addr >> CAP_TAG_SHFT;
        CheriTagBlock *tagblk = cheri_tag_block(tag, ram);
        if (tagblk != NULL) {
            const size_t tagblk_index = CAP_TAGBLK_IDX(tag);
            if (unlikely(env && qemu_log_instr_enabled(env))) {
                bool old_tag =
                    qemu_tcg_mttcg_enabled()
                        ? tagblock_get_locktag(tagblk, tagblk_index, NULL)
                        : tagblock_get_tag(tagblk, tagblk_index);
                if (vaddr) {
                    target_ulong write_vaddr =
                        QEMU_ALIGN_DOWN(*vaddr, CAP_SIZE) + (addr - startaddr);
                    qemu_log_instr_extra(env,
                                         "    Cap Tag Write [" TARGET_FMT_lx
                                         "/" RAM_ADDR_FMT "] %d -> 0\n",
                                         write_vaddr, addr, old_tag);
                } else {
                    qemu_log_instr_extra(
                        env,
                        "    Cap Tag ramaddr Write [" RAM_ADDR_FMT
                        "] %d -> 0\n",
                        addr, old_tag);
                }
            }
            // changed |= tagblock_get_tag(tagblk, tagblk_index);
            if (qemu_tcg_mttcg_enabled()) {
                tagblock_clear_locktag(tagblk, tagblk_index, false, false);
            } else {
                tagblock_clear_tag(tagblk, tagblk_index);
            }
        }
    }
}

// FIXME: The problem with this restarting an instructions is it may be
// FIXME: called after some state change that cannot be undone locally.
// FIXME: I suggest we find a way to do this without restarting.

static void cheri_tlb_flush(CPUArchState *env, target_ulong vaddr, uintptr_t pc,
                            bool took_lock, RAMBlock *ram,
                            ram_addr_t ram_offset, bool set_bit)
{
    // new tag block allocated, flush the TCG tlb so that the magic zero
    // value is removed from the iotlb.
    // Note: each tag block contains tags for multiple pages
    const size_t covered_bytes = CAP_TAGBLK_SIZE * CHERI_CAP_SIZE;
    const target_ulong start = QEMU_ALIGN_DOWN(vaddr, covered_bytes);
    const target_ulong end = start + covered_bytes;
    cheri_debug_assert(is_power_of_2(covered_bytes));
    cheri_debug_assert(covered_bytes / TARGET_PAGE_SIZE > 1);
    cheri_debug_assert(is_power_of_2(covered_bytes / TARGET_PAGE_SIZE));
    CPUState *cpu = env_cpu(env);

    for (target_ulong page_addr = start; page_addr < end;
         page_addr += TARGET_PAGE_SIZE) {
        qemu_maybe_log_instr_extra(env,
                                   "Allocated new tag block -> flushing "
                                   "TCG tlb for " TARGET_FMT_plx "\n",
                                   page_addr);
        tlb_flush_page_all_cpus_synced(cpu, page_addr);
    }

    // We have to exit the current TCG block so that QEMU refills the TLB
    if (set_bit) {
        tag_bit_clear(ram_offset >> CAP_TAG_SHFT, ram, NULL);
        if (took_lock && qemu_tcg_mttcg_enabled()) {
            tag_release_write_lock(ram_offset >> CAP_TAG_SHFT, ram);
        }
        qemu_maybe_log_instr_extra(env,
                                   "    Clearing Tag [" TARGET_FMT_lx
                                   "/" RAM_ADDR_FMT
                                   "] to allow restarting instruction\n",
                                   vaddr, ram_offset);
    }

    cpu_restore_state(cpu, pc, true);
    cpu_loop_exit_noexc(cpu);
}

void cheri_tag_set_impl(CPUArchState *env, target_ulong vaddr, int reg,
                        hwaddr *ret_paddr, uintptr_t pc,
                        tag_writer_lock_t *lock, bool lock_only)
{

    if (lock && *lock) {
        if (*lock != TAG_LOCK_NONE)
            lock_tag_write_tag_and_release((lock_tag *)*lock, 1);
        return;
    }

    /*
     * This attempt to resolve a virtual address may cause both a data store
     * TLB fault (entry missing or D bit clear) and a capability store TLB
     * fault (SC bit set).  v2r_addr bypasses the generic QEMU TCG soft-TLB
     * (env->tlb_table and friends) but will populate the MIPS-specific TLB
     * (env->tlb->mmu.r4k.tlb).  Therefore, while a data store to vaddr
     * after a tag has been set might miss in the QEMU TCG soft-TLB, it
     * won't miss in the MIPS model and therefore won't raise a processor
     * exception (and will populate the QEMU TCG soft-TLB for subsequent
     * data stores).
     */
    ram_addr_t ram_offset;
    RAMBlock *ram = v2r_addr(env, vaddr, ret_paddr, &ram_offset,
                             MMU_DATA_CAP_STORE, reg, pc);
    if (!ram) {
        if (lock)
            *lock = TAG_LOCK_NONE;
        return;
    }

    bool allocated_new_block = false;
    bool old = tag_bit_set(ram_offset >> CAP_TAG_SHFT, ram,
                           &allocated_new_block, lock, lock_only);
    /* Get the tag number and tag block ptr. */
    qemu_maybe_log_instr_extra(
        env, "    Cap Tag Write [" TARGET_FMT_lx "/" RAM_ADDR_FMT "] %d -> 1\n",
        vaddr, ram_offset, old);

    if (allocated_new_block) {
        cheri_tlb_flush(env, vaddr, pc, lock != NULL, ram, ram_offset,
                        !lock_only);
    }
}

static CheriTagBlock *cheri_tag_get_block(CPUArchState *env, target_ulong vaddr,
                                          MMUAccessType at, int reg, int xshift,
                                          uintptr_t pc, hwaddr *ret_paddr,
                                          uint64_t *ret_tag_idx, int *prot,
                                          bool create, bool *created)
{
    hwaddr paddr;

    if (created)
        *created = false;

    paddr = v2p_addr(env, vaddr, at, reg, pc, prot);

    if (ret_paddr)
        *ret_paddr = paddr;

    ram_addr_t ram_offset;
    RAMBlock *ram = p2r_addr(env, paddr, &ram_offset, NULL);
    if (!ram)
        return NULL;

    /* Get the tag number and tag block ptr. */
    *ret_tag_idx = (ram_offset >> (CAP_TAG_SHFT + xshift)) << xshift;
    CheriTagBlock *blk = cheri_tag_block(*ret_tag_idx, ram);

    if (!blk && create) {
        blk = cheri_tag_new_tagblk(ram, *ret_tag_idx);
        *created = true;
    }

    return blk;
}

bool cheri_tag_get_impl(CPUArchState *env, target_ulong vaddr, int reg,
                        hwaddr *ret_paddr, int *prot, uintptr_t pc,
                        tag_reader_lock_t *lock, bool lock_only)
{
    if (lock && *lock) {
        return (*lock == TAG_LOCK_NONE)
                   ? 0
                   : lock_tag_release_read((lock_tag *)*lock);
    }

    uint64_t tag;
    CheriTagBlock *tagblk =
        cheri_tag_get_block(env, vaddr, MMU_DATA_CAP_LOAD, reg, 0, pc,
                            ret_paddr, &tag, prot, false, NULL);

    bool result;

    size_t block_index = CAP_TAGBLK_IDX(tag);

    if (qemu_tcg_mttcg_enabled()) {
        result = tagblock_get_locktag(tagblk, block_index, lock);
    } else {
        result = tagblock_get_tag(tagblk, block_index);
    }

    qemu_maybe_log_instr_extra(
        env, "    Cap Tag Read [" TARGET_FMT_lx "/" RAM_ADDR_FMT "] -> %d\n",
        vaddr, (ram_addr_t)(tag << CAP_TAG_SHFT), result);
    return result;
}

int cheri_tag_get_many(CPUArchState *env, target_ulong vaddr, int reg,
        hwaddr *ret_paddr, uintptr_t pc)
{
    uint64_t tag_index;
    int prot;
    CheriTagBlock *tagblk = cheri_tag_get_block(
        env, vaddr, MMU_DATA_CAP_LOAD, reg, CAP_TAG_GET_MANY_SHFT, pc,
        ret_paddr, &tag_index, &prot, false, NULL);

    size_t block_index = CAP_TAGBLK_IDX(tag_index);
    int result;

    if (qemu_tcg_mttcg_enabled()) {
        result = tagblock_get_locktag_many(tagblk, block_index, false);
    } else {
        result = tagblock_get_tag_many(tagblk, block_index);
    }

    /*
     * The Mips CLoadTags, can "see around" the TLB capability load inhibit.
     * That should perhaps change?
     */
#ifndef TARGET_MIPS
    if (result && (prot & PAGE_LC_TRAP)) {
        raise_load_tag_exception(env, vaddr, reg, pc);
    }
    if (prot & PAGE_LC_CLEAR) {
        result = 0;
    }
#endif

    return result;
}

void cheri_tag_set_many(CPUArchState *env, uint32_t tags, target_ulong vaddr,
                        int reg, hwaddr *ret_paddr, uintptr_t pc)
{
    uint64_t tag_index;
    int prot;

    tags &= CAP_TAG_GET_MANY_MASK;

    MMUAccessType accessType = tags ? MMU_DATA_CAP_STORE : MMU_DATA_STORE;

    bool created;

    CheriTagBlock *tagblk = cheri_tag_get_block(
        env, vaddr, accessType, reg, CAP_TAG_GET_MANY_SHFT, pc, ret_paddr,
        &tag_index, &prot, tags != 0, &created);

    if (created) {
        cheri_tlb_flush(env, vaddr, pc, false, NULL, 0, false);
        __builtin_unreachable();
    }

    if (tagblk) {
        size_t block_index = CAP_TAGBLK_IDX(tag_index);
        if (qemu_tcg_mttcg_enabled()) {
            tagblock_set_locktag_many(tagblk, block_index, tags, false);
        } else {
            tagblock_set_tag_many(tagblk, block_index, tags);
        }
    }
}

void cheri_tag_reader_lock_impl(tag_reader_lock_t lock)
{
    lock_tag *lockTag = (lock_tag *)lock;
    lock_tag_release_read(lockTag);
}
void cheri_tag_writer_lock_impl(tag_writer_lock_t lock)
{
    lock_tag *lockTag = (lock_tag *)lock;
    lock_tag_release_write(lockTag);
}

void cheri_tag_free_lock(tag_lock_t lock)
{
    lock_tag *lockTag = (lock_tag *)lock;
    if (!lock || lock == TAG_LOCK_NONE || lock == TAG_LOCK_ERROR)
        return;
    if (lockTag->as_int & LOCKTAG_MASK_WRITE_LOCKED)
        lock_tag_release_write(lockTag);
    else
        lock_tag_release_read(lockTag);
}

#define DUMMY_LOCK_BITS 16

lock_tag dummy_tag_table[1 << DUMMY_LOCK_BITS];

static target_ulong hash_vaddr(target_ulong vaddr)
{
    size_t shft = sizeof(target_ulong);
    while (shft > DUMMY_LOCK_BITS) {
        shft /= 2;
        vaddr |= vaddr >> shft;
    }
    return vaddr & ((1 << DUMMY_LOCK_BITS) - 1);
}

static lock_tag *get_dummy(target_ulong vaddr)
{
    return &dummy_tag_table[hash_vaddr(vaddr)];
}

void get_dummy_locks(target_ulong vaddr, tag_writer_lock_t *low,
                     tag_writer_lock_t *high)
{
    target_ulong tag_addr1 = vaddr >> CAP_TAG_SHFT;
    target_ulong tag_addr2 = tag_addr1 + 1;
    tag_writer_lock_t low_dummy = NULL;
    tag_writer_lock_t high_dummy = NULL;

    if (*low == TAG_LOCK_NONE) {
        low_dummy = get_dummy(tag_addr1);
    }

    if (*high == TAG_LOCK_NONE) {
        high_dummy = get_dummy(tag_addr2);
    }

    if (low_dummy && high_dummy && high_dummy < low_dummy) {
        // Keep lock order consistent.
        tag_writer_lock_t tmp = high_dummy;
        high_dummy = low_dummy;
        low_dummy = tmp;
    }

    if (low_dummy) {
        lock_tag_write_acquire_lock(low_dummy);
        *low = low_dummy;
    }

    if (high_dummy) {
        lock_tag_write_acquire_lock(high_dummy);
        *high = high_dummy;
    }
}

#ifdef CHERI_MAGIC128
void cheri_tag_set_m128(CPUArchState *env, target_ulong vaddr, int reg,
                        uint8_t tagbit, uint64_t tps, uint64_t length,
                        uintptr_t pc)
{
    // We index the "magic" table by physical address:
    hwaddr paddr;
    cheri_tag_assert_not_mttcg();
    cheri_tag_set(env, vaddr, reg, &paddr, pc);
    gpointer htable_key = GINT_TO_POINTER(paddr);
    struct Magic128Data *data = g_hash_table_lookup(magic128_table, htable_key);
    if (!data) {
        data = g_malloc(sizeof(struct Magic128Data));
        g_hash_table_insert(magic128_table, htable_key, data);
    }
    data->tps = tps;
    data->length = length;
}

bool cheri_tag_get_m128(CPUArchState *env, target_ulong vaddr, int reg,
                        uint64_t *ret_tps, uint64_t *ret_length,
                        hwaddr *ret_paddr, int *prot, uintptr_t pc)
{
    hwaddr paddr;
    cheri_tag_assert_not_mttcg();
    bool tag = cheri_tag_get(env, vaddr, reg, &paddr, prot, pc, NULL);
    // Only fetch the extra data if the value is tagged
    struct Magic128Data *data = NULL;
    if (tag) {
        data = g_hash_table_lookup(magic128_table, GINT_TO_POINTER(paddr));
    }
    if (data) {
        *ret_tps = data->tps;
        *ret_length = data->length;
    } else {
        *ret_tps = 0ULL;
        *ret_length = 0ULL;
    }
    if (ret_paddr)
        *ret_paddr = paddr;
    return tag;
}
#endif /* CHERI_MAGIC128 */
