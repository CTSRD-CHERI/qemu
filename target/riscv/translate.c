/*
 * RISC-V emulation for qemu: main translation routines.
 *
 * Copyright (c) 2016-2017 Sagar Karandikar, sagark@eecs.berkeley.edu
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2 or later, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "qemu/osdep.h"
#include "qemu/log.h"
#include "cpu.h"
#include "tcg/tcg-op.h"
#include "disas/disas.h"
#include "exec/cpu_ldst.h"
#include "exec/exec-all.h"
#include "exec/helper-proto.h"
#include "exec/helper-gen.h"

#include "exec/translator.h"
#include "exec/log.h"
#include "exec/log_instr.h"

#include "instmap.h"
#include "internals.h"

/* global register indices */
#ifdef TARGET_CHERI
#include "cheri-lazy-capregs.h"
static TCGv _cpu_cursors_do_not_access_directly[32];
static TCGv cpu_pc;  // Note: this is PCC.cursor
#else
static TCGv cpu_gpr[32], cpu_gprh[32], cpu_pc;
#endif
#ifdef CONFIG_RVFI_DII
TCGv_i32 cpu_rvfi_available_fields;
#endif
static TCGv cpu_vl, cpu_vstart;
static TCGv_i64 cpu_fpr[32]; /* assume F and D extensions */
static TCGv_cap_checked_ptr load_res;
static TCGv load_val;
/* globals for PM CSRs */
static TCGv pm_mask;
static TCGv pm_base;

#include "exec/gen-icount.h"

/*
 * If an operation is being performed on less than TARGET_LONG_BITS,
 * it may require the inputs to be sign- or zero-extended; which will
 * depend on the exact operation being performed.
 */
typedef enum {
    EXT_NONE,
    EXT_SIGN,
    EXT_ZERO,
} DisasExtend;

typedef struct DisasContext {
    DisasContextBase base;
    /* pc_succ_insn points to the instruction following base.pc_next */
    target_ulong pc_succ_insn;
    target_ulong priv_ver;
    RISCVMXL misa_mxl_max;
    RISCVMXL xl;
    uint32_t misa_ext;
    uint32_t opcode;
    uint32_t mstatus_fs;
    uint32_t mstatus_vs;
    uint32_t mstatus_hs_fs;
    uint32_t mstatus_hs_vs;
    uint32_t mem_idx;
    /* Remember the rounding mode encoded in the previous fp instruction,
       which we have already installed into env->fp_status.  Or -1 for
       no previous fp instruction.  Note that we exit the TB when writing
       to any system register, which includes CSR_FRM, so we do not have
       to reset this known value.  */
    int frm;
    RISCVMXL ol;
    bool virt_enabled;
    const RISCVCPUConfig *cfg_ptr;
    bool hlsx;
#ifdef TARGET_CHERI
    bool capmode;
    bool hybrid;
    bool cre;
#ifdef TARGET_CHERI_RISCV_V9
    bool cheri_v9_semantics;
#endif
#endif
    /* vector extension */
    bool vill;
    /*
     * Encode LMUL to lmul as follows:
     *     LMUL    vlmul    lmul
     *      1       000       0
     *      2       001       1
     *      4       010       2
     *      8       011       3
     *      -       100       -
     *     1/8      101      -3
     *     1/4      110      -2
     *     1/2      111      -1
     */
    int8_t lmul;
    uint8_t sew;
    target_ulong vstart;
    bool vl_eq_vlmax;
    uint8_t ntemp;
    CPUState *cs;
    TCGv zero;
    /* Space for 3 operands plus 1 extra for address computation. */
    TCGv temp[4];
    /* Space for 4 operands(1 dest and <=3 src) for float point computation */
    TCGv_i64 ftemp[4];
    uint8_t nftemp;
    /* PointerMasking extension */
    bool pm_mask_enabled;
    bool pm_base_enabled;
} DisasContext;

#ifdef CONFIG_DEBUG_TCG
#define gen_mark_pc_updated() tcg_gen_movi_tl(_pc_is_current, 1)
#else
#define gen_mark_pc_updated() ((void)0)
#endif

static inline bool has_ext(DisasContext *ctx, uint32_t ext)
{
    return ctx->misa_ext & ext;
}

static bool always_true_p(DisasContext *ctx  __attribute__((__unused__)))
{
    return true;
}

#define MATERIALISE_EXT_PREDICATE(ext)  \
    static bool has_ ## ext ## _p(DisasContext *ctx)    \
    { \
        return ctx->cfg_ptr->ext_ ## ext ; \
    }

MATERIALISE_EXT_PREDICATE(XVentanaCondOps);

#ifdef TARGET_RISCV32
#define get_xl(ctx)    MXL_RV32
#elif defined(CONFIG_USER_ONLY)
#define get_xl(ctx)    MXL_RV64
#else
#define get_xl(ctx)    ((ctx)->xl)
#endif

/* The word size for this machine mode. */
static inline int __attribute__((unused)) get_xlen(DisasContext *ctx)
{
    return 16 << get_xl(ctx);
}

/* The operation length, as opposed to the xlen. */
#ifdef TARGET_RISCV32
#define get_ol(ctx)    MXL_RV32
#else
#define get_ol(ctx)    ((ctx)->ol)
#endif

static inline int get_olen(DisasContext *ctx)
{
    return 16 << get_ol(ctx);
}

/* The maximum register length */
#ifdef TARGET_RISCV32
#define get_xl_max(ctx)    MXL_RV32
#else
#define get_xl_max(ctx)    ((ctx)->misa_mxl_max)
#endif

/*
 * RISC-V requires NaN-boxing of narrower width floating point values.
 * This applies when a 32-bit value is assigned to a 64-bit FP register.
 * For consistency and simplicity, we nanbox results even when the RVD
 * extension is not present.
 */
static void gen_nanbox_s(TCGv_i64 out, TCGv_i64 in)
{
    tcg_gen_ori_i64(out, in, MAKE_64BIT_MASK(32, 32));
}

static void gen_nanbox_h(TCGv_i64 out, TCGv_i64 in)
{
    tcg_gen_ori_i64(out, in, MAKE_64BIT_MASK(16, 48));
}

/*
 * A narrow n-bit operation, where n < FLEN, checks that input operands
 * are correctly Nan-boxed, i.e., all upper FLEN - n bits are 1.
 * If so, the least-significant bits of the input are used, otherwise the
 * input value is treated as an n-bit canonical NaN (v2.2 section 9.2).
 *
 * Here, the result is always nan-boxed, even the canonical nan.
 */
static void gen_check_nanbox_h(TCGv_i64 out, TCGv_i64 in)
{
    TCGv_i64 t_max = tcg_const_i64(0xffffffffffff0000ull);
    TCGv_i64 t_nan = tcg_const_i64(0xffffffffffff7e00ull);

    tcg_gen_movcond_i64(TCG_COND_GEU, out, in, t_max, in, t_nan);
    tcg_temp_free_i64(t_max);
    tcg_temp_free_i64(t_nan);
}

static void gen_check_nanbox_s(TCGv_i64 out, TCGv_i64 in)
{
    TCGv_i64 t_max = tcg_constant_i64(0xffffffff00000000ull);
    TCGv_i64 t_nan = tcg_constant_i64(0xffffffff7fc00000ull);

    tcg_gen_movcond_i64(TCG_COND_GEU, out, in, t_max, in, t_nan);
}

static void gen_set_pc_imm(DisasContext *ctx, target_ulong dest)
{
    if (get_xl(ctx) == MXL_RV32) {
        dest = (int32_t)dest;
    }
    tcg_gen_movi_tl(cpu_pc, dest);
    gen_mark_pc_updated();
}

