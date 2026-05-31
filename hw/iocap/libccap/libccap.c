// Include platform-specific setup, including the path to the header file.
// This file should be customized for each platform.
#include "libccap_platform.incl.c"

// Include generated bitfield for Cap2024_11
#include "libccap_bitfields.incl.c"

#if !defined(LIBCCAP_MEMCMP)
// Previously I defined it as LIBCCAP_MEMCMP(left, right, size) = memcmp(left, right, size)
// but that breaks down in more complicated scenarios.
// See https://tech.lgbt/@fay59/113903948791120892
#define LIBCCAP_MEMCMP memcmp
#endif

#if !defined(libccap_dbg_trace)
#define libccap_dbg_trace(...)
#endif

#if !defined(LIBCCAP_INTERNAL_FUNC_PREFIX)
#define LIBCCAP_INTERNAL_FUNC_PREFIX static inline
#endif

#if !defined(LIBCCAP_EXTERNAL_FUNC_PREFIX)
#define LIBCCAP_EXTERNAL_FUNC_PREFIX
#endif

#if !defined(LIBCCAP_OPTIMIZE_OUT_PANIC)
#define LIBCCAP_OPTIMIZE_OUT_PANIC 0
#endif

#if !defined(LIBCCAP_USE_INITIAL_REGION_LUT)
#define LIBCCAP_USE_INITIAL_REGION_LUT 1
#endif
#if !LIBCCAP_USE_INITIAL_REGION_LUT
#error "Libccap doesn't work without the initial region LUT right now"
#endif

// LIBCCAP_USE_RESULT_FUNC_PREFIX is defined by libccap.h, which libccap_platform.incl.c includes

#if !defined(LIBCCAP_ILOG2)
// https://stackoverflow.com/a/22418446
LIBCCAP_INTERNAL_FUNC_PREFIX uint8_t libccap_ilog2(uint64_t x) {
    if (x == 0) return 0;

    return 64 - __builtin_clzl(x) - 1;
}
#define LIBCCAP_ILOG2(x) libccap_ilog2(x)
#endif

#if !defined(LIBCCAP_NORETURN_SPECIFIER)
    #if defined(__STDC_VERSION__)
        #if (__STDC_VERSION__ >= 202311L)
            #define LIBCCAP_NORETURN_SPECIFIER [[noreturn]]
        #elif (__STDC_VERSION__ >= 201112L)
            #define LIBCCAP_NORETURN_SPECIFIER _Noreturn
        #else
            #define LIBCCAP_NORETURN_SPECIFIER
        #endif
    #else
        #define LIBCCAP_NORETURN_SPECIFIER
    #endif
#endif

// Simple reimplementation of the Rust panic! without nice argument handling, for internal use only
LIBCCAP_INTERNAL_FUNC_PREFIX LIBCCAP_NORETURN_SPECIFIER void libccap_simple_panic(const char* msg) {
    #if LIBCCAP_OPTIMIZE_OUT_PANIC
    __builtin_unreachable();
    #else
    // very simple strlen, because we can't depend on libraries
    uint64_t len;
    for (len = 0; msg[len] != '\0'; len++);
    // write out the message
    ccap_panic_write_utf8((const uint8_t*)msg, len);
    // call the finalizing function, which may never return to us
    ccap_panic_complete();
    // block forever
    do {} while(1);
    #endif
}

// CapPermsChain
enum CapPermsChain {
    CapPermsChain_ReadWrite_0Cav = 0b0011,
    CapPermsChain_ReadWrite_1Cav = 0b0111,
    CapPermsChain_ReadWrite_2Cav = 0b1011,
    // 0b1111 not used

    // if 0b00_XX where XX != 11 then it's read only, no reductions, XX = num caveats
    CapPermsChain_ReadOnly_0Cav = 0b0000,
    CapPermsChain_ReadOnly_1Cav = 0b0001,
    CapPermsChain_ReadOnly_2Cav = 0b0010,

    // if 0b10_10 then shrunk to read by first and only caveat
    CapPermsChain_ShrunkToRead_1Cav = 0b1010,
    // if 0b10_0X then 2 caveats, shrunk to read by caveat #(X+1)
    CapPermsChain_ShrunkToRead_2Cav_ByCav1 = 0b1000,
    CapPermsChain_ShrunkToRead_2Cav_ByCav2 = 0b1001,

    // if 0b11_XX where XX != 11 then it's write only, no reductions, XX caveats
    CapPermsChain_WriteOnly_0Cav = 0b1100,
    CapPermsChain_WriteOnly_1Cav = 0b1101,
    CapPermsChain_WriteOnly_2Cav = 0b1110,

    // if 0b01_10 then shrunk to write by first and only caveat
    CapPermsChain_ShrunkToWrite_1Cav = 0b0110,
    // if 0b01_0X then 2 caveats, shrunk to write by caveat #(X+1)
    CapPermsChain_ShrunkToWrite_2Cav_ByCav1 = 0b0100,
    CapPermsChain_ShrunkToWrite_2Cav_ByCav2 = 0b0101,
};
typedef uint8_t CapPermsChain;

