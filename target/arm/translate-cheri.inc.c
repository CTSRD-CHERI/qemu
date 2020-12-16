/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2020 Lawrence Esswood
 *
 * This work was supported by Innovate UK project 105694, "Digital Security
 * by Design (DSbD) Technology Platform Prototype".
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

// Included from translate-a64.c

#include "decode-cheri.c.inc"

// LETODO: I may have not paid enough care to whether c31 is 0 / csp here.
// For X registers, calling the _sp variant gives sp rather than 0
// For C registers, register NULL_CAPREG_INDEX (32, use AS_ZERO) gives a null
// cap in helpers.

// For the most part, new morello instructions seem to use the variant where 31
// is SP, and X regs varies a lot

#define SE_64(Val, Width) (uint64_t) sextract64(Val, 0, Width)
#define ONES(X) ((1ULL << (X)) - 1)

#define AS_ZERO(X) ((X) == 32 ? 33 : (X))

// Get a cctlr bit cached in CHERI flags.
static inline bool cctlr_set(DisasContext *ctx, uint32_t bits)
{
    uint32_t mask =
        bits << (TB_FLAG_CHERI_SPARE_INDEX_START - CCTLR_DEFINED_START);
    return (ctx->base.cheri_flags & mask) == mask;
}

#define TRANS_F(NAME) static bool trans_##NAME(DisasContext *ctx, arg_##NAME *a)

