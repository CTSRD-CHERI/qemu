/*
 *  mips cpu init and loop
 *
 *  Copyright (c) 2013 Stacey D. Son
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

#if defined(TARGET_ABI_MIPSN32)
#  define TARGET_DEFAULT_CPU_MODEL "24Kc"
#else
#  define TARGET_DEFAULT_CPU_MODEL "24Kf"
#endif

#define TARGET_CPU_RESET(env)

static inline void target_cpu_init(CPUMIPSState *env,
        struct target_pt_regs *regs)
{
    int i;

    for (i = 0; i < 32; i++) {
        env->active_tc.gpr[i] = regs->regs[i];
    }
    env->active_tc.PC = regs->cp0_epc & ~(target_ulong)1;
    if (regs->cp0_epc & 1) {
        env->hflags |= MIPS_HFLAG_M16;
    }
}

static int do_store_exclusive(CPUMIPSState *env)
{
    target_ulong addr;
    target_ulong page_addr;
    target_ulong val;
    int flags;
    int segv = 0;
    int reg;
    int d;

    addr = env->lladdr;
    page_addr = addr & TARGET_PAGE_MASK;
    start_exclusive();
    mmap_lock();
    flags = page_get_flags(page_addr);
    if ((flags & PAGE_READ) == 0) {
        segv = 1;
    } else {
        reg = env->llreg & 0x1f;
        d = (env->llreg & 0x20) != 0;
        if (d) {
            segv = get_user_s64(val, addr);
        } else {
            segv = get_user_s32(val, addr);
        }
        if (!segv) {
            if (val != env->llval) {
                env->active_tc.gpr[reg] = 0;
            } else {
                if (d) {
                    segv = put_user_u64(env->llnewval, addr);
                } else {
                    segv = put_user_u32(env->llnewval, addr);
                }
                if (!segv) {
                    env->active_tc.gpr[reg] = 1;
                }
            }
        }
    }
    env->lladdr = -1;
    if (!segv) {
        env->active_tc.PC += 4;
    }
    mmap_unlock();
    end_exclusive();
    return segv;
}

static inline void target_cpu_loop(CPUMIPSState *env)
{
    CPUState *cs = CPU(mips_env_get_cpu(env));
    target_siginfo_t info;
    int trapnr;
    abi_long ret;
    unsigned int syscall_num;

    for (;;) {
        cpu_exec_start(cs);
        trapnr = cpu_mips_exec(env);
        cpu_exec_end(cs);
        switch (trapnr) {
        case EXCP_SYSCALL: /* syscall exception */
            if (bsd_type == target_freebsd) {
                syscall_num = env->active_tc.gpr[2]; /* v0 */
                env->active_tc.PC += TARGET_INSN_SIZE;
                if (syscall_num >= TARGET_FREEBSD_NR_MAXSYSCALL) {
                    ret = -TARGET_ENOSYS;
                } else {
		    abi_ulong arg4 = 0, arg5 = 0, arg6 = 0, arg7 =0;
		    abi_ulong sp_reg = env->active_tc.gpr[29];

# ifdef TARGET_ABI_MIPSO32
		    get_user_ual(arg4, sp_reg + 16);
		    get_user_ual(arg5, sp_reg + 20);
		    get_user_ual(arg6, sp_reg + 24);
		    get_user_ual(arg7, sp_reg + 28);
#else
		    arg4 = env->active_tc.gpr[12]; /* t4/arg4 */
		    arg5 = env->active_tc.gpr[13]; /* t5/arg5 */
		    arg6 = env->active_tc.gpr[14]; /* t6/arg6 */
		    arg7 = env->active_tc.gpr[15]; /* t7/arg7 */
#endif
                    /* mips(32) uses regs 4-7,12-15 for args */
                    if (TARGET_FREEBSD_NR___syscall == syscall_num ||
                            TARGET_FREEBSD_NR_syscall == syscall_num) {
                        /* indirect syscall */
                        ret = do_freebsd_syscall(env,
                                env->active_tc.gpr[4],/* syscall #*/
                                env->active_tc.gpr[5], /* a1/arg0 */
                                env->active_tc.gpr[6], /* a2/arg1 */
                                env->active_tc.gpr[7], /* a3/arg2 */
				arg4,
				arg5,
				arg6,
				arg7,
                                0  /* no arg7 */
                                );
                    } else {
                        /* direct syscall */
                        ret = do_freebsd_syscall(env,
                                syscall_num,
                                env->active_tc.gpr[4], /* a0/arg0 */
                                env->active_tc.gpr[5], /* a1/arg1 */
                                env->active_tc.gpr[6], /* a2/arg2 */
                                env->active_tc.gpr[7], /* a3/arg3 */
				arg4,
				arg5,
				arg6,
				arg7
                                );
                    }
                }
                /* Compare to mips/mips/vm_machdep.c cpu_set_syscall_retval() */
                if (-TARGET_EJUSTRETURN == ret) {
                    /*
                     * Returning from a successful sigreturn
                     * syscall.  Avoid clobbering register state.
                     */
                    break;
                }
                if (-TARGET_ERESTART == ret) {
                    /* Backup the pc to point at the swi. */
                    env->active_tc.PC -= TARGET_INSN_SIZE;
                    break;
                }
                if ((unsigned int)ret >= (unsigned int)(-1133)) {
                    env->active_tc.gpr[7] = 1;
                    ret = -ret;
                } else {
                    env->active_tc.gpr[7] = 0;
                }
                env->active_tc.gpr[2] = ret; /* v0 <- ret */
            } /* else if (bsd_type == target_openbsd)... */
            else {
                fprintf(stderr, "qemu: bsd_type (= %d) syscall not supported\n",
                        bsd_type);
            }
            break;

        case EXCP_TLBL: /* TLB miss on load */
        case EXCP_TLBS: /* TLB miss on store */
        case EXCP_AdEL: /* bad address on load */
        case EXCP_AdES: /* bad address on store */
            info.target_si_signo = TARGET_SIGSEGV;
            info.target_si_errno = 0;
            /* XXX: check env->error_code */
            info.target_si_code = TARGET_SEGV_MAPERR;
            info.target_si_addr = env->CP0_BadVAddr;
            queue_signal(env, info.si_signo, &info);
            break;

        case EXCP_CpU: /* coprocessor unusable */
        case EXCP_RI:  /* reserved instruction */
            info.target_si_signo = TARGET_SIGILL;
            info.target_si_errno = 0;
            info.target_si_code = 0;
            queue_signal(env, info.target_si_signo, &info);
            break;

        case EXCP_INTERRUPT: /* async interrupt */
            /* just indicate that signals should be handled asap */
            break;

        case EXCP_DEBUG: /* cpu stopped after a breakpoint */
            {
                int sig;

                sig = gdb_handlesig(cs, TARGET_SIGTRAP);
                if (sig) {
                    info.target_si_signo = sig;
                    info.target_si_errno = 0;
                    info.target_si_code = TARGET_TRAP_BRKPT;
                    queue_signal(env, info.target_si_signo, &info);
                }
            }
            break;

        case EXCP_SC:
            if (do_store_exclusive(env)) {
                info.target_si_signo = TARGET_SIGSEGV;
                info.target_si_errno = 0;
                info.target_si_code = TARGET_SEGV_MAPERR;
                info.target_si_addr = env->active_tc.PC;
                queue_signal(env, info.target_si_signo, &info);
            }
            break;

        default:
            fprintf(stderr, "qemu: unhandled CPU exception "
                "0x%x - aborting\n", trapnr);
            cpu_dump_state(cs, stderr, fprintf, 0);
            abort();
        }
        process_pending_signals(env);
    }
}

static inline void target_cpu_clone_regs(CPUMIPSState *env, target_ulong newsp)
{
    if (newsp)
        env->active_tc.gpr[29] = newsp;
    env->active_tc.gpr[7] = 0;
    env->active_tc.gpr[2] = 0;
}

static inline void target_cpu_reset(CPUArchState *cpu)
{
}

#endif /* ! _TARGET_ARCH_CPU_H_ */