static void gen_set_pc(DisasContext *ctx, TCGv dest)
{
    if (get_xl(ctx) == MXL_RV32) {
        tcg_gen_ext32s_tl(cpu_pc, dest);
    } else {
        tcg_gen_mov_tl(cpu_pc, dest);
    }
    gen_mark_pc_updated();
}

static void generate_exception(DisasContext *ctx, int excp)
{
    gen_set_pc_imm(ctx, ctx->base.pc_next);
    gen_helper_raise_exception(cpu_env, tcg_constant_i32(excp));
    ctx->base.is_jmp = DISAS_NORETURN;
}

static void generate_exception_mtval(DisasContext *ctx, int excp)
{
    gen_set_pc_imm(ctx, ctx->base.pc_next);
    tcg_gen_st_tl(cpu_pc, cpu_env, offsetof(CPURISCVState, badaddr));
    gen_helper_raise_exception(cpu_env, tcg_constant_i32(excp));
    ctx->base.is_jmp = DISAS_NORETURN;
}

static void gen_exception_illegal(DisasContext *ctx)
{
    tcg_gen_st_i32(tcg_constant_i32(ctx->opcode), cpu_env,
                   offsetof(CPURISCVState, bins));

    generate_exception(ctx, RISCV_EXCP_ILLEGAL_INST);
}

static void gen_exception_inst_addr_mis(DisasContext *ctx)
{
    generate_exception_mtval(ctx, RISCV_EXCP_INST_ADDR_MIS);
}

static void gen_check_branch_target(DisasContext *ctx, target_ulong dest);
static void gen_goto_tb(DisasContext *ctx, int n, target_ulong dest,
                        bool bounds_check)
{
    if (bounds_check)
        gen_check_branch_target(ctx, dest);

    if (translator_use_goto_tb(&ctx->base, dest)) {
        tcg_gen_goto_tb(n);
        gen_set_pc_imm(ctx, dest);
        tcg_gen_exit_tb(ctx->base.tb, n);
    } else {
        gen_set_pc_imm(ctx, dest);
        tcg_gen_lookup_and_goto_ptr();
    }
}

/*
 * Wrappers for getting reg values.
 *
 * The $zero register does not have cpu_gpr[0] allocated -- we supply the
 * constant zero as a source, and an uninitialized sink as destination.
 *
 * Further, we may provide an extension for word operations.
 */
static TCGv temp_new(DisasContext *ctx)
{
    assert(ctx->ntemp < ARRAY_SIZE(ctx->temp));
    return ctx->temp[ctx->ntemp++] = tcg_temp_new();
}

static TCGv get_gpr(DisasContext *ctx, int reg_num, DisasExtend ext)
{
    TCGv t;
#ifdef TARGET_CHERI
    TCGv *cpu_gpr_cursors = _cpu_cursors_do_not_access_directly;
#else
    TCGv *cpu_gpr_cursors = cpu_gpr;
#endif

    if (reg_num == 0) {
        return ctx->zero;
    }

    switch (get_ol(ctx)) {
    case MXL_RV32:
        switch (ext) {
        case EXT_NONE:
            break;
        case EXT_SIGN:
            t = temp_new(ctx);
            tcg_gen_ext32s_tl(t, cpu_gpr_cursors[reg_num]);
            return t;
        case EXT_ZERO:
            t = temp_new(ctx);
            tcg_gen_ext32u_tl(t, cpu_gpr_cursors[reg_num]);
            return t;
        default:
            g_assert_not_reached();
        }
        break;
    case MXL_RV64:
    case MXL_RV128:
        break;
    default:
        g_assert_not_reached();
    }
    return cpu_gpr_cursors[reg_num];
}

static void gen_get_gpr(DisasContext *ctx, TCGv t, int reg_num)
{
    tcg_gen_mov_tl(t, get_gpr(ctx, reg_num, EXT_NONE));
}

#ifndef TARGET_CHERI
static TCGv get_gprh(DisasContext *ctx, int reg_num)
{
    assert(get_xl(ctx) == MXL_RV128);
    if (reg_num == 0) {
        return ctx->zero;
    }
    return cpu_gprh[reg_num];
}
#endif

static TCGv dest_gpr(DisasContext *ctx, int reg_num)
{
    if (reg_num == 0 || get_olen(ctx) < TARGET_LONG_BITS) {
        return temp_new(ctx);
    }
#ifdef TARGET_CHERI
    return _cpu_cursors_do_not_access_directly[reg_num];
#else
    return cpu_gpr[reg_num];
#endif
}

#ifndef TARGET_CHERI
static TCGv dest_gprh(DisasContext *ctx, int reg_num)
{
    if (reg_num == 0) {
        return temp_new(ctx);
    }
    return cpu_gprh[reg_num];
}
#endif

#include "cheri-translate-utils.h"

static void _gen_set_gpr(DisasContext *ctx, int reg_num, TCGv t,
                        bool clear_pesbt)
{
    if (reg_num != 0) {
#ifdef TARGET_CHERI
        TCGv *dest_gpr = _cpu_cursors_do_not_access_directly;
        if (clear_pesbt) {
            // Reset the register type to int.
            gen_lazy_cap_set_int(ctx, reg_num);
        }
#else
        TCGv *dest_gpr = cpu_gpr;
#endif
        switch (get_ol(ctx)) {
        case MXL_RV32:
            tcg_gen_ext32s_tl(dest_gpr[reg_num], t);
            break;
        case MXL_RV64:
        case MXL_RV128:
            tcg_gen_mov_tl(dest_gpr[reg_num], t);
            break;
        default:
            g_assert_not_reached();
        }
#ifndef TARGET_CHERI
        if (get_xl_max(ctx) == MXL_RV128) {
            tcg_gen_sari_tl(cpu_gprh[reg_num], cpu_gpr[reg_num], 63);
        }
#endif
        gen_rvfi_dii_set_field_const_i8(INTEGER, rd_addr, reg_num);
        gen_rvfi_dii_set_field_zext_tl(INTEGER, rd_wdata, t);
#ifdef CONFIG_TCG_LOG_INSTR
        // Log GPR writes here
        if (qemu_ctx_logging_enabled(ctx)) {
            gen_helper_riscv_log_gpr_write(cpu_env, tcg_constant_i32(reg_num),
                                           t);
        }
#endif
    }
}

static void gen_set_gpri(DisasContext *ctx, int reg_num, target_long imm)
{
    if (reg_num != 0) {
#ifdef TARGET_CHERI
        TCGv *dest_gpr = _cpu_cursors_do_not_access_directly;
        /* Reset the register type to int. */
        gen_lazy_cap_set_int(ctx, reg_num);
#else
        TCGv *dest_gpr = cpu_gpr;
#endif
        switch (get_ol(ctx)) {
        case MXL_RV32:
            tcg_gen_movi_tl(dest_gpr[reg_num], (int32_t)imm);
            break;
        case MXL_RV64:
        case MXL_RV128:
            tcg_gen_movi_tl(dest_gpr[reg_num], imm);
            break;
        default:
            g_assert_not_reached();
        }
#ifndef TARGET_CHERI
        if (get_xl_max(ctx) == MXL_RV128) {
            tcg_gen_movi_tl(cpu_gprh[reg_num], -(imm < 0));
        }
#endif
    }
}

#define gen_set_gpr(ctx, reg_num_dst, t) _gen_set_gpr(ctx, reg_num_dst, t, true)
#define gen_set_gpr_const(ctx, reg_num_dst, t) gen_set_gpri(ctx, reg_num_dst, t)

