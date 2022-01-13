/*
 *  RISC-V CPU init and loop
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

#ifndef _TARGET_ARCH_CPU_H_
#define _TARGET_ARCH_CPU_H_

#include "target_arch.h"
#include "target_arch_proc.h"

#ifdef TARGET_CHERI
#include "machine/cheri.h"
#endif

#define TARGET_DEFAULT_CPU_MODEL "any"

/* TODO Don't know whether this needs to point to cpu_reset or not. */
#define TARGET_CPU_RESET(cpu)

static inline void target_cpu_init(CPURISCVState *env,
        struct target_pt_regs *regs)
{
    int i;
    
#ifdef TARGET_CHERI
    for (i = 1; i < 32; i++)
        update_capreg(env, i, &regs->regs[i]);

    env->PCC = regs->sepc;
#else
    for (i = 1; i < 32; i++)
        gpr_set_int_value(env, i, regs->regs[i]);

    riscv_update_pc(env, regs->sepc, true);
#endif
}

static abi_long
target_syscallarg_value(CPURISCVState *env, unsigned regnum)
{

#ifdef TARGET_CHERI
    return (syscallarg_value(get_readonly_capreg(env, regnum)));
#else
    return (gpr_int_value(env, regnum));
#endif
}

static abi_long
target_cpu_fetch_syscall_args(CPURISCVState *env,
    struct target_syscall_args *sa)
{
#ifdef TARGET_CHERI
    target_ulong stack_args;
    abi_long error;
#endif
    uint8_t i;

    sa->code = gpr_int_value(env, xT0);
#ifdef TARGET_CHERI
    stack_args = 0;
#endif

