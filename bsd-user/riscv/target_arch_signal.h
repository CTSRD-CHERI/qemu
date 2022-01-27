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

#include "qemu.h"
#include "qemu-common.h"
#include "cpu.h"

#ifdef TARGET_CHERI
#include "cheri/cheric.h"
#include "machine/cheri.h"
#endif

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

#ifdef TARGET_CHERI
struct target_capregs {
    abi_uintcap_t    cp_cra;
    abi_uintcap_t    cp_csp;
    abi_uintcap_t    cp_cgp;
    abi_uintcap_t    cp_ctp;
    abi_uintcap_t    cp_ct[7];
    abi_uintcap_t    cp_cs[12];
    abi_uintcap_t    cp_ca[8];
    abi_uintcap_t    cp_sepcc;
    abi_uintcap_t    cp_ddc;
    uint64_t         cp_sstatus;
    uint64_t         cp_pad;
};
#else
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
#endif

struct target_fpregs {
    uint64_t        fp_x[32][2];
    uint64_t        fp_fcsr;
    uint32_t        fp_flags;
    uint32_t        pad;
};


typedef struct target_mcontext {
#ifdef TARGET_CHERI
    struct target_capregs  mc_capregs;
#else
    struct target_gpregs   mc_gpregs;
#endif
    struct target_fpregs   mc_fpregs;
    uint32_t               mc_flags;
#define TARGET_MC_FP_VALID 0x01
    uint32_t               mc_pad;
#ifdef TARGET_CHERI
    uint64_t               mc_spare[8];
#else
    uint64_t               mc_capregs;
    uint64_t               mc_spare[7];
#endif
} target_mcontext_t;

typedef struct target_ucontext {
    target_sigset_t   uc_sigmask;
    target_mcontext_t uc_mcontext;
    abi_uintptr_t     uc_link;
    target_stack_t    uc_stack;
    int32_t           uc_flags;
    int32_t         __spare__[4];
} target_ucontext_t;

struct target_sigframe {
    target_ucontext_t   sf_uc; /* = *sf_uncontext */
    target_siginfo_t    sf_si; /* = *sf_siginfo (SA_SIGINFO case)*/
};

struct target_trapframe {
#ifdef TARGET_CHERI
    abi_uintcap_t tf_ra;
    abi_uintcap_t tf_sp;
    abi_uintcap_t tf_gp;
    abi_uintcap_t tf_tp;
    abi_uintcap_t tf_t[7];
    abi_uintcap_t tf_s[12];
    abi_uintcap_t tf_a[8];
    abi_uintcap_t tf_sepc;
    abi_uintcap_t tf_ddc;
#else
    uint64_t tf_ra;
    uint64_t tf_sp;
    uint64_t tf_gp;
    uint64_t tf_tp;
    uint64_t tf_t[7];
    uint64_t tf_s[12];
    uint64_t tf_a[8];
    uint64_t tf_sepc;
#endif
    uint64_t tf_sstatus;
    uint64_t tf_stval;
    uint64_t tf_scause;
};

/*
 * Compare with sendsig() in riscv/riscv/machdep.c
 * Assumes that target stack frame memory is locked.
 */
static inline abi_long
set_sigtramp_args(CPURISCVState *regs, TaskState *ts, int sig,
    struct target_sigframe *frame, abi_ulong frame_addr,
    struct target_sigaction *ka)
{
#ifdef TARGET_CHERI
    cap_register_t cap;
#endif

