/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2019 Alex Richardson
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
#include "qemu/compiler.h"
#include "qemu/osdep.h"
#include "qemu/error-report.h"

#include "cheri_defs.h"

#ifdef TARGET_AARCH64
#define PRINT_CAP_MODE(cr) (unsigned)(cap_get_cursor(cr) & 1)
#else
#define PRINT_CAP_MODE(cr) (unsigned)cap_get_exec_mode(cr)
#endif
#define PRINT_CAP_FMTSTR                                                       \
    "v:%d m:%d p:%2x ct:" TARGET_FMT_ld " b:" TARGET_FMT_lx                    \
    " a:" TARGET_FMT_lx " t:" TARGET_FMT_lx " bv: %d"
#define PRINT_CAP_ARGS(cr)                                                     \
    (cr)->cr_tag, PRINT_CAP_MODE(cr), (unsigned)cap_get_all_perms(cr),         \
        cap_get_otype_signext(cr), cap_get_base(cr), cap_get_cursor(cr),       \
        cap_get_top(cr), (cr)->cr_bounds_valid

#ifdef TARGET_CHERI

static inline target_ulong cap_get_cursor(const cap_register_t *c)
{
    return c->_cr_cursor;
}

// Getters to allow future changes to the cap_register_t struct layout:
static inline target_ulong cap_get_base(const cap_register_t *c)
{
    return c->cr_base;
}

static inline cap_offset_t cap_get_offset(const cap_register_t *c)
{
    return (cap_offset_t)c->_cr_cursor - (cap_offset_t)c->cr_base;
}

static inline target_ulong cap_get_all_perms(const cap_register_t *c)
{
    return CAP_cc(get_all_permissions)(c);
}

#ifdef TARGET_CHERI_RISCV_STD
/* TODO: move this into cheri-compressed-cap */
#define RVY_AP_C   CAP_CC(PERM_CAPABILITY)
#define RVY_AP_W   CAP_CC(PERM_WRITE)
#define RVY_AP_R   CAP_CC(PERM_READ)
#define RVY_AP_X   CAP_CC(PERM_EXECUTE)
#define RVY_AP_ASR CAP_CC(PERM_ACCESS_SYS_REGS)
#define RVY_AP_LM  CAP_CC(PERM_LOAD_MUTABLE)
#define RVY_AP_EL  CAP_CC(PERM_ELEVATE_LEVEL)
#define RVY_AP_SL  CAP_CC(PERM_STORE_LEVEL)
#define PERM_RULE(bit, cond)                                                   \
    do {                                                                       \
        if (perms & (bit)) {                                                   \
            if (!(cond)) {                                                     \
                perms &= ~(bit);                                               \
                updated = true;                                                \
            }                                                                  \
        }                                                                      \
    } while (0)

/*
 * Fix up a set of (M, AP) to be in line with the rules for the acperm
 * instruction, resulting in a set that could have been created by acperm.
 * Return true if the input set had to be modified for this or false if
 * the input set is already compliant to the acperm rules.
 */
