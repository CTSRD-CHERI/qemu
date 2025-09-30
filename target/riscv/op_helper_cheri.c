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
#include "cheri_tagmem.h"
#ifndef TARGET_CHERI
#error TARGET_CHERI must be set
#endif

/* Raises an exception if the CSR access is not permitted. */
static void check_csr_cap_permissions(CPURISCVState *env, uint32_t csrno,
                                      bool write_access,
                                      riscv_csr_cap_ops *csr_cap_info,
                                      uintptr_t hostpc)
{
    RISCVException exc = riscv_csr_accessible(env, csrno, write_access);
    if (exc != RISCV_EXCP_NONE && (csr_cap_info->flags & CSR_OP_REQUIRE_CRE) &&
        !riscv_cpu_mode_cre(env)) {
        exc = RISCV_EXCP_ILLEGAL_INST;
    }
    if (exc == RISCV_EXCP_CHERI) {
        raise_cheri_exception_impl(env, CapEx_AccessSystemRegsViolation,
                                   CHERI_EXC_REGNUM_PCC, 0, true, hostpc);
    } else if (exc != RISCV_EXCP_NONE) {
        riscv_raise_exception(env, exc, hostpc);
    }
}

/* Copy a capability to a register, or update address if we are not capmode.*/
static inline void writeback_csrrw(CPUArchState *env, cap_register_t cap,
                                   uint32_t cd, riscv_csr_cap_ops *csr_cap_info)
{
    tcg_debug_assert(cd != 0);
    if (csr_cap_info->flags & CSR_OP_EXTENDED_REG) {
        /* Extended register -> reads return CLEN only in capmode */
        if (!cheri_in_capmode(env)) {
            update_capreg_to_intval(env, cd, cap_get_cursor(&cap));
        } else {
            update_capreg(env, cd, &cap);
        }
    } else {
        /* Newly added CLEN register -> all reads return CLEN */
        update_capreg(env, cd, &cap);
    }
}

void HELPER(csrrw_cap)(CPUArchState *env, uint32_t csr, uint32_t rd,
                       uint32_t rs1)
{
    riscv_csr_cap_ops *csr_cap_info = get_csr_cap_info(csr);
    assert(csr_cap_info);

    check_csr_cap_permissions(env, csr, true, csr_cap_info, GETPC());
    /* Read rs1 first since it could be clobbered by writeback_csrrw */
    cap_register_t rs_cap = *get_readonly_capreg(env, rs1);
    if (rd) {
        cap_register_t csr_cap = csr_cap_info->read(env, csr_cap_info);
        writeback_csrrw(env, csr_cap, rd, csr_cap_info);
    }
    /* CSRRW always performs the write operation even for rs1=zero. */
    csr_cap_info->write(env, csr_cap_info, rs_cap, cap_get_cursor(&rs_cap),
                        cheri_in_capmode(env));
}

static inline void do_csr_set_clear(CPUArchState *env, uint32_t csr,
                                    uint32_t rd, bool is_clear,
                                    target_ulong operand, bool perform_write,
                                    uintptr_t hostpc)
{
    riscv_csr_cap_ops *csr_cap_info = get_csr_cap_info(csr);

    assert(csr_cap_info);
    check_csr_cap_permissions(env, csr, perform_write, csr_cap_info, GETPC());
    /*
     * CSRR{S,C}* Always perform a read operation even for rd==0
     * https://riscv.github.io/riscv-isa-manual/snapshot/unprivileged/#csrinsts
     */
    cap_register_t csr_cap = csr_cap_info->read(env, csr_cap_info);

    if (perform_write) {
        target_ulong new_addr = cap_get_cursor(&csr_cap);
        if (is_clear) {
            new_addr &= ~operand;
        } else {
            new_addr |= operand;
        }
        csr_cap_info->write(env, csr_cap_info, csr_cap, new_addr, false);
    }
    if (rd) {
        writeback_csrrw(env, csr_cap, rd, csr_cap_info);
    }
}

