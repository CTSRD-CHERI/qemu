/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2020 Alex Richardson
 * All rights reserved.
 *
 * This software was developed by SRI International and the University of
 * Cambridge Computer Laboratory (Department of Computer Science and
 * Technology) under DARPA contract HR0011-18-C-0016 ("ECATS"), as part of the
 * DARPA SSITH research programme.
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
#include "tcg/tcg.h"
#include "tcg/tcg-op.h"
// Note: Due to cyclic dependencies we can only use DisasContextBase in this
// file and must avoid anything defined in translate.c as it is also included
// from accel/tcg/translator.c.

void cheri_tcg_prepare_for_unconditional_exception(DisasContextBase *db);
void cheri_tcg_save_pc(DisasContextBase *db);

#ifdef TARGET_CHERI
static inline bool in_pcc_bounds(DisasContextBase *db, target_ulong addr)
{
    return addr >= db->pcc_base && addr < db->pcc_top;
}

// Raise a bounds violation exception on PCC
static inline void gen_raise_pcc_violation_tcgv(DisasContextBase *db,
                                                TCGv taddr, uint32_t num_bytes)
{
    // Ensure correct PCC.cursor
    cheri_tcg_save_pc(db);
    TCGv_i32 tbytes = tcg_const_i32(num_bytes);
    gen_helper_raise_exception_pcc_bounds(cpu_env, taddr, tbytes);
    tcg_temp_free_i32(tbytes);
    // Note: we don't set DISAS_NORETURN (must be called before helper) since
    // this helper function might only be called in a conditional branch
}

static inline void gen_raise_pcc_violation(DisasContextBase *db,
                                           target_ulong addr,
                                           uint32_t num_bytes)
{
    tcg_debug_assert(!in_pcc_bounds(db, addr + num_bytes) ||
                     !in_pcc_bounds(db, addr));
    TCGv taddr = tcg_const_tl(addr);
    gen_raise_pcc_violation_tcgv(db, taddr, num_bytes);
    tcg_temp_free(taddr);
}

#endif // TARGET_CHERI
