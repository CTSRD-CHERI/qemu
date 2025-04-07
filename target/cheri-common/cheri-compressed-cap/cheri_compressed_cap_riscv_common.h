// SPDX-License-Identifier: BSD-2-Clause

static inline _cc_addr_t _cc_N(get_reserved)(const _cc_cap_t* cap) {
    _cc_addr_t reserved = cap->cr_pesbt & (_CC_N(FIELD_RESERVED0_MASK64) | _CC_N(FIELD_RESERVED1_MASK64));
    if (cap->cr_lvbits == 0) {
        reserved |= cap->cr_pesbt & _CC_N(FIELD_LEVEL_MASK64);
    }
    return reserved;
}
static inline uint32_t _cc_N(get_level)(const _cc_cap_t* cap) {
    // With lvbits==0 we always report global.
    return cap->cr_lvbits == 0 ? 1 : _CC_EXTRACT_FIELD(cap->cr_pesbt, LEVEL);
}
static inline void _cc_N(update_level)(_cc_cap_t* cap, uint8_t level) {
    _cc_api_requirement(level <= _CC_N(MAX_LEVEL_VALUE), "invalid level");
    if (cap->cr_lvbits == 0) {
        _cc_api_requirement(level == 1, "cannot change level when levels are reserved");
        return;
    }
    cap->cr_pesbt = _CC_DEPOSIT_FIELD(cap->cr_pesbt, level, LEVEL);
}

static inline bool _cc_N(bounds_malformed)(_cc_bounds_bits bounds) {
    // The spec defines this check as checking for E < 0, but since we store it as an unsigned number, we compare it to
    // the maximum exponent instead.
    bool malformedLSB = bounds.E > _CC_MAX_EXPONENT;
#if _CC_N(USES_LEN_MSB) != 0
    // For MXLEN==32, we also report invalid bounds for IE && E == 0
    malformedLSB = malformedLSB || (bounds.E == 0);
#endif
    bool malformedMSB = (bounds.E == _CC_MAX_EXPONENT && bounds.B != 0) ||
                        (bounds.E == _CC_MAX_EXPONENT - 1 && (bounds.B & (1u << (_CC_MANTISSA_WIDTH - 1))) != 0);
    return bounds.IE && (malformedLSB || malformedMSB);
}

static inline bool _cc_N(compute_base_top_special_cases)(_cc_bounds_bits bounds, _cc_addr_t* base_out,
                                                         _cc_length_t* top_out, bool* valid) {
    if (_cc_N(bounds_malformed)(bounds)) {
        *base_out = 0;
        *top_out = 0;
        *valid = false;
        return true;
    }
    return false;
}

// Check that there is no XOR mask for this encoding format (i.e. NULL encodes to all-zeroes in memory).
_CC_STATIC_ASSERT_SAME(_CC_N(MEM_XOR_MASK), UINT64_C(0));
enum { _CC_N(PERMS_RESERVED_ONES) = _CC_BITMASK64_RANGE(6 + _CC_N(FIELD_SDP_SIZE), 15) | _CC_BITMASK64_RANGE(19, 23) };
