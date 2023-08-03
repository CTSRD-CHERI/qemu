/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2015-2016 Stacey Son <sson@FreeBSD.org>
 * Copyright (c) 2016-2018 Alfredo Mazzinghi <am2419@cl.cam.ac.uk>
 * Copyright (c) 2016-2018 Alex Richardson <Alexander.Richardson@cl.cam.ac.uk>
 * Copyright (c) 2021-2021 Lawrence Esswood <le277@cam.ac.uk>
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
#include "cheri_usermem.h"
#include "exec/exec-all.h"
#include "exec/log.h"
#include "exec/ramblock.h"
#ifdef CONFIG_USER_ONLY
#include "qemu.h"
#else
#include "hw/boards.h"
#endif
#include "cheri_defs.h"
#include "cheri-helper-utils.h"
// XXX: use hbitmap? Or a different data structure?
#include "qemu/bitmap.h"
#include "glib/ghash.h"

#if defined(TARGET_MIPS)
#include "cheri_utils.h"
#include "internal.h"
#endif

#if !defined(TARGET_CHERI)
#error "Should only be included for TARGET_CHERI"
#endif

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
 * If MTTCG is not enabled, one bit is used per tag in a dense bitmap.
 *
 * If MTTCG is enabled, each tag bit is combined with a lock making the size
 * up to a byte per tag. See cheri_tagmem.h for how the interface should be
 * used.
 *
 * XXX Should consider adding a reference count per tag block so that
 * blocks can be deallocated when no longer used maybe.
 *
 * FIXME: rewrite using somethign more like the upcoming MTE changes
 * (https://github.com/rth7680/qemu/commits/tgt-arm-mte-user)
 *
 * XXX: I/O threads still exist even without MTTCG and need to have tag
 * clearing be atomic with their writes. Currently various places just write to
 * guest memory directly and then we tag clear in invalidate_and_set_dirty.
 * Well-behaved guests should not be touching memory handed off to DMA-capable
 * devices but a malicious guest could repeatedly DMA powerful capability bit
 * patterns on top of valid capabilities and try to race to read in between the
 * DMA write and the tag invalidate.
 */

/*
 * In the system mode, SMP is configured when the CPU is starting and QEMU can
 * determine if concurrent tags are needed at the time of the first tag block
 * allocation.
 *
 * In the user mode, a number of parallel threads is not known at the time of
 * the first tag block allocation.
 *
 * In both cases, _need_concurrent_tags cannot be adjusted after the first block
 * allocation because tag blocks allocated before _need_concurrent_tags is
 * enabled could not be accessed in parallel after it is enabled.
 * It means that the user mode must always enable concurrent tags in case
 * an emulated process starts using threads.
 */
#if defined(CONFIG_USER_ONLY) && defined(TARGET_SUPPORTS_MTTCG)
bool _need_concurrent_tags = true;
bool _need_concurrent_tags_initialized = true;
#else
bool _need_concurrent_tags = false;
bool _need_concurrent_tags_initialized = false;
#endif

/* Define to do some extra checks around spinlocks */
//#define DEBUG_SPIN_LOCKS

/*
 * Report if spin locks are being held too long (only if DEBUG_SPIN_LOCKS).
 * I see O(10 000) pretty regularly, at least every second or so.
 * I more rarely see O(10 000 000), however, printing the debug messages is done
 * with the lock held, so may be significantly increasing the time they are held
 * for.
 * TODO: If these are being held too long, maybe we should be sleeping? Should
 * only ever happen if the core holding the lock is de-scheduled.
 */

#define INSTRUMENT_SPIN_REPORT 10000
/* Assert if held a very long time */
#define SPIN_TOO_MANY 10000000000ULL

#define CAP_TAGBLK_SHFT     12          // 2^12 or 4096 tags per block
#define CAP_TAGBLK_MSK      ((1 << CAP_TAGBLK_SHFT) - 1)
#define CAP_TAGBLK_SIZE       (1 << CAP_TAGBLK_SHFT)
#define CAP_TAGBLK_IDX(tag_idx) ((tag_idx) & CAP_TAGBLK_MSK)
#define TAGS_PER_PAGE        (TARGET_PAGE_SIZE / CHERI_CAP_SIZE)

#ifndef CAP_TAG_GET_MANY_SHFT
#error "Define a CAP_TAG_GET_MANY_SHFT in appropriate cheri-archspecific.h"
#endif

_Static_assert(CAP_TAG_GET_MANY_SHFT <= 3, "");

/* Pack/unpack to make get/set multiple tags atomic
 * (assuming the host atomic size is large enough)
 * lock_tags are indexed as bytes, and so are always little endian.
 * Bitmaps are indexed as longs, and so are host endian.
 */

#if (CAP_TAG_GET_MANY_SHFT == 3)
#define byte_unpack_he byte_unpack_64
#define byte_pack_he   byte_pack_64
#ifdef HOST_WORDS_BIGENDIAN
#define byte_unpack_le byte_unpack_swap_64
#define byte_pack_le   byte_pack_swap_64
#else
#define byte_unpack_le byte_unpack_64
#define byte_pack_le   byte_pack_64
#endif
#define packed_t uint64_t
#elif (CAP_TAG_GET_MANY_SHFT == 2)
#define byte_unpack_he byte_unpack_32
#define byte_pack_he   byte_pack_32
#ifdef HOST_WORDS_BIGENDIAN
#define byte_unpack_le byte_unpack_swap_32
#define byte_pack_le   byte_pack_swap_32
#else
#define byte_unpack_le byte_unpack_32
#define byte_pack_le   byte_pack_32
#endif
#define packed_t uint32_t
#else
#error "No packed type defined for this CAP_TAG_GET_MANY_SHFT"
#endif

#define CAP_TAG_GET_MANY_MASK ((1 << (1UL << CAP_TAG_GET_MANY_SHFT)) - 1UL)
#define CAP_TAG_MANY_DATA_SIZE (CHERI_CAP_SIZE << CAP_TAG_GET_MANY_SHFT)

static inline size_t num_tagblocks(RAMBlock* ram)
{
    uint64_t memory_size;
#ifdef CONFIG_USER_ONLY
    memory_size = ram->size;
#else
    memory_size = memory_region_size(ram->mr);
#endif
    size_t result = DIV_ROUND_UP(memory_size, CHERI_CAP_SIZE * CAP_TAGBLK_SIZE);
#ifndef CONFIG_USER_ONLY
    assert(result == (memory_size / CHERI_CAP_SIZE) >> CAP_TAGBLK_SHFT);
#endif
    return result;
}

/* A CHERI tag with a readers-writer lock.
 * Write preferring as if one core is spinning waiting for a tag to be updated,
 * the write needs to make it through for progress to be made.
 * This lock should never be held for more than the duration of an instruction.
 */

typedef struct lock_tag {
    uint8_t as_int;
} lock_tag;

#define LOCKTAG_MASK_TAG           (1 << 0)
#define LOCKTAG_MASK_WRITE_LOCKED  (1 << 1)
#define LOCKTAG_MASK_WRITE_WAITING (1 << 2)
#define LOCKTAG_MASK_READERS       (0b11111 << 3)
#define LOCKTAG_MASK_READER_INC    (1 << 3)

#ifdef DEBUG_SPIN_LOCKS

uint64_t read_spins_max = 0;
uint64_t write_spins_max = 0;

#define assert_write_locked(lock)                                              \
    assert(lock->as_int &LOCKTAG_MASK_WRITE_LOCKED)
#define assert_read_locked(lock) assert(lock->as_int &LOCKTAG_MASK_READERS)

static void spins_report(const char *info, uint64_t new, uint64_t *max_ptr)
{
    uint64_t max = *max_ptr;
    if (new > max) {
        qatomic_cmpxchg(max_ptr, max, new);
        printf("New maximum spins for %s: %ld\n", info, new);
    }
    if (new > INSTRUMENT_SPIN_REPORT) {
        printf("Large number of spins for %s: %ld\n", info, new);
    }
}

#define spins_assert(spins)                                                    \
    assert(spins < SPIN_TOO_MANY && "lock held too long");

#else

#define spins_report(...)
#define spins_assert(...)
#define assert_write_locked(...)
#define assert_read_locked(...)

#endif

/* Acquire / release. All acquires (read and write) return the tag.
 * read acquire and read release can optionally set the tag.
 */

static bool lock_tag_read_tag_and_acquire_lock(lock_tag *lock)
{
    lock_tag old = *lock;

    int64_t spins = 0;
    do {
        if (!(old.as_int &
              (LOCKTAG_MASK_WRITE_WAITING | LOCKTAG_MASK_WRITE_LOCKED))) {
            lock_tag new = old;
            /* (TODO: a static assert that we don't have more than 31 host
             * threads). Overflow precluded by number of threads. */
            assert(new.as_int <= UINT8_MAX - LOCKTAG_MASK_READER_INC);
            new.as_int += LOCKTAG_MASK_READER_INC;
            lock_tag got = { .as_int = qatomic_cmpxchg(
                                 &lock->as_int, old.as_int, new.as_int) };
            if (got.as_int == old.as_int)
                break;
            old = got;
        } else {
            old = *lock;
        }
        spins++;
        spins_assert(spins);
    } while (true);
    spins_report("read acquire", spins, &read_spins_max);
    return old.as_int & LOCKTAG_MASK_TAG;
}

static bool lock_tag_release_read(lock_tag *lock)
{
    assert_read_locked(lock);
    lock_tag tag = { .as_int = qatomic_fetch_sub(&lock->as_int,
                                                 LOCKTAG_MASK_READER_INC) };
    return tag.as_int & LOCKTAG_MASK_TAG;
}

/* Generic implementation for anything that might want to take the lock as
 * a writer.
 * @set_tag: will also set the tag when taking the lock
 * @tag: what to set the tag to (if set_tag)
 * @with_release: will also release the lock afterwards
 */
static bool lock_tag_write_tag_and_acquire_lock_impl(lock_tag *lock,
                                                     bool set_tag, bool tag,
                                                     bool with_release)
{
    lock_tag old = *lock;
    uint64_t spins = 0;
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
                if (!with_release)
                    new.as_int |= LOCKTAG_MASK_WRITE_LOCKED;
                if (set_tag)
                    new.as_int = (new.as_int & ~LOCKTAG_MASK_TAG) | tag;
            }
            lock_tag got = { .as_int = qatomic_cmpxchg(
                                 &lock->as_int, old.as_int, new.as_int) };
            if ((got.as_int == old.as_int) &&
                !(old.as_int & LOCKTAG_MASK_READERS))
                break;

            old = got;
        }
        spins++;
        spins_assert(spins);
    } while (true);
    spins_report("write acquire", spins, &read_spins_max);
    assert_write_locked(lock);
    return old.as_int & LOCKTAG_MASK_TAG;
}

