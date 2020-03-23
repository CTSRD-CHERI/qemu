/*
 * RISC-V Emulation Helpers for QEMU.
 *
 * Copyright (c) 2016-2017 Sagar Karandikar, sagark@eecs.berkeley.edu
 * Copyright (c) 2017-2018 SiFive, Inc.
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
#include "qemu/main-loop.h"
#include "exec/exec-all.h"
#include "exec/helper-proto.h"
#ifdef TARGET_CHERI
#include "cheri-helper-utils.h"
#endif


static inline const char* exception_str(uint32_t exception)
{
    // See Table 3.6 In privileged ISA spec (20190608-Priv-MSU-Ratified)
    switch(exception) {
    case RISCV_EXCP_INST_ADDR_MIS: return "Instruction address misaligned";
    case RISCV_EXCP_INST_ACCESS_FAULT: return "Instruction access fault";
    case RISCV_EXCP_ILLEGAL_INST: return "Illegal instruction";
    case RISCV_EXCP_BREAKPOINT: return "Breakpoint";
    case RISCV_EXCP_LOAD_ADDR_MIS: return "Load address misaligned";
    case RISCV_EXCP_LOAD_ACCESS_FAULT: return "Load access fault";
    case RISCV_EXCP_STORE_AMO_ADDR_MIS: return "Store/AMO address misaligned";
    case RISCV_EXCP_STORE_AMO_ACCESS_FAULT: return "Store/AMO access fault";
    case RISCV_EXCP_U_ECALL: return "Environment call from U-mode";
    case RISCV_EXCP_S_ECALL: return "Environment call from S-mode";
    case RISCV_EXCP_VS_ECALL: return "Environment call from H-mode";
    case RISCV_EXCP_M_ECALL: return "Environment call from M-mode";
    case RISCV_EXCP_INST_PAGE_FAULT: return "Instruction page fault";
    case RISCV_EXCP_LOAD_PAGE_FAULT: return "Load page fault";
    // 14 Reserved for future standard use
    case RISCV_EXCP_STORE_PAGE_FAULT: return "Store/AMO page fault";
    // 16–23 Reserved for future standard use
    case RISCV_EXCP_INST_GUEST_PAGE_FAULT: return "Guest instruction page fault";
    case RISCV_EXCP_LOAD_GUEST_ACCESS_FAULT: return "Guest load page fault";
    case RISCV_EXCP_STORE_GUEST_AMO_ACCESS_FAULT: return "Guest store/AMO page fault";
    // 24-31 Reserved for custom use (we use 28/0x1c) for CHERI
    case RISCV_EXCP_CHERI: return "CHERI fault";
    // 32–47 Reserved for future standard use
    // 48-63 Reserved for custom use
    // >64 Reserved for future standard use
    default: return "Unknown exception";
    }
}

/* Exceptions processing helpers */
void QEMU_NORETURN riscv_raise_exception(CPURISCVState *env,
                                          uint32_t exception, uintptr_t pc)
{
    CPUState *cs = env_cpu(env);
    qemu_log_mask(CPU_LOG_INT, "%s: %s (%d)\n", __func__, exception_str(exception), exception);
    if (pc != 0 && exception == RISCV_EXCP_ILLEGAL_INST) {
        // Try to fetch the faulting instruction and store it in badaddr
        uint32_t opcode = 0;
        int ret =
            cpu_memory_rw_debug(env_cpu(env), PC_ADDR(env), (uint8_t *)&opcode,
                                sizeof(opcode), /*is_write=*/false);
        opcode = tswap32(opcode); // FIXME is this needed?
        if (ret != 0) {
            warn_report("RISCV_EXCP_ILLEGAL_INST: Could not read %zu bytes at "
                        "vaddr 0x" TARGET_FMT_lx "\r\n",
                        sizeof(opcode), PC_ADDR(env));
        } else {
            env->badaddr = opcode;
        }
    }
    cs->exception_index = exception;
    cpu_loop_exit_restore(cs, pc);
}

