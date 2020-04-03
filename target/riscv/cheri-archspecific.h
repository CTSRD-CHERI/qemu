/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2020 Alex Richardson <Alexander.Richardson@cl.cam.ac.uk>
 * All rights reserved.
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

#include "cpu.h"
#include "cheri-lazy-capregs.h"

extern bool cheri_debugger_on_trap;

static inline void QEMU_NORETURN raise_cheri_exception_impl(
    CPUArchState *env, CheriCapExcCause cause, unsigned regnum,
    bool instavail, uintptr_t hostpc)
{
    env->cap_cause = cause;
    env->cap_index = regnum;
    // Allow drop into debugger on first CHERI trap:
    // FIXME: allow c command to work by adding another boolean flag to skip
    // this breakpoint when GDB asks to continue
    if (cheri_debugger_on_trap)
        riscv_raise_exception(env, EXCP_DEBUG, hostpc);
    riscv_raise_exception(env, RISCV_EXCP_CHERI, hostpc);
}

static inline bool
cheri_tag_prot_clear_or_trap(CPUArchState *env, target_ulong va,
                             int cb, const cap_register_t* cbp,
                             int prot, uintptr_t retpc, target_ulong tag)
{
    // TODO: implement load-inhibit tlb flags
    return tag;
}

static inline void QEMU_NORETURN raise_unaligned_load_exception(
    CPUArchState *env, target_ulong addr, uintptr_t retpc)
{
    env->badaddr = addr;
    riscv_raise_exception(env, RISCV_EXCP_LOAD_ADDR_MIS, retpc);
}

static inline void QEMU_NORETURN raise_unaligned_store_exception(
    CPUArchState *env, target_ulong addr, uintptr_t retpc)
{
    env->badaddr = addr;
    // Note: RISCV_EXCP_STORE_AMO_ADDR_MIS means "Store/AMO address misaligned"
    riscv_raise_exception(env, RISCV_EXCP_STORE_AMO_ADDR_MIS, retpc);

}

static inline bool validate_cjalr_target(CPUArchState *env,
                                         const cap_register_t *new_pcc,
                                         unsigned regnum,
                                         uintptr_t retpc)
{
    target_ulong new_base = cap_get_base(new_pcc);
    target_ulong new_addr = cap_get_cursor(new_pcc);
    unsigned min_base_alignment = riscv_has_ext(env, RVC) ? 2 : 4;
    if (!QEMU_IS_ALIGNED(new_base, min_base_alignment)) {
        raise_cheri_exception_impl(env, CapEx_UnalignedBase, regnum, true, retpc);
    }
    // XXX: Sail only checks bit 1 why not also bit zero? Is it because that is ignored?
    if (!riscv_has_ext(env, RVC) && (new_addr & 0x2)) {
        riscv_raise_exception(env, RISCV_EXCP_INST_ADDR_MIS, retpc);
    }
    return true;
}
