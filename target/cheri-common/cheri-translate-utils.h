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

// TODO Move to target specific headers
#ifdef TARGET_MIPS

static inline void gen_load_gpr(TCGv t, int reg);
#define target_get_gpr(ctx, t, reg) gen_load_gpr((TCGv)t, reg)
#define MERGED_FILE 0

#elif defined(TARGET_AARCH64)

TCGv_i64 cpu_reg(DisasContext *s, int reg);
#define target_get_gpr(ctx, t, reg)                                            \
    if (reg == NULL_CAPREG_INDEX)                                              \
        tcg_gen_movi_tl((TCGv)t, 0);                                           \
    else                                                                       \
        tcg_gen_mov_i64((TCGv)t, cpu_reg_sp(ctx, reg))
#define MERGED_FILE 1

#elif defined(TARGET_RISCV)

#define target_get_gpr(ctx, t, reg) gen_get_gpr((TCGv)t, reg)
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
        gen_helper_raise_exception_ddc_perms(cpu_env, tperms);
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
    if (unlikely(!have_cheri_tb_flags(ctx, full_as_flags))) {
        // We need a bounds check since PCC/DDC is not full address space.
        TCGv tbytes = tcg_const_tl(num_bytes);
        if (use_ddc)
            gen_helper_ddc_check_bounds(cpu_env, (TCGv)checked_addr, tbytes);
        else
            gen_helper_pcc_check_bounds(cpu_env, (TCGv)checked_addr, tbytes);
        tcg_temp_free(tbytes);
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
    if (ctx->base.pcc_base > 0) {
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

#if defined(TARGET_CHERI)

// NOTE: I wrote all this for 128, but it may apply to other formats.

// This function belongs in a target specific header
static inline uint32_t gp_register_offset(int reg_num)
{
    return offsetof(CPUArchState, CHERI_GPCAPREGS_MEMBER.decompressed[reg_num]);
}

// Copies some number of bytes between min_size and max_size.
// max_size must be padded to make use of the most optimal vector op.
static inline void gen_vector_copy(uint32_t dest_off, uint32_t source_off,
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
            tcg_gen_ld_vec(tmp, cpu_env, source_off + i);
            tcg_gen_st_vec(tmp, cpu_env, dest_off + i);
        }
        tcg_temp_free_vec(tmp);

    } else {

        TCGv_i64 tmp = tcg_temp_new_i64();
        for (size_t i = 0; i != copy_size; i += copy_chunk) {
            tcg_gen_ld_i64(tmp, cpu_env, source_off + i);
            tcg_gen_st_i64(tmp, cpu_env, dest_off + i);
        }
        tcg_temp_free_i64(tmp);
    }
}

// Generates a capability move of an entire padded cap.
// WARNING: If using lazy cap regs you will need to modify the state of the
// destination to match the source.
static inline void gen_move_cap(uint32_t dest_off, uint32_t source_off)
{
    gen_vector_copy(dest_off, source_off, sizeof(cap_register_t),
                    sizeof(cap_register_t));
}

// Generate a capability move of only the pesbt + cursor of a cap.
// Again, you must also modify lazy state.
static inline void gen_move_compressed_cap(uint32_t dest_off,
                                           uint32_t source_off)
{
    gen_vector_copy(dest_off, source_off, 16, 16);
}

static inline void gen_lazy_cap_get_state(DisasContext *ctx, TCGv_i32 val,
                                          int regnum);

// Decompress if need be and set state to fully decompressed
static inline void gen_lazy_cap_decompress(DisasContext *ctx, int regnum)
{
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

    // Label l1
    gen_set_label(l1);

    tcg_temp_free_i32(reg);
    tcg_temp_free_i32(decompressed);
    tcg_temp_free_i32(val);
}

// TODO: Gen some tracing for all of these