#ifndef TARGET_CHERI
static void gen_set_gpr128(DisasContext *ctx, int reg_num, TCGv rl, TCGv rh)
{
    assert(get_ol(ctx) == MXL_RV128);
    if (reg_num != 0) {
        tcg_gen_mov_tl(cpu_gpr[reg_num], rl);
        tcg_gen_mov_tl(cpu_gprh[reg_num], rh);
    }
}
#endif

#ifdef CONFIG_TCG_LOG_INSTR
static inline void gen_riscv_log_instr(DisasContext *ctx, uint32_t opcode,
                                       int width)
{
    if (qemu_ctx_logging_enabled(ctx)) {
        TCGv tpc = tcg_const_tl(ctx->base.pc_next);
        TCGv_i32 topc = tcg_constant_i32(opcode);
        TCGv_i32 twidth = tcg_constant_i32(width);
        // TODO(am2419): bswap opcode if target byte-order != host byte-order
        gen_helper_riscv_log_instr(cpu_env, tpc, topc, twidth);
        tcg_temp_free(tpc);
    }
}

#else /* ! CONFIG_TCG_LOG_INSTR */
#define gen_riscv_log_instr(ctx, opcode, width) ((void)0)
#endif /* ! CONFIG_TCG_LOG_INSTR */

#define gen_riscv_log_instr16(ctx, opcode)              \
    gen_riscv_log_instr(ctx, opcode, sizeof(uint16_t))
#define gen_riscv_log_instr32(ctx, opcode)              \
    gen_riscv_log_instr(ctx, opcode, sizeof(uint32_t))


void cheri_tcg_save_pc(DisasContextBase *db)
{
    DisasContext *ctx = container_of(db, DisasContext, base);
    gen_set_pc_imm(ctx, db->pc_next);
}
// We have to call gen_set_pc_imm() before setting DISAS_NORETURN (see
// generate_exception())
void cheri_tcg_prepare_for_unconditional_exception(DisasContextBase *db)
{
    cheri_tcg_save_pc(db);
    db->is_jmp = DISAS_NORETURN;
}

static TCGv_i64 ftemp_new(DisasContext *ctx)
{
    assert(ctx->nftemp < ARRAY_SIZE(ctx->ftemp));
    return ctx->ftemp[ctx->nftemp++] = tcg_temp_new_i64();
}

static TCGv_i64 get_fpr_hs(DisasContext *ctx, int reg_num)
{
    if (!ctx->cfg_ptr->ext_zfinx) {
        return cpu_fpr[reg_num];
    }

    if (reg_num == 0) {
        return tcg_constant_i64(0);
    }
    switch (get_xl(ctx)) {
    case MXL_RV32:
#ifdef TARGET_RISCV32
    {
        TCGv_i64 t = ftemp_new(ctx);
        tcg_gen_ext_i32_i64(t, get_gpr(ctx, reg_num, EXT_NONE));
        return t;
    }
#else
    /* fall through */
    case MXL_RV64:
        return get_gpr(ctx, reg_num, EXT_NONE);
#endif
    default:
        g_assert_not_reached();
    }
}

static TCGv_i64 get_fpr_d(DisasContext *ctx, int reg_num)
{
    if (!ctx->cfg_ptr->ext_zfinx) {
        return cpu_fpr[reg_num];
    }

    if (reg_num == 0) {
        return tcg_constant_i64(0);
    }
    switch (get_xl(ctx)) {
    case MXL_RV32:
    {
        TCGv_i64 t = ftemp_new(ctx);
        tcg_gen_concat_tl_i64(t, get_gpr(ctx, reg_num, EXT_NONE),
                         get_gpr(ctx, reg_num + 1, EXT_NONE));
        return t;
    }
#ifdef TARGET_RISCV64
    case MXL_RV64:
        return get_gpr(ctx, reg_num, EXT_NONE);
#endif
    default:
        g_assert_not_reached();
    }
}

static TCGv_i64 dest_fpr(DisasContext *ctx, int reg_num)
{
    if (!ctx->cfg_ptr->ext_zfinx) {
        return cpu_fpr[reg_num];
    }

    if (reg_num == 0) {
        return ftemp_new(ctx);
    }

    switch (get_xl(ctx)) {
    case MXL_RV32:
        return ftemp_new(ctx);
#ifdef TARGET_RISCV64
    case MXL_RV64:
        return dest_gpr(ctx, reg_num);
#endif
    default:
        g_assert_not_reached();
    }
}

/* assume t is nanboxing (for normal) or sign-extended (for zfinx) */
static void gen_set_fpr_hs(DisasContext *ctx, int reg_num, TCGv_i64 t)
{
    if (!ctx->cfg_ptr->ext_zfinx) {
        tcg_gen_mov_i64(cpu_fpr[reg_num], t);
        return;
    }
    if (reg_num != 0) {
        switch (get_xl(ctx)) {
        case MXL_RV32:
#ifdef TARGET_RISCV32
            tcg_gen_extrl_i64_i32(dest_gpr(ctx, reg_num), t);
            break;
#else
        /* fall through */
        case MXL_RV64:
            tcg_gen_mov_i64(dest_gpr(ctx, reg_num), t);
            break;
#endif
        default:
            g_assert_not_reached();
        }
    }
}

static void gen_set_fpr_d(DisasContext *ctx, int reg_num, TCGv_i64 t)
{
    if (!ctx->cfg_ptr->ext_zfinx) {
        tcg_gen_mov_i64(cpu_fpr[reg_num], t);
        return;
    }

    if (reg_num != 0) {
        switch (get_xl(ctx)) {
        case MXL_RV32:
#ifdef TARGET_RISCV32

            tcg_gen_extr_i64_i32(dest_gpr(ctx, reg_num), dest_gpr(ctx, reg_num + 1), t);
            break;
#else
            tcg_gen_ext32s_i64(dest_gpr(ctx, reg_num), t);
            tcg_gen_sari_i64(dest_gpr(ctx, reg_num + 1), t, 32);
            break;
        case MXL_RV64:
            tcg_gen_mov_i64(dest_gpr(ctx, reg_num), t);
            break;
#endif
        default:
            g_assert_not_reached();
        }
    }
}

static void gen_jal(DisasContext *ctx, int rd, target_ulong imm)
{
    target_ulong next_pc;

    /* check misaligned: */
    next_pc = ctx->base.pc_next + imm;
    gen_check_branch_target(ctx, next_pc);
    if (!has_ext(ctx, RVC) && !ctx->cfg_ptr->ext_zca) {
        if ((next_pc & 0x3) != 0) {
            gen_exception_inst_addr_mis(ctx);
            return;
        }
    }
    /* For CHERI ISAv8 the result is an offset relative to PCC.base */
    gen_set_gpr_const(ctx, rd, ctx->pc_succ_insn - pcc_reloc(ctx));

    gen_goto_tb(ctx, 0, ctx->base.pc_next + imm, /*bounds_check=*/true); /* must use this for safety */
    ctx->base.is_jmp = DISAS_NORETURN;
}

