/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2020 Alex Richardson
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
#include "tcg/tcg.h"
#include "tcg/tcg-op.h"
#include "cheri-translate-utils-base.h"
#include "cheri-lazy-capregs-types.h"
#include "tcg-target.h"
#include "exec/log_instr.h"

#ifdef TARGET_CHERI

#define CHERI_TCG_LOGGING 0

#define cheri_tcg_printf_standard(...)                                         \
    if (CHERI_TCG_LOGGING >= 1)                                                \
    qemu_log_gen_printf(&ctx->base, __VA_ARGS__)
#define cheri_tcg_printf_verbose(...)                                          \
    if (CHERI_TCG_LOGGING >= 2)                                                \
    qemu_log_gen_printf(&ctx->base, __VA_ARGS__)

// Generate a trace without decompressing. Should be used only to debug TCG.
// Otherwise use the proper logging mechanism via qemu_log_gen_printf.
static inline void gen_cap_debug(DisasContext *ctx, int rx)
{
    TCGv_i32 tmp = tcg_const_i32(rx);
    gen_helper_debug_cap(cpu_env, tmp);
    tcg_temp_free_i32(tmp);
}

#define _gen_cap_check(type)                                                   \
    static inline void generate_cap_##type##_check(                            \
        TCGv_cap_checked_ptr resultaddr, uint32_t capreg, TCGv offset,         \
        MemOp op)                                                              \
    {                                                                          \
        TCGv_i32 tcs = tcg_const_i32(capreg);                                  \
        TCGv_i32 tsize = tcg_const_i32(memop_size(op));                        \
        gen_helper_cap_##type##_check(resultaddr, cpu_env, tcs, offset,        \
                                      tsize);                                  \
        tcg_temp_free_i32(tsize);                                              \
        tcg_temp_free_i32(tcs);                                                \
    }                                                                          \
    static inline void generate_cap_##type##_check_imm(                        \
        TCGv_cap_checked_ptr resultaddr, uint32_t capreg, target_long offset,  \
        MemOp op)                                                              \
    {                                                                          \
        TCGv toffset = tcg_const_tl(offset);                                   \
        generate_cap_##type##_check(resultaddr, capreg, toffset, op);          \
        tcg_temp_free(toffset);                                                \
    }

_gen_cap_check(load)
_gen_cap_check(store)
_gen_cap_check(rmw)

// FIXME: assumes small endian order between two 64-bit halves in a 128-bit
// integer.
// FIXME: I have no idea GCC actually implements this across platforms. Might be
// MAD/SAD for all I know.
#define CAP_TOP_LOBYTES_OFFSET 0
#define CAP_TOP_HIBYTES_OFFSET 8

// TODO Move to target specific headers
#ifdef TARGET_MIPS

#define DDC_ENV_OFFSET offsetof(CPUArchState, active_tc.CHWR.DDC)
static inline void gen_load_gpr(TCGv t, int reg);
#define target_get_gpr(ctx, t, reg) gen_load_gpr((TCGv)t, reg)
#define MERGED_FILE 0

#elif defined(TARGET_AARCH64)

#define DDC_ENV_OFFSET offsetof(CPUArchState, DDC_current)
// Other targets can only enable these once every call to a helper properly
// updates disas_capreg_state
#define ENABLE_STATIC_CAP_OPTS 1
// Other targets need to check usages of _generate_special_checked_ptr as with
// TCG bounds checks a branch is generated
#define DO_TCG_BOUNDS_CHECKS 1
#define DDC_INTERPOSITION_IS_BASE 1
#define BOUNDS_DO_NOT_WRAP
#define TRACE_MODIFIED_REGISTERS

TCGv_i64 cpu_reg(DisasContext *s, int reg);
#define target_get_gpr_global(ctx, reg) cpu_reg_sp(ctx, reg)
#define target_get_gpr(ctx, t, reg)                                            \
    if (reg == NULL_CAPREG_INDEX)                                              \
        tcg_gen_movi_tl((TCGv)t, 0);                                           \
    else                                                                       \
        tcg_gen_mov_i64((TCGv)t, cpu_reg_sp(ctx, reg))
#define target_set_gpr(ctx, reg, t)                                            \
    if (reg != NULL_CAPREG_INDEX)                                                   \
        tcg_gen_mov_i64(cpu_reg_sp(ctx, reg), (TCGv)t);

#define MERGED_FILE 1

#elif defined(TARGET_RISCV)

#define DDC_ENV_OFFSET offsetof(CPUArchState, DDC)
#define target_get_gpr_global(ctx, reg) (assert(0), (TCGv_i64)NULL)
#define target_get_gpr(ctx, t, reg) gen_get_gpr((TCGv)t, reg)
    static inline void _gen_set_gpr(DisasContext *ctx, int reg_num_dst, TCGv t,
                                    bool clear_pesbt);
#define target_set_gpr(ctx, reg, t) _gen_set_gpr(ctx, reg, (TCGv)t, false)
#define MERGED_FILE 1

#else
#error "Don't know how to fetch a GPR value"
#endif

static inline void generate_get_ddc_checked_gpr_plus_offset(
    TCGv_cap_checked_ptr addr, DisasContext *ctx, int reg_num,
    target_long offset, MemOp mop,
    void (*check_ddc)(TCGv_cap_checked_ptr, DisasContext *, TCGv, target_ulong))
{
    target_get_gpr(ctx, addr, reg_num);

    if (!__builtin_constant_p(offset) || offset != 0) {
        tcg_gen_addi_tl((TCGv)addr, (TCGv)addr, offset);
    }
    check_ddc(addr, ctx, /* overwrite addr */ (TCGv)addr, memop_size(mop));
}

static inline bool have_cheri_tb_flags(DisasContext *ctx, uint32_t flags)
{
    return (ctx->base.cheri_flags & flags) == flags;
}

static inline void generate_get_ddc_base(DisasContext *ctx, TCGv base)
{
#ifdef DDC_INTERPOSITION_IS_BASE
    // Should not use underlying memory if represented by a global
    tcg_gen_mov_tl(base, ddc_interposition);
#else
    tcg_gen_ld_tl(base, cpu_env,
                  DDC_ENV_OFFSET + offsetof(cap_register_t, cr_base));
#endif
}

static inline void generate_get_ddc_top_lo(DisasContext *ctx, TCGv top)
{
    tcg_gen_ld_tl(top, cpu_env,
                  DDC_ENV_OFFSET + offsetof(cap_register_t, _cr_top) +
                      CAP_TOP_LOBYTES_OFFSET);
}

// Checks an address against PCC or DDC.
// Permissions are checked at translate time, bounds checks can be skipped if
// CHERI flags indicate bounds would make checks trivial.
static inline void _generate_special_checked_ptr(
    DisasContext *ctx, CheriTbFlags tb_perm_flags, CheriPermissions req_perms,
    TCGv_cap_checked_ptr checked_addr, TCGv integer_addr,
    target_ulong num_bytes, bool use_ddc)
{
    if (unlikely(!have_cheri_tb_flags(ctx, tb_perm_flags))) {
        // PCC/DDC is untagged, sealed, or missing permissions
        TCGv_i32 tperms = tcg_const_i32(req_perms);
        cheri_tcg_prepare_for_unconditional_exception(&ctx->base);
        if (use_ddc)
            gen_helper_raise_exception_ddc_perms(cpu_env, tperms);
        else
            gen_helper_raise_exception_pcc_perms_not_if(cpu_env, tperms);
        tcg_temp_free_i32(tperms);
        return;
    }
    // PCC interposition currently done mostly by the caller.
    bool need_interposition =
        use_ddc && !have_cheri_tb_flags(ctx, TB_FLAG_CHERI_DDC_NO_INTERPOSE);

    // Would be nice to get rid of the ifdefs in this otherwise (mostly) target
    // indepedent header. Probably a call in to somethging in cheri-archspecific
    // for each use of an ifdef.
#ifdef TARGET_AARCH64
    // Recover cctlr stashed in flags, and check if we need a base offset
    uint32_t cctlr = (ctx->base.cheri_flags >> TB_FLAG_CHERI_SPARE_INDEX_START)
                     << CCTLR_DEFINED_START;
    need_interposition &= (cctlr & CCTLR_DDCBO) != 0;
#endif

    // We need interposition since the base/cursor is not zero.
    if (unlikely(need_interposition)) {
        tcg_gen_add_tl((TCGv)checked_addr, integer_addr, ddc_interposition);
    } else if ((TCGv)checked_addr != integer_addr) {
        tcg_gen_mov_tl((TCGv)checked_addr, integer_addr);
    }
    CheriTbFlags full_as_flags =
        use_ddc ? TB_FLAG_CHERI_DDC_FULL_AS : TB_FLAG_CHERI_PCC_FULL_AS;
    bool do_checks = !have_cheri_tb_flags(ctx, full_as_flags);
#ifdef BOUNDS_DO_NOT_WRAP
    do_checks = true;
#endif
    if (unlikely(do_checks)) {
        // We need a bounds check since PCC/DDC is not full address space.
#ifdef DO_TCG_BOUNDS_CHECKS
        TCGv in_bounds = tcg_const_tl(1);
        TCGv local_addr = tcg_temp_local_new();
        // Save checked_addr to a local so it does not get clobbered.
        tcg_gen_mov_tl(local_addr, ((TCGv)checked_addr));
        // Then use checked_addr as a tmp.
        TCGv tmp = (TCGv)checked_addr;
        TCGv tmp2 = tcg_temp_new();

        // Base
        TCGv base = NULL;

        if (use_ddc && !have_cheri_tb_flags(ctx, TB_FLAG_CHERI_DDC_BASE_ZERO)) {
            generate_get_ddc_base(ctx, tmp2);
            base = tmp2;
        } else if (!use_ddc && !have_cheri_tb_flags(ctx, TB_FLAG_CHERI_PCC_BASE_ZERO)) {
            tcg_gen_movi_tl(tmp2, ctx->base.pcc_base);
            base = tmp2;
        }

        if (base) {
            tcg_gen_setcond_tl(TCG_COND_GEU, tmp, (TCGv)local_addr, base);
            tcg_gen_and_tl(in_bounds, in_bounds, tmp);
        }

        // Because we check tb flags for the trival check, we only need 64 bit
        // comparisons for top
        TCGv top = NULL;

        if (use_ddc && !have_cheri_tb_flags(ctx, TB_FLAG_CHERI_DDC_TOP_MAX)) {
            generate_get_ddc_top_lo(ctx, tmp2);
            top = tmp2;
        } else if (!use_ddc && !have_cheri_tb_flags(ctx, TB_FLAG_CHERI_PCC_TOP_MAX)) {
            tcg_gen_movi_tl(tmp2, (target_ulong)ctx->base.pcc_top);
            top = tmp2;
        }

        if (top) {
            if (num_bytes > 1) {
                tcg_gen_addi_tl(tmp, (TCGv)local_addr, num_bytes);
                tcg_gen_setcond_tl(TCG_COND_LEU, tmp, tmp, top);
            } else {
                tcg_gen_setcond_tl(num_bytes ? TCG_COND_LTU : TCG_COND_LEU, tmp,
                                   (TCGv)local_addr, top);
            }
            tcg_gen_and_tl(in_bounds, in_bounds, tmp);
        } else {
#ifdef BOUNDS_DO_NOT_WRAP
            // If top is 1 << 64 then the only way to out of bounds is if addr +
            // num_bytes wraps This is still technically wrong if top is greater
            // than 1 << 64. But I don't really care.
            tcg_gen_addi_tl(tmp, (TCGv)local_addr, num_bytes);
            tcg_gen_setcond_tl(TCG_COND_LEU, tmp, (TCGv)local_addr, tmp);
            tcg_gen_and_tl(in_bounds, in_bounds, tmp);
#endif
        }

        TCGLabel *skip = gen_new_label();
        tcg_gen_movi_tl(tmp, 0);
        tcg_gen_brcond_tl(TCG_COND_NE, in_bounds, tmp, skip);
        tcg_gen_mov_i64((TCGv)checked_addr, local_addr);
#endif
        TCGv tbytes = tcg_const_tl(num_bytes);

        void (*bounds_check_helper)(TCGv_env, TCGv, TCGv);
        if (use_ddc) {
#ifdef TARGET_AARCH64
            if (!(req_perms & CAP_PERM_LOAD))
                bounds_check_helper = &gen_helper_ddc_check_bounds_store;
            else
#endif
                bounds_check_helper = &gen_helper_ddc_check_bounds;
        } else {
            assert(!(req_perms & CAP_PERM_STORE));
            bounds_check_helper = &gen_helper_pcc_check_bounds;
        }
        bounds_check_helper(cpu_env, (TCGv)checked_addr, tbytes);
        tcg_temp_free(tbytes);
#ifdef DO_TCG_BOUNDS_CHECKS
        gen_set_label(skip);
        tcg_gen_mov_i64((TCGv)checked_addr, local_addr);
        tcg_temp_free(in_bounds);
        tcg_temp_free(local_addr);
        tcg_temp_free(tmp2);
#endif
    }
    // PCC/DDC has been checked now and checked_addr can be used directly.
}
static inline void
generate_ddc_checked_load_ptr(TCGv_cap_checked_ptr checked_addr,
                              DisasContext *ctx, TCGv ddc_offset,
                              target_ulong num_bytes)
{
    _generate_special_checked_ptr(ctx, TB_FLAG_CHERI_DDC_READABLE,
                                  CAP_PERM_LOAD, checked_addr, ddc_offset,
                                  num_bytes, true);
}
static inline void
generate_ddc_checked_store_ptr(TCGv_cap_checked_ptr checked_addr,
                               DisasContext *ctx, TCGv ddc_offset,
                               target_ulong num_bytes)
{
    _generate_special_checked_ptr(ctx, TB_FLAG_CHERI_DDC_WRITABLE,
                                  CAP_PERM_STORE, checked_addr, ddc_offset,
                                  num_bytes, true);
}
static inline void
generate_ddc_checked_rmw_ptr(TCGv_cap_checked_ptr checked_addr,
                             DisasContext *ctx, TCGv ddc_offset,
                             target_ulong num_bytes)
{
    _generate_special_checked_ptr(
        ctx, TB_FLAG_CHERI_DDC_READABLE | TB_FLAG_CHERI_DDC_WRITABLE,
        CAP_PERM_LOAD | CAP_PERM_STORE, checked_addr, ddc_offset, num_bytes,
        true);
}

