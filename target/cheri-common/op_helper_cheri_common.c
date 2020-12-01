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

#ifdef DO_CHERI_STATISTICS

static DEFINE_CHERI_STAT(cgetpccsetoffset);
static DEFINE_CHERI_STAT(cgetpccincoffset);
static DEFINE_CHERI_STAT(cgetpccsetaddr);

#endif

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

void CHERI_HELPER_IMPL(pcc_check_bounds(CPUArchState *env, target_ulong addr,
                                        target_ulong num_bytes))
{
    const cap_register_t *pcc = cheri_get_recent_pcc(env);
    cheri_debug_assert(pcc->cr_tag && cap_is_unsealed(pcc) &&
                       "Should have been checked before bounds!");
    check_cap(env, pcc, 0, addr, CHERI_EXC_REGNUM_PCC, num_bytes,
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

void CHERI_HELPER_IMPL(cgetpccsetoffset(CPUArchState *env, uint32_t cd,
                                        target_ulong rs))
{
    // PCC.cursor does not need to be up-to-date here since we only look at the
    // base.
    uint64_t new_addr = rs + cap_get_base(cheri_get_recent_pcc(env));
    derive_cap_from_pcc(env, cd, new_addr, GETPC(), OOB_INFO(cgetpccsetoffset));
}

void CHERI_HELPER_IMPL(cgetpccincoffset(CPUArchState *env, uint32_t cd,
                                        target_ulong rs))
{
    uint64_t new_addr = rs + PC_ADDR(env);
    derive_cap_from_pcc(env, cd, new_addr, GETPC(), OOB_INFO(cgetpccincoffset));
}

// TODO: This is basically the riscv auipc again. Should probably refactor.
void CHERI_HELPER_IMPL(cgetpccsetaddr(CPUArchState *env, uint32_t cd,
                                      target_ulong rs))
{
    uint64_t new_addr = rs;
    derive_cap_from_pcc(env, cd, new_addr, GETPC(), OOB_INFO(cgetpccsetaddr));
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
     * CGetFlags: Move Flags to a General-Purpose Register.
     */
    return (target_ulong)get_readonly_capreg(env, cb)->cr_flags;
}

target_ulong CHERI_HELPER_IMPL(cgetlen(CPUArchState *env, uint32_t cb))
{
    /*
     * CGetLen: Move Length to a General-Purpose Register.
     *
     * Note: For 128-bit Capabilities we must handle len >= 2^64:
     * cap_get_length_sat() converts 1 << 64 to UINT64_MAX
     */
    return (target_ulong)cap_get_length_sat(get_readonly_capreg(env, cb));
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
    const target_long otype = cap_get_otype(cbp);
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

void cheri_jump_and_link(CPUArchState *env, const cap_register_t *target,
                         target_ulong addr, uint32_t link_reg,
                         target_ulong link_pc, uint32_t cjalr_flags)
{
    // FIXME: I am concerned that morello will want to jump to sealed things and
    // have the exception occur at the target.
    // FIXME: I am still not entirely sure of where morello takes its
    // exceptions.
    cheri_debug_assert(cap_is_unsealed(target) || cap_is_sealed_entry(target));
    cap_register_t next_pcc = *target;

    if (cap_is_sealed_entry(target)) {
        // If we are calling a "sentry" cap, remove the sealed flag
        cap_unseal_entry(&next_pcc);
        assert(cap_get_cursor(&next_pcc) == addr &&
               "Should have raised an exception");
    } else if (cjalr_flags & CJALR_MUST_BE_SENTRY) {
        // LETODO
        assert(0);
    } else {
        // Can never create an unrepresentable capability since we
        // bounds-checked the jump target.
        assert(is_representable_cap_with_addr(&next_pcc, addr) &&
               "Target addr must be representable");
        next_pcc._cr_cursor = addr;
    }

    // Don't generate a link capability if link_reg == zero register
    if (link_reg != NULL_CAPREG_INDEX) {
        // Note: PCC.cursor doesn't need to be up-to-date, TB start is fine
        // since we are writing a new cursor anyway.
        cap_register_t result = *cheri_get_recent_pcc(env);
        // can never create an unrepresentable capability since PCC must be in
        // bounds
        result._cr_cursor = link_pc;
        assert(is_representable_cap_with_addr(&result, link_pc) &&
               "Link addr must be representable");
        // The return capability should always be a sentry
        if (!(cjalr_flags & CJALR_DONT_MAKE_SENTRY)) {
            cap_make_sealed_entry(&result);
        }
        update_capreg(env, link_reg, &result);
    }
    update_next_pcc_for_tcg(env, &next_pcc, cjalr_flags);
}

void CHERI_HELPER_IMPL(cjalr(CPUArchState *env, uint32_t cd,
                             uint32_t cb_with_flags, target_ulong offset,
                             target_ulong link_pc))
{
    /*
     * CJALR: Jump and Link Capability Register
     */
    GET_HOST_RETPC();
    uint32_t cjalr_flags = cb_with_flags;
    uint32_t cb = cb_with_flags & HELPER_REG_MASK;

    const cap_register_t *cbp = get_readonly_capreg(env, cb);
    const target_ulong cursor = cap_get_cursor(cbp);
    const target_ulong addr = cursor + (target_long)offset;
    if (!cbp->cr_tag) {
        raise_cheri_exception(env, CapEx_TagViolation, cb);
    } else if (cap_is_sealed_with_type(cbp) ||
               (offset != 0 && !cap_is_unsealed(cbp))) {
        // Note: "sentry" caps can be called using cjalr, but only if the
        // immediate offset is 0.
        raise_cheri_exception(env, CapEx_SealViolation, cb);
    } else if (!cap_has_perms(cbp, CAP_PERM_EXECUTE)) {
        raise_cheri_exception(env, CapEx_PermitExecuteViolation, cb);
    } else if (!cap_has_perms(cbp, CAP_PERM_GLOBAL)) {
        raise_cheri_exception(env, CapEx_GlobalViolation, cb);
    } else if (!validate_jump_target(env, cbp, addr, cb,
                                     _host_return_address)) {
        assert(false && "Should have raised an exception");
    }

    cheri_jump_and_link(env, cbp, addr, cd, link_pc, cjalr_flags);
}

void CHERI_HELPER_IMPL(cinvoke(CPUArchState *env, uint32_t code_regnum,
                               uint32_t data_regnum))
{
    GET_HOST_RETPC();
    const cap_register_t *code_cap = get_readonly_capreg(env, code_regnum);
    const cap_register_t *data_cap = get_readonly_capreg(env, data_regnum);
    /*
     * CInvoke: Call into a new security domain (with matching otypes)
     */
    if (!code_cap->cr_tag) {
        raise_cheri_exception(env, CapEx_TagViolation, code_regnum);
    } else if (!data_cap->cr_tag) {
        raise_cheri_exception(env, CapEx_TagViolation, data_regnum);
    } else if (!cap_is_sealed_with_type(code_cap)) {
        raise_cheri_exception(env, CapEx_SealViolation, code_regnum);
    } else if (!cap_is_sealed_with_type(data_cap)) {
        raise_cheri_exception(env, CapEx_SealViolation, data_regnum);
    } else if ((code_cap->cr_otype != data_cap->cr_otype || code_cap->cr_otype > CAP_LAST_NONRESERVED_OTYPE)) {
        raise_cheri_exception(env, CapEx_TypeViolation, code_regnum);
    } else if (!cap_has_perms(code_cap, CAP_PERM_CINVOKE)) {
        raise_cheri_exception(env, CapEx_PermitCCallViolation, code_regnum);
    } else if (!cap_has_perms(data_cap, CAP_PERM_CINVOKE)) {
        raise_cheri_exception(env, CapEx_PermitCCallViolation, data_regnum);
    } else if (!cap_has_perms(code_cap, CAP_PERM_EXECUTE)) {
        raise_cheri_exception(env, CapEx_PermitExecuteViolation, code_regnum);
    } else if (cap_has_perms(data_cap, CAP_PERM_EXECUTE)) {
        raise_cheri_exception(env, CapEx_PermitExecuteViolation, data_regnum);
    } else if (!validate_jump_target(env, code_cap, cap_get_cursor(code_cap),
                                     code_regnum, _host_return_address)) {
        raise_cheri_exception(env, CapEx_LengthViolation, code_regnum);
    } else {
        // Unseal code and data cap now that the checks have succeeded.
        cap_register_t idc = *data_cap;
        cap_set_unsealed(&idc);
        cap_register_t target = *code_cap;
        cap_set_unsealed(&target);
        update_next_pcc_for_tcg(env, &target, 0);
        update_capreg(env, CINVOKE_DATA_REGNUM, &idc);
    }
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
    // In QEMU we do this by performing a csetbounds on a maximum permissions
    // capability and returning the resulting length
    cap_register_t tmpcap;
    set_max_perms_capability(&tmpcap, 0);
    CAP_cc(setbounds)(&tmpcap, 0, len);
    // Previously QEMU return (1<<64)-1 for a representable length of 1<<64
    // (similar to CGetLen), but all other implementations just strip the
    // high bit instead. Note: This allows a subsequent CSetBoundsExact to
    // succeed instead of trapping.
    // TODO: We may want to change CRRL to trap in this case. This could avoid
    //  potential bugs caused by accientally returning a zero-length capability.
    //  However, most code should already be guarding against large inputs so
    //  it is unclear if this makes much of a difference, and knowing that the
    //  instruction never traps could be useful for optimization purposes.
    // See also https://github.com/CTSRD-CHERI/cheri-architecture/issues/32
    return (target_ulong)cap_get_length_full(&tmpcap);
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
    // The mask used to align down is all ones followed by (required exponent
    // for compressed representation) zeroes
    target_ulong result = CAP_cc(get_alignment_mask)(len);
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
    } else if (cap_get_base(ctp) < cap_get_base(cbp)) {
        raise_cheri_exception(env, CapEx_LengthViolation, cb_exc);
    } else if (cap_get_top_full(ctp) > cap_get_top_full(cbp)) {
        raise_cheri_exception(env, CapEx_LengthViolation, cb_exc);
    } else if (cap_get_base(ctp) > cap_get_top_full(ctp)) {
        // check for length < 0 - possible because cs2 might be untagged
        raise_cheri_exception(env, CapEx_LengthViolation, ct);
    } else if ((ctp->cr_perms & cbp->cr_perms) != ctp->cr_perms) {
        raise_cheri_exception(env, CapEx_UserDefViolation, cb_exc);
    } else if ((ctp->cr_uperms & cbp->cr_uperms) != ctp->cr_uperms) {
        raise_cheri_exception(env, CapEx_UserDefViolation, cb_exc);
    } else if (ctp->cr_reserved) {
        // TODO: It would be nice to use a different exception code for this
        //  case but this should match Flute.
        // See also https://github.com/CTSRD-CHERI/cheri-architecture/issues/48
        raise_cheri_exception(env, CapEx_LengthViolation, ct);
    } else {
        cap_register_t result = *ctp;

        CAP_cc(cap_set_decompressed_cr_otype)(&result, CAP_OTYPE_UNSEALED);
        result.cr_tag = 1;

        /*
         * cbuildcap is allowed to seal at any ambiently-available otype,
         * subject to their construction conditions.  Otherwise, the result is
         * unsealed.
         */
        if (cap_is_sealed_entry(ctp) && cap_has_perms(ctp, CAP_PERM_EXECUTE)) {
          cap_make_sealed_entry(&result);
        }

        update_capreg(env, cd, &result);
    }
}

void CHERI_HELPER_IMPL(ccopytype(CPUArchState *env, uint32_t cd, uint32_t cb,
                                 uint32_t ct))
{
    GET_HOST_RETPC();
    const cap_register_t *cbp = get_readonly_capreg(env, cb);
    const cap_register_t *ctp = get_readonly_capreg(env, ct);
    if (!cbp->cr_tag) {
        raise_cheri_exception(env, CapEx_TagViolation, cb);
    } else if (is_cap_sealed(cbp)) {
        raise_cheri_exception(env, CapEx_SealViolation, cb);
    } else if (!cap_is_sealed_with_type(ctp)) {
        // For reserved otypes we return a null-derived value.
        cap_register_t result;
        update_capreg(env, cd, int_to_cap(cap_get_otype(ctp), &result));
    } else if (ctp->cr_otype < cap_get_base(cbp)) {
        raise_cheri_exception(env, CapEx_LengthViolation, cb);
    } else if (ctp->cr_otype >= cap_get_top(cbp)) {
        raise_cheri_exception(env, CapEx_LengthViolation, cb);
    } else {
        cap_register_t result = *cbp;
        result._cr_cursor = ctp->cr_otype;
        cheri_debug_assert(cap_is_representable(&result));
        update_capreg(env, cd, &result);
    }
}

static void cseal_common(CPUArchState *env, uint32_t cd, uint32_t cs,
                         uint32_t ct, bool conditional,
                         uintptr_t _host_return_address)
{
    const cap_register_t *csp = get_readonly_capreg(env, cs);
    const cap_register_t *ctp = get_readonly_capreg(env, ct);
    target_ulong ct_base_plus_offset = cap_get_cursor(ctp);
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
    } else if (conditional && !cap_is_unsealed(csp)) {
        update_capreg(env, cd, csp);
    } else if (conditional && !cap_cursor_in_bounds(ctp)) {
        update_capreg(env, cd, csp);
    } else if (conditional && cap_get_cursor(ctp) == CAP_OTYPE_UNSEALED_SIGNED) {
        update_capreg(env, cd, csp);
    } else if (!conditional && !cap_is_unsealed(csp)) {
        raise_cheri_exception(env, CapEx_SealViolation, cs);
    } else if (!cap_is_unsealed(ctp)) {
        raise_cheri_exception(env, CapEx_SealViolation, ct);
    } else if (!(ctp->cr_perms & CAP_PERM_SEAL)) {
        raise_cheri_exception(env, CapEx_PermitSealViolation, ct);
    } else if (!conditional && !cap_cursor_in_bounds(ctp)) {
        raise_cheri_exception(env, CapEx_LengthViolation, ct);
    } else if (ct_base_plus_offset > (target_ulong)CAP_LAST_NONRESERVED_OTYPE) {
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
    const target_ulong ct_cursor = cap_get_cursor(ctp);
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
    } else if (!cap_cursor_in_bounds(ctp)) {
        // Must be within bounds and not one past end (i.e. not equal to top).
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
#endif

static inline QEMU_ALWAYS_INLINE void
cincoffset_impl(CPUArchState *env, uint32_t cd, uint32_t cb, target_ulong rt,
                uintptr_t retpc,
                struct oob_stats_info *oob_info ATTRIBUTE_UNUSED)
{
#ifdef DO_CHERI_STATISTICS
    oob_info->num_uses++;
#endif
    const cap_register_t *cbp = get_readonly_capreg(env, cb);
    /*
     * CIncOffset: Increase Offset
     */
    if (unlikely(cbp->cr_tag && is_cap_sealed(cbp))) {
        raise_cheri_exception_impl(env, CapEx_SealViolation, cb, true, retpc);
    }
    target_ulong new_addr = cap_get_cursor(cbp) + rt;
    if (likely(addr_in_cap_bounds(cbp, new_addr))) {
        /* Common case: updating an in-bounds capability. */
        update_capreg_cursor_from(env, cd, cbp, cb, new_addr);
    } else {
        /* Result is out-of-bounds, check if it's representable. */
        if (unlikely(!is_representable_cap_with_addr(cbp, new_addr))) {
            if (cbp->cr_tag) {
                became_unrepresentable(env, cd, oob_info, retpc);
            }
            cap_register_t result = *cbp;
            cap_mark_unrepresentable(new_addr, &result);
            update_capreg(env, cd, &result);
        } else {
            /* out-of-bounds but still representable. */
            update_capreg_cursor_from(env, cd, cbp, cb, new_addr);
            check_out_of_bounds_stat(env, oob_info,
                                     get_readonly_capreg(env, cd),
                                     _host_return_address);
        }
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
        CAP_cc(cap_set_decompressed_cr_perms)(&result, cbp->cr_perms & rt_perms);
        CAP_cc(cap_set_decompressed_cr_uperms)(&result, cbp->cr_uperms & rt_uperms);
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

    bool not_null = (cb & CFROMPTR_0_IS_NOT_NULL) != 0;
    bool set_addr = (cb & CFROMPTR_SET_ADDR) != 0;
    cb &= HELPER_REG_MASK;

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
    if (!not_null && (rt == (target_ulong)0)) {
        cap_register_t result;
        update_capreg(env, cd, null_capability(&result));
    } else if (!cbp->cr_tag) {
        raise_cheri_exception(env, CapEx_TagViolation, cb_exc);
    } else if (is_cap_sealed(cbp)) {
        raise_cheri_exception(env, CapEx_SealViolation, cb_exc);
    } else {
        cap_register_t result = *cbp;
        target_ulong new_addr = cbp->cr_base + rt;
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
    target_ulong cursor = cap_get_cursor(cbp);
    cap_length_t new_top = (cap_length_t)cursor + length; // 65 bits
    /*
     * CSetBounds: Set Bounds
     */
    if (!cbp->cr_tag) {
        raise_cheri_exception(env, CapEx_TagViolation, cb);
    } else if (is_cap_sealed(cbp)) {
        raise_cheri_exception(env, CapEx_SealViolation, cb);
    } else if (cursor < cbp->cr_base) {
        raise_cheri_exception(env, CapEx_LengthViolation, cb);
    } else if (new_top > cap_get_top_full(cbp)) {
        raise_cheri_exception(env, CapEx_LengthViolation, cb);
    } else {
        cap_register_t result = *cbp;
        /*
         * With compressed capabilities we may need to increase the range of
         * memory addresses to be wider than requested so it is
         * representable.
         */
        const bool exact = CAP_cc(setbounds)(&result, cursor, new_top);
        if (!exact)
            env->statcounters_imprecise_setbounds++;
        if (must_be_exact && !exact) {
            raise_cheri_exception(env, CapEx_InexactBounds, cb);
            return;
        }
        assert(cap_is_representable(&result) &&
               "CSetBounds must create a representable capability");
        assert(result.cr_base >= cbp->cr_base &&
               "CSetBounds broke monotonicity (base)");
        assert(cap_get_length_full(&result) <= cap_get_length_full(cbp) &&
               "CSetBounds broke monotonicity (length)");
        assert(cap_get_top_full(&result) <= cap_get_top_full(cbp) &&
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
#ifndef TARGET_AARCH64
    // Morello thinks flags are something slightly different
    _Static_assert(CAP_FLAGS_ALL_BITS == 1, "Only one flag should exist");
#endif
    CAP_cc(cap_set_decompressed_cr_flags)(&result, flags);
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

target_ulong CHERI_HELPER_IMPL(cseqx(CPUArchState *env, uint32_t cb,
                                     uint32_t ct))
{
    const cap_register_t *cbp = get_readonly_capreg(env, cb);
    const cap_register_t *ctp = get_readonly_capreg(env, ct);

    return cap_exactly_equal(cbp, ctp);
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
    target_ulong cb_cursor = cap_get_cursor(cbp);
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
#elif defined(TARGET_RISCV) || defined(TARGET_AARCH64)
    // However, RISCV does not use this encoding and uses zero for the
    // null register (i.e. always trap).
    // The helpers can also be invoked from the explicitly DDC-relative
    // instructions with cb == CHERI_EXC_REGNUM_DDC which means DDC:
    return get_capreg_or_special(env, cb);
#else
#error "Wrong arch?"
#endif
}

static inline QEMU_ALWAYS_INLINE target_ulong cap_check_common(
    uint32_t required_perms, CPUArchState *env, uint32_t cb,
    target_ulong offset, uint32_t size, uintptr_t _host_return_address)
{
    const cap_register_t *cbp = get_load_store_base_cap(env, cb);
    return cap_check_common_reg(required_perms, env, cb, offset, size,
                                _host_return_address, cbp, size,
                                /*unaligned_handler=*/NULL);
}

/*
 * Load Via Capability Register
 */
target_ulong CHERI_HELPER_IMPL(cap_load_check(CPUArchState *env, uint32_t cb,
                                              target_ulong offset,
                                              uint32_t size))
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

    const target_ulong addr = cap_check_common_reg(
        perms_for_load(), env, cb, offset, CHERI_CAP_SIZE, _host_return_address,
        cbp, CHERI_CAP_SIZE, raise_unaligned_load_exception);

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

    const target_ulong addr =
        cap_check_common_reg(perms_for_store(env, cs), env, cb, offset,
                             CHERI_CAP_SIZE, _host_return_address, cbp,
                             CHERI_CAP_SIZE, raise_unaligned_store_exception);

    store_cap_to_memory(env, cs, addr, _host_return_address);
}

static inline bool
cheri_tag_prot_clear_or_trap(CPUArchState *env, target_ulong va,
                             int cb, const cap_register_t* cbp,
                             int prot, uintptr_t retpc, target_ulong tag)
{
    if (tag && (prot & PAGE_LC_CLEAR)) {
        qemu_maybe_log_instr_extra(env, "Clearing tag loaded from " TARGET_FMT_lx
            " due to MMU permissions\n", va);
        return 0;
    }
    if (tag && !(cbp->cr_perms & CAP_PERM_LOAD_CAP)) {
        qemu_maybe_log_instr_extra(env, "Clearing tag loaded from " TARGET_FMT_lx
            " due to missing CAP_PERM_LOAD_CAP\n", va);
        return 0;
    }
    if (tag && (prot & PAGE_LC_TRAP))
        raise_load_tag_exception(env, va, cb, retpc);
    return tag;
}

void squash_mutable_permissions(uint64_t *pesbt, const cap_register_t *source)
{
#ifdef TARGET_AARCH64
    if (!(source->cr_perms & CC128_PERM_MUTABLE_LOAD) &&
        (cc128_cap_pesbt_extract_OTYPE(*pesbt) == CAP_OTYPE_UNSEALED)) {
        *pesbt &= ~cc128_cap_pesbt_encode_HWPERMS(
            CC128_PERM_MUTABLE_LOAD | CC128_PERM_STORE_LOCAL |
            CC128_PERM_STORE_CAP | CC128_PERM_STORE);
    }
#endif
}

bool load_cap_from_memory_raw(CPUArchState *env, target_ulong *pesbt,
                              target_ulong *cursor, uint32_t cb,
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
#if TARGET_LONG_BITS == 32
#define ld_cap_word_p ldl_p
#elif TARGET_LONG_BITS == 64
#define ld_cap_word_p ldq_p
#else
#error "Unhandled target long width"
#endif
        *pesbt = ld_cap_word_p((char *)host + CHERI_MEM_OFFSET_METADATA) ^
                CAP_NULL_XOR_MASK;
        *cursor = ld_cap_word_p((char *)host + CHERI_MEM_OFFSET_CURSOR);
#undef ld_cap_word_p
    } else {
        // Slow path for e.g. IO regions.
        qemu_maybe_log_instr_extra(env, "Using slow path for load from guest "
            "address " TARGET_FMT_lx "\n", vaddr);
        *pesbt = cpu_ld_cap_word_ra(env, vaddr + CHERI_MEM_OFFSET_METADATA, retpc) ^
                CAP_NULL_XOR_MASK;
        *cursor = cpu_ld_cap_word_ra(env, vaddr + CHERI_MEM_OFFSET_CURSOR, retpc);
    }
    int prot;
    bool tag = cheri_tag_get(env, vaddr, cb, physaddr, &prot, retpc);
    if (tag) {
        tag = cheri_tag_prot_clear_or_trap(env, vaddr, cb, source, prot, retpc, tag);
        squash_mutable_permissions(pesbt, source);
    }

    env->statcounters_cap_read++;
    if (tag)
        env->statcounters_cap_read_tagged++;

#if defined(TARGET_RISCV) && defined(CONFIG_RVFI_DII)
    env->rvfi_dii_trace.MEM.rvfi_mem_addr = vaddr;
    env->rvfi_dii_trace.MEM.rvfi_mem_rdata[0] = *cursor;
    env->rvfi_dii_trace.MEM.rvfi_mem_rdata[1] = *pesbt;
    env->rvfi_dii_trace.MEM.rvfi_mem_rdata[2] = tag;
    env->rvfi_dii_trace.MEM.rvfi_mem_rmask = (1 << CHERI_CAP_SIZE) - 1;
    // TODO: Add one extra bit to include the tag?
    env->rvfi_dii_trace.available_fields |= RVFI_MEM_DATA;
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
        CAP_cc(decompress_raw)(*pesbt, *cursor, tag, &ncd);
        qemu_log_instr_ld_cap(env, vaddr, &ncd);
    }
#endif
    return tag;
}

void load_cap_from_memory(CPUArchState *env, uint32_t cd, uint32_t cb,
                          const cap_register_t *source, target_ulong vaddr,
                          target_ulong retpc, hwaddr *physaddr)
{
    target_ulong pesbt;
    target_ulong cursor;
    bool tag = load_cap_from_memory_raw(env, &pesbt, &cursor, cb, source, vaddr,
                                        retpc, physaddr);
    update_compressed_capreg(env, cd, pesbt, tag, cursor);
}

void store_cap_to_memory(CPUArchState *env, uint32_t cs,
                         target_ulong vaddr, target_ulong retpc)
{
    target_ulong cursor = get_capreg_cursor(env, cs);
    target_ulong pesbt_for_mem = get_capreg_pesbt(env, cs) ^ CAP_NULL_XOR_MASK;
#ifdef CONFIG_DEBUG_TCG
    if (get_capreg_state(cheri_get_gpcrs(env), cs) == CREG_INTEGER) {
        tcg_debug_assert(pesbt_for_mem == 0 && "Integer values should have NULL PESBT");
    }
#endif
    bool tag = get_capreg_tag_filtered(env, cs);
    if (cs == NULL_CAPREG_INDEX) {
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
        cheri_tag_invalidate_aligned(env, vaddr, retpc);
    }
    /* No TLB fault possible, should be safe to get a host pointer now */
    void* host = probe_write(env, vaddr, CHERI_CAP_SIZE, cpu_mmu_index(env, false), retpc);
    // When writing back pesbt we have to XOR with the NULL mask to ensure that
    // NULL capabilities have an all-zeroes representation.
    if (likely(host)) {
#if TARGET_LONG_BITS == 32
#define st_cap_word_p stl_p
#elif TARGET_LONG_BITS == 64
#define st_cap_word_p stq_p
#else
#error "Unhandled target long width"
#endif
        // Fast path, host address in TLB
        st_cap_word_p((char*)host + CHERI_MEM_OFFSET_METADATA, pesbt_for_mem);
        st_cap_word_p((char*)host + CHERI_MEM_OFFSET_CURSOR, cursor);
#undef st_cap_word_p
    } else {
        // Slow path for e.g. IO regions.
        qemu_maybe_log_instr_extra(env, "Using slow path for store to guest "
            "address " TARGET_FMT_lx "\n", vaddr);
        cpu_st_cap_word_ra(env, vaddr + CHERI_MEM_OFFSET_METADATA,
                           pesbt_for_mem, retpc);
        cpu_st_cap_word_ra(env, vaddr + CHERI_MEM_OFFSET_CURSOR, cursor,
                           retpc);
    }
#if defined(TARGET_RISCV) && defined(CONFIG_RVFI_DII)
    env->rvfi_dii_trace.MEM.rvfi_mem_addr = vaddr;
    env->rvfi_dii_trace.MEM.rvfi_mem_wdata[0] = cursor;
    env->rvfi_dii_trace.MEM.rvfi_mem_wdata[1] = pesbt_for_mem;
    env->rvfi_dii_trace.MEM.rvfi_mem_wdata[2] = tag;
    env->rvfi_dii_trace.MEM.rvfi_mem_wmask = (1 << CHERI_CAP_SIZE) - 1;
    // TODO: Add one extra bit to include the tag?
    env->rvfi_dii_trace.available_fields |= RVFI_MEM_DATA;
#endif
#if defined(CONFIG_TCG_LOG_INSTR)
    /* Log capability memory access as a single access */
    if (qemu_log_instr_enabled(env)) {
        /*
         * Decompress to log all fields
         * TODO(am2419): see notes on the load path on compression.
         */
        cap_register_t stored_cap;
        const target_ulong pesbt = pesbt_for_mem ^ CAP_NULL_XOR_MASK;
        CAP_cc(decompress_raw)(pesbt, cursor, tag, &stored_cap);
        cheri_debug_assert(cursor == cap_get_cursor(&stored_cap));
        qemu_log_instr_st_cap(env, vaddr, &stored_cap);
    }
#endif
}

QEMU_NORETURN static inline void raise_pcc_fault(CPUArchState *env,
                                                 CheriCapExcCause cause,
                                                 uintptr_t _host_return_address)
{
    cheri_debug_assert(pc_is_current(env));
    // Note: we set pc=0 since PC will have been saved prior to calling the
    // helper and we don't need to recompute it from the generated code.
    raise_cheri_exception_impl(env, cause, CHERI_EXC_REGNUM_PCC,
        /*instavail=*/true, _host_return_address);
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
    raise_pcc_fault(env, cause, GETPC());
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
    cheri_debug_assert(!cap_is_in_bounds(cheri_get_current_pcc(env), addr,
                                         num_bytes == 0 ? 1 : num_bytes));
    raise_pcc_fault(env, CapEx_LengthViolation, GETPC());
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

void CHERI_HELPER_IMPL(decompress_cap(CPUArchState *env, uint32_t regndx))
{
    get_readonly_capreg(env, regndx);
}