static void gen_jalr(DisasContext *ctx, int rd, int rs1, target_ulong imm)
{
    /* no chaining with JALR */
    TCGLabel *misaligned = NULL;
    // Note: We need to use tcg_temp_local_new() for t0 since
    // gen_check_branch_target_dynamic() inserts branches.
    TCGv t0 = tcg_temp_local_new();

    gen_get_gpr(ctx, t0, rs1);
    /* For CHERI ISAv8 the destination is an offset relative to PCC.base. */
    tcg_gen_addi_tl(t0, t0, imm + pcc_reloc(ctx));
    tcg_gen_andi_tl(t0, t0, (target_ulong)-2);
    gen_check_branch_target_dynamic(ctx, t0);
    // Note: Only update cpu_pc after a successful bounds check to avoid
    // representability issues caused by directly modifying PCC.cursor.
    gen_set_pc(ctx, t0);

    if (!has_ext(ctx, RVC) && !ctx->cfg_ptr->ext_zca) {
        misaligned = gen_new_label();
        tcg_gen_andi_tl(t0, cpu_pc, 0x2);
        tcg_gen_brcondi_tl(TCG_COND_NE, t0, 0x0, misaligned);
    }

    /* For CHERI ISAv8 the result is an offset relative to PCC.base */
    gen_set_gpri(ctx, rd, ctx->pc_succ_insn - pcc_reloc(ctx));
    /* No chaining with JALR. */
    tcg_gen_lookup_and_goto_ptr();

    if (misaligned) {
        gen_set_label(misaligned);
        gen_exception_inst_addr_mis(ctx);
    }
    ctx->base.is_jmp = DISAS_NORETURN;

    tcg_temp_free(t0);
}

/* Compute a canonical address from a register plus offset. */
static TCGv get_address(DisasContext *ctx, int rs1, int imm)
{
    TCGv addr = temp_new(ctx);
    TCGv src1 = get_gpr(ctx, rs1, EXT_NONE);

    tcg_gen_addi_tl(addr, src1, imm);
    if (ctx->pm_mask_enabled) {
        tcg_gen_and_tl(addr, addr, pm_mask);
    } else if (get_xl(ctx) == MXL_RV32) {
        tcg_gen_ext32u_tl(addr, addr);
    }
    if (ctx->pm_base_enabled) {
        tcg_gen_or_tl(addr, addr, pm_base);
    }
    return addr;
}

#ifndef CONFIG_USER_ONLY
/* The states of mstatus_fs are:
 * 0 = disabled, 1 = initial, 2 = clean, 3 = dirty
 * We will have already diagnosed disabled state,
 * and need to turn initial/clean into dirty.
 */
static void mark_fs_dirty(DisasContext *ctx)
{
    TCGv tmp;

    if (!has_ext(ctx, RVF)) {
        return;
    }

    if (ctx->mstatus_fs != MSTATUS_FS) {
        /* Remember the state change for the rest of the TB. */
        ctx->mstatus_fs = MSTATUS_FS;

        tmp = tcg_temp_new();
        tcg_gen_ld_tl(tmp, cpu_env, offsetof(CPURISCVState, mstatus));
        tcg_gen_ori_tl(tmp, tmp, MSTATUS_FS);
        tcg_gen_st_tl(tmp, cpu_env, offsetof(CPURISCVState, mstatus));
        tcg_temp_free(tmp);
    }

    if (ctx->virt_enabled && ctx->mstatus_hs_fs != MSTATUS_FS) {
        /* Remember the stage change for the rest of the TB. */
        ctx->mstatus_hs_fs = MSTATUS_FS;

        tmp = tcg_temp_new();
        tcg_gen_ld_tl(tmp, cpu_env, offsetof(CPURISCVState, mstatus_hs));
        tcg_gen_ori_tl(tmp, tmp, MSTATUS_FS);
        tcg_gen_st_tl(tmp, cpu_env, offsetof(CPURISCVState, mstatus_hs));
        tcg_temp_free(tmp);
    }
}
#else
static inline void mark_fs_dirty(DisasContext *ctx) { }
#endif

#ifndef CONFIG_USER_ONLY
/* The states of mstatus_vs are:
 * 0 = disabled, 1 = initial, 2 = clean, 3 = dirty
 * We will have already diagnosed disabled state,
 * and need to turn initial/clean into dirty.
 */
static void mark_vs_dirty(DisasContext *ctx)
{
    TCGv tmp;

    if (ctx->mstatus_vs != MSTATUS_VS) {
        /* Remember the state change for the rest of the TB.  */
        ctx->mstatus_vs = MSTATUS_VS;

        tmp = tcg_temp_new();
        tcg_gen_ld_tl(tmp, cpu_env, offsetof(CPURISCVState, mstatus));
        tcg_gen_ori_tl(tmp, tmp, MSTATUS_VS);
        tcg_gen_st_tl(tmp, cpu_env, offsetof(CPURISCVState, mstatus));
        tcg_temp_free(tmp);
    }

    if (ctx->virt_enabled && ctx->mstatus_hs_vs != MSTATUS_VS) {
        /* Remember the stage change for the rest of the TB. */
        ctx->mstatus_hs_vs = MSTATUS_VS;

        tmp = tcg_temp_new();
        tcg_gen_ld_tl(tmp, cpu_env, offsetof(CPURISCVState, mstatus_hs));
        tcg_gen_ori_tl(tmp, tmp, MSTATUS_VS);
        tcg_gen_st_tl(tmp, cpu_env, offsetof(CPURISCVState, mstatus_hs));
        tcg_temp_free(tmp);
    }
}
#else
static inline void mark_vs_dirty(DisasContext *ctx) { }
#endif

static void gen_set_rm(DisasContext *ctx, int rm)
{
    if (ctx->frm == rm) {
        return;
    }
    ctx->frm = rm;

    if (rm == RISCV_FRM_ROD) {
        gen_helper_set_rod_rounding_mode(cpu_env);
        return;
    }

    gen_helper_set_rounding_mode(cpu_env, tcg_constant_i32(rm));
}

static int ex_plus_1(DisasContext *ctx, int nf)
{
    return nf + 1;
}

#define EX_SH(amount) \
    static int ex_shift_##amount(DisasContext *ctx, int imm) \
    {                                         \
        return imm << amount;                 \
    }
EX_SH(1)
EX_SH(2)
EX_SH(3)
EX_SH(4)
EX_SH(12)

#define REQUIRE_EXT(ctx, ext) do { \
    if (!has_ext(ctx, ext)) {      \
        return false;              \
    }                              \
} while (0)

#define REQUIRE_32BIT(ctx) do {    \
    if (get_xl(ctx) != MXL_RV32) { \
        return false;              \
    }                              \
} while (0)

#define REQUIRE_64BIT(ctx) do {     \
    if (get_xl(ctx) != MXL_RV64) {  \
        return false;               \
    }                               \
} while (0)

#define REQUIRE_128BIT(ctx) do {    \
    if (get_xl(ctx) != MXL_RV128) { \
        return false;               \
    }                               \
} while (0)

#define REQUIRE_64_OR_128BIT(ctx) do { \
    if (get_xl(ctx) == MXL_RV32) {     \
        return false;                  \
    }                                  \
} while (0)

#define REQUIRE_EITHER_EXT(ctx, A, B) do {       \
    if (!ctx->cfg_ptr->ext_##A &&      \
        !ctx->cfg_ptr->ext_##B) {      \
        return false;                            \
    }                                            \
} while (0)

static int ex_rvc_register(DisasContext *ctx, int reg)
{
    return 8 + reg;
}

static int ex_rvc_shifti(DisasContext *ctx, int imm)
{
    /* For RV128 a shamt of 0 means a shift by 64. */
    return imm ? imm : 64;
}

static bool pred_capmode(DisasContext *ctx)
{
#ifdef TARGET_CHERI
    return ctx->capmode;
#else
    return false;
#endif
}

static bool pred_rv64_capmode(DisasContext *ctx)
{
    REQUIRE_64BIT(ctx);
    return pred_capmode(ctx);
}