#define FLAG_READABLE(use_ddc)                                                 \
    (use_ddc ? TB_FLAG_CHERI_DDC_READABLE : TB_FLAG_CHERI_PCC_READABLE)
static inline CheriTbFlags FLAG_WRITABLE(bool use_ddc)
{
    assert(use_ddc && "Writable flag should only be checked for DDC");
    return TB_FLAG_CHERI_DDC_WRITABLE;
}

static inline void
generate_special_checked_load_ptr(TCGv_cap_checked_ptr checked_addr,
                                  DisasContext *ctx, TCGv ddc_offset,
                                  target_ulong num_bytes, bool use_ddc)
{
    _generate_special_checked_ptr(ctx, FLAG_READABLE(use_ddc), CAP_PERM_LOAD,
                                  checked_addr, ddc_offset, num_bytes, use_ddc);
}
static inline void
generate_special_checked_store_ptr(TCGv_cap_checked_ptr checked_addr,
                                   DisasContext *ctx, TCGv ddc_offset,
                                   target_ulong num_bytes, bool use_ddc)
{
    _generate_special_checked_ptr(ctx, FLAG_WRITABLE(use_ddc), CAP_PERM_STORE,
                                  checked_addr, ddc_offset, num_bytes, use_ddc);
}
static inline void
generate_special_checked_rmw_ptr(TCGv_cap_checked_ptr checked_addr,
                                 DisasContext *ctx, TCGv ddc_offset,
                                 target_ulong num_bytes, bool use_ddc)
{
    _generate_special_checked_ptr(
        ctx, FLAG_READABLE(use_ddc) | FLAG_WRITABLE(use_ddc),
        CAP_PERM_LOAD | CAP_PERM_STORE, checked_addr, ddc_offset, num_bytes,
        use_ddc);
}

#else // !TARGET_CHERI
#define generate_ddc_checked_load_ptr(checked_addr, ctx, offset, num_bytes)    \
    tcg_gen_mov_tl(checked_addr, offset)
#define generate_ddc_checked_store_ptr(checked_addr, ctx, offset, num_bytes)   \
    tcg_gen_mov_tl(checked_addr, offset)
#define generate_ddc_checked_rmw_ptr(checked_addr, ctx, offset, num_bytes)     \
    tcg_gen_mov_tl(checked_addr, offset)
#define generate_special_checked_load_ptr(checked_addr, ctx, offset,           \
                                          num_bytes, ddc)                      \
    tcg_gen_mov_tl(checked_addr, offset)
#define generate_special_checked_store_ptr(checked_addr, ctx, offset,          \
                                           num_bytes, ddc)                     \
    tcg_gen_mov_tl(checked_addr, offset)
#define generate_special_checked_rmw_ptr(checked_addr, ctx, offset, num_bytes, \
                                         ddc)                                  \
    tcg_gen_mov_tl(checked_addr, offset)
#endif // TARGET_CHERI

static inline void gen_special_interposed_ld_i64(
    DisasContext *ctx, TCGv_i64 result, TCGv_cap_checked_ptr checked_addr,
    TCGv ddc_offset, TCGArg arg, MemOp op, bool use_ddc)
{
    if (checked_addr == NULL) {
        checked_addr = (TCGv_cap_checked_ptr)ddc_offset;
    }
    generate_special_checked_load_ptr(checked_addr, ctx, ddc_offset,
                                      memop_size(op), use_ddc);
    tcg_gen_qemu_ld_i64_with_checked_addr(result, checked_addr, arg, op);
}

static inline void gen_special_interposed_ld_i32(
    DisasContext *ctx, TCGv_i32 result, TCGv_cap_checked_ptr checked_addr,
    TCGv ddc_offset, TCGArg arg, MemOp op, bool use_ddc)
{
    if (checked_addr == NULL) {
        checked_addr = (TCGv_cap_checked_ptr)ddc_offset;
    }
    generate_special_checked_load_ptr(checked_addr, ctx, ddc_offset,
                                      memop_size(op), use_ddc);
    tcg_gen_qemu_ld_i32_with_checked_addr(result, checked_addr, arg, op);
}

static inline void gen_special_interposed_st_i64(
    DisasContext *ctx, TCGv_i64 value, TCGv_cap_checked_ptr checked_addr,
    TCGv ddc_offset, TCGArg arg, MemOp op, bool use_ddc)
{
    if (checked_addr == NULL) {
        checked_addr = (TCGv_cap_checked_ptr)ddc_offset;
    }
    generate_special_checked_store_ptr(checked_addr, ctx, ddc_offset,
                                       memop_size(op), use_ddc);
    tcg_gen_qemu_st_i64_with_checked_addr(value, checked_addr, arg, op);
}
static inline void gen_special_interposed_st_i32(
    DisasContext *ctx, TCGv_i32 value, TCGv_cap_checked_ptr checked_addr,
    TCGv ddc_offset, TCGArg arg, MemOp op, bool use_ddc)
{
    if (checked_addr == NULL) {
        checked_addr = (TCGv_cap_checked_ptr)ddc_offset;
    }
    generate_special_checked_store_ptr(checked_addr, ctx, ddc_offset,
                                       memop_size(op), use_ddc);
    tcg_gen_qemu_st_i32_with_checked_addr(value, checked_addr, arg, op);
}

static inline void gen_ddc_interposed_ld_i64(DisasContext *ctx, TCGv_i64 result,
                                             TCGv_cap_checked_ptr checked_addr,
                                             TCGv ddc_offset, TCGArg arg,
                                             MemOp op)
{
    gen_special_interposed_ld_i64(ctx, result, checked_addr, ddc_offset, arg,
                                  op, /*use_ddc=*/true);
}
static inline void gen_ddc_interposed_ld_i32(DisasContext *ctx, TCGv_i32 result,
                                             TCGv_cap_checked_ptr checked_addr,
                                             TCGv ddc_offset, TCGArg arg,
                                             MemOp op)
{
    gen_special_interposed_ld_i32(ctx, result, checked_addr, ddc_offset, arg,
                                  op, /*use_ddc=*/true);
}
static inline void gen_ddc_interposed_st_i64(DisasContext *ctx, TCGv_i64 value,
                                             TCGv_cap_checked_ptr checked_addr,
                                             TCGv ddc_offset, TCGArg arg,
                                             MemOp op)
{
    gen_special_interposed_st_i64(ctx, value, checked_addr, ddc_offset, arg, op,
                                  /*use_ddc=*/true);
}
static inline void gen_ddc_interposed_st_i32(DisasContext *ctx, TCGv_i32 value,
                                             TCGv_cap_checked_ptr checked_addr,
                                             TCGv ddc_offset, TCGArg arg,
                                             MemOp op)
{
    gen_special_interposed_st_i32(ctx, value, checked_addr, ddc_offset, arg, op,
                                  /*use_ddc=*/true);
}

#if TARGET_LONG_BITS == 32
#define gen_ddc_interposed_ld_tl gen_ddc_interposed_ld_i32
#define gen_ddc_interposed_st_tl gen_ddc_interposed_st_i32
#elif TARGET_LONG_BITS == 64
#define gen_ddc_interposed_ld_tl gen_ddc_interposed_ld_i64
#define gen_ddc_interposed_st_tl gen_ddc_interposed_st_i64
#endif

