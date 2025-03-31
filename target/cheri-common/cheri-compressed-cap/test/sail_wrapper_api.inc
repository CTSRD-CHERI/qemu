/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2020-2024 Alex Richardson
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

#define _sailwrapper_cc_N(name) _CC_CONCAT(_CC_CONCAT(_CC_CONCAT(cc, SAIL_WRAPPER_FORMAT_LOWER), _), name)
#define _sailwrapper_API(name) _CC_CONCAT(sail_, _CC_CONCAT(name, _CC_CONCAT(_, SAIL_WRAPPER_FORMAT_LOWER)))
#define _sail_cap_t _sailwrapper_cc_N(cap_t)
#define _sail_addr_t _sailwrapper_cc_N(addr_t)

#ifdef __cplusplus
extern "C" {
#endif

_sail_cap_t _sailwrapper_API(decode_mem)(_sail_addr_t pesbt, _sail_addr_t cursor, bool tag);
_sail_cap_t _sailwrapper_API(decode_raw)(_sail_addr_t pesbt, _sail_addr_t cursor, bool tag);
struct _sailwrapper_cc_N(bounds_bits) _sailwrapper_API(extract_bounds_bits)(_sail_addr_t pesbt);
_sail_addr_t _sailwrapper_API(compress_raw)(const _sail_cap_t* csp);
_sail_addr_t _sailwrapper_API(compress_mem)(const _sail_cap_t* csp);
bool _sailwrapper_API(fast_is_representable)(const _sail_cap_t* cap, _sail_addr_t new_addr);
bool _sailwrapper_API(precise_is_representable)(const _sail_cap_t* cap, _sail_addr_t new_addr);
_sail_addr_t _sailwrapper_API(null_pesbt)(void);
_sail_addr_t _sailwrapper_API(reset_pesbt)(void);
_sail_addr_t _sailwrapper_API(representable_mask)(_sail_addr_t len);
_sail_addr_t _sailwrapper_API(representable_length)(_sail_addr_t len);
bool _sailwrapper_API(setbounds)(_sail_cap_t* cap, _sailwrapper_cc_N(length_t) req_len);
_sail_cap_t _sailwrapper_API(reset_capability)(void);

#ifdef __cplusplus
}

class _CC_CONCAT(TestAPI, SAIL_WRAPPER_FORMAT_LOWER) : public _CC_CONCAT(CompressedCap, SAIL_WRAPPER_FORMAT_LOWER) {
public:
    static inline cap_t sail_decode_mem(addr_t pesbt, addr_t cursor, bool tag) {
        return _sailwrapper_API(decode_mem)(pesbt, cursor, tag);
    }
    static inline cap_t sail_decode_raw(addr_t pesbt, addr_t cursor, bool tag) {
        return _sailwrapper_API(decode_raw)(pesbt, cursor, tag);
    }
    static inline bounds_bits sail_extract_bounds_bits(addr_t pesbt) {
        return _sailwrapper_API(extract_bounds_bits)(pesbt);
    }
    static inline addr_t sail_compress_raw(const cap_t& csp) { return _sailwrapper_API(compress_raw)(&csp); }
    static inline addr_t sail_compress_mem(const cap_t& csp) { return _sailwrapper_API(compress_mem)(&csp); }
    static inline bool sail_fast_is_representable(const cap_t& cap, addr_t new_addr) {
        return _sailwrapper_API(fast_is_representable)(&cap, new_addr);
    }
    static inline bool sail_precise_is_representable(const cap_t& cap, addr_t new_addr) {
        return _sailwrapper_API(precise_is_representable)(&cap, new_addr);
    }
    static inline addr_t sail_null_pesbt() { return _sailwrapper_API(null_pesbt)(); }
    static inline addr_t sail_reset_pesbt() { return _sailwrapper_API(reset_pesbt)(); }
    static inline addr_t sail_representable_mask(addr_t len) { return _sailwrapper_API(representable_mask)(len); }
    static inline addr_t sail_representable_length(addr_t len) { return _sailwrapper_API(representable_length)(len); }
    static inline bool sail_setbounds(cap_t* cap, length_t req_len) {
        return _sailwrapper_API(setbounds)(cap, req_len);
    }
    static inline cap_t sail_reset_capability() { return _sailwrapper_API(reset_capability)(); }
};

#endif

#undef _sail_addr_t
#undef _sail_cap_t
#undef _sailwrapper_API
#undef _sailwrapper_cc_N
