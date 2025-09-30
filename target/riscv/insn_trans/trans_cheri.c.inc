/*
 * RISC-V translation routines for the CHERI Extension.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2020 Alex Richardson <Alexander.Richardson@cl.cam.ac.uk>
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

#define DO_TRANSLATE_HELPER(name, gen_helper, helper_name, ...)                \
    static bool trans_##name(DisasContext *ctx, arg_##name *a)                 \
    {                                                                          \
        return gen_helper(ctx, __VA_ARGS__, &gen_helper_##helper_name);        \
    }

#define DO_TRANSLATE(name, gen_helper, ...)                                    \
    DO_TRANSLATE_HELPER(name, gen_helper, name, __VA_ARGS__)

typedef void(cheri_int_cap_helper)(TCGv, TCGv_env, TCGv_i32);
static inline bool gen_cheri_int_cap(DisasContext *ctx, arg_rc *a,
                                     cheri_int_cap_helper *gen_func)
{
    TCGv result = tcg_temp_new();
    gen_func(result, cpu_env, tcg_constant_i32(a->cs1));
    gen_set_gpr(ctx, a->rd, result);
    tcg_temp_free(result);
    return true;
}

#define TRANSLATE_INT_CAP_HELPER(name, helper)                                 \
    DO_TRANSLATE_HELPER(name, gen_cheri_int_cap, helper, a)
#define TRANSLATE_INT_CAP(name) TRANSLATE_INT_CAP_HELPER(name, name)

typedef void(cheri_cap_cap_helper)(TCGv_env, TCGv_i32, TCGv_i32);
static inline bool gen_cheri_cap_cap(DisasContext *ctx G_GNUC_UNUSED, arg_cc *a,
                                     cheri_cap_cap_helper *gen_func)
{
    gen_func(cpu_env, tcg_constant_i32(a->cd), tcg_constant_i32(a->cs1));
    return true;
}
#define TRANSLATE_CAP_CAP_HELPER(name, helper)                                 \
    DO_TRANSLATE_HELPER(name, gen_cheri_cap_cap, helper, a)
#define TRANSLATE_CAP_CAP(name) TRANSLATE_CAP_CAP_HELPER(name, name)

typedef void(cheri_cap_int_helper)(TCGv_env, TCGv_i32, TCGv);
static inline QEMU_ALWAYS_INLINE bool
gen_cheri_cap_int_plus_imm(DisasContext *ctx, int cd, int rs, target_long imm,
                           cheri_cap_int_helper *gen_func)
{
    TCGv gpr_value = tcg_temp_new();
    gen_get_gpr(ctx, gpr_value, rs);
    tcg_gen_addi_tl(gpr_value, gpr_value, imm);
    gen_func(cpu_env, tcg_constant_i32(cd), gpr_value);
    tcg_temp_free(gpr_value);
    return true;
}

typedef void(cheri_int_int_helper)(TCGv, TCGv_env, TCGv);
static inline bool gen_cheri_int_int(DisasContext *ctx, arg_r2 *a,
                                     cheri_int_int_helper *gen_func)
{
    TCGv result = tcg_temp_new();
    TCGv gpr_src_value = tcg_temp_new();
    gen_get_gpr(ctx, gpr_src_value, a->rs1);
    gen_func(result, cpu_env, gpr_src_value);
    gen_set_gpr(ctx, a->rd, result);
    tcg_temp_free(gpr_src_value);
    tcg_temp_free(result);
    return true;
}

typedef void(cheri_cap_cap_cap_helper)(TCGv_env, TCGv_i32, TCGv_i32, TCGv_i32);
static inline bool gen_cheri_cap_cap_cap(DisasContext *ctx G_GNUC_UNUSED,
                                         arg_ccc *arg,
                                         cheri_cap_cap_cap_helper *gen_func)
{
    gen_func(cpu_env, tcg_constant_i32(arg->cd), tcg_constant_i32(arg->cs1),
             tcg_constant_i32(arg->cs2));
    return true;
}
// We assume that all these instructions can trap (e.g. seal violation)
#define TRANSLATE_CAP_CAP_CAP_HELPER(name, helper)                             \
    DO_TRANSLATE_HELPER(name, gen_cheri_cap_cap_cap, helper, a)
#define TRANSLATE_CAP_CAP_CAP(name) TRANSLATE_CAP_CAP_CAP_HELPER(name, name)

typedef void(cheri_cap_cap_int_helper)(TCGv_env, TCGv_i32, TCGv_i32, TCGv);
static inline bool gen_cheri_cap_cap_int(DisasContext *ctx, arg_ccr *a,
                                         cheri_cap_cap_int_helper *gen_func)
{
    TCGv gpr_value = tcg_temp_new();
    gen_get_gpr(ctx, gpr_value, a->rs2);
    gen_func(cpu_env, tcg_constant_i32(a->cd), tcg_constant_i32(a->cs1),
             gpr_value);
    tcg_temp_free(gpr_value);
    return true;
}
#define TRANSLATE_CAP_CAP_INT_HELPER(name, helper)                             \
    DO_TRANSLATE_HELPER(name, gen_cheri_cap_cap_int, helper, a)
#define TRANSLATE_CAP_CAP_INT(name) TRANSLATE_CAP_CAP_INT_HELPER(name, name)

typedef void(cheri_cap_loadstore_helper)(TCGv_env, TCGv_i32, TCGv, TCGv_i32);
static inline bool gen_cheri_cap_loadstore(DisasContext *ctx, int srcdst,
                                           int auth, target_long imm,
                                           cheri_cap_loadstore_helper *gen_func)
{
    TCGv addr = tcg_temp_new();
    gen_cap_get_cursor(ctx, auth, addr);
    if (imm != 0) {
        tcg_gen_addi_tl(addr, addr, imm);
    }
    gen_func(cpu_env, tcg_constant_i32(srcdst), addr, tcg_constant_i32(auth));
    tcg_temp_free(addr);
    return true;
}

typedef void(cheri_int_cap_cap_helper)(TCGv, TCGv_env, TCGv_i32, TCGv_i32);
static inline bool gen_cheri_int_cap_cap(DisasContext *ctx, arg_rcc *a,
                                         cheri_int_cap_cap_helper *gen_func)
{
    TCGv result = tcg_temp_new();
    gen_func(result, cpu_env, tcg_constant_i32(a->cs1),
             tcg_constant_i32(a->cs2));
    gen_set_gpr(ctx, a->rd, result);
    tcg_temp_free(result);
    return true;
}
#define TRANSLATE_INT_CAP_CAP_HELPER(name, helper)                             \
    DO_TRANSLATE_HELPER(name, gen_cheri_int_cap_cap, helper, a)
#define TRANSLATE_INT_CAP_CAP(name) TRANSLATE_INT_CAP_CAP_HELPER(name, name)

typedef void(cheri_cap_cap_imm_helper)(TCGv_env, TCGv_i32, TCGv_i32, TCGv);
static inline bool gen_cheri_cap_cap_imm(int cd, int cs1, target_long imm,
                                         cheri_cap_cap_imm_helper *gen_func)
{
    gen_func(cpu_env, tcg_constant_i32(cd), tcg_constant_i32(cs1),
             tcg_constant_tl(imm));
    return true;
}

#define TRANSLATE_CAP_CAP_IMM(name)                                            \
    TRANSLATE_MAYBE_TRAP(name, gen_cheri_cap_cap_imm, a->rd, a->rs1, a->imm)

/// Control-flow instructions
static void gen_cjal(DisasContext *ctx, int rd, target_ulong imm)
{
    /* Mask the LSB to match the RISC-V spec for JAL. */
    target_ulong next_pc = (ctx->base.pc_next + imm) & ~(target_ulong)1;

    TCGv target_addr = tcg_constant_tl(next_pc);
    TCGv link_addr = tcg_constant_tl(ctx->pc_succ_insn);
    /*
     * helper_cjal() checks for misalignment and capability bounds. It also
     * ensures that the written value is a sentry and marks PC as up-to-date.
     */
    gen_helper_cjal(cpu_env, tcg_constant_i32(rd), target_addr, link_addr);

    /* No bounds check needed here since we did it in helper_cjal(). */
    gen_goto_tb(ctx, 0, next_pc, false);
    ctx->base.is_jmp = DISAS_NORETURN;
}

