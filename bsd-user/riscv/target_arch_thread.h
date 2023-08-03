/*
 *  RISC-V thread support
 *
 *  Copyright (c) 2019 Mark Corbin <mark.corbin@embecosm.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, see <http://www.gnu.org/licenses/>.
 */
 
#ifndef _TARGET_ARCH_THREAD_H_
#define _TARGET_ARCH_THREAD_H_

#ifdef TARGET_CHERI
#include "cheri/cherireg.h"
#include "cheri/cheri.h"
#endif

/* Compare with cpu_set_upcall() in riscv/riscv/vm_machdep.c */
static inline void target_thread_set_upcall(CPURISCVState *regs,
    abi_uintptr_t entry, abi_uintptr_t arg, abi_uintptr_t stack_base,
    abi_ulong stack_size)
{
#ifdef TARGET_CHERI
    cap_register_t cap;
#endif
    abi_ulong sp;
    
#ifdef TARGET_CHERI
    sp = cheri_getaddress(cheri_load(&cap, &stack_base));
#else
    sp = stack_base;
#endif
    sp = (abi_ulong)((sp + stack_size) -
        sizeof(struct target_trapframe)) & ~(16 - 1);

    /* sp = stack base */
#ifdef TARGET_CHERI
    (void)cheri_setaddress(&cap, sp);
    update_capreg(regs, xSP, &cap);
#else
    gpr_set_int_value(regs, xSP, sp);
#endif
    /* pc = start function entry */
#ifdef TARGET_CHERI
    (void)cheri_load(&cap, &entry);
    cheri_prepare_pcc(&cap, regs);
#else
    riscv_update_pc(regs, entry, true);
#endif
    /* a0 = arg */
#ifdef TARGET_CHERI
    (void)cheri_load(&cap, &arg);
    update_capreg(regs, xA0, &cap);
#else
    gpr_set_int_value(regs, xA0, arg);
#endif
}

/* Compare with exec_setregs() in riscv/riscv/machdep.c */
static inline void target_thread_init(struct target_pt_regs *regs,
#ifdef TARGET_CHERI
        cap_register_t *sigcodecapp,
#endif
        struct image_info *infop)
{

#ifdef TARGET_CHERI
    /* XXXKW: Check SV_CHERI. */
    (void)cheri_auxv_capability(&regs->regs[xA0]);
    (void)cheri_exec_stack_pointer(cheri_andperm(&regs->regs[xSP],
        CHERI_CAP_USER_DATA_PERMS), infop->start_stack);

    (void)cheri_exec_pcc(&regs->sepc, infop);
    (void)cheri_sigcode_capability(sigcodecapp);
#else
    regs->sepc = infop->entry & ~0x03;
    regs->regs[10] = infop->start_stack;               /* a0 */
    regs->regs[xSP] = infop->start_stack & ~(16 - 1);
#endif

    regs->regs[xRA] = regs->sepc;
}

#endif /* !_TARGET_ARCH_THREAD_H_ */
