#include "sail_wrapper.h"

#include "sail.h"

#include "sail_compression_128.c"

__attribute__((constructor, used)) static void sail_startup(void) { model_init(); }
__attribute__((destructor,used)) static void sail_cleanup(void) { model_fini(); }

static inline void sail_dump_cap(const char* msg, struct zCapability cap) {
    sail_string zcap_str;
    CREATE(sail_string)(&zcap_str);
    sailgen_capToString(&zcap_str, cap);
    fprintf(stderr, "%s: %s\n", msg, zcap_str);
    KILL(sail_string)(&zcap_str);
}

static uint64_t extract_bits(lbits op, uint64_t start, uint64_t len) {
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

static void set_top_base_from_sail(const struct zCapability* sail, cap_register_t* c) {
    // Would be nice to have a stable name for this tuple:
    struct ztuple_z8z5bv64zCz0z5bvz9 base_top;
    CREATE(ztuple_z8z5bv64zCz0z5bvz9)(&base_top);
    sailgen_getCapBoundsBits(&base_top, *sail);
    c->cr_base = base_top.ztup0;
    sail_int top_len;
    CREATE(sail_int)(&top_len);
    length_lbits(&top_len, base_top.ztup1);
    assert(CONVERT_OF(mach_int, sail_int)(top_len) == 65);
    KILL(sail_int)(&top_len);
    fbits top_high = extract_bits(base_top.ztup1, 64, 1);
    fbits top_low = CONVERT_OF(fbits, lbits)(base_top.ztup1, true);
    c->_cr_top = (((cc128_length_t)top_high) << 64) | (cc128_length_t)top_low;
    KILL(ztuple_z8z5bv64zCz0z5bvz9)(&base_top);
}

static void sail_cap_to_cap_register_t(const struct zCapability* sail, cap_register_t* c) {
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
    lbits raw_cap_bits;
    // TODO: avoid roundtrip via sailgen_capToBits?
    CREATE(lbits)(&raw_cap_bits);
    sailgen_capToBits(&raw_cap_bits, *sail);
    uint64_t sail_pesbt = extract_bits(raw_cap_bits, 64, 64);
    c->cr_ebt = (uint32_t)CC128_EXTRACT_FIELD(sail_pesbt, EBT);
    KILL(lbits)(&raw_cap_bits);
}

static void two_u64s_to_sail_128(lbits* out, uint64_t first64, uint64_t second64) {
    lbits sail_first64;
    lbits sail_second64;
    CREATE_OF(lbits, fbits)(&sail_first64, first64, UINT64_C(64), true);
    CREATE_OF(lbits, fbits)(&sail_second64, second64, UINT64_C(64), true);
    CREATE(lbits)(out);
    append(out, sail_first64, sail_second64);
    KILL(lbits)(&sail_first64);
    KILL(lbits)(&sail_second64);
}

void sail_decode_128_mem(uint64_t mem_pesbt, uint64_t mem_cursor, bool tag, cap_register_t* cdp) {
    lbits sail_all_bits;
    two_u64s_to_sail_128(&sail_all_bits, mem_pesbt, mem_cursor);
    struct zCapability sail_result = sailgen_memBitsToCapability(tag, sail_all_bits);
    KILL(lbits)(&sail_all_bits);
    // sail_dump_cap("sail_result", sail_result);
    sail_cap_to_cap_register_t(&sail_result, cdp);
}

struct zCapability _sail_decode_128_raw_impl(uint64_t pesbt, uint64_t cursor, bool tag) {
    lbits sail_all_bits;
    two_u64s_to_sail_128(&sail_all_bits, pesbt, cursor);
    struct zCapability sail_result = sailgen_capBitsToCapability(tag, sail_all_bits);
    KILL(lbits)(&sail_all_bits);
    return sail_result;
}

void sail_decode_128_raw(uint64_t pesbt, uint64_t cursor, bool tag, cap_register_t* cdp) {
    struct zCapability sail_result = _sail_decode_128_raw_impl(pesbt, cursor, tag);
    sail_cap_to_cap_register_t(&sail_result, cdp);
}

struct cc128_bounds_bits sail_extract_bounds_bits_128(uint64_t pesbt) {
    struct cc128_bounds_bits result;
    struct zCapability sail_result = _sail_decode_128_raw_impl(pesbt, 0, false);
    result.E = sail_result.zE;
    result.B = sail_result.zB;
    result.T = sail_result.zT;
    result.IE = sail_result.zinternal_e;
    return result;
}

static inline lbits cc_length_to_lbits(cc128_length_t l) {
    lbits highbit;
    CREATE_OF(lbits, fbits)(&highbit, (uint64_t)(l >> 64), 1, true);
    lbits low64;
    CREATE_OF(lbits, fbits)(&low64, (uint64_t)l, 64, true);
    lbits result;
    CREATE(lbits)(&result);
    append(&result, highbit, low64);
    KILL(lbits)(&highbit);
    KILL(lbits)(&low64);
    // print_bits("cclen: ", result);
    return result;
}

static struct zCapability cap_register_t_to_sail_cap(const cap_register_t* c) {
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
    uint64_t fake_pesbt = CC128_ENCODE_FIELD(c->cr_ebt, EBT);
    struct cc128_bounds_bits cc128_bounds = cc128_extract_bounds_bits(fake_pesbt);
    result.zinternal_e = cc128_bounds.IE;
    result.zE = cc128_bounds.E;
    result.zT = cc128_bounds.T;
    result.zB = cc128_bounds.B;
#if 0
    lbits top = cc_length_to_lbits(c->_cr_top);
    struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 bounded = sailgen_setCapBounds(result, c->cr_base, top);
    KILL(lbits)(&top);
    assert(bounded.ztup0 && "Setbounds not exact?");
    result = bounded.ztup1; // Get E/B/T from bounded cap
    sail_dump_cap("sail_result", result);
#endif
    return result;
}

uint64_t sail_compress_128_raw(const cap_register_t* csp) {
    struct zCapability sailcap = cap_register_t_to_sail_cap(csp);
    lbits sailbits;
    CREATE(lbits)(&sailbits);
    sailgen_capToBits(&sailbits, sailcap);
    uint64_t result = extract_bits(sailbits, 64, 64);
    KILL(lbits)(&sailbits);
    return result;
}

uint64_t sail_compress_128_mem(const cap_register_t* csp) {
    struct zCapability sailcap = cap_register_t_to_sail_cap(csp);
    lbits sailbits;
    CREATE(lbits)(&sailbits);
    sailgen_capToMemBits(&sailbits, sailcap);
    uint64_t result = extract_bits(sailbits, 64, 64);
    KILL(lbits)(&sailbits);
    return result;
}

uint64_t sail_null_pesbt(void) {
    // NULL CAP BITS:
    lbits null_bits;
    CREATE(lbits)(&null_bits);
    sailgen_capToBits(&null_bits, znull_cap);
    sail_dump_cap("null cap", znull_cap);
    print_bits("null bits: ", null_bits);
    uint64_t result = extract_bits(null_bits, 64, 64);
    KILL(lbits)(&null_bits);
    return result;
}
