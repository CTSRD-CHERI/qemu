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
#ifdef TARGET_CHERI
#include "cheri_defs.h"
#include "cheri_utils.h"
#include "cheri-lazy-capregs-types.h"
#include "cheri-archspecific-early.h"
#include "cpu.h"
#include "tcg/tcg.h"
#include "qemu/log.h"
#include "exec/log_instr.h"

#if !defined(CHERI_EXC_REGNUM_PCC) || !defined(CHERI_EXC_REGNUM_DDC) ||        \
    !defined(ZERO_REG_NUM)
#error "Special register numbers should be defined in archspecific-earlier.h"
#endif

static inline GPCapRegs *cheri_get_gpcrs(CPUArchState *env);

static inline uint64_t capreg_state_set_to_integer_mask(unsigned reg)
{
    return ~(UINT64_C(3) << (reg * 2));
}

static inline CapRegState get_capreg_state(GPCapRegs *gpcrs, unsigned reg)
{
    if (reg == ZERO_REG_NUM || reg == SCRATCH_REG_NUM)
        return CREG_FULLY_DECOMPRESSED;

    cheri_debug_assert(reg < 32);
    return (CapRegState)extract64(gpcrs->capreg_state, reg * 2, 2);
}

static inline void sanity_check_capreg(GPCapRegs *gpcrs, unsigned regnum)
{
#ifdef CONFIG_DEBUG_TCG
    if (get_capreg_state(gpcrs, regnum) == CREG_FULLY_DECOMPRESSED) {
        cheri_debug_assert(regnum < NUM_LAZY_CAP_REGS);
        cheri_debug_assert(get_capreg_state(gpcrs, regnum) ==
                           CREG_FULLY_DECOMPRESSED);
        const cap_register_t *c = get_cap_in_gpregs(gpcrs, regnum);
#if QEMU_USE_COMPRESSED_CHERI_CAPS
        // Check that the compressed and decompressed caps are in sync
        cheri_debug_assert(cc128_compress_raw(c) ==
                           get_cap_in_gpregs(gpcrs, regnum)->cached_pesbt);
#endif
        cheri_debug_assert((c->cr_tag == 0 || c->cr_tag == 1) &&
                           "Unitialized value used?");
    } else {
        // Reset decompressed values to invalid data to check they aren't
        // accessed. However, the cursor and pesbt must remain valid.
        cap_register_t *decompressed = get_cap_in_gpregs(gpcrs, regnum);
        target_ulong cursor = decompressed->_cr_cursor;
        target_ulong pesbt = decompressed->cached_pesbt;
        memset(decompressed, 0xaa, sizeof(*decompressed));
        decompressed->_cr_cursor = cursor;
        decompressed->cached_pesbt = pesbt;
    }
#if QEMU_USE_COMPRESSED_CHERI_CAPS

    if (regnum == ZERO_REG_NUM) {
        cheri_debug_assert(get_cap_in_gpregs(gpcrs, regnum)->cached_pesbt ==
                           CC128_NULL_PESBT);
        cheri_debug_assert(get_cap_in_gpregs(gpcrs, regnum)->_cr_cursor == 0);
        cheri_debug_assert(cc128_compress_raw(get_cap_in_gpregs(gpcrs, regnum)) == CC128_NULL_PESBT);
        cheri_debug_assert(get_capreg_state(gpcrs, regnum) ==
                             CREG_FULLY_DECOMPRESSED &&
                         "Null should always be fully decompressed");
    }
#endif
#endif // CONFIG_DEBUG_TCG
}

static inline void set_capreg_state(GPCapRegs *gpcrs, unsigned regnum,
                                    CapRegState new_state)
{
    if (regnum == ZERO_REG_NUM || regnum == SCRATCH_REG_NUM) {
        cheri_debug_assert(new_state == CREG_FULLY_DECOMPRESSED &&
                           "NULL is always fully decompressed");
        return;
    }

    cheri_debug_assert(regnum < 32);
    gpcrs->capreg_state =
        deposit64(gpcrs->capreg_state, regnum * 2, 2, new_state);
    // Check that the compressed and decompressed caps are in sync
    sanity_check_capreg(gpcrs, regnum);
}