static inline bool fix_up_ap(CPUArchState *env, target_ulong *perms_result)
{
    bool updated = false;
    RISCVCPU *cpu = env_archcpu(env);
    target_ulong perms = *perms_result;
    uint8_t lvbits = cpu->cfg.lvbits;

    /*
     * The code below tries to follow the rules in the risc-v cheri
     * specification as closely as possible. This should make it easier
     * to find bugs.
     */

#if CAP_CC(ADDR_WIDTH) == 32
    {
        target_ulong non_asr_perms = RVY_AP_C | RVY_AP_R | RVY_AP_W | RVY_AP_X;
        non_asr_perms |= RVY_AP_LM;
        if (lvbits > 0) {
            non_asr_perms |= RVY_AP_EL | RVY_AP_SL;
        }

        /* rule 1 */
        PERM_RULE(RVY_AP_ASR, (perms & non_asr_perms) == non_asr_perms);
    }
#endif

    /* rule 2 */
    PERM_RULE(RVY_AP_C, perms & (RVY_AP_R | RVY_AP_W));

#if CAP_CC(ADDR_WIDTH) == 32
    /* rule 3 */
    PERM_RULE(RVY_AP_C, perms & RVY_AP_R);

    /* rule 4 */
    PERM_RULE(RVY_AP_X, perms & RVY_AP_R);

    /* rule 5 */
    PERM_RULE(RVY_AP_W, !(perms & RVY_AP_C) || (perms & RVY_AP_LM));

    /* rule 6 */
    PERM_RULE(RVY_AP_X, perms & (RVY_AP_W | RVY_AP_C));
#endif

    /* rule 7 */
    if (lvbits > 0) {
        PERM_RULE(RVY_AP_EL,
                  (perms & (RVY_AP_C | RVY_AP_R)) == (RVY_AP_C | RVY_AP_R));
    }

#if CAP_CC(ADDR_WIDTH) == 32
    /* rule 8 */
    if (lvbits > 0) {
        PERM_RULE(RVY_AP_EL, perms & RVY_AP_LM);
    }
#endif

    /* rule 9 */
    PERM_RULE(RVY_AP_LM,
              (perms & (RVY_AP_C | RVY_AP_R)) == (RVY_AP_C | RVY_AP_R));

#if CAP_CC(ADDR_WIDTH) == 32
    /* rule 10 */
    if (lvbits > 0) {
        PERM_RULE(RVY_AP_LM, perms & (RVY_AP_W | RVY_AP_EL));
    }
#endif

    /* rule 11 */
    if (lvbits > 0) {
        PERM_RULE(RVY_AP_SL, perms & RVY_AP_C);
    }

#if CAP_CC(ADDR_WIDTH) == 32
    /* rule 12 */
    if (lvbits > 0) {
        /* SL requires LM and (X or W) */
        PERM_RULE(RVY_AP_SL, perms & RVY_AP_LM);
        PERM_RULE(RVY_AP_SL, perms & (RVY_AP_X | RVY_AP_W));
    }

    /* rule 13 */
    target_ulong cmp_mask = RVY_AP_C | RVY_AP_LM;
    if (lvbits > 0) {
        cmp_mask |= (RVY_AP_EL | RVY_AP_SL);
    }
    PERM_RULE(RVY_AP_X,
              ((perms & cmp_mask) == 0) || ((perms & cmp_mask) == cmp_mask));
#endif

    /* rule 14 */
    PERM_RULE(RVY_AP_ASR, perms & RVY_AP_X);

    *perms_result = perms;
    return updated;
}

static inline bool fix_up_exec_mode(G_GNUC_UNUSED CPUArchState *env,
                                    G_GNUC_UNUSED CheriExecMode *mode,
                                    G_GNUC_UNUSED target_ulong perms)
{
#if CAP_CC(ADDR_WIDTH) == 32
    /* ACPERM rule 15 (RV32 only in 0.9.3) */
    _Static_assert(CHERI_EXEC_CAPMODE == 1, "wrong definition of modes");
    if (*mode == 1) {
        bool hybrid_support = riscv_feature(env, RISCV_FEATURE_CHERI_HYBRID);
        if (!(perms & RVY_AP_X) || !hybrid_support) {
            *mode = 0;
            return true;
        }
    }
#endif
    return false;
}

#endif
/**
 * Updates @p cap and @p perms to legal and encodable values.
 * For example ASR can only be set if X is also set.
 * @returns true if the values are legal, false if changes were made.
 */
static inline bool cap_legalize_perms(G_GNUC_UNUSED CPUArchState *env,
                                      G_GNUC_UNUSED const cap_register_t *cap,
                                      G_GNUC_UNUSED target_ulong *perms)
{
    /*
     * For now the only architecture with constraints on the valid permissions
     * is the RISC-V standard version.
     */
#ifdef TARGET_CHERI_RISCV_STD
    return fix_up_ap(env, perms) == false;
#else
    return true;
#endif
}

static inline void cap_set_perms(CPUArchState *env, cap_register_t *c,
                                 target_ulong perms)
{
    bool success = CAP_cc(set_permissions)(c, perms);
    assert(success);
}

