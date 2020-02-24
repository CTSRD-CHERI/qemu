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

#define CHERI_EXC_REGNUM_DDC 32
#define CHERI_EXC_REGNUM_PCC 0xff


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
} CheriCapExcCause;

static inline void check_cap(CPURISCVState *env, const cap_register_t *cr,
                             uint32_t perm, uint64_t addr, uint16_t regnum,
                             uint32_t len, bool instavail, uintptr_t pc) {
    return; // No checks yet
}

static inline const cap_register_t *cheri_get_ddc(CPURISCVState *env) {
    return &env->DDC;
}

static inline const cap_register_t *cheri_get_pcc(CPURISCVState *env) {
    return &env->PCC;
}

static inline GPCapRegs *cheri_get_gpcrs(CPUArchState *env) {
    return &env->gpcapregs;
}

static inline void QEMU_NORETURN raise_cheri_exception_impl(
    CPUArchState *env, CheriCapExcCause cause, unsigned regnum, uintptr_t hostpc)
{
    // TODO: use cause and regnum
    riscv_raise_exception(env, RISCV_EXCP_CHERI, hostpc);
}

static inline unsigned cheri_get_asid(CPUArchState *env) {
    uint16_t ASID = 0; // TODO: implement?
    return ASID;
}
