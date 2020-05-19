/*
 * RISC-V CPU helpers for qemu.
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
#include "qemu/main-loop.h"
#include "cpu.h"
#include "exec/exec-all.h"
#include "exec/log_instr.h"
#include "tcg/tcg-op.h"
#include "trace.h"
#include "disas/disas.h"
#include "helper_utils.h"
#ifdef TARGET_CHERI
#include "cheri-helper-utils.h"
#endif

#ifdef TARGET_CHERI
#define COPY_SPECIAL_REG(env, dst, cheri_dst, src, cheri_src)   \
    do {                                                        \
        (env)->cheri_dst = (env)->cheri_src;                    \
    } while (false)
#else
#define COPY_SPECIAL_REG(env, dst, cheri_dst, src, cheri_src)   \
    do {                                                        \
        (env)->dst = (env)->src;                                \
    } while (false)
#endif

#ifdef CONFIG_TCG_LOG_INSTR
#ifdef TARGET_CHERI
#define LOG_SPECIAL_REG(env, csrno, scrno)      \
    riscv_log_instr_scr_changed(env, scrno)
#else
#define LOG_SPECIAL_REG(env, csrno, scrno)      \
    riscv_log_instr_csr_changed(env, csrno);
#endif
#else /* !CONFIG_TCG_LOG_INSTR */
#define LOG_SPECIAL_REG(env, csrno, scrno) ((void)0)
#endif /* !CONFIG_TCG_LOG_INSTR */

int riscv_cpu_mmu_index(CPURISCVState *env, bool ifetch)
{
#ifdef CONFIG_USER_ONLY
    return 0;
#else
    return env->priv;
#endif
}

#ifndef CONFIG_USER_ONLY
static int riscv_cpu_local_irq_pending(CPURISCVState *env)
{
    target_ulong irqs;

    target_ulong mstatus_mie = get_field(env->mstatus, MSTATUS_MIE);
    target_ulong mstatus_sie = get_field(env->mstatus, MSTATUS_SIE);
    target_ulong hs_mstatus_sie = get_field(env->mstatus_hs, MSTATUS_SIE);

    target_ulong pending = env->mip & env->mie &
                               ~(MIP_VSSIP | MIP_VSTIP | MIP_VSEIP);
    target_ulong vspending = (env->mip & env->mie &
                              (MIP_VSSIP | MIP_VSTIP | MIP_VSEIP));

    target_ulong mie    = env->priv < PRV_M ||
                          (env->priv == PRV_M && mstatus_mie);
    target_ulong sie    = env->priv < PRV_S ||
                          (env->priv == PRV_S && mstatus_sie);
    target_ulong hs_sie = env->priv < PRV_S ||
                          (env->priv == PRV_S && hs_mstatus_sie);

    if (riscv_cpu_virt_enabled(env)) {
        target_ulong pending_hs_irq = pending & -hs_sie;

        if (pending_hs_irq) {
            riscv_cpu_set_force_hs_excep(env, FORCE_HS_EXCEP);
            return ctz64(pending_hs_irq);
        }

        pending = vspending;
    }

    irqs = (pending & ~env->mideleg & -mie) | (pending &  env->mideleg & -sie);

    if (irqs) {
        return ctz64(irqs); /* since non-zero */
    } else {
        return EXCP_NONE; /* indicates no pending interrupt */
    }
}
#endif

bool riscv_cpu_exec_interrupt(CPUState *cs, int interrupt_request)
{
#if !defined(CONFIG_USER_ONLY)
    if (interrupt_request & CPU_INTERRUPT_HARD) {
        RISCVCPU *cpu = RISCV_CPU(cs);
        CPURISCVState *env = &cpu->env;
        int interruptno = riscv_cpu_local_irq_pending(env);
        if (interruptno >= 0) {
            cs->exception_index = RISCV_EXCP_INT_FLAG | interruptno;
            riscv_cpu_do_interrupt(cs);
            return true;
        }
    }
#endif
    return false;
}

#if !defined(CONFIG_USER_ONLY)

/* Return true is floating point support is currently enabled */
bool riscv_cpu_fp_enabled(CPURISCVState *env)
{
    if (env->mstatus & MSTATUS_FS) {
        if (riscv_cpu_virt_enabled(env) && !(env->mstatus_hs & MSTATUS_FS)) {
            return false;
        }
        return true;
    }

    return false;
}

