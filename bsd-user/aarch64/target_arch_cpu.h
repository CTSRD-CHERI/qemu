/*
 *  ARM AArch64 cpu init and loop
 *
 * Copyright (c) 2015 Stacey D. Son <sson at Freebsd>
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
#ifndef _TARGET_ARCH_CPU_H_
#define _TARGET_ARCH_CPU_H_

#include "target_arch.h"

#ifdef TARGER_CHERI
#include "machine/cheri.h"
#endif

// #define DEBUG_PRINTF(...) fprintf(stderr, __VA_ARGS__)
#define DEBUG_PRINTF(...)

#define TARGET_DEFAULT_CPU_MODEL "any"

#define TARGET_CPU_RESET(cpu)

static inline void target_cpu_init(CPUARMState *env,
    struct target_pt_regs *regs)
{
    int i;

    if (!(arm_feature(env, ARM_FEATURE_AARCH64))) {
        fprintf(stderr, "The selected ARM CPU does not support 64 bit mode\n");
        exit(1);
    }
#ifdef TARGET_CHERI
    pstate_write(env, pstate_read(env) | PSTATE_C64);
    arm_rebuild_chflags(env);

    for (i = 0; i < 31; i++) {
        update_capreg(env, i, &regs->regs[i]);
    }

    cheri_prepare_pcc(&regs->pc, env);
    update_capreg(env, 31, &regs->sp);
#else
    for (i = 0; i < 31; i++) {
        env->xregs[i] = regs->regs[i];
    }
    env->pc = regs->pc;
    env->xregs[31] = regs->sp;
#endif
}

static abi_long
target_syscallarg_value(CPUARMState *env, unsigned regnum)
{

    return (arm_get_xreg(env, regnum));
}

static abi_long
target_cpu_fetch_syscall_args(CPUARMState *env,
    struct target_syscall_args *sa)
{
    uint8_t aii;

    aii = 0;

    sa->code = arm_get_xreg(env, 8);
    memset(&sa->args, 0, sizeof(sa->args));

    if (sa->code == TARGET_FREEBSD_NR_syscall ||
        sa->code == TARGET_FREEBSD_NR___syscall) {
        sa->code = target_syscallarg_value(env, aii++);
    } else {
#ifdef TARGET_CHERI
        sa->args[0] = *get_readonly_capreg(env, aii++);
#else
        sa->args[0] = target_syscallarg_value(env, aii++);
#endif
    }

    /*
     * XXXKW: target_syscall_args.callp isn't currently used to call a system
     * call. We should refactor the user mode to handle system calls easier
     * using the target_sysent system call table.
     */
    if (__predict_false(sa->code >= TARGET_FREEBSD_NR_MAXSYSCALL))
        sa->callp = &target_sysent[0];
    else
        sa->callp = &target_sysent[sa->code];

    assert(sa->callp->sy_narg <= nitems(sa->args));

    for (; aii < TARGET_MAXARGS - 1; aii++) {
#ifdef TARGET_CHERI
        sa->args[aii] = *get_readonly_capreg(env, aii);
#else
        sa->args[aii] = target_syscallarg_value(env, aii);
#endif
    }

    return (0);
}

static inline void target_cpu_loop(CPUARMState *env)
{
    CPUState *cs = env_cpu(env);
    int trapnr;
    target_siginfo_t info;
    uint32_t pstate;
    abi_long ret;
    abi_syscallret_t retval;
    struct target_syscall_args sa;

    for (;;) {
        cpu_exec_start(cs);
        trapnr = cpu_exec(cs);
        cpu_exec_end(cs);
	process_queued_cpu_work(cs);

        switch (trapnr) {
        case EXCP_SWI:
            /* See arm/arm/trap.c cpu_fetch_syscall_args() */
            ret = target_cpu_fetch_syscall_args(env, &sa);
            if (ret == 0) {
                ret = do_freebsd_syscall(env, &retval, &sa);
            }
            /*
             * The carry bit is cleared for no error; set for error.
             * See arm64/arm64/vm_machdep.c cpu_set_syscall_retval()
             */
            pstate = pstate_read(env);
            if (ret >= 0) {
                pstate &= ~PSTATE_C;
                if (retval != NULL) {
#ifdef TARGET_CHERI
                    update_capreg(env, 0, retval);
#else
                    arm_set_xreg(env, 0, *retval);
#endif
                } else {
                    arm_set_xreg(env, 0, ret);
                }
            } else if (ret == -TARGET_ERESTART) {
                arm_update_pc(env, arm_fetch_pc(env) - 4, false);
                break;
            } else if (ret != -TARGET_EJUSTRETURN) {
                pstate |= PSTATE_C;
                arm_set_xreg(env, 0, -ret);
            }
            pstate_write(env, pstate);
#ifdef TARGET_CHERI
            cheri_prepare_pcc(&env->pc.cap, env);
#endif
            break;

		case EXCP_INTERRUPT:
            /* Just indicate that signals should be handle ASAP. */
            break;

        case EXCP_UDEF:
            info.si_signo = TARGET_SIGILL;
            info.si_errno = 0;
            info.si_code = TARGET_ILL_ILLOPN;
#ifdef TARGET_CHERI
            info.si_addr = cheri_uintptr(cheri_get_current_pcc(env));
#else
            info.si_addr = arm_fetch_pc(env);
#endif
            queue_signal(env, info.si_signo, &info);
            break;


	case EXCP_PREFETCH_ABORT:
	case EXCP_DATA_ABORT:
            info.si_signo = TARGET_SIGSEGV;
            info.si_errno = 0;
            /* XXX: check env->error_code */
            info.si_code = TARGET_SEGV_MAPERR;
#ifdef TARGET_CHERI
            info.si_addr = cheri_uintptr(cheri_ptr_to_unbounded_cap(
                (const void *)(uintptr_t)env->exception.vaddress));
#else
            info.si_addr = env->exception.vaddress;
#endif
            queue_signal(env, info.si_signo, &info);
            break;

	case EXCP_DEBUG:
        case EXCP_BKPT:
            info.si_signo = TARGET_SIGTRAP;
            info.si_errno = 0;
            info.si_code = TARGET_TRAP_BRKPT;
            queue_signal(env, info.si_signo, &info);
            break;

        case EXCP_ATOMIC:
            cpu_exec_step_atomic(cs);
            break;

        case EXCP_YIELD:
            /* nothing to do here for user-mode, just resume guest code */
            break;
        default:
            fprintf(stderr, "qemu: unhandled CPU exception 0x%x - aborting\n",
                    trapnr);
            cpu_dump_state(cs, stderr, 0);
            abort();
        } /* switch() */
        process_pending_signals(env);
        /*
         * Exception return on AArch64 always clears the exclusive
         * monitor, so any return to running guest code implies this.
         * A strex (successful or otherwise) also clears the monitor, so
         * we don't need to specialcase EXCP_STREX.
         */
        env->exclusive_addr = -1;
    } /* for (;;) */
}

/* See arm64/arm64/vm_machdep.c cpu_fork() */
static inline void target_cpu_clone_regs(CPUARMState *env, target_ulong newsp)
{
    uint32_t pstate;

    pstate = 0;
#ifdef TARGET_CHERI
    pstate |= PSTATE_C64;
#endif

    if (newsp)
        arm_set_xreg(env, 31, newsp);
    arm_set_xreg(env, 0, 0);
    arm_set_xreg(env, 1, 0);
    pstate_write(env, pstate);
}

static inline void target_cpu_reset(CPUArchState *cpu)
{
}

#endif /* !_TARGET_ARCH_CPU_H */