static void gen_cjalr(DisasContext *ctx, int cd, int cs1, target_ulong imm)
{
    TCGv t0 = tcg_constant_tl(ctx->pc_succ_insn); /* Link addr + resulting pc */
    gen_helper_cjalr(cpu_env, tcg_constant_i32(cd), tcg_constant_i32(cs1),
                     tcg_constant_tl(imm), t0);

    tcg_gen_lookup_and_goto_ptr();
    // PC has been updated -> exit translation block
    ctx->base.is_jmp = DISAS_NORETURN;
}

static bool gen_cmv(DisasContext *ctx, arg_cc *a)
{
    gen_move_cap_gp_gp(ctx, a->cd, a->cs1);
    gen_reg_modified_cap(ctx, a->cd);
    return true;
}

#ifdef TARGET_CHERI_RISCV_V9
// TODO: all of these could be implemented in TCG without calling a helper
// Two operand (int cap)
TRANSLATE_INT_CAP(cgetaddr)
TRANSLATE_INT_CAP(cgetbase)
TRANSLATE_INT_CAP(cgetflags)
TRANSLATE_INT_CAP(cgethigh)
TRANSLATE_INT_CAP(cgetlen)
TRANSLATE_INT_CAP(cgetoffset)
TRANSLATE_INT_CAP(cgetperm)
TRANSLATE_INT_CAP(cgettag)
TRANSLATE_INT_CAP(cgettype)
TRANSLATE_INT_CAP(cgetsealed)

