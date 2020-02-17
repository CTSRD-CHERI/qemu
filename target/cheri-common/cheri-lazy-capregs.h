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
#pragma once
#ifdef TARGET_CHERI
#include "cheri-archspecific.h"
#include "cheri-lazy-capregs-types.h"
#include "cheri_defs.h"
#include "cheri_utils.h"
#include "cpu.h"
#include "tcg/tcg.h"

static inline const cap_register_t *cheri_get_ddc(CPUArchState *env);
static inline GPCapRegs *cheri_get_gpcrs(CPUArchState *env);

static inline uint64_t capreg_state_set_to_integer_mask(unsigned reg)
{
    return ~(UINT64_C(3) << (reg * 2));
}

static inline CapRegState get_capreg_state(GPCapRegs *gpcrs, unsigned reg)
{
    cheri_debug_assert(reg < 32);
    return (CapRegState)extract64(gpcrs->capreg_state, reg * 2, 2);
}

static inline void sanity_check_capreg(GPCapRegs *gpcrs, unsigned regnum)
{
#ifdef CONFIG_DEBUG_TCG
    if (get_capreg_state(gpcrs, regnum) == CREG_FULLY_DECOMPRESSED) {
        cheri_debug_assert(regnum < 32);
        cheri_debug_assert(get_capreg_state(gpcrs, regnum) ==
                           CREG_FULLY_DECOMPRESSED);
        const cap_register_t *c = &gpcrs->decompressed[regnum];
        // Check that the compressed and decompressed caps are in sync
        cheri_debug_assert(compress_128cap_without_xor(c) ==
                           gpcrs->pesbt[regnum]);
        cheri_debug_assert(cap_get_cursor(c) == gpcrs->cursor[regnum]);
        cheri_debug_assert((c->cr_tag == 0 || c->cr_tag == 1) &&
                           "Unitialized value used?");
    } else {
        // Reset decompressed values to invalid data to check they aren't
        // accessed.
        cap_register_t *decompressed = &gpcrs->decompressed[regnum];
        memset(decompressed, 0xaa, sizeof(*decompressed));
    }
#endif
}

static inline void set_capreg_state(GPCapRegs *gpcrs, unsigned regnum,
                                    CapRegState new_state)
{
    cheri_debug_assert(regnum < 32);
    if (regnum == 0) {
        cheri_debug_assert(new_state == CREG_FULLY_DECOMPRESSED &&
                           "NULL is always fully decompressed");
    }
    gpcrs->capreg_state =
        deposit64(gpcrs->capreg_state, regnum * 2, 2, new_state);
    // Check that the compressed and decompressed caps are in sync
    sanity_check_capreg(gpcrs, regnum);
}

static inline const cap_register_t *
_update_from_compressed(GPCapRegs *gpcrs, unsigned regnum, bool tag)
{
    decompress_128cap_already_xored(gpcrs->pesbt[regnum], gpcrs->cursor[regnum],
                                    &gpcrs->decompressed[regnum]);
    gpcrs->decompressed[regnum].cr_tag = tag;
    set_capreg_state(gpcrs, regnum, CREG_FULLY_DECOMPRESSED);
    return &gpcrs->decompressed[regnum];
}

static inline __attribute__((always_inline)) const cap_register_t *
get_readonly_capreg(CPUArchState *env, unsigned regnum)
{
    GPCapRegs *gpcrs = cheri_get_gpcrs(env);
    if (regnum == 0) {
        // NULL is immuatable and should always be fully decompressed
        cheri_debug_assert(get_capreg_state(gpcrs, regnum) ==
                           CREG_FULLY_DECOMPRESSED);
        return &gpcrs->decompressed[0];
    }
    switch (get_capreg_state(gpcrs, regnum)) {
    case CREG_INTEGER: {
        // Update capreg to a decompressed integer value and clear pesbt
        gpcrs->pesbt[regnum] = CC128_NULL_PESBT;
        const cap_register_t *result =
            int_to_cap(gpcrs->cursor[regnum], &gpcrs->decompressed[regnum]);
        set_capreg_state(gpcrs, regnum, CREG_FULLY_DECOMPRESSED);
        return result;
    }
    case CREG_FULLY_DECOMPRESSED:
        // Check that the compressed and decompressed caps are in sync
        sanity_check_capreg(gpcrs, regnum);
        return &gpcrs->decompressed[regnum];
    case CREG_TAGGED_CAP:
        return _update_from_compressed(gpcrs, regnum, /*tag=*/true);
    case CREG_UNTAGGED_CAP:
        return _update_from_compressed(gpcrs, regnum, /*tag=*/false);
    default:
        tcg_abort();
    }
}