LIBCCAP_INTERNAL_FUNC_PREFIX CapPermsChain libccap_root_perms_chain_for_cap_perms(CCapPerms perms) {
    switch (perms) {
        case CCapPerms_Read:
            return CapPermsChain_ReadOnly_0Cav;
        case CCapPerms_Write:
            return CapPermsChain_WriteOnly_0Cav;
        case CCapPerms_ReadWrite:
            return CapPermsChain_ReadWrite_0Cav;
        default:
            libccap_simple_panic("Invalid perms");
            return 0b1111; // Invalid value
    }
}
LIBCCAP_USE_RESULT_FUNC_PREFIX
LIBCCAP_INTERNAL_FUNC_PREFIX CCapResult libccap_add_perms_caveat_to_cap_perms_chain(uint8_t chain, CCapPerms perms, CapPermsChain* out) {
    switch (chain) {
        case CapPermsChain_ReadOnly_2Cav:
        case CapPermsChain_ReadWrite_2Cav:
        case CapPermsChain_WriteOnly_2Cav:
        case CapPermsChain_ShrunkToRead_2Cav_ByCav1:
        case CapPermsChain_ShrunkToRead_2Cav_ByCav2:
        case CapPermsChain_ShrunkToWrite_2Cav_ByCav1:
        case CapPermsChain_ShrunkToWrite_2Cav_ByCav2:
            return CCapResult_Encode_NoCaveatsLeft;

        case CapPermsChain_ReadWrite_0Cav:
            switch (perms) {
                case CCapPerms_Read:
                    *out = CapPermsChain_ShrunkToRead_1Cav;
                    return CCapResult_Success;
                case CCapPerms_Write:
                    *out =  CapPermsChain_ShrunkToWrite_1Cav;
                    return CCapResult_Success;
                case CCapPerms_ReadWrite:
                    *out =  CapPermsChain_ReadWrite_1Cav;
                    return CCapResult_Success;
                default:
                    return CCapResult_Encode_InvalidPerms;
            }

        case CapPermsChain_ReadWrite_1Cav:
            switch (perms) {
                case CCapPerms_Read:
                    *out = CapPermsChain_ShrunkToRead_2Cav_ByCav2;
                    return CCapResult_Success;
                case CCapPerms_Write:
                    *out =  CapPermsChain_ShrunkToWrite_2Cav_ByCav2;
                    return CCapResult_Success;
                case CCapPerms_ReadWrite:
                    *out =  CapPermsChain_ReadWrite_2Cav;
                    return CCapResult_Success;
                default:
                    return CCapResult_Encode_InvalidPerms;
            }

        case CapPermsChain_ReadOnly_0Cav:
            if (perms != CCapPerms_Read) {
                return CCapResult_Encode_CantShrinkPerms;
            }
            *out = CapPermsChain_ReadOnly_1Cav;
            return CCapResult_Success;

        case CapPermsChain_ReadOnly_1Cav:
            if (perms != CCapPerms_Read) {
                return CCapResult_Encode_CantShrinkPerms;
            }
            *out = CapPermsChain_ReadOnly_2Cav;
            return CCapResult_Success;

        case CapPermsChain_ShrunkToRead_1Cav:
            if (perms != CCapPerms_Read) {
                return CCapResult_Encode_CantShrinkPerms;
            }
            *out = CapPermsChain_ShrunkToRead_2Cav_ByCav1;
            return CCapResult_Success;

        case CapPermsChain_WriteOnly_0Cav:
            if (perms != CCapPerms_Write) {
                return CCapResult_Encode_CantShrinkPerms;
            }
            *out = CapPermsChain_WriteOnly_1Cav;
            return CCapResult_Success;

        case CapPermsChain_WriteOnly_1Cav:
            if (perms != CCapPerms_Write) {
                return CCapResult_Encode_CantShrinkPerms;
            }
            *out = CapPermsChain_WriteOnly_2Cav;
            return CCapResult_Success;

        case CapPermsChain_ShrunkToWrite_1Cav:
            if (perms != CCapPerms_Write) {
                return CCapResult_Encode_CantShrinkPerms;
            }
            *out = CapPermsChain_ShrunkToWrite_2Cav_ByCav1;
            return CCapResult_Success;

        default:
            return CCapResult_Encode_InvalidPerms;
    }
}
LIBCCAP_USE_RESULT_FUNC_PREFIX
LIBCCAP_INTERNAL_FUNC_PREFIX CCapResult libccap_perms_for_cap_perms_chain(uint8_t chain, CCapPerms* out) {
    switch (chain) {
        case CapPermsChain_ReadWrite_0Cav:
        case CapPermsChain_ReadWrite_1Cav:
        case CapPermsChain_ReadWrite_2Cav:
            *out = CCapPerms_ReadWrite;
            return CCapResult_Success;
        case CapPermsChain_ReadOnly_0Cav:
        case CapPermsChain_ReadOnly_1Cav:
        case CapPermsChain_ReadOnly_2Cav:
        case CapPermsChain_ShrunkToRead_1Cav:
        case CapPermsChain_ShrunkToRead_2Cav_ByCav1:
        case CapPermsChain_ShrunkToRead_2Cav_ByCav2:
            *out = CCapPerms_Read;
            return CCapResult_Success;
        case CapPermsChain_WriteOnly_0Cav:
        case CapPermsChain_WriteOnly_1Cav:
        case CapPermsChain_WriteOnly_2Cav:
        case CapPermsChain_ShrunkToWrite_1Cav:
        case CapPermsChain_ShrunkToWrite_2Cav_ByCav1:
        case CapPermsChain_ShrunkToWrite_2Cav_ByCav2:
            *out = CCapPerms_Write;
            return CCapResult_Success;
        default:
            return CCapResult_Decode_InvalidCapPermsChain;
    }
}
LIBCCAP_USE_RESULT_FUNC_PREFIX
LIBCCAP_INTERNAL_FUNC_PREFIX CCapResult libccap_num_cavs_for_cap_perms_chain(uint8_t chain, uint8_t* out) {
    switch (chain) {
        case CapPermsChain_ReadWrite_0Cav:
        case CapPermsChain_ReadOnly_0Cav:
        case CapPermsChain_WriteOnly_0Cav:
            *out = 0;
            return CCapResult_Success;

        case CapPermsChain_ReadWrite_1Cav:
        case CapPermsChain_ReadOnly_1Cav:
        case CapPermsChain_ShrunkToRead_1Cav:
        case CapPermsChain_WriteOnly_1Cav:
        case CapPermsChain_ShrunkToWrite_1Cav:
            *out = 1;
            return CCapResult_Success;

        case CapPermsChain_ReadWrite_2Cav:
        case CapPermsChain_ReadOnly_2Cav:
        case CapPermsChain_WriteOnly_2Cav:
        case CapPermsChain_ShrunkToRead_2Cav_ByCav1:
        case CapPermsChain_ShrunkToRead_2Cav_ByCav2:
        case CapPermsChain_ShrunkToWrite_2Cav_ByCav1:
        case CapPermsChain_ShrunkToWrite_2Cav_ByCav2:
            *out = 2;
            return CCapResult_Success;

        default:
            return CCapResult_Decode_InvalidCapPermsChain;
    }
}
LIBCCAP_INTERNAL_FUNC_PREFIX void libccap_cap_perms_chain_at_root(CapPermsChain chain, CapPermsChain* out) {
    switch (chain) {
        case CapPermsChain_ReadOnly_0Cav:
        case CapPermsChain_ReadOnly_1Cav:
        case CapPermsChain_ReadOnly_2Cav:
            *out = CapPermsChain_ReadOnly_0Cav;
            return;
        case CapPermsChain_WriteOnly_0Cav:
        case CapPermsChain_WriteOnly_1Cav:
        case CapPermsChain_WriteOnly_2Cav:
            *out = CapPermsChain_WriteOnly_0Cav;
            return;
        case CapPermsChain_ReadWrite_0Cav:
        case CapPermsChain_ReadWrite_1Cav:
        case CapPermsChain_ReadWrite_2Cav:
        case CapPermsChain_ShrunkToRead_1Cav:
        case CapPermsChain_ShrunkToWrite_1Cav:
        case CapPermsChain_ShrunkToRead_2Cav_ByCav1:
        case CapPermsChain_ShrunkToRead_2Cav_ByCav2:
        case CapPermsChain_ShrunkToWrite_2Cav_ByCav1:
        case CapPermsChain_ShrunkToWrite_2Cav_ByCav2:
            *out = CapPermsChain_ReadWrite_0Cav;
            return;
    }
}
LIBCCAP_INTERNAL_FUNC_PREFIX bool libccap_cap_perms_chain_at_cav1(CapPermsChain chain, CapPermsChain* out) {
    switch (chain) {
        // 0-caveat chains don't have a cav1
        case CapPermsChain_ReadOnly_0Cav:
        case CapPermsChain_WriteOnly_0Cav:
        case CapPermsChain_ReadWrite_0Cav:
            return false;
        // 1-caveat chains *are* their cav1
        case CapPermsChain_ReadOnly_1Cav:
        case CapPermsChain_WriteOnly_1Cav:
        case CapPermsChain_ReadWrite_1Cav:
        case CapPermsChain_ShrunkToRead_1Cav:
        case CapPermsChain_ShrunkToWrite_1Cav:
            *out = chain;
            return true;
        // 2-caveat chains are based on a cav1
        case CapPermsChain_ReadOnly_2Cav:
            *out = CapPermsChain_ReadOnly_1Cav;
            return true;
        case CapPermsChain_WriteOnly_2Cav:
            *out = CapPermsChain_WriteOnly_1Cav;
            return true;
        case CapPermsChain_ReadWrite_2Cav:
            *out = CapPermsChain_ReadWrite_1Cav;
            return true;
        case CapPermsChain_ShrunkToRead_2Cav_ByCav1:
            *out = CapPermsChain_ShrunkToRead_1Cav;
            return true;
        case CapPermsChain_ShrunkToRead_2Cav_ByCav2:
            *out = CapPermsChain_ReadWrite_1Cav;
            return true;
        case CapPermsChain_ShrunkToWrite_2Cav_ByCav1:
            *out = CapPermsChain_ShrunkToWrite_1Cav;
            return true;
        case CapPermsChain_ShrunkToWrite_2Cav_ByCav2:
            *out = CapPermsChain_ReadWrite_1Cav;
            return true;
        default:
            return false;
    }
}
LIBCCAP_INTERNAL_FUNC_PREFIX bool libccap_cap_perms_chain_at_cav2(CapPermsChain chain, CapPermsChain* out) {
    switch (chain) {
        // 0-caveat chains don't have a cav2
        case CapPermsChain_ReadOnly_0Cav:
        case CapPermsChain_WriteOnly_0Cav:
        case CapPermsChain_ReadWrite_0Cav:
            return false;
        // 1-caveat chains don't have a cav2
        case CapPermsChain_ReadOnly_1Cav:
        case CapPermsChain_WriteOnly_1Cav:
        case CapPermsChain_ReadWrite_1Cav:
        case CapPermsChain_ShrunkToRead_1Cav:
        case CapPermsChain_ShrunkToWrite_1Cav:
            return false;
        // 2-caveat chains *are* their cav2
        case CapPermsChain_ReadOnly_2Cav:
        case CapPermsChain_WriteOnly_2Cav:
        case CapPermsChain_ReadWrite_2Cav:
        case CapPermsChain_ShrunkToRead_2Cav_ByCav1:
        case CapPermsChain_ShrunkToRead_2Cav_ByCav2:
        case CapPermsChain_ShrunkToWrite_2Cav_ByCav1:
        case CapPermsChain_ShrunkToWrite_2Cav_ByCav2:
            *out = chain;
            return true;
        default:
            return false;
    }
}


#if LIBCCAP_USE_INITIAL_REGION_LUT
typedef struct {
    // uint8_t w;
    uint8_t e;
    uint8_t m_top;
    uint8_t m_bits;
    // uint8_t m_top_and_bits; // (m_top << 7) | m_bits
    uint8_t b_bits; // = b_m_width - m_bits
    uint8_t b_align; // = addr_size - b_bits
    uint8_t min_used_pow2;
    uint8_t max_encodable_pow2;
} CCapWidthClass;
#define PACK_M_BITS(M_TOP, M_BITS) \
    .m_top=M_TOP,                  \
    .m_bits=M_BITS,                \
    .b_bits=49-M_BITS,             \
    .b_align=(54 - (49 - M_BITS))
    // ((((uint8_t)m_top)<<7) | ((uint8_t)m_bits))
