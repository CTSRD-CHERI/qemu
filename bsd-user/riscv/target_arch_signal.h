/*
 *  RISC-V signal definitions
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
 
#ifndef _TARGET_ARCH_SIGNAL_H_
#define _TARGET_ARCH_SIGNAL_H_

#include "cpu.h"


#define TARGET_INSN_SIZE     4  /* riscv instruction size */

/* Size of the signal trampoline code placed on the stack. */
#define TARGET_SZSIGCODE    ((abi_ulong)(7 * TARGET_INSN_SIZE))

/* Compare with riscv/include/_limits.h */
#define TARGET_MINSIGSTKSZ  (1024 * 4)
#define TARGET_SIGSTKSZ     (TARGET_MINSIGSTKSZ + 32768)

#define TARGET_MC_GET_CLEAR_RET 0x0001

struct target_sigcontext {
    int32_t     _dummy;
};

struct target_gpregs {
    uint64_t    gp_ra;
    uint64_t    gp_sp;
    uint64_t    gp_gp;
    uint64_t    gp_tp;
    uint64_t    gp_t[7];
    uint64_t    gp_s[12];
    uint64_t    gp_a[8];
    uint64_t    gp_sepc;
    uint64_t    gp_sstatus;
};

struct target_fpregs {
    uint64_t        fp_x[32][2];
    uint64_t        fp_fcsr;
    uint32_t        fp_flags;
    uint32_t        pad;
};


typedef struct target_mcontext {
    struct target_gpregs   mc_gpregs;
    struct target_fpregs   mc_fpregs;
    uint32_t               mc_flags;
#define TARGET_MC_FP_VALID 0x01
    uint32_t               mc_pad;
    uint64_t               mc_spare[8];
} target_mcontext_t;

typedef struct target_ucontext {
    target_sigset_t   uc_sigmask;
    target_mcontext_t uc_mcontext;
    abi_ulong         uc_link;
    target_stack_t    uc_stack;
    int32_t           uc_flags;
    int32_t         __spare__[4];
} target_ucontext_t;

struct target_sigframe {
    target_ucontext_t   sf_uc; /* = *sf_uncontext */
    target_siginfo_t    sf_si; /* = *sf_siginfo (SA_SIGINFO case)*/
};

struct target_trapframe {
    uint64_t tf_ra;
    uint64_t tf_sp;
    uint64_t tf_gp;
    uint64_t tf_tp;
    uint64_t tf_t[7];
    uint64_t tf_s[12];
    uint64_t tf_a[8];
    uint64_t tf_sepc;
    uint64_t tf_sstatus;
    uint64_t tf_stval;
    uint64_t tf_scause;
};

/*
 * Compare with sendsig() in riscv/riscv/machdep.c
 * Assumes that target stack frame memory is locked.
 */
static inline abi_long
set_sigtramp_args(CPURISCVState *regs, int sig, struct target_sigframe *frame,
    abi_ulong frame_addr, struct target_sigaction *ka)
{
    /*
     * Arguments to signal handler:
     *  a0 (10) = signal number
     *  a1 (11) = siginfo pointer
     *  a2 (12) = ucontext pointer
     *  pc      = signal pointer handler
     *  sp (2)  = sigframe pointer
     *  ra (1)  = sigtramp at base of user stack
     */

     regs->gpr[10] = sig;
     regs->gpr[11] = frame_addr +
         offsetof(struct target_sigframe, sf_si);
     regs->gpr[12] = frame_addr +
         offsetof(struct target_sigframe, sf_uc);
     regs->pc = ka->_sa_handler;
     regs->gpr[2] = frame_addr;
     regs->gpr[1] = TARGET_PS_STRINGS - TARGET_SZSIGCODE;
     return 0;
}

/*
 * Compare with get_mcontext() in riscv/riscv/machdep.c
 * Assumes that the memory is locked if mcp points to user memory.
 */
