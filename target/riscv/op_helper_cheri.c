/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2020 Alex Richardson
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
#include "qemu/log.h"
#include "cpu.h"
#include "qemu/main-loop.h"
#include "exec/exec-all.h"
#include "exec/log.h"
#include "exec/helper-proto.h"
#include "cheri-helper-utils.h"
#ifndef TARGET_CHERI
#error TARGET_CHERI must be set
#endif

enum SCRAccessMode {
    SCR_Invalid = 0,
    ASR_Flag = 1,
    U_Always = (PRV_U + 1) << 1,
    U_ASR = U_Always | ASR_Flag,
    S_Always = (PRV_S + 1) << 1,
    S_ASR = S_Always | ASR_Flag,
    M_Always = (PRV_M + 1) << 1,
    M_ASR = M_Always | ASR_Flag,
};

static inline int scr_min_priv(enum SCRAccessMode mode)
{
    return ((int)mode >> 1) - 1;
}
static inline int scr_needs_asr(enum SCRAccessMode mode)
{
    return (mode & ASR_Flag) == ASR_Flag;
}

struct SCRInfo {
    bool r;
    bool w;
    enum SCRAccessMode access; /* Default = Invalid */
    const char* name;
    //#define PRV_U 0
    //#define PRV_S 1
    //#define PRV_H 2 /* Reserved */
    //#define PRV_M 3
} scr_info[32] = {
    [CheriSCR_PCC] = {.r = true, .w = false, .access = U_Always, .name = "PCC"},
    [CheriSCR_DDC] = {.r = true, .w = true, .access = U_Always, .name = "DDC"},

    [CheriSCR_UTCC] = {.r = true, .w = true, .access = U_ASR, .name = "UTCC"},
    [CheriSCR_UTDC] = {.r = true, .w = true, .access = U_ASR, .name = "UTDC"},
    [CheriSCR_UScratchC] = {.r = true, .w = true, .access = U_ASR, .name = "UScratchC"},
    [CheriSCR_UEPCC] = {.r = true, .w = true, .access = U_ASR, .name = "UEPCC"},

    [CheriSCR_STCC] = {.r = true, .w = true, .access = S_ASR, .name = "STCC"},
    [CheriSCR_STDC] = {.r = true, .w = true, .access = S_ASR, .name = "STDC"},
    [CheriSCR_SScratchC] = {.r = true, .w = true, .access = S_ASR, .name = "SScratchC"},
    [CheriSCR_SEPCC] = {.r = true, .w = true, .access = S_ASR, .name = "SEPCC"},

    [CheriSCR_MTCC] = {.r = true, .w = true, .access = M_ASR, .name = "MTCC"},
    [CheriSCR_MTDC] = {.r = true, .w = true, .access = M_ASR, .name = "MTDC"},
    [CheriSCR_MScratchC] = {.r = true, .w = true, .access = M_ASR, .name = "MScratchC"},
    [CheriSCR_MEPCC] = {.r = true, .w = true, .access = M_ASR, .name = "MEPCC"},
};

static inline cap_register_t *get_scr(CPUArchState *env, uint32_t index)
{
    switch (index) {
    case CheriSCR_PCC:
        return &env->PCC;
    case CheriSCR_DDC:
        return &env->DDC;

    case CheriSCR_UTCC:
        return &env->UTCC;
    case CheriSCR_UTDC:
        return &env->UTDC;
    case CheriSCR_UScratchC:
        return &env->UScratchC;
    case CheriSCR_UEPCC:
        return &env->UEPCC;

    case CheriSCR_STCC:
        return &env->STCC;
    case CheriSCR_STDC:
        return &env->STDC;
    case CheriSCR_SScratchC:
        return &env->SScratchC;
    case CheriSCR_SEPCC:
        return &env->SEPCC;

    case CheriSCR_MTCC:
        return &env->MTCC;
    case CheriSCR_MTDC:
        return &env->MTDC;
    case CheriSCR_MScratchC:
        return &env->MScratchC;
    case CheriSCR_MEPCC:
        return &env->MEPCC;
    default:
        assert(false && "Should have raised an invalid inst trap!");
    }
}

void HELPER(cspecialrw)(CPUArchState *env, uint32_t cd, uint32_t cs,
                        uint32_t index)
{
    uintptr_t _host_return_address = GETPC();
    // Ensure that env->PCC.cursor is correct:
    cpu_restore_state(env_cpu(env), GETPC(), false);

    assert(index <= 31 && "Bug in translator?");
    enum SCRAccessMode mode = scr_info[index].access;
    if (mode == SCR_Invalid) {
        riscv_raise_exception(env, RISCV_EXCP_ILLEGAL_INST,
                              _host_return_address);
    }
    // XXXAR: Raising Access_System_Registers for write to read-only SCR seems
    // odd to me
    if (cs != 0 && !scr_info[index].w) {
        raise_cheri_exception(env, CapEx_AccessSystemRegsViolation, 32 + index);
    }
    bool can_access_sysregs = cheri_have_access_sysregs(env);
    if (scr_needs_asr(mode) && !can_access_sysregs) {
        raise_cheri_exception(env, CapEx_AccessSystemRegsViolation, 32 + index);
    }
    if (scr_min_priv(mode) > env->priv) {
        raise_cheri_exception(env, CapEx_AccessSystemRegsViolation, 32 + index);
    }
    cap_register_t *scr = get_scr(env, index);
    // Make a copy of the write value in case cd == cs
    cap_register_t new_val = *get_readonly_capreg(env, cs);
    if (cd != 0) {
        assert(scr_info[index].r && "Bug? Should be readable");
        update_capreg(env, cd, scr);
    }
    if (cs != 0) {
        assert(scr_info[index].w && "Bug? Should be writable");
        qemu_log_mask(CPU_LOG_INSTR | CPU_LOG_INT, "  %s <- " PRINT_CAP_FMTSTR "\n", scr_info[index].name,
                      PRINT_CAP_ARGS(&new_val));
        if (index == CheriSCR_DDC && !new_val.cr_tag && !cheri_in_capmode(env)) {
            qemu_log_mask(CPU_LOG_INSTR | CPU_LOG_INT,
                          "  Note: Installed untagged $ddc (while not in "
                          "capmode) at " TARGET_FMT_lx "\n", PC_ADDR(env));
        }
        *scr = new_val;
    }
}

#ifdef DO_CHERI_STATISTICS
static DEFINE_CHERI_STAT(auipcc);
#endif

void HELPER(auipcc)(CPUArchState *env, uint32_t cd, target_ulong new_cursor)
{
    derive_cap_from_pcc(env, cd, new_cursor, GETPC(), OOB_INFO(auipcc));
}
