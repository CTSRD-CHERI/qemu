/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2015-2016 Stacey Son <sson@FreeBSD.org>
 * Copyright (c) 2016-2018 Alfredo Mazzinghi <am2419@cl.cam.ac.uk>
 * Copyright (c) 2016-2018 Alex Richardson <Alexander.Richardson@cl.cam.ac.uk>
 * All rights reserved.
 *
 * This software was developed by SRI International and the University of
 * Cambridge Computer Laboratory under DARPA/AFRL contract FA8750-10-C-0237
 * ("CTSRD"), as part of the DARPA CRASH research programme.
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
#if defined(CONFIG_MIPS_LOG_INSTR)
static inline void
generate_dump_load(int op, TCGv addr, TCGv value)
{
    TCGv_i32 top = tcg_const_i32(op);
    gen_helper_dump_load(cpu_env, top, addr, value);
    tcg_temp_free_i32(top);
}
#define GEN_CAP_DUMP_LOAD(op, addr, value) \
    generate_dump_load(op, addr, value)

static inline void
generate_dump_load32(int op, TCGv addr, TCGv_i32 value)
{
    TCGv_i32 top = tcg_const_i32(op);
    gen_helper_dump_load32(cpu_env, top, addr, value);
    tcg_temp_free_i32(top);
}
#define GEN_CAP_DUMP_LOAD32(op, addr, value) \
    generate_dump_load32(op, addr, value)

#else
#define GEN_CAP_DUMP_LOAD(op, addr, value)
#define GEN_CAP_DUMP_LOAD32(op, addr, value)
#define generate_dump_load(op, addr, value)
#define generate_dump_load32(op, addr, value)
#endif // CONFIG_MIPS_LOG_INSTR

#if defined(TARGET_CHERI)
/* Verify that the processor is running with CHERI instructions enabled. */
static inline void check_cop2x(DisasContext *ctx)
{

    if (unlikely(!(ctx->hflags & MIPS_HFLAG_COP2X))) {
        generate_exception_err(ctx, EXCP_CpU, 2);
    }
    // Pretty much every CHERI instruction can throw an exception so we need to
    // save the hflags if we are in a branch. If we don't do this we mess up
    // state on trap (e.g. the BDELAY flag will not be set in CP0_Cause)
    // It also causes BadInstrP to not be updated (and we depend on that in CheriBSD)
    if (unlikely(ctx->hflags & MIPS_HFLAG_BMASK))
        save_cpu_state(ctx, 0);
}

/* generic sign extension helper */
static inline int32_t sign_extend(int32_t x, int offset)
{
    int32_t const mask = 1U << (offset - 1);

    x = x & ((1U << offset) - 1);
    return (x ^ mask) - mask;
}

/*
static inline bool is_cop2x_enabled(DisasContext *ctx)
{

    return (likely(ctx->hflags & MIPS_HFLAG_COP2X));
}
*/

static inline void generate_ccall(int32_t cs, int32_t cb)
{
    /*
     * XXXAM can a ccall be in a delay slot?
     */
    TCGv_i32 tcs = tcg_const_i32(cs);
    TCGv_i32 tcb = tcg_const_i32(cb);

    gen_helper_ccall(cpu_env, tcs, tcb);

    tcg_temp_free_i32(tcb);
    tcg_temp_free_i32(tcs);
}

static inline void generate_ccall_notrap(DisasContext *ctx, int32_t cs, int32_t cb)
{
    /*
     * This version of ccall has a delay slot and also can not
     * be used in a delay slot
     */
    if (ctx->hflags & MIPS_HFLAG_BMASK) {
#ifdef MIPS_DEBUG_DISAS
        LOG_DISAS("Branch in delay / forbidden slot at PC 0x"
                  TARGET_FMT_lx "\n", ctx->base.pc_next);
#endif
        generate_exception(ctx, EXCP_RI);
    } else {
        TCGv_i32 tcs = tcg_const_i32(cs);
        TCGv_i32 tcb = tcg_const_i32(cb);

        gen_helper_ccall_notrap(btarget, cpu_env, tcs, tcb);
        /* Set ccall branch and delay slot flags */
        ctx->hflags |= (MIPS_HFLAG_BRCCALL);
        /* Save capability register index that is new PCC */
        // ctx->btcr = cs;
        save_cpu_state(ctx, 0);

        tcg_temp_free_i32(tcb);
        tcg_temp_free_i32(tcs);
    }
}

static inline void generate_candperm(int32_t cd, int32_t cb, int32_t rt)
{
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv_i32 tcd = tcg_const_i32(cd);
    TCGv t0 = tcg_temp_new();

    gen_load_gpr(t0, rt);
    gen_helper_candperm(cpu_env, tcd, tcb, t0);

    tcg_temp_free(t0);
    tcg_temp_free_i32(tcd);
    tcg_temp_free_i32(tcb);
}

static inline void gen_ccheck_conditional_branch(target_ulong btgt) {
    tcg_debug_assert(btgt != -1 && "btgt should have been set!");
    TCGLabel *skip_btarget_check = gen_new_label();
    // skip the check if bcond == 0
    tcg_gen_brcondi_tl(TCG_COND_EQ, bcond, 0, skip_btarget_check);
    tcg_gen_movi_tl(btarget, btgt);  // save btarget so that the helper can read it:
    gen_helper_ccheck_btarget(cpu_env);
    gen_set_label(skip_btarget_check); // skip helper call
}

typedef void (cheri_branch_helper)(TCGv, TCGv_ptr, TCGv_i32, TCGv_i32);
static inline void gen_compute_cheri_branch(DisasContext *ctx, int32_t cb,
                                         int32_t offset, cheri_branch_helper* gen_func)
{
    if (ctx->hflags & MIPS_HFLAG_BMASK) {
#ifdef MIPS_DEBUG_DISAS
        LOG_DISAS("Branch in delay / forbidden slot at PC 0x"
                TARGET_FMT_lx "\n", ctx->base.pc_next);
#endif
        generate_exception(ctx, EXCP_RI);
    } else {
        TCGv_i32 tcb = tcg_const_i32(cb);
        TCGv_i32 toffset = tcg_const_i32(offset);

        gen_func(bcond, cpu_env, tcb, toffset);
        ctx->btarget = ctx->base.pc_next + 4 * offset + 4;
        /* Set conditional branch and branch delay slot flags */
        ctx->hflags |= (MIPS_HFLAG_BC | MIPS_HFLAG_BDS32);

        // Check that the branch isn't out-of-bounds of PCC before executing the delay slot
        // Check that the conditional branch target is in range (but only if the branch is taken)
        gen_ccheck_conditional_branch(ctx->btarget);

        tcg_temp_free_i32(toffset);
        tcg_temp_free_i32(tcb);
    }
}

static inline void generate_cbez(DisasContext *ctx, int32_t cb, int32_t offset)
{
    gen_compute_cheri_branch(ctx, cb, offset, &gen_helper_cbez);
}

static inline void generate_cbnz(DisasContext *ctx, int32_t cb, int32_t offset)
{
    gen_compute_cheri_branch(ctx, cb, offset, &gen_helper_cbnz);
}

static inline void generate_cbts(DisasContext *ctx, int32_t cb, int32_t offset)
{
    gen_compute_cheri_branch(ctx, cb, offset, &gen_helper_cbts);
}

static inline void generate_cbtu(DisasContext *ctx, int32_t cb, int32_t offset)
{
    gen_compute_cheri_branch(ctx, cb, offset, &gen_helper_cbtu);
}

static inline void generate_cjalr(DisasContext *ctx, int32_t cd, int32_t cb)
{

    if (ctx->hflags & MIPS_HFLAG_BMASK) {
#ifdef MIPS_DEBUG_DISAS
        LOG_DISAS("Branch in delay / forbidden slot at PC 0x"
                TARGET_FMT_lx "\n", ctx->base.pc_next);
#endif
        generate_exception(ctx, EXCP_RI);
    } else {
        TCGv_i32 tcd = tcg_const_i32(cd);
        TCGv_i32 tcb = tcg_const_i32(cb);

        gen_helper_cjalr(btarget, cpu_env, tcd, tcb);
        /* Set branch and delay slot flags */
        ctx->hflags |= (MIPS_HFLAG_BRC | MIPS_HFLAG_BDS32);
        /* Save capability register index that is new PCC */
        // ctx->btcr = cb;
        save_cpu_state(ctx, 0);

        tcg_temp_free_i32(tcb);
        tcg_temp_free_i32(tcd);
    }
}

static inline void generate_cjr(DisasContext *ctx, int32_t cb)
{
    if (ctx->hflags & MIPS_HFLAG_BMASK) {
#ifdef MIPS_DEBUG_DISAS
        LOG_DISAS("Branch in delay / forbidden slot at PC 0x"
                TARGET_FMT_lx "\n", ctx->base.pc_next);
#endif
        generate_exception(ctx, EXCP_RI);
    } else {
        TCGv_i32 tcb = tcg_const_i32(cb);

        gen_helper_cjr(btarget, cpu_env, tcb);
        /* Set branch and delay slot flags */
        ctx->hflags |= (MIPS_HFLAG_BRC | MIPS_HFLAG_BDS32);
        /* Save capability register index that is new PCC */
        // ctx->btcr = cb;
        save_cpu_state(ctx, 0);

        tcg_temp_free_i32(tcb);
    }
}

static inline void generate_ccheckperm(int32_t cs, int32_t rt)
{
    TCGv_i32 tcs = tcg_const_i32(cs);
    TCGv t0 = tcg_temp_new();

    gen_load_gpr(t0, rt);
    gen_helper_ccheckperm(cpu_env, tcs, t0);

    tcg_temp_free(t0);
    tcg_temp_free_i32(tcs);
}

static inline void generate_cchecktype(int32_t cs, int32_t cb)
{
    TCGv_i32 tcs = tcg_const_i32(cs);
    TCGv_i32 tcb = tcg_const_i32(cb);

    gen_helper_cchecktype(cpu_env, tcs, tcb);
    tcg_temp_free_i32(tcb);
    tcg_temp_free_i32(tcs);
}

static inline void generate_ccleartag(int32_t cd, int32_t cb)
{
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv_i32 tcd = tcg_const_i32(cd);

    gen_helper_ccleartag(cpu_env, tcd, tcb);
    tcg_temp_free_i32(tcd);
    tcg_temp_free_i32(tcb);
}

static inline void generate_cfromptr(int32_t cd, int32_t cb, int32_t rt)
{
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv_i32 tcd = tcg_const_i32(cd);
    TCGv t0 = tcg_temp_new();

    gen_load_gpr(t0, rt);
    gen_helper_cfromptr(cpu_env, tcd, tcb, t0);

    tcg_temp_free(t0);
    tcg_temp_free_i32(tcd);
    tcg_temp_free_i32(tcb);
}

static inline void generate_cgetbase(int32_t rd, int32_t cb)
{
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv t0 = tcg_temp_new();

    gen_helper_cgetbase(t0, cpu_env, tcb);
    gen_store_gpr (t0, rd);

    tcg_temp_free(t0);
    tcg_temp_free_i32(tcb);
}

static inline void generate_cgetaddr(int32_t rd, int32_t cb)
{
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv t0 = tcg_temp_new();

    gen_helper_cgetaddr(t0, cpu_env, tcb);
    gen_store_gpr (t0, rd);

    tcg_temp_free(t0);
    tcg_temp_free_i32(tcb);
}

static inline void generate_cgetcause(int32_t rd)
{
    TCGv t0 = tcg_temp_new();

    gen_helper_cgetcause(t0, cpu_env);
    gen_store_gpr (t0, rd);

    tcg_temp_free(t0);
}

static inline void generate_cgetpcc(int32_t cd)
{
    TCGv_i32 tcd = tcg_const_i32(cd);

    gen_helper_cgetpcc(cpu_env, tcd);
    tcg_temp_free_i32(tcd);
}

static inline void generate_cgetpccsetoffset(int32_t cd, int32_t rs)
{
    TCGv_i32 tcd = tcg_const_i32(cd);
    TCGv t0 = tcg_temp_new();

    gen_load_gpr(t0, rs);
    gen_helper_cgetpccsetoffset(cpu_env, tcd, t0);
    tcg_temp_free(t0);
    tcg_temp_free_i32(tcd);
}

static inline void generate_cgetlen(int32_t rd, int32_t cb)
{
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv t0 = tcg_temp_new();

    gen_helper_cgetlen(t0, cpu_env, tcb);
    gen_store_gpr (t0, rd);

    tcg_temp_free(t0);
    tcg_temp_free_i32(tcb);
}

