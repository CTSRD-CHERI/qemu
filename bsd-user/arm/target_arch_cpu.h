/*
 *  arm cpu init and loop
 *
 *  Olivier Houchard
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

// #define DEBUG_PRINTF(...) fprintf(stderr, __VA_ARGS__)
#define DEBUG_PRINTF(...)

#define TARGET_DEFAULT_CPU_MODEL "any"

#define TARGET_CPU_RESET(cpu)

static inline void target_cpu_init(CPUARMState *env,
        struct target_pt_regs *regs)
{
    int i;

    cpsr_write(env, regs->uregs[16], 0xffffffff, CPSRWriteRaw);
    for (i = 0; i < 16; i++) {
        env->regs[i] = regs->uregs[i];
    }
}

static inline int do_strex(CPUARMState *env)
{
    uint32_t val;
    int size;
    int rc = 1;
    int segv = 0;
    uint32_t addr;
    start_exclusive();
    addr = env->exclusive_addr;
    if (addr != env->exclusive_test) {
        goto fail;
    }
    size = env->exclusive_info & 0xf;
    switch (size) {
    case 0:
        segv = get_user_u8(val, addr);
        break;
    case 1:
        segv = get_user_u16(val, addr);
        break;
    case 2:
    case 3:
        segv = get_user_u32(val, addr);
        break;
    default:
        abort();
    }
    if (segv) {
        env->cp15.far_el[1] = deposit64(env->cp15.far_el[1], 0, 32,
                                      addr);
        goto done;
    }
    if (val != env->exclusive_val) {
        goto fail;
    }
    if (size == 3) {
        segv = get_user_u32(val, addr + 4);
        if (segv) {
            env->cp15.far_el[1] = deposit64(env->cp15.far_el[1], 0, 32,
                                          addr + 4);
            goto done;
        }
        if (val != env->exclusive_high) {
            goto fail;
        }
    }
    val = env->regs[(env->exclusive_info >> 8) & 0xf];
    switch (size) {
    case 0:
        segv = put_user_u8(val, addr);
        break;
    case 1:
        segv = put_user_u16(val, addr);
        break;
    case 2:
    case 3:
        segv = put_user_u32(val, addr);
        break;
    }
    if (segv) {
        env->cp15.far_el[1] = deposit64(env->cp15.far_el[1], 0, 32,
                                      addr);
        goto done;
    }
    if (size == 3) {
        val = env->regs[(env->exclusive_info >> 12) & 0xf];
        segv = put_user_u32(val, addr + 4);
        if (segv) {
            env->cp15.far_el[1] = deposit64(env->cp15.far_el[1], 0, 32,
                                          addr + 4);
            goto done;
        }
    }
    rc = 0;
fail:
    env->regs[15] += 4;
    env->regs[(env->exclusive_info >> 4) & 0xf] = rc;
done:
    end_exclusive();
    return segv;
}

static inline void target_cpu_loop(CPUARMState *env)
{
    int trapnr;
    target_siginfo_t info;
    unsigned int n;
    uint32_t addr;
    CPUState *cs = CPU(arm_env_get_cpu(env));

    for (;;) {
        DEBUG_PRINTF("CPU_LOOPING\n");
        cpu_exec_start(cs);
        DEBUG_PRINTF("EXECUTING...\n");
        trapnr = cpu_arm_exec(env);
        DEBUG_PRINTF("trapnr %d\n", trapnr);
        cpu_exec_end(cs);
        switch (trapnr) {
        case EXCP_UDEF:
            {
                /* See arm/arm/undefined.c undefinedinstruction(); */
                info.si_addr = env->regs[15];

                /*
                 * Make sure the PC is correctly aligned. (It should
                 * be.)
                 */
                if ((info.si_addr & 3) != 0) {
                    info.si_signo = SIGILL;
                    info.si_errno = 0;
                    info.si_code = TARGET_ILL_ILLADR;
                    queue_signal(env, info.si_signo, &info);
                } else {
                    int rc = 0;
#ifdef NOT_YET
                    uint32_t opcode;

                    /*
                     * Get the opcode.
                     *
                     * FIXME - what to do if get_user() fails?
                     */
                    get_user_u32(opcode, env->regs[15]);

                    /* Check the opcode with CP handlers we may have. */
                    rc = EmulateAll(opcode, &ts-fpa, env);
#endif /* NOT_YET */
                    if (rc == 0) {
                        /* illegal instruction */
                        info.si_signo = SIGILL;
                        info.si_errno = 0;
                        info.si_code = TARGET_ILL_ILLOPC;
                        queue_signal(env, info.si_signo, &info);
                    }
                }
            }
            break;
        case EXCP_SWI:
        case EXCP_BKPT:
            {
                unsigned int insn;
#ifdef FREEBSD_ARM_OABI
                env->eabi = 0;
#else
                env->eabi = 1;
#endif
                /*
                 * system call
                 * See arm/arm/trap.c cpu_fetch_syscall_args()
                 */
                if (trapnr == EXCP_BKPT) {
                    if (env->thumb) {
                        if (env->eabi) {
                            n = env->regs[7];
                        } else {
                            /* FIXME - what to do if get_user() fails? */
                            get_user_u16(insn, env->regs[15]);
                            n = insn & 0xff;
                        }
                        env->regs[15] += 2;
                    } else {
                        if (env->eabi) {
                            n = env->regs[7];
                        } else {
                            /* FIXME - what to do if get_user() fails? */
                            get_user_u32(insn, env->regs[15]);
                            n = (insn & 0xf) | ((insn >> 4) & 0xff0);
                        }
                        env->regs[15] += 4;
                    }
                } else { /* trapnr != EXCP_BKPT */
                    if (env->thumb) {
                        if (env->eabi) {
                            n = env->regs[7];
                        } else {
                            /* FIXME - what to do if get_user() fails? */
                            get_user_u16(insn, env->regs[15] - 2);
                            n = insn & 0xff;
                        }
                    } else {
                        if (env->eabi) {
                            n = env->regs[7];
                        } else {
                            /* FIXME - what to do if get_user() fails? */
                            get_user_u32(insn, env->regs[15] - 4);
                            n = insn & 0xffffff;
                        }
                    }
                }
                DEBUG_PRINTF("AVANT CALL %d\n", n);
                if (bsd_type == target_freebsd) {
                    int ret;
                    abi_ulong params = get_sp_from_cpustate(env);
                    int32_t syscall_nr = n;
                    int32_t arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8;

                    /* See arm/arm/trap.c cpu_fetch_syscall_args() */
                    if (syscall_nr == TARGET_FREEBSD_NR_syscall) {
                        syscall_nr = env->regs[0];
                        arg1 = env->regs[1];
                        arg2 = env->regs[2];
                        arg3 = env->regs[3];
                        get_user_s32(arg4, params);
                        params += sizeof(int32_t);
                        get_user_s32(arg5, params);
                        params += sizeof(int32_t);
                        get_user_s32(arg6, params);
                        params += sizeof(int32_t);
                        get_user_s32(arg7, params);
                        arg8 = 0;
                    } else if (syscall_nr == TARGET_FREEBSD_NR___syscall) {
#ifdef TARGET_WORDS_BIGENDIAN
                        syscall_nr = env->regs[1];
#else
                        syscall_nr = env->regs[0];
#endif
                        arg1 = env->regs[2];
                        arg2 = env->regs[3];
                        get_user_s32(arg3, params);
                        params += sizeof(int32_t);
                        get_user_s32(arg4, params);
                        params += sizeof(int32_t);
                        get_user_s32(arg5, params);
                        params += sizeof(int32_t);
                        get_user_s32(arg6, params);
                        arg7 = 0;
                        arg8 = 0;
                    } else {
                        arg1 = env->regs[0];
                        arg2 = env->regs[1];
                        arg3 = env->regs[2];
                        arg4 = env->regs[3];
                        get_user_s32(arg5, params);
                        params += sizeof(int32_t);
                        get_user_s32(arg6, params);
                        params += sizeof(int32_t);
                        get_user_s32(arg7, params);
                        params += sizeof(int32_t);
                        get_user_s32(arg8, params);
                    }
                    ret = do_freebsd_syscall(env, syscall_nr, arg1, arg2, arg3,
                            arg4, arg5, arg6, arg7, arg8);
                    /*
                     * Compare to arm/arm/vm_machdep.c
                     * cpu_set_syscall_retval()
                     */
                    /* XXX armeb may need some extra magic here */
                    if (-TARGET_EJUSTRETURN == ret) {
                        /*
                         * Returning from a successful sigreturn syscall.
                         * Avoid clobbering register state.
                         */
                        break;
                    }
                    /*
                     * XXX Need to handle ERESTART. Backup the PC by
                     * 1 instruction.
                     */
                    if ((unsigned int)ret >= (unsigned int)(-515)) {
                        ret = -ret;
                        cpsr_write(env, CPSR_C, CPSR_C, CPSRWriteByInstr);
                        env->regs[0] = ret;
                    } else {
                        cpsr_write(env, 0, CPSR_C, CPSRWriteByInstr);
                        env->regs[0] = ret; /* XXX need to handle lseek()? */
                        /* env->regs[1] = 0; */
                    }
                } /* else if (bsd_type == target_openbsd)... */
                else {
                    fprintf(stderr, "qemu: bsd_type (= %d) syscall "
                            "not supported\n", bsd_type);
                }
                DEBUG_PRINTF("APRES CALL\n");
            }
            break;
        case EXCP_INTERRUPT:
            /* just indicate that signals should be handled asap */
            break;
        case EXCP_PREFETCH_ABORT:
            /* See arm/arm/trap.c prefetch_abort_handler() */
            addr = (uint32_t)(env->cp15.far_el[1] >> 32);;
            goto do_segv;
        case EXCP_DATA_ABORT:
            /* See arm/arm/trap.c data_abort_handler() */
            addr = (uint32_t)env->cp15.far_el[1];
        do_segv:
            {
                info.si_signo = SIGSEGV;
                info.si_errno = 0;
                /* XXX: check env->error_code */
                info.si_code = 0;
                info.si_addr = addr;
                queue_signal(env, info.si_signo, &info);
            }
            break;
        case EXCP_DEBUG:
            {
                int sig;

                sig = gdb_handlesig(cs, TARGET_SIGTRAP);
                if (sig) {
                    info.si_signo = sig;
                    info.si_errno = 0;
                    info.si_code = TARGET_TRAP_BRKPT;
                    queue_signal(env, info.si_signo, &info);
                }
            }
            break;
        /* XXX case EXCP_KERNEL_TRAP: */
        case EXCP_STREX:
            if (do_strex(env)) {
                addr = (uint32_t)env->cp15.far_el[1];
                goto do_segv;
            }
            break;
        default:
            fprintf(stderr, "qemu: unhandled CPU exception 0x%x - aborting\n",
                    trapnr);
            cpu_dump_state(cs, stderr, fprintf, 0);
            abort();
        } /* switch() */
        process_pending_signals(env);
    } /* for (;;) */
}

static inline void target_cpu_clone_regs(CPUARMState *env, target_ulong newsp)
{
    if (newsp)
        env->regs[13] = newsp;
    env->regs[0] = 0;
}

static inline void target_cpu_reset(CPUArchState *cpu)
{
}

#endif /* !_TARGET_ARCH_CPU_H */