// Translate this instruction where it corresponds to a generic CHERI one (name
// may be different)
#define TRANSLATE_GENERIC_BASE(MORELLO_NAME, CHERI_NAME, HELPER, ...)          \
    TRANS_F(MORELLO_NAME)                                                      \
    {                                                                          \
        return HELPER(__VA_ARGS__, &gen_helper_##CHERI_NAME);                  \
    }

typedef void(cheri_cget_helper)(TCGv, TCGv_env, TCGv_i32);
static inline bool gen_cheri_get(DisasContext *ctx, int rd, int cn,
                                 cheri_cget_helper *gen_func)
{
    TCGv_i32 source_regnum = tcg_const_i32(cn);
    TCGv result = cpu_reg(ctx, rd);
    gen_func(result, cpu_env, source_regnum);
    tcg_temp_free_i32(source_regnum);
    return true;
}

typedef void(cheri_int_cap_cap_helper)(TCGv, TCGv_env, TCGv_i32, TCGv_i32);
static inline bool gen_cheri_int_cap_cap(DisasContext *ctx, int rd, int cn,
                                         int cm,
                                         cheri_int_cap_cap_helper *gen_func)
{
    TCGv_i32 source_regnum = tcg_const_i32(cn);
    TCGv_i32 source_regnum2 = tcg_const_i32(cm);
    TCGv result = cpu_reg(ctx, rd);
    gen_func(result, cpu_env, source_regnum, source_regnum2);
    tcg_temp_free_i32(source_regnum);
    tcg_temp_free_i32(source_regnum2);
    return true;
}

typedef void(cheri_cap_cap_helper)(TCGv_env, TCGv_i32, TCGv_i32);
static inline bool gen_cheri_cap_cap(int cd, int cn,
                                     cheri_cap_cap_helper *gen_func)
{
    TCGv_i32 dest_regnum = tcg_const_i32(cd);
    TCGv_i32 source_regnum = tcg_const_i32(cn);
    gen_func(cpu_env, dest_regnum, source_regnum);
    tcg_temp_free_i32(source_regnum);
    tcg_temp_free_i32(dest_regnum);
    return true;
}

typedef void(cheri_cap_int_helper)(TCGv_env, TCGv_i32, TCGv);
static inline bool gen_cheri_cap_int(DisasContext *ctx, int cd, int rs,
                                     cheri_cap_int_helper *gen_func)
{
    TCGv_i32 dest_regnum = tcg_const_i32(cd);
    TCGv gpr_value = read_cpu_reg(ctx, rs, 1);
    gen_func(cpu_env, dest_regnum, gpr_value);
    tcg_temp_free_i32(dest_regnum);
    return true;
}

typedef void(cheri_cap_cap_imm_helper)(TCGv_env, TCGv_i32, TCGv_i32, TCGv);
static inline bool gen_cheri_cap_cap_imm(int cd, int cn, target_long imm,
                                         cheri_cap_cap_imm_helper *gen_func)
{
    TCGv_i32 dest_regnum = tcg_const_i32(cd);
    TCGv_i32 source_regnum = tcg_const_i32(cn);
    TCGv imm_value = tcg_const_tl(imm);
    gen_func(cpu_env, dest_regnum, source_regnum, imm_value);
    tcg_temp_free(imm_value);
    tcg_temp_free_i32(source_regnum);
    tcg_temp_free_i32(dest_regnum);
    return true;
}

typedef void(cheri_cap_cap_cap_helper)(TCGv_env, TCGv_i32, TCGv_i32, TCGv_i32);
static inline bool gen_cheri_cap_cap_cap(int cd, int cn, int cm,
                                         cheri_cap_cap_cap_helper *gen_func)
{
    TCGv_i32 dest_regnum = tcg_const_i32(cd);
    TCGv_i32 source_regnum1 = tcg_const_i32(cm);
    TCGv_i32 source_regnum2 = tcg_const_i32(cm);
    gen_func(cpu_env, dest_regnum, source_regnum1, source_regnum2);
    tcg_temp_free_i32(source_regnum2);
    tcg_temp_free_i32(source_regnum1);
    tcg_temp_free_i32(dest_regnum);
    return true;
}

typedef void(cheri_cap_cap_int_helper)(TCGv_env, TCGv_i32, TCGv_i32, TCGv);
static inline bool gen_cheri_cap_cap_int_imm(DisasContext *ctx, int cd, int cn,
                                             int rm, target_long imm,
                                             cheri_cap_cap_int_helper *gen_func)
{
    TCGv_i32 dest_regnum = tcg_const_i32(cd);
    TCGv_i32 source_regnum = tcg_const_i32(cn);
    TCGv gpr_value = read_cpu_reg(ctx, rm, 1);
    if (imm != 0) {
        tcg_gen_addi_tl(gpr_value, gpr_value, imm);
    }
    gen_func(cpu_env, dest_regnum, source_regnum, gpr_value);
    tcg_temp_free_i32(source_regnum);
    tcg_temp_free_i32(dest_regnum);
    return true;
}
static inline bool gen_cheri_cap_cap_int(DisasContext *ctx, int cd, int cn,
                                         int rm,
                                         cheri_cap_cap_int_helper *gen_func)
{
    return gen_cheri_cap_cap_int_imm(ctx, cd, cn, rm, 0, gen_func);
}

typedef void(gen_cheri_cap_cap_eint_helper)(TCGv_env, TCGv_i32, TCGv_i32, TCGv);
static inline bool
gen_cheri_cap_cap_eint(DisasContext *ctx, int cd, int cn, int rm, int shift,
                       int option, gen_cheri_cap_cap_eint_helper *gen_func)
{
    TCGv_i32 dest_regnum = tcg_const_i32(cd);
    TCGv_i32 source_regnum = tcg_const_i32(cn);

    TCGv_i64 extended = read_cpu_reg(ctx, rm, 1);
    ext_and_shift_reg(extended, extended, option, shift);

    gen_func(cpu_env, dest_regnum, source_regnum, extended);

    tcg_temp_free_i32(source_regnum);
    tcg_temp_free_i32(dest_regnum);

    return true;
}

enum ADDR_OPS {
    OP_ALIGN_DOWN = 0,
    OP_OR = 1,
    OP_EOR = 2,
    OP_ALIGN_UP,
    OP_SET,
};

#define OP_AND_NOT OP_ALIGN_DOWN

static inline bool gen_cheri_addr_op_imm(DisasContext *ctx, int cd, int cn,
                                         uint64_t val, enum ADDR_OPS op)
{
    TCGv_i32 dest_regnum = tcg_const_i32(cd);
    TCGv_i32 source_regnum = tcg_const_i32(cn);
    TCGv_i64 result = read_cpu_reg_sp(ctx, cn, 1);

    switch (op) {
    case OP_OR:
        tcg_gen_ori_i64(result, result, val);
        break;
    case OP_EOR:
        tcg_gen_xori_i64(result, result, val);
        break;
    case OP_ALIGN_UP:
        tcg_gen_addi_i64(result, result, val);
    case OP_ALIGN_DOWN:
        tcg_gen_andi_i64(result, result, ~val);
        break;
    default:
        assert(0);
    }

    gen_helper_csetaddr(cpu_env, dest_regnum, source_regnum, result);

    tcg_temp_free_i32(source_regnum);
    tcg_temp_free_i32(dest_regnum);

    return true;
}

static inline bool gen_cheri_addr_op_tcgval(DisasContext *ctx, int cd, int cn,
                                            TCGv_i64 val, enum ADDR_OPS op)
{
    TCGv_i32 dest_regnum = tcg_const_i32(cd);
    TCGv_i32 source_regnum = tcg_const_i32(cn);
    TCGv_i64 result = read_cpu_reg_sp(ctx, cn, 1);

    switch (op) {
    case OP_SET:
        tcg_gen_andi_i64(result, result, 0xFULL << 56);
    case OP_OR:
        tcg_gen_or_i64(result, result, val);
        break;
    case OP_EOR:
        tcg_gen_xor_i64(result, result, val);
        break;
    case OP_ALIGN_UP:
        tcg_gen_add_i64(result, result, val);
    case OP_ALIGN_DOWN:
        tcg_gen_not_i64(val, val);
        tcg_gen_and_i64(result, result, val);
        break;
    default:
        assert(0);
    }

    gen_helper_csetaddr(cpu_env, dest_regnum, source_regnum, result);

    tcg_temp_free_i32(source_regnum);
    tcg_temp_free_i32(dest_regnum);

    return true;
}

#define TRANSLATE_UNDEF(MORELLO_NAME)                                          \
    TRANS_F(MORELLO_NAME)                                                      \
    {                                                                          \
        assert(0 && #MORELLO_NAME " not implemented");                         \
    }

#define TRANSLATE_REDUNDANT(MORELLO_NAME)                                      \
    TRANS_F(MORELLO_NAME)                                                      \
    {                                                                          \
        return false;                                                          \
    }

#define TRANSLATE_GENERIC_CAP_CAP_INT(MORELLO_NAME, CHERI_NAME)                \
    TRANSLATE_GENERIC_BASE(MORELLO_NAME, CHERI_NAME,                           \
                           gen_cheri_cap_cap_int_imm, ctx, a->Cd, a->Cn,       \
                           a->Rm, 0)

#define TRANSLATE_GENERIC_CAP_CAP_CAP(MORELLO_NAME, CHERI_NAME)                \
    TRANSLATE_GENERIC_BASE(MORELLO_NAME, CHERI_NAME, gen_cheri_cap_cap_cap,    \
                           a->Cd, a->Cn, a->Cm)

#define TRANSLATE_GENERIC_CAP_CAP(MORELLO_NAME, CHERI_NAME)                    \
    TRANSLATE_GENERIC_BASE(MORELLO_NAME, CHERI_NAME, gen_cheri_cap_cap, a->Cd, \
                           a->Cn)

#define TRANSLATE_GENERIC_CAP_CAP_EINT(MORELLO_NAME, CHERI_NAME)               \
    TRANSLATE_GENERIC_BASE(MORELLO_NAME, CHERI_NAME, gen_cheri_cap_cap_eint,   \
                           ctx, a->Cd, a->Cn, a->Rm, a->imm3, a->option_name)

#define TRANSLATE_CGET(MORELLO_NAME, CHERI_NAME)                               \
    TRANSLATE_GENERIC_BASE(MORELLO_NAME, CHERI_NAME, gen_cheri_get, ctx,       \
                           a->Rd, a->Cn)

#define TRANSLATE_ADDR_OP_IM(MORELLO_NAME, VAL, OP)                            \
    TRANS_F(MORELLO_NAME)                                                      \
    {                                                                          \
        return gen_cheri_addr_op_imm(ctx, a->Cd, a->Cn, VAL, OP);              \
    }

#define TRANSLATE_FLAG_OP_IMM(MORELLO_NAME, OP)                                \
    TRANSLATE_ADDR_OP_IM(MORELLO_NAME, (uint64_t)a->imm8 << 56, OP)

#define TRANSLATE_FLAG_OP_REG(MORELLO_NAME, OP)                                \
    TRANS_F(MORELLO_NAME)                                                      \
    {                                                                          \
        TCGv_i64 mask = read_cpu_reg(ctx, a->Rm, 1);                           \
        tcg_gen_andi_i64(mask, mask, 0xFULL << 56);                            \
        return gen_cheri_addr_op_tcgval(ctx, a->Cd, a->Cn, mask, OP);          \
    }

#define REG_NONE 0x77
#define OPTION_NONE 0xff

// Load/store common code
// loads/stores pair rd/rd2, base rn, reg offset rm (or rs in some cases), imm
// offset imm pre_inc / post_inc stores back to base. Most of these options
// should optimise away.
static inline __attribute__((always_inline)) bool load_store_implementation(
    DisasContext *ctx, bool is_load, bool vector, uint32_t rd, uint32_t rd2,
    uint8_t size, uint8_t extend_size, uint32_t rn, uint32_t rm,
    target_ulong imm, bool pre_inc, bool post_inc, bool non_temporal_hint,
    bool alternate_base, bool pcc_base, bool exclusive, bool acquire_release,
    int option, unsigned int shift)
{
    // base on capability rn, otherwise ddc (or maybe even pcc) with rn as
    // value/offset In C64 base is the cap provided, or DDC/PCC otherwise.
    // Alternate is the opposite.
    bool capability_base = !pcc_base && (IS_C64(ctx) != alternate_base);

    // Still got these to do
    // Exclusive is done by by remembering BOTH the value and address in a load
    // (cpu_exclusive*) These are checked again on store using an atomic cmpxchg
    assert(!exclusive);
    assert(!acquire_release);

    if (vector) {
        if (!fp_access_check(ctx)) {
            // What am I meant to do here?
            assert(0);
        }
    }

    if (rn == 31)
        gen_check_sp_alignment(ctx);

    TCGv_i64 addr;

    // Get integer component of base
    if (rn == REG_NONE) {
        // For PCC/DDC relative things there is no base rn
        addr = tcg_const_i64(imm);
    } else {
        // Things with a base (still unchecked)
        addr = read_cpu_reg_sp(ctx, rn, 1);
        if (imm != 0 && !post_inc) {
            tcg_gen_addi_i64(addr, addr, imm);
        }
    }

    // Now add register offset if any
    if (rm != REG_NONE) {
        TCGv_i64 tcg_rm = read_cpu_reg_sp(ctx, rm, 1);
        if (option != OPTION_NONE) {
            ext_and_shift_reg(tcg_rm, tcg_rm, option, shift);
        }
        tcg_gen_add_i64(addr, addr, tcg_rm);
    }

    // Just special casing this for now until we get better TCG handling for
    // caps
    if (size == 4 && !vector) {
        uint32_t base_reg = capability_base ? rn
                                            : (pcc_base ? CHERI_EXC_REGNUM_PCC
                                                        : CHERI_EXC_REGNUM_DDC);

        TCGv_i32 tcg_base_reg = tcg_const_i32(base_reg);
        TCGv_i32 tcg_rd = tcg_const_i32(rd);

        TCGv_i32 tcg_rd2 = NULL;

        if (rd2 != REG_NONE) {
            tcg_rd2 = tcg_const_i32(rd2);
        }

        if (!capability_base) {
            // might have to add base of ppc / ddc
            if (!pcc_base && cctlr_set(ctx, CCTLR_DDCBO)) {
                tcg_gen_add_i64(addr, addr, ddc_interposition);
            } else if (pcc_base && cctlr_set(ctx, CCTLR_PCCBO)) {
                // I think this value might be stale?
                assert(0); // Until I check if PC is updated after every
                           // instruction. Plus this is base offset anyway..
                tcg_gen_add_i64(addr, addr, cpu_pc);
            }
        }

        if (rd2 == REG_NONE) {
            (is_load ? gen_helper_load_cap_via_cap
                     : gen_helper_store_cap_via_cap)(cpu_env, tcg_rd,
                                                     tcg_base_reg, addr);
        } else {
            (is_load ? gen_helper_load_cap_pair_via_cap
                     : gen_helper_store_cap_pair_via_cap)(
                cpu_env, tcg_rd, tcg_rd2, tcg_base_reg, addr);
        }

        tcg_temp_free_i32(tcg_base_reg);
        tcg_temp_free_i32(tcg_rd);

        if (tcg_rd2 != NULL) {
            tcg_temp_free_i32(tcg_rd2);
        }

    } else {
        // Calculate src/dst registers and base
        assert(rd != REG_NONE);

        // Perform bounds checks and do load / stores
        int memidx = get_mem_index(ctx);

        TCGv_cap_checked_ptr checked = gen_mte_and_cheri_check1(
            ctx, addr, !is_load, false, (rd2 == REG_NONE) ? size : (size + 1),
            rn, alternate_base, !pcc_base);

        if (!vector) {
            MemOp memop = ctx->be_data + size;

            TCGv_i64 tcg_rd = cpu_reg_sp(ctx, rd);
            TCGv_i64 tcg_rd2;

            if (rd2 != REG_NONE) {
                tcg_rd2 = cpu_reg_sp(ctx, rd2);
            }

            // Do the actual load / stores
            TCGv_i64 load_tmp = tcg_rd;

            // If loading two things we have to not make any modifications until
            // the second has succeeded
            if (is_load && (rd2 != REG_NONE)) {
                load_tmp = tcg_temp_new_i64();
            }

            void (*mem_op_fn)(TCGv_i64, TCGv_cap_checked_ptr, TCGArg, MemOp);

            mem_op_fn = is_load ? &tcg_gen_qemu_ld_i64_with_checked_addr
                                : &tcg_gen_qemu_st_i64_with_checked_addr;

            mem_op_fn(tcg_rd, checked, memidx, memop);
            gen_lazy_cap_set_int(ctx, rd);

            if (rd2 != REG_NONE) {
                // already checked, just coerce
                tcg_gen_addi_i64((TCGv_i64)checked, (TCGv_i64)checked,
                                 1 << size);
                mem_op_fn(tcg_rd2, checked, memidx, memop);
                gen_lazy_cap_set_int(ctx, rd2);
                if (is_load) {
                    // Now the second load has finished we can store the first
                    // one back
                    tcg_gen_mov_i64(tcg_rd, load_tmp);
                    tcg_temp_free_i64(load_tmp);
                }
            }

            // Sign extend
            if (extend_size > size) {
                assert(rd2 == REG_NONE);
                assert(is_load);
                assert(extend_size == 3 || extend_size == 2);
                switch (size) {
                case 0:
                    tcg_gen_ext8s_i64(tcg_rd, tcg_rd);
                    break;
                case 1:
                    tcg_gen_ext16s_i64(tcg_rd, tcg_rd);
                    break;
                case 2:
                    tcg_gen_ext32s_i64(tcg_rd, tcg_rd);
                    break;
                default:
                    assert(0);
                }
                if (extend_size == 2) {
                    tcg_gen_andi_i64(tcg_rd, tcg_rd, 0xFFFFFFFF);
                }
            }
        } else {
            if (is_load) {
                do_fp_ld(ctx, rd, checked, size);
            } else {
                do_fp_st(ctx, rd, checked, size);
            }
        }
    }

    // Add immediate afterwards if post incrementing
    if (post_inc && imm != 0) {
        tcg_gen_addi_i64(addr, addr, imm);
    }

    if (post_inc || pre_inc) {
        // The reason we dont just use the base reg directly is that we might
        // otherwise add DDC to it
        tcg_gen_mov_i64(cpu_reg_sp(ctx, rn), addr);
        incremented_register_base(ctx, rn, capability_base);
    }

    if (rn == REG_NONE) {
        tcg_temp_free_i64(addr);
    }

    return true;
}

// All the instructions

TRANSLATE_UNDEF(ADR)

TRANS_F(ADD_SUB)
{
    uint64_t imm = a->imm12;
    if (a->sh)
        imm <<= 12;
    if (a->A)
        imm = -imm;
    return gen_cheri_cap_cap_imm(a->Cd, a->Cn, imm, &gen_helper_cincoffset);
}

// This covers all the pairs
TRANS_F(LDP_STP)
{
    bool load = a->L;

    if (a->op1 == 0 && a->op2 != 0b11) {
        bool op = a->imm7 >> 6;
        uint32_t rs = (a->imm7 >> 1) & 0b11111;
        bool o2 = a->imm7 & 1;

        if (a->op2 == 1) {
            // Load / Store exclusive family
            // o2: acquire/release
            // CT all 1's when only single register
            // Rs (status) all 1's for loads
            bool pair = op;
            if (load)
                assert(rs == 0b11111);
            if (!pair)
                assert(a->Ct2 == 0b11111);
            return load_store_implementation(
                ctx, load, false, a->Ct, pair ? a->Ct2 : REG_NONE, 4, 4, a->Rn,
                rs, 0, false, false, false, false, false, true, o2, OPTION_NONE,
                0);
        } else if (a->op2 == 2) {
            // acquire/release
            assert(rs == 0b11111);
            assert(a->Ct2 == 0b11111);
            bool alternate_base = op || !o2;
            uint64_t size = op == 0 ? 4 : (o2 ? 2 : 0);
            return load_store_implementation(
                ctx, false, load, a->Ct, REG_NONE, size, size, a->Rn, REG_NONE,
                0, false, false, false, alternate_base, false, false, true,
                OPTION_NONE, 0);
        } else
            return false;
    } else if (a->op1 == 0b00 || a->op1 == 0b01) {
        // 01 01 post-index
        // 11 01 pre-index
        // 11 00 non-temporal
        // 10 01 signed

        bool non_temporal_hint = a->op1 == 0b00;
        if (non_temporal_hint)
            assert(a->op2 == 0b11);
        bool pre_inc = !non_temporal_hint && (a->op2 == 0b11);
        bool post_inc = !non_temporal_hint && (a->op2 == 0b01);
        uint64_t imm = SE_64((a->imm7 << 4), 11);
        return load_store_implementation(ctx, a->L, false, a->Ct, a->Ct2, 4, 4,
                                         a->Rn, REG_NONE, imm, pre_inc,
                                         post_inc, non_temporal_hint, false,
                                         false, false, false, OPTION_NONE, 0);
    } else
        return false;
}

TRANSLATE_GENERIC_CAP_CAP_EINT(ADD1, cincoffset)

// Load/Store unscaled immediate via alternate base
TRANS_F(ALD_ASTUR)
{
    int size, extend_size;
    bool is_load;

    if (!a->V) {
        size = ((a->op2 == 0b11) && (a->op1 >= 0b10)) ? 4 : a->op1;
        is_load = !((a->op2 == 0b00) || ((a->op2 == 0b11) && (a->op1 == 0b10)));
        extend_size = (a->op2 == 0b10) ? 3 : ((a->op2 == 0b11) ? 2 : 0);
        if (extend_size < size)
            extend_size = size;
    } else {
        is_load = a->op2 & 1;
        size = ((a->op1) == 0 && (a->op2 & 2)) ? 4 : a->op1;
        extend_size = size;
    }
    return load_store_implementation(ctx, is_load, a->V, a->Rt, REG_NONE, size,
                                     extend_size, a->Rn, REG_NONE,
                                     SE_64(a->imm9, 9), false, false, false,
                                     true, false, false, false, OPTION_NONE, 0);
}

TRANSLATE_ADDR_OP_IM(ALIGN, ONES(a->imm6), a->U ? OP_ALIGN_UP : OP_ALIGN_DOWN)

TRANS_F(FLGS)
{
    if (a->op == 0b11)
        return false;

    return gen_cheri_addr_op_imm(ctx, a->Cd, a->Cn, (uint64_t)a->imm8 << 56,
                                 (enum ADDR_OPS)a->op);
}

static bool cvt_impl_ptr_to_cap(DisasContext *ctx, uint32_t cd, uint32_t cn,
                                uint32_t rm, bool zero_is_null, bool pcc_bo)
{
    if (!zero_is_null)
        cn |= CFROMPTR_0_IS_NOT_NULL;
    if ((pcc_bo && !cctlr_set(ctx, CCTLR_PCCBO)) ||
        (!pcc_bo && !cctlr_set(ctx, CCTLR_DDCBO)))
        cn |= CFROMPTR_SET_ADDR;
    return gen_cheri_cap_cap_int(ctx, cd, cn, rm, &gen_helper_cfromptr);
}

static bool cvt_impl_cap_to_ptr(DisasContext *ctx, uint32_t rd, uint32_t cn,
                                uint32_t cm, bool pcc_bo)
{
    if ((pcc_bo && !cctlr_set(ctx, CCTLR_PCCBO)) ||
        (!pcc_bo && !cctlr_set(ctx, CCTLR_DDCBO)))
        cn |= CFROMPTR_SET_ADDR;
    return gen_cheri_int_cap_cap(ctx, rd, cn, cm, &gen_helper_ctoptr);
}

TRANS_F(CVT)
{
    return cvt_impl_ptr_to_cap(ctx, a->Cd, a->Cn, a->Rm, false, false);
}
TRANS_F(CVT1)
{
    return cvt_impl_ptr_to_cap(ctx, a->Cd, a->Cn, a->Rm, true, false);
}
TRANSLATE_UNDEF(SUBS);

TRANS_F(FLGS_CTHI)
{
    if (a->opc == 0b11) {
        assert(0);
    } else {
        TCGv_i64 mask = read_cpu_reg(ctx, a->Rm, 1);
        tcg_gen_andi_i64(mask, mask, 0xFULL << 56);
        return gen_cheri_addr_op_tcgval(ctx, a->Cd, a->Cn, mask, a->opc);
    }
}

static void set_flag(TCGv_i32 dest, TCGv_i32 source, int shift,
                     int default_value)
{
    if (source) {
        if (shift == 0) {
            tcg_gen_mov_i32(dest, source);
        } else {
            tcg_gen_shli_i32(dest, source, shift);
        }
    } else {
        tcg_gen_movi_i32(dest, default_value);
    }
}

static void set_NZCV(TCGv_i32 N, TCGv_i32 Z, TCGv_i32 C, TCGv_i32 V)
{
    if (Z) {
        tcg_gen_setcondi_i32(TCG_COND_EQ, Z, Z, 0);
    }
    set_flag(cpu_NF, N, 31, 0);
    set_flag(cpu_ZF, Z, 0, 1);
    set_flag(cpu_CF, C, 0, 0);
    set_flag(cpu_VF, V, 31, 0);
}

TRANS_F(BLR_BR_RET_CHKD)
{
    // op:
    //  00 is branch indirect
    //  01 are checks
    //  10 is branch sealed (slightly diffrent behavior if unsealed)
    //  11 is branch restricted (same behavior as sealed basically)
    // opc:
    //  00: BR
    //  01: BLR
    //  10: RET
    //  11: BX (requires CN = 0b11111, op == 0b00)

    if (a->op == 0b01) {
        TCGv_i32 tag = NULL;
        TCGv_i32 sealed = NULL;
        switch (a->opc) {
        case 0b00: // CHKSLD
            sealed = tcg_temp_new_i32();
            gen_cap_get_sealed_i32(ctx, a->opc, sealed);
            break;
        case 0b01: // CHKTGD
            tag = tcg_temp_new_i32();
            gen_cap_get_tag_i32(ctx, a->opc, tag);
            break;
        default:
            assert(0);
        }
        set_NZCV(NULL, NULL, tag, sealed);
        if (tag)
            tcg_temp_free_i32(tag);
        if (sealed)
            tcg_temp_free_i32(sealed);
        return true;
    }

    bool executive = !!GET_FLAG(ctx, EXECUTIVE);

    if (a->op == 0b11 && !executive) {
        unallocated_encoding(ctx);
    }

    if (a->opc == 0b11) {
        // BX is not _really_ a branch, just a mode switch.
        // Flip pstate.C64
        TCGv_i32 pstate = tcg_temp_new_i32();
        tcg_gen_ld_i32(pstate, cpu_env, offsetof(CPUARMState, pstate));
        tcg_gen_xori_i32(pstate, pstate, PSTATE_C64);
        tcg_gen_st_i32(pstate, cpu_env, offsetof(CPUARMState, pstate));
        // update hflags because we know that a bit will have changed
        TCGv_i32 tcg_el = tcg_const_i32(ctx->current_el);
        gen_helper_rebuild_hflags_a64(cpu_env, tcg_el);
        // Also exit the translation block
        ctx->base.is_jmp = DISAS_UPDATE_EXIT;
        tcg_temp_free_i32(tcg_el);
        tcg_temp_free_i32(pstate);

    } else {
        uint32_t source = AS_ZERO(a->Cn);
        // Sealed / restricted branches must branch to sentries if SBL set
        int sbl = cctlr_set(ctx, CCTLR_SBL);
        if (sbl && (a->op >= 0b10))
            source |= CJALR_MUST_BE_SENTRY;
        // Can only go into restricted mode if this branch is used
        if (a->op == 0b11)
            source |= CJALR_CAN_BRANCH_RESTRICTED;
        // Only make sentries in sbl mode
        if (!sbl)
            source |= CJALR_DONT_MAKE_SENTRY;

        TCGv_i32 target_regnum = tcg_const_i32(source);

        TCGv_i32 link_regnum =
            tcg_const_i32(a->opc == 0b01 ? 30 : NULL_CAPREG_INDEX);

        TCGv link_addr = tcg_const_tl(ctx->base.pc_next);
        TCGv cjalr_imm = tcg_const_tl(0);

        gen_helper_cjalr(cpu_env, link_regnum, target_regnum, cjalr_imm,
                         link_addr);

        tcg_temp_free(cjalr_imm);
        tcg_temp_free(link_addr);
        tcg_temp_free_i32(target_regnum);
        tcg_temp_free_i32(link_regnum);

        // LETODO: Check this.
        // Hopefully this jump will rebuild flags? Otherwise will have to make
        // it happen in cjalr.
        ctx->base.is_jmp = DISAS_JUMP;
    }

    return true;
}

TRANSLATE_UNDEF(BRS)
TRANS_F(CHK)
{
    int op1 = a->Cn;
    int op2 = AS_ZERO(a->Cm);
    switch (a->opc) {
    case 0b00: // CHKSS
        assert(0);
    case 0b01: // CHKEQ
    {
        TCGv_i32 eq = tcg_temp_new_i32();
        gen_cap_get_eq_i32(ctx, op1, op2, eq);
        set_NZCV(NULL, eq, NULL, NULL);
        tcg_temp_free_i32(eq);
    }
    default:
        return false;
    }
}

TRANSLATE_UNDEF(CAS)
// return gen_cheri_cap_cap_int_imm(ctx, a->Cd, a->Cn, a->Cm, 0,
// &gen_helper_csetaddr);
TRANS_F(BUILD_CSEAL_CPYE)
{
    assert(0);
    // maps to cbuildcap/cseal
}

TRANSLATE_UNDEF(CLRPERM)

static bool isTagSettingDisabled(DisasContext *ctx)
{
    // This flag is maps to the correct CSR for the current exception level
    return GET_FLAG(ctx, SETTAG) ? true : false;
}

// Set capability tag. Got weirdly named by script
TRANS_F(SCG)
{

    // First move to target
    gen_move_compressed_cap(gp_register_offset(a->Cd),
                            gp_register_offset(a->Cn));

    // Then set state
    if (!isTagSettingDisabled(ctx) && cheri_is_system_ctx(ctx)) {
        TCGv_i64 tcgrm = read_cpu_reg(ctx, a->Rm, 1);
        gen_cap_set_tag(ctx, a->Cd, tcgrm);
    } else {
        gen_lazy_cap_set_state(ctx, a->Cd, CREG_UNTAGGED_CAP);
    }

    return true;
}

TRANSLATE_UNDEF(SCG1)

TRANS_F(CLRTAG_CPY)
{
    // 10 is cpy, 00 is clr tag
    if (a->opc & 1)
        return false;

    return gen_cheri_cap_cap(
        a->Cd, a->Cn, (a->opc & 1) ? &gen_helper_cmove : gen_helper_ccleartag);
}
TRANSLATE_UNDEF(SEAL)
TRANSLATE_UNDEF(SEAL_CHKSSU)
TRANSLATE_UNDEF(CSEL)
TRANS_F(CFHI)
{
    TCGv_i64 dst = cpu_reg(ctx, a->Rd);

    switch (a->opc) {
    case 0b00: // GCLIM
        gen_cap_get_top(ctx, a->Cn, dst);
        return true;
    case 0b01: // GCFLGS
        gen_cap_get_flags(ctx, a->Cn, dst);
        return true;
    case 0b10: // CFHI
        gen_cap_get_hi(ctx, a->Cn, dst);
        return true;
    default:
        return false;
    }
}
// CVT with PCC / DDC as a base, cap -> ptr
TRANS_F(CVT2)
{
    bool pcc = (a->opc & 1) != 0;
    return cvt_impl_cap_to_ptr(
        ctx, a->Rd, a->Cn, pcc ? CHERI_EXC_REGNUM_PCC : CHERI_EXC_REGNUM_DDC,
        pcc);
}

TRANS_F(GC)
{
    int regnum = a->Cn;
    TCGv_i64 result = cpu_reg(ctx, a->Rd);

    switch (a->opc) {
    case 0b000: // base
        gen_cap_get_base(ctx, regnum, result);
        break;
    case 0b001: // len
        gen_cap_get_length(ctx, regnum, result);
        break;
    case 0b010: // value
        gen_cap_get_cursor(ctx, regnum, result);
        break;
    case 0b011: // off
        gen_cap_get_offset(ctx, regnum, result);
        break;
    case 0b100: // tag
        gen_cap_get_tag(ctx, regnum, result);
        break;
    case 0b101: // seal
        gen_cap_get_sealed(ctx, regnum, result);
        break;
    case 0b110: // perm
        gen_cap_get_perms(ctx, regnum, result);
        break;
    case 0b111: // type
        gen_cap_get_type(ctx, regnum, result);
        break;
    }

    gen_lazy_cap_set_int(ctx, a->Rd);

    return true;
}

TRANSLATE_UNDEF(LDPBR)

TRANS_F(LDR)
{
    return load_store_implementation(
        ctx, true, false, a->Ct, REG_NONE, 4, 4, REG_NONE, REG_NONE,
        SE_64(a->imm17 << 4, 21), false, false, false, false, true, false,
        false, OPTION_NONE, 0);
}

// op:  00 capsized
//      01 byte
//      10 word
//      11 dw
static uint64_t size_log(uint64_t op)
{
    return (op & 2) ? op : (op == 0 ? 4 : 0);
}

// imm9 with op for different sizes
TRANS_F(LDR_STR)
{
    // op:      01 post increment
    //          11 pre increment
    //          10 immediate translated
    //          00 unscaled immediate

    // opc:     0L

    bool pre = (a->op == 0b11);
    bool post = (a->op == 0b01);
    bool scale = (a->op != 0b00);

    assert((a->opc & 2) == 0);

    uint64_t imm = a->imm9;
    if (scale)
        imm <<= 4;
    imm = SE_64(imm, scale ? 13 : 9);

    return load_store_implementation(
        ctx, a->opc & 1, false, a->Ct, REG_NONE, 4, 4, a->Rn, REG_NONE, imm,
        pre, post, false, false, false, false, false, OPTION_NONE, 0);
}

// imm12
TRANS_F(LDR_STR1)
{
    return load_store_implementation(
        ctx, a->L, false, a->Ct, REG_NONE, 4, 4, a->Rn, REG_NONE, a->imm12 << 4,
        false, false, false, false, false, false, false, OPTION_NONE, 0);
}

TRANS_F(LDR_STR2)
{
    return load_store_implementation(ctx, a->opc == 0b01, false, a->Ct,
                                     REG_NONE, 16, 16, a->Rn, a->Rm, 0, false,
                                     false, false, false, false, false, false,
                                     a->option_name, a->S ? 4 : 0);
}

TRANS_F(MRS_MSR)
{
    handle_sys(ctx, 0x1 << 25, a->L, a->o0 + 2, a->op1, a->op2, a->CRn, a->CRm,
               AS_ZERO(a->Ct));
    return true;
}

TRANSLATE_UNDEF(RR)
TRANSLATE_UNDEF(SCBNDS)

TRANS_F(SC)
{
    cheri_cap_cap_int_helper *helper;
    if (a->opc == 0) {
        helper = &gen_helper_csetbounds;
    } else if (a->opc == 1) {
        helper = &gen_helper_csetboundsexact;
    } else if (a->opc == 2) {
        helper = &gen_helper_csetaddr;
    } else {
        helper = &gen_helper_csetoffset;
    }
    return gen_cheri_cap_cap_int(ctx, a->Cd, a->Cn, a->Rm, helper);
}

// CVT cap -> ptr
TRANS_F(CVT3)
{
    return cvt_impl_cap_to_ptr(ctx, a->Rd, a->Cn, a->Cm, false);
}

TRANSLATE_UNDEF(SWP)

// CVT with PCC / DDC as a base, ptr -> cap
TRANS_F(CVT4)
{
    bool pcc = (a->opc & 1) == 1;
    bool zero_is_null = (a->opc & 2) == 2;
    uint32_t reg = pcc ? CHERI_EXC_REGNUM_PCC : CHERI_EXC_REGNUM_DDC;
    return cvt_impl_ptr_to_cap(ctx, a->Cd, reg, a->Rn, zero_is_null, pcc);
}

TRANSLATE_UNDEF(CLRPERM1)
TRANSLATE_UNDEF(BR)
TRANSLATE_UNDEF(BLR1)

// Load store via alternate base
TRANS_F(ASTRB_ALD)
{
    uint64_t log = size_log(a->op);
    return load_store_implementation(ctx, a->L, false, a->Rt, REG_NONE, log,
                                     log, a->Rn, REG_NONE, a->imm9 << log,
                                     false, false, false, true, false, false,
                                     false, OPTION_NONE, 0);
}

// Load store via alternate base register offset
TRANS_F(ASTR_ALD)
{
    bool is_load = a->L || (!a->op && (a->opc == 0b01 || a->opc == 0b10));
    // op,opc   0 00 byte
    //          0 01 signed byte (extend to word if L, otherwise double)
    //          0 10 signed half (extend to word if L, otherwise double)
    //          0 11 half
    //          1 00 word
    //          1 01 doubleword
    //          1 10 simd&fp
    //          1 11 simd&fp

    size_t size;
    size_t extend_size;
    bool vector = false;

    switch (a->opc | (a->op << 2)) {
    case 0b000:
        size = 0;
        extend_size = 0;
        break;
    case 0b001:
        size = 0;
        extend_size = a->L ? 2 : 3;
        break;
    case 0b010:
        size = 1;
        extend_size = a->L ? 2 : 3;
        break;
    case 0b011:
        size = 1;
        extend_size = 1;
        break;
    case 0b100:
        size = 2;
        extend_size = 2;
        break;
    case 0b110:
        size = 3;
        extend_size = 3;
        vector = true;
        break;
    case 0b111:
        size = 2;
        extend_size = 2;
        vector = true;
    case 0b101:
        size = 3;
        extend_size = 3;
        break;
    default:
        assert(0);
    }

    return load_store_implementation(ctx, is_load, vector, a->Rt, REG_NONE,
                                     size, extend_size, a->Rn, a->Rm, 0, false,
                                     false, false, true, false, false, false,
                                     a->option_name, a->S ? size : 0);
}