static inline void generate_cgetoffset(int32_t rd, int32_t cb)
{
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv t0 = tcg_temp_new();

    gen_helper_cgetoffset(t0, cpu_env, tcb);
    gen_store_gpr (t0, rd);

    tcg_temp_free(t0);
    tcg_temp_free_i32(tcb);
}

static inline void generate_cgetperm(int32_t rd, int32_t cb)
{
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv t0 = tcg_temp_new();

    gen_helper_cgetperm(t0, cpu_env, tcb);
    gen_store_gpr (t0, rd);

    tcg_temp_free(t0);
    tcg_temp_free_i32(tcb);
}

static inline void generate_cgetsealed(int32_t rd, int32_t cb)
{
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv t0 = tcg_temp_new();

    gen_helper_cgetsealed(t0, cpu_env, tcb);
    gen_store_gpr (t0, rd);

    tcg_temp_free(t0);
    tcg_temp_free_i32(tcb);
}

static inline void generate_cgettag(int32_t rd, int32_t cb)
{
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv t0 = tcg_temp_new();

    gen_helper_cgettag(t0, cpu_env, tcb);
    gen_store_gpr (t0, rd);

    tcg_temp_free(t0);
    tcg_temp_free_i32(tcb);
}

static inline void generate_cgettype(int32_t rd, int32_t cb)
{
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv t0 = tcg_temp_new();

    gen_helper_cgettype(t0, cpu_env, tcb);
    gen_store_gpr (t0, rd);

    tcg_temp_free(t0);
    tcg_temp_free_i32(tcb);
}

static inline void generate_cincbase(int32_t cd, int32_t cb, int32_t rt)
{
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv_i32 tcd = tcg_const_i32(cd);
    TCGv t0 = tcg_temp_new();

    gen_load_gpr(t0, rt);
    gen_helper_cincbase(cpu_env, tcd, tcb, t0);

    tcg_temp_free(t0);
    tcg_temp_free_i32(tcd);
    tcg_temp_free_i32(tcb);
}

static inline void generate_cincoffset(int32_t cd, int32_t cb, int32_t rt)
{
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv_i32 tcd = tcg_const_i32(cd);
    TCGv t0 = tcg_temp_new();

    gen_load_gpr(t0, rt);
    gen_helper_cincoffset(cpu_env, tcd, tcb, t0);

    tcg_temp_free(t0);
    tcg_temp_free_i32(tcd);
    tcg_temp_free_i32(tcb);
}

static inline void generate_cincoffset_imm(int32_t cd, int32_t cs, int32_t increment)
{
    TCGv_i32 tcd = tcg_const_i32(cd);
    TCGv_i32 tcs = tcg_const_i32(cs);
    TCGv t0 = tcg_temp_new();

    tcg_gen_movi_tl(t0, sign_extend(increment, 11));
    gen_helper_cincoffset(cpu_env, tcd, tcs, t0);

    tcg_temp_free(t0);
    tcg_temp_free_i32(tcd);
    tcg_temp_free_i32(tcs);
}

static inline void generate_cmove(int32_t cd, int32_t cs)
{
    TCGv_i32 tcd = tcg_const_i32(cd);
    TCGv_i32 tcs = tcg_const_i32(cs);
    TCGv t0 = tcg_const_tl(0);

    gen_helper_cmovz(cpu_env, tcd, tcs, t0);

    tcg_temp_free(t0);
    tcg_temp_free_i32(tcd);
    tcg_temp_free_i32(tcs);
}

static inline void generate_cmovz(int32_t cd, int32_t cs, int32_t rs)
{
    TCGv_i32 tcd = tcg_const_i32(cd);
    TCGv_i32 tcs = tcg_const_i32(cs);
    TCGv t0 = tcg_temp_new();

    gen_load_gpr(t0, rs);
    gen_helper_cmovz(cpu_env, tcd, tcs, t0);

    tcg_temp_free(t0);
    tcg_temp_free_i32(tcd);
    tcg_temp_free_i32(tcs);
}

static inline void generate_cmovn(int32_t cd, int32_t cs, int32_t rs)
{
    TCGv_i32 tcd = tcg_const_i32(cd);
    TCGv_i32 tcs = tcg_const_i32(cs);
    TCGv t0 = tcg_temp_new();

    gen_load_gpr(t0, rs);
    gen_helper_cmovn(cpu_env, tcd, tcs, t0);

    tcg_temp_free(t0);
    tcg_temp_free_i32(tcd);
    tcg_temp_free_i32(tcs);
}

static inline void generate_cbuildcap(int32_t cd, int32_t cb, int32_t ct)
{
    TCGv_i32 tcd = tcg_const_i32(cd);
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv_i32 tct = tcg_const_i32(ct);

    gen_helper_cbuildcap(cpu_env, tcd, tcb, tct);
    tcg_temp_free_i32(tcd);
    tcg_temp_free_i32(tcb);
    tcg_temp_free_i32(tct);
}

static inline void generate_ccseal(int32_t cd, int32_t cs, int32_t ct)
{
    TCGv_i32 tcd = tcg_const_i32(cd);
    TCGv_i32 tcs = tcg_const_i32(cs);
    TCGv_i32 tct = tcg_const_i32(ct);

    gen_helper_ccseal(cpu_env, tcd, tcs, tct);
    tcg_temp_free_i32(tcd);
    tcg_temp_free_i32(tcs);
    tcg_temp_free_i32(tct);
}

static inline void generate_ccopytype(int32_t cd, int32_t cb, int32_t ct)
{
    TCGv_i32 tcd = tcg_const_i32(cd);
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv_i32 tct = tcg_const_i32(ct);

    gen_helper_ccopytype(cpu_env, tcd, tcb, tct);
    tcg_temp_free_i32(tcd);
    tcg_temp_free_i32(tcb);
    tcg_temp_free_i32(tct);
}

static inline void generate_ctestsubset(int32_t rd, int32_t cb, int32_t ct)
{
    TCGv t0 = tcg_temp_new();
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv_i32 tct = tcg_const_i32(ct);

    gen_helper_ctestsubset(t0, cpu_env, tcb, tct);
    gen_store_gpr(t0, rd);

    tcg_temp_free(t0);
    tcg_temp_free_i32(tcb);
    tcg_temp_free_i32(tct);
}

static inline void generate_creturn(void)
{
    gen_helper_creturn(cpu_env);
}

static inline void generate_cseal(int32_t cd, int32_t cb, int32_t ct)
{
    TCGv_i32 tcd = tcg_const_i32(cd);
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv_i32 tct = tcg_const_i32(ct);

    gen_helper_cseal(cpu_env, tcd, tcb, tct);
    tcg_temp_free_i32(tct);
    tcg_temp_free_i32(tcb);
    tcg_temp_free_i32(tcd);
}

static inline void generate_csetbounds(int32_t cd, int32_t cb, int32_t rt)
{
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv_i32 tcd = tcg_const_i32(cd);
    TCGv t0 = tcg_temp_new();

    gen_load_gpr(t0, rt);
    gen_helper_csetbounds(cpu_env, tcd, tcb, t0);

    tcg_temp_free(t0);
    tcg_temp_free_i32(tcd);
    tcg_temp_free_i32(tcb);
}

static inline void generate_csetboundsexact(int32_t cd, int32_t cb, int32_t rt)
{
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv_i32 tcd = tcg_const_i32(cd);
    TCGv t0 = tcg_temp_new();

    gen_load_gpr(t0, rt);
    gen_helper_csetboundsexact(cpu_env, tcd, tcb, t0);

    tcg_temp_free(t0);
    tcg_temp_free_i32(tcd);
    tcg_temp_free_i32(tcb);
}

static inline void generate_csetbounds_imm(int32_t cd, int32_t cb, int32_t length)
{
    TCGv_i32 tcd = tcg_const_i32(cd);
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv t0 = tcg_temp_new();

    tcg_gen_movi_tl(t0, sign_extend(length, 11));
    gen_helper_csetbounds(cpu_env, tcd, tcb, t0);

    tcg_temp_free(t0);
    tcg_temp_free_i32(tcd);
    tcg_temp_free_i32(tcb);
}

static inline void generate_csub(int32_t rd, int32_t cb, int32_t ct)
{
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv_i32 tct = tcg_const_i32(ct);
    TCGv t0 = tcg_temp_new();

    gen_helper_csub(t0, cpu_env, tcb, tct);
    gen_store_gpr(t0, rd);

    tcg_temp_free(t0);
    tcg_temp_free_i32(tct);
    tcg_temp_free_i32(tcb);
}

static inline void generate_csetcause(int32_t rd)
{
    TCGv t0 = tcg_temp_new();

    gen_load_gpr(t0, rd);
    gen_helper_csetcause(cpu_env, t0);
    tcg_temp_free(t0);
}

static inline void generate_csetlen(int32_t cd, int32_t cb, int32_t rt)
{
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv_i32 tcd = tcg_const_i32(cd);
    TCGv t0 = tcg_temp_new();

    gen_load_gpr(t0, rt);
    gen_helper_csetlen(cpu_env, tcd, tcb, t0);

    tcg_temp_free(t0);
    tcg_temp_free_i32(tcd);
    tcg_temp_free_i32(tcb);
}

static inline void generate_csetoffset(int32_t cd, int32_t cb, int32_t rt)
{
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv_i32 tcd = tcg_const_i32(cd);
    TCGv t0 = tcg_temp_new();

    gen_load_gpr(t0, rt);
    gen_helper_csetoffset(cpu_env, tcd, tcb, t0);

    tcg_temp_free(t0);
    tcg_temp_free_i32(tcd);
    tcg_temp_free_i32(tcb);
}

static inline void generate_ctoptr(int32_t rd, int32_t cb, int32_t ct)
{
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv_i32 tct = tcg_const_i32(ct);
    TCGv t0 = tcg_temp_new();

    gen_helper_ctoptr(t0, cpu_env, tcb, tct);
    gen_store_gpr(t0, rd);

    tcg_temp_free(t0);
    tcg_temp_free_i32(tct);
    tcg_temp_free_i32(tcb);
}

static inline void generate_cunseal(int32_t cd, int32_t cb, int32_t ct)
{
    TCGv_i32 tcd = tcg_const_i32(cd);
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv_i32 tct = tcg_const_i32(ct);

    gen_helper_cunseal(cpu_env, tcd, tcb, tct);
    tcg_temp_free_i32(tct);
    tcg_temp_free_i32(tcb);
    tcg_temp_free_i32(tcd);
}

static inline int generate_cclearregs(DisasContext *ctx, int32_t regset, int32_t mask)
{
    int i;
    TCGv t0;
    TCGv_i32 tcr0;

    switch(regset) {
    case 0: /* ClearLO */
        if (!mask)
            return 0;
        t0 = tcg_temp_new();
        tcg_gen_movi_tl(t0, 0);
        mask = mask >> 1; /* Skip R0, the zero register */
        for(i = 1; i < 16; i++) {
            if (mask & 0x1)
                gen_store_gpr(t0, i);
            mask = mask >> 1;
        }
        tcg_temp_free(t0);
        break;
    case 1: /* ClearHi */
        if (!mask)
            return 0;
        t0 = tcg_temp_new();
        tcg_gen_movi_tl(t0, 0);
        for(i = 16; i < 32; i++) {
            if (mask & 0x1)
                gen_store_gpr(t0, i);
            mask = mask >> 1;
        }
        tcg_temp_free(t0);
        break;
    case 2: /* CClearLO */
        if (!mask)
            return 0;
        tcr0 = tcg_const_i32(mask);
        gen_helper_cclearreg(cpu_env, tcr0);
        tcg_temp_free_i32(tcr0);
        break;
    case 3: /* CClearHi */
        if (!mask)
            return 0;
        if ((ctx->hflags & MIPS_HFLAG_BMASK_BASE) == MIPS_HFLAG_BRCCALL &&
            (mask & 1 << (CP2CAP_IDC - 16))) {
            // custom check for idc being accessed in a ccall delay slot
            // XXXAM it may be cleaner to just make an helper that throws the exception.
            TCGv_i32 tmp = tcg_const_i32(CP2CAP_IDC);
            tcg_temp_free_i32(tmp);
        }
        tcr0 = tcg_const_i32(mask << 16);
        gen_helper_cclearreg(cpu_env, tcr0);
        tcg_temp_free_i32(tcr0);
        break;
    default:
        return 1; /* Invalid */
    }
    return 0;
}

