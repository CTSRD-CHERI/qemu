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

#ifdef TARGET_CHERI
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

#ifdef TARGET_MIPS
static inline void gen_load_gpr(TCGv t, int reg);
#endif

static inline void generate_get_ddc_checked_gpr_plus_offset(
    TCGv_cap_checked_ptr addr, DisasContext *ctx, int reg_num,
    target_long offset, MemOp mop,
    void (*check_ddc)(TCGv_cap_checked_ptr, DisasContext *, TCGv, target_ulong))
{
#if defined(TARGET_RISCV)
    gen_get_gpr((TCGv)addr, reg_num);
#elif defined(TARGET_MIPS)
    gen_load_gpr((TCGv)addr, reg_num);
#else
#error "Don't know how to fetch a GPR value"
#endif
    if (!__builtin_constant_p(offset) || offset != 0) {
        tcg_gen_addi_tl((TCGv)addr, (TCGv)addr, offset);
    }
    check_ddc(addr, ctx, /* overwrite addr */ (TCGv)addr, memop_size(mop));
}

static inline bool have_cheri_tb_flags(DisasContext *ctx, uint32_t flags)
{
    return (ctx->base.cheri_flags & flags) == flags;
}

static inline void _generate_ddc_checked_ptr(
    DisasContext *ctx, CheriTbFlags tb_perm_flags, CheriPermissions req_perms,
    TCGv_cap_checked_ptr checked_addr, TCGv ddc_offset, target_ulong num_bytes)
{
    if (unlikely(!have_cheri_tb_flags(ctx, tb_perm_flags))) {
        // DDC is untagged, sealed, or missing PERM_STORE
        TCGv_i32 tperms = tcg_const_i32(req_perms);
        cheri_tcg_prepare_for_unconditional_exception(&ctx->base);
        gen_helper_raise_exception_ddc_perms(cpu_env, tperms);
        tcg_temp_free_i32(tperms);
        return;
    }
    const bool need_interposition =
        !have_cheri_tb_flags(ctx, TB_FLAG_CHERI_DDC_NO_INTERPOSE);
    // We need DDC interposition since the base/cursor is not zero
    if (unlikely(need_interposition)) {
        tcg_gen_add_tl((TCGv)checked_addr, ddc_offset, ddc_interposition);
    } else {
        tcg_gen_mov_tl((TCGv)checked_addr, ddc_offset);
    }
    if (unlikely(!have_cheri_tb_flags(ctx, TB_FLAG_CHERI_DDC_FULL_AS))) {
        // We need a bounds check since DDC is not full address space
        TCGv tbytes = tcg_const_tl(num_bytes);
        gen_helper_ddc_check_bounds(cpu_env, (TCGv)checked_addr, tbytes);
        tcg_temp_free(tbytes);
    }
    // DDC has been checked now and checked_addr can be used directly.
}
static inline void
generate_ddc_checked_load_ptr(TCGv_cap_checked_ptr checked_addr,
                              DisasContext *ctx, TCGv ddc_offset,
                              target_ulong num_bytes)
{
    _generate_ddc_checked_ptr(ctx, TB_FLAG_CHERI_DDC_READABLE, CAP_PERM_LOAD,
                              checked_addr, ddc_offset, num_bytes);
}
static inline void
generate_ddc_checked_store_ptr(TCGv_cap_checked_ptr checked_addr,
                               DisasContext *ctx, TCGv ddc_offset,
                               target_ulong num_bytes)
{
    _generate_ddc_checked_ptr(ctx, TB_FLAG_CHERI_DDC_WRITABLE, CAP_PERM_STORE,
                              checked_addr, ddc_offset, num_bytes);
}
static inline void
generate_ddc_checked_rmw_ptr(TCGv_cap_checked_ptr checked_addr,
                             DisasContext *ctx, TCGv ddc_offset,
                             target_ulong num_bytes)
{
    _generate_ddc_checked_ptr(
        ctx, TB_FLAG_CHERI_DDC_READABLE | TB_FLAG_CHERI_DDC_WRITABLE,
        CAP_PERM_LOAD | CAP_PERM_STORE, checked_addr, ddc_offset, num_bytes);
}
#else // !TARGET_CHERI
#define generate_ddc_checked_load_ptr(checked_addr, ctx, offset, num_bytes)    \
    tcg_gen_mov_tl(checked_addr, offset)