static bool lock_tag_write_tag_and_acquire_lock(lock_tag *lock, bool tag)
{
    return lock_tag_write_tag_and_acquire_lock_impl(lock, true, tag, false);
}

static bool lock_tag_write_acquire_lock(lock_tag *lock)
{
    return lock_tag_write_tag_and_acquire_lock_impl(lock, false, false, false);
}

static void lock_tag_release_write(lock_tag *lock)
{
    assert_write_locked(lock);
    qatomic_fetch_and(&lock->as_int, LOCKTAG_MASK_TAG);
}

static void lock_tag_write_tag_and_release(lock_tag *lock, bool tag)
{
    assert_write_locked(lock);
    qatomic_set(&lock->as_int, tag ? LOCKTAG_MASK_TAG : 0);
}

/* These two don't take locks outs, so use with caution */
/* I think, as long as every other access respects locks, we don't need
 * atomics here */
static bool lock_tag_read(lock_tag *lock)
{
#ifdef CONFIG_DEBUG_TCG
    assert(!need_concurrent_tags() ||
           (lock->as_int & (LOCKTAG_MASK_READERS | LOCKTAG_MASK_WRITE_LOCKED)));
#endif
    return lock->as_int & LOCKTAG_MASK_TAG;
}

static bool lock_tag_write(lock_tag *lock, bool tag, bool check_locked)
{
#ifdef CONFIG_DEBUG_TCG
    if (check_locked)
        assert(!need_concurrent_tags() ||
               (lock->as_int & LOCKTAG_MASK_WRITE_LOCKED));
#endif
    bool old = lock->as_int & LOCKTAG_MASK_TAG;
    lock->as_int = (lock->as_int & ~LOCKTAG_MASK_TAG) | tag;
    return old;
}