    /*
     * Arguments to signal handler:
     *  a0 (10) = signal number
     *  a1 (11) = siginfo pointer
     *  a2 (12) = ucontext pointer
     *  pc      = signal pointer handler
     *  sp (2)  = sigframe pointer
     *  ra (1)  = sigtramp at base of user stack
     */
#ifdef TARGET_CHERI
    update_capreg_to_intval(regs, xA0, sig);
    update_capreg(regs, xA1, cheri_ptr((void *)(uintptr_t)(frame_addr +
        offsetof(typeof(*frame), sf_si)), sizeof(frame->sf_si)));
    update_capreg(regs, xA2, cheri_ptr((void *)(uintptr_t)(frame_addr +
        offsetof(typeof(*frame), sf_uc)), sizeof(frame->sf_uc)));
    cheri_load(&cap, &ka->_sa_handler);
    cheri_prepare_pcc(&cap, regs);
    update_capreg(regs, xSP, cheri_setaddress(cheri_zerocap(),
        (uintptr_t)frame_addr));
    update_capreg(regs, xRA, &ts->cheri_sigcode_cap);
#else
    regs->gpr[xA0] = sig;
    regs->gpr[xA1] = frame_addr +
        offsetof(struct target_sigframe, sf_si);
    regs->gpr[xA2] = frame_addr +
        offsetof(struct target_sigframe, sf_uc);
    regs->pc = ka->_sa_handler;
    regs->gpr[xSP] = frame_addr;
    regs->gpr[xRA] = TARGET_PS_STRINGS - TARGET_SZSIGCODE;
#endif
    return 0;
}

static inline void get_mcontext_regs(CPURISCVState *regs,
#ifdef TARGET_CHERI
    abi_uintcap_t dstregs[],
#else
    uint64_t dstregs[],
#endif
    size_t nregs, ...)
{
    va_list ap;
    size_t ii;
    int regnum;

    va_start(ap, nregs);
    for (ii = 0; ii < nregs; ii++) {
        regnum = va_arg(ap, int);
#ifdef TARGET_CHERI
        cheri_store(&dstregs[ii], get_readonly_capreg(regs, regnum));
#else
        dstregs[ii] = tswap64(regs->gpr[regnum]);
#endif
    }
    va_end(ap);
}

/*
 * Compare with get_mcontext() in riscv/riscv/machdep.c
 * Assumes that the memory is locked if mcp points to user memory.
 */
static inline abi_long get_mcontext(CPURISCVState *regs, target_mcontext_t *mcp,
        int flags)
{

#ifdef TARGET_CHERI
    get_mcontext_regs(regs, mcp->mc_capregs.cp_ct,
        nitems(mcp->mc_capregs.cp_ct), xT0, xT1, xT2, xT3, xT4, xT5, xT6);
    get_mcontext_regs(regs, mcp->mc_capregs.cp_cs,
        nitems(mcp->mc_capregs.cp_cs), xS0, xS1, xS2, xS3, xS4, xS5, xS6, xS7,
        xS8, xS9, xS10, xS11);
    get_mcontext_regs(regs, mcp->mc_capregs.cp_ca,
        nitems(mcp->mc_capregs.cp_ca), xA0, xA1, xA2, xA3, xA4, xA5, xA6, xA7);

    if (flags & TARGET_MC_GET_CLEAR_RET) {
        cheri_store(&mcp->mc_capregs.cp_ca[0], cheri_nullcap());
        cheri_store(&mcp->mc_capregs.cp_ct[0], cheri_nullcap());
    }

    cheri_store(&mcp->mc_capregs.cp_cra, get_readonly_capreg(regs, xRA));
    cheri_store(&mcp->mc_capregs.cp_csp, get_readonly_capreg(regs, xSP));
    cheri_store(&mcp->mc_capregs.cp_cgp, get_readonly_capreg(regs, xGP));
    cheri_store(&mcp->mc_capregs.cp_ctp, get_readonly_capreg(regs, xTP));
    cheri_store(&mcp->mc_capregs.cp_sepcc, cheri_get_current_pcc(regs));
    cheri_store(&mcp->mc_capregs.cp_ddc, cheri_get_ddc(regs));
    /*
     * The sstatus register is unavailable in the user mode.
     */
    mcp->mc_capregs.cp_sstatus = 0;
#else
    get_mcontext_regs(regs, mcp->mc_gpregs.gp_t,
        nitems(mcp->mc_gpregs.gp_t), xT0, xT1, xT2, xT3, xT4, xT5, xT6);
    get_mcontext_regs(regs, mcp->mc_gpregs.gp_s,
        nitems(mcp->mc_gpregs.gp_s), xS0, xS1, xS2, xS3, xS4, xS5, xS6, xS7,
        xS8, xS9, xS10, xS11);
    get_mcontext_regs(regs, mcp->mc_gpregs.gp_a,
        nitems(mcp->mc_gpregs.gp_a), xA0, xA1, xA2, xA3, xA4, xA5, xA6, xA7);

    if (flags & TARGET_MC_GET_CLEAR_RET) {
        mcp->mc_gpregs.gp_a[0] = 0;
        mcp->mc_gpregs.gp_t[0] = 0;
    }

    mcp->mc_gpregs.gp_ra = tswap64(regs->gpr[xRA]);
    mcp->mc_gpregs.gp_sp = tswap64(regs->gpr[xSP]);
    mcp->mc_gpregs.gp_gp = tswap64(regs->gpr[xGP]);
    mcp->mc_gpregs.gp_tp = tswap64(regs->gpr[xTP]);
    mcp->mc_gpregs.gp_sepc = tswap64(regs->pc);
    /*
     * The sstatus register is unavailable in the user mode.
     */
    mcp->mc_gpregs.gp_sstatus = 0;
#endif

    return 0;
}