void riscv_cpu_swap_hypervisor_regs(CPURISCVState *env, bool hs_mode_trap)
{
    target_ulong mstatus_mask = MSTATUS_MXR | MSTATUS_SUM | MSTATUS_FS |
                                MSTATUS_SPP | MSTATUS_SPIE | MSTATUS_SIE;
    bool current_virt = riscv_cpu_virt_enabled(env);

    g_assert(riscv_has_ext(env, RVH));

#if defined(TARGET_RISCV64)
    mstatus_mask |= MSTATUS64_UXL;
#endif

    if (current_virt) {
        /* Current V=1 and we are about to change to V=0 */
        env->vsstatus = env->mstatus & mstatus_mask;
        env->mstatus &= ~mstatus_mask;
        env->mstatus |= env->mstatus_hs;
        riscv_log_instr_csr_changed(env, CSR_VSSTATUS);
        /* mstatus may be modified again by do_interrupt */

#if defined(TARGET_RISCV32)
        env->vsstatush = env->mstatush;
        env->mstatush |= env->mstatush_hs;
        /* TODO(am2419): CSR_VSSTATUSH does not exist, why? */
        /* riscv_log_instr_csr_changed(env, CSR_VSSTATUSH); */
        if (hs_mode_trap) {
            /* mstatush is modified again when trapping to M-mode */
            riscv_log_instr_csr_changed(env, CSR_MSTATUSH);
        }
#endif

        COPY_SPECIAL_REG(env, stvec, VSTCC, stvec, STCC);
        COPY_SPECIAL_REG(env, stvec, STCC, stvec_hs, STCC_HS);
        LOG_SPECIAL_REG(env, CSR_VSTVEC, CheriSCR_BSTCC);
        LOG_SPECIAL_REG(env, CSR_STVEC, CheriSCR_STCC);
        
        env->vsscratch = env->sscratch;
        env->sscratch = env->sscratch_hs;
        riscv_log_instr_csr_changed(env, CSR_VSSCRATCH);
        riscv_log_instr_csr_changed(env, CSR_SSCRATCH);

        COPY_SPECIAL_REG(env, vsepc, VSEPCC, sepc, SEPCC);
        COPY_SPECIAL_REG(env, sepc, SEPCC, sepc_hs, SEPCC_HS);
        LOG_SPECIAL_REG(env, CSR_VSEPC, CheriSCR_BSEPCC);
        LOG_SPECIAL_REG(env, CSR_SEPC, CheriSCR_SEPCC);

        env->vscause = env->scause;
        env->scause = env->scause_hs;
        riscv_log_instr_csr_changed(env, CSR_VSCAUSE);
        if (!hs_mode_trap) {
            /* scause is modified again when trapping to HS-mode */
            riscv_log_instr_csr_changed(env, CSR_SCAUSE);
        }

        env->vstval = env->sbadaddr;
        env->sbadaddr = env->stval_hs;
        riscv_log_instr_csr_changed(env, CSR_VSTVAL);
        if (!hs_mode_trap) {
            /* sbaddaddr is modified again when trapping to HS-mode */
            riscv_log_instr_csr_changed(env, CSR_SBADADDR);
        }

        env->vsatp = env->satp;
        env->satp = env->satp_hs;
        riscv_log_instr_csr_changed(env, CSR_VSATP);
        riscv_log_instr_csr_changed(env, CSR_SATP);
    } else {
        /* Current V=0 and we are about to change to V=1 */
        /*
         * TODO(am2419): Not sure whether we should log assignments to
         * *_hs register variants.
         */
        env->mstatus_hs = env->mstatus & mstatus_mask;
        env->mstatus &= ~mstatus_mask;
        env->mstatus |= env->vsstatus;

#if defined(TARGET_RISCV32)
        env->mstatush_hs = env->mstatush;
        env->mstatush |= env->vsstatush;
#endif

        COPY_SPECIAL_REG(env, stvec_hs, STCC_HS, stvec, STCC);
        COPY_SPECIAL_REG(env, stvec, STCC, vstvec, VSTCC);
        LOG_SPECIAL_REG(env, CSR_STVEC, CheriSCR_STCC);

        env->sscratch_hs = env->sscratch;
        env->sscratch = env->vsscratch;
        riscv_log_instr_csr_changed(env, CSR_SSCRATCH);

        COPY_SPECIAL_REG(env, sepc_hs, SEPCC_HS, sepc, SEPCC);
        COPY_SPECIAL_REG(env, sepc, SEPCC, vsepc, VSEPCC);
        LOG_SPECIAL_REG(env, CSR_SEPC, CheriSCR_SEPCC);

        env->scause_hs = env->scause;
        env->scause = env->vscause;
        riscv_log_instr_csr_changed(env, CSR_SCAUSE);

        env->stval_hs = env->sbadaddr;
        env->sbadaddr = env->vstval;
        riscv_log_instr_csr_changed(env, CSR_SBADADDR);

        env->satp_hs = env->satp;
        env->satp = env->vsatp;
        riscv_log_instr_csr_changed(env, CSR_SATP);
    }
}

bool riscv_cpu_virt_enabled(CPURISCVState *env)
{
    if (!riscv_has_ext(env, RVH)) {
        return false;
    }

    return get_field(env->virt, VIRT_ONOFF);
}

void riscv_cpu_set_virt_enabled(CPURISCVState *env, bool enable)
{
    if (!riscv_has_ext(env, RVH)) {
        return;
    }

    /* Flush the TLB on all virt mode changes. */
    if (get_field(env->virt, VIRT_ONOFF) != enable) {
        tlb_flush(env_cpu(env));
    }

    env->virt = set_field(env->virt, VIRT_ONOFF, enable);
}

bool riscv_cpu_force_hs_excep_enabled(CPURISCVState *env)
{
    if (!riscv_has_ext(env, RVH)) {
        return false;
    }

    return get_field(env->virt, FORCE_HS_EXCEP);
}

void riscv_cpu_set_force_hs_excep(CPURISCVState *env, bool enable)
{
    if (!riscv_has_ext(env, RVH)) {
        return;
    }

    env->virt = set_field(env->virt, FORCE_HS_EXCEP, enable);
}

int riscv_cpu_claim_interrupts(RISCVCPU *cpu, uint32_t interrupts)
{
    CPURISCVState *env = &cpu->env;
    if (env->miclaim & interrupts) {
        return -1;
    } else {
        env->miclaim |= interrupts;
        return 0;
    }
}

uint32_t riscv_cpu_update_mip(RISCVCPU *cpu, uint32_t mask, uint32_t value)
{
    CPURISCVState *env = &cpu->env;
    CPUState *cs = CPU(cpu);
    uint32_t old = env->mip;
    bool locked = false;

    if (!qemu_mutex_iothread_locked()) {
        locked = true;
        qemu_mutex_lock_iothread();
    }

    env->mip = (env->mip & ~mask) | (value & mask);

    if (env->mip) {
        cpu_interrupt(cs, CPU_INTERRUPT_HARD);
    } else {
        cpu_reset_interrupt(cs, CPU_INTERRUPT_HARD);
    }

    if (locked) {
        qemu_mutex_unlock_iothread();
    }

    return old;
}

void riscv_cpu_set_rdtime_fn(CPURISCVState *env, uint64_t (*fn)(void))
{
    env->rdtime_fn = fn;
}

void riscv_cpu_set_mode(CPURISCVState *env, target_ulong newpriv)
{
    if (newpriv > PRV_M) {
        g_assert_not_reached();
    }
    if (newpriv == PRV_H) {
        newpriv = PRV_U;
    }
    /* tlb_flush is unnecessary as mode is contained in mmu_idx */
    env->priv = newpriv;

    /*
     * Clear the load reservation - otherwise a reservation placed in one
     * context/process can be used by another, resulting in an SC succeeding
     * incorrectly. Version 2.2 of the ISA specification explicitly requires
     * this behaviour, while later revisions say that the kernel "should" use
     * an SC instruction to force the yielding of a load reservation on a
     * preemptive context switch. As a result, do both.
     */
    env->load_res = -1;

#ifdef CONFIG_TCG_LOG_INSTR
    qemu_log_instr_cpu_mode_t mode;

    switch (newpriv) {
    case PRV_M:
        mode = RISCV_LOG_INSTR_CPU_M;
        break;
    case PRV_S:
        mode = RISCV_LOG_INSTR_CPU_S;
        break;
    case PRV_H:
        mode = RISCV_LOG_INSTR_CPU_H;
        break;
    default:
        mode = RISCV_LOG_INSTR_CPU_U;
    }
    qemu_log_instr_mode_switch(env, mode, cpu_get_recent_pc(env));
#endif
}