static inline void gen_check_pcc_bounds_next_inst(DisasContext *ctx,
                                                  uint32_t num_bytes)
{
#ifdef TARGET_CHERI
    if (have_cheri_tb_flags(ctx, TB_FLAG_CHERI_PCC_FULL_AS)) {
        return; // PCC spans the full address space, no need to check
    }

    // Note: PC can only be incremented since a branch exits the TB, so checking
    // for pc_next < pcc.base should not be needed. Add a debug assertion in
    // case this assumption no longer holds in the future.
    // Note: we don't have to check for wraparound here since this case is
    // already handled by the TB_FLAG_CHERI_PCC_FULL_AS check above. Wraparound is
    // permitted to avoid any differences with non-CHERI enabled CPUs.
    tcg_debug_assert(ctx->base.pc_next >= ctx->base.pc_first);
    if (unlikely(ctx->base.pc_next + num_bytes > ctx->base.pcc_top)) {
        cheri_tcg_prepare_for_unconditional_exception(&ctx->base);
        gen_raise_pcc_violation(&ctx->base, ctx->base.pc_next, num_bytes);
    }
#endif
}

static inline void gen_check_branch_target(DisasContext *ctx, target_ulong addr)
{
#ifdef TARGET_CHERI
    if (unlikely(!in_pcc_bounds(&ctx->base, addr))) {
        cheri_tcg_prepare_for_unconditional_exception(&ctx->base);
        gen_raise_pcc_violation(&ctx->base, addr, 1);
    }
#endif
}

// Same as above, but in this case the actual address isn't known at translate
// time so we have to emit the branches in the generated code
static inline void gen_check_branch_target_dynamic(DisasContext *ctx, TCGv addr)
{
#ifdef TARGET_CHERI
    // XXXAR: would it be faster to do two setconds and a single branch?
    // GCC and clang output the following assembly for the following code:
    // See https://godbolt.org/z/bdwJ7P
    // if (unlikely(addr < base || addr > top)) {
    //   raise_bounds_violation(1);
    //  }
    // bounds_violation_unlikely:
    //        cmp     rdi, rsi
    //        jl      .L4
    //        cmp     rdi, rdx
    //        jg      .L4
    //        ret
    //.L4:
    //        sub     rsp, 8
    //        mov     edi, 1
    //        call    raise_bounds_violation
    // Note: JR/JALR will often be used in hybrid/non-CHERI cases, so we can
    // skip the less than check if pcc.base is zero and top is MAX:
    if (have_cheri_tb_flags(ctx, TB_FLAG_CHERI_PCC_FULL_AS)) {
        return; // PCC spans the full address space, no need to check
    }

    TCGLabel *skip_btarget_check = gen_new_label();
    TCGLabel *bounds_violation = gen_new_label();
    // We can skip the check of pcc.base if it is zero (common case in
    // hybrid/non-CHERI  mode).
    if (!have_cheri_tb_flags(ctx, TB_FLAG_CHERI_PCC_BASE_ZERO)) {
        tcg_gen_brcondi_tl(TCG_COND_LTU, addr, ctx->base.pcc_base,
                           bounds_violation);
    }
    if (!have_cheri_tb_flags(ctx, TB_FLAG_CHERI_PCC_TOP_MAX)) {
        tcg_gen_brcondi_tl(TCG_COND_GEU, addr, ctx->base.pcc_top,
                           bounds_violation);
    }
    tcg_gen_br(skip_btarget_check); // No violation -> return
    // One of the branches taken -> raise a bounds violation exception
    gen_set_label(bounds_violation); // skip helper call
    gen_raise_pcc_violation_tcgv(&ctx->base, addr, 0);
    gen_set_label(skip_btarget_check); // skip helper call
#endif
}

static inline void gen_check_cond_branch_target(DisasContext *ctx,
                                                TCGv branchcond,
                                                target_ulong addr)
{
#ifdef TARGET_CHERI
    // In the common case the target will be within the bounds of PCC so we
    // don't need a check no matter whether the branch is taken or not.
    if (likely(in_pcc_bounds(&ctx->base, addr)))
        return;

    TCGLabel *skip_btarget_check = gen_new_label();
    // skip the bounds violation if bcond == 0 (i.e. branch not taken)
    tcg_gen_brcondi_tl(TCG_COND_EQ, branchcond, 0, skip_btarget_check);
    // Brach taken -> raise a bounds violation exception
    gen_raise_pcc_violation(&ctx->base, addr, 0);
    gen_set_label(skip_btarget_check); // skip helper call
#endif
}

#if defined(TARGET_CHERI)

// This function belongs in a target specific header
static inline uint32_t gp_register_offset(int reg_num)
{
#ifdef TARGET_MIPS
    return offsetof(CPUArchState, active_tc.gpcapregs.decompressed[reg_num]);
#else
    return offsetof(CPUArchState, gpcapregs.decompressed[reg_num]);
#endif
}

// Copies some number of bytes between min_size and max_size.
// max_size must be padded to make use of the most optimal vector op.
static inline void gen_vector_copy(TCGv_ptr dest_ptr, TCGv_ptr source_ptr,
                                   uint32_t dest_off, uint32_t source_off,
                                   size_t min_size, size_t max_size)
{
    // Use largest chunk as possible
    size_t copy_chunk =
        (TCG_TARGET_HAS_v256 && (min_size >= 32))
            ? 32
            : ((TCG_TARGET_HAS_v128 && (min_size >= 16)) ? 16 : 8);
    size_t mask = copy_chunk - 1;
    size_t copy_size = (min_size + mask) & ~mask;
    assert(copy_size <= max_size);

    if (copy_chunk > 8) {

        TCGv_vec tmp = tcg_temp_new_vec((copy_chunk == 32) ? TCG_TYPE_V256
                                                           : TCG_TYPE_V128);
        for (size_t i = 0; i != copy_size; i += copy_chunk) {
            tcg_gen_ld_vec(tmp, source_ptr, source_off + i);
            tcg_gen_st_vec(tmp, dest_ptr, dest_off + i);
        }
        tcg_temp_free_vec(tmp);

    } else {

        TCGv_i64 tmp = tcg_temp_new_i64();
        for (size_t i = 0; i != copy_size; i += copy_chunk) {
            tcg_gen_ld_i64(tmp, source_ptr, source_off + i);
            tcg_gen_st_i64(tmp, dest_ptr, dest_off + i);
        }
        tcg_temp_free_i64(tmp);
    }
}

// Generates a capability move of an entire padded cap.
// WARNING: If using lazy cap regs you will need to modify the state of the
// destination to match the source.
static inline void gen_move_cap(uint32_t dest_off, uint32_t source_off)
{
    gen_vector_copy(cpu_env, cpu_env, dest_off, source_off,
                    sizeof(cap_register_t), sizeof(aligned_cap_register_t));
}

// Generate a capability move of only the pesbt + cursor of a cap.
// Again, you must also modify lazy state.
static inline void gen_move_compressed_cap(uint32_t dest_off,
                                           uint32_t source_off)
{
    gen_vector_copy(cpu_env, cpu_env, dest_off, source_off, 16, 16);
}

// Does cpu_env[dest_off] = cpu_env[(v cond 0) ? true_off : false_off]
static inline void gen_mov_cap_select(uint32_t dest_off, uint32_t true_off,
                                      uint32_t false_off, TCGCond cond,
                                      TCGv_i32 v)
{

    // Calculate either constant 0 if true, or false_off - true_off otherwise
    TCGv_i32 offset_i32 = tcg_const_i32(0);
    TCGv_i32 diff = tcg_const_i32((int32_t)false_off - (int32_t)true_off);
    tcg_gen_movcond_i32(cond, offset_i32, v, offset_i32, offset_i32, diff);

    // Add to cpu_env
    TCGv_ptr source_ptr = tcg_temp_new_ptr();
    tcg_gen_ext_i32_ptr(source_ptr, offset_i32);
    tcg_gen_add_ptr(source_ptr, source_ptr, cpu_env);

    // Using an offset of true_off + (0, false_off - true_off) = (true_off,
    // false_off)
    gen_vector_copy(cpu_env, source_ptr, dest_off, true_off,
                    sizeof(cap_register_t), sizeof(cap_register_t));
    tcg_temp_free_ptr(source_ptr);
    tcg_temp_free_i32(offset_i32);
    tcg_temp_free_i32(diff);
}

static inline bool disas_capreg_state_could_be(DisasContext *ctx, int reg,
                                               CapRegState state)
{
#ifdef ENABLE_STATIC_CAP_OPTS
    return !!(ctx->base.cap_compression_states[reg] & (1 << state));
#else
    return true;
#endif
}

static inline bool disas_capreg_state_must_be(DisasContext *ctx, int reg,
                                              CapRegState state)
{
#ifdef ENABLE_STATIC_CAP_OPTS
    return (ctx->base.cap_compression_states[reg] == (1 << state));
#else
    return false;
#endif
}

// Must be one of two
static inline bool disas_capreg_state_must_be2(DisasContext *ctx, int reg,
                                               CapRegState state1,
                                               CapRegState state2)
{
#ifdef ENABLE_STATIC_CAP_OPTS
    return !(ctx->base.cap_compression_states[reg] &
             ~((1 << state1) | (1 << state2)));
#else
    return false;
#endif
}

static inline void disas_capreg_state_set_unknown(DisasContext *ctx, int reg)
{
#ifdef ENABLE_STATIC_CAP_OPTS
    if (lazy_capreg_number_is_special(reg))
        return;
    ctx->base.cap_compression_states[reg] = ALL_CAPREG_STATES;
#endif
}

static inline void disas_capreg_state_set(DisasContext *ctx, int reg,
                                          CapRegState state)
{
#ifdef ENABLE_STATIC_CAP_OPTS
    if (lazy_capreg_number_is_special(reg))
        return;
    ctx->base.cap_compression_states[reg] = 1 << state;
#endif
}

static inline void disas_capreg_state_include(DisasContext *ctx, int reg,
                                              CapRegState state)
{
#ifdef ENABLE_STATIC_CAP_OPTS
    if (lazy_capreg_number_is_special(reg))
        return;
    ctx->base.cap_compression_states[reg] |= 1 << state;
#endif
}

static inline void disas_capreg_state_remove(DisasContext *ctx, int reg,
                                             CapRegState state)
{
#ifdef ENABLE_STATIC_CAP_OPTS
    if (lazy_capreg_number_is_special(reg))
        return;
    ctx->base.cap_compression_states[reg] &= ~(1 << state);
#endif
}

static inline void gen_lazy_cap_get_state(DisasContext *ctx, TCGv_i32 val,
                                          int regnum);

// Decompress only if not fully decompressed
static inline void gen_conditional_cap_decompress(DisasContext *ctx, int regnum)
{

    cheri_tcg_printf_verbose("c", "Decompress regnum %d (conditional)\n",
                             regnum);

    TCGLabel *l1 = gen_new_label();

    // If state[regnum] == CREG_FULLY_DECOMPRESSED goto l1
    _Static_assert(CREG_FULLY_DECOMPRESSED == 0b11, "I like optimisation");
    TCGv_i32 decompressed = tcg_const_i32(CREG_FULLY_DECOMPRESSED);
    TCGv_i32 val = tcg_temp_new_i32();
    gen_lazy_cap_get_state(ctx, val, regnum);
    tcg_gen_brcond_i32(TCG_COND_EQ, val, decompressed, l1);

    // Call decompress helper
    TCGv_i32 reg = tcg_const_i32(regnum);
    gen_helper_decompress_cap(cpu_env, reg);
    cheri_tcg_printf_verbose("", "Condition true\n");
    // Label l1
    gen_set_label(l1);

    tcg_temp_free_i32(reg);
    tcg_temp_free_i32(decompressed);
    tcg_temp_free_i32(val);
}