#if QEMU_USE_COMPRESSED_CHERI_CAPS
static inline const cap_register_t *
_update_from_compressed(GPCapRegs *gpcrs, unsigned regnum, bool tag)
{
    // Note: The _cr_cusor field is always valid. All others are lazy.
    cc128_decompress_raw(get_cap_in_gpregs(gpcrs, regnum)->cached_pesbt,
                         get_cap_in_gpregs(gpcrs, regnum)->_cr_cursor, tag,
                         get_cap_in_gpregs(gpcrs, regnum));
    set_capreg_state(gpcrs, regnum, CREG_FULLY_DECOMPRESSED);
    return get_cap_in_gpregs(gpcrs, regnum);
}
#endif

static inline __attribute__((always_inline)) const cap_register_t *
get_readonly_capreg(CPUArchState *env, unsigned regnum)
{
    GPCapRegs *gpcrs = cheri_get_gpcrs(env);

    if (regnum == ZERO_REG_NUM) {
        // NULL is immuatable and should always be fully decompressed
        cheri_debug_assert(get_capreg_state(gpcrs, regnum) ==
                           CREG_FULLY_DECOMPRESSED);
        return get_cap_in_gpregs(gpcrs, regnum);
    }

    switch (get_capreg_state(gpcrs, regnum)) {
    case CREG_INTEGER: {
        // Update capreg to a decompressed integer value and clear pesbt
        const cap_register_t *result =
            int_to_cap(get_cap_in_gpregs(gpcrs, regnum)->_cr_cursor,
                       get_cap_in_gpregs(gpcrs, regnum));
        set_capreg_state(gpcrs, regnum, CREG_FULLY_DECOMPRESSED);
        return result;
    }
    case CREG_FULLY_DECOMPRESSED:
        // Check that the compressed and decompressed caps are in sync
        sanity_check_capreg(gpcrs, regnum);
        return get_cap_in_gpregs(gpcrs, regnum);
#if QEMU_USE_COMPRESSED_CHERI_CAPS
    case CREG_TAGGED_CAP:
        return _update_from_compressed(gpcrs, regnum, /*tag=*/true);
    case CREG_UNTAGGED_CAP:
        return _update_from_compressed(gpcrs, regnum, /*tag=*/false);
#endif
    default:
        __builtin_unreachable();
    }
    tcg_abort();
}

static inline __attribute__((always_inline)) bool
get_without_decompress_tag(CPUArchState *env, unsigned regnum)
{
    GPCapRegs *gpcrs = cheri_get_gpcrs(env);
    CapRegState state = get_capreg_state(gpcrs, regnum);
    bool tag = (state == CREG_FULLY_DECOMPRESSED) &&
               get_cap_in_gpregs(gpcrs, regnum)->cr_tag;
#if QEMU_USE_COMPRESSED_CHERI_CAPS
    tag |= (state == CREG_TAGGED_CAP);
#endif
    return tag;
}

static inline __attribute__((always_inline)) target_ulong
get_without_decompress_cursor(CPUArchState *env, unsigned regnum)
{
    GPCapRegs *gpcrs = cheri_get_gpcrs(env);
    return get_cap_in_gpregs(gpcrs, regnum)->_cr_cursor;
}

#if QEMU_USE_COMPRESSED_CHERI_CAPS
static inline __attribute__((always_inline)) target_ulong
get_without_decompress_pesbt(CPUArchState *env, unsigned regnum)
{
    GPCapRegs *gpcrs = cheri_get_gpcrs(env);
    return get_cap_in_gpregs(gpcrs, regnum)->cached_pesbt;
}
#endif

// Return a CREG or DDC or PCC.
static inline __attribute__((always_inline)) const cap_register_t *
get_capreg_or_special(CPUArchState *env, unsigned regnum)
{

    if (unlikely(regnum == CHERI_EXC_REGNUM_PCC))
        return _cheri_get_pcc_unchecked(env);
    if (unlikely(regnum == CHERI_EXC_REGNUM_DDC))
        return cheri_get_ddc(env);
    else
        return get_readonly_capreg(env, regnum);
}

