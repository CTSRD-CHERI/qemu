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
#pragma once
#include "cheri_defs.h"
#include "cheri_utils.h"
#include "cheri-lazy-capregs.h"
#include "cheri-bounds-stats.h"
#include "tcg/tcg.h"
#include "tcg/tcg-op.h"
#include "exec/exec-all.h"

static inline const cap_register_t *_cheri_get_pcc_unchecked(CPUArchState *env);

// Returns a recebt PCC value. This means that the cursor field may be out of
// date (possibly pointing to the beginning of the translation block). However,
// all permissions, flags and bounds are valid and can be accessed.
// This function exists since updating pcc.cursor on every instruction is
// expensive and generally not required. If the current value is needed, it can
// be computed from the return address (GETPC() in the toplevel helper function)
// by calling cpu_restore_state(). However, it is preferable if the translator
// code writes the updated pcc.cursor before calling the helper if this is the
// common case (for example, it should not be saved if the current pc value is
// only needed if a trap is raised).
static inline const cap_register_t *cheri_get_recent_pcc(CPUArchState *env)
{
    return _cheri_get_pcc_unchecked(env);
}

static inline bool pc_is_current(CPUArchState *env);

// This function can be called if the pcc.cursor value must be accurate.
static inline const cap_register_t *cheri_get_current_pcc(CPUArchState *env)
{
    tcg_debug_assert(pc_is_current(env));
    return _cheri_get_pcc_unchecked(env);
}

static inline const cap_register_t *
cheri_update_and_get_current_pcc(CPUArchState *env, uintptr_t retpc)
{
    cpu_restore_state(env_cpu(env), retpc, false);
    return cheri_get_current_pcc(env);
}

static inline target_ulong cpu_get_current_pc(CPUArchState *env,
                                              uintptr_t retpc)
{
    cpu_restore_state(env_cpu(env), retpc, false);
    tcg_debug_assert(pc_is_current(env));
    return cpu_get_recent_pc(env);
}

static inline target_ulong cpu_get_current_pc_checked(CPUArchState *env)
{
    tcg_debug_assert(pc_is_current(env));
    return cpu_get_recent_pc(env);
}

static inline void derive_cap_from_pcc(CPUArchState *env, uint32_t cd,
                                       target_ulong new_addr, uintptr_t retpc,
                                       struct oob_stats_info *oob_info)
{
#ifdef DO_CHERI_STATISTICS
    oob_info->num_uses++;
#else
    (void)oob_info;
#endif
    // Note: we can use a PCC value with an outdated cursor since we are writing
    // a new address.
    const cap_register_t *pccp = cheri_get_recent_pcc(env);
    cap_register_t result = *pccp;
    if (!is_representable_cap_with_addr(pccp, new_addr)) {
        if (pccp->cr_tag)
            became_unrepresentable(env, cd, oob_info, retpc);
        cap_mark_unrepresentable(new_addr, &result);
    } else {
        result._cr_cursor = new_addr;
        check_out_of_bounds_stat(env, oob_info, &result, retpc);
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
    return cap_has_perms(cheri_get_recent_pcc(env), CAP_ACCESS_SYS_REGS);
}

static inline bool should_log_mem_access(CPUArchState *env, int log_mask, target_ulong addr) {
    if (likely(!(qemu_loglevel_mask(log_mask))))
        return false;

    // Try not to dump all stores when -dfilter is enabled
    // Note: we check both PC and memory location in -dfilter
    if (likely(!qemu_log_in_addr_range(cpu_get_recent_pc(env)) &&
        !qemu_log_in_addr_range(addr)))
        return false;
    return true;
}

static inline const char* cheri_cause_str(CheriCapExcCause cause) {
    switch (cause) {
    case CapEx_None: return "None";
    case CapEx_LengthViolation: return "Length Violation";
    case CapEx_TagViolation: return "Tag Violation";
    case CapEx_SealViolation: return "Seal Violation";
    case CapEx_TypeViolation: return "Type Violation";
    case CapEx_CallTrap: return "Call Trap";
    case CapEx_ReturnTrap: return "Return Trap";
    case CapEx_TSSUnderFlow: return "Underflow of Trusted System Stack";
    case CapEx_UserDefViolation: return "User-defined Permission Violation";
    case CapEx_TLBNoStoreCap: return "TLB prohibits Store Capability";
    case CapEx_InexactBounds: return "Bounds Cannot Be Represented Exactly";
    case CapEx_UnalignedBase: return "Unaligned Base";
    case CapEx_CapLoadGen: return "Cap Load Gen Mismatch";
    case CapEx_GlobalViolation: return "Global Violation";
    case CapEx_PermitExecuteViolation: return "Permit_Execute Violation";
    case CapEx_PermitLoadViolation: return "Permit_Load Violation";
    case CapEx_PermitStoreViolation: return "Permit_Store Violation";
    case CapEx_PermitLoadCapViolation: return "Permit_Load_Capability Violation";
    case CapEx_PermitStoreCapViolation: return "Permit_Store_Capability Violation";
    case CapEx_PermitStoreLocalCapViolation: return "Permit_Store_Local_Capability Violation";
    case CapEx_PermitSealViolation: return "Permit_Seal Violation";
    case CapEx_AccessSystemRegsViolation: return "Access_System_Registers Violation";
    case CapEx_AccessCCallIDCViolation: return "IDC used in CCall delay slot";
    case CapEx_PermitCCallViolation: return "Permit_CCall Violation";
    case CapEx_PermitUnsealViolation: return "Permit_Unseal Violation";
    case CapEx_PermitSetCIDViolation: return "Permit_SetCID Violation";
    }
    // default: return "Unknown cause";
    __builtin_unreachable();
    abort();
}

const cap_register_t *get_load_store_base_cap(CPUArchState *env, uint32_t cb);
void store_cap_to_memory(CPUArchState *env, uint32_t cs, target_ulong vaddr,
                         target_ulong retpc);
void load_cap_from_memory(CPUArchState *env, uint32_t cd, uint32_t cb,
                          const cap_register_t *source, target_ulong vaddr,
                          target_ulong retpc, hwaddr *physaddr);
// Helper for RISCV AMOSWAP
bool load_cap_from_memory_128(CPUArchState *env, uint64_t *pesbt,
                              uint64_t *cursor, uint32_t cb,
                              const cap_register_t *source, target_ulong vaddr,
                              target_ulong retpc, hwaddr *physaddr);