typedef struct CheriTagBlock {
    /* It would be silly to use locks for non-mttcg. So support both formats and
     * use one or the other depending on need_concurrent_tags()
     */
    union {
        DECLARE_BITMAP(tag_bitmap, CAP_TAGBLK_SIZE);
        lock_tag locked_tags[CAP_TAGBLK_SIZE];
    };
} CheriTagBlock;


static CheriTagBlock *cheri_tag_new_tagblk(RAMBlock *ram, uint64_t tagidx)
{
    CheriTagBlock *tagblk, *old;

    size_t size = need_concurrent_tags()
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

static inline QEMU_ALWAYS_INLINE bool tagmem_get_tag(void *tagmem, size_t index,
                                                     tag_reader_lock_t *lock)
{
    if (need_concurrent_tags()) {
        lock_tag *locktag = (lock_tag *)tagmem + index;
        if (lock) {
            *lock = (tag_reader_lock_t)locktag;
            return lock_tag_read_tag_and_acquire_lock(locktag);
        } else {
            return lock_tag_read(locktag);
        }
    } else {
        if (lock)
            *lock = TAG_LOCK_NONE;
        unsigned long *p = (unsigned long *)tagmem + BIT_WORD(index);
        unsigned long word;

        word = qatomic_read(p);
        return (word & BIT_MASK(index)) != 0;
    }
}

static inline QEMU_ALWAYS_INLINE bool tagblock_get_tag(CheriTagBlock *block,
                                                       size_t block_index)
{
    return block ? test_bit(block_index, block->tag_bitmap) : false;
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

static inline QEMU_ALWAYS_INLINE int
tagmem_get_tag_many(void *tagmem, size_t block_index, bool take_lock)
{
    if (need_concurrent_tags()) {
        lock_tag *lock = ((lock_tag *)tagmem) + block_index;
        if (!take_lock) {
            /* If we don't need the lock we can just read a bunch of
             * these at once */
            packed_t raw = qatomic_read((packed_t *)lock);
            return byte_pack_le(raw);
        } else {
            /* Otherwise acquire the locks in order */
            lock_tag *start = lock;
            int result = 0;
            for (int ndx = 0; ndx != (1 << CAP_TAG_GET_MANY_SHFT); ndx++) {
                result |= lock_tag_read_tag_and_acquire_lock(start + ndx)
                              ? (1 << ndx)
                              : 0;
            }
            return result;
        }
    } else {
        unsigned long *p = (unsigned long *)tagmem + BIT_WORD(block_index);
        unsigned long word;

        word = qatomic_read(p);
        return (word >> (block_index % BITS_PER_LONG)) & CAP_TAG_GET_MANY_MASK;
    }
}

static inline QEMU_ALWAYS_INLINE void tagmem_set_tag(void *tagmem,
                                                     size_t block_index,
                                                     tag_writer_lock_t *lock,
                                                     bool lock_only)
{
    if (need_concurrent_tags()) {
        lock_tag *lockTag = (lock_tag *)tagmem + block_index;
        if (lock) {
            *lock = (tag_writer_lock_t)(lockTag);
            lock_tag_write_tag_and_acquire_lock_impl(lockTag, !lock_only, 1,
                                                     false);
        } else {
            lock_tag_write(lockTag, 1, true);
        }
    } else {
        if (lock)
            *lock = TAG_LOCK_NONE;

        unsigned long *p = (unsigned long *)tagmem + BIT_WORD(block_index);

        qatomic_or(p, BIT_MASK(block_index));
    }
}

/* A 1 in the lowest bit in each byte of a packed_t */
#define GET_SET_MANY_LOCKTAG_TAG_MASK (((packed_t)~0ULL) / 0xFF)

static inline QEMU_ALWAYS_INLINE void tagmem_set_tag_many(void *tagmem,
                                                          size_t block_index,
                                                          uint8_t tags,
                                                          bool take_lock)
{
    if (need_concurrent_tags()) {
        lock_tag *lock = ((lock_tag *)tagmem) + block_index;

        /* TODO: Needs testing on Morello multicore.
         *  If something breaks around the STCT instruction it is THIS. */

        if (!take_lock) {
            /* Although we are not taking any locks, we still need to not
             * touch any of the lock bits as other operations may be using them.
             */
            if (tags == 0) {
                /* If setting to zeros this is just an and. */
                qatomic_and((packed_t *)lock, ~GET_SET_MANY_LOCKTAG_TAG_MASK);
            } else if (tags == CAP_TAG_GET_MANY_MASK) {
                /* If setting to ones it is an or. */
                qatomic_or((packed_t *)lock, GET_SET_MANY_LOCKTAG_TAG_MASK);
            } else {
                /* Otherwise we need a cmpxchg loop. */
                packed_t unpacked = byte_unpack_le(tags);
                packed_t old;
                packed_t got = *(packed_t *)lock;
                do {
                    old = got;
                    got = qatomic_cmpxchg(
                        (packed_t *)lock, old,
                        (old & ~GET_SET_MANY_LOCKTAG_TAG_MASK) | unpacked);
                } while (old != got);
            }
        } else {
            lock_tag *start = lock;
            lock_tag *end = start + (1 << CAP_TAG_GET_MANY_SHFT);
            for (; start != end; start++) {
                lock_tag_write_tag_and_acquire_lock(start, tags & 1);
                tags = tags >> 1;
            }
        }
    } else {
        unsigned long *p = (unsigned long *)tagmem + BIT_WORD(block_index);
        size_t shift = block_index % BITS_PER_LONG;
        unsigned long mask = CAP_TAG_GET_MANY_MASK << shift;
        unsigned long tags_shifted = ((unsigned long)tags << shift) & mask;

        if (likely(tags_shifted == 0)) {
            qatomic_and(p, ~mask);
        } else {
            unsigned long old, new, cmp;
            cmp = qatomic_read(p);
            do {
                old = cmp;
                new = (old & ~mask) | tags_shifted;
                cmp = qatomic_cmpxchg(p, old, new);
            } while (cmp != old);
        }
    }
}

static inline QEMU_ALWAYS_INLINE void tagmem_clear_tag(void *tagmem,
                                                       size_t index,
                                                       tag_writer_lock_t *lock,
                                                       bool lock_only)
{
    if (need_concurrent_tags()) {
        lock_tag *lockTag = (lock_tag *)tagmem + index;
        if (lock) {
            *lock = (tag_writer_lock_t)lockTag;
            lock_tag_write_tag_and_acquire_lock_impl(lockTag, !lock_only, 0,
                                                     false);
        } else {
            lock_tag_write(lockTag, 0, true);
        }
    } else {
        if (lock)
            *lock = TAG_LOCK_NONE;
        unsigned long *p = (unsigned long *)tagmem + BIT_WORD(index);
        qatomic_and(p, ~BIT_MASK(index));
    }
}

#ifndef CONFIG_USER_ONLY
static inline QEMU_ALWAYS_INLINE void tagblock_clear_tag(CheriTagBlock *block,
                                                         size_t block_index)
{
    unsigned long *p =
        (unsigned long *)block->tag_bitmap + BIT_WORD(block_index);
    qatomic_and(p, ~BIT_MASK(block_index));
}

static inline QEMU_ALWAYS_INLINE void
tagblock_clear_locktag_with_lock_and_release(CheriTagBlock *block,
                                             size_t block_index)
{
    lock_tag *lockTag = &block->locked_tags[block_index];
    lock_tag_write_tag_and_acquire_lock_impl(lockTag, true, 0, true);
}
#endif

#ifdef CONFIG_USER_ONLY
void cheri_tag_init(RAMBlock *ram, uint64_t memory_size)
#else
void cheri_tag_init(MemoryRegion *mr, uint64_t memory_size)
#endif
{
#ifndef CONFIG_USER_ONLY
    RAMBlock *ram;
    assert(memory_region_is_ram(mr));
    assert(memory_region_size(mr) == memory_size &&
           "Incorrect tag mem size passed?");
    ram = mr->ram_block;
#endif
    assert(ram->cheri_tags == NULL && "Already initialized?");

#ifndef CONFIG_USER_ONLY
    if (!_need_concurrent_tags_initialized) {
        _need_concurrent_tags =
            qemu_tcg_mttcg_enabled() && current_machine->smp.max_cpus > 1;
        _need_concurrent_tags_initialized = true;
    } else {
        assert(_need_concurrent_tags ==
               (qemu_tcg_mttcg_enabled() && current_machine->smp.max_cpus > 1));
    }
#endif

    size_t cheri_ntagblks = num_tagblocks(ram);
    ram->cheri_tags =
        g_malloc0(cheri_ntagblks * sizeof(CheriTagBlock *));
    if (ram->cheri_tags == NULL) {
        error_report("%s: Can't allocated tag memory", __func__);
        exit(-1);
    }
}

#ifdef CONFIG_USER_ONLY
void cheri_tag_free(RAMBlock *ram)
{
    CheriTagBlock **tagmem;
    size_t ii;

    assert(ram != NULL);
    assert(ram->cheri_tags != NULL);

    tagmem = (CheriTagBlock **)ram->cheri_tags;
    for (ii = 0; ii < num_tagblocks(ram); ii++) {
        if (tagmem[ii] == NULL)
            continue;
        g_free(tagmem[ii]);
    }
    g_free(ram->cheri_tags);
}
#endif

void *cheri_tagmem_for_addr(CPUArchState *env, target_ulong vaddr,
                            RAMBlock *ram, ram_addr_t ram_offset, size_t size,
                            int *prot, bool tag_write)
{

    if (unlikely(!ram || !ram->cheri_tags)) {
        /* Tags stored here are effectively cleared (unless they should trap) */
        if (!(*prot & PAGE_SC_TRAP)) {
            *prot |= PAGE_SC_CLEAR;
        }
        if (tag_write) {
            error_report("Attempting change tag bit on memory without tags:");
#ifdef CONFIG_USER_ONLY
            error_report("%s: vaddr=0x%jx -> 0x%jx", __func__,
                         (uintmax_t)vaddr, (uintmax_t)ram_offset);
#else
            error_report("%s: vaddr=0x%jx -> %s+0x%jx", __func__,
                         (uintmax_t)vaddr, ram ? ram->idstr : NULL,
                         (uintmax_t)ram_offset);
#endif
        }
        return ALL_ZERO_TAGBLK;
    }

    uint64_t tag = ram_offset / CHERI_CAP_SIZE;
#ifndef TARGET_AARCH64
    // AArch64 seems to use different sizes. Might be worth looking into.
    cheri_debug_assert(size == TARGET_PAGE_SIZE && "Unexpected size");
#endif
    CheriTagBlock *tagblk = cheri_tag_block(tag, ram);

    if (tag_write && !tagblk) {
        cheri_tag_new_tagblk(ram, tag);
        CPUState *cpu = env_cpu(env);
        /*
         * A vaddr-based shootdown is insufficient as multiple mappings may
         * exist. Short of an inverted table, a complete shootdown is required.
         */
        tlb_flush_all_cpus_synced(cpu);
        /*
         * An un-synced flush the current cpu is required as we want to complete
         * this instruction and THEN exit.
         */
        tlb_flush(cpu);
        tagblk = cheri_tag_block(tag, ram);
        cheri_debug_assert(tagblk);
    }

    if (tagblk != NULL) {
        const size_t tagblk_index = CAP_TAGBLK_IDX(tag);
        if (need_concurrent_tags()) {
            return tagblk->locked_tags + tagblk_index;
        } else {
            return tagblk->tag_bitmap + BIT_WORD(tagblk_index);
        }
    }

    if (!(*prot & PAGE_SC_CLEAR)) {
        // Add in a (fake) SC_TRAP to prompt a TLB refill if a tag is stored
        // to this location. See the comment around TLBENTRYCAP_INVALID_WRITE_*.
        *prot |= PAGE_SC_TRAP;
    }

    return ALL_ZERO_TAGBLK;
}

#ifndef CONFIG_USER_ONLY
static inline void *get_tagmem_from_iotlb_entry(CPUArchState *env,
                                                target_ulong vaddr, int mmu_idx,
                                                bool isWrite,
                                                uintptr_t *flags_out)
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
    g_assert(tlb_hit(isWrite ? tlb_addr_write(entry) : entry->addr_read, vaddr));
#endif
    CPUIOTLBEntry *iotlbentry =
        &env_tlb(env)->d[mmu_idx].iotlb[tlb_index(env, mmu_idx, vaddr)];
    if (isWrite) {
        *flags_out = IOTLB_GET_TAGMEM_FLAGS(iotlbentry, write);
        return IOTLB_GET_TAGMEM(iotlbentry, write);
    } else {
        *flags_out = IOTLB_GET_TAGMEM_FLAGS(iotlbentry, read);
        return IOTLB_GET_TAGMEM(iotlbentry, read);
    }
}
#endif

/*
 * Returns a pointer to a block of tags that corresponds to a page including
 * vaddr.
 */
static inline void *cheri_tag_get_tagmem(CPUArchState *env,
                                         target_ulong vaddr, int mmu_idx,
                                         bool isWrite,
                                         uintptr_t *flags_out)
{
#ifdef CONFIG_USER_ONLY
    RAMBlock *ram;
    void *host_address, *tagmem;
    ram_addr_t ram_address, ram_offset;
    int prot;
    target_ulong vaddr_page;

    vaddr_page = (vaddr & TARGET_PAGE_MASK);
    host_address = g2h(vaddr_page);
    ram_address = qemu_ram_addr_from_host(host_address);
    ram = qemu_get_ram_block(ram_address);
    if (ram != NULL) {
        ram_offset = qemu_ram_block_host_offset(ram, host_address);
    } else {
        ram_offset = ram_address;
    }
    prot = 0;
    tagmem = cheri_tagmem_for_addr(env, vaddr_page, ram, ram_offset,
        TARGET_PAGE_SIZE, &prot, isWrite);
    *flags_out = 0;
    if ((prot & (PAGE_LC_CLEAR | PAGE_SC_CLEAR)) != 0) {
        *flags_out |= TLBENTRYCAP_FLAG_CLEAR;
    }
    if ((prot & (PAGE_LC_TRAP | PAGE_SC_TRAP)) != 0) {
        *flags_out |= TLBENTRYCAP_FLAG_TRAP;
    }
    if ((prot & PAGE_LC_TRAP_ANY) != 0) {
        *flags_out |= TLBENTRYCAP_FLAG_TRAP_ANY;
    }
    return (tagmem);
#else

    return (get_tagmem_from_iotlb_entry(env, vaddr, mmu_idx, isWrite,
                                        flags_out));
#endif
}

typedef struct TagOffset {
    target_ulong value;
} TagOffset;

static QEMU_ALWAYS_INLINE TagOffset addr_to_tag_offset(target_ulong addr)
{
    return (struct TagOffset){.value = addr / CHERI_CAP_SIZE};
}

static QEMU_ALWAYS_INLINE target_ulong
page_vaddr_to_tag_offset(target_ulong addr)
{
    return (addr & ~TARGET_PAGE_MASK) / CHERI_CAP_SIZE;
}

static QEMU_ALWAYS_INLINE target_ulong tag_offset_to_addr(TagOffset offset)
{
    return offset.value * CHERI_CAP_SIZE;
}

static void *cheri_tag_invalidate_one(CPUArchState *env, target_ulong vaddr,
                                      uintptr_t pc, int mmu_idx,
                                      tag_writer_lock_t *lock, bool lock_only);

static void invalidate_from_locktag(lock_tag *lock)
{
    if (lock != TAG_LOCK_NONE) {
        assert(lock != TAG_LOCK_ERROR);
        lock_tag_write_tag_and_release(lock, 0);
    }
}

void *cheri_tag_invalidate_aligned_impl(CPUArchState *env, target_ulong vaddr,
                                        uintptr_t pc, int mmu_idx,
                                        tag_writer_lock_t *lock, bool lock_only)
{
    cheri_debug_assert(QEMU_IS_ALIGNED(vaddr, CHERI_CAP_SIZE));
    if (lock && *lock) {
        invalidate_from_locktag((lock_tag *)*lock);
        return NULL;
    }
    return cheri_tag_invalidate_one(env, vaddr, pc, mmu_idx, lock, lock_only);
}

void cheri_tag_invalidate_impl(CPUArchState *env, target_ulong vaddr,
                               int32_t size, uintptr_t pc, int mmu_idx,
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
        cheri_tag_invalidate_one(env, vaddr, pc, mmu_idx, first, lock_only);
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
        cheri_tag_invalidate_one(env, addr, pc, mmu_idx, first, lock_only);
        first = second;
    }
}