TRANSLATE_INT_CAP(cloadtags)

// Two operand (int int)
static inline bool trans_crrl(DisasContext *ctx, arg_crrl *a)
{
    return gen_cheri_int_int(ctx, a, &gen_helper_crap);
}

// Two operand (cap cap)
TRANSLATE_CAP_CAP(ccleartag)
TRANSLATE_CAP_CAP(csealentry)

// Three operand (cap cap cap)
TRANSLATE_CAP_CAP_CAP(cbuildcap)
TRANSLATE_CAP_CAP_CAP(ccopytype)
TRANSLATE_CAP_CAP_CAP(ccseal)
TRANSLATE_CAP_CAP_CAP(cseal)
TRANSLATE_CAP_CAP_CAP(cunseal)

// Not quite (cap cap cap) but the index argument can be handled the same way
static bool trans_cspecialrw(DisasContext *ctx, arg_cspecialrw *a)
{
    if (gen_cheri_cap_cap_cap(ctx, a, &gen_helper_cspecialrw)) {
        if (a->cs1 != 0 && a->cs2 == CheriSCR_DDC) {
            // When DDC changes we have to exit the current translation block
            // since we cache DDC properties in the flags to optimize out
            // bounds/permission checks.
            gen_update_cpu_pc(ctx->pc_succ_insn);
            tcg_gen_exit_tb(NULL, 0);
            ctx->base.is_jmp = DISAS_NORETURN;
        }
        return true;
    }
    return false;
}

// Three operand (cap cap int)
TRANSLATE_CAP_CAP_INT(candperm)
TRANSLATE_CAP_CAP_INT(cfromptr)
TRANSLATE_CAP_CAP_INT(cincoffset)
TRANSLATE_CAP_CAP_INT(csetaddr)
TRANSLATE_CAP_CAP_INT(csetbounds)
TRANSLATE_CAP_CAP_INT(csetboundsexact)
TRANSLATE_CAP_CAP_INT(csetflags)
TRANSLATE_CAP_CAP_INT(csethigh)
TRANSLATE_CAP_CAP_INT(csetoffset)

// Three operand (int cap cap)
TRANSLATE_INT_CAP_CAP(csub)
TRANSLATE_INT_CAP_CAP(ctestsubset)
TRANSLATE_INT_CAP_CAP(cseqx)
TRANSLATE_INT_CAP_CAP(ctoptr)

static bool trans_csetboundsimm(DisasContext *ctx, arg_csetboundsimm *a)
{
    tcg_debug_assert(a->imm >= 0);
    return gen_cheri_cap_cap_imm(a->cd, a->cs1, a->imm, &gen_helper_csetbounds);
}

static bool trans_jalr_cap(DisasContext *ctx, arg_jalr_cap *a)
{
    gen_cjalr(ctx, a->cd, a->cs1, 0);
    return true;
}

