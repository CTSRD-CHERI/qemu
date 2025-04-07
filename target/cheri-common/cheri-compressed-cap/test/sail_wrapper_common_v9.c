/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright 2025 Alex Richardson
 */

// This file contains the shared code for the ISAv9 sail warpper that is not different in the new RISC-V sail model.

static _cc_addr_t _compress_sailcap_raw(struct zCapability sailcap) {
    sail_cap_bits sailbits;
    CREATE(sail_cap_bits)(&sailbits);
#ifdef SAIL_COMPRESSION_INDIRECT_BITS
    sailgen_capToBits(&sailbits, sailcap);
#else
    sailbits = sailgen_capToBits(sailcap);
#endif
    _cc_addr_t result = extract_sail_cap_bits(&sailbits, _CC_ADDR_WIDTH, _CC_ADDR_WIDTH);
    KILL(sail_cap_bits)(&sailbits);
    return result;
}

static _cc_cap_t sail_cap_to_cap_t(const struct zCapability* sail) {
    _cc_cap_t c;
    memset(&c, 0, sizeof(c));
    c._cr_cursor = sail->zaddress;
    set_top_base_from_sail(sail, &c);
    _cc_N(update_perms)(&c, sailgen_getCapHardPerms(*sail));
    _cc_N(update_uperms)(&c, sail->zuperms);
    _cc_N(update_otype)(&c, sail->zotype);
    _cc_N(update_flags)(&c, sailgen_getCapFlags(*sail));
    c.cr_pesbt = _CC_DEPOSIT_FIELD(c.cr_pesbt, sail->zreserved, RESERVED);
    c.cr_tag = sail->ztag;
    c.cr_exp = sail->zE;
    c.cr_bounds_valid = true; // This field is only ever false for Morello.
    // extract cc128 EBT field:
    // TODO: avoid roundtrip via sailgen_capToBits?
    uint64_t sail_pesbt = _compress_sailcap_raw(*sail);
    c.cr_pesbt = sail_pesbt;
    return c;
}

_cc_cap_t _cc_sail(decode_mem)(_cc_addr_t mem_pesbt, _cc_addr_t mem_cursor, bool tag) {
    sail_cap_bits sail_all_bits;
    pesbt_and_addr_to_sail_cap_bits(&sail_all_bits, mem_pesbt, mem_cursor);
    struct zCapability sail_result = sailgen_memBitsToCapability(tag, sail_all_bits);
    KILL(sail_cap_bits)(&sail_all_bits);
    // sail_dump_cap("sail_result", sail_result);
    return sail_cap_to_cap_t(&sail_result);
}

static struct zCapability _sail_decode_common_raw_impl(_cc_addr_t pesbt, _cc_addr_t cursor, bool tag) {
    sail_cap_bits sail_all_bits;
    pesbt_and_addr_to_sail_cap_bits(&sail_all_bits, pesbt, cursor);
    struct zCapability sail_result = sailgen_capBitsToCapability(tag, sail_all_bits);
    KILL(sail_cap_bits)(&sail_all_bits);
    return sail_result;
}

_cc_cap_t _cc_sail(decode_raw)(_cc_addr_t pesbt, _cc_addr_t cursor, bool tag) {
    struct zCapability sail_result = _sail_decode_common_raw_impl(pesbt, cursor, tag);
    return sail_cap_to_cap_t(&sail_result);
}

static _cc_bounds_bits sail_extract_bounds_bits_common(_cc_addr_t pesbt) {
    _cc_bounds_bits result;
    struct zCapability sail_result = _sail_decode_common_raw_impl(pesbt, 0, false);
    result.E = sail_result.zE;
    result.B = sail_result.zB;
    result.T = sail_result.zT;
    result.IE = sail_result.zinternal_E;
    return result;
}

static struct zCapability cap_t_to_sail_cap(const _cc_cap_t* c) {
    struct zCapability result;
    result = znull_cap;
    result = sailgen_setCapPerms(result, _cc_N(get_perms)(c));
    result.ztag = c->cr_tag;
    result.zreserved = _CC_EXTRACT_FIELD(c->cr_pesbt, RESERVED);
    result.zaddress = c->_cr_cursor;
    result.zuperms = _cc_N(get_uperms)(c);
    result.zotype = _cc_N(get_otype)(c);
    result.zflag_cap_mode = _cc_N(get_flags)(c);

    // Extract E,B,T,IE from the cr_pesbt field:
    _cc_addr_t fake_pesbt = c->cr_pesbt;
    _cc_bounds_bits c_bounds = _cc_N(extract_bounds_bits)(fake_pesbt);
    result.zinternal_E = c_bounds.IE;
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
#ifdef SAIL_COMPRESSION_INDIRECT_BITS
    sailgen_capToMemBits(&sailbits, sailcap);
#else
    sailbits = sailgen_capToMemBits(sailcap);
#endif
    _cc_addr_t result = extract_sail_cap_bits(&sailbits, _CC_ADDR_WIDTH, _CC_ADDR_WIDTH);
    KILL(sail_cap_bits)(&sailbits);
    return result;
}

bool _cc_sail(fast_is_representable)(const _cc_cap_t* cap, _cc_addr_t new_addr) {
    struct zCapability sailcap = cap_t_to_sail_cap(cap);
    uint64_t increment = new_addr - cap->_cr_cursor;
    return sailgen_fastRepCheck(sailcap, increment);
}

_cc_addr_t _cc_sail(compress_raw)(const _cc_cap_t* csp) { return sail_compress_common_raw(csp); }
_cc_addr_t _cc_sail(compress_mem)(const _cc_cap_t* csp) { return sail_compress_common_mem(csp); }