// Set the state of a lazy register to a given constant
static inline void gen_lazy_cap_set_state(DisasContext *ctx, int regnum,
                                          CapRegState state)
{
    TCGv_i32 tcg_state = tcg_const_i32(state);
#ifdef TARGET_MIPS
    g_assert_not_reached(); // Not supported on MIPS
#else
    tcg_gen_st8_i32(tcg_state, cpu_env,
                    offsetof(CPUArchState, gpcapregs.capreg_state[regnum]));
#endif
    tcg_temp_free_i32(tcg_state);
}

static inline void gen_lazy_cap_set_int(DisasContext *ctx, int regnum)
{
    gen_lazy_cap_set_state(ctx, regnum, CREG_INTEGER);
    // Doing this keeps pesbt always up to date, which is good for stores and
    // comparisons
    TCGv_i64 null_pesbt = tcg_const_i64(CC128_NULL_PESBT);
    tcg_gen_st_i64(null_pesbt, cpu_env,
                   gp_register_offset(regnum) +
                       offsetof(cap_register_t, cr_pesbt));
    tcg_temp_free_i64(null_pesbt);
}

static inline void gen_lazy_cap_get_state(DisasContext *ctx, TCGv_i32 val,
                                          int regnum)
{
    tcg_gen_ld8u_i32(
        val, cpu_env,
        offsetof(CPUArchState, CHERI_GPCAPREGS_MEMBER.capreg_state[regnum]));
    // cheri_tcg_printf_verbose("cw", "Register %d lazy state get: %d\n",
    // regnum, val);
}

// Set a lazy GP to the state of a special purpose register in env
static inline void gen_move_cap_gp_sp(DisasContext *ctx, int dest_num,
                                      uint32_t source_off)
{
    if (dest_num == NULL_CAPREG_INDEX)
        return;
    gen_move_cap(gp_register_offset(dest_num), source_off);
    gen_lazy_cap_set_state(ctx, dest_num, CREG_FULLY_DECOMPRESSED);
    // cheri_log_instr_changed_gp_capreg <- will this work?
}

// Set a special purpose register in env to the state of lazy capreg
static inline void gen_move_cap_sp_gp(DisasContext *ctx, uint32_t dest_off,
                                      int source_num)
{
    gen_lazy_cap_decompress(ctx, source_num);
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
    gen_lazy_cap_decompress(ctx, source_num);
    gen_move_cap(gp_register_offset(dest_num), gp_register_offset(source_num));
    gen_lazy_cap_set_state(ctx, dest_num, CREG_FULLY_DECOMPRESSED);
}

// TCG templates to directly modify fields in PESBT. Hopefully we will
// eventually delete most fields and use these. Until then, if a field is
// modified (in decompressed state), then pesbt should be modified too so
// comparisons works.

