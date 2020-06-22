/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2018 Lawrence Esswood
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

#include <stdbool.h>

#define _cc_length_t _cc_N(length_t)
#define _cc_offset_t _cc_N(offset_t)
#define _cc_addr_t _cc_N(addr_t)

#define _CC_MANTISSA_WIDTH _CC_N(MANTISSA_WIDTH)
#define _CC_MAX_EXPONENT _CC_N(MAX_EXPONENT)
#define _CC_BOT_INTERNAL_EXP_WIDTH _CC_N(FIELD_EXP_NONZERO_BOTTOM_SIZE)
#define _CC_EXP_LOW_WIDTH _CC_N(FIELD_EXPONENT_LOW_PART_SIZE)
#define _CC_EXP_HIGH_WIDTH _CC_N(FIELD_EXPONENT_HIGH_PART_SIZE)
#define _CC_ADDR_WIDTH _CC_N(ADDR_WIDTH)
#define _CC_LEN_WIDTH _CC_N(LEN_WIDTH)
#define _CC_MAX_ADDR _CC_N(MAX_ADDR)
#define _CC_MAX_TOP _CC_N(MAX_TOP)

// CHeck that the sizes of the individual fields match up
_CC_STATIC_ASSERT_SAME(_CC_N(FIELD_EBT_SIZE) + _CC_N(FIELD_OTYPE_SIZE) + _CC_N(FIELD_FLAGS_SIZE) +
                           _CC_N(FIELD_RESERVED_SIZE) + _CC_N(FIELD_HWPERMS_SIZE) + _CC_N(FIELD_UPERMS_SIZE),
                       _CC_ADDR_WIDTH);
_CC_STATIC_ASSERT_SAME(_CC_N(FIELD_INTERNAL_EXPONENT_SIZE) + _CC_N(FIELD_EXP_ZERO_TOP_SIZE) +
                           _CC_N(FIELD_EXP_ZERO_BOTTOM_SIZE),
                       _CC_N(FIELD_EBT_SIZE));
_CC_STATIC_ASSERT_SAME(_CC_N(FIELD_INTERNAL_EXPONENT_SIZE) + _CC_N(FIELD_TOP_ENCODED_SIZE) +
                           _CC_N(FIELD_BOTTOM_ENCODED_SIZE),
                       _CC_N(FIELD_EBT_SIZE));
_CC_STATIC_ASSERT_SAME(_CC_N(FIELD_INTERNAL_EXPONENT_SIZE) + _CC_N(FIELD_EXP_NONZERO_TOP_SIZE) +
                           _CC_N(FIELD_EXP_NONZERO_BOTTOM_SIZE) + _CC_N(FIELD_EXPONENT_HIGH_PART_SIZE) +
                           _CC_N(FIELD_EXPONENT_LOW_PART_SIZE),
                       _CC_N(FIELD_EBT_SIZE));

struct _cc_N(cap) {
    /* offset = cursor - base */
    _cc_addr_t _cr_cursor; /* Capability cursor */
    _cc_addr_t cr_base;    /* Capability base addr */
    _cc_length_t _cr_top;  /* Capability top */
    uint32_t cr_perms;     /* Permissions */
    uint32_t cr_uperms;    /* User Permissions */
    uint32_t cr_otype;     /* Object Type, 24/16 bits */
    /* Original EBT from the decompressed capability. If you modify
     * cursor/base/top, you must be sure to either recalculate this field to
     * match or that the result is still representable, since this will be the
     * EBT written out to memory */
    uint32_t cr_ebt;     /* Exponent/Base/Top */
    uint8_t cr_flags;    /* Flags */
    uint8_t cr_reserved; /* Remaining hardware-reserved bits to preserve */
    uint8_t cr_tag;      /* Tag */
#ifdef __cplusplus
    inline _cc_addr_t base() const { return cr_base; }
    inline _cc_addr_t address() const { return _cr_cursor; }
    inline _cc_offset_t offset() const { return (_cc_offset_t)_cr_cursor - (_cc_offset_t)cr_base; }
    inline _cc_length_t top() const { return _cr_top; }
    inline _cc_addr_t top64() const {
        const _cc_length_t t = top();
        return t > _CC_MAX_ADDR ? _CC_MAX_ADDR : (_cc_addr_t)t;
    }
    inline _cc_length_t length() const { return _cr_top - cr_base; }
    inline _cc_addr_t length64() const {
        const _cc_length_t l = length();
        return l > _CC_MAX_ADDR ? _CC_MAX_ADDR : (_cc_addr_t)l;
    }
    inline uint32_t software_permissions() const { return cr_uperms; }
    inline uint32_t permissions() const { return cr_perms; }
    inline uint32_t type() const { return cr_otype; }
    inline bool is_sealed() const { return cr_otype != _CC_N(OTYPE_UNSEALED); }
    inline bool operator==(const _cc_N(cap) & other) const {
        return _cr_cursor == other._cr_cursor && cr_base == other.cr_base && _cr_top == other._cr_top &&
               cr_perms == other.cr_perms && cr_uperms == other.cr_uperms && cr_otype == other.cr_otype &&
               cr_ebt == other.cr_ebt && cr_flags == other.cr_flags && cr_reserved == other.cr_reserved &&
               cr_tag == other.cr_tag;
    }
#endif
};
typedef struct _cc_N(cap) _cc_N(cap_t);
#define _cc_cap_t _cc_N(cap_t)

