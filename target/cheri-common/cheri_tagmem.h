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
void cheri_tag_invalidate(CPUArchState *env, target_ulong vaddr, int32_t size,
                          uintptr_t pc);
bool cheri_tag_get(CPUArchState *env, target_ulong vaddr, int reg,
                   hwaddr *ret_paddr, int *prot, uintptr_t pc);
int cheri_tag_get_many(CPUArchState *env, target_ulong vaddr, int reg,
                       hwaddr *ret_paddr, uintptr_t pc);
void cheri_tag_set(CPUArchState *env, target_ulong vaddr, int reg,
                   hwaddr *ret_paddr, uintptr_t pc);
#ifdef CHERI_MAGIC128
bool cheri_tag_get_m128(CPUArchState *env, target_ulong vaddr, int reg,
        uint64_t *tps, uint64_t *length, hwaddr *ret_paddr, int *prot, uintptr_t pc);
void cheri_tag_set_m128(CPUArchState *env, target_ulong vaddr, int reg,
        uint8_t tag, uint64_t tps, uint64_t length, uintptr_t pc);
#endif /* CHERI_MAGIC128 */
#endif /* TARGET_CHERI */
