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

// XXXAR: Keep Legacy CP2Ca_* names for now
typedef enum CheriCapExc {
    CapEx_None                          = 0x0,
#define CP2Ca_NONE          0x00    /* None */
    CapEx_LengthViolation               = 0x1,
#define CP2Ca_LENGTH        0x01    /* Length Violation */
    CapEx_TagViolation                  = 0x2,
#define CP2Ca_TAG           0x02    /* Tag Violation */
    CapEx_SealViolation                 = 0x3,
#define CP2Ca_SEAL          0x03    /* Seal Violation */
    CapEx_TypeViolation                 = 0x4,
#define CP2Ca_TYPE          0x04    /* Type Violation */
    CapEx_CallTrap                      = 0x5,
#define CP2Ca_CALL          0x05    /* Call Trap */
    CapEx_ReturnTrap                    = 0x6,
#define CP2Ca_RETURN        0x06    /* Return Trap */
    CapEx_TSSUnderFlow                  = 0x7,
#define CP2Ca_UNDERFLOW     0x07    /* Underflow of trusted system stack */
    CapEx_UserDefViolation              = 0x8,
#define CP2Ca_USRDEFINE     0x08    /* User-defined Permission Violation */
    CapEx_TLBNoStoreCap                 = 0x9,
#define CP2Ca_TLB_STORE     0x09    /* TLB prohibits store capability */
    CapEx_InexactBounds                 = 0xA,
#define CP2Ca_INEXACT       0x0A    /* Bounds cannot be represented exactly */
    CapEx_UnalignedBase                 = 0xB,
// 0x0b-0x0f Reserved
    CapEx_GlobalViolation               = 0x10,
#define CP2Ca_GLOBAL        0x10 /* Global Violation */
    CapEx_PermitExecuteViolation        = 0x11,
#define CP2Ca_PERM_EXE      0x11 /* Permit_Execute Violation */
    CapEx_PermitLoadViolation           = 0x12,
#define CP2Ca_PERM_LD       0x12 /* Permit_Load Violation */
    CapEx_PermitStoreViolation          = 0x13,
#define CP2Ca_PERM_ST       0x13 /* Permit_Store Violation */
    CapEx_PermitLoadCapViolation        = 0x14,
#define CP2Ca_PERM_LD_CAP   0x14 /* Permit_Load_Capability Violation */
    CapEx_PermitStoreCapViolation       = 0x15,
#define CP2Ca_PERM_ST_CAP   0x15 /* Permit_Store_Capability Violation */
    CapEx_PermitStoreLocalCapViolation  = 0x16,
#define CP2Ca_PERM_ST_LC_CAP 0x16 /* Permit_Store_Local_Capability Violation */
    CapEx_PermitSealViolation           = 0x17,
#define CP2Ca_PERM_SEAL     0x17 /* Permit_Seal Violation */
    CapEx_AccessSystemRegsViolation     = 0x18,
#define CP2Ca_ACCESS_SYS_REGS 0x18 /* Access System Registers Violation */
    CapEx_PermitCCallViolation          = 0x19,
#define CP2Ca_PERM_CCALL    0x19 /* Permit_CCall Violation */
    CapEx_AccessCCallIDCViolation       = 0x1A,
#define CP2Ca_ACCESS_CCALL_IDC 0x1a /* Access IDC in a CCall delay slot */
    CapEx_PermitUnsealViolation         = 0x1B,
#define CP2Ca_PERM_UNSEAL   0x1b /* Permit_Unseal violation */
    CapEx_PermitSetCIDViolation         = 0x1C,
// 0x1b-0x1f Reserved
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
     * (1) <ctag> must be set (CP2Ca_TAG Violation).
     * (2) Seal bit must be unset (CP2Ca_SEAL Violation).
     * (3) <perm> permission must be set (CP2Ca_PERM_EXE, CP2Ca_PERM_LD,
     * or CP2Ca_PERM_ST Violation).
     * (4) <addr> must be within bounds (CP2Ca_LENGTH Violation).
     */
    if (!cr->cr_tag) {
        cause = CP2Ca_TAG;
        // qemu_log("CAP Tag VIOLATION: ");
        goto do_exception;
    }
    if (!cap_is_unsealed(cr)) {
        cause = CP2Ca_SEAL;
        // qemu_log("CAP Seal VIOLATION: ");
        goto do_exception;
    }
    if ((cr->cr_perms & perm) != perm) {
        if (perm & CAP_PERM_EXECUTE) {
            cause = CP2Ca_PERM_EXE;
            // qemu_log("CAP Exe VIOLATION: ");
            goto do_exception;
        } else if (perm & CAP_PERM_STORE) {
            cause = CP2Ca_PERM_ST;
            // qemu_log("CAP ST VIOLATION: ");
            goto do_exception;
        } else if (perm & CAP_PERM_LOAD) {
            cause = CP2Ca_PERM_LD;
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
        cause = CP2Ca_LENGTH;
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

static inline GPCapRegs *cheri_get_gpcrs(CPUArchState *env) {
    return &env->active_tc.gpcapregs;
}

static inline void QEMU_NORETURN raise_cheri_exception_impl(
    CPUArchState *env, CheriCapExcCause cause, unsigned regnum, uintptr_t hostpc)
{
    do_raise_c2_exception_impl(env, cause, regnum, hostpc);
}
