/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2020 Alex Richardson
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

#include "contrib/sail.h"

/* Provide the Morello-specific APIs for sail_wrapper_common.c */

#define SAIL_COMPRESSION_GENERATED_C_FILE "contrib/sail_compression_128m.c"
// Two 65-bit bounds and a bool (for exactness)
#define sail_bounds_tuple ztuple_z8z5bvzCz0z5bvzCz0z5boolz9

#define SAIL_WRAPPER_CC_IS_MORELLO
#define SAIL_WRAPPER_CC_FORMAT_LOWER 128m
#define SAIL_WRAPPER_CC_FORMAT_UPPER 128M

#include "sail_wrapper_common.c"

static inline uint64_t extract_low_bits(lbits bits) {
    fbits bits_low = CONVERT_OF(fbits, lbits)(bits, true);
    return (uint64_t)bits_low;
}

#define CHECK_FIELD_LENGTHS

static inline void check_length(lbits bits, uint64_t length) {
#ifdef CHECK_FIELD_LENGTHS
    sail_int len;
    CREATE(sail_int)(&len);
    length_lbits(&len, bits);
    assert(CONVERT_OF(mach_int, sail_int)(len) == length);
    KILL(sail_int)(&len);
#endif
}

// The Morello cap is just all the bits slammed together with no extra decode
static lbits to_sail_cap(uint64_t mem_pesbt, uint64_t mem_cursor, bool tag) {
    lbits cap_without_tag;
    pesbt_and_addr_to_sail_cap_bits(&cap_without_tag, mem_pesbt, mem_cursor);
    lbits capbits;
    CREATE(lbits)(&capbits);
    lbits captag;
    CREATE_OF(lbits, fbits)(&captag, tag ? 1 : 0, 1, true);
    append(&capbits, captag, cap_without_tag);
    KILL(lbits)(&captag);
    KILL(lbits)(&cap_without_tag);

    check_length(capbits, 129);
    return capbits;
}

static lbits cap_t_to_sail_cap(const _cc_cap_t* c) { return to_sail_cap(c->cr_pesbt, c->_cr_cursor, c->cr_tag); }

/* Exported API */
static _cc_cap_t from_sail_cap(const lbits* sail_cap) {
    _cc_cap_t result;
    memset(&result, 0, sizeof(result));

    // Now extract some fields
    // Bounds (base and top) and exponent
    struct sail_bounds_tuple bounds;
    _CC_CONCAT(create_, sail_bounds_tuple)(&bounds);
    sailgen_CapGetBounds(&bounds, *sail_cap);

    check_length(bounds.ztup0, 65);
    check_length(bounds.ztup1, 65);

    result.cr_base = extract_bits(bounds.ztup0, 0, 64);
    uint64_t top_hi = extract_bits(bounds.ztup1, 64, 1);
    uint64_t top_lo = extract_low_bits(bounds.ztup1);
    result._cr_top = (((_cc_length_t)top_hi) << 64) | (_cc_length_t)top_lo;
    _CC_CONCAT(kill_, sail_bounds_tuple)(&bounds);

    result.cr_bounds_valid = bounds.ztup2;
    result.cr_exp = sailgen_CapGetExponent(*sail_cap);

    // Address including flag bits
    result._cr_cursor = sailgen_CapGetValue(*sail_cap);
    _cc_N(set_permissions)(&result, sailgen_CapGetPermissions(*sail_cap));
    _cc_N(update_otype)(&result, sailgen_CapGetObjectType(*sail_cap));
    result.cr_tag = sailgen_CapGetTag(*sail_cap);

    // Fix these extra fields not really present in sail
    _cc_N(update_flags)(&result, 0);
    _cc_N(update_uperms)(&result, 0);
    result.cr_pesbt = extract_bits(*sail_cap, 64, 64);
    return result;
}

_cc_cap_t _cc_sail(decode_mem)(uint64_t mem_pesbt, uint64_t mem_cursor, bool tag) {
    lbits capbits = to_sail_cap(mem_pesbt, mem_cursor, tag);
    _cc_cap_t result = from_sail_cap(&capbits);
    KILL(lbits)(&capbits);
    return result;
}