void HELPER(csrrs_cap)(CPUArchState *env, uint32_t csr, uint32_t rd,
                       uint32_t rs1)
{
    target_ulong operand = get_capreg_cursor(env, rs1);
    do_csr_set_clear(env, csr, rd, /*is_clear=*/false, operand,
                     /*perform_write=*/rs1 != 0, GETPC());
}

void HELPER(csrrc_cap)(CPUArchState *env, uint32_t csr, uint32_t rd,
                       uint32_t rs1)
{
    target_ulong operand = get_capreg_cursor(env, rs1);
    do_csr_set_clear(env, csr, rd, /*is_clear=*/true, operand,
                     /*perform_write=*/rs1 != 0, GETPC());
}

void HELPER(csrrwi_cap)(CPUArchState *env, uint32_t csr, uint32_t rd,
                        uint32_t uimm)
{
    riscv_csr_cap_ops *csr_cap_info = get_csr_cap_info(csr);
    assert(csr_cap_info);

    check_csr_cap_permissions(env, csr, true, csr_cap_info, GETPC());
    /*
     * NOTE: according to spec read should not be visible for rd=0, but we
     * have to read it anyway to update the address.
     */
    cap_register_t csr_cap = csr_cap_info->read(env, csr_cap_info);
    /* CSRRWI always performs XLEN write operation even for rs1=zero. */
    csr_cap_info->write(env, csr_cap_info, csr_cap, uimm, false);
    if (rd) {
        writeback_csrrw(env, csr_cap, rd, csr_cap_info);
    }
}

void HELPER(csrrsi_cap)(CPUArchState *env, uint32_t csr, uint32_t rd,
                        uint32_t uimm)
{
    do_csr_set_clear(env, csr, rd, /*is_clear=*/false, uimm,
                     /*perform_write=*/uimm != 0, GETPC());
}

void HELPER(csrrci_cap)(CPUArchState *env, uint32_t csr, uint32_t rd,
                        uint32_t uimm)
{
    do_csr_set_clear(env, csr, rd, /*is_clear=*/true, uimm,
                     /*perform_write=*/uimm != 0, GETPC());
}

#ifdef TARGET_CHERI_RISCV_V9
static uint32_t csr_for_cspecialrw(enum CheriSCR scr)
{
    switch (scr) {
    case CheriSCR_PCC:
        return CSR_PCC;
    case CheriSCR_DDC:
        return CSR_DDC;
    case CheriSCR_STCC:
        return CSR_STVECC;
    case CheriSCR_MTCC:
        return CSR_MTVECC;
    case CheriSCR_VSTCC:
        return CSR_VSTVEC;
    case CheriSCR_SEPCC:
        return CSR_SEPCC;
    case CheriSCR_MEPCC:
        return CSR_MEPCC;
    case CheriSCR_VSEPCC:
        return CSR_VSEPCC;
    case CheriSCR_SScratchC:
        return CSR_SSCRATCHC;
    case CheriSCR_MScratchC:
        return CSR_MSCRATCHC;
    case CheriSCR_VSScratchC:
        return CSR_VSSCRATCHC;
    case CheriSCR_STDC:
        return CSR_STDC;
    case CheriSCR_MTDC:
        return CSR_MTDC;
    case CheriSCR_VSTDC:
        return CSR_VSTDC;
    case CheriSCR_UTIDC:
        return CSR_UTIDC;
    case CheriSCR_STIDC:
        return CSR_STIDC;
    case CheriSCR_MTIDC:
        return CSR_MTIDC;
    }
    assert(false);
}