// Unconditionally call decompression helper
static inline void gen_unconditional_cap_decompress(DisasContext *ctx,
                                                    int regnum)
{
    cheri_tcg_printf_verbose("c", "Decompress regnum %d\n", regnum);
    TCGv_i32 reg = tcg_const_i32(regnum);
    gen_helper_decompress_cap(cpu_env, reg);
    tcg_temp_free_i32(reg);
}

// Ensures the cap is decompressed
// WARN: calling this will kill any temps
static void gen_ensure_cap_decompressed(DisasContext *ctx, int regnum)
{

    if (disas_capreg_state_must_be(ctx, regnum, CREG_FULLY_DECOMPRESSED))
        return;
    else if (!disas_capreg_state_could_be(ctx, regnum, CREG_FULLY_DECOMPRESSED))
        gen_unconditional_cap_decompress(ctx, regnum);
    else
        gen_conditional_cap_decompress(ctx, regnum);

    disas_capreg_state_set(ctx, regnum, CREG_FULLY_DECOMPRESSED);
}

static inline void gen_cap_get_cursor(DisasContext *ctx, int regnum,
                                      TCGv cursor)
{
    // On platforms with merged register file it is better to do the GPR loads
    // as they will likely refer to globals.
    if (MERGED_FILE && !lazy_capreg_number_is_special(regnum)) {
        target_get_gpr(ctx, cursor, regnum);
    } else {
        if (regnum == NULL_CAPREG_INDEX) {
            tcg_gen_movi_tl(cursor, 0);
        } else {
            tcg_gen_ld_tl(cursor, cpu_env,
                          gp_register_offset(regnum) +
                              offsetof(cap_register_t, _cr_cursor));
        }
    }
    cheri_tcg_printf_verbose("cd", "Get reg %d cursor: %lx\n", regnum, cursor);
}

static inline void gen_reg_modified_cap_base(DisasContext *ctx,
                                             const char *str_name,
                                             size_t env_offset)
{
    if (qemu_ctx_logging_enabled(ctx)) {
        TCGv_ptr name = tcg_const_ptr(str_name);
        TCGv_ptr reg = tcg_const_ptr(env_offset);
        tcg_gen_add_ptr(reg, reg, cpu_env);
        gen_helper_qemu_log_instr_cap(cpu_env, name, reg);
        tcg_temp_free_ptr(reg);
        tcg_temp_free_ptr(name);
    }
}

// Call this at the end of any instruction that modifies a capreg with a new
// value
static inline void gen_reg_modified_cap(DisasContext *ctx, int regnum)
{
    if (regnum == NULL_CAPREG_INDEX)
        return;
    if (qemu_ctx_logging_enabled(ctx)) {
        gen_ensure_cap_decompressed(ctx, regnum);
        gen_reg_modified_cap_base(ctx, cheri_gp_regnames[regnum],
                                  gp_register_offset(regnum));
    }
}

static inline void gen_reg_modified_int_base(DisasContext *ctx,
                                             const char *str_name, TCGv new_val)
{
    if (qemu_ctx_logging_enabled(ctx)) {
        TCGv_ptr name = tcg_const_ptr(str_name);
        gen_helper_qemu_log_instr_reg(cpu_env, name, new_val);
        tcg_temp_free_ptr(name);
    }
}

// Call this at the end of any instruction that modifies a capreg with a new int
// value
static inline void gen_reg_modified_int(DisasContext *ctx, int regnum)
{
    if (regnum == NULL_CAPREG_INDEX)
        return;
    if (qemu_ctx_logging_enabled(ctx)) {
        TCGv new_val = tcg_temp_new();
        gen_cap_get_cursor(ctx, regnum, new_val);
        const char *str_name =
#ifdef TARGET_AARCH64
            cheri_gp_int_regnames[regnum];
#else
            cheri_gp_regnames[regnum];
        // TODO: Add some integer names to riscv/mips
#endif
        gen_reg_modified_int_base(ctx, str_name, new_val);
        tcg_temp_free(new_val);
    }
}
// TODO: Gen some tracing for all of these

// Set the state of a lazy register to a given constant.
// Also sets static state tracking, set conditional true if this is only
// conditionally executed
static inline void gen_lazy_cap_set_state_cond(DisasContext *ctx, int regnum,
                                               CapRegState state,
                                               bool conditional)
{

    if (disas_capreg_state_must_be(ctx, regnum, state))
        return;

    assert(!lazy_capreg_number_is_special(regnum));

    cheri_tcg_printf_verbose("cc", "Register %d lazy state set to %s\n", regnum,
                             cap_reg_state_string(state));

    TCGv_i32 tcg_state = tcg_const_i32(state);
#ifdef TARGET_MIPS
    g_assert_not_reached(); // Not supported on MIPS
#else
    tcg_gen_st8_i32(tcg_state, cpu_env,
                    offsetof(CPUArchState, gpcapregs.capreg_state[regnum]));
#endif
    tcg_temp_free_i32(tcg_state);
    if (conditional)
        disas_capreg_state_include(ctx, regnum, state);
    else
        disas_capreg_state_set(ctx, regnum, state);
}

static inline void gen_lazy_cap_set_state(DisasContext *ctx, int regnum,
                                          CapRegState state)
{
    return gen_lazy_cap_set_state_cond(ctx, regnum, state, false);
}

static inline void gen_lazy_cap_get_state(DisasContext *ctx, TCGv_i32 val,
                                          int regnum)
{
#ifdef TARGET_MIPS
    g_assert_not_reached(); // Not supported on MIPS
#else
    tcg_gen_ld8u_i32(val, cpu_env,
                     offsetof(CPUArchState, gpcapregs.capreg_state[regnum]));
    cheri_tcg_printf_verbose("cw", "Register %d lazy state get: %d\n", regnum,
                             val);
#endif
}

static inline void gen_lazy_cap_set_int_cond(DisasContext *ctx, int regnum,
                                             bool conditional)
{
    if (regnum == NULL_CAPREG_INDEX)
        return;
    if (disas_capreg_state_must_be(ctx, regnum, CREG_INTEGER))
        return;

    gen_lazy_cap_set_state_cond(ctx, regnum, CREG_INTEGER, conditional);
    // Doing this keeps pesbt always up to date, which is good for stores and
    // comparisons.
    TCGv null_pesbt = tcg_const_tl(CAP_NULL_PESBT);
    tcg_gen_st_tl(null_pesbt, cpu_env,
                  gp_register_offset(regnum) +
                      offsetof(cap_register_t, cached_pesbt));
    tcg_temp_free(null_pesbt);
}

static inline void gen_lazy_cap_set_int(DisasContext *ctx, int regnum)
{
    gen_lazy_cap_set_int_cond(ctx, regnum, false);
#ifdef TRACE_MODIFIED_REGISTERS
    gen_reg_modified_int(ctx, regnum);
#endif
}

#endif // defined(TARGET_CHERI)
#if defined(TARGET_CHERI) && TARGET_LONG_BITS == 64
// NOTE: I wrote all this for 128, but it may apply to other formats.

// Null everything but cursor at some general offset. Use this for special
// purpose registers. The GP register file can be handled lazily with
// gen_lazy_cap_set_int.
static inline void gen_sp_set_decompressed_int(DisasContext *ctx, size_t offset)
{
    // PESBT
    TCGv_i64 temp = tcg_const_i64(CC128_NULL_PESBT);
    tcg_gen_st_i64(temp, cpu_env,
                   offset + offsetof(cap_register_t, cached_pesbt));
    // Type
    tcg_gen_movi_i64(temp, CAP_OTYPE_UNSEALED);
    tcg_gen_st_i64(temp, cpu_env, offset + offsetof(cap_register_t, cr_otype));
    // Base + Perms
    tcg_gen_movi_i64(temp, 0);
    tcg_gen_st_i64(temp, cpu_env, offset + offsetof(cap_register_t, cr_base));
    tcg_gen_st_i64(temp, cpu_env, offset + offsetof(cap_register_t, cr_perms));
    // Top
    tcg_gen_st_i64(temp, cpu_env,
                   offset + offsetof(cap_register_t, _cr_top) +
                       CAP_TOP_LOBYTES_OFFSET);
    tcg_gen_movi_i64(temp, 1);
    tcg_gen_st_i64(temp, cpu_env,
                   offset + offsetof(cap_register_t, _cr_top) +
                       CAP_TOP_HIBYTES_OFFSET);

#ifdef TARGET_AARCH64
    // Bounds valid
    tcg_gen_st_i64(temp, cpu_env,
                   offset + offsetof(cap_register_t, cr_bounds_valid));
#endif

    tcg_temp_free_i64(temp);
}

// In a merged register file, cursors are also globals. If backing is to also be
// used for memory operations, We need to take special
static inline void gen_cap_sync_cursor(DisasContext *ctx, int regnum)
{
#if MERGED_FILE
    if (!lazy_capreg_number_is_special(regnum))
        tcg_gen_sync_i64(target_get_gpr_global(ctx, regnum));
#endif
}

static inline void gen_cap_invalidate_cursor(DisasContext *ctx, int regnum)
{
#if MERGED_FILE
    if (!lazy_capreg_number_is_special(regnum))
        tcg_gen_discard_i64(target_get_gpr_global(ctx, regnum));
#endif
}

// Set a lazy GP to the state of a special purpose register in env
static inline void gen_move_cap_gp_sp(DisasContext *ctx, int dest_num,
                                      uint32_t source_off)
{
    if (dest_num == NULL_CAPREG_INDEX)
        return;
    gen_move_cap(gp_register_offset(dest_num), source_off);
    gen_cap_invalidate_cursor(ctx, dest_num);
    gen_lazy_cap_set_state(ctx, dest_num, CREG_FULLY_DECOMPRESSED);
}

// Set a special purpose register in env to the state of lazy capreg
static inline void gen_move_cap_sp_gp(DisasContext *ctx, uint32_t dest_off,
                                      int source_num)
{
    gen_ensure_cap_decompressed(ctx, source_num);
    gen_cap_sync_cursor(ctx, source_num);
    gen_move_cap(dest_off, gp_register_offset(source_num));
}

// TODO Its probably only worth doing the whole move if decompressed. If
// compressed just copy pesbt. Move a GP register to a GP register. Currently
// decompresses before move.
static inline void gen_move_cap_gp_gp(DisasContext *ctx, int dest_num,
                                      int source_num)
{
    if (dest_num == NULL_CAPREG_INDEX)
        return;
    gen_ensure_cap_decompressed(ctx, source_num);
    if (dest_num == source_num)
        return;
    cheri_tcg_printf_verbose("cc", "Move to %d from %d\n", dest_num,
                             source_num);
    gen_cap_sync_cursor(ctx, source_num);
    gen_move_cap(gp_register_offset(dest_num), gp_register_offset(source_num));
    gen_cap_invalidate_cursor(ctx, dest_num);
    gen_lazy_cap_set_state(ctx, dest_num, CREG_FULLY_DECOMPRESSED);
}

