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

#pragma once

#include "qemu/osdep.h"
#include "cpu.h"
#include "exec/cpu-common.h"
#include "exec/memory.h"
#include "cheri_tagmem_ex_locks.h"

#if defined(TARGET_CHERI)

/*
 * TODO: We might want to think about the performance correctness trade-off
 * here. On multicore, we always want to use locking.
 * For singlecore, it would be more performant to have no locks. Not only could
 * we cram the tags into less memory, but code paths would be simplified.
 * However, there is still a race between normal tag access and the mass clear
 * in cheri_tag_phys_invalidate, due to IO threads. In order to be secure,
 * we could turn on locks. This might be valuable for people looking to test the
 * architecture. However, this race only matters as it may create extra tags for
 * memory written by devices. This shouldn't actually break anything. So if
 * developers want a "fast but not precise" singlecore, we could have locks off.
 *
 * If we want to throw caution to wind and really optimise the single thread
 * case, it should be noted qemu_tcg_mttcg_enabled is dynamic and so will
 * still be generating an extra branch. TARGET_SUPPORTS_MTTCG is the
 * static thing. Although qemu_tcg_mttcg_enabled will default to false if
 * TARGET_SUPPORTS_MTTCG is undefined, it can be still be overridden on the
 * command line.
 */

#define UNSAFE_SINGLE_CORE true

static bool tagmem_use_locking(void)
{
    /* return constant true here to always be correct, but slower than needed
     * on single core */
    if (UNSAFE_SINGLE_CORE)
        return qemu_tcg_mttcg_enabled();
    else
        return true;
}

/*
 * Usage notes:
 *
 * There are few different locking patterns that might be employed, depending
 * on whether TCG is currently multithreaded, whether reading / writing tags,
 * whether exceptions can be thrown etc.
 *
 * tag get / set / invalidate have an (optional) lock argument.
 * If it is NULL, then no locks will be taken or released
 * (although they should be already taken elsewhere if data is also to be
 * read/written). If provided, it can either be an input (not NULL when
 * dereferenced) or an output. If an input, then the lock is to be released
 * atomically with the operation. If an output, then the lock will be taken and
 * returned to be released by the caller.
 *
 * The cheri_lock_for_X family will return the relevent lock for an intended
 * operation that needs to be performed at the end of a sequence, but will not
 * apply the operation. cheri_lock_for_tag_set can be used for invalidate,
 * but not the other way around.
 *
 * Any lock taken, either with lock_for_X, or as an output from other tag
 * operations needs to be freed by the caller. This can be done by calling
 * cheri_tag_reader_release / cheri_tag_writer_release, if no more updates need
 * to happen. Otherwise, using tag get / set / invalidate with an input will
 * release a lock. Finally, if exceptions may occur, use push_free_on_exception
 * before such an exception, and pop_free_on_exception after the possibly
 * exception throwing behavior is finished.
 */

// Opaque types for locks
typedef void *tag_reader_lock_t;
typedef void *tag_writer_lock_t;

// A magic value meaning there is no lock for a particular tag
// Useful to be different from NULL.
#define TAG_LOCK_NONE ((void *)-1)
// This is a lock indicating some kind of error in lookup that got supressed
#define TAG_LOCK_ERROR ((void *)-2)

// If we for some reason require a tag for untagged memory
void get_dummy_locks(target_ulong vaddr, tag_writer_lock_t *low,
                     tag_writer_lock_t *high);

extern tag_writer_lock_t notag_lock_high;

void cheri_tag_reader_lock_impl(tag_reader_lock_t lock);
void cheri_tag_writer_lock_impl(tag_writer_lock_t lock);

static inline void cheri_tag_reader_release(tag_reader_lock_t lock)
{
    if (lock != TAG_LOCK_NONE && lock != NULL)
        cheri_tag_reader_lock_impl(lock);
}

static inline void cheri_tag_writer_release(tag_writer_lock_t lock)
{
    if (lock != TAG_LOCK_NONE && lock != NULL)
        cheri_tag_writer_lock_impl(lock);
}

static inline cheri_exception_locks_t *
cheri_get_exception_locks(CPUArchState *env)
{
    return &env_cpu(env)->cheri_exception_locks;
}

