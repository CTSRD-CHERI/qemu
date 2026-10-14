/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2018-2025 Alex Richardson
 * Copyright (c) 2024 Codasip
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

// Legacy 128-bit CHERI compressed capability format for the RISC-V standard v0.9.3-v0.9.8

// The following macros are expected to be defined
#define CC_FORMAT_LOWER 128r093
#define CC_FORMAT_UPPER 128R093
#define CC128R093_CAP_SIZE 16
#define CC128R093_CAP_BITS 128
#define CC128R093_ADDR_WIDTH 64
#define CC128R093_LEN_WIDTH 65

#define CC128R093_MANTISSA_WIDTH 14
// Max exponent is the largest exponent _required_, not that can be encoded.
#define CC128R093_MAX_EXPONENT 52
#define CC128R093_CURSOR_MASK 0xFFFFFFFFFFFFFFFF
#define CC128R093_MAX_ADDRESS_PLUS_ONE ((cc128r093_length_t)1u << CC128R093_ADDR_WIDTH)
#define CC128R093_NULL_TOP CC128R093_MAX_ADDRESS_PLUS_ONE
#define CC128R093_NULL_LENGTH CC128R093_MAX_ADDRESS_PLUS_ONE
#define CC128R093_MAX_LENGTH CC128R093_MAX_ADDRESS_PLUS_ONE
#define CC128R093_MAX_TOP CC128R093_MAX_ADDRESS_PLUS_ONE
#define CC128R093_MAX_ADDR UINT64_MAX
/* Special otypes are allocated upwards from 0 */
#define CC128R093_SPECIAL_OTYPE_VAL(val) (val##u)
#define CC128R093_SPECIAL_OTYPE_VAL_SIGNED(val) (val##u)
// Levels support depend on Zcherilevels extension. When not supported we have zero level bits.
#define CC128R093_MANDATORY_LEVEL_BITS 0
// The encoding allows for many levels, but the current implementation is limited to one level bit.
#define CC128R093_MAX_LEVEL_BITS 1

#include "cheri_compressed_cap_macros.h"
typedef enum _CC_N(Mode) { _CC_N(MODE_CAP) = 0, _CC_N(MODE_INT) = 1 } _CC_N(Mode);

/* ignore ISO C restricts enumerator values to range of 'int' */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
enum {
    _CC_FIELD(RESERVED1, 127, 121),
    _CC_FIELD(SDP, 120, 117),
    _CC_FIELD(AP_M, 116, 108), // Combined architectural permissions and mode
    _CC_FIELD(MODE, 116, 116),
    _CC_FIELD(AP, 115, 108),
    _CC_FIELD(LEVEL, 107, 107),
    _CC_FIELD(RESERVED0, 106, 92),
    _CC_FIELD(OTYPE, 91, 91),
    _CC_FIELD(EBT, 90, 64),

    _CC_FIELD(EXPONENT_FORMAT, 90, 90),
    // The FIELD_INTERNAL_EXPONENT_SIZE name is currently required by various static assertions.
    _CC_N(FIELD_INTERNAL_EXPONENT_SIZE) = _CC_N(FIELD_EXPONENT_FORMAT_SIZE),

    // Top/bottom offsets depending on INTERNAL_EXPONENT flag:
    // Without internal exponent:
    _CC_FIELD(EXP_ZERO_TOP, 89, 78),
    _CC_FIELD(EXP_ZERO_BOTTOM, 77, 64),
    // With internal exponent:
    _CC_FIELD(EXP_NONZERO_TOP, 89, 81),
    _CC_FIELD(EXPONENT_HIGH_PART, 80, 78),
    _CC_FIELD(EXP_NONZERO_BOTTOM, 77, 67),
    _CC_FIELD(EXPONENT_LOW_PART, 66, 64),
};
#pragma GCC diagnostic pop

#define CC128R093_OTYPE_BITS CC128R093_FIELD_OTYPE_SIZE