// Does GP register file operation of dest_reg = (value cond 0) ? true_reg :
// false_reg
static inline void gen_move_cap_gp_select_gp(DisasContext *ctx, int dest_num,
                                             int true_source_num,
                                             int false_source_num, TCGCond cond,
                                             TCGv_i32 value)
{
    if (dest_num == NULL_CAPREG_INDEX)
        return;

    const char *format =
        is_unsigned_cond(cond)
            ? "Conditional move to %d from (%u %d 0) ? %d : %d\n"
            : "Conditional move to %d from (%d %s 0) ? %d : %d\n";
    cheri_tcg_printf_verbose("cwccc", format, dest_num, value,
                             tcg_cond_string(cond), true_source_num,
                             false_source_num);

    assert(disas_capreg_state_must_be(ctx, true_source_num,
                                      CREG_FULLY_DECOMPRESSED));
    assert(disas_capreg_state_must_be(ctx, false_source_num,
                                      CREG_FULLY_DECOMPRESSED));

    gen_cap_sync_cursor(ctx, true_source_num);

    gen_cap_sync_cursor(ctx, false_source_num);

    gen_mov_cap_select(gp_register_offset(dest_num),
                       gp_register_offset(true_source_num),
                       gp_register_offset(false_source_num), cond, value);
    gen_cap_invalidate_cursor(ctx, dest_num);

    gen_lazy_cap_set_state(ctx, dest_num, CREG_FULLY_DECOMPRESSED);
}

// TCG templates to directly modify fields in PESBT. Hopefully we will
// eventually delete most fields and use these. Until then, if a field is
// modified (in decompressed state), then pesbt should be modified too so
// comparisons works.

