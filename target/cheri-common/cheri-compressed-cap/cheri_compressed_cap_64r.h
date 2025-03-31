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

// CHERI compressed capability format for the RISC-V standard

// The following macros are expected to be defined
#define CC_FORMAT_LOWER 64r
#define CC_FORMAT_UPPER 64R
#define CC64R_CAP_SIZE 8
#define CC64R_CAP_BITS 64
#define CC64R_ADDR_WIDTH 32
#define CC64R_LEN_WIDTH 33

#define CC64R_MANTISSA_WIDTH 10
// Max exponent is the largest exponent _required_, not that can be encoded.
#define CC64R_MAX_EXPONENT 24
#define CC64R_CURSOR_MASK 0xFFFFFFFF
#define CC64R_MAX_ADDRESS_PLUS_ONE ((cc64r_length_t)1u << CC64R_ADDR_WIDTH)
#define CC64R_NULL_TOP CC64R_MAX_ADDRESS_PLUS_ONE
#define CC64R_NULL_LENGTH CC64R_MAX_ADDRESS_PLUS_ONE
#define CC64R_MAX_LENGTH CC64R_MAX_ADDRESS_PLUS_ONE
#define CC64R_MAX_TOP CC64R_MAX_ADDRESS_PLUS_ONE
#define CC64R_MAX_ADDR UINT32_MAX

/* Special otypes are allocated upwards from 0 */
#define CC64R_SPECIAL_OTYPE_VAL(val) (val##u)
#define CC64R_SPECIAL_OTYPE_VAL_SIGNED(val) (val##u)
// Levels support depend on Zcherilevels extension. When not supported we have zero level bits.
#define CC64R_MANDATORY_LEVELS 0
// The encoding allows for up to two levels, but the current implementation is limited to one level bit.
#define CC64R_MAX_LEVELS 1

/* Use uint64_t to represent 33 bit length */
typedef uint64_t cc64r_length_t;
typedef int64_t cc64r_offset_t;
typedef uint32_t cc64r_addr_t;
typedef int32_t cc64r_saddr_t;
#include "cheri_compressed_cap_macros.h"
typedef enum _CC_N(Mode) { _CC_N(MODE_CAP) = 0, _CC_N(MODE_INT) = 1 } _CC_N(Mode);

/* ignore ISO C restricts enumerator values to range of 'int' */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
enum {
    _CC_FIELD(SDP, 63, 62),
    _CC_FIELD(AP_M, 61, 57),      // combined architectural permissions and mode
    _CC_FIELD(MODE, 57, 57),      // Only valid if AP_M grant execute (quadrant 1)
    _CC_FIELD(FLAGS, 57, 57),     // TODO: remove this field
    _CC_FIELD(RESERVED1, 56, 56), // Actually the CL field, but reserved for now to match sail
    _CC_FIELD(LEVEL, 56, 56),
    _CC_FIELD(RESERVED0, 55, 53),
    _CC_FIELD(OTYPE, 52, 52),
    _CC_FIELD(EBT, 51, 32),

    _CC_FIELD(EXPONENT_FORMAT, 51, 51),
    _CC_FIELD(LEN_MSB, 50, 50), // Either MSB of exponent or MSB of length depending on EXPONENT_FORMAT.
    // The FIELD_INTERNAL_EXPONENT_SIZE name is currently required by various static assertions.
    _CC_N(FIELD_INTERNAL_EXPONENT_SIZE) = _CC_N(FIELD_EXPONENT_FORMAT_SIZE),
    _CC_FIELD(TOP_ENCODED, 49, 42),
    _CC_FIELD(BOTTOM_ENCODED, 41, 32),

    // Top/bottom offsets depending on INTERNAL_EXPONENT flag:
    // Without internal exponent:
    _CC_FIELD(EXP_ZERO_TOP, 49, 42),
    _CC_FIELD(EXP_ZERO_BOTTOM, 41, 32),
    // With internal exponent:
    _CC_FIELD(EXP_NONZERO_TOP, 49, 44),
    _CC_FIELD(EXPONENT_HIGH_PART, 43, 42),
    _CC_FIELD(EXP_NONZERO_BOTTOM, 41, 34),
    _CC_FIELD(EXPONENT_LOW_PART, 33, 32),
};
#pragma GCC diagnostic pop

