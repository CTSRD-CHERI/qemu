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
        switch (perm) {
        case CAP_PERM_EXECUTE:
            cause = CP2Ca_PERM_EXE;
            // qemu_log("CAP Exe VIOLATION: ");
            goto do_exception;
        case CAP_PERM_LOAD:
            cause = CP2Ca_PERM_LD;
            // qemu_log("CAP LD VIOLATION: ");
            goto do_exception;
        case CAP_PERM_STORE:
            cause = CP2Ca_PERM_ST;
            // qemu_log("CAP ST VIOLATION: ");
            goto do_exception;
        default:
            break;
        }
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
