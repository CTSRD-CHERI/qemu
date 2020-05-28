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
#include "sail.h"
#include "sail_failure.h"
#include "sail_wrapper.h"

#include SAIL_COMPRESSION_GENERATED_C_FILE
#undef CC_BITS
#define CC_BITS SAIL_WRAPPER_CC_BITS

__attribute__((constructor, used)) static void sail_startup(void) { model_init(); }
__attribute__((destructor, used)) static void sail_cleanup(void) { model_fini(); }

static inline void sail_dump_cap(const char* msg, struct zCapability cap) {
    sail_string zcap_str;
    CREATE(sail_string)(&zcap_str);
    sailgen_capToString(&zcap_str, cap);
    fprintf(stderr, "%s: %s\n", msg, zcap_str);
    KILL(sail_string)(&zcap_str);
}

static inline uint64_t extract_bits(lbits op, uint64_t start, uint64_t len) {
    sail_int start_sail;
    CREATE_OF(sail_int, mach_int)(&start_sail, start);
    sail_int len_sail;
    CREATE_OF(sail_int, mach_int)(&len_sail, len);
    lbits slice_bits;
    CREATE(lbits)(&slice_bits);
    slice(&slice_bits, op, start_sail, len_sail);
    KILL(sail_int)(&start_sail);
    KILL(sail_int)(&len_sail);
    uint64_t result = CONVERT_OF(fbits, lbits)(slice_bits, true);
    KILL(lbits)(&slice_bits);
    return result;
}

static inline void set_top_base_from_sail(const struct zCapability* sail, _cc_cap_t* c);
static inline uint64_t extract_sail_cap_bits(sail_cap_bits* bits, uint64_t start, uint64_t len);

static _cc_addr_t _compress_sailcap_raw(struct zCapability sailcap) {
    sail_cap_bits sailbits;
    CREATE(sail_cap_bits)(&sailbits);
#if SAIL_WRAPPER_CC_BITS == 128
    sailgen_capToBits(&sailbits, sailcap);
#else
    sailbits = sailgen_capToBits(sailcap);
#endif
    _cc_addr_t result = extract_sail_cap_bits(&sailbits, _CC_ADDR_WIDTH, _CC_ADDR_WIDTH);
    KILL(sail_cap_bits)(&sailbits);
    return result;
}

static void sail_cap_to_cap_t(const struct zCapability* sail, _cc_cap_t* c) {
    memset(c, 0, sizeof(*c));
    c->_cr_cursor = sail->zaddress;
    set_top_base_from_sail(sail, c);
    c->cr_perms = sailgen_getCapHardPerms(*sail);
    c->cr_uperms = sail->zuperms;
    c->cr_otype = sail->zotype;
    c->cr_flags = sailgen_getCapFlags(*sail);
    c->cr_reserved = sail->zreserved;
    c->cr_tag = sail->ztag;
    // extract cc128 EBT field:
    // TODO: avoid roundtrip via sailgen_capToBits?
    uint64_t sail_pesbt = _compress_sailcap_raw(*sail);
    c->cr_ebt = (uint32_t)_CC_EXTRACT_FIELD(sail_pesbt, EBT);
}

static void sail_decode_common_mem(_cc_addr_t mem_pesbt, _cc_addr_t mem_cursor, bool tag, _cc_cap_t* cdp) {
    sail_cap_bits sail_all_bits;
    pesbt_and_addr_to_sail_cap_bits(&sail_all_bits, mem_pesbt, mem_cursor);
    struct zCapability sail_result = sailgen_memBitsToCapability(tag, sail_all_bits);
    KILL(sail_cap_bits)(&sail_all_bits);
    // sail_dump_cap("sail_result", sail_result);
    sail_cap_to_cap_t(&sail_result, cdp);
}

static struct zCapability _sail_decode_common_raw_impl(_cc_addr_t pesbt, _cc_addr_t cursor, bool tag) {
    sail_cap_bits sail_all_bits;
    pesbt_and_addr_to_sail_cap_bits(&sail_all_bits, pesbt, cursor);
    struct zCapability sail_result = sailgen_capBitsToCapability(tag, sail_all_bits);
    KILL(sail_cap_bits)(&sail_all_bits);
    return sail_result;
}