static inline void generate_ceq(int32_t rd, int32_t cb, int32_t ct)
{
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv_i32 tct = tcg_const_i32(ct);
    TCGv t0 = tcg_temp_new();

    gen_helper_ceq(t0, cpu_env, tcb, tct);
    gen_store_gpr(t0, rd);

    tcg_temp_free(t0);
    tcg_temp_free_i32(tct);
    tcg_temp_free_i32(tcb);
}

static inline void generate_cne(int32_t rd, int32_t cb, int32_t ct)
{
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv_i32 tct = tcg_const_i32(ct);
    TCGv t0 = tcg_temp_new();

    gen_helper_cne(t0, cpu_env, tcb, tct);
    gen_store_gpr(t0, rd);

    tcg_temp_free(t0);
    tcg_temp_free_i32(tct);
    tcg_temp_free_i32(tcb);
}

static inline void generate_clt(int32_t rd, int32_t cb, int32_t ct)
{
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv_i32 tct = tcg_const_i32(ct);
    TCGv t0 = tcg_temp_new();

    gen_helper_clt(t0, cpu_env, tcb, tct);
    gen_store_gpr(t0, rd);

    tcg_temp_free(t0);
    tcg_temp_free_i32(tct);
    tcg_temp_free_i32(tcb);
}

static inline void generate_cle(int32_t rd, int32_t cb, int32_t ct)
{
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv_i32 tct = tcg_const_i32(ct);
    TCGv t0 = tcg_temp_new();

    gen_helper_cle(t0, cpu_env, tcb, tct);
    gen_store_gpr(t0, rd);

    tcg_temp_free(t0);
    tcg_temp_free_i32(tct);
    tcg_temp_free_i32(tcb);
}

static inline void generate_cltu(int32_t rd, int32_t cb, int32_t ct)
{
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv_i32 tct = tcg_const_i32(ct);
    TCGv t0 = tcg_temp_new();

    gen_helper_cltu(t0, cpu_env, tcb, tct);
    gen_store_gpr(t0, rd);

    tcg_temp_free(t0);
    tcg_temp_free_i32(tct);
    tcg_temp_free_i32(tcb);
}

static inline void generate_cleu(int32_t rd, int32_t cb, int32_t ct)
{
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv_i32 tct = tcg_const_i32(ct);
    TCGv t0 = tcg_temp_new();

    gen_helper_cleu(t0, cpu_env, tcb, tct);
    gen_store_gpr(t0, rd);

    tcg_temp_free(t0);
    tcg_temp_free_i32(tct);
    tcg_temp_free_i32(tcb);
}

static inline void generate_cexeq(int32_t rd, int32_t cb, int32_t ct)
{
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv_i32 tct = tcg_const_i32(ct);
    TCGv t0 = tcg_temp_new();

    gen_helper_cexeq(t0, cpu_env, tcb, tct);
    gen_store_gpr(t0, rd);

    tcg_temp_free(t0);
    tcg_temp_free_i32(tct);
    tcg_temp_free_i32(tcb);
}

static inline void generate_cnexeq(int32_t rd, int32_t cb, int32_t ct)
{
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv_i32 tct = tcg_const_i32(ct);
    TCGv t0 = tcg_temp_new();

    gen_helper_cnexeq(t0, cpu_env, tcb, tct);
    gen_store_gpr(t0, rd);

    tcg_temp_free(t0);
    tcg_temp_free_i32(tct);
    tcg_temp_free_i32(tcb);
}

static inline int32_t cload_sign_extend(int32_t x)
{
    int32_t const mask = 1U << (8 - 1);

    x = x & ((1U << 8) - 1);
    return (x ^ mask) - mask;
}

/* Load Via Capability Register */
static inline void generate_clbu(DisasContext *ctx, int32_t rd, int32_t cb,
        int32_t rt, int32_t offset)
{
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv_i32 toffset = tcg_const_i32(cload_sign_extend(offset));
    TCGv t0 = tcg_temp_new();
    TCGv t1 = tcg_temp_new();
    TCGv_i32 tlen = tcg_const_i32(1);

    gen_load_gpr(t1, rt);
    gen_helper_cload(t0, cpu_env, tcb, t1, toffset, tlen);
    tcg_gen_qemu_ld_tl(t1, t0, ctx->mem_idx, MO_UB);
    generate_dump_load(OPC_CLBU, t0, t1);
    gen_store_gpr(t1, rd);

    tcg_temp_free_i32(tlen);
    tcg_temp_free(t1);
    tcg_temp_free(t0);
    tcg_temp_free_i32(toffset);
    tcg_temp_free_i32(tcb);
}

static inline void generate_clhu(DisasContext *ctx, int32_t rd, int32_t cb,
        int32_t rt, int32_t offset)
{
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv_i32 toffset = tcg_const_i32(cload_sign_extend(offset) * 2);
    TCGv t0 = tcg_temp_new();
    TCGv t1 = tcg_temp_new();
    TCGv_i32 tlen = tcg_const_i32(2);

    gen_load_gpr(t1, rt);
    gen_helper_cload(t0, cpu_env, tcb, t1, toffset, tlen);
    tcg_gen_qemu_ld_tl(t1, t0, ctx->mem_idx, MO_TEUW |
            ctx->default_tcg_memop_mask);
    generate_dump_load(OPC_CLHU, t0, t1);
    gen_store_gpr(t1, rd);

    tcg_temp_free_i32(tlen);
    tcg_temp_free(t1);
    tcg_temp_free(t0);
    tcg_temp_free_i32(toffset);
    tcg_temp_free_i32(tcb);
}

static inline void generate_clwu(DisasContext *ctx, int32_t rd, int32_t cb,
        int32_t rt, int32_t offset)
{
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv_i32 toffset = tcg_const_i32(cload_sign_extend(offset) * 4);
    TCGv t0 = tcg_temp_new();
    TCGv t1 = tcg_temp_new();
    TCGv_i32 tlen = tcg_const_i32(4);

    gen_load_gpr(t1, rt);
    gen_helper_cload(t0, cpu_env, tcb, t1, toffset, tlen);
    tcg_gen_qemu_ld_tl(t1, t0, ctx->mem_idx, MO_TEUL |
            ctx->default_tcg_memop_mask);
    generate_dump_load(OPC_CLWU, t0, t1);
    gen_store_gpr(t1, rd);

    tcg_temp_free_i32(tlen);
    tcg_temp_free(t1);
    tcg_temp_free(t0);
    tcg_temp_free_i32(toffset);
    tcg_temp_free_i32(tcb);
}

static inline void generate_clb(DisasContext *ctx, int32_t rd, int32_t cb,
        int32_t rt, int32_t offset)
{
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv_i32 toffset = tcg_const_i32(cload_sign_extend(offset));
    TCGv t0 = tcg_temp_new();
    TCGv t1 = tcg_temp_new();
    TCGv_i32 tlen = tcg_const_i32(1);

    gen_load_gpr(t1, rt);
    gen_helper_cload(t0, cpu_env, tcb, t1, toffset, tlen);
    tcg_gen_qemu_ld_tl(t1, t0, ctx->mem_idx, MO_SB);
    generate_dump_load(OPC_CLB, t0, t1);
    gen_store_gpr(t1, rd);

    tcg_temp_free_i32(tlen);
    tcg_temp_free(t1);
    tcg_temp_free(t0);
    tcg_temp_free_i32(toffset);
    tcg_temp_free_i32(tcb);
}

static inline void generate_clh(DisasContext *ctx, int32_t rd, int32_t cb,
        int32_t rt, int32_t offset)
{
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv_i32 toffset = tcg_const_i32(cload_sign_extend(offset) * 2);
    TCGv t0 = tcg_temp_new();
    TCGv t1 = tcg_temp_new();
    TCGv_i32 tlen = tcg_const_i32(2);

    gen_load_gpr(t1, rt);
    gen_helper_cload(t0, cpu_env, tcb, t1, toffset, tlen);
    tcg_gen_qemu_ld_tl(t1, t0, ctx->mem_idx, MO_TESW |
            ctx->default_tcg_memop_mask);
    generate_dump_load(OPC_CLH, t0, t1);
    gen_store_gpr(t1, rd);

    tcg_temp_free_i32(tlen);
    tcg_temp_free(t1);
    tcg_temp_free(t0);
    tcg_temp_free_i32(toffset);
    tcg_temp_free_i32(tcb);
}

static inline void generate_clw(DisasContext *ctx, int32_t rd, int32_t cb,
        int32_t rt, int32_t offset)
{
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv_i32 toffset = tcg_const_i32(cload_sign_extend(offset) * 4);
    TCGv t0 = tcg_temp_new();
    TCGv t1 = tcg_temp_new();
    TCGv_i32 tlen = tcg_const_i32(4);

    gen_load_gpr(t1, rt);
    gen_helper_cload(t0, cpu_env, tcb, t1, toffset, tlen);
    tcg_gen_qemu_ld_tl(t1, t0, ctx->mem_idx, MO_TESL |
            ctx->default_tcg_memop_mask);
    generate_dump_load(OPC_CLW, t0, t1);
    gen_store_gpr(t1, rd);

    tcg_temp_free_i32(tlen);
    tcg_temp_free(t1);
    tcg_temp_free(t0);
    tcg_temp_free_i32(toffset);
    tcg_temp_free_i32(tcb);
}

static inline void generate_cld(DisasContext *ctx, int32_t rd, int32_t cb,
        int32_t rt, int32_t offset)
{
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv_i32 toffset = tcg_const_i32(cload_sign_extend(offset) * 8);
    TCGv t0 = tcg_temp_new();
    TCGv t1 = tcg_temp_new();
    TCGv_i32 tlen = tcg_const_i32(8);

    gen_load_gpr(t1, rt);
    gen_helper_cload(t0, cpu_env, tcb, t1, toffset, tlen);
    tcg_gen_qemu_ld_tl(t1, t0, ctx->mem_idx, MO_TEQ |
            ctx->default_tcg_memop_mask);
    generate_dump_load(OPC_CLD, t0, t1);
    gen_store_gpr(t1, rd);

    tcg_temp_free_i32(tlen);
    tcg_temp_free(t1);
    tcg_temp_free(t0);
    tcg_temp_free_i32(toffset);
    tcg_temp_free_i32(tcb);
}

static inline void generate_cllb(DisasContext *ctx, int32_t rd, int32_t cb)
{
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv t0 = tcg_temp_new();
    TCGv taddr = tcg_temp_new();
    TCGv_i32 tlen = tcg_const_i32(1);

    gen_helper_cloadlinked(taddr, cpu_env, tcb, tlen);
    tcg_gen_qemu_ld_tl(t0, taddr, ctx->mem_idx, MO_UB);
    generate_dump_load(OPC_CLLB, taddr, t0);
    gen_store_gpr(t0, rd);

    tcg_temp_free_i32(tlen);
    tcg_temp_free(taddr);
    tcg_temp_free(t0);
    tcg_temp_free_i32(tcb);
}

static inline void generate_cllbu(DisasContext *ctx, int32_t rd, int32_t cb)
{
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv t0 = tcg_temp_new();
    TCGv taddr = tcg_temp_new();
    TCGv_i32 tlen = tcg_const_i32(1);

    gen_helper_cloadlinked(taddr, cpu_env, tcb, tlen);
    tcg_gen_qemu_ld_tl(t0, taddr, ctx->mem_idx, MO_SB);
    generate_dump_load(OPC_CLLBU, taddr, t0);
    gen_store_gpr(t0, rd);

    tcg_temp_free_i32(tlen);
    tcg_temp_free(taddr);
    tcg_temp_free(t0);
    tcg_temp_free_i32(tcb);
}

static inline void generate_cllh(DisasContext *ctx, int32_t rd, int32_t cb)
{
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv t0 = tcg_temp_new();
    TCGv taddr = tcg_temp_new();
    TCGv_i32 tlen = tcg_const_i32(2);

    gen_helper_cloadlinked(taddr, cpu_env, tcb, tlen);
    tcg_gen_qemu_ld_tl(t0, taddr, ctx->mem_idx, MO_TEUW |
        ctx->default_tcg_memop_mask);
    generate_dump_load(OPC_CLLH, taddr, t0);
    gen_store_gpr(t0, rd);

    tcg_temp_free_i32(tlen);
    tcg_temp_free(taddr);
    tcg_temp_free(t0);
    tcg_temp_free_i32(tcb);
}