/* get_physical_address - get the physical address for this virtual address
 *
 * Do a page table walk to obtain the physical address corresponding to a
 * virtual address. Returns 0 if the translation was successful
 *
 * Adapted from Spike's mmu_t::translate and mmu_t::walk
 *
 * @env: CPURISCVState
 * @physical: This will be set to the calculated physical address
 * @prot: The returned protection attributes
 * @addr: The virtual address to be translated
 * @access_type: The type of MMU access
 * @mmu_idx: Indicates current privilege level
 * @first_stage: Are we in first stage translation?
 *               Second stage is used for hypervisor guest translation
 * @two_stage: Are we going to perform two stage translation
 */
static int get_physical_address(CPURISCVState *env, hwaddr *physical,
                                int *prot, target_ulong addr,
                                int access_type, int mmu_idx,
                                bool first_stage, bool two_stage)
{
    /* NOTE: the env->pc value visible here will not be
     * correct, but the value visible to the exception handler
     * (riscv_cpu_do_interrupt) is correct */
    MemTxResult res;
    MemTxAttrs attrs = MEMTXATTRS_UNSPECIFIED;
    int mode = mmu_idx;
    bool use_background = false;
#ifdef CONFIG_RVFI_DII
    // For RVFI-DII we have to reject all memory accesses outside of the RAM
    // region (even if there is a valid ROM there)
    // However, we still have to allow MMU_INST_FETCH accesess since they are
    // triggered by tb_find().
    if (env->rvfi_dii_have_injected_insn) {
        if (access_type == MMU_INST_FETCH) {
            // Pretend we have a 1:1 mapping and never fail for instruction
            // fetches since the instruction is injected directly via
            // env->rvfi_dii_trace.rvfi_dii_insn
            *physical = addr;
            *prot = PAGE_EXEC;
            return TRANSLATE_SUCCESS;
        } else if (addr < RVFI_DII_RAM_START || addr >= RVFI_DII_RAM_END) {
            fprintf(stderr, "Rejecting memory access to " TARGET_FMT_lx
                    " since it is outside the RVFI-DII range\n", addr);
            return TRANSLATE_FAIL; // XXX: TRANSLATE_PMP_FAIL?
        }
    }
#endif

    /*
     * Check if we should use the background registers for the two
     * stage translation. We don't need to check if we actually need
     * two stage translation as that happened before this function
     * was called. Background registers will be used if the guest has
     * forced a two stage translation to be on (in HS or M mode).
     */
    if (mode == PRV_M && access_type != MMU_INST_FETCH) {
        if (get_field(env->mstatus, MSTATUS_MPRV)) {
            mode = get_field(env->mstatus, MSTATUS_MPP);

            if (riscv_has_ext(env, RVH) &&
                MSTATUS_MPV_ISSET(env)) {
                use_background = true;
            }
        }
    }

    if (mode == PRV_S && access_type != MMU_INST_FETCH &&
        riscv_has_ext(env, RVH) && !riscv_cpu_virt_enabled(env)) {
        if (get_field(env->hstatus, HSTATUS_SPRV)) {
            mode = get_field(env->mstatus, SSTATUS_SPP);
            use_background = true;
        }
    }

    if (first_stage == false) {
        /* We are in stage 2 translation, this is similar to stage 1. */
        /* Stage 2 is always taken as U-mode */
        mode = PRV_U;
    }

    if (mode == PRV_M || !riscv_feature(env, RISCV_FEATURE_MMU)) {
        *physical = addr;
        *prot = PAGE_READ | PAGE_WRITE | PAGE_EXEC;
        return TRANSLATE_SUCCESS;
    }

    *prot = 0;

    hwaddr base;
    int levels, ptidxbits, ptesize, vm, sum, mxr, widened;

    if (first_stage == true) {
        mxr = get_field(env->mstatus, MSTATUS_MXR);
    } else {
        mxr = get_field(env->vsstatus, MSTATUS_MXR);
    }

    if (env->priv_ver >= PRIV_VERSION_1_10_0) {
        if (first_stage == true) {
            if (use_background) {
                base = (hwaddr)get_field(env->vsatp, SATP_PPN) << PGSHIFT;
                vm = get_field(env->vsatp, SATP_MODE);
            } else {
                base = (hwaddr)get_field(env->satp, SATP_PPN) << PGSHIFT;
                vm = get_field(env->satp, SATP_MODE);
            }
            widened = 0;
        } else {
            base = (hwaddr)get_field(env->hgatp, HGATP_PPN) << PGSHIFT;
            vm = get_field(env->hgatp, HGATP_MODE);
            widened = 2;
        }
        sum = get_field(env->mstatus, MSTATUS_SUM);
        switch (vm) {
        case VM_1_10_SV32:
          levels = 2; ptidxbits = 10; ptesize = 4; break;
        case VM_1_10_SV39:
          levels = 3; ptidxbits = 9; ptesize = 8; break;
        case VM_1_10_SV48:
          levels = 4; ptidxbits = 9; ptesize = 8; break;
        case VM_1_10_SV57:
          levels = 5; ptidxbits = 9; ptesize = 8; break;
        case VM_1_10_MBARE:
            *physical = addr;
            *prot = PAGE_READ | PAGE_WRITE | PAGE_EXEC;
            return TRANSLATE_SUCCESS;
        default:
          g_assert_not_reached();
        }
    } else {
        widened = 0;
        base = (hwaddr)(env->sptbr) << PGSHIFT;
        sum = !get_field(env->mstatus, MSTATUS_PUM);
        vm = get_field(env->mstatus, MSTATUS_VM);
        switch (vm) {
        case VM_1_09_SV32:
          levels = 2; ptidxbits = 10; ptesize = 4; break;
        case VM_1_09_SV39:
          levels = 3; ptidxbits = 9; ptesize = 8; break;
        case VM_1_09_SV48:
          levels = 4; ptidxbits = 9; ptesize = 8; break;
        case VM_1_09_MBARE:
            *physical = addr;
            *prot = PAGE_READ | PAGE_WRITE | PAGE_EXEC;
            return TRANSLATE_SUCCESS;
        default:
          g_assert_not_reached();
        }
    }

    CPUState *cs = env_cpu(env);
    int va_bits = PGSHIFT + levels * ptidxbits + widened;
    target_ulong mask, masked_msbs;

    if (TARGET_LONG_BITS > (va_bits - 1)) {
        mask = (1L << (TARGET_LONG_BITS - (va_bits - 1))) - 1;
    } else {
        mask = 0;
    }
    masked_msbs = (addr >> (va_bits - 1)) & mask;

    if (masked_msbs != 0 && masked_msbs != mask) {
        return TRANSLATE_FAIL;
    }

    int ptshift = (levels - 1) * ptidxbits;
    int i;

#if !TCG_OVERSIZED_GUEST
restart:
#endif
    for (i = 0; i < levels; i++, ptshift -= ptidxbits) {
        target_ulong idx;
        if (i == 0) {
            idx = (addr >> (PGSHIFT + ptshift)) &
                           ((1 << (ptidxbits + widened)) - 1);
        } else {
            idx = (addr >> (PGSHIFT + ptshift)) &
                           ((1 << ptidxbits) - 1);
        }

        /* check that physical address of PTE is legal */
        hwaddr pte_addr;

        if (two_stage && first_stage) {
            hwaddr vbase;

            /* Do the second stage translation on the base PTE address. */
            get_physical_address(env, &vbase, prot, base, access_type,
                                 mmu_idx, false, true);

            pte_addr = vbase + idx * ptesize;
        } else {
            pte_addr = base + idx * ptesize;
        }

        if (riscv_feature(env, RISCV_FEATURE_PMP) &&
            !pmp_hart_has_privs(env, pte_addr, sizeof(target_ulong),
            1 << MMU_DATA_LOAD, PRV_S)) {
            return TRANSLATE_PMP_FAIL;
        }

#if defined(TARGET_RISCV32)
        target_ulong pte = address_space_ldl(cs->as, pte_addr, attrs, &res);
#elif defined(TARGET_RISCV64)
        target_ulong pte = address_space_ldq(cs->as, pte_addr, attrs, &res);
#endif
        if (res != MEMTX_OK) {
            return TRANSLATE_FAIL;
        }

        hwaddr ppn = pte >> PTE_PPN_SHIFT;

        if (!(pte & PTE_V)) {
            /* Invalid PTE */
            return TRANSLATE_FAIL;
        } else if (!(pte & (PTE_R | PTE_W | PTE_X))) {
            /* Inner PTE, continue walking */
            base = ppn << PGSHIFT;
        } else if ((pte & (PTE_R | PTE_W | PTE_X)) == PTE_W) {
            /* Reserved leaf PTE flags: PTE_W */
            return TRANSLATE_FAIL;
        } else if ((pte & (PTE_R | PTE_W | PTE_X)) == (PTE_W | PTE_X)) {
            /* Reserved leaf PTE flags: PTE_W + PTE_X */
            return TRANSLATE_FAIL;
        } else if ((pte & PTE_U) && ((mode != PRV_U) &&
                   (!sum || access_type == MMU_INST_FETCH))) {
            /* User PTE flags when not U mode and mstatus.SUM is not set,
               or the access type is an instruction fetch */
            return TRANSLATE_FAIL;
        } else if (!(pte & PTE_U) && (mode != PRV_S)) {
            /* Supervisor PTE flags when not S mode */
            return TRANSLATE_FAIL;
        } else if (ppn & ((1ULL << ptshift) - 1)) {
            /* Misaligned PPN */
            return TRANSLATE_FAIL;
        } else if (access_type == MMU_DATA_LOAD && !((pte & PTE_R) ||
                   ((pte & PTE_X) && mxr))) {
            /* Read access check failed */
            return TRANSLATE_FAIL;
        } else if (access_type == MMU_DATA_STORE && !(pte & PTE_W)) {
            /* Write access check failed */
            return TRANSLATE_FAIL;
        } else if (access_type == MMU_INST_FETCH && !(pte & PTE_X)) {
            /* Fetch access check failed */
            return TRANSLATE_FAIL;
        } else {
            /* if necessary, set accessed and dirty bits. */
            target_ulong updated_pte = pte | PTE_A |
                (access_type == MMU_DATA_STORE ? PTE_D : 0);

            /* Page table updates need to be atomic with MTTCG enabled */
            if (updated_pte != pte) {
                /*
                 * - if accessed or dirty bits need updating, and the PTE is
                 *   in RAM, then we do so atomically with a compare and swap.
                 * - if the PTE is in IO space or ROM, then it can't be updated
                 *   and we return TRANSLATE_FAIL.
                 * - if the PTE changed by the time we went to update it, then
                 *   it is no longer valid and we must re-walk the page table.
                 */
                MemoryRegion *mr;
                hwaddr l = sizeof(target_ulong), addr1;
                mr = address_space_translate(cs->as, pte_addr,
                    &addr1, &l, false, MEMTXATTRS_UNSPECIFIED);
                if (memory_region_is_ram(mr)) {
                    target_ulong *pte_pa =
                        qemu_map_ram_ptr(mr->ram_block, addr1);
#if TCG_OVERSIZED_GUEST
                    /* MTTCG is not enabled on oversized TCG guests so
                     * page table updates do not need to be atomic */
                    *pte_pa = pte = updated_pte;
#else
                    target_ulong old_pte =
                        atomic_cmpxchg(pte_pa, pte, updated_pte);
                    if (old_pte != pte) {
                        goto restart;
                    } else {
                        pte = updated_pte;
                    }
#endif
                } else {
                    /* misconfigured PTE in ROM (AD bits are not preset) or
                     * PTE is in IO space and can't be updated atomically */
                    return TRANSLATE_FAIL;
                }
            }

            /* for superpage mappings, make a fake leaf PTE for the TLB's
               benefit. */
            target_ulong vpn = addr >> PGSHIFT;
            if (i == 0) {
                *physical = ((ppn | (vpn & ((1L << (ptshift + widened)) - 1))) <<
                             PGSHIFT) | (addr & ~TARGET_PAGE_MASK);
            } else {
                *physical = ((ppn | (vpn & ((1L << ptshift) - 1))) << PGSHIFT) | (addr & ~TARGET_PAGE_MASK);
            }

            /* set permissions on the TLB entry */
            if ((pte & PTE_R) || ((pte & PTE_X) && mxr)) {
                *prot |= PAGE_READ;
            }
            if ((pte & PTE_X)) {
                *prot |= PAGE_EXEC;
            }
            /* add write permission on stores or if the page is already dirty,
               so that we TLB miss on later writes to update the dirty bit */
            if ((pte & PTE_W) &&
                    (access_type == MMU_DATA_STORE || (pte & PTE_D))) {
                *prot |= PAGE_WRITE;
            }
            return TRANSLATE_SUCCESS;
        }
    }
    return TRANSLATE_FAIL;
}

