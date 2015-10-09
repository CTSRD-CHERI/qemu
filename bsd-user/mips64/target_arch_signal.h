/*
 *  mips64 signal definitions
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

#define TARGET_INSN_SIZE     4  /* mips64 instruction size */

/* Size of the signal trampolin code placed on the stack. */
#define TARGET_SZSIGCODE    ((abi_ulong)(4 * TARGET_INSN_SIZE))

#define TARGET_MINSIGSTKSZ  (512 * 4)
#define TARGET_SIGSTKSZ     (TARGET_MINSIGSTKSZ + 32768)

/* compare to sys/mips/include/asm.h */
#define TARGET_SZREG        8
#define TARGET_CALLFRAME_SIZ    (TARGET_SZREG * 4)

/* mips/mips/pm_machdep.c */
#define TARGET_UCONTEXT_MAGIC   0xACEDBADE
#define TARGET_MC_GET_CLEAR_RET 0x0001
#define TARGET_MC_ADD_MAGIC     0x0002
#define TARGET_MC_SET_ONSTACK   0x0004

struct target_sigcontext {
    target_sigset_t sc_mask;    /* signal mask to retstore */
    int32_t     sc_onstack;     /* sigstack state to restore */
    abi_long    sc_pc;          /* pc at time of signal */
    abi_long    sc_reg[32];     /* processor regs 0 to 31 */
    abi_long    mullo, mulhi;   /* mullo and mulhi registers */
    int32_t     sc_fpused;      /* fp has been used */
    abi_long    sc_fpregs[33];  /* fp regs 0 to 31 & csr */
    abi_long    sc_fpc_eir;     /* fp exception instr reg */
    /* int32_t reserved[8]; */
};

typedef struct target_mcontext {
    int32_t     mc_onstack;     /* sigstack state to restore */
    abi_long    mc_pc;          /* pc at time of signal */
    abi_long    mc_regs[32];    /* process regs 0 to 31 */
    abi_long    sr;             /* status register */
    abi_long    mullo, mulhi;
    int32_t     mc_fpused;      /* fp has been used */
    abi_long    mc_fpregs[33];  /* fp regs 0 to 32 & csr */
    abi_long    mc_fpc_eir;     /* fp exception instr reg */
    abi_ulong   mc_tls;         /* pointer to TLS area */
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
    abi_ulong   sf_signum;
    abi_ulong   sf_siginfo;    /* code or pointer to sf_si */
    abi_ulong   sf_ucontext;   /* points to sf_uc */
    abi_ulong   sf_addr;       /* undocumented 4th arg */
    target_ucontext_t   sf_uc; /* = *sf_uncontext */
    target_siginfo_t    sf_si; /* = *sf_siginfo (SA_SIGINFO case)*/
    uint32_t    __spare__[2];
};

/* Forward declare due to unfortunate header nesting */
void target_cpu_set_tls(CPUMIPSState *env, target_ulong newtls);
target_ulong target_cpu_get_tls(CPUMIPSState *env);

/*
 * Compare to mips/mips/pm_machdep.c sendsig()
 * Assumes that target stack frame memory is locked.
 */
static inline abi_long
set_sigtramp_args(CPUMIPSState *regs, int sig, struct target_sigframe *frame,
    abi_ulong frame_addr, struct target_sigaction *ka)
{

    /* frame->sf_si.si_addr = regs->CP0_BadVAddr; */

    /* MIPS only struct target_sigframe members: */
    frame->sf_signum = sig;
    frame->sf_siginfo = frame_addr + offsetof(struct target_sigframe, sf_si);
    frame->sf_ucontext = frame_addr + offsetof(struct target_sigframe, sf_uc);

    /*
     * Arguments to signal handler:
     *  a0 ($4) = signal number
     *  a1 ($5) = siginfo pointer
     *  a2 ($6) = ucontext pointer
     *  PC = signal handler pointer
     *  t9 ($25) = signal handler pointer
     *  $29 = point to sigframe struct
     *  ra ($31) = sigtramp at base of user stack
     */
    regs->active_tc.gpr[4] = sig;
    regs->active_tc.gpr[5] = frame_addr +
        offsetof(struct target_sigframe, sf_si);
    regs->active_tc.gpr[6] = frame_addr +
        offsetof(struct target_sigframe, sf_uc);
    regs->active_tc.gpr[25] = regs->active_tc.PC = ka->_sa_handler;
    regs->active_tc.gpr[29] = frame_addr;
    regs->active_tc.gpr[31] = TARGET_PS_STRINGS - TARGET_SZSIGCODE;

    return 0;
}