void HELPER(cspecialrw)(CPUArchState *env, uint32_t cd, uint32_t cs,
                        uint32_t index)
{
    uintptr_t _host_return_address = GETPC();
    // Ensure that env->pcc.cursor is correct:
    cpu_restore_state(env_cpu(env), _host_return_address, false);

    assert(index <= 31 && "Bug in translator?");
    uint32_t csrno = csr_for_cspecialrw(index);
    riscv_csr_cap_ops *ops = get_csr_cap_info(csrno);
    assert(ops != NULL);
    bool is_write = cs != 0;
    bool is_read = cd != 0;
    check_csr_cap_permissions(env, csrno, is_write, ops, GETPC());

    // Make a copy of the write value in case cd == cs
    cap_register_t new_val = *get_readonly_capreg(env, cs);
    if (is_read) {
        cap_register_t csr_cap = ops->read(env, ops);
        writeback_csrrw(env, csr_cap, cd, ops);
    }
    if (is_write) {
        ops->write(env, ops, new_val, cap_get_cursor(&new_val), /*clen=*/true);
    }
}
#endif /* TARGET_CHERI_RISCV_V9 */

#ifdef DO_CHERI_STATISTICS
static DEFINE_CHERI_STAT(auipcc);
#endif

void HELPER(auipcc)(CPUArchState *env, uint32_t cd, target_ulong new_cursor)
{
    derive_cap_from_pcc(env, cd, new_cursor, GETPC(), OOB_INFO(auipcc));
}

void HELPER(cjal)(CPUArchState *env, uint32_t cd, target_ulong target_addr,
                  target_ulong link_addr)
{
    cheri_jump_and_link_checked(env, cd, link_addr, CHERI_EXC_REGNUM_PCC,
                                cheri_get_recent_pcc(env), target_addr,
                                0, GETPC());
}

void HELPER(modesw)(CPUArchState *env, int to_capmode)
{
    _Static_assert(CAP_FLAGS_ALL_BITS == 1, "Only one flag should exist");
    assert(cheri_in_capmode(env) != to_capmode &&
           "Should have skipped this call during translate");
    cap_set_exec_mode(&env->pcc,
                      to_capmode ? CHERI_EXEC_CAPMODE : CHERI_EXEC_INTMODE);
}

void HELPER(amoswap_cap)(CPUArchState *env, uint32_t dest_reg,
                         uint32_t addr_reg, uint32_t val_reg)
{
    uintptr_t _host_return_address = GETPC();
    assert(!qemu_tcg_mttcg_enabled() ||
           (cpu_in_exclusive_context(env_cpu(env)) &&
            "Should have raised EXCP_ATOMIC"));
    target_long addr = get_capreg_cursor(env, addr_reg);
    if (!cheri_in_capmode(env)) {
        addr = cheri_ddc_relative_addr(env, addr);
        addr_reg = CHERI_EXC_REGNUM_DDC;
    }
    const cap_register_t *cbp = get_load_store_base_cap(env, addr_reg);

    if (!cbp->cr_tag) {
        raise_cheri_exception(env, CapEx_TagViolation, addr_reg);
    } else if (!cap_is_unsealed(cbp)) {
        raise_cheri_exception(env, CapEx_SealViolation, addr_reg);
    } else if (!cap_has_perms(cbp, CAP_PERM_LOAD)) {
        raise_cheri_exception(env, CapEx_PermitLoadViolation, addr_reg);
    } else if (!cap_has_perms(cbp, CAP_PERM_STORE)) {
        raise_cheri_exception(env, CapEx_PermitStoreViolation, addr_reg);
    } else if (!cap_has_perms(cbp, CAP_PERM_STORE_CAP)) {
        raise_cheri_exception(env, CapEx_PermitStoreCapViolation, addr_reg);
    } else if (!cap_has_perms(cbp, CAP_PERM_STORE_LOCAL) &&
               get_capreg_tag(env, val_reg) &&
               !(get_capreg_hwperms(env, val_reg) & CAP_PERM_GLOBAL)) {
        raise_cheri_exception(env, CapEx_PermitStoreLocalCapViolation, val_reg);
    }

    if (!cap_is_in_bounds(cbp, addr, CHERI_CAP_SIZE)) {
        qemu_log_instr_or_mask_msg(
            env, CPU_LOG_INT,
            "Failed capability bounds check: addr=" TARGET_FMT_ld
            " base=" TARGET_FMT_lx " top=" TARGET_FMT_lx "\n",
            addr, cap_get_cursor(cbp), cap_get_top(cbp));
        raise_cheri_exception(env, CapEx_LengthViolation, addr_reg);
    } else if (!QEMU_IS_ALIGNED(addr, CHERI_CAP_SIZE)) {
        raise_unaligned_store_exception(env, addr, _host_return_address);
    }
    if (addr == env->load_res) {
        env->load_res = -1; // Invalidate LR/SC to the same address
    }
    // Load the value to store from the register file now in case the
    // load_cap_from_memory call overwrites that register
    target_ulong loaded_pesbt;
    target_ulong loaded_cursor;
    bool loaded_tag =
        load_cap_from_memory_raw(env, &loaded_pesbt, &loaded_cursor, addr_reg,
                                 cbp, addr, _host_return_address, NULL);
    // The store may still trap, so we must only update the dest register after
    // the store succeeded.
    store_cap_to_memory(env, val_reg, addr, _host_return_address);
    // Store succeeded -> we can update cd
    update_compressed_capreg(env, dest_reg, loaded_pesbt, loaded_tag,
                             loaded_cursor);
}

