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

typedef void(cheri_cget_helper)(TCGv, TCGv_ptr, TCGv_i32);
static inline bool gen_cheri_get(int rd, int cs, cheri_cget_helper *gen_func)
{
    TCGv_i32 source_regnum = tcg_const_i32(cs);
    TCGv result = tcg_temp_new();
    gen_func(result, cpu_env, source_regnum);
    gen_set_gpr(rd, result);
    tcg_temp_free(result);
    tcg_temp_free_i32(source_regnum);
    return true;
}

#define TRANSLATE_CGET(name)                                                   \
    static bool trans_##name(DisasContext *ctx, arg_##name *a)                 \
    {                                                                          \
        return gen_cheri_get(a->rd, a->rs1, &gen_helper_##name);               \
    }

typedef void(cheri_cap_cap_helper)(TCGv_ptr, TCGv_i32, TCGv_i32);
static inline bool gen_cheri_cap_cap(int cd, int cs,
                                     cheri_cap_cap_helper *gen_func)
{
    TCGv_i32 dest_regnum = tcg_const_i32(cd);
    TCGv_i32 source_regnum = tcg_const_i32(cs);
    gen_func(cpu_env, dest_regnum, source_regnum);
    tcg_temp_free_i32(source_regnum);
    tcg_temp_free_i32(dest_regnum);
    return true;
}

typedef void(cheri_cap_int_helper)(TCGv_ptr, TCGv_i32, TCGv);
static inline bool gen_cheri_cap_int(int cd, int rs,
                                     cheri_cap_int_helper *gen_func)
{
    TCGv_i32 dest_regnum = tcg_const_i32(cd);
    TCGv gpr_value = tcg_temp_new();
    gen_get_gpr(gpr_value, rs);
    gen_func(cpu_env, dest_regnum, gpr_value);
    tcg_temp_free(gpr_value);
    tcg_temp_free_i32(dest_regnum);
    return true;
}

#define INSN_CAN_TRAP(ctx) tcg_gen_movi_tl(cpu_pc, ctx->base.pc_next)

// TODO: all of these could be implemented in TCG without calling a helper
TRANSLATE_CGET(cgetaddr)
TRANSLATE_CGET(cgetbase)
TRANSLATE_CGET(cgetflags)
TRANSLATE_CGET(cgetlen)
TRANSLATE_CGET(cgetoffset)
TRANSLATE_CGET(cgetperm)
TRANSLATE_CGET(cgettag)
TRANSLATE_CGET(cgettype)
TRANSLATE_CGET(cgetsealed)

static bool trans_cmove(DisasContext *ctx, arg_cmove *a)
{
    return gen_cheri_cap_cap(a->rd, a->rs1, &gen_helper_cmove);
}

static bool trans_ccleartag(DisasContext *ctx, arg_ccleartag *a)
{
    return gen_cheri_cap_cap(a->rd, a->rs1, &gen_helper_ccleartag);
}

static bool trans_ccheckperm(DisasContext *ctx, arg_ccheckperm *a)
{
    INSN_CAN_TRAP(ctx); // Update env->pc for trap
    return gen_cheri_cap_int(a->rd, a->rs1, &gen_helper_ccheckperm);
}

static bool trans_cchecktype(DisasContext *ctx, arg_cchecktype *a)
{
    INSN_CAN_TRAP(ctx); // Update env->pc for trap
    return gen_cheri_cap_cap(a->rd, a->rs1, &gen_helper_cchecktype);
}