static inline abi_long get_mcontext(CPURISCVState *regs, target_mcontext_t *mcp,
        int flags)
{

    mcp->mc_gpregs.gp_t[0] = tswap64(regs->gpr[5]);
    mcp->mc_gpregs.gp_t[1] = tswap64(regs->gpr[6]);
    mcp->mc_gpregs.gp_t[2] = tswap64(regs->gpr[7]);
    mcp->mc_gpregs.gp_t[3] = tswap64(regs->gpr[28]);
    mcp->mc_gpregs.gp_t[4] = tswap64(regs->gpr[29]);
    mcp->mc_gpregs.gp_t[5] = tswap64(regs->gpr[30]);
    mcp->mc_gpregs.gp_t[6] = tswap64(regs->gpr[31]);

    mcp->mc_gpregs.gp_s[0] = tswap64(regs->gpr[8]);
    mcp->mc_gpregs.gp_s[1] = tswap64(regs->gpr[9]);
    mcp->mc_gpregs.gp_s[2] = tswap64(regs->gpr[18]);
    mcp->mc_gpregs.gp_s[3] = tswap64(regs->gpr[19]);
    mcp->mc_gpregs.gp_s[4] = tswap64(regs->gpr[20]);
    mcp->mc_gpregs.gp_s[5] = tswap64(regs->gpr[21]);
    mcp->mc_gpregs.gp_s[6] = tswap64(regs->gpr[22]);
    mcp->mc_gpregs.gp_s[7] = tswap64(regs->gpr[23]);
    mcp->mc_gpregs.gp_s[8] = tswap64(regs->gpr[24]);
    mcp->mc_gpregs.gp_s[9] = tswap64(regs->gpr[25]);
    mcp->mc_gpregs.gp_s[10] = tswap64(regs->gpr[26]);
    mcp->mc_gpregs.gp_s[11] = tswap64(regs->gpr[27]);

    mcp->mc_gpregs.gp_a[0] = tswap64(regs->gpr[10]);
    mcp->mc_gpregs.gp_a[1] = tswap64(regs->gpr[11]);
    mcp->mc_gpregs.gp_a[2] = tswap64(regs->gpr[12]);
    mcp->mc_gpregs.gp_a[3] = tswap64(regs->gpr[13]);
    mcp->mc_gpregs.gp_a[4] = tswap64(regs->gpr[14]);
    mcp->mc_gpregs.gp_a[5] = tswap64(regs->gpr[15]);
    mcp->mc_gpregs.gp_a[6] = tswap64(regs->gpr[16]);
    mcp->mc_gpregs.gp_a[7] = tswap64(regs->gpr[17]);

    if (flags & TARGET_MC_GET_CLEAR_RET) {
        mcp->mc_gpregs.gp_a[0] = 0; /* a0 */        
        mcp->mc_gpregs.gp_a[1] = 0; /* a1 */        
        mcp->mc_gpregs.gp_t[0] = 0; /* clear syscall error */
    }

    mcp->mc_gpregs.gp_ra = tswap64(regs->gpr[1]);
    mcp->mc_gpregs.gp_sp = tswap64(regs->gpr[2]);
    mcp->mc_gpregs.gp_gp = tswap64(regs->gpr[3]);
    mcp->mc_gpregs.gp_tp = tswap64(regs->gpr[4]);
    mcp->mc_gpregs.gp_sepc = tswap64(regs->pc);

    return 0;
}

/* Compare with set_mcontext() in riscv/riscv/machdep.c */
static inline abi_long set_mcontext(CPURISCVState *regs, target_mcontext_t *mcp,
        int srflag)
{

    regs->gpr[5] = tswap64(mcp->mc_gpregs.gp_t[0]);
    regs->gpr[6] = tswap64(mcp->mc_gpregs.gp_t[1]);
    regs->gpr[7] = tswap64(mcp->mc_gpregs.gp_t[2]);
    regs->gpr[28] = tswap64(mcp->mc_gpregs.gp_t[3]);
    regs->gpr[29] = tswap64(mcp->mc_gpregs.gp_t[4]);
    regs->gpr[30] = tswap64(mcp->mc_gpregs.gp_t[5]);
    regs->gpr[31] = tswap64(mcp->mc_gpregs.gp_t[6]);

    regs->gpr[8] = tswap64(mcp->mc_gpregs.gp_s[0]);
    regs->gpr[9] = tswap64(mcp->mc_gpregs.gp_s[1]);
    regs->gpr[18] = tswap64(mcp->mc_gpregs.gp_s[2]);
    regs->gpr[19] = tswap64(mcp->mc_gpregs.gp_s[3]);
    regs->gpr[20] = tswap64(mcp->mc_gpregs.gp_s[4]);
    regs->gpr[21] = tswap64(mcp->mc_gpregs.gp_s[5]);
    regs->gpr[22] = tswap64(mcp->mc_gpregs.gp_s[6]);
    regs->gpr[23] = tswap64(mcp->mc_gpregs.gp_s[7]);
    regs->gpr[24] = tswap64(mcp->mc_gpregs.gp_s[8]);
    regs->gpr[25] = tswap64(mcp->mc_gpregs.gp_s[9]);
    regs->gpr[26] = tswap64(mcp->mc_gpregs.gp_s[10]);
    regs->gpr[27] = tswap64(mcp->mc_gpregs.gp_s[11]);

    regs->gpr[10] = tswap64(mcp->mc_gpregs.gp_a[0]);
    regs->gpr[11] = tswap64(mcp->mc_gpregs.gp_a[1]);
    regs->gpr[12] = tswap64(mcp->mc_gpregs.gp_a[2]);
    regs->gpr[13] = tswap64(mcp->mc_gpregs.gp_a[3]);
    regs->gpr[14] = tswap64(mcp->mc_gpregs.gp_a[4]);
    regs->gpr[15] = tswap64(mcp->mc_gpregs.gp_a[5]);
    regs->gpr[16] = tswap64(mcp->mc_gpregs.gp_a[6]);
    regs->gpr[17] = tswap64(mcp->mc_gpregs.gp_a[7]);


    regs->gpr[1] = tswap64(mcp->mc_gpregs.gp_ra);
    regs->gpr[2] = tswap64(mcp->mc_gpregs.gp_sp);
    regs->gpr[3] = tswap64(mcp->mc_gpregs.gp_gp);
    regs->gpr[4] = tswap64(mcp->mc_gpregs.gp_tp);
    regs->pc = tswap64(mcp->mc_gpregs.gp_sepc);

    return 0;
}

/* Compare with sys_sigreturn() in riscv/riscv/machdep.c */
static inline abi_long get_ucontext_sigreturn(CPURISCVState *regs,
                        abi_ulong target_sf, abi_ulong *target_uc)
{

    *target_uc = target_sf;
    return 0;
}

#endif /* !_TARGET_ARCH_SIGNAL_H_ */