#define CC64R_OTYPE_BITS CC64R_FIELD_OTYPE_SIZE
#define CC64R_BOT_WIDTH CC64R_FIELD_EXP_ZERO_BOTTOM_SIZE
#define CC64R_BOT_INTERNAL_EXP_WIDTH CC64R_FIELD_EXP_NONZERO_BOTTOM_SIZE
#define CC64R_EXP_LOW_WIDTH CC64R_FIELD_EXPONENT_LOW_PART_SIZE

#define CC64R_PERM_WRITE (1 << 0)
#define CC64R_PERM_LOAD_MUTABLE (1 << 1)
#define CC64R_PERM_ELEVATE_LEVEL (1 << 2)
#define CC64R_PERM_STORE_LEVEL (1 << 3)
#define CC64R_PERM_LEVEL (1 << 4)
#define CC64R_PERM_CAPABILITY (1 << 5)
// Software permissions start at bit 6
#define CC64R_UPERMS_ALL (0x3) // 2 bits
#define CC64R_UPERMS_SHFT (6)
#define CC64R_PERM_SW_ALL (CC64R_UPERMS_ALL << CC64R_UPERMS_SHFT)
_CC_STATIC_ASSERT_SAME(CC64R_UPERMS_ALL, CC64R_FIELD_SDP_MAX_VALUE);
#define CC64R_PERM_ACCESS_SYS_REGS (1 << 16)
#define CC64R_PERM_EXECUTE (1 << 17)
#define CC64R_PERM_READ (1 << 18)
#define CC64R_PERMS_ALL (0x7003f)

#define CC64R_AP_Q_MASK ((uint8_t)(3 << 3))
#define CC64R_AP_Q0 ((uint8_t)(0 << 3))
#define CC64R_AP_Q1 ((uint8_t)(1 << 3))
#define CC64R_AP_Q2 ((uint8_t)(2 << 3))
#define CC64R_AP_Q3 ((uint8_t)(3 << 3))
// The infinite cap has mode = 1 (if hybrid is supported) and is the first value in quadrant 1 (0x8 or 0x9)
#define CC64R_ENCODED_INFINITE_PERMS()                                                                                 \
    (_CC_ENCODE_FIELD(CC64R_UPERMS_ALL, SDP) | _CC_ENCODE_FIELD((CC64R_AP_Q1 | 1), AP_M))
#define CC64R_PERMS_MASK (CC64R_PERMS_ALL | CC64R_PERM_SW_ALL)

// Currently, only one type (sentry) is defined.
// However, other extensions (e.g. CHERIoT) define additional otypes beyond this.
enum _CC_N(OTypes) {
    CC64R_MAX_REPRESENTABLE_OTYPE = ((1u << CC64R_OTYPE_BITS) - 1u),
    _CC_SPECIAL_OTYPE(OTYPE_UNSEALED, 0),
    _CC_SPECIAL_OTYPE(OTYPE_SENTRY, 1),
    _CC_N(MIN_RESERVED_OTYPE) = _CC_N(OTYPE_UNSEALED),
    _CC_N(MAX_RESERVED_OTYPE) = _CC_N(OTYPE_SENTRY),
};

#define CC64R_LS_SPECIAL_OTYPES(ITEM, ...)                                                                             \
    ITEM(OTYPE_UNSEALED, __VA_ARGS__)                                                                                  \
    ITEM(OTYPE_SENTRY, __VA_ARGS__)

_CC_STATIC_ASSERT_SAME(CC64R_MANTISSA_WIDTH, CC64R_FIELD_EXP_ZERO_BOTTOM_SIZE);

// The RISC-V extension uses an "exponent zero" flag.
#define CC64R_ENCODE_IE(IE) _CC_ENCODE_FIELD(!(IE), EXPONENT_FORMAT)
#define CC64R_EXTRACT_IE(value) (!_CC_EXTRACT_FIELD(value, EXPONENT_FORMAT))
// The exponent bits in memory are subtracted from the max exponent when decoding in the IE case.
#define _CC64R_ENCODE_EXPONENT_RAW(e_enc) _CC_ENCODE_SPLIT_EXPONENT(e_enc) | _CC_ENCODE_FIELD(((e_enc) >> 4), LEN_MSB)
#define _CC64R_EXTRACT_EXPONENT_RAW(pesbt)                                                                             \
    ((_CC_EXTRACT_FIELD(pesbt, LEN_MSB) << 4) | _CC_EXTRACT_SPLIT_EXPONENT(pesbt))
