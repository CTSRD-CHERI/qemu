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

#include "../cheri_compressed_cap.h"

#ifdef __cplusplus
extern "C" {
#endif

cc64_cap_t sail_decode_64_mem(cc64_addr_t pesbt, cc64_addr_t cursor, bool tag);
cc64_cap_t sail_decode_64_raw(cc64_addr_t pesbt, cc64_addr_t cursor, bool tag);
struct cc64_bounds_bits sail_extract_bounds_bits_64(uint32_t pesbt);
cc64_addr_t sail_compress_64_raw(const cc64_cap_t* csp);
cc64_addr_t sail_compress_64_mem(const cc64_cap_t* csp);
bool sail_fast_is_representable_64(const cc64_cap_t* cap, cc64_addr_t new_addr);
bool sail_precise_is_representable_64(const cc64_cap_t* cap, cc64_addr_t new_addr);
cc64_addr_t sail_null_pesbt_64(void);
cc64_addr_t sail_representable_mask_64(cc64_addr_t len);
cc64_addr_t sail_representable_length_64(cc64_addr_t len);
bool sail_setbounds_64(cc64_cap_t* cap, cc64_length_t req_len);
cc64_cap_t sail_reset_capability_64(void);

cc128_cap_t sail_decode_128_mem(cc128_addr_t pesbt, cc128_addr_t cursor, bool tag);
cc128_cap_t sail_decode_128_raw(cc128_addr_t pesbt, cc128_addr_t cursor, bool tag);
struct cc128_bounds_bits sail_extract_bounds_bits_128(cc128_addr_t pesbt);
cc128_addr_t sail_compress_128_raw(const cc128_cap_t* csp);
cc128_addr_t sail_compress_128_mem(const cc128_cap_t* csp);
bool sail_fast_is_representable_128(const cc128_cap_t* cap, cc128_addr_t new_addr);
bool sail_precise_is_representable_128(const cc128_cap_t* cap, cc128_addr_t new_addr);
cc128_addr_t sail_null_pesbt_128(void);
cc128_addr_t sail_representable_mask_128(cc128_addr_t len);
cc128_addr_t sail_representable_length_128(cc128_addr_t len);
bool sail_setbounds_128(cc128_cap_t* cap, cc128_length_t req_len);
cc128_cap_t sail_reset_capability_128(void);

cc128m_cap_t sail_decode_128m_mem(cc128m_addr_t pesbt, cc128m_addr_t cursor, bool tag);
cc128m_cap_t sail_decode_128m_raw(cc128m_addr_t pesbt, cc128m_addr_t cursor, bool tag);
struct cc128m_bounds_bits sail_extract_bounds_bits_128m(cc128m_addr_t pesbt);
cc128m_addr_t sail_compress_128m_raw(const cc128m_cap_t* csp);
cc128m_addr_t sail_compress_128m_mem(const cc128m_cap_t* csp);
bool sail_fast_is_representable_128m(const cc128m_cap_t* cap, cc128m_addr_t new_addr);
bool sail_precise_is_representable_128m(const cc128m_cap_t* cap, cc128m_addr_t new_addr);
cc128m_addr_t sail_null_pesbt_128m(void);
cc128m_addr_t sail_representable_mask_128m(cc128m_addr_t len);
cc128m_addr_t sail_representable_length_128m(cc128m_addr_t len);
bool sail_setbounds_128m(cc128m_cap_t* cap, cc128m_length_t req_len);
cc128m_cap_t sail_reset_capability_128m(void);

#define _cc_sail_decode_mem _CC_CONCAT(_CC_CONCAT(sail_decode_, CC_FORMAT_LOWER), _mem)
#define _cc_sail_decode_raw _CC_CONCAT(_CC_CONCAT(sail_decode_, CC_FORMAT_LOWER), _raw)
#define _cc_sail_compress_raw _CC_CONCAT(_CC_CONCAT(sail_compress_, CC_FORMAT_LOWER), _raw)
#define _cc_sail_compress_mem _CC_CONCAT(_CC_CONCAT(sail_compress_, CC_FORMAT_LOWER), _mem)
#define _cc_sail(name) _CC_CONCAT(sail_, _CC_CONCAT(name, _CC_CONCAT(_, CC_FORMAT_LOWER)))

#ifdef __cplusplus
}