#define generate_ddc_checked_store_ptr(checked_addr, ctx, offset, num_bytes)   \
    tcg_gen_mov_tl(checked_addr, offset)
#define generate_ddc_checked_rmw_ptr(checked_addr, ctx, offset, num_bytes)     \
    tcg_gen_mov_tl(checked_addr, offset)
#endif // TARGET_CHERI

static inline void gen_ddc_interposed_ld_i64(DisasContext *ctx, TCGv_i64 result,
                                             TCGv_cap_checked_ptr checked_addr,
                                             TCGv ddc_offset, TCGArg arg,
                                             MemOp op)
{
    if (checked_addr == NULL) {
        checked_addr = (TCGv_cap_checked_ptr)ddc_offset;
    }
    generate_ddc_checked_load_ptr(checked_addr, ctx, ddc_offset,
                                  memop_size(op));
    tcg_gen_qemu_ld_i64_with_checked_addr(result, checked_addr, arg, op);
}

static inline void gen_ddc_interposed_ld_i32(DisasContext *ctx, TCGv_i32 result,
                                             TCGv_cap_checked_ptr checked_addr,
                                             TCGv ddc_offset, TCGArg arg,
                                             MemOp op)
{
    if (checked_addr == NULL) {
        checked_addr = (TCGv_cap_checked_ptr)ddc_offset;
    }
    generate_ddc_checked_load_ptr(checked_addr, ctx, ddc_offset,
                                  memop_size(op));
    tcg_gen_qemu_ld_i32_with_checked_addr(result, checked_addr, arg, op);
}

static inline void gen_ddc_interposed_st_i64(DisasContext *ctx, TCGv_i64 value,
                                             TCGv_cap_checked_ptr checked_addr,
                                             TCGv ddc_offset, TCGArg arg,
                                             MemOp op)
{
    if (checked_addr == NULL) {
        checked_addr = (TCGv_cap_checked_ptr)ddc_offset;
    }
    generate_ddc_checked_store_ptr(checked_addr, ctx, ddc_offset,
                                   memop_size(op));
    tcg_gen_qemu_st_i64_with_checked_addr(value, checked_addr, arg, op);
}
static inline void gen_ddc_interposed_st_i32(DisasContext *ctx, TCGv_i32 value,
                                             TCGv_cap_checked_ptr checked_addr,
                                             TCGv ddc_offset, TCGArg arg,
                                             MemOp op)
{
    if (checked_addr == NULL) {
        checked_addr = (TCGv_cap_checked_ptr)ddc_offset;
    }
    generate_ddc_checked_store_ptr(checked_addr, ctx, ddc_offset,
                                   memop_size(op));
    tcg_gen_qemu_st_i32_with_checked_addr(value, checked_addr, arg, op);
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
    if (have_cheri_tb_flags(ctx, TB_FLAG_PCC_FULL_AS)) {
        return; // PCC spans the full address space, no need to check
    }

    // Note: PC can only be incremented since a branch exits the TB, so checking
    // for pc_next < pcc.base should not be needed. Add a debug assertion in
    // case this assumption no longer holds in the future.
    // Note: we don't have to check for wraparound here since this case is
    // already handled by the TB_FLAG_PCC_FULL_AS check above. Wraparound is
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
        gen_raise_pcc_violation(&ctx->base, addr, 0);
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
    if (likely(have_cheri_tb_flags(ctx, TB_FLAG_PCC_FULL_AS))) {
        return; // PCC spans the full address space, no need to check
    }

    TCGLabel *skip_btarget_check = gen_new_label();
    TCGLabel *bounds_violation = gen_new_label();
    // We can skip the check of pcc.base if it is zero (common case in
    // hybrid/non-CHERI  mode).
    if (unlikely(ctx->base.pcc_base > 0)) {
        tcg_gen_brcondi_tl(TCG_COND_LTU, addr, ctx->base.pcc_base,
                           bounds_violation);
    }
    if (ctx->base.pcc_top != TYPE_MAXIMUM(target_ulong)) {
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