/// return a read-only capability register with register number 0 meaning $ddc
/// This is useful for cl*/cs*/cll*/csc*/cfromptr/cbuildcap since using $ddc as
/// the address argument there will cause a trap We also use it for the cb
/// argument to ctoptr/cbuildcap since using a ctoptr relative to $ddc make
/// sense whereas using it relative to NULL is the same as just cmove $cN,
/// $cnull
/// 0 can only be DDC on mips/risv. On Morello 0 is always normal register.
/// Having the switch here rather than in general code makes things slightly
/// neater. We could always call this "0_is_maybe_ddc" to be less confusing.
static inline __attribute__((always_inline)) const cap_register_t *
get_capreg_0_is_ddc(CPUArchState *env, unsigned regnum)
{
#ifdef TARGET_AARCH64
    return get_capreg_or_special(env, regnum);
#else
    return get_capreg_or_special(env,
                                 regnum == 0 ? CHERI_EXC_REGNUM_DDC : regnum);
#endif
}


#ifdef CONFIG_TCG_LOG_INSTR

/*
 * Log instruction update to the given capability register.
 */
#define cheri_log_instr_changed_capreg(env, name, newval) do {          \
        if (qemu_log_instr_enabled(env)) {                              \
            qemu_log_instr_cap(env, name, newval);                      \
        }                                                               \
    } while (0)

/*
 * Log instruction update to the given capability register with
 * integer value.
 */
#define cheri_log_instr_changed_capreg_int(env, name, newval) do {      \
        if (qemu_log_instr_enabled(env)) {                              \
            qemu_log_instr_cap_int(env, name, newval);                  \
        }                                                               \
    } while (0)

#else
#define cheri_log_instr_changed_capreg(env, name, newval) ((void)0)
#define cheri_log_instr_changed_capreg_int(env, name, newval) ((void)0)
#endif

/* Note: cheri_gp_regnames should be declared in cpu.h or cheri-archspecific.h */
#define cheri_log_instr_changed_gp_capreg(env, regnum, newval)          \
    cheri_log_instr_changed_capreg(env, cheri_gp_regnames[regnum], newval)

static inline void rvfi_changed_capreg(CPUArchState *env, unsigned regnum,
                                       target_ulong cursor)
{
#if defined(TARGET_RISCV) && defined(CONFIG_RVFI_DII)
    env->rvfi_dii_trace.INTEGER.rvfi_rd_addr = regnum;
    env->rvfi_dii_trace.INTEGER.rvfi_rd_wdata = cursor;
    env->rvfi_dii_trace.available_fields |= RVFI_INTEGER_DATA;
#endif
}

static inline void update_capreg(CPUArchState *env, unsigned regnum,
                                 const cap_register_t *newval)
{
    // writing to $c0/$cnull is a no-op

    if (unlikely(regnum == ZERO_REG_NUM))
        return;

    GPCapRegs *gpcrs = cheri_get_gpcrs(env);
    cap_register_t *target = get_cap_in_gpregs(gpcrs, regnum);
    *target = *newval;
    // Update the compressed values for fast access from TCG
#if QEMU_USE_COMPRESSED_CHERI_CAPS
    // NOTE: This is only needed if pesbt has fallen out of sync.
    // We should find occurences of this and get rid of them.
    get_cap_in_gpregs(gpcrs, regnum)->cached_pesbt = cc128_compress_raw(target);
#endif
    set_capreg_state(gpcrs, regnum, CREG_FULLY_DECOMPRESSED);
    sanity_check_capreg(gpcrs, regnum);
    rvfi_changed_capreg(env, regnum, newval->_cr_cursor);
    cheri_log_instr_changed_gp_capreg(env, regnum, target);
}

