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

#define PRINT_CAP_FMTSTR_L1                                                    \
    "v:%d s:%d p:%08x f:%d b:%016" PRIx64 " l:%016" PRIx64
#define COMBINED_PERMS_VALUE(cr)                                               \
    (unsigned)((((cr)->cr_uperms & CAP_UPERMS_ALL) << CAP_UPERMS_SHFT) |       \
               ((cr)->cr_perms & CAP_PERMS_ALL))
#define PRINT_CAP_ARGS_L1(cr)                                                  \
    (cr)->cr_tag, cap_is_sealed_with_type(cr), COMBINED_PERMS_VALUE(cr),       \
        (cr)->cr_flags, cap_get_base(cr), cap_get_length64(cr)
#define PRINT_CAP_FMTSTR_L2 "o:%016" PRIx64 " t:%x"
#define PRINT_CAP_ARGS_L2(cr) (uint64_t)cap_get_offset(cr), (cr)->cr_otype


#define PRINT_CAP_FMTSTR PRINT_CAP_FMTSTR_L1 " " PRINT_CAP_FMTSTR_L2
#define PRINT_CAP_ARGS(cr) PRINT_CAP_ARGS_L1(cr), PRINT_CAP_ARGS_L2(cr)

#define GET_HOST_RETPC() const uintptr_t _host_return_address = GETPC()

#ifdef TARGET_CHERI

// Import the appropriate functions from CC lib without namespacing
#if defined(CHERI_128)
#define CC_LIB_FN(X) cc128_##X
#elif defined(CHERI_64)
#define CC_LIB_FN(X) cc64_##X
#else
#error "Unknown compressed lib format required"
#endif

#define cap_set_decompressed_cr_otype CC_LIB_FN(cap_set_decompressed_cr_otype)
#define cap_set_decompressed_cr_perms CC_LIB_FN(cap_set_decompressed_cr_perms)
#define cap_set_decompressed_cr_uperms CC_LIB_FN(cap_set_decompressed_cr_uperms)
#define cap_set_decompressed_cr_flags CC_LIB_FN(cap_set_decompressed_cr_flags)
#define make_max_perms_cap CC_LIB_FN(make_max_perms_cap)

static inline uint64_t cap_get_cursor(const cap_register_t* c) {
    return c->_cr_cursor;
}

// Getters to allow future changes to the cap_register_t struct layout:
static inline uint64_t cap_get_base(const cap_register_t* c) {
    return c->cr_base;
}

static inline cap_offset_t cap_get_offset(const cap_register_t* c) {
    return (cap_offset_t)c->_cr_cursor - (cap_offset_t)c->cr_base;
}

// The top of the capability (exclusive -- i.e., one past the end)
static inline uint64_t cap_get_top(const cap_register_t* c) {
    // TODO: should handle last byte of address space properly
    unsigned __int128 top = c->_cr_top;
    return top > UINT64_MAX ? UINT64_MAX : (uint64_t)top;
}

static inline unsigned __int128 cap_get_length65(const cap_register_t* c) {
    return c->_cr_top - c->cr_base;
}

static inline uint64_t cap_get_length64(const cap_register_t* c) {
    cheri_debug_assert((!c->cr_tag || c->_cr_top >= c->cr_base) &&
        "Tagged capabilities must be in bounds!");
    unsigned __int128 length = cap_get_length65(c);
    // Clamp the length to UINT64_MAX
    return length > UINT64_MAX ? UINT64_MAX : (uint64_t)length;
}

static inline unsigned __int128 cap_get_top65(const cap_register_t* c) {
    return c->_cr_top;
}

static inline int64_t cap_get_otype(const cap_register_t* c) {
    uint64_t result = c->cr_otype;
    if (result > CAP_MAX_REPRESENTABLE_OTYPE) {
        // raw bits loaded from memory
        assert(!c->cr_tag && "Capabilities with otype > max cannot be tagged!");
        return result;
    }

#ifdef TARGET_AARCH64
    // Morello does not sign extend like the rest of CHERI
    return result;
#else
    // "sign" extend to a 64-bit number by subtracting the maximum: 2^24-1 -> 2^64-1
    return result < CAP_LAST_NONRESERVED_OTYPE ? result : result - CAP_MAX_REPRESENTABLE_OTYPE - 1;
#endif
}

static inline bool cap_exactly_equal(const cap_register_t *cbp, const cap_register_t *ctp)
{
#ifdef CHERI_128
    return cc128_exactly_equal(cbp, ctp);
#else
    return cc256_exactly_equal(cbp, ctp);
#endif
}