static void raise_mmu_exception(CPURISCVState *env, target_ulong address,
                                MMUAccessType access_type, bool pmp_violation,
                                bool first_stage)
{
    CPUState *cs = env_cpu(env);
    int page_fault_exceptions;
    if (first_stage) {
        page_fault_exceptions =
            (env->priv_ver >= PRIV_VERSION_1_10_0) &&
            get_field(env->satp, SATP_MODE) != VM_1_10_MBARE &&
            !pmp_violation;
    } else {
        page_fault_exceptions =
            get_field(env->hgatp, HGATP_MODE) != VM_1_10_MBARE &&
            !pmp_violation;
    }
    switch (access_type) {
    case MMU_INST_FETCH:
        if (riscv_cpu_virt_enabled(env) && !first_stage) {
            cs->exception_index = RISCV_EXCP_INST_GUEST_PAGE_FAULT;
        } else {
            cs->exception_index = page_fault_exceptions ?
                RISCV_EXCP_INST_PAGE_FAULT : RISCV_EXCP_INST_ACCESS_FAULT;
        }
        break;
    case MMU_DATA_LOAD:
    case MMU_DATA_CAP_LOAD:
        if (riscv_cpu_virt_enabled(env) && !first_stage) {
            cs->exception_index = RISCV_EXCP_LOAD_GUEST_ACCESS_FAULT;
        } else {
            cs->exception_index = page_fault_exceptions ?
                RISCV_EXCP_LOAD_PAGE_FAULT : RISCV_EXCP_LOAD_ACCESS_FAULT;
        }
        break;
    case MMU_DATA_STORE:
    case MMU_DATA_CAP_STORE:
        if (riscv_cpu_virt_enabled(env) && !first_stage) {
            cs->exception_index = RISCV_EXCP_STORE_GUEST_AMO_ACCESS_FAULT;
        } else {
            cs->exception_index = page_fault_exceptions ?
                RISCV_EXCP_STORE_PAGE_FAULT : RISCV_EXCP_STORE_AMO_ACCESS_FAULT;
        }
        break;
    default:
        g_assert_not_reached();
    }
    env->badaddr = address;
}

