/*
 *  sparc64 thread support
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

#define STACK_BIAS              2047

/* Compare to vm_machdep.c cpu_set_upcall_kse() */
static inline void target_thread_set_upcall(CPUSPARCState *regs,
    abi_ulong entry, abi_ulong arg, abi_ulong stack_base, abi_ulong stack_size)
{
    /* XXX */
}

static inline void target_thread_init(struct target_pt_regs *regs,
        struct image_info *infop)
{
#ifndef TARGET_ABI32
    regs->tstate = 0;
#endif
    regs->pc = infop->entry;
    regs->npc = regs->pc + 4;
    regs->y = 0;
#ifdef TARGET_ABI32
    regs->u_regs[14] = infop->start_stack - 16 * 4;
#else
    if (personality(infop->personality) == PER_LINUX32)
        regs->u_regs[14] = infop->start_stack - 16 * 4;
    else {
        regs->u_regs[14] = infop->start_stack - 16 * 8 - STACK_BIAS;
        if (bsd_type == target_freebsd) {
            regs->u_regs[8] = infop->start_stack;
            regs->u_regs[11] = infop->start_stack;
        }
    }
#endif
}

#endif /* !_TARGET_ARCH_THREAD_H_ */