/* Returns the index of the most significant bit set in x */
static inline uint32_t _cc_N(idx_MSNZ)(uint64_t x) {
#if defined(__GNUC__) && !defined(__clang__)
#define CAP_HAVE_BUILTIN_CLZ
#elif defined(__has_builtin)
#if __has_builtin(__builtin_clzll)
#define CAP_HAVE_BUILTIN_CLZ
#endif
#endif

#ifndef CAP_HAVE_BUILTIN_CLZ
/* floor(log2(x)) != floor(log2(y)) */
#warning "__builtin_clzll not supported, using slower path"
#define ld_neq(x, y) (((x) ^ (y)) > ((x) & (y)))
    uint32_t r = ld_neq(x, x & 0x5555555555555555ull) + (ld_neq(x, x & 0x3333333333333333ull) << 1) +
                 (ld_neq(x, x & 0x0f0f0f0f0f0f0f0full) << 2) + (ld_neq(x, x & 0x00ff00ff00ff00ffull) << 3) +
                 (ld_neq(x, x & 0x0000ffff0000ffffull) << 4) + (ld_neq(x, x & 0x00000000ffffffffull) << 5);
#undef ld_neq
#else
    _cc_debug_assert(x != 0);
    uint32_t r = 63u - (uint32_t)__builtin_clzll(x);
#endif
    return r;
}

/*
 * e = idxMSNZ( (rlength + (rlength >> 6)) >> 19 )
 * where (rlength + (rlength >> 6)) needs to be a 65 bit integer
 */
static inline uint32_t _cc_N(compute_e)(_cc_addr_t rlength, uint32_t bwidth) {
    if (rlength < (1u << (bwidth - 1)))
        return 0;

    return (_cc_N(idx_MSNZ)(rlength) - (bwidth - 2));
}

static inline uint32_t _cc_N(get_exponent)(_cc_length_t length) {
    const uint32_t bwidth = _CC_MANTISSA_WIDTH;
    if (length > _CC_MAX_ADDR) {
        return _CC_LEN_WIDTH - (bwidth - 1);
    } else {
        return _cc_N(compute_e)((_cc_addr_t)length, bwidth);
    }
}

static inline uint64_t _cc_N(getbits)(uint64_t src, uint32_t start, uint32_t size) {
    return ((src >> start) & ((UINT64_C(1) << size) - UINT64_C(1)));
}

// truncates `value`, keeping only the _least_ significant `n` bits.
static inline uint64_t _cc_N(truncate64)(uint64_t value, size_t n) { return value & ((UINT64_C(1) << n) - 1); }

// truncates `value`, keeping only the _most_ significant `n` bits.
#define TRUNCATE_LSB_FUNC(type_width)                                                                                  \
    static inline uint64_t _CC_CONCAT(_cc_N(truncateLSB_), type_width)(uint64_t value, size_t n) {                     \
        _CC_STATIC_ASSERT(type_width <= 64, "");                                                                       \
        return value >> (type_width - n);                                                                              \
    }
TRUNCATE_LSB_FUNC(_CC_MANTISSA_WIDTH)
TRUNCATE_LSB_FUNC(32)
TRUNCATE_LSB_FUNC(64)

#define _cc_truncateLSB_generic(type_width) _CC_CONCAT(_cc_N(truncateLSB_), _CC_EXPAND(type_width))
#define _cc_truncateLSB(type_width) _cc_N(_CC_CONCAT(truncateLSB_, type_width))

struct _cc_N(bounds_bits) {
    uint16_t B; // bottom bits (currently 14 bits)
    uint16_t T; // top bits (12 bits plus two implied bits)
    uint8_t E;  // exponent
    bool IE;    // internal exponent flag
};
#define _cc_bounds_bits struct _cc_N(bounds_bits)

/// Extract the bits used for bounds and infer the top two bits of T
static inline _cc_bounds_bits _cc_N(extract_bounds_bits)(_cc_addr_t pesbt) {
    _CC_STATIC_ASSERT(_CC_MANTISSA_WIDTH == _CC_N(BOT_WIDTH), "Wrong bot width?");
    uint32_t BWidth = _CC_MANTISSA_WIDTH;
    uint32_t BMask = (1u << BWidth) - 1;
    uint32_t TMask = BMask >> 2;
    _cc_bounds_bits result;
    _CC_STATIC_ASSERT(sizeof(result.B) * __CHAR_BIT__ >= _CC_MANTISSA_WIDTH, "B field too small");
    _CC_STATIC_ASSERT(sizeof(result.T) * __CHAR_BIT__ >= _CC_MANTISSA_WIDTH, "T field too small");
    _CC_STATIC_ASSERT(sizeof(result.E) * __CHAR_BIT__ >=
                          _CC_N(FIELD_EXPONENT_LOW_PART_SIZE) + _CC_N(FIELD_EXPONENT_HIGH_PART_SIZE),
                      "E field too small");
    result.IE = (bool)(uint32_t)_CC_EXTRACT_FIELD(pesbt, INTERNAL_EXPONENT);
    uint8_t L_msb;
    if (result.IE) {
        result.E = (uint8_t)(_CC_EXTRACT_FIELD(pesbt, EXPONENT_LOW_PART) |
                             (_CC_EXTRACT_FIELD(pesbt, EXPONENT_HIGH_PART) << _CC_N(FIELD_EXPONENT_LOW_PART_SIZE)));
        // Do not offset by 1! We also need to encode E=0 even with IE
        // Also allow nonsense values over 64 - BWidth + 2: this is expected by sail-generated tests
        // E = MIN(64 - BWidth + 2, E);
        result.B = (uint16_t)_CC_EXTRACT_FIELD(pesbt, EXP_NONZERO_BOTTOM) << _CC_N(FIELD_EXPONENT_LOW_PART_SIZE);
        result.T = (uint16_t)_CC_EXTRACT_FIELD(pesbt, EXP_NONZERO_TOP) << _CC_N(FIELD_EXPONENT_HIGH_PART_SIZE);
        L_msb = 1;
    } else {
        result.E = 0;
        L_msb = 0;
        result.B = (uint16_t)_CC_EXTRACT_FIELD(pesbt, EXP_ZERO_BOTTOM);
        result.T = (uint16_t)_CC_EXTRACT_FIELD(pesbt, EXP_ZERO_TOP);
    }
    /*
        Reconstruct top two bits of T given T = B + len and:
        1) the top two bits of B
        2) most significant two bits of length derived from format above
        3) carry out of B[20..0] + len[20..0] that is implied if T[20..0] < B[20..0]
    */
    uint8_t L_carry = result.T < (result.B & TMask) ? 1 : 0;
    uint64_t BTop2 = _cc_N(getbits)(result.B, _CC_MANTISSA_WIDTH - 2, 2);
    uint8_t T_infer = (BTop2 + L_carry + L_msb) & 0x3;
    result.T |= ((uint16_t)T_infer) << (BWidth - 2);
    return result;
}