static inline void generate_cllhu(DisasContext *ctx, int32_t rd, int32_t cb)
{
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv t0 = tcg_temp_new();
    TCGv taddr = tcg_temp_new();
    TCGv_i32 tlen = tcg_const_i32(2);

    gen_helper_cloadlinked(taddr, cpu_env, tcb, tlen);
    tcg_gen_qemu_ld_tl(t0, taddr, ctx->mem_idx, MO_TESW |
        ctx->default_tcg_memop_mask);
    generate_dump_load(OPC_CLLHU, taddr, t0);
    gen_store_gpr(t0, rd);

    tcg_temp_free_i32(tlen);
    tcg_temp_free(taddr);
    tcg_temp_free(t0);
    tcg_temp_free_i32(tcb);
}

static inline void generate_cllw(DisasContext *ctx, int32_t rd, int32_t cb)
{
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv t0 = tcg_temp_new();
    TCGv taddr = tcg_temp_new();
    TCGv_i32 tlen = tcg_const_i32(4);

    gen_helper_cloadlinked(taddr, cpu_env, tcb, tlen);
    tcg_gen_qemu_ld_tl(t0, taddr, ctx->mem_idx, MO_TEUL |
        ctx->default_tcg_memop_mask);
    generate_dump_load(OPC_CLLW, taddr, t0);
    gen_store_gpr(t0, rd);

    tcg_temp_free_i32(tlen);
    tcg_temp_free(taddr);
    tcg_temp_free(t0);
    tcg_temp_free_i32(tcb);
}

static inline void generate_cllwu(DisasContext *ctx, int32_t rd, int32_t cb)
{
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv t0 = tcg_temp_new();
    TCGv taddr = tcg_temp_new();
    TCGv_i32 tlen = tcg_const_i32(4);

    gen_helper_cloadlinked(taddr, cpu_env, tcb, tlen);
    tcg_gen_qemu_ld_tl(t0, taddr, ctx->mem_idx, MO_TESL |
        ctx->default_tcg_memop_mask);
    generate_dump_load(OPC_CLLWU, taddr, t0);
    gen_store_gpr(t0, rd);

    tcg_temp_free_i32(tlen);
    tcg_temp_free(taddr);
    tcg_temp_free(t0);
    tcg_temp_free_i32(tcb);
}

static inline void generate_clld(DisasContext *ctx, int32_t rd, int32_t cb)
{
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv t0 = tcg_temp_new();
    TCGv taddr = tcg_temp_new();
    TCGv_i32 tlen = tcg_const_i32(8);

    gen_helper_cloadlinked(taddr, cpu_env, tcb, tlen);
    tcg_gen_qemu_ld_tl(t0, taddr, ctx->mem_idx, MO_TEQ |
        ctx->default_tcg_memop_mask);
    generate_dump_load(OPC_CLLD, taddr, t0);
    gen_store_gpr(t0, rd);

    tcg_temp_free_i32(tlen);
    tcg_temp_free(taddr);
    tcg_temp_free(t0);
    tcg_temp_free_i32(tcb);
}

static inline void generate_cinvalidate_tag(TCGv addr, int32_t len, int32_t opc,
                TCGv value);

static inline void generate_cstorecond(TCGv taddr, int32_t cb, int32_t len)
{
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv_i32 tlen = tcg_const_i32(len);

    gen_helper_cstorecond(taddr, cpu_env, tcb, tlen);
    tcg_temp_free_i32(tcb);
    tcg_temp_free_i32(tlen);
}

static inline void generate_cscb(DisasContext *ctx, int32_t rs, int32_t cb,
        int32_t rd)
{
    const int size = 1;
    TCGv taddr = tcg_temp_local_new();
    TCGv t0 = tcg_temp_new();
    TCGv tlf = tcg_temp_local_new();
    TCGLabel *l1 = gen_new_label();

    generate_cstorecond(taddr, cb, size);

    /* Get linkedFlag. */
    tcg_gen_ld_tl(tlf, cpu_env, offsetof(CPUMIPSState, linkedflag));

    /* If linkedFlag is zero then don't store rs, invalidate tag */
    tcg_gen_brcondi_tl(TCG_COND_EQ, tlf, 0, l1);

    /* Write rs to memory. */
    gen_load_gpr(t0, rs);
    tcg_gen_qemu_st_tl(t0, taddr, ctx->mem_idx, MO_8);

    /* Invalidate tag and log write to memory, if enabled. */
    generate_cinvalidate_tag(taddr, size, OPC_CSCB, t0);

    tcg_temp_free(taddr);
    tcg_temp_free(t0);

    gen_set_label(l1);
    /* Store linkedflag in rd. */
    gen_store_gpr(tlf, rd);
    tcg_temp_free(tlf);
}

static inline void generate_csch(DisasContext *ctx, int32_t rs, int32_t cb,
        int32_t rd)
{
    const int size = 2;
    TCGv taddr = tcg_temp_local_new();
    TCGv t0 = tcg_temp_new();
    TCGv tlf = tcg_temp_local_new();
    TCGLabel *l1 = gen_new_label();

    generate_cstorecond(taddr, cb, size);

    /* Get linkedFlag. */
    tcg_gen_ld_tl(tlf, cpu_env, offsetof(CPUMIPSState, linkedflag));

    /* If linkedFlag is zero then don't store rs, invalidate tag */
    tcg_gen_brcondi_tl(TCG_COND_EQ, tlf, 0, l1);

    /* Write rs to memory. */
    gen_load_gpr(t0, rs);
    tcg_gen_qemu_st_tl(t0, taddr, ctx->mem_idx, MO_TEUW |
            ctx->default_tcg_memop_mask);

    /* Invalidate tag and log write, if enabled. */
    generate_cinvalidate_tag(taddr, size, OPC_CSCH, t0);

    tcg_temp_free(taddr);
    tcg_temp_free(t0);

    gen_set_label(l1);
    /* Store linkedflag in rd. */
    gen_store_gpr(tlf, rd);
    tcg_temp_free(tlf);
}

static inline void generate_cscw(DisasContext *ctx, int32_t rs, int32_t cb,
        int32_t rd)
{
    const int size = 4;
    TCGv taddr = tcg_temp_local_new();
    TCGv t0 = tcg_temp_new();
    TCGv tlf = tcg_temp_local_new();
    TCGLabel *l1 = gen_new_label();

    generate_cstorecond(taddr, cb, size);

    /* Get linkedFlag. */
    tcg_gen_ld_tl(tlf, cpu_env, offsetof(CPUMIPSState, linkedflag));

    /* If linkedFlag is zero then don't store rs, invalidate tag */
    tcg_gen_brcondi_tl(TCG_COND_EQ, tlf, 0, l1);

    /* Write rs to memory. */
    gen_load_gpr(t0, rs);
    tcg_gen_qemu_st_tl(t0, taddr, ctx->mem_idx, MO_TEUL |
            ctx->default_tcg_memop_mask);

    /* Invalidate tag and log write, if enabled. */
    generate_cinvalidate_tag(taddr, size, OPC_CSCW, t0);

    tcg_temp_free(taddr);
    tcg_temp_free(t0);

    gen_set_label(l1);
    /* Store linkedflag in rd. */
    gen_store_gpr(tlf, rd);
    tcg_temp_free(tlf);
}

static inline void generate_cscd(DisasContext *ctx, int32_t rs, int32_t cb,
        int32_t rd)
{
    const int size = 8;
    TCGv taddr = tcg_temp_local_new();
    TCGv t0 = tcg_temp_new();
    TCGv tlf = tcg_temp_local_new();
    TCGLabel *l1 = gen_new_label();

    generate_cstorecond(taddr, cb, size);

    /* Get linkedFlag. */
    tcg_gen_ld_tl(tlf, cpu_env, offsetof(CPUMIPSState, linkedflag));

    /* If linkedFlag is zero then don't store rs, invalidate tag */
    tcg_gen_brcondi_tl(TCG_COND_EQ, tlf, 0, l1);

    /* Write rs to memory. */
    gen_load_gpr(t0, rs);
    tcg_gen_qemu_st_tl(t0, taddr, ctx->mem_idx, MO_TEQ |
            ctx->default_tcg_memop_mask);

    /* Invalidate tag and log write, if enabled. */
    generate_cinvalidate_tag(taddr, size, OPC_CSCD, t0);

    tcg_temp_free(taddr);
    tcg_temp_free(t0);

    gen_set_label(l1);
    /* Store linkedflag in rd. */
    gen_store_gpr(tlf, rd);
    tcg_temp_free(tlf);
}

static inline void generate_cstore(TCGv taddr, TCGv trt, int32_t cb,
        int32_t offset, const int size)
{
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv_i32 toffset = tcg_const_i32(cload_sign_extend(offset) * size);
    TCGv_i32 tlen = tcg_const_i32(size);

    gen_helper_cstore(taddr, cpu_env, tcb, trt, toffset, tlen);

    tcg_temp_free_i32(tlen);
    tcg_temp_free_i32(toffset);
    tcg_temp_free_i32(tcb);
}

static inline void generate_csb(DisasContext *ctx, int32_t rs, int32_t cb,
        int32_t rt, int32_t offset)
{
    const int size = 1;
    TCGv taddr = tcg_temp_new();
    TCGv t0 = tcg_temp_new();

    gen_load_gpr(t0, rt);

    generate_cstore(taddr, t0, cb, offset, size);

    gen_load_gpr(t0, rs);
    tcg_gen_qemu_st_tl(t0, taddr, ctx->mem_idx, MO_8);

    /* Invalidate tag and log write, if enabled. */
    generate_cinvalidate_tag(taddr, size, OPC_CSB, t0);

    tcg_temp_free(t0);
    tcg_temp_free(taddr);
}

static inline void generate_csh(DisasContext *ctx, int32_t rs, int32_t cb,
        int32_t rt, int32_t offset)
{
    const int size = 2;
    TCGv taddr = tcg_temp_new();
    TCGv t0 = tcg_temp_new();

    gen_load_gpr(t0, rt);

    generate_cstore(taddr, t0, cb, offset, size);

    gen_load_gpr(t0, rs);
    tcg_gen_qemu_st_tl(t0, taddr, ctx->mem_idx, MO_TEUW |
            ctx->default_tcg_memop_mask);

    /* Invalidate tag and log write, if enabled. */
    generate_cinvalidate_tag(taddr, size, OPC_CSH, t0);

    tcg_temp_free(t0);
    tcg_temp_free(taddr);
}

static inline void generate_csw(DisasContext *ctx, int32_t rs, int32_t cb,
        int32_t rt, int32_t offset)
{
    const int size = 4;
    TCGv taddr = tcg_temp_new();
    TCGv t0 = tcg_temp_new();

    gen_load_gpr(t0, rt);

    generate_cstore(taddr, t0, cb, offset, size);

    gen_load_gpr(t0, rs);
    tcg_gen_qemu_st_tl(t0, taddr, ctx->mem_idx, MO_TEUL |
            ctx->default_tcg_memop_mask);

    /* Invalidate tag and log write, if enabled. */
    generate_cinvalidate_tag(taddr, size, OPC_CSW, t0);

    tcg_temp_free(t0);
    tcg_temp_free(taddr);
}

static inline void generate_csd(DisasContext *ctx, int32_t rs, int32_t cb,
        int32_t rt, int32_t offset)
{
    const int size = 8;
    TCGv taddr = tcg_temp_new();
    TCGv t0 = tcg_temp_new();

    gen_load_gpr(t0, rt);

    generate_cstore(taddr, t0, cb, offset, size);

    gen_load_gpr(t0, rs);
    tcg_gen_qemu_st_tl(t0, taddr, ctx->mem_idx, MO_TEQ |
            ctx->default_tcg_memop_mask);

    /* Invalidate tag and log write, if enabled. */
    generate_cinvalidate_tag(taddr, size, OPC_CSD, t0);

    tcg_temp_free(t0);
    tcg_temp_free(taddr);
}

static inline int32_t clc_sign_extend(int32_t x, bool big_imm)
{
    const int32_t bits = big_imm ? 16 : 11;
    int32_t const mask = 1U << (bits - 1);

    x = x & ((1U << bits) - 1);
    return (x ^ mask) - mask;
}

