/*
 *  sparc64 dependent signal definitions
 *
 *    Copyright (c) 2015 Stacey D. Son
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

#define UREG_O0     16
#define UREG_O1     17
#define UREG_O2     18
#define UREG_O3     19
#define UREG_O4     20
#define UREG_O5     21
#define UREG_O6     22
#define UREG_O7     23


/* Size of the signal trampolin code placed on the stack. */
/* #define TARGET_SZSIGCODE    (0) */      /* XXX to be added. */

/* compare to  sparc64/include/_limits.h */
#define TARGET_MINSIGSTKSZ  (1024 * 4)              /* min sig stack size */
#define TARGET_SIGSTKSZ     (MINSIGSTKSZ + 32768)   /* recommended size */

#define TARGET_MC_GET_CLEAR_RET 0x0001

struct target_sigcontext {
    /* to be added */
};

/* See sparc64/include/ucontext.h */
struct target_mcontext {
    uint64_t    mc_global[8];
    uint64_t    mc_out[8];
    uint64_t    mc_local[8];
    uint64_t    mc_in[8];
    uint32_t    mc_fp[64];
} __aligned(64);

#define _mc_flags   mc_global[0]
#define _mc_sp      mc_out[6]
#define _mc_fprs    mc_local[0]
#define _mc_fsr     mc_local[1]
#define _mc_gsr     mc_local[2]
#define _mc_tnpc    mc_in[0]
#define _mc_tpc     mc_in[1]
#define _mc_tstate  mc_in[2]
#define _mc_y       mc_in[4]
#define _mc_wstate  mc_in[5]

#define _MC_VERSION 1L

typedef struct target_mcontext target_mcontext_t;

typedef struct target_ucontext {
    target_sigset_t   uc_sigmask;
    target_mcontext_t uc_mcontext;
    abi_ulong         uc_link;
    target_stack_t    uc_stack;
    int32_t           uc_flags;
    int32_t         __spare__[4];
} target_ucontext_t;

struct target_sigframe {
    abi_ulong   sf_signum;
    abi_ulong   sf_siginfo;    /* code or pointer to sf_si */
    abi_ulong   sf_ucontext;   /* points to sf_uc */
    abi_ulong   sf_addr;       /* undocumented 4th arg */
    target_ucontext_t   sf_uc; /* = *sf_uncontext */
    target_siginfo_t    sf_si; /* = *sf_siginfo (SA_SIGINFO case)*/
    uint32_t    __spare__[2];
};

/*
 * Compare to sparc64/sparc64/machdep.c sendsig()
 * Assumes that target stack frame memory is locked.
 */
static inline abi_long set_sigtramp_args(CPUSPARCState *regs,
        int sig, struct target_sigframe *frame, abi_ulong frame_addr,
        struct target_sigaction *ka)
{
    /* XXX */
    printf ("incomplete %s: %s:%d\n", __func__, __FILE__, __LINE__);
    exit(-1);
    return -TARGET_EOPNOTSUPP;
}

/* Compare to sparc64/sparc64/machdep.c get_mcontext() */
static inline abi_long get_mcontext(CPUSPARCState *regs,
        target_mcontext_t *mcp, int flags)
{
    trap_state *tsptr = cpu_tsptr(regs);

    mcp->_mc_flags = _MC_VERSION;
    mcp->mc_global[1] = tswapal(regs->gregs[1]);
    mcp->mc_global[2] = tswapal(regs->gregs[2]);
    mcp->mc_global[3] = tswapal(regs->gregs[3]);
    mcp->mc_global[4] = tswapal(regs->gregs[4]);
    mcp->mc_global[5] = tswapal(regs->gregs[5]);
    mcp->mc_global[6] = tswapal(regs->gregs[6]);
    if (flags & TARGET_MC_GET_CLEAR_RET) {
        mcp->mc_out[0] = 0;
        mcp->mc_out[1] = 0;
    } else {
        mcp->mc_out[0] = tswapal(regs->regwptr[UREG_O0]);
        mcp->mc_out[1] = tswapal(regs->regwptr[UREG_O1]);
    }
    mcp->mc_out[2] = tswapal(regs->regwptr[UREG_O2]);
    mcp->mc_out[3] = tswapal(regs->regwptr[UREG_O3]);
    mcp->mc_out[4] = tswapal(regs->regwptr[UREG_O4]);
    mcp->mc_out[5] = tswapal(regs->regwptr[UREG_O5]);
    mcp->mc_out[6] = tswapal(regs->regwptr[UREG_O6]);
    mcp->mc_out[7] = tswapal(regs->regwptr[UREG_O7]);
    mcp->_mc_fprs = tswapal(regs->fprs);
    mcp->_mc_fsr = tswapal(regs->fsr);
    mcp->_mc_gsr = tswapal(regs->gsr);
    mcp->_mc_tnpc = tswapal(tsptr->tnpc);
    mcp->_mc_tpc = tswapal(tsptr->tpc);
    mcp->_mc_tstate = tswapal(tsptr->tstate);
    mcp->_mc_y = tswapal(regs->y);

    /* XXX FPRS_FEF flag and floating point regs */

    return 0;
}

/* Compare to sparc64/space64/machdep.c set_mcontext() */
static inline abi_long set_mcontext(CPUSPARCState *regs,
        target_mcontext_t *mcp, int srflag)
{
    trap_state *tsptr = cpu_tsptr(regs);

    regs->gregs[1] = tswapal(mcp->mc_global[1]);
    regs->gregs[2] = tswapal(mcp->mc_global[2]);
    regs->gregs[3] = tswapal(mcp->mc_global[3]);
    regs->gregs[4] = tswapal(mcp->mc_global[4]);
    regs->gregs[5] = tswapal(mcp->mc_global[5]);
    regs->gregs[6] = tswapal(mcp->mc_global[6]);

    regs->regwptr[UREG_O0] = tswapal(mcp->mc_out[0]);
    regs->regwptr[UREG_O1] = tswapal(mcp->mc_out[1]);
    regs->regwptr[UREG_O2] = tswapal(mcp->mc_out[2]);
    regs->regwptr[UREG_O3] = tswapal(mcp->mc_out[3]);
    regs->regwptr[UREG_O4] = tswapal(mcp->mc_out[4]);
    regs->regwptr[UREG_O5] = tswapal(mcp->mc_out[5]);
    regs->regwptr[UREG_O6] = tswapal(mcp->mc_out[6]);
    regs->regwptr[UREG_O7] = tswapal(mcp->mc_out[7]);

    regs->fprs = tswapal(mcp->_mc_fprs);
    regs->fsr = tswapal(mcp->_mc_fsr);
    regs->gsr = tswapal(mcp->_mc_gsr);
    tsptr->tnpc = tswapal(mcp->_mc_tnpc);
    tsptr->tpc = tswapal(mcp->_mc_tpc);
    tsptr->tstate = tswapal(mcp->_mc_tstate);
    regs->y = tswapal(mcp->_mc_y);

    /* XXX FPRS_FEF flag and floating point regs */

    return 0;
}

static inline abi_long get_ucontext_sigreturn(CPUSPARCState *regs,
        abi_ulong target_sf, abi_ulong *target_uc)
{
    /* XXX */
    printf ("incomplete %s: %s:%d\n", __func__, __FILE__, __LINE__);
    exit(-1);
    *target_uc = 0;
    return -TARGET_EOPNOTSUPP;
}

#endif /* !_TARGET_ARCH_SIGNAL_H_ */