#ifndef TARGET_AARCH64
static inline CheriExecMode cap_get_exec_mode(const cap_register_t *c)
{
    /*
     * NB: For the RISC-V standard these values are inverted, but this will
     * be handled by the next cheri-compressed-cap upgrade.
     */
    return CAP_cc(get_flags)(c) == 1 ? CHERI_EXEC_CAPMODE : CHERI_EXEC_INTMODE;
}

static inline void cap_set_exec_mode(cap_register_t *c, CheriExecMode mode)
{
    /*
     * NB: For the RISC-V standard these values are inverted, but this will
     * be handled by the next cheri-compressed-cap upgrade.
     */
    CAP_cc(update_flags)(c, mode == CHERI_EXEC_CAPMODE ? 1 : 0);
}
#endif

static inline bool cap_has_reserved_bits_set(const cap_register_t *c)
{
    return CAP_cc(get_reserved)(c) != 0;
}

/**
 * Returns true if the permissions encoding in @p c could not have been
 * produced by a valid ACPERM sequence.
 */
static inline bool
cap_has_invalid_perms_encoding(G_GNUC_UNUSED CPUArchState *env,
                               const cap_register_t *c)
{
#ifdef TARGET_CHERI_RISCV_STD
    target_ulong perms = cap_get_all_perms(c);
    return fix_up_ap(env, &perms) == true;
#else
    return false;
#endif
}

// The top of the capability (exclusive -- i.e., one past the end)
static inline target_ulong cap_get_top(const cap_register_t *c)
{
    // TODO: should handle last byte of address space properly
    cap_length_t top = c->_cr_top;
    return top > ~(target_ulong)0 ? ~(target_ulong)0 : (target_ulong)top;
}

static inline cap_length_t cap_get_length_full(const cap_register_t *c)
{
    return c->_cr_top - c->cr_base;
}

static inline target_ulong cap_get_length_sat(const cap_register_t *c)
{
#ifndef TARGET_AARCH64
    cheri_debug_assert((!c->cr_tag || c->_cr_top >= c->cr_base) &&
                       "Tagged capabilities must be in bounds!");
#endif
    cap_length_t length = cap_get_length_full(c);
    // Clamp the length to ~(target_ulong)0
    return length > ~(target_ulong)0 ? ~(target_ulong)0 : (target_ulong)length;
}

static inline cap_length_t cap_get_top_full(const cap_register_t *c)
{
    return c->_cr_top;
}

static inline bool cap_otype_is_reserved(target_ulong otype)
{
    cheri_debug_assert(otype <= CAP_MAX_REPRESENTABLE_OTYPE &&
                       "Should only be called for in-range otypes!");
    /* Silence -Wtype-limits by using an intermediate variable. */
    target_ulong min = CAP_CC(MIN_RESERVED_OTYPE);
    return otype >= min && otype <= CAP_CC(MAX_RESERVED_OTYPE);
}

static inline target_ulong cap_get_otype_unsigned(const cap_register_t *c)
{
    target_ulong otype = CAP_cc(get_otype)(c);
    /*
     * It is impossible to have out-of-range otypes in all targets for the
     * currently used capability compression schemes.
     */
    cheri_debug_assert(otype <= CAP_MAX_REPRESENTABLE_OTYPE);
    return otype;
}

static inline target_long cap_get_otype_signext(const cap_register_t *c)
{
    target_ulong result = cap_get_otype_unsigned(c);
    if (result > CAP_MAX_REPRESENTABLE_OTYPE) {
        // raw bits loaded from memory
        assert(!c->cr_tag && "Capabilities with otype > max cannot be tagged!");
        return result;
    }
#if defined(TARGET_AARCH64) || defined(TARGET_CHERI_RISCV_STD)
    /*
     * Morello and the RISC-V standard encodings do not sign extend like the
     * ISAv9 version of CHERI.
     */
    return result;
#else
    /*
     * We "sign" extend to a 64-bit number by subtracting the maximum:
     * e.g. for 64-bit CHERI-RISC-V unsigned 2^18-1 maps to 2^64-1
     */
    return result < CAP_CC(MIN_RESERVED_OTYPE)
               ? result
               : result - CAP_MAX_REPRESENTABLE_OTYPE - 1;
#endif
}