#ifdef CHERI_128
static inline void generate_clc(DisasContext *ctx, int32_t cd, int32_t cb,
        int32_t rt, int32_t offset, bool big_imm)
{
    TCGv_i32 tcd = tcg_const_i32(cd);
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv_i32 toffset = tcg_const_i32(clc_sign_extend(offset, big_imm) * 16);
    TCGv taddr = tcg_temp_new();
    TCGv t0 = tcg_temp_new();
    TCGv t1 = tcg_temp_new();
    TCGv ttag = tcg_temp_new();

    /* Check the cap registers and compute the address. */
    gen_load_gpr(t0, rt);
    gen_helper_clc_addr(taddr, cpu_env, tcd, tcb, t0, toffset);

    /* Fetch 1st 64-bits in t0 */
    tcg_gen_qemu_ld_tl(t0, taddr, ctx->mem_idx,
            MO_TEQ | ctx->default_tcg_memop_mask);
    tcg_gen_addi_tl(taddr, taddr, 8);

    /* Fetch 2nd 64-bits in t1 */
    tcg_gen_qemu_ld_tl(t1, taddr, ctx->mem_idx,
            MO_TEQ | ctx->default_tcg_memop_mask);
    tcg_gen_subi_tl(taddr, taddr, 8);

    /* Store in the capability register. */
    gen_helper_bytes2cap_128_tag_get(ttag, cpu_env, tcd, tcb, taddr);
    gen_helper_bytes2cap_128(cpu_env, tcd, t0, t1);
    gen_helper_bytes2cap_128_tag_set(cpu_env, tcd, ttag, taddr, t1);

    tcg_temp_free(ttag);
    tcg_temp_free(t1);
    tcg_temp_free(t0);
    tcg_temp_free(taddr);
    tcg_temp_free_i32(toffset);
    tcg_temp_free_i32(tcb);
    tcg_temp_free_i32(tcd);
}

static inline void generate_cllc(DisasContext *ctx, int32_t cd, int32_t cb)
{
    TCGv_i32 tcd = tcg_const_i32(cd);
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv taddr = tcg_temp_new();
    TCGv t0 = tcg_temp_new();
    TCGv t1 = tcg_temp_new();
    TCGv ttag = tcg_temp_new();

    /* Check the cap registers and compute the address. */
    gen_helper_cllc_addr(taddr, cpu_env, tcd, tcb);

    /* Fetch 1st 64-bits in t0 */
    tcg_gen_qemu_ld_tl(t0, taddr, ctx->mem_idx,
            MO_TEQ | ctx->default_tcg_memop_mask);
    tcg_gen_addi_tl(taddr, taddr, 8);

    /* Fetch 2nd 64-bits in t1 */
    tcg_gen_qemu_ld_tl(t1, taddr, ctx->mem_idx,
            MO_TEQ | ctx->default_tcg_memop_mask);
    tcg_gen_subi_tl(taddr, taddr, 8);

    /* Store in the capability register. */
    gen_helper_bytes2cap_128_tag_get(ttag, cpu_env, tcd, tcb, taddr);
    gen_helper_bytes2cap_128(cpu_env, tcd, t0, t1);
    gen_helper_bytes2cap_128_tag_set(cpu_env, tcd, ttag, taddr, t1);

    tcg_temp_free(ttag);
    tcg_temp_free(t1);
    tcg_temp_free(t0);
    tcg_temp_free(taddr);
    tcg_temp_free_i32(tcb);
    tcg_temp_free_i32(tcd);
}

static inline void generate_csc(DisasContext *ctx, int32_t cs, int32_t cb,
        int32_t rt, int32_t offset, bool big_imm)
{
    TCGv_i32 tcs = tcg_const_i32(cs);
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv_i32 toffset = tcg_const_i32(clc_sign_extend(offset, big_imm) * 16);
    TCGv taddr = tcg_temp_new();
    TCGv t0 = tcg_temp_new();
    TCGv_i32 tbdoffset;

    /* Check the cap registers and compute the address. */
    gen_load_gpr(t0, rt);
    gen_helper_csc_addr(taddr, cpu_env, tcs, tcb, t0, toffset);

    /* Store 1st 64-bits in memory. */
    gen_helper_cap2bytes_128b(t0, cpu_env, tcs, taddr);
    tcg_gen_qemu_st_tl(t0, taddr, ctx->mem_idx, MO_TEQ |
            ctx->default_tcg_memop_mask);

    /* Store 2nd 64-bits in memory. */
    /* Is this instruction in a branch delay slot? */
    if (ctx->hflags & MIPS_HFLAG_BMASK) {
        tbdoffset = (ctx->hflags & MIPS_HFLAG_BDS16) ? tcg_const_i32(2) :
            tcg_const_i32(4);
    } else {
        tbdoffset = tcg_const_i32(0);
    }
    gen_helper_cap2bytes_128c(t0, cpu_env, tcs, tbdoffset, taddr);
    tcg_temp_free_i32(tbdoffset);
    tcg_gen_addi_tl(taddr, taddr, 8);
    tcg_gen_qemu_st_tl(t0, taddr, ctx->mem_idx, MO_TEQ |
            ctx->default_tcg_memop_mask);

    tcg_temp_free(t0);
    tcg_temp_free(taddr);
    tcg_temp_free_i32(toffset);
    tcg_temp_free_i32(tcb);
    tcg_temp_free_i32(tcs);
}

static inline void generate_cscc(DisasContext *ctx, int32_t cs, int32_t cb,
        int32_t rd)
{
    TCGv_i32 tcs = tcg_const_i32(cs);
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv taddr = tcg_temp_local_new();
    TCGv t1 = tcg_temp_local_new();
    TCGLabel *l1 = gen_new_label();
    TCGv_i32 tbdoffset;

    /* Check the cap registers and compute the address. */
    gen_helper_cscc_addr(taddr, cpu_env, tcs, tcb);
    tcg_temp_free_i32(tcb);
    tcg_temp_free_i32(tcs);

    /* Set the rd based on the linkedflag. */
    tcg_gen_ld_tl(t1, cpu_env, offsetof(CPUMIPSState, linkedflag));
    gen_store_gpr(t1, rd);

    /* If linkedflag is zero then don't store capability. */
    tcg_gen_brcondi_tl(TCG_COND_EQ, t1, 0, l1);
    tcg_temp_free(t1);

    TCGv t0 = tcg_temp_new();
    TCGv_i32 tcs2 = tcg_const_i32(cs);


    /* Store 1st 64-bits in memory. */
    gen_helper_cap2bytes_128b(t0, cpu_env, tcs2, taddr);
    tcg_gen_qemu_st_tl(t0, taddr, ctx->mem_idx, MO_TEQ |
            ctx->default_tcg_memop_mask);

    /* Store 2nd 64-bits in memory. */
    /* Is this instruction in a branch delay slot? */
    if (ctx->hflags & MIPS_HFLAG_BMASK) {
        tbdoffset = (ctx->hflags & MIPS_HFLAG_BDS16) ? tcg_const_i32(2) :
            tcg_const_i32(4);
    } else {
        tbdoffset = tcg_const_i32(0);
    }
    gen_helper_cap2bytes_128c(t0, cpu_env, tcs2, tbdoffset, taddr);
    tcg_temp_free_i32(tbdoffset);
    tcg_gen_addi_tl(taddr, taddr, 8);
    tcg_gen_qemu_st_tl(t0, taddr, ctx->mem_idx, MO_TEQ |
            ctx->default_tcg_memop_mask);

    tcg_temp_free(t0);
    tcg_temp_free(taddr);
    tcg_temp_free_i32(tcs2);

    gen_set_label(l1);
}

#elif defined(CHERI_MAGIC128)

static inline void generate_clc(DisasContext *ctx, int32_t cd, int32_t cb,
        int32_t rt, int32_t offset, bool big_imm)
{
    TCGv_i32 tcd = tcg_const_i32(cd);
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv_i32 toffset = tcg_const_i32(clc_sign_extend(offset, big_imm) * 16);
    TCGv taddr = tcg_temp_new();
    TCGv t0 = tcg_temp_new();
    TCGv t1 = tcg_temp_new();

    /* Check the cap registers and compute the address. */
    gen_load_gpr(t0, rt);
    gen_helper_clc_addr(taddr, cpu_env, tcd, tcb, t0, toffset);

    /* Fetch 1st 64-bits in t0 (base) */
    tcg_gen_qemu_ld_tl(t0, taddr, ctx->mem_idx,
            MO_TEQ | ctx->default_tcg_memop_mask);
    tcg_gen_addi_tl(taddr, taddr, 8);

    /* Fetch 2nd 64-bits in t1 (cursor) */
    tcg_gen_qemu_ld_tl(t1, taddr, ctx->mem_idx,
            MO_TEQ | ctx->default_tcg_memop_mask);
    tcg_gen_subi_tl(taddr, taddr, 8);

    /* Store in the capability register. */
    gen_helper_bytes2cap_m128(cpu_env, tcd, t0, t1, taddr);
    gen_helper_bytes2cap_m128_tag(cpu_env, tcb, tcd, t1, taddr);

    tcg_temp_free(t1);
    tcg_temp_free(t0);
    tcg_temp_free(taddr);
    tcg_temp_free_i32(toffset);
    tcg_temp_free_i32(tcb);
    tcg_temp_free_i32(tcd);
}

static inline void generate_cllc(DisasContext *ctx, int32_t cd, int32_t cb)
{
    TCGv_i32 tcd = tcg_const_i32(cd);
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv taddr = tcg_temp_new();
    TCGv t0 = tcg_temp_new();
    TCGv t1 = tcg_temp_new();

    /* Check the cap registers and compute the address. */
    gen_helper_cllc_addr(taddr, cpu_env, tcd, tcb);

    /* Fetch 1st 64-bits in t0 (base) */
    tcg_gen_qemu_ld_tl(t0, taddr, ctx->mem_idx,
            MO_TEQ | ctx->default_tcg_memop_mask);
    tcg_gen_addi_tl(taddr, taddr, 8);

    /* Fetch 2nd 64-bits in t1 (cursor) */
    tcg_gen_qemu_ld_tl(t1, taddr, ctx->mem_idx,
            MO_TEQ | ctx->default_tcg_memop_mask);
    tcg_gen_subi_tl(taddr, taddr, 8);

    /* Store in the capability register. */
    gen_helper_bytes2cap_m128(cpu_env, tcd, t0, t1, taddr);
    gen_helper_bytes2cap_m128_tag(cpu_env, tcb, tcd, t1, taddr);

    tcg_temp_free(t1);
    tcg_temp_free(t0);
    tcg_temp_free(taddr);
    tcg_temp_free_i32(tcb);
    tcg_temp_free_i32(tcd);
}

static inline void generate_csc(DisasContext *ctx, int32_t cs, int32_t cb,
        int32_t rt, int32_t offset, bool big_imm)
{
    TCGv_i32 tcs = tcg_const_i32(cs);
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv_i32 toffset = tcg_const_i32(clc_sign_extend(offset, big_imm) * 16);
    TCGv taddr = tcg_temp_new();
    TCGv t0 = tcg_temp_new();
    TCGv_i32 tbdoffset;

    /* Check the cap registers and compute the address. */
    gen_load_gpr(t0, rt);
    gen_helper_csc_addr(taddr, cpu_env, tcs, tcb, t0, toffset);

    /* Store 1st 64-bits (base) in memory. */
    /* Is this instruction in a branch delay slot? */
    if (ctx->hflags & MIPS_HFLAG_BMASK) {
        tbdoffset = (ctx->hflags & MIPS_HFLAG_BDS16) ? tcg_const_i32(2) :
            tcg_const_i32(4);
    } else {
        tbdoffset = tcg_const_i32(0);
    }
    gen_helper_cap2bytes_m128b(t0, cpu_env, tcs, tbdoffset, taddr);
    tcg_temp_free_i32(tbdoffset);
    tcg_gen_qemu_st_tl(t0, taddr, ctx->mem_idx, MO_TEQ |
            ctx->default_tcg_memop_mask);

    /* Store 2nd 64-bits (cursor) in memory. */
    gen_helper_cap2bytes_m128c(t0, cpu_env, tcs, taddr);
    tcg_gen_addi_tl(taddr, taddr, 8);
    tcg_gen_qemu_st_tl(t0, taddr, ctx->mem_idx, MO_TEQ |
            ctx->default_tcg_memop_mask);

    tcg_temp_free(t0);
    tcg_temp_free(taddr);
    tcg_temp_free_i32(toffset);
    tcg_temp_free_i32(tcb);
    tcg_temp_free_i32(tcs);
}

