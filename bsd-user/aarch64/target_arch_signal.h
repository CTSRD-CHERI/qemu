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

#include "errno_defs.h"

#ifdef TARGET_CHERI
#include "cheri/cheric.h"
#include "machine/cheri.h"
#endif

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

#ifdef TARGET_CHERI
struct target_capregs {
    abi_uintcap_t   cap_x[30];
    abi_uintcap_t   cap_lr;
    abi_uintcap_t   cap_sp;
    abi_uintcap_t   cap_elr;
    abi_uintcap_t   cap_ddc;
};
#endif

struct target__mcontext {
#ifdef TARGET_CHERI
    struct target_capregs mc_capregs;
#else
    struct target_gpregs  mc_gpregs;
#endif
    struct target_fpregs  mc_fpregs;
    uint32_t    mc_flags;
#define TARGET_MC_FP_VALID  0x1
#ifdef TARGET_CHERI
    uint32_t    mc_spsr;
    uint64_t    mc_spare[8];
#else
    uint32_t    mc_pad;
    uint64_t    mc_spare[7];
    uint64_t    mc_capregs;
#endif
};

typedef struct target__mcontext target_mcontext_t;

typedef struct target_ucontext {
    target_sigset_t     uc_sigmask;
    target_mcontext_t   uc_mcontext;
    abi_uintptr_t       uc_link;
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
#ifdef TARGET_CHERI
    abi_uintcap_t tf_sp;
    abi_uintcap_t tf_lr;
    abi_uintcap_t tf_elr;
    abi_uintcap_t tf_ddc;
#else
    uint64_t      tf_sp;
    uint64_t      tf_lr;
    uint64_t      tf_elr;
#endif
    uint32_t      tf_spsr;
    uint32_t      tf_esr;
#ifdef TARGET_CHERI
    uint64_t      tf_pad;
    abi_uintcap_t tf_x[30];
#else
    uint64_t      tf_x[30];
#endif
};

/*
 * Compare to sendsig() in sys/arm64/arm64/machdep.c
 * Assumes that target stack frame memory is locked.
 */
static inline abi_long
set_sigtramp_args(CPUARMState *regs, TaskState *ts, int sig,
    struct target_sigframe *frame, abi_ulong frame_addr,
    struct target_sigaction *ka)
{
#ifdef TARGET_CHERI
    cap_register_t cap;
#endif

    /*
     * Arguments to signal handler:
     *  x0 = signal number
     *  x1 = siginfo pointer
     *  x2 = ucontext pointer
     *  pc/elr = signal handler pointer
     *  sp = sigframe struct pointer
     *  lr = sigtramp at base of user stack
     */

#ifdef TARGET_CHERI
    update_capreg_to_intval(regs, 0, sig);
    update_capreg(regs, 1, cheri_ptr((void *)(uintptr_t)(frame_addr +
        offsetof(typeof(*frame), sf_si)), sizeof(frame->sf_si)));
    update_capreg(regs, 2, cheri_ptr((void *)(uintptr_t)(frame_addr +
        offsetof(typeof(*frame), sf_uc)), sizeof(frame->sf_uc)));
    cheri_load(&cap, &ka->_sa_handler);
    if (ts->info->benchmarkabi)
        (void)cheri_setaddress(&regs->pc.cap, cheri_getaddress(&cap));
    else
        regs->pc.cap = cap;
    cheri_prepare_pcc(&regs->pc.cap, regs);
    update_capreg(regs, TARGET_REG_SP, cheri_setaddress(cheri_zerocap(),
        (uintptr_t)frame_addr));
    update_capreg(regs, TARGET_REG_LR, &ts->cheri_sigcode_cap);
#else
    regs->xregs[0] = sig;
    regs->xregs[1] = frame_addr +
        offsetof(struct target_sigframe, sf_si);
    regs->xregs[2] = frame_addr +
        offsetof(struct target_sigframe, sf_uc);