// Certain bit patterns can result in invalid bounds bits. These values must never be tagged!
static inline bool _cc_N(bounds_bits_valid)(_cc_bounds_bits bounds) {
    // https://github.com/CTSRD-CHERI/sail-cheri-riscv/blob/7a308ef3661e43461c8431c391aaece7fba6e992/src/cheri_properties.sail#L104
    _cc_addr_t Bmsb = _cc_N(getbits)(bounds.B, _CC_MANTISSA_WIDTH - 1, 1);
    _cc_addr_t Bmsb2 = _cc_N(getbits)(bounds.B, _CC_MANTISSA_WIDTH - 2, 2);
    _cc_addr_t Tmsb = _cc_N(getbits)(bounds.T, _CC_MANTISSA_WIDTH - 1, 1);
    if (bounds.E >= _CC_MAX_EXPONENT) {
        return Tmsb == 0 && Bmsb2 == 0;
    } else if (bounds.E == _CC_MAX_EXPONENT - 1) {
        return Bmsb == 0;
    } else {
        return true;
    }
}

static inline void _cc_N(compute_base_top)(_cc_bounds_bits bounds, _cc_addr_t cursor, _cc_addr_t* base_out,
                                           _cc_length_t* top_out) {
    // For the remaining computations we have to clamp E to max_E
    //  let E = min(maxE, unsigned(c.E)) in
    uint8_t E = _CC_MIN(_CC_MAX_EXPONENT, bounds.E);
    /* Extract bits we need to make the top correction and calculate representable limit */
    // let a3 = truncate(a >> (E + mantissa_width - 3), 3) in
    // let B3 = truncateLSB(c.B, 3) in
    // let T3 = truncateLSB(c.T, 3) in
    unsigned a3 = (unsigned)_cc_N(truncate64)(cursor >> (E + _CC_MANTISSA_WIDTH - 3), 3);
    unsigned B3 = (unsigned)_cc_truncateLSB(_CC_MANTISSA_WIDTH)(bounds.B, 3);
    unsigned T3 = (unsigned)_cc_truncateLSB(_CC_MANTISSA_WIDTH)(bounds.T, 3);
    // let R3 = B3 - 0b001 in /* wraps */
    unsigned R3 = (unsigned)_cc_N(truncate64)(B3 - 1, 3); // B3 == 0 ? 7 : B3 - 1;
    /* Do address, base and top lie in the R aligned region above the one containing R? */
    // let aHi = if a3 <_u R3 then 1 else 0 in
    // let bHi = if B3 <_u R3 then 1 else 0 in
    // let tHi = if T3 <_u R3 then 1 else 0 in
    int aHi = a3 < R3 ? 1 : 0;
    int bHi = B3 < R3 ? 1 : 0;
    int tHi = T3 < R3 ? 1 : 0;

    /* Compute region corrections for top and base relative to a */
    // let correction_base = bHi - aHi in
    // let correction_top  = tHi - aHi in
    int correction_base = bHi - aHi;
    int correction_top = tHi - aHi;
    // Note: shifting by 64 is UB and causes wrong results -> clamp the shift value!
    const unsigned a_top_shift = E + _CC_MANTISSA_WIDTH;
    // let a_top = (a >> (E + mantissa_width)) in
    _cc_addr_t a_top = a_top_shift >= _CC_ADDR_WIDTH ? 0 : cursor >> a_top_shift;

    // base : CapLenBits = truncate((a_top + correction_base) @ c.B @ zeros(E), cap_len_width);
    _cc_length_t base = (_cc_addr_t)((int64_t)a_top + correction_base);
    base <<= _CC_MANTISSA_WIDTH;
    base |= bounds.B;
    base <<= E;
    base &= ((_cc_length_t)1 << _CC_LEN_WIDTH) - 1;
    _cc_debug_assert((_cc_addr_t)(base >> _CC_ADDR_WIDTH) <= 1); // max 65/33 bits
    // top  : truncate((a_top + correction_top)  @ c.T @ zeros(E), cap_len_width);
    _cc_length_t top = (_cc_addr_t)((int64_t)a_top + correction_top);
    top <<= _CC_MANTISSA_WIDTH;
    top |= bounds.T;
    top <<= E;
    top &= ((_cc_length_t)1 << _CC_LEN_WIDTH) - 1;
    _cc_debug_assert((_cc_addr_t)(top >> _CC_ADDR_WIDTH) <= 1); // max 65 bits

    /* If the base and top are more than an address space away from each other,
       invert the MSB of top.  This corrects for errors that happen when the
       representable space wraps the address space. */
    //  let base2 : bits(2) = 0b0 @ [base[cap_addr_width - 1]];
    // Note: we ignore the top bit of base here. If we don't we can get cases
    // where setbounds/incoffset/etc. break monotonicity.
    unsigned base2 = _cc_N(truncate64)(base >> (_CC_ADDR_WIDTH - 1), 1);
    //  let top2  : bits(2) = top[cap_addr_width .. cap_addr_width - 1];
    unsigned top2 = _cc_N(truncate64)(top >> (_CC_ADDR_WIDTH - 1), 2);
    //  if (E < (maxE - 1)) & (unsigned(top2 - base2) > 1) then {
    //      top[cap_addr_width] = ~(top[cap_addr_width]);
    //  };
    if (E < (_CC_MAX_EXPONENT - 1) && (top2 - base2) > 1) {
        top = top ^ ((_cc_length_t)1 << _CC_ADDR_WIDTH);
    }

    _cc_debug_assert((_cc_addr_t)(top >> _CC_ADDR_WIDTH) <= 1); // should be at most 1 bit over
    // Check that base <= top for valid inputs
    if (_cc_N(bounds_bits_valid)(bounds)) {
        // Note: base can be > 2^64 for some (untagged) inputs with E near maxE
        // It can also be > top for some (untagged) inputs.
        _cc_debug_assert((_cc_addr_t)base <= top);
    } else {
        // _cc_debug_assert(!tagged && "Should not create invalid tagged capabilities");
    }
    *base_out = (_cc_addr_t)base; // strip the (invalid) top bit
    *top_out = top;
}

