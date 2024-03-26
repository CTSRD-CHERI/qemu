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
static DEFINE_CHERI_STAT(misc);

#endif

/*
 * To keep the refactor minimal we make use of a few ugly macros to change
 * exception behavior to tag clearing.
 * These semantics are used for Morello and CHERI-RISC-V, whereas (legacy)
 * CHERI-MIPS raises exceptions on invalid modifications.
 */
#define DEFINE_RESULT_VALID bool _cap_valid = true
#define RESULT_VALID        _cap_valid
#define raise_cheri_exception_or_invalidate(env, cause, reg)                   \
    do {                                                                       \
        if (CHERI_TAG_CLEAR_ON_INVALID(env))                                   \
            _cap_valid = false;                                                \
        else                                                                   \
            raise_cheri_exception(env, cause, reg);                            \
    } while (false)
#define raise_cheri_exception_or_invalidate_impl(env, cause, reg, pc)          \
    do {                                                                       \
        if (CHERI_TAG_CLEAR_ON_INVALID(env))                                   \
            _cap_valid = false;                                                \
        else                                                                   \
            raise_cheri_exception_impl(env, cause, reg, 0, true, pc);          \
    } while (false)
#define GET_HOST_RETPC_IF_TRAPPING_CHERI_ARCH() GET_HOST_RETPC()

static inline bool is_cap_sealed(const cap_register_t *cp)
{
    // TODO: remove this function and update all callers to use the correct
    // function
    return !cap_is_unsealed(cp);
}

// Try set cursor without changing bounds or modifying a sealed type
// On some architectures this will be an exception, on others it will be allowed
// but untag the result
static inline QEMU_ALWAYS_INLINE bool
try_set_cap_cursor(CPUArchState *env, const cap_register_t *cptr,
                   int regnum_src, int regnum_dst, target_ulong new_addr,
                   bool precise_repr_check, uintptr_t retpc,
                   struct oob_stats_info *oob_info)
{
    DEFINE_RESULT_VALID;
#ifdef DO_CHERI_STATISTICS
    oob_info->num_uses++;
#endif

    if (unlikely(cptr->cr_tag && is_cap_sealed(cptr))) {
        raise_cheri_exception_or_invalidate_impl(env, CapEx_SealViolation,
                                                 regnum_src, retpc);
    }
#ifndef TARGET_MORELLO
    /*
     * For Morello we can't just check for in-bounds since changing the sign
     * bit can affect representability. Additionally, the high bits are not
     * included in the capability bounds. Therefore, we skip this fast-path
     * optimzation for Morello and fall back to is_representable_cap_with_addr.
     */
    if (likely(addr_in_cap_bounds(cptr, new_addr))) {
        /* Common case: updating an in-bounds capability. */
        update_capreg_cursor_from(env, regnum_dst, cptr, regnum_src, new_addr,
                                  !RESULT_VALID);
        return RESULT_VALID;
    }
    /* Result is out-of-bounds, check if it's representable. */
#endif
    if (unlikely(!CAP_cc(is_representable_with_addr)(cptr, new_addr,
                                                     precise_repr_check))) {
        if (cptr->cr_tag) {
            became_unrepresentable(env, regnum_dst, oob_info, retpc);
        }
        cap_register_t result = *cptr;
        cap_mark_unrepresentable(new_addr, &result);
        update_capreg(env, regnum_dst, &result);
    } else {
        /* (Possibly) out-of-bounds but still representable. */
        update_capreg_cursor_from(env, regnum_dst, cptr, regnum_src, new_addr,
                                  !RESULT_VALID);
        check_out_of_bounds_stat(env, oob_info,
                                 get_readonly_capreg(env, regnum_dst),
                                 _host_return_address);
    }
    return RESULT_VALID;
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

#ifdef TARGET_AARCH64
void CHERI_HELPER_IMPL(ddc_check_bounds_store(CPUArchState *env,
                                              target_ulong addr,
                                              target_ulong num_bytes))
{
    const cap_register_t *ddc = cheri_get_ddc(env);
    cheri_debug_assert(ddc->cr_tag && cap_is_unsealed(ddc) &&
                       "Should have been checked before bounds!");
    check_cap(env, ddc, CAP_PERM_STORE, addr, CHERI_EXC_REGNUM_DDC, num_bytes,
              /*instavail=*/true, GETPC());
}
#endif

void CHERI_HELPER_IMPL(pcc_check_bounds(CPUArchState *env, target_ulong addr,
                                        target_ulong num_bytes))
{
    const cap_register_t *pcc = cheri_get_recent_pcc(env);
    cheri_debug_assert(pcc->cr_tag && cap_is_unsealed(pcc) &&
                       "Should have been checked before bounds!");
    check_cap(env, pcc, 0, addr, CHERI_EXC_REGNUM_PCC, num_bytes,
              /*instavail=*/true, GETPC());
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
                                             target_ulong vaddr,
                                             MemOpIdx oi))
{
    cheri_tag_invalidate(env, vaddr, memop_size(get_memop(oi)), GETPC(),
                         get_mmuidx(oi));
}

/*
 * Use this for conditional clear when needing to avoid a branch in the TCG
 * backend.
 */