class TestAPI64 : public CompressedCap64 {
public:
    static inline cap_t sail_decode_mem(addr_t pesbt, addr_t cursor, bool tag) {
        return sail_decode_64_mem(pesbt, cursor, tag);
    }
    static inline cap_t sail_decode_raw(addr_t pesbt, addr_t cursor, bool tag) {
        return sail_decode_64_raw(pesbt, cursor, tag);
    }
    static inline bounds_bits sail_extract_bounds_bits(addr_t pesbt) { return sail_extract_bounds_bits_64(pesbt); }
    static inline addr_t sail_compress_raw(const cap_t& csp) { return sail_compress_64_raw(&csp); }
    static inline addr_t sail_compress_mem(const cap_t& csp) { return sail_compress_64_mem(&csp); }
    static inline bool sail_fast_is_representable(const cap_t& cap, addr_t new_addr) {
        return sail_fast_is_representable_64(&cap, new_addr);
    }
    static inline bool sail_precise_is_representable(const cap_t& cap, addr_t new_addr) {
        return sail_precise_is_representable_64(&cap, new_addr);
    }
    static inline addr_t sail_null_pesbt() { return sail_null_pesbt_64(); }
    static inline addr_t sail_representable_mask(addr_t len) { return sail_representable_mask_64(len); }
    static inline addr_t sail_representable_length(addr_t len) { return sail_representable_length_64(len); }
    static inline bool sail_setbounds(cap_t* cap, length_t req_len) { return sail_setbounds_64(cap, req_len); }
    static inline cap_t sail_reset_capability() { return sail_reset_capability_64(); }
};

class TestAPI128 : public CompressedCap128 {
public:
    static inline cap_t sail_decode_mem(addr_t pesbt, addr_t cursor, bool tag) {
        return sail_decode_128_mem(pesbt, cursor, tag);
    }
    static inline cap_t sail_decode_raw(addr_t pesbt, addr_t cursor, bool tag) {
        return sail_decode_128_raw(pesbt, cursor, tag);
    }
    static inline bounds_bits sail_extract_bounds_bits(addr_t pesbt) { return sail_extract_bounds_bits_128(pesbt); }
    static inline addr_t sail_compress_raw(const cap_t& csp) { return sail_compress_128_raw(&csp); }
    static inline addr_t sail_compress_mem(const cap_t& csp) { return sail_compress_128_mem(&csp); }
    static inline bool sail_fast_is_representable(const cap_t& cap, addr_t new_addr) {
        return sail_fast_is_representable_128(&cap, new_addr);
    }
    static inline bool sail_precise_is_representable(const cap_t& cap, addr_t new_addr) {
        return sail_precise_is_representable_128(&cap, new_addr);
    }
    static inline addr_t sail_null_pesbt() { return sail_null_pesbt_128(); }
    static inline addr_t sail_representable_mask(addr_t len) { return sail_representable_mask_128(len); }
    static inline addr_t sail_representable_length(addr_t len) { return sail_representable_length_128(len); }
    static inline bool sail_setbounds(cap_t* cap, length_t req_len) { return sail_setbounds_128(cap, req_len); }
    static inline cap_t sail_reset_capability() { return sail_reset_capability_128(); }
};

class TestAPI128m : public CompressedCap128m {
public:
    static inline cap_t sail_decode_mem(addr_t pesbt, addr_t cursor, bool tag) {
        return sail_decode_128m_mem(pesbt, cursor, tag);
    }
    static inline cap_t sail_decode_raw(addr_t pesbt, addr_t cursor, bool tag) {
        return sail_decode_128m_raw(pesbt, cursor, tag);
    }
    static inline bounds_bits sail_extract_bounds_bits(addr_t pesbt) { return sail_extract_bounds_bits_128m(pesbt); }
    static inline addr_t sail_compress_raw(const cap_t& csp) { return sail_compress_128m_raw(&csp); }
    static inline addr_t sail_compress_mem(const cap_t& csp) { return sail_compress_128m_mem(&csp); }
    static inline bool sail_fast_is_representable(const cap_t& cap, addr_t new_addr) {
        return sail_fast_is_representable_128m(&cap, new_addr);
    }
    static inline bool sail_precise_is_representable(const cap_t& cap, addr_t new_addr) {
        return sail_precise_is_representable_128m(&cap, new_addr);
    }
    static inline addr_t sail_null_pesbt() { return sail_null_pesbt_128m(); }
    static inline addr_t sail_representable_mask(addr_t len) { return sail_representable_mask_128m(len); }
    static inline addr_t sail_representable_length(addr_t len) { return sail_representable_length_128m(len); }
    static inline bool sail_setbounds(cap_t* cap, addr_t req_len) { return sail_setbounds_128m(cap, req_len); }
    static inline cap_t sail_reset_capability() { return sail_reset_capability_128m(); }
};

#define TestAPICC _CC_CONCAT(TestAPI, CC_FORMAT_LOWER)
#endif
