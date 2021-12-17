/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2015-2016 Stacey Son <sson@FreeBSD.org>
 * Copyright (c) 2016-2018 Alfredo Mazzinghi <am2419@cl.cam.ac.uk>
 * Copyright (c) 2016-2018 Alex Richardson <Alexander.Richardson@cl.cam.ac.uk>
 * Copyright (c) 2021 Microsoft <robert.norton@microsoft.com>
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
#include "qemu/osdep.h"
#include "cpu.h"
#include "exec/cpu-common.h"
#include "exec/memory.h"

#if defined(TARGET_CHERI)
/* Note: for cheri_tag_phys_invalidate, env may be NULL */
void cheri_tag_phys_invalidate(CPUArchState *env, RAMBlock *ram,
                               ram_addr_t offset, size_t len,
                               const target_ulong *vaddr);
void cheri_tag_init(MemoryRegion* mr, uint64_t memory_size);
/**
 * Generic tag invalidation function to be called for a *single* data store:
 * Note: this will currently invalidate at most two tags (as can happen
 * for a unaligned store that crosses a CHERI_CAP_SIZE alignment boundary).
 */
void cheri_tag_invalidate(CPUArchState *env, target_ulong vaddr, int32_t size,
                          uintptr_t pc, int mmu_idx);
/**
 * Like cheri_tag_invalidate, but the address must be aligned and it will only
 * invalidate a single tag (i.e. no unaligned accesses). A bit faster since it
 * can avoid some branches.
 * @return the host address as returned by probe_write().
 */
void *cheri_tag_invalidate_aligned(CPUArchState *env, target_ulong vaddr,
                                   uintptr_t pc, int mmu_idx);
/**
 * If probe_read() has already been called, the result can be passed as the
 * @p host_addr argument to avoid another (expensive) probe_read() call.
 */
bool cheri_tag_get(CPUArchState *env, target_ulong vaddr, int reg,
                   hwaddr *ret_paddr, int *prot, uintptr_t pc, int mmu_idx,
                   void *host_addr);
/*
 * Get/set many currently don't have an mmu_idx because no targets currently
 * require it.
 */
int cheri_tag_get_many(CPUArchState *env, target_ulong vaddr, int reg,
                       hwaddr *ret_paddr, uintptr_t pc);
void cheri_tag_set_many(CPUArchState *env, uint32_t tags, target_ulong vaddr,
                        int reg, hwaddr *ret_paddr, uintptr_t pc);

/**
 * Update a tag for virtual address @vaddr.
 * @return the host address as returned by probe_cap_write()
 */
void *cheri_tag_set(CPUArchState *env, target_ulong vaddr, int reg,
                    hwaddr *ret_paddr, uintptr_t pc, int mmu_idx);

void *cheri_tagmem_for_addr(CPUArchState *env, target_ulong vaddr,
                            RAMBlock *ram, ram_addr_t ram_offset, size_t size,
                            int *prot, bool tag_write, void **vermem_out);

/**
 * Returns the version of the granule-aligned @vaddr
 * @reg XXX is redundant on RISC-V
 * @ret_paddr is used to return the physical address if non-NULL 
 * @prot returns protection attributes form translation. Must not be NULL.
 * May cause translation faults as for data loads.
 */
cap_version_t cheri_version_get_aligned(CPUArchState *env, target_ulong vaddr, int reg,
                  hwaddr *ret_paddr, int *prot, uintptr_t pc);
/**
 * Sets the version of the granule-aligned @vaddr to @val.
 * @reg XXX is redundant on RISC-V
 * @ret_paddr is used to return the physical address fr
 * May cause translation faults as for data stores.
 */
void cheri_version_set_aligned(CPUArchState *env, target_ulong vaddr, int reg, hwaddr* ret_paddr, uintptr_t pc, cap_version_t val);
/**
 * Returns true if the memory version matches the give @expected version for all
 * granules in [vaddr, vaddr+size). May cause translation faults as per the 
 * access type rw.
 */
bool cheri_version_check(CPUArchState *env, target_ulong vaddr, int32_t size,
                         MMUAccessType rw, uintptr_t pc, cap_version_t expected);
#endif /* TARGET_CHERI */