static inline void set_mcontext_regs(CPURISCVState *regs,
#ifdef TARGET_CHERI
    abi_uintcap_t srcregs[],
#else
    uint64_t srcregs[],
#endif
    int nregs, ...)
{
    va_list ap;
    size_t ii;
    int regnum;
#ifdef TARGET_CHERI
    static cap_register_t cap;
#endif

    va_start(ap, nregs);
    for (ii = 0; ii < nregs; ii++) {
        regnum = va_arg(ap, int);
#ifdef TARGET_CHERI
        update_capreg(regs, regnum, cheri_load(&cap, &srcregs[ii]));
#else
        regs->gpr[regnum] = tswap64(srcregs[ii]);
#endif
    }
    va_end(ap);
}

/* Compare with set_mcontext() in riscv/riscv/machdep.c */
static inline abi_long set_mcontext(CPURISCVState *regs, target_mcontext_t *mcp,
        int srflag)
{
#ifdef TARGET_CHERI
    cap_register_t cap;

    set_mcontext_regs(regs, mcp->mc_capregs.cp_ct,
        nitems(mcp->mc_capregs.cp_ct), xT0, xT1, xT2, xT3, xT4, xT5, xT6);
    set_mcontext_regs(regs, mcp->mc_capregs.cp_cs,
        nitems(mcp->mc_capregs.cp_cs), xS0, xS1, xS2, xS3, xS4, xS5, xS6, xS7,
        xS8, xS9, xS10, xS11);
    set_mcontext_regs(regs, mcp->mc_capregs.cp_ca,
        nitems(mcp->mc_capregs.cp_ca), xA0, xA1, xA2, xA3, xA4, xA5, xA6, xA7);

    update_capreg(regs, xRA, cheri_load(&cap, &mcp->mc_capregs.cp_cra));
    update_capreg(regs, xSP, cheri_load(&cap, &mcp->mc_capregs.cp_csp));
    update_capreg(regs, xGP, cheri_load(&cap, &mcp->mc_capregs.cp_cgp));
    update_capreg(regs, xTP, cheri_load(&cap, &mcp->mc_capregs.cp_ctp));
    cheri_load(&cap, &mcp->mc_capregs.cp_sepcc);
    cheri_prepare_pcc(&cap, regs);
#else
    set_mcontext_regs(regs, mcp->mc_gpregs.gp_t,
        nitems(mcp->mc_gpregs.gp_t), xT0, xT1, xT2, xT3, xT4, xT5, xT6);
    set_mcontext_regs(regs, mcp->mc_gpregs.gp_s,
        nitems(mcp->mc_gpregs.gp_s), xS0, xS1, xS2, xS3, xS4, xS5, xS6, xS7,
        xS8, xS9, xS10, xS11);
    set_mcontext_regs(regs, mcp->mc_gpregs.gp_a,
        nitems(mcp->mc_gpregs.gp_a), xA0, xA1, xA2, xA3, xA4, xA5, xA6, xA7);

    regs->gpr[xRA] = tswap64(mcp->mc_gpregs.gp_ra);
    regs->gpr[xSP] = tswap64(mcp->mc_gpregs.gp_sp);
    regs->gpr[xGP] = tswap64(mcp->mc_gpregs.gp_gp);
    regs->gpr[xTP] = tswap64(mcp->mc_gpregs.gp_tp);
    regs->pc = tswap64(mcp->mc_gpregs.gp_sepc);
#endif

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
