/*
 *  arm thread support
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

/* Compare to arm/arm/vm_machdep.c cpu_set_upcall_kse() */
static inline void target_thread_set_upcall(CPUARMState *regs, abi_ulong entry,
    abi_ulong arg, abi_ulong stack_base, abi_ulong stack_size)
{
    abi_ulong sp;

    /*
     * Make sure the stack is properly aligned.
     * arm/include/param.h (STACKLIGN() macro)
     */
    sp = (u_int)((stack_base + stack_size) -
        sizeof(struct target_trapframe)) & ~0x7;

    /* sp = stack base */
    regs->regs[13] = sp;
    /* pc = start function entry */
    regs->regs[15] = entry & 0xfffffffe;
    /* r0 = arg */
    regs->regs[0] = arg;
    regs->spsr = ARM_CPU_MODE_USR;
}

static inline void target_thread_init(struct target_pt_regs *regs,
        struct image_info *infop)
{
    abi_long stack = infop->start_stack;
    memset(regs, 0, sizeof(*regs));
    regs->ARM_cpsr = 0x10;
    if (infop->entry & 1)
      regs->ARM_cpsr |= CPSR_T;
    regs->ARM_pc = infop->entry & 0xfffffffe;
    regs->ARM_sp = infop->start_stack;
    if (bsd_type == target_freebsd) {
        regs->ARM_lr = infop->entry & 0xfffffffe;
    }
    /* FIXME - what to for failure of get_user()? */
    get_user_ual(regs->ARM_r2, stack + 8); /* envp */
    get_user_ual(regs->ARM_r1, stack + 4); /* envp */
    /* XXX: it seems that r0 is zeroed after ! */
    regs->ARM_r0 = 0;
    /* For uClinux PIC binaries.  */
    /* XXX: Linux does this only on ARM with no MMU (do we care ?) */
    regs->ARM_r10 = infop->start_data;
}

#endif /* !_TARGET_ARCH_THREAD_H_ */