#define CC128R093_PERM_WRITE (1 << 0)
#define CC128R093_PERM_LOAD_MUTABLE (1 << 1)
#define CC128R093_PERM_ELEVATE_LEVEL (1 << 2)
#define CC128R093_PERM_STORE_LEVEL (1 << 3)
#define CC128R093_PERM_LEVEL (1 << 4)
#define CC128R093_PERM_CAPABILITY (1 << 5)
// Software permissions start at bit 6
#define CC128R093_UPERMS_ALL (0xf) /* 4 bits */
#define CC128R093_UPERMS_SHFT (6)
#define CC128R093_PERM_SW_ALL (CC128R093_UPERMS_ALL << CC128R093_UPERMS_SHFT)
#define CC128R093_PERM_ACCESS_SYS_REGS (1 << 16)
#define CC128R093_PERM_EXECUTE (1 << 17)
#define CC128R093_PERM_READ (1 << 18)
#define CC128R093_PERMS_ALL (0x7003f)

_CC_STATIC_ASSERT_SAME(CC128R093_UPERMS_ALL, CC128R093_FIELD_SDP_MAX_VALUE);
// Encoded value is 0b00111111 since SL and EL are not supported in sail yet.
#define CC128R093_ENCODED_INFINITE_PERMS(lvbits)                                                                       \
    (_CC_ENCODE_FIELD(CC128R093_UPERMS_ALL, SDP) | _CC_ENCODE_FIELD(lvbits == 0 ? 0x3f : 0xff, AP) |                   \
     _CC_ENCODE_FIELD(_CC_BITMASK64(lvbits), LEVEL) | _CC_ENCODE_FIELD(1, MODE))
#define CC128R093_PERMS_MASK (CC128R093_PERMS_ALL | CC128R093_PERM_SW_ALL)

// Currently, only one type (sentry) is defined.
// However, other extensions (e.g. CHERIoT) define additional otypes beyond this.
enum _CC_N(OTypes) {
    CC128R093_MAX_REPRESENTABLE_OTYPE = ((1u << CC128R093_OTYPE_BITS) - 1u),
    _CC_SPECIAL_OTYPE(OTYPE_UNSEALED, 0),
    _CC_SPECIAL_OTYPE(OTYPE_SENTRY, 1),
    _CC_N(MIN_RESERVED_OTYPE) = _CC_N(OTYPE_UNSEALED),
    _CC_N(MAX_RESERVED_OTYPE) = _CC_N(OTYPE_SENTRY),
};

#define CC128R093_LS_SPECIAL_OTYPES(ITEM, ...)                                                                         \
    ITEM(OTYPE_UNSEALED, __VA_ARGS__)                                                                                  \
    ITEM(OTYPE_SENTRY, __VA_ARGS__)

// The RISC-V extension uses an "exponent zero" flag.
#define CC128R093_ENCODE_IE(IE) _CC_ENCODE_FIELD(!(IE), EXPONENT_FORMAT)
#define CC128R093_EXTRACT_IE(value) (!_CC_EXTRACT_FIELD(value, EXPONENT_FORMAT))
// The exponent bits in memory are subtracted from the max exponent when decoding in the IE case.
#define CC128R093_ENCODE_EXPONENT(E) _CC_ENCODE_SPLIT_EXPONENT(CC128R093_MAX_EXPONENT - (E))
#define CC128R093_EXTRACT_EXPONENT(pesbt) (CC128R093_MAX_EXPONENT - _CC_EXTRACT_SPLIT_EXPONENT(pesbt))
#define CC128R093_RESERVED_BITS (CC128R093_FIELD_RESERVED0_SIZE + CC128R093_FIELD_RESERVED1_SIZE)
#define CC128R093_HAS_BASE_TOP_SPECIAL_CASES 1
#define CC128R093_USES_V9_CORRECTION_FACTORS 0
#define CC128R093_USES_LEN_MSB 0

#include "cheri_compressed_cap_common.h"
#include "cheri_compressed_cap_riscv_common.h"