/*
 * Compare to mips/mips/pm_machdep.c get_mcontext()
 * Assumes that the memory is locked if mcp points to user memory.
 */
static inline abi_long get_mcontext(CPUMIPSState *regs, target_mcontext_t *mcp,
        int flags)
{
    int i, err = 0;

    if (flags & TARGET_MC_ADD_MAGIC) {
        mcp->mc_regs[0] = tswapal(TARGET_UCONTEXT_MAGIC);
    } else {
        mcp->mc_regs[0] = 0;
    }

    if (flags & TARGET_MC_SET_ONSTACK) {
        mcp->mc_onstack = tswapal(1);
    } else {
        mcp->mc_onstack = 0;
    }

    for (i = 1; i < 32; i++) {
        mcp->mc_regs[i] = tswapal(regs->active_tc.gpr[i]);
    }

    mcp->mc_fpused = 1;
    for (i = 0; i < 32; i++) {
        mcp->mc_fpregs[i] = tswapal(regs->active_fpu.fpr[i].d);
    }
    mcp->mc_fpregs[32] = tswapal(regs->active_fpu.fcr0);
    mcp->mc_fpc_eir = tswapal(regs->active_fpu.fcr31);

    if (flags & TARGET_MC_GET_CLEAR_RET) {
        mcp->mc_regs[2] = 0;    /* v0 = 0 */
        mcp->mc_regs[3] = 0;    /* v1 = 0 */
        mcp->mc_regs[7] = 0;    /* a3 = 0 */
    }

    mcp->mc_pc = tswapal(regs->active_tc.PC);
    mcp->mullo = tswapal(regs->active_tc.LO[0]);
    mcp->mulhi = tswapal(regs->active_tc.HI[0]);
    mcp->mc_tls = tswapal(target_cpu_get_tls(regs));

    /* Don't do any of the status and cause registers. */

    return err;
}

/* Compare to mips/mips/pm_machdep.c set_mcontext() */
static inline abi_long set_mcontext(CPUMIPSState *regs, target_mcontext_t *mcp,
        int srflag)
{
    int i, err = 0;

    for (i = 1; i < 32; i++) {
        regs->active_tc.gpr[i] = tswapal(mcp->mc_regs[i]);
    }

    if (mcp->mc_fpused) {
        /* restore fpu context if we have used it before */
        for (i = 0; i < 32; i++) {
            regs->active_fpu.fpr[i].d = tswapal(mcp->mc_fpregs[i]);
        }
        regs->active_fpu.fcr0 = tswapal(mcp->mc_fpregs[32]);
        regs->active_fpu.fcr31 = tswapal(mcp->mc_fpc_eir);
    }

    regs->CP0_EPC = tswapal(mcp->mc_pc);
    regs->active_tc.LO[0] = tswapal(mcp->mullo);
    regs->active_tc.HI[0] = tswapal(mcp->mulhi);
    target_cpu_set_tls(regs, tswapal(mcp->mc_tls));

    if (srflag) {
        /* doing sigreturn() */
        regs->active_tc.PC = regs->CP0_EPC;
        regs->CP0_EPC = 0;  /* XXX  for nested signals ? */
    }

    /* Don't do any of the status and cause registers. */

    return err;
}

static inline abi_long get_ucontext_sigreturn(CPUMIPSState *regs,
                        abi_ulong target_sf, abi_ulong *target_uc)
{

    /* mips passes ucontext struct as the stack frame */
    *target_uc = target_sf;
    return 0;
}

#endif /* !_TARGET_ARCH_SIGNAL_H_ */