static bool trans_jalr_pcc(DisasContext *ctx, arg_jalr_pcc *a)
{
    gen_jalr(ctx, a->rd, a->rs1, 0);
    return true;
}

static inline bool trans_cinvoke(DisasContext *ctx, arg_cinvoke *a)
{
    gen_helper_cinvoke(cpu_env, tcg_constant_i32(a->rs1),
                       tcg_constant_i32(a->rs2));

    tcg_gen_lookup_and_goto_ptr();
    // PC has been updated -> exit translation block
    ctx->base.is_jmp = DISAS_NORETURN;
    return true;
}
#endif

static inline bool gen_cap_load_mem_idx(DisasContext *ctx, int32_t rd,
                                        int32_t cs, target_long offset,
                                        int mem_idx, MemOp op)
{
    // FIXME: just do everything in the helper
    TCGv value = tcg_temp_new();
    TCGv_cap_checked_ptr vaddr = tcg_temp_new_cap_checked();
    generate_cap_load_check_imm(vaddr, cs, offset, op);
    tcg_gen_qemu_ld_tl_with_checked_addr(value, vaddr, mem_idx, op);
    gen_set_gpr(ctx, rd, value);
    tcg_temp_free_cap_checked(vaddr);
    tcg_temp_free(value);
    return true;
}

static inline bool gen_cap_load(DisasContext *ctx, int32_t rd, int32_t cs,
                                target_long offset, MemOp op)
{
    return gen_cap_load_mem_idx(ctx, rd, cs, offset, ctx->mem_idx, op);
}

#ifdef TARGET_CHERI_RISCV_V9
static bool gen_ddc_load(DisasContext *ctx, int rd, int rs1, MemOp memop)
{
    TCGv addr = tcg_temp_new();
    TCGv value = tcg_temp_new();
    gen_get_gpr(ctx, addr, rs1);
    gen_ddc_interposed_ld_tl(ctx, value, /* Update addr in-place */ NULL, addr,
                             ctx->mem_idx, memop);
    gen_set_gpr(ctx, rd, value);
    tcg_temp_free(addr);
    tcg_temp_free(value);
    return true;
}
#define TRANSLATE_EXPLICIT_LOAD(name, op)                                      \
    static bool trans_##name##_ddc(DisasContext *ctx, arg_##name##_ddc *a)     \
    {                                                                          \
        return gen_ddc_load(ctx, a->rd, a->rs1, op);                           \
    }                                                                          \
    static bool trans_##name##_cap(DisasContext *ctx, arg_##name##_cap *a)     \
    {                                                                          \
        return gen_cap_load(ctx, a->rd, a->rs1, /*offset=*/0, op);             \
    }

TRANSLATE_EXPLICIT_LOAD(ld_b, MO_SB)
TRANSLATE_EXPLICIT_LOAD(ld_h, MO_SW)
TRANSLATE_EXPLICIT_LOAD(ld_w, MO_SL)
#ifdef TARGET_RISCV64
TRANSLATE_EXPLICIT_LOAD(ld_d, MO_Q)
#endif
TRANSLATE_EXPLICIT_LOAD(ld_bu, MO_UB)
TRANSLATE_EXPLICIT_LOAD(ld_hu, MO_UW)
#ifdef TARGET_RISCV64
TRANSLATE_EXPLICIT_LOAD(ld_wu, MO_UL)
#endif

static inline bool trans_ld_c_ddc(DisasContext *ctx, arg_ld_c_ddc *a)
{
    // always uses DDC as the base register
    return gen_cheri_cap_int_plus_imm(ctx, a->rd, a->rs1, 0,
                                      &gen_helper_load_cap_via_ddc);
}

static inline bool trans_ld_c_cap(DisasContext *ctx, arg_ld_c_cap *a)
{
    // No immediate available for lccap
    return gen_cheri_cap_loadstore(ctx, a->rd, a->rs1, 0,
                                   &gen_helper_load_cap_via_cap);
}
#endif /* TARGET_CHERI_RISCV_V9 */

