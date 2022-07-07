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

static inline void append(GByteArray *buf, target_ulong value)
{
    target_ulong tmp = tswapl(value);
    g_byte_array_append(buf, (uint8_t *)&tmp, TARGET_LONG_SIZE);
}

static inline void append_tag(GByteArray *buf, uint8_t tag)
{
    g_byte_array_append(buf, &tag, 1);
}

int gdb_get_capreg(GByteArray *buf, const cap_register_t *cap)
{
    // If the capability has a valid tag bit we must recompress since the
    // pesbt value might not match the current value (csetbounds could have
    // changed the bounds).
    append_tag(buf, cap->cr_tag);
#if CHERI_MEM_OFFSET_METADATA == 0
    append(buf, CAP_cc(compress_mem)(cap));
    append(buf, cap_get_cursor(cap));
#else
    _Static_assert(CHERI_MEM_OFFSET_CURSOR == 0, "");
    append(buf, cap_get_cursor(cap));
    append(buf, CAP_cc(compress_mem)(cap));
#endif
    return CHERI_CAP_SIZE + 1;
}

int gdb_get_general_purpose_capreg(GByteArray *buf, CPUArchState *env,
                                   unsigned regnum)
{
    // No need to decompress:
    target_ulong pesbt_for_mem =
        get_capreg_pesbt(env, regnum) ^ CAP_NULL_XOR_MASK;
    append_tag(buf, get_capreg_tag(env, regnum));
#if CHERI_MEM_OFFSET_METADATA == 0
    append(buf, pesbt_for_mem);
    append(buf, get_capreg_cursor(env, regnum));
#else
    _Static_assert(CHERI_MEM_OFFSET_CURSOR == 0, "");
    append(buf, get_capreg_cursor(env, regnum));
    append(buf, pesbt_for_mem);
#endif
    return CHERI_CAP_SIZE + 1;
}