static inline void generate_cscc(DisasContext *ctx, int32_t cs, int32_t cb,
        int32_t rd)
{
    TCGv_i32 tcs = tcg_const_i32(cs);
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv taddr = tcg_temp_local_new();
    TCGv t1 = tcg_temp_local_new();
    TCGLabel *l1 = gen_new_label();
    TCGv_i32 tbdoffset;

    /* Check the cap registers and compute the address. */
    gen_helper_cscc_addr(taddr, cpu_env, tcs, tcb);
    tcg_temp_free_i32(tcb);
    tcg_temp_free_i32(tcs);

    /* Set the rd based on the linkedflag. */
    tcg_gen_ld_tl(t1, cpu_env, offsetof(CPUMIPSState, linkedflag));
    gen_store_gpr(t1, rd);

    /* If linkedflag is zero then don't store capability. */
    tcg_gen_brcondi_tl(TCG_COND_EQ, t1, 0, l1);
    tcg_temp_free(t1);

    TCGv t0 = tcg_temp_new();
    TCGv_i32 tcs2 = tcg_const_i32(cs);

    /* Store 1st 64-bits in memory (base). */
    /* Is this instruction in a branch delay slot? */
    if (ctx->hflags & MIPS_HFLAG_BMASK) {
        tbdoffset = (ctx->hflags & MIPS_HFLAG_BDS16) ? tcg_const_i32(2) :
            tcg_const_i32(4);
    } else {
        tbdoffset = tcg_const_i32(0);
    }
    gen_helper_cap2bytes_m128b(t0, cpu_env, tcs2, tbdoffset, taddr);
    tcg_temp_free_i32(tbdoffset);
    tcg_gen_qemu_st_tl(t0, taddr, ctx->mem_idx, MO_TEQ |
            ctx->default_tcg_memop_mask);

    /* Store 2nd 64-bits in memory (cursor). */
    gen_helper_cap2bytes_m128c(t0, cpu_env, tcs2, taddr);
    tcg_gen_addi_tl(taddr, taddr, 8);
    tcg_gen_qemu_st_tl(t0, taddr, ctx->mem_idx, MO_TEQ |
            ctx->default_tcg_memop_mask);

    tcg_temp_free(t0);
    tcg_temp_free(taddr);
    tcg_temp_free_i32(tcs2);

    gen_set_label(l1);
}

#else /* ! CHERI_MAGIC128 */

static inline void generate_clc(DisasContext *ctx, int32_t cd, int32_t cb,
        int32_t rt, int32_t offset, bool big_imm)
{
    TCGv_i32 tcd = tcg_const_i32(cd);
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv_i32 toffset = tcg_const_i32(clc_sign_extend(offset, big_imm) * 16);
    TCGv taddr = tcg_temp_new();
    TCGv t0 = tcg_temp_new();
    TCGv t1 = tcg_temp_new();
    TCGv t2 = tcg_temp_new();

    /* Check the cap registers and compute the address. */
    gen_load_gpr(t0, rt);
    gen_helper_clc_addr(taddr, cpu_env, tcd, tcb, t0, toffset);

    /* Fetch the otype and perms from memory */
    tcg_gen_qemu_ld_tl(t0, taddr, ctx->mem_idx,
            MO_TEQ | ctx->default_tcg_memop_mask);

    /* Store in the capability register. */
    gen_helper_bytes2cap_op(cpu_env, tcb, tcd, t0, taddr);

    /* Fetch the cursor, base, and length from memory */
    tcg_gen_addi_tl(taddr, taddr, 8);

    /* Fetch cursor in t0 */
    tcg_gen_qemu_ld_tl(t0, taddr, ctx->mem_idx,
            MO_TEQ | ctx->default_tcg_memop_mask);
    tcg_gen_addi_tl(taddr, taddr, 8);

    /* Fetch base in t1 */
    tcg_gen_qemu_ld_tl(t1, taddr, ctx->mem_idx,
            MO_TEQ | ctx->default_tcg_memop_mask);
    tcg_gen_addi_tl(taddr, taddr, 8);

    /* Fetch length in t2 */
    tcg_gen_qemu_ld_tl(t2, taddr, ctx->mem_idx,
            MO_TEQ | ctx->default_tcg_memop_mask);

    /* Store in the capability register. */
    gen_helper_bytes2cap_cbl(cpu_env, tcd, t0, t1, t2);

    tcg_temp_free(t2);
    tcg_temp_free(t1);
    tcg_temp_free(t0);
    tcg_temp_free(taddr);
    tcg_temp_free_i32(toffset);
    tcg_temp_free_i32(tcb);
    tcg_temp_free_i32(tcd);
}

static inline void generate_cllc(DisasContext *ctx, int32_t cd, int32_t cb)
{
    TCGv_i32 tcd = tcg_const_i32(cd);
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv taddr = tcg_temp_new();
    TCGv t0 = tcg_temp_new();
    TCGv t1 = tcg_temp_new();
    TCGv t2 = tcg_temp_new();

    /* Check the cap registers and compute the address. */
    gen_helper_cllc_addr(taddr, cpu_env, tcd, tcb);

    /* Fetch the otype and perms from memory */
    tcg_gen_qemu_ld_tl(t0, taddr, ctx->mem_idx,
            MO_TEQ | ctx->default_tcg_memop_mask);

    /* Store in the capability register. */
    gen_helper_bytes2cap_opll(cpu_env, tcb, tcd, t0, taddr);

    /* Fetch the cursor, base, and length from memory */
    tcg_gen_addi_tl(taddr, taddr, 8);

    /* Fetch cursor in t0 */
    tcg_gen_qemu_ld_tl(t0, taddr, ctx->mem_idx,
            MO_TEQ | ctx->default_tcg_memop_mask);
    tcg_gen_addi_tl(taddr, taddr, 8);

    /* Fetch base in t1 */
    tcg_gen_qemu_ld_tl(t1, taddr, ctx->mem_idx,
            MO_TEQ | ctx->default_tcg_memop_mask);
    tcg_gen_addi_tl(taddr, taddr, 8);

    /* Fetch length in t2 */
    tcg_gen_qemu_ld_tl(t2, taddr, ctx->mem_idx,
            MO_TEQ | ctx->default_tcg_memop_mask);

    /* Store in the capability register. */
    gen_helper_bytes2cap_cbl(cpu_env, tcd, t0, t1, t2);

    tcg_temp_free(t2);
    tcg_temp_free(t1);
    tcg_temp_free(t0);
    tcg_temp_free(taddr);
    tcg_temp_free_i32(tcb);
    tcg_temp_free_i32(tcd);
}

static inline void generate_csc(DisasContext *ctx, int32_t cs, int32_t cb,
        int32_t rt, int32_t offset, bool big_imm)
{
    TCGv_i32 tcs = tcg_const_i32(cs);
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv_i32 toffset = tcg_const_i32(clc_sign_extend(offset, big_imm) * 16);
    TCGv taddr = tcg_temp_new();
    TCGv t0 = tcg_temp_new();
    TCGv_i32 tbdoffset;

    /* Check the cap registers and compute the address. */
    gen_load_gpr(t0, rt);
    gen_helper_csc_addr(taddr, cpu_env, tcs, tcb, t0, toffset);

    /* Store otype and perms to memory. */
    gen_helper_cap2bytes_op(t0, cpu_env, tcs, taddr);
    tcg_gen_qemu_st_tl(t0, taddr, ctx->mem_idx, MO_TEQ |
            ctx->default_tcg_memop_mask);

    /*
     * Store cursor to memory. Also, set the tag bit.  We
     * set the tag bit here because the store above would
     * have faulted the TLB if it didn't have an entry for
     * this address.  Once we are sure the TLB has an entry
     * we can set the tab bit.
     */
    /* Is this instruction in a branch delay slot? */
    if (ctx->hflags & MIPS_HFLAG_BMASK) {
        tbdoffset = (ctx->hflags & MIPS_HFLAG_BDS16) ? tcg_const_i32(2) :
            tcg_const_i32(4);
    } else {
        tbdoffset = tcg_const_i32(0);
    }
    gen_helper_cap2bytes_cursor(t0, cpu_env, tcs, tbdoffset, taddr);
    tcg_temp_free_i32(tbdoffset);
    tcg_gen_addi_tl(taddr, taddr, 8);
    tcg_gen_qemu_st_tl(t0, taddr, ctx->mem_idx, MO_TEQ |
            ctx->default_tcg_memop_mask);

    /* Store base to memory. */
    gen_helper_cap2bytes_base(t0, cpu_env, tcs);
    tcg_gen_addi_tl(taddr, taddr, 8);
    tcg_gen_qemu_st_tl(t0, taddr, ctx->mem_idx, MO_TEQ |
            ctx->default_tcg_memop_mask);

    /* Store length to memory. */
    gen_helper_cap2bytes_length(t0, cpu_env, tcs);
    tcg_gen_addi_tl(taddr, taddr, 8);
    tcg_gen_qemu_st_tl(t0, taddr, ctx->mem_idx, MO_TEQ |
            ctx->default_tcg_memop_mask);

    tcg_temp_free(t0);
    tcg_temp_free(taddr);
    tcg_temp_free_i32(toffset);
    tcg_temp_free_i32(tcb);
    tcg_temp_free_i32(tcs);
}

static inline void generate_cscc(DisasContext *ctx, int32_t cs, int32_t cb,
        int32_t rd)
{
    TCGv_i32 tcs = tcg_const_i32(cs);
    TCGv_i32 tcb = tcg_const_i32(cb);
    TCGv taddr = tcg_temp_local_new();
    TCGv t1 = tcg_temp_local_new();
    TCGLabel *l1 = gen_new_label();
    TCGv_i32 tbdoffset;

    /* Check the cap registers and compute the address. */
    gen_helper_cscc_addr(taddr, cpu_env, tcs, tcb);
    tcg_temp_free_i32(tcb);
    tcg_temp_free_i32(tcs);

    /* Set the rd based on the linkedflag. */
    tcg_gen_ld_tl(t1, cpu_env, offsetof(CPUMIPSState, linkedflag));
    gen_store_gpr(t1, rd);

    /* If linkedflag is zero then don't store capability. */
    tcg_gen_brcondi_tl(TCG_COND_EQ, t1, 0, l1);
    tcg_temp_free(t1);

    TCGv t0 = tcg_temp_new();
    TCGv_i32 tcs2 = tcg_const_i32(cs);

    /* Store otype and perms to memory. */
    gen_helper_cap2bytes_op(t0, cpu_env, tcs, taddr);
    tcg_gen_qemu_st_tl(t0, taddr, ctx->mem_idx, MO_TEQ |
            ctx->default_tcg_memop_mask);

    /*
     * Store cursor to memory. Also, set the tag bit.  We
     * set the tag bit here because the store above would
     * have faulted the TLB if it didn't have an entry for
     * this address.  Once we are sure the TLB has an entry
     * we can set the tab bit.
     */

    /* Is this instruction in a branch delay slot? */
    if (ctx->hflags & MIPS_HFLAG_BMASK) {
        tbdoffset = (ctx->hflags & MIPS_HFLAG_BDS16) ? tcg_const_i32(2) :
            tcg_const_i32(4);
    } else {
        tbdoffset = tcg_const_i32(0);
    }
    gen_helper_cap2bytes_cursor(t0, cpu_env, tcs, tbdoffset, taddr);
    tcg_temp_free_i32(tbdoffset);
    tcg_gen_addi_tl(taddr, taddr, 8);
    tcg_gen_qemu_st_tl(t0, taddr, ctx->mem_idx, MO_TEQ |
            ctx->default_tcg_memop_mask);

    /* Store base to memory. */
    gen_helper_cap2bytes_base(t0, cpu_env, tcs);
    tcg_gen_addi_tl(taddr, taddr, 8);
    tcg_gen_qemu_st_tl(t0, taddr, ctx->mem_idx, MO_TEQ |
            ctx->default_tcg_memop_mask);

    /* Store length to memory. */
    gen_helper_cap2bytes_length(t0, cpu_env, tcs);
    tcg_gen_addi_tl(taddr, taddr, 8);
    tcg_gen_qemu_st_tl(t0, taddr, ctx->mem_idx, MO_TEQ |
            ctx->default_tcg_memop_mask);


    tcg_temp_free(t0);
    tcg_temp_free(taddr);
    tcg_temp_free_i32(tcs2);

    gen_set_label(l1);
}
#endif /* ! CHERI_MAGIC128 */

static inline void generate_ccheck_pc(DisasContext *ctx)
{
    TCGv_i64 tpc = tcg_const_i64(ctx->base.pc_next);
    gen_helper_ccheck_pc(cpu_env, tpc);
    tcg_temp_free_i64(tpc);
}

#define GEN_CAP_CHECK_PC(ctx)    generate_ccheck_pc(ctx)

static inline void generate_ccheck_store(TCGv addr, TCGv offset, int32_t len)
{
    TCGv_i32 tlen = tcg_const_i32(len);
    gen_helper_ccheck_store(addr, cpu_env, offset, tlen);
    tcg_temp_free_i32(tlen);
}

