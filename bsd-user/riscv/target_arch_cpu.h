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

#include "machine/cheri.h"

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
    for (i = 0; i < 32; i++)
        gpr_set_int_value(env, i, regs->regs[i]);

    riscv_update_pc(env, regs->sepc, true);
#endif
}

static abi_syscallarg_t
target_syscallarg(CPURISCVState *env, unsigned regnum)
{

#ifdef TARGET_CHERI
    return (get_readonly_capreg(env, regnum));
#else
    return (gpr_int_value(env, regnum));
#endif
}

static inline void target_cpu_loop(CPURISCVState *env)
{
    CPUState *cs = env_cpu(env);
    int trapnr;
    target_siginfo_t info;
    abi_long error;
    abi_syscallret_t retval;
    unsigned int syscall_num;

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
                syscall_num = gpr_int_value(env, xT0);
                riscv_update_pc(env, riscv_fetch_pc(env) + TARGET_INSN_SIZE,
                    false);
                /*
                 * Compare to cpu_fetch_syscall_args() in riscv/riscv/trap.c.
                 * */
                if (TARGET_FREEBSD_NR___syscall == syscall_num ||
                            TARGET_FREEBSD_NR_syscall == syscall_num) {
                    syscall_num = syscallarg_value(target_syscallarg(env, xA0));
                    error = do_freebsd_syscall(env, &retval, syscall_num,
                        target_syscallarg(env, xA1),
                        target_syscallarg(env, xA2),
                        target_syscallarg(env, xA3),
                        target_syscallarg(env, xA4),
                        target_syscallarg(env, xA5),
                        target_syscallarg(env, xA6),
                        target_syscallarg(env, xA7),
                        NULL);
                } else {
                    error = do_freebsd_syscall(env, &retval, syscall_num,
                        target_syscallarg(env, xA0),
                        target_syscallarg(env, xA1),
                        target_syscallarg(env, xA2),
                        target_syscallarg(env, xA3),
                        target_syscallarg(env, xA4),
                        target_syscallarg(env, xA5),
                        target_syscallarg(env, xA6),
                        target_syscallarg(env, xA7));
                }
                
                /*
                 * Compare to cpu_set_syscall_retval() in
                 * riscv/riscv/vm_machdep.c.
                 */
                if (error >= 0) {
                    if (retval != NULL) {
                        update_capreg(env, xA0, retval);
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