static inline void _cc_N(decompress_raw)(_cc_addr_t pesbt, _cc_addr_t cursor, bool tag, _cc_cap_t* cdp) {
    cdp->cr_tag = tag;
    cdp->_cr_cursor = cursor;
    cdp->cr_perms = (uint32_t)_CC_EXTRACT_FIELD(pesbt, HWPERMS);
    cdp->cr_uperms = (uint32_t)_CC_EXTRACT_FIELD(pesbt, UPERMS);
    cdp->cr_otype = (uint32_t)_CC_EXTRACT_FIELD(pesbt, OTYPE);
    cdp->cr_flags = (uint8_t)_CC_EXTRACT_FIELD(pesbt, FLAGS);
    cdp->cr_reserved = (uint8_t)_CC_EXTRACT_FIELD(pesbt, RESERVED);
    cdp->cr_ebt = (uint32_t)_CC_EXTRACT_FIELD(pesbt, EBT);

    _cc_bounds_bits bounds = _cc_N(extract_bounds_bits)(pesbt);
    _cc_N(compute_base_top)(bounds, cursor, &cdp->cr_base, &cdp->_cr_top);
    if (tag) {
        _cc_debug_assert(cdp->cr_base <= _CC_N(MAX_ADDR));
        _cc_debug_assert(cdp->_cr_top <= _CC_N(MAX_TOP));
        _cc_debug_assert(cdp->cr_base <= cdp->_cr_top);
        _cc_debug_assert(cdp->cr_reserved == 0);
    }
}

/*
 * Decompress a 128-bit capability.
 */
static inline void _cc_N(decompress_mem)(uint64_t pesbt, uint64_t cursor, bool tag, _cc_cap_t* cdp) {
    _cc_N(decompress_raw)(pesbt ^ _CC_N(NULL_XOR_MASK), cursor, tag, cdp);
}

static inline bool _cc_N(is_cap_sealed)(const _cc_cap_t* cp) { return cp->cr_otype != _CC_N(OTYPE_UNSEALED); }

/*
 * Compress a capability to 128 bits.
 */
static inline _cc_addr_t _cc_N(compress_raw)(const _cc_cap_t* csp) {
    _cc_debug_assert(!(csp->cr_tag && csp->cr_reserved) && "Unknown reserved bits set it tagged capability");
    _cc_addr_t pesbt = _CC_ENCODE_FIELD(csp->cr_uperms, UPERMS) | _CC_ENCODE_FIELD(csp->cr_perms, HWPERMS) |
                       _CC_ENCODE_FIELD(csp->cr_otype, OTYPE) | _CC_ENCODE_FIELD(csp->cr_reserved, RESERVED) |
                       _CC_ENCODE_FIELD(csp->cr_flags, FLAGS) | _CC_ENCODE_FIELD(csp->cr_ebt, EBT);
    return pesbt;
}

static inline _cc_addr_t _cc_N(compress_mem)(const _cc_cap_t* csp) {
    return _cc_N(compress_raw)(csp) ^ _CC_N(NULL_XOR_MASK);
}

/*
 * Define the following to do the is_representable() check by simply
 * compressing and decompressing the capability and checking to
 * see if it is the same.
 */
// #define SIMPLE_REPRESENT_CHECK

