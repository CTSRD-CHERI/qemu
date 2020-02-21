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
#include "cheri-lazy-capregs.h"
#include "exec/cpu-all.h"

int gdb_get_capreg(uint8_t *mem_buf, const cap_register_t *cap)
{
#if CHERI_CAP_SIZE == 16
    // If the capability has a valid tag bit we must recompress since the
    // pesbt value might not match the current value (csetbounds could have
    // changed the bounds).
    stq_p(mem_buf + CHERI_MEM_OFFSET_METADATA, compress_128cap(cap));
    stq_p(mem_buf + CHERI_MEM_OFFSET_CURSOR, cap_get_cursor(cap));
#elif CHERI_CAP_SIZE == 32
    inmemory_chericap256 memory_representation;
    compress_256cap(&memory_representation, cap);
    stq_p(mem_buf + CHERI_MEM_OFFSET_METADATA, memory_representation.u64s[0]);
    stq_p(mem_buf + CHERI_MEM_OFFSET_CURSOR, memory_representation.u64s[1]);
    stq_p(mem_buf + CHERI_MEM_OFFSET_BASE, memory_representation.u64s[2]);
    stq_p(mem_buf + CHERI_MEM_OFFSET_LENGTH, memory_representation.u64s[3]);
#else
#error "64-bit capabilities not implemented yet"
#endif
    return CHERI_CAP_SIZE;
}

int gdb_get_general_purpose_capreg(uint8_t *mem_buf, CPUArchState *env,
                                   unsigned regnum)
{
#if CHERI_CAP_SIZE == 16
    // No need to decompress:
    stq_p(mem_buf + CHERI_MEM_OFFSET_METADATA, get_capreg_pesbt(env, regnum));
    stq_p(mem_buf + CHERI_MEM_OFFSET_CURSOR, get_capreg_cursor(env, regnum));
    return CHERI_CAP_SIZE;
#else
    return gdb_get_capreg(mem_buf, get_readonly_capreg(env, regnum))
#endif
}
