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

static inline uint64_t cap_get_length64(const cap_register_t* c) {
    // TODO: should handle last byte of address space properly
    cheri_debug_assert((!c->cr_tag || c->_cr_top >= c->cr_base) &&
                       "Tagged capabilities must be in bounds!");
    unsigned __int128 length = c->_cr_top - c->cr_base;
    if (unlikely(c->_cr_top < c->cr_base)) {
        return (uint64_t)length; // negative length -> strip top bits
    }
    // Otherwise clamp the length to UINT64_MAX
    return length > UINT64_MAX ? UINT64_MAX : (uint64_t)length;
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
    // "sign" extend to a 64-bit number by subtracting the maximum: 2^24-1 -> 2^64-1
    return result < CAP_LAST_NONRESERVED_OTYPE ? result : result - CAP_MAX_REPRESENTABLE_OTYPE - 1;
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
    return c->cr_otype <= CAP_LAST_NONRESERVED_OTYPE;
}

// Check if num_bytes bytes at addr can be read using capability c
static inline bool cap_is_in_bounds(const cap_register_t* c, uint64_t addr, uint64_t num_bytes) {
    if (addr < cap_get_base(c)) {
        return false;
    }
    // Use __builtin_add_overflow to avoid wrapping around the end of the addres space
    uint64_t access_end_addr = 0;
    if (__builtin_add_overflow(addr, num_bytes, &access_end_addr)) {
        warn_report("Found capability access that wraps around: 0x%" PRIx64
                    " + %" PRId64 ". Authorizing cap: " PRINT_CAP_FMTSTR,
                    addr, num_bytes, PRINT_CAP_ARGS(c));
        return false;
    }
    if (access_end_addr > cap_get_top(c)) {
        return false;
    }
    return true;
}

static inline bool cap_has_perms(const cap_register_t* reg, uint32_t perms)
{
    return (reg->cr_perms & perms) == perms;
}

static inline bool cap_is_unsealed(const cap_register_t* c) {
    // TODO: how should we treat the other reserved types? as sealed?
    // TODO: what about untagged capabilities with out-of-range otypes?
    _Static_assert(CAP_MAX_REPRESENTABLE_OTYPE == CAP_OTYPE_UNSEALED, "");
    if (c->cr_tag) {
        cheri_debug_assert(c->cr_otype <= CAP_MAX_REPRESENTABLE_OTYPE);
    }
    return c->cr_otype >= CAP_OTYPE_UNSEALED;
}

static inline void cap_set_sealed(cap_register_t* c, uint32_t type) {
    assert(c->cr_tag);
    assert(c->cr_otype == CAP_OTYPE_UNSEALED && "should not use this on caps with reserved otypes");
    assert(type <= CAP_LAST_NONRESERVED_OTYPE);
    _Static_assert(CAP_LAST_NONRESERVED_OTYPE < CAP_OTYPE_UNSEALED, "");
    c->cr_otype = type;
}

static inline void cap_set_unsealed(cap_register_t* c) {
    assert(c->cr_tag);
    assert(cap_is_sealed_with_type(c));
    assert(c->cr_otype <= CAP_LAST_NONRESERVED_OTYPE && "should not use this to unsealed reserved types");
    c->cr_otype = CAP_OTYPE_UNSEALED;
}

static inline bool cap_is_sealed_entry(const cap_register_t* c) {
    return c->cr_otype == CAP_OTYPE_SENTRY;
}

static inline void cap_unseal_entry(cap_register_t* c) {
    assert(c->cr_tag && cap_is_sealed_entry(c) && "Should only be used with sentry capabilities");
    c->cr_otype = CAP_OTYPE_UNSEALED;
}

static inline void cap_make_sealed_entry(cap_register_t* c) {
    assert(c->cr_tag && cap_is_unsealed(c) && "Should only be used with unsealed capabilities");
    c->cr_otype = CAP_OTYPE_SENTRY;
}


static inline cap_register_t *null_capability(cap_register_t *cp)
{
    memset(cp, 0, sizeof(*cp)); // Set everything to zero including padding
    // For CHERI128 max length is 1 << 64 (__int128) for CHERI256 UINT64_MAX
    cp->_cr_top = CAP_MAX_TOP;
    cp->cr_otype = CAP_OTYPE_UNSEALED; // and otype should be unsealed
#ifdef CHERI_128
    cp->cr_ebt = CC128_RESET_EBT;
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
#ifdef CHERI_128
    // re-compute the compressed representation to ensure we have the same
    // resulting values for offset/base/top as the hardware:
    // TODO: this could go away if we used a cap_register_t representation
    // more like the hardware and sail.
    uint64_t pesbt = cc128_compress_raw(cr);
    cc128_decompress_raw(pesbt, addr, false, cr);
#endif
    return cr;
}

static inline void set_max_perms_capability(cap_register_t *crp, uint64_t cursor)
{
    crp->cr_tag = 1;
    crp->cr_perms = CAP_PERMS_ALL;
    crp->cr_uperms = CAP_UPERMS_ALL;
    // crp->cr_cursor = 0UL;
    crp->cr_base = 0UL;
    crp->_cr_cursor = cursor;
    // For CHERI128 max length is 1 << 64 (__int128) for CHERI256 UINT64_MAX
    crp->_cr_top = CAP_MAX_TOP;
    crp->cr_otype = CAP_OTYPE_UNSEALED;
    crp->cr_flags = 0;
    crp->cr_reserved = 0;
#ifdef CHERI_128
    crp->cr_ebt = CC128_RESET_EBT;
    cheri_debug_assert(cc128_is_representable_cap_exact(crp));
#endif
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

#endif /* TARGET_CHERI */

