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

#include "cheri_defs.h"

// Table 3.3: Capability Exception Codes
typedef enum CheriCapExc {
    CapEx_None                          = 0x0,
    CapEx_LengthViolation               = 0x1,
    CapEx_TagViolation                  = 0x2,
    CapEx_SealViolation                 = 0x3,
    CapEx_TypeViolation                 = 0x4,
    CapEx_CallTrap                      = 0x5,
    CapEx_ReturnTrap                    = 0x6,
    CapEx_TSSUnderFlow                  = 0x7,
    CapEx_UserDefViolation              = 0x8,
    CapEx_TLBNoStoreCap                 = 0x9,
    CapEx_InexactBounds                 = 0xA,
    CapEx_UnalignedBase                 = 0xB,
    CapEx_CapLoadGen                    = 0xC,
    // 0xd - 0xf reserved
    CapEx_GlobalViolation               = 0x10,
    CapEx_PermitExecuteViolation        = 0x11,
    CapEx_PermitLoadViolation           = 0x12,
    CapEx_PermitStoreViolation          = 0x13,
    CapEx_PermitLoadCapViolation        = 0x14,
    CapEx_PermitStoreCapViolation       = 0x15,
    CapEx_PermitStoreLocalCapViolation  = 0x16,
    CapEx_PermitSealViolation           = 0x17,
    CapEx_AccessSystemRegsViolation     = 0x18,
    CapEx_PermitCCallViolation          = 0x19,
    CapEx_AccessCCallIDCViolation       = 0x1A,
    CapEx_PermitUnsealViolation         = 0x1B,
    CapEx_PermitSetCIDViolation         = 0x1C,
    // 0x1d - 0x1f reserved
} CheriCapExcCause;

enum CheriSCR {
    CheriSCR_PCC = 0,
    CheriSCR_DDC = 1,

    CheriSCR_UTCC = 4,
    CheriSCR_UTDC = 5,
    CheriSCR_UScratchC = 6,
    CheriSCR_UEPCC = 7,

    CheriSCR_STCC = 12,
    CheriSCR_STDC = 13,
    CheriSCR_SScratchC = 14,
    CheriSCR_SEPCC = 15,

    CheriSCR_MTCC = 28,
    CheriSCR_MTDC = 29,
    CheriSCR_MScratchC = 30,
    CheriSCR_MEPCC = 31,
};

#define CHERI_EXC_REGNUM_PCC (32 + CheriSCR_PCC)
#define CHERI_EXC_REGNUM_DDC (32 + CheriSCR_DDC)


static inline const cap_register_t *cheri_get_ddc(CPURISCVState *env) {
    return &env->DDC;
}

static inline const cap_register_t *_cheri_get_pcc_unchecked(CPURISCVState *env)
{
    return &env->PCC;
}

static inline bool cheri_in_capmode(CPURISCVState *env) {
    // Note: No need to synchronize the PCC.cursor value from TCG since
    // Every change to capmode will exit the current translation block.
    return (env->PCC.cr_flags & CHERI_FLAG_CAPMODE) == CHERI_FLAG_CAPMODE;
}

static inline GPCapRegs *cheri_get_gpcrs(CPUArchState *env) {
    return &env->gpcapregs;
}

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

static inline unsigned cheri_get_asid(CPUArchState *env) {
    uint16_t ASID = 0; // TODO: implement?
    return ASID;
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