static inline bool cap_exactly_equal(const cap_register_t *cbp,
                                     const cap_register_t *ctp)
{
    return CAP_cc(exactly_equal)(cbp, ctp);
}

static inline bool cap_is_sealed_with_type(const cap_register_t *c)
{
    /* cap_otype_is_reserved() returns true for unsealed capabilities. */
    return !cap_otype_is_reserved(cap_get_otype_unsigned(c));
}

static inline bool cap_is_sealed_with_reserved_otype(const cap_register_t *c)
{
    target_ulong otype = cap_get_otype_unsigned(c);
    return cap_otype_is_reserved(otype) && otype != CAP_OTYPE_UNSEALED;
}

static inline bool cap_is_unsealed(const cap_register_t *c)
{
    target_ulong otype = cap_get_otype_unsigned(c);
    return otype == CAP_OTYPE_UNSEALED;
}

static inline void cap_set_sealed(cap_register_t *c, uint32_t type)
{
    assert(c->cr_tag);
    assert(cap_is_unsealed(c) && "Should only use this with unsealed caps");
    assert(!cap_otype_is_reserved(type) &&
           "Can't use this to set reserved otypes");
    CAP_cc(update_otype)(c, type);
}

static inline void cap_set_unsealed(cap_register_t *c)
{
    assert(c->cr_tag);
    assert(cap_is_sealed_with_type(c) &&
           "should not use this to unseal reserved types");
    CAP_cc(update_otype)(c, CAP_OTYPE_UNSEALED);
}

static inline bool cap_is_sealed_entry(const cap_register_t *c)
{
    return cap_get_otype_unsigned(c) == CAP_OTYPE_SENTRY;
}

static inline void cap_unseal_reserved_otype(cap_register_t *c)
{
    assert(c->cr_tag && cap_is_sealed_with_reserved_otype(c) &&
           "Should only be used with reserved object types");
    CAP_cc(update_otype)(c, CAP_OTYPE_UNSEALED);
}

static inline void cap_unseal_entry(cap_register_t *c)
{
    assert(c->cr_tag && cap_is_sealed_entry(c) &&
           "Should only be used with sentry capabilities");
    CAP_cc(update_otype)(c, CAP_OTYPE_UNSEALED);
}

static inline void cap_make_sealed_entry(cap_register_t *c)
{
    assert(c->cr_tag && cap_is_unsealed(c) &&
           "Should only be used with unsealed capabilities");
    CAP_cc(update_otype)(c, CAP_OTYPE_SENTRY);
}

// Check if num_bytes bytes at addr can be read using capability c
static inline bool cap_is_in_bounds(const cap_register_t *c, target_ulong addr,
                                    size_t num_bytes)
{
#ifdef TARGET_AARCH64
    // Invalid exponent caps are always considered out of bounds.
    if (!c->cr_bounds_valid)
        return false;
#endif
    if (addr < cap_get_base(c)) {
        return false;
    }
    /*
     * Use __builtin_add_overflow to detect avoid wrapping around the end of
     * the address space. However, we have to be careful to allow accesses to
     * the last byte (wrapping to exactly zero) since that is fine when
     * checking against given an omnipotent capability.
     */
    target_ulong access_end_addr = 0;
    if (unlikely(__builtin_add_overflow(addr, num_bytes, &access_end_addr))) {
        /* Only do the extended precision addition if we do overflow. */
        if (cap_get_top_full(c) >= (cap_length_t)addr + num_bytes) {
            return true;
        }
        if (c->cr_tag)
            warn_report("Found capability access that wraps around: 0x" TARGET_FMT_lx
                        " + %zd. Authorizing cap: " PRINT_CAP_FMTSTR,
                        addr, num_bytes, PRINT_CAP_ARGS(c));
        return false;
    }
    if (access_end_addr > cap_get_top_full(c)) {
        return false;
    }
    return true;
}

static inline QEMU_ALWAYS_INLINE bool
addr_in_cap_bounds(const cap_register_t *c, target_ulong addr)
{
    return addr >= cap_get_base(c) && addr < cap_get_top_full(c);
}

static inline QEMU_ALWAYS_INLINE bool
cap_cursor_in_bounds(const cap_register_t *c)
{
    return addr_in_cap_bounds(c, cap_get_cursor(c));
}