static bool pred_xcheri(DisasContext *ctx)
{
#ifdef TARGET_CHERI_RISCV_V9
    return true;
#else
    return false;
#endif
}

static bool pred_xcheri_rv32(DisasContext *ctx)
{
    return pred_xcheri(ctx) && get_xl(ctx) == MXL_RV32;
}

#if defined(TARGET_CHERI)
static bool pred_hybrid(DisasContext *ctx)
{
    return ctx->hybrid;
}
#endif

#ifdef TARGET_CHERI_RISCV_STD
static bool pred_cre(DisasContext *ctx)
{
    return ctx->cre;
}
#endif

/* Include the auto-generated decoder for 32 bit insn */
#include "decode-insn32.c.inc"

static bool gen_logic_imm_fn(DisasContext *ctx, arg_i *a,
                             void (*func)(TCGv, TCGv, target_long))
{
    TCGv dest = dest_gpr(ctx, a->rd);
    TCGv src1 = get_gpr(ctx, a->rs1, EXT_NONE);

    func(dest, src1, a->imm);

    if (get_xl(ctx) == MXL_RV128) {
#ifdef TARGET_CHERI
        return false; /* RV128 not supported */
#else
        TCGv src1h = get_gprh(ctx, a->rs1);
        TCGv desth = dest_gprh(ctx, a->rd);

        func(desth, src1h, -(a->imm < 0));
        gen_set_gpr128(ctx, a->rd, dest, desth);
#endif
    } else {
        gen_set_gpr(ctx, a->rd, dest);
    }

    return true;
}

static bool gen_logic(DisasContext *ctx, arg_r *a,
                      void (*func)(TCGv, TCGv, TCGv))
{
    TCGv dest = dest_gpr(ctx, a->rd);
    TCGv src1 = get_gpr(ctx, a->rs1, EXT_NONE);
    TCGv src2 = get_gpr(ctx, a->rs2, EXT_NONE);

    func(dest, src1, src2);

    if (get_xl(ctx) == MXL_RV128) {
#ifdef TARGET_CHERI
        return false; /* RV128 not supported */
#else
        TCGv src1h = get_gprh(ctx, a->rs1);
        TCGv src2h = get_gprh(ctx, a->rs2);
        TCGv desth = dest_gprh(ctx, a->rd);

        func(desth, src1h, src2h);
        gen_set_gpr128(ctx, a->rd, dest, desth);
#endif
    } else {
        gen_set_gpr(ctx, a->rd, dest);
    }

    return true;
}

static bool gen_arith_imm_fn(DisasContext *ctx, arg_i *a, DisasExtend ext,
                             void (*func)(TCGv, TCGv, target_long),
                             void (*f128)(TCGv, TCGv, TCGv, TCGv, target_long))
{
    TCGv dest = dest_gpr(ctx, a->rd);
    TCGv src1 = get_gpr(ctx, a->rs1, ext);

    if (get_ol(ctx) < MXL_RV128) {
        func(dest, src1, a->imm);
        gen_set_gpr(ctx, a->rd, dest);
    } else {
        if (f128 == NULL) {
            return false;
        }
#ifdef TARGET_CHERI
        return false; /* RV128 not supported */
#else
        TCGv src1h = get_gprh(ctx, a->rs1);
        TCGv desth = dest_gprh(ctx, a->rd);

        f128(dest, desth, src1, src1h, a->imm);
        gen_set_gpr128(ctx, a->rd, dest, desth);
#endif
    }
    return true;
}

static bool gen_arith_imm_tl(DisasContext *ctx, arg_i *a, DisasExtend ext,
                             void (*func)(TCGv, TCGv, TCGv),
                             void (*f128)(TCGv, TCGv, TCGv, TCGv, TCGv, TCGv))
{
    TCGv dest = dest_gpr(ctx, a->rd);
    TCGv src1 = get_gpr(ctx, a->rs1, ext);
    TCGv src2 = tcg_constant_tl(a->imm);

    if (get_ol(ctx) < MXL_RV128) {
        func(dest, src1, src2);
        gen_set_gpr(ctx, a->rd, dest);
    } else {
        if (f128 == NULL) {
            return false;
        }
#ifdef TARGET_CHERI
        return false; /* RV128 not supported */
#else
        TCGv src1h = get_gprh(ctx, a->rs1);
        TCGv src2h = tcg_constant_tl(-(a->imm < 0));
        TCGv desth = dest_gprh(ctx, a->rd);

        f128(dest, desth, src1, src1h, src2, src2h);
        gen_set_gpr128(ctx, a->rd, dest, desth);
#endif
    }
    return true;
}

static bool gen_arith(DisasContext *ctx, arg_r *a, DisasExtend ext,
                      void (*func)(TCGv, TCGv, TCGv),
                      void (*f128)(TCGv, TCGv, TCGv, TCGv, TCGv, TCGv))
{
    TCGv dest = dest_gpr(ctx, a->rd);
    TCGv src1 = get_gpr(ctx, a->rs1, ext);
    TCGv src2 = get_gpr(ctx, a->rs2, ext);

    if (get_ol(ctx) < MXL_RV128) {
        func(dest, src1, src2);
        gen_set_gpr(ctx, a->rd, dest);
    } else {
        if (f128 == NULL) {
            return false;
        }
#ifdef TARGET_CHERI
        return false; /* RV128 not supported */
#else
        TCGv src1h = get_gprh(ctx, a->rs1);
        TCGv src2h = get_gprh(ctx, a->rs2);
        TCGv desth = dest_gprh(ctx, a->rd);

        f128(dest, desth, src1, src1h, src2, src2h);
        gen_set_gpr128(ctx, a->rd, dest, desth);
#endif
    }
    return true;
}

static bool gen_arith_per_ol(DisasContext *ctx, arg_r *a, DisasExtend ext,
                             void (*f_tl)(TCGv, TCGv, TCGv),
                             void (*f_32)(TCGv, TCGv, TCGv),
                             void (*f_128)(TCGv, TCGv, TCGv, TCGv, TCGv, TCGv))
{
    int olen = get_olen(ctx);

    if (olen != TARGET_LONG_BITS) {
        if (olen == 32) {
            f_tl = f_32;
        } else if (olen != 128) {
            g_assert_not_reached();
        }
    }
    return gen_arith(ctx, a, ext, f_tl, f_128);
}

static bool gen_shift_imm_fn(DisasContext *ctx, arg_shift *a, DisasExtend ext,
                             void (*func)(TCGv, TCGv, target_long),
                             void (*f128)(TCGv, TCGv, TCGv, TCGv, target_long))
{
    TCGv dest, src1;
    int max_len = get_olen(ctx);

    if (a->shamt >= max_len) {
        return false;
    }

    dest = dest_gpr(ctx, a->rd);
    src1 = get_gpr(ctx, a->rs1, ext);

    if (max_len < 128) {
        func(dest, src1, a->shamt);
        gen_set_gpr(ctx, a->rd, dest);
    } else {
#ifdef TARGET_CHERI
        return false; /* RV128 not supported */
#else
        TCGv src1h = get_gprh(ctx, a->rs1);
        TCGv desth = dest_gprh(ctx, a->rd);

        if (f128 == NULL) {
            return false;
        }
        f128(dest, desth, src1, src1h, a->shamt);
        gen_set_gpr128(ctx, a->rd, dest, desth);
#endif
    }
    return true;
}