/// return a read-only capability register with register number 0 meaning $ddc
/// This is useful for cl*/cs*/cll*/csc*/cfromptr/cbuildcap since using $ddc as
/// the address argument there will cause a trap We also use it for the cb
/// argument to ctoptr/cbuildcap since using a ctoptr relative to $ddc make
/// sense whereas using it relative to NULL is the same as just cmove $cN,
/// $cnull
static inline __attribute__((always_inline)) const cap_register_t *
get_capreg_0_is_ddc(CPUArchState *env, unsigned regnum)
{
    if (unlikely(regnum == 0)) {
        return cheri_get_ddc(env);
    }
    return get_readonly_capreg(env, regnum);
}

static inline void update_capreg(CPUArchState *env, unsigned regnum,
                                 const cap_register_t *newval)
{
    // writing to $c0/$cnull is a no-op
    if (unlikely(regnum == 0))
        return;
    GPCapRegs *gpcrs = cheri_get_gpcrs(env);
    cap_register_t *target = &gpcrs->decompressed[regnum];
    *target = *newval;
    // Update the compressed values for fast access from TCG
    gpcrs->cursor[regnum] = cap_get_cursor(target);
    gpcrs->pesbt[regnum] = compress_128cap_without_xor(target);
    set_capreg_state(gpcrs, regnum, CREG_FULLY_DECOMPRESSED);
    sanity_check_capreg(gpcrs, regnum);
}

static inline void update_compressed_capreg(CPUArchState *env, unsigned regnum,
                                            target_ulong pesbt, bool tag,
                                            target_ulong cursor)
{
    // writing to $c0/$cnull is a no-op
    if (unlikely(regnum == 0))
        return;
    GPCapRegs *gpcrs = cheri_get_gpcrs(env);
    gpcrs->cursor[regnum] = cursor;
    gpcrs->pesbt[regnum] = pesbt;
    CapRegState new_state = tag ? CREG_TAGGED_CAP : CREG_UNTAGGED_CAP;
    set_capreg_state(gpcrs, regnum, new_state);
    cheri_debug_assert(get_capreg_state(gpcrs, regnum) == new_state);
    sanity_check_capreg(gpcrs, regnum);
}

static inline target_ulong get_capreg_cursor(CPUArchState *env, unsigned regnum)
{
    GPCapRegs *gpcrs = cheri_get_gpcrs(env);
    sanity_check_capreg(gpcrs, regnum);
    return gpcrs->cursor[regnum];
}

static inline target_ulong get_capreg_pesbt(CPUArchState *env, unsigned regnum)
{
    GPCapRegs *gpcrs = cheri_get_gpcrs(env);
    sanity_check_capreg(gpcrs, regnum);
    return gpcrs->pesbt[regnum];
}

static inline target_ulong get_capreg_tag(CPUArchState *env, unsigned regnum)
{
    GPCapRegs *gpcrs = cheri_get_gpcrs(env);
    sanity_check_capreg(gpcrs, regnum);
    // We can determine the value of the tag by looking at the current state:
    // Three of them directly encode the tag value and in the fully decompressed
    // state we simply read the cr_tag member.
    switch (get_capreg_state(gpcrs, regnum)) {
    case CREG_INTEGER:
        return false;
    case CREG_TAGGED_CAP:
        return true;
    case CREG_UNTAGGED_CAP:
        return false;
    case CREG_FULLY_DECOMPRESSED:
        return gpcrs->decompressed[regnum].cr_tag;
    default:
        tcg_abort();
    }
}

static inline void nullify_capreg(CPUArchState *env, unsigned regnum)
{
    cheri_debug_assert(regnum != 0);
    GPCapRegs *gpcrs = cheri_get_gpcrs(env);
    gpcrs->pesbt[regnum] = CC128_NULL_PESBT;
    gpcrs->cursor[regnum] = 0;
    (void)null_capability(&gpcrs->decompressed[regnum]);
    set_capreg_state(gpcrs, regnum, CREG_FULLY_DECOMPRESSED);
    // TODO: could also set state to integer and not bother updating
    sanity_check_capreg(gpcrs, regnum);
}

static inline void reset_capregs(GPCapRegs *gpcrs)
{
    // Reset all to NULL:
    for (size_t i = 0; i < ARRAY_SIZE(gpcrs->pesbt); i++) {
        gpcrs->pesbt[i] = CC128_NULL_PESBT;
        gpcrs->cursor[i] = 0;
    }
    gpcrs->capreg_state = 0; // All integer values
#ifdef CONFIG_DEBUG_TCG
    // Reset decompressed values to invalid data to check they aren't accessed.
    memset(gpcrs->decompressed, 0xaa, sizeof(gpcrs->decompressed));
#endif
    // Mark NULL register as fully decompressed
    null_capability(&gpcrs->decompressed[0]);
    set_capreg_state(gpcrs, 0, CREG_FULLY_DECOMPRESSED);
}

#endif