const static CCapWidthClass ccap2024_width_classes[32] = {
    { .e=10, PACK_M_BITS(0, 5), .min_used_pow2=10, .max_encodable_pow2=15, },
    { .e=10, PACK_M_BITS(1, 5), .min_used_pow2=15, .max_encodable_pow2=16, },
    { .e=11, PACK_M_BITS(1, 5), .min_used_pow2=16, .max_encodable_pow2=17, },
    { .e=12, PACK_M_BITS(1, 5), .min_used_pow2=17, .max_encodable_pow2=18, },
    { .e=13, PACK_M_BITS(1, 5), .min_used_pow2=18, .max_encodable_pow2=19, },
    { .e=14, PACK_M_BITS(1, 5), .min_used_pow2=19, .max_encodable_pow2=20, },
    { .e=15, PACK_M_BITS(1, 5), .min_used_pow2=20, .max_encodable_pow2=21, },
    { .e=16, PACK_M_BITS(1, 5), .min_used_pow2=21, .max_encodable_pow2=22, },

    { .e=17, PACK_M_BITS(0, 12), .min_used_pow2=22, .max_encodable_pow2=29, },
    { .e=17, PACK_M_BITS(1, 12), .min_used_pow2=29, .max_encodable_pow2=30, },
    { .e=18, PACK_M_BITS(1, 12), .min_used_pow2=30, .max_encodable_pow2=31, },
    { .e=19, PACK_M_BITS(1, 12), .min_used_pow2=31, .max_encodable_pow2=32, },
    { .e=20, PACK_M_BITS(1, 12), .min_used_pow2=32, .max_encodable_pow2=33, },
    { .e=21, PACK_M_BITS(1, 12), .min_used_pow2=33, .max_encodable_pow2=34, },
    { .e=22, PACK_M_BITS(1, 12), .min_used_pow2=34, .max_encodable_pow2=35, },
    { .e=23, PACK_M_BITS(1, 12), .min_used_pow2=35, .max_encodable_pow2=36, },

    { .e=24, PACK_M_BITS(0, 19), .min_used_pow2=36, .max_encodable_pow2=43, },
    { .e=24, PACK_M_BITS(1, 19), .min_used_pow2=43, .max_encodable_pow2=44, },
    { .e=25, PACK_M_BITS(1, 19), .min_used_pow2=44, .max_encodable_pow2=45, },
    { .e=26, PACK_M_BITS(1, 19), .min_used_pow2=45, .max_encodable_pow2=46, },
    { .e=27, PACK_M_BITS(1, 19), .min_used_pow2=46, .max_encodable_pow2=47, },
    { .e=28, PACK_M_BITS(1, 19), .min_used_pow2=47, .max_encodable_pow2=48, },
    { .e=29, PACK_M_BITS(1, 19), .min_used_pow2=48, .max_encodable_pow2=49, },
    { .e=30, PACK_M_BITS(1, 19), .min_used_pow2=49, .max_encodable_pow2=50, },

    { .e=31, PACK_M_BITS(0, 26), .min_used_pow2=50, .max_encodable_pow2=57, },
    { .e=31, PACK_M_BITS(1, 26), .min_used_pow2=57, .max_encodable_pow2=58, },
    { .e=32, PACK_M_BITS(1, 26), .min_used_pow2=58, .max_encodable_pow2=59, },
    { .e=33, PACK_M_BITS(1, 26), .min_used_pow2=59, .max_encodable_pow2=60, },
    { .e=34, PACK_M_BITS(1, 26), .min_used_pow2=60, .max_encodable_pow2=61, },
    { .e=35, PACK_M_BITS(1, 26), .min_used_pow2=61, .max_encodable_pow2=62, },
    { .e=36, PACK_M_BITS(1, 26), .min_used_pow2=62, .max_encodable_pow2=63, },
    { .e=37, PACK_M_BITS(1, 26), .min_used_pow2=63, .max_encodable_pow2=64, }
};

const static uint8_t ccap2024_length_pow2_to_width_class[64] = {
    // 0..9 -> w0
    0,0,0,0,0,
    0,0,0,0,0,
    // 10..15 -> w0
    0,0,0,0,0,
    // 16->w1, .. 22->w7
    1,2,3,4,5,6,7,
    // 23..29 -> w8
    8,8,8,8,8,8,8,
    // 30->w9, 36->w15
    9,10,11,12,13,14,15,
    // 37..43 -> w16
    16,16,16,16,16,16,16,
    // 44->w17, 50->w23
    17,18,19,20,21,22,23,
    // 51..57 -> w24
    24,24,24,24,24,24,24,
    // 58..63 (would be 64, but we don't support those lengths here)
    // w25->w31
    25,26,27,28,29,30,31
};
#endif
// INTERNAL
LIBCCAP_USE_RESULT_FUNC_PREFIX
LIBCCAP_INTERNAL_FUNC_PREFIX CCapResult libccap_read_bits_range(const struct Cap2024_11_Bits bits,
                                  uint64_t *base_ptr,
                                  uint64_t *len_ptr,
                                  bool *len_64_ptr) {

    uint8_t num_cavs = 0;
    if (libccap_num_cavs_for_cap_perms_chain(bits.perms, &num_cavs) != CCapResult_Success) {
        return CCapResult_Decode_InvalidCapPermsChain;
    }
    libccap_dbg_trace("libccap_read_bits_range perms %x n_cavs %d\n", bits.perms, num_cavs);
    // Ensure that caveats are only nonzero when they are used by the capability
    if (num_cavs < 1 && (bits.index != 0 || bits.index_size_div != 0)) {
        return CCapResult_Decode_UnexpectedCaveat;
    }
    if (num_cavs < 2 && (bits.range_x != 0 || bits.range_y_minus_one != 0)) {
        return CCapResult_Decode_UnexpectedCaveat;
    }

    #if LIBCCAP_USE_INITIAL_REGION_LUT
    uint8_t w = (bits.encoded_elem_width + 1) & 0b11111;
    const CCapWidthClass* w_cl = &ccap2024_width_classes[w];
    uint64_t base = (bits.b_c >> (w_cl->m_bits)) << (w_cl->b_align);
    uint64_t elem_count_mask = ((uint64_t)-1) >> (64 - w_cl->m_bits);
    uint64_t elem_count = ((
        ((uint64_t)w_cl->m_top) << (w_cl->m_bits)
    ) | (
        bits.b_c & elem_count_mask
    )) + 1;
    uint8_t elem_width_log2 = w_cl->e;
    #else
    // Initial resource decoding
    uint64_t implicit_1 = (bits.encoded_elem_width & 0b111) != 0b111;

    uint8_t q = (bits.encoded_elem_width >> 3) & 0b11;
    uint8_t elem_width_log2;
    switch (q) {
        case 0:
            elem_width_log2 = bits.encoded_elem_width + 10;
            break;
        case 1:
            elem_width_log2 = bits.encoded_elem_width + 10 - 1;
            break;
        case 2:
            elem_width_log2 = bits.encoded_elem_width + 10 - 2;
            break;
        case 3:
            if (bits.encoded_elem_width == 31) {
                elem_width_log2 = 10;
            } else {
                elem_width_log2 = bits.encoded_elem_width + 10 - 3;
            }
            break;
        default:
            libccap_simple_panic("Impossible q value != 0,1,2,3");
            return CCapResult_CatastrophicFailure;
    }

    uint8_t quadrant = q;
    if (!implicit_1) {
        quadrant = (q + 1) & 0b11;
    }

    uint64_t base;
    uint64_t elem_count;

    switch (quadrant) {
        case 0:
            base = (bits.b_c >> 5) << 10;
            elem_count = ((implicit_1 << 5) | (bits.b_c & 0x1F)) + 1;
            break;
        case 1:
            base = (bits.b_c >> 12) << 17;
            elem_count = ((implicit_1 << 12) | (bits.b_c & 0xFFF)) + 1;
            break;
        case 2:
            base = (bits.b_c >> 19) << 24;
            elem_count = ((implicit_1 << 19) | (bits.b_c & 0x7FFFF)) + 1;
            break;
        case 3:
            base = (bits.b_c >> 26) << 31;
            elem_count = ((implicit_1 << 26) | (bits.b_c & 0x3FFFFFF)) + 1;
            break;
        default:
            libccap_simple_panic("Impossible quadrant value != 0,1,2,3");
            return CCapResult_CatastrophicFailure;
    }
    #endif

    // The length may be (1 << 64) - detect by shifting up by one less than the actual amount and seeing if the top bit is set.
    bool len_64 = (elem_count << (elem_width_log2 - 1)) >> 63;
    uint64_t length = elem_count << elem_width_log2;

    libccap_dbg_trace("libccap_read_bits_range initial w %d elem_count %lx elem_count_mask %lx m_top %d base %lx len %lx\n", w, elem_count, elem_count_mask, w_cl->m_top, base, length);

    if (num_cavs > 0) {
        // cav1

        #if LIBCCAP_USE_INITIAL_REGION_LUT
        uint8_t log_max_count = w_cl->m_bits + w_cl->m_top;
        #else
        uint8_t log_max_count;
        switch (quadrant) {
            case 0:
                log_max_count = 5 + implicit_1;
                break;
            case 1:
                log_max_count = 12 + implicit_1;
                break;
            case 2:
                log_max_count = 19 + implicit_1;
                break;
            case 3:
                log_max_count = 26 + implicit_1;
                break;
            default:
                libccap_simple_panic("Impossible quadrant value != 0,1,2,3");
                return CCapResult_CatastrophicFailure;
        }
        #endif

        uint64_t check_lhs = ((uint64_t)(bits.index + 1)) << log_max_count;
        uint64_t check_rhs = elem_count << bits.index_size_div;

        uint8_t elem_width_log2_cav1 = elem_width_log2 + log_max_count - bits.index_size_div;
        uint64_t elem_count_cav1 = 1;
        uint64_t base_offset_cav1 = ((uint64_t)bits.index) << elem_width_log2_cav1;

        bool identity_cav1 = false;

        if (check_lhs > check_rhs) {
            // cav1 exceeds the bounds of the initial resource
            if (bits.index == 0) {
                // This is an identity-sized cav1, which uses the initial base and length but can be shrunk further by a cav2
                identity_cav1 = true;
                libccap_dbg_trace("libccap_read_bits_range cav1    identity base %lx len %lx\n", base, length);
            } else {
                // This is invalid
                return CCapResult_Decode_InvalidCaveat;
            }
        } else {
            uint64_t base_cav1 = base + base_offset_cav1;
            if (base_cav1 < base) {
                // overflowed 64 bits
                return CCapResult_Decode_InvalidCaveat;
            }
            base = base_cav1;
            len_64 = (elem_width_log2_cav1 == 64);
            length = len_64 ? 0 : (elem_count_cav1 << elem_width_log2_cav1);

            libccap_dbg_trace("libccap_read_bits_range cav1    base %lx len %lx\n", base, length);
        }

        if (num_cavs > 1) {
            if (bits.range_y_minus_one < bits.range_x) {
                return CCapResult_Decode_InvalidCaveat;
            }

            uint16_t elem_count_cav2 = bits.range_y_minus_one - bits.range_x + 1;
            uint8_t elem_width_log2_cav2;
            uint64_t base_offset_cav2;
            bool cav2_force_match_cav1 = false;
            if (elem_width_log2_cav1 >= 14) {
                elem_width_log2_cav2 = elem_width_log2_cav1 - 14;
                base_offset_cav2 = ((uint64_t)bits.range_x) << elem_width_log2_cav2;

                if (identity_cav1) {
                    // Compute the offset of the top of cav2 from the base of cav1
                    uint64_t top_offset_cav2 = ((((uint64_t)bits.range_y_minus_one) + 1) << elem_width_log2_cav2);
                    bool top_offset_64_cav2 = (elem_width_log2_cav2 == 0) ? 0 : (((uint64_t)(bits.range_y_minus_one + 1)) << (elem_width_log2_cav2 - 1)) >> 63;

                    // if that exceeds the length of cav0 (i.e. the current (len, len_64) bits) then fail
                    if (
                        // either top_offset has the 64th bit set and original length doesn't
                        (top_offset_64_cav2 && !len_64)
                        // or the top bits are equal and the the bottom bits of top_offset are greater
                        || ((top_offset_cav2 > length) && (top_offset_64_cav2 == len_64))
                    ) {
                        if (bits.range_x == 0) {
                            cav2_force_match_cav1 = true;
                        } else {
                            return CCapResult_Decode_InvalidCaveat;
                        }
                    }
                }
            } else {
                elem_count_cav2 = elem_count_cav2 >> (14 - elem_width_log2_cav1);
                elem_width_log2_cav2 = 0;
                base_offset_cav2 = bits.range_x >> (14 - elem_width_log2_cav1);

                if (identity_cav1) {
                    // as above, except we need to be careful here to add together the shifted-down versions
                    // of elem_count and base_offset rather than use range_y directly,
                    // (range_y - range_x + 1) >> BLAH + range_x >> BLAH
                    //                         !=
                    //               (range_y + 1) >> BLAH
                    uint16_t top_offset_cav2 = elem_count_cav2 + base_offset_cav2;

                    // top_offset is always <2^16 here, so don't need to consider the top
                    if ((!len_64) && (top_offset_cav2 > length)) {
                        if (bits.range_x == 0) {
                            cav2_force_match_cav1 = true;
                        } else {
                            return CCapResult_Decode_InvalidCaveat;
                        }
                    }
                }
            }

            if (!cav2_force_match_cav1) {
                uint64_t base_cav2 = base + base_offset_cav2;
                if (base_cav2 < base) {
                    // overflowed 64 bits
                    return CCapResult_Decode_InvalidCaveat;
                }
                base = base_cav2;
                if (elem_width_log2_cav2 == 0)
                    len_64 = false;
                else
                    len_64 = (((uint64_t)elem_count_cav2) << (elem_width_log2_cav2 - 1)) >> 63;
                length = (((uint64_t)elem_count_cav2) << elem_width_log2_cav2);
            }

            libccap_dbg_trace("libccap_read_bits_range cav2    base %lx len %lx\n", base, length);
        }
    }

    if (base_ptr != NULL) {
        *base_ptr = base;
    }
    if (len_ptr != NULL) {
        *len_ptr = length;
    }
    if (len_64_ptr != NULL) {
        *len_64_ptr = len_64;
    }

    return CCapResult_Success;
}