static inline bool trans_lc(DisasContext *ctx, arg_lc *a)
{
    if (!ctx->capmode) {
        // Without capmode we load relative to DDC (lc instructions)
        return gen_cheri_cap_int_plus_imm(ctx, a->rd, a->rs1, a->imm,
                                          &gen_helper_load_cap_via_ddc);
    }
    return gen_cheri_cap_loadstore(ctx, a->rd, a->rs1, /*offset=*/a->imm,
                                   &gen_helper_load_cap_via_cap);
}

/* Load Via Capability Register */
static inline bool gen_cap_store_mem_idx(DisasContext *ctx, int32_t addr_regnum,
                                         int32_t val_regnum, target_long offset,
                                         int mem_idx, MemOp op)
{
    // FIXME: just do everything in the helper
    TCGv_cap_checked_ptr vaddr = tcg_temp_new_cap_checked();
    generate_cap_store_check_imm(vaddr, addr_regnum, offset, op);

    TCGv value = tcg_temp_new();
    gen_get_gpr(ctx, value, val_regnum);
    tcg_gen_qemu_st_tl_with_checked_addr(value, vaddr, mem_idx, op);
    tcg_temp_free(value);
    tcg_temp_free_cap_checked(vaddr);
    return true;
}

static inline bool gen_cap_store(DisasContext *ctx, int32_t addr_regnum,
                                 int32_t val_regnum, target_long offset,
                                 MemOp op)
{
    gen_cap_store_mem_idx(ctx, addr_regnum, val_regnum, offset, ctx->mem_idx,
                          op);
    return true;
}

#ifdef TARGET_CHERI_RISCV_V9
static bool gen_ddc_store(DisasContext *ctx, int rs1, int rs2, MemOp memop)
{
    TCGv addr = tcg_temp_new();
    TCGv value = tcg_temp_new();
    gen_get_gpr(ctx, addr, rs1);
    gen_get_gpr(ctx, value, rs2);
    gen_ddc_interposed_st_tl(ctx, value, /* Update addr in-place */ NULL, addr,
                             ctx->mem_idx, memop);
    tcg_temp_free(value);
    tcg_temp_free(addr);
    return true;
}

#define TRANSLATE_EXPLICIT_STORE(name, op)                                     \
    static bool trans_##name##_ddc(DisasContext *ctx, arg_##name##_ddc *a)     \
    {                                                                          \
        return gen_ddc_store(ctx, a->rs1, a->rs2, op);                         \
    }                                                                          \
    static bool trans_##name##_cap(DisasContext *ctx, arg_##name##_cap *a)     \
    {                                                                          \
        return gen_cap_store(ctx, a->rs1, a->rs2, /*offset=*/0, op);           \
    }

TRANSLATE_EXPLICIT_STORE(st_b, MO_UB)
TRANSLATE_EXPLICIT_STORE(st_h, MO_UW)
TRANSLATE_EXPLICIT_STORE(st_w, MO_UL)
#ifdef TARGET_RISCV64
TRANSLATE_EXPLICIT_STORE(st_d, MO_Q)
#endif

// RS2 is the value, RS1 is the capability/ddc offset
static inline bool trans_st_c_ddc(DisasContext *ctx, arg_st_c_ddc *a)
{
    // always uses DDC as the base register
    return gen_cheri_cap_int_plus_imm(ctx, a->rs2, a->rs1, 0,
                                      &gen_helper_store_cap_via_ddc);
}

static inline bool trans_st_c_cap(DisasContext *ctx, arg_st_c_cap *a)
{
    // No immediate available for sc.cap
    return gen_cheri_cap_loadstore(ctx, a->rs2, a->rs1, /*offset=*/0,
                                   &gen_helper_store_cap_via_cap);
}
#endif /* TARGET_CHERI_RISCV_V9 */

static inline bool trans_sc(DisasContext *ctx, arg_sc *a)
{
    // RS2 is the value, RS1 is the capability
    if (!ctx->capmode) {
        // Without capmode we store relative to DDC (sc instructions)
        return gen_cheri_cap_int_plus_imm(ctx, a->rs2, a->rs1, a->imm,
                                          &gen_helper_store_cap_via_ddc);
    }
    return gen_cheri_cap_loadstore(ctx, a->rs2, a->rs1, /*offset=*/a->imm,
                                   &gen_helper_store_cap_via_cap);
}

