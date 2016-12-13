/*
 *  sparc64 cpu init and loop
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

#ifndef _TARGET_ARCH_CPU_H_
#define _TARGET_ARCH_CPU_H_

#include "target_arch.h"

#define TARGET_DEFAULT_CPU_MODEL "TI UltraSparc II"

#define TARGET_CPU_RESET(cpu) cpu_reset(cpu)

#define UREG_L0     8
#define UREG_L1     9
#define UREG_L2     10
#define UREG_L3     11
#define UREG_L4     12
#define UREG_L5     13
#define UREG_L6     14
#define UREG_L7     15
//#define UREG_FP     16
#define UREG_SP     22

extern abi_ulong target_sparc_utrap_precise[TARGET_UT_MAX];

static inline void target_cpu_init(CPUSPARCState *env,
        struct target_pt_regs *regs)
{
    int i;

    env->pc = regs->pc;
    env->npc = regs->npc;
    env->y = regs->y;
    for (i = 0; i < 8; i++) {
        env->gregs[i] = regs->u_regs[i];
    }
    for (i = 0; i < 8; i++) {
        env->regwptr[i] = regs->u_regs[i + 8];
    }
}

/* see sparc64/sparc64/exception.S tl0_utrap() */
static inline int target_cpu_utrap(CPUSPARCState *env, abi_ulong utrap_addr)
{
    trap_state *tsptr = cpu_tsptr(env);

    if (utrap_addr == 0)
        return 1;

   /*
    * Pass %fsr in %l4, %tstate in %l5, %tpc in %l6 and %tnpc
    * in %l7.
    */
    env->regwptr[UREG_L4] = env->fsr;
    env->regwptr[UREG_L5] = tsptr->tstate;
    env->regwptr[UREG_L6] = tsptr->tpc;
    env->regwptr[UREG_L7] = tsptr->tnpc;

    /*
     * Setup %tnpc to return to.
     */
    tsptr->tnpc = utrap_addr;

    /*
     * Setup %wstate for return, clear WSTATE_TRANSITION.
     * WSTATE_NORMAL_MASK = 1
     */
    env->wstate &= 1 /* WSTATE_NORMAL_MASK */;

    /*
     * Setup %tstate for return, change the saved cwp to point
     * to the current window instead of the window at the time
     * of the trap.
     */
    tsptr->tstate = cpu_get_cwp64(env);

    /*
     * Setup %sp.  Userland processes will crash if this is not
     * setup.  %fp = in_6  CCFSZ = 192
     */
    env->regwptr[UREG_SP] = env->regwptr[UREG_FP] - 192 /* CCFSZ */;

    return 0;
}