// INTERNAL
LIBCCAP_USE_RESULT_FUNC_PREFIX
LIBCCAP_INTERNAL_FUNC_PREFIX CCapResult libccap_generate_initial(struct Cap2024_11_Bits* bits, CCapPerms perms, uint64_t base, uint64_t len, uint64_t* actual_base_ptr, uint64_t* actual_len_ptr, bool* actual_len_64_ptr) {
    if (len == 0) {
        return CCapResult_Encode_UnrepresentableBaseRange;
    }
    if (base >= ((uint64_t)1 << 54)) {
        uint64_t new_base = ((uint64_t)1 << 54) - 1;
        len += (base - new_base);
        base = new_base;
    }

    // The Rust encoder has this uncommented. There is no benefit in doing this.
    // It makes the encoder believe you are trying to encode a 2^10 range,
    // which means it prioritizes keeping (base + 2^10) in the initial range.
    // if base is not 2^10 aligned, then the encoded base will be rounded down, so the length will be push up *past 2^10*
    // to keep (base + 2^10) in range
    // even if the original (base + length) we wanted was small.
    // This doesn't make much of a difference - you can reach all of the same values from 2^11 as 2^10 with the 2024_11 caveat setup - but it's still silly.
    // if (len < (1 << 10)) {
    //     len = 1 << 10;
    // }

    uint8_t log_len_minus1 = (len == 1) ? 0 : LIBCCAP_ILOG2(len - 1);
    uint8_t w = ccap2024_length_pow2_to_width_class[log_len_minus1];
    const CCapWidthClass* w_cl = &ccap2024_width_classes[w];

    bool implicit_1 = w_cl->m_top;
    uint8_t elem_width_log2 = w_cl->e;
    uint8_t encoded_elem_width_log2 = (w == 0) ? 31 : (w - 1);
    uint8_t count_bits = w_cl->m_bits;
    uint8_t base_shift = w_cl->b_align;

    // First, see if we round down the base
    uint64_t encoded_base = base >> base_shift;
    uint64_t expected_decoded_base = encoded_base << base_shift;
    uint64_t required_len = len + (base - expected_decoded_base);

    // The initial resource is expressed as (count << elem_width_log2) where count is a positive integer
    uint64_t count = required_len >> elem_width_log2;
    // Round up count if we shifted bits of len out
    if ((count << elem_width_log2) < required_len) {
        count++;
    }

    if (w_cl->max_encodable_pow2 < 64 && (count << elem_width_log2) > ((uint64_t)1 << w_cl->max_encodable_pow2)) {
        // we need to retry with a longer length to compensate.
        // increasing the length might bump us up a count_bits quadrant, thus further shrinking the base_bits,
        // but after that happens once it won't recurse again.

        w = w + 1;
        if (w == 32) {
            libccap_simple_panic("too-large length\n");
        }
        w_cl = &ccap2024_width_classes[w];

        implicit_1 = w_cl->m_top;
        elem_width_log2 = w_cl->e;
        encoded_elem_width_log2 = (w == 0) ? 31 : (w - 1);
        count_bits = w_cl->m_bits;
        base_shift = w_cl->b_align;

        // First, see if we round down the base
        encoded_base = base >> base_shift;
        expected_decoded_base = encoded_base << base_shift;
        required_len = len + (base - expected_decoded_base);

        // The initial resource is expressed as (count << elem_width_log2) where count is a positive integer
        count = required_len >> elem_width_log2;
        // Round up count if we shifted bits of len out
        if ((count << elem_width_log2) < required_len) {
            count++;
        }
    }

    uint64_t encoded_count = count - 1;
    if (implicit_1) {
        // Remove the top bit, it will be implicitly added in the decode
        encoded_count = encoded_count & (~(1 << count_bits));
    }

    libccap_dbg_trace("log %d w %d e %d count %016lx encoded_count %016lx\n", log_len_minus1, w, w_cl->e, count, encoded_count);

    // sanity check
    if (encoded_count >= ((uint64_t)1 << w_cl->m_bits)) {
        libccap_simple_panic("encoded_count too big\n");
        return CCapResult_CatastrophicFailure;
    }
    if (encoded_base >= ((uint64_t)1 << w_cl->b_bits)) {
        libccap_simple_panic("encoded_base too big\n");
        return CCapResult_CatastrophicFailure;
    }


    uint64_t b_c = (encoded_base << count_bits) | encoded_count;

    uint64_t actual_base = expected_decoded_base;
    uint64_t actual_len = count << elem_width_log2; // TODO I think this is exactly equal to re-decoding encoded_count?
    bool actual_len_64 = (count << (elem_width_log2 - 1)) >> 63;

    if (actual_base > base) {
        libccap_simple_panic("Too-large base\n");
        return CCapResult_CatastrophicFailure;
    }
    // intended len_64 is always false, so if we have set actual_len_64 we can guarantee we are long enough.
    if ((actual_len < len || (base - actual_base) > (actual_len - len)) && !actual_len_64) {
        libccap_simple_panic("Not-long-enough len\n");
        return CCapResult_CatastrophicFailure;
    }

    bits->b_c = b_c;
    bits->encoded_elem_width = encoded_elem_width_log2;

    *actual_base_ptr = actual_base;
    *actual_len_ptr = actual_len;
    *actual_len_64_ptr = actual_len_64;

    return CCapResult_Success;
}

