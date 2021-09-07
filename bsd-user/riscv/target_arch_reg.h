/*
 *  RISC-V register structures
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

#ifndef _TARGET_ARCH_REG_H_
#define _TARGET_ARCH_REG_H_

/* Compare with riscv/include/reg.h */
typedef struct target_reg {
    uint64_t ra;            /* return address */
    uint64_t sp;            /* stack pointer */
    uint64_t gp;            /* global pointer */
    uint64_t tp;            /* thread pointer */
    uint64_t t[7];          /* temporaries */
    uint64_t s[12];         /* saved registers */
    uint64_t a[8];          /* function arguments */
    uint64_t sepc;          /* exception program counter */
    uint64_t sstatus;       /* status register */
} target_reg_t;

typedef struct target_fpreg {
    uint64_t        fp_x[32][2];    /* Floating point registers */
    uint64_t        fp_fcsr;        /* Floating point control reg */
} target_fpreg_t;

#define tswapreg(ptr)   tswapal(ptr)

/* Compare with struct trapframe in riscv/include/frame.h */
static inline void target_copy_regs(target_reg_t *regs, const CPURISCVState *env)
{

    regs->ra = tswapreg(gpr_int_value(env, xRA));
    regs->sp = tswapreg(gpr_int_value(env, xSP));
    regs->gp = tswapreg(gpr_int_value(env, xGP));
    regs->tp = tswapreg(gpr_int_value(env, xTP));

    regs->t[0] = tswapreg(gpr_int_value(env, xT0));
    regs->t[1] = tswapreg(gpr_int_value(env, xT1));
    regs->t[2] = tswapreg(gpr_int_value(env, xT2));
    regs->t[3] = tswapreg(gpr_int_value(env, xT3));
    regs->t[4] = tswapreg(gpr_int_value(env, xT4));
    regs->t[5] = tswapreg(gpr_int_value(env, xT5));
    regs->t[6] = tswapreg(gpr_int_value(env, xT6));

    regs->s[0] = tswapreg(gpr_int_value(env, xS0));
    regs->s[1] = tswapreg(gpr_int_value(env, xS1));
    regs->s[2] = tswapreg(gpr_int_value(env, xS2));
    regs->s[3] = tswapreg(gpr_int_value(env, xS3));
    regs->s[4] = tswapreg(gpr_int_value(env, xS4));
    regs->s[5] = tswapreg(gpr_int_value(env, xS5));
    regs->s[6] = tswapreg(gpr_int_value(env, xS6));
    regs->s[7] = tswapreg(gpr_int_value(env, xS7));
    regs->s[8] = tswapreg(gpr_int_value(env, xS8));
    regs->s[9] = tswapreg(gpr_int_value(env, xS9));
    regs->s[10] = tswapreg(gpr_int_value(env, xS10));
    regs->s[11] = tswapreg(gpr_int_value(env, xS11));

    regs->a[0] = tswapreg(gpr_int_value(env, xA0));
    regs->a[1] = tswapreg(gpr_int_value(env, xA1));
    regs->a[2] = tswapreg(gpr_int_value(env, xA2));
    regs->a[3] = tswapreg(gpr_int_value(env, xA3));
    regs->a[4] = tswapreg(gpr_int_value(env, xA4));
    regs->a[5] = tswapreg(gpr_int_value(env, xA5));
    regs->a[6] = tswapreg(gpr_int_value(env, xA6));
    regs->a[7] = tswapreg(gpr_int_value(env, xA7));

    regs->sepc = tswapreg(riscv_fetch_pc(env));
}

#undef tswapreg

#endif /* !_TARGET_ARCH_REG_H_ */