void CHERI_HELPER_IMPL(cheri_invalidate_tags_condition(
    CPUArchState *env, target_ulong vaddr, MemOpIdx oi, uint32_t cond))
{
    if (cond) {
        cheri_tag_invalidate(env, vaddr, memop_size(get_memop(oi)), GETPC(),
                             get_mmuidx(oi));
    }
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

target_ulong CHERI_HELPER_IMPL(cgethigh(CPUArchState *env, uint32_t cb))
{
    /*
     * CGetHigh: Move Metadata bits to a General-Purpose Register
     * TODO: could do this directly from TCG now.
     */
    return CAP_cc(compress_mem)(get_readonly_capreg(env, cb));
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
    target_ulong perms = cap_get_all_perms(cbp);
    cheri_debug_assert(((perms & CAP_VALID_PERM_BITS) == perms) &&
                       "Unknown permission bits set!");
#ifdef TARGET_CHERI_RISCV_STD_093
    /* The reserved 1-bits were not present in 0.9.3, zero them */
    perms &= ~(CAP_CC(PERMS_RESERVED_ONES));
#endif
    return perms;
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
    return cap_is_unsealed(cbp) ? 0 : 1;
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
    const target_long otype = cap_get_otype_signext(cbp);
#if defined(TARGET_MORELLO) || defined(TARGET_CHERI_RISCV_STD)
    cheri_debug_assert(otype == cap_get_otype_unsigned(cbp));
#else
    // Must be either a valid positive type < maximum or one of the special
    // hardware-interpreted otypes
    if (otype < 0) {
        cheri_debug_assert(
            (cap_is_unsealed(cbp) || cap_is_sealed_with_reserved_otype(cbp)) &&
            "all negative return values are used for reserved otypes.");
    } else {
        cheri_debug_assert(
            cap_is_sealed_with_type(cbp) &&
            "non-negative return values are used for non-reserved otypes");
    }
#endif
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
    cap_register_t next_pcc = *target;

#if CHERI_CONTROLFLOW_CHECK_AT_TARGET
    update_target_for_jump(env, &next_pcc, cjalr_flags);
#else
    cheri_debug_assert(cap_is_unsealed(target) || cap_is_sealed_entry(target));
#endif

    if (next_pcc.cr_tag && cap_is_sealed_entry(&next_pcc)) {
        // If we are calling a "sentry" cap, remove the sealed flag
        cap_unseal_entry(&next_pcc);
        assert(cap_get_cursor(&next_pcc) == addr &&
               "Should have raised an exception");
    } else if (cjalr_flags & CJALR_MUST_BE_SENTRY) {
        qemu_log_mask_and_addr(CPU_LOG_INSTR | LOG_GUEST_ERROR,
                       cpu_get_recent_pc(env),
                       "Requested jump to sentry but got invalid cap."
                       "\n  Current PCC: " PRINT_CAP_FMTSTR
                       "\n  Target cap: " PRINT_CAP_FMTSTR  "\n",
                       PRINT_CAP_ARGS(cheri_get_recent_pcc(env)),
                       PRINT_CAP_ARGS(target));
        next_pcc.cr_tag = 0;
    } else {
        /*
         * For RISC-V This can never create an unrepresentable capability since
         * we bounds-checked the jump target. However, Morello performs the
         * checks after the jump, so we can't unconditionally assert that the
         * result is representable.
         */
#if !CHERI_CONTROLFLOW_CHECK_AT_TARGET
        assert(is_representable_cap_with_addr(&next_pcc, addr) &&
               "Target addr must be representable");
#endif
        cap_set_cursor(&next_pcc, addr);
    }

    // Don't generate a link capability if link_reg == zero register
    if (link_reg != NULL_CAPREG_INDEX) {
        // Note: PCC.cursor doesn't need to be up-to-date, TB start is fine
        // since we are writing a new cursor anyway.
        cap_register_t result = *cheri_get_recent_pcc(env);
        // can never create an unrepresentable capability since PCC must be in
        // bounds
#ifdef TARGET_AARCH64
        // Encode C64 state here (we could also bake this in to the tcg, but
        // would then need to remember to do it everywhere)
        if (env->pstate & PSTATE_C64)
            link_pc |= 1;
#endif
        result._cr_cursor = link_pc;
        assert(is_representable_cap_with_addr(&result, link_pc) &&
               "Link addr must be representable");
        // The return capability should always be a sentry
        if (!(cjalr_flags & CJALR_DONT_MAKE_SENTRY)) {
            cap_make_sealed_entry(&result);
        }
        update_capreg(env, link_reg, &result);
    }
    if (!next_pcc.cr_tag) {
        qemu_log_mask_and_addr(CPU_LOG_INSTR | LOG_GUEST_ERROR,
                       cpu_get_recent_pc(env),
                       "Jumping to untagged capability."
                       "\n  Current PCC: " PRINT_CAP_FMTSTR
                       "\n  Target cap: " PRINT_CAP_FMTSTR  "\n",
                       PRINT_CAP_ARGS(cheri_get_recent_pcc(env)),
                       PRINT_CAP_ARGS(target));
    }
    update_next_pcc_for_tcg(env, &next_pcc, cjalr_flags);
}

void cheri_jump_and_link_checked(CPUArchState *env, uint32_t link_reg,
                                 target_ulong link_pc, uint32_t target_reg,
                                 const cap_register_t *target,
                                 target_ulong target_addr, uint32_t flags,
                                 uintptr_t _host_return_address)
{
#ifdef TARGET_RISCV
    /* On RISC-V we mask the LSB of the target to match JALR behaviour. */
    target_addr &= ~(target_ulong)1;
#endif
    /* Morello takes the exception at the target. */
#if !CHERI_CONTROLFLOW_CHECK_AT_TARGET
    if (!target->cr_tag) {
        raise_cheri_exception_branch(env, CapEx_TagViolation, target_reg);
    } else if (cap_is_sealed_with_type(target) ||
               (!cap_is_unsealed(target) &&
                target_addr != cap_get_cursor(target))) {
        /*
         * Note: "sentry" caps can be called using cjalr, but only if the
         * immediate offset is 0, i.e. target_addr==target.address.
         */
        raise_cheri_exception_branch(env, CapEx_SealViolation, target_reg);
    } else if (!cap_has_perms(target, CAP_PERM_EXECUTE)) {
        raise_cheri_exception_branch(env, CapEx_PermitExecuteViolation,
                                     target_reg);
    } else if (!validate_jump_target(env, target, target_addr, target_reg,
                                     _host_return_address)) {
        assert(false && "Should have raised an exception");
    }
#endif
    cheri_jump_and_link(env, target, target_addr, link_reg, link_pc, flags);
}

void CHERI_HELPER_IMPL(cjalr(CPUArchState *env, uint32_t cd,
                             uint32_t cb_with_flags, target_ulong offset,
                             target_ulong link_pc))
{
    /*
     * CJALR: Jump and Link Capability Register
     */
    uint32_t cjalr_flags = cb_with_flags;
    uint32_t cb = cb_with_flags & HELPER_REG_MASK;

    const cap_register_t *cbp = get_readonly_capreg(env, cb);
    const target_ulong cursor = cap_get_cursor(cbp);
    const target_ulong target_addr = cursor + (target_long)offset;
    cheri_jump_and_link_checked(env, cd, link_pc, cb, cbp, target_addr,
                                cjalr_flags, GETPC());
}

#if CHERI_HAS_OTYPE_SEALING
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
        raise_cheri_exception_branch(env, CapEx_TagViolation, code_regnum);
    } else if (!data_cap->cr_tag) {
        raise_cheri_exception_branch(env, CapEx_TagViolation, data_regnum);
    } else if (!cap_is_sealed_with_type(code_cap)) {
        raise_cheri_exception_branch(env, CapEx_SealViolation, code_regnum);
    } else if (!cap_is_sealed_with_type(data_cap)) {
        raise_cheri_exception_branch(env, CapEx_SealViolation, data_regnum);
    } else if (cap_get_otype_unsigned(code_cap) !=
                   cap_get_otype_unsigned(data_cap) ||
               !cap_is_sealed_with_type(code_cap)) {
        raise_cheri_exception_branch(env, CapEx_TypeViolation, code_regnum);
    } else if (!cap_has_perms(code_cap, CAP_PERM_CINVOKE)) {
        raise_cheri_exception_branch(env, CapEx_PermitCCallViolation,
                                     code_regnum);
    } else if (!cap_has_perms(data_cap, CAP_PERM_CINVOKE)) {
        raise_cheri_exception_branch(env, CapEx_PermitCCallViolation,
                                     data_regnum);
    } else if (!cap_has_perms(code_cap, CAP_PERM_EXECUTE)) {
        raise_cheri_exception_branch(env, CapEx_PermitExecuteViolation,
                                     code_regnum);
    } else if (cap_has_perms(data_cap, CAP_PERM_EXECUTE)) {
        raise_cheri_exception_branch(env, CapEx_PermitExecuteViolation,
                                     data_regnum);
    } else if (!validate_jump_target(env, code_cap, cap_get_cursor(code_cap),
                                     code_regnum, _host_return_address)) {
        raise_cheri_exception_branch(env, CapEx_LengthViolation, code_regnum);
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
#endif

void CHERI_HELPER_IMPL(cmove(CPUArchState *env, uint32_t cd, uint32_t cb))
{
    /*
     * CMove: Move Capability to another Register
     */
    // TODO: could do this without decompressing.
    const cap_register_t *cbp = get_readonly_capreg(env, cb);
    update_capreg(env, cd, cbp);
}

void CHERI_HELPER_IMPL(csealentry(CPUArchState *env, uint32_t cd, uint32_t cs))
{
    /*
     * CSealEntry: Seal a code capability so it is only callable with cjr/cjalr
     * (all other permissions are ignored so it can't be used for loads, etc)
     */
    GET_HOST_RETPC_IF_TRAPPING_CHERI_ARCH();
    DEFINE_RESULT_VALID;
    const cap_register_t *csp = get_readonly_capreg(env, cs);
    if (!csp->cr_tag) {
        raise_cheri_exception_or_invalidate(env, CapEx_TagViolation, cs);
    } else if (!cap_is_unsealed(csp)) {
        raise_cheri_exception_or_invalidate(env, CapEx_SealViolation, cs);
    }
#ifdef TARGET_MIPS
    /*
     * The legacy MIPS testsuite expects traps when attempting to seal
     * non-executable capabilities with CSealEntry.
     * Keep this code rather than fixing the MIPS testsuite. We can GC this
     * workaround once we completely drop MIPS support (most likely once we
     * have a CHERI-RISC-V testsuite).
     */
    if (!cap_has_perms(csp, CAP_PERM_EXECUTE)) {
        raise_cheri_exception(env, CapEx_PermitExecuteViolation, cs);
    }
#endif
    cap_register_t result = *csp;
    if (!RESULT_VALID) {
        result.cr_tag = 0;
    }
    /* NB: Not using `cap_make_sealed_entry` since the input can be untagged. */
    CAP_cc(update_otype)(&result, CAP_OTYPE_SENTRY);
    /* Capability can now only be used in cjr/cjalr (or rederived). */
    update_capreg(env, cd, &result);
}

/// Two operands (capability and int)
void CHERI_HELPER_IMPL(ccheckperm(CPUArchState *env, uint32_t cs,
                                  target_ulong rt))
{
    GET_HOST_RETPC();
    const cap_register_t *csp = get_readonly_capreg(env, cs);
    /*
     * CCheckPerm: Raise exception if don't have permission
     */
    if (!csp->cr_tag) {
        raise_cheri_exception(env, CapEx_TagViolation, cs);
    } else if ((cap_get_all_perms(csp) & rt) != rt) {
        raise_cheri_exception(env, CapEx_UserDefViolation, cs);
    }
}

/// Two operands (int int)

static target_ulong crap_impl(CPUArchState *env, target_ulong len)
{
    // In QEMU we do this by performing a csetbounds on a maximum permissions
    // capability and returning the resulting length
    cap_register_t tmpcap;
    set_max_perms_capability(env, &tmpcap, 0);
    CAP_cc(setbounds)(&tmpcap, len);
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
    return crap_impl(env, len);
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
    target_ulong rounded_with_crap = crap_impl(env, len);
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
    GET_HOST_RETPC_IF_TRAPPING_CHERI_ARCH();
    DEFINE_RESULT_VALID;
    const cap_register_t *ctp = get_readonly_capreg(env, ct);
    cap_register_t result = *ctp;

#ifdef TARGET_CHERI_RISCV_STD
    /* New behaviour in the RISC-V standard (no longer reads ddc): */
    if (cb == 0) {
        /* If cs1 is the NULL register, we copy cs2 to cd and clear cd's tag. */
        result.cr_tag = false;
        update_capreg(env, cd, &result);
        return;
    }
#endif
    const cap_register_t *cbp = get_capreg_0_is_ddc(env, cb);
    if (!cbp->cr_tag) {
        raise_cheri_exception_or_invalidate(env, CapEx_TagViolation, cb);
    } else if (!cbp->cr_bounds_valid) {
        /* Malformed bounds should raise a length violation. */
        raise_cheri_exception_or_invalidate(env, CapEx_LengthViolation, cb);
    } else if (!cap_is_unsealed(cbp)) {
        raise_cheri_exception_or_invalidate(env, CapEx_SealViolation, cb);
    } else if (cap_get_base(ctp) < cap_get_base(cbp)) {
        raise_cheri_exception_or_invalidate(env, CapEx_LengthViolation, cb);
    } else if (cap_get_top_full(ctp) > cap_get_top_full(cbp)) {
        raise_cheri_exception_or_invalidate(env, CapEx_LengthViolation, cb);
    } else if (cap_get_base(ctp) > cap_get_top_full(ctp)) {
        // check for length < 0 - possible because cs2 might be untagged
        raise_cheri_exception_or_invalidate(env, CapEx_LengthViolation, ct);
    } else if ((cap_get_all_perms(ctp) & cap_get_all_perms(cbp)) !=
               cap_get_all_perms(ctp)) {
        raise_cheri_exception_or_invalidate(env, CapEx_UserDefViolation, cb);
    } else if (cap_has_invalid_perms_encoding(env, ctp)) {
        raise_cheri_exception_or_invalidate(env, CapEx_UserDefViolation, ct);
    } else if (cap_has_reserved_bits_set(ctp)) {
        raise_cheri_exception_or_invalidate(env, CapEx_LengthViolation, ct);
    }

    if (!RESULT_VALID) {
        result.cr_tag = 0; /* Not a valid subset. */
    } else {
        /* Check if the capability bounds are canonical by deriving. */
        cap_register_t derived = *cbp;
        assert(!cap_has_reserved_bits_set(&derived));
        if (!cap_is_unsealed(&derived)) {
            derived.cr_tag = 0;
        }
        cap_set_cursor(&derived, cap_get_base(&result));
        CAP_cc(setbounds)(&derived, cap_get_length_full(&result));
        cap_set_cursor(&derived, cap_get_cursor(&result));
        cap_set_perms(env, &derived,
                      cap_get_all_perms(cbp) & cap_get_all_perms(ctp));
#ifndef TARGET_AARCH64
        cap_set_exec_mode(&derived, cap_get_exec_mode(ctp));
#endif
        if (cap_is_sealed_entry(ctp)) {
            cap_make_sealed_entry(&derived);
        }
        result.cr_tag = 1; /* Set tag to true for comparison with derived. */
        if (cap_exactly_equal(&result, &derived)) {
            /*
             * If this was a valid derivation sequence return that to ensure
             * canonical bounds encoding.
             */
            result = derived;
        } else {
            /* Valid subset but not canonical -> return the untagged input. */
            result.cr_tag = 0;
        }
    }
    update_capreg(env, cd, &result);
}

#if CHERI_HAS_OTYPE_SEALING
void CHERI_HELPER_IMPL(ccopytype(CPUArchState *env, uint32_t cd, uint32_t cb,
                                 uint32_t ct))
{
    GET_HOST_RETPC_IF_TRAPPING_CHERI_ARCH();
    DEFINE_RESULT_VALID;
    const cap_register_t *cbp = get_readonly_capreg(env, cb);
    const cap_register_t *ctp = get_readonly_capreg(env, ct);
    if (!cbp->cr_tag) {
        raise_cheri_exception_or_invalidate(env, CapEx_TagViolation, cb);
    } else if (is_cap_sealed(cbp)) {
        raise_cheri_exception_or_invalidate(env, CapEx_SealViolation, cb);
    }
    if (cap_is_sealed_with_reserved_otype(ctp) || cap_is_unsealed(ctp)) {
        if (CHERI_TAG_CLEAR_ON_INVALID(env)) {
            RESULT_VALID = false;
        } else {
            /* For reserved otypes we return a null-derived value. */
            cap_register_t result =
                make_capability_from_int(env, cap_get_otype_signext(ctp));
            update_capreg(env, cd, &result);
            return;
        }
    }
    if (cap_get_otype_unsigned(ctp) < cap_get_base(cbp)) {
        raise_cheri_exception_or_invalidate(env, CapEx_LengthViolation, cb);
    } else if (cap_get_otype_unsigned(ctp) >= cap_get_top(cbp)) {
        raise_cheri_exception_or_invalidate(env, CapEx_LengthViolation, cb);
    }
    cap_register_t result = *cbp;
    if (!RESULT_VALID) {
        result.cr_tag = 0;
    }
    try_set_cap_cursor(env, &result, cb, cd, cap_get_otype_signext(ctp),
                       /*precise_repr_check=*/true, GETPC(),
                       OOB_INFO(ccopytype));
}

static void cseal_common(CPUArchState *env, uint32_t cd, uint32_t cs,
                         uint32_t ct, bool conditional,
                         uintptr_t _host_return_address)
{
    DEFINE_RESULT_VALID;
    const cap_register_t *csp = get_readonly_capreg(env, cs);
    const cap_register_t *ctp = get_readonly_capreg(env, ct);
    target_ulong ct_base_plus_offset = cap_get_cursor(ctp);
    /*
     * CSeal: Seal a capability
     */
    if (!ctp->cr_tag) {
        if (conditional) {
            update_capreg(env, cd, csp);
            return;
        }
        raise_cheri_exception_or_invalidate(env, CapEx_TagViolation, ct);
    } else if (!csp->cr_tag) {
        raise_cheri_exception_or_invalidate(env, CapEx_TagViolation, cs);
    } else if (conditional && !cap_is_unsealed(csp)) {
        update_capreg(env, cd, csp);
        return;
    } else if (conditional && !cap_cursor_in_bounds(ctp)) {
        update_capreg(env, cd, csp);
        return;
    } else if (conditional &&
               cap_get_cursor(ctp) == CAP_OTYPE_UNSEALED_SIGNED) {
        update_capreg(env, cd, csp);
        return;
    } else if (!conditional && !cap_is_unsealed(csp)) {
        raise_cheri_exception_or_invalidate(env, CapEx_SealViolation, cs);
    } else if (!cap_is_unsealed(ctp)) {
        raise_cheri_exception_or_invalidate(env, CapEx_SealViolation, ct);
    } else if (!cap_has_perms(ctp, CAP_PERM_SEAL)) {
        raise_cheri_exception_or_invalidate(env, CapEx_PermitSealViolation, ct);
    } else if (!conditional && !cap_cursor_in_bounds(ctp)) {
        raise_cheri_exception_or_invalidate(env, CapEx_LengthViolation, ct);
    } else if (ct_base_plus_offset > CAP_MAX_REPRESENTABLE_OTYPE ||
               cap_otype_is_reserved(ct_base_plus_offset)) {
        raise_cheri_exception_or_invalidate(env, CapEx_LengthViolation, ct);
    } else if (!is_representable_cap_with_addr(csp, cap_get_cursor(csp))) {
        raise_cheri_exception_or_invalidate(env, CapEx_InexactBounds, cs);
    }
    cap_register_t result = *csp;
    if (!RESULT_VALID) {
        result.cr_tag = false;
        uint32_t new_otype = (uint32_t)ct_base_plus_offset;
        new_otype &= CAP_OTYPE_ALL_BITS;
        CAP_cc(update_otype)(&result, new_otype);
    } else {
        cap_set_sealed(&result, (uint32_t)ct_base_plus_offset);
    }
    update_capreg(env, cd, &result);
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
    GET_HOST_RETPC_IF_TRAPPING_CHERI_ARCH();
    DEFINE_RESULT_VALID;
    const cap_register_t *csp = get_readonly_capreg(env, cs);
    const cap_register_t *ctp = get_readonly_capreg(env, ct);
    const target_ulong ct_cursor = cap_get_cursor(ctp);
    /*
     * CUnseal: Unseal a sealed capability
     */
    if (!csp->cr_tag) {
        raise_cheri_exception_or_invalidate(env, CapEx_TagViolation, cs);
    } else if (!ctp->cr_tag) {
        raise_cheri_exception_or_invalidate(env, CapEx_TagViolation, ct);
    } else if (cap_is_unsealed(csp)) {
        raise_cheri_exception_or_invalidate(env, CapEx_SealViolation, cs);
    } else if (!cap_is_unsealed(ctp)) {
        raise_cheri_exception_or_invalidate(env, CapEx_SealViolation, ct);
    } else if (!cap_is_sealed_with_type(csp)) {
        /* Reserved otypes are not allowed. */
        raise_cheri_exception_or_invalidate(env, CapEx_TypeViolation, cs);
    } else if (ct_cursor != cap_get_otype_unsigned(csp)) {
        raise_cheri_exception_or_invalidate(env, CapEx_TypeViolation, ct);
    } else if (!cap_has_perms(ctp, CAP_PERM_UNSEAL)) {
        raise_cheri_exception_or_invalidate(env, CapEx_PermitUnsealViolation,
                                            ct);
    } else if (!cap_cursor_in_bounds(ctp)) {
        /* Must be in bounds and not one past end (i.e. not equal to top). */
        raise_cheri_exception_or_invalidate(env, CapEx_LengthViolation, ct);
    } else if (ct_cursor > CAP_MAX_REPRESENTABLE_OTYPE ||
               cap_otype_is_reserved(ct_cursor)) {
        /* This should never happen due to the ct_cursor != cs_otype check. */
        raise_cheri_exception_or_invalidate(env, CapEx_LengthViolation, ct);
    }

    cap_register_t result = *csp;
    target_ulong new_perms = cap_get_all_perms(&result);
    if (cap_has_perms(csp, CAP_PERM_GLOBAL) &&
        cap_has_perms(ctp, CAP_PERM_GLOBAL)) {
        new_perms |= CAP_PERM_GLOBAL;
    } else {
        new_perms &= ~CAP_PERM_GLOBAL;
    }
    cap_set_perms(env, &result, new_perms);
    if (RESULT_VALID) {
        cap_set_unsealed(&result);
    } else {
        result.cr_tag = 0; /* Detag on invalid input/argument. */
        CAP_cc(update_otype)(&result, CAP_OTYPE_UNSEALED);
    }
    update_capreg(env, cd, &result);
}
#endif /* CHERI_HAS_OTYPE_SEALING */

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
                uintptr_t retpc, struct oob_stats_info *oob_info)
{
    const cap_register_t *cbp = get_readonly_capreg(env, cb);
    /*
     * CIncOffset: Increase Offset
     */
    target_ulong new_addr = cap_get_cursor(cbp) + rt;
    /*
     * CIncOffset and CSetOffset use the approximate fast representability
     * check rather than a precise one.
     */
    try_set_cap_cursor(env, cbp, cb, cd, new_addr,
                       /*precise_repr_check=*/false, retpc, oob_info);
}