// Atomic ops
static inline bool trans_lr_c_impl(DisasContext *ctx, arg_atomic *a,
                                   cheri_cap_cap_helper *helper)
{
    REQUIRE_EXT(ctx, RVA);
    if (tb_cflags(ctx->base.tb) & CF_PARALLEL) {
        // In a parallel context, stop the world and single step.
        gen_helper_exit_atomic(cpu_env);
        ctx->base.is_jmp = DISAS_NORETURN;
    } else {
        // Note: we ignore the Acquire/release flags since using
        // helper_exit_atomic forces exlusive execution so we get SC semantics.
        tcg_debug_assert(a->rs2 == 0);
        arg_cc cc = { .cd = a->rd, .cs1 = a->rs1 };
        gen_cheri_cap_cap(ctx, &cc, helper);
    }
    return true;
}

static inline bool trans_lr_c(DisasContext *ctx, arg_lr_c *a)
{
    // Note: The capmode dependent address interpretation happens in the
    // helper and not during translation.
    return trans_lr_c_impl(ctx, a, &gen_helper_lr_c_modedep);
}

#ifdef TARGET_CHERI_RISCV_V9
static inline bool trans_lr_c_ddc(DisasContext *ctx, arg_lr_c_ddc *a)
{
    return trans_lr_c_impl(ctx, a, &gen_helper_lr_c_ddc);
}

static inline bool trans_lr_c_cap(DisasContext *ctx, arg_lr_c_cap *a)
{
    return trans_lr_c_impl(ctx, a, &gen_helper_lr_c_cap);
}
#endif /* TARGET_CHERI_RISCV_V9 */

static inline bool trans_sc_c_impl(DisasContext *ctx, arg_atomic *a,
                                   cheri_int_cap_cap_helper *helper)
{
    REQUIRE_EXT(ctx, RVA);
    if (tb_cflags(ctx->base.tb) & CF_PARALLEL) {
        // In a parallel context, stop the world and single step.
        gen_helper_exit_atomic(cpu_env);
        ctx->base.is_jmp = DISAS_NORETURN;
    } else {
        // Note: we ignore the Acquire/release flags since using
        // helper_exit_atomic forces exclusive execution so we get SC semantics.
        arg_rcc rcc = { .rd = a->rd, .cs1 = a->rs1, .cs2 = a->rs2 };
        gen_cheri_int_cap_cap(ctx, &rcc, helper);
    }
    return true;
}

static inline bool trans_sc_c(DisasContext *ctx, arg_sc_c *a)
{
    // Note: The capmode dependent address interpretation happens in the
    // helper and not during translation.
    return trans_sc_c_impl(ctx, a, &gen_helper_sc_c_modedep);
}

#ifdef TARGET_CHERI_RISCV_V9
static inline bool trans_sc_c_ddc(DisasContext *ctx, arg_sc_c_ddc *a)
{
    a->rd = a->rs2; /* Not enough encoding space for explicit rd */
    return trans_sc_c_impl(ctx, a, &gen_helper_sc_c_ddc);
}

static inline bool trans_sc_c_cap(DisasContext *ctx, arg_sc_c_cap *a)
{
    a->rd = a->rs2; /* Not enough encoding space for explicit rd */
    return trans_sc_c_impl(ctx, a, &gen_helper_sc_c_cap);
}
#endif /* TARGET_CHERI_RISCV_V9 */

static inline bool trans_amoswap_c(DisasContext *ctx, arg_amoswap_c *a)
{
    REQUIRE_EXT(ctx, RVA);
    if (tb_cflags(ctx->base.tb) & CF_PARALLEL) {
        // In a parallel context, stop the world and single step.
        gen_helper_exit_atomic(cpu_env);
        ctx->base.is_jmp = DISAS_NORETURN;
    } else {
        // Note: we ignore the Acquire/release flags since using
        // helper_exit_atomic forces exlusive execution so we get SC semantics.
        arg_ccc ccc = { .cd = a->rd, .cs1 = a->rs1, .cs2 = a->rs2 };
        gen_cheri_cap_cap_cap(ctx, &ccc, &gen_helper_amoswap_cap);
    }
    return true;
}

