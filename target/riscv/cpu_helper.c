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
#include "tcg/tcg-op.h"
#include "trace.h"
#include "disas/disas.h"
#ifdef TARGET_CHERI
#include "cheri-helper-utils.h"
#endif

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
    target_ulong mstatus_mie = get_field(env->mstatus, MSTATUS_MIE);
    target_ulong mstatus_sie = get_field(env->mstatus, MSTATUS_SIE);
    target_ulong pending = env->mip & env->mie;
    target_ulong mie = env->priv < PRV_M || (env->priv == PRV_M && mstatus_mie);
    target_ulong sie = env->priv < PRV_S || (env->priv == PRV_S && mstatus_sie);
    target_ulong irqs = (pending & ~env->mideleg & -mie) |
                        (pending &  env->mideleg & -sie);

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
        return true;
    }

    return false;
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
}

/* get_physical_address - get the physical address for this virtual address
 *
 * Do a page table walk to obtain the physical address corresponding to a
 * virtual address. Returns 0 if the translation was successful
 *
 * Adapted from Spike's mmu_t::translate and mmu_t::walk
 *
 */
static int get_physical_address(CPURISCVState *env, hwaddr *physical,
                                int *prot, target_ulong addr,
                                int access_type, int mmu_idx)
{
    /* NOTE: the env->pc value visible here will not be
     * correct, but the value visible to the exception handler
     * (riscv_cpu_do_interrupt) is correct */
    MemTxResult res;
    MemTxAttrs attrs = MEMTXATTRS_UNSPECIFIED;
    int mode = mmu_idx;
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

    if (mode == PRV_M && access_type != MMU_INST_FETCH) {
        if (get_field(env->mstatus, MSTATUS_MPRV)) {
            mode = get_field(env->mstatus, MSTATUS_MPP);
        }
    }

    if (mode == PRV_M || !riscv_feature(env, RISCV_FEATURE_MMU)) {
        *physical = addr;
        *prot = PAGE_READ | PAGE_WRITE | PAGE_EXEC;
        return TRANSLATE_SUCCESS;
    }

    *prot = 0;

    hwaddr base;
    int levels, ptidxbits, ptesize, vm, sum;
    int mxr = get_field(env->mstatus, MSTATUS_MXR);

    if (env->priv_ver >= PRIV_VERSION_1_10_0) {
        base = (hwaddr)get_field(env->satp, SATP_PPN) << PGSHIFT;
        sum = get_field(env->mstatus, MSTATUS_SUM);
        vm = get_field(env->satp, SATP_MODE);
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
    int va_bits = PGSHIFT + levels * ptidxbits;
    target_ulong mask = (1L << (TARGET_LONG_BITS - (va_bits - 1))) - 1;
    target_ulong masked_msbs = (addr >> (va_bits - 1)) & mask;
    if (masked_msbs != 0 && masked_msbs != mask) {
        return TRANSLATE_FAIL;
    }

    int ptshift = (levels - 1) * ptidxbits;
    int i;

#if !TCG_OVERSIZED_GUEST
restart:
#endif
    for (i = 0; i < levels; i++, ptshift -= ptidxbits) {
        target_ulong idx = (addr >> (PGSHIFT + ptshift)) &
                           ((1 << ptidxbits) - 1);

        /* check that physical address of PTE is legal */
        hwaddr pte_addr = base + idx * ptesize;

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
            *physical = ((ppn | (vpn & ((1L << ptshift) - 1))) << PGSHIFT)  | (addr & ~TARGET_PAGE_MASK);

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
                                MMUAccessType access_type, bool pmp_violation)
{
    CPUState *cs = env_cpu(env);
    int page_fault_exceptions =
        (env->priv_ver >= PRIV_VERSION_1_10_0) &&
        get_field(env->satp, SATP_MODE) != VM_1_10_MBARE &&
        !pmp_violation;
    switch (access_type) {
    case MMU_INST_FETCH:
        cs->exception_index = page_fault_exceptions ?
            RISCV_EXCP_INST_PAGE_FAULT : RISCV_EXCP_INST_ACCESS_FAULT;
        break;
    case MMU_DATA_LOAD:
    case MMU_DATA_CAP_LOAD:
        cs->exception_index = page_fault_exceptions ?
            RISCV_EXCP_LOAD_PAGE_FAULT : RISCV_EXCP_LOAD_ACCESS_FAULT;
        break;
    case MMU_DATA_STORE:
    case MMU_DATA_CAP_STORE:
        cs->exception_index = page_fault_exceptions ?
            RISCV_EXCP_STORE_PAGE_FAULT : RISCV_EXCP_STORE_AMO_ACCESS_FAULT;
        break;
    default:
        g_assert_not_reached();
    }
    env->badaddr = address;
}

hwaddr riscv_cpu_get_phys_page_debug(CPUState *cs, vaddr addr)
{
    RISCVCPU *cpu = RISCV_CPU(cs);
    hwaddr phys_addr;
    int prot;
    int mmu_idx = cpu_mmu_index(&cpu->env, false);

    if (get_physical_address(&cpu->env, &phys_addr, &prot, addr, 0, mmu_idx)) {
        return -1;
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

bool riscv_cpu_tlb_fill(CPUState *cs, vaddr address, int size,
                        MMUAccessType access_type, int mmu_idx,
                        bool probe, uintptr_t retaddr)
{
    RISCVCPU *cpu = RISCV_CPU(cs);
    CPURISCVState *env = &cpu->env;
#ifndef CONFIG_USER_ONLY
    hwaddr pa = 0;
    int prot;
    bool pmp_violation = false;
    int ret = TRANSLATE_FAIL;
    int mode = mmu_idx;

    qemu_log_mask(CPU_LOG_MMU, "%s ad %" VADDR_PRIx " rw %d mmu_idx %d\n",
                  __func__, address, access_type, mmu_idx);

    ret = get_physical_address(env, &pa, &prot, address, access_type, mmu_idx);
#ifdef CONFIG_RVFI_DII
    // For RVFI-DII we have to reject all memory accesses outside of the RAM
    // region (even if there is a valid ROM there)
    // However, we still have to allow MMU_INST_FETCH accesess since they are
    // triggered by tb_find().
    if (env->rvfi_dii_have_injected_insn && ret == TRANSLATE_SUCCESS) {
        if (access_type == MMU_INST_FETCH) {
            // Avoid filling the QEMU guest->host TLB with read/write entries
            // for the faked instr fetch translation
            prot &= PAGE_EXEC;
        } else if (address < RVFI_DII_RAM_START || address >= RVFI_DII_RAM_END) {
            fprintf(stderr, "Rejecting memory access to " TARGET_FMT_plx
                    " since it is outside the RVFI-DII range", address);
            ret = TRANSLATE_FAIL;
        }
    }
#endif

    if (mode == PRV_M && access_type != MMU_INST_FETCH) {
        if (get_field(env->mstatus, MSTATUS_MPRV)) {
            mode = get_field(env->mstatus, MSTATUS_MPP);
        }
    }

    qemu_log_mask(CPU_LOG_MMU,
                  "%s address=%" VADDR_PRIx " ret %d physical " TARGET_FMT_plx
                  " prot %d\n", __func__, address, ret, pa, prot);

    if (riscv_feature(env, RISCV_FEATURE_PMP) &&
        (ret == TRANSLATE_SUCCESS) &&
        !pmp_hart_has_privs(env, pa, size, 1 << access_type, mode)) {
        ret = TRANSLATE_PMP_FAIL;
    }
    if (ret == TRANSLATE_PMP_FAIL) {
        pmp_violation = true;
    }
    if (ret == TRANSLATE_SUCCESS) {
        tlb_set_page(cs, address & TARGET_PAGE_MASK, pa & TARGET_PAGE_MASK,
                     prot, mmu_idx, TARGET_PAGE_SIZE);
        return true;
    } else if (probe) {
        return false;
    } else {
        raise_mmu_exception(env, address, access_type, pmp_violation);
        riscv_raise_exception(env, cs->exception_index, retaddr);
    }
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
                                          target_ulong address, int rw, int reg,
                                          int *prot, uintptr_t retpc)
{
    hwaddr physical = -1LL;
    int ret = get_physical_address(env, &physical, prot, address, rw,
                               cpu_mmu_index(env, false));
    if (ret != TRANSLATE_SUCCESS) {
        raise_mmu_exception(env, address, rw, false);
        // TODO: use register
        riscv_raise_exception(env, env_cpu(env)->exception_index, retpc);
    }
    tcg_debug_assert((address & ~TARGET_PAGE_MASK) == (physical &~TARGET_PAGE_MASK));
    return physical;
}
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

    /* cs->exception is 32-bits wide unlike mcause which is XLEN-bits wide
     * so we mask off the MSB and separate into trap type and cause.
     */
    bool async = !!(cs->exception_index & RISCV_EXCP_INT_FLAG);
    target_ulong cause = cs->exception_index & RISCV_EXCP_INT_MASK;
    target_ulong deleg = async ? env->mideleg : env->medeleg;
    target_ulong tval = 0;

    static const int ecall_cause_map[] = {
        [PRV_U] = RISCV_EXCP_U_ECALL,
        [PRV_S] = RISCV_EXCP_S_ECALL,
        [PRV_H] = RISCV_EXCP_H_ECALL,
        [PRV_M] = RISCV_EXCP_M_ECALL
    };

    bool log_inst = true;
    if (!async) {
        /* set tval to badaddr for traps with address information */
        switch (cause) {
        case RISCV_EXCP_INST_PAGE_FAULT:
        case RISCV_EXCP_LOAD_PAGE_FAULT:
        case RISCV_EXCP_STORE_PAGE_FAULT:
            log_inst = false;
            // FALLTHROUGH
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
            qemu_log_mask(CPU_LOG_INT | CPU_LOG_INSTR,
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
            cause = ecall_cause_map[env->priv];
        }
    }

    trace_riscv_trap(env->mhartid, async, cause, PC_ADDR(env), tval, cause < 16 ?
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
        target_ulong s = env->mstatus;
        s = set_field(s, MSTATUS_SPIE, env->priv_ver >= PRIV_VERSION_1_10_0 ?
            get_field(s, MSTATUS_SIE) : get_field(s, MSTATUS_UIE << env->priv));
        s = set_field(s, MSTATUS_SPP, env->priv);
        s = set_field(s, MSTATUS_SIE, 0);
        env->mstatus = s;
        env->scause = cause | ((target_ulong)async << (TARGET_LONG_BITS - 1));
        COPY_SPECIAL_REG(env, sepc, SEPCC, pc, PCC);
        env->sbadaddr = tval;
        target_ulong stvec = GET_SPECIAL_REG(env, stvec, STCC);
        target_ulong new_pc = (stvec >> 2 << 2) +
            ((async && (stvec & 3) == 1) ? cause * 4 : 0);
#ifdef TARGET_CHERI
        env->PCC = env->STCC;
        assert(cap_is_in_bounds(&env->PCC, new_pc, 0));
        env->PCC._cr_cursor = new_pc;
        qemu_log_mask_and_addr(CPU_LOG_INSTR, cpu_get_recent_pc(env),
                               "%s: Set SEPCC from PCC: " PRINT_CAP_FMTSTR "\n",
                               __func__, PRINT_CAP_ARGS(&env->SEPCC));
        qemu_log_mask_and_addr(CPU_LOG_INSTR, cpu_get_recent_pc(env),
                               "%s: Set PCC from STCC: " PRINT_CAP_FMTSTR "\n",
                               __func__, PRINT_CAP_ARGS(&env->PCC));
#else
        env->pc = new_pc;
#endif
        riscv_cpu_set_mode(env, PRV_S);
    } else {
        /* handle the trap in M-mode */
        target_ulong s = env->mstatus;
        s = set_field(s, MSTATUS_MPIE, env->priv_ver >= PRIV_VERSION_1_10_0 ?
            get_field(s, MSTATUS_MIE) : get_field(s, MSTATUS_UIE << env->priv));
        s = set_field(s, MSTATUS_MPP, env->priv);
        s = set_field(s, MSTATUS_MIE, 0);
        env->mstatus = s;
        env->mcause = cause | ~(((target_ulong)-1) >> async);
        COPY_SPECIAL_REG(env, mepc, MEPCC, pc, PCC);
        env->mbadaddr = tval;
        target_ulong mtvec = GET_SPECIAL_REG(env, mtvec, MTCC);
        target_ulong new_pc = (mtvec >> 2 << 2) +
            ((async && (mtvec & 3) == 1) ? cause * 4 : 0);
#ifdef TARGET_CHERI
        env->PCC = env->MTCC;
        assert(cap_is_in_bounds(&env->PCC, new_pc, 0));
        env->PCC._cr_cursor = new_pc;
        qemu_log_mask_and_addr(CPU_LOG_INSTR, cpu_get_recent_pc(env),
                               "%s: Set MEPCC from PCC: " PRINT_CAP_FMTSTR "\n",
                               __func__, PRINT_CAP_ARGS(&env->MEPCC));
        qemu_log_mask_and_addr(CPU_LOG_INSTR, cpu_get_recent_pc(env),
                               "%s: Set PCC from MTCC: " PRINT_CAP_FMTSTR "\n",
                               __func__, PRINT_CAP_ARGS(&env->PCC));
#else
        env->pc = new_pc;
#endif
        riscv_cpu_set_mode(env, PRV_M);
    }

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
        qemu_log("Attempting to modify sealed %s: " PRINT_CAP_FMTSTR "\n", name,
                 PRINT_CAP_ARGS(scr));
        // Clear the tag bit and update the cursor:
        cap_mark_unrepresentable(new_cursor, scr);
    } else if (!is_representable_cap_with_addr(scr, new_cursor)) {
        error_report(
            "Attempting to set unrepresentable cursor (0x" TARGET_FMT_lx
            ") on %s: " PRINT_CAP_FMTSTR "\r\n",
            new_cursor, name, PRINT_CAP_ARGS(scr));
        qemu_log("Attempting to set unrepresentable cursor (0x" TARGET_FMT_lx
                 ") on %s: " PRINT_CAP_FMTSTR "\r\n",
                 new_cursor, name, PRINT_CAP_ARGS(scr));
        cap_mark_unrepresentable(new_cursor, scr);
    } else {
        scr->_cr_cursor = new_cursor;
    }
    log_changed_capreg(env, name, scr);
}
#endif