#ifndef SIMPLE_REPRESENT_CHECK
static inline bool _cc_N(all_ones)(uint64_t offset, uint32_t e, uint32_t bwidth) {
    uint64_t Itop;
    uint32_t shift = e + bwidth;

    if (shift >= 63)
        return false;
    Itop = offset >> shift;
    return Itop == (0xfffffffffffffffful >> shift);
}

static inline bool _cc_N(all_zeroes)(uint64_t offset, uint32_t e, uint32_t bwidth) {
    uint32_t shift = e + bwidth;
    uint64_t Itop;

    if (shift >= 63)
        Itop = 0ul;
    else
        Itop = offset >> shift;
    return Itop == 0ul;
}
#endif /* ! SIMPLE_REPRESENT_CHECK */

static bool _cc_N(fast_is_representable_new_addr)(bool sealed, _cc_addr_t base, _cc_length_t length, _cc_addr_t cursor,
                                                  _cc_addr_t new_cursor);

/// Check that a capability is representable by compressing and recompressing
static bool _cc_N(is_representable_cap_exact)(const _cc_cap_t* cap) {
    _cc_addr_t pesbt = _cc_N(compress_raw)(cap);
    _cc_cap_t decompressed_cap;
    _cc_N(decompress_raw)(pesbt, cap->_cr_cursor, cap->cr_tag, &decompressed_cap);
    // These fields must not change:
    _cc_debug_assert(decompressed_cap._cr_cursor == cap->_cr_cursor);
    _cc_debug_assert(decompressed_cap.cr_perms == cap->cr_perms);
    _cc_debug_assert(decompressed_cap.cr_uperms == cap->cr_uperms);
    _cc_debug_assert(decompressed_cap.cr_otype == cap->cr_otype);
    _cc_debug_assert(decompressed_cap.cr_ebt == cap->cr_ebt);
    _cc_debug_assert(decompressed_cap.cr_flags == cap->cr_flags);
    _cc_debug_assert(decompressed_cap.cr_reserved == cap->cr_reserved);
    // If any of these fields changed then the capability is not representable:
    if (decompressed_cap.cr_base != cap->cr_base || decompressed_cap._cr_top != cap->_cr_top) {
        return false;
    }
    return true;
}