// INTERNAL
LIBCCAP_USE_RESULT_FUNC_PREFIX
LIBCCAP_INTERNAL_FUNC_PREFIX CCapResult libccap_generate_caveats(struct Cap2024_11_Bits* bits, CCapPerms perms, uint64_t base, uint64_t len, uint64_t* actual_base_ptr, uint64_t* actual_len_ptr, bool* actual_len_64_ptr) {
    uint64_t actual_base = *actual_base_ptr;
    uint64_t actual_len = *actual_len_ptr;
    bool actual_len_64 = *actual_len_64_ptr;

    if (actual_base != base || actual_len != len) {
        // Add cav1

        // fprintf(stderr, "target base 0x%lx len 0x%lx\n", base, len);
        // fprintf(stderr, "actual base 0x%lx len 0x%lx\n", actual_base, actual_len);


        /*
        // Do a mini-decode of the initial resource to get log_max_count and elem_width_log2
        uint8_t implicit_1 = 1;
        uint8_t log_max_count; // = count_bits + implicit_1;
        uint8_t elem_width_log2;
        uint8_t c_bits_selector = (bits->encoded_elem_width >> 3) & 0b11;
        if ((bits->encoded_elem_width & 0b111) == 0b111) {
            c_bits_selector = (c_bits_selector + 1) & 0b11;
            implicit_1 = 0;
        }
        switch (c_bits_selector) {
            case 0b00:
                log_max_count = 5 + implicit_1;
                break;

            case 0b01:
                log_max_count = 12 + implicit_1;
                break;

            case 0b10:
                log_max_count = 19 + implicit_1;
                break;

            case 0b11:
                log_max_count = 26 + implicit_1;
                break;
        }
        switch ((bits->encoded_elem_width >> 3) & 0b11) {
            case 0b00:
                elem_width_log2 = bits->encoded_elem_width + 10;
                break;
            case 0b01:
                elem_width_log2 = bits->encoded_elem_width + 10 - 1;
                break;
            case 0b10:
                elem_width_log2 = bits->encoded_elem_width + 10 - 2;
                break;
            case 0b11:
                if (bits->encoded_elem_width == 31) {
                    elem_width_log2 = 10;
                } else {
                    elem_width_log2 = bits->encoded_elem_width + 10 - 3;
                }
                break;
        }
        */
        uint8_t w = (bits->encoded_elem_width + 1) & 0b11111;
        const CCapWidthClass* w_cl = &ccap2024_width_classes[w];

        uint8_t max_pow2_of_cav1 = w_cl->max_encodable_pow2;
        uint8_t min_pow2_of_cav1 = (max_pow2_of_cav1 >= 15) ? (max_pow2_of_cav1 - 15) : 0;

        uint64_t intended_base_rel = base - actual_base;
        uint64_t intended_top_rel = intended_base_rel + len;
        uint64_t intended_top_rel_aligned = (intended_top_rel >> min_pow2_of_cav1) << min_pow2_of_cav1;
        if (intended_top_rel_aligned < intended_top_rel) {
            intended_top_rel_aligned += ((uint64_t)1) << min_pow2_of_cav1;
        }
        uint64_t top_rel_rounded_up = intended_top_rel_aligned - 1;

        if (top_rel_rounded_up >= actual_len) {
            bits->index = 0;
            bits->index_size_div = 0;

            libccap_dbg_trace("forced into null cav1; initial w: %d, intended_base_rel: %016lx, top_rel_rounded_up: %016lx, actual_len: %016lx\n", w, intended_base_rel, top_rel_rounded_up, actual_len);
        } else {
            uint64_t b_offset_at_min = intended_base_rel >> min_pow2_of_cav1;
            uint64_t t_offset_at_min = top_rel_rounded_up >> min_pow2_of_cav1;
            uint64_t x = b_offset_at_min ^ t_offset_at_min;
            uint64_t target_shift = 64 - __builtin_clzl(x);

            bits->index = b_offset_at_min >> target_shift;
            bits->index_size_div = 15 - target_shift;
        }

        uint64_t elem_count_mask = ((uint64_t)-1) >> (64 - w_cl->m_bits);
        uint64_t elem_count = ((
            ((uint64_t)w_cl->m_top) << (w_cl->m_bits)
        ) | (
            bits->b_c & elem_count_mask
        )) + 1;
        uint64_t check_lhs = ((uint64_t)(bits->index + 1)) << (w_cl->m_bits + w_cl->m_top);
        uint64_t check_rhs = elem_count << bits->index_size_div;
        uint8_t elem_width_log2_cav1 = w_cl->max_encodable_pow2 - bits->index_size_div;
        uint64_t base_offset_cav1 = ((uint64_t)bits->index) << elem_width_log2_cav1;

        if (check_lhs > check_rhs) {
            // cav1 exceeds the bounds of the initial resource
            if (bits->index == 0) {
                // This is an identity-sized cav1, which uses the initial base and length but can be shrunk further by a cav2
                // identity_cav1 = true;
                libccap_dbg_trace("libccap_generate_caveats cav1    identity base %lx len %lx\n", actual_base, actual_len);
            } else {
                // This is invalid
                return CCapResult_CatastrophicFailure;
            }
        } else {
            uint64_t base_cav1 = actual_base + base_offset_cav1;
            if (base_cav1 < actual_base) {
                // overflowed 64 bits
                return CCapResult_CatastrophicFailure;
            }
            actual_base = base_cav1;
            actual_len_64 = (elem_width_log2_cav1 == 64);
            actual_len = actual_len_64 ? 0 : (1 << elem_width_log2_cav1);

            libccap_dbg_trace("libccap_generate_caveats cav1    base %lx len %lx\n", actual_base, actual_len);
        }

        CCapResult res = libccap_add_perms_caveat_to_cap_perms_chain(bits->perms, perms, &bits->perms);
        if (res != CCapResult_Success) {
            // Should be impossible because the perms should be the same as the initial perms of the root of the chain
            libccap_dbg_trace("failed to extend caveat chain 0x%x with perms %s\n", bits->perms, ccap_perms_str(perms));
            libccap_simple_panic("failed to extend caveat chain\n");
        }

        libccap_dbg_trace("added cav1   index %x size_div %x len_pow2 %d\n", bits->index, bits->index_size_div, max_pow2_of_cav1 - bits->index_size_div);

        // Check we didn't accidentally shrink it too far
        if (actual_base > base) {
            libccap_simple_panic("cav1 Too-large base\n");
            return CCapResult_CatastrophicFailure;
        }
        // intended len_64 is always false, so if we have set actual_len_64 we can guarantee we are long enough.
        if ((actual_len < len || (base - actual_base) > (actual_len - len)) && !actual_len_64) {
            libccap_simple_panic("Not-long-enough len\n");
            return CCapResult_CatastrophicFailure;
        }

        uint64_t cav1_base = actual_base;
        uint64_t cav1_len = actual_len;

        // fprintf(stderr, "cav1   base 0x%lx len 0x%lx\n", actual_base, actual_len);

        if (actual_base != base || actual_len != len) {
            // Add cav2

            uint8_t cav1_pow2 = max_pow2_of_cav1 - bits->index_size_div;

            // fprintf(stderr, "cav1 pow2 %d.. %d ..%d\n", min_pow2_of_cav1, cav1_pow2, max_pow2_of_cav1);

            if (cav1_pow2 >= 14) {
                uint8_t slice_pow2 = cav1_pow2 - 14;


                // cav1_range encompasses base and base + len
                // =>  (base - cav1_range.base())       <  2^cav1_pow2
                // and (base + len - cav1_range.base()) <= 2^cav1_pow2
                // => when shifted down by (cav1_pow2 - 14), they are both <=2^14

                uint64_t top_offset = (base - actual_base) + len;
                uint16_t range_y = (top_offset >> slice_pow2);
                if ((((uint64_t)range_y) << slice_pow2) < top_offset) {
                    range_y++;
                }

                uint64_t len_shifted = len >> slice_pow2;
                if ((len_shifted << slice_pow2) < len) {
                    len_shifted++;
                }

                libccap_dbg_trace("top_offset %016lx slice_pow2 %d range_y %04x\n", top_offset, slice_pow2, range_y);

                if (range_y >= len_shifted) {
                    bits->range_x = 0;
                } else {
                    bits->range_x = (base - actual_base) >> slice_pow2;
                }

                bits->range_y_minus_one = range_y - 1;
                // fprintf(stderr, "cav2   x 0x%lx y 0x%lx\n", bits->range_x, bits->range_y_minus_one+1);
            } else {
                bits->range_x = (base - actual_base) << (14 - cav1_pow2);
                bits->range_y_minus_one = ((base + len - actual_base) << (14 - cav1_pow2)) - 1;
            }

            res = libccap_add_perms_caveat_to_cap_perms_chain(bits->perms, perms, &bits->perms);
            if (res != CCapResult_Success) {
                // Should be impossible because the perms should be the same as the initial perms of the root of the chain
                libccap_dbg_trace("failed to extend caveat chain 0x%x with perms %s\n", bits->perms, ccap_perms_str(perms));
                libccap_simple_panic("failed to extend caveat chain\n");
            }

            libccap_dbg_trace("added cav2   cav1_pow2 %d range_x %x range_y-1 %x\n", cav1_pow2, bits->range_x, bits->range_y_minus_one);

            // if (libccap_read_bits_range(*bits, &actual_base, &actual_len, NULL) != CCapResult_Success) {
            //     libccap_simple_panic("Added invalid cav2");
            //     return CCapResult_CatastrophicFailure;
            // }

            if (cav1_pow2 >= 14) {
                uint8_t slice_pow2 = cav1_pow2 - 14;

                actual_base += ((uint64_t)bits->range_x) << slice_pow2;
                actual_len = (((uint64_t)bits->range_y_minus_one) - ((uint64_t)bits->range_x) + 1) << slice_pow2;
                if ((actual_len - cav1_base + actual_base) > cav1_len && bits->range_x == 0) {
                    actual_len = cav1_len;
                }
            } else {

                uint8_t slice_pow2 = (14 - cav1_pow2);

                actual_base += ((uint64_t)bits->range_x) >> slice_pow2;
                actual_len = (((uint64_t)bits->range_y_minus_one) - ((uint64_t)bits->range_x) + 1) >> slice_pow2;
                if ((actual_len - cav1_base + actual_base) > cav1_len && bits->range_x == 0) {
                    actual_len = cav1_len;
                }
            }

            // fprintf(stderr, "cav2   base 0x%lx len 0x%lx\n", actual_base, actual_len);

            if (actual_base > base) {
                libccap_simple_panic("cav2 Too-large base\n");
                return CCapResult_CatastrophicFailure;
            }
            if (actual_len < len || (base - actual_base) > (actual_len - len)) {
                libccap_dbg_trace("base %016lx len %016lx\nactual_base %016lx actual_len %016lx\n", base, len, actual_base, actual_len);
                libccap_simple_panic("cav2 Not-long-enough len\n");
                return CCapResult_CatastrophicFailure;
            }
            if ((actual_len - cav1_base + actual_base) > cav1_len) {
                libccap_simple_panic("cav2 too-large len\n");
                return CCapResult_CatastrophicFailure;
            }
            if (cav1_base > actual_base) {
                libccap_simple_panic("cav1 too-small base\n");
                return CCapResult_CatastrophicFailure;
            }
        } else {
            bits->range_x = 0;
            bits->range_y_minus_one = 0;
        }
    } else {
        bits->index = 0;
        bits->index_size_div = 0;
        bits->range_x = 0;
        bits->range_y_minus_one = 0;
    }

    *actual_base_ptr = actual_base;
    *actual_len_ptr = actual_len;

    return CCapResult_Success;
}

