/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2020 Alex Richardson
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

/* All helpers shared between RISCV and MIPS are defined here.
 * Helpers are grouped by instruction kind and sorted alphabetically within
 * each group.
 *
 * Most are implemented in op_helper_cheri_common.c, but some have
 * target-dependent implementations and are defined in
 * target/mips/op_helper_cheri.c or target/riscv/op_helper_cheri.c.
 */

// PCC bounds checks:
DEF_HELPER_1(raise_exception_pcc_perms, noreturn, env)
DEF_HELPER_3(raise_exception_pcc_bounds, noreturn, env, tl, i32)
DEF_HELPER_2(raise_exception_ddc_perms, noreturn, env, i32)
DEF_HELPER_3(raise_exception_ddc_bounds, noreturn, env, tl, i32)

// Two-operand capability inspection
DEF_HELPER_FLAGS_2(cgetaddr, TCG_CALL_NO_WG, tl, env, i32)
DEF_HELPER_FLAGS_2(cgetbase, TCG_CALL_NO_WG, tl, env, i32)
DEF_HELPER_FLAGS_2(cgetflags, TCG_CALL_NO_WG, tl, env, i32)
DEF_HELPER_FLAGS_2(cgetlen, TCG_CALL_NO_WG, tl, env, i32)
DEF_HELPER_FLAGS_2(cgetperm, TCG_CALL_NO_WG, tl, env, i32)
DEF_HELPER_FLAGS_2(cgetoffset, TCG_CALL_NO_WG, tl, env, i32)
DEF_HELPER_FLAGS_2(cgetsealed, TCG_CALL_NO_WG, tl, env, i32)
DEF_HELPER_FLAGS_2(cgettag, TCG_CALL_NO_WG, tl, env, i32)
DEF_HELPER_FLAGS_2(cgettype, TCG_CALL_NO_WG, tl, env, i32)

// Two operands (cap cap)
DEF_HELPER_3(ccleartag, void, env, i32, i32)
DEF_HELPER_3(cmove, void, env, i32, i32)
DEF_HELPER_3(cchecktype, void, env, i32, i32)
DEF_HELPER_3(csealentry, void, env, i32, i32)

// Two operands (cap int)
DEF_HELPER_3(ccheckperm, void, env, i32, tl)

// Two operands (int int)
DEF_HELPER_FLAGS_2(crap, TCG_CALL_NO_RWG_SE, tl, env, tl)
DEF_HELPER_FLAGS_2(cram, TCG_CALL_NO_RWG_SE, tl, env, tl)

// Three operands (cap cap cap)
DEF_HELPER_4(cbuildcap, void, env, i32, i32, i32)
DEF_HELPER_4(ccopytype, void, env, i32, i32, i32)
DEF_HELPER_4(ccseal, void, env, i32, i32, i32)
DEF_HELPER_4(cseal, void, env, i32, i32, i32)
DEF_HELPER_4(cunseal, void, env, i32, i32, i32)

// Three operands (cap cap int)
DEF_HELPER_4(candaddr, void, env, i32, i32, tl)
DEF_HELPER_4(candperm, void, env, i32, i32, tl)
DEF_HELPER_4(cfromptr, void, env, i32, i32, tl)
DEF_HELPER_4(cincoffset, void, env, i32, i32, tl)
DEF_HELPER_4(cjalr, tl, env, i32, i32, tl)
DEF_HELPER_4(csetaddr, void, env, i32, i32, tl)
DEF_HELPER_4(csetbounds, void, env, i32, i32, tl)
DEF_HELPER_4(csetboundsexact, void, env, i32, i32, tl)
DEF_HELPER_4(csetflags, void, env, i32, i32, tl)
DEF_HELPER_4(csetoffset, void, env, i32, i32, tl)

// Three operands (int cap cap)
DEF_HELPER_FLAGS_3(csub, TCG_CALL_NO_WG, tl, env, i32, i32)
DEF_HELPER_FLAGS_3(ctestsubset, TCG_CALL_NO_WG, tl, env, i32, i32)
DEF_HELPER_FLAGS_3(ctoptr, TCG_CALL_NO_WG, tl, env, i32, i32)

// Loads+Stores
DEF_HELPER_4(cap_load_check, cap_checked_ptr, env, i32, tl, i32)
DEF_HELPER_4(cap_store_check, cap_checked_ptr, env, i32, tl, i32)
DEF_HELPER_4(cap_rmw_check, cap_checked_ptr, env, i32, tl, i32)
DEF_HELPER_4(load_cap_via_cap, void, env, i32, i32, tl)
DEF_HELPER_4(store_cap_via_cap, void, env, i32, i32, tl)