static bool gen_shift_imm_fn_per_ol(DisasContext *ctx, arg_shift *a,
                                    DisasExtend ext,
                                    void (*f_tl)(TCGv, TCGv, target_long),
                                    void (*f_32)(TCGv, TCGv, target_long),
                                    void (*f_128)(TCGv, TCGv, TCGv, TCGv,
                                                  target_long))
{
    int olen = get_olen(ctx);
    if (olen != TARGET_LONG_BITS) {
        if (olen == 32) {
            f_tl = f_32;
        } else if (olen != 128) {
            g_assert_not_reached();
        }
    }
    return gen_shift_imm_fn(ctx, a, ext, f_tl, f_128);
}

static bool gen_shift_imm_tl(DisasContext *ctx, arg_shift *a, DisasExtend ext,
                             void (*func)(TCGv, TCGv, TCGv))
{
    TCGv dest, src1, src2;
    int max_len = get_olen(ctx);

    if (a->shamt >= max_len) {
        return false;
    }

    dest = dest_gpr(ctx, a->rd);
    src1 = get_gpr(ctx, a->rs1, ext);
    src2 = tcg_constant_tl(a->shamt);

    func(dest, src1, src2);

    gen_set_gpr(ctx, a->rd, dest);
    return true;
}

static bool gen_shift(DisasContext *ctx, arg_r *a, DisasExtend ext,
                      void (*func)(TCGv, TCGv, TCGv),
                      void (*f128)(TCGv, TCGv, TCGv, TCGv, TCGv))
{
    TCGv src2 = get_gpr(ctx, a->rs2, EXT_NONE);
    TCGv ext2 = tcg_temp_new();
    int max_len = get_olen(ctx);

    tcg_gen_andi_tl(ext2, src2, max_len - 1);

    TCGv dest = dest_gpr(ctx, a->rd);
    TCGv src1 = get_gpr(ctx, a->rs1, ext);

    if (max_len < 128) {
        func(dest, src1, ext2);
        gen_set_gpr(ctx, a->rd, dest);
    } else {
#ifdef TARGET_CHERI
        return false; /* RV128 not supported */
#else
        TCGv src1h = get_gprh(ctx, a->rs1);
        TCGv desth = dest_gprh(ctx, a->rd);

        if (f128 == NULL) {
            return false;
        }
        f128(dest, desth, src1, src1h, ext2);
        gen_set_gpr128(ctx, a->rd, dest, desth);
#endif
    }
    tcg_temp_free(ext2);
    return true;
}

static bool gen_shift_per_ol(DisasContext *ctx, arg_r *a, DisasExtend ext,
                             void (*f_tl)(TCGv, TCGv, TCGv),
                             void (*f_32)(TCGv, TCGv, TCGv),
                             void (*f_128)(TCGv, TCGv, TCGv, TCGv, TCGv))
{
    int olen = get_olen(ctx);
    if (olen != TARGET_LONG_BITS) {
        if (olen == 32) {
            f_tl = f_32;
        } else if (olen != 128) {
            g_assert_not_reached();
        }
    }
    return gen_shift(ctx, a, ext, f_tl, f_128);
}

static bool gen_unary(DisasContext *ctx, arg_r2 *a, DisasExtend ext,
                      void (*func)(TCGv, TCGv))
{
    TCGv dest = dest_gpr(ctx, a->rd);
    TCGv src1 = get_gpr(ctx, a->rs1, ext);

    func(dest, src1);

    gen_set_gpr(ctx, a->rd, dest);
    return true;
}

static bool gen_unary_per_ol(DisasContext *ctx, arg_r2 *a, DisasExtend ext,
                             void (*f_tl)(TCGv, TCGv),
                             void (*f_32)(TCGv, TCGv))
{
    int olen = get_olen(ctx);

    if (olen != TARGET_LONG_BITS) {
        if (olen == 32) {
            f_tl = f_32;
        } else {
            g_assert_not_reached();
        }
    }
    return gen_unary(ctx, a, ext, f_tl);
}

static uint32_t opcode_at(DisasContextBase *dcbase, target_ulong pc)
{
    DisasContext *ctx = container_of(dcbase, DisasContext, base);
    CPUState *cpu = ctx->cs;
    CPURISCVState *env = cpu->env_ptr;

    return cpu_ldl_code(env, pc);
}

/* Include insn module translation function */
#ifdef TARGET_CHERI
/* Must be included first since the helpers are used by trans_rvi.c.inc */
#include "insn_trans/trans_cheri.c.inc"
#endif

// Helpers to generate a virtual address that has been checked by the CHERI
// capability helpers: If ctx->capmode is set, the register number will be
// a capability and we check that capability, otherwise we treat the register
// as an offset relative to $ddc and check if that is in bounds.
// Note: the return value must be freed with tcg_temp_free_cap_checked()
static inline TCGv_cap_checked_ptr _get_capmode_dependent_addr(
    DisasContext *ctx, int reg_num, target_long regoffs,
#ifdef TARGET_CHERI
    void (*gen_check_cap)(TCGv_cap_checked_ptr, uint32_t, target_long, MemOp),
    void (*check_ddc)(TCGv_cap_checked_ptr, DisasContext *, TCGv, target_ulong),
#endif
    MemOp mop)

{
    TCGv_cap_checked_ptr result = tcg_temp_new_cap_checked();
#ifdef TARGET_CHERI
    // XXX-AM: Unsupported pointer masking extensions
    if (ctx->capmode) {
        gen_check_cap(result, reg_num, regoffs, mop);
    } else {
        generate_get_ddc_checked_gpr_plus_offset(result, ctx, reg_num, regoffs,
                                                 mop, check_ddc);
    }
#else
    TCGv tmp = get_address(ctx, reg_num, regoffs);
    tcg_gen_mov_cap_checked(result, (TCGv_cap_checked_ptr)tmp);
#endif
    return result;
}

static inline TCGv_cap_checked_ptr
get_capmode_dependent_load_addr(DisasContext *ctx, int reg_num,
                               target_long regoffs, MemOp mop)
{
    return _get_capmode_dependent_addr(ctx, reg_num, regoffs,
#ifdef TARGET_CHERI
                                       &generate_cap_load_check_imm,
                                       &generate_ddc_checked_load_ptr,
#endif
                                       mop);
}

static inline TCGv_cap_checked_ptr
get_capmode_dependent_store_addr(DisasContext *ctx, int reg_num,
                                target_long regoffs, MemOp mop)
{
    return _get_capmode_dependent_addr(ctx, reg_num, regoffs,
#ifdef TARGET_CHERI
                                       &generate_cap_store_check_imm,
                                       &generate_ddc_checked_store_ptr,
#endif
                                       mop);
}

static inline TCGv_cap_checked_ptr
get_capmode_dependent_rmw_addr(DisasContext *ctx, int reg_num,
                               target_long regoffs, MemOp mop)
{
    return _get_capmode_dependent_addr(ctx, reg_num, regoffs,
#ifdef TARGET_CHERI
                                       &generate_cap_rmw_check_imm,
                                       &generate_ddc_checked_rmw_ptr,
#endif
                                       mop);
}

#include "insn_trans/trans_rvi.c.inc"
#include "insn_trans/trans_rvm.c.inc"
#include "insn_trans/trans_rva.c.inc"
#include "insn_trans/trans_rvf.c.inc"
#include "insn_trans/trans_rvd.c.inc"
#include "insn_trans/trans_rvh.c.inc"
#include "insn_trans/trans_rvv.c.inc"
#include "insn_trans/trans_rvb.c.inc"
#include "insn_trans/trans_rvzicbo.c.inc"
#include "insn_trans/trans_rvzfh.c.inc"
#include "insn_trans/trans_privileged.c.inc"
#include "insn_trans/trans_svinval.c.inc"
#include "insn_trans/trans_xventanacondops.c.inc"