#if QEMU_USE_COMPRESSED_CHERI_CAPS
static inline void update_compressed_capreg(CPUArchState *env, unsigned regnum,
                                            target_ulong pesbt, bool tag,
                                            target_ulong cursor)
{
    // writing to $c0/$cnull is a no-op

    if (unlikely(regnum == ZERO_REG_NUM))
        return;

    GPCapRegs *gpcrs = cheri_get_gpcrs(env);
    get_cap_in_gpregs(gpcrs, regnum)->_cr_cursor = cursor;
    get_cap_in_gpregs(gpcrs, regnum)->cached_pesbt = pesbt;
    CapRegState new_state = tag ? CREG_TAGGED_CAP : CREG_UNTAGGED_CAP;
    set_capreg_state(gpcrs, regnum, new_state);
    cheri_debug_assert(get_capreg_state(gpcrs, regnum) == new_state);
    sanity_check_capreg(gpcrs, regnum);
    rvfi_changed_capreg(env, regnum, cursor);
    if (qemu_log_instr_enabled(env)) {
        // Decompress and log value if instruction logging is on
        const cap_register_t *decompressed = get_readonly_capreg(env, regnum);
        cheri_log_instr_changed_gp_capreg(env, regnum, decompressed);
    }
}

static inline target_ulong get_capreg_pesbt(CPUArchState *env, unsigned regnum)
{
    GPCapRegs *gpcrs = cheri_get_gpcrs(env);
    sanity_check_capreg(gpcrs, regnum);
    // The pesbt field is invalid for capability registers holding integers
    // and we have to return the value that NULL would have.
    if (get_capreg_state(gpcrs, regnum) == CREG_INTEGER) {
        return CC128_NULL_PESBT;
    }
    return get_cap_in_gpregs(gpcrs, regnum)->cached_pesbt;
}
#endif

static inline void update_capreg_to_intval(CPUArchState *env, unsigned regnum,
                                           target_ulong newval)
{
    // writing to $c0/$cnull is a no-op
    if (unlikely(regnum == ZERO_REG_NUM))
        return;

    GPCapRegs *gpcrs = cheri_get_gpcrs(env);
    get_cap_in_gpregs(gpcrs, regnum)->_cr_cursor = newval;
    get_cap_in_gpregs(gpcrs, regnum)->cached_pesbt = CC128_NULL_PESBT;
    set_capreg_state(gpcrs, regnum, CREG_INTEGER);
    sanity_check_capreg(gpcrs, regnum);
    rvfi_changed_capreg(env, regnum, newval);
    cheri_log_instr_changed_capreg_int(env, cheri_gp_regnames[regnum], newval);
}

static inline target_ulong get_capreg_cursor(CPUArchState *env, unsigned regnum)
{
    GPCapRegs *gpcrs = cheri_get_gpcrs(env);
    sanity_check_capreg(gpcrs, regnum);
    return get_cap_in_gpregs(gpcrs, regnum)->_cr_cursor;
}

static inline target_ulong get_capreg_tag(CPUArchState *env, unsigned regnum)
{
    GPCapRegs *gpcrs = cheri_get_gpcrs(env);
    sanity_check_capreg(gpcrs, regnum);
    // We can determine the value of the tag by looking at the current state:
    // Three of them directly encode the tag value and in the fully decompressed
    // state we simply read the cr_tag member.
    switch (get_capreg_state(gpcrs, regnum)) {
    case CREG_FULLY_DECOMPRESSED:
        return get_cap_in_gpregs(gpcrs, regnum)->cr_tag;
    case CREG_INTEGER:
        return false;
#if QEMU_USE_COMPRESSED_CHERI_CAPS
    case CREG_TAGGED_CAP:
        return true;
    case CREG_UNTAGGED_CAP:
        return false;
#endif
    default:
        __builtin_unreachable();
    }
    tcg_abort();
}

static inline target_ulong get_capreg_tag_filtered(CPUArchState *env, unsigned regnum) {
#ifdef TARGET_MIPS
    target_ulong tagged = get_capreg_tag(env, regnum);

    // Try avoid decompress if at all possible
    if (!tagged || env->cheri_capfilter_hi == 0) return tagged;

    const cap_register_t *csp = get_readonly_capreg(env, regnum);

    // TODO: Also revoke if (lo <= type < hi) && (csp->cr_perms & MAGIC_REVOKE_TYPE)
    if (csp->cr_base >= env->cheri_capfilter_lo &&
        csp->_cr_top <= env->cheri_capfilter_hi &&
        (csp->cr_perms & env->cheri_capfilter_perms) == csp->cr_perms)
      return 0;

    return tagged;
#else
    return get_capreg_tag(env, regnum);
#endif
}