    if (sa->code == TARGET_FREEBSD_NR_syscall ||
        sa->code == TARGET_FREEBSD_NR___syscall) {
        sa->code = target_syscallarg_value(env, xA0);

#ifdef TARGET_CHERI
        stack_args = gpr_int_value(env, xSP);
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

    memset(&sa->args, 0, sizeof(sa->args));
#ifdef TARGET_CHERI
    if (__predict_false(stack_args != 0)) {
        abi_uintcap_t capval;
        int64_t intval;
        int offset, ptrmask;

        if (sa->code >= nitems(target_sysargmask))
            ptrmask = 0;
        else
            ptrmask = target_sysargmask[sa->code];

        offset = 0;
        for (i = 0; i < sa->callp->sy_narg; i++) {
            if (ptrmask & (1 << i)) {
                offset = roundup2(offset, sizeof(abi_uintcap_t));
                error = get_user_uintcap(capval, stack_args + offset);
                cheri_load(&sa->args[i], &capval);
                offset += sizeof(abi_uintcap_t);
            } else {
                error = get_user_s64(intval, stack_args + offset);
                sa->args[i] = *cheri_fromint(intval);
                offset += sizeof(intval);
            }
            if (error)
                return (error);
        }
    } else
#endif
    {
        for (i = 0; i < sa->callp->sy_narg; i++) {
#ifdef TARGET_CHERI
            sa->args[i] = *get_readonly_capreg(env, xA0 + i);
#else
            sa->args[i] = target_syscallarg_value(env, xA0 + i);
#endif
        }
    }

    return (0);
}

static inline void target_cpu_loop(CPURISCVState *env)
{
    CPUState *cs = env_cpu(env);
    int trapnr;
    target_siginfo_t info;
    abi_long error;
    abi_syscallret_t retval;
    struct target_syscall_args sa;

    for (;;) {
        cpu_exec_start(cs);
        trapnr = cpu_exec(cs);
        cpu_exec_end(cs);
        process_queued_cpu_work(cs);

        info.si_signo = 0;
        info.si_errno = 0;
#ifdef TARGET_CHERI
        info.si_addr = cheri_uintptr(cheri_zerocap());
#else
        info.si_addr = (uintptr_t)0;
#endif
        
        switch (trapnr) {
        case EXCP_INTERRUPT:
            /* just indicate that signals should be handled asap */
            break;
        case EXCP_ATOMIC:
            cpu_exec_step_atomic(cs);
            break;
        case RISCV_EXCP_U_ECALL:
            if (bsd_type == target_freebsd) {
                error = target_cpu_fetch_syscall_args(env, &sa);
                if (error == 0) {
                    riscv_update_pc(env, riscv_fetch_pc(env) + TARGET_INSN_SIZE,
                        false);
                    error = do_freebsd_syscall(env, &retval, &sa);
                }
                /*
                 * Compare to cpu_set_syscall_retval() in
                 * riscv/riscv/vm_machdep.c.
                 */
                if (error >= 0) {
                    if (retval != NULL) {
#ifdef TARGET_CHERI
                        update_capreg(env, xA0, retval);
#else
                        gpr_set_int_value(env, xA0, *retval);
#endif
                    } else {
                        gpr_set_int_value(env, xA0, error);
                    }
                    /* XXXKW: xA1 should be set as well. */
                    gpr_set_int_value(env, xA1, 0);
                    gpr_set_int_value(env, xT0, 0);
                } else if (error == -TARGET_ERESTART) {
                    riscv_update_pc(env, riscv_fetch_pc(env) - TARGET_INSN_SIZE,
                        false);
                } else if (error != -TARGET_EJUSTRETURN) {
                    gpr_set_int_value(env, xA0, -error);
                    gpr_set_int_value(env, xT0, 1);
                }
            } else {
                fprintf(stderr, "qemu: bsd_type (= %d) syscall not supported\n",
                        bsd_type);
            }
            break;
        case RISCV_EXCP_ILLEGAL_INST:
            info.si_signo = TARGET_SIGILL;
            info.si_code = TARGET_ILL_ILLOPC;
            break;
        case RISCV_EXCP_BREAKPOINT:
            info.si_signo = TARGET_SIGTRAP;
            info.si_code = TARGET_TRAP_BRKPT;
#ifdef TARGET_CHERI
            info.si_addr = cheri_uintptr(cheri_get_current_pcc(env));
#else
            info.si_addr = riscv_fetch_pc(env);
#endif
            break;
        case RISCV_EXCP_INST_PAGE_FAULT:
        case RISCV_EXCP_LOAD_PAGE_FAULT:
        case RISCV_EXCP_STORE_PAGE_FAULT:
            info.si_signo = TARGET_SIGSEGV;
            info.si_code = TARGET_SEGV_MAPERR;
            break;
        case EXCP_DEBUG:
            info.si_signo = TARGET_SIGTRAP;
            info.si_code = TARGET_TRAP_BRKPT;
            break;
#ifdef TARGET_CHERI
        case RISCV_EXCP_CHERI:
            info.target_si_signo = TARGET_SIGPROT;
            info.target_si_code = cheri_exccode_to_sicode(env->last_cap_cause);
            info.target_si_addr = cheri_uintptr(cheri_get_current_pcc(env));
            info.target_si_capreg = env->last_cap_index;
            info.target_si_trapno = trapnr;
            break;
#endif
        default:
            fprintf(stderr, "qemu: unhandled CPU exception "
                "0x%x - aborting\n", trapnr);
            cpu_dump_state(cs, stderr, 0);
            abort();
        }

        if (info.si_signo) {
            queue_signal(env, info.si_signo, &info);
        }

        process_pending_signals(env);
    }
}

static inline void target_cpu_clone_regs(CPURISCVState *env, target_ulong newsp)
{
    if (newsp) {
        gpr_set_int_value(env, xSP, newsp);
    }

    gpr_set_int_value(env, xA0, 0);
    gpr_set_int_value(env, xT0, 0);
}

static inline void target_cpu_reset(CPUArchState *cpu)
{
}

#endif /* ! _TARGET_ARCH_CPU_H_ */