#define CC64R_ENCODE_EXPONENT(E) _CC64R_ENCODE_EXPONENT_RAW(CC64R_MAX_EXPONENT - (E))
#define CC64R_EXTRACT_EXPONENT(pesbt) (CC64R_MAX_EXPONENT - _CC64R_EXTRACT_EXPONENT_RAW(pesbt))
#define CC64R_RESERVED_FIELDS 2
#define CC64R_RESERVED_BITS (CC128R_FIELD_RESERVED0_SIZE + CC128R_FIELD_RESERVED1_SIZE)
#define CC64R_HAS_BASE_TOP_SPECIAL_CASES 1
#define CC64R_USES_V9_CORRECTION_FACTORS 0
#define CC64R_USES_LEN_MSB 1
#define CC64R_MAX_LEVEL_BITS 1 // LVLBITS=2 not supported yet

#include "cheri_compressed_cap_common.h"
#include "cheri_compressed_cap_riscv_common.h"

static inline _cc_mode _cc_N(get_execution_mode)(const _cc_cap_t* cap) {
    _cc_addr_t raw_perms = _CC_EXTRACT_FIELD(cap->cr_pesbt, AP_M);
    // Mode is only encodable quandrant 1 (executable caps), where it is stored as the LSB
    if ((raw_perms & CC64R_AP_Q_MASK) == CC64R_AP_Q1)
        return (_cc_mode)(raw_perms & 1);
    return (_cc_mode)0;
}
static inline bool _cc_N(set_execution_mode)(_cc_cap_t* cap, _cc_mode new_mode) {
    // Mode is only encodable quandrant 1 (executable caps), where it is stored as the LSB
    _cc_addr_t raw_perms = _CC_EXTRACT_FIELD(cap->cr_pesbt, AP_M);
    if ((raw_perms & CC64R_AP_Q_MASK) == CC64R_AP_Q1) {
        _cc_debug_assert(_cc_N(has_permissions)(cap, _CC_N(PERM_EXECUTE)));
        cap->cr_pesbt = _CC_DEPOSIT_FIELD(cap->cr_pesbt, (unsigned)new_mode, MODE);
        return true;
    }
    _cc_debug_assert(!_cc_N(has_permissions)(cap, _CC_N(PERM_EXECUTE)));
    return false;
}