static inline void generate_ccheck_store_right(TCGv addr, TCGv offset, int32_t len)
{
    TCGv_i32 tlen = tcg_const_i32(len);
    gen_helper_ccheck_store_right(addr, cpu_env, offset, tlen);
    tcg_temp_free_i32(tlen);
}

#define GEN_CAP_CHECK_STORE_RIGHT(addr, offset, len) \
    generate_ccheck_store_right(addr, offset, len)
#define GEN_CAP_CHECK_STORE(addr, offset, len) \
    generate_ccheck_store(addr, offset, len)

static inline void generate_ccheck_load(TCGv addr, TCGv offset, int32_t len)
{
    TCGv_i32 tlen = tcg_const_i32(len);

    gen_helper_ccheck_load(addr, cpu_env, offset, tlen);
    tcg_temp_free_i32(tlen);
}
static inline void generate_ccheck_load_right(TCGv addr, TCGv offset, int32_t len)
{
    TCGv_i32 tlen = tcg_const_i32(len);

    gen_helper_ccheck_load_right(addr, cpu_env, offset, tlen);
    tcg_temp_free_i32(tlen);
}
#define GEN_CAP_CHECK_LOAD_RIGHT(save, addr, offset, len) \
    generate_ccheck_load_right(addr, offset, len); tcg_gen_mov_tl(save, addr)
#define GEN_CAP_CHECK_LOAD(save, addr, offset, len) \
    generate_ccheck_load(addr, offset, len); tcg_gen_mov_tl(save, addr)

static inline void generate_cinvalidate_tag(TCGv addr, int32_t len, int32_t opc,
        TCGv value)
{
    TCGv_i32 tlen = tcg_const_i32(len);
    TCGv_i32 topc = tcg_const_i32(opc);

    gen_helper_cinvalidate_tag(cpu_env, addr, tlen, topc, value);
    tcg_temp_free_i32(topc);
    tcg_temp_free_i32(tlen);
}

static inline void generate_cinvalidate_tag32(TCGv addr, int32_t len,
        int32_t opc, TCGv_i32 value)
{
    TCGv_i32 tlen = tcg_const_i32(len);
    TCGv_i32 topc = tcg_const_i32(opc);

    gen_helper_cinvalidate_tag32(cpu_env, addr, tlen, topc, value);
    tcg_temp_free_i32(topc);
    tcg_temp_free_i32(tlen);
}

#define GEN_CAP_INVADIATE_TAG(addr, len, opc, value) \
    generate_cinvalidate_tag(addr, len, opc, value)

#define GEN_CAP_INVADIATE_TAG32(addr, len, opc, value) \
    generate_cinvalidate_tag32(addr, len, opc, value)

static void gen_mtc2(DisasContext *ctx, TCGv arg, int reg, int sel)
{
    const char *rn = "invalid";

    switch (reg) {
    case 0:
        switch (sel) {
        case 6:
            gen_helper_mtc2_dumpcstate(cpu_env, arg);
            rn = "capdump";
            goto out;
        default:
            goto cp2_unimplemented;
        }
    default:
        goto cp2_unimplemented;
    }
out:
    (void)rn; /* avoid a compiler warning */
    LOG_DISAS("mtc2 %s (reg %d sel %d)\n", rn, reg, sel);
    /* For simplicity assume that all writes can cause interrupts.  */
    if (ctx->base.tb->cflags & CF_USE_ICOUNT) {
        gen_io_end();
        ctx->base.is_jmp = DISAS_EXIT;
    }
    return;

cp2_unimplemented:
    LOG_DISAS("mtc2 %s (reg %d sel %d)\n", rn, reg, sel);
}


#else /* ! TARGET_CHERI */

#define GEN_CAP_CHECK_PC(ctx)
#define GEN_CAP_CHECK_STORE(addr, offset, len)
#define GEN_CAP_CHECK_LOAD(save, addr, offset, len)
#define GEN_CAP_CHECK_STORE_RIGHT(addr, offset, len)
#define GEN_CAP_CHECK_LOAD_RIGHT(save, addr, offset, len)
#define GEN_CAP_INVADIATE_TAG(addr, len, opc, value)
#define GEN_CAP_INVADIATE_TAG32(addr, len, opc, value)
#endif /* ! TARGET_CHERI */


