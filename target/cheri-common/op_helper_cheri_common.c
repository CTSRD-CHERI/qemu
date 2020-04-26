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

#include "cheri-helper-utils.h"
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

static inline bool is_cap_sealed(const cap_register_t *cp)
{
    // TODO: remove this function and update all callers to use the correct
    // function
    return !cap_is_unsealed(cp);
}

void CHERI_HELPER_IMPL(ddc_check_bounds(CPUArchState *env, target_ulong addr,
                                        target_ulong num_bytes))
{
    const cap_register_t *ddc = cheri_get_ddc(env);
    cheri_debug_assert(ddc->cr_tag && cap_is_unsealed(ddc) &&
                       "Should have been checked before bounds!");
    check_cap(env, ddc, 0, addr, CHERI_EXC_REGNUM_DDC, num_bytes,
              /*instavail=*/true, GETPC());
}

target_ulong CHERI_HELPER_IMPL(pcc_check_load(CPUArchState *env,
                                              target_ulong pcc_offset,
                                              MemOp op))
{
    uintptr_t retpc = GETPC();
    const cap_register_t *pcc = cheri_get_current_pcc_fetch_from_tcg(env, retpc);
    target_ulong addr = pcc_offset + cap_get_cursor(pcc);
    check_cap(env, pcc, CAP_PERM_LOAD, addr, CHERI_EXC_REGNUM_PCC,
              memop_size(op), /*instavail=*/true, retpc);
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
    const cap_register_t *cbp = get_readonly_capreg(env, cb);
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

target_ulong CHERI_HELPER_IMPL(cjalr(CPUArchState *env, uint32_t cd,
                                     uint32_t cb, target_ulong link_pc))
{
    /*
     * CJALR: Jump and Link Capability Register
     */
    GET_HOST_RETPC();
    const cap_register_t *cbp = get_readonly_capreg(env, cb);
    if (!cbp->cr_tag) {
        raise_cheri_exception(env, CapEx_TagViolation, cb);
    } else if (cap_is_sealed_with_type(cbp)) {
        // Note: "sentry" caps can be called using cjalr
        raise_cheri_exception(env, CapEx_SealViolation, cb);
    } else if (!(cbp->cr_perms & CAP_PERM_EXECUTE)) {
        raise_cheri_exception(env, CapEx_PermitExecuteViolation, cb);
    } else if (!(cbp->cr_perms & CAP_PERM_GLOBAL)) {
        raise_cheri_exception(env, CapEx_GlobalViolation, cb);
    } else if (!cap_is_in_bounds(cbp, cap_get_cursor(cbp), 4)) {
        raise_cheri_exception(env, CapEx_LengthViolation, cb);
    } else if (!validate_cjalr_target(env, cbp, cb, _host_return_address)) {
        assert(false && "Should have raised an exception");
    }

    cheri_debug_assert(cap_is_unsealed(cbp) || cap_is_sealed_entry(cbp));
    cap_register_t next_pcc = *cbp;
    if (cap_is_sealed_entry(cbp)) {
        // If we are calling a "sentry" cap, remove the sealed flag
        cap_unseal_entry(&next_pcc);
    }
    // Don't generate a link capability if cd == zero register
    if (cd != 0) {
        // Note: PCC.cursor doesn't need to be up-to-date, TB start is fine
        // since we are writing a new cursor anyway.
        cap_register_t result = *cheri_get_recent_pcc(env);
        // can never create an unrepresentable capability since PCC must be in bounds
        result._cr_cursor = link_pc;
#if QEMU_USE_COMPRESSED_CHERI_CAPS == 1
        assert(cc128_is_representable_with_addr(&result, link_pc) &&
               "Link addr must be representable");
#endif
        if (cap_is_sealed_entry(cbp)) {
            // When calling a sentry capability the return capability is
            // turned into a sentry, too.
            cap_make_sealed_entry(&result);
        }
        update_capreg(env, cd, &result);
    }

#ifdef TARGET_MIPS
    // The capability register is loaded into PCC during delay slot
    env->active_tc.CapBranchTarget = next_pcc;
#elif defined(TARGET_RISCV)
    // Update PCC now. On return to TCG we will jump there immediately, so
    // updating it now should be fine.
    env->PCC = next_pcc;
#ifdef CONFIG_DEBUG_TCG
    env->_pc_is_current = true;
#endif
#else
#error "No CJALR for this target"
#endif
    // Return the branch target address
    return cap_get_cursor(cbp);
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

void CHERI_HELPER_IMPL(cchecktype(CPUArchState *env, uint32_t cs, uint32_t cb))
{
    GET_HOST_RETPC();
    const cap_register_t *csp = get_readonly_capreg(env, cs);
    const cap_register_t *cbp = get_readonly_capreg(env, cb);
    /*
     * CCheckType: Raise exception if otypes don't match
     */
    if (!csp->cr_tag) {
        raise_cheri_exception(env, CapEx_TagViolation, cs);
    } else if (!cbp->cr_tag) {
        raise_cheri_exception(env, CapEx_TagViolation, cb);
    } else if (cap_is_unsealed(csp)) {
        raise_cheri_exception(env, CapEx_SealViolation, cs);
    } else if (cap_is_unsealed(cbp)) {
        raise_cheri_exception(env, CapEx_SealViolation, cb);
    } else if (csp->cr_otype != cbp->cr_otype ||
               csp->cr_otype > CAP_LAST_NONRESERVED_OTYPE) {
        raise_cheri_exception(env, CapEx_TypeViolation, cs);
    }
}

void CHERI_HELPER_IMPL(csealentry(CPUArchState *env, uint32_t cd, uint32_t cs))
{
    /*
     * CSealEntry: Seal a code capability so it is only callable with cjr/cjalr
     * (all other permissions are ignored so it can't be used for loads, etc)
     */
    GET_HOST_RETPC();
    const cap_register_t *csp = get_readonly_capreg(env, cs);
    if (!csp->cr_tag) {
        raise_cheri_exception(env, CapEx_TagViolation, cs);
    } else if (!cap_is_unsealed(csp)) {
        raise_cheri_exception(env, CapEx_SealViolation, cs);
    } else if (!(csp->cr_perms & CAP_PERM_EXECUTE)) {
        // Capability must be executable otherwise csealentry doesn't make sense
        raise_cheri_exception(env, CapEx_PermitExecuteViolation, cs);
    } else {
        cap_register_t result = *csp;
        // capability can now only be used in cjr/cjalr
        cap_make_sealed_entry(&result);
        update_capreg(env, cd, &result);
    }
}

/// Two operands (capability and int)
void CHERI_HELPER_IMPL(ccheckperm(CPUArchState *env, uint32_t cs,
                                  target_ulong rt))
{
    GET_HOST_RETPC();
    const cap_register_t *csp = get_readonly_capreg(env, cs);
    uint32_t rt_perms = (uint32_t)rt & (CAP_PERMS_ALL);
    uint32_t rt_uperms = ((uint32_t)rt >> CAP_UPERMS_SHFT) & CAP_UPERMS_ALL;
    /*
     * CCheckPerm: Raise exception if don't have permission
     */
    if (!csp->cr_tag) {
        raise_cheri_exception(env, CapEx_TagViolation, cs);
    } else if ((csp->cr_perms & rt_perms) != rt_perms) {
        raise_cheri_exception(env, CapEx_UserDefViolation, cs);
    } else if ((csp->cr_uperms & rt_uperms) != rt_uperms) {
        raise_cheri_exception(env, CapEx_UserDefViolation, cs);
    } else if ((rt >> (16 + CAP_MAX_UPERM)) != 0UL) {
        raise_cheri_exception(env, CapEx_UserDefViolation, cs);
    }
}

/// Two operands (int int)

static target_ulong crap_impl(target_ulong len) {
#ifdef CHERI_128
    // In QEMU we do this by performing a csetbounds on a maximum permissions
    // capability and returning the resulting length
    cap_register_t tmpcap;
    set_max_perms_capability(&tmpcap, 0);
    cc128_setbounds(&tmpcap, 0, len);
    // FIXME: should we return 0 for 1 << 64? instead?
    return cap_get_length64(&tmpcap);
#else
    // For MAGIC128 and 256 everything is representable -> we can return len
  return len;
#endif
}

target_ulong CHERI_HELPER_IMPL(crap(CPUArchState *env, target_ulong len))
{
    // CRoundArchitecturalPrecision rt, rs:
    // rt is set to the smallest value greater or equal to rs that can be used
    // by CSetBoundsExact without trapping (assuming a suitably aligned base).
    // Now renamed to CRoundReprensentableLength (CRRL)
    return crap_impl(len);
}

target_ulong CHERI_HELPER_IMPL(cram(CPUArchState *env, target_ulong len))
{
    // CRepresentableAlignmentMask rt, rs:
    // rt is set to a mask that can be used to align down addresses to a value
    // that is sufficiently aligned to set precise bounds for the nearest
    // representable length of rs (as obtained by CRoundArchitecturalPrecision).
#ifdef CHERI_128
    // The mask used to align down is all ones followed by (required exponent
    // for compressed representation) zeroes
    target_ulong result = cc128_get_alignment_mask(len);
    target_ulong rounded_with_crap = crap_impl(len);
    target_ulong rounded_with_cram = (len + ~result) & result;
    qemu_maybe_log_instr_extra(env, "cram(" TARGET_FMT_lx ") rounded="
        TARGET_FMT_lx " rounded with mask=" TARGET_FMT_lx " mask result="
        TARGET_FMT_lx "\n", len, rounded_with_crap, rounded_with_cram, result);
    if (rounded_with_cram != rounded_with_crap) {
        warn_report("CRAM and CRRL disagree for " TARGET_FMT_lx ": crrl=" TARGET_FMT_lx
                    " cram=" TARGET_FMT_lx, len, rounded_with_crap, rounded_with_cram);
        qemu_maybe_log_instr_extra(env, "WARNING: CRAM and CRRL disagree for "
            TARGET_FMT_lx ": crrl=" TARGET_FMT_lx " cram=" TARGET_FMT_lx,
            len, rounded_with_crap, rounded_with_cram);
    }
    return result;
#else
    // For MAGIC128 and 256 everything is representable -> we can return all ones
    return UINT64_MAX;
#endif
}

/// Three operands (capability capability capability)

void CHERI_HELPER_IMPL(cbuildcap(CPUArchState *env, uint32_t cd, uint32_t cb,
                                 uint32_t ct))
{
    GET_HOST_RETPC();
    // CBuildCap traps on cbp == NULL so we use reg0 as $ddc. This saves
    // encoding space and also means a cbuildcap relative to $ddc can be one
    // instr instead of two.
    const cap_register_t *cbp = get_capreg_0_is_ddc(env, cb);
#ifdef TARGET_RISCV
    uint32_t cb_exc = cb == 0 ? CHERI_EXC_REGNUM_DDC : cb;
#else
    uint32_t cb_exc = cb;
#endif
    const cap_register_t *ctp = get_readonly_capreg(env, ct);
    /*
     * CBuildCap: create capability from untagged register.
     * XXXAM: Note this is experimental and may change.
     */
    if (!cbp->cr_tag) {
        raise_cheri_exception(env, CapEx_TagViolation, cb_exc);
    } else if (is_cap_sealed(cbp)) {
        raise_cheri_exception(env, CapEx_SealViolation, cb_exc);
    } else if (ctp->cr_base < cbp->cr_base) {
        raise_cheri_exception(env, CapEx_LengthViolation, cb_exc);
    } else if (cap_get_top(ctp) > cap_get_top(cbp)) {
        raise_cheri_exception(env, CapEx_LengthViolation, cb_exc);
        // } else if (ctp->cr_length < 0) {
        //    raise_cheri_exception(env, CapEx_LengthViolation, ct);
    } else if ((ctp->cr_perms & cbp->cr_perms) != ctp->cr_perms) {
        raise_cheri_exception(env, CapEx_UserDefViolation, cb_exc);
    } else if ((ctp->cr_uperms & cbp->cr_uperms) != ctp->cr_uperms) {
        raise_cheri_exception(env, CapEx_UserDefViolation, cb_exc);
    } else {
        /* XXXAM basic trivial implementation may not handle
         * compressed capabilities fully, does not perform renormalization.
         */
        // Without the temporary cap_register_t we would copy cb into cd
        // if cdp cd == ct (this was caught by testing cbuildcap $c3, $c1, $c3)
        cap_register_t result = *cbp;
        result.cr_base = ctp->cr_base;
        result._cr_top = ctp->_cr_top;
        result.cr_perms = ctp->cr_perms;
        result.cr_uperms = ctp->cr_uperms;
        result._cr_cursor = ctp->_cr_cursor;
        if (cap_is_sealed_entry(ctp))
            cap_make_sealed_entry(&result);
        else
            result.cr_otype = CAP_OTYPE_UNSEALED;
        update_capreg(env, cd, &result);
    }
}

void CHERI_HELPER_IMPL(ccopytype(CPUArchState *env, uint32_t cd, uint32_t cb,
                                 uint32_t ct))
{
    GET_HOST_RETPC();
    const cap_register_t *cbp = get_readonly_capreg(env, cb);
    const cap_register_t *ctp = get_readonly_capreg(env, ct);
    /*
     * CCopyType: copy object type from untagged capability.
     * XXXAM: Note this is experimental and may change.
     */
    if (!cbp->cr_tag) {
        raise_cheri_exception(env, CapEx_TagViolation, cb);
    } else if (is_cap_sealed(cbp)) {
        raise_cheri_exception(env, CapEx_SealViolation, cb);
    } else if (!cap_is_sealed_with_type(ctp)) {
        cap_register_t result;
        update_capreg(env, cd, int_to_cap(-1, &result));
    } else if (ctp->cr_otype < cap_get_base(cbp)) {
        raise_cheri_exception(env, CapEx_LengthViolation, cb);
    } else if (ctp->cr_otype >= cap_get_top(cbp)) {
        raise_cheri_exception(env, CapEx_LengthViolation, cb);
    } else {
        cap_register_t result = *cbp;
        result._cr_cursor = ctp->cr_otype;
        update_capreg(env, cd, &result);
    }
}

static void cseal_common(CPUArchState *env, uint32_t cd, uint32_t cs,
                         uint32_t ct, bool conditional,
                         uintptr_t _host_return_address)
{
    const cap_register_t *csp = get_readonly_capreg(env, cs);
    const cap_register_t *ctp = get_readonly_capreg(env, ct);
    uint64_t ct_base_plus_offset = cap_get_cursor(ctp);
    /*
     * CSeal: Seal a capability
     */
    if (!csp->cr_tag) {
        raise_cheri_exception(env, CapEx_TagViolation, cs);
    } else if (!ctp->cr_tag) {
        if (conditional)
            update_capreg(env, cd, csp);
        else
            raise_cheri_exception(env, CapEx_TagViolation, ct);
    } else if (conditional && cap_get_cursor(ctp) == -1) {
        update_capreg(env, cd, csp);
    } else if (!cap_is_unsealed(csp)) {
        raise_cheri_exception(env, CapEx_SealViolation, cs);
    } else if (!cap_is_unsealed(ctp)) {
        raise_cheri_exception(env, CapEx_SealViolation, ct);
    } else if (!(ctp->cr_perms & CAP_PERM_SEAL)) {
        raise_cheri_exception(env, CapEx_PermitSealViolation, ct);
    } else if (!cap_is_in_bounds(ctp, ct_base_plus_offset, /*num_bytes=*/1)) {
        // Must be within bounds -> num_bytes=1
        raise_cheri_exception(env, CapEx_LengthViolation, ct);
    } else if (ct_base_plus_offset > (uint64_t)CAP_LAST_NONRESERVED_OTYPE) {
        raise_cheri_exception(env, CapEx_LengthViolation, ct);
    } else if (!is_representable_cap_when_sealed_with_addr(
                   csp, cap_get_cursor(csp))) {
        raise_cheri_exception(env, CapEx_InexactBounds, cs);
    } else {
        cap_register_t result = *csp;
        cap_set_sealed(&result, (uint32_t)ct_base_plus_offset);
        update_capreg(env, cd, &result);
    }
}

void CHERI_HELPER_IMPL(ccseal(CPUArchState *env, uint32_t cd, uint32_t cs,
                              uint32_t ct))
{
    /*
     * CCSeal: Conditionally seal a capability.
     */
    cseal_common(env, cd, cs, ct, true, GETPC());
}

void CHERI_HELPER_IMPL(cseal(CPUArchState *env, uint32_t cd, uint32_t cs,
                             uint32_t ct))
{
    /*
     * CSeal: Seal a capability
     */
    cseal_common(env, cd, cs, ct, false, GETPC());
}

void CHERI_HELPER_IMPL(cunseal(CPUArchState *env, uint32_t cd, uint32_t cs,
                               uint32_t ct))
{
    GET_HOST_RETPC();
    const cap_register_t *csp = get_readonly_capreg(env, cs);
    const cap_register_t *ctp = get_readonly_capreg(env, ct);
    const uint64_t ct_cursor = cap_get_cursor(ctp);
    /*
     * CUnseal: Unseal a sealed capability
     */
    if (!csp->cr_tag) {
        raise_cheri_exception(env, CapEx_TagViolation, cs);
    } else if (!ctp->cr_tag) {
        raise_cheri_exception(env, CapEx_TagViolation, ct);
    } else if (cap_is_unsealed(csp)) {
        raise_cheri_exception(env, CapEx_SealViolation, cs);
    } else if (!cap_is_unsealed(ctp)) {
        raise_cheri_exception(env, CapEx_SealViolation, ct);
    } else if (!cap_is_sealed_with_type(csp)) {
        raise_cheri_exception(env, CapEx_TypeViolation,
                              cs); /* Reserved otypes */
    } else if (ct_cursor != csp->cr_otype) {
        raise_cheri_exception(env, CapEx_TypeViolation, ct);
    } else if (!(ctp->cr_perms & CAP_PERM_UNSEAL)) {
        raise_cheri_exception(env, CapEx_PermitUnsealViolation, ct);
    } else if (!cap_is_in_bounds(ctp, ct_cursor, /*num_bytes=1*/ 1)) {
        // Must be within bounds and not one past end (i.e. not equal to top ->
        // num_bytes=1)
        raise_cheri_exception(env, CapEx_LengthViolation, ct);
    } else if (ct_cursor >= CAP_LAST_NONRESERVED_OTYPE) {
        // This should never happen due to the ct_cursor != csp->cr_otype check
        // above that should never succeed for
        raise_cheri_exception(env, CapEx_LengthViolation, ct);
    } else {
        cap_register_t result = *csp;
        if ((csp->cr_perms & CAP_PERM_GLOBAL) &&
            (ctp->cr_perms & CAP_PERM_GLOBAL)) {
            result.cr_perms |= CAP_PERM_GLOBAL;
        } else {
            result.cr_perms &= ~CAP_PERM_GLOBAL;
        }
        cap_set_unsealed(&result);
        update_capreg(env, cd, &result);
    }
}

/// Three operands (capability capability int)

#ifdef DO_CHERI_STATISTICS
struct bounds_bucket bounds_buckets[NUM_BOUNDS_BUCKETS] = {
    {1, "1  "}, // 1
    {2, "2  "}, // 2
    {4, "4  "}, // 3
    {8, "8  "}, // 4
    {16, "16 "},        {32, "32 "},          {64, "64 "},
    {256, "256"},       {1024, "1K "},        {4096, "4K "},
    {64 * 1024, "64K"}, {1024 * 1024, "1M "}, {64 * 1024 * 1024, "64M"},
};

DEFINE_CHERI_STAT(cincoffset);
DEFINE_CHERI_STAT(csetoffset);
DEFINE_CHERI_STAT(csetaddr);
DEFINE_CHERI_STAT(candaddr);
DEFINE_CHERI_STAT(cfromptr);

static void cincoffset_impl(CPUArchState *env, uint32_t cd, uint32_t cb,
                            target_ulong rt, uintptr_t retpc,
                            struct oob_stats_info *oob_info)
{
    oob_info->num_uses++;
#else
static void cincoffset_impl(CPUArchState *env, uint32_t cd, uint32_t cb,
                            target_ulong rt, uintptr_t retpc, void *dummy_arg)
{
    (void)dummy_arg;
#endif
    const cap_register_t *cbp = get_readonly_capreg(env, cb);
    /*
     * CIncOffset: Increase Offset
     */
    if (cbp->cr_tag && is_cap_sealed(cbp)) {
        raise_cheri_exception_impl(env, CapEx_SealViolation, cb, true, retpc);
    } else {
        uint64_t new_addr = cap_get_cursor(cbp) + rt;
        cap_register_t result = *cbp;
        if (unlikely(!is_representable_cap_with_addr(cbp, new_addr))) {
            if (cbp->cr_tag) {
                became_unrepresentable(env, cd, oob_info, retpc);
            }
            cap_mark_unrepresentable(new_addr, &result);
        } else {
            result._cr_cursor = new_addr;
            check_out_of_bounds_stat(env, oob_info, &result,
                                     _host_return_address);
        }
        update_capreg(env, cd, &result);
    }
}

void CHERI_HELPER_IMPL(candperm(CPUArchState *env, uint32_t cd, uint32_t cb,
                                target_ulong rt))
{
    const cap_register_t *cbp = get_readonly_capreg(env, cb);
    GET_HOST_RETPC();
    /*
     * CAndPerm: Restrict Permissions
     */
    if (!cbp->cr_tag) {
        raise_cheri_exception(env, CapEx_TagViolation, cb);
    } else if (!cap_is_unsealed(cbp)) {
        raise_cheri_exception(env, CapEx_SealViolation, cb);
    } else {
        uint32_t rt_perms = (uint32_t)rt & (CAP_PERMS_ALL);
        uint32_t rt_uperms = ((uint32_t)rt >> CAP_UPERMS_SHFT) & CAP_UPERMS_ALL;

        cap_register_t result = *cbp;
        result.cr_perms = cbp->cr_perms & rt_perms;
        result.cr_uperms = cbp->cr_uperms & rt_uperms;
        update_capreg(env, cd, &result);
    }
}

void CHERI_HELPER_IMPL(cincoffset(CPUArchState *env, uint32_t cd, uint32_t cb,
                                  target_ulong rt))
{
    return cincoffset_impl(env, cd, cb, rt, GETPC(), OOB_INFO(cincoffset));
}

void CHERI_HELPER_IMPL(candaddr(CPUArchState *env, uint32_t cd, uint32_t cb,
                                target_ulong rt))
{
    target_ulong cursor = get_capreg_cursor(env, cb);
    target_ulong target_addr = cursor & rt;
    target_ulong diff = target_addr - cursor;
    cincoffset_impl(env, cd, cb, diff, GETPC(), OOB_INFO(candaddr));
}

void CHERI_HELPER_IMPL(csetaddr(CPUArchState *env, uint32_t cd, uint32_t cb,
                                target_ulong target_addr))
{
    target_ulong cursor = get_capreg_cursor(env, cb);
    target_ulong diff = target_addr - cursor;
    cincoffset_impl(env, cd, cb, diff, GETPC(), OOB_INFO(csetaddr));
}

void CHERI_HELPER_IMPL(csetoffset(CPUArchState *env, uint32_t cd, uint32_t cb,
                                  target_ulong target_offset))
{
    target_ulong offset = cap_get_offset(get_readonly_capreg(env, cb));
    target_ulong diff = target_offset - offset;
    cincoffset_impl(env, cd, cb, diff, GETPC(), OOB_INFO(csetoffset));
}

void CHERI_HELPER_IMPL(cfromptr(CPUArchState *env, uint32_t cd, uint32_t cb,
                                target_ulong rt))
{
    GET_HOST_RETPC();
#ifdef DO_CHERI_STATISTICS
    OOB_INFO(cfromptr)->num_uses++;
#endif
    // CFromPtr traps on cbp == NULL so we use reg0 as $ddc to save encoding
    // space (and for backwards compat with old binaries).
    // Note: This is also still required for new binaries since clang assumes it
    // can use zero as $ddc in cfromptr/ctoptr
    const cap_register_t *cbp = get_capreg_0_is_ddc(env, cb);
#ifdef TARGET_RISCV
    uint32_t cb_exc = cb == 0 ? CHERI_EXC_REGNUM_DDC : cb;
#else
    uint32_t cb_exc = cb;
#endif
    /*
     * CFromPtr: Create capability from pointer
     */
    if (rt == (target_ulong)0) {
        cap_register_t result;
        update_capreg(env, cd, null_capability(&result));
    } else if (!cbp->cr_tag) {
        raise_cheri_exception(env, CapEx_TagViolation, cb_exc);
    } else if (is_cap_sealed(cbp)) {
        raise_cheri_exception(env, CapEx_SealViolation, cb_exc);
    } else {
        cap_register_t result = *cbp;
        uint64_t new_addr = cbp->cr_base + rt;
        if (!is_representable_cap_with_addr(cbp, new_addr)) {
            became_unrepresentable(env, cd, OOB_INFO(cfromptr),
                                   _host_return_address);
            cap_mark_unrepresentable(new_addr, &result);
        } else {
            result._cr_cursor = new_addr;
            check_out_of_bounds_stat(env, OOB_INFO(cfromptr), &result,
                                     _host_return_address);
        }
        update_capreg(env, cd, &result);
    }
}

static void do_setbounds(bool must_be_exact, CPUArchState *env, uint32_t cd,
                         uint32_t cb, target_ulong length,
                         uintptr_t _host_return_address)
{
    const cap_register_t *cbp = get_readonly_capreg(env, cb);
    uint64_t cursor = cap_get_cursor(cbp);
    unsigned __int128 new_top = (unsigned __int128)cursor + length; // 65 bits
    /*
     * CSetBounds: Set Bounds
     */
    if (!cbp->cr_tag) {
        raise_cheri_exception(env, CapEx_TagViolation, cb);
    } else if (is_cap_sealed(cbp)) {
        raise_cheri_exception(env, CapEx_SealViolation, cb);
    } else if (cursor < cbp->cr_base) {
        raise_cheri_exception(env, CapEx_LengthViolation, cb);
    } else if (new_top > cap_get_top65(cbp)) {
        raise_cheri_exception(env, CapEx_LengthViolation, cb);
    } else {
        cap_register_t result = *cbp;
#if QEMU_USE_COMPRESSED_CHERI_CAPS
        _Static_assert(CHERI_CAP_SIZE == 16, "");
        /*
         * With compressed capabilities we may need to increase the range of
         * memory addresses to be wider than requested so it is
         * representable.
         */
        const bool exact = cc128_setbounds(&result, cursor, new_top);
        if (!exact)
            env->statcounters_imprecise_setbounds++;
        if (must_be_exact && !exact) {
            raise_cheri_exception(env, CapEx_InexactBounds, cb);
            return;
        }
        assert(cc128_is_representable_cap_exact(&result) &&
               "CSetBounds must create a representable capability");
#else
        (void)must_be_exact;
        /* Capabilities are precise -> can just set the values here */
        result.cr_base = cursor;
        result._cr_top = new_top;
        result._cr_cursor = cursor;
#endif
        assert(result.cr_base >= cbp->cr_base &&
               "CSetBounds broke monotonicity (base)");
        assert(cap_get_length65(&result) <= cap_get_length65(cbp) &&
               "CSetBounds broke monotonicity (length)");
        assert(cap_get_top65(&result) <= cap_get_top65(cbp) &&
               "CSetBounds broke monotonicity (top)");
        update_capreg(env, cd, &result);
    }
}

void CHERI_HELPER_IMPL(csetbounds(CPUArchState *env, uint32_t cd, uint32_t cb,
                                  target_ulong rt))
{
    do_setbounds(false, env, cd, cb, rt, GETPC());
}

void CHERI_HELPER_IMPL(csetboundsexact(CPUArchState *env, uint32_t cd,
                                       uint32_t cb, target_ulong rt))
{
    do_setbounds(true, env, cd, cb, rt, GETPC());
}

void CHERI_HELPER_IMPL(csetflags(CPUArchState *env, uint32_t cd, uint32_t cb,
                                 target_ulong flags))
{
    const cap_register_t *cbp = get_readonly_capreg(env, cb);
    GET_HOST_RETPC();
    /*
     * CSetFlags: Set Flags
     */
    if (cbp->cr_tag && !cap_is_unsealed(cbp)) {
        raise_cheri_exception(env, CapEx_SealViolation, cb);
    }
    // FIXME: should we trap instead of masking?
    cap_register_t result = *cbp;
    flags &= CAP_FLAGS_ALL_BITS;
    _Static_assert(CAP_FLAGS_ALL_BITS == 1, "Only one flag should exist");
    result.cr_flags = flags;
    update_capreg(env, cd, &result);
}

/// Three operands (int capability capability)

// static inline bool cap_bounds_are_subset(const cap_register_t *first, const
// cap_register_t *second) {
//    return cap_get_base(first) <= cap_get_base(second) && cap_get_top(second)
//    <= cap_get_top(first);
//}

target_ulong CHERI_HELPER_IMPL(csub(CPUArchState *env, uint32_t cb,
                                    uint32_t ct))
{
    // This is very noisy, but may be interesting for C-compatibility analysis
#if 0
    const cap_register_t *cbp = get_readonly_capreg(env, cb);
    const cap_register_t *ctp = get_readonly_capreg(env, ct);


    // If the capabilities are not subsets (i.e. at least one tagged and derived from different caps,
    // emit a warning to see how many subtractions are being performed that are invalid in ISO C
    if (cbp->cr_tag != ctp->cr_tag ||
        (cbp->cr_tag && !cap_bounds_are_subset(cbp, ctp) && !cap_bounds_are_subset(ctp, cbp))) {
        // Don't warn about subtracting NULL:
        if (!is_null_capability(ctp)) {
            warn_report("Subtraction between two capabilities that are not subsets: \r\n"
                    "\tLHS: " PRINT_CAP_FMTSTR "\r\n\tRHS: " PRINT_CAP_FMTSTR "\r",
                    PRINT_CAP_ARGS(cbp), PRINT_CAP_ARGS(ctp));
        }
    }
#endif
    /*
     * CSub: Subtract Capabilities
     */
    return (target_ulong)(get_capreg_cursor(env, cb) -
                          get_capreg_cursor(env, ct));
}

target_ulong CHERI_HELPER_IMPL(ctestsubset(CPUArchState *env, uint32_t cb,
                                           uint32_t ct))
{
    const cap_register_t *cbp = get_capreg_0_is_ddc(env, cb);
    const cap_register_t *ctp = get_readonly_capreg(env, ct);
    bool is_subset = false;
    /*
     * CTestSubset: Test if capability is a subset of another
     */
    if (cbp->cr_tag == ctp->cr_tag &&
        /* is_cap_sealed(cbp) == is_cap_sealed(ctp) && */
        cap_get_base(cbp) <= cap_get_base(ctp) &&
        cap_get_top(ctp) <= cap_get_top(cbp) &&
        (cbp->cr_perms & ctp->cr_perms) == ctp->cr_perms &&
        (cbp->cr_uperms & ctp->cr_uperms) == ctp->cr_uperms) {
        is_subset = true;
    }
    return (target_ulong)is_subset;
}

target_ulong CHERI_HELPER_IMPL(ctoptr(CPUArchState *env, uint32_t cb,
                                      uint32_t ct))
{
    GET_HOST_RETPC();
    // CToPtr traps on ctp == NULL so we use reg0 as $ddc there. This means we
    // can have a CToPtr relative to $ddc as one instruction instead of two and
    // is required since clang still assumes it can use zero as $ddc in
    // cfromptr/ctoptr
    const cap_register_t *cbp = get_readonly_capreg(env, cb);
    const cap_register_t *ctp = get_capreg_0_is_ddc(env, ct);
    uint64_t cb_cursor = cap_get_cursor(cbp);
#ifdef TARGET_RISCV
    uint32_t ct_exc = ct == 0 ? CHERI_EXC_REGNUM_DDC : ct;
#else
    uint32_t ct_exc = ct;
#endif
    /*
     * CToPtr: Capability to Pointer
     */
    if (!ctp->cr_tag) {
        raise_cheri_exception(env, CapEx_TagViolation, ct_exc);
    } else if (!cbp->cr_tag) {
        return (target_ulong)0;
    } else if (ctp->cr_base > cb_cursor) {
        return (target_ulong)(ctp->cr_base - cb_cursor);
    } else {
        return (target_ulong)(cb_cursor - ctp->cr_base);
    }

    return (target_ulong)0;
}

/// Loads and stores

const cap_register_t *get_load_store_base_cap(CPUArchState *env, uint32_t cb)
{
#ifdef TARGET_MIPS
    // CLC/CSC and the integer variants trap on cbp == NULL so we use reg0 as
    // $ddc to save encoding space and increase code density since loading
    // relative to $ddc is common in the hybrid ABI (and also for backwards
    // compat with old binaries).
    return get_capreg_0_is_ddc(env, cb);
#elif defined(TARGET_RISCV)
    // However, RISCV does not use this encoding and uses zero for the
    // null register (i.e. always trap).
    // The helpers can also be invoked from the explicitly DDC-relative
    // instructions with cb == 33 which means DDC:
    return (cb == CHERI_EXC_REGNUM_DDC) ? cheri_get_ddc(env)
                                        : get_readonly_capreg(env, cb);
#else
#error "Wrong arch?"
#endif
}

static inline target_ulong cap_check_common(uint32_t required_perms,
                                            CPUArchState *env, uint32_t cb,
                                            target_ulong offset, uint32_t size,
                                            uintptr_t _host_return_address)
{
    const cap_register_t *cbp = get_load_store_base_cap(env, cb);
    if (!cbp->cr_tag) {
        raise_cheri_exception(env, CapEx_TagViolation, cb);
    } else if (is_cap_sealed(cbp)) {
        raise_cheri_exception(env, CapEx_SealViolation, cb);
    } else if ((required_perms & CAP_PERM_LOAD) &&
               !(cbp->cr_perms & CAP_PERM_LOAD)) {
        raise_cheri_exception(env, CapEx_PermitLoadViolation, cb);
    } else if ((required_perms & CAP_PERM_STORE) &&
               !(cbp->cr_perms & CAP_PERM_STORE)) {
        raise_cheri_exception(env, CapEx_PermitStoreViolation, cb);
    }
    const target_ulong cursor = cap_get_cursor(cbp);
    const target_ulong addr = cursor + (target_long)offset;
    if (!cap_is_in_bounds(cbp, addr, size)) {
        qemu_log_instr_or_mask_msg(env, CPU_LOG_INT,
            "Failed capability bounds check: offset=" TARGET_FMT_plx
            " cursor=" TARGET_FMT_plx " addr=" TARGET_FMT_plx "\n",
            offset, cursor, addr);
        raise_cheri_exception(env, CapEx_LengthViolation, cb);
    }
#ifdef TARGET_MIPS
    if (!QEMU_IS_ALIGNED(addr, size)) {
#if defined(CHERI_UNALIGNED)
        const char *access_type =
            (required_perms == (CAP_PERM_STORE | CAP_PERM_LOAD))
                ? "RMW"
                : ((required_perms == CAP_PERM_STORE) ? "store" : "load");
        qemu_maybe_log_instr_extra(env, "Allowing unaligned %d-byte %s of "
            "address 0x%" PRIx64 "\n", size, access_type, addr);
#endif
    }
#endif // TARGET_MIPS
    return addr;
}

/*
 * Load Via Capability Register
 */
target_ulong CHERI_HELPER_IMPL(cap_load_check(CPUArchState *env, uint32_t cb,
                                           target_ulong offset, uint32_t size))
{
    return cap_check_common(CAP_PERM_LOAD, env, cb, offset, size, GETPC());
}

/*
 * Store Via Capability Register
 */
target_ulong CHERI_HELPER_IMPL(cap_store_check(CPUArchState *env, uint32_t cb,
                                            target_ulong offset, uint32_t size))
{
    return cap_check_common(CAP_PERM_STORE, env, cb, offset, size, GETPC());
}

/*
 * Read-modify-write Via Capability Register
 */
target_ulong CHERI_HELPER_IMPL(cap_rmw_check(CPUArchState *env, uint32_t cb,
    target_ulong offset, uint32_t size))
{
    return cap_check_common(CAP_PERM_LOAD | CAP_PERM_STORE, env, cb, offset,
                            size, GETPC());
}

/// Capability loads and stores
void CHERI_HELPER_IMPL(load_cap_via_cap(CPUArchState *env, uint32_t cd,
                                        uint32_t cb, target_ulong offset))
{
    GET_HOST_RETPC();
    const cap_register_t *cbp = get_load_store_base_cap(env, cb);

    if (!cbp->cr_tag) {
        raise_cheri_exception(env, CapEx_TagViolation, cb);
    } else if (is_cap_sealed(cbp)) {
        raise_cheri_exception(env, CapEx_SealViolation, cb);
    } else if (!(cbp->cr_perms & CAP_PERM_LOAD)) {
        raise_cheri_exception(env, CapEx_PermitLoadViolation, cb);
    }

    uint64_t addr = (uint64_t)(cap_get_cursor(cbp) + (target_long)offset);
    if (!cap_is_in_bounds(cbp, addr, CHERI_CAP_SIZE)) {
        qemu_log_instr_or_mask_msg(env, CPU_LOG_INT,
            "Failed capability bounds check: offset=" TARGET_FMT_plx " cursor="
            TARGET_FMT_plx " addr=" TARGET_FMT_plx "\n", offset,
            cap_get_cursor(cbp), addr);
        raise_cheri_exception(env, CapEx_LengthViolation, cb);
    } else if (!QEMU_IS_ALIGNED(addr, CHERI_CAP_SIZE)) {
        raise_unaligned_load_exception(env, addr, _host_return_address);
    }
    load_cap_from_memory(env, cd, cb, cbp, addr, _host_return_address,
                         /*physaddr_out=*/NULL);
}

void CHERI_HELPER_IMPL(store_cap_via_cap(CPUArchState *env, uint32_t cs,
                                         uint32_t cb, target_ulong offset))
{
    GET_HOST_RETPC();
    // CSC traps on cbp == NULL so we use reg0 as $ddc to save encoding
    // space and increase code density since storing relative to $ddc is common
    // in the hybrid ABI (and also for backwards compat with old binaries).
    const cap_register_t *cbp = get_load_store_base_cap(env, cb);

    if (!cbp->cr_tag) {
        raise_cheri_exception(env, CapEx_TagViolation, cb);
    } else if (is_cap_sealed(cbp)) {
        raise_cheri_exception(env, CapEx_SealViolation, cb);
    } else if (!(cbp->cr_perms & CAP_PERM_STORE)) {
        raise_cheri_exception(env, CapEx_PermitStoreViolation, cb);
    } else if (!(cbp->cr_perms & CAP_PERM_STORE_CAP)) {
        raise_cheri_exception(env, CapEx_PermitStoreCapViolation, cb);
    } else if (!(cbp->cr_perms & CAP_PERM_STORE_LOCAL) &&
        get_capreg_tag(env, cs) &&
        !(get_capreg_hwperms(env, cs) & CAP_PERM_GLOBAL)) {
        raise_cheri_exception(env, CapEx_PermitStoreLocalCapViolation, cb);
    }

    const uint64_t addr = (uint64_t)(cap_get_cursor(cbp) + (target_long)offset);
    if (!cap_is_in_bounds(cbp, addr, CHERI_CAP_SIZE)) {
        qemu_log_instr_or_mask_msg(env, CPU_LOG_INT,
            "Failed capability bounds check: offset=" TARGET_FMT_plx " cursor="
            TARGET_FMT_plx " addr=" TARGET_FMT_plx "\n", offset,
            cap_get_cursor(cbp), addr);
        raise_cheri_exception(env, CapEx_LengthViolation, cb);
    } else if (!QEMU_IS_ALIGNED(addr, CHERI_CAP_SIZE)) {
        raise_unaligned_store_exception(env, addr, _host_return_address);
    }
    store_cap_to_memory(env, cs, addr, _host_return_address);
}

#if defined(CHERI_128) && QEMU_USE_COMPRESSED_CHERI_CAPS

bool load_cap_from_memory_128(CPUArchState *env, uint64_t *pesbt,
                              uint64_t *cursor, uint32_t cb,
                              const cap_register_t *source, target_ulong vaddr,
                              target_ulong retpc, hwaddr *physaddr)
{
    cheri_debug_assert(QEMU_IS_ALIGNED(vaddr, CHERI_CAP_SIZE));
    /*
     * Load otype and perms from memory (might trap on load)
     *
     * Note: In-memory capabilities pesbt is xored with a mask to ensure that
     * NULL capabilities have an all zeroes representation.
     */
    /* No TLB fault possible, should be safe to get a host pointer now */
    void *host = probe_read(env, vaddr, CHERI_CAP_SIZE,
                            cpu_mmu_index(env, false), retpc);
    // When writing back pesbt we have to XOR with the NULL mask to ensure that
    // NULL capabilities have an all-zeroes representation.
    if (likely(host)) {
        // Fast path, host address in TLB
        *pesbt = ldq_p((char *)host + CHERI_MEM_OFFSET_METADATA) ^
                CC128_NULL_XOR_MASK;
        *cursor = ldq_p((char *)host + CHERI_MEM_OFFSET_CURSOR);
    } else {
        // Slow path for e.g. IO regions.
        qemu_maybe_log_instr_extra(env, "Using slow path for load from guest "
            "address " TARGET_FMT_plx "\n", vaddr);
        *pesbt = cpu_ldq_cap_data_ra(env, vaddr + CHERI_MEM_OFFSET_METADATA, retpc) ^
                CC128_NULL_XOR_MASK;
        *cursor = cpu_ldq_cap_data_ra(env, vaddr + CHERI_MEM_OFFSET_CURSOR, retpc);
    }
    int prot;
    bool tag = cheri_tag_get(env, vaddr, cb, physaddr, &prot, retpc);
    if (tag) {
        tag = cheri_tag_prot_clear_or_trap(env, vaddr, cb, source, prot, retpc, tag);
    }

    env->statcounters_cap_read++;
    if (tag)
        env->statcounters_cap_read_tagged++;

#if defined(TARGET_RISCV) && defined(CONFIG_RVFI_DII)
    env->rvfi_dii_trace.rvfi_dii_mem_addr = vaddr;
    // env->rvfi_dii_trace.rvfi_dii_mem_rdata = cursor;
    env->rvfi_dii_trace.rvfi_dii_mem_rmask = 0xff;
#endif
#if defined(CONFIG_TCG_LOG_INSTR)
    /* Log capability memory access as a single access */
    if (qemu_log_instr_enabled(env)) {
        /*
         * Decompress to log all fields
         * TODO(am2419): why do we decompress? we and up having to compress
         * again in logging implementation. Passing pesbt + cursor would
         * assume a 128-bit format and be less generic?
         */
        cap_register_t ncd;
        decompress_128cap_already_xored(*pesbt, *cursor, &ncd);
        ncd.cr_tag = tag;
        qemu_log_instr_ld_cap(env, vaddr, &ncd);
    }
#endif
    return tag;
}

void load_cap_from_memory(CPUArchState *env, uint32_t cd, uint32_t cb,
                          const cap_register_t *source, target_ulong vaddr,
                          target_ulong retpc, hwaddr *physaddr)
{
    uint64_t pesbt;
    uint64_t cursor;
    bool tag = load_cap_from_memory_128(env, &pesbt, &cursor, cb, source, vaddr,
                                        retpc, physaddr);
    update_compressed_capreg(env, cd, pesbt, tag, cursor);
}

void store_cap_to_memory(CPUArchState *env, uint32_t cs,
                         target_ulong vaddr, target_ulong retpc)
{
    uint64_t cursor = get_capreg_cursor(env, cs);
    uint64_t pesbt_for_mem = get_capreg_pesbt(env, cs) ^ CC128_NULL_XOR_MASK;
#ifdef CONFIG_DEBUG_TCG
    if (get_capreg_state(cheri_get_gpcrs(env), cs) == CREG_INTEGER) {
        tcg_debug_assert(pesbt_for_mem == 0 && "Integer values should have NULL PESBT");
    }
#endif
    bool tag = get_capreg_tag(env, cs);
    if (cs == 0) {
        tcg_debug_assert(pesbt_for_mem == 0 && "Wrong value for cnull?");
        tcg_debug_assert(cursor == 0 && "Wrong value for cnull?");
        tcg_debug_assert(!tag && "Wrong value for cnull?");
    }
    /*
     * Touching the tags will take both the data write TLB fault and
     * capability write TLB fault before updating anything.  Thereafter, the
     * data stores will not take additional faults, so there is no risk of
     * accidentally tagging a shorn data write.  This, like the rest of the
     * tag logic, is not multi-TCG-thread safe.
     */

    env->statcounters_cap_write++;
    if (tag) {
        env->statcounters_cap_write_tagged++;
        cheri_tag_set(env, vaddr, cs, NULL, retpc);
    } else {
        cheri_tag_invalidate(env, vaddr, CHERI_CAP_SIZE, retpc);
    }
    /* No TLB fault possible, should be safe to get a host pointer now */
    void* host = probe_write(env, vaddr, CHERI_CAP_SIZE, cpu_mmu_index(env, false), retpc);
    // When writing back pesbt we have to XOR with the NULL mask to ensure that
    // NULL capabilities have an all-zeroes representation.
    if (likely(host)) {
        // Fast path, host address in TLB
        stq_p((char*)host + CHERI_MEM_OFFSET_METADATA, pesbt_for_mem);
        stq_p((char*)host + CHERI_MEM_OFFSET_CURSOR, cursor);
    } else {
        // Slow path for e.g. IO regions.
        qemu_maybe_log_instr_extra(env, "Using slow path for store to guest "
            "address " TARGET_FMT_plx "\n", vaddr);
        cpu_stq_cap_data_ra(env, vaddr + CHERI_MEM_OFFSET_METADATA,
                            pesbt_for_mem, retpc);
        cpu_stq_cap_data_ra(env, vaddr + CHERI_MEM_OFFSET_CURSOR, cursor,
                            retpc);
    }
#if defined(TARGET_RISCV) && defined(CONFIG_RVFI_DII)
    env->rvfi_dii_trace.rvfi_dii_mem_addr = vaddr;
    // env->rvfi_dii_trace.rvfi_dii_mem_wdata = cursor;
    env->rvfi_dii_trace.rvfi_dii_mem_wmask = 0xff;
#endif
#if defined(CONFIG_TCG_LOG_INSTR)
    /* Log capability memory access as a single access */
    if (qemu_log_instr_enabled(env)) {
        /*
         * Decompress to log all fields
         * TODO(am2419): see notes on the load path on compression.
         */
        cap_register_t stored_cap;
        const uint64_t pesbt = pesbt_for_mem ^ CC128_NULL_XOR_MASK;
        decompress_128cap_already_xored(pesbt, cursor, &stored_cap);
        stored_cap.cr_tag = tag;
        cheri_debug_assert(cursor == cap_get_cursor(&stored_cap));
        qemu_log_instr_st_cap(env, vaddr, &stored_cap);
    }
#endif
}
#endif

QEMU_NORETURN static inline void raise_pcc_fault(CPUArchState *env,
                                                 CheriCapExcCause cause)
{
    cheri_debug_assert(pc_is_current(env));
    // Note: we set pc=0 since PC will have been saved prior to calling the
    // helper and we don't need to recompute it from the generated code.
    raise_cheri_exception_impl(env, cause, CHERI_EXC_REGNUM_PCC,
        /*instavail=*/true, 0);
}

void CHERI_HELPER_IMPL(raise_exception_pcc_perms(CPUArchState *env))
{
    // On translation block entry we check that PCC is tagged and unsealed,
    // has the required permissions and is within bounds
    // The running of the end check is performed in the translator
    const cap_register_t *pcc = cheri_get_current_pcc(env);
    CheriCapExcCause cause;
    if (!pcc->cr_tag) {
        cause = CapEx_TagViolation;
    } else if (!cap_is_unsealed(pcc)) {
        cause = CapEx_SealViolation;
    } else if (!cap_has_perms(pcc, CAP_PERM_EXECUTE)) {
        cause = CapEx_PermitExecuteViolation;
    } else {
        error_report("%s: PCC must be invalid. Logic error in translator? "
                     "PCC=" PRINT_CAP_FMTSTR,
                     __func__, PRINT_CAP_ARGS(pcc));
        tcg_abort();
    }
    raise_pcc_fault(env, cause);
}

void CHERI_HELPER_IMPL(raise_exception_pcc_bounds(CPUArchState *env,
                                                  target_ulong addr,
                                                  uint32_t num_bytes))
{
    // This helper is called either when ifetch runs off the end of pcc or when
    // a branch (e.g. fixed offset relative branchor a jr/jalr instruction)
    // would result in an out-of-bounds pcc value.
    // It is useful to trap on branch rather than ifetch since it greatly
    // improves the debugging experience (exception pc points somewhere
    // helpful).
    cheri_debug_assert(
        !cap_is_in_bounds(cheri_get_current_pcc(env), addr, num_bytes));
    raise_pcc_fault(env, CapEx_LengthViolation);
}

void CHERI_HELPER_IMPL(raise_exception_ddc_perms(CPUArchState *env,
                                                 uint32_t required_perms))
{
    check_cap(env, cheri_get_ddc(env), required_perms,
              cap_get_base(cheri_get_ddc(env)), CHERI_EXC_REGNUM_DDC, 0,
              /*instavail=*/true, GETPC());
    error_report("%s should not return! DDC= " PRINT_CAP_FMTSTR, __func__,
                 PRINT_CAP_ARGS(cheri_get_ddc(env)));
    tcg_abort();
}

void CHERI_HELPER_IMPL(raise_exception_ddc_bounds(CPUArchState *env,
                                                     target_ulong addr,
                                                     uint32_t num_bytes))
{
    const cap_register_t *ddc = cheri_get_ddc(env);
    cheri_debug_assert(ddc->cr_tag && cap_is_unsealed(ddc) &&
                       "Should have been checked before bounds!");
    check_cap(env, ddc, 0, addr, CHERI_EXC_REGNUM_DDC, num_bytes,
              /*instavail=*/true, GETPC());
    error_report("%s should not return! DDC= " PRINT_CAP_FMTSTR, __func__,
                 PRINT_CAP_ARGS(cheri_get_ddc(env)));
    tcg_abort();
}
