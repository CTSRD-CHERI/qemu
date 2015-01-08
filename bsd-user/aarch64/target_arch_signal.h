/*
 * ARM AArch64 specific signal definitions for bsd-user
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

#ifndef _TARGET_ARCH_SIGNAL_H_
#define _TARGET_ARCH_SIGNAL_H_

#include "cpu.h"

#define TARGET_REG_X0   0
#define TARGET_REG_X30  30
#define TARGET_REG_X31  31
#define TARGET_REG_LR   TARGET_REG_X30
#define TARGET_REG_SP   TARGET_REG_X31

#define TARGET_INSN_SIZE    4       /* arm64 instruction size */

/* Size of the signal trampolin code. See _sigtramp(). */
#define TARGET_SZSIGCODE    ((abi_ulong)(9 * TARGET_INSN_SIZE))

/* compare to sys/arm64/include/_limits.h */
#define TARGET_MINSIGSTKSZ  (1024 * 4)                  /* min sig stack size */
#define TARGET_SIGSTKSZ     (TARGET_MINSIGSTKSZ + 32768)  /* recommended size */

/* sys/sys/ucontext.h XXX maybe should be in a FreeBSD wide header */
#define TARGET_MC_GET_CLEAR_RET 0x0001

/* sys/arm64/include/signal.h */
struct target_sigcontext {
    int32_t     _dummy;
};

/* struct __mcontext in sys/arm64/include/ucontext.h */

struct target_gpregs {
    uint64_t    gp_x[30];
    uint64_t    gp_lr;
    uint64_t    gp_sp;
    uint64_t    gp_elr;
    uint32_t    gp_spsr;
    uint32_t	gp_pad;
};

struct target_fpregs {
    __uint128_t fp_q[32];
    uint32_t    fp_sr;
    uint32_t    fp_cr;
    uint32_t    fp_flags;
    uint32_t	fp_pad;
};

struct target__mcontext {
    struct target_gpregs mc_gpregs;
    struct target_fpregs mc_fpregs;
    uint32_t    mc_flags;
#define TARGET_MC_FP_VALID  0x1
    uint32_t	mc_pad;
    uint64_t	mc_spare[8];
};

typedef struct target__mcontext target_mcontext_t;

typedef struct target_ucontext {
    target_sigset_t     uc_sigmask;
    target_mcontext_t   uc_mcontext;
    abi_ulong           uc_link;
    target_stack_t      uc_stack;
    int32_t             uc_flags;
    int32_t             __spare__[4];
} target_ucontext_t;

struct target_sigframe {
    target_siginfo_t    sf_si;  /* saved siginfo */
    target_ucontext_t   sf_uc;  /* saved ucontext */
};


/* compare to struct trapframe in sys/arm64/include/frame.h */
struct target_trapframe {
        uint64_t    tf_sp;
        uint64_t    tf_lr;
        uint64_t    tf_elr;
        uint64_t    tf_spsr;
        uint64_t    tf_x[30];
};

/*
 * Compare to sendsig() in sys/arm64/arm64/machdep.c
 * Assumes that target stack frame memory is locked.
 */
static inline abi_long
set_sigtramp_args(CPUARMState *regs, int sig, struct target_sigframe *frame,
    abi_ulong frame_addr, struct target_sigaction *ka)
{
    /*
     * Arguments to signal handler:
     *  x0 = signal number
     *  x1 = siginfo pointer
     *  x2 = ucontext pointer
     *  pc/elr = signal handler pointer
     *  sp = sigframe struct pointer
     *  lr = sigtramp at base of user stack
     */

    regs->xregs[0] = sig;
    regs->xregs[1] = frame_addr +
        offsetof(struct target_sigframe, sf_si);
    regs->xregs[2] = frame_addr +
        offsetof(struct target_sigframe, sf_uc);

    regs->pc = ka->_sa_handler;
    regs->xregs[TARGET_REG_SP] = frame_addr;
    regs->xregs[TARGET_REG_LR] = TARGET_PS_STRINGS - TARGET_SZSIGCODE;

    return 0;
}

/*
 * Compare to get_mcontext() in arm64/arm64/machdep.c
 * Assumes that the memory is locked if mcp points to user memory.
 */
static inline abi_long get_mcontext(CPUARMState *regs, target_mcontext_t *mcp,
        int flags)
{
    int err = 0, i;
    uint64_t *gr = mcp->mc_gpregs.gp_x;


    if (flags & TARGET_MC_GET_CLEAR_RET) {
        gr[0] = 0UL;
    } else {
        gr[0] = tswap64(regs->xregs[0]);
    }

    for (i = 1; i < 30; i++)
        gr[i] = tswap64(regs->xregs[i]);

    mcp->mc_gpregs.gp_sp = tswap64(regs->xregs[TARGET_REG_SP]);
    mcp->mc_gpregs.gp_lr = tswap64(regs->xregs[TARGET_REG_LR]);
    mcp->mc_gpregs.gp_elr = tswap64(regs->pc);
    mcp->mc_gpregs.gp_spsr = pstate_read(regs);

    /* XXX FP? */

    return err;
}

/* Compare to set_mcontext() in arm64/arm64/machdep.c */
static inline abi_long set_mcontext(CPUARMState *regs, target_mcontext_t *mcp,
        int srflag)
{
    int err = 0, i;
    const uint64_t *gr = mcp->mc_gpregs.gp_x;

    for (i = 0; i < 30; i++)
        regs->xregs[i] = tswap64(gr[i]);

    regs->xregs[TARGET_REG_SP] = tswap64(mcp->mc_gpregs.gp_sp);
    regs->xregs[TARGET_REG_LR] = tswap64(mcp->mc_gpregs.gp_lr);
    regs->pc = mcp->mc_gpregs.gp_elr;
    pstate_write(regs, mcp->mc_gpregs.gp_spsr);

    /* XXX FP? */

    return err;
}

/* Compare to sys_sigreturn() in  arm64/arm64/machdep.c */
static inline abi_long get_ucontext_sigreturn(CPUARMState *regs,
        abi_ulong target_sf, abi_ulong *target_uc)
{
    uint32_t pstate = pstate_read(regs);

    *target_uc = 0;

    if ((pstate & PSTATE_M) != PSTATE_MODE_EL0t  ||
        (pstate & (PSTATE_F | PSTATE_I | PSTATE_A | PSTATE_D)) != 0) {
        return -TARGET_EINVAL;
    }

    *target_uc = target_sf;

    return 0;
}


#endif /* !_TARGET_ARCH_SIGNAL_H_ */
