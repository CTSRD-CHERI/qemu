/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2025 Alex Richardson
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

#define SAIL_INFINITE_CAP zinfinite_cap
#define sail_bounds_tuple zoptionzIz8bzCbz9zK
#define sail_bounds_tuple_base(t) (t).variants.zSomezIz8bzCbz9zK.ztup0
#define sail_bounds_tuple_top(t) (t).variants.zSomezIz8bzCbz9zK.ztup1

#include "sail_wrapper_common.c"

static inline void set_top_base_from_sail(const struct zCapability* sail, _cc_cap_t* c) {
    // Would be nice to have a stable name for this tuple:
    struct sail_bounds_tuple base_top;
    _CC_CONCAT(create_, sail_bounds_tuple)(&base_top);
    sailgen_getCapBoundsBits(&base_top, *sail);
    if (base_top.kind == Kind_zNonezIz8bzCbz9zK) {
        c->_cr_top = 0;
        c->cr_base = 0;
        c->cr_bounds_valid = false;
        assert(sailgen_boundsMalformed(*sail));
        return;
    }
    assert(base_top.kind == Kind_zSomezIz8bzCbz9zK);

    sail_int base_len;
    CREATE(sail_int)(&base_len);
    length_lbits(&base_len, sail_bounds_tuple_base(base_top));
    assert(CONVERT_OF(mach_int, sail_int)(base_len) == _CC_ADDR_WIDTH);
    c->cr_base = CONVERT_OF(fbits, lbits)(sail_bounds_tuple_base(base_top), true);
    KILL(sail_int)(&base_len);

    sail_int top_len;
    CREATE(sail_int)(&top_len);
    length_lbits(&top_len, sail_bounds_tuple_top(base_top));
    assert(CONVERT_OF(mach_int, sail_int)(top_len) == _CC_LEN_WIDTH);
    KILL(sail_int)(&top_len);
    fbits top_low = CONVERT_OF(fbits, lbits)(sail_bounds_tuple_top(base_top), true);
#if _CC_LEN_WIDTH <= 64
    c->_cr_top = top_low;
#else
    fbits top_high = extract_bits(sail_bounds_tuple_top(base_top), 64, 1);
    c->_cr_top = (((cc128_length_t)top_high) << 64) | (cc128_length_t)top_low;
#endif
    _CC_CONCAT(kill_, sail_bounds_tuple)(&base_top);
}

static _cc_addr_t _compress_sailcap_raw(struct zCapability sailcap) { return sailgen_capToMetadataBits(sailcap).zbits; }

static struct zCapability cap_t_to_sail_cap(const _cc_cap_t* c) {
    struct zCapability result;
    result = znull_cap;
    result.ztag = c->cr_tag;
    // TODO: remove this once we have sail with level support
    result.zreserved_0 = _CC_EXTRACT_FIELD(c->cr_pesbt, RESERVED0);
    result.zreserved_1 = _CC_EXTRACT_FIELD(c->cr_pesbt, RESERVED1);
    result.zaddress = c->_cr_cursor;
    result.zsealed = _cc_N(get_otype)(c) != 0;
    result.zsd_perms = _cc_N(get_uperms)(c);
    result.zap_m = _CC_EXTRACT_FIELD(c->cr_pesbt, AP_M);
    result.zcl = _CC_EXTRACT_FIELD(c->cr_pesbt, LEVEL);

    // Extract E,B,T,IE from the cr_pesbt field:
    _cc_addr_t fake_pesbt = c->cr_pesbt;
    _cc_bounds_bits c_bounds = _cc_N(extract_bounds_bits)(fake_pesbt);
    result.zinternal_exponent = c_bounds.IE;
    result.zE = c_bounds.E;
    result.zT = c_bounds.T;
    result.zB = c_bounds.B;
    return result;
}

static _cc_cap_t sail_cap_to_cap_t(const struct zCapability* sail) {
    _cc_cap_t c;
    memset(&c, 0, sizeof(c));
    c._cr_cursor = sail->zaddress;
    set_top_base_from_sail(sail, &c);
    c.cr_tag = sail->ztag;
    c.cr_bounds_valid = !sailgen_boundsMalformed(*sail);
    if (!c.cr_bounds_valid) {
        assert(!sail->ztag && "Input cannot be tagged with malformed bounds");
    }
    c.cr_exp = sail->zE;
    c.cr_pesbt = _compress_sailcap_raw(*sail);
    // sail currently always has lvbits == 1
    c.cr_lvbits = 1;
    return c;
}

static struct zCapability _sail_decode(_cc_addr_t pesbt, _cc_addr_t cursor, bool tag) {
    sail_cap_bits sail_all_bits;
    pesbt_and_addr_to_sail_cap_bits(&sail_all_bits, pesbt, cursor);
    struct zCapability sail_result = sailgen_bitsToCap(tag, sail_all_bits);
    KILL(sail_cap_bits)(&sail_all_bits);
    return sail_result;
}

/* Exported API */
_cc_bounds_bits _cc_sail(extract_bounds_bits)(_cc_addr_t pesbt) {
    _cc_bounds_bits result;
    struct zCapability sail_result = _sail_decode(pesbt, 0, false);
    result.E = sail_result.zE;
    result.B = sail_result.zB;
    result.T = sail_result.zT;
    result.IE = sail_result.zinternal_exponent;
    return result;
}
_cc_addr_t _cc_sail(representable_mask)(_cc_addr_t len) { return sailgen_getRepresentableAlignmentMask(len); }
_cc_addr_t _cc_sail(representable_length)(_cc_addr_t len) { return sailgen_getRepresentableLength(len); }
bool _cc_sail(fast_is_representable)(const _cc_cap_t* cap, _cc_addr_t new_addr) {
    // The RISC-V standard version no longer uses the fast representability check.
    return _cc_sail(precise_is_representable)(cap, new_addr);
}

_cc_cap_t _cc_sail(decode_mem)(_cc_addr_t pesbt, _cc_addr_t cursor, bool tag) {
    struct zCapability sail_result = _sail_decode(pesbt, cursor, tag);
    return sail_cap_to_cap_t(&sail_result);
}

_cc_addr_t _cc_sail(compress_mem)(const _cc_cap_t* csp) { return _compress_sailcap_raw(cap_t_to_sail_cap(csp)); }

// No difference between in-register and in-memory format.
_cc_cap_t _cc_sail(decode_raw)(_cc_addr_t pesbt, _cc_addr_t cursor, bool tag) {
    return _cc_sail(decode_mem)(pesbt, cursor, tag);
}
_cc_addr_t _cc_sail(compress_raw)(const _cc_cap_t* csp) { return _cc_sail(compress_mem)(csp); }