void helper_raise_exception(CPURISCVState *env, uint32_t exception)
{
    riscv_raise_exception(env, exception, 0);
}

target_ulong helper_csrrw(CPURISCVState *env, target_ulong src,
        target_ulong csr)
{
    target_ulong val = 0;
    if (riscv_csrrw(env, csr, &val, src, -1, GETPC()) < 0) {
        riscv_raise_exception(env, RISCV_EXCP_ILLEGAL_INST, GETPC());
    }
    return val;
}

target_ulong helper_csrrs(CPURISCVState *env, target_ulong src,
        target_ulong csr, target_ulong rs1_pass)
{
    target_ulong val = 0;
    if (riscv_csrrw(env, csr, &val, -1, rs1_pass ? src : 0, GETPC()) < 0) {
        riscv_raise_exception(env, RISCV_EXCP_ILLEGAL_INST, GETPC());
    }
    return val;
}

target_ulong helper_csrrc(CPURISCVState *env, target_ulong src,
        target_ulong csr, target_ulong rs1_pass)
{
    target_ulong val = 0;
    if (riscv_csrrw(env, csr, &val, 0, rs1_pass ? src : 0, GETPC()) < 0) {
        riscv_raise_exception(env, RISCV_EXCP_ILLEGAL_INST, GETPC());
    }
    return val;
}

#ifndef CONFIG_USER_ONLY

target_ulong helper_sret(CPURISCVState *env, target_ulong cpu_pc_deb)
{
    target_ulong prev_priv, prev_virt, mstatus;

    if (!(env->priv >= PRV_S)) {
        riscv_raise_exception(env, RISCV_EXCP_ILLEGAL_INST, GETPC());
    }

    target_ulong retpc = GET_SPECIAL_REG(env, sepc, SEPCC);

    if (!riscv_has_ext(env, RVC) && (retpc & 0x3)) {
        riscv_raise_exception(env, RISCV_EXCP_INST_ADDR_MIS, GETPC());
    }

    if (env->priv_ver >= PRIV_VERSION_1_10_0 &&
        get_field(env->mstatus, MSTATUS_TSR) && !(env->priv >= PRV_M)) {
        riscv_raise_exception(env, RISCV_EXCP_ILLEGAL_INST, GETPC());
    }

    mstatus = env->mstatus;

    if (riscv_has_ext(env, RVH) && !riscv_cpu_virt_enabled(env)) {
        /* We support Hypervisor extensions and virtulisation is disabled */
        target_ulong hstatus = env->hstatus;

        prev_priv = get_field(mstatus, MSTATUS_SPP);
        prev_virt = get_field(hstatus, HSTATUS_SPV);

        hstatus = set_field(hstatus, HSTATUS_SPV,
                                 get_field(hstatus, HSTATUS_SP2V));
        mstatus = set_field(mstatus, MSTATUS_SPP,
                            get_field(hstatus, HSTATUS_SP2P));
        hstatus = set_field(hstatus, HSTATUS_SP2V, 0);
        hstatus = set_field(hstatus, HSTATUS_SP2P, 0);
        mstatus = set_field(mstatus, SSTATUS_SIE,
                            get_field(mstatus, SSTATUS_SPIE));
        mstatus = set_field(mstatus, SSTATUS_SPIE, 1);

        env->mstatus = mstatus;
        env->hstatus = hstatus;

        if (prev_virt) {
            riscv_cpu_swap_hypervisor_regs(env);
        }

        riscv_cpu_set_virt_enabled(env, prev_virt);
    } else {
        prev_priv = get_field(mstatus, MSTATUS_SPP);

        mstatus = set_field(mstatus,
            env->priv_ver >= PRIV_VERSION_1_10_0 ?
            MSTATUS_SIE : MSTATUS_UIE << prev_priv,
            get_field(mstatus, MSTATUS_SPIE));
        mstatus = set_field(mstatus, MSTATUS_SPIE, 1);
        mstatus = set_field(mstatus, MSTATUS_SPP, PRV_U);
        env->mstatus = mstatus;
    }

    riscv_cpu_set_mode(env, prev_priv);

#ifdef TARGET_CHERI
    env->PCC = env->SEPCC;
    qemu_log_mask_and_addr(CPU_LOG_INSTR, cpu_get_recent_pc(env),
                           "%s: Updating PCC from SEPCC: " PRINT_CAP_FMTSTR "\n",
                           __func__, PRINT_CAP_ARGS(&env->SEPCC));
#endif
    return retpc;
}