// instruction decoding for CHERI insns:
#if defined(TARGET_CHERI)
static void gen_cp2 (DisasContext *ctx, uint32_t opc, int r16, int r11, int r6)
{
    const char *opn = "cp2inst";

    /*
     * r16 = (ctx->opcode >> 16) & 0x1f;
     * r11 = (ctx->opcode >> 11) & 0x1f;
     * r6  = (ctx->opcode >> 6) & 0x1f;
     */

    /*
     * Every instruction checks whether cp2 is enabled and if
     * it can be in a delay slot, checks if the delay slot is a ccall
     * delay slot and is accessing IDC.
     */

    switch (MASK_CP2(opc)) {
    case OPC_CGET:  /* same as OPC_CAP_NI, 0x00 */
        switch(MASK_CAP6(opc)) {
        case OPC_CGETPERM:          /* 0x00 */
            check_cop2x(ctx);
            generate_cgetperm(r16, r11);
            opn = "cgetperm";
            break;
        case OPC_CGETTYPE:          /* 0x01 */
            check_cop2x(ctx);
            generate_cgettype(r16, r11);
            opn = "cgettype";
            break;
        case OPC_CGETBASE:          /* 0x02 */
            check_cop2x(ctx);
            generate_cgetbase(r16, r11);
            opn = "cgetbase";
            break;
        case OPC_CGETLEN:           /* 0x03 */
            check_cop2x(ctx);
            generate_cgetlen(r16, r11);
            opn = "cgetlen";
            break;
        case OPC_CGETCAUSE:         /* 0x04 */
            check_cop2x(ctx);
            generate_cgetcause(r16);
            opn = "cgetcause";
            break;
        case OPC_CGETTAG:           /* 0x05 */
            check_cop2x(ctx);
            generate_cgettag(r16, r11);
            opn = "cgettag";
            break;
        case OPC_CGETSEALED:        /* 0x06 */
            check_cop2x(ctx);
            generate_cgetsealed(r16, r11);
            opn = "cgetsealed";
            break;
        case OPC_CGETPCC:           /* 0x07 */
            check_cop2x(ctx);
            generate_cgetpcc(r11);
            opn = "cgetpcc";
            break;
                                    /* 0x08 */
        case OPC_CSETBOUNDSEXACT:   /* 0x09 */
            check_cop2x(ctx);
            generate_csetboundsexact(r16, r11, r6);
            opn = "csetboundsexact";
            break;
        case OPC_CSUB:              /* 0x0a */
            check_cop2x(ctx);
            generate_csub(r16, r11, r6);
            opn = "csub";
            break;
        case OPC_CSEAL_NI: /* 0x0b */
            check_cop2x(ctx);
            generate_cseal(r16, r11, r6);
            opn = "cseal";
            break;
        case OPC_CUNSEAL_NI: /* 0x0c */
            check_cop2x(ctx);
            generate_cunseal(r16, r11, r6);
            opn = "cunseal";
            break;
        case OPC_CANDPERM_NI: /* 0x0d */
            check_cop2x(ctx);
            generate_candperm(r16, r11, r6);
            opn = "candperm";
            break;
        case OPC_CSETOFFSET_NI: /* 0x0f */
            check_cop2x(ctx);
            generate_csetoffset(r16, r11, r6);
            opn = "csetoffset";
            break;
        case OPC_CSETBOUNDS_NI: /* 0x10 */
            check_cop2x(ctx);
            generate_csetbounds(r16, r11, r6);
            opn = "csetbounds";
            break;
        case OPC_CINCOFFSET_NI: /* 0x11 */
            check_cop2x(ctx);
            generate_cincoffset(r16, r11, r6);
            opn = "cincoffset";
            break;
        case OPC_CTOPTR_NI: /* 0x12 */
            check_cop2x(ctx);
            generate_ctoptr(r16, r11, r6);
            opn = "ctoptr";
            break;
        case OPC_CFROMPTR_NI: /* 0x13 */
            check_cop2x(ctx);
            generate_cfromptr(r16, r11, r6);
            opn = "cfromptr";
            break;
        case OPC_CEQ_NI: /* 0x14 */
            check_cop2x(ctx);
            generate_ceq(r16, r11, r6);
            opn = "ceq";
            break;
        case OPC_CNE_NI: /* 0x15 */
            check_cop2x(ctx);
            generate_cne(r16, r11, r6);
            opn = "cne";
            break;
        case OPC_CLT_NI: /* 0x16 */
            check_cop2x(ctx);
            generate_clt(r16, r11, r6);
            opn = "clt";
            break;
        case OPC_CLE_NI: /* 0x17 */
            check_cop2x(ctx);
            generate_cle(r16, r11, r6);
            opn = "cle";
            break;
        case OPC_CLTU_NI: /* 0x18 */
            check_cop2x(ctx);
            generate_cltu(r16, r11, r6);
            opn = "cltu";
            break;
        case OPC_CLEU_NI: /* 0x19 */
            check_cop2x(ctx);
            generate_cleu(r16, r11, r6);
            opn = "cleu";
            break;
        case OPC_CEXEQ_NI: /* 0x1a */
            check_cop2x(ctx);
            generate_cexeq(r16, r11, r6);
            opn = "cexeq";
            break;
        case OPC_CMOVZ_NI: /* 0x1b */
            check_cop2x(ctx);
            generate_cmovz(r16, r11, r6);
            opn = "cmovz";
            break;
        case OPC_CMOVN_NI: /* 0x1c */
            check_cop2x(ctx);
            generate_cmovn(r16, r11, r6);
            opn = "cmovn";
            break;
        case OPC_CBUILDCAP_NI: /* 0x1d */
            check_cop2x(ctx);
            generate_cbuildcap(r16, r11, r6);
            opn = "cbuildcap";
            break;
        case OPC_CCOPYTYPE_NI: /* 0x1e */
            check_cop2x(ctx);
            generate_ccopytype(r16, r11, r6);
            opn = "ccopytype";
            break;
        case OPC_CCSEAL_NI: /* 0x1f */
            check_cop2x(ctx);
            generate_ccseal(r16, r11, r6);
        case OPC_CTESTSUBSET_NI: /* 0x20 */
            check_cop2x(ctx);
            generate_ctestsubset(r16, r11, r6);
            opn = "ctestsubset";
            break;
        case OPC_CNEXEQ_NI: /* 0x21 */
            check_cop2x(ctx);
            generate_cnexeq(r16, r11, r6);
            opn = "cnexeq";
            break;
        /* Two-operand cap instructions. */
        case OPC_C2OPERAND_NI:         /* 0x3f */
            switch(MASK_CAP7(opc)) {
            case OPC_CGETPERM_NI:   /* 0x00 << 6 */
                check_cop2x(ctx);
                generate_cgetperm(r16, r11);
                opn = "cgetperm";
                break;
            case OPC_CGETTYPE_NI:   /* 0x01 << 6 */
                check_cop2x(ctx);
                generate_cgettype(r16, r11);
                opn = "cgettype";
                break;
            case OPC_CGETBASE_NI:   /* 0x02 << 6 */
                check_cop2x(ctx);
                generate_cgetbase(r16, r11);
                opn = "cgetbase";
                break;
            case OPC_CGETLEN_NI:    /* 0x03 << 6 */
                check_cop2x(ctx);
                generate_cgetlen(r16, r11);
                opn = "cgetlen";
                break;
            case OPC_CGETTAG_NI:    /* 0x04 << 6 */
                check_cop2x(ctx);
                generate_cgettag(r16, r11);
                opn = "cgettag";
                break;
            case OPC_CGETSEALED_NI: /* 0x05 << 6 */
                check_cop2x(ctx);
                generate_cgetsealed(r16, r11);
                opn = "cgetsealed";
                break;
            case OPC_CGETOFFSET_NI: /* 0x06 << 6 */
                check_cop2x(ctx);
                generate_cgetoffset(r16, r11);
                opn = "cgetoffset";
                break;
            case OPC_CGETPCCSETOFF_NI: /* 0x07 << 6 */
                check_cop2x(ctx);
                generate_cgetpccsetoffset(r16, r11);
                opn = "cgetpccsetoffset";
                break;
            case OPC_CCHECKPERM_NI:    /* 0x08 << 6 */
                check_cop2x(ctx);
                generate_ccheckperm(r16, r11);
                opn = "ccheckperm";
                break;
            case OPC_CCHECKTYPE_NI:    /* 0x09 << 6 */
                check_cop2x(ctx);
                generate_cchecktype(r16, r11);
                opn = "cchecktype";
                break;
            case OPC_CMOVE_NI:      /* 0x0a << 6 */
                check_cop2x(ctx);
                generate_cmove(r16, r11);
                opn = "cmove";
                break;
            case OPC_CCLEARTAG_NI:  /* 0x0b << 6 */
                check_cop2x(ctx);
                generate_ccleartag(r16, r11);
                opn = "ccleartag";
                break;
            case OPC_CJALR_NI:      /* 0x0c << 6 */
                check_cop2x(ctx);
                generate_cjalr(ctx, r16, r11);
                opn = "cjalr";
                break;
            case OPC_CREADHWR_NI:   /* 0x0d << 6 */
                check_cop2x(ctx);
                gen_helper_2_consti32(creadhwr, r16, r11);
                opn = "creadhwr";
                break;
            case OPC_CWRITEHWR_NI:  /* 0x0e << 6 */
                check_cop2x(ctx);
                gen_helper_2_consti32(cwritehwr, r16, r11);
                opn = "cwritehwr";
                break;
            case OPC_CGETADDR_NI:   /* 0x0f << 6 */
                check_cop2x(ctx);
                generate_cgetaddr(r16, r11);
                opn = "cgetaddr";
                break;

            /* One-operand cap instructions. */
            case OPC_C1OPERAND_NI:     /* 0x1f << 6 */
                switch(MASK_CAP8(opc)) {
                case OPC_CGETPCC_NI:    /* 0x00 << 11 */
                    check_cop2x(ctx);
                    generate_cgetpcc(r16);
                    opn = "cgetpcc";
                    break;
                case OPC_CGETCAUSE_NI:  /* 0x01 << 11 */
                    check_cop2x(ctx);
                    generate_cgetcause(r16);
                    opn = "cgetcause";
                    break;
                case OPC_CSETCAUSE_NI:  /* 0x02 << 11 */
                    check_cop2x(ctx);
                    generate_csetcause(r16);
                    opn = "csetcause";
                    break;
                case OPC_CJR_NI:        /* 0x03 << 11 */
                    check_cop2x(ctx);
                    generate_cjr(ctx, r16);
                    opn = "cjr";
                    break;

                default:
                    opn = "c1operand";
                    goto invalid;
                }
                break;

            default:
                opn = "c2operand";
                goto invalid;
            }
            break;

        default:
            opn = "cget";
            goto invalid;
        }
        break;
    case OPC_CSETBOUNDS: /* 0x01 */
        check_cop2x(ctx);
        generate_csetbounds(r16, r11, r6);
        opn = "csetbounds";
        break;
    case OPC_CSEAL:  /* 0x02 */
        check_cop2x(ctx);
        generate_cseal(r16, r11, r6);
        opn = "cseal";
        break;
    case OPC_CUNSEAL: /* 0x03 */
        check_cop2x(ctx);
        generate_cunseal(r16, r11, r6);
        opn = "cunseal";
        break;
    case OPC_CMISC: /* 0x04 */
        switch(MASK_CAP3(opc)) {
        case OPC_CANDPERM: /* 0x0 */
            check_cop2x(ctx);
            generate_candperm(r16, r11, r6);
            opn = "candperm";
            break;

        case OPC_CINCBASE: /* 0x2 */
            check_cop2x(ctx);
            generate_cincbase(r16, r11, r6);
            opn = "cincbase";
            break;
        case OPC_CSETLEN: /* 0x3 */
            check_cop2x(ctx);
            generate_csetlen(r16, r11, r6);
            opn = "csetlen";
            break;
        case OPC_CSETCAUSE: /* 0x4 */
            check_cop2x(ctx);
            generate_csetcause(r6);
            opn = "csetcause";
            break;
        case OPC_CCLEARTAG: /* 0x5 */
            check_cop2x(ctx);
            generate_ccleartag(r16, r11);
            opn = "ccleartag";
            break;
        case OPC_MTC2SEL6: /* 0x6 */
            {
                TCGv t0 = tcg_temp_new();

                gen_load_gpr(t0, r16);
                gen_mtc2(ctx, t0, r11, ctx->opcode & 0x7);
                tcg_temp_free(t0);
            }
            opn = "mtc2";
            break;
        case OPC_CFROMPTR: /* 0x7 */
            check_cop2x(ctx);
            generate_cfromptr(r16, r11, r6);
            opn = "cfromptr";
            break;
        default:
            opn = "cmisc";
            goto invalid;
        }
        break;
    case OPC_CCALL: /* 0x05 */
        switch(MASK_CCALL_SEL(opc)) {
        case CCALL_SELECTOR_CRETURN: /* 0x7ff */
            check_cop2x(ctx);
            opn = "creturn";
            generate_creturn();
            break;
        case CCALL_SELECTOR_0: /* 0x000 */
            check_cop2x(ctx);
            generate_ccall(r16, r11);
            opn = "ccall";
            break;
        case CCALL_SELECTOR_1: /* 0x001 */
            check_cop2x(ctx);
            generate_ccall_notrap(ctx, r16, r11);
            opn = "ccall";
            break;
        default:
            opn = "ccall";
            goto invalid;
        }
        break;
    case OPC_CRETURN: /* 0x06 */
        check_cop2x(ctx);
        generate_creturn();
        opn = "creturn";
        break;
    case OPC_CJALR: /* 0x07 */
        check_cop2x(ctx);
        generate_cjalr(ctx, r16, r11);
        opn = "cjalr";
        break;
    case OPC_CJR: /* 0x08 */
        check_cop2x(ctx);
        generate_cjr(ctx, r11);
        opn = "cjr";
        break;
    case OPC_CBTU: /* 0x09 */
        opn = "cbtu";
        check_cop2x(ctx);
        generate_cbtu(ctx, r16, (int16_t)(ctx->opcode));
        break;
    case OPC_CBTS: /* 0x0a */
        opn = "cbts";
        check_cop2x(ctx);
        generate_cbts(ctx, r16, (int16_t)(ctx->opcode));
        break;
    case OPC_CCHECK: /* 0x0b */
        switch(MASK_CAP3(opc)) {
        case OPC_CCHECKPERM: /* 0x0 */
            check_cop2x(ctx);
            generate_ccheckperm(r16, r6);
            opn = "ccheckperm";
            break;
        case OPC_CCHECKTYPE: /* 0x1 */
            check_cop2x(ctx);
            generate_cchecktype(r16, r11);
            opn = "cchecktype";
            break;
        default:
            opn = "ccheck";
            goto invalid;
        }
        break;
    case OPC_CTOPTR: /* 0x0c */
        check_cop2x(ctx);
        generate_ctoptr(r16, r11, r6);
        opn = "ctoptr";
        break;
    case OPC_COFFSET: /* 0x0d */
        switch(MASK_CAP3(opc)) {
        case OPC_CINCOFFSET: /* 0x0 */
            check_cop2x(ctx);
            generate_cincoffset(r16, r11, r6);
            opn = "cincoffset";
            break;
        case OPC_CSETOFFSET: /* 0x1 */
            check_cop2x(ctx);
            generate_csetoffset(r16, r11, r6);
            opn = "csetoffset";
            break;
        case OPC_CGETOFFSET: /* 0x2 */
            check_cop2x(ctx);
            generate_cgetoffset(r16, r11);
            opn = "cgetoffset";
            break;
        default:
            opn = "coffset";
            goto invalid;
        }
        break;
    case OPC_CPTRCMP: /* 0x0e */
        switch(MASK_CAP3(opc)) {
        case OPC_CEQ:  /* 0x0 */
            check_cop2x(ctx);
            generate_ceq(r16, r11, r6);
            opn = "ceq";
            break;
        case OPC_CNE:  /* 0x1 */
            check_cop2x(ctx);
            generate_cne(r16, r11, r6);
            opn = "cne";
            break;
        case OPC_CLT:  /* 0x2 */
            check_cop2x(ctx);
            generate_clt(r16, r11, r6);
            opn = "clt";
            break;
        case OPC_CLE:  /* 0x3 */
            check_cop2x(ctx);
            generate_cle(r16, r11, r6);
            opn = "cle";
            break;
        case OPC_CLTU: /* 0x4 */
            check_cop2x(ctx);
            generate_cltu(r16, r11, r6);
            opn = "cltu";
            break;
        case OPC_CLEU: /* 0x5 */
            check_cop2x(ctx);
            generate_cleu(r16, r11, r6);
            opn = "cleu";
            break;
        case OPC_CEXEQ: /* 0x6 */
            check_cop2x(ctx);
            generate_cexeq(r16, r11, r6);
            opn = "cexeq";
            break;
        case OPC_CNEXEQ: /* 0x7 */
            check_cop2x(ctx);
            generate_cnexeq(r16, r11, r6);
            opn = "cnexeq";
            break;
        default: /* Can't happen, because all possible values are allocated */
            opn = "cptrcmp";
            goto invalid;
        }
        break;
    case OPC_CCLEARREGS: /* 0x0f */
        opn = "cclearregs";
        check_cop2x(ctx);
        if (generate_cclearregs(ctx, r16, opc & 0xffff) != 0)
            goto invalid;
        break;
    case OPC_CLL:   /* 0x10 */
        /*
         * check_cop2x(ctx);
         */
        switch(MASK_CAP4(opc)) {
        case OPC_CSCB: /* 0x0 */
            check_cop2x(ctx);
            generate_cscb(ctx, r16, r11, r6);
            opn = "cscb";
            break;
        case OPC_CSCH: /* 0x1 */
            check_cop2x(ctx);
            generate_csch(ctx, r16, r11, r6);
            opn = "csch";
            break;
        case OPC_CSCW: /* 0x2 */
            check_cop2x(ctx);
            generate_cscw(ctx, r16, r11, r6);
            opn = "cscw";
            break;
        case OPC_CSCD: /* 0x3 */
            check_cop2x(ctx);
            generate_cscd(ctx, r16, r11, r6);
            opn = "cscd";
            break;

        case OPC_CSCC: /* 0x7 */
            check_cop2x(ctx);
            generate_cscc(ctx, r16, r11, r6);
            opn = "cscc";
            break;

        case OPC_CLLB: /* 0x8 */
            check_cop2x(ctx);
            generate_cllb(ctx, r16, r11);
            opn = "cllb";
            break;
        case OPC_CLLH: /* 0x9 */
            check_cop2x(ctx);
            generate_cllh(ctx, r16, r11);
            opn = "cllh";
            break;
        case OPC_CLLW: /* 0xa */
            check_cop2x(ctx);
            generate_cllw(ctx, r16, r11);
            opn = "cllw";
            break;
        case OPC_CLLD: /* 0xb */
            check_cop2x(ctx);
            generate_clld(ctx, r16, r11);
            opn = "clld";
            break;
        case OPC_CLLBU: /* 0xc */
            check_cop2x(ctx);
            generate_cllbu(ctx, r16, r11);
            opn = "cllbu";
            break;
        case OPC_CLLHU: /* 0xd */
            check_cop2x(ctx);
            generate_cllhu(ctx, r16, r11);
            opn = "cllhu";
            break;
        case OPC_CLLWU: /* 0xe */
            check_cop2x(ctx);
            generate_cllwu(ctx, r16, r11);
            opn = "cllwu";
            break;
        case OPC_CLLC: /* 0xf */
            check_cop2x(ctx);
            generate_cllc(ctx, r16, r11);
            opn = "cllc";
            break;

        default:
            opn = "cll";
            goto invalid;
        }
        break;
    case OPC_CBEZ_NI: /* 0x11 */
        check_cop2x(ctx);
        generate_cbez(ctx, r16, (int16_t)opc);
        opn = "cbez";
        break;
    case OPC_CBNZ_NI: /* 0x12 */
        check_cop2x(ctx);
        generate_cbnz(ctx, r16, (int16_t)opc);
        opn = "cbnz";
        break;
    case OPC_CINCOFFSETIMM_NI: /* 0x13 */
        check_cop2x(ctx);
        generate_cincoffset_imm(r16, r11, (opc & 0x7ff));
        opn = "cincoffsetimmediate";
        break;
    case OPC_CSETBOUNDSIMM_NI: /* 0x14 */
        check_cop2x(ctx);
        generate_csetbounds_imm(r16, r11, (opc & 0x7ff));
        opn = "csetboundsimmediate";
        break;

    default:
        goto invalid;
    }
    // FIXME: we can remove opn
    (void)opn; /* avoid a compiler warning */
    return;

invalid:
    MIPS_INVAL(opn);
    generate_exception (ctx, EXCP_RI);
    return;
}
#endif /* TARGET_CHERI */
