/*
 *  mips thread support
 *
 *  Copyright (c) 2013 Stacey D. Son
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

/* Compare to mips/mips/vm_machdep.c cpu_set_upcall_kse() */
static inline void target_thread_set_upcall(CPUMIPSState *regs, abi_ulong entry,
    abi_ulong arg, abi_ulong stack_base, abi_ulong stack_size)
{
    abi_ulong sp;

    /*
     * At the point where a function is called, sp must be 8
     * byte aligned[for compatibility with 64-bit CPUs]
     * in ``See MIPS Run'' by D. Sweetman, p. 269
     * align stack
     */
    sp = ((stack_base + stack_size) & ~0x7) - TARGET_CALLFRAME_SIZ;

    /* t9 = pc = start function entry */
    regs->active_tc.gpr[25] = regs->active_tc.PC = entry;
    /* a0 = arg */
    regs->active_tc.gpr[4] = arg;
    /* sp = top of the stack */
    regs->active_tc.gpr[29] = sp;
}

static inline void target_thread_init(struct target_pt_regs *regs,
        struct image_info *infop)
{
    regs->cp0_status = 2 << CP0St_KSU;
    regs->regs[25] = regs->cp0_epc = infop->entry & ~3;  /* t9/pc = entry */
    regs->regs[4] = regs->regs[29] = infop->start_stack; /* a0/sp = stack */
    regs->regs[5] = regs->regs[6] = 0;                   /* a1/a2 = 0 */
    regs->regs[7] = TARGET_PS_STRINGS;                   /* a3 = ps_strings */
}

#endif /* !_TARGET_ARCH_THREAD_H_ */