static void sail_decode_common_raw(_cc_addr_t pesbt, _cc_addr_t cursor, bool tag, _cc_cap_t* cdp) {
    struct zCapability sail_result = _sail_decode_common_raw_impl(pesbt, cursor, tag);
    sail_cap_to_cap_t(&sail_result, cdp);
}

static _cc_bounds_bits sail_extract_bounds_bits_common(_cc_addr_t pesbt) {
    _cc_bounds_bits result;
    struct zCapability sail_result = _sail_decode_common_raw_impl(pesbt, 0, false);
    result.E = sail_result.zE;
    result.B = sail_result.zB;
    result.T = sail_result.zT;
    result.IE = sail_result.zinternal_e;
    return result;
}

static struct zCapability cap_t_to_sail_cap(const _cc_cap_t* c) {
    struct zCapability result;
    result = znull_cap;
    result = sailgen_setCapPerms(result, c->cr_perms);
    result.ztag = c->cr_tag;
    result.zreserved = c->cr_reserved;
    result.zaddress = c->_cr_cursor;
    result.zuperms = c->cr_uperms;
    result.zotype = c->cr_otype;
    result.zflag_cap_mode = c->cr_flags;
    result.zsealed = result.zotype != (uint64_t)zotype_unsealed;

    // Extract E,B,T,IE from the cr_ebt field:
    _cc_addr_t fake_pesbt = _CC_ENCODE_FIELD(c->cr_ebt, EBT);
    _cc_bounds_bits c_bounds = _cc_N(extract_bounds_bits)(fake_pesbt);
    result.zinternal_e = c_bounds.IE;
    result.zE = c_bounds.E;
    result.zT = c_bounds.T;
    result.zB = c_bounds.B;
    return result;
}

static _cc_addr_t sail_compress_common_raw(const _cc_cap_t* csp) {
    return _compress_sailcap_raw(cap_t_to_sail_cap(csp));
}

static _cc_addr_t sail_compress_common_mem(const _cc_cap_t* csp) {
    struct zCapability sailcap = cap_t_to_sail_cap(csp);
    sail_cap_bits sailbits;
    CREATE(sail_cap_bits)(&sailbits);
#if SAIL_WRAPPER_CC_BITS == 128
    sailgen_capToMemBits(&sailbits, sailcap);
#else
    sailbits = sailgen_capToMemBits(sailcap);
#endif
    _cc_addr_t result = extract_sail_cap_bits(&sailbits, _CC_ADDR_WIDTH, _CC_ADDR_WIDTH);
    return result;
}

_cc_addr_t _CC_CONCAT(sail_null_pesbt_, SAIL_WRAPPER_CC_BITS)(void) {
    // NULL CAP BITS:
    _cc_addr_t null_pesbt = _compress_sailcap_raw(znull_cap);
    sail_dump_cap("null cap", znull_cap);
    fprintf(stderr, "Sail%d null pesbt 0x%jx\n", SAIL_WRAPPER_CC_BITS, (uintmax_t)null_pesbt);
    return null_pesbt;
}

bool _CC_CONCAT(sail_setbounds_, SAIL_WRAPPER_CC_BITS)(_cc_cap_t* cap, _cc_addr_t req_base, _cc_length_t req_top) {
    struct zCapability sailcap = cap_t_to_sail_cap(cap);
    sail_cap_bits sailtop;
    CREATE(sail_cap_bits)(&sailtop);
    cc_length_t_to_sail_cap_bits(&sailtop, req_top);
    __typeof__(sailgen_setCapBounds(sailcap, req_base, sailtop)) result =
        sailgen_setCapBounds(sailcap, req_base, sailtop);
    KILL(sail_cap_bits)(&sailtop);
    bool exact = result.ztup0;
    sail_cap_to_cap_t(&result.ztup1, cap);
    return exact;
}