static inline uint32_t _cc_N(compute_ebt)(_cc_addr_t req_base, _cc_length_t req_top, _cc_addr_t* alignment_mask,
                                          bool* exact) {
    _cc_debug_assert(req_base <= req_top && "Cannot invert base and top");
    /*
     * With compressed capabilities we may need to increase the range of
     * memory addresses to be wider than requested so it is
     * representable.
     */
    _cc_length_t req_length65 = req_top - req_base;
    // function setCapBounds(cap, base, top) : (Capability, bits(64), bits(65)) -> (bool, Capability) = {
    //  /* {cap with base=base; length=(bits(64)) length; offset=0} */
    //  let base65 = 0b0 @ base;
    //  let length = top - base65;
    //  /* Find an exponent that will put the most significant bit of length
    //     second from the top as assumed during decoding. We ignore the bottom
    //     MW - 1 bits because those are handled by the ie = 0 format. */
    //  let e = 52 - CountLeadingZeros(length[64..13]);
    uint8_t E = (uint8_t)_cc_N(get_exponent)(req_length65);
    const uint64_t req_length64 = (uint64_t)req_length65;
    // Use internal exponent if e is non-zero or if e is zero but
    // but the implied bit of length is not zero (denormal vs. normal case)
    //  let ie = (e != 0) | length[12];
    //
    const bool InternalExponent = E != 0 || _cc_N(getbits)(req_length64, _CC_BOT_INTERNAL_EXP_WIDTH + 1, 1);
    if (!InternalExponent) {
        //  /* The non-ie e == 0 case is easy. It is exact so just extract relevant bits. */
        //  Bbits = truncate(base, 14);
        //  Tbits = truncate(top, 14);
        //  lostSignificantTop  : bool = false;
        //  lostSignificantBase : bool = false;
        //  incE : bool = false;
        uint32_t ebt_bits = _CC_ENCODE_EBT_FIELD(0, INTERNAL_EXPONENT) | _CC_ENCODE_EBT_FIELD(req_top, EXP_ZERO_TOP) |
                            _CC_ENCODE_EBT_FIELD(req_base, EXP_ZERO_BOTTOM);
        if (alignment_mask)
            *alignment_mask = _CC_MAX_ADDR; // no adjustment to base required
        *exact = true;
        return ebt_bits; /* Exactly representable */
    }
    // Handle IE case:
    //  if ie then {
    //    /* the internal exponent case is trickier */
    //
    //    /* Extract B and T bits (we lose 3 bits of each to store the exponent) */
    //    B_ie = truncate(base >> (e + 3), 11);
    //    T_ie = truncate(top >> (e + 3), 11);
    //
    _cc_addr_t bot_ie = _cc_N(truncate64)(req_base >> (E + _CC_EXP_LOW_WIDTH), _CC_BOT_INTERNAL_EXP_WIDTH);
    if (alignment_mask) {
        *alignment_mask = UINT64_MAX << (E + _CC_EXP_LOW_WIDTH);
    }
    _cc_addr_t top_ie = _cc_N(truncate64)((_cc_addr_t)(req_top >> (E + _CC_EXP_LOW_WIDTH)), _CC_BOT_INTERNAL_EXP_WIDTH);
    //    /* Find out whether we have lost significant bits of base and top using a
    //       mask of bits that we will lose (including 3 extra for exp). */
    //    maskLo : bits(65) = zero_extend(replicate_bits(0b1, e + 3));
    //    z65    : bits(65) = zeros();
    //    lostSignificantBase = (base65 & maskLo) != z65;
    //    lostSignificantTop = (top & maskLo) != z65;
    // TODO: stop using _cc_length_t and just handle bit65 set specially?
    const _cc_length_t maskLo = (((_cc_length_t)1u) << (E + _CC_EXP_LOW_WIDTH)) - 1;
    const _cc_length_t zero65 = 0;
    bool lostSignificantBase = (req_base & maskLo) != zero65;
    bool lostSignificantTop = (req_top & maskLo) != zero65;
    //    if lostSignificantTop then {
    //      /* we must increment T to make sure it is still above top even with lost bits.
    //         It might wrap around but if that makes B<T then decoding will compensate. */
    //      T_ie = T_ie + 1;
    //    };
    if (lostSignificantTop) {
        top_ie = _cc_N(truncate64)(top_ie + 1, _CC_BOT_INTERNAL_EXP_WIDTH);
    }
    //    /* Has the length overflowed? We chose e so that the top two bits of len would be 0b01,
    //       but either because of incrementing T or losing bits of base it might have grown. */
    //    len_ie = T_ie - B_ie;
    //    if len_ie[10] then {
    //      /* length overflow -- increment E by one and then recalculate
    //         T, B etc accordingly */
    //      incE = true;
    //
    //      lostSignificantBase = lostSignificantBase | B_ie[0];
    //      lostSignificantTop  = lostSignificantTop | T_ie[0];
    //
    //      B_ie = truncate(base >> (e + 4), 11);
    //      let incT : range(0,1) = if lostSignificantTop then 1 else 0;
    //      T_ie = truncate(top >> (e + 4), 11) + incT;
    //    };
    const _cc_addr_t len_ie = _cc_N(truncate64)(top_ie - bot_ie, _CC_BOT_INTERNAL_EXP_WIDTH);
    bool incE = false;
    if (_cc_N(getbits)(len_ie, _CC_BOT_INTERNAL_EXP_WIDTH - 1, 1)) {
        incE = true;
        lostSignificantBase = lostSignificantBase || _cc_N(getbits)(bot_ie, 0, 1);
        lostSignificantTop = lostSignificantTop || _cc_N(getbits)(top_ie, 0, 1);
        bot_ie = _cc_N(truncate64)(req_base >> (E + _CC_EXP_LOW_WIDTH + 1), _CC_BOT_INTERNAL_EXP_WIDTH);
        // If we had to adjust bot_ie (shift by one more) also update alignment_mask
        if (alignment_mask) {
            *alignment_mask = UINT64_MAX << (E + _CC_EXP_LOW_WIDTH + 1);
        }
        const bool incT = lostSignificantTop;
        top_ie = _cc_N(truncate64)((_cc_addr_t)(req_top >> (E + _CC_EXP_LOW_WIDTH + 1)), _CC_BOT_INTERNAL_EXP_WIDTH);
        if (incT) {
            top_ie = _cc_N(truncate64)(top_ie + 1, _CC_BOT_INTERNAL_EXP_WIDTH);
        }
    }
    //
    //    Bbits = B_ie @ 0b000;
    //    Tbits = T_ie @ 0b000;
    const _cc_addr_t Bbits = bot_ie << _CC_N(FIELD_EXPONENT_LOW_PART_SIZE);
    const _cc_addr_t Tbits = top_ie << _CC_N(FIELD_EXPONENT_LOW_PART_SIZE);
    const uint8_t newE = E + (incE ? 1 : 0);

    //  };
    //  let exact = not(lostSignificantBase | lostSignificantTop);
    *exact = !lostSignificantBase && !lostSignificantTop;
    // Split E between T and B
    const _cc_addr_t expHighBits =
        _cc_N(getbits)(newE >> _CC_N(FIELD_EXPONENT_LOW_PART_SIZE), 0, _CC_N(FIELD_EXPONENT_HIGH_PART_SIZE));
    const _cc_addr_t expLowBits = _cc_N(getbits)(newE, 0, _CC_N(FIELD_EXPONENT_LOW_PART_SIZE));
    const _cc_addr_t Te = Tbits | expHighBits;
    const _cc_addr_t Be = Bbits | expLowBits;
    return _CC_ENCODE_EBT_FIELD(1, INTERNAL_EXPONENT) | _CC_ENCODE_EBT_FIELD(Te, TOP_ENCODED) |
           _CC_ENCODE_EBT_FIELD(Be, BOTTOM_ENCODED);
}

/*
 * Check to see if a memory region is representable by a compressed
 * capability. It is representable if:
 *
 *   representable = (inRange && inLimits) || (E >= 44)
 *
 * where:
 *
 *   E = compression exponent (see _cc_N(compute_e)() above)
 *
 *   inRange = -s < i < s  where i is the increment (or offset)
 *   (In other words, all the bits of i<63, E+20> are the same.)
 *
 *   inLimits = (i < 0) ? (Imid >= (R - Amid)) && (R != Amid) : (R - Amid - 1)
 *   where Imid = i<E+19, E>, Amid = a<E+19, E>, R = B - 2^12 and a =
 *   base + offset.
 */
