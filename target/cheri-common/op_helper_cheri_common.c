/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2015-2016 Stacey Son <sson@FreeBSD.org>
 * Copyright (c) 2016-2018 Alfredo Mazzinghi <am2419@cl.cam.ac.uk>
 * Copyright (c) 2016-2020 Alex Richardson <Alexander.Richardson@cl.cam.ac.uk>
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
#include "exec/exec-all.h"
#include "exec/helper-proto.h"
#include "exec/memop.h"

#include "cheri-lazy-capregs.h"
#include "cheri_tagmem.h"

#ifndef TARGET_CHERI
#error "This file should only be compiled for CHERI"
#endif

#ifdef __clang__
#pragma clang diagnostic error "-Wdeprecated-declarations"
#else
#pragma GCC diagnostic error "-Wdeprecated-declarations"
#endif
#define CHERI_HELPER_IMPL(name)                                                \
    __attribute__(                                                             \
        (deprecated("Do not call the helper directly, it will crash at "       \
                    "runtime. Call the _impl variant instead"))) helper_##name

#ifndef TARGET_MIPS
static inline /* Currently needed for other helpers */
#endif
    target_ulong
    check_ddc(CPUArchState *env, uint32_t perm, uint64_t ddc_offset,
              uint32_t len, uintptr_t retpc)
{
    const cap_register_t *ddc = cheri_get_ddc(env);
    target_ulong addr = ddc_offset + cap_get_cursor(ddc);
    check_cap(env, ddc, perm, addr, CHERI_EXC_REGNUM_DDC, len,
              /*instavail=*/true, retpc);
    return addr;
}

target_ulong CHERI_HELPER_IMPL(ddc_check_load(CPUArchState *env,
                                              target_ulong offset, MemOp op))
{
    return check_ddc(env, CAP_PERM_LOAD, offset, memop_size(op), GETPC());
}

target_ulong CHERI_HELPER_IMPL(ddc_check_store(CPUArchState *env,
                                               target_ulong offset, MemOp op))
{
    return check_ddc(env, CAP_PERM_STORE, offset, memop_size(op), GETPC());
}

target_ulong CHERI_HELPER_IMPL(ddc_check_rmw(CPUArchState *env,
                                             target_ulong offset, MemOp op))
{
    return check_ddc(env, CAP_PERM_LOAD | CAP_PERM_STORE, offset,
                     memop_size(op), GETPC());
}

target_ulong CHERI_HELPER_IMPL(pcc_check_load(CPUArchState *env,
                                              target_ulong pcc_offset,
                                              MemOp op))
{
    const cap_register_t *pcc = cheri_get_pcc(env);
    target_ulong addr = pcc_offset + cap_get_cursor(pcc);
    check_cap(env, pcc, CAP_PERM_LOAD, addr, CHERI_EXC_REGNUM_PCC,
              memop_size(op), /*instavail=*/true, GETPC());
    return addr;
}

void CHERI_HELPER_IMPL(cheri_invalidate_tags(CPUArchState *env,
                                             target_ulong vaddr, MemOp op))
{
    cheri_tag_invalidate(env, vaddr, memop_size(op), GETPC());
}

/// Implementations of individual instructions start here

/// Two operand inspection instructions:

target_ulong CHERI_HELPER_IMPL(cgetaddr(CPUArchState *env, uint32_t cb))
{
    /*
     * CGetAddr: Move Virtual Address to a General-Purpose Register
     * TODO: could do this directly from TCG now.
     */
    return (target_ulong)get_capreg_cursor(env, cb);
}

target_ulong CHERI_HELPER_IMPL(cgetbase(CPUArchState *env, uint32_t cb))
{
    /*
     * CGetBase: Move Base to a General-Purpose Register.
     */
    return (target_ulong)cap_get_base(get_readonly_capreg(env, cb));
}

target_ulong CHERI_HELPER_IMPL(cgetflags(CPUArchState *env, uint32_t cb))
{
    /*
     * CGetBase: Move Base to a General-Purpose Register.
     */
    return (target_ulong)get_readonly_capreg(env, cb)->cr_flags;
}