static inline _cc_addr_t _cc_N(get_all_permissions)(const _cc_cap_t* cap) {
    _cc_addr_t raw_perms = _CC_EXTRACT_FIELD(cap->cr_pesbt, AP_M);
    _cc_addr_t res = 0;

    // If levels are not supported the encodings with levels are reserved and we return an invalid result.
    unsigned level_bits = cap->cr_lvbits;
    _cc_api_requirement(level_bits <= _CC_N(MAX_LEVEL_BITS), "Invalid number of level bits");

    switch (raw_perms & CC64R_AP_Q_MASK) {
    case CC64R_AP_Q0: // Non-capability data read/write
        switch (raw_perms & ~CC64R_AP_Q_MASK) {
        case 0: // No permissions
            break;
        case 1: // Data RO
            res |= CC64R_PERM_READ;
            break;
        case 4: // Data WO
            res |= CC64R_PERM_WRITE;
            break;
        case 5: // Data RW
            res |= CC64R_PERM_READ | CC64R_PERM_WRITE;
            break;
        default:
            // Reserved encoding in quadrant 0. All permissions are denied.
            res = 0;
            break;
        }
        break;
    case CC64R_AP_Q1: // Executable capabilities
        // All encodings in this quadrant grant R+X
        res |= CC64R_PERM_READ | CC64R_PERM_EXECUTE;
        // Bit 0 is the mode which can be ignored inside this function
        switch ((raw_perms & ~CC64R_AP_Q_MASK) >> 1) {
        case 0: // Infinite
            res |= CC64R_PERM_WRITE | CC64R_PERM_CAPABILITY | CC64R_PERM_LOAD_MUTABLE | CC64R_PERM_ACCESS_SYS_REGS |
                   CC64R_PERM_ELEVATE_LEVEL | CC64R_PERM_STORE_LEVEL;
            break;
        case 1: // Execute + Data & Cap RO
            res |= CC64R_PERM_CAPABILITY | CC64R_PERM_LOAD_MUTABLE | CC64R_PERM_ELEVATE_LEVEL | CC64R_PERM_STORE_LEVEL;
            break;
        case 2: // Execute + Data & Cap RW
            res |= CC64R_PERM_WRITE | CC64R_PERM_CAPABILITY | CC64R_PERM_LOAD_MUTABLE | CC64R_PERM_ELEVATE_LEVEL |
                   CC64R_PERM_STORE_LEVEL;
            break;
        case 3: // Execute + Data RW
            res |= CC64R_PERM_WRITE;
            break;
        default:
            __builtin_unreachable(); // all values checked.
            break;
        }
        break;
    case CC64R_AP_Q2:                                   // Restricted capability data read/write
        res |= CC64R_PERM_READ | CC64R_PERM_CAPABILITY; // All encodings in this quadrant grant R+C
        switch (raw_perms & ~CC64R_AP_Q_MASK) {
        // 0-2 reserved
        case 3: // Data & Cap R0 (without LM-permission)
            break;
        // 4, 5 reserved for LVLBITS>1, we don't support this yet
        case 6: // Data & Cap RW (with store local, no EL-permission)
            if (level_bits > 0) {
                res |= CC64R_PERM_WRITE | CC64R_PERM_LOAD_MUTABLE | CC64R_PERM_STORE_LEVEL;
            } else {
                res = 0; // reserved encoding if levels are not supported
            }
            break;
        case 7: // Data & Cap RW (no store local, no EL-permission)
            if (level_bits > 0) {
                res |= CC64R_PERM_WRITE | CC64R_PERM_LOAD_MUTABLE;
            } else {
                res = 0; // reserved encoding if levels are not supported
            }
            break;
        default:
            // Reserved encoding in quadrant 2. All permissions are denied.
            res = 0;
            break;
        }
        break;
    case CC64R_AP_Q3: // Capability data read/write
        // All encodings in this quadrant grant R+C+EL+LM
        res |= CC64R_PERM_READ | CC64R_PERM_CAPABILITY | CC64R_PERM_LOAD_MUTABLE | CC64R_PERM_ELEVATE_LEVEL;
        switch (raw_perms & ~CC64R_AP_Q_MASK) {
        // 0-2 reserved
        case 3: // Data & Cap R0
            break;
        // 4, 5 reserved for LVLBITS>1, we don't support this yet
        case 6: // Data & Cap RW (with store local)
            if (level_bits > 0) {
                res |= CC64R_PERM_WRITE | CC64R_PERM_STORE_LEVEL;
            } else {
                res = 0; // reserved encoding if levels are not supported
            }
            break;
        case 7: // Data & Cap RW (no store local)
            res |= CC64R_PERM_WRITE;
            break;
        default:
            // Reserved encoding in quadrant 3. All permissions are denied.
            res = 0;
            break;
        }
        break;
    }

    if (level_bits > 0) {
        if (_CC_EXTRACT_FIELD(cap->cr_pesbt, LEVEL))
            res |= CC64R_PERM_LEVEL;
    } else {
        // Levels extension not supported -> treat as reserved one-bits.
        res |= CC64R_PERM_ELEVATE_LEVEL | CC64R_PERM_STORE_LEVEL | CC64R_PERM_LEVEL;
    }
    res |= _CC_EXTRACT_FIELD(cap->cr_pesbt, SDP) << _CC_N(UPERMS_SHFT); // Add the software permissions
    res |= _CC_N(PERMS_RESERVED_ONES);                                  // Finally include the hardcoded one-bits
    return res;
}