static inline uint32_t get_capreg_hwperms(CPUArchState *env, unsigned regnum)
{
    GPCapRegs *gpcrs = cheri_get_gpcrs(env);
    sanity_check_capreg(gpcrs, regnum);
    // We can determine the value of the tag by looking at the current state:
    // Three of them directly encode the tag value and in the fully decompressed
    // state we simply read the cr_tag member.
    switch (get_capreg_state(gpcrs, regnum)) {
    case CREG_FULLY_DECOMPRESSED:
        return get_cap_in_gpregs(gpcrs, regnum)->cr_perms;
    case CREG_INTEGER:
        return 0; /* No permissions */
#if QEMU_USE_COMPRESSED_CHERI_CAPS
    case CREG_TAGGED_CAP:
    case CREG_UNTAGGED_CAP:
        return (uint32_t)CC128_EXTRACT_FIELD(
            get_cap_in_gpregs(gpcrs, regnum)->cached_pesbt, HWPERMS);
#endif
    default:
        __builtin_unreachable();
    }
    tcg_abort();
}

static inline void nullify_capreg(CPUArchState *env, unsigned regnum)
{

    cheri_debug_assert(regnum != ZERO_REG_NUM);

    GPCapRegs *gpcrs = cheri_get_gpcrs(env);
    const cap_register_t* newval = null_capability(get_cap_in_gpregs(gpcrs, regnum));
    set_capreg_state(gpcrs, regnum, CREG_FULLY_DECOMPRESSED);
    sanity_check_capreg(gpcrs, regnum);
    cheri_log_instr_changed_gp_capreg(env, regnum, newval);
}

static inline void reset_capregs(CPUArchState* env)
{
    // Reset all to NULL:
    GPCapRegs *gpcrs = cheri_get_gpcrs(env);
    gpcrs->capreg_state = UINT64_MAX; // All decompressed values
    for (size_t i = 0; i < ARRAY_SIZE(gpcrs->decompressed); i++) {
        const cap_register_t* newval = null_capability(get_cap_in_gpregs(gpcrs, i));
        // Mark register as fully decompressed
        cheri_debug_assert(get_capreg_state(gpcrs, i) == CREG_FULLY_DECOMPRESSED);
        sanity_check_capreg(gpcrs, i);
        cheri_log_instr_changed_gp_capreg(env, i, newval);
    }
}

static inline void set_max_perms_capregs(CPUArchState* env)
{
    // Reset all to max perms (except NULL of course):
    GPCapRegs *gpcrs = cheri_get_gpcrs(env);
    gpcrs->capreg_state = UINT64_MAX; // All decompressed values
    null_capability(get_cap_in_gpregs(gpcrs, ZERO_REG_NUM));
    sanity_check_capreg(gpcrs, ZERO_REG_NUM);
    for (size_t i = 0; i < ARRAY_SIZE(gpcrs->decompressed); i++) {

        if (i == ZERO_REG_NUM)
            continue;

        set_max_perms_capability(get_cap_in_gpregs(gpcrs, i), 0);
#if QEMU_USE_COMPRESSED_CHERI_CAPS
        get_cap_in_gpregs(gpcrs, i)->cached_pesbt =
            cc128_compress_raw(get_cap_in_gpregs(gpcrs, i));
#endif
        // Mark register as fully decompressed
        cheri_debug_assert(get_capreg_state(gpcrs, i) == CREG_FULLY_DECOMPRESSED);
        sanity_check_capreg(gpcrs, i);
        cheri_log_instr_changed_gp_capreg(env, i, get_cap_in_gpregs(gpcrs, i));
    }
}

#endif // TARGET_CHERI