static inline void cheri_tag_push_free_on_exception(CPUArchState *env,
                                                    tag_lock_t lock)
{
    if (tagmem_use_locking()) {
        cheri_exception_locks_t *locks = cheri_get_exception_locks(env);
        assert(locks->fill != MAX_CHERI_EXCEPTION_LOCKS);
        locks->locks[locks->fill++] = lock;
    }
}

static inline tag_lock_t cheri_tag_pop_on_exception(CPUArchState *env)
{
    if (tagmem_use_locking()) {
        cheri_exception_locks_t *locks = cheri_get_exception_locks(env);
        assert(locks->fill != 0);
        tag_lock_t lock = locks->locks[--locks->fill];
        locks->locks[locks->fill] = NULL;
        return lock;
    }
    return NULL;
}

void cheri_tag_free_lock(tag_lock_t lock);

static inline void cheri_tag_locks_exception_thrown(CPUState *cpu)
{
    if (!tagmem_use_locking())
        return;
    cheri_exception_locks_t *locks = &cpu->cheri_exception_locks;
    for (size_t i = 0; i != MAX_CHERI_EXCEPTION_LOCKS; i++) {
        if (locks->locks[i] && locks->locks[i] != TAG_LOCK_NONE)
            cheri_tag_free_lock(locks->locks[i]);
        locks->locks[i] = NULL;
    }
    locks->fill = 0;
}

static inline void
cheri_tag_reader_push_free_on_exception(CPUArchState *env,
                                        tag_reader_lock_t lock)
{
    cheri_tag_push_free_on_exception(env, (tag_lock_t)lock);
}

static inline void
cheri_tag_writer_push_free_on_exception(CPUArchState *env,
                                        tag_writer_lock_t lock)
{
    cheri_tag_push_free_on_exception(env, (tag_lock_t)lock);
}

static inline tag_reader_lock_t
cheri_tag_reader_pop_free_on_exception(CPUArchState *env)
{
    return (tag_reader_lock_t)cheri_tag_pop_on_exception(env);
}

static inline tag_writer_lock_t
cheri_tag_writer_pop_free_on_exception(CPUArchState *env)
{
    return (tag_writer_lock_t)cheri_tag_pop_on_exception(env);
}

#define cheri_tag_assert_not_mttcg() assert(!qemu_tcg_mttcg_enabled());

/* Note: for cheri_tag_phys_invalidate, env may be NULL */
void cheri_tag_phys_invalidate(CPUArchState *env, RAMBlock *ram,
                               ram_addr_t offset, size_t len,
                               const target_ulong *vaddr);
void cheri_tag_init(MemoryRegion *mr, uint64_t memory_size);

/**
 * Generic tag invalidation function to be called for a *single* data store:
 * Note: this will currently invalidate at most two tags (as can happen
 * for a unaligned store that crosses a CHERI_CAP_SIZE alignment boundary).
 */
void cheri_tag_invalidate_impl(CPUArchState *env, target_ulong vaddr,
                               int32_t size, uintptr_t pc, int mmu_idx,
                               tag_writer_lock_t *first,
                               tag_writer_lock_t *second, bool lock_only);

static inline void cheri_tag_invalidate(CPUArchState *env, target_ulong vaddr,
                                        int32_t size, uintptr_t pc, int mmu_idx,
                                        tag_writer_lock_t *first,
                                        tag_writer_lock_t *second)
{
    cheri_tag_invalidate_impl(env, vaddr, size, pc, mmu_idx, first, second,
                              false);
}

static inline void cheri_lock_for_tag_invalidate(
    CPUArchState *env, target_ulong vaddr, int32_t size, uintptr_t pc,
    int mmu_idx, tag_writer_lock_t *first, tag_writer_lock_t *second)
{
    if (!tagmem_use_locking()) {
        if (first)
            *first = NULL;
        if (second)
            *second = NULL;
    } else {
        cheri_tag_invalidate_impl(env, vaddr, size, pc, mmu_idx, first, second,
                                  true);
    }
}

/**
 * If probe_read() has already been called, the result can be passed as the
 * @p host_addr argument to avoid another (expensive) probe_read() call.
 */

