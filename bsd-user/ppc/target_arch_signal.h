/*
 *  powerpc signal definitions
 *
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

#define TARGET_INSN_SIZE     4  /* powerpc instruction size */

#if defined(TARGET_PPC64) && !defined(TARGET_ABI32)
/* Size of the signal trampolin code placed on the stack. */
#define TARGET_SZSIGCODE    ((abi_ulong)(12 * TARGET_INSN_SIZE))
#else
#define TARGET_SZSIGCODE    ((abi_ulong)(7 * TARGET_INSN_SIZE))
#endif

#define TARGET_MINSIGSTKSZ  (512 * 4)
#define TARGET_SIGSTKSZ     (TARGET_MINSIGSTKSZ + 32768)

/* compare to sys/powerpc/include/frame.h */
#if defined(TARGET_PPC64) && !defined(TARGET_ABI32)
#define TARGET_SZREG        8
#define TARGET_CALLFRAME_SIZ    (TARGET_SZREG * 10)
#else
#define TARGET_SZREG        4
#define TARGET_CALLFRAME_SIZ    (TARGET_SZREG * 6)
#endif

/* powerpc/powerpc/exec_machdep.c */
#define TARGET_MC_GET_CLEAR_RET 	0x0001
#define TARGET_MC_SET_ONSTACK	 	0x0004
#define TARGET_MC_FP_VALID		0x0001
#define TARGET_MC_AV_VALID		0x0002

typedef struct target_trapframe {
	abi_long fixreg[32];
	abi_long lr;
	abi_long cr;
	abi_long xer;
	abi_long ctr;
	abi_long srr0;
	abi_long srr1;
	abi_long exc;
	union {
		struct {
			/* dar & dsisr are only filled on a DSI trap */
			abi_long dar;
			abi_long dsisr;
		} aim;
		struct {
			abi_long dear;
			abi_long esr;
			abi_long dbcr0;
		} booke;
	} cpu;
} target_trapframe_t;

struct target_sigcontext {
    int32_t            sc_onstack;     /* sigstack state to restore */
    int32_t            __sc_mask13;
    target_trapframe_t sc_frame;
    target_sigset_t    sc_mask;    /* signal mask to retstore */
};

typedef struct target_mcontext {
	int32_t     mc_vers;
	int32_t     mc_flags;
    int32_t     mc_onstack;     /* sigstack state to restore */
    int32_t     mc_len;
    uint64_t    mc_avec[32*2];
    uint32_t    mc_av[2];
    abi_long    mc_frame[42];    /* process regs 0 to 31 */
    uint64_t    mc_fpreg[33];  /* fp regs 0 to 31  */
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
	target_ucontext_t sf_uc;
	target_siginfo_t  sf_si;
};

/*
 * Compare to powerpc/powerpc/exec_machdep.c sendsig()
 * Assumes that target stack frame memory is locked.
 */
static inline abi_long
set_sigtramp_args(CPUPPCState *regs, int sig, struct target_sigframe *frame,
    abi_ulong frame_addr, struct target_sigaction *ka)
{
	/* XXX:TODO: set_sigtramp_args(). */
    /* frame->sf_si.si_addr = regs->CP0_BadVAddr; */

    /*
     * Arguments to signal handler:
     *  r1 = point to sigframe struct
     *  r3 = signal number
     *  r4 = siginfo pointer
     *  r5 = ucontext pointer
     *  PC = sigtramp at base of user stack
     *  lr = signal handler pointer
     */
    regs->gpr[1] = frame_addr;
    regs->gpr[3] = sig;
    regs->gpr[4] = frame_addr +
        offsetof(struct target_sigframe, sf_si);
    regs->gpr[5] = frame_addr +
        offsetof(struct target_sigframe, sf_uc);
    regs->lr = ka->_sa_handler;
    regs->nip = TARGET_PS_STRINGS - TARGET_SZSIGCODE;

    return 0;
}

/*
 * Compare to powerpc/powerpc/exec_machdep.c get_mcontext()
 * Assumes that the memory is locked if mcp points to user memory.
 */
static inline abi_long get_mcontext(CPUPPCState *regs, target_mcontext_t *mcp,
        int flags)
{
    int i, err = 0;

    if (flags & TARGET_MC_SET_ONSTACK) {
        mcp->mc_onstack = tswapal(1);
    } else {
        mcp->mc_onstack = 0;
    }

	mcp->mc_flags = 0;

    for (i = 1; i < 42; i++) {
        mcp->mc_frame[i] = tswapal(regs->gpr[i]);
    }

    mcp->mc_flags |= TARGET_MC_FP_VALID;
    for (i = 0; i < 32; i++) {
        mcp->mc_fpreg[i] = tswapal(regs->fpr[i]);
    }
    mcp->mc_fpreg[32] = tswapal(regs->fpscr);

    mcp->mc_flags |= TARGET_MC_AV_VALID;
    for (i = 0; i < 32*2; i++) {
        mcp->mc_avec[i] = tswapal(regs->avr[i/2].u64[i%2]);
    }
    mcp->mc_av[0] = tswapal(regs->vscr);
    mcp->mc_av[1] = tswapal(regs->spr[SPR_VRSAVE]);

    if (flags & TARGET_MC_GET_CLEAR_RET) {
        mcp->mc_frame[3] = 0;    /* r3 = 0 */
        mcp->mc_frame[4] = 0;    /* r4 = 0 */
    }

	mcp->mc_len = sizeof(*mcp);
    /* Don't do any of the status and cause registers. */

    return err;
}

/* Compare to powerpc/powerpc/exec_machdep.c set_mcontext() */
static inline abi_long set_mcontext(CPUPPCState *regs, target_mcontext_t *mcp,
        int srflag)
{
    abi_long tls;
    int i, err = 0;

#if defined(TARGET_PPC64) && !defined(TARGET_ABI32)
	tls = regs->gpr[13];
#else
	tls = regs->gpr[2];
#endif
    for (i = 1; i < 42; i++) {
        regs->gpr[i] = tswapal(mcp->mc_frame[i]);
    }
#if defined(TARGET_PPC64) && !defined(TARGET_ABI32)
	regs->gpr[13] = tls;
#else
	regs->gpr[2] = tls;
#endif

    if (mcp->mc_flags & TARGET_MC_FP_VALID) {
        /* restore fpu context if we have used it before */
        for (i = 0; i < 32; i++) {
            regs->fpr[i] = tswapal(mcp->mc_fpreg[i]);
        }
        regs->fpscr = tswapal(mcp->mc_fpreg[32]);
    }

    if (mcp->mc_flags & TARGET_MC_AV_VALID) {
        /* restore altivec context if we have used it before */
        for (i = 0; i < 32*2; i++) {
            regs->avr[i/2].u64[i%2] = tswapal(mcp->mc_avec[i]);
        }
        regs->vscr = tswapal(mcp->mc_av[0]);
        regs->spr[SPR_VRSAVE] = tswapal(mcp->mc_av[1]);
    }

    return err;
}

static inline abi_long get_ucontext_sigreturn(CPUPPCState *regs,
                        abi_ulong target_sf, abi_ulong *target_uc)
{

    /* powerpc passes ucontext struct as the stack frame */
    *target_uc = target_sf;
    return 0;
}

#endif /* !_TARGET_ARCH_SIGNAL_H_ */