static inline QEMU_ALWAYS_INLINE bool cap_has_perms(const cap_register_t *reg,
                                                    uint32_t perms)
{
    return (cap_get_all_perms(reg) & perms) == perms;
}

static inline bool cap_is_representable(const cap_register_t *c)
{
    return CAP_cc(is_representable_cap_exact)(c);
}

static inline void assert_valid_jump_target(const cap_register_t *target)
{
    // All of these properties should have been checked in the helper:
    cheri_debug_assert(cap_is_unsealed(target));
    cheri_debug_assert(cap_has_perms(target, CAP_PERM_EXECUTE));
    cheri_debug_assert(target->cr_tag);
    cheri_debug_assert(cap_cursor_in_bounds(target));
}

/*
 * Convert an integer to a capability that holds the integer the address field.
 *
 * The contents of other fields will be those of a NULL capability.
 */
static inline QEMU_ALWAYS_INLINE cap_register_t
make_capability_from_int(CPUArchState *env, target_ulong addr)
{
    uint8_t lvbits = CAP_CC(MANDATORY_LEVEL_BITS);
#ifdef TARGET_CHERI_RISCV_STD
    /* Keep the number of level bits from the input capability. */
    lvbits = env_archcpu(env)->cfg.lvbits;
#endif
    cap_register_t result = CAP_cc(make_null_derived_cap_ext)(addr, lvbits);
    result.cr_extra = CREG_FULLY_DECOMPRESSED;
    return result;
}

static inline QEMU_ALWAYS_INLINE cap_register_t
make_null_capability(CPUArchState *env)
{
    return make_capability_from_int(env, 0);
}

static inline bool is_null_capability(CPUArchState *env,
                                      const cap_register_t *cp)
{
    cap_register_t null = make_null_capability(env);
    /*
     * This also compares padding, but it should always be zeroed if the correct
     * initialization function were used.
     */
    return memcmp(&null, cp, sizeof(cap_register_t)) == 0;
}

/*
 * Clear the tag bit of a capability that became unrepresentable and update
 * the cursor to point to @p addr.
 * Only clearing the tag bit instead of updating the offset to be semantically
 * valid improves timing on the FPGA and with the shift towards address-based
 * interpretation it becomes less useful to retain the offset value.
 *
 * Previous behaviour was to create a NULL-derived capability instead
 *
 */
static inline cap_register_t *cap_mark_unrepresentable(target_ulong addr,
                                                       cap_register_t *cr)
{
    // Clear the tag and update the address:
    cr->_cr_cursor = addr;
    cr->cr_tag = false;
    /*
     * Recompute the decompressed bounds relative to the new address. In most
     * cases they will refer to a different region of memory now.
     *
     * Keep the number of level bits from the input capability. Reading it
     * from the cpu configuration would need lots of refactoring.
     */
    uint8_t lvbits = CAP_CC(MANDATORY_LEVEL_BITS);
#ifdef TARGET_CHERI_RISCV_STD
    lvbits = cr->cr_lvbits;
#endif
    CAP_cc(decompress_raw_ext)(cr->cr_pesbt, addr, false, lvbits, cr);
    cr->cr_extra = CREG_FULLY_DECOMPRESSED;
    return cr;
}

/*
 * attribute unused marks a potentially unused paramter. We keep it to squelch
 * compiler warnings for architectures other than risc-v.
 */
static inline void set_max_perms_capability(G_GNUC_UNUSED CPUArchState *env,
                                            cap_register_t *crp,
                                            target_ulong cursor)
{
#if defined(TARGET_CHERI_RISCV_STD)
    /*
     * If hybrid mode is supported, the infinite capability has to set integer
     * pointer mode (M = 1).
     */
    CAP_CC(Mode) m = riscv_feature(env, RISCV_FEATURE_CHERI_HYBRID)
                         ? CAP_CC(MODE_INT)
                         : CAP_CC(MODE_CAP);
    uint8_t lvbits = env_archcpu(env)->cfg.lvbits;
    *crp = CAP_cc(make_max_perms_cap_ext)(0, cursor, CAP_MAX_TOP, m, lvbits);
#else
    *crp = CAP_cc(make_max_perms_cap)(0, cursor, CAP_MAX_TOP);

#endif
    crp->cr_extra = CREG_FULLY_DECOMPRESSED;
}