// TODO: for things in integer state we can just return 0's without loads and
// stores
#define WRAP_PESBT_EXTRACT(name)                                               \
    static inline void gen_cap_pesbt_extract_##name(                           \
        DisasContext *ctx, int regnum, TCGv_i64 result)                        \
    {                                                                          \
        tcg_gen_ld_i64(result, cpu_env,                                        \
                       gp_register_offset(regnum) +                            \
                           offsetof(cap_register_t, cached_pesbt));            \
        tcg_gen_extract_i64(result, result, CC128_FIELD_##name##_START,        \
                            CC128_FIELD_##name##_SIZE);                        \
    }

#define WRAP_PESBT_X(name, X, clear, invert, op, ...)                          \
    static inline void gen_cap_pesbt_##X##_##name(DisasContext *ctx,           \
                                                  int regnum, TCGv_i64 value)  \
    {                                                                          \
        uint32_t offset = gp_register_offset(regnum) +                         \
                          offsetof(cap_register_t, cached_pesbt);              \
        tcg_gen_shli_i64(value, value, CC128_FIELD_##name##_START);            \
        if (invert)                                                            \
            tcg_gen_not_i64(value, value);                                     \
        TCGv_i64 pesbt = tcg_temp_new_i64();                                   \
        tcg_gen_ld_i64(pesbt, cpu_env, offset);                                \
        if (clear)                                                             \
            tcg_gen_andi_i64(pesbt, pesbt, ~(CC128_FIELD_##name##_MASK64));    \
        tcg_gen_##op##_i64(pesbt, pesbt, __VA_ARGS__);                         \
        tcg_gen_st_i64(pesbt, cpu_env, offset);                                \
        tcg_temp_free_i64(pesbt);                                              \
    }

static inline void gen_cap_load_pesbt(DisasContext *ctx, int regnum,
                                      TCGv_i64 pesbt)
{
    tcg_gen_ld_i64(pesbt, cpu_env,
                   gp_register_offset(regnum) +
                       offsetof(cap_register_t, cached_pesbt));
}

#define WRAP_PESBT_DEPOSIT(name)                                               \
    WRAP_PESBT_X(name, deposit, true, false, or, value)
#define WRAP_PESBT_SET(name) WRAP_PESBT_X(name, set, false, false, or, value)
#define WRAP_PESBT_CLEAR(name)                                                 \
    WRAP_PESBT_X(name, clear, false, true, and, value)

#define WRAP_PESBT_ALL(name)                                                   \
    WRAP_PESBT_EXTRACT(name)                                                   \
    WRAP_PESBT_DEPOSIT(name)                                                   \
    WRAP_PESBT_SET(name)                                                       \
    WRAP_PESBT_CLEAR(name)

WRAP_PESBT_ALL(OTYPE)
WRAP_PESBT_ALL(HWPERMS)
#ifndef TARGET_ARRCH64
WRAP_PESBT_ALL(FLAGS)
#endif

// Set the tag bit of register to the lowest bit of tagbit
static inline void gen_cap_set_tag(DisasContext *ctx, int regnum, TCGv tagbit,
                                   bool canonicalise)
{

    TCGv one = tcg_const_tl(1);

    if (canonicalise)
        tcg_gen_and_tl(tagbit, tagbit, one);

    if (disas_capreg_state_must_be(ctx, regnum, CREG_FULLY_DECOMPRESSED)) {
        tcg_gen_st8_tl(tagbit, cpu_env,
                       gp_register_offset(regnum) +
                           offsetof(cap_register_t, cr_tag));
    } else {
        /*
         * Just set the state of the lazy register. This will cause next
         * decompression to set the tag. 0b01 is untagged, 0b10 is tagged.
         * So do 0b01 + tagbit to convert.
         */
        _Static_assert(CREG_UNTAGGED_CAP == 1 && CREG_TAGGED_CAP == 2,
                       "Optimised for these values");
        tcg_gen_add_tl(tagbit, tagbit, one);
#ifdef TARGET_MIPS
        g_assert_not_reached(); // Not supported on MIPS
#else
        tcg_gen_st8_i64(tagbit, cpu_env,
                        offsetof(CPUArchState, gpcapregs.capreg_state[regnum]));
#endif
        disas_capreg_state_set(ctx, regnum, CREG_UNTAGGED_CAP);
        disas_capreg_state_include(ctx, regnum, CREG_TAGGED_CAP);
    }

    tcg_temp_free(one);
}

static inline void gen_cap_clear_tag(DisasContext *ctx, int regnum)
{
    if (disas_capreg_state_must_be(ctx, regnum, CREG_FULLY_DECOMPRESSED)) {
        TCGv_i32 tag = tcg_const_i32(0);
        tcg_gen_st8_i32(tag, cpu_env,
                        gp_register_offset(regnum) +
                            offsetof(cap_register_t, cr_tag));
        tcg_temp_free_i32(tag);
    } else {
        gen_lazy_cap_set_state(ctx, regnum, CREG_UNTAGGED_CAP);
    }
}

static inline void gen_cap_get_tag_i32(DisasContext *ctx, int regnum,
                                       TCGv_i32 tagged)
{

    if (disas_capreg_state_must_be(ctx, regnum, CREG_TAGGED_CAP)) {
        tcg_gen_movi_i32(tagged, 1);
    } else if (disas_capreg_state_must_be2(ctx, regnum, CREG_UNTAGGED_CAP,
                                           CREG_INTEGER)) {
        tcg_gen_movi_i32(tagged, 0);
    } else {
        // If fully decompressed just read the field
        tcg_gen_ld8u_i32(tagged, cpu_env,
                         gp_register_offset(regnum) +
                             offsetof(cap_register_t, cr_tag));

        if (!disas_capreg_state_must_be(ctx, regnum, CREG_FULLY_DECOMPRESSED)) {
            // I think this code is more evidence that tag should always be a
            // valid field, and CREG state Just track integer / compressed /
            // decompressed

            // State == CREG_TAGGED_CAP || (State == CREG_FULLY_DECOMPRESSED &&
            // tag = 1)
            TCGv_i32 state = tcg_temp_new_i32();
            gen_lazy_cap_get_state(ctx, state, regnum);

            TCGv_i32 cmpv = tcg_const_i32(CREG_FULLY_DECOMPRESSED);
            tcg_gen_setcond_i32(TCG_COND_EQ, cmpv, cmpv, state);
            tcg_gen_and_i32(tagged, tagged, cmpv);

            tcg_gen_movi_i32(cmpv, CREG_TAGGED_CAP);
            tcg_gen_setcond_i32(TCG_COND_EQ, cmpv, cmpv, state);
            tcg_gen_or_i32(tagged, tagged, cmpv);

            tcg_temp_free_i32(state);
            tcg_temp_free_i32(cmpv);
        }
    }

    cheri_tcg_printf_verbose("cw", "Get reg %d tag: %d\n", regnum, tagged);
}

static inline void gen_cap_get_tag(DisasContext *ctx, int regnum,
                                   TCGv_i64 tagged)
{
    TCGv_i32 tag32 = tcg_temp_new_i32();
    gen_cap_get_tag_i32(ctx, regnum, tag32);
    tcg_gen_extu_i32_i64(tagged, tag32);
    tcg_temp_free_i32(tag32);
}

static inline void gen_cap_get_type(DisasContext *ctx, int regnum, TCGv type)
{
    if (disas_capreg_state_must_be(ctx, regnum, CREG_FULLY_DECOMPRESSED)) {
        tcg_gen_ld32u_tl(type, cpu_env,
                         gp_register_offset(regnum) +
                             offsetof(cap_register_t, cr_otype));
    } else {
        gen_cap_pesbt_extract_OTYPE(ctx, regnum, type);
    }
    cheri_tcg_printf_verbose("cd", "Get reg %d type: %d\n", regnum, type);
}

// returns -1 for unsealed, rather than 0
static inline void gen_cap_get_type_for_copytype(DisasContext *ctx, int regnum,
                                                 TCGv type)
{
    gen_cap_get_type(ctx, regnum, type);
    TCGv temp1 = tcg_const_tl(0);
    tcg_gen_setcond_tl(TCG_COND_EQ, temp1, temp1, type);
    tcg_gen_sub_tl(type, type, temp1);
    tcg_temp_free(temp1);
}

static inline void gen_cap_set_type_unchecked(DisasContext *ctx, int regnum,
                                              TCGv type)
{
    cheri_tcg_printf_verbose("cd", "Set reg %d type: %d\n", regnum, type);
    if (disas_capreg_state_could_be(ctx, regnum, CREG_FULLY_DECOMPRESSED)) {
        tcg_gen_st32_tl(type, cpu_env,
                        gp_register_offset(regnum) +
                            offsetof(cap_register_t, cr_otype));
    }
    gen_cap_pesbt_deposit_OTYPE(ctx, regnum, type);
}

static inline void gen_cap_get_sealed(DisasContext *ctx, int regnum,
                                      TCGv sealed)
{
    gen_cap_get_type(ctx, regnum, sealed);
    TCGv type_unsealed = tcg_const_tl(CAP_OTYPE_UNSEALED);
    tcg_gen_setcond_tl(TCG_COND_NE, sealed, sealed, type_unsealed);
    tcg_temp_free(type_unsealed);
}

static inline void gen_cap_get_unsealed(DisasContext *ctx, int regnum,
                                        TCGv sealed)
{
    gen_cap_get_type(ctx, regnum, sealed);
    TCGv type_unsealed = tcg_const_tl(CAP_OTYPE_UNSEALED);
    tcg_gen_setcond_tl(TCG_COND_EQ, sealed, sealed, type_unsealed);
    tcg_temp_free(type_unsealed);
}

static inline void gen_cap_get_sealed_i32(DisasContext *ctx, int regnum,
                                          TCGv_i32 sealed)
{
    TCGv sealedv = tcg_temp_new();
    gen_cap_get_sealed(ctx, regnum, sealedv);
    tcg_gen_extrl_i64_i32(sealed, sealedv);
    tcg_temp_free(sealedv);
}

static inline void gen_cap_get_perms(DisasContext *ctx, int regnum, TCGv perms)
{
    if (disas_capreg_state_must_be(ctx, regnum, CREG_FULLY_DECOMPRESSED)) {
        tcg_gen_ld32u_tl(perms, cpu_env,
                         gp_register_offset(regnum) +
                             offsetof(cap_register_t, cr_perms));
    } else {
        gen_cap_pesbt_extract_HWPERMS(ctx, regnum, perms);
    }
    cheri_tcg_printf_verbose("cd", "Ge reg %d perms: %x\n", regnum, perms);
}

static inline void gen_cap_get_hi(DisasContext *ctx, int regnum, TCGv_i64 lim)
{
    size_t offset =
        gp_register_offset(regnum) + offsetof(cap_register_t, cached_pesbt);
    // The pesbt field has been XOR'd with a mask on load
    // This will need to be undone again here.
    // (I think it might be sensible to refactor everything to apply the mask at
    // decompress time)
    tcg_gen_ld_i64(lim, cpu_env, offset);
    tcg_gen_xori_i64(lim, lim, CC128_NULL_PESBT);
    cheri_tcg_printf_verbose("cd", "Get reg %d hi: %lx\n", regnum, lim);
}

// Get top, clamping results to UINT64_T MAX
static inline void gen_cap_get_top_clamped(DisasContext *ctx, int regnum,
                                           TCGv_i64 top)
{
    gen_ensure_cap_decompressed(ctx, regnum);
    size_t offset =
        gp_register_offset(regnum) + offsetof(cap_register_t, _cr_top);
    tcg_gen_ld_i64(top, cpu_env, offset + CAP_TOP_LOBYTES_OFFSET);
    // Set to all ones if top bit of 65-bit top is set.
    TCGv_i64 tmp = tcg_temp_new_i64();
    tcg_gen_ld_i64(tmp, cpu_env, offset + CAP_TOP_HIBYTES_OFFSET);
    // Could have any others bits in a top with a high bit set
    tcg_gen_neg_i64(tmp, tmp);
    tcg_gen_or_i64(top, top, tmp);
    tcg_temp_free_i64(tmp);
    cheri_tcg_printf_verbose("cd", "Get reg %d top: %lx\n", regnum, top);
}

static inline void gen_cap_get_top_lo(DisasContext *ctx, int regnum,
                                      TCGv_i64 top_lo)
{
    gen_ensure_cap_decompressed(ctx, regnum);
    size_t offset =
        gp_register_offset(regnum) + offsetof(cap_register_t, _cr_top);
    tcg_gen_ld_i64(top_lo, cpu_env, offset + CAP_TOP_LOBYTES_OFFSET);
}

static inline void gen_cap_get_top_hi(DisasContext *ctx, int regnum,
                                      TCGv_i64 top_hi)
{
    gen_ensure_cap_decompressed(ctx, regnum);
    size_t offset =
        gp_register_offset(regnum) + offsetof(cap_register_t, _cr_top);
    tcg_gen_ld_i64(top_hi, cpu_env, offset + CAP_TOP_HIBYTES_OFFSET);
}

// Does addr + offset <= top. If offset non zero, Addr MUST be a multiple of
// offset.
static inline void gen_cap_addr_below_top(DisasContext *ctx, int regnum,
                                          TCGv_i64 addr, TCGv_i64 result,
                                          int offset)
{
    // Because of the above invariant,
    TCGv_i64 temp = tcg_temp_new_i64();
    // Offsets of less than one can be folded into the comparison
    if (offset > 1) {
        TCGv_i64 addrtmp = tcg_temp_new_i64();
        tcg_gen_movi_i64(addrtmp, offset);
        tcg_gen_add_i64(addrtmp, addrtmp, addr);
        addr = addrtmp;
    }
    gen_cap_get_top_lo(ctx, regnum, temp);
    tcg_gen_setcond_i64(offset == 1 ? TCG_COND_LTU : TCG_COND_LEU, result, addr,
                        temp);
    if (offset > 1) {
        // The only overflow that can occur will make address exactly 0.
        tcg_gen_movi_i64(temp, 0);
        tcg_gen_setcond_i64(TCG_COND_NE, temp, addr, temp);
        // doing this before the or below will make full length caps still work
        // properly
        tcg_gen_and_i64(result, result, temp);
        tcg_temp_free_i64(addr);
    }
    gen_cap_get_top_hi(ctx, regnum, temp);
    tcg_gen_or_i64(result, result, temp);
    tcg_temp_free_i64(temp);
}

static inline void gen_cap_get_base(DisasContext *ctx, int regnum,
                                    TCGv_i64 base)
{
    gen_ensure_cap_decompressed(ctx, regnum);
    size_t offset =
        gp_register_offset(regnum) + offsetof(cap_register_t, cr_base);
    tcg_gen_ld_i64(base, cpu_env, offset);
    cheri_tcg_printf_verbose("cd", "Get reg %d base: %lx\n", regnum, base);
}

// Get length, where LENGTH_MAX results in UINT64_T MAX
static inline void gen_cap_get_length(DisasContext *ctx, int regnum,
                                      TCGv_i64 length)
{
    gen_ensure_cap_decompressed(ctx, regnum);
    size_t top_offset =
        gp_register_offset(regnum) + offsetof(cap_register_t, _cr_top);
    size_t base_offset =
        gp_register_offset(regnum) + offsetof(cap_register_t, cr_base);

    TCGv_i64 tmp = tcg_temp_new_i64();
    TCGv_i64 carry = tcg_temp_new_i64();
    tcg_gen_ld_i64(length, cpu_env, top_offset + CAP_TOP_LOBYTES_OFFSET);
    tcg_gen_ld_i64(tmp, cpu_env, base_offset);

    // Carry of top - base
    tcg_gen_setcond_i64(TCG_COND_LTU, carry, length, tmp);
    // top - base
    tcg_gen_sub_i64(length, length, tmp);
    // 1 if we should clamp
    tcg_gen_ld_i64(tmp, cpu_env, top_offset + CAP_TOP_HIBYTES_OFFSET);
    tcg_gen_xor_i64(carry, carry, tmp);

    tcg_gen_neg_i64(carry, carry);
    tcg_gen_or_i64(length, length, carry);

    tcg_temp_free_i64(carry);
    tcg_temp_free_i64(tmp);
    cheri_tcg_printf_verbose("cd", "Get reg %d length: %lx\n", regnum, length);
}

static inline void gen_cap_get_base_below_top(DisasContext *ctx, int regnum,
                                              TCGv_i64 result)
{
    // TODO: Is this possible for caps that are validly decoded? In which case
    // this can be replaced with a single load.
    gen_cap_get_base(ctx, regnum, result);
    gen_cap_addr_below_top(ctx, regnum, result, result, 0);
}

static inline void gen_cap_set_cursor_unsafe(DisasContext *ctx, int regnum,
                                             TCGv_i64 new_cursor)
{
    if (regnum == NULL_CAPREG_INDEX)
        return;
    if (MERGED_FILE && !lazy_capreg_number_is_special(regnum)) {
#if MERGED_FILE
        target_set_gpr(ctx, regnum, new_cursor);
#endif
    } else {
        tcg_gen_st_i64(new_cursor, cpu_env,
                       gp_register_offset(regnum) +
                           offsetof(cap_register_t, _cr_cursor));
    }
}

// Gets the address part of the capability cursor (on morello this is different
// from the cursor)
static inline void gen_cap_get_cursor_addr(DisasContext *ctx,
                                           TCGv_i64 cursor_addr,
                                           TCGv_i64 cursor)
{
#ifdef TARGET_AARCH64
    // Remove top 8 bits then sign extend
    tcg_gen_shli_i64(cursor_addr, cursor, MORELLO_FLAG_BITS);
    tcg_gen_sari_i64(cursor_addr, cursor_addr, MORELLO_FLAG_BITS);
#else
    tcg_gen_mov_i64(cursor_addr, cursor);
#endif
}

static inline void gen_cap_get_offset(DisasContext *ctx, int regnum,
                                      TCGv_i64 offset)
{
    TCGv_i64 base = tcg_temp_new_i64();
    gen_cap_get_base(ctx, regnum, base);
    gen_cap_get_cursor(ctx, regnum, offset);
    tcg_gen_sub_i64(offset, offset, base);
    tcg_temp_free_i64(base);
    cheri_tcg_printf_verbose("cd", "Get reg %d offset: %lx\n", regnum, offset);
}

static inline void gen_cap_in_bounds(DisasContext *ctx, int regnum,
                                     TCGv_i64 addr, TCGv_i64 result,
                                     uint32_t size)
{
    gen_ensure_cap_decompressed(ctx, regnum);
    TCGv_i64 temp = tcg_temp_new_i64();
    gen_cap_get_base(ctx, regnum, temp);
    tcg_gen_setcond_i64(TCG_COND_GEU, result, addr, temp);
    gen_cap_addr_below_top(ctx, regnum, addr, temp, size);
    tcg_gen_and_i64(result, result, temp);
#ifdef TARGET_AARCH64
    // On Morello all invalid exponant caps are always out of bounds.
    tcg_gen_ld8u_i64(temp, cpu_env,
                     gp_register_offset(regnum) +
                         offsetof(cap_register_t, cr_bounds_valid));
    tcg_gen_and_i64(result, result, temp);
#endif
    tcg_temp_free_i64(temp);
    cheri_tcg_printf_verbose("cd", "Get reg %d in bounds: %d\n", regnum,
                             result);
}

static inline void gen_cap_has_perms(DisasContext *ctx, int regnum,
                                     uint32_t perms, TCGv_i64 result)
{
    gen_cap_load_pesbt(ctx, regnum, result);
    TCGv_i64 compare = tcg_const_i64(cc128_cap_pesbt_encode_HWPERMS(perms));
    tcg_gen_and_i64(result, result, compare);
    tcg_gen_setcond_i64(TCG_COND_EQ, result, result, compare);
    tcg_temp_free_i64(compare);
}

// TODO: handle the slightly different semantics around precise and im-precise
// checks Handles sealed and unrepresentable caps when the cursor is changed. If
// you are only changing flag bits, specify flag_bits_only. Do NOT modify cursor
// before calling this, as the old cursor may be used for the check. WARN:
// calling this will kill any temps
static inline void gen_cap_set_cursor(DisasContext *ctx, int regnum,
                                      TCGv_i64 new_val, bool flag_bits_only)
{

    cheri_tcg_printf_verbose("cd", "Set reg %d cursor: %lx\n", regnum, new_val);

    // TODO: This is not correct on non-morello where this might be exceptional
    // behaviour
    if (disas_capreg_state_must_be2(ctx, regnum, CREG_INTEGER,
                                    CREG_UNTAGGED_CAP)) {
        gen_cap_set_cursor_unsafe(ctx, regnum, new_val);
        return;
    }

    // Use a local value to store new_val (as we branch later)
    // new_val can be used as a tmp
    TCGv_i64 new_val_local = tcg_temp_local_new_i64();
    tcg_gen_mov_i64(new_val_local, new_val);
    TCGv_i64 temp0 = new_val;

    gen_ensure_cap_decompressed(ctx, regnum);

    TCGv_i64 possible_bad_modification = tcg_temp_new_i64();
    gen_cap_get_sealed(ctx, regnum, possible_bad_modification);

    // Modifying non flag bits can make capabilities unrepresentable
    if (!flag_bits_only) {

        TCGv_i64 temp1 = tcg_temp_new_i64();

        // In bounds -> representable. If either out of bounds or sealed we will
        // instead go to a helper That will pay more attenttion and can also do
        // platform specific things like throw an exception / untag.
        gen_cap_get_cursor_addr(ctx, temp0, new_val_local);

        gen_cap_get_base(ctx, regnum, temp1);
        tcg_gen_setcond_i64(TCG_COND_LTU, temp1, temp0, temp1);
        tcg_gen_or_i64(possible_bad_modification, possible_bad_modification,
                       temp1);

        // OK to use 64-bit top as falls over to a more complicated check
        gen_cap_get_top_clamped(ctx, regnum, temp1);
        tcg_gen_setcond_i64(TCG_COND_GTU, temp1, temp0, temp1);
        tcg_gen_or_i64(possible_bad_modification, possible_bad_modification,
                       temp1);

#ifdef TARGET_AARCH64
        // On morello the top non-flag bit changing can cause problems.
        // If it does it is best to go to the more complicated check.
        gen_cap_get_cursor(ctx, regnum, temp1);
        tcg_gen_xor_i64(temp1, temp1, temp0);
        tcg_gen_movi_i64(temp0, 1ULL << (63 - MORELLO_FLAG_BITS));
        tcg_gen_and_i64(temp1, temp1, temp0);
        tcg_gen_or_i64(possible_bad_modification, possible_bad_modification,
                       temp1);
#endif

        cheri_tcg_printf_verbose("d", "Quick check fail: %d\n",
                                 possible_bad_modification);

        TCGLabel *l1 = gen_new_label();
        // if (possible_bad_modification)
        tcg_gen_movi_i64(temp1, 0);
        tcg_gen_brcond_i64(TCG_COND_EQ, possible_bad_modification, temp1, l1);
        // call handler (just do a setaddr)
        TCGv_i32 tcg_regnum = tcg_const_i32(regnum);
        gen_helper_csetaddr(cpu_env, tcg_regnum, tcg_regnum, new_val_local);
        tcg_temp_free_i32(tcg_regnum);
        // else
        gen_set_label(l1);
        gen_cap_set_cursor_unsafe(ctx, regnum, new_val_local);
        tcg_temp_free_i64(temp1);
    } else {
        cheri_tcg_printf_verbose("", "Only modifying flags\n",
                                 possible_bad_modification);
        tcg_gen_ld8u_tl(temp0, cpu_env,
                        gp_register_offset(regnum) +
                            offsetof(cap_register_t, cr_tag));
        tcg_gen_not_i64(possible_bad_modification, possible_bad_modification);
        tcg_gen_and_i64(temp0, possible_bad_modification, temp0);
        tcg_gen_st8_tl(temp0, cpu_env,
                       gp_register_offset(regnum) +
                           offsetof(cap_register_t, cr_tag));
        gen_cap_set_cursor_unsafe(ctx, regnum, new_val_local);
    }

    tcg_temp_free_i64(new_val_local);
    tcg_temp_free_i64(possible_bad_modification);
}

// Clear tag bit if sealed
// WARN: calling this will kill any temps
static inline void gen_cap_untag_if_sealed(DisasContext *ctx, int regnum)
{

    // Handling a bunch of different cases here because this will be used by
    // most cap instructions on morello

    // Untagged things can never be untagged
    if (disas_capreg_state_must_be2(ctx, regnum, CREG_INTEGER,
                                    CREG_UNTAGGED_CAP))
        return;

    TCGv type = tcg_temp_new();
    gen_cap_get_type(ctx, regnum, type);

    TCGv type_unsealed = tcg_const_tl(CAP_OTYPE_UNSEALED);

    if (disas_capreg_state_must_be(ctx, regnum, CREG_FULLY_DECOMPRESSED)) {
        // If decompressed perform an and with the existing tagbit to avoid a
        // branch
        uint32_t offset =
            gp_register_offset(regnum) + offsetof(cap_register_t, cr_tag);
        tcg_gen_setcond_tl(TCG_COND_EQ, type, type, type_unsealed);
        tcg_gen_ld8u_tl(type_unsealed, cpu_env, offset);
        tcg_gen_and_tl(type, type, type_unsealed);
        tcg_gen_st8_tl(type, cpu_env, offset);
    } else {
        // If not fully decompressed its probably just worth branching over a
        // generic tag clear
        TCGLabel *l1 = gen_new_label();
        // if(type != unsealed)
        tcg_gen_brcond_tl(TCG_COND_EQ, type, type_unsealed, l1);
        // set tag 0
        tcg_gen_movi_tl(type_unsealed, 0);
        gen_cap_set_tag(ctx, regnum, type_unsealed, false);
        // else
        gen_set_label(l1);
    }

    tcg_temp_free(type);
    tcg_temp_free(type_unsealed);
}

static inline void gen_cap_get_flags(DisasContext *ctx, int regnum,
                                     TCGv_i64 flags)
{
#ifdef TARGET_AARCH64
    // Morello flags are just different from CHERI flags.
    gen_cap_get_cursor(ctx, regnum, flags);
    tcg_gen_andi_i64(flags, flags, 0xFFULL << (64 - MORELLO_FLAG_BITS));
#else
    gen_cap_pesbt_extract_FLAGS(ctx, regnum, flags);
#endif
}

// Returns a boolean if rx and ry have equal pesbt/tag/cursor.
// You may be concerned about equivalent non-canonical capabilities not
// comparing equal, but to this I say: 1) This is why we cant have nice things.
// 2) Morello docs imply this is what would happen on hardware, other platforms
// can write their own TCG.
static inline void gen_cap_get_eq_i32(DisasContext *ctx, int rx, int ry,
                                      TCGv_i32 eq)
{
    size_t offsetx = gp_register_offset(rx);
    size_t offsety = gp_register_offset(ry);

    TCGv_i64 tmp1 = tcg_temp_new_i64();
    TCGv_i64 tmp2 = tcg_temp_new_i64();

    TCGv_i32 tmp3 = tcg_temp_new_i32();

    // Tags equal
    gen_cap_get_tag_i32(ctx, rx, eq);
    gen_cap_get_tag_i32(ctx, ry, tmp3);
    tcg_gen_eqv_i32(eq, eq, tmp3);

    // Cursor equal
    gen_cap_get_cursor(ctx, rx, tmp1);
    gen_cap_get_cursor(ctx, ry, tmp2);

    tcg_gen_setcond_i64(TCG_COND_EQ, tmp1, tmp1, tmp2);
    tcg_gen_extrl_i64_i32(tmp3, tmp1);
    tcg_gen_and_i32(eq, eq, tmp3);

    // Pesbt equal
    tcg_gen_ld_i64(tmp1, cpu_env,
                   offsetx + offsetof(cap_register_t, cached_pesbt));
    tcg_gen_ld_i64(tmp2, cpu_env,
                   offsety + offsetof(cap_register_t, cached_pesbt));
    tcg_gen_setcond_i64(TCG_COND_EQ, tmp1, tmp1, tmp2);
    tcg_gen_extrl_i64_i32(tmp3, tmp1);
    tcg_gen_and_i32(eq, eq, tmp3);

    tcg_temp_free_i64(tmp1);
    tcg_temp_free_i64(tmp2);
    tcg_temp_free_i32(tmp3);

    cheri_tcg_printf_verbose("ccw", "Get reg %d equal reg %d: %d\n", rx, ry,
                             eq);
}

static inline void gen_cap_clear_perms(DisasContext *ctx, int regnum,
                                       TCGv_i64 mask, bool canon)
{
    if (regnum == NULL_CAPREG_INDEX)
        return;

    if (canon)
        tcg_gen_andi_i64(mask, mask, CC128_FIELD_HWPERMS_MASK_NOT_SHIFTED);

    // Could get rid of this by removing cr_perms completely
    if (disas_capreg_state_could_be(ctx, regnum, CREG_FULLY_DECOMPRESSED)) {
        TCGv_i64 tmp = tcg_temp_new_i64();
        int offset =
            gp_register_offset(regnum) + offsetof(cap_register_t, cr_perms);
        tcg_gen_ld32u_i64(tmp, cpu_env, offset);
        tcg_gen_andc_i64(tmp, tmp, mask);
        tcg_gen_st32_i64(tmp, cpu_env, offset);
        tcg_temp_free_i64(tmp);
    }

    gen_cap_pesbt_clear_HWPERMS(ctx, regnum, mask);
}

// Set a capability to a fixed (tag clearing if sealed and check_sealed)
static inline void gen_cap_set_type_const(DisasContext *ctx, int regnum,
                                          target_ulong type, bool check_sealed)
{

    if (regnum == NULL_CAPREG_INDEX)
        return;

    if (check_sealed) {
        gen_cap_untag_if_sealed(ctx, regnum);
    }

    // Now set the type
    TCGv_i64 tmp = tcg_const_i64(type);
    gen_cap_set_type_unchecked(ctx, regnum, tmp);
    tcg_temp_free_i64(tmp);
}

static inline void gen_cap_seal(DisasContext *ctx, int regnum, int auth_regnum,
                                bool conditional, TCGv_i64 success)
{

    if (regnum == NULL_CAPREG_INDEX)
        return;

    gen_ensure_cap_decompressed(ctx, auth_regnum);

    // Copy low bits of auth_regnum to regnum's type, clearing regnums tag if
    // conditions not met

    TCGv_i64 tag_result = tcg_temp_new_i64();
    TCGv_i64 temp0 = tcg_temp_new_i64();
    TCGv_i64 new_type = tcg_temp_new_i64();

    gen_cap_get_cursor(ctx, auth_regnum, new_type);

    // regnum unsealed
    gen_cap_get_unsealed(ctx, regnum, tag_result);

    // auth_regnum unsealed
    gen_cap_get_unsealed(ctx, auth_regnum, temp0);
    tcg_gen_and_i64(tag_result, tag_result, temp0);

    // Set type of regnum
    tcg_gen_movi_i64(temp0, CC128_FIELD_OTYPE_MASK_NOT_SHIFTED);

    if (!conditional) {
        tcg_gen_and_i64(temp0, temp0, new_type);
        gen_cap_set_type_unchecked(ctx, regnum, temp0);
    } else {
        // success = (type & CC128_FIELD_OTYPE_MASK_NOT_SHIFTED) ==
        // CC128_FIELD_OTYPE_MASK_NOT_SHIFTED;
        tcg_gen_and_i64(success, temp0, new_type);
        tcg_gen_setcond_i64(TCG_COND_EQ, success, success, temp0);
    }

    // type <= CAP_MAX_REPRESENTABLE_OTYPE
    tcg_gen_movi_i64(temp0, CAP_MAX_REPRESENTABLE_OTYPE);
    tcg_gen_setcond_i64(TCG_COND_LEU, temp0, new_type, temp0);
    tcg_gen_and_i64(tag_result, tag_result, temp0);

    // auth in bounds
    gen_cap_in_bounds(ctx, auth_regnum, new_type, temp0, 1);
    tcg_gen_and_i64(tag_result, tag_result, temp0);

    // regnum tagged
    gen_cap_get_tag(ctx, regnum, temp0);
    tcg_gen_and_i64(tag_result, tag_result, temp0);

    // auth tagged
    gen_cap_get_tag(ctx, auth_regnum, temp0);
    tcg_gen_and_i64(tag_result, tag_result, temp0);

    // auth has perm
    gen_cap_has_perms(ctx, auth_regnum, CAP_PERM_SEAL, temp0);
    tcg_gen_and_i64(tag_result, tag_result, temp0);

    if (conditional) {
        gen_cap_get_type(ctx, regnum, temp0);
        // type = tag_result && ~success ? new_type : old_type
        tcg_gen_movcond_i64(TCG_COND_LTU, temp0, success, tag_result, new_type,
                            temp0);
        gen_cap_set_type_unchecked(ctx, regnum, temp0);
        tcg_gen_or_i64(success, success, tag_result);
    } else {
        gen_cap_set_tag(ctx, regnum, tag_result, false);
    }

    tcg_temp_free_i64(tag_result);
    tcg_temp_free_i64(temp0);
    tcg_temp_free_i64(new_type);
}

static inline void gen_cap_unseal(DisasContext *ctx, int regnum,
                                  int auth_regnum)
{
    if (regnum == NULL_CAPREG_INDEX)
        return;

    gen_ensure_cap_decompressed(ctx, auth_regnum);

    TCGv_i64 tag_result = tcg_temp_new_i64();
    TCGv_i64 temp0 = tcg_temp_new_i64();
    TCGv_i64 temp1 = tcg_temp_new_i64();

    // Clear global if auth does not have it set

    gen_cap_get_perms(ctx, auth_regnum, temp0);
    tcg_gen_movi_i64(temp1, CAP_PERM_GLOBAL);
    tcg_gen_andc_i64(temp0, temp1, temp0);
    gen_cap_clear_perms(ctx, regnum, temp0, false);

    // Cursor == type
    gen_cap_get_cursor(ctx, auth_regnum, temp0);
    gen_cap_get_type(ctx, regnum, temp1);
    tcg_gen_setcond_i64(TCG_COND_EQ, tag_result, temp0, temp1);

    // type != 0
    tcg_gen_movi_i64(temp0, CAP_OTYPE_UNSEALED);
    tcg_gen_setcond_i64(TCG_COND_NE, temp1, temp0, temp1);
    tcg_gen_and_i64(tag_result, tag_result, temp1);

    // set unsealed
    gen_cap_set_type_unchecked(ctx, regnum, temp0);

    // auth in bounds
    gen_cap_get_cursor(ctx, auth_regnum, temp1);
    gen_cap_in_bounds(ctx, auth_regnum, temp1, temp0, 1);
    tcg_gen_and_i64(tag_result, tag_result, temp0);

    // auth tagged
    gen_cap_get_tag(ctx, auth_regnum, temp0);
    tcg_gen_and_i64(tag_result, tag_result, temp0);

    // regnum tagged
    gen_cap_get_tag(ctx, regnum, temp0);
    tcg_gen_and_i64(tag_result, tag_result, temp0);

    // auth has perm
    gen_cap_has_perms(ctx, auth_regnum, CAP_PERM_UNSEAL, temp0);
    tcg_gen_and_i64(tag_result, tag_result, temp0);

    gen_cap_set_tag(ctx, regnum, tag_result, false);

    tcg_temp_free_i64(tag_result);
    tcg_temp_free_i64(temp0);
    tcg_temp_free_i64(temp1);
}

static inline void gen_cap_is_subset(DisasContext *ctx, int rega, int regb,
                                     TCGv_i64 result)
{
    gen_ensure_cap_decompressed(ctx, rega);
    gen_ensure_cap_decompressed(ctx, regb);
    TCGv_i64 tempa = tcg_temp_new_i64();
    TCGv_i64 tempb = tcg_temp_new_i64();

    // Check bounds
    gen_cap_get_top_lo(ctx, rega, tempa);
    gen_cap_get_top_lo(ctx, regb, tempb);
    tcg_gen_setcond_i64(TCG_COND_LEU, result, tempa, tempb);

    gen_cap_get_top_hi(ctx, rega, tempa);
    gen_cap_get_top_hi(ctx, regb, tempb);

    // (!hia && hib) || (hia == hib && x) <=> x + hib > hia
    tcg_gen_add_i64(result, result, tempb);
    tcg_gen_setcond_i64(TCG_COND_GTU, result, result, tempa);

    gen_cap_get_base(ctx, rega, tempa);
    gen_cap_get_base(ctx, regb, tempb);
    tcg_gen_setcond_i64(TCG_COND_GEU, tempa, tempa, tempb);
    tcg_gen_and_i64(result, result, tempa);

    // Check perms (a.perms & ~b.perms) == 0
    gen_cap_load_pesbt(ctx, rega, tempa);
    gen_cap_load_pesbt(ctx, regb, tempb);

    tcg_gen_andc_i64(tempa, tempa, tempb);
    tcg_gen_movi_i64(tempb, CC128_FIELD_HWPERMS_MASK64);
    tcg_gen_and_i64(tempa, tempa, tempb);
    tcg_gen_movi_i64(tempb, 0);
    tcg_gen_setcond_i64(TCG_COND_EQ, tempa, tempa, tempb);
    tcg_gen_and_i64(result, result, tempa);

    // Finally that both caps have valid bounds
    tcg_gen_ld8u_i64(tempa, cpu_env,
                     gp_register_offset(rega) +
                         offsetof(cap_register_t, cr_bounds_valid));
    tcg_gen_ld8u_i64(tempb, cpu_env,
                     gp_register_offset(regb) +
                         offsetof(cap_register_t, cr_bounds_valid));
    tcg_gen_and_i64(result, result, tempa);
    tcg_gen_and_i64(result, result, tempb);

    tcg_temp_free_i64(tempa);
    tcg_temp_free_i64(tempb);

    cheri_tcg_printf_verbose("ccd", "Get reg %d subset reg %d: %d\n", rega,
                             regb, result);
}

static inline void gen_cap_is_subset_and_tag_eq(DisasContext *ctx, int rega,
                                                int regb, TCGv_i64 result)
{
    gen_cap_is_subset(ctx, rega, regb, result);

    TCGv_i64 tempa = tcg_temp_new_i64();
    TCGv_i64 tempb = tcg_temp_new_i64();

    tcg_gen_ld8u_i64(tempa, cpu_env,
                     gp_register_offset(rega) +
                         offsetof(cap_register_t, cr_tag));
    tcg_gen_ld8u_i64(tempb, cpu_env,
                     gp_register_offset(regb) +
                         offsetof(cap_register_t, cr_tag));

    tcg_gen_xor_i64(tempa, tempa, tempb);
    tcg_gen_andc_i64(result, result, tempa);

    tcg_temp_free_i64(tempa);
    tcg_temp_free_i64(tempb);
}

static inline void gen_cap_set_pesbt(DisasContext *ctx, int regnum,
                                     TCGv_i64 pesbt)
{
    if (regnum == NULL_CAPREG_INDEX)
        return;

    gen_lazy_cap_set_state(ctx, regnum, CREG_UNTAGGED_CAP);
    // Again, once I bother to change where we apply this mask this should go
    // away
    TCGv_i64 mem_pesbt = tcg_const_i64(CC128_NULL_PESBT);
    tcg_gen_xor_i64(mem_pesbt, mem_pesbt, pesbt);
    tcg_gen_st_i64(mem_pesbt, cpu_env,
                   gp_register_offset(regnum) +
                       offsetof(cap_register_t, cached_pesbt));
    tcg_temp_free_i64(mem_pesbt);
}

// Checks a cap is in bounds for a given size, has specified perms, is tagged,
// and not sealed. Or throws an exception. Because of the branch, this will kill
// every temp. Addr is specially conserved as it will probably be used again.
static inline void gen_cap_memop_checks(DisasContext *ctx, int regnum,
                                        TCGv addr, target_ulong size, int perms)
{
#ifdef DO_TCG_BOUNDS_CHECKS
    // We use addr as a tmp, and tmp as address because we can ensure tmp is
    // local
    TCGv_i64 local_addr = tcg_temp_local_new_i64();
    TCGv_i64 result = tcg_temp_new_i64();
    tcg_gen_mov_i64(local_addr, addr);
    TCGv_i64 tmp2 = addr;

    // Bounds
    gen_cap_in_bounds(ctx, regnum, local_addr, result, size);
    // Perms
    gen_cap_has_perms(ctx, regnum, perms, tmp2);
    tcg_gen_and_i64(result, result, tmp2);
    // Unsealed
    gen_cap_get_unsealed(ctx, regnum, tmp2);
    tcg_gen_and_i64(result, result, tmp2);
    // Tagged
    gen_cap_get_tag(ctx, regnum, tmp2);
    tcg_gen_and_i64(result, result, tmp2);

    /* If failure: */
    TCGLabel *skip = gen_new_label();
    tcg_gen_movi_i64(tmp2, 0);
    tcg_gen_brcond_i64(TCG_COND_NE, result, tmp2, skip);
    tcg_gen_mov_i64(addr, local_addr);
#endif
    // We just repeat the checks again in the helper to get the appropriate
    // exception.
    TCGv_i32 tcg_regnum = tcg_const_i32(regnum);
    TCGv_i32 tcg_size = tcg_const_i32(size);
    bool load = perms & CAP_PERM_LOAD;
    bool store = perms & CAP_PERM_STORE;
    // I dislike how there are different helpers for different permissions.
    // Should Refactor.
    ((load && store)
         ? gen_helper_cap_rmw_check
         : (load ? gen_helper_cap_load_check : gen_helper_cap_store_check))(
        (TCGv_cap_checked_ptr)addr, cpu_env, tcg_regnum, addr, tcg_size);
    tcg_temp_free_i32(tcg_regnum);
    tcg_temp_free_i32(tcg_size);
#ifdef DO_TCG_BOUNDS_CHECKS
    /* Else */
    gen_set_label(skip);
    tcg_gen_mov_i64(addr, local_addr);
    tcg_temp_free_i64(local_addr);
    tcg_temp_free_i64(result);
#endif
}

#endif // TARGET_CHERI