static inline bool do_trans_modesw(DisasContext *ctx, bool to_capmode)
{
    if (ctx->capmode == to_capmode) {
        qemu_log_mask_and_addr(CPU_LOG_INSTR, ctx->base.pc_first,
                               "Redundant modesw at " TARGET_FMT_lx " (%s)",
                               ctx->base.pc_first,
                               lookup_symbol(ctx->base.pc_first));
        return true;
    }
    gen_helper_modesw(cpu_env, tcg_constant_i32(to_capmode));

    /*
     * The current execution mode is checked at translation time (not at
     * execution time) and is part of the translation block flags, so these
     * flags are no longer correct after executing modesw -> we have to process
     * the mode update and exit the translation block.
     *
     * Note: Translating the same code with a different initial mode will never
     * find a cached block with incorrect initial mode since the mode is part of
     * the TB flags and they are used as part of the hash table lookup.
     *
     * TODO: it should be possible to update the flag in DisasContext and
     * continue translation after the modesw, but I am not confident this will
     * be correct. Once we have some unit tests that we can run we should try
     * to make this change since it will improve the performance of hybrid code.
     * See Morello code which uses DISAS_UPDATE_EXIT.
     */
    /* create tcg instruction to exit the tb */
    gen_update_cpu_pc(ctx->pc_succ_insn);
    tcg_gen_exit_tb(NULL, 0);
    /* This indicates to riscv_tr_tb_stop that no cleanup is needed. */
    ctx->base.is_jmp = DISAS_NORETURN;
    return true;
}

static inline bool trans_modesw_cap(DisasContext *ctx, arg_modesw_cap *a)
{
    return do_trans_modesw(ctx, /*to_capmode=*/true);
}

static inline bool trans_modesw_int(DisasContext *ctx, arg_modesw_int *a)
{
    return do_trans_modesw(ctx, /*to_capmode=*/false);
}

#ifdef TARGET_CHERI_RISCV_V9
// Explicit CAP/DDC atomic ops (no unsigned versions):
// Reuses gen_lr_impl, defined in trans_rva.c.inc
static inline bool gen_lr_impl(DisasContext *ctx, TCGv_cap_checked_ptr addr,
                               arg_atomic *a, MemOp mop);

#define TRANSLATE_LR_EXPLICIT(name, op)                                        \
    static bool trans_##name##_ddc(DisasContext *ctx, arg_##name##_ddc *a)     \
    {                                                                          \
        REQUIRE_EXT(ctx, RVA);                                                 \
        TCGv_cap_checked_ptr addr = tcg_temp_new_cap_checked();                \
        generate_get_ddc_checked_gpr_plus_offset(                              \
            addr, ctx, a->rs1, 0, op, &generate_ddc_checked_load_ptr);         \
        bool result = gen_lr_impl(ctx, addr, a, op);                           \
        tcg_temp_free_cap_checked(addr);                                       \
        return result;                                                         \
    }                                                                          \
    static bool trans_##name##_cap(DisasContext *ctx, arg_##name##_cap *a)     \
    {                                                                          \
        REQUIRE_EXT(ctx, RVA);                                                 \
        TCGv_cap_checked_ptr addr = tcg_temp_new_cap_checked();                \
        generate_cap_load_check_imm(addr, a->rs1, 0, op);                      \
        bool result = gen_lr_impl(ctx, addr, a, op);                           \
        tcg_temp_free_cap_checked(addr);                                       \
        return result;                                                         \
    }
TRANSLATE_LR_EXPLICIT(lr_b, MO_ALIGN | MO_SB);
TRANSLATE_LR_EXPLICIT(lr_h, MO_ALIGN | MO_SW);
TRANSLATE_LR_EXPLICIT(lr_w, MO_ALIGN | MO_SL);
#ifdef TARGET_RISCV64
TRANSLATE_LR_EXPLICIT(lr_d, MO_ALIGN | MO_Q);
#endif

static inline bool gen_sc_impl(DisasContext *ctx, TCGv_cap_checked_ptr addr,
                               arg_atomic *a, MemOp mop);