target_ulong CHERI_HELPER_IMPL(cgetlen(CPUArchState *env, uint32_t cb))
{
    /*
     * CGetLen: Move Length to a General-Purpose Register.
     *
     * Note: For 128-bit Capabilities we must handle len >= 2^64:
     * cap_get_length64() converts 1 << 64 to UINT64_MAX
     */
    return (target_ulong)cap_get_length64(get_readonly_capreg(env, cb));
}

target_ulong CHERI_HELPER_IMPL(cgetperm(CPUArchState *env, uint32_t cb))
{
    /*
     * CGetPerm: Move Memory Permissions Field to a General-Purpose
     * Register.
     */
    const cap_register_t *cbp = get_readonly_capreg(env, cb);
    cheri_debug_assert((cbp->cr_perms & CAP_PERMS_ALL) == cbp->cr_perms &&
        "Unknown HW perms bits set!");
    cheri_debug_assert((cbp->cr_uperms & CAP_UPERMS_ALL) == cbp->cr_uperms &&
        "Unknown SW perms bits set!");

    return (target_ulong)cbp->cr_perms |
        ((target_ulong)cbp->cr_uperms << CAP_UPERMS_SHFT);
}

target_ulong CHERI_HELPER_IMPL(cgetoffset(CPUArchState *env, uint32_t cb))
{
    /*
     * CGetOffset: Move Offset to a General-Purpose Register
     */
    return (target_ulong)cap_get_offset(get_readonly_capreg(env, cb));
}


target_ulong CHERI_HELPER_IMPL(cgetsealed(CPUArchState *env, uint32_t cb))
{
    /*
     * CGetSealed: Move sealed bit to a General-Purpose Register
     */
    const cap_register_t* cbp = get_readonly_capreg(env, cb);
    if (cap_is_sealed_with_type(cbp) || cap_is_sealed_entry(cbp))
        return (target_ulong)1;
    assert(cap_is_unsealed(cbp) && "Unknown reserved otype?");
    return (target_ulong)0;
}

target_ulong CHERI_HELPER_IMPL(cgettag(CPUArchState *env, uint32_t cb))
{
    /*
     * CGetTag: Move Tag to a General-Purpose Register
     */
    return (target_ulong)get_capreg_tag(env, cb);
}

target_ulong CHERI_HELPER_IMPL(cgettype(CPUArchState *env, uint32_t cb))
{
    /*
     * CGetType: Move Object Type Field to a General-Purpose Register.
     */
    const cap_register_t *cbp = get_readonly_capreg(env, cb);
    const int64_t otype = cap_get_otype(cbp);
    // Must be either a valid positive type < maximum or one of the special
    // hardware-interpreted otypes
    if (otype < 0) {
        cheri_debug_assert(otype <= CAP_FIRST_SPECIAL_OTYPE_SIGNED);
        cheri_debug_assert(otype >= CAP_LAST_SPECIAL_OTYPE_SIGNED);
    } else {
        cheri_debug_assert(otype <= CAP_LAST_NONRESERVED_OTYPE);
    }
    return otype;
}

/// Two operands (both capabilities)

void CHERI_HELPER_IMPL(ccleartag(CPUArchState *env, uint32_t cd, uint32_t cb))
{
    /*
     * CClearTag: Clear the tag bit
     */
    // TODO: could do this without decompressing.
    const cap_register_t *cbp = get_readonly_capreg(env, cb);
    cap_register_t result = *cbp;
    result.cr_tag = 0;
    update_capreg(env, cd, &result);
}

void CHERI_HELPER_IMPL(cmove(CPUArchState *env, uint32_t cd, uint32_t cb))
{
    /*
     * CMove: Move Capability to another Register
     */
    // TODO: could do this without decompressing.
    const cap_register_t *cbp = get_readonly_capreg(env, cb);
    update_capreg(env, cd, cbp);
}