static inline bool _cc_N(is_representable_new_addr)(bool sealed, _cc_addr_t base, _cc_length_t length,
                                                    _cc_addr_t cursor, _cc_addr_t new_cursor) {
    _cc_length_t top = (_cc_length_t)base + length;
    // in-bounds capabilities are always representable
    if (__builtin_expect(new_cursor >= base && new_cursor < top, true)) {
        return true;
    }

#if defined(_CC_USE_FAST_REP_CHECK)
    const bool slow_representable_check = false;
#else
    const bool slow_representable_check = true;
#endif

    if (slow_representable_check) {
        /* Simply compress and uncompress to check. */
        _cc_cap_t c;
        memset(&c, 0, sizeof(c));
        c.cr_base = base;
        c._cr_top = top;
        c._cr_cursor = cursor;
        c.cr_otype = sealed ? 42 : _CC_N(OTYPE_UNSEALED); // important to set as compress assumes this is in bounds
        /* Get an EBT */
        bool exact_input = false;
        c.cr_ebt = _cc_N(compute_ebt)(base, top, NULL, &exact_input);
        _cc_debug_assert(exact_input &&
                         "Input capability bounds not representable? They should have been rounded before!");
        /* Check with new cursor */
        c._cr_cursor = new_cursor;
        return _cc_N(is_representable_cap_exact)(&c);
    } else {
        return _cc_N(fast_is_representable_new_addr)(sealed, base, length, cursor, new_cursor);
    }
}

static inline bool _cc_N(is_representable_with_addr)(const _cc_cap_t* cap, _cc_addr_t new_addr) {
    // in-bounds capabilities are always representable
    if (__builtin_expect(new_addr >= cap->cr_base && new_addr < cap->_cr_top, true)) {
        return true;
    }
    const _cc_length_t length = cap->_cr_top - cap->cr_base;
    return _cc_N(is_representable_new_addr)(_cc_N(is_cap_sealed)(cap), cap->cr_base, length, cap->_cr_cursor, new_addr);
}

static bool _cc_N(fast_is_representable_new_addr)(bool sealed, _cc_addr_t base, _cc_length_t length, _cc_addr_t cursor,
                                                  _cc_addr_t new_cursor) {
    (void)sealed;
    uint32_t bwidth = _CC_MANTISSA_WIDTH;
    uint32_t highest_exp = (64 - bwidth + 2);

    _cc_length_t top = base + length;
    // If top is 0xffff... we assume we meant it to be 1 << 64
    if (top == _CC_MAX_TOP && base == 0) {
        return true; // 1 << 65 is always representable
    }
    if (length == 0) {
        return true; // length 0 is always representable
    }

    uint32_t e = _cc_N(get_exponent)(length);

    int64_t b, r, Imid, Amid;
    bool inRange, inLimits;
    int64_t inc = (int64_t)(new_cursor - cursor);

#define MOD_MASK ((UINT64_C(1) << bwidth) - UINT64_C(1))

    /* Check for the boundary cases. */

    b = (int64_t)((base >> e) & MOD_MASK);
    Imid = (int64_t)((uint64_t)(inc >> e) & MOD_MASK);
    Amid = (int64_t)(((cursor) >> e) & MOD_MASK);

    r = (int64_t)(((uint64_t)((b >> (bwidth - 3)) - 1) << (bwidth - 3)) & MOD_MASK);

    /* inRange, test if bits are all the same */
    inRange = _cc_N(all_ones)((uint64_t)inc, e, bwidth) || _cc_N(all_zeroes)((uint64_t)inc, e, bwidth);

    /* inLimits */
    if (inc >= 0) {
        inLimits = ((uint64_t)Imid < (((uint64_t)(r - Amid - 1l)) & MOD_MASK));
    } else {
        inLimits = ((uint64_t)Imid >= (((uint64_t)(r - Amid)) & MOD_MASK)) && (r != Amid);
    }
#undef MOD_MASK

    return ((inRange && inLimits) || (e >= highest_exp));
}

/* @return whether the operation was able to set precise bounds precise or not */
static inline bool _cc_N(setbounds_impl)(_cc_cap_t* cap, _cc_addr_t req_base, _cc_length_t req_top,
                                         _cc_addr_t* alignment_mask) {
    _cc_debug_assert((cap->cr_tag) && "Cannot be used on untagged capabilities");
    _cc_debug_assert((!_cc_N(is_cap_sealed)(cap)) && "Cannot be used on sealed capabilities");
    _cc_debug_assert(req_base <= req_top && "Cannot invert base and top");
    /*
     * With compressed capabilities we may need to increase the range of
     * memory addresses to be wider than requested so it is
     * representable.
     */
    const _cc_addr_t cursor = cap->_cr_cursor;
    _cc_debug_assert(((cap->_cr_top - cap->cr_base) >> _CC_ADDR_WIDTH) <= 1 && "Length must be smaller than 1 << 65");
    _cc_debug_assert((req_top >> _CC_ADDR_WIDTH) <= 1 && "New top must be smaller than 1 << 65");
    _cc_debug_assert(req_base >= cap->cr_base && "Cannot decrease base");
    _cc_debug_assert(req_top <= cap->_cr_top && "Cannot increase top");
    assert((cap->_cr_cursor < cap->_cr_top ||
            (cap->_cr_cursor == cap->_cr_top && req_base == cap->_cr_top && req_base == req_top)) &&
           "Must be used on inbounds caps or request zero-length cap at top");
    assert((cap->_cr_cursor >= cap->cr_base) && "Must be used on inbounds caps");
    _CC_STATIC_ASSERT(_CC_EXP_LOW_WIDTH == 3, "expected 3 bits to be used by");  // expected 3 bits to
    _CC_STATIC_ASSERT(_CC_EXP_HIGH_WIDTH == 3, "expected 3 bits to be used by"); // expected 3 bits to
    bool exact = false;
    uint32_t new_ebt = _cc_N(compute_ebt)(req_base, req_top, alignment_mask, &exact);

    // TODO: find a faster way to compute top and bot:
    const _cc_addr_t pesbt = _CC_ENCODE_FIELD(0, UPERMS) | _CC_ENCODE_FIELD(0, HWPERMS) |
                             _CC_ENCODE_FIELD(_CC_N(OTYPE_UNSEALED), OTYPE) | _CC_ENCODE_FIELD(new_ebt, EBT);
    _cc_cap_t new_cap;
    _cc_N(decompress_raw)(pesbt, cursor, cap->cr_tag, &new_cap);
    _cc_addr_t new_base = new_cap.cr_base;
    _cc_length_t new_top = new_cap._cr_top;
    if (exact) {
        _cc_debug_assert(new_base == req_base && "Should be exact");
        _cc_debug_assert(new_top == req_top && "Should be exact");
    } else {
        _cc_debug_assert((new_base != req_base || new_top != req_top) &&
                         "Was inexact, but neither base nor top different?");
    }

    _cc_debug_assert(new_top >= new_base);
    _cc_debug_assert(!(cap->cr_tag && cap->cr_reserved) && "Unknown reserved bits set in tagged capability");
    cap->cr_base = new_base;
    cap->_cr_cursor = cursor; // ensure that the address is correct
    cap->_cr_top = new_top;
    cap->cr_ebt = new_ebt;
    // TODO: update pesbt?
    //  let newCap = {cap with address=base, E=to_bits(6, if incE then e + 1 else e), B=Bbits, T=Tbits, internal_e=ie};
    //  (exact, newCap)
    return exact;
}