hwaddr riscv_cpu_get_phys_page_debug(CPUState *cs, vaddr addr)
{
    RISCVCPU *cpu = RISCV_CPU(cs);
    CPURISCVState *env = &cpu->env;
    hwaddr phys_addr;
    int prot;
    int mmu_idx = cpu_mmu_index(&cpu->env, false);

    if (get_physical_address(env, &phys_addr, &prot, addr, 0, mmu_idx,
                             true, riscv_cpu_virt_enabled(env))) {
        return -1;
    }

    if (riscv_cpu_virt_enabled(env)) {
        if (get_physical_address(env, &phys_addr, &prot, phys_addr,
                                 0, mmu_idx, false, true)) {
            return -1;
        }
    }

    return phys_addr;
}

void riscv_cpu_do_transaction_failed(CPUState *cs, hwaddr physaddr,
                                     vaddr addr, unsigned size,
                                     MMUAccessType access_type,
                                     int mmu_idx, MemTxAttrs attrs,
                                     MemTxResult response, uintptr_t retaddr)
{
    RISCVCPU *cpu = RISCV_CPU(cs);
    CPURISCVState *env = &cpu->env;

    if (access_type == MMU_DATA_STORE) {
        cs->exception_index = RISCV_EXCP_STORE_AMO_ACCESS_FAULT;
    } else {
        cs->exception_index = RISCV_EXCP_LOAD_ACCESS_FAULT;
    }

    env->badaddr = addr;
    riscv_raise_exception(&cpu->env, cs->exception_index, retaddr);
}

void riscv_cpu_do_unaligned_access(CPUState *cs, vaddr addr,
                                   MMUAccessType access_type, int mmu_idx,
                                   uintptr_t retaddr)
{
    RISCVCPU *cpu = RISCV_CPU(cs);
    CPURISCVState *env = &cpu->env;
    switch (access_type) {
    case MMU_INST_FETCH:
        cs->exception_index = RISCV_EXCP_INST_ADDR_MIS;
        break;
    case MMU_DATA_LOAD:
        cs->exception_index = RISCV_EXCP_LOAD_ADDR_MIS;
        break;
    case MMU_DATA_STORE:
        cs->exception_index = RISCV_EXCP_STORE_AMO_ADDR_MIS;
        break;
    default:
        g_assert_not_reached();
    }
    env->badaddr = addr;
    riscv_raise_exception(env, cs->exception_index, retaddr);
}
#endif

static inline int rvfi_dii_check_addr(CPURISCVState *env, int ret,
                                      vaddr address, int size, int *prot,
                                      MMUAccessType access_type)
{
#ifdef CONFIG_RVFI_DII
    // For RVFI-DII we have to reject all memory accesses outside of the RAM
    // region (even if there is a valid ROM there)
    // However, we still have to allow MMU_INST_FETCH accesess since they are
    // triggered by tb_find().
    if (env->rvfi_dii_have_injected_insn && ret == TRANSLATE_SUCCESS) {
        if (access_type == MMU_INST_FETCH) {
            // Avoid filling the QEMU guest->host TLB with read/write entries
            // for the faked instr fetch translation
            *prot &= PAGE_EXEC;
        } else if (address < RVFI_DII_RAM_START ||
                   (address + size) > RVFI_DII_RAM_END) {
            fprintf(stderr,
                    "Rejecting memory access to " TARGET_FMT_plx
                    " since it is outside the RVFI-DII range",
                    address);
            return TRANSLATE_FAIL;
        }
    }
#endif
    return ret;
}