// INTERNAL
LIBCCAP_INTERNAL_FUNC_PREFIX void libccap_get_expected_signature(const CCapU128* data, const CCapU128* secret, CCapU128* expected_sig) {
    struct Cap2024_11_Bits bits = Cap2024_11_Bits_unpack(data);

    CCapPerms root_perms;
    libccap_cap_perms_chain_at_root(bits.perms, &root_perms);
    struct Cap2024_11_Bits root_bits = bits;
    root_bits.perms = root_perms;
    root_bits.index = 0;
    root_bits.index_size_div = 0;
    root_bits.range_x = 0;
    root_bits.range_y_minus_one = 0;

    CCapU128 root_u128;
    Cap2024_11_Bits_pack(&root_bits, &root_u128);

    // encrypto the root data with the secret, update expected_sig with the root signature
    ccap_aes_encrypt_128_func(secret, &root_u128, expected_sig);

    CCapPerms cav1_perms;
    if (libccap_cap_perms_chain_at_cav1(bits.perms, &cav1_perms)) {
        // We have cav1
        struct Cap2024_11_Bits cav1_bits = bits;
        cav1_bits.perms = cav1_perms;
        cav1_bits.range_x = 0;
        cav1_bits.range_y_minus_one = 0;

        CCapU128 cav1_u128;
        Cap2024_11_Bits_pack(&cav1_bits, &cav1_u128);

        // encrypt the cav1 data with the root signature, update expected_sig with the cav1 signature
        ccap_aes_encrypt_128_func(expected_sig, &cav1_u128, expected_sig);

        CCapPerms cav2_perms;
        if (libccap_cap_perms_chain_at_cav2(bits.perms, &cav2_perms)) {
            // We have cav2
            // encrypt the cav2 data with the cav1 signature, update expected_sig with the cav2 signature
            ccap_aes_encrypt_128_func(expected_sig, data, expected_sig);
        }
    }
}

/**
 * Convert a CCapPerms enum to a null-terminated static immutable C string.
 * Invokes undefined behaviour if passed an value not in the enumeration.
 */
LIBCCAP_EXTERNAL_FUNC_PREFIX const char *ccap_perms_str(CCapPerms perms) {
    switch (perms) {
        case CCapPerms_Read:
            return "CCapPerms_Read";
        case CCapPerms_Write:
            return "CCapPerms_Write";
        case CCapPerms_ReadWrite:
            return "CCapPerms_ReadWrite";
        default:
            return "CCapPerms_Invalid";
    }
}

/**
 * Convert a CCapResult enum to a null-terminated static immutable C string.
 * Invokes undefined behaviour if passed an value not in the enumeration.
 */
LIBCCAP_EXTERNAL_FUNC_PREFIX const char *ccap_result_str(CCapResult res) {
    switch (res) {
        case (CCapResult_Success): return "CCapResult_Success";
        case (CCapResult_Encode_UnrepresentableBaseRange): return "CCapResult_Encode_UnrepresentableBaseRange";
        case (CCapResult_Encode_UnrepresentableCaveat): return "CCapResult_Encode_UnrepresentableCaveat";
        case (CCapResult_Encode_InvalidCaveat): return "CCapResult_Encode_InvalidCaveat";
        case (CCapResult_Encode_NoCaveatsLeft): return "CCapResult_Encode_NoCaveatsLeft";
        case (CCapResult_Encode_CantShrinkPerms): return "CCapResult_Encode_CantShrinkPerms";
        case (CCapResult_Decode_InvalidCaveat): return "CCapResult_Decode_InvalidCaveat";
        case (CCapResult_Decode_InvalidSignature): return "CCapResult_Decode_InvalidSignature";
        case (CCapResult_Decode_InvalidCapPermsChain): return "CCapResult_Decode_InvalidCapPermsChain";
        case (CCapResult_Decode_UnexpectedCaveat): return "CCapResult_Decode_UnexpectedCaveat";
        case (CCapResult_NullRequiredArgs): return "CCapResult_NullRequiredArgs";
        case (CCapResult_Encode_TooBigSecretId): return "CCapResult_Encode_TooBigSecretId";
        case (CCapResult_Encode_InvalidPerms): return "CCapResult_Encode_InvalidPerms";
        case (CCapResult_Decode_NotVirtio): return "CCapResult_Decode_NotVirtio";
        case (CCapResult_CatastrophicFailure): return "CCapResult_CatastrophicFailure";
        default:
            return "CCapResult_Invalid";
    }
}

/**
 * Initialize a capability from scratch allowing access to the full 64-bit address range (base = 0, len = 1<<64), given the permissions (Read|Write|Both), and the secret id.
 * Calculates the capability signature given the packed data and the secret.
 *
 * cap and secret are non-optional, and the function returns `NullRequiredArgs` if either are null.
 *
 * Does not use caveats.
 */
LIBCCAP_USE_RESULT_FUNC_PREFIX
LIBCCAP_EXTERNAL_FUNC_PREFIX CCapResult ccap2024_11_init_almighty(struct CCap2024_11 *cap,
                                     const CCapU128 *secret,
                                     uint32_t secret_id,
                                     CCapPerms perms) {
    struct Cap2024_11_Bits bits = {
        .encoded_elem_width = 31,
        // 26 bits of 1s for c, base = 0 => b = 0
        // {b, c} = 26 bits of 1s
        .b_c = 0x3FFFFFF,
        .index = 0,
        .index_size_div = 0,
        .range_x = 0,
        .range_y_minus_one = 0,
        .secret_key_id = secret_id,
        .perms = libccap_root_perms_chain_for_cap_perms(perms)
    };

    Cap2024_11_Bits_pack(&bits, &cap->data);

    ccap_aes_encrypt_128_func(secret, &cap->data, &cap->signature);

    return CCapResult_Success;
}

/**
 * Initialize a capability from scratch, given the contiguous memory range it grants access to, the permissions (Read|Write|Both), and the secret_id.
 * Only uses the initial resource.
 * Calculates the capability signature given the packed data and the secret.
 *
 * cap and secret are non-optional, and the function returns `NullRequiredArgs` if either are null.
 *
 * Returns an Encode error if the base/length is not exactly representable.
 * Use [ccap$version_init_inexact] to allow rounding the bounds up in this case instead of returning an error.
 *
 * Does not use caveats.
 */
LIBCCAP_USE_RESULT_FUNC_PREFIX
LIBCCAP_EXTERNAL_FUNC_PREFIX CCapResult ccap2024_11_init_exact(struct CCap2024_11 *cap,
                                  const CCapU128 *secret,
                                  uint64_t base,
                                  uint64_t len,
                                  uint32_t secret_id,
                                  CCapPerms perms) {
    uint64_t actual_base;
    uint64_t actual_len;
    bool actual_len_64;

    CCapResult inexact_res = ccap2024_11_init_inexact(cap, secret, base, len, secret_id, perms, &actual_base, &actual_len, &actual_len_64);
    if (inexact_res != CCapResult_Success) {
        return inexact_res;
    }

    // // don't need to check len_64 - this function cannot take arguments that imply the len should be 1 << 64, and if it were upgraded from (len) to (1 << 64) actual_len would be == 0.
    if (actual_base != base || actual_len != len) {
        return CCapResult_Encode_UnrepresentableBaseRange;
    }

    return CCapResult_Success;
}