static inline void target_cpu_loop(CPUSPARCState *env)
{
    CPUState *cs = CPU(sparc_env_get_cpu(env));
    int trapnr, ret, syscall_nr;
    /* target_siginfo_t info; */

    while (1) {
	cpu_exec_start(cs);
        trapnr = cpu_exec(cs);
	cpu_exec_end(cs);
	process_queued_cpu_work(cs);

        switch (trapnr) {
        case 0x10:
            /* T_ILLEGAL_INSTRUCTION see sparc64/sparc64/exception.S */
            fprintf(stderr, "T_ILLEGAL_INSTRUCTION\n");
            if (target_cpu_utrap(env, target_sparc_utrap_precise[5]))
                goto badtrap;
            break;

        case 0x20:
            /* T_FP_DISABLED see sparc64/sparc64/exception.S */
            fprintf(stderr, "T_FP_DISABLED\n");
            if (target_cpu_utrap(env, target_sparc_utrap_precise[7]))
                goto badtrap;
            break;
        case 0x21:
            fprintf(stderr, "T_FP_EXCEPTION_IEEE_754\n");
            /* T_FP_EXCEPTION_IEEE_754 see sparc64/sparc64/exception.S */
            if (target_cpu_utrap(env, target_sparc_utrap_precise[8]))
                goto badtrap;
            break;
        case 0x22:
            fprintf(stderr, "T_FP_EXCEPTION_OTHER\n");
            /* T_FP_EXCEPTION_OTHER see sparc64/sparc64/exception.S */
            if (target_cpu_utrap(env, target_sparc_utrap_precise[9]))
                goto badtrap;
            break;

#if 0
        case ???:
            /* T_MEM_ADDRESS_NOT_ALIGNED see sparc64/sparc64/exception.S */
            if (target_cpu_utrap(env, target_sparc_utrap_precise[15]))
                goto badtrap;
            break;
#endif

        /* FreeBSD uses 0x141 for syscalls too */
        case 0x141:
            if (bsd_type != target_freebsd) {
                goto badtrap;
            }
        case 0x100:
            syscall_nr = env->gregs[1];
            if (bsd_type == target_freebsd) {
                ret = do_freebsd_syscall(env, syscall_nr,
                                        env->regwptr[0], env->regwptr[1],
                                        env->regwptr[2], env->regwptr[3],
                                        env->regwptr[4], env->regwptr[5], 0, 0);
            } else if (bsd_type == target_netbsd) {
                ret = do_netbsd_syscall(env, syscall_nr,
                                        env->regwptr[0], env->regwptr[1],
                                        env->regwptr[2], env->regwptr[3],
                                        env->regwptr[4], env->regwptr[5]);
            } else { /* if (bsd_type == target_openbsd) */
                syscall_nr &= ~(TARGET_OPENBSD_SYSCALL_G7RFLAG |
                                TARGET_OPENBSD_SYSCALL_G2RFLAG);
                ret = do_openbsd_syscall(env, syscall_nr,
                                         env->regwptr[0], env->regwptr[1],
                                         env->regwptr[2], env->regwptr[3],
                                         env->regwptr[4], env->regwptr[5]);
            }
            cpu_put_ccr(env, cpu_get_ccr(env));
            if ((unsigned int)ret >= (unsigned int)(-515)) {
                ret = -ret;
#if !defined(TARGET_ABI32)
                env->xcc |= PSR_CARRY;
#else
                env->psr |= PSR_CARRY;
#endif
            } else {
#if !defined(TARGET_ABI32)
                env->xcc &= ~PSR_CARRY;
#else
                env->psr &= ~PSR_CARRY;
#endif
            }
            env->regwptr[0] = ret;
            /* next instruction */
            if (bsd_type == target_openbsd &&
                env->gregs[1] & TARGET_OPENBSD_SYSCALL_G2RFLAG) {
                env->pc = env->gregs[2];
                env->npc = env->pc + 4;
            } else if (bsd_type == target_openbsd &&
                       env->gregs[1] & TARGET_OPENBSD_SYSCALL_G7RFLAG) {
                env->pc = env->gregs[7];
                env->npc = env->pc + 4;
            } else {
                env->pc = env->npc;
                env->npc = env->npc + 4;
            }
            break;

        case 0x83: /* flush windows */
#ifdef TARGET_ABI32
        case 0x103:
#endif
            bsd_sparc64_flush_windows(env);
            /* next instruction */
            env->pc = env->npc;
            env->npc = env->npc + 4;
            break;

        case TT_SPILL: /* window overflow */
            bsd_sparc64_save_window(env);
            break;

        case TT_FILL: /* window underflow */
            bsd_sparc64_restore_window(env);
            break;

        case TT_TFAULT:
        case TT_DFAULT:
#if 0
            {
                info.si_signo = SIGSEGV;
                info.si_errno = 0;
                /* XXX: check env->error_code */
                info.si_code = TARGET_SEGV_MAPERR;
                if (trapnr == TT_DFAULT) {
                    info._sifields._sigfault._addr = env->dmmuregs[4];
                } else {
                    info._sifields._sigfault._addr = cpu_tsptr(env)->tpc;
                    /* queue_signal(env, info.si_signo, &info); */
                }
            }
#endif
            break;

        case EXCP_INTERRUPT:
            /* just indicate that signals should be handled asap */
            break;

        case EXCP_DEBUG:
            {
                int sig;

                sig = gdb_handlesig(cs, TARGET_SIGTRAP);
#if 0
                if (sig) {
                    info.si_signo = sig;
                    info.si_errno = 0;
                    info.si_code = TARGET_TRAP_BRKPT;
                    /* queue_signal(env, info.si_signo, &info); */
                }
#endif
            }
            break;

        default:
badtrap:
            printf("Unhandled trap: 0x%x\n", trapnr);
            cpu_dump_state(cs, stderr, fprintf, 0);
            exit(1);
        }
        process_pending_signals(env);
    }
}

static inline void target_cpu_clone_regs(CPUSPARCState *env, target_ulong newsp)
{
    if (newsp)
        env->regwptr[UREG_SP] = newsp;
    env->regwptr[0] = 0;
    env->xcc &= ~PSR_CARRY;
}

static inline void target_cpu_reset(CPUArchState *cpu)
{
    cpu_reset(ENV_GET_CPU(cpu));
}

#endif /* ! _TARGET_ARCH_CPU_H_ */