#ifndef CONFIG_USER_ONLY
// Do all of riscv_cpu_tlb_fill() except for filling the TLB/raising a trap
static int riscv_cpu_tlb_fill_impl(CPURISCVState *env, vaddr address, int size,
                                   MMUAccessType access_type, int mmu_idx,
                                   bool *pmp_violation, bool *first_stage_error,
                                   int *prot, hwaddr *pa, uintptr_t retaddr)
{
    vaddr im_address;
    bool m_mode_two_stage = false;
    bool hs_mode_two_stage = false;
    int ret = TRANSLATE_FAIL;
    int mode = mmu_idx;

    env->guest_phys_fault_addr = 0;

    qemu_log_mask(CPU_LOG_MMU, "%s ad %" VADDR_PRIx " rw %d mmu_idx %d\n",
                  __func__, address, access_type, mmu_idx);

    /*
     * Determine if we are in M mode and MPRV is set or in HS mode and SPRV is
     * set and we want to access a virtulisation address.
     */
    if (riscv_has_ext(env, RVH)) {
        m_mode_two_stage = env->priv == PRV_M &&
                           access_type != MMU_INST_FETCH &&
                           get_field(env->mstatus, MSTATUS_MPRV) &&
                           MSTATUS_MPV_ISSET(env);

        hs_mode_two_stage = env->priv == PRV_S &&
                            !riscv_cpu_virt_enabled(env) &&
                            access_type != MMU_INST_FETCH &&
                            get_field(env->hstatus, HSTATUS_SPRV) &&
                            get_field(env->hstatus, HSTATUS_SPV);
    }

    if (mode == PRV_M && access_type != MMU_INST_FETCH) {
        if (get_field(env->mstatus, MSTATUS_MPRV)) {
            mode = get_field(env->mstatus, MSTATUS_MPP);
        }
    }

    if (riscv_cpu_virt_enabled(env) || m_mode_two_stage || hs_mode_two_stage) {
        /* Two stage lookup */
        ret = get_physical_address(env, pa, prot, address, access_type,
                                   mmu_idx, true, true);

        qemu_log_mask(CPU_LOG_MMU,
                      "%s 1st-stage address=%" VADDR_PRIx " ret %d physical "
                      TARGET_FMT_plx " prot %d\n",
                      __func__, address, ret, *pa, *prot);
        ret = rvfi_dii_check_addr(env, ret, address, size, prot, access_type);
        if (ret != TRANSLATE_FAIL) {
            /* Second stage lookup */
            im_address = *pa;

            ret = get_physical_address(env, pa, prot, im_address,
                                       access_type, mmu_idx, false, true);

            qemu_log_mask(CPU_LOG_MMU,
                    "%s 2nd-stage address=%" VADDR_PRIx " ret %d physical "
                    TARGET_FMT_plx " prot %d\n",
                    __func__, im_address, ret, *pa, *prot);

            if (riscv_feature(env, RISCV_FEATURE_PMP) &&
                (ret == TRANSLATE_SUCCESS) &&
                !pmp_hart_has_privs(env, *pa, size,
                                    access_type_to_pmp_priv(access_type),
                                    mode)) {
                ret = TRANSLATE_PMP_FAIL;
            }

            if (ret != TRANSLATE_SUCCESS) {
                /*
                 * Guest physical address translation failed, this is a HS
                 * level exception
                 */
                *first_stage_error = false;
                env->guest_phys_fault_addr = (im_address |
                                              (address &
                                               (TARGET_PAGE_SIZE - 1))) >> 2;
            }
        }
    } else {
        /* Single stage lookup */
        ret = get_physical_address(env, pa, prot, address, access_type,
                                   mmu_idx, true, false);
        ret = rvfi_dii_check_addr(env, ret, address, size, prot, access_type);

        qemu_log_mask(CPU_LOG_MMU,
                      "%s address=%" VADDR_PRIx " ret %d physical "
                      TARGET_FMT_plx " prot %d\n",
                      __func__, address, ret, *pa, *prot);
    }

    if (riscv_feature(env, RISCV_FEATURE_PMP) && (ret == TRANSLATE_SUCCESS) &&
        !pmp_hart_has_privs(env, *pa, size,
                            access_type_to_pmp_priv(access_type), mode)) {
        ret = TRANSLATE_PMP_FAIL;
    }
    if (ret == TRANSLATE_PMP_FAIL) {
        *pmp_violation = true;
    }

    return ret;
}
#endif

bool riscv_cpu_tlb_fill(CPUState *cs, vaddr address, int size,
                        MMUAccessType access_type, int mmu_idx,
                        bool probe, uintptr_t retaddr)
{
#ifndef CONFIG_USER_ONLY
    RISCVCPU *cpu = RISCV_CPU(cs);
    CPURISCVState *env = &cpu->env;
    bool pmp_violation = false;
    bool first_stage_error = true;
    int prot = 0;
    hwaddr pa = 0;
    int ret = riscv_cpu_tlb_fill_impl(env, address, size, access_type, mmu_idx,
                                      &pmp_violation, &first_stage_error, &prot,
                                      &pa, retaddr);
    if (ret == TRANSLATE_SUCCESS) {
        tlb_set_page(cs, address & TARGET_PAGE_MASK, pa & TARGET_PAGE_MASK,
                     prot, mmu_idx, TARGET_PAGE_SIZE);
        return true;
    } else if (probe) {
        return false;
    } else {
        raise_mmu_exception(env, address, access_type, pmp_violation,
                            first_stage_error);
        riscv_raise_exception(env, cs->exception_index, retaddr);
    }

    return true;
#else
    switch (access_type) {
    case MMU_INST_FETCH:
        cs->exception_index = RISCV_EXCP_INST_PAGE_FAULT;
        break;
    case MMU_DATA_LOAD:
        cs->exception_index = RISCV_EXCP_LOAD_PAGE_FAULT;
        break;
    case MMU_DATA_STORE:
        cs->exception_index = RISCV_EXCP_STORE_PAGE_FAULT;
        break;
    default:
        g_assert_not_reached();
    }
    env->badaddr = address;
    cpu_loop_exit_restore(cs, retaddr);
#endif
}

#ifdef TARGET_CHERI
hwaddr cpu_riscv_translate_address_tagmem(CPUArchState *env,
                                          target_ulong address,
                                          MMUAccessType rw, int reg, int *prot,
                                          uintptr_t retpc)
{
    bool pmp_violation = false;
    bool first_stage_error = true;
    hwaddr pa = 0;
    int ret = riscv_cpu_tlb_fill_impl(env, address, 1, rw, cpu_mmu_index(env, false),
                                      &pmp_violation, &first_stage_error, prot,
                                      &pa, retpc);
    if (ret != TRANSLATE_SUCCESS) {
        raise_mmu_exception(env, address, rw, pmp_violation, first_stage_error);
        riscv_raise_exception(env, env_cpu(env)->exception_index, retpc);
    }
    tcg_debug_assert((address & ~TARGET_PAGE_MASK) == (pa & ~TARGET_PAGE_MASK));
    return pa;
}
/* TODO(am2419): do we log PCC as a changed register? */
#define riscv_update_pc_for_exc_handler(env, src_cap, new_pc)           \
    do {                                                                \
        cheri_update_pcc_for_exc_handler(&env->PCC, src_cap, new_pc);   \
        qemu_log_instr_dbg_cap(env, "PCC", &env->PCC);                  \
    } while (false)
#else
/*
 * TODO(am2419): We don't have a register ID for pc, move to a separate
 * logging helper that maps hwreg id to names for extra registers.
 * TODO(am2419): do we log PCC as a changed register?
 */
#define riscv_update_pc_for_exc_handler(env, src_cap, new_pc)           \
    do {                                                                \
        riscv_update_pc(env, new_pc, /*can_be_unrepresentable=*/true);  \
        qemu_log_instr_dbg_reg(env, "pc", new_pc);                      \
    } while (false)
#endif /* TARGET_CHERI */

/*
 * Handle Traps
 *
 * Adapted from Spike's processor_t::take_trap.
 *
 */