target_ulong helper_mret(CPURISCVState *env, target_ulong cpu_pc_deb)
{
    if (!(env->priv >= PRV_M)) {
        riscv_raise_exception(env, RISCV_EXCP_ILLEGAL_INST, GETPC());
    }

    target_ulong retpc = GET_SPECIAL_REG(env, mepc, MEPCC);
    if (!riscv_has_ext(env, RVC) && (retpc & 0x3)) {
        riscv_raise_exception(env, RISCV_EXCP_INST_ADDR_MIS, GETPC());
    }

    target_ulong mstatus = env->mstatus;
    target_ulong prev_priv = get_field(mstatus, MSTATUS_MPP);
    target_ulong prev_virt = MSTATUS_MPV_ISSET(env);
    mstatus = set_field(mstatus,
        env->priv_ver >= PRIV_VERSION_1_10_0 ?
        MSTATUS_MIE : MSTATUS_UIE << prev_priv,
        get_field(mstatus, MSTATUS_MPIE));
    mstatus = set_field(mstatus, MSTATUS_MPIE, 1);
    mstatus = set_field(mstatus, MSTATUS_MPP, PRV_U);
#ifdef TARGET_RISCV32
    env->mstatush = set_field(env->mstatush, MSTATUS_MPV, 0);
#else
    mstatus = set_field(mstatus, MSTATUS_MPV, 0);
#endif
    env->mstatus = mstatus;
    riscv_cpu_set_mode(env, prev_priv);

    if (riscv_has_ext(env, RVH)) {
        if (prev_virt) {
            riscv_cpu_swap_hypervisor_regs(env);
        }

        riscv_cpu_set_virt_enabled(env, prev_virt);
    }

#ifdef TARGET_CHERI
    env->PCC = env->MEPCC;
    qemu_log_mask_and_addr(CPU_LOG_INSTR, cpu_get_recent_pc(env),
                           "%s: Updating PCC from MEPCC: " PRINT_CAP_FMTSTR "\n",
                           __func__, PRINT_CAP_ARGS(&env->MEPCC));
#endif
    return retpc;
}

#ifdef CONFIG_MIPS_LOG_INSTR
void HELPER(log_gpr_write)(uint32_t regnum, target_ulong value, target_ulong pc)
{
    qemu_log_mask_and_addr(CPU_LOG_INSTR, pc,
                           "    Write %s = " TARGET_FMT_lx "\n",
                           riscv_int_regnames[regnum], value);
}
#endif

void helper_wfi(CPURISCVState *env)
{
    CPUState *cs = env_cpu(env);

    if ((env->priv == PRV_S &&
        env->priv_ver >= PRIV_VERSION_1_10_0 &&
        get_field(env->mstatus, MSTATUS_TW)) ||
        riscv_cpu_virt_enabled(env)) {
        riscv_raise_exception(env, RISCV_EXCP_ILLEGAL_INST, GETPC());
    } else {
        cs->halted = 1;
        cs->exception_index = EXCP_HLT;
        cpu_loop_exit(cs);
    }
}

void helper_tlb_flush(CPURISCVState *env)
{
    CPUState *cs = env_cpu(env);
    if (!(env->priv >= PRV_S) ||
        (env->priv == PRV_S &&
         env->priv_ver >= PRIV_VERSION_1_10_0 &&
         get_field(env->mstatus, MSTATUS_TVM))) {
        riscv_raise_exception(env, RISCV_EXCP_ILLEGAL_INST, GETPC());
    } else {
        tlb_flush(cs);
    }
}

#endif /* !CONFIG_USER_ONLY */
