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
    const char *name;
    //#define PRV_U 0
    //#define PRV_S 1
    //#define PRV_H 2 /* Reserved */
    //#define PRV_M 3
} scr_info[32] = {
    [CheriSCR_PCC] = {.r = true, .w = false, .access = U_Always, .name = "PCC"},
    [CheriSCR_DDC] = {.r = true, .w = true, .access = U_Always, .name = "DDC"},

    [CheriSCR_UTCC] = {.r = true, .w = true, .access = U_ASR, .name = "UTCC"},
    [CheriSCR_UTDC] = {.r = true, .w = true, .access = U_ASR, .name = "UTDC"},
    [CheriSCR_UScratchC] = {.r = true,
                            .w = true,
                            .access = U_ASR,
                            .name = "UScratchC"},
    [CheriSCR_UEPCC] = {.r = true, .w = true, .access = U_ASR, .name = "UEPCC"},

    [CheriSCR_STCC] = {.r = true, .w = true, .access = S_ASR, .name = "STCC"},
    [CheriSCR_STDC] = {.r = true, .w = true, .access = S_ASR, .name = "STDC"},
    [CheriSCR_SScratchC] = {.r = true,
                            .w = true,
                            .access = S_ASR,
                            .name = "SScratchC"},
    [CheriSCR_SEPCC] = {.r = true, .w = true, .access = S_ASR, .name = "SEPCC"},

    [CheriSCR_MTCC] = {.r = true, .w = true, .access = M_ASR, .name = "MTCC"},
    [CheriSCR_MTDC] = {.r = true, .w = true, .access = M_ASR, .name = "MTDC"},
    [CheriSCR_MScratchC] = {.r = true,
                            .w = true,
                            .access = M_ASR,
                            .name = "MScratchC"},
    [CheriSCR_MEPCC] = {.r = true, .w = true, .access = M_ASR, .name = "MEPCC"},
};

static inline cap_register_t *get_scr(CPUArchState *env, uint32_t index)
{
    switch (index) {
    case CheriSCR_PCC: return &env->PCC;
    case CheriSCR_DDC: return &env->DDC;

    case CheriSCR_UTCC: return &env->UTCC;
    case CheriSCR_UTDC: return &env->UTDC;
    case CheriSCR_UScratchC: return &env->UScratchC;
    case CheriSCR_UEPCC: return &env->UEPCC;

    case CheriSCR_STCC: return &env->STCC;
    case CheriSCR_STDC: return &env->STDC;
    case CheriSCR_SScratchC: return &env->SScratchC;
    case CheriSCR_SEPCC: return &env->SEPCC;

    case CheriSCR_MTCC: return &env->MTCC;
    case CheriSCR_MTDC: return &env->MTDC;
    case CheriSCR_MScratchC: return &env->MScratchC;
    case CheriSCR_MEPCC: return &env->MEPCC;
    default: assert(false && "Should have raised an invalid inst trap!");
    }
}

void HELPER(cspecialrw)(CPUArchState *env, uint32_t cd, uint32_t cs,
                        uint32_t index)
{
    uintptr_t _host_return_address = GETPC();
    // Ensure that env->PCC.cursor is correct:
    cpu_restore_state(env_cpu(env), _host_return_address, false);

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
        qemu_log_mask_and_addr(CPU_LOG_INSTR, cpu_get_recent_pc(env),
                               "  %s <- " PRINT_CAP_FMTSTR "\n",
                               scr_info[index].name, PRINT_CAP_ARGS(&new_val));
        if (index == CheriSCR_DDC && !new_val.cr_tag) {
            target_ulong recent_pc = cpu_get_recent_pc(env);
            qemu_log_mask_and_addr(
                CPU_LOG_INSTR | CPU_LOG_INT, recent_pc,
                "  Note: Installed untagged $ddc at " TARGET_FMT_lx "\n",
                recent_pc);
        }
        *scr = new_val;
        log_changed_capreg(env, scr_info[index].name, scr);
    }
}

#ifdef DO_CHERI_STATISTICS
static DEFINE_CHERI_STAT(auipcc);
#endif

void HELPER(auipcc)(CPUArchState *env, uint32_t cd, target_ulong new_cursor)
{
    derive_cap_from_pcc(env, cd, new_cursor, GETPC(), OOB_INFO(auipcc));
}