void riscv_cpu_do_interrupt(CPUState *cs)
{
#if !defined(CONFIG_USER_ONLY)

    RISCVCPU *cpu = RISCV_CPU(cs);
    CPURISCVState *env = &cpu->env;
    tcg_debug_assert(pc_is_current(env));
    bool force_hs_execp = riscv_cpu_force_hs_excep_enabled(env);
    target_ulong s;

    /* cs->exception is 32-bits wide unlike mcause which is XLEN-bits wide
     * so we mask off the MSB and separate into trap type and cause.
     */
    bool async = !!(cs->exception_index & RISCV_EXCP_INT_FLAG);
    target_ulong cause = cs->exception_index & RISCV_EXCP_INT_MASK;
    target_ulong deleg = async ? env->mideleg : env->medeleg;
    target_ulong tval = 0;
    target_ulong htval = 0;
    target_ulong mtval2 = 0;

    bool log_inst = true;
    if (!async) {
        /* set tval to badaddr for traps with address information */
        switch (cause) {
        case RISCV_EXCP_INST_GUEST_PAGE_FAULT:
        case RISCV_EXCP_LOAD_GUEST_ACCESS_FAULT:
        case RISCV_EXCP_STORE_GUEST_AMO_ACCESS_FAULT:
            force_hs_execp = true;
            /* fallthrough */
        case RISCV_EXCP_INST_PAGE_FAULT:
        case RISCV_EXCP_LOAD_PAGE_FAULT:
        case RISCV_EXCP_STORE_PAGE_FAULT:
            log_inst = false;
            /* fallthrough */
        case RISCV_EXCP_INST_ADDR_MIS:
        case RISCV_EXCP_INST_ACCESS_FAULT:
        case RISCV_EXCP_LOAD_ADDR_MIS:
        case RISCV_EXCP_STORE_AMO_ADDR_MIS:
        case RISCV_EXCP_LOAD_ACCESS_FAULT:
        case RISCV_EXCP_STORE_AMO_ACCESS_FAULT:
            tval = env->badaddr;
            break;
#ifdef TARGET_CHERI
        case RISCV_EXCP_CHERI:
            tcg_debug_assert(env->cap_cause < 32);
            tcg_debug_assert(env->cap_index < 64);
            tval = env->cap_cause | env->cap_index << 5;
            qemu_log_instr_or_mask_msg(env, CPU_LOG_INT,
                "Got CHERI trap %s caused by register %d\n",
                cheri_cause_str(env->cap_cause), env->cap_index);
            break;
#endif
        case RISCV_EXCP_ILLEGAL_INST:
            tval = env->badaddr;
            break;
        default:
            break;
        }
        /* ecall is dispatched as one cause so translate based on mode */
        if (cause == RISCV_EXCP_U_ECALL) {
            assert(env->priv <= 3);

            if (env->priv == PRV_M) {
                cause = RISCV_EXCP_M_ECALL;
            } else if (env->priv == PRV_S && riscv_cpu_virt_enabled(env)) {
                cause = RISCV_EXCP_VS_ECALL;
            } else if (env->priv == PRV_S && !riscv_cpu_virt_enabled(env)) {
                cause = RISCV_EXCP_S_ECALL;
            } else if (env->priv == PRV_U) {
                cause = RISCV_EXCP_U_ECALL;
            }
        }
    }

    trace_riscv_trap(env->mhartid, async, cause, PC_ADDR(env), tval, cause < 23 ?
        (async ? riscv_intr_names : riscv_excp_names)[cause] : "(unknown)");

    if (unlikely(log_inst && qemu_loglevel_mask(CPU_LOG_INT))) {
        FILE* logf = qemu_log_lock();
        qemu_log("Trap (%s) was probably caused by: ", (async ? riscv_intr_names : riscv_excp_names)[cause]);
        target_disas(logf, cs, PC_ADDR(env), /* Only one instr*/-1);
        qemu_log_unlock(logf);
    }

    if (env->priv <= PRV_S &&
            cause < TARGET_LONG_BITS && ((deleg >> cause) & 1)) {
        /* handle the trap in S-mode */
        if (riscv_has_ext(env, RVH)) {
            target_ulong hdeleg = async ? env->hideleg : env->hedeleg;

            if (riscv_cpu_virt_enabled(env) && ((hdeleg >> cause) & 1) &&
                !force_hs_execp) {
                /*
                 * See if we need to adjust cause. Yes if its VS mode interrupt
                 * no if hypervisor has delegated one of hs mode's interrupt
                 */
                if (cause == IRQ_VS_TIMER || cause == IRQ_VS_SOFT ||
                    cause == IRQ_VS_EXT)
                    cause = cause - 1;
                /* Trap to VS mode */
            } else if (riscv_cpu_virt_enabled(env)) {
                /* Trap into HS mode, from virt */
                riscv_cpu_swap_hypervisor_regs(env, /*hs_mode_trap*/true);
                env->hstatus = set_field(env->hstatus, HSTATUS_SP2V,
                                         get_field(env->hstatus, HSTATUS_SPV));
                env->hstatus = set_field(env->hstatus, HSTATUS_SP2P,
                                         get_field(env->mstatus, SSTATUS_SPP));
                env->hstatus = set_field(env->hstatus, HSTATUS_SPV,
                                         riscv_cpu_virt_enabled(env));

                htval = env->guest_phys_fault_addr;

                riscv_cpu_set_virt_enabled(env, 0);
                riscv_cpu_set_force_hs_excep(env, 0);
            } else {
                /* Trap into HS mode */
                env->hstatus = set_field(env->hstatus, HSTATUS_SP2V,
                                         get_field(env->hstatus, HSTATUS_SPV));
                env->hstatus = set_field(env->hstatus, HSTATUS_SP2P,
                                         get_field(env->mstatus, SSTATUS_SPP));
                env->hstatus = set_field(env->hstatus, HSTATUS_SPV,
                                         riscv_cpu_virt_enabled(env));

                htval = env->guest_phys_fault_addr;
            }
        }
        riscv_log_instr_csr_changed(env, CSR_HSTATUS);

        s = env->mstatus;
        s = set_field(s, MSTATUS_SPIE, env->priv_ver >= PRIV_VERSION_1_10_0 ?
            get_field(s, MSTATUS_SIE) : get_field(s, MSTATUS_UIE << env->priv));
        s = set_field(s, MSTATUS_SPP, env->priv);
        s = set_field(s, MSTATUS_SIE, 0);
        env->mstatus = s;
        riscv_log_instr_csr_changed(env, CSR_MSTATUS);
        env->scause = cause | ((target_ulong)async << (TARGET_LONG_BITS - 1));
        riscv_log_instr_csr_changed(env, CSR_SCAUSE);

        COPY_SPECIAL_REG(env, sepc, SEPCC, pc, PCC);
        LOG_SPECIAL_REG(env, CSR_SEPC, CheriSCR_SEPCC);

        env->sbadaddr = tval;
        riscv_log_instr_csr_changed(env, CSR_SBADADDR);
        env->htval = htval;
        riscv_log_instr_csr_changed(env, CSR_HTVAL);

        target_ulong stvec = GET_SPECIAL_REG_ADDR(env, stvec, STCC);
        target_ulong new_pc = (stvec >> 2 << 2) +
            ((async && (stvec & 3) == 1) ? cause * 4 : 0);
        riscv_update_pc_for_exc_handler(env, &env->STCC, new_pc);
        riscv_cpu_set_mode(env, PRV_S);
    } else {
        /* handle the trap in M-mode */
        if (riscv_has_ext(env, RVH)) {
            if (riscv_cpu_virt_enabled(env)) {
                riscv_cpu_swap_hypervisor_regs(env, /*hs_mode_trap*/false);
            }
#ifdef TARGET_RISCV32
            env->mstatush = set_field(env->mstatush, MSTATUS_MPV,
                                       riscv_cpu_virt_enabled(env));
            env->mstatush = set_field(env->mstatush, MSTATUS_MTL,
                                       riscv_cpu_force_hs_excep_enabled(env));
            riscv_log_instr_csr_changed(env, CSR_MSTATUSH);
#else
            env->mstatus = set_field(env->mstatus, MSTATUS_MPV,
                                      riscv_cpu_virt_enabled(env));
            env->mstatus = set_field(env->mstatus, MSTATUS_MTL,
                                      riscv_cpu_force_hs_excep_enabled(env));
#endif

            mtval2 = env->guest_phys_fault_addr;

            /* Trapping to M mode, virt is disabled */
            riscv_cpu_set_virt_enabled(env, 0);
            riscv_cpu_set_force_hs_excep(env, 0);
        }

        s = env->mstatus;
        s = set_field(s, MSTATUS_MPIE, env->priv_ver >= PRIV_VERSION_1_10_0 ?
            get_field(s, MSTATUS_MIE) : get_field(s, MSTATUS_UIE << env->priv));
        s = set_field(s, MSTATUS_MPP, env->priv);
        s = set_field(s, MSTATUS_MIE, 0);
        env->mstatus = s;
        riscv_log_instr_csr_changed(env, CSR_MSTATUS);
        env->mcause = cause | ~(((target_ulong)-1) >> async);
        riscv_log_instr_csr_changed(env, CSR_MCAUSE);

        COPY_SPECIAL_REG(env, mepc, MEPCC, pc, PCC);
        LOG_SPECIAL_REG(env, CSR_MEPC, CheriSCR_MEPCC);

        env->mbadaddr = tval;
        riscv_log_instr_csr_changed(env, CSR_MBADADDR);
        env->mtval2 = mtval2;
        riscv_log_instr_csr_changed(env, CSR_MTVAL2);

        target_ulong mtvec = GET_SPECIAL_REG_ADDR(env, mtvec, MTCC);
        target_ulong new_pc = (mtvec >> 2 << 2) +
            ((async && (mtvec & 3) == 1) ? cause * 4 : 0);
        riscv_update_pc_for_exc_handler(env, &env->MTCC, new_pc);
        riscv_cpu_set_mode(env, PRV_M);
    }

#ifdef CONFIG_TCG_LOG_INSTR
    if (qemu_log_instr_enabled(env)) {
        if (async) {
            qemu_log_instr_interrupt(env, cause,
                GET_SPECIAL_REG_ADDR(env, pc, PCC));
        } else {
            qemu_log_instr_exception(env, cause,
                GET_SPECIAL_REG_ADDR(env, pc, PCC), tval);
        }
    }
#endif

#ifdef CONFIG_RVFI_DII
    if (unlikely(env->rvfi_dii_have_injected_insn)) {
        qemu_log_mask(CPU_LOG_INT, "%s: Got real exception %d\n", __func__,
                      cs->exception_index);
        env->rvfi_dii_trace.rvfi_dii_trap = true;
        rvfi_dii_communicate(env_cpu(env), env);
    }
#endif
    /* NOTE: it is not necessary to yield load reservations here. It is only
     * necessary for an SC from "another hart" to cause a load reservation
     * to be yielded. Refer to the memory consistency model section of the
     * RISC-V ISA Specification.
     */

#endif
    cs->exception_index = EXCP_NONE; /* mark handled to qemu */
}