static inline bool cap_is_sealed_with_type(const cap_register_t* c) {
    // TODO: how should we treat the other reserved types? as sealed?
    // TODO: what about untagged capabilities with out-of-range otypes?
    if (c->cr_tag) {
        cheri_debug_assert(c->cr_otype <= CAP_MAX_REPRESENTABLE_OTYPE);
    }
#ifdef TARGET_AARCH64
    // Morello has positive signed otypes
    return c->cr_otype > CAP_LAST_NONRESERVED_OTYPE;
#else
    return c->cr_otype <= CAP_LAST_NONRESERVED_OTYPE;
#endif
}

// Check if num_bytes bytes at addr can be read using capability c
static inline bool cap_is_in_bounds(const cap_register_t* c, uint64_t addr, size_t num_bytes) {
    cheri_debug_assert(num_bytes != 0);
#ifdef TARGET_AARCH64
    // Invalid exponant caps are always considered out of bounds.
    if (!c->cr_bounds_valid)
        return false;
#endif
    if (addr < cap_get_base(c)) {
        return false;
    }
    // Use __builtin_add_overflow to avoid wrapping around the end of the addres space
    uint64_t access_end_addr = 0;
    if (__builtin_add_overflow(addr, num_bytes, &access_end_addr)) {
        warn_report("Found capability access that wraps around: 0x%" PRIx64
                    " + %zd. Authorizing cap: " PRINT_CAP_FMTSTR,
                    addr, num_bytes, PRINT_CAP_ARGS(c));
        return false;
    }
    if (access_end_addr > cap_get_top(c)) {
        return false;
    }
    return true;
}

static inline bool cap_cursor_in_bounds(const cap_register_t *c)
{
    return cap_get_cursor(c) >= cap_get_base(c) &&
           cap_get_cursor(c) < cap_get_top65(c);
}

static inline bool cap_has_perms(const cap_register_t* reg, uint32_t perms)
{
    return (reg->cr_perms & perms) == perms;
}

static inline bool cap_is_unsealed(const cap_register_t* c) {
    // TODO: how should we treat the other reserved types? as sealed?
    // TODO: what about untagged capabilities with out-of-range otypes?
#ifdef TARGET_AARCH64
    return c->cr_otype == CAP_OTYPE_UNSEALED;
#else
    _Static_assert(CAP_MAX_REPRESENTABLE_OTYPE == CAP_OTYPE_UNSEALED, "");
    if (c->cr_tag) {
        cheri_debug_assert(c->cr_otype <= CAP_MAX_REPRESENTABLE_OTYPE);
    }
    return c->cr_otype >= CAP_OTYPE_UNSEALED;
#endif
}

static inline void cap_set_sealed(cap_register_t* c, uint32_t type) {
#ifdef TARGET_AARCH64
    type &= CC128_FIELD_OTYPE_MASK_NOT_SHIFTED;
#endif
    assert(c->cr_tag);
    assert(c->cr_otype == CAP_OTYPE_UNSEALED && "should not use this on caps with reserved otypes");
    assert(type <= CAP_LAST_NONRESERVED_OTYPE);
#ifndef TARGET_AARCH64
    _Static_assert(CAP_LAST_NONRESERVED_OTYPE < CAP_OTYPE_UNSEALED, "");
#endif
    cap_set_decompressed_cr_otype(c, type);
}

static inline void cap_set_unsealed(cap_register_t* c) {
    assert(c->cr_tag);
#ifndef TARGET_AARCH64
    assert(cap_is_sealed_with_type(c));
    assert(c->cr_otype <= CAP_LAST_NONRESERVED_OTYPE && "should not use this to unsealed reserved types");
#endif
    cap_set_decompressed_cr_otype(c, CAP_OTYPE_UNSEALED);
}

static inline bool cap_is_sealed_entry(const cap_register_t* c) {
    return c->cr_otype == CAP_OTYPE_SENTRY;
}

static inline void cap_unseal_entry(cap_register_t* c) {
    assert(c->cr_tag && cap_is_sealed_entry(c) && "Should only be used with sentry capabilities");
    cap_set_decompressed_cr_otype(c, CAP_OTYPE_UNSEALED);
}

static inline void cap_make_sealed_entry(cap_register_t* c) {
    assert(c->cr_tag && cap_is_unsealed(c) && "Should only be used with unsealed capabilities");
    cap_set_decompressed_cr_otype(c, CAP_OTYPE_SENTRY);
}

static inline bool cap_is_representable(const cap_register_t* c) {
#ifdef CHERI_128
    return cc128_is_representable_cap_exact(c);
#else
#error "!CHERI128 is not supported"
    return true;
#endif
}

static inline void assert_valid_jump_target(const cap_register_t *target)
{
    // All of these properties should have been checked in the helper:
    cheri_debug_assert(cap_is_unsealed(target));
    cheri_debug_assert(cap_has_perms(target, CAP_PERM_EXECUTE));
    cheri_debug_assert(target->cr_tag);
    cheri_debug_assert(cap_cursor_in_bounds(target));
}