static void *cheri_tag_invalidate_one(CPUArchState *env, target_ulong vaddr,
                                      uintptr_t pc, int mmu_idx,
                                      tag_writer_lock_t *lock, bool lock_only)
{

    if (lock)
        *lock = TAG_LOCK_NONE;

    /*
     * When resolving this address in the TLB, treat it like a data store
     * (MMU_DATA_STORE) rather than a capability store (MMU_DATA_CAP_STORE),
     * so that we don't require that the SC inhibit be clear.
     */

    /*
     * We use the flags version to suppress exceptions if we only want the
     * lock. This is required as the data access might throw an exception with a
     * higher priority. It should be the case that the data access will throw an
     * MMU exception as well, but if it doesn't, I am returning another special
     * lock value which can be tested later
     * */
    void *host_addr;
    int access_flags = probe_access_flags(env, vaddr, MMU_DATA_STORE, mmu_idx,
                                          lock_only, &host_addr, pc);

    if (access_flags & TLB_INVALID_MASK) {
        if (lock)
            *lock = TAG_LOCK_ERROR;
        return host_addr;
    }

    // Only RAM and ROM regions are backed by host addresses so if
    // probe_write() returns NULL we know that we can't write the tagmem.
    if (unlikely(!host_addr)) {
        return host_addr;
    }

    uintptr_t tagmem_flags;
    void *tagmem = cheri_tag_get_tagmem(env, vaddr, mmu_idx, true,
                                        &tagmem_flags);

    if (tagmem == ALL_ZERO_TAGBLK) {
        // All tags for this page are zero -> no need to invalidate. We also
        // couldn't invalidate if we wanted to since ALL_ZERO_TAGBLK is not a
        // valid pointer but a magic constant.
        return host_addr;
    }

    cheri_debug_assert(!(tagmem_flags & TLBENTRYCAP_FLAG_CLEAR) &&
                       "Unimplemented");

    // We cached the tagblock address in the iotlb -> find find the tag
    // bit: The iotlb->tagmem pointer always points to the tag memory
    // for the start of the page so we can simply add the index for the
    // page offset.
    target_ulong tag_offset = page_vaddr_to_tag_offset(vaddr);
    if (qemu_log_instr_enabled(env)) {
        bool old_value;
        tag_reader_lock_t read_lock = NULL;
        /*
         * A read lock must be taken if the tag isn't already locked, i.e. this
         * function is called with a lock to be set.
         */
        old_value = tagmem_get_tag(tagmem, tag_offset,
            lock ? &read_lock : NULL);
        qemu_log_instr_extra(
            env,
            "    Cap Tag Write [" TARGET_FMT_lx "/" RAM_ADDR_FMT "] %d -> 0\n",
            vaddr, qemu_ram_addr_from_host(host_addr), old_value);
        if (read_lock != NULL && read_lock != TAG_LOCK_NONE) {
            cheri_tag_reader_lock_release_impl(read_lock);
        }
    }

    tagmem_clear_tag(tagmem, tag_offset, lock, lock_only);
    return host_addr;
}