/* @return whether the operation was able to set precise bounds precise or not */
static inline bool _cc_N(setbounds)(_cc_cap_t* cap, _cc_addr_t req_base, _cc_length_t req_top) {
    return _cc_N(setbounds_impl)(cap, req_base, req_top, NULL);
}

/* @return the mask that needs to be applied to base in order to get a precisely representable capability */
static inline _cc_addr_t _cc_N(get_alignment_mask)(_cc_addr_t req_length) {
    if (req_length == 0) {
        // With a lenght of zero we know it is precise so we can just return an
        // all ones mask.
        // This avoids undefined behaviour when counting most significant bit later.
        return _CC_MAX_ADDR;
    }
    // To compute the mask we set bounds on a maximum permissions capability and
    // return the mask that was used to adjust the length
    _cc_cap_t tmpcap;
    memset(&tmpcap, 0, sizeof(tmpcap));
    tmpcap.cr_tag = 1;
    tmpcap.cr_base = 0;
    tmpcap._cr_top = _CC_MAX_TOP;
    tmpcap.cr_otype = _CC_N(OTYPE_UNSEALED);
    _cc_addr_t mask = 0;
    // Ensure that the base always needs rounding down by making it all ones until
    // one bit before the most significant bit in length
    // _cc_addr_t req_base = UINT64_MAX & ~(UINT64_C(1) << _cc_N(idx_MSNZ)(req_length));
    _cc_addr_t req_base = _CC_MAX_ADDR - req_length;
    tmpcap._cr_cursor = req_base;
    _cc_N(setbounds_impl)(&tmpcap, req_base, req_base + req_length, &mask);
    // base should have been rounded down using this mask:
    _cc_debug_assert((req_base & mask) == tmpcap.cr_base);
    return mask;
}

static inline _cc_addr_t _cc_N(get_required_alignment)(_cc_addr_t req_length) {
    // To get the required alignment from the CRAM mask we can just invert
    // the bits and add one to get a power-of-two
    return ~_cc_N(get_alignment_mask)(req_length) + 1;
}

static inline _cc_addr_t _cc_N(get_representable_length)(_cc_addr_t req_length) {
    _cc_addr_t mask = _cc_N(get_alignment_mask)(req_length);
    return (req_length + ~mask) & mask;
}

/// Provide a C++ class with the same function names
/// to simplify writing code that handles both 128 and 64-bit capabilities
#ifdef __cplusplus
class _CC_CONCAT(CompressedCap, CC_BITS) {
public:
    using length_t = _cc_length_t;
    using offset_t = _cc_offset_t;
    using addr_t = _cc_addr_t;
    using cap_t = _cc_cap_t;
    using bounds_bits = _cc_bounds_bits;

    static inline addr_t compress_raw(const cap_t* csp) { return _cc_N(compress_raw)(csp); }
    static inline void decompress_raw(addr_t pesbt, addr_t cursor, bool tag, cap_t* cdp) {
        _cc_N(decompress_raw)(pesbt, cursor, tag, cdp);
    }
    static inline addr_t compress_mem(const cap_t* csp) { return _cc_N(compress_mem)(csp); }
    static inline void decompress_mem(addr_t pesbt, addr_t cursor, bool tag, cap_t* cdp) {
        _cc_N(decompress_mem)(pesbt, cursor, tag, cdp);
    }
    static inline bounds_bits extract_bounds_bits(addr_t pesbt) { return _cc_N(extract_bounds_bits)(pesbt); }
    static inline bool setbounds(cap_t* cap, addr_t req_base, length_t req_top) {
        return _cc_N(setbounds)(cap, req_base, req_top);
    }
    static inline bool is_representable_cap_exact(const cap_t* cap) { return _cc_N(is_representable_cap_exact)(cap); }
};
#endif