void HELPER(amoswap_cap)(CPUArchState *env, uint32_t dest_reg,
                         uint32_t addr_reg, uint32_t val_reg)
{
    uintptr_t _host_return_address = GETPC();
    assert(cpu_in_exclusive_context(env_cpu(env)) &&
           "Should have raised EXCP_ATOMIC");
    target_long offset = 0;
    if (!cheri_in_capmode(env)) {
        offset = get_capreg_cursor(env, addr_reg);
        addr_reg = CHERI_EXC_REGNUM_DDC;
    }
    const cap_register_t *cbp = get_load_store_base_cap(env, addr_reg);

    if (!cbp->cr_tag) {
        raise_cheri_exception(env, CapEx_TagViolation, addr_reg);
    } else if (!cap_is_unsealed(cbp)) {
        raise_cheri_exception(env, CapEx_SealViolation, addr_reg);
    } else if (!(cbp->cr_perms & CAP_PERM_LOAD)) {
        raise_cheri_exception(env, CapEx_PermitLoadViolation, addr_reg);
    } else if (!(cbp->cr_perms & CAP_PERM_STORE)) {
        raise_cheri_exception(env, CapEx_PermitStoreViolation, addr_reg);
    } else if (!(cbp->cr_perms & CAP_PERM_STORE_CAP)) {
        raise_cheri_exception(env, CapEx_PermitStoreCapViolation, addr_reg);
    } else if (!(cbp->cr_perms & CAP_PERM_STORE_LOCAL) &&
               get_capreg_tag(env, val_reg) &&
               !(get_capreg_hwperms(env, val_reg) & CAP_PERM_GLOBAL)) {
        raise_cheri_exception(env, CapEx_PermitStoreLocalCapViolation, val_reg);
    }

    uint64_t addr = (uint64_t)(cap_get_cursor(cbp) + (target_long)offset);
    if (!cap_is_in_bounds(cbp, addr, CHERI_CAP_SIZE)) {
        qemu_log_mask_and_addr(
            CPU_LOG_INSTR | CPU_LOG_INT, cpu_get_recent_pc(env),
            "Failed capability bounds check:"
            "offset=" TARGET_FMT_plx " cursor=" TARGET_FMT_plx
            " addr=" TARGET_FMT_plx "\n",
            offset, cap_get_cursor(cbp), addr);
        raise_cheri_exception(env, CapEx_LengthViolation, addr_reg);
    } else if (!QEMU_IS_ALIGNED(addr, CHERI_CAP_SIZE)) {
        raise_unaligned_store_exception(env, addr, _host_return_address);
    }
    if (addr == env->load_res) {
        env->load_res = -1; // Invalidate LR/SC to the same address
    }
    // Load the value to store from the register file now in case the
    // load_cap_from_memory call overwrites that register
    uint64_t loaded_pesbt;
    uint64_t loaded_cursor;
    bool loaded_tag =
        load_cap_from_memory_128(env, &loaded_pesbt, &loaded_cursor, addr_reg,
                                 cbp, addr, _host_return_address, NULL);
    // The store may still trap, so we must only update the dest register after
    // the store succeeded.
    store_cap_to_memory(env, val_reg, addr, _host_return_address);
    // Store succeeded -> we can update cd
    update_compressed_capreg(env, dest_reg, loaded_pesbt, loaded_tag,
                             loaded_cursor);
}

static void lr_c_impl(CPUArchState *env, uint32_t dest_reg, uint32_t addr_reg,
                      target_long offset, uintptr_t _host_return_address)
{
    assert(cpu_in_exclusive_context(env_cpu(env)) &&
        "Should have raised EXCP_ATOMIC");
    const cap_register_t *cbp = get_load_store_base_cap(env, addr_reg);
    if (!cbp->cr_tag) {
        raise_cheri_exception(env, CapEx_TagViolation, addr_reg);
    } else if (!cap_is_unsealed(cbp)) {
        raise_cheri_exception(env, CapEx_SealViolation, addr_reg);
    } else if (!(cbp->cr_perms & CAP_PERM_LOAD)) {
        raise_cheri_exception(env, CapEx_PermitLoadViolation, addr_reg);
    }

    uint64_t addr = (uint64_t)(cap_get_cursor(cbp) + (target_long)offset);
    if (!cap_is_in_bounds(cbp, addr, CHERI_CAP_SIZE)) {
        qemu_log_mask_and_addr(
            CPU_LOG_INSTR | CPU_LOG_INT, cpu_get_recent_pc(env),
            "Failed capability bounds check:"
            "offset=" TARGET_FMT_plx " cursor=" TARGET_FMT_plx
            " addr=" TARGET_FMT_plx "\n",
            offset, cap_get_cursor(cbp), addr);
        raise_cheri_exception(env, CapEx_LengthViolation, addr_reg);
    } else if (!QEMU_IS_ALIGNED(addr, CHERI_CAP_SIZE)) {
        raise_unaligned_store_exception(env, addr, _host_return_address);
    }
    target_ulong pesbt;
    target_ulong cursor;
    bool tag = load_cap_from_memory_128(env, &pesbt, &cursor, addr_reg, cbp,
                                        addr, _host_return_address, NULL);
    // If this didn't trap, update the lr state:
    env->load_res = addr;
    env->load_val = cursor;
    env->load_pesbt = pesbt;
    env->load_tag = tag;
    update_compressed_capreg(env, dest_reg, pesbt, tag, cursor);
}