/**
 * Initialize a capability from scratch, given the contiguous memory range it grants access to, the permissions (Read|Write|Both), and the secret_id.
 * Uses the initial resource and both caveats if necessary.
 * Calculates the capability signature given the packed data and the secret.
 */
LIBCCAP_USE_RESULT_FUNC_PREFIX
LIBCCAP_EXTERNAL_FUNC_PREFIX CCapResult ccap2024_11_init_cavs_exact(struct CCap2024_11 *cap,
                                       const CCapU128 *secret,
                                       uint64_t base,
                                       uint64_t len,
                                       uint32_t secret_id,
                                       CCapPerms perms) {
    if (cap == NULL) {
        return CCapResult_NullRequiredArgs;
    }
    if (secret == NULL) {
        return CCapResult_NullRequiredArgs;
    }

    libccap_dbg_trace("base %lx len %lx\n", base, len);
    bool len_64 = false;

    struct Cap2024_11_Bits bits = {
        .index = 0,
        .index_size_div = 0,
        .range_x = 0,
        .range_y_minus_one = 0,
        .perms = libccap_root_perms_chain_for_cap_perms(perms),
        .secret_key_id = secret_id,
    };
    uint64_t actual_base;
    uint64_t actual_len;
    bool actual_len_64;
    CCapResult res = libccap_generate_initial(&bits, perms, base, len, &actual_base, &actual_len, &actual_len_64);
    if (res != CCapResult_Success) {
        return res;
    }
    libccap_dbg_trace("post-initial actual base %lx actual len %d%016lx perms chain %x\n", actual_base, actual_len_64, actual_len, bits.perms);
    res = libccap_generate_caveats(&bits, perms, base, len, &actual_base, &actual_len, &actual_len_64);
    if (res != CCapResult_Success) {
        return res;
    }
    libccap_dbg_trace("    post-cav actual base %lx actual len %lx perms chain %x\n", actual_base, actual_len, bits.perms);

    Cap2024_11_Bits_pack(&bits, &cap->data);
    libccap_get_expected_signature(&cap->data, secret, &cap->signature);

    // fprintf(stderr, "test: base target/ 0x%lx actual 0x%lx\n      len target 0x%lx actual 0x%lx\n", base, actual_base, len, actual_len);
    if (actual_base != base || actual_len != len || actual_len_64 != len_64) {
        return CCapResult_Encode_UnrepresentableBaseRange;
    }

    return CCapResult_Success;
}

/**
 * Initialize a capability from scratch, given the contiguous memory range it grants access to, the permissions (Read|Write|Both), and the secret_id.
 * Calculates the capability signature given the packed data and the secret.
 *
 * cap and secret are non-optional, and the function returns `NullRequiredArgs` if either are null.
 * Writes into actual_{base,len,len_64}_ptrs if non-NULL and returning Success.
 *
 * Will round the bounds up to the smallest possible value that encloses [base, base+len].
 * If exact bounds are required use [ccap$version_init_exact].
 *
 * Does not use caveats.
 */
LIBCCAP_USE_RESULT_FUNC_PREFIX
LIBCCAP_EXTERNAL_FUNC_PREFIX CCapResult ccap2024_11_init_inexact(struct CCap2024_11 *cap,
                                    const CCapU128 *secret,
                                    uint64_t base,
                                    uint64_t len,
                                    uint32_t secret_id,
                                    CCapPerms perms,

                                    uint64_t *actual_base_ptr,
                                    uint64_t *actual_len_ptr,
                                    bool *actual_len_64_ptr) {
    if (cap == NULL) {
        return CCapResult_NullRequiredArgs;
    }
    if (secret == NULL) {
        return CCapResult_NullRequiredArgs;
    }

    struct Cap2024_11_Bits bits = {
        .index = 0,
        .index_size_div = 0,
        .range_x = 0,
        .range_y_minus_one = 0,
        .perms = libccap_root_perms_chain_for_cap_perms(perms),
        .secret_key_id = secret_id,
    };
    uint64_t actual_base;
    uint64_t actual_len;
    bool actual_len_64;
    CCapResult res = libccap_generate_initial(&bits, perms, base, len, &actual_base, &actual_len, &actual_len_64);

    if (res != CCapResult_Success) {
        return res;
    }

    Cap2024_11_Bits_pack(&bits, &cap->data);
    libccap_get_expected_signature(&cap->data, secret, &cap->signature);

    if (actual_base_ptr != NULL) {
        *actual_base_ptr = actual_base;
    }
    if (actual_len_ptr != NULL) {
        *actual_len_ptr = actual_len;
    }
    if (actual_len_64_ptr != NULL) {
        *actual_len_64_ptr = actual_len_64;
    }
    return CCapResult_Success;
}

/**
 * Check if a capability has a valid signature, assuming it was encrypted with the given secret.
 *
 * cap and secret are non-optional, and the function returns `CCapResult_NullRequiredArgs` if either are null.
 *
 * Returns `CCapResult_Success` if the signature is valid.
 * Returns `CCapResult_DecodeInvalidSignature` if the signature is invalid.
 * Returns other errors if the capability is otherwise malformed.
 */
LIBCCAP_USE_RESULT_FUNC_PREFIX
LIBCCAP_EXTERNAL_FUNC_PREFIX CCapResult ccap2024_11_check_signature(const struct CCap2024_11 *cap,
                                       const CCapU128 *secret) {
    if (cap == NULL) {
        return CCapResult_NullRequiredArgs;
    }
    if (secret == NULL) {
        return CCapResult_NullRequiredArgs;
    }

    CCapU128 expected_sig;
    libccap_get_expected_signature(&cap->data, secret, &expected_sig);

    if (LIBCCAP_MEMCMP(expected_sig, cap->signature, 16) == 0) {
        return CCapResult_Success;
    } else {
        return CCapResult_Decode_InvalidSignature;
    }
}

/**
 * Given a pointer to a capability, read off its base and length.
 * len_64 will be set if the range.len() has the 64th bit set.
 * base, len, and len_64 are optional arguments, and are ignored if null.
 * cap is non-optional, and the function returns `NullRequiredArgs` if null.
 * Returns a Decode error if the capability data is invalid.
 * Doesn't check the capability signature.
 */
LIBCCAP_USE_RESULT_FUNC_PREFIX
LIBCCAP_EXTERNAL_FUNC_PREFIX CCapResult ccap2024_11_read_range(const struct CCap2024_11 *cap,
                                  uint64_t *base_ptr,
                                  uint64_t *len_ptr,
                                  bool *len_64_ptr) {
    if (cap == NULL) {
        return CCapResult_NullRequiredArgs;
    }

    struct Cap2024_11_Bits bits = Cap2024_11_Bits_unpack(&cap->data);

    return libccap_read_bits_range(bits, base_ptr, len_ptr, len_64_ptr);
}

/**
 * Given a pointer to a capability, read off it's permissions (Read, Write, or both).
 * cap and perms are non-optional, and the function returns `NullRequiredArgs` if they're null.
 * Returns a Decode error if the capability permissions field is invalid, but does not check any other part of the capability.
 * Doesn't check the capability signature.
 */
LIBCCAP_USE_RESULT_FUNC_PREFIX
LIBCCAP_EXTERNAL_FUNC_PREFIX CCapResult ccap2024_11_read_perms(const struct CCap2024_11 *cap,
                                  CCapPerms *perms) {
    if (cap == NULL) {
        return CCapResult_NullRequiredArgs;
    }
    if (perms == NULL) {
        return CCapResult_NullRequiredArgs;
    }

    struct Cap2024_11_Bits bits = Cap2024_11_Bits_unpack(&cap->data);
    return libccap_perms_for_cap_perms_chain(bits.perms, perms);
}

/**
 * Given a pointer to a capability, read off the secret-key id it claims to use.
 * cap and secret_id are non-optional, and the function returns `NullRequiredArgs` if they're null.
 * Returns a Decode error if the capability data is invalid.
 * Doesn't check the capability signature.
 */
LIBCCAP_USE_RESULT_FUNC_PREFIX
LIBCCAP_EXTERNAL_FUNC_PREFIX CCapResult ccap2024_11_read_secret_id(const struct CCap2024_11 *cap, uint32_t *secret_id) {
    if (cap == NULL) {
        return CCapResult_NullRequiredArgs;
    }
    if (secret_id == NULL) {
        return CCapResult_NullRequiredArgs;
    }

    struct Cap2024_11_Bits bits = Cap2024_11_Bits_unpack(&cap->data);
    *secret_id = bits.secret_key_id;

    return CCapResult_Success;
}