void CHERI_HELPER_IMPL(candperm(CPUArchState *env, uint32_t cd, uint32_t cb,
                                target_ulong rt))
{
    GET_HOST_RETPC_IF_TRAPPING_CHERI_ARCH();
    DEFINE_RESULT_VALID;
    const cap_register_t *cbp = get_readonly_capreg(env, cb);
    /*
     * CAndPerm: Restrict Permissions
     */
    if (!cbp->cr_tag) {
        raise_cheri_exception_or_invalidate(env, CapEx_TagViolation, cb);
    } else if (!cap_is_unsealed(cbp)) {
        raise_cheri_exception_or_invalidate(env, CapEx_SealViolation, cb);
    }

    cap_register_t result = *cbp;
    if (!RESULT_VALID) {
        result.cr_tag = 0;
    }
    target_ulong new_perms = cap_get_all_perms(cbp) & rt;
    /* Ensure that the permission can be encoded */
    cap_legalize_perms(env, &result, &new_perms);
#ifdef TARGET_CHERI_RISCV_STD
    /* If the execution mode is no longer encodable with X removed, strip it. */
    CheriExecMode mode = cap_get_exec_mode(cbp);
    if (fix_up_exec_mode(env, &mode, new_perms)) {
        cap_set_exec_mode(&result, mode);
    }
#endif
    cap_set_perms(env, &result, new_perms);
    update_capreg(env, cd, &result);
}

