/*
 *  powerpc thread support
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

/* Compare to powerpc/powerpc/exec_machdep.c cpu_set_upcall_kse() */
static inline void target_thread_set_upcall(CPUPPCState *regs, abi_ulong entry,
    abi_ulong arg, abi_ulong stack_base, abi_ulong stack_size)
{
    abi_ulong sp;

    /*
     * Make sure the stack is properly aligned.
     * powerpc/include/param.h (STACKLIGN() macro)
     */
#if defined(TARGET_PPC64) && !defined(TARGET_ABI32)
    sp = ((u_int)(stack_base + stack_size) - 48) & ~0x1f;
#else
    sp = ((u_int)(stack_base + stack_size) - 8) & ~0x1f;
#endif

    /* r1 = stack base */
    regs->gpr[1] = sp;
    /* r3 = arg */
    regs->gpr[3] = arg;
    /* srr0 = start function entry */
#if defined(TARGET_PPC64) && !defined(TARGET_ABI32)
    get_user_ual(regs->nip, entry);
    get_user_ual(regs->gpr[2], entry + sizeof(abi_ulong));
    get_user_ual(regs->gpr[11], entry + 2 * sizeof(abi_ulong));
#else
    regs->nip = entry;
#endif
}

static inline void target_thread_init(struct target_pt_regs *regs,
        struct image_info *infop)
{
	abi_long stack = infop->start_stack;
	abi_long argc;

#if defined(TARGET_PPC64) && !defined(TARGET_ABI32)
    regs->gpr[1] = -roundup(-stack + 48, 16);
    get_user_u64(regs->gpr[2], infop->entry + 8);
    regs->gpr[2] += infop->load_addr;
    get_user_u64(regs->gpr[11], infop->entry + 16);
    regs->gpr[11] += infop->load_addr;

    get_user_u64(infop->entry, infop->entry);
    infop->entry += infop->load_addr;
#else
    regs->gpr[1] = -roundup(-stack + 8, 16);
#endif
    regs->nip = infop->entry;
    /* FIXME - what to for failure of get_user()? */
    get_user_ual(argc, stack); /* argc */
    regs->gpr[3] = argc;
    regs->gpr[4] = stack + sizeof(abi_long);
    regs->gpr[5] = regs->gpr[4] + ((1 + argc) * sizeof(abi_long));
    regs->gpr[6] = 0;
    regs->gpr[7] = 0;
    regs->gpr[8] = TARGET_PS_STRINGS;
    if (bsd_type == target_freebsd) {
        regs->lr = infop->entry;
    }
}

#endif /* !_TARGET_ARCH_THREAD_H_ */