void *cheri_tag_invalidate_aligned_impl(CPUArchState *env, target_ulong vaddr,
                                        uintptr_t pc, int mmu_idx,
                                        tag_writer_lock_t *lock,
                                        bool lock_only);

/**
 * Like cheri_tag_invalidate, but the address must be aligned and it will only
 * invalidate a single tag (i.e. no unaligned accesses). A bit faster since it
 * can avoid some branches.
 * @return the host address as returned by probe_write().
 */
static inline void *cheri_tag_invalidate_aligned(CPUArchState *env,
                                                 target_ulong vaddr,
                                                 uintptr_t pc, int mmu_idx,
                                                 tag_writer_lock_t *lock)
{
    return cheri_tag_invalidate_aligned_impl(env, vaddr, pc, mmu_idx, lock,
                                             false);
}

static inline void *
cheri_lock_for_tag_invalidate_aligned(CPUArchState *env, target_ulong vaddr,
                                      uintptr_t pc, int mmu_idx,
                                      tag_writer_lock_t *lock)
{
    if (!tagmem_use_locking()) {
        *lock = NULL;
        return NULL;
    } else {
        return cheri_tag_invalidate_aligned_impl(env, vaddr, pc, mmu_idx, lock,
                                                 true);
    }
}

/* Get a single tag */

bool cheri_tag_get(CPUArchState *env, target_ulong vaddr, int reg,
                   hwaddr *ret_paddr, int *prot, uintptr_t pc, int mmu_idx,
                   void *host_addr, tag_reader_lock_t *lock);

static inline void cheri_lock_for_tag_get(CPUArchState *env, target_ulong vaddr,
                                          int reg, hwaddr *ret_paddr, int *prot,
                                          uintptr_t pc, int mmu_idx,
                                          void *host_addr,
                                          tag_reader_lock_t *lock)
{
    if (!tagmem_use_locking()) {
        *lock = NULL;
    } else {
        cheri_tag_get(env, vaddr, reg, ret_paddr, prot, pc, mmu_idx, host_addr,
                      lock);
    }
}

/* Set a tag to one
 * @return the host address as returned by probe_cap_write()
 */
void *cheri_tag_set_impl(CPUArchState *env, target_ulong vaddr, int reg,
                         hwaddr *ret_paddr, uintptr_t pc, int mmu_idx,
                         tag_writer_lock_t *lock, bool lock_only);

/**
 * Update a tag for virtual address @vaddr.
 * @return the host address as returned by probe_cap_write()
 */
static inline void *cheri_tag_set(CPUArchState *env, target_ulong vaddr,
                                  int reg, hwaddr *ret_paddr, uintptr_t pc,
                                  int mmu_idx, tag_writer_lock_t *lock)
{
    return cheri_tag_set_impl(env, vaddr, reg, ret_paddr, pc, mmu_idx, lock,
                              false);
}

static inline void cheri_lock_for_tag_set(CPUArchState *env, target_ulong vaddr,
                                          int reg, hwaddr *ret_paddr,
                                          uintptr_t pc, int mmu_idx,
                                          tag_writer_lock_t *lock)
{
    if (!tagmem_use_locking()) {
        *lock = NULL;
    } else {
        cheri_tag_set_impl(env, vaddr, reg, ret_paddr, pc, mmu_idx, lock, true);
    }
}

/* Get and set many do not take any locks, as they are not meant for use in
 * conjunction with modififying data. They are atomic where possible, but
 * no guarantee is made
 * Get/set many currently don't have an mmu_idx because no targets currently
 * require it. */

int cheri_tag_get_many(CPUArchState *env, target_ulong vaddr, int reg,
                       hwaddr *ret_paddr, uintptr_t pc);
void cheri_tag_set_many(CPUArchState *env, uint32_t tags, target_ulong vaddr,
                        int reg, hwaddr *ret_paddr, uintptr_t pc);

void *cheri_tagmem_for_addr(CPUArchState *env, target_ulong vaddr,
                            RAMBlock *ram, ram_addr_t ram_offset, size_t size,
                            int *prot, bool tag_write);

#else /* !TARGET_CHERI */

#define cheri_tag_locks_exception_thrown(...)

#endif /* TARGET_CHERI */