static inline bool _cc_N(set_permissions)(_cc_cap_t* cap, _cc_addr_t permissions) {
    _cc_api_requirement((permissions & (_CC_N(PERMS_MASK) | _CC_N(PERMS_RESERVED_ONES))) == permissions,
                        "invalid permissions");
    uint8_t res = 0;
    unsigned level_bits = cap->cr_lvbits;
    _cc_api_requirement(level_bits <= _CC_N(MAX_LEVEL_BITS), "Invalid number of level bits");
    bool valid = true;
    _cc_addr_t sw_perms = (permissions >> _CC_N(UPERMS_SHFT)) & _CC_N(UPERMS_ALL);
    // TODO: legalize
    const _cc_addr_t maybe_el = level_bits > 0 ? CC64R_PERM_ELEVATE_LEVEL : 0;
    const _cc_addr_t maybe_sl = level_bits > 0 ? CC64R_PERM_STORE_LEVEL : 0;
    if (level_bits == 0) {
        // Levels extension not supported -> clear the level related permissions
        permissions &= ~(CC64R_PERM_STORE_LEVEL | CC64R_PERM_ELEVATE_LEVEL | CC64R_PERM_LEVEL);
    }
    if (permissions & CC64R_PERM_EXECUTE) {
        res |= CC64R_AP_Q1;
        res |= _cc_N(get_execution_mode)(cap); // Mode is encoded as bit 0, keep the existing value
        switch (permissions & (CC64R_PERM_READ | CC64R_PERM_WRITE | CC64R_PERM_CAPABILITY | CC64R_PERM_LOAD_MUTABLE |
                               CC64R_PERM_ACCESS_SYS_REGS)) {
        case CC64R_PERM_READ | CC64R_PERM_WRITE | CC64R_PERM_CAPABILITY | CC64R_PERM_LOAD_MUTABLE |
            CC64R_PERM_ACCESS_SYS_REGS:
            res |= 0;
            break;
        case CC64R_PERM_READ | CC64R_PERM_CAPABILITY | CC64R_PERM_LOAD_MUTABLE: res |= 2; break;
        case CC64R_PERM_READ | CC64R_PERM_WRITE | CC64R_PERM_CAPABILITY | CC64R_PERM_LOAD_MUTABLE: res |= 4; break;
        case CC64R_PERM_READ | CC64R_PERM_WRITE: res |= 6; break;
        default: valid = false;
        }
    } else if (permissions & CC64R_PERM_ACCESS_SYS_REGS) {
        valid = false; // ASR requires execute
    } else if ((permissions & (CC64R_PERM_READ | CC64R_PERM_CAPABILITY | CC64R_PERM_LOAD_MUTABLE | maybe_el)) ==
               (CC64R_PERM_READ | CC64R_PERM_CAPABILITY | CC64R_PERM_LOAD_MUTABLE | maybe_el)) {
        res |= CC64R_AP_Q3;
        switch (permissions & (CC64R_PERM_WRITE | maybe_sl)) {
        // 0-2 are reserved
        case 0: res |= 3; break;
        // 4, 5 reserved for LVLBITS>1, we don't support this yet
        case CC64R_PERM_WRITE | CC64R_PERM_STORE_LEVEL: res |= 6; break;
        case CC64R_PERM_WRITE: res |= 7; break;
        default: valid = false;
        }
    } else if ((permissions & (CC64R_PERM_READ | CC64R_PERM_CAPABILITY | maybe_el)) ==
               (CC64R_PERM_READ | CC64R_PERM_CAPABILITY)) {
        res |= CC64R_AP_Q2;
        switch (permissions & (CC64R_PERM_WRITE | CC64R_PERM_LOAD_MUTABLE | maybe_sl)) {
        // 0-2 are reserved
        case 0: res |= 3; break;
        // 4, 5 reserved for LVLBITS>1, we don't support this yet
        case CC64R_PERM_WRITE | CC64R_PERM_LOAD_MUTABLE | CC64R_PERM_STORE_LEVEL: res |= 6; break;
        case CC64R_PERM_WRITE | CC64R_PERM_LOAD_MUTABLE: res |= 7; break;
        default: valid = false;
        }
    } else {
        res |= CC64R_AP_Q0;
        if (permissions & (CC64R_PERM_CAPABILITY | CC64R_PERM_EXECUTE | CC64R_PERM_ACCESS_SYS_REGS)) {
            valid = false;
        } else {
            switch (permissions & (CC64R_PERM_READ | CC64R_PERM_WRITE)) {
            case 0: break;
            case CC64R_PERM_READ: res |= 1; break;
            // 2, 3 are reserved
            case CC64R_PERM_WRITE: res |= 4; break;
            case CC64R_PERM_READ | CC64R_PERM_WRITE:
                res |= 5;
                break;
                // 6, 7 are reserved
            }
        }
    }

    // TODO: We should warn about invalid permissions here.
    if (!valid) {
        res = 0; // Exact match not found -> set to no permissions
    }
    if (level_bits > 0) {
        unsigned new_level = permissions & CC64R_PERM_LEVEL ? 1 : 0;
        cap->cr_pesbt = _CC_DEPOSIT_FIELD(cap->cr_pesbt, new_level, LEVEL);
    }
    cap->cr_pesbt = _CC_DEPOSIT_FIELD(cap->cr_pesbt, res, AP_M);
    cap->cr_pesbt = _CC_DEPOSIT_FIELD(cap->cr_pesbt, sw_perms, SDP);
    return valid;
}

#undef CC_FORMAT_LOWER
#undef CC_FORMAT_UPPER
