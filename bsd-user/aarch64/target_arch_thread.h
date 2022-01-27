/*
 * ARM AArch64 thread support for bsd-user.
 *
 * Copyright (c) 2015 Stacey D. Son <sson at FreeBSD>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _TARGET_ARCH_THREAD_H_
#define _TARGET_ARCH_THREAD_H_

#ifdef TARGET_CHERI
#include "cheri/cherireg.h"
#include "cheri/cheri.h"
#endif

/* Compare to arm64/arm64/vm_machdep.c cpu_set_upcall_kse() */
static inline void target_thread_set_upcall(CPUARMState *regs, abi_ulong entry,
    abi_ulong arg, abi_ulong stack_base, abi_ulong stack_size)
{
    abi_ulong sp;
    uint32_t pstate;

    /*
     * Make sure the stack is properly aligned.
     * arm64/include/param.h (STACKLIGN() macro)
     */
    sp = (abi_ulong)((stack_base + stack_size) -
        sizeof(struct target_trapframe)) & ~(16 - 1);

    pstate = PSTATE_MODE_EL0t;
#ifdef TARGET_CHERI
    pstate |= PSTATE_C64;
#endif
    /*
     * Update PSTATE before preparing PCC as it might require rebuilding CPU
     * flags.
     */
    pstate_write(regs, pstate);

    /* sp = stack base */
    arm_set_xreg(regs, 31, sp);
    /* pc = start function entry */
#ifdef TARGET_CHERI
    cheri_update_pcc(&regs->pc.cap, entry, false);
    cheri_prepare_pcc(&regs->pc.cap, regs);
#else
    regs->pc = entry &  ~0x3ULL;
#endif
    /* r0 = arg */
    arm_set_xreg(regs, 0, arg);
}

/*
 * See exec_setregs() from sys/arm64/arm64/machdep.c.
 */
static inline void target_thread_init(struct target_pt_regs *regs,
#ifdef TARGET_CHERI
        cap_register_t *mmapcapp, cap_register_t *sigcodecapp,
#endif
        struct image_info *infop)
{

    memset(regs, 0, sizeof(*regs));
#ifdef TARGET_CHERI
    /* XXXKW: Check SV_CHERI. */
    (void)cheri_auxv_capability(&regs->regs[0]);
    (void)cheri_exec_stack_pointer(cheri_andperm(&regs->sp,
        CHERI_CAP_USER_DATA_PERMS), infop->start_stack);
    cheri_set_mmap_capability(mmapcapp, infop, &regs->sp);

    (void)cheri_exec_pcc(&regs->regs[30], infop);
    (void)cheri_sigcode_capability(sigcodecapp);
#else
    regs->regs[0] = infop->start_stack;
    regs->pc = infop->entry &  ~0x3ULL;
    regs->sp = infop->start_stack & ~(16 - 1);
#endif
}

#endif /* !_TARGET_ARCH_THREAD_H_ */