// TODO: for things in integer state we can just return 0's without loads and
// stores
#define WRAP_PESBT_EXTRACT(name)                                               \
    static inline void gen_cap_pesbt_extract_##name(DisasContext *ctx,         \
                                                    int regnum, TCGv result)   \
    {                                                                          \
        tcg_gen_ld_tl(result, cpu_env,                                         \
                      gp_register_offset(regnum) +                             \
                          offsetof(cap_register_t, cr_pesbt));                 \
        tcg_gen_extract_tl(result, result, CAP_CC(FIELD_##name##_START),       \
                           CAP_CC(FIELD_##name##_SIZE));                       \
    }

#define WRAP_PESBT_X(name, X, clear, invert, op, ...)                          \
    static inline void gen_cap_pesbt_##X##_##name(DisasContext *ctx,           \
                                                  int regnum, TCGv value)      \
    {                                                                          \
        uint32_t offset =                                                      \
            gp_register_offset(regnum) + offsetof(cap_register_t, cr_pesbt);   \
        tcg_gen_shli_tl(value, value, CAP_CC(FIELD_##name##_START));           \
        if (invert)                                                            \
            tcg_gen_not_tl(value, value);                                      \
        TCGv pesbt = tcg_temp_new();                                           \
        tcg_gen_ld_tl(pesbt, cpu_env, offset);                                 \
        if (clear)                                                             \
            tcg_gen_andi_tl(pesbt, pesbt,                                      \
                            ~(target_ulong)(CAP_CC(FIELD_##name##_MASK64)));   \
        tcg_gen_##op##_tl(pesbt, pesbt, __VA_ARGS__);                          \
        tcg_gen_st_tl(pesbt, cpu_env, offset);                                 \
        tcg_temp_free(pesbt);                                                  \
    }

static inline void gen_cap_load_pesbt(DisasContext *ctx, int regnum, TCGv pesbt)
{
    tcg_gen_ld_tl(pesbt, cpu_env,
                  gp_register_offset(regnum) +
                      offsetof(cap_register_t, cr_pesbt));
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

// If sealed untag, otherwise mark unrepsentable if out of bounds
static inline void gen_cap_modified(DisasContext *ctx, int regnum)
{
    assert(0 && "TODO");
}

// Set the tag bit of register to the lowest bit of tagbit
static inline void gen_cap_set_tag(DisasContext *ctx, int regnum, TCGv tagbit)
{
    // TODO if I start using set_tag for basically every capability modification
    // on CHERI then this is probably inneficient Just set the state of the lazy
    // register. This will cause next decompression to set the tag. 0b01 is
    // untagged, 0b10 is tagged. So do 0b01 + tagbit
    TCGv one = tcg_const_tl(1);
    tcg_gen_and_tl(tagbit, tagbit, one);
    _Static_assert(CREG_UNTAGGED_CAP == 1 && CREG_TAGGED_CAP == 2,
                   "Optimised for these values");
    tcg_gen_add_tl(tagbit, tagbit, one);
    tcg_gen_st8_tl(
        tagbit, cpu_env,
        offsetof(CPUArchState, CHERI_GPCAPREGS_MEMBER.capreg_state[regnum]));
    tcg_temp_free(one);
}

static inline void gen_cap_get_tag_i32(DisasContext *ctx, int regnum,
                                       TCGv_i32 tagged)
{
    // State == CREG_TAGGED_CAP || (State == CREG_FULLY_DECOMPRESSED && tag = 1)
    tcg_gen_ld8u_i32(tagged, cpu_env,
                     gp_register_offset(regnum) +
                         offsetof(cap_register_t, cr_tag));

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
    gen_cap_pesbt_extract_OTYPE(ctx, regnum, type);
}

static inline void gen_cap_get_sealed(DisasContext *ctx, int regnum,
                                      TCGv sealed)
{
    gen_cap_get_type(ctx, regnum, sealed);
    TCGv type_unsealed = tcg_const_tl(CAP_OTYPE_UNSEALED);
    tcg_gen_setcond_tl(TCG_COND_NE, sealed, sealed, type_unsealed);
    tcg_temp_free(type_unsealed);
}

static inline void gen_cap_get_sealed_i32(DisasContext *ctx, int regnum,
                                          TCGv_i32 sealed)
{
    TCGv sealedv = tcg_temp_new();
    gen_cap_get_sealed(ctx, regnum, sealedv);
    tcg_gen_trunc_tl_i32(sealed, sealedv);
    tcg_temp_free(sealedv);
}

static inline void gen_cap_get_perms(DisasContext *ctx, int regnum, TCGv perms)
{
    gen_cap_pesbt_extract_HWPERMS(ctx, regnum, perms);
}

static inline void gen_cap_get_hi(DisasContext *ctx, int regnum, TCGv_i64 lim)
{
    size_t offset =
        gp_register_offset(regnum) + offsetof(cap_register_t, cr_pesbt);
    // The pesbt field has been XOR'd with a mask on load
    // This will need to be undone again here.
    // (I think it might be sensible to refactor everything to apply the mask at
    // decompress time)
    tcg_gen_ld_i64(lim, cpu_env, offset);
    tcg_gen_xori_i64(lim, lim, CC128_NULL_PESBT);
}

// FIXME: assumes small endian order between two 64-bit halves in a 128-bit
// integer.
// FIXME: I have no idea GCC actually implements this across platforms. Might be
// MAD/SAD for all I know.
#define CAP_TOP_LOBYTES_OFFSET 0
#define CAP_TOP_HIBYTES_OFFSET 8

// Get top, clamping results to UINT64_T MAX
static inline void gen_cap_get_top(DisasContext *ctx, int regnum, TCGv_i64 top)
{
    gen_lazy_cap_decompress(ctx, regnum);
    size_t offset =
        gp_register_offset(regnum) + offsetof(cap_register_t, _cr_top);
    tcg_gen_ld_i64(top, cpu_env, offset + CAP_TOP_LOBYTES_OFFSET);
    // Set to all ones if top bit of 65-bit top is set.
    TCGv_i64 tmp = tcg_temp_new_i64();
    tcg_gen_ld_i64(tmp, cpu_env, offset + CAP_TOP_HIBYTES_OFFSET);
#ifdef TARGET_AARCH64
    // AARCH could have any others bits in a top with a high bit set
    tcg_gen_neg_i64(tmp, tmp);
    tcg_gen_or_i64(top, top, tmp);
#else
    // Other platforms only top that needs clamping is 1<<64
    tcg_gen_sub_i64(top, top, tmp);
#endif
    tcg_temp_free_i64(tmp);
}

static inline void gen_cap_get_base(DisasContext *ctx, int regnum,
                                    TCGv_i64 base)
{
    gen_lazy_cap_decompress(ctx, regnum);
    size_t offset =
        gp_register_offset(regnum) + offsetof(cap_register_t, cr_base);
    tcg_gen_ld_i64(base, cpu_env, offset);
}

// Get length, where LENGTH_MAX results in UINT64_T MAX
static inline void gen_cap_get_length(DisasContext *ctx, int regnum,
                                      TCGv_i64 length)
{
    gen_lazy_cap_decompress(ctx, regnum);
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
}

static inline void gen_cap_get_cursor(DisasContext *ctx, int regnum,
                                      TCGv_i64 cursor)
{
    // On platforms with merged register file it is better to do the GPR loads
    // as they will likely refer to globals.
#if MERGED_FILE
    target_get_gpr(ctx, cursor, regnum);
#else
    tcg_gen_ld_i64(cursor, cpu_env,
                   gp_register_offset(regnum) +
                       offsetof(cap_register_t, _cr_cursor));
#endif
}

static inline void gen_cap_get_offset(DisasContext *ctx, int regnum,
                                      TCGv_i64 offset)
{
    gen_cap_get_cursor(ctx, regnum, offset);
    TCGv_i64 base = tcg_temp_new_i64();
    gen_cap_get_base(ctx, regnum, base);
    tcg_gen_sub_i64(offset, offset, base);
    tcg_temp_free_i64(base);
}

static inline void gen_cap_get_flags(DisasContext *ctx, int regnum, TCGv flags)
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
    tcg_gen_ld_i64(tmp1, cpu_env,
                   offsetx + offsetof(cap_register_t, _cr_cursor));
    tcg_gen_ld_i64(tmp2, cpu_env,
                   offsety + offsetof(cap_register_t, _cr_cursor));
    tcg_gen_setcond_i64(TCG_COND_EQ, tmp1, tmp1, tmp2);
    tcg_gen_extrl_i64_i32(tmp3, tmp1);
    tcg_gen_and_i32(eq, eq, tmp3);

    // Pesbt equal
    tcg_gen_ld_i64(tmp1, cpu_env, offsetx + offsetof(cap_register_t, cr_pesbt));
    tcg_gen_ld_i64(tmp2, cpu_env, offsety + offsetof(cap_register_t, cr_pesbt));
    tcg_gen_setcond_i64(TCG_COND_EQ, tmp1, tmp1, tmp2);
    tcg_gen_extrl_i64_i32(tmp3, tmp1);
    tcg_gen_and_i32(eq, eq, tmp3);

    tcg_temp_free_i64(tmp1);
    tcg_temp_free_i64(tmp2);
    tcg_temp_free_i32(tmp3);
}
#endif // TARGET_CHERI