void HELPER(lr_c_modedep)(CPUArchState *env, uint32_t dest_reg, uint32_t addr_reg)
{
    target_long offset = 0;
    if (!cheri_in_capmode(env)) {
        offset = get_capreg_cursor(env, addr_reg);
        addr_reg = CHERI_EXC_REGNUM_DDC;
    }
    lr_c_impl(env, dest_reg, addr_reg, offset, GETPC());
}

void HELPER(lr_c_ddc)(CPUArchState *env, uint32_t dest_reg, uint32_t addr_reg)
{
    target_long offset = get_capreg_cursor(env, addr_reg);
    lr_c_impl(env, dest_reg, CHERI_EXC_REGNUM_DDC, offset, GETPC());
}

void HELPER(lr_c_cap)(CPUArchState *env, uint32_t dest_reg, uint32_t addr_reg)
{
    lr_c_impl(env, dest_reg, addr_reg, /*offset=*/0, GETPC());
}

// SC returns zero on success, one on failure
static target_ulong sc_c_impl(CPUArchState *env, uint32_t addr_reg,
                              uint32_t val_reg, target_ulong offset,
                              uintptr_t _host_return_address)
{
    assert(cpu_in_exclusive_context(env_cpu(env)) &&
           "Should have raised EXCP_ATOMIC");
    const cap_register_t *cbp = get_load_store_base_cap(env, addr_reg);

    if (!cbp->cr_tag) {
        raise_cheri_exception(env, CapEx_TagViolation, addr_reg);
    } else if (!cap_is_unsealed(cbp)) {
        raise_cheri_exception(env, CapEx_SealViolation, addr_reg);
    } else if (!(cbp->cr_perms & CAP_PERM_STORE)) {
        raise_cheri_exception(env, CapEx_PermitStoreViolation, addr_reg);
    } else if (!(cbp->cr_perms & CAP_PERM_STORE_CAP)) {
        raise_cheri_exception(env, CapEx_PermitStoreCapViolation, addr_reg);
    } else if (!(cbp->cr_perms & CAP_PERM_STORE_LOCAL) &&
               get_capreg_tag(env, val_reg) &&
               !(get_capreg_hwperms(env, val_reg) & CAP_PERM_GLOBAL)) {
        raise_cheri_exception(env, CapEx_PermitStoreLocalCapViolation, val_reg);
    }

    uint64_t addr = (uint64_t)(cap_get_cursor(cbp) + (target_long)offset);
    if (!cap_is_in_bounds(cbp, addr, CHERI_CAP_SIZE)) {
        qemu_log_mask_and_addr(
            CPU_LOG_INSTR | CPU_LOG_INT, cpu_get_recent_pc(env),
            "Failed capability bounds check:"
            "offset=" TARGET_FMT_plx " cursor=" TARGET_FMT_plx
            " addr=" TARGET_FMT_plx "\n",
            offset, cap_get_cursor(cbp), addr);
        raise_cheri_exception(env, CapEx_LengthViolation, addr_reg);
    } else if (!QEMU_IS_ALIGNED(addr, CHERI_CAP_SIZE)) {
        raise_unaligned_store_exception(env, addr, _host_return_address);
    }

    // Save the expected addr
    const target_ulong expected_addr = env->load_res;
    // Clear the load reservation, since an SC must fail if there is
    // an SC to any address, in between an LR and SC pair.
    // We do this regardless of success/failure.
    env->load_res = -1;
    if (addr != expected_addr) {
        goto sc_failed;
    }
    // Now perform the "cmpxchg" operation
    if (get_capreg_cursor(env, val_reg) != env->load_val ||
        get_capreg_pesbt(env, val_reg) != env->load_pesbt ||
        get_capreg_tag(env, val_reg) != env->load_tag) {
        goto sc_failed;
    }
    // This store may still trap, so we should update env->load_res before
    store_cap_to_memory(env, val_reg, addr, _host_return_address);
    tcg_debug_assert(env->load_res == -1);
    return 0; // success
sc_failed:
    tcg_debug_assert(env->load_res == -1);
    return 1; // failure
}

target_ulong HELPER(sc_c_modedep)(CPUArchState *env, uint32_t addr_reg, uint32_t val_reg)
{
    target_long offset = 0;
    if (!cheri_in_capmode(env)) {
        offset = get_capreg_cursor(env, addr_reg);
        addr_reg = CHERI_EXC_REGNUM_DDC;
    }
    return sc_c_impl(env, addr_reg, val_reg, offset, GETPC());
}

target_ulong HELPER(sc_c_ddc)(CPUArchState *env, uint32_t addr_reg, uint32_t val_reg)
{
    target_long offset = get_capreg_cursor(env, addr_reg);
    return sc_c_impl(env, CHERI_EXC_REGNUM_DDC, val_reg, offset, GETPC());
}

target_ulong HELPER(sc_c_cap)(CPUArchState *env, uint32_t addr_reg, uint32_t val_reg)
{
    return sc_c_impl(env, addr_reg, val_reg, /*offset=*/0, GETPC());
}
