/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2018-2020 Alex Richardson
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
#include "cheri_defs.h"
#include "cheri_utils.h"
#include "cheri-lazy-capregs.h"
#include "cheri-bounds-stats.h"

static inline void derive_cap_from_pcc(CPUArchState *env, uint32_t cd,
                                       target_ulong new_addr, uintptr_t retpc,
                                       struct oob_stats_info *oob_info)
{
#ifdef DO_CHERI_STATISTICS
    oob_info->num_uses++;
#else
    (void)oob_info;
#endif
    const cap_register_t *pccp = cheri_get_pcc(env);
    cap_register_t result = *pccp;
    if (!is_representable_cap_with_addr(pccp, new_addr)) {
        if (pccp->cr_tag)
            became_unrepresentable(env, cd, oob_info, retpc);
        cap_mark_unrepresentable(new_addr, &result);
    } else {
        result._cr_cursor = new_addr;
        check_out_of_bounds_stat(env, oob_info, &result);
    }
    update_capreg(env, cd, &result);
}

static inline void check_cap(CPUArchState *env, const cap_register_t *cr,
                             uint32_t perm, uint64_t addr, uint16_t regnum,
                             uint32_t len, bool instavail, uintptr_t pc)
{
    CheriCapExcCause cause;
    /*
     * See section 5.6 in CHERI Architecture.
     *
     * Capability checks (in order of priority):
     * (1) <ctag> must be set (CapEx_TagViolation Violation).
     * (2) Seal bit must be unset (CapEx_SealViolation Violation).
     * (3) <perm> permission must be set (CapEx_PermitExecuteViolation,
     * CapEx_PermitLoadViolation, or CapEx_PermitStoreViolation Violation). (4)
     * <addr> must be within bounds (CapEx_LengthViolation Violation).
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
    //     (size_t)addr, (size_t)len, (size_t)cr->cr_base,
    //     (size_t)cr->cr_length);
    if (!cap_is_in_bounds(cr, addr, len)) {
        cause = CapEx_LengthViolation;
        // qemu_log("CAP Len VIOLATION: ");
        goto do_exception;
    }
    return;

do_exception:
    raise_cheri_exception_impl(env, cause, regnum, instavail, pc);
}

static inline bool cheri_have_access_sysregs(CPUArchState* env)
{
    return cap_has_perms(cheri_get_pcc(env), CAP_ACCESS_SYS_REGS);
}