_cc_cap_t _cc_sail(decode_raw)(uint64_t pesbt, uint64_t cursor, bool tag) {
    // There is no difference between raw and memory format for Morello
    return _cc_sail(decode_mem)(pesbt, cursor, tag);
}

uint64_t sail_compress_morello(const _cc_cap_t* csp) {
    lbits capbits = cap_t_to_sail_cap(csp);
    uint64_t mem_pesbt = extract_bits(capbits, 64, 64);
    KILL(lbits)(&capbits);
    return mem_pesbt;
}

/* Exported API */
struct cc128m_bounds_bits sail_extract_bounds_bits_128m(uint64_t pesbt) {
    lbits sailcap = to_sail_cap(pesbt, 0, false);
    struct cc128m_bounds_bits result = {.E = sailgen_CapGetExponent(sailcap),
                                        .B = sailgen_CapGetBottom(sailcap),
                                        .T = sailgen_CapGetTop(sailcap),
                                        .IE = sailgen_CapIsInternalExponent(sailcap)};
    KILL(lbits)(&sailcap);
    return result;
}

// There is no difference between raw and memory format for Morello
uint64_t sail_compress_raw_128m(const cc128m_cap_t* csp) { return sail_compress_morello(csp); }
uint64_t sail_compress_mem_128m(const cc128m_cap_t* csp) { return sail_compress_morello(csp); }

bool sail_setbounds_128m(cc128m_cap_t* cap, cc128m_length_t req_len) {
    lbits sail_len;
    CREATE(sail_cap_bits)(&sail_len);
    cc_length_t_to_sail_cap_bits(&sail_len, req_len);
    lbits sail_result;
    CREATE(lbits)(&sail_result);
    lbits capbits = cap_t_to_sail_cap(cap);
    sailgen_doCSetBounds(&sail_result, capbits, sail_len);
    KILL(lbits)(&sail_len);
    KILL(lbits)(&capbits);
    _cc_cap_t tmp = from_sail_cap(&sail_result);
    // Check if the resulting bounds were exact. The sail code does not return the value of lostBot/Top, so we check the
    // resulting bounds against the expected values. NB: We have to compare the low 56 bits of base and top since the
    // high bits and change of sign are not included in the exactness check.
    cc128_length_t req_base = sailgen_CapBoundsAddress(cap->_cr_cursor);
    cc128_length_t req_top = sailgen_CapBoundsAddress((cc128_length_t)cap->_cr_cursor + req_len);
    bool exact = sailgen_CapBoundsAddress(tmp._cr_top) == req_top && sailgen_CapBoundsAddress(tmp.cr_base) == req_base;
    *cap = tmp;
    KILL(lbits)(&sail_result);
    return exact;
}

bool sail_fast_is_representable_128m(const cc128m_cap_t* cap, cc128m_addr_t new_addr) {
    lbits sailcap = cap_t_to_sail_cap(cap);
    uint64_t increment = new_addr - cap->_cr_cursor;
    bool result = sailgen_CapIsRepresentableFast(sailcap, increment);
    KILL(lbits)(&sailcap);
    return result;
}

bool sail_precise_is_representable_128m(const cc128m_cap_t* cap, cc128m_addr_t new_addr) {
    lbits sailcap = cap_t_to_sail_cap(cap);
    bool result = sailgen_CapIsRepresentable(sailcap, new_addr);
    KILL(lbits)(&sailcap);
    return result;
}

_cc_cap_t sail_reset_capability_128m(void) {
    _cc_cap_t result;
    return from_sail_cap(&zCMAX);
    return result;
}

_cc_addr_t sail_null_pesbt_128m(void) {
    return 0; // Morello does not invert capabilities when loading/storing to/from memory
}

_cc_addr_t sail_reset_pesbt_128m(void) { return extract_bits(zCMAX, 64, 64); }