static void lr_c_impl(CPUArchState *env, uint32_t dest_reg, uint32_t auth_reg,
                      target_ulong addr, uintptr_t _host_return_address)
{
    assert(!qemu_tcg_mttcg_enabled() ||
           (cpu_in_exclusive_context(env_cpu(env)) &&
            "Should have raised EXCP_ATOMIC"));
    const cap_register_t *cbp = get_load_store_base_cap(env, auth_reg);
    if (!cbp->cr_tag) {
        raise_cheri_exception(env, CapEx_TagViolation, auth_reg);
    } else if (!cap_is_unsealed(cbp)) {
        raise_cheri_exception(env, CapEx_SealViolation, auth_reg);
    } else if (!cap_has_perms(cbp, CAP_PERM_LOAD)) {
        raise_cheri_exception(env, CapEx_PermitLoadViolation, auth_reg);
    }

    if (!cap_is_in_bounds(cbp, addr, CHERI_CAP_SIZE)) {
        qemu_log_instr_or_mask_msg(
            env, CPU_LOG_INT,
            "Failed capability bounds check: addr=" TARGET_FMT_ld
            " base=" TARGET_FMT_lx " top=" TARGET_FMT_lx "\n",
            addr, cap_get_cursor(cbp), cap_get_top(cbp));
        raise_cheri_exception(env, CapEx_LengthViolation, auth_reg);
    } else if (!QEMU_IS_ALIGNED(addr, CHERI_CAP_SIZE)) {
        raise_unaligned_store_exception(env, addr, _host_return_address);
    }
    target_ulong pesbt;
    target_ulong cursor;
    bool tag = load_cap_from_memory_raw(env, &pesbt, &cursor, auth_reg, cbp,
                                        addr, _host_return_address, NULL);
    // If this didn't trap, update the lr state:
    env->load_res = addr;
    env->load_val = cursor;
    env->load_pesbt = pesbt;
    env->load_tag = tag;
    log_changed_special_reg(env, "load_res", env->load_res, ~0u, 0);
    log_changed_special_reg(env, "load_val", env->load_val, ~0u, 0);
    log_changed_special_reg(env, "load_pesbt", env->load_pesbt, ~0u, 0);
    log_changed_special_reg(env, "load_tag", (target_ulong)env->load_tag, ~0u,
                            0);
    update_compressed_capreg(env, dest_reg, pesbt, tag, cursor);
}

void HELPER(lr_c_modedep)(CPUArchState *env, uint32_t dest_reg, uint32_t addr_reg)
{
    target_ulong addr = get_capreg_cursor(env, addr_reg);
    if (!cheri_in_capmode(env)) {
        addr = cheri_ddc_relative_addr(env, addr);
        addr_reg = CHERI_EXC_REGNUM_DDC;
    }
    lr_c_impl(env, dest_reg, addr_reg, addr, GETPC());
}

