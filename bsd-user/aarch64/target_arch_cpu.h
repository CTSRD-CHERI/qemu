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
    for (i = 0; i < 31; i++) {
        env->xregs[i] = regs->regs[i];
    }
    env->pc = regs->pc;
    env->xregs[31] = regs->sp;
}

/*
 * AArch64 store-release exclusive.
 *
 * rs = result of store exclusive
 * rt = register that is stored
 * rt2 = second register store (in STP)
 */
static inline int do_strex_a64(CPUARMState *env)
{
    uint64_t val;
    int size;
    bool is_pair;
    int rc = 1;
    int segv = 0;
    uint64_t addr;
    int rs, rt, rt2;

    start_exclusive();
    /* size | is_pair << 2 | (rs << 4) | (rt << 9) | (rt2 << 14)); */
    size = extract32(env->exclusive_info, 0, 2);
    is_pair = extract32(env->exclusive_info, 2, 1);
    rs = extract32(env->exclusive_info, 4, 5);
    rt = extract32(env->exclusive_info, 9, 5);
    rt2 = extract32(env->exclusive_info, 14, 5);

    addr = env->exclusive_addr;
    if (addr != env->exclusive_test) {
        goto finish;
    }

    switch (size) {
    case 0:
        segv = get_user_u8(val, addr);
        break;
    case 1:
        segv = get_user_u16(val, addr);
        break;
    case 2:
        segv = get_user_u32(val, addr);
        break;
    case 3:
        segv = get_user_u64(val, addr);
        break;
    default:
        abort();
    }
    if (segv) {
        env->exception.vaddress = addr;
        goto error;
    }
    if (val != env->exclusive_val) {
        goto finish;
    }
    if (is_pair) {
        if (size == 2) {
            segv = get_user_u32(val, addr + 4);
        } else {
            segv = get_user_u64(val, addr + 8);
        }
        if (segv) {
            env->exception.vaddress = addr + (size == 2 ? 4 : 8);
            goto error;
        }
        if (val != env->exclusive_high) {
            goto finish;
        }
    }
    /* Handle the zero register */
    val = rt == 31 ? 0 : env->xregs[rt];
    switch (size) {
    case 0:
        segv = put_user_u8(val, addr);
        break;
    case 1:
        segv = put_user_u16(val, addr);
        break;
    case 2:
        segv = put_user_u32(val, addr);
        break;
    case 3:
        segv = put_user_u64(val, addr);
        break;
    }
    if (segv) {
        goto error;
    }
    if (is_pair) {
        /* Handle the zero register. */
        val = rt2 == 31 ? 0 : env->xregs[rt2];
        if (size == 2) {
            segv = put_user_u32(val, addr + 4);
        } else {
            segv = put_user_u64(val, addr + 8);
        }
        if (segv) {
            env->exception.vaddress = addr + (size == 2 ? 4 : 8);
            goto error;
        }
    }
    rc = 0;

finish:
    env->pc += 4;
    /*
     * rs == 31 encodes a write to the ZR, thus throwing away
     * the status return.
     */
    if (rs < 31) {
        env->xregs[rs] = rc;
    }

error:
    /*
     * Instruction faulted, PC doesn't advance. Either way a strex
     * release any exclusive lock we have.
     */
    env->exclusive_addr = -1;
    end_exclusive();
    return segv;
}

static inline void target_cpu_loop(CPUARMState *env)
{
    CPUState *cs = CPU(arm_env_get_cpu(env));
    int trapnr, sig;
    target_siginfo_t info;
    uint64_t code, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8;
    uint32_t pstate;
    abi_long ret;

    for (;;) {
        cpu_exec_start(cs);
        trapnr = cpu_exec(cs);
        cpu_exec_end(cs);

        switch (trapnr) {
        case EXCP_SWI:
            /* See arm/arm/trap.c cpu_fetch_syscall_args() */
            code = env->xregs[8];
            if (code == TARGET_FREEBSD_NR_syscall) {
                code = env->xregs[0];
                arg1 = env->xregs[1];
                arg2 = env->xregs[2];
                arg3 = env->xregs[3];
                arg4 = env->xregs[4];
                arg5 = env->xregs[5];
                arg6 = env->xregs[6];
                arg7 = env->xregs[7];
                arg8 = 0;
            } else if (code == TARGET_FREEBSD_NR___syscall) {
#ifdef TARGET_WORDS_BIGENDIAN
                code = env->xregs[1];
#else
                code = env->xregs[0];
#endif
                arg1 = env->xregs[2];
                arg2 = env->xregs[3];
                arg3 = env->xregs[4];
                arg4 = env->xregs[5];
                arg5 = env->xregs[6];
                arg6 = env->xregs[7];
                arg7 = 0;
                arg8 = 0;
            } else {
                arg1 = env->xregs[0];
                arg2 = env->xregs[1];
                arg3 = env->xregs[2];
                arg4 = env->xregs[3];
                arg5 = env->xregs[4];
                arg6 = env->xregs[5];
                arg7 = env->xregs[6];
                arg8 = env->xregs[7];
            }
            ret = do_freebsd_syscall(env, code, arg1, arg2, arg3,
                    arg4, arg5, arg6, arg7, arg8);
            /*
             * The carry bit is cleared for no error; set for error.
             * See arm64/arm64/vm_machdep.c cpu_set_syscall_retval()
             */
            pstate = pstate_read(env);
            if (ret >= 0) {
                pstate &= ~PSTATE_C;
                env->xregs[0] = ret;
            } else if (ret == -TARGET_ERESTART) {
                env->pc -= 4;
                env->xregs[0] = -ret;
            } else if (ret != -TARGET_EJUSTRETURN) {
                pstate |= PSTATE_C;
                env->xregs[0] = -ret;
            }
            pstate_write(env, pstate);
            break;

		case EXCP_INTERRUPT:
            /* Just indicate that signals should be handle ASAP. */
            break;

        case EXCP_UDEF:
            info.si_signo = SIGILL;
            info.si_errno = 0;
            info.si_code = TARGET_ILL_ILLOPN;
            info.si_addr = env->pc;
            queue_signal(env, info.si_signo, &info);
            break;

        case EXCP_STREX:
            if (!do_strex_a64(env)) {
                break;
            }
            /* Fall through for segv... */

		case EXCP_PREFETCH_ABORT:
		case EXCP_DATA_ABORT:
            info.si_signo = SIGSEGV;
            info.si_errno = 0;
            /* XXX: check env->error_code */
            info.si_code = TARGET_SEGV_MAPERR;
            info.si_addr = env->exception.vaddress;
            queue_signal(env, info.si_signo, &info);
            break;

		case EXCP_DEBUG:
        case EXCP_BKPT:
            sig = gdb_handlesig(cs, TARGET_SIGTRAP);
            if (sig) {
                info.si_signo = sig;
                info.si_errno = 0;
                info.si_code = TARGET_TRAP_BRKPT;
                queue_signal(env, info.si_signo, &info);
            }
            break;

        default:
            fprintf(stderr, "qemu: unhandled CPU exception 0x%x - aborting\n",
                    trapnr);
            cpu_dump_state(cs, stderr, fprintf, 0);
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
    if (newsp)
        env->xregs[31] = newsp;
    env->regs[0] = 0;
    env->regs[1] = 0;
    pstate_write(env, 0);
}

static inline void target_cpu_reset(CPUArchState *cpu)
{
}

#endif /* !_TARGET_ARCH_CPU_H */