void CHERI_HELPER_IMPL(cincoffset(CPUArchState *env, uint32_t cd, uint32_t cb,
                                  target_ulong rt))
{
    cincoffset_impl(env, cd, cb, rt, GETPC(), OOB_INFO(cincoffset));
}

void CHERI_HELPER_IMPL(candaddr(CPUArchState *env, uint32_t cd, uint32_t cb,
                                target_ulong rt))
{
    target_ulong cursor = get_capreg_cursor(env, cb);
    target_ulong target_addr = cursor & rt;
    try_set_cap_cursor(env, get_readonly_capreg(env, cb), cb, cd, target_addr,
                       /*precise_repr_check=*/true, GETPC(),
                       OOB_INFO(candaddr));
}

void CHERI_HELPER_IMPL(csetaddr(CPUArchState *env, uint32_t cd, uint32_t cb,
                                target_ulong target_addr))
{
    try_set_cap_cursor(env, get_readonly_capreg(env, cb), cb, cd, target_addr,
                       /*precise_repr_check=*/true, GETPC(),
                       OOB_INFO(csetaddr));
}

void CHERI_HELPER_IMPL(csethigh(CPUArchState *env, uint32_t cd, uint32_t cs1,
                                target_ulong new_mem_pesbt))
{
    cap_register_t result;
    CAP_cc(decompress_mem)(new_mem_pesbt, get_capreg_cursor(env, cs1), false,
                           &result);
    result.cr_extra = CREG_FULLY_DECOMPRESSED;
    update_capreg(env, cd, &result);
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
    DEFINE_RESULT_VALID;
#ifdef DO_CHERI_STATISTICS
    OOB_INFO(cfromptr)->num_uses++;
#endif
    // CFromPtr traps on cbp == NULL so we use reg0 as $ddc to save encoding
    // space (and for backwards compat with old binaries).
    // Note: This is also still required for new binaries since clang assumes it
    // can use zero as $ddc in cfromptr/ctoptr
    const cap_register_t *cbp = get_capreg_0_is_ddc(env, cb);
    /*
     * CFromPtr: Create capability from pointer
     */
    if (rt == (target_ulong)0) {
        cap_register_t result = make_null_capability(env);
        update_capreg(env, cd, &result);
        return;
    } else if (!cbp->cr_tag) {
        raise_cheri_exception_or_invalidate(env, CapEx_TagViolation, cb);
    } else if (is_cap_sealed(cbp)) {
        raise_cheri_exception_or_invalidate(env, CapEx_SealViolation, cb);
    }
    cap_register_t result = *cbp;
    if (!RESULT_VALID) {
        result.cr_tag = 0; /* Detag sealed inputs  */
    }
    target_ulong new_addr = cbp->cr_base + rt;
    if (!is_representable_cap_with_addr(cbp, new_addr)) {
        if (cbp->cr_tag) {
            became_unrepresentable(env, cd, OOB_INFO(cfromptr),
                                   _host_return_address);
        }
        cap_mark_unrepresentable(new_addr, &result);
    } else {
        result._cr_cursor = new_addr;
        check_out_of_bounds_stat(env, OOB_INFO(cfromptr), &result,
                                 _host_return_address);
    }
    update_capreg(env, cd, &result);
}