#ifdef TARGET_CHERI
void update_special_register_offset(CPURISCVState *env, cap_register_t *scr,
                                    const char *name, target_ulong new_value)
{
    // Any write to the CSR shall set the offset of the SCR to the value
    // written. This shall be equivalent to a CSetOffset instruction, but with
    // any exception condition instead just clearing the tag of the SCR.
    target_ulong cursor = cap_get_cursor(scr);
    target_ulong current_offset = cap_get_offset(scr);
    target_ulong diff = new_value - current_offset;
    target_ulong new_cursor = cursor + diff;

    if (!cap_is_unsealed(scr)) {
        error_report("Attempting to modify sealed %s: " PRINT_CAP_FMTSTR "\r\n",
                     name, PRINT_CAP_ARGS(scr));
        qemu_log_instr_extra(env, "Attempting to modify sealed %s: "
            PRINT_CAP_FMTSTR "\n", name, PRINT_CAP_ARGS(scr));
        // Clear the tag bit and update the cursor:
        cap_mark_unrepresentable(new_cursor, scr);
    } else if (!is_representable_cap_with_addr(scr, new_cursor)) {
        error_report(
            "Attempting to set unrepresentable cursor (0x" TARGET_FMT_lx
            ") on %s: " PRINT_CAP_FMTSTR "\r\n",
            new_cursor, name, PRINT_CAP_ARGS(scr));
        qemu_log_instr_extra(env, "Attempting to set unrepresentable cursor (0x"
            TARGET_FMT_lx ") on %s: " PRINT_CAP_FMTSTR "\r\n", new_cursor,
            name, PRINT_CAP_ARGS(scr));
        cap_mark_unrepresentable(new_cursor, scr);
    } else {
        scr->_cr_cursor = new_cursor;
    }
    /*
     * TODO(am2419): this is redundant as we are already logging the update
     * of the CSR register? Although it migth be useful in case we are making
     * the capability unrepresentable.
     */
    cheri_log_instr_changed_capreg(env, name, scr);
}
#endif