/* Include the auto-generated decoder for 16 bit insn */
#include "decode-insn16.c.inc"
#include "insn_trans/trans_rvzce.c.inc"
/* Include decoders for factored-out extensions */
#include "decode-XVentanaCondOps.c.inc"

static bool trans_c_hint(DisasContext *ctx, arg_c_hint *a)
{
    return true;
}

#ifndef TARGET_CHERI

#define TRANS_STUB(instr)                                                      \
    static bool trans_##instr(DisasContext *ctx, arg_##instr *a)               \
    {                                                                          \
        g_assert_not_reached();                                                \
        return false;                                                          \
    }

/* Stubs needed for mode-dependent compressed instructions */
TRANS_STUB(lc)
TRANS_STUB(sc)
TRANS_STUB(caddi)
TRANS_STUB(cmv)
TRANS_STUB(lr_c)
TRANS_STUB(sc_c)
TRANS_STUB(amoswap_c)
#else
static bool trans_sq(DisasContext *ctx, arg_sq *a) { return false; }
#endif

static void decode_opc(CPURISCVState *env, DisasContext *ctx, uint16_t opcode)
{
    /*
     * A table with predicate (i.e., guard) functions and decoder functions
     * that are tested in-order until a decoder matches onto the opcode.
     */
    static const struct {
        bool (*guard_func)(DisasContext *);
        bool (*decode_func)(DisasContext *, uint32_t);
    } decoders[] = {
        { always_true_p,  decode_insn32 },
        { has_XVentanaCondOps_p,  decode_XVentanaCodeOps },
    };

    /* Check for compressed insn */
    if (extract16(opcode, 0, 2) != 3) {
        gen_riscv_log_instr16(ctx, opcode);
        gen_check_pcc_bounds_next_inst(ctx, 2);
        gen_rvfi_dii_set_field_const_i64(INST, insn, opcode);
        /*
         * The Zca extension is added as way to refer to instructions in the C
         * extension that do not include the floating-point loads and stores
         */
        if (!has_ext(ctx, RVC) || !ctx->cfg_ptr->ext_zca) {
            gen_exception_illegal(ctx);
        } else {
            ctx->opcode = opcode;
            ctx->pc_succ_insn = ctx->base.pc_next + 2;
            if (decode_insn16(ctx, opcode)) {
                return;
            }
        }
    } else {
#ifdef CONFIG_RVFI_DII
        // We have to avoid memory accesses for injected instructions since
        // the PC could point somewhere invalid.
        uint16_t next_16 = env->rvfi_dii_have_injected_insn
                          ? (env->rvfi_dii_injected_insn >> 16)
                          : translator_lduw(env, &ctx->base,
                                            ctx->base.pc_next + 2);
#else
        uint16_t next_16 = translator_lduw(env, &ctx->base,
                                           ctx->base.pc_next + 2);
#endif
        uint32_t opcode32 = opcode;
        opcode32 = deposit32(opcode32, 16, 16, next_16);
        gen_riscv_log_instr32(ctx, opcode32);
        gen_check_pcc_bounds_next_inst(ctx, 4);
        ctx->opcode = opcode32;
        ctx->pc_succ_insn = ctx->base.pc_next + 4;
        gen_rvfi_dii_set_field_const_i64(INST, insn, opcode32);

        for (size_t i = 0; i < ARRAY_SIZE(decoders); ++i) {
            if (decoders[i].guard_func(ctx) &&
                decoders[i].decode_func(ctx, opcode32)) {
                return;
            }
        }
    }

    gen_exception_illegal(ctx);
}

static void riscv_tr_init_disas_context(DisasContextBase *dcbase, CPUState *cs)
{
    DisasContext *ctx = container_of(dcbase, DisasContext, base);
    CPURISCVState *env = cs->env_ptr;
    RISCVCPU *cpu = RISCV_CPU(cs);
    uint32_t tb_flags = ctx->base.tb->flags;

    ctx->pc_succ_insn = ctx->base.pc_first;
    ctx->mem_idx = FIELD_EX32(tb_flags, TB_FLAGS, MEM_IDX);
    ctx->mstatus_fs = tb_flags & TB_FLAGS_MSTATUS_FS;
    ctx->mstatus_vs = tb_flags & TB_FLAGS_MSTATUS_VS;
#ifdef TARGET_CHERI
    ctx->capmode = tb_in_capmode(ctx->base.tb);
#ifdef TARGET_CHERI_RISCV_V9
    ctx->cheri_v9_semantics = cpu->cfg.ext_cheri_v9;
#endif
    ctx->hybrid = riscv_feature(env, RISCV_FEATURE_CHERI_HYBRID);
    ctx->cre = riscv_cpu_mode_cre(env);
#endif
    ctx->priv_ver = env->priv_ver;
#if !defined(CONFIG_USER_ONLY)
    if (riscv_has_ext(env, RVH)) {
        ctx->virt_enabled = riscv_cpu_virt_enabled(env);
    } else {
        ctx->virt_enabled = false;
    }
#else
    ctx->virt_enabled = false;
#endif
    ctx->misa_ext = env->misa_ext;
    ctx->frm = -1;  /* unknown rounding mode */
    ctx->cfg_ptr = &(cpu->cfg);
    ctx->mstatus_hs_fs = FIELD_EX32(tb_flags, TB_FLAGS, MSTATUS_HS_FS);
    ctx->mstatus_hs_vs = FIELD_EX32(tb_flags, TB_FLAGS, MSTATUS_HS_VS);
    ctx->hlsx = FIELD_EX32(tb_flags, TB_FLAGS, HLSX);
    ctx->vill = FIELD_EX32(tb_flags, TB_FLAGS, VILL);
    ctx->sew = FIELD_EX32(tb_flags, TB_FLAGS, SEW);
    ctx->lmul = sextract32(FIELD_EX32(tb_flags, TB_FLAGS, LMUL), 0, 3);
    ctx->vstart = env->vstart;
    ctx->vl_eq_vlmax = FIELD_EX32(tb_flags, TB_FLAGS, VL_EQ_VLMAX);
    ctx->misa_mxl_max = env->misa_mxl_max;
    ctx->xl = FIELD_EX32(tb_flags, TB_FLAGS, XL);
    ctx->cs = cs;
    ctx->ntemp = 0;
    memset(ctx->temp, 0, sizeof(ctx->temp));
    ctx->nftemp = 0;
    memset(ctx->ftemp, 0, sizeof(ctx->ftemp));
    ctx->pm_mask_enabled = FIELD_EX32(tb_flags, TB_FLAGS, PM_MASK_ENABLED);
    ctx->pm_base_enabled = FIELD_EX32(tb_flags, TB_FLAGS, PM_BASE_ENABLED);
    ctx->zero = tcg_constant_tl(0);
}

static void riscv_tr_tb_start(DisasContextBase *db, CPUState *cpu)
{
}

static void riscv_tr_insn_start(DisasContextBase *dcbase, CPUState *cpu)
{
    DisasContext *ctx = container_of(dcbase, DisasContext, base);

    tcg_gen_insn_start(ctx->base.pc_next);
}

