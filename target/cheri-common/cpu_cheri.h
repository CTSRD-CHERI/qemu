/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2018-2020 Alex Richardson
 * All rights reserved.
 *
 * This software was developed by SRI International and the University of
 * Cambridge Computer Laboratory under DARPA/AFRL contract FA8750-10-C-0237
 * ("CTSRD"), as part of the DARPA CRASH research programme.
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
// This file should be included from cpu.h after CPURISCVState has been defined
#include "cheri_defs.h"

bool cpu_restore_state(CPUState *cpu, uintptr_t host_pc, bool will_exit);
static inline bool pc_is_current(CPUArchState *env);
static inline target_ulong cpu_get_recent_pc(CPUArchState *env);

#ifdef TARGET_CHERI
static inline const cap_register_t *_cheri_get_pcc_unchecked(CPUArchState *env);
#include "cheri-archspecific-early.h"
// Returns a recebt PCC value. This means that the cursor field may be out of
// date (possibly pointing to the beginning of the translation block). However,
// all permissions, flags and bounds are valid and can be accessed.
// This function exists since updating pcc.cursor on every instruction is
// expensive and generally not required. If the current value is needed, it can
// be computed from the return address (GETPC() in the toplevel helper function)
// by calling cpu_restore_state(). However, it is preferable if the translator
// code writes the updated pcc.cursor before calling the helper if this is the
// common case (for example, it should not be saved if the current pc value is
// only needed if a trap is raised).
static inline const cap_register_t *cheri_get_recent_pcc(CPUArchState *env)
{
    return _cheri_get_pcc_unchecked(env);
}
// This function can be called if the pcc.cursor value must be accurate.
static inline const cap_register_t *cheri_get_current_pcc(CPUArchState *env)
{
    cheri_debug_assert(pc_is_current(env));
    return _cheri_get_pcc_unchecked(env);
}

static inline const cap_register_t *
cheri_update_and_get_current_pcc(CPUArchState *env, uintptr_t retpc)
{
    cpu_restore_state(env_cpu(env), retpc, false);
    return cheri_get_current_pcc(env);
}

static inline bool cheri_in_capmode(CPUArchState *env) {
    // Note: No need to synchronize the PCC.cursor value from TCG since
    // Every change to capmode will exit the current translation block.
    return (cheri_get_recent_pcc(env)->cr_flags & CHERI_FLAG_CAPMODE) == CHERI_FLAG_CAPMODE;
}

#endif

static inline target_ulong cpu_get_current_pc(CPUArchState *env,
                                              uintptr_t retpc, bool will_exit)
{
    cpu_restore_state(env_cpu(env), retpc, will_exit);
    cheri_debug_assert(pc_is_current(env));
    return cpu_get_recent_pc(env);
}

static inline target_ulong cpu_get_current_pc_checked(CPUArchState *env)
{
    cheri_debug_assert(pc_is_current(env));
    return cpu_get_recent_pc(env);
}