void HELPER(lr_c_ddc)(CPUArchState *env, uint32_t dest_reg, uint32_t addr_reg)
{
    target_ulong addr =
        cheri_ddc_relative_addr(env, get_capreg_cursor(env, addr_reg));
    lr_c_impl(env, dest_reg, CHERI_EXC_REGNUM_DDC, addr, GETPC());
}

void HELPER(lr_c_cap)(CPUArchState *env, uint32_t dest_reg, uint32_t addr_reg)
{
    target_ulong addr = get_capreg_cursor(env, addr_reg);
    lr_c_impl(env, dest_reg, addr_reg, addr, GETPC());
}

// SC returns zero on success, one on failure
static target_ulong sc_c_impl(CPUArchState *env, uint32_t addr_reg,
                              uint32_t val_reg, target_ulong addr,
                              uintptr_t _host_return_address)
{
    assert(!qemu_tcg_mttcg_enabled() ||
        (cpu_in_exclusive_context(env_cpu(env)) &&
            "Should have raised EXCP_ATOMIC"));
    const cap_register_t *cbp = get_load_store_base_cap(env, addr_reg);

    if (!cbp->cr_tag) {
        raise_cheri_exception(env, CapEx_TagViolation, addr_reg);
    } else if (!cap_is_unsealed(cbp)) {
        raise_cheri_exception(env, CapEx_SealViolation, addr_reg);
    } else if (!cap_has_perms(cbp, CAP_PERM_STORE)) {
        raise_cheri_exception(env, CapEx_PermitStoreViolation, addr_reg);
    } else if (!cap_has_perms(cbp, CAP_PERM_STORE_CAP)) {
        raise_cheri_exception(env, CapEx_PermitStoreCapViolation, addr_reg);
    } else if (!cap_has_perms(cbp, CAP_PERM_STORE_LOCAL) &&
               get_capreg_tag(env, val_reg) &&
               !(get_capreg_hwperms(env, val_reg) & CAP_PERM_GLOBAL)) {
        raise_cheri_exception(env, CapEx_PermitStoreLocalCapViolation, val_reg);
    }

    if (!cap_is_in_bounds(cbp, addr, CHERI_CAP_SIZE)) {
        qemu_log_instr_or_mask_msg(
            env, CPU_LOG_INT,
            "Failed capability bounds check: addr=" TARGET_FMT_ld
            " base=" TARGET_FMT_lx " top=" TARGET_FMT_lx "\n",
            addr, cap_get_cursor(cbp), cap_get_top(cbp));
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
    log_changed_special_reg(env, "load_res", env->load_res, ~0u, 0);
    if (addr != expected_addr) {
        goto sc_failed;
    }
    // Now perform the "cmpxchg" operation by checking if the current values
    // in memory are the same as the ones that the load-reserved observed.
    // FIXME: There is a bug here. If the MMU / Cap Permissions squash the tag,
    // we may think the location has changed when it has not.
    // Use load_cap_from_memory_128_raw_tag to get the real tag, and strip the
    // LOAD_CAP permission to ensure no MMU load faults occur
    // (this is not a real load).
    target_ulong current_pesbt;
    target_ulong current_cursor;
#ifdef CONFIG_RVFI_DII
    /* The read that is part of the cmpxchg should not be visible in traces. */
    uint32_t old_rmask = env->rvfi_dii_trace.MEM.rvfi_mem_rmask;
#endif
    bool current_tag =
        load_cap_from_memory_raw(env, &current_pesbt, &current_cursor, addr_reg,
                                 cbp, addr, _host_return_address, NULL);
#ifdef CONFIG_RVFI_DII
    /* The read that is part of the cmpxchg should not be visible in traces. */
    env->rvfi_dii_trace.MEM.rvfi_mem_rmask = old_rmask;
#endif
    if (current_cursor != env->load_val || current_pesbt != env->load_pesbt ||
        current_tag != env->load_tag) {
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

target_ulong HELPER(sc_c_modedep)(CPUArchState *env, uint32_t addr_reg,
                                  uint32_t val_reg)
{
    target_ulong addr = get_capreg_cursor(env, addr_reg);
    if (!cheri_in_capmode(env)) {
        addr = cheri_ddc_relative_addr(env, addr);
        addr_reg = CHERI_EXC_REGNUM_DDC;
    }
    return sc_c_impl(env, addr_reg, val_reg, addr, GETPC());
}

target_ulong HELPER(sc_c_ddc)(CPUArchState *env, uint32_t addr_reg,
                              uint32_t val_reg)
{
    target_ulong addr =
        cheri_ddc_relative_addr(env, get_capreg_cursor(env, addr_reg));
    return sc_c_impl(env, CHERI_EXC_REGNUM_DDC, val_reg, addr, GETPC());
}

target_ulong HELPER(sc_c_cap)(CPUArchState *env, uint32_t addr_reg,
                              uint32_t val_reg)
{
    target_ulong addr = get_capreg_cursor(env, addr_reg);
    return sc_c_impl(env, addr_reg, val_reg, addr, GETPC());
}

target_ulong HELPER(gcmode)(CPUArchState *env, uint32_t cs1)
{
    /*
     * The output in rd is 0 if the capability in cs1 does not have X-permission
     * set or the AP field cannot be produced by ACPERM; otherwise, the output
     * is 0 if cs1 's CHERI execution mode is Capability Pointer Mode or
     * 1 if the mode is Integer Pointer Mode.
     */
    const cap_register_t *cs1p = get_readonly_capreg(env, cs1);
    if (!cap_has_perms(cs1p, CAP_PERM_EXECUTE) ||
        cap_has_invalid_perms_encoding(cs1p)) {
        return 0;
    }
    return cap_get_exec_mode(cs1p) == CHERI_EXEC_CAPMODE ? 0 : 1;
}

void HELPER(scmode)(CPUArchState *env, uint32_t cd, uint32_t cs1,
                    target_ulong imm)
{
    cap_register_t result = *get_readonly_capreg(env, cs1);
    if (result.cr_tag && !cap_is_unsealed(&result)) {
        result.cr_tag = 0;
    }
    /* Mode is only updated if X is present and the permissions are valid. */
    if (cap_has_perms(&result, CAP_PERM_EXECUTE) &&
        !cap_has_invalid_perms_encoding(&result)) {
        CheriExecMode mode = imm & 1 ? CHERI_EXEC_INTMODE : CHERI_EXEC_CAPMODE;
        cap_set_exec_mode(&result, mode);
    }
    update_capreg(env, cd, &result);
}

target_ulong HELPER(scss)(CPUArchState *env, uint32_t cs1, uint32_t cs2)
{
    /*
     * V9's ctestsubset used ddc for cs1 == 0. This case was removed for scss,
     * we need no special handling for cs1 == 0.
     */
    const cap_register_t *cs1p = get_readonly_capreg(env, cs1);
    const cap_register_t *cs2p = get_readonly_capreg(env, cs2);
    if (!cs1p->cr_bounds_valid || !cs2p->cr_bounds_valid) {
        return 0;
    }
    if (cap_has_reserved_bits_set(cs1p) || cap_has_reserved_bits_set(cs2p)) {
        return 0;
    }
    if (cs1p->cr_tag != cs2p->cr_tag) {
        return 0;
    }
    /*
     * cs2's bounds must be equal to or a subset of cs1's
     * base1 <= base2, top2 <= top1
     */
    if (cap_get_base(cs1p) > cap_get_base(cs2p)) {
        return 0;
    }
    if (cap_get_top_full(cs2p) > cap_get_top_full(cs1p)) {
        return 0;
    }

    /* Explicitly verify that the permissions are valid. */
    if (cap_has_invalid_perms_encoding(cs1p) ||
        cap_has_invalid_perms_encoding(cs2p)) {
        return 0;
    }
    /* Return 0 if the permissions or level are not identical. */
    if ((cap_get_all_perms(cs2p) & cap_get_all_perms(cs1p)) !=
        cap_get_all_perms(cs2p)) {
        return 0;
    }

    return 1;
}