static inline QEMU_ALWAYS_INLINE bool
is_representable_cap_with_addr(const cap_register_t *cap, target_ulong new_addr)
{
    return CAP_cc(is_representable_with_addr)(
        cap, new_addr, /*precise_representable_check=*/true);
}

int gdb_get_capreg(GByteArray *buf, const cap_register_t *cap);
int gdb_get_general_purpose_capreg(GByteArray *buf, CPUArchState *env,
                                   unsigned regnum);

#define raise_cheri_exception(env, cause, reg)                                 \
    raise_cheri_exception_impl(env, cause, reg, 0, true, _host_return_address)

#define raise_cheri_exception_addr(env, cause, reg, addr)                      \
    raise_cheri_exception_impl(env, cause, reg, addr, true,                    \
                               _host_return_address)

#ifdef TARGET_AARCH64
#define raise_cheri_exception_if(env, cause, addr, reg)                        \
    raise_cheri_exception_impl_if_wnr(env, cause, reg, addr, true, /*pc=*/0,   \
                                      true, false)
#define raise_cheri_exception_addr_wnr(env, cause, reg, addr, is_write)        \
    raise_cheri_exception_impl_if_wnr(env, cause, reg, addr, true,             \
                                      _host_return_address, false, is_write)
#else
#ifdef TARGET_CHERI_RISCV_STD_093
#define raise_cheri_exception_if(env, cause, addr, reg)                        \
    raise_cheri_exception_with_093_type(env, cause, CapEx093_Type_InstrAccess, \
                                        reg, addr, /*instavail=*/true,         \
                                        /*pc=*/0)
#else
#define raise_cheri_exception_if(env, cause, addr, reg)                        \
    raise_cheri_exception_impl(env, cause, reg, addr, true, /*pc=*/0)
#endif
#define raise_cheri_exception_addr_wnr(env, cause, reg, addr, is_write)        \
    raise_cheri_exception_addr(env, cause, reg, addr)
#endif

#ifdef TARGET_CHERI_RISCV_STD_093
#define raise_cheri_exception_branch_impl(env, cause, reg, addr, retpc)        \
    raise_cheri_exception_with_093_type(env, cause, CapEx093_Type_Branch, reg, \
                                        addr, /*instavail=*/true, retpc)
#define raise_cheri_exception_branch(env, cause, reg)                          \
    raise_cheri_exception_branch_impl(env, cause, reg, 0, _host_return_address)
#else
#define raise_cheri_exception_branch(env, cause, reg)                          \
    raise_cheri_exception(env, cause, reg)
#define raise_cheri_exception_branch_impl(env, cause, reg, addr, retpc)        \
    raise_cheri_exception_impl(env, cause, reg, addr, true, retpc)
#endif

static inline void cap_set_cursor(cap_register_t *cap, uint64_t new_addr)
{
    if (!is_representable_cap_with_addr(cap, new_addr)) {
        cap_mark_unrepresentable(new_addr, cap);
    } else {
        cap->_cr_cursor = new_addr;
    }
}

static inline void cap_increment_offset(cap_register_t *cap, uint64_t offset)
{
    uint64_t new_addr = cap->_cr_cursor + offset;
    return cap_set_cursor(cap, new_addr);
}

#ifndef TARGET_CHERI_RISCV_STD
/** Encode the permissions for the in-memory capability representation. */
static inline target_ulong cap_encode_perms(target_ulong perms)
{
    /* This assumes permissions are single-bit checks (Morello, ISAv9) */
    cap_register_t reg =
        CAP_cc(make_null_derived_cap_ext)(0, CAP_CC(MANDATORY_LEVEL_BITS));
    assert(reg.cr_pesbt == CAP_MEM_XOR_MASK);
    CAP_cc(set_permissions)(&reg, perms);
    return reg.cr_pesbt ^ CAP_MEM_XOR_MASK;
}
#endif

#endif /* TARGET_CHERI */