static inline cap_register_t *null_capability(cap_register_t *cp)
{
    memset(cp, 0, sizeof(*cp)); // Set everything to zero including padding
    // For CHERI128 max length is 1 << 64 (__int128) for CHERI256 UINT64_MAX
    cp->_cr_top = CAP_MAX_TOP;
    cp->cr_otype = CAP_OTYPE_UNSEALED; // and otype should be unsealed
#ifdef CHERI_128
    cp->cached_pesbt = CC128_NULL_PESBT;
    cheri_debug_assert(cc128_is_representable_cap_exact(cp));
#endif
    return cp;
}

static inline bool is_null_capability(const cap_register_t *cp)
{
    cap_register_t null;
    // This also compares padding but it should always be NULL assuming
    // null_capability() was used
    return memcmp(null_capability(&null), cp, sizeof(cap_register_t)) == 0;
}

/*
 * Convert 64-bit integer into a capability that holds the integer in
 * its offset field.
 *
 *       cap.base = 0, cap.tag = false, cap.offset = x
 *
 * The contents of other fields of int to cap depends on the capability
 * compression scheme in use (e.g. 256-bit capabilities or 128-bit
 * compressed capabilities). In particular, with 128-bit compressed
 * capabilities, length is not always zero. The length of a capability
 * created via int to cap is not semantically meaningful, and programs
 * should not rely on it having any particular value.
 */
static inline const cap_register_t*
int_to_cap(uint64_t x, cap_register_t *cr)
{

    (void)null_capability(cr);
    cr->_cr_cursor = x;
    return cr;
}

/**
 * Clear the tag bit of a capability that became unrepresenable and update
 * the cursor to point to @p addr.
 * Only clearing the tag bit instead of updating the offset to be semantically
 * valid improves timing on the FPGA and with the shift towards address-based
 * interpretation it becomes less useful to retain the offset value.
 *
 * Previous behaviour was to use int_to_cap instead
 *
 */
static inline cap_register_t *cap_mark_unrepresentable(uint64_t addr, cap_register_t *cr)
{
    // Clear the tag and update the address:
    cr->_cr_cursor = addr;
    cr->cr_tag = false;
#ifdef TARGET_AARCH64
    // Morello never modifies pesbt if representability changes, instead bounds just change
    cc128_decompress_raw(cr->cached_pesbt, addr, false, cr);
#else
#ifdef CHERI_128
    // re-compute the compressed representation to ensure we have the same
    // resulting values for offset/base/top as the hardware:
    // TODO: this could go away if we used a cap_register_t representation
    // more like the hardware and sail.
    uint64_t pesbt = cc128_compress_raw(cr);
    cc128_decompress_raw(pesbt, addr, false, cr);
#endif
#endif
    return cr;
}

static inline void set_max_perms_capability(cap_register_t *crp, uint64_t cursor)
{
    *crp = make_max_perms_cap(0, cursor, CAP_MAX_TOP);
}

#if defined(CHERI_128) && !defined(CHERI_MAGIC128)
static inline bool
is_representable_cap_with_addr(const cap_register_t* cap, uint64_t new_addr)
{
    return cc128_is_representable_with_addr(cap, new_addr);
}
static inline bool
is_representable_cap_when_sealed_with_addr(const cap_register_t* cap, uint64_t new_addr)
{
    cheri_debug_assert(cap_is_unsealed(cap));
    return cc128_is_representable_new_addr(true, cap_get_base(cap), cap_get_length65(cap), cap_get_cursor(cap), new_addr);
}
#else
static inline bool
is_representable_cap_with_addr(const cap_register_t* cap, uint64_t new_offset)
{
    return true;
}
static inline bool
is_representable_cap_when_sealed_with_addr(const cap_register_t* cap, uint64_t new_offset)
{
    return true;
}
#endif

int gdb_get_capreg(GByteArray *buf, const cap_register_t *cap);
int gdb_get_general_purpose_capreg(GByteArray *buf, CPUArchState *env,
                                   unsigned regnum);

#define raise_cheri_exception(env, cause, reg)                                 \
    raise_cheri_exception_impl(env, cause, reg, true, _host_return_address)


static inline void
cap_set_cursor(cap_register_t* cap, uint64_t new_addr) {
    if (!is_representable_cap_with_addr(cap, new_addr)) {
        cap_mark_unrepresentable(new_addr, cap);
    } else {
        cap->_cr_cursor = new_addr;
    }
}

static inline void
cap_increment_offset(cap_register_t* cap, uint64_t offset) {
    uint64_t new_addr = cap->_cr_cursor + offset;
    return cap_set_cursor(cap, new_addr);
}

#endif /* TARGET_CHERI */