/**
 * Initialize a capability from scratch, given the contiguous memory range it grants access to, the permissions (Read|Write), and the secret_id.
 * Uses the initial resource and both caveats if necessary.
 * Calculates the capability signature given the packed data and the secret.
 *
 * The memory range, permissions, and some parts of the secret_id are extracted from the virtio_desc.
 *
 * The addr and len fields of virtio_desc dictate the base and len of the capability respectively.
 *
 * The INDIRECT and NEXT virtio flags are packed into the top two bits of the secret_id.
 * The `next` field of the virtio descriptor is packed into the next thirteen bits of the secret_id,
 * leaving only 8 bits for the actual secret_id.
 *
 * |- INDIRECT -|- NEXT -|- next[12:0] -|- key[7:0] -|
 *      [22]       [21]       [20:8]         [7:0]
 *
 * The WRITE virtio flag determines the capability permissions: if it is set, the permissions are `CCapPerms::Write`, else `CCapPerms::Read`.
 *
 * cap, secret, and virtio_desc are non-optional, and the function returns `NullRequiredArgs` if any are null.
 *
 * Returns an Encode error if the base/length is not exactly representable.
 * Use [ccap$version_init_virtio_cavs_inexact] to allow rounding the bounds up in this case instead of returning an error.
 *
 * Returns `Encode_TooBigSecretId` if `virtio_desc.next` does not fit into 13 bits.
 *
 * Uses caveats
 */
LIBCCAP_USE_RESULT_FUNC_PREFIX
LIBCCAP_EXTERNAL_FUNC_PREFIX CCapResult ccap2024_11_init_virtio_cavs_exact(struct CCap2024_11 *cap,
                                              const CCapU128 *secret,
                                              const struct CCapNativeVirtqDesc *virtio_desc,
                                              uint8_t secret_id) {
    if (cap == NULL) {
        return CCapResult_NullRequiredArgs;
    }
    if (secret == NULL) {
        return CCapResult_NullRequiredArgs;
    }
    if (virtio_desc == NULL) {
        return CCapResult_NullRequiredArgs;
    }

    CCapPerms perms = CCapPerms_Read;
    if (virtio_desc->flags & CCAP_VIRTQ_F_WRITE) {
        perms = CCapPerms_Write;
    }

    if (virtio_desc->next >= (1 << 13)) {
        return CCapResult_Encode_TooBigSecretId;
    }

    uint32_t packed_secret_id =
        ((virtio_desc->flags & CCAP_VIRTQ_F_INDIRECT) ? (1 << 22) : 0)
        | ((virtio_desc->flags & CCAP_VIRTQ_F_NEXT) ? (1 << 21) : 0)
        | (((uint32_t)virtio_desc->next) << 8)
        | (((uint32_t)secret_id) << 0);

    uint64_t base = virtio_desc->addr;
    uint64_t len = virtio_desc->len;

    libccap_dbg_trace("base %lx len %lx\n", base, len);

    return ccap2024_11_init_cavs_exact(cap, secret, base, len, packed_secret_id, perms);
}

/**
 * Given a pointer to a capability, extract all data into a virtio-esque descriptor.
 *
 * The INDIRECT and NEXT virtio flags are packed into the top two bits of the secret_id.
 * The `next` field of the virtio descriptor is packed into the next thirteen bits of the secret_id,
 * leaving only 8 bits for the actual secret_id.
 *
 * |- INDIRECT -|- NEXT -|- next[12:0] -|- key[7:0] -|
 *      [22]       [21]       [20:8]         [7:0]
 *
 * The WRITE virtio flag determines the capability permissions: if it is set, the permissions are `CCapPerms::Write`, else `CCapPerms::Read`.
 *
 * cap and virtio_desc are non-optional, and the function returns `NullRequiredArgs` if they're null.
 *
 * Returns a Decode error if the capability data is invalid.
 *
 * Additionally, returns `Decode_NotVirtio` if
 * - the permissions allow both read and write
 * - the length does not fit into a u32
 *
 * In practice this means you should only call this function on capabilities encoded through [$ccap_version_init_virtio_exact], where those invariants are enforced.
 */
LIBCCAP_USE_RESULT_FUNC_PREFIX
LIBCCAP_EXTERNAL_FUNC_PREFIX CCapResult ccap2024_11_read_virtio(const struct CCap2024_11 *cap,
                                   struct CCapNativeVirtqDesc *virtio_desc) {
    if (cap == NULL) {
        return CCapResult_NullRequiredArgs;
    }

    struct Cap2024_11_Bits bits = Cap2024_11_Bits_unpack(&cap->data);

    uint64_t base;
    uint64_t len;
    bool len_64;
    CCapResult res = libccap_read_bits_range(bits, &base, &len, &len_64);
    if (res != CCapResult_Success) {
        return res;
    }

    if (virtio_desc == NULL) {
        return CCapResult_NullRequiredArgs;
    }

    CCapPerms perms;
    res = libccap_perms_for_cap_perms_chain(bits.perms, &perms);
    if (res != CCapResult_Success) {
        return res;
    }
    if (perms == CCapPerms_ReadWrite) {
        return CCapResult_Decode_NotVirtio;
    }

    if ((len > 0xFFFFFFFF) || len_64) {
        // Length exceeds uint32_t
        return CCapResult_Decode_NotVirtio;
    }

    uint16_t flags =
        (((bits.secret_key_id >> 22) & 1) ? CCAP_VIRTQ_F_INDIRECT : 0)
        | (((bits.secret_key_id >> 21) & 1) ? CCAP_VIRTQ_F_NEXT : 0)
        | ((perms == CCapPerms_Write) ? CCAP_VIRTQ_F_WRITE : 0);

    uint16_t next = (bits.secret_key_id >> 8) & 0x1FFF;

    virtio_desc->addr = base;
    virtio_desc->len = len;
    virtio_desc->flags = flags;
    virtio_desc->next = next;

    return CCapResult_Success;
}

/**
 * Given a pointer to a capaility initialized with ccap2024_11_init_virtio_*,
 * read out the encoded 'next' field.
 * Can never fail.
 * DOES NOT CHECK IF cap IS NULL.
 *
 * |- INDIRECT -|- NEXT -|- next[12:0] -|- key[7:0] -|
 *      [22]       [21]       [20:8]         [7:0]
 *
 */
LIBCCAP_EXTERNAL_FUNC_PREFIX uint16_t ccap2024_11_read_virtio_next(const struct CCap2024_11 *cap) {
    struct Cap2024_11_Bits bits = Cap2024_11_Bits_unpack(&cap->data);

    uint16_t next = (bits.secret_key_id >> 8) & 0x1FFF;

    return next;
}

/**
 * Given a pointer to a capability initialized with ccap2024_11_init_virtio_*,
 * read out the INDIRECT and NEXT flags.
 * Can never fail. Does not read out the WRITE flag because that is encoded in the permissions chain,
 * which has one invalid value and thus can fail.
 * DOES NOT CHECK IF cap IS NULL.
 *
 * |- INDIRECT -|- NEXT -|- next[12:0] -|- key[7:0] -|
 *      [22]       [21]       [20:8]         [7:0]
 */
LIBCCAP_EXTERNAL_FUNC_PREFIX uint16_t ccap2024_11_read_virtio_flags_indirect_next(const struct CCap2024_11 *cap) {
    struct Cap2024_11_Bits bits = Cap2024_11_Bits_unpack(&cap->data);

    uint16_t flags =
        (((bits.secret_key_id >> 22) & 1) ? CCAP_VIRTQ_F_INDIRECT : 0)
        | (((bits.secret_key_id >> 21) & 1) ? CCAP_VIRTQ_F_NEXT : 0);

    return flags;
}

/**
 * Given a pointer to a capability, clear the data with 0s and overwrite the data that would encode the 'next' field with the given value.
 * This is useful for systems like FreeBSD which use this field in normal descriptors to store information.
 *
 * cap is non-optional, and the function returns `NullRequiredArgs` if they're null.
 *
 * Returns `Encode_TooBigSecretId` if `next` does not fit into 13 bits.
 */
LIBCCAP_USE_RESULT_FUNC_PREFIX
LIBCCAP_EXTERNAL_FUNC_PREFIX CCapResult ccap2024_11_clear_and_write_virtio_next(struct CCap2024_11* cap, uint16_t next) {
    if (cap == NULL) {
        return CCapResult_NullRequiredArgs;
    }
    if (next >= (1 << 13)) {
        return CCapResult_Encode_TooBigSecretId;
    }

    struct Cap2024_11_Bits bits = {
        .secret_key_id = (((uint32_t)next) << 8),
        0
    };

    Cap2024_11_Bits_pack(&bits, &cap->data);

    return CCapResult_Success;
}


// Debug a cap data
LIBCCAP_EXTERNAL_FUNC_PREFIX void ccap2024_11_debug_bits(struct CCap2024_11* cap) {
    if (cap != NULL) {
        struct Cap2024_11_Bits bits = Cap2024_11_Bits_unpack(&cap->data);
        #pragma unused(bits) // suppress unused
    	// uint8_t encoded_elem_width;
    	// uint64_t b_c;
    	// uint8_t index_size_div;
    	// uint16_t index;
    	// uint16_t range_x;
    	// uint16_t range_y_minus_one;
    	// uint32_t secret_key_id;
    	// uint8_t perms;
        libccap_dbg_trace("ccap {\n\tw_enc: %d\n\tb_c: 0x%016lx\n\tsize_div: %x\n\tidx: %x\n\tx: %x\n\ty_minus_one: %x\n\tkey_id: %x\n\tperms: %x\n}\n",
            bits.encoded_elem_width,
            bits.b_c,
            bits.index_size_div,
            bits.index,
            bits.range_x,
            bits.range_y_minus_one,
            bits.secret_key_id,
            bits.perms
        );
    } else {
        libccap_dbg_trace("ccap <null>");
    }
}