    regs->pc = ka->_sa_handler;
    regs->xregs[TARGET_REG_SP] = frame_addr;
    regs->xregs[TARGET_REG_LR] = TARGET_PS_STRINGS - TARGET_SZSIGCODE;
#endif

    return 0;
}

#ifdef TARGET_CHERI
static inline abi_long get_mcontext(CPUARMState *regs, target_mcontext_t *mcp,
    int flags)
{
    int ii;

    if (flags & TARGET_MC_GET_CLEAR_RET) {
        cheri_store(&mcp->mc_capregs.cap_x[0], cheri_nullcap());
        mcp->mc_spsr = pstate_read(regs) & ~CPSR_C;
    } else {
        cheri_store(&mcp->mc_capregs.cap_x[0], get_readonly_capreg(regs, 0));
        mcp->mc_spsr = pstate_read(regs);
    }

    for (ii = 1; ii < 30; ii++) {
        cheri_store(&mcp->mc_capregs.cap_x[ii], get_readonly_capreg(regs, ii));
    }

    cheri_store(&mcp->mc_capregs.cap_sp, get_readonly_capreg(regs,
        TARGET_REG_SP));
    cheri_store(&mcp->mc_capregs.cap_lr, get_readonly_capreg(regs,
        TARGET_REG_LR));
    /*
     * XXXKW: How can we synchronize PCC to make sure it's current?
     */
    cheri_store(&mcp->mc_capregs.cap_elr, _cheri_get_pcc_unchecked(regs));
    cheri_store(&mcp->mc_capregs.cap_ddc, cheri_get_ddc(regs));

    /* XXX FP? */

    return (0);
}

static inline abi_long set_mcontext(CPUARMState *regs, target_mcontext_t *mcp,
        int srflag)
{
    int ii;
    uint32_t spsr;
    cap_register_t cap;

    spsr = mcp->mc_spsr;
    if ((spsr & PSTATE_M) != PSTATE_MODE_EL0t ||
        (spsr & PSTATE_nRW) != 0 ||
        (spsr & PSTATE_DAIF) != (pstate_read(regs) & PSTATE_DAIF)) {
        return (-TARGET_EINVAL);
    }

    /*
     * Update PSTATE before preparing PCC as it might require rebuilding CPU
     * flags.
     */
    pstate_write(regs, mcp->mc_spsr);

    for (ii = 0; ii < 30; ii++) {
        update_capreg(regs, ii, cheri_load(&cap, &mcp->mc_capregs.cap_x[ii]));
    }

    update_capreg(regs, TARGET_REG_SP, cheri_load(&cap,
        &mcp->mc_capregs.cap_sp));
    update_capreg(regs, TARGET_REG_LR, cheri_load(&cap,
        &mcp->mc_capregs.cap_lr));
    cheri_load(&cap, &mcp->mc_capregs.cap_elr);
    cheri_prepare_pcc(&cap, regs);

    /* XXX FP? */

    return (0);
}
#else
/*
 * Compare to get_mcontext() in arm64/arm64/machdep.c
 * Assumes that the memory is locked if mcp points to user memory.
 */
static inline abi_long get_mcontext(CPUARMState *regs, target_mcontext_t *mcp,
        int flags)
{
    int err = 0, i;
    uint64_t *gr = mcp->mc_gpregs.gp_x;

    mcp->mc_gpregs.gp_spsr = pstate_read(regs);
    if (flags & TARGET_MC_GET_CLEAR_RET) {
        gr[0] = 0UL;
        mcp->mc_gpregs.gp_spsr &= ~CPSR_C;
    } else {
        gr[0] = tswap64(regs->xregs[0]);
    }

    for (i = 1; i < 30; i++)
        gr[i] = tswap64(regs->xregs[i]);

    mcp->mc_gpregs.gp_sp = tswap64(regs->xregs[TARGET_REG_SP]);
    mcp->mc_gpregs.gp_lr = tswap64(regs->xregs[TARGET_REG_LR]);
    mcp->mc_gpregs.gp_elr = tswap64(regs->pc);

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
#endif

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
