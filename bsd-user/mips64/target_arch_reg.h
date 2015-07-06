/*
 *  FreeBSD mips64 register structures
 *
 *  Copyright (c) 2015 Stacey Son
 *  All rights reserved.
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

#ifndef _TARGET_ARCH_REG_H_
#define _TARGET_ARCH_REG_H_

/* See sys/mips64/include/reg.h */
#define TARGET_NUMSAVEREGS  40
typedef struct target_reg {
    uint64_t        r_regs[TARGET_NUMSAVEREGS];
} target_reg_t;

#define TARGET_NUMFPREGS    34
typedef struct target_fpreg {
    uint64_t        r_regs[TARGET_NUMFPREGS];
} target_fpreg_t;

#define tswapreg(ptr)   tswapal(ptr)

static inline void target_copy_regs(target_reg_t *regs, const CPUMIPSState *env)
{
    int i;

    /*
     * See sys/mips/include/frame.h struct trapframe or
     * sys/mips/include/regnum.h for register order.
     */
    for (i = 0; i < 32; i++)
        regs->r_regs[i] = tswapreg(env->active_tc.gpr[i]);
    regs->r_regs[32] = tswapreg(env->CP0_Status);       /* sr */
    regs->r_regs[33] = tswapreg(env->active_tc.LO[0]);  /* mullo */
    regs->r_regs[34] = tswapreg(env->active_tc.HI[0]);  /* mulhi */
    regs->r_regs[35] = tswapreg(env->CP0_BadVAddr);
    regs->r_regs[36] = tswapreg(env->CP0_Cause);
    regs->r_regs[37] = tswapreg(env->active_tc.PC);
    regs->r_regs[38] = 0;   /* ic (RM7k and RM9k specific) */
    regs->r_regs[39] = 0;   /* Dummy for 8 byte alignment */
}

#undef tswapreg

#endif /* !_TARGET_ARCH_REG_H_ */
