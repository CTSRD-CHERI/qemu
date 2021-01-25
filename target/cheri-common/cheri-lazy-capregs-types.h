/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2020 Alex Richardson
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
#pragma once
#ifdef TARGET_CHERI

#include "cheri_defs.h"
// This needs to be a separate header so that cpu.h can include it.
// The rest of cheri-lazy-capregs.h depends on including cpu.h

// We store capability registers in their compressed form an decompress
// on demand. To allow fast use of GPRs from TCG we expose the integer part
// and maintain an array of the state for each capability register (integer,
// capability with tag set, or with tag unset).
// As there are only three states, we can store the metadata for all capability
// registers in a single 64-bit value (which is easy to manipulate from TCG).
// We define the state for holding an integer to be zero so that When writing an
// integer to a capability register, can simply perform a bitwise-AND to mark
// the capability register being written as holding an integer value.
//
// Note: Could also use all-ones and perform a bitwise or instead. Not sure
// if this makes a difference for the x86 tcg backend.
typedef enum CapRegState {
    /// This capability register holds an integer value, therefore
    /// the PESBT bits are ignored and assumed to be those of a NULL capability.
    CREG_INTEGER = 0b0,
#if QEMU_USE_COMPRESSED_CHERI_CAPS
    /// This capability register holds a capability with the tag cleared (not decompressed yet)
    CREG_UNTAGGED_CAP = 0b01,
    /// This capability register holds a capability with the tag set (not decompressed yet)
    CREG_TAGGED_CAP = 0b10,
#endif
    /// This capability register holds a fully decompressed capability.
    /// The tag bit can be read from the cap_register_t structure.
    CREG_FULLY_DECOMPRESSED = 0b11
} CapRegState;

static inline const char* cap_reg_state_string(CapRegState state) {
    const char* strings[] = {"Int", "Untagged Cap", "Tagged Cap", "Decompressed"};
    return strings[(int)state];
}

// Cap registers should be padded so they are easier to move.
_Static_assert(sizeof(cap_register_t) == 64, "");
// pesbt should come directly before reg._cr_cursor, so that the two can be moved with a single 128bit vector op.
_Static_assert((offsetof(cap_register_t, cached_pesbt)-offsetof(cap_register_t, _cr_cursor)) == 8, "");

#define NUM_LAZY_CAP_REGS 34

#define SCRATCH_REG_NUM 33

typedef struct GPCapRegs {
    // We cache the decompressed capregs here (to avoid constantly decompressing
    // We use one of the padding fields as
    // values such as $csp which are used frequently)
    // 33 allows us to have an actual 0 register that is none of the others
    // a 34'th is also used as a temporary when side-effect free scratch space is needed
    // These special extra registers are always in state decompressed.
    cap_register_t decompressed[NUM_LAZY_CAP_REGS];

    uint64_t capreg_state; // 32 times CapRegState compressed to one uint64_t
} QEMU_ALIGNED(64) GPCapRegs;
#endif
