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
#include "qemu/osdep.h"
#include "cpu.h"
#include "cheri-helper-utils.h"
#include "exec/cpu-all.h"

static inline void append(GByteArray *buf, uint64_t value)
{
    uint64_t tmp = tswap64(value);
    g_byte_array_append(buf, (uint8_t *)&tmp, 8);
}

int gdb_get_capreg(GByteArray *buf, const cap_register_t *cap)
{
#if CHERI_CAP_SIZE == 16
    // If the capability has a valid tag bit we must recompress since the
    // pesbt value might not match the current value (csetbounds could have
    // changed the bounds).
#if CHERI_MEM_OFFSET_METADATA == 0
    append(buf, cc128_compress_mem(cap));
    append(buf, cap_get_cursor(cap));
#else
    _Static_assert(CHERI_MEM_OFFSET_CURSOR == 0, "");
    append(buf, cap_get_cursor(cap));
    append(buf, cc128_compress_mem(cap));
#endif
#elif CHERI_CAP_SIZE == 32
    inmemory_chericap256 memory_representation;
    compress_256cap(&memory_representation, cap);
    _Static_assert(CHERI_MEM_OFFSET_METADATA == 0, "");
    append(buf, memory_representation.u64s[0]);
    _Static_assert(CHERI_MEM_OFFSET_CURSOR == 8, "");
    append(buf, memory_representation.u64s[1]);
    _Static_assert(CHERI_MEM_OFFSET_BASE == 16, "");
    append(buf, memory_representation.u64s[2]);
    _Static_assert(CHERI_MEM_OFFSET_LENGTH == 24, "");
    append(buf, memory_representation.u64s[3]);
#else
#error "64-bit capabilities not implemented yet"
#endif
    return CHERI_CAP_SIZE;
}

int gdb_get_general_purpose_capreg(GByteArray *buf, CPUArchState *env,
                                   unsigned regnum)
{
#if CHERI_CAP_SIZE == 16 && QEMU_USE_COMPRESSED_CHERI_CAPS
    // No need to decompress:
    uint64_t pesbt_for_mem =
        get_capreg_pesbt(env, regnum) ^ CC128_NULL_XOR_MASK;
#if CHERI_MEM_OFFSET_METADATA == 0
    append(buf, pesbt_for_mem);
    append(buf, get_capreg_cursor(env, regnum));
#else
    _Static_assert(CHERI_MEM_OFFSET_CURSOR == 0, "");
    append(buf, get_capreg_cursor(env, regnum));
    append(buf, pesbt_for_mem);
#endif
    return CHERI_CAP_SIZE;
#else
    return gdb_get_capreg(buf, get_readonly_capreg(env, regnum));
#endif
}