static void do_setbounds(bool must_be_exact, CPUArchState *env, uint32_t cd,
                         uint32_t cb, target_ulong length,
                         uintptr_t _host_return_address)
{
    const cap_register_t *cbp = get_readonly_capreg(env, cb);

    DEFINE_RESULT_VALID;
    cap_register_t result = *cbp;
    /*
     * CSetBounds: Set Bounds
     */
    bool exact;
    if (!CHERI_TAG_CLEAR_ON_INVALID(env)) {
        /*
         * The setbounds call will invalidate any results with larger bounds
         * than the input, but for trapping architectures we still need to
         * perform these checks here.
         */
        if (!cbp->cr_tag) {
            raise_cheri_exception(env, CapEx_TagViolation, cb);
        } else if (is_cap_sealed(cbp)) {
            raise_cheri_exception(env, CapEx_SealViolation, cb);
        } else if (!cap_is_in_bounds(cbp, cap_get_cursor(cbp), length)) {
            raise_cheri_exception(env, CapEx_LengthViolation, cb);
        }
        /* Use checked_setbounds to ensure we didn't missed any checks. */
        exact = CAP_cc(checked_setbounds)(&result, length);
    } else {
        exact = CAP_cc(setbounds)(&result, length);
        RESULT_VALID = cbp->cr_tag && result.cr_tag;
    }
    /*
     * With compressed capabilities we may need to increase the range of
     * memory addresses to be wider than requested so it is representable.
     */
    if (!exact)
        env->statcounters_imprecise_setbounds++;
    if (must_be_exact && !exact) {
        raise_cheri_exception_or_invalidate(env, CapEx_InexactBounds, cb);
    }

    if (RESULT_VALID) {
        assert(cap_is_representable(&result) &&
               "CSetBounds must create a representable capability");
        assert(result.cr_base >= cbp->cr_base &&
               "CSetBounds broke monotonicity (base)");
        assert(cap_get_length_full(&result) <= cap_get_length_full(cbp) &&
               "CSetBounds broke monotonicity (length)");
        assert(cap_get_top_full(&result) <= cap_get_top_full(cbp) &&
               "CSetBounds broke monotonicity (top)");
    } else {
        result.cr_tag = 0;
    }

    update_capreg(env, cd, &result);
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

#ifndef TARGET_AARCH64
/* Morello does not have flags in the capability metadata */
target_ulong CHERI_HELPER_IMPL(cgetflags(CPUArchState *env, uint32_t cb))
{
    /*
     * CGetFlags: Move Flags to a General-Purpose Register.
     * Returns 1 for capability mode, 0 for integer mode.
     */
    CheriExecMode mode = cap_get_exec_mode(get_readonly_capreg(env, cb));
    return mode == CHERI_EXEC_CAPMODE ? 1 : 0;
}

void CHERI_HELPER_IMPL(csetflags(CPUArchState *env, uint32_t cd, uint32_t cb,
                                 target_ulong flags))
{
    GET_HOST_RETPC_IF_TRAPPING_CHERI_ARCH();
    DEFINE_RESULT_VALID;
    const cap_register_t *cbp = get_readonly_capreg(env, cb);
    /*
     * CSetFlags: Set Flags
     */
    if (cbp->cr_tag && !cap_is_unsealed(cbp)) {
        raise_cheri_exception_or_invalidate(env, CapEx_SealViolation, cb);
    }
    cap_register_t result = *cbp;
    if (!RESULT_VALID) {
        result.cr_tag = 0;
    }
    flags &= CAP_FLAGS_ALL_BITS;
    _Static_assert(CAP_FLAGS_ALL_BITS == 1, "Only one flag should exist");
    cap_set_exec_mode(&result, flags ? CHERI_EXEC_CAPMODE : CHERI_EXEC_INTMODE);
    update_capreg(env, cd, &result);
}
#endif

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
        if (!is_null_capability(env, ctp)) {
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
        cap_get_top_full(ctp) <= cap_get_top_full(cbp) &&
        (cap_get_all_perms(cbp) & cap_get_all_perms(ctp)) ==
            cap_get_all_perms(ctp)) {
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
    GET_HOST_RETPC_IF_TRAPPING_CHERI_ARCH();
    // CToPtr traps on ctp == NULL so we use reg0 as $ddc there. This means we
    // can have a CToPtr relative to $ddc as one instruction instead of two and
    // is required since clang still assumes it can use zero as $ddc in
    // cfromptr/ctoptr
    const cap_register_t *cbp = get_readonly_capreg(env, cb);
    const cap_register_t *ctp = get_capreg_0_is_ddc(env, ct);
    target_ulong cb_cursor = cap_get_cursor(cbp);
    /*
     * CToPtr: Capability to Pointer
     */
    if (!CHERI_TAG_CLEAR_ON_INVALID(env) && !ctp->cr_tag) {
        raise_cheri_exception(env, CapEx_TagViolation, ct);
    }
    if (!cbp->cr_tag) {
        return (target_ulong)0;
    } else {
        return (target_ulong)(cb_cursor - ctp->cr_base);
    }
}

/// Loads and stores

static inline QEMU_ALWAYS_INLINE target_ulong cap_check_common(
    uint32_t required_perms, CPUArchState *env, uint32_t cb,
    target_ulong offset, uint32_t size, uintptr_t _host_return_address)
{
#ifdef TARGET_AARCH64
    /* These helpers cannot be used on Morello since TCG passes addresses. */
    tcg_abort();
#endif
    const cap_register_t *cbp = get_load_store_base_cap(env, cb);
    return cap_check_common_reg(required_perms, env, cb,
                                cap_get_cursor(cbp) + offset, size,
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

target_ulong CHERI_HELPER_IMPL(cap_check_addr(CPUArchState *env,
                                              uint32_t authreg,
                                              target_ulong addr, uint32_t size,
                                              uint32_t required_perms))
{
    const cap_register_t *auth = get_load_store_base_cap(env, authreg);
    return cap_check_common_reg(required_perms, env, authreg, addr, size,
                                GETPC(), auth, size,
                                /*unaligned_handler=*/NULL);
}

/// Capability loads and stores
void CHERI_HELPER_IMPL(load_cap_via_cap(CPUArchState *env, uint32_t dstreg,
                                        target_ulong addr, uint32_t authreg))
{
    GET_HOST_RETPC();
    const cap_register_t *cbp = get_capreg_or_special(env, authreg);

    const target_ulong checked_addr =
        cap_check_common_reg(min_perms_for_load(), env, authreg, addr,
                             CHERI_CAP_SIZE, _host_return_address, cbp,
                             CHERI_CAP_SIZE, raise_unaligned_load_exception);

    load_cap_from_memory(env, dstreg, authreg, cbp, checked_addr,
                         _host_return_address, /*physaddr_out=*/NULL);
}

void CHERI_HELPER_IMPL(load_cap_via_ddc(CPUArchState *env, uint32_t dstreg,
                                        target_ulong intaddr))
{
    GET_HOST_RETPC();
    const cap_register_t *ddc = cheri_get_ddc(env);
    const target_ulong checked_addr =
        cap_check_common_reg(min_perms_for_load(), env, CHERI_EXC_REGNUM_DDC,
                             cheri_ddc_relative_addr(env, intaddr),
                             CHERI_CAP_SIZE, _host_return_address, ddc,
                             CHERI_CAP_SIZE, raise_unaligned_load_exception);
    load_cap_from_memory(env, dstreg, CHERI_EXC_REGNUM_DDC, ddc, checked_addr,
                         _host_return_address, /*physaddr_out=*/NULL);
}

void CHERI_HELPER_IMPL(store_cap_via_cap(CPUArchState *env, uint32_t valreg,
                                         target_ulong addr, uint32_t authreg))
{
    GET_HOST_RETPC();
    const cap_register_t *cbp = get_capreg_or_special(env, authreg);

    const target_ulong checked_addr =
        cap_check_common_reg(min_perms_for_store(env, valreg), env, authreg,
                             addr, CHERI_CAP_SIZE, _host_return_address, cbp,
                             CHERI_CAP_SIZE, raise_unaligned_store_exception);

    store_cap_to_memory(env, valreg, authreg, checked_addr,
                        _host_return_address);
}

void CHERI_HELPER_IMPL(store_cap_via_ddc(CPUArchState *env, uint32_t valreg,
                                         target_ulong intaddr))
{
    GET_HOST_RETPC();
    const target_ulong checked_addr = cap_check_common_reg(
        min_perms_for_store(env, valreg), env, CHERI_EXC_REGNUM_DDC,
        cheri_ddc_relative_addr(env, intaddr), CHERI_CAP_SIZE,
        _host_return_address, cheri_get_ddc(env), CHERI_CAP_SIZE,
        raise_unaligned_store_exception);
    store_cap_to_memory(env, valreg, CHERI_EXC_REGNUM_DDC, checked_addr,
                        _host_return_address);
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
    if (tag && !cap_has_perms(cbp, CAP_PERM_LOAD_CAP)) {
        qemu_maybe_log_instr_extra(env, "Clearing tag loaded from " TARGET_FMT_lx
            " due to missing CAP_PERM_LOAD_CAP\n", va);
        return 0;
    }
    if ((tag && (prot & PAGE_LC_TRAP)) || (prot & PAGE_LC_TRAP_ANY))
        raise_load_tag_exception(env, va, cb, retpc);
    return tag;
}

/*
 * Update the permissions of a capability that has just been loaded from
 * memory. pesbt points to the metadata of the loaded capability, source is
 * the capability that authorized the load.
 *
 * Permission updates may be enforced by load mutable or capability levels.
 *
 * source must be fully decompressed. This is a reasonable assumption since
 * it's always pointing to a register.
 */
static void update_loaded_cap_perms(CPUArchState *env, target_ulong *pesbt,
                                    const cap_register_t *source)
{
#if defined(TARGET_AARCH64) || defined(TARGET_CHERI_RISCV_STD)
    /*
     * Create a temporary capability for checking and updating
     * permissions, its address is not used. All capabilities in the
     * system use the same number of lvbits, we can copy the value from
     * any other capability.
     */
    cap_register_t tmp = *source;
    tmp.cr_pesbt = *pesbt;
    /* Note: original_perms also contains the level */
    const target_ulong original_perms = cap_get_all_perms(&tmp);
    target_ulong perms = original_perms;
    if (!cap_has_perms(source, CAP_PERM_MUTABLE_LOAD)) {
        /*
         * The spec says "Capabilities that are sealed or untagged do not have
         * their permissions changed."
         * The tag has already been checked by the caller.
         */
        if (cap_is_unsealed(&tmp)) {
            qemu_maybe_log_instr_extra(env, "Squashing mutable load perms\n");
#if defined(TARGET_AARCH64)
            perms &= ~(CAP_PERM_MUTABLE_LOAD | CAP_PERM_STORE_LOCAL |
                       CAP_PERM_STORE_CAP | CAP_PERM_STORE);
#elif defined(TARGET_CHERI_RISCV_STD)
            perms &= ~(CAP_PERM_MUTABLE_LOAD | CAP_PERM_STORE);
#endif
        }
    }

#if defined(TARGET_CHERI_RISCV_STD_093)
    /*
     * Any unsealed capability with its tag set to 1 that is loaded from memory
     * has its EL-permission cleared and its Capability Level (CL) restricted to
     * the authorizing capability’s Capability Level (CL) if the authorizing
     * capability does not grant EL-permission. If sealed, then only CL is
     * modified, EL-permission is unchanged. This permission is similar to the
     * existing LM-permission, but instead of applying to the W-permission on
     * the loaded capability it restricts the CL field.
     */
    if (source->cr_lvbits > 0 &&
        !cap_has_perms(source, CAP_PERM_ELEVATE_LEVEL)) {
        cheri_debug_assert(source->cr_lvbits == 1 && "Only 0 and 1 supported");
        qemu_maybe_log_instr_extra(env, "Squashing elevate level perms\n");
        if (!cap_has_perms(source, CAP_PERM_GLOBAL)) {
            /* Note: CL is always modified, even for sealed capabilities. */
            perms &= ~CAP_PERM_GLOBAL;
        }
        if (cap_is_unsealed(&tmp)) {
            perms &= ~CAP_PERM_ELEVATE_LEVEL;
        }
    }
#endif
    if (perms != original_perms) {
        /* Strip any other permissions that can no longer be encoded. */
        cap_legalize_perms(env, &tmp, &perms);
        cap_set_perms(env, &tmp, perms);
        *pesbt = tmp.cr_pesbt;
    }
#endif
}

bool load_cap_from_memory_raw_tag_mmu_idx(
    CPUArchState *env, target_ulong *pesbt, target_ulong *cursor, uint32_t cb,
    const cap_register_t *source, target_ulong vaddr, uintptr_t retpc,
    hwaddr *physaddr, bool *raw_tag, int mmu_idx, bool all_raw)
{
    /*
     * If all_raw is set, we return tag, pesbt and cursor exactly as they are
     * stored in memory.
     *
     * source/cb point to the authorizing capability. Generally, the caller
     * must have checked permissions for the memory read. We use the
     * authorizing capability's permission to fix up the mem capability (e.g.
     * clear the tag, strip W for missing LM, ...). In the all_raw case, there
     * is no such fixup, cb/source are not needed. raw_tag isn't needed either,
     * the function returns the raw tag.
     */
    if (all_raw) {
        cheri_debug_assert(cb == 0);
        cheri_debug_assert(source == NULL);
        cheri_debug_assert(raw_tag == NULL);
    }

    cheri_debug_assert(QEMU_IS_ALIGNED(vaddr, CHERI_CAP_SIZE));
    /*
     * Load otype and perms from memory (might trap on load)
     *
     * Note: In-memory capabilities pesbt is xored with a mask to ensure that
     * NULL capabilities have an all zeroes representation.
     */
    /* No TLB fault possible, should be safe to get a host pointer now */
    void *host = probe_read(env, vaddr, CHERI_CAP_SIZE, mmu_idx, retpc);
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
                 CAP_MEM_XOR_MASK;
        *cursor = ld_cap_word_p((char *)host + CHERI_MEM_OFFSET_CURSOR);
#undef ld_cap_word_p
    } else {
        // Slow path for e.g. IO regions.
        qemu_maybe_log_instr_extra(env, "Using slow path for load from guest "
            "address " TARGET_FMT_lx "\n", vaddr);
        *pesbt =
            cpu_ld_cap_word_ra(env, vaddr + CHERI_MEM_OFFSET_METADATA, retpc) ^
            CAP_MEM_XOR_MASK;
        *cursor = cpu_ld_cap_word_ra(env, vaddr + CHERI_MEM_OFFSET_CURSOR, retpc);
    }
    int prot;
    bool tag =
        cheri_tag_get(env, vaddr, cb, physaddr, &prot, retpc, mmu_idx, host);

#if defined(CONFIG_TCG_LOG_INSTR)
    /* Log capability memory access as a single access */
    if (qemu_log_instr_enabled(env)) {
        /*
         * Decompress to log all fields
         * TODO(am2419): why do we decompress? we and up having to compress
         * again in logging implementation. Passing pesbt + cursor would
         * assume a 128-bit format and be less generic?
         */
        uint8_t lvbits = 0;
#ifdef TARGET_CHERI_RISCV_STD
        lvbits = env_archcpu(env)->cfg.lvbits;
#endif
        cap_register_t ncd;
        CAP_cc(decompress_raw_ext)(*pesbt, *cursor, tag, lvbits, &ncd);
        qemu_log_instr_ld_cap(env, vaddr, &ncd);
    }
#endif

    /* Skip fixups if the caller wants the exact raw capability, see above. */
    if (!all_raw) {
        if (raw_tag) {
            *raw_tag = tag;
        }
        tag = cheri_tag_prot_clear_or_trap(env, vaddr, cb, source, prot, retpc,
                                           tag);
        if (tag) {
            update_loaded_cap_perms(env, pesbt, source);
        }
    }

    env->statcounters_cap_read++;
    if (tag)
        env->statcounters_cap_read_tagged++;

#if defined(TARGET_RISCV) && defined(CONFIG_RVFI_DII)
    env->rvfi_dii_trace.MEM.rvfi_mem_addr = vaddr;
    env->rvfi_dii_trace.MEM.rvfi_mem_rdata[0] = *cursor;
    env->rvfi_dii_trace.MEM.rvfi_mem_rdata[1] = *pesbt ^ CAP_MEM_XOR_MASK;
    env->rvfi_dii_trace.MEM.rvfi_mem_rdata[2] = tag;
    env->rvfi_dii_trace.MEM.rvfi_mem_rmask = (1 << CHERI_CAP_SIZE) - 1;
    // TODO: Add one extra bit to include the tag?
    env->rvfi_dii_trace.available_fields |= RVFI_MEM_DATA;
#endif
    return tag;
}

bool load_raw_cap_from_memory(CPUArchState *env, target_ulong *pesbt,
                              target_ulong *cursor, target_ulong vaddr,
                              uintptr_t retpc)
{
    return load_cap_from_memory_raw_tag_mmu_idx(env, pesbt, cursor, 0, NULL,
                                                vaddr, retpc, NULL, NULL,
                                                cpu_mmu_index(env, false),
                                                /* all_raw */ true);
}

bool load_cap_from_memory_raw_tag(CPUArchState *env, target_ulong *pesbt,
                                  target_ulong *cursor, uint32_t cb,
                                  const cap_register_t *source,
                                  target_ulong vaddr, uintptr_t retpc,
                                  hwaddr *physaddr, bool *raw_tag)
{
    return load_cap_from_memory_raw_tag_mmu_idx(env, pesbt, cursor, cb, source,
                                                vaddr, retpc, physaddr, raw_tag,
                                                cpu_mmu_index(env, false),
                                                /* all_raw */ false);
}

bool load_cap_from_memory_raw(CPUArchState *env, target_ulong *pesbt,
                              target_ulong *cursor, uint32_t cb,
                              const cap_register_t *source, target_ulong vaddr,
                              uintptr_t retpc, hwaddr *physaddr)
{
    return load_cap_from_memory_raw_tag(env, pesbt, cursor, cb, source, vaddr,
                                        retpc, physaddr, NULL);
}

cap_register_t load_and_decompress_cap_from_memory_raw(
    CPUArchState *env, uint32_t cb, const cap_register_t *source,
    target_ulong vaddr, uintptr_t retpc, hwaddr *physaddr)
{
    target_ulong pesbt, cursor;
    bool tag = load_cap_from_memory_raw(env, &pesbt, &cursor, cb, source, vaddr,
                                        retpc, physaddr);
    cap_register_t result;
    uint8_t lvbits = 0;
#ifdef TARGET_CHERI_RISCV_STD
    lvbits = env_archcpu(env)->cfg.lvbits;
#endif
    CAP_cc(decompress_raw_ext)(pesbt, cursor, tag, lvbits, &result);
    result.cr_extra = CREG_FULLY_DECOMPRESSED;
    return result;
}

void load_cap_from_memory(CPUArchState *env, uint32_t cd, uint32_t cb,
                          const cap_register_t *source, target_ulong vaddr,
                          uintptr_t retpc, hwaddr *physaddr)
{
    target_ulong pesbt;
    target_ulong cursor;
    bool tag = load_cap_from_memory_raw(env, &pesbt, &cursor, cb, source, vaddr,
                                        retpc, physaddr);
    update_compressed_capreg(env, cd, pesbt, tag, cursor);
}

/*
 * cs is the register of the capability that will be stored
 * cb is the register of the authorizing capability
 */
void store_cap_to_memory_mmu_index(CPUArchState *env, uint32_t cs,
                                   uint32_t cb __attribute__((unused)),
                                   target_ulong vaddr, uintptr_t retpc,
                                   int mmu_idx)
{
    target_ulong cursor = get_capreg_cursor(env, cs);
    target_ulong pesbt_for_mem = get_capreg_pesbt(env, cs) ^ CAP_MEM_XOR_MASK;
#ifdef CONFIG_DEBUG_TCG
    if (get_capreg_state(cheri_get_gpcrs(env), cs) == CREG_INTEGER) {
        tcg_debug_assert(pesbt_for_mem == 0 && "Integer values should have NULL PESBT");
    }
#endif
    bool tag = get_capreg_tag_filtered(env, cs);
#if defined(TARGET_CHERI_RISCV_STD)
    RISCVCPU *cpu = env_archcpu(env);
    const cap_register_t *cbp = get_capreg_or_special(env, cb);
    /*
     * For risc-v cheri, we have to store the capability with its tag cleared
     * if the authorizing capability grants R (this has already been checked),
     * but not C.
     */
    if (!cap_has_perms(cbp, CAP_PERM_STORE_CAP)) {
        tag = false;
    }

    /* If lvbits == 0, the format still has SL, but it's always 0 (reserved). */
    if ((cpu->cfg.lvbits > 0) && !cap_has_perms(cbp, CAP_PERM_STORE_LOCAL)) {
        const cap_register_t *csp = get_capreg_or_special(env, cs);
        if (!cap_has_perms(csp, CAP_PERM_GLOBAL)) {
            tag = false;
        }
    }

#endif
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
    void *host = NULL;
    if (tag) {
        env->statcounters_cap_write_tagged++;
        host = cheri_tag_set(env, vaddr, cs, NULL, retpc, mmu_idx);
    } else {
        host = cheri_tag_invalidate_aligned(env, vaddr, retpc, mmu_idx);
    }
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
        const target_ulong pesbt = pesbt_for_mem ^ CAP_MEM_XOR_MASK;
        CAP_cc(decompress_raw)(pesbt, cursor, tag, &stored_cap);
        cheri_debug_assert(cursor == cap_get_cursor(&stored_cap));
        qemu_log_instr_st_cap(env, vaddr, &stored_cap);
    }
#endif
}

void store_cap_to_memory(CPUArchState *env, uint32_t cs, uint32_t cb,
                         target_ulong vaddr, uintptr_t retpc)
{
    return store_cap_to_memory_mmu_index(env, cs, cb, vaddr, retpc,
                                         cpu_mmu_index(env, false));
}

target_ulong CHERI_HELPER_IMPL(cloadtags(CPUArchState *env, uint32_t cb))
{
    static const uint32_t perms = CAP_PERM_LOAD | CAP_PERM_LOAD_CAP;
    static const size_t ncaps = 1 << CAP_TAG_GET_MANY_SHFT;
    static const uint32_t sizealign = ncaps * CHERI_CAP_SIZE;

    GET_HOST_RETPC();
    const cap_register_t *cbp = get_load_store_base_cap(env, cb);

    const target_ulong addr = cap_check_common_reg(
        perms, env, cb, cap_get_cursor(cbp), sizealign, _host_return_address,
        cbp, sizealign, raise_unaligned_load_exception);

    target_ulong result = cheri_tag_get_many(env, addr, cb, NULL, GETPC());
#if defined(TARGET_RISCV) && defined(CONFIG_RVFI_DII)
    /* For RVFI tracing, sail reports the valu of th last capability read. */
    target_ulong unused1, unused2;
    (void)load_cap_from_memory_raw(env, &unused1, &unused2, cb, cbp,
                                   addr + sizealign - CHERI_CAP_SIZE,
                                   _host_return_address, NULL);
#endif
    return result;
}

QEMU_NORETURN static inline void
raise_pcc_fault(CPUArchState *env, CheriCapExcCause cause, target_ulong addr)
{
    cheri_debug_assert(pc_is_current(env));
    /*
     * Note: we set pc=0 since PC will have been saved prior to calling the
     * helper. Therefore, we don't need to recompute it from the generated code.
     * The PC fetched from the generated code will often be out-of-bounds, so
     * fetching it will trigger an assertion.
     */
    raise_cheri_exception_if(env, cause, addr, CHERI_EXC_REGNUM_PCC);
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
    raise_pcc_fault(env, cause, PC_ADDR(env));
}

void CHERI_HELPER_IMPL(raise_exception_pcc_perms_not_if(
    CPUArchState *env, target_ulong addr, uint32_t required_perms))
{
    const cap_register_t *pcc = cheri_get_recent_pcc(env);
    check_cap(env, pcc, required_perms, addr, CHERI_EXC_REGNUM_PCC, 1,
              /*instavail=*/true, GETPC());
    __builtin_unreachable();
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
    raise_pcc_fault(env, CapEx_LengthViolation, addr);
}

void CHERI_HELPER_IMPL(raise_exception_ddc_perms(CPUArchState *env,
                                                 target_ulong addr,
                                                 uint32_t required_perms))
{
    const cap_register_t *ddc = cheri_get_ddc(env);

    cap_check_common_reg(required_perms, env, CHERI_EXC_REGNUM_DDC, addr, 1,
                         GETPC(), ddc, 1, NULL);
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

void CHERI_HELPER_IMPL(debug_cap(CPUArchState *env, uint32_t regndx))
{
    GPCapRegs *gpcrs = cheri_get_gpcrs(env);
    // Index manually in order not to decompress
    const cap_register_t *cap = (regndx < 32)
                                    ? get_cap_in_gpregs(gpcrs, regndx)
                                    : get_capreg_or_special(env, regndx);
    CapRegState state =
        regndx < 32 ? get_capreg_state(gpcrs, regndx) : CREG_FULLY_DECOMPRESSED;
    bool stateMeansTagged = state == CREG_TAGGED_CAP;
    bool decompressedMeansTagged =
        (state == CREG_FULLY_DECOMPRESSED) && cap->cr_tag;
    target_ulong pesbt = cap->cr_pesbt;
    printf("Debug Cap %2d: Cursor " TARGET_FMT_lx ". Pesbt " TARGET_FMT_lx
           ". Tagged %d (%d,%d). Type " TARGET_FMT_lx ". "
           "Perms " TARGET_FMT_lx "\n",
           regndx, cap->_cr_cursor, pesbt ^ CAP_MEM_XOR_MASK,
           stateMeansTagged || decompressedMeansTagged, state, cap->cr_tag,
           cap_get_otype_unsigned(cap), cap_get_all_perms(cap));
    if (state == CREG_FULLY_DECOMPRESSED) {
        printf("Base: " TARGET_FMT_lx ". Top " TARGET_FMT_lu TARGET_FMT_lx
               ".\n",
               cap->cr_base, (target_ulong)(cap->_cr_top >> CAP_CC(ADDR_WIDTH)),
               (target_ulong)cap->_cr_top);
    }
}

void helper_capreg_state_debug(CPUArchState *env, uint32_t regnum,
                               uint64_t flags, uint64_t pc)
{
    GPCapRegs *gpcrs = cheri_get_gpcrs(env);
    CapRegState regstate = get_capreg_state(gpcrs, regnum);

    // Should include the actual state
    assert((flags & (1 << (uint64_t)regstate)) && pc);
}

cap_register_t cap_scaddr(target_ulong addr, cap_register_t dest)
{
    if (is_cap_sealed(&dest)) {
        dest.cr_tag = false;
    }
    // cap_set_cursor checks the representable range
    cap_set_cursor(&dest, addr);
    return dest;
}