#ifndef CONFIG_USER_ONLY
void cheri_tag_phys_invalidate(CPUArchState *env, RAMBlock *ram,
                               ram_addr_t ram_offset, ram_addr_t len,
                               const target_ulong *vaddr)
{
    // Ignore tag clearing requests for ROM, etc.
    if (!ram->cheri_tags) {
        return;
    }
#ifndef CONFIG_USER_ONLY
    cheri_debug_assert(!memory_region_is_rom(ram->mr) &&
                       !memory_region_is_romd(ram->mr));
#endif

    ram_addr_t endaddr = (uint64_t)(ram_offset + len);
    ram_addr_t startaddr = QEMU_ALIGN_DOWN(ram_offset, CHERI_CAP_SIZE);

    for(ram_addr_t addr = startaddr; addr < endaddr; addr += CHERI_CAP_SIZE) {
        uint64_t tag = addr / CHERI_CAP_SIZE;
        CheriTagBlock *tagblk = cheri_tag_block(tag, ram);
        if (tagblk != NULL) {
            const size_t tagblk_index = CAP_TAGBLK_IDX(tag);
            if (unlikely(env && qemu_log_instr_enabled(env))) {
                bool old_tag =
                    need_concurrent_tags()
                        ? tagblock_get_locktag(tagblk, tagblk_index, NULL)
                        : tagblock_get_tag(tagblk, tagblk_index);
                if (vaddr) {
                    target_ulong write_vaddr =
                        QEMU_ALIGN_DOWN(*vaddr, CHERI_CAP_SIZE) +
                        (addr - startaddr);
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

            if (need_concurrent_tags()) {
                tagblock_clear_locktag_with_lock_and_release(tagblk,
                                                             tagblk_index);
            } else {
                tagblock_clear_tag(tagblk, tagblk_index);
            }
        }
    }
}
#endif

/*
 * TODO: Basically nothing uses this physical address. Tag set probably should
 * not have to return it.
 */
#ifdef CONFIG_USER_ONLY
#define handle_paddr_return(rw) vaddr
#else
#define handle_paddr_return(rw)                                                \
    do {                                                                       \
        if (ret_paddr) {                                                       \
            *ret_paddr = (vaddr & ~TARGET_PAGE_MASK) |                         \
                         (tlb_entry(env, mmu_idx, vaddr)->addr_##rw &          \
                          TARGET_PAGE_MASK);                                   \
        }                                                                      \
    } while (0)
#endif

#ifdef TARGET_MIPS
#define store_capcause_reg(env, reg) cpu_mips_store_capcause_reg(env, reg)
#define clear_capcause_reg(env) cpu_mips_clear_capcause_reg(env)
#else
#define store_capcause_reg(env, reg)
#define clear_capcause_reg(env)
#endif

void *cheri_tag_set_impl(CPUArchState *env, target_ulong vaddr, int reg,
                         hwaddr *ret_paddr, uintptr_t pc, int mmu_idx,
                         tag_writer_lock_t *lock, bool lock_only)
{

    if (lock && *lock) {
        if (*lock != TAG_LOCK_NONE)
            lock_tag_write_tag_and_release((lock_tag *)*lock, 1);
        return NULL;
    }

    /*
     * This attempt to resolve a virtual address may cause both a data store
     * TLB fault (entry missing or D bit clear) and a capability store TLB
     * fault (SC bit set).
     */
    store_capcause_reg(env, reg);
    // Note: this probe will handle any store cap faults
    void *host_addr = probe_cap_write(env, vaddr, 1, mmu_idx, pc);
    clear_capcause_reg(env);

    handle_paddr_return(write);

    if (lock)
        *lock = TAG_LOCK_NONE;

    if (unlikely(!host_addr)) {
        return NULL;
    }

    uintptr_t tagmem_flags;
    void *tagmem = cheri_tag_get_tagmem(env, vaddr, mmu_idx, /*write=*/true,
            &tagmem_flags);

    /* Clear + ALL_ZERO_TAGBLK means no tags can be stored here. */
    if ((tagmem_flags & TLBENTRYCAP_FLAG_CLEAR) &&
        (tagmem == ALL_ZERO_TAGBLK)) {
        return host_addr;
    }

    cheri_debug_assert(!(tagmem_flags & TLBENTRYCAP_FLAG_CLEAR) &&
                       "Unimplemented");

    cheri_debug_assert(!(tagmem_flags & TLBENTRYCAP_FLAG_TRAP));

    /*
     * probe_cap_write() should have ensured there was a tagmem for this
     * location. A NULL ram should have been indicated via
     * TLBENTRYCAPFLAG_CLEAR or TLBENTRYCAPFLAG_TRAP.
     */
    cheri_debug_assert(tagmem != ALL_ZERO_TAGBLK);

    target_ulong tag_offset = page_vaddr_to_tag_offset(vaddr);

    if (qemu_log_instr_enabled(env)) {
        bool old_value;
        tag_reader_lock_t read_lock = NULL;
        /*
         * A read lock must be taken if the tag isn't already locked, i.e. this
         * function is called with a lock to be set.
         */
        old_value = tagmem_get_tag(tagmem, tag_offset,
            lock ? &read_lock : NULL);
        qemu_maybe_log_instr_extra(
            env, "    Cap Tag Write [" TARGET_FMT_lx "/" RAM_ADDR_FMT "] %d -> 1\n",
            vaddr, qemu_ram_addr_from_host(host_addr),
            old_value);
        if (read_lock != NULL && read_lock != TAG_LOCK_NONE) {
            cheri_tag_reader_lock_release_impl(read_lock);
        }
    }

    tagmem_set_tag(tagmem, tag_offset, lock, lock_only);
    return host_addr;
}

bool cheri_tag_get(CPUArchState *env, target_ulong vaddr, int reg,
                   hwaddr *ret_paddr, int *prot, uintptr_t pc, int mmu_idx,
                   void *host_addr, tag_reader_lock_t *lock)
{
    if (lock && *lock) {
        return (*lock == TAG_LOCK_NONE)
                   ? 0
                   : lock_tag_release_read((lock_tag *)*lock);
    }

    if (host_addr == NULL) {
        host_addr = probe_read(env, vaddr, 1, mmu_idx, pc);
    }
    handle_paddr_return(read);

    uintptr_t tagmem_flags;
    void *tagmem = cheri_tag_get_tagmem(env, vaddr, mmu_idx,
                                        /*write=*/false, &tagmem_flags);

    if (prot) {
        *prot = 0;
        if (tagmem_flags & TLBENTRYCAP_FLAG_CLEAR) {
            *prot |= PAGE_LC_CLEAR;
        }
        if (tagmem_flags & TLBENTRYCAP_FLAG_TRAP) {
            *prot |= PAGE_LC_TRAP;
        }
        if (tagmem_flags & TLBENTRYCAP_FLAG_TRAP_ANY) {
            *prot |= PAGE_LC_TRAP_ANY;
        }
    }

    /*
     * Squash happens in the caller, so read the tagblk even if
     * TLBENTRYCAP_FLAG_CLEAR
     */
    bool result =
        (tagmem == ALL_ZERO_TAGBLK)
            ? 0
            : tagmem_get_tag(tagmem, page_vaddr_to_tag_offset(vaddr), lock);

    // XXX: Not atomic w.r.t. writes to tag memory
    qemu_maybe_log_instr_extra(
        env, "    Cap Tag Read [" TARGET_FMT_lx "/" RAM_ADDR_FMT "] -> %d\n",
        vaddr, qemu_ram_addr_from_host(host_addr), result);
    return result;
}

int cheri_tag_get_many(CPUArchState *env, target_ulong vaddr, int reg,
        hwaddr *ret_paddr, uintptr_t pc)
{

    const int mmu_idx = cpu_mmu_index(env, false);
    probe_read(env, vaddr, CAP_TAG_MANY_DATA_SIZE, mmu_idx, pc);
    handle_paddr_return(read);

    uintptr_t tagmem_flags;
    void *tagmem = cheri_tag_get_tagmem(env, vaddr, mmu_idx,
                                        /*write=*/false, &tagmem_flags);

    int result =
        ((tagmem == ALL_ZERO_TAGBLK) || (tagmem_flags & TLBENTRYCAP_FLAG_CLEAR))
            ? 0
            : tagmem_get_tag_many(tagmem, page_vaddr_to_tag_offset(vaddr),
                                  false);

    if ((result && (tagmem_flags & TLBENTRYCAP_FLAG_TRAP)) ||
        (tagmem_flags & TLBENTRYCAP_FLAG_TRAP_ANY)) {
        raise_load_tag_exception(env, vaddr, reg, pc);
    }

    return result;
}

void cheri_tag_set_many(CPUArchState *env, uint32_t tags, target_ulong vaddr,
                        int reg, hwaddr *ret_paddr, uintptr_t pc)
{
    tags &= CAP_TAG_GET_MANY_MASK;

    const int mmu_idx = cpu_mmu_index(env, false);
    store_capcause_reg(env, reg);
    /*
     * We call probe_(cap)_write rather than probe_access since the branches
     * checking access_type can be eliminated.
     */
    if (tags) {
        // Note: this probe will handle any store cap faults
        probe_cap_write(env, vaddr, CAP_TAG_MANY_DATA_SIZE, mmu_idx, pc);
    } else {
        probe_write(env, vaddr, CAP_TAG_MANY_DATA_SIZE, mmu_idx, pc);
    }
    clear_capcause_reg(env);

    handle_paddr_return(write);

    uintptr_t tagmem_flags;
    void *tagmem = cheri_tag_get_tagmem(env, vaddr, mmu_idx, true,
                                        &tagmem_flags);

    if ((tagmem_flags & TLBENTRYCAP_FLAG_CLEAR) &&
        (tagmem == ALL_ZERO_TAGBLK)) {
        return;
    }

    if (!tags && tagmem == ALL_ZERO_TAGBLK) {
        return;
    }

    cheri_debug_assert(!(tagmem_flags & TLBENTRYCAP_FLAG_CLEAR) &&
                       "Unimplemented");

    cheri_debug_assert(!(tagmem_flags & TLBENTRYCAP_FLAG_TRAP));

    cheri_debug_assert(tagmem);

    tagmem_set_tag_many(tagmem, page_vaddr_to_tag_offset(vaddr), tags, false);
}

void cheri_tag_reader_lock_release_impl(tag_reader_lock_t lock)
{
    lock_tag *lockTag = (lock_tag *)lock;
    lock_tag_release_read(lockTag);
}
void cheri_tag_writer_lock_release_impl(tag_writer_lock_t lock)
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

/*
 * Some TCG operations on data need to be atomic. This is done using atomic host
 * operations, or single stepping. Tags pose a problem, as no host operations
 * can simultaneously set tags and data, so that would leave only single
 * stepping, which would not perform very well.
 * Luckily, a lock now exists on every capability sized unit of memory, so
 * atomic TCG operations are done by taking the lock and then doing a non-atomic
 * sequence. Because it is unknown at translation time whether a location will
 * have a tag (and therefore a lock), we cannot know whether we can rely
 * on the tags lock or not.
 * Therefore, we assume every location is tagged. If it is not, we asign it a
 * dummy lock (that will always have a zero tag).
 * We have a few of these to avoid false sharing.
 */

#define DUMMY_LOCK_BITS 16

lock_tag dummy_tag_table[1 << DUMMY_LOCK_BITS];

static target_ulong hash_vaddr(target_ulong vaddr)
{
    size_t shft = sizeof(target_ulong);
    while (shft > DUMMY_LOCK_BITS) {
        shft /= 2;
        vaddr ^= vaddr >> shft;
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
    target_ulong tag_addr1 = vaddr / CHERI_CAP_SIZE;
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

bool cheri_tag_get_debug(RAMBlock *ram, ram_addr_t ram_offset)
{
    /* Return zero tag for ROM, etc. */
    if (!ram->cheri_tags) {
        return false;
    }
#ifndef CONFIG_USER_ONLY
    cheri_debug_assert(!memory_region_is_rom(ram->mr) &&
                       !memory_region_is_romd(ram->mr));
#endif

    cheri_debug_assert(QEMU_ALIGN_DOWN(ram_offset, CHERI_CAP_SIZE) ==
                       ram_offset);

    uint64_t tag = ram_offset / CHERI_CAP_SIZE;
    CheriTagBlock *tagblk = cheri_tag_block(tag, ram);
    const size_t tagblk_index = CAP_TAGBLK_IDX(tag);
    return tagblock_get_tag(tagblk, tagblk_index);
}