#define TRANSLATE_SC_EXPLICIT(name, op)                                        \
    static bool trans_##name##_ddc(DisasContext *ctx, arg_##name##_ddc *a)     \
    {                                                                          \
        REQUIRE_EXT(ctx, RVA);                                                 \
        TCGv_cap_checked_ptr addr = tcg_temp_new_cap_checked();                \
        generate_get_ddc_checked_gpr_plus_offset(                              \
            addr, ctx, a->rs1, 0, op, &generate_ddc_checked_load_ptr);         \
        a->rd = a->rs2; /* Not enough encoding space for explicit rd */        \
        bool result = gen_sc_impl(ctx, addr, a, op);                           \
        tcg_temp_free_cap_checked(addr);                                       \
        return result;                                                         \
    }                                                                          \
    static bool trans_##name##_cap(DisasContext *ctx, arg_##name##_cap *a)     \
    {                                                                          \
        REQUIRE_EXT(ctx, RVA);                                                 \
        TCGv_cap_checked_ptr addr = tcg_temp_new_cap_checked();                \
        generate_cap_load_check_imm(addr, a->rs1, 0, op);                      \
        a->rd = a->rs2; /* Not enough encoding space for explicit rd */        \
        bool result = gen_sc_impl(ctx, addr, a, op);                           \
        tcg_temp_free_cap_checked(addr);                                       \
        return result;                                                         \
    }
TRANSLATE_SC_EXPLICIT(sc_b, MO_ALIGN | MO_SB);
TRANSLATE_SC_EXPLICIT(sc_h, MO_ALIGN | MO_SW);
TRANSLATE_SC_EXPLICIT(sc_w, MO_ALIGN | MO_SL);
#ifdef TARGET_RISCV64
TRANSLATE_SC_EXPLICIT(sc_d, MO_ALIGN | MO_Q);
#endif
#endif /* TARGET_CHERI_RISCV_V9 */

#ifdef TARGET_CHERI_RISCV_STD
/*
 * Instructions from the RISC-V CHERI standard.
 */

TRANSLATE_INT_CAP_HELPER(gctag, cgettag)
TRANSLATE_INT_CAP_HELPER(gcperm, cgetperm)
TRANSLATE_INT_CAP_HELPER(gctype, cgettype)
TRANSLATE_INT_CAP(gcmode)
TRANSLATE_INT_CAP_HELPER(gchi, cgethigh)
TRANSLATE_INT_CAP_HELPER(gcbase, cgetbase)
TRANSLATE_INT_CAP_HELPER(gclen, cgetlen)

TRANSLATE_CAP_CAP_HELPER(sentry, csealentry)

static bool trans_cadd(DisasContext *ctx, arg_cadd *a)
{
    assert(a->rs2 != 0 && "This encoding is reserved");
    return gen_cheri_cap_cap_int(ctx, a, &gen_helper_cincoffset);
}

TRANSLATE_CAP_CAP_INT_HELPER(acperm, candperm)
TRANSLATE_CAP_CAP_INT_HELPER(scaddr, csetaddr)
TRANSLATE_CAP_CAP_INT_HELPER(scbnds, csetboundsexact)
TRANSLATE_CAP_CAP_INT_HELPER(scbndsr, csetbounds)
TRANSLATE_CAP_CAP_INT_HELPER(schi, csethigh)
TRANSLATE_CAP_CAP_INT(scmode)
TRANSLATE_CAP_CAP_CAP_HELPER(cbld, cbuildcap)

TRANSLATE_INT_CAP_CAP_HELPER(sceq, cseqx)
TRANSLATE_INT_CAP_CAP(scss)

static bool trans_scbndsi(DisasContext *ctx, arg_scbndsi *a)
{
    if (a->scale == 1 && a->imm <= 1) {
        return false; /* Reserved redundant encodings */
    }
    tcg_debug_assert(a->imm >= 0);
    target_ulong imm = a->scale ? a->imm << 4 : a->imm;
    return gen_cheri_cap_cap_imm(a->cd, a->cs1, imm,
                                 &gen_helper_csetboundsexact);
}
#endif

// Common for V9 and standard CHERI:
static inline bool trans_cram(DisasContext *ctx, arg_cram *a)
{
    return gen_cheri_int_int(ctx, a, &gen_helper_cram);
}

static bool trans_caddi(DisasContext *ctx, arg_caddi *a)
{
    return gen_cheri_cap_cap_imm(a->cd, a->cs1, a->imm, &gen_helper_cincoffset);
}

static bool trans_cmv(DisasContext *ctx, arg_cc *a)
{
    return gen_cmv(ctx, a);
}