// The 64-bit format uses one bit per permission but we have to move them to the correct offset in the final result.
static inline _cc_addr_t _cc_N(get_all_permissions)(const _cc_cap_t* cap) {
    _cc_addr_t sw_perms = _CC_EXTRACT_FIELD(cap->cr_pesbt, SDP);
    _cc_addr_t arch_perms = _CC_EXTRACT_FIELD(cap->cr_pesbt, AP);
    _cc_addr_t result = sw_perms << _CC_N(UPERMS_SHFT);
    // See "Encoding of architectural permissions for MXLEN=64" in the spec
    if (arch_perms & _CC_BIT64(0))
        result |= CC128R093_PERM_CAPABILITY;
    if (arch_perms & _CC_BIT64(1))
        result |= CC128R093_PERM_WRITE;
    if (arch_perms & _CC_BIT64(2))
        result |= CC128R093_PERM_READ;
    if (arch_perms & _CC_BIT64(3))
        result |= CC128R093_PERM_EXECUTE;
    if (arch_perms & _CC_BIT64(4))
        result |= CC128R093_PERM_ACCESS_SYS_REGS;
    if (arch_perms & _CC_BIT64(5))
        result |= CC128R093_PERM_LOAD_MUTABLE;
    if (cap->cr_lvbits > 0) {
        if (arch_perms & _CC_BIT64(6))
            result |= CC128R093_PERM_ELEVATE_LEVEL;
        if (arch_perms & _CC_BIT64(7))
            result |= CC128R093_PERM_STORE_LEVEL;
        if (_CC_EXTRACT_FIELD(cap->cr_pesbt, LEVEL))
            result |= CC128R093_PERM_LEVEL;
    } else {
        // Levels extension not supported -> treat as reserved one-bits.
        result |= CC128R093_PERM_LEVEL | CC128R093_PERM_STORE_LEVEL | CC128R093_PERM_ELEVATE_LEVEL;
    }
    result |= _CC_N(PERMS_RESERVED_ONES); // Finally include the hardcoded one-bits
    return result;
}

static inline bool _cc_N(set_permissions)(_cc_cap_t* cap, _cc_addr_t permissions) {
    _cc_api_requirement((permissions & (_CC_N(PERMS_MASK) | _CC_N(PERMS_RESERVED_ONES))) == permissions,
                        "invalid permissions");
    // TODO: legalize the remaining permission dependencies or reject invalid requests
    _cc_addr_t sw_perms = (permissions >> _CC_N(UPERMS_SHFT)) & _CC_N(UPERMS_ALL);
    _cc_mode mode = (_cc_mode)_CC_EXTRACT_FIELD(cap->cr_pesbt, MODE);
    // See "Encoding of architectural permissions for MXLEN=64" in the spec
    _cc_addr_t result = 0;
    if (permissions & CC128R093_PERM_CAPABILITY)
        result |= _CC_BIT64(0);
    if (permissions & CC128R093_PERM_WRITE)
        result |= _CC_BIT64(1);
    if (permissions & CC128R093_PERM_READ)
        result |= _CC_BIT64(2);
    if (permissions & CC128R093_PERM_EXECUTE)
        result |= _CC_BIT64(3);
    if (permissions & CC128R093_PERM_ACCESS_SYS_REGS)
        result |= _CC_BIT64(4);
    if (permissions & CC128R093_PERM_LOAD_MUTABLE)
        result |= _CC_BIT64(5);
    if (cap->cr_lvbits > 0) {
        if (permissions & CC128R093_PERM_ELEVATE_LEVEL)
            result |= _CC_BIT64(6);
        if (permissions & CC128R093_PERM_STORE_LEVEL)
            result |= _CC_BIT64(7);
        unsigned new_level = permissions & CC128R093_PERM_LEVEL ? 1 : 0;
        cap->cr_pesbt = _CC_DEPOSIT_FIELD(cap->cr_pesbt, new_level, LEVEL);
    }
    cap->cr_pesbt = _CC_DEPOSIT_FIELD(cap->cr_pesbt, result, AP);
    cap->cr_pesbt = _CC_DEPOSIT_FIELD(cap->cr_pesbt, sw_perms, SDP);
    if ((permissions & CC128R093_PERM_EXECUTE) == 0 && mode == _CC_N(MODE_INT)) {
        cap->cr_pesbt = _CC_DEPOSIT_FIELD(cap->cr_pesbt, (unsigned)_CC_N(MODE_CAP), MODE);
        return false;
    }
    return true;
}

static inline _cc_mode _cc_N(get_execution_mode)(const _cc_cap_t* cap) {
    return (_cc_mode)_CC_EXTRACT_FIELD(cap->cr_pesbt, MODE);
}

static inline bool _cc_N(set_execution_mode)(_cc_cap_t* cap, _cc_mode new_mode) {
    // While mode could always be set, the spec requires execute permission.
    if (!_cc_N(has_permissions)(cap, _CC_N(PERM_EXECUTE)))
        return false;
    cap->cr_pesbt = _CC_DEPOSIT_FIELD(cap->cr_pesbt, (unsigned)new_mode, MODE);
    return true;
}

#undef CC_FORMAT_LOWER
#undef CC_FORMAT_UPPER
