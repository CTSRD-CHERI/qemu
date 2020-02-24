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
#pragma once

#include "internal.h"
#include "cheri_defs.h"

#define CHERI_EXC_REGNUM_DDC 0 /* TODO: 32 */
#define CHERI_EXC_REGNUM_PCC 0xff

typedef enum CheriCapExc {
    CapEx_None                          = 0x0,  /* None */
    CapEx_LengthViolation               = 0x1,  /* Length Violation */
    CapEx_TagViolation                  = 0x2,  /* Tag Violation */
    CapEx_SealViolation                 = 0x3,  /* Seal Violation */
    CapEx_TypeViolation                 = 0x4,  /* Type Violation */
    CapEx_CallTrap                      = 0x5,  /* Call Trap */
    CapEx_ReturnTrap                    = 0x6,  /* Return Trap */
    CapEx_TSSUnderFlow                  = 0x7,  /* Underflow of trusted system stack */
    CapEx_UserDefViolation              = 0x8,  /* User-defined Permission Violation */
    CapEx_TLBNoStoreCap                 = 0x9,  /* TLB prohibits store capability */
    CapEx_InexactBounds                 = 0xA,  /* Bounds cannot be represented exactly */
    CapEx_UnalignedBase                 = 0xB,
    // 0x0C-0x0F Reserved
    CapEx_GlobalViolation               = 0x10,  /* Global Violation */
    CapEx_PermitExecuteViolation        = 0x11,  /* Permit_Execute Violation */
    CapEx_PermitLoadViolation           = 0x12,  /* Permit_Load Violation */
    CapEx_PermitStoreViolation          = 0x13,  /* Permit_Store Violation */
    CapEx_PermitLoadCapViolation        = 0x14,  /* Permit_Load_Capability Violation */
    CapEx_PermitStoreCapViolation       = 0x15,  /* Permit_Store_Capability Violation */
    CapEx_PermitStoreLocalCapViolation  = 0x16,  /* Permit_Store_Local_Capability Violation */
    CapEx_PermitSealViolation           = 0x17,  /* Permit_Seal Violation */
    CapEx_AccessSystemRegsViolation     = 0x18,  /* Access System Registers Violation */
    CapEx_PermitCCallViolation          = 0x19,  /* Permit_CCall Violation */
    CapEx_AccessCCallIDCViolation       = 0x1A,  /* Access IDC in a CCall delay slot */
    CapEx_PermitUnsealViolation         = 0x1B,  /* Permit_Unseal violation */
    CapEx_PermitSetCIDViolation         = 0x1C,  /* Permit_SetCID violation */
    // 0x1d-0x1f Reserved
} CheriCapExcCause;

target_ulong check_ddc(CPUMIPSState *env, uint32_t perm, uint64_t addr, uint32_t len, uintptr_t retpc);

static inline void check_cap(CPUMIPSState *env, const cap_register_t *cr,
                             uint32_t perm, uint64_t addr, uint16_t regnum,
                             uint32_t len, bool instavail, uintptr_t pc) {
    uint16_t cause;
    /*
     * See section 5.6 in CHERI Architecture.
     *
     * Capability checks (in order of priority):
     * (1) <ctag> must be set (CapEx_TagViolation Violation).
     * (2) Seal bit must be unset (CapEx_SealViolation Violation).
     * (3) <perm> permission must be set (CapEx_PermitExecuteViolation, CapEx_PermitLoadViolation,
     * or CapEx_PermitStoreViolation Violation).
     * (4) <addr> must be within bounds (CapEx_LengthViolation Violation).
     */
    if (!cr->cr_tag) {
        cause = CapEx_TagViolation;
        // qemu_log("CAP Tag VIOLATION: ");
        goto do_exception;
    }
    if (!cap_is_unsealed(cr)) {
        cause = CapEx_SealViolation;
        // qemu_log("CAP Seal VIOLATION: ");
        goto do_exception;
    }
    if ((cr->cr_perms & perm) != perm) {
        if (perm & CAP_PERM_EXECUTE) {
            cause = CapEx_PermitExecuteViolation;
            // qemu_log("CAP Exe VIOLATION: ");
            goto do_exception;
        } else if (perm & CAP_PERM_STORE) {
            cause = CapEx_PermitStoreViolation;
            // qemu_log("CAP ST VIOLATION: ");
            goto do_exception;
        } else if (perm & CAP_PERM_LOAD) {
            cause = CapEx_PermitLoadViolation;
            // qemu_log("CAP LD VIOLATION: ");
            goto do_exception;
        }
        // Multiple missing permissions:
        error_report("Bad permissions check %d", perm);
        tcg_abort();
    }
    // fprintf(stderr, "addr=%zx, len=%zd, cr_base=%zx, cr_len=%zd\n",
    //     (size_t)addr, (size_t)len, (size_t)cr->cr_base, (size_t)cr->cr_length);
    if (!cap_is_in_bounds(cr, addr, len)) {
        cause = CapEx_LengthViolation;
        // qemu_log("CAP Len VIOLATION: ");
        goto do_exception;
    }

    return;

    do_exception:
    env->CP0_BadVAddr = addr;
    if (!instavail)
        env->error_code |= EXCP_INST_NOTAVAIL;
    do_raise_c2_exception_impl(env, cause, regnum, pc);
}

static inline const cap_register_t *cheri_get_ddc(CPUMIPSState *env) {
    return &env->active_tc.CHWR.DDC;
}

static inline const cap_register_t *cheri_get_pcc(CPUMIPSState *env) {
    return &env->active_tc.PCC;
}

static inline unsigned cheri_get_asid(CPUMIPSState *env) {
    uint16_t ASID = env->CP0_EntryHi & env->CP0_EntryHi_ASID_mask;
    return ASID;
}

static inline GPCapRegs *cheri_get_gpcrs(CPUArchState *env) {
    return &env->active_tc.gpcapregs;
}

static inline void QEMU_NORETURN raise_cheri_exception_impl(
    CPUArchState *env, CheriCapExcCause cause, unsigned regnum, uintptr_t hostpc)
{
    do_raise_c2_exception_impl(env, cause, regnum, hostpc);
}