static void riscv_tr_translate_insn(DisasContextBase *dcbase, CPUState *cpu)
{
    DisasContext *ctx = container_of(dcbase, DisasContext, base);
    CPURISCVState *env = cpu->env_ptr;
#ifdef CONFIG_RVFI_DII
    // We have to avoid memory accesses for injected instructions since
    // the PC could point somewhere invalid.
    uint16_t opcode16 = env->rvfi_dii_have_injected_insn
        ? env->rvfi_dii_injected_insn
        : translator_lduw(env, &ctx->base, ctx->base.pc_next);
    gen_rvfi_dii_set_field_const_i64(PC, pc_rdata, ctx->base.pc_next);
#else
    uint16_t opcode16 = translator_lduw(env, &ctx->base, ctx->base.pc_next);
#endif
    int i;

    ctx->ol = ctx->xl;
    decode_opc(env, ctx, opcode16);
    ctx->base.pc_next = ctx->pc_succ_insn;
    gen_rvfi_dii_set_field_const_i64(PC, pc_wdata, ctx->base.pc_next);

    for (i = ctx->ntemp - 1; i >= 0; --i) {
        tcg_temp_free(ctx->temp[i]);
        ctx->temp[i] = NULL;
    }
    ctx->ntemp = 0;
    for (i = ctx->nftemp - 1; i >= 0; --i) {
        tcg_temp_free_i64(ctx->ftemp[i]);
        ctx->ftemp[i] = NULL;
    }
    ctx->nftemp = 0;

    if (ctx->base.is_jmp == DISAS_NEXT) {
        target_ulong page_start;

        page_start = ctx->base.pc_first & TARGET_PAGE_MASK;
        if (ctx->base.pc_next - page_start >= TARGET_PAGE_SIZE) {
            ctx->base.is_jmp = DISAS_TOO_MANY;
        }
    }
}

static void riscv_tr_tb_stop(DisasContextBase *dcbase, CPUState *cpu)
{
    DisasContext *ctx = container_of(dcbase, DisasContext, base);

    switch (ctx->base.is_jmp) {
    case DISAS_TOO_MANY:
        /* CHERI PCC bounds check done on next ifetch. */
        gen_goto_tb(ctx, 0, ctx->base.pc_next, /*bounds_check=*/false);
        break;
    case DISAS_NORETURN:
        break;
    default:
        g_assert_not_reached();
    }
}

static void riscv_tr_disas_log(const DisasContextBase *dcbase, CPUState *cpu)
{
#ifndef CONFIG_USER_ONLY
    RISCVCPU *rvcpu = RISCV_CPU(cpu);
    CPURISCVState *env = &rvcpu->env;
#endif

#ifdef CONFIG_RVFI_DII
    if (env->rvfi_dii_have_injected_insn) {
        assert(dcbase->num_insns == 1);
        FILE *logfile = qemu_log_lock();
        uint32_t insn = env->rvfi_dii_injected_insn;
        if (logfile) {
            fprintf(logfile, "IN: %s\n", lookup_symbol(dcbase->pc_first));
            target_disas_buf(stderr, cpu, &insn, sizeof(insn), dcbase->pc_first, 1);
        }
        qemu_log_unlock(logfile);
    }
#else
    qemu_log("IN: %s\n", lookup_symbol(dcbase->pc_first));
#ifndef CONFIG_USER_ONLY
    qemu_log("Priv: "TARGET_FMT_ld"; Virt: "TARGET_FMT_ld"\n", env->priv, env->virt);
#endif
    log_target_disas(cpu, dcbase->pc_first, dcbase->tb->size);
#endif
}

static const TranslatorOps riscv_tr_ops = {
    .init_disas_context = riscv_tr_init_disas_context,
    .tb_start           = riscv_tr_tb_start,
    .insn_start         = riscv_tr_insn_start,
    .translate_insn     = riscv_tr_translate_insn,
    .tb_stop            = riscv_tr_tb_stop,
    .disas_log          = riscv_tr_disas_log,
};

void gen_intermediate_code(CPUState *cs, TranslationBlock *tb, int max_insns)
{
    DisasContext ctx;

    translator_loop(&riscv_tr_ops, &ctx.base, cs, tb, max_insns);
}

void riscv_translate_init(void)
{
    int i;

#ifndef TARGET_CHERI
    /*
     * cpu_gpr[0] is a placeholder for the zero register. Do not use it.
     * Use the gen_set_gpr and get_gpr helper functions when accessing regs,
     * unless you specifically block reads/writes to reg 0.
     */
    cpu_gpr[0] = NULL;

    for (i = 1; i < 32; i++) {
        cpu_gpr[i] = tcg_global_mem_new(cpu_env,
            offsetof(CPURISCVState, gpr[i]), riscv_int_regnames[i]);
        cpu_gprh[i] = tcg_global_mem_new(cpu_env,
            offsetof(CPURISCVState, gprh[i]), riscv_int_regnamesh[i]);
    }
#else
    /* CNULL cursor should never be written! */
    _cpu_cursors_do_not_access_directly[0] = NULL;
    /*
     * Provide fast access to integer part of capability registers using
     * gen_get_gpr() and get_set_gpr(). But don't expose the cpu_gprs TCGv
     * directly to avoid errors.
     */
    for (i = 1; i < 32; i++) {
        _cpu_cursors_do_not_access_directly[i] = tcg_global_mem_new(
            cpu_env,
            offsetof(CPURISCVState, gpcapregs.decompressed[i].cap._cr_cursor),
            riscv_int_regnames[i]);
    }
#endif
#ifdef CONFIG_RVFI_DII
    cpu_rvfi_available_fields = tcg_global_mem_new_i32(
        cpu_env, offsetof(CPURISCVState, rvfi_dii_trace.available_fields),
        "rvfi_available_fields");
#endif

    for (i = 0; i < 32; i++) {
        cpu_fpr[i] = tcg_global_mem_new_i64(cpu_env,
            offsetof(CPURISCVState, fpr[i]), riscv_fpr_regnames[i]);
    }

#ifdef TARGET_CHERI
    cpu_pc = tcg_global_mem_new(cpu_env,
                                offsetof(CPURISCVState, pcc._cr_cursor), "pc");
    /// XXXAR: We currently interpose using DDC.cursor and not DDC.base!
    ddc_interposition = tcg_global_mem_new(
        cpu_env, offsetof(CPURISCVState, ddc._cr_cursor), "ddc_interpose");
#else
    cpu_pc = tcg_global_mem_new(cpu_env, offsetof(CPURISCVState, pc), "pc");
#endif
    cpu_vl = tcg_global_mem_new(cpu_env, offsetof(CPURISCVState, vl), "vl");
    cpu_vstart = tcg_global_mem_new(cpu_env, offsetof(CPURISCVState, vstart),
                            "vstart");
#ifdef CONFIG_DEBUG_TCG
    _pc_is_current = tcg_global_mem_new(
        cpu_env, offsetof(CPURISCVState, _pc_is_current), "_pc_is_current");
#endif
    load_res = (TCGv_cap_checked_ptr)tcg_global_mem_new(
        cpu_env, offsetof(CPURISCVState, load_res), "load_res");
    load_val = tcg_global_mem_new(cpu_env, offsetof(CPURISCVState, load_val),
                             "load_val");
    /* Assign PM CSRs to tcg globals */
    pm_mask = tcg_global_mem_new(cpu_env, offsetof(CPURISCVState, cur_pmmask),
                                 "pmmask");
    pm_base = tcg_global_mem_new(cpu_env, offsetof(CPURISCVState, cur_pmbase),
                                 "pmbase");
}

void gen_cheri_break_loadlink(TCGv_cap_checked_ptr out_addr)
{
    // The SC implementation uses load_res directly, and apparently this helper
    // can be called from inside the addr==load_res check and the cmpxchg being
    // executed.
    // Until this is fixed, comment out the invalidation
    // tcg_gen_movi_tl((TCGv)load_res, -1);
}
