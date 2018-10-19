/*
 *  MIPS emulation helpers for qemu.
 *
 *  Copyright (c) 2004-2005 Jocelyn Mayer
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
#include "qemu/osdep.h"
#include "qemu/main-loop.h"
#include "cpu.h"
#include "internal.h"
#include "qemu/host-utils.h"
#include "qemu/error-report.h"
#include "exec/helper-proto.h"
#include "exec/exec-all.h"
#include "exec/cpu_ldst.h"
#ifdef CONFIG_MIPS_LOG_INSTR
#include "exec/log.h"
#endif
#include "sysemu/kvm.h"
#include "sysemu/sysemu.h"

/*****************************************************************************/
/* Exceptions processing helpers */

void helper_raise_exception_err(CPUMIPSState *env, uint32_t exception,
                                int error_code)
{
    do_raise_exception_err(env, exception, error_code, 0);
}

void helper_raise_exception(CPUMIPSState *env, uint32_t exception)
{
    do_raise_exception(env, exception, GETPC());
}

void helper_raise_exception_debug(CPUMIPSState *env)
{
    do_raise_exception(env, EXCP_DEBUG, 0);
}

static void raise_exception(CPUMIPSState *env, uint32_t exception)
{
    do_raise_exception(env, exception, 0);
}

void helper_check_breakcount(struct CPUMIPSState* env)
{
    CPUState *cs = CPU(mips_env_get_cpu(env));
    /* Decrement the startup breakcount, if set. */
    if (unlikely(cs->breakcount)) {
        cs->breakcount--;
        if (cs->breakcount == 0UL) {
            qemu_log_mask(CPU_LOG_INSTR | CPU_LOG_INT | CPU_LOG_EXEC, "Reached breakcount!\n");
            helper_raise_exception(env, EXCP_DEBUG);
        }
    }
}

#ifdef CONFIG_MIPS_LOG_INSTR
/*
 * Print the instruction to log file.
 */
void helper_log_instruction(CPUMIPSState *env, target_ulong pc)
{
    int isa = (env->hflags & MIPS_HFLAG_M16) == 0 ? 0 : (env->insn_flags & ASE_MICROMIPS) ? 1 : 2;
    if (unlikely(qemu_loglevel_mask(CPU_LOG_INSTR))) {
        MIPSCPU *cpu = mips_env_get_cpu(env);
        CPUState *cs = CPU(cpu);

        /* Disassemble and print instruction. */
        if (isa == 0) {
            log_target_disas(cs, pc, 4);
        } else {
            log_target_disas(cs, pc, 2);
        }
    }

    if (unlikely(qemu_loglevel_mask(CPU_LOG_CVTRACE))) {
        static uint16_t cycles = 0;  /* XXX */
        uint32_t opcode;
        MIPSCPU *cpu = mips_env_get_cpu(env);
        CPUState *cs = CPU(cpu);

        /* if the logfile is empty we need to emit the cvt magic */
        if (env->cvtrace.version != 0 && ftell(qemu_logfile) != 0) {
            /* Write previous instruction trace to log. */
            fwrite(&env->cvtrace, sizeof(env->cvtrace), 1, qemu_logfile);
        } else {
            char buffer[sizeof(env->cvtrace)];

            buffer[0] = CVT_QEMU_VERSION;
            g_strlcpy(buffer+1, CVT_QEMU_MAGIC, sizeof(env->cvtrace)-2);
            fwrite(buffer, sizeof(env->cvtrace), 1, qemu_logfile);
            cycles = 0;
        }
        bzero(&env->cvtrace, sizeof(env->cvtrace));
        env->cvtrace.version = CVT_NO_REG;
        env->cvtrace.pc = tswap64(pc);
        env->cvtrace.cycles = tswap16(cycles++);
        env->cvtrace.thread = (uint8_t)cs->cpu_index;
        env->cvtrace.asid = (uint8_t)(env->active_tc.CP0_TCStatus & 0xff);
        env->cvtrace.exception = 31;

        /* Fetch opcode. */
        if (isa == 0) {
            /* mips32/mips64 instruction. */
            opcode = cpu_ldl_code(env, pc);
        } else {
            /* micromips or mips16. */
            opcode = cpu_lduw_code(env, pc);
            if (isa == 1) {
                /* micromips */
                switch (opcode >> 10) {
                case 0x01: case 0x02: case 0x03: case 0x09:
                case 0x0a: case 0x0b:
                case 0x11: case 0x12: case 0x13: case 0x19:
                case 0x1a: case 0x1b:
                case 0x20: case 0x21: case 0x22: case 0x23:
                case 0x28: case 0x29: case 0x2a: case 0x2b:
                case 0x30: case 0x31: case 0x32: case 0x33:
                case 0x38: case 0x39: case 0x3a: case 0x3b:
                    break;
                default:
                    opcode <<= 16;
                    opcode |= cpu_lduw_code(env, pc + 2);
                    break;
                }
            } else {
                /* mips16 */
                switch (opcode >> 11) {
                case 0x03:
                case 0x1e:
                    opcode <<= 16;
                    opcode |= cpu_lduw_code(env, pc + 2);
                    break;
                }
            }
        }
        env->cvtrace.inst = opcode;  /* XXX need bswapped? */
    }
}
#endif

#if defined(CONFIG_USER_ONLY)
#define HELPER_LD(name, insn, type)                                     \
static inline type do_##name(CPUMIPSState *env, target_ulong addr,      \
                             int mem_idx, uintptr_t retaddr)            \
{                                                                       \
    return (type) cpu_##insn##_data_ra(env, addr, retaddr);             \
}
#else
#define HELPER_LD(name, insn, type)                                     \
static inline type do_##name(CPUMIPSState *env, target_ulong addr,      \
                             int mem_idx, uintptr_t retaddr)            \
{                                                                       \
    switch (mem_idx)                                                    \
    {                                                                   \
    case 0: return (type) cpu_##insn##_kernel_ra(env, addr, retaddr);   \
    case 1: return (type) cpu_##insn##_super_ra(env, addr, retaddr);    \
    default:                                                            \
    case 2: return (type) cpu_##insn##_user_ra(env, addr, retaddr);     \
    case 3: return (type) cpu_##insn##_error_ra(env, addr, retaddr);    \
    }                                                                   \
}
#endif
HELPER_LD(lw, ldl, int32_t)
#if defined(TARGET_MIPS64)
HELPER_LD(ld, ldq, int64_t)
#endif
#undef HELPER_LD

#if defined(CONFIG_USER_ONLY)
#define HELPER_ST(name, insn, type)                                     \
static inline void do_##name(CPUMIPSState *env, target_ulong addr,      \
                             type val, int mem_idx, uintptr_t retaddr)  \
{                                                                       \
    cpu_##insn##_data_ra(env, addr, val, retaddr);                      \
}
#else
#define HELPER_ST(name, insn, type)                                     \
static inline void do_##name(CPUMIPSState *env, target_ulong addr,      \
                             type val, int mem_idx, uintptr_t retaddr)  \
{                                                                       \
    switch (mem_idx)                                                    \
    {                                                                   \
    case 0: cpu_##insn##_kernel_ra(env, addr, val, retaddr); break;     \
    case 1: cpu_##insn##_super_ra(env, addr, val, retaddr); break;      \
    default:                                                            \
    case 2: cpu_##insn##_user_ra(env, addr, val, retaddr); break;       \
    case 3:                                                             \
        cpu_##insn##_error_ra(env, addr, val, retaddr);                 \
        break;                                                          \
    }                                                                   \
}
#endif
HELPER_ST(sb, stb, uint8_t)
HELPER_ST(sw, stl, uint32_t)
#if defined(TARGET_MIPS64)
HELPER_ST(sd, stq, uint64_t)
#endif
#undef HELPER_ST

/* 64 bits arithmetic for 32 bits hosts */
static inline uint64_t get_HILO(CPUMIPSState *env)
{
    return ((uint64_t)(env->active_tc.HI[0]) << 32) | (uint32_t)env->active_tc.LO[0];
}

static inline target_ulong set_HIT0_LO(CPUMIPSState *env, uint64_t HILO)
{
    env->active_tc.LO[0] = (int32_t)(HILO & 0xFFFFFFFF);
    return env->active_tc.HI[0] = (int32_t)(HILO >> 32);
}

static inline target_ulong set_HI_LOT0(CPUMIPSState *env, uint64_t HILO)
{
    target_ulong tmp = env->active_tc.LO[0] = (int32_t)(HILO & 0xFFFFFFFF);
    env->active_tc.HI[0] = (int32_t)(HILO >> 32);
    return tmp;
}

/* Multiplication variants of the vr54xx. */
target_ulong helper_muls(CPUMIPSState *env, target_ulong arg1,
                         target_ulong arg2)
{
    return set_HI_LOT0(env, 0 - ((int64_t)(int32_t)arg1 *
                                 (int64_t)(int32_t)arg2));
}

target_ulong helper_mulsu(CPUMIPSState *env, target_ulong arg1,
                          target_ulong arg2)
{
    return set_HI_LOT0(env, 0 - (uint64_t)(uint32_t)arg1 *
                       (uint64_t)(uint32_t)arg2);
}

target_ulong helper_macc(CPUMIPSState *env, target_ulong arg1,
                         target_ulong arg2)
{
    return set_HI_LOT0(env, (int64_t)get_HILO(env) + (int64_t)(int32_t)arg1 *
                       (int64_t)(int32_t)arg2);
}

target_ulong helper_macchi(CPUMIPSState *env, target_ulong arg1,
                           target_ulong arg2)
{
    return set_HIT0_LO(env, (int64_t)get_HILO(env) + (int64_t)(int32_t)arg1 *
                       (int64_t)(int32_t)arg2);
}

target_ulong helper_maccu(CPUMIPSState *env, target_ulong arg1,
                          target_ulong arg2)
{
    return set_HI_LOT0(env, (uint64_t)get_HILO(env) +
                       (uint64_t)(uint32_t)arg1 * (uint64_t)(uint32_t)arg2);
}

target_ulong helper_macchiu(CPUMIPSState *env, target_ulong arg1,
                            target_ulong arg2)
{
    return set_HIT0_LO(env, (uint64_t)get_HILO(env) +
                       (uint64_t)(uint32_t)arg1 * (uint64_t)(uint32_t)arg2);
}

target_ulong helper_msac(CPUMIPSState *env, target_ulong arg1,
                         target_ulong arg2)
{
    return set_HI_LOT0(env, (int64_t)get_HILO(env) - (int64_t)(int32_t)arg1 *
                       (int64_t)(int32_t)arg2);
}

target_ulong helper_msachi(CPUMIPSState *env, target_ulong arg1,
                           target_ulong arg2)
{
    return set_HIT0_LO(env, (int64_t)get_HILO(env) - (int64_t)(int32_t)arg1 *
                       (int64_t)(int32_t)arg2);
}

target_ulong helper_msacu(CPUMIPSState *env, target_ulong arg1,
                          target_ulong arg2)
{
    return set_HI_LOT0(env, (uint64_t)get_HILO(env) -
                       (uint64_t)(uint32_t)arg1 * (uint64_t)(uint32_t)arg2);
}

target_ulong helper_msachiu(CPUMIPSState *env, target_ulong arg1,
                            target_ulong arg2)
{
    return set_HIT0_LO(env, (uint64_t)get_HILO(env) -
                       (uint64_t)(uint32_t)arg1 * (uint64_t)(uint32_t)arg2);
}

target_ulong helper_mulhi(CPUMIPSState *env, target_ulong arg1,
                          target_ulong arg2)
{
    return set_HIT0_LO(env, (int64_t)(int32_t)arg1 * (int64_t)(int32_t)arg2);
}

target_ulong helper_mulhiu(CPUMIPSState *env, target_ulong arg1,
                           target_ulong arg2)
{
    return set_HIT0_LO(env, (uint64_t)(uint32_t)arg1 *
                       (uint64_t)(uint32_t)arg2);
}

target_ulong helper_mulshi(CPUMIPSState *env, target_ulong arg1,
                           target_ulong arg2)
{
    return set_HIT0_LO(env, 0 - (int64_t)(int32_t)arg1 *
                       (int64_t)(int32_t)arg2);
}

target_ulong helper_mulshiu(CPUMIPSState *env, target_ulong arg1,
                            target_ulong arg2)
{
    return set_HIT0_LO(env, 0 - (uint64_t)(uint32_t)arg1 *
                       (uint64_t)(uint32_t)arg2);
}

static inline target_ulong bitswap(target_ulong v)
{
    v = ((v >> 1) & (target_ulong)0x5555555555555555ULL) |
              ((v & (target_ulong)0x5555555555555555ULL) << 1);
    v = ((v >> 2) & (target_ulong)0x3333333333333333ULL) |
              ((v & (target_ulong)0x3333333333333333ULL) << 2);
    v = ((v >> 4) & (target_ulong)0x0F0F0F0F0F0F0F0FULL) |
              ((v & (target_ulong)0x0F0F0F0F0F0F0F0FULL) << 4);
    return v;
}

#ifdef TARGET_MIPS64
target_ulong helper_dbitswap(target_ulong rt)
{
    return bitswap(rt);
}
#endif

target_ulong helper_bitswap(target_ulong rt)
{
    return (int32_t)bitswap(rt);
}

#ifndef CONFIG_USER_ONLY

static inline hwaddr do_translate_address(CPUMIPSState *env,
                                                      target_ulong address,
                                                      int rw, uintptr_t retaddr)
{
    hwaddr lladdr;
    CPUState *cs = CPU(mips_env_get_cpu(env));

    lladdr = cpu_mips_translate_address(env, address, rw);

    if (lladdr == -1LL) {
        cpu_loop_exit_restore(cs, retaddr);
    } else {
        return lladdr;
    }
}

#define HELPER_LD_ATOMIC(name, insn, almask)                                  \
target_ulong helper_##name(CPUMIPSState *env, target_ulong arg, int mem_idx)  \
{                                                                             \
    if (arg & almask) {                                                       \
        env->CP0_BadVAddr = arg;                                              \
        do_raise_exception(env, EXCP_AdEL, GETPC());                          \
    }                                                                         \
    env->lladdr = do_translate_address(env, arg, 0, GETPC());                 \
    env->llval = do_##insn(env, arg, mem_idx, GETPC());                       \
    return env->llval;                                                        \
}
HELPER_LD_ATOMIC(ll, lw, 0x3)
#ifdef TARGET_MIPS64
HELPER_LD_ATOMIC(lld, ld, 0x7)
#endif
#undef HELPER_LD_ATOMIC

#define HELPER_ST_ATOMIC(name, ld_insn, st_insn, almask)                      \
target_ulong helper_##name(CPUMIPSState *env, target_ulong arg1,              \
                           target_ulong arg2, int mem_idx)                    \
{                                                                             \
    target_long tmp;                                                          \
                                                                              \
    if (arg2 & almask) {                                                      \
        env->CP0_BadVAddr = arg2;                                             \
        do_raise_exception(env, EXCP_AdES, GETPC());                          \
    }                                                                         \
    if (do_translate_address(env, arg2, 1, GETPC()) == env->lladdr) {         \
        tmp = do_##ld_insn(env, arg2, mem_idx, GETPC());                      \
        if (tmp == env->llval) {                                              \
            do_##st_insn(env, arg2, arg1, mem_idx, GETPC());                  \
            return 1;                                                         \
        }                                                                     \
    }                                                                         \
    return 0;                                                                 \
}
HELPER_ST_ATOMIC(sc, lw, sw, 0x3)
#ifdef TARGET_MIPS64
HELPER_ST_ATOMIC(scd, ld, sd, 0x7)
#endif
#undef HELPER_ST_ATOMIC
#endif

#ifdef TARGET_WORDS_BIGENDIAN
#define GET_LMASK(v) ((v) & 3)
#define GET_OFFSET(addr, offset) (addr + (offset))
#else
#define GET_LMASK(v) (((v) & 3) ^ 3)
#define GET_OFFSET(addr, offset) (addr - (offset))
#endif

void helper_swl(CPUMIPSState *env, target_ulong arg1, target_ulong arg2,
                int mem_idx)
{
    do_sb(env, arg2, (uint8_t)(arg1 >> 24), mem_idx, GETPC());

    if (GET_LMASK(arg2) <= 2) {
        do_sb(env, GET_OFFSET(arg2, 1), (uint8_t)(arg1 >> 16), mem_idx,
              GETPC());
    }

    if (GET_LMASK(arg2) <= 1) {
        do_sb(env, GET_OFFSET(arg2, 2), (uint8_t)(arg1 >> 8), mem_idx,
              GETPC());
    }

    if (GET_LMASK(arg2) == 0) {
        do_sb(env, GET_OFFSET(arg2, 3), (uint8_t)arg1, mem_idx,
              GETPC());
    }
}

void helper_swr(CPUMIPSState *env, target_ulong arg1, target_ulong arg2,
                int mem_idx)
{
    do_sb(env, arg2, (uint8_t)arg1, mem_idx, GETPC());

    if (GET_LMASK(arg2) >= 1) {
        do_sb(env, GET_OFFSET(arg2, -1), (uint8_t)(arg1 >> 8), mem_idx,
              GETPC());
    }

    if (GET_LMASK(arg2) >= 2) {
        do_sb(env, GET_OFFSET(arg2, -2), (uint8_t)(arg1 >> 16), mem_idx,
              GETPC());
    }

    if (GET_LMASK(arg2) == 3) {
        do_sb(env, GET_OFFSET(arg2, -3), (uint8_t)(arg1 >> 24), mem_idx,
              GETPC());
    }
}

#if defined(TARGET_MIPS64)
/* "half" load and stores.  We must do the memory access inline,
   or fault handling won't work.  */

#ifdef TARGET_WORDS_BIGENDIAN
#define GET_LMASK64(v) ((v) & 7)
#else
#define GET_LMASK64(v) (((v) & 7) ^ 7)
#endif

void helper_sdl(CPUMIPSState *env, target_ulong arg1, target_ulong arg2,
                int mem_idx)
{
    do_sb(env, arg2, (uint8_t)(arg1 >> 56), mem_idx, GETPC());

    if (GET_LMASK64(arg2) <= 6) {
        do_sb(env, GET_OFFSET(arg2, 1), (uint8_t)(arg1 >> 48), mem_idx,
              GETPC());
    }

    if (GET_LMASK64(arg2) <= 5) {
        do_sb(env, GET_OFFSET(arg2, 2), (uint8_t)(arg1 >> 40), mem_idx,
              GETPC());
    }

    if (GET_LMASK64(arg2) <= 4) {
        do_sb(env, GET_OFFSET(arg2, 3), (uint8_t)(arg1 >> 32), mem_idx,
              GETPC());
    }

    if (GET_LMASK64(arg2) <= 3) {
        do_sb(env, GET_OFFSET(arg2, 4), (uint8_t)(arg1 >> 24), mem_idx,
              GETPC());
    }

    if (GET_LMASK64(arg2) <= 2) {
        do_sb(env, GET_OFFSET(arg2, 5), (uint8_t)(arg1 >> 16), mem_idx,
              GETPC());
    }

    if (GET_LMASK64(arg2) <= 1) {
        do_sb(env, GET_OFFSET(arg2, 6), (uint8_t)(arg1 >> 8), mem_idx,
              GETPC());
    }

    if (GET_LMASK64(arg2) <= 0) {
        do_sb(env, GET_OFFSET(arg2, 7), (uint8_t)arg1, mem_idx,
              GETPC());
    }
}

void helper_sdr(CPUMIPSState *env, target_ulong arg1, target_ulong arg2,
                int mem_idx)
{
    do_sb(env, arg2, (uint8_t)arg1, mem_idx, GETPC());

    if (GET_LMASK64(arg2) >= 1) {
        do_sb(env, GET_OFFSET(arg2, -1), (uint8_t)(arg1 >> 8), mem_idx,
              GETPC());
    }

    if (GET_LMASK64(arg2) >= 2) {
        do_sb(env, GET_OFFSET(arg2, -2), (uint8_t)(arg1 >> 16), mem_idx,
              GETPC());
    }

    if (GET_LMASK64(arg2) >= 3) {
        do_sb(env, GET_OFFSET(arg2, -3), (uint8_t)(arg1 >> 24), mem_idx,
              GETPC());
    }

    if (GET_LMASK64(arg2) >= 4) {
        do_sb(env, GET_OFFSET(arg2, -4), (uint8_t)(arg1 >> 32), mem_idx,
              GETPC());
    }

    if (GET_LMASK64(arg2) >= 5) {
        do_sb(env, GET_OFFSET(arg2, -5), (uint8_t)(arg1 >> 40), mem_idx,
              GETPC());
    }

    if (GET_LMASK64(arg2) >= 6) {
        do_sb(env, GET_OFFSET(arg2, -6), (uint8_t)(arg1 >> 48), mem_idx,
              GETPC());
    }

    if (GET_LMASK64(arg2) == 7) {
        do_sb(env, GET_OFFSET(arg2, -7), (uint8_t)(arg1 >> 56), mem_idx,
              GETPC());
    }
}
#endif /* TARGET_MIPS64 */

static const int multiple_regs[] = { 16, 17, 18, 19, 20, 21, 22, 23, 30 };

void helper_lwm(CPUMIPSState *env, target_ulong addr, target_ulong reglist,
                uint32_t mem_idx)
{
    target_ulong base_reglist = reglist & 0xf;
    target_ulong do_r31 = reglist & 0x10;

    if (base_reglist > 0 && base_reglist <= ARRAY_SIZE (multiple_regs)) {
        target_ulong i;

        for (i = 0; i < base_reglist; i++) {
            env->active_tc.gpr[multiple_regs[i]] =
                (target_long)do_lw(env, addr, mem_idx, GETPC());
            addr += 4;
        }
    }

    if (do_r31) {
        env->active_tc.gpr[31] = (target_long)do_lw(env, addr, mem_idx,
                                                    GETPC());
    }
}

void helper_swm(CPUMIPSState *env, target_ulong addr, target_ulong reglist,
                uint32_t mem_idx)
{
    target_ulong base_reglist = reglist & 0xf;
    target_ulong do_r31 = reglist & 0x10;

    if (base_reglist > 0 && base_reglist <= ARRAY_SIZE (multiple_regs)) {
        target_ulong i;

        for (i = 0; i < base_reglist; i++) {
            do_sw(env, addr, env->active_tc.gpr[multiple_regs[i]], mem_idx,
                  GETPC());
            addr += 4;
        }
    }

    if (do_r31) {
        do_sw(env, addr, env->active_tc.gpr[31], mem_idx, GETPC());
    }
}

#if defined(TARGET_MIPS64)
void helper_ldm(CPUMIPSState *env, target_ulong addr, target_ulong reglist,
                uint32_t mem_idx)
{
    target_ulong base_reglist = reglist & 0xf;
    target_ulong do_r31 = reglist & 0x10;

    if (base_reglist > 0 && base_reglist <= ARRAY_SIZE (multiple_regs)) {
        target_ulong i;

        for (i = 0; i < base_reglist; i++) {
            env->active_tc.gpr[multiple_regs[i]] = do_ld(env, addr, mem_idx,
                                                         GETPC());
            addr += 8;
        }
    }

    if (do_r31) {
        env->active_tc.gpr[31] = do_ld(env, addr, mem_idx, GETPC());
    }
}

void helper_sdm(CPUMIPSState *env, target_ulong addr, target_ulong reglist,
                uint32_t mem_idx)
{
    target_ulong base_reglist = reglist & 0xf;
    target_ulong do_r31 = reglist & 0x10;

    if (base_reglist > 0 && base_reglist <= ARRAY_SIZE (multiple_regs)) {
        target_ulong i;

        for (i = 0; i < base_reglist; i++) {
            do_sd(env, addr, env->active_tc.gpr[multiple_regs[i]], mem_idx,
                  GETPC());
            addr += 8;
        }
    }

    if (do_r31) {
        do_sd(env, addr, env->active_tc.gpr[31], mem_idx, GETPC());
    }
}
#endif

#ifndef CONFIG_USER_ONLY
/* SMP helpers.  */
static bool mips_vpe_is_wfi(MIPSCPU *c)
{
    CPUState *cpu = CPU(c);
    CPUMIPSState *env = &c->env;

    /* If the VPE is halted but otherwise active, it means it's waiting for
       an interrupt.  */
    return cpu->halted && mips_vpe_active(env);
}

static bool mips_vp_is_wfi(MIPSCPU *c)
{
    CPUState *cpu = CPU(c);
    CPUMIPSState *env = &c->env;

    return cpu->halted && mips_vp_active(env);
}

static inline void mips_vpe_wake(MIPSCPU *c)
{
    /* Don't set ->halted = 0 directly, let it be done via cpu_has_work
       because there might be other conditions that state that c should
       be sleeping.  */
    cpu_interrupt(CPU(c), CPU_INTERRUPT_WAKE);
}

static inline void mips_vpe_sleep(MIPSCPU *cpu)
{
    CPUState *cs = CPU(cpu);

    /* The VPE was shut off, really go to bed.
       Reset any old _WAKE requests.  */
    cs->halted = 1;
    cpu_reset_interrupt(cs, CPU_INTERRUPT_WAKE);
}

static inline void mips_tc_wake(MIPSCPU *cpu, int tc)
{
    CPUMIPSState *c = &cpu->env;

    /* FIXME: TC reschedule.  */
    if (mips_vpe_active(c) && !mips_vpe_is_wfi(cpu)) {
        mips_vpe_wake(cpu);
    }
}

static inline void mips_tc_sleep(MIPSCPU *cpu, int tc)
{
    CPUMIPSState *c = &cpu->env;

    /* FIXME: TC reschedule.  */
    if (!mips_vpe_active(c)) {
        mips_vpe_sleep(cpu);
    }
}

/**
 * mips_cpu_map_tc:
 * @env: CPU from which mapping is performed.
 * @tc: Should point to an int with the value of the global TC index.
 *
 * This function will transform @tc into a local index within the
 * returned #CPUMIPSState.
 */
/* FIXME: This code assumes that all VPEs have the same number of TCs,
          which depends on runtime setup. Can probably be fixed by
          walking the list of CPUMIPSStates.  */
static CPUMIPSState *mips_cpu_map_tc(CPUMIPSState *env, int *tc)
{
    MIPSCPU *cpu;
    CPUState *cs;
    CPUState *other_cs;
    int vpe_idx;
    int tc_idx = *tc;

    if (!(env->CP0_VPEConf0 & (1 << CP0VPEC0_MVP))) {
        /* Not allowed to address other CPUs.  */
        *tc = env->current_tc;
        return env;
    }

    cs = CPU(mips_env_get_cpu(env));
    vpe_idx = tc_idx / cs->nr_threads;
    *tc = tc_idx % cs->nr_threads;
    other_cs = qemu_get_cpu(vpe_idx);
    if (other_cs == NULL) {
        return env;
    }
    cpu = MIPS_CPU(other_cs);
    return &cpu->env;
}

/* The per VPE CP0_Status register shares some fields with the per TC
   CP0_TCStatus registers. These fields are wired to the same registers,
   so changes to either of them should be reflected on both registers.

   Also, EntryHi shares the bottom 8 bit ASID with TCStauts.

   These helper call synchronizes the regs for a given cpu.  */

/* Called for updates to CP0_Status.  Defined in "cpu.h" for gdbstub.c.  */
/* static inline void sync_c0_status(CPUMIPSState *env, CPUMIPSState *cpu,
                                     int tc);  */

/* Called for updates to CP0_TCStatus.  */
static void sync_c0_tcstatus(CPUMIPSState *cpu, int tc,
                             target_ulong v)
{
    uint32_t status;
    uint32_t tcu, tmx, tasid, tksu;
    uint32_t mask = ((1U << CP0St_CU3)
                       | (1 << CP0St_CU2)
                       | (1 << CP0St_CU1)
                       | (1 << CP0St_CU0)
                       | (1 << CP0St_MX)
                       | (3 << CP0St_KSU));

    tcu = (v >> CP0TCSt_TCU0) & 0xf;
    tmx = (v >> CP0TCSt_TMX) & 0x1;
    tasid = v & cpu->CP0_EntryHi_ASID_mask;
    tksu = (v >> CP0TCSt_TKSU) & 0x3;

    status = tcu << CP0St_CU0;
    status |= tmx << CP0St_MX;
    status |= tksu << CP0St_KSU;

    cpu->CP0_Status &= ~mask;
    cpu->CP0_Status |= status;

    /* Sync the TASID with EntryHi.  */
    cpu->CP0_EntryHi &= ~cpu->CP0_EntryHi_ASID_mask;
    cpu->CP0_EntryHi |= tasid;

    compute_hflags(cpu);
}

/* Called for updates to CP0_EntryHi.  */
static void sync_c0_entryhi(CPUMIPSState *cpu, int tc)
{
    int32_t *tcst;
    uint32_t asid, v = cpu->CP0_EntryHi;

    asid = v & cpu->CP0_EntryHi_ASID_mask;

    if (tc == cpu->current_tc) {
        tcst = &cpu->active_tc.CP0_TCStatus;
    } else {
        tcst = &cpu->tcs[tc].CP0_TCStatus;
    }

    *tcst &= ~cpu->CP0_EntryHi_ASID_mask;
    *tcst |= asid;
}

/* CP0 helpers */
target_ulong helper_mfc0_mvpcontrol(CPUMIPSState *env)
{
    return env->mvp->CP0_MVPControl;
}

target_ulong helper_mfc0_mvpconf0(CPUMIPSState *env)
{
    return env->mvp->CP0_MVPConf0;
}

target_ulong helper_mfc0_mvpconf1(CPUMIPSState *env)
{
    return env->mvp->CP0_MVPConf1;
}

target_ulong helper_mfc0_random(CPUMIPSState *env)
{
    return (int32_t)cpu_mips_get_random(env);
}

target_ulong helper_mfc0_tcstatus(CPUMIPSState *env)
{
    return env->active_tc.CP0_TCStatus;
}

target_ulong helper_mftc0_tcstatus(CPUMIPSState *env)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    if (other_tc == other->current_tc)
        return other->active_tc.CP0_TCStatus;
    else
        return other->tcs[other_tc].CP0_TCStatus;
}

target_ulong helper_mfc0_tcbind(CPUMIPSState *env)
{
    return env->active_tc.CP0_TCBind;
}

target_ulong helper_mftc0_tcbind(CPUMIPSState *env)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    if (other_tc == other->current_tc)
        return other->active_tc.CP0_TCBind;
    else
        return other->tcs[other_tc].CP0_TCBind;
}

target_ulong helper_mfc0_tcrestart(CPUMIPSState *env)
{
    return env->active_tc.PC;
}

target_ulong helper_mftc0_tcrestart(CPUMIPSState *env)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    if (other_tc == other->current_tc)
        return other->active_tc.PC;
    else
        return other->tcs[other_tc].PC;
}

target_ulong helper_mfc0_tchalt(CPUMIPSState *env)
{
    return env->active_tc.CP0_TCHalt;
}

target_ulong helper_mftc0_tchalt(CPUMIPSState *env)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    if (other_tc == other->current_tc)
        return other->active_tc.CP0_TCHalt;
    else
        return other->tcs[other_tc].CP0_TCHalt;
}

target_ulong helper_mfc0_tccontext(CPUMIPSState *env)
{
    return env->active_tc.CP0_TCContext;
}

target_ulong helper_mftc0_tccontext(CPUMIPSState *env)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    if (other_tc == other->current_tc)
        return other->active_tc.CP0_TCContext;
    else
        return other->tcs[other_tc].CP0_TCContext;
}

target_ulong helper_mfc0_tcschedule(CPUMIPSState *env)
{
    return env->active_tc.CP0_TCSchedule;
}

target_ulong helper_mftc0_tcschedule(CPUMIPSState *env)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    if (other_tc == other->current_tc)
        return other->active_tc.CP0_TCSchedule;
    else
        return other->tcs[other_tc].CP0_TCSchedule;
}

target_ulong helper_mfc0_tcschefback(CPUMIPSState *env)
{
    return env->active_tc.CP0_TCScheFBack;
}

target_ulong helper_mftc0_tcschefback(CPUMIPSState *env)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    if (other_tc == other->current_tc)
        return other->active_tc.CP0_TCScheFBack;
    else
        return other->tcs[other_tc].CP0_TCScheFBack;
}

target_ulong helper_mfc0_count(CPUMIPSState *env)
{
    int32_t count;
    qemu_mutex_lock_iothread();
    count = (int32_t) cpu_mips_get_count(env);
    qemu_mutex_unlock_iothread();
    return count;
}

target_ulong helper_mftc0_entryhi(CPUMIPSState *env)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    return other->CP0_EntryHi;
}

target_ulong helper_mftc0_cause(CPUMIPSState *env)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    int32_t tccause;
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    if (other_tc == other->current_tc) {
        tccause = other->CP0_Cause;
    } else {
        tccause = other->CP0_Cause;
    }

    return tccause;
}

target_ulong helper_mftc0_status(CPUMIPSState *env)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    return other->CP0_Status;
}

target_ulong helper_mfc0_lladdr(CPUMIPSState *env)
{
    return (int32_t)(env->lladdr >> env->CP0_LLAddr_shift);
}

target_ulong helper_mfc0_maar(CPUMIPSState *env)
{
    return (int32_t) env->CP0_MAAR[env->CP0_MAARI];
}

target_ulong helper_mfhc0_maar(CPUMIPSState *env)
{
    return env->CP0_MAAR[env->CP0_MAARI] >> 32;
}

target_ulong helper_mfc0_watchlo(CPUMIPSState *env, uint32_t sel)
{
    return (int32_t)env->CP0_WatchLo[sel];
}

target_ulong helper_mfc0_watchhi(CPUMIPSState *env, uint32_t sel)
{
    return env->CP0_WatchHi[sel];
}

target_ulong helper_mfc0_debug(CPUMIPSState *env)
{
    target_ulong t0 = env->CP0_Debug;
    if (env->hflags & MIPS_HFLAG_DM)
        t0 |= 1 << CP0DB_DM;

    return t0;
}

target_ulong helper_mftc0_debug(CPUMIPSState *env)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    int32_t tcstatus;
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    if (other_tc == other->current_tc)
        tcstatus = other->active_tc.CP0_Debug_tcstatus;
    else
        tcstatus = other->tcs[other_tc].CP0_Debug_tcstatus;

    /* XXX: Might be wrong, check with EJTAG spec. */
    return (other->CP0_Debug & ~((1 << CP0DB_SSt) | (1 << CP0DB_Halt))) |
            (tcstatus & ((1 << CP0DB_SSt) | (1 << CP0DB_Halt)));
}

#if defined(TARGET_MIPS64)
target_ulong helper_dmfc0_tcrestart(CPUMIPSState *env)
{
    return env->active_tc.PC;
}

target_ulong helper_dmfc0_tchalt(CPUMIPSState *env)
{
    return env->active_tc.CP0_TCHalt;
}

target_ulong helper_dmfc0_tccontext(CPUMIPSState *env)
{
    return env->active_tc.CP0_TCContext;
}

target_ulong helper_dmfc0_tcschedule(CPUMIPSState *env)
{
    return env->active_tc.CP0_TCSchedule;
}

target_ulong helper_dmfc0_tcschefback(CPUMIPSState *env)
{
    return env->active_tc.CP0_TCScheFBack;
}

target_ulong helper_dmfc0_lladdr(CPUMIPSState *env)
{
    return env->lladdr >> env->CP0_LLAddr_shift;
}

target_ulong helper_dmfc0_maar(CPUMIPSState *env)
{
    return env->CP0_MAAR[env->CP0_MAARI];
}

target_ulong helper_dmfc0_watchlo(CPUMIPSState *env, uint32_t sel)
{
    return env->CP0_WatchLo[sel];
}
#endif /* TARGET_MIPS64 */

void helper_mtc0_index(CPUMIPSState *env, target_ulong arg1)
{
    uint32_t index_p = env->CP0_Index & 0x80000000;
    uint32_t tlb_index = arg1 & 0x7fffffff;
    if (tlb_index < env->tlb->nb_tlb) {
        if (env->insn_flags & ISA_MIPS32R6) {
            index_p |= arg1 & 0x80000000;
        }
        env->CP0_Index = index_p | tlb_index;
    }
}

void helper_mtc0_mvpcontrol(CPUMIPSState *env, target_ulong arg1)
{
    uint32_t mask = 0;
    uint32_t newval;

    if (env->CP0_VPEConf0 & (1 << CP0VPEC0_MVP))
        mask |= (1 << CP0MVPCo_CPA) | (1 << CP0MVPCo_VPC) |
                (1 << CP0MVPCo_EVP);
    if (env->mvp->CP0_MVPControl & (1 << CP0MVPCo_VPC))
        mask |= (1 << CP0MVPCo_STLB);
    newval = (env->mvp->CP0_MVPControl & ~mask) | (arg1 & mask);

    // TODO: Enable/disable shared TLB, enable/disable VPEs.

    env->mvp->CP0_MVPControl = newval;
}

void helper_mtc0_vpecontrol(CPUMIPSState *env, target_ulong arg1)
{
    uint32_t mask;
    uint32_t newval;

    mask = (1 << CP0VPECo_YSI) | (1 << CP0VPECo_GSI) |
           (1 << CP0VPECo_TE) | (0xff << CP0VPECo_TargTC);
    newval = (env->CP0_VPEControl & ~mask) | (arg1 & mask);

    /* Yield scheduler intercept not implemented. */
    /* Gating storage scheduler intercept not implemented. */

    // TODO: Enable/disable TCs.

    env->CP0_VPEControl = newval;
}

void helper_mttc0_vpecontrol(CPUMIPSState *env, target_ulong arg1)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);
    uint32_t mask;
    uint32_t newval;

    mask = (1 << CP0VPECo_YSI) | (1 << CP0VPECo_GSI) |
           (1 << CP0VPECo_TE) | (0xff << CP0VPECo_TargTC);
    newval = (other->CP0_VPEControl & ~mask) | (arg1 & mask);

    /* TODO: Enable/disable TCs.  */

    other->CP0_VPEControl = newval;
}

target_ulong helper_mftc0_vpecontrol(CPUMIPSState *env)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);
    /* FIXME: Mask away return zero on read bits.  */
    return other->CP0_VPEControl;
}

target_ulong helper_mftc0_vpeconf0(CPUMIPSState *env)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    return other->CP0_VPEConf0;
}

void helper_mtc0_vpeconf0(CPUMIPSState *env, target_ulong arg1)
{
    uint32_t mask = 0;
    uint32_t newval;

    if (env->CP0_VPEConf0 & (1 << CP0VPEC0_MVP)) {
        if (env->CP0_VPEConf0 & (1 << CP0VPEC0_VPA))
            mask |= (0xff << CP0VPEC0_XTC);
        mask |= (1 << CP0VPEC0_MVP) | (1 << CP0VPEC0_VPA);
    }
    newval = (env->CP0_VPEConf0 & ~mask) | (arg1 & mask);

    // TODO: TC exclusive handling due to ERL/EXL.

    env->CP0_VPEConf0 = newval;
}

void helper_mttc0_vpeconf0(CPUMIPSState *env, target_ulong arg1)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);
    uint32_t mask = 0;
    uint32_t newval;

    mask |= (1 << CP0VPEC0_MVP) | (1 << CP0VPEC0_VPA);
    newval = (other->CP0_VPEConf0 & ~mask) | (arg1 & mask);

    /* TODO: TC exclusive handling due to ERL/EXL.  */
    other->CP0_VPEConf0 = newval;
}

void helper_mtc0_vpeconf1(CPUMIPSState *env, target_ulong arg1)
{
    uint32_t mask = 0;
    uint32_t newval;

    if (env->mvp->CP0_MVPControl & (1 << CP0MVPCo_VPC))
        mask |= (0xff << CP0VPEC1_NCX) | (0xff << CP0VPEC1_NCP2) |
                (0xff << CP0VPEC1_NCP1);
    newval = (env->CP0_VPEConf1 & ~mask) | (arg1 & mask);

    /* UDI not implemented. */
    /* CP2 not implemented. */

    // TODO: Handle FPU (CP1) binding.

    env->CP0_VPEConf1 = newval;
}

void helper_mtc0_yqmask(CPUMIPSState *env, target_ulong arg1)
{
    /* Yield qualifier inputs not implemented. */
    env->CP0_YQMask = 0x00000000;
}

void helper_mtc0_vpeopt(CPUMIPSState *env, target_ulong arg1)
{
    env->CP0_VPEOpt = arg1 & 0x0000ffff;
}

#define MTC0_ENTRYLO_MASK(env) ((env->PAMask >> 6) & 0x3FFFFFFF)

void helper_mtc0_entrylo0(CPUMIPSState *env, target_ulong arg1)
{
    /* 1k pages not implemented */
    target_ulong rxi = arg1 & (env->CP0_PageGrain & (3u << CP0PG_XIE));
    env->CP0_EntryLo0 = (arg1 & MTC0_ENTRYLO_MASK(env))
#if defined(TARGET_CHERI)
                        | (rxi << (CP0EnLo_L - 30));
#else
                        | (rxi << (CP0EnLo_XI - 30));
#endif /* TARGET_CHERI */
}

#if defined(TARGET_MIPS64)
#ifdef TARGET_CHERI
#define DMTC0_ENTRYLO_MASK(env) ((env->PAMask >> 6) | (1ull << CP0EnLo_S) | (1ull << CP0EnLo_L))
#else
#define DMTC0_ENTRYLO_MASK(env) (env->PAMask >> 6)
#endif /* TARGET_CHERI */

void helper_dmtc0_entrylo0(CPUMIPSState *env, uint64_t arg1)
{
    uint64_t rxi = arg1 & ((env->CP0_PageGrain & (3ull << CP0PG_XIE)) << 32);
    env->CP0_EntryLo0 = (arg1 & DMTC0_ENTRYLO_MASK(env)) | rxi;
}
#endif

void helper_mtc0_tcstatus(CPUMIPSState *env, target_ulong arg1)
{
    uint32_t mask = env->CP0_TCStatus_rw_bitmask;
    uint32_t newval;

    newval = (env->active_tc.CP0_TCStatus & ~mask) | (arg1 & mask);

    env->active_tc.CP0_TCStatus = newval;
    sync_c0_tcstatus(env, env->current_tc, newval);
}

void helper_mttc0_tcstatus(CPUMIPSState *env, target_ulong arg1)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    if (other_tc == other->current_tc)
        other->active_tc.CP0_TCStatus = arg1;
    else
        other->tcs[other_tc].CP0_TCStatus = arg1;
    sync_c0_tcstatus(other, other_tc, arg1);
}

void helper_mtc0_tcbind(CPUMIPSState *env, target_ulong arg1)
{
    uint32_t mask = (1 << CP0TCBd_TBE);
    uint32_t newval;

    if (env->mvp->CP0_MVPControl & (1 << CP0MVPCo_VPC))
        mask |= (1 << CP0TCBd_CurVPE);
    newval = (env->active_tc.CP0_TCBind & ~mask) | (arg1 & mask);
    env->active_tc.CP0_TCBind = newval;
}

void helper_mttc0_tcbind(CPUMIPSState *env, target_ulong arg1)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    uint32_t mask = (1 << CP0TCBd_TBE);
    uint32_t newval;
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    if (other->mvp->CP0_MVPControl & (1 << CP0MVPCo_VPC))
        mask |= (1 << CP0TCBd_CurVPE);
    if (other_tc == other->current_tc) {
        newval = (other->active_tc.CP0_TCBind & ~mask) | (arg1 & mask);
        other->active_tc.CP0_TCBind = newval;
    } else {
        newval = (other->tcs[other_tc].CP0_TCBind & ~mask) | (arg1 & mask);
        other->tcs[other_tc].CP0_TCBind = newval;
    }
}

void helper_mtc0_tcrestart(CPUMIPSState *env, target_ulong arg1)
{
    env->active_tc.PC = arg1;
    env->active_tc.CP0_TCStatus &= ~(1 << CP0TCSt_TDS);
    env->lladdr = 0ULL;
    /* MIPS16 not implemented. */
}

void helper_mttc0_tcrestart(CPUMIPSState *env, target_ulong arg1)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    if (other_tc == other->current_tc) {
        other->active_tc.PC = arg1;
        other->active_tc.CP0_TCStatus &= ~(1 << CP0TCSt_TDS);
        other->lladdr = 0ULL;
        /* MIPS16 not implemented. */
    } else {
        other->tcs[other_tc].PC = arg1;
        other->tcs[other_tc].CP0_TCStatus &= ~(1 << CP0TCSt_TDS);
        other->lladdr = 0ULL;
        /* MIPS16 not implemented. */
    }
}

void helper_mtc0_tchalt(CPUMIPSState *env, target_ulong arg1)
{
    MIPSCPU *cpu = mips_env_get_cpu(env);

    env->active_tc.CP0_TCHalt = arg1 & 0x1;

    // TODO: Halt TC / Restart (if allocated+active) TC.
    if (env->active_tc.CP0_TCHalt & 1) {
        mips_tc_sleep(cpu, env->current_tc);
    } else {
        mips_tc_wake(cpu, env->current_tc);
    }
}

void helper_mttc0_tchalt(CPUMIPSState *env, target_ulong arg1)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);
    MIPSCPU *other_cpu = mips_env_get_cpu(other);

    // TODO: Halt TC / Restart (if allocated+active) TC.

    if (other_tc == other->current_tc)
        other->active_tc.CP0_TCHalt = arg1;
    else
        other->tcs[other_tc].CP0_TCHalt = arg1;

    if (arg1 & 1) {
        mips_tc_sleep(other_cpu, other_tc);
    } else {
        mips_tc_wake(other_cpu, other_tc);
    }
}

void helper_mtc0_tccontext(CPUMIPSState *env, target_ulong arg1)
{
    env->active_tc.CP0_TCContext = arg1;
}

void helper_mttc0_tccontext(CPUMIPSState *env, target_ulong arg1)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    if (other_tc == other->current_tc)
        other->active_tc.CP0_TCContext = arg1;
    else
        other->tcs[other_tc].CP0_TCContext = arg1;
}

void helper_mtc0_tcschedule(CPUMIPSState *env, target_ulong arg1)
{
    env->active_tc.CP0_TCSchedule = arg1;
}

void helper_mttc0_tcschedule(CPUMIPSState *env, target_ulong arg1)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    if (other_tc == other->current_tc)
        other->active_tc.CP0_TCSchedule = arg1;
    else
        other->tcs[other_tc].CP0_TCSchedule = arg1;
}

void helper_mtc0_tcschefback(CPUMIPSState *env, target_ulong arg1)
{
    env->active_tc.CP0_TCScheFBack = arg1;
}

void helper_mttc0_tcschefback(CPUMIPSState *env, target_ulong arg1)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    if (other_tc == other->current_tc)
        other->active_tc.CP0_TCScheFBack = arg1;
    else
        other->tcs[other_tc].CP0_TCScheFBack = arg1;
}

void helper_mtc0_entrylo1(CPUMIPSState *env, target_ulong arg1)
{
    /* 1k pages not implemented */
    target_ulong rxi = arg1 & (env->CP0_PageGrain & (3u << CP0PG_XIE));
    env->CP0_EntryLo1 = (arg1 & MTC0_ENTRYLO_MASK(env))
#if defined(TARGET_CHERI)
                        | (rxi << (CP0EnLo_L - 30));
#else
                        | (rxi << (CP0EnLo_XI - 30));
#endif /* TARGET_CHERI */
}

#if defined(TARGET_MIPS64)
void helper_dmtc0_entrylo1(CPUMIPSState *env, uint64_t arg1)
{
    uint64_t rxi = arg1 & ((env->CP0_PageGrain & (3ull << CP0PG_XIE)) << 32);
    env->CP0_EntryLo1 = (arg1 & DMTC0_ENTRYLO_MASK(env)) | rxi;
}
#endif

void helper_mtc0_context(CPUMIPSState *env, target_ulong arg1)
{
    env->CP0_Context = (env->CP0_Context & 0x007FFFFF) | (arg1 & ~0x007FFFFF);
}

void helper_mtc0_pagemask(CPUMIPSState *env, target_ulong arg1)
{
    uint64_t mask = arg1 >> (TARGET_PAGE_BITS + 1);
    if (!(env->insn_flags & ISA_MIPS32R6) || (arg1 == ~0) ||
        (mask == 0x0000 || mask == 0x0003 || mask == 0x000F ||
         mask == 0x003F || mask == 0x00FF || mask == 0x03FF ||
         mask == 0x0FFF || mask == 0x3FFF || mask == 0xFFFF)) {
        env->CP0_PageMask = arg1 & (0x1FFFFFFF & (TARGET_PAGE_MASK << 1));
    }
}

void helper_mtc0_pagegrain(CPUMIPSState *env, target_ulong arg1)
{
    /* SmartMIPS not implemented */
    /* 1k pages not implemented */
    env->CP0_PageGrain = (arg1 & env->CP0_PageGrain_rw_bitmask) |
                         (env->CP0_PageGrain & ~env->CP0_PageGrain_rw_bitmask);
    compute_hflags(env);
    restore_pamask(env);
}

void helper_mtc0_segctl0(CPUMIPSState *env, target_ulong arg1)
{
    CPUState *cs = CPU(mips_env_get_cpu(env));

    env->CP0_SegCtl0 = arg1 & CP0SC0_MASK;
    tlb_flush(cs);
}

void helper_mtc0_segctl1(CPUMIPSState *env, target_ulong arg1)
{
    CPUState *cs = CPU(mips_env_get_cpu(env));

    env->CP0_SegCtl1 = arg1 & CP0SC1_MASK;
    tlb_flush(cs);
}

void helper_mtc0_segctl2(CPUMIPSState *env, target_ulong arg1)
{
    CPUState *cs = CPU(mips_env_get_cpu(env));

    env->CP0_SegCtl2 = arg1 & CP0SC2_MASK;
    tlb_flush(cs);
}

void helper_mtc0_wired(CPUMIPSState *env, target_ulong arg1)
{
    if (env->insn_flags & ISA_MIPS32R6) {
        if (arg1 < env->tlb->nb_tlb) {
            env->CP0_Wired = arg1;
        }
    } else {
        env->CP0_Wired = arg1 % env->tlb->nb_tlb;
    }
}

void helper_mtc0_srsconf0(CPUMIPSState *env, target_ulong arg1)
{
    env->CP0_SRSConf0 |= arg1 & env->CP0_SRSConf0_rw_bitmask;
}

void helper_mtc0_srsconf1(CPUMIPSState *env, target_ulong arg1)
{
    env->CP0_SRSConf1 |= arg1 & env->CP0_SRSConf1_rw_bitmask;
}

void helper_mtc0_srsconf2(CPUMIPSState *env, target_ulong arg1)
{
    env->CP0_SRSConf2 |= arg1 & env->CP0_SRSConf2_rw_bitmask;
}

void helper_mtc0_srsconf3(CPUMIPSState *env, target_ulong arg1)
{
    env->CP0_SRSConf3 |= arg1 & env->CP0_SRSConf3_rw_bitmask;
}

void helper_mtc0_srsconf4(CPUMIPSState *env, target_ulong arg1)
{
    env->CP0_SRSConf4 |= arg1 & env->CP0_SRSConf4_rw_bitmask;
}

void helper_mtc0_hwrena(CPUMIPSState *env, target_ulong arg1)
{
    uint32_t mask = 0x0000000F;
#define STATCOUNTERS_HWRENA_MASK 0x7FF0
    if (is_beri_or_cheri(env)) {
        mask |= STATCOUNTERS_HWRENA_MASK;
    }
    if ((env->CP0_Config1 & (1 << CP0C1_PC)) &&
        (env->insn_flags & ISA_MIPS32R6)) {
        mask |= (1 << 4);
    }
    if (env->insn_flags & ISA_MIPS32R6) {
        mask |= (1 << 5);
    }
    if (env->CP0_Config3 & (1 << CP0C3_ULRI)) {
        mask |= (1 << 29);

        if (arg1 & (1 << 29)) {
            env->hflags |= MIPS_HFLAG_HWRENA_ULR;
        } else {
            env->hflags &= ~MIPS_HFLAG_HWRENA_ULR;
        }
    }

    env->CP0_HWREna = arg1 & mask;
}

void helper_mtc0_count(CPUMIPSState *env, target_ulong arg1)
{
    qemu_mutex_lock_iothread();
    cpu_mips_store_count(env, arg1);
    qemu_mutex_unlock_iothread();
}

uint64_t helper_mfc0_rtc64(CPUMIPSState *env)
{
    if (!is_beri_or_cheri(env)) {
        qemu_log_mask(CPU_LOG_INSTR, "Error: Attempted to use BERI RTC64"
                      " register for non-BERI CPU %s\n", env->cpu_model->name);
        do_raise_exception(env, EXCP_RI, GETPC());
    }
    return cpu_mips_get_rtc64(env);
}

void helper_mtc0_rtc64(CPUMIPSState *env, uint64_t arg1)
{
    if (!is_beri_or_cheri(env)) {
        qemu_log_mask(CPU_LOG_INSTR, "Error: Attempted to use BERI RTC64"
                      " register for non-BERI CPU %s\n", env->cpu_model->name);
        do_raise_exception(env, EXCP_RI, GETPC());
    }
    cpu_mips_set_rtc64(env, arg1);
}

/*
 * Return the CHERI/BERI CoreID Register:
 *
 *  31            16 15            0
 *  ----------------+---------------
 * |    MaxCoreID   |    CoreID     |
 *  ----------------+---------------
 *
 * Cores are numbered from 0 to MaxCoreID.
 *
 * See section 7.3.5 of BERI Hardware reference.
 */
target_ulong helper_mfc0_coreid(CPUMIPSState *env)
{
    if (!is_beri_or_cheri(env)) {
        qemu_log_mask(CPU_LOG_INSTR, "Error: Attempted to use BERI CoreID"
                      " register for non-BERI CPU %s\n", env->cpu_model->name);
        do_raise_exception(env, EXCP_RI, GETPC());
    }
    MIPSCPU *cpu = mips_env_get_cpu(env);
    CPUState *cs = CPU(cpu);

    return (uint32_t)(((cs->nr_cores - 1) << 16) |
            (cs->cpu_index & 0xffff));
}

void helper_mtc0_entryhi(CPUMIPSState *env, target_ulong arg1)
{
    target_ulong old, val, mask;
    mask = (TARGET_PAGE_MASK << 1) | env->CP0_EntryHi_ASID_mask;
    if (((env->CP0_Config4 >> CP0C4_IE) & 0x3) >= 2) {
        mask |= 1 << CP0EnHi_EHINV;
    }

    /* 1k pages not implemented */
#if defined(TARGET_MIPS64)
    if (env->insn_flags & ISA_MIPS32R6) {
        int entryhi_r = extract64(arg1, 62, 2);
        int config0_at = extract32(env->CP0_Config0, 13, 2);
        bool no_supervisor = (env->CP0_Status_rw_bitmask & 0x8) == 0;
        if ((entryhi_r == 2) ||
            (entryhi_r == 1 && (no_supervisor || config0_at == 1))) {
            /* skip EntryHi.R field if new value is reserved */
            mask &= ~(0x3ull << 62);
        }
    }
    mask &= env->SEGMask;
#endif
    old = env->CP0_EntryHi;
    val = (arg1 & mask) | (old & ~mask);
    env->CP0_EntryHi = val;
    if (env->CP0_Config3 & (1 << CP0C3_MT)) {
        sync_c0_entryhi(env, env->current_tc);
    }
    /* If the ASID changes, flush qemu's TLB.  */
    if ((old & env->CP0_EntryHi_ASID_mask) !=
        (val & env->CP0_EntryHi_ASID_mask)) {
        tlb_flush(CPU(mips_env_get_cpu(env)));
    }
}

void helper_mttc0_entryhi(CPUMIPSState *env, target_ulong arg1)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    other->CP0_EntryHi = arg1;
    sync_c0_entryhi(other, other_tc);
}

void helper_mtc0_compare(CPUMIPSState *env, target_ulong arg1)
{
    qemu_mutex_lock_iothread();
    cpu_mips_store_compare(env, arg1);
    qemu_mutex_unlock_iothread();
}

void helper_mtc0_status(CPUMIPSState *env, target_ulong arg1)
{
    MIPSCPU *cpu = mips_env_get_cpu(env);
    uint32_t val, old;

    old = env->CP0_Status;
    cpu_mips_store_status(env, arg1);
    val = env->CP0_Status;

    if (qemu_loglevel_mask(CPU_LOG_EXEC)) {
        qemu_log("Status %08x (%08x) => %08x (%08x) Cause %08x",
                old, old & env->CP0_Cause & CP0Ca_IP_mask,
                val, val & env->CP0_Cause & CP0Ca_IP_mask,
                env->CP0_Cause);
        switch (cpu_mmu_index(env, false)) {
        case 3:
            qemu_log(", ERL\n");
            break;
        case MIPS_HFLAG_UM: qemu_log(", UM\n"); break;
        case MIPS_HFLAG_SM: qemu_log(", SM\n"); break;
        case MIPS_HFLAG_KM: qemu_log("\n"); break;
        default:
            cpu_abort(CPU(cpu), "Invalid MMU mode!\n");
            break;
        }
    }
}

void helper_mttc0_status(CPUMIPSState *env, target_ulong arg1)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    uint32_t mask = env->CP0_Status_rw_bitmask & ~0xf1000018;
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    other->CP0_Status = (other->CP0_Status & ~mask) | (arg1 & mask);
    sync_c0_status(env, other, other_tc);
}

void helper_mtc0_intctl(CPUMIPSState *env, target_ulong arg1)
{
    env->CP0_IntCtl = (env->CP0_IntCtl & ~0x000003e0) | (arg1 & 0x000003e0);
}

void helper_mtc0_srsctl(CPUMIPSState *env, target_ulong arg1)
{
    uint32_t mask = (0xf << CP0SRSCtl_ESS) | (0xf << CP0SRSCtl_PSS);
    env->CP0_SRSCtl = (env->CP0_SRSCtl & ~mask) | (arg1 & mask);
}

void helper_mtc0_cause(CPUMIPSState *env, target_ulong arg1)
{
    qemu_mutex_lock_iothread();
    cpu_mips_store_cause(env, arg1);
    qemu_mutex_unlock_iothread();
}

void helper_mttc0_cause(CPUMIPSState *env, target_ulong arg1)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    cpu_mips_store_cause(other, arg1);
}

target_ulong helper_mftc0_epc(CPUMIPSState *env)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    return other->CP0_EPC;
}

target_ulong helper_mftc0_ebase(CPUMIPSState *env)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    return other->CP0_EBase;
}

void helper_mtc0_ebase(CPUMIPSState *env, target_ulong arg1)
{
    target_ulong mask = 0x3FFFF000 | env->CP0_EBaseWG_rw_bitmask;
    if (arg1 & env->CP0_EBaseWG_rw_bitmask) {
        mask |= ~0x3FFFFFFF;
    }
    env->CP0_EBase = (env->CP0_EBase & ~mask) | (arg1 & mask);
}

void helper_mttc0_ebase(CPUMIPSState *env, target_ulong arg1)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);
    target_ulong mask = 0x3FFFF000 | env->CP0_EBaseWG_rw_bitmask;
    if (arg1 & env->CP0_EBaseWG_rw_bitmask) {
        mask |= ~0x3FFFFFFF;
    }
    other->CP0_EBase = (other->CP0_EBase & ~mask) | (arg1 & mask);
}

target_ulong helper_mftc0_configx(CPUMIPSState *env, target_ulong idx)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    switch (idx) {
    case 0: return other->CP0_Config0;
    case 1: return other->CP0_Config1;
    case 2: return other->CP0_Config2;
    case 3: return other->CP0_Config3;
    /* 4 and 5 are reserved.  */
    case 6: return other->CP0_Config6;
    case 7: return other->CP0_Config7;
    default:
        break;
    }
    return 0;
}

void helper_mtc0_config0(CPUMIPSState *env, target_ulong arg1)
{
    env->CP0_Config0 = (env->CP0_Config0 & 0x81FFFFF8) | (arg1 & 0x00000007);
}

void helper_mtc0_config2(CPUMIPSState *env, target_ulong arg1)
{
    /* tertiary/secondary caches not implemented */
    env->CP0_Config2 = (env->CP0_Config2 & 0x8FFF0FFF);
}

void helper_mtc0_config3(CPUMIPSState *env, target_ulong arg1)
{
    if (env->insn_flags & ASE_MICROMIPS) {
        env->CP0_Config3 = (env->CP0_Config3 & ~(1 << CP0C3_ISA_ON_EXC)) |
                           (arg1 & (1 << CP0C3_ISA_ON_EXC));
    }
}

void helper_mtc0_config4(CPUMIPSState *env, target_ulong arg1)
{
    env->CP0_Config4 = (env->CP0_Config4 & (~env->CP0_Config4_rw_bitmask)) |
                       (arg1 & env->CP0_Config4_rw_bitmask);
}

void helper_mtc0_config5(CPUMIPSState *env, target_ulong arg1)
{
    env->CP0_Config5 = (env->CP0_Config5 & (~env->CP0_Config5_rw_bitmask)) |
                       (arg1 & env->CP0_Config5_rw_bitmask);
    compute_hflags(env);
}

void helper_mtc0_lladdr(CPUMIPSState *env, target_ulong arg1)
{
    target_long mask = env->CP0_LLAddr_rw_bitmask;
    arg1 = arg1 << env->CP0_LLAddr_shift;
    env->lladdr = (env->lladdr & ~mask) | (arg1 & mask);
}

#define MTC0_MAAR_MASK(env) \
        ((0x1ULL << 63) | ((env->PAMask >> 4) & ~0xFFFull) | 0x3)

void helper_mtc0_maar(CPUMIPSState *env, target_ulong arg1)
{
    env->CP0_MAAR[env->CP0_MAARI] = arg1 & MTC0_MAAR_MASK(env);
}

void helper_mthc0_maar(CPUMIPSState *env, target_ulong arg1)
{
    env->CP0_MAAR[env->CP0_MAARI] =
        (((uint64_t) arg1 << 32) & MTC0_MAAR_MASK(env)) |
        (env->CP0_MAAR[env->CP0_MAARI] & 0x00000000ffffffffULL);
}

void helper_mtc0_maari(CPUMIPSState *env, target_ulong arg1)
{
    int index = arg1 & 0x3f;
    if (index == 0x3f) {
        /* Software may write all ones to INDEX to determine the
           maximum value supported. */
        env->CP0_MAARI = MIPS_MAAR_MAX - 1;
    } else if (index < MIPS_MAAR_MAX) {
        env->CP0_MAARI = index;
    }
    /* Other than the all ones, if the
       value written is not supported, then INDEX is unchanged
       from its previous value. */
}

void helper_mtc0_watchlo(CPUMIPSState *env, target_ulong arg1, uint32_t sel)
{
    /* Watch exceptions for instructions, data loads, data stores
       not implemented. */
    env->CP0_WatchLo[sel] = (arg1 & ~0x7);
}

void helper_mtc0_watchhi(CPUMIPSState *env, target_ulong arg1, uint32_t sel)
{
    int mask = 0x40000FF8 | (env->CP0_EntryHi_ASID_mask << CP0WH_ASID);
    env->CP0_WatchHi[sel] = arg1 & mask;
    env->CP0_WatchHi[sel] &= ~(env->CP0_WatchHi[sel] & arg1 & 0x7);
}

void helper_mtc0_xcontext(CPUMIPSState *env, target_ulong arg1)
{
    target_ulong mask = (1ULL << (env->SEGBITS - 7)) - 1;
    env->CP0_XContext = (env->CP0_XContext & mask) | (arg1 & ~mask);
}

void helper_mtc0_framemask(CPUMIPSState *env, target_ulong arg1)
{
    env->CP0_Framemask = arg1; /* XXX */
}

void helper_mtc0_debug(CPUMIPSState *env, target_ulong arg1)
{
    env->CP0_Debug = (env->CP0_Debug & 0x8C03FC1F) | (arg1 & 0x13300120);
    if (arg1 & (1 << CP0DB_DM))
        env->hflags |= MIPS_HFLAG_DM;
    else
        env->hflags &= ~MIPS_HFLAG_DM;

    qemu_system_shutdown_request(SHUTDOWN_CAUSE_GUEST_SHUTDOWN);
    qemu_system_powerdown_request();
    // The shutdown request will only be handled once we exit the current
    // translation block so we need to stop the CPU and then exit the current TB
    cpu_stop_current();
    cpu_loop_exit_noexc(ENV_GET_CPU(env));
}

void helper_mttc0_debug(CPUMIPSState *env, target_ulong arg1)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    uint32_t val = arg1 & ((1 << CP0DB_SSt) | (1 << CP0DB_Halt));
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    /* XXX: Might be wrong, check with EJTAG spec. */
    if (other_tc == other->current_tc)
        other->active_tc.CP0_Debug_tcstatus = val;
    else
        other->tcs[other_tc].CP0_Debug_tcstatus = val;
    other->CP0_Debug = (other->CP0_Debug &
                     ((1 << CP0DB_SSt) | (1 << CP0DB_Halt))) |
                     (arg1 & ~((1 << CP0DB_SSt) | (1 << CP0DB_Halt)));
}

void helper_mtc0_performance0(CPUMIPSState *env, target_ulong arg1)
{
    env->CP0_Performance0 = arg1 & 0x000007ff;
}

void helper_mtc0_errctl(CPUMIPSState *env, target_ulong arg1)
{
    int32_t wst = arg1 & (1 << CP0EC_WST);
    int32_t spr = arg1 & (1 << CP0EC_SPR);
    int32_t itc = env->itc_tag ? (arg1 & (1 << CP0EC_ITC)) : 0;

    env->CP0_ErrCtl = wst | spr | itc;

    if (itc && !wst && !spr) {
        env->hflags |= MIPS_HFLAG_ITC_CACHE;
    } else {
        env->hflags &= ~MIPS_HFLAG_ITC_CACHE;
    }
}

void helper_mtc0_taglo(CPUMIPSState *env, target_ulong arg1)
{
    if (env->hflags & MIPS_HFLAG_ITC_CACHE) {
        /* If CACHE instruction is configured for ITC tags then make all
           CP0.TagLo bits writable. The actual write to ITC Configuration
           Tag will take care of the read-only bits. */
        env->CP0_TagLo = arg1;
    } else {
        env->CP0_TagLo = arg1 & 0xFFFFFCF6;
    }
}


#ifdef CONFIG_MIPS_LOG_INSTR

extern int cl_default_trace_format;


#define USER_TRACE_DEBUG 0
#if USER_TRACE_DEBUG
#define user_trace_dbg(...) qemu_log("=== " __VA_ARGS__)
#else
#define user_trace_dbg(...)
#endif

/* Start instruction trace logging. */
void helper_instr_start(CPUMIPSState *env, target_ulong pc)
{
    env->trace_explicitly_disabled = false;
    /* Don't turn on tracing if user-mode only is selected and we are in the kernel */
    if (env->user_only_tracing_enabled && !IN_USERSPACE(env)) {
        assert(qemu_loglevel_mask(CPU_LOG_USER_ONLY));
        user_trace_dbg("Delaying tracing request at 0x%lx "
            "until next switch to user mode, ASID %lu\n",
            pc, env->CP0_EntryHi & 0xFF);
        env->tracing_suspended = true;
    } else {
        qemu_set_log(qemu_loglevel | cl_default_trace_format);
        user_trace_dbg("Switching on tracing @ 0x%lx ASID %lu\n",
            pc, env->CP0_EntryHi & 0xFF);
        env->tracing_suspended = false;
    }
}

/* Stop instruction trace logging. */
void helper_instr_stop(CPUMIPSState *env, target_ulong pc)
{
    user_trace_dbg("Switching off tracing @ 0x%lx ASID %lu\n",
        pc, env->CP0_EntryHi & 0xFF);
    qemu_set_log(qemu_loglevel & ~cl_default_trace_format);
    /* Make sure a kernel -> user switch does not turn on tracing */
    env->tracing_suspended = false;
    /* don't turn on on next kernel -> userspace change */
    env->trace_explicitly_disabled = true;
}

/* Set instruction trace logging to user mode only. */
void helper_instr_start_user_mode_only(CPUMIPSState *env, target_ulong pc)
{
    /*
     * Make sure that qemu_loglevel doesn't get set to zero when we
     * suspend tracing because otherwise qemu will close the logfile.
     */
    qemu_set_log(qemu_loglevel | CPU_LOG_USER_ONLY);
    user_trace_dbg("User-mode only tracing enabled at 0x%lx, ASID %lu\n",
        pc, env->CP0_EntryHi & 0xFF);
    env->user_only_tracing_enabled = true;
    /* Disable tracing if we are not currently in user mode */
    if (!IN_USERSPACE(env)) {
        qemu_set_log(qemu_loglevel & ~cl_default_trace_format);
        env->tracing_suspended = true;
    } else {
        env->tracing_suspended = false;
    }
}

/* Stop instruction trace logging to user mode only. */
void helper_instr_stop_user_mode_only(CPUMIPSState *env, target_ulong pc)
{
    /* Disable user-mode only and restore the previous tracing level */
    if (env->tracing_suspended && !env->trace_explicitly_disabled) {
        user_trace_dbg("User-only trace turned off -> Restoring old trace level"
            " at 0x%lx, ASID %lu\n", pc, env->CP0_EntryHi & 0xFF);
        qemu_set_log(qemu_loglevel | cl_default_trace_format);
    }
    env->tracing_suspended = false;
    env->user_only_tracing_enabled = false;
    user_trace_dbg("User-mode only tracing disabled at 0x%lx, ASID %lu\n",
        pc, env->CP0_EntryHi & 0xFF);
    qemu_set_log(qemu_loglevel & ~CPU_LOG_USER_ONLY);
}

static void do_hexdump(FILE* f, uint8_t* buffer, target_ulong length, target_ulong vaddr) {
    char ascii_chars[17] = { 0 };
    target_ulong line_start = vaddr & ~0xf;
    target_ulong addr;

    /* print leading empty space to always start with an aligned address */
    if (line_start != vaddr) {
        fprintf(f, "    " TARGET_FMT_lx" : ", line_start);
        for (addr = line_start; addr < vaddr; addr++) {
            if ((addr % 4) == 0) {
                fprintf(f, "   ");
            } else {
                fprintf(f, "  ");
            }
            ascii_chars[addr % 16] = ' ';
        }
    }
    ascii_chars[16] = '\0';
    for (addr = vaddr; addr < vaddr + length; addr++) {
        if ((addr % 16) == 0) {
            fprintf(f, "    " TARGET_FMT_lx ": ", line_start);
        }
        if ((addr % 4) == 0) {
            fprintf(f, " ");
        }
        unsigned char c = (unsigned char)buffer[addr - vaddr];
        fprintf(f, "%02x", c);
        ascii_chars[addr % 16] = isprint(c) ? c : '.';
        if ((addr % 16) == 15) {
            fprintf(f, "  %s\r\n", ascii_chars);
            line_start += 16;
        }
    }
    if (line_start != vaddr + length) {
        const target_ulong hexdump_end_addr = (vaddr + length) | 0xf;
        for (addr = vaddr + length; addr <= hexdump_end_addr; addr++) {
            if ((addr % 4) == 0) {
                fprintf(f, "   ");
            } else {
                fprintf(f, "  ");
            }
            ascii_chars[addr % 16] = ' ';
        }
        fprintf(f, "  %s\r\n", ascii_chars);
    }
}


/* Stop instruction trace logging to user mode only. */
void helper_cheri_debug_message(struct CPUMIPSState* env, uint64_t pc)
{
    uint32_t mode = qemu_loglevel & (CPU_LOG_CVTRACE | CPU_LOG_INSTR);
    if (!mode && env->tracing_suspended) {
        /* Always print these messages even if user-space only tracing is on */
        mode = cl_default_trace_format;
    }

    if (!mode && qemu_loglevel_mask(CPU_LOG_GUEST_DEBUG_MSG))
        mode = CPU_LOG_INSTR;

    if (!mode)
        return;

    uint8_t buffer[4096];
    /* Address loaded from a0, length from a1, print mode in a2 */
    typedef enum _PrintMode {
        DEBUG_MESSAGE_CSTRING = 0,
        DEBUG_MESSAGE_HEXDUMP = 1,
        DEBUG_MESSAGE_PTR = 2,
        DEBUG_MESSAGE_DECIMAL= 3
    } PrintMode;
    target_ulong vaddr = env->active_tc.gpr[4];
    target_ulong length = MIN(sizeof(buffer), env->active_tc.gpr[5]);
    PrintMode print_mode = (PrintMode)env->active_tc.gpr[6];

    // For ptr + decimal mode we only need
    if (print_mode == DEBUG_MESSAGE_PTR) {
        if (mode & CPU_LOG_INSTR) {
            qemu_log("   ptr = 0x" TARGET_FMT_lx "\r\n", vaddr);
        }
        return;
    } else if (print_mode == DEBUG_MESSAGE_DECIMAL) {
        if (mode & CPU_LOG_INSTR) {
            qemu_log("   value = " TARGET_FMT_ld "\r\n", vaddr);
        }
        return;
    }
    // Otherwise we meed to fetch the memory referenced by vaddr+length
    int ret = cpu_memory_rw_debug(ENV_GET_CPU(env), vaddr, buffer, sizeof(buffer), false);
    if (ret != 0) {
        warn_report("CHERI DEBUG HELPER: Could not write " TARGET_FMT_ld
                    " bytes at vaddr 0x" TARGET_FMT_lx "\r\n", length, vaddr);
    }
    if ((mode & CPU_LOG_INSTR) || qemu_logfile) {
        qemu_log("DEBUG MESSAGE @ 0x" TARGET_FMT_lx "\r\n", pc);
        if (print_mode == DEBUG_MESSAGE_CSTRING) {
            /* XXXAR: Escape newlines, etc.? */
            qemu_log("    message = \"%s\"\n", buffer);
        } else if (print_mode == DEBUG_MESSAGE_HEXDUMP) {
            qemu_log("   Dumping " TARGET_FMT_lu " bytes starting at 0x"
                     TARGET_FMT_lx "\r\n", length, vaddr);
            do_hexdump(qemu_logfile, buffer, length, vaddr);
        }
    } else if (mode & CPU_LOG_CVTRACE) {
        warn_report("NOT IMPLEMENTED: CVTRACE debug message nop at 0x"
                    TARGET_FMT_lx "\n", pc);
    } else {
        assert(false && "logic error");
    }
}


static inline void cvtrace_dump_gpr(cvtrace_t *cvtrace, uint64_t value)
{
    if (qemu_loglevel_mask(CPU_LOG_CVTRACE)) {
        if (cvtrace->version == CVT_NO_REG)
            cvtrace->version = CVT_GPR;
        cvtrace->val2 = tswap64(value);
    }
}

#endif // CONFIG_MIPS_LOG_INSTR

static void simple_dump_state(CPUMIPSState *env, FILE *f,
        fprintf_function cpu_fprintf)
{

/* gxemul compat:
    cpu_fprintf(f, "pc = 0x" TARGET_FMT_lx "\n", env->active_tc.PC);
    cpu_fprintf(f, "hi = 0x" TARGET_FMT_lx "    lo = 0x" TARGET_FMT_lx "\n",
            env->active_tc.HI[0], env->active_tc.LO[0]);
    cpu_fprintf(f, "                       ""    s0 = 0x" TARGET_FMT_lx "\n",
            env->active_tc.gpr[16]);
    cpu_fprintf(f, "at = 0x" TARGET_FMT_lx "    s1 = 0x" TARGET_FMT_lx "\n",
            env->active_tc.gpr[1], env->active_tc.gpr[17]);
    cpu_fprintf(f, "v0 = 0x" TARGET_FMT_lx "    s2 = 0x" TARGET_FMT_lx "\n",
            env->active_tc.gpr[2], env->active_tc.gpr[18]);
    cpu_fprintf(f, "v1 = 0x" TARGET_FMT_lx "    s3 = 0x" TARGET_FMT_lx "\n",
            env->active_tc.gpr[3], env->active_tc.gpr[19]);
    cpu_fprintf(f, "a0 = 0x" TARGET_FMT_lx "    s4 = 0x" TARGET_FMT_lx "\n",
            env->active_tc.gpr[4], env->active_tc.gpr[20]);
    cpu_fprintf(f, "a1 = 0x" TARGET_FMT_lx "    s5 = 0x" TARGET_FMT_lx "\n",
            env->active_tc.gpr[5], env->active_tc.gpr[21]);
    cpu_fprintf(f, "a2 = 0x" TARGET_FMT_lx "    s6 = 0x" TARGET_FMT_lx "\n",
            env->active_tc.gpr[6], env->active_tc.gpr[22]);
    cpu_fprintf(f, "a3 = 0x" TARGET_FMT_lx "    s7 = 0x" TARGET_FMT_lx "\n",
            env->active_tc.gpr[7], env->active_tc.gpr[23]);
    cpu_fprintf(f, "t0 = 0x" TARGET_FMT_lx "    t8 = 0x" TARGET_FMT_lx "\n",
            env->active_tc.gpr[8], env->active_tc.gpr[24]);
    cpu_fprintf(f, "t1 = 0x" TARGET_FMT_lx "    t9 = 0x" TARGET_FMT_lx "\n",
            env->active_tc.gpr[9], env->active_tc.gpr[25]);
    cpu_fprintf(f, "t2 = 0x" TARGET_FMT_lx "    k0 = 0x" TARGET_FMT_lx "\n",
            env->active_tc.gpr[10], env->active_tc.gpr[26]);
    cpu_fprintf(f, "t3 = 0x" TARGET_FMT_lx "    k1 = 0x" TARGET_FMT_lx "\n",
            env->active_tc.gpr[11], env->active_tc.gpr[27]);
    cpu_fprintf(f, "t4 = 0x" TARGET_FMT_lx "    gp = 0x" TARGET_FMT_lx "\n",
            env->active_tc.gpr[12], env->active_tc.gpr[28]);
    cpu_fprintf(f, "t5 = 0x" TARGET_FMT_lx "    sp = 0x" TARGET_FMT_lx "\n",
            env->active_tc.gpr[13], env->active_tc.gpr[29]);
    cpu_fprintf(f, "t6 = 0x" TARGET_FMT_lx "    fp = 0x" TARGET_FMT_lx "\n",
            env->active_tc.gpr[14], env->active_tc.gpr[30]);
    cpu_fprintf(f, "t7 = 0x" TARGET_FMT_lx "    ra = 0x" TARGET_FMT_lx "\n",
            env->active_tc.gpr[15], env->active_tc.gpr[31]);
*/

    /* sim compatible register dump: */
    cpu_fprintf(f, "DEBUG MIPS COREID 0\n");
    cpu_fprintf(f, "DEBUG MIPS PC 0x" TARGET_FMT_lx "\n", env->active_tc.PC);
    cpu_fprintf(f, "DEBUG MIPS REG 00 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[0]);
    cpu_fprintf(f, "DEBUG MIPS REG 01 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[1]);
    cpu_fprintf(f, "DEBUG MIPS REG 02 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[2]);
    cpu_fprintf(f, "DEBUG MIPS REG 03 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[3]);
    cpu_fprintf(f, "DEBUG MIPS REG 04 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[4]);
    cpu_fprintf(f, "DEBUG MIPS REG 05 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[5]);
    cpu_fprintf(f, "DEBUG MIPS REG 06 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[6]);
    cpu_fprintf(f, "DEBUG MIPS REG 07 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[7]);
    cpu_fprintf(f, "DEBUG MIPS REG 08 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[8]);
    cpu_fprintf(f, "DEBUG MIPS REG 09 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[9]);
    cpu_fprintf(f, "DEBUG MIPS REG 10 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[10]);
    cpu_fprintf(f, "DEBUG MIPS REG 11 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[11]);
    cpu_fprintf(f, "DEBUG MIPS REG 12 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[12]);
    cpu_fprintf(f, "DEBUG MIPS REG 13 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[13]);
    cpu_fprintf(f, "DEBUG MIPS REG 14 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[14]);
    cpu_fprintf(f, "DEBUG MIPS REG 15 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[15]);
    cpu_fprintf(f, "DEBUG MIPS REG 16 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[16]);
    cpu_fprintf(f, "DEBUG MIPS REG 17 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[17]);
    cpu_fprintf(f, "DEBUG MIPS REG 18 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[18]);
    cpu_fprintf(f, "DEBUG MIPS REG 19 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[19]);
    cpu_fprintf(f, "DEBUG MIPS REG 20 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[20]);
    cpu_fprintf(f, "DEBUG MIPS REG 21 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[21]);
    cpu_fprintf(f, "DEBUG MIPS REG 22 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[22]);
    cpu_fprintf(f, "DEBUG MIPS REG 23 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[23]);
    cpu_fprintf(f, "DEBUG MIPS REG 24 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[24]);
    cpu_fprintf(f, "DEBUG MIPS REG 25 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[25]);
    cpu_fprintf(f, "DEBUG MIPS REG 26 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[26]);
    cpu_fprintf(f, "DEBUG MIPS REG 27 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[27]);
    cpu_fprintf(f, "DEBUG MIPS REG 28 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[28]);
    cpu_fprintf(f, "DEBUG MIPS REG 29 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[29]);
    cpu_fprintf(f, "DEBUG MIPS REG 30 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[30]);
    cpu_fprintf(f, "DEBUG MIPS REG 31 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[31]);

}

void helper_mtc0_dumpstate(CPUMIPSState *env, target_ulong arg1)
{
#if 0
    cpu_dump_state(CPU(mips_env_get_cpu(env)),
            (qemu_logfile == NULL) ? stderr : qemu_logfile,
            fprintf, CPU_DUMP_CODE);
#else
    simple_dump_state(env, (qemu_logfile == NULL) ? stderr : qemu_logfile,
            fprintf);
#endif
}



void helper_mtc0_datalo(CPUMIPSState *env, target_ulong arg1)
{
    env->CP0_DataLo = arg1; /* XXX */
}

void helper_mtc0_taghi(CPUMIPSState *env, target_ulong arg1)
{
    env->CP0_TagHi = arg1; /* XXX */
}

void helper_mtc0_datahi(CPUMIPSState *env, target_ulong arg1)
{
    env->CP0_DataHi = arg1; /* XXX */
}

/* MIPS MT functions */
target_ulong helper_mftgpr(CPUMIPSState *env, uint32_t sel)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    if (other_tc == other->current_tc)
        return other->active_tc.gpr[sel];
    else
        return other->tcs[other_tc].gpr[sel];
}

target_ulong helper_mftlo(CPUMIPSState *env, uint32_t sel)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    if (other_tc == other->current_tc)
        return other->active_tc.LO[sel];
    else
        return other->tcs[other_tc].LO[sel];
}

target_ulong helper_mfthi(CPUMIPSState *env, uint32_t sel)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    if (other_tc == other->current_tc)
        return other->active_tc.HI[sel];
    else
        return other->tcs[other_tc].HI[sel];
}

target_ulong helper_mftacx(CPUMIPSState *env, uint32_t sel)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    if (other_tc == other->current_tc)
        return other->active_tc.ACX[sel];
    else
        return other->tcs[other_tc].ACX[sel];
}

target_ulong helper_mftdsp(CPUMIPSState *env)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    if (other_tc == other->current_tc)
        return other->active_tc.DSPControl;
    else
        return other->tcs[other_tc].DSPControl;
}

void helper_mttgpr(CPUMIPSState *env, target_ulong arg1, uint32_t sel)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    if (other_tc == other->current_tc)
        other->active_tc.gpr[sel] = arg1;
    else
        other->tcs[other_tc].gpr[sel] = arg1;
}

void helper_mttlo(CPUMIPSState *env, target_ulong arg1, uint32_t sel)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    if (other_tc == other->current_tc)
        other->active_tc.LO[sel] = arg1;
    else
        other->tcs[other_tc].LO[sel] = arg1;
}

void helper_mtthi(CPUMIPSState *env, target_ulong arg1, uint32_t sel)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    if (other_tc == other->current_tc)
        other->active_tc.HI[sel] = arg1;
    else
        other->tcs[other_tc].HI[sel] = arg1;
}

void helper_mttacx(CPUMIPSState *env, target_ulong arg1, uint32_t sel)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    if (other_tc == other->current_tc)
        other->active_tc.ACX[sel] = arg1;
    else
        other->tcs[other_tc].ACX[sel] = arg1;
}

void helper_mttdsp(CPUMIPSState *env, target_ulong arg1)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    if (other_tc == other->current_tc)
        other->active_tc.DSPControl = arg1;
    else
        other->tcs[other_tc].DSPControl = arg1;
}

/* MIPS MT functions */
target_ulong helper_dmt(void)
{
    // TODO
     return 0;
}

target_ulong helper_emt(void)
{
    // TODO
    return 0;
}

target_ulong helper_dvpe(CPUMIPSState *env)
{
    CPUState *other_cs = first_cpu;
    target_ulong prev = env->mvp->CP0_MVPControl;

    CPU_FOREACH(other_cs) {
        MIPSCPU *other_cpu = MIPS_CPU(other_cs);
        /* Turn off all VPEs except the one executing the dvpe.  */
        if (&other_cpu->env != env) {
            other_cpu->env.mvp->CP0_MVPControl &= ~(1 << CP0MVPCo_EVP);
            mips_vpe_sleep(other_cpu);
        }
    }
    return prev;
}

target_ulong helper_evpe(CPUMIPSState *env)
{
    CPUState *other_cs = first_cpu;
    target_ulong prev = env->mvp->CP0_MVPControl;

    CPU_FOREACH(other_cs) {
        MIPSCPU *other_cpu = MIPS_CPU(other_cs);

        if (&other_cpu->env != env
            /* If the VPE is WFI, don't disturb its sleep.  */
            && !mips_vpe_is_wfi(other_cpu)) {
            /* Enable the VPE.  */
            other_cpu->env.mvp->CP0_MVPControl |= (1 << CP0MVPCo_EVP);
            mips_vpe_wake(other_cpu); /* And wake it up.  */
        }
    }
    return prev;
}
#endif /* !CONFIG_USER_ONLY */

void helper_fork(target_ulong arg1, target_ulong arg2)
{
    // arg1 = rt, arg2 = rs
    // TODO: store to TC register
}

target_ulong helper_yield(CPUMIPSState *env, target_ulong arg)
{
    target_long arg1 = arg;

    if (arg1 < 0) {
        /* No scheduling policy implemented. */
        if (arg1 != -2) {
            if (env->CP0_VPEControl & (1 << CP0VPECo_YSI) &&
                env->active_tc.CP0_TCStatus & (1 << CP0TCSt_DT)) {
                env->CP0_VPEControl &= ~(0x7 << CP0VPECo_EXCPT);
                env->CP0_VPEControl |= 4 << CP0VPECo_EXCPT;
                do_raise_exception(env, EXCP_THREAD, GETPC());
            }
        }
    } else if (arg1 == 0) {
        if (0 /* TODO: TC underflow */) {
            env->CP0_VPEControl &= ~(0x7 << CP0VPECo_EXCPT);
            do_raise_exception(env, EXCP_THREAD, GETPC());
        } else {
            // TODO: Deallocate TC
        }
    } else if (arg1 > 0) {
        /* Yield qualifier inputs not implemented. */
        env->CP0_VPEControl &= ~(0x7 << CP0VPECo_EXCPT);
        env->CP0_VPEControl |= 2 << CP0VPECo_EXCPT;
        do_raise_exception(env, EXCP_THREAD, GETPC());
    }
    return env->CP0_YQMask;
}

/* R6 Multi-threading */
#ifndef CONFIG_USER_ONLY
target_ulong helper_dvp(CPUMIPSState *env)
{
    CPUState *other_cs = first_cpu;
    target_ulong prev = env->CP0_VPControl;

    if (!((env->CP0_VPControl >> CP0VPCtl_DIS) & 1)) {
        CPU_FOREACH(other_cs) {
            MIPSCPU *other_cpu = MIPS_CPU(other_cs);
            /* Turn off all VPs except the one executing the dvp. */
            if (&other_cpu->env != env) {
                mips_vpe_sleep(other_cpu);
            }
        }
        env->CP0_VPControl |= (1 << CP0VPCtl_DIS);
    }
    return prev;
}

target_ulong helper_evp(CPUMIPSState *env)
{
    CPUState *other_cs = first_cpu;
    target_ulong prev = env->CP0_VPControl;

    if ((env->CP0_VPControl >> CP0VPCtl_DIS) & 1) {
        CPU_FOREACH(other_cs) {
            MIPSCPU *other_cpu = MIPS_CPU(other_cs);
            if ((&other_cpu->env != env) && !mips_vp_is_wfi(other_cpu)) {
                /* If the VP is WFI, don't disturb its sleep.
                 * Otherwise, wake it up. */
                mips_vpe_wake(other_cpu);
            }
        }
        env->CP0_VPControl &= ~(1 << CP0VPCtl_DIS);
    }
    return prev;
}
#endif /* !CONFIG_USER_ONLY */

#ifndef CONFIG_USER_ONLY
/* TLB management */
static void r4k_mips_tlb_flush_extra (CPUMIPSState *env, int first)
{
    /* Discard entries from env->tlb[first] onwards.  */
    while (env->tlb->tlb_in_use > first) {
        r4k_invalidate_tlb(env, --env->tlb->tlb_in_use, 0);
    }
}

static inline uint64_t get_tlb_pfn_from_entrylo(uint64_t entrylo)
{
#if defined(TARGET_MIPS64)
    return extract64(entrylo, 6, 54);
#else
    return extract64(entrylo, 6, 24) | /* PFN */
           (extract64(entrylo, 32, 32) << 24); /* PFNX */
#endif
}

static void r4k_fill_tlb(CPUMIPSState *env, int idx)
{
    r4k_tlb_t *tlb;
    uint64_t mask = env->CP0_PageMask >> (TARGET_PAGE_BITS + 1);

    /* XXX: detect conflicting TLBs and raise a MCHECK exception when needed */
    tlb = &env->tlb->mmu.r4k.tlb[idx];
    if (env->CP0_EntryHi & (1 << CP0EnHi_EHINV)) {
        tlb->EHINV = 1;
        return;
    }
    tlb->EHINV = 0;
    tlb->VPN = env->CP0_EntryHi & (TARGET_PAGE_MASK << 1);
#if defined(TARGET_MIPS64)
    tlb->VPN &= env->SEGMask;
#endif
    tlb->ASID = env->CP0_EntryHi & env->CP0_EntryHi_ASID_mask;
    tlb->PageMask = env->CP0_PageMask;
    tlb->G = env->CP0_EntryLo0 & env->CP0_EntryLo1 & 1;
    tlb->V0 = (env->CP0_EntryLo0 & 2) != 0;
    tlb->D0 = (env->CP0_EntryLo0 & 4) != 0;
    tlb->C0 = (env->CP0_EntryLo0 >> 3) & 0x7;
#if defined(TARGET_CHERI)
    tlb->L0 = (env->CP0_EntryLo0 >> CP0EnLo_L) & 1;
    tlb->S0 = (env->CP0_EntryLo0 >> CP0EnLo_S) & 1;
#else
    tlb->XI0 = (env->CP0_EntryLo0 >> CP0EnLo_XI) & 1;
    tlb->RI0 = (env->CP0_EntryLo0 >> CP0EnLo_RI) & 1;
#endif /* TARGET_CHERI */
    tlb->PFN[0] = (get_tlb_pfn_from_entrylo(env->CP0_EntryLo0) & ~mask) << 12;
    tlb->V1 = (env->CP0_EntryLo1 & 2) != 0;
    tlb->D1 = (env->CP0_EntryLo1 & 4) != 0;
    tlb->C1 = (env->CP0_EntryLo1 >> 3) & 0x7;
#if defined(TARGET_CHERI)
    tlb->L1 = (env->CP0_EntryLo1 >> CP0EnLo_L) & 1;
    tlb->S1 = (env->CP0_EntryLo1 >> CP0EnLo_S) & 1;
#else
    tlb->XI1 = (env->CP0_EntryLo1 >> CP0EnLo_XI) & 1;
    tlb->RI1 = (env->CP0_EntryLo1 >> CP0EnLo_RI) & 1;
#endif /* TARGET_CHERI */
    tlb->PFN[1] = (get_tlb_pfn_from_entrylo(env->CP0_EntryLo1) & ~mask) << 12;
}

#ifdef TARGET_CHERI
static void r4k_dump_tlb(CPUMIPSState *env, int idx)
{
    r4k_tlb_t *tlb = &env->tlb->mmu.r4k.tlb[idx];
    unsigned pagemask, hi, lo0, lo1;

    if (tlb->EHINV) {
        pagemask = 0;
        hi  = 1 << CP0EnHi_EHINV;
        lo0 = 0;
        lo1 = 0;
    } else {
        pagemask = tlb->PageMask;
        hi = tlb->VPN | tlb->ASID;
        lo0 = tlb->G | (tlb->V0 << 1) | (tlb->D0 << 2) |
#if 1
            ((target_ulong)tlb->S0 << CP0EnLo_S) |
            ((target_ulong)tlb->L0 << CP0EnLo_L) |
#else
            ((target_ulong)tlb->RI0 << CP0EnLo_RI) |
            ((target_ulong)tlb->XI0 << CP0EnLo_XI) |
#endif
            (tlb->C0 << 3) | (tlb->PFN[0] >> 6);
        lo1 = tlb->G | (tlb->V1 << 1) | (tlb->D1 << 2) |
#if 1
            ((target_ulong)tlb->S1 << CP0EnLo_S) |
            ((target_ulong)tlb->L1 << CP0EnLo_L) |
#else
            ((target_ulong)tlb->RI1 << CP0EnLo_RI) |
            ((target_ulong)tlb->XI1 << CP0EnLo_XI) |
#endif
            (tlb->C1 << 3) | (tlb->PFN[1] >> 6);
    }
    fprintf(qemu_logfile, "    Write TLB[%u] = pgmsk:%08x hi:%08x lo0:%08x lo1:%08x\n",
            idx, pagemask, hi, lo0, lo1);
}
#endif /* TARGET_CHERI */

void r4k_helper_tlbinv(CPUMIPSState *env)
{
    int idx;
    r4k_tlb_t *tlb;
    uint16_t ASID = env->CP0_EntryHi & env->CP0_EntryHi_ASID_mask;

    for (idx = 0; idx < env->tlb->nb_tlb; idx++) {
        tlb = &env->tlb->mmu.r4k.tlb[idx];
        if (!tlb->G && tlb->ASID == ASID) {
            tlb->EHINV = 1;
        }
    }
    cpu_mips_tlb_flush(env);
}

void r4k_helper_tlbinvf(CPUMIPSState *env)
{
    int idx;

    for (idx = 0; idx < env->tlb->nb_tlb; idx++) {
        env->tlb->mmu.r4k.tlb[idx].EHINV = 1;
    }
    cpu_mips_tlb_flush(env);
}

void r4k_helper_tlbwi(CPUMIPSState *env)
{
    r4k_tlb_t *tlb;
    int idx;
    target_ulong VPN;
    uint16_t ASID;
    bool EHINV, G, V0, D0, V1, D1;
#if defined(TARGET_CHERI)
    bool S0, S1, L0, L1;
#else
    bool XI0, XI1, RI0, RI1;
#endif
    idx = (env->CP0_Index & ~0x80000000) % env->tlb->nb_tlb;
    tlb = &env->tlb->mmu.r4k.tlb[idx];
    VPN = env->CP0_EntryHi & (TARGET_PAGE_MASK << 1);
#if defined(TARGET_MIPS64)
    VPN &= env->SEGMask;
#endif
    ASID = env->CP0_EntryHi & env->CP0_EntryHi_ASID_mask;
    EHINV = (env->CP0_EntryHi & (1 << CP0EnHi_EHINV)) != 0;
    G = env->CP0_EntryLo0 & env->CP0_EntryLo1 & 1;
    V0 = (env->CP0_EntryLo0 & 2) != 0;
    D0 = (env->CP0_EntryLo0 & 4) != 0;
#if defined(TARGET_CHERI)
    S0 = (env->CP0_EntryLo0 >> CP0EnLo_S) &1;
    L0 = (env->CP0_EntryLo0 >> CP0EnLo_L) &1;
#else
    XI0 = (env->CP0_EntryLo0 >> CP0EnLo_XI) &1;
    RI0 = (env->CP0_EntryLo0 >> CP0EnLo_RI) &1;
#endif
    V1 = (env->CP0_EntryLo1 & 2) != 0;
    D1 = (env->CP0_EntryLo1 & 4) != 0;
#if defined(TARGET_CHERI)
    S1 = (env->CP0_EntryLo1 >> CP0EnLo_S) &1;
    L1 = (env->CP0_EntryLo1 >> CP0EnLo_L) &1;
#else
    XI1 = (env->CP0_EntryLo1 >> CP0EnLo_XI) &1;
    RI1 = (env->CP0_EntryLo1 >> CP0EnLo_RI) &1;
#endif

    /* Discard cached TLB entries, unless tlbwi is just upgrading access
       permissions on the current entry. */
    if (tlb->VPN != VPN || tlb->ASID != ASID || tlb->G != G ||
        (!tlb->EHINV && EHINV) ||
        (tlb->V0 && !V0) || (tlb->D0 && !D0) ||
#if defined(TARGET_CHERI)
        (!tlb->S0 && S0) || (!tlb->L0 && L0) ||
#else
        (!tlb->XI0 && XI0) || (!tlb->RI0 && RI0) ||
#endif
        (tlb->V1 && !V1) || (tlb->D1 && !D1) ||
#if defined(TARGET_CHERI)
        (!tlb->S1 && S1) || (!tlb->L1 && L1)) {
#else
        (!tlb->XI1 && XI1) || (!tlb->RI1 && RI1)) {
#endif
        r4k_mips_tlb_flush_extra(env, env->tlb->nb_tlb);
    }

    r4k_invalidate_tlb(env, idx, 0);
    r4k_fill_tlb(env, idx);
#ifdef TARGET_CHERI
    if (qemu_loglevel_mask(CPU_LOG_INSTR))
        r4k_dump_tlb(env, idx);
#endif /* TARGET_CHERI */
}

void r4k_helper_tlbwr(CPUMIPSState *env)
{
    int r = cpu_mips_get_random(env);

    r4k_invalidate_tlb(env, r, 1);
    r4k_fill_tlb(env, r);
#ifdef TARGET_CHERI
    if (qemu_loglevel_mask(CPU_LOG_INSTR))
        r4k_dump_tlb(env, r);
#endif /* TARGET_CHERI */
}

void r4k_helper_tlbp(CPUMIPSState *env)
{
    r4k_tlb_t *tlb;
    target_ulong mask;
    target_ulong tag;
    target_ulong VPN;
    uint16_t ASID;
    int i;

    ASID = env->CP0_EntryHi & env->CP0_EntryHi_ASID_mask;
    for (i = 0; i < env->tlb->nb_tlb; i++) {
        tlb = &env->tlb->mmu.r4k.tlb[i];
        /* 1k pages are not supported. */
        mask = tlb->PageMask | ~(TARGET_PAGE_MASK << 1);
        tag = env->CP0_EntryHi & ~mask;
        VPN = tlb->VPN & ~mask;
#if defined(TARGET_MIPS64)
        tag &= env->SEGMask;
#endif
        /* Check ASID, virtual page number & size */
        if ((tlb->G == 1 || tlb->ASID == ASID) && VPN == tag && !tlb->EHINV) {
            /* TLB match */
            env->CP0_Index = i;
            break;
        }
    }
    if (i == env->tlb->nb_tlb) {
        /* No match.  Discard any shadow entries, if any of them match.  */
        for (i = env->tlb->nb_tlb; i < env->tlb->tlb_in_use; i++) {
            tlb = &env->tlb->mmu.r4k.tlb[i];
            /* 1k pages are not supported. */
            mask = tlb->PageMask | ~(TARGET_PAGE_MASK << 1);
            tag = env->CP0_EntryHi & ~mask;
            VPN = tlb->VPN & ~mask;
#if defined(TARGET_MIPS64)
            tag &= env->SEGMask;
#endif
            /* Check ASID, virtual page number & size */
            if ((tlb->G == 1 || tlb->ASID == ASID) && VPN == tag) {
                r4k_mips_tlb_flush_extra (env, i);
                break;
            }
        }

        env->CP0_Index |= 0x80000000;
    }
}

static inline uint64_t get_entrylo_pfn_from_tlb(uint64_t tlb_pfn)
{
#if defined(TARGET_MIPS64)
    return tlb_pfn << 6;
#else
    return (extract64(tlb_pfn, 0, 24) << 6) | /* PFN */
           (extract64(tlb_pfn, 24, 32) << 32); /* PFNX */
#endif
}

void r4k_helper_tlbr(CPUMIPSState *env)
{
    r4k_tlb_t *tlb;
    uint16_t ASID;
    int idx;

    ASID = env->CP0_EntryHi & env->CP0_EntryHi_ASID_mask;
    idx = (env->CP0_Index & ~0x80000000) % env->tlb->nb_tlb;
    tlb = &env->tlb->mmu.r4k.tlb[idx];

    /* If this will change the current ASID, flush qemu's TLB.  */
    if (ASID != tlb->ASID)
        cpu_mips_tlb_flush(env);

    r4k_mips_tlb_flush_extra(env, env->tlb->nb_tlb);

    if (tlb->EHINV) {
        env->CP0_EntryHi = 1 << CP0EnHi_EHINV;
        env->CP0_PageMask = 0;
        env->CP0_EntryLo0 = 0;
        env->CP0_EntryLo1 = 0;
    } else {
        env->CP0_EntryHi = tlb->VPN | tlb->ASID;
        env->CP0_PageMask = tlb->PageMask;
        env->CP0_EntryLo0 = tlb->G | (tlb->V0 << 1) | (tlb->D0 << 2) |
#if defined(TARGET_CHERI)
                        ((uint64_t)tlb->L0 << CP0EnLo_L) |
                        ((uint64_t)tlb->S0 << CP0EnLo_S) | (tlb->C0 << 3) |
#else
                        ((uint64_t)tlb->RI0 << CP0EnLo_RI) |
                        ((uint64_t)tlb->XI0 << CP0EnLo_XI) | (tlb->C0 << 3) |
#endif /* TARGET_CHERI */
                        get_entrylo_pfn_from_tlb(tlb->PFN[0] >> 12);
        env->CP0_EntryLo1 = tlb->G | (tlb->V1 << 1) | (tlb->D1 << 2) |
#if defined(TARGET_CHERI)
                        ((uint64_t)tlb->L1 << CP0EnLo_L) |
                        ((uint64_t)tlb->S1 << CP0EnLo_S) | (tlb->C0 << 3) |
#else
                        ((uint64_t)tlb->RI1 << CP0EnLo_RI) |
                        ((uint64_t)tlb->XI1 << CP0EnLo_XI) | (tlb->C1 << 3) |
#endif /* TARGET_CHERI */
                        get_entrylo_pfn_from_tlb(tlb->PFN[1] >> 12);
    }
}

void helper_tlbwi(CPUMIPSState *env)
{
    env->tlb->helper_tlbwi(env);
}

void helper_tlbwr(CPUMIPSState *env)
{
    env->tlb->helper_tlbwr(env);
}

void helper_tlbp(CPUMIPSState *env)
{
    env->tlb->helper_tlbp(env);
}

void helper_tlbr(CPUMIPSState *env)
{
    env->tlb->helper_tlbr(env);
}

void helper_tlbinv(CPUMIPSState *env)
{
    env->tlb->helper_tlbinv(env);
}

void helper_tlbinvf(CPUMIPSState *env)
{
    env->tlb->helper_tlbinvf(env);
}

/* Specials */
target_ulong helper_di(CPUMIPSState *env)
{
    target_ulong t0 = env->CP0_Status;

    env->CP0_Status = t0 & ~(1 << CP0St_IE);
    return t0;
}

target_ulong helper_ei(CPUMIPSState *env)
{
    target_ulong t0 = env->CP0_Status;

    env->CP0_Status = t0 | (1 << CP0St_IE);
    return t0;
}

static void debug_pre_eret(CPUMIPSState *env)
{
    if (qemu_loglevel_mask(CPU_LOG_EXEC | CPU_LOG_INSTR)) {
        qemu_log("ERET: PC " TARGET_FMT_lx " EPC " TARGET_FMT_lx,
                env->active_tc.PC, env->CP0_EPC);
        if (should_use_error_epc(env))
            qemu_log(" ErrorEPC " TARGET_FMT_lx, env->CP0_ErrorEPC);
        if (env->hflags & MIPS_HFLAG_DM)
            qemu_log(" DEPC " TARGET_FMT_lx, env->CP0_DEPC);
        qemu_log("\n");
    }
}

static void debug_post_eret(CPUMIPSState *env)
{
    MIPSCPU *cpu = mips_env_get_cpu(env);

    if (qemu_loglevel_mask(CPU_LOG_EXEC | CPU_LOG_INSTR)) {
        qemu_log("  =>  PC " TARGET_FMT_lx " EPC " TARGET_FMT_lx,
                env->active_tc.PC, env->CP0_EPC);
        if (should_use_error_epc(env))
            qemu_log(" ErrorEPC " TARGET_FMT_lx, env->CP0_ErrorEPC);
        if (env->hflags & MIPS_HFLAG_DM)
            qemu_log(" DEPC " TARGET_FMT_lx, env->CP0_DEPC);
        switch (cpu_mmu_index(env, false)) {
        case 3:
            qemu_log(", ERL\n");
            break;
        case MIPS_HFLAG_UM: qemu_log(", UM\n"); break;
        case MIPS_HFLAG_SM: qemu_log(", SM\n"); break;
        case MIPS_HFLAG_KM: qemu_log("\n"); break;
        default:
            cpu_abort(CPU(cpu), "Invalid MMU mode!\n");
            break;
        }
    }
}

static void set_pc(CPUMIPSState *env, target_ulong error_pc)
{
    env->active_tc.PC = error_pc & ~(target_ulong)1;
#ifdef TARGET_CHERI
    env->active_tc.PC += env->active_tc.PCC.cr_base;
    env->active_tc.PCC.cr_offset = error_pc;
#endif
    if (error_pc & 1) {
        env->hflags |= MIPS_HFLAG_M16;
    } else {
        env->hflags &= ~(MIPS_HFLAG_M16);
    }
}

static inline void exception_return(CPUMIPSState *env)
{
    debug_pre_eret(env);
#ifdef TARGET_CHERI
    // qemu_log_mask(CPU_LOG_INSTR, "%s: PCC <- EPCC\n", __func__);
#ifdef CONFIG_MIPS_LOG_INSTR
    if (unlikely(qemu_loglevel_mask(CPU_LOG_INSTR))) {
         // Print the new PCC value for debugging traces (compare to null
         // so that we always print it)
         cap_register_t null_cap;
         null_capability(&null_cap);
         dump_changed_capreg(env, &env->active_tc.PCC, &null_cap, "PCC");
         null_capability(&null_cap);
         dump_changed_capreg(env, &env->active_tc.CHWR.EPCC, &null_cap, "EPCC");
    }
#endif // CONFIG_MIPS_LOG_INSTR
    tcg_debug_assert(env->active_tc.CHWR.EPCC.cr_offset == CP2CAP_EPCC_FAKE_OFFSET_VALUE);
    env->active_tc.PCC = env->active_tc.CHWR.EPCC;
#endif /* TARGET_CHERI */
    if (env->CP0_Status & (1 << CP0St_ERL)) {
        set_pc(env, env->CP0_ErrorEPC);
        env->CP0_Status &= ~(1 << CP0St_ERL);
    } else {
        set_pc(env, env->CP0_EPC);
        env->CP0_Status &= ~(1 << CP0St_EXL);
    }
    cheri_debug_assert(env->active_tc.PCC.cr_offset != CP2CAP_EPCC_FAKE_OFFSET_VALUE);
    compute_hflags(env);
    debug_post_eret(env);
}

void helper_eret(CPUMIPSState *env)
{
    exception_return(env);
    env->lladdr = 1;
#ifdef TARGET_CHERI
    env->linkedflag = 0;
#endif /* TARGET_CHERI */
}

void helper_eretnc(CPUMIPSState *env)
{
    exception_return(env);
}

void helper_deret(CPUMIPSState *env)
{
    debug_pre_eret(env);
    set_pc(env, env->CP0_DEPC);

    env->hflags &= ~MIPS_HFLAG_DM;
    compute_hflags(env);
    debug_post_eret(env);
}
#endif /* !CONFIG_USER_ONLY */

static inline void check_hwrena(CPUMIPSState *env, int reg, uintptr_t pc)
{
    if ((env->hflags & MIPS_HFLAG_CP0) || (env->CP0_HWREna & (1 << reg))) {
        return;
    }
    do_raise_exception(env, EXCP_RI, pc);
}

target_ulong helper_rdhwr_cpunum(CPUMIPSState *env)
{
    check_hwrena(env, 0, GETPC());
    return env->CP0_EBase & 0x3ff;
}

target_ulong helper_rdhwr_synci_step(CPUMIPSState *env)
{
    check_hwrena(env, 1, GETPC());
    return env->SYNCI_Step;
}

target_ulong helper_rdhwr_cc(CPUMIPSState *env)
{
    int32_t count;
    check_hwrena(env, 2, GETPC());
#ifdef CONFIG_USER_ONLY
    count = env->CP0_Count;
#else
    qemu_mutex_lock_iothread();
    count = (int32_t)cpu_mips_get_count(env);
    qemu_mutex_unlock_iothread();
#endif
    return count;
}

target_ulong helper_rdhwr_ccres(CPUMIPSState *env)
{
    check_hwrena(env, 3, GETPC());
    return env->CCRes;
}

target_ulong helper_rdhwr_performance(CPUMIPSState *env)
{
    check_hwrena(env, 4, GETPC());
    return env->CP0_Performance0;
}

target_ulong helper_rdhwr_xnp(CPUMIPSState *env)
{
    check_hwrena(env, 5, GETPC());
    return (env->CP0_Config5 >> CP0C5_XNP) & 1;
}

void helper_pmon(CPUMIPSState *env, int function)
{
    function /= 2;
    switch (function) {
    case 2: /* TODO: char inbyte(int waitflag); */
        if (env->active_tc.gpr[4] == 0)
            env->active_tc.gpr[2] = -1;
        /* Fall through */
    case 11: /* TODO: char inbyte (void); */
        env->active_tc.gpr[2] = -1;
        break;
    case 3:
    case 12:
        printf("%c", (char)(env->active_tc.gpr[4] & 0xFF));
        break;
    case 17:
        break;
    case 158:
        {
            unsigned char *fmt = (void *)(uintptr_t)env->active_tc.gpr[4];
            printf("%s", fmt);
        }
        break;
    }
}

void helper_wait(CPUMIPSState *env)
{
    CPUState *cs = CPU(mips_env_get_cpu(env));

    cs->halted = 1;
    cpu_reset_interrupt(cs, CPU_INTERRUPT_WAKE);
    /* Last instruction in the block, PC was updated before
       - no need to recover PC and icount */
    raise_exception(env, EXCP_HLT);
}

#if !defined(CONFIG_USER_ONLY)

void mips_cpu_do_unaligned_access(CPUState *cs, vaddr addr,
                                  MMUAccessType access_type,
                                  int mmu_idx, uintptr_t retaddr)
{
    MIPSCPU *cpu = MIPS_CPU(cs);
    CPUMIPSState *env = &cpu->env;
    int error_code = 0;
    int excp;

    env->CP0_BadVAddr = addr;

    if (access_type == MMU_DATA_STORE) {
        excp = EXCP_AdES;
    } else {
        excp = EXCP_AdEL;
        if (access_type == MMU_INST_FETCH) {
            error_code |= EXCP_INST_NOTAVAIL;
        }
    }

    do_raise_exception_err(env, excp, error_code, retaddr);
}

void tlb_fill(CPUState *cs, target_ulong addr, int size,
              MMUAccessType access_type, int mmu_idx, uintptr_t retaddr)
{
    int ret;

    ret = mips_cpu_handle_mmu_fault(cs, addr, size, access_type, mmu_idx);
    if (ret) {
        MIPSCPU *cpu = MIPS_CPU(cs);
        CPUMIPSState *env = &cpu->env;

        do_raise_exception_err(env, cs->exception_index,
                               env->error_code, retaddr);
    }
}

void mips_cpu_unassigned_access(CPUState *cs, hwaddr addr,
                                bool is_write, bool is_exec, int unused,
                                unsigned size)
{
    MIPSCPU *cpu = MIPS_CPU(cs);
    CPUMIPSState *env = &cpu->env;

    /*
     * Raising an exception with KVM enabled will crash because it won't be from
     * the main execution loop so the longjmp won't have a matching setjmp.
     * Until we can trigger a bus error exception through KVM lets just ignore
     * the access.
     */
    if (kvm_enabled()) {
        return;
    }

    if (is_exec) {
        raise_exception(env, EXCP_IBE);
    } else {
        raise_exception(env, EXCP_DBE);
    }
}

#ifdef CONFIG_MIPS_LOG_INSTR

/*
 * Print changed kernel/user/debug mode.
 */
static const char* mips_cpu_get_changed_mode(CPUMIPSState *env)
{
    const char *kernel0, *kernel1, *mode;

#if defined(TARGET_MIPS64)
    if (env->CP0_Status & (1 << CP0St_KX)) {
        kernel0 = "Kernel mode (ERL=0, KX=1)";
        kernel1 = "Kernel mode (ERL=1, KX=1)";
    } else {
        kernel0 = "Kernel mode (ERL=0, KX=0)";
        kernel1 = "Kernel mode (ERL=1, KX=0)";
    }
#else
    kernel0 = "Kernel mode (ERL=0)";
    kernel1 = "Kernel mode (ERL=1)";
#endif

    if (env->CP0_Debug & (1 << CP0DB_DM)) {
        mode = "Debug mode";
    } else if (env->CP0_Status & (1 << CP0St_ERL)) {
        mode = kernel1;
    } else if (env->CP0_Status & (1 << CP0St_EXL)) {
        mode = kernel0;
    } else {
        switch (extract32(env->CP0_Status, CP0St_KSU, 2)) {
        case 0:  mode = kernel0;           break;
        case 1:  mode = "Supervisor mode"; break;
        default: mode = TRACE_MODE_USER;   break;
        }
    }
    return mode;
}

/*
 * Names of coprocessor 0 registers.
 */
static const char *cop0_name[32*8] = {
/*0*/   "Index",        "MVPControl",   "MVPConf0",     "MVPConf1",
        0,              0,              0,              0,
/*1*/   "Random",       "VPEControl",   "VPEConf0",     "VPEConf1",
        "YQMask",       "VPESchedule",  "VPEScheFBack", "VPEOpt",
/*2*/   "EntryLo0",     "TCStatus",     "TCBind",       "TCRestart",
        "TCHalt",       "TCContext",    "TCSchedule",   "TCScheFBack",
/*3*/   "EntryLo1",     0,              0,              0,
        0,              0,              0,              "TCOpt",
/*4*/   "Context",      "ContextConfig","UserLocal",    "XContextConfig",
        0,              0,              0,              0,
/*5*/   "PageMask",     "PageGrain",    "SegCtl0",      "SegCtl1",
        "SegCtl2",      0,              0,              0,
/*6*/   "Wired",        "SRSConf0",     "SRSConf1",     "SRSConf2",
        "SRSConf3",     "SRSConf4",     0,              0,
/*7*/   "HWREna",       0,              0,              0,
        0,              0,              0,              0,
/*8*/   "BadVAddr",     "BadInstr",     "BadInstrP",    0,
        0,              0,              0,              0,
/*9*/   "Count",        0,              0,              0,
        0,              0,              0,              0,
/*10*/  "EntryHi",      0,              0,              0,
        0,              "MSAAccess",    "MSASave",      "MSARequest",
/*11*/  "Compare",      0,              0,              0,
        0,              0,              0,              0,
/*12*/  "Status",       "IntCtl",       "SRSCtl",       "SRSMap",
        "ViewIPL",      "SRSMap2",      0,              0,
/*13*/  "Cause",        0,              0,              0,
        "ViewRIPL",     "NestedExc",    0,              0,
/*14*/  "EPC",          0,              "NestedEPC",    0,
        0,              0,              0,              0,
/*15*/  "PRId",         "EBase",        "CDMMBase",     "CMGCRBase",
        0,              0,              0,              0,
/*16*/  "Config",       "Config1",      "Config2",      "Config3",
        "Config4",      "Config5",      "Config6",      "Config7",
/*17*/  "LLAddr",       0,              0,              0,
        0,              0,              0,              0,
/*18*/  "WatchLo",      "WatchLo1",     "WatchLo2",     "WatchLo3",
        "WatchLo4",     "WatchLo5",     "WatchLo6",     "WatchLo7",
/*19*/  "WatchHi",      "WatchHi1",     "WatchHi2",     "WatchHi3",
        "WatchHi4",     "WatchHi5",     "WatchHi6",     "WatchHi7",
/*20*/  "XContext",     0,              0,              0,
        0,              0,              0,              0,
/*21*/  0,              0,              0,              0,
        0,              0,              0,              0,
/*22*/  0,              0,              0,              0,
        0,              0,              0,              0,
/*23*/  "Debug",        "TraceControl", "TraceControl2","UserTraceData",
        "TraceIBPC",    "TraceDBPC",    "Debug2",       0,
/*24*/  "DEPC",         0,              "TraceControl3","UserTraceData2",
        0,              0,              0,              0,
/*25*/  "PerfCnt",      "PerfCnt1",     "PerfCnt2",     "PerfCnt3",
        "PerfCnt4",     "PerfCnt5",     "PerfCnt6",     "PerfCnt7",
/*26*/  "ErrCtl",       0,              0,              0,
        0,              0,              0,              0,
/*27*/  "CacheErr",     0,              0,              0,
        0,              0,              0,              0,
/*28*/  "ITagLo",       "IDataLo",      "DTagLo",       "DDataLo",
        "L23TagLo",     "L23DataLo",    0,              0,
/*29*/  "ITagHi",       "IDataHi",      "DTagHi",       0,
        0,              "L23DataHi",    0,              0,
/*30*/  "ErrorEPC",     0,              0,              0,
        0,              0,              0,              0,
/*31*/  "DESAVE",       0,              "KScratch1",    "KScratch2",
        "KScratch3",    "KScratch4",    "KScratch5",    "KScratch6",
};

/*
 * Print changed values of COP0 registers.
 */
static void dump_changed_cop0_reg(CPUMIPSState *env, int idx,
        target_ulong value)
{
    if (value != env->last_cop0[idx]) {
        env->last_cop0[idx] = value;
        if (cop0_name[idx])
            fprintf(qemu_logfile, "    Write %s = " TARGET_FMT_lx "\n",
                    cop0_name[idx], value);
        else
            fprintf(qemu_logfile, "    Write (idx=%d) = " TARGET_FMT_lx "\n",
                    idx, value);

    }
}

/*
 * Print changed values of COP0 registers.
 */
static void dump_changed_cop0(CPUMIPSState *env)
{
    dump_changed_cop0_reg(env, 0*8 + 0, env->CP0_Index);
    if (env->CP0_Config3 & (1 << CP0C3_MT)) {
        dump_changed_cop0_reg(env, 0*8 + 1, env->mvp->CP0_MVPControl);
        dump_changed_cop0_reg(env, 0*8 + 2, env->mvp->CP0_MVPConf0);
        dump_changed_cop0_reg(env, 0*8 + 3, env->mvp->CP0_MVPConf1);

        dump_changed_cop0_reg(env, 1*8 + 1, env->CP0_VPEControl);
        dump_changed_cop0_reg(env, 1*8 + 2, env->CP0_VPEConf0);
        dump_changed_cop0_reg(env, 1*8 + 3, env->CP0_VPEConf1);
        dump_changed_cop0_reg(env, 1*8 + 4, env->CP0_YQMask);
        dump_changed_cop0_reg(env, 1*8 + 5, env->CP0_VPESchedule);
        dump_changed_cop0_reg(env, 1*8 + 6, env->CP0_VPEScheFBack);
        dump_changed_cop0_reg(env, 1*8 + 7, env->CP0_VPEOpt);
    }

    dump_changed_cop0_reg(env, 2*8 + 0, env->CP0_EntryLo0);
    if (env->CP0_Config3 & (1 << CP0C3_MT)) {
        dump_changed_cop0_reg(env, 2*8 + 1, env->active_tc.CP0_TCStatus);
        dump_changed_cop0_reg(env, 2*8 + 2, env->active_tc.CP0_TCBind);
        dump_changed_cop0_reg(env, 2*8 + 3, env->active_tc.PC);
        dump_changed_cop0_reg(env, 2*8 + 4, env->active_tc.CP0_TCHalt);
        dump_changed_cop0_reg(env, 2*8 + 5, env->active_tc.CP0_TCContext);
        dump_changed_cop0_reg(env, 2*8 + 6, env->active_tc.CP0_TCSchedule);
        dump_changed_cop0_reg(env, 2*8 + 7, env->active_tc.CP0_TCScheFBack);
    }

    dump_changed_cop0_reg(env, 3*8 + 0, env->CP0_EntryLo1);

    dump_changed_cop0_reg(env, 4*8 + 0, env->CP0_Context);
    /* 4/1 not implemented - ContextConfig */
    dump_changed_cop0_reg(env, 4*8 + 2, env->active_tc.CP0_UserLocal);
    /* 4/3 not implemented - XContextConfig */

    dump_changed_cop0_reg(env, 5*8 + 0, env->CP0_PageMask);
    dump_changed_cop0_reg(env, 5*8 + 1, env->CP0_PageGrain);

    dump_changed_cop0_reg(env, 6*8 + 0, env->CP0_Wired);
    dump_changed_cop0_reg(env, 6*8 + 1, env->CP0_SRSConf0);
    dump_changed_cop0_reg(env, 6*8 + 2, env->CP0_SRSConf1);
    dump_changed_cop0_reg(env, 6*8 + 3, env->CP0_SRSConf2);
    dump_changed_cop0_reg(env, 6*8 + 4, env->CP0_SRSConf3);
    dump_changed_cop0_reg(env, 6*8 + 5, env->CP0_SRSConf4);

    dump_changed_cop0_reg(env, 7*8 + 0, env->CP0_HWREna);

    dump_changed_cop0_reg(env, 8*8 + 0, env->CP0_BadVAddr);
    if (env->CP0_Config3 & (1 << CP0C3_BI))
        dump_changed_cop0_reg(env, 8*8 + 1, env->CP0_BadInstr);
    if (env->CP0_Config3 & (1 << CP0C3_BP))
        dump_changed_cop0_reg(env, 8*8 + 2, env->CP0_BadInstrP);

    dump_changed_cop0_reg(env, 10*8 + 0, env->CP0_EntryHi);

    dump_changed_cop0_reg(env, 11*8 + 0, env->CP0_Compare);

    dump_changed_cop0_reg(env, 12*8 + 0, env->CP0_Status);
    dump_changed_cop0_reg(env, 12*8 + 1, env->CP0_IntCtl);
    dump_changed_cop0_reg(env, 12*8 + 2, env->CP0_SRSCtl);
    dump_changed_cop0_reg(env, 12*8 + 3, env->CP0_SRSMap);

    dump_changed_cop0_reg(env, 13*8 + 0, env->CP0_Cause);

    dump_changed_cop0_reg(env, 14*8 + 0, env->CP0_EPC);

    dump_changed_cop0_reg(env, 15*8 + 0, env->CP0_PRid);
    dump_changed_cop0_reg(env, 15*8 + 1, env->CP0_EBase);

    dump_changed_cop0_reg(env, 16*8 + 0, env->CP0_Config0);
    dump_changed_cop0_reg(env, 16*8 + 1, env->CP0_Config1);
    dump_changed_cop0_reg(env, 16*8 + 2, env->CP0_Config2);
    dump_changed_cop0_reg(env, 16*8 + 3, env->CP0_Config3);
    dump_changed_cop0_reg(env, 16*8 + 4, env->CP0_Config4);
    dump_changed_cop0_reg(env, 16*8 + 5, env->CP0_Config5);
    dump_changed_cop0_reg(env, 16*8 + 6, env->CP0_Config6);
    dump_changed_cop0_reg(env, 16*8 + 7, env->CP0_Config7);

    dump_changed_cop0_reg(env, 17*8 + 0, env->lladdr >> env->CP0_LLAddr_shift);

    dump_changed_cop0_reg(env, 18*8 + 0, env->CP0_WatchLo[0]);
    dump_changed_cop0_reg(env, 18*8 + 1, env->CP0_WatchLo[1]);
    dump_changed_cop0_reg(env, 18*8 + 2, env->CP0_WatchLo[2]);
    dump_changed_cop0_reg(env, 18*8 + 3, env->CP0_WatchLo[3]);
    dump_changed_cop0_reg(env, 18*8 + 4, env->CP0_WatchLo[4]);
    dump_changed_cop0_reg(env, 18*8 + 5, env->CP0_WatchLo[5]);
    dump_changed_cop0_reg(env, 18*8 + 6, env->CP0_WatchLo[6]);
    dump_changed_cop0_reg(env, 18*8 + 7, env->CP0_WatchLo[7]);

    dump_changed_cop0_reg(env, 19*8 + 0, env->CP0_WatchHi[0]);
    dump_changed_cop0_reg(env, 19*8 + 1, env->CP0_WatchHi[1]);
    dump_changed_cop0_reg(env, 19*8 + 2, env->CP0_WatchHi[2]);
    dump_changed_cop0_reg(env, 19*8 + 3, env->CP0_WatchHi[3]);
    dump_changed_cop0_reg(env, 19*8 + 4, env->CP0_WatchHi[4]);
    dump_changed_cop0_reg(env, 19*8 + 5, env->CP0_WatchHi[5]);
    dump_changed_cop0_reg(env, 19*8 + 6, env->CP0_WatchHi[6]);
    dump_changed_cop0_reg(env, 19*8 + 7, env->CP0_WatchHi[7]);

#if defined(TARGET_MIPS64)
    dump_changed_cop0_reg(env, 20*8 + 0, env->CP0_XContext);
#endif

    dump_changed_cop0_reg(env, 21*8 + 0, env->CP0_Framemask);

    /* 22/x not defined */

    dump_changed_cop0_reg(env, 23*8 + 0, helper_mfc0_debug(env));

    dump_changed_cop0_reg(env, 24*8 + 0, env->CP0_DEPC);

    dump_changed_cop0_reg(env, 25*8 + 0, env->CP0_Performance0);

    /* 26/0 - ErrCtl */
    dump_changed_cop0_reg(env, 25*8 + 0, env->CP0_ErrCtl);

    /* 27/0 not implemented - CacheErr */

    dump_changed_cop0_reg(env, 28*8 + 0, env->CP0_TagLo);
    dump_changed_cop0_reg(env, 28*8 + 1, env->CP0_DataLo);

    dump_changed_cop0_reg(env, 29*8 + 0, env->CP0_TagHi);
    dump_changed_cop0_reg(env, 29*8 + 1, env->CP0_DataHi);

    dump_changed_cop0_reg(env, 30*8 + 0, env->CP0_ErrorEPC);

    dump_changed_cop0_reg(env, 31*8 + 0, env->CP0_DESAVE);
    dump_changed_cop0_reg(env, 31*8 + 2, env->CP0_KScratch[0]);
    dump_changed_cop0_reg(env, 31*8 + 3, env->CP0_KScratch[1]);
    dump_changed_cop0_reg(env, 31*8 + 4, env->CP0_KScratch[2]);
    dump_changed_cop0_reg(env, 31*8 + 5, env->CP0_KScratch[3]);
    dump_changed_cop0_reg(env, 31*8 + 6, env->CP0_KScratch[4]);
    dump_changed_cop0_reg(env, 31*8 + 7, env->CP0_KScratch[5]);
}
#endif /* TARGET_CHERI */
#endif /* !CONFIG_USER_ONLY */

#ifdef CONFIG_MIPS_LOG_INSTR

/*
 * Print changed values of GPR, HI/LO and DSPControl registers.
 */
static void dump_changed_regs(CPUMIPSState *env)
{
    TCState *cur = &env->active_tc;

#ifndef TARGET_MIPS64
    static const char * const gpr_name[32] =
    {
      "zero", "at",   "v0",   "v1",   "a0",   "a1",   "a2",   "a3",
      "t0",   "t1",   "t2",   "t3",   "t4",   "t5",   "t6",   "t7",
      "s0",   "s1",   "s2",   "s3",   "s4",   "s5",   "s6",   "s7",
      "t8",   "t9",   "k0",   "k1",   "gp",   "sp",   "s8",   "ra"
    };
#else
    // Use n64 register names
    static const char * const gpr_name[32] =
    {
      "zero", "at",   "v0",   "v1",   "a0",   "a1",   "a2",   "a3",
      "a4",   "a5",   "a6",   "a7",   "t0",   "t1",   "t2",   "t3",
      "s0",   "s1",   "s2",   "s3",   "s4",   "s5",   "s6",   "s7",
      "t8",   "t9",   "k0",   "k1",   "gp",   "sp",   "s8",   "ra"
    };
#endif

    int i;

    for (i=1; i<32; i++) {
        if (cur->gpr[i] != env->last_gpr[i]) {
            env->last_gpr[i] = cur->gpr[i];
            cvtrace_dump_gpr(&env->cvtrace, cur->gpr[i]);
            if (qemu_loglevel_mask(CPU_LOG_INSTR)) {
                fprintf(qemu_logfile, "    Write %s = " TARGET_FMT_lx "\n",
                        gpr_name[i], cur->gpr[i]);
            }
        }
    }
#ifdef TARGET_CHERI
    dump_changed_cop2(env, cur);
#endif
}


static void update_tracing_on_mode_change(CPUMIPSState *env, const char* new_mode)
{
    if (!env->user_only_tracing_enabled) {
        return;
    }
    if (IN_USERSPACE(env)) {
        assert(strcmp(new_mode, TRACE_MODE_USER) != 0);
        /* When changing from user mode to kernel mode disable tracing */
        user_trace_dbg("%s -> %s: 0x%lx ASID %lu -- switching off tracing \n",
            env->last_mode, new_mode, env->active_tc.PC, env->CP0_EntryHi & 0xFF);
        env->tracing_suspended = true;
        qemu_set_log(qemu_loglevel & ~cl_default_trace_format);
    } else if (strcmp(new_mode, TRACE_MODE_USER) == 0) {
        /* When changing back to user mode restore instruction tracing */
        assert(!IN_USERSPACE(env));
        if (env->trace_explicitly_disabled) {
            user_trace_dbg("Not turning on tracing on switch %s -> %s 0x%lx. "
                "Tracing was explicitly disabled, ASID=%lu\n",
                env->last_mode, new_mode, env->active_tc.PC, env->CP0_EntryHi & 0xFF);
        } else if (env->tracing_suspended) {
            qemu_set_log(qemu_loglevel | cl_default_trace_format);
            user_trace_dbg("%s -> %s 0x%lx ASID %lu -- switching on tracing\n",
                env->last_mode, new_mode, env->active_tc.PC, env->CP0_EntryHi & 0xFF);
            env->tracing_suspended = false;
        }
    }
}

/*
 * Print the changed processor state.
 */
void helper_dump_changed_state(CPUMIPSState *env)
{
    const char* new_mode = mips_cpu_get_changed_mode(env);
    /* Testing pointer equality is fine, it always points to the same constants */
    if (new_mode != env->last_mode) {
        update_tracing_on_mode_change(env, new_mode);
        env->last_mode = new_mode;
        qemu_log_mask(CPU_LOG_INSTR, "--- %s\n", new_mode);
    }

    if (qemu_loglevel_mask(CPU_LOG_INSTR | CPU_LOG_CVTRACE)) {
        /* Print changed state: GPR, Cap. */
        dump_changed_regs(env);
    }

    if (qemu_loglevel_mask(CPU_LOG_INSTR)) {
        /* Print change state: HI/LO COP0 (not included in CVTRACE) */
        dump_changed_cop0(env);
    }
}

enum {
    /* Load and stores */
    OPC_LDL      = (0x1A << 26),
    OPC_LDR      = (0x1B << 26),
    OPC_LB       = (0x20 << 26),
    OPC_LH       = (0x21 << 26),
    OPC_LWL      = (0x22 << 26),
    OPC_LW       = (0x23 << 26),
    OPC_LWPC     = OPC_LW | 0x5,
    OPC_LBU      = (0x24 << 26),
    OPC_LHU      = (0x25 << 26),
    OPC_LWR      = (0x26 << 26),
    OPC_LWU      = (0x27 << 26),
    OPC_SB       = (0x28 << 26),
    OPC_SH       = (0x29 << 26),
    OPC_SWL      = (0x2A << 26),
    OPC_SW       = (0x2B << 26),
    OPC_SDL      = (0x2C << 26),
    OPC_SDR      = (0x2D << 26),
    OPC_SWR      = (0x2E << 26),
    OPC_LL       = (0x30 << 26),

    OPC_LWC1     = (0x31 << 26),
    OPC_LDC1     = (0x35 << 26),
    OPC_SWC1     = (0x39 << 26),
    OPC_SDC1     = (0x3D << 26),

    OPC_LWXC1   = 0x00 | (0x13 << 26),
    OPC_LDXC1   = 0x01 | (0x13 << 26),
    OPC_LUXC1   = 0x05 | (0x13 << 26),
    OPC_SWXC1   = 0x08 | (0x13 << 26),
    OPC_SDXC1   = 0x09 | (0x13 << 26),
    OPC_SUXC1   = 0x0D | (0x13 << 26),

    OPC_CSCB     = (0x12 << 26) | (0x10 << 21) | (0x0),
    OPC_CSCH     = (0x12 << 26) | (0x10 << 21) | (0x1),
    OPC_CSCW     = (0x12 << 26) | (0x10 << 21) | (0x2),
    OPC_CSCD     = (0x12 << 26) | (0x10 << 21) | (0x3),

    OPC_CSCC     = (0x12 << 26) | (0x10 << 21) | (0x7),

    OPC_CLLB     = (0x12 << 26) | (0x10 << 21) | (0x8),
    OPC_CLLH     = (0x12 << 26) | (0x10 << 21) | (0x9),
    OPC_CLLW     = (0x12 << 26) | (0x10 << 21) | (0xa),
    OPC_CLLD     = (0x12 << 26) | (0x10 << 21) | (0xb),
    OPC_CLLBU    = (0x12 << 26) | (0x10 << 21) | (0xc),
    OPC_CLLHU    = (0x12 << 26) | (0x10 << 21) | (0xd),
    OPC_CLLWU    = (0x12 << 26) | (0x10 << 21) | (0xe),

    OPC_CLLC     = (0x12 << 26) | (0x10 << 21) | (0xf),

    OPC_CLOADTAGS = (0x12 << 26) | (0x00 << 21) | (0x3f) | (0x1e << 6),

    OPC_CLBU     = (0x32 << 26) | (0x0),
    OPC_CLHU     = (0x32 << 26) | (0x1),
    OPC_CLWU     = (0x32 << 26) | (0x2),
    OPC_CLDU     = (0x32 << 26) | (0x3),
    OPC_CLB      = (0x32 << 26) | (0x4),
    OPC_CLH      = (0x32 << 26) | (0x5),
    OPC_CLW      = (0x32 << 26) | (0x6),
    OPC_CLD      = (0x32 << 26) | (0x7),

    OPC_CLOADC   = (0x36 << 26),

    OPC_CSB      = (0x3A << 26) | (0x0),
    OPC_CSH      = (0x3A << 26) | (0x1),
    OPC_CSW      = (0x3A << 26) | (0x2),
    OPC_CSD      = (0x3A << 26) | (0x3),

    OPC_CSTOREC  = (0x3E << 26),

    OPC_LLD      = (0x34 << 26),
    OPC_LD       = (0x37 << 26),
    OPC_LDPC     = OPC_LD | 0x5,
    OPC_SC       = (0x38 << 26),
    OPC_SCD      = (0x3C << 26),
    OPC_SD       = (0x3F << 26),
};

/*
 * dump non-capability data to cvtrace entry
 */
static inline void cvtrace_dump_gpr_ldst(cvtrace_t *cvtrace, uint8_t version,
        uint64_t addr, uint64_t value)
{
    if (qemu_loglevel_mask(CPU_LOG_CVTRACE)) {
        cvtrace->version = version;
        cvtrace->val1 = tswap64(addr);
        cvtrace->val2 = tswap64(value);
    }
}
#define cvtrace_dump_gpr_load(trace, addr, val)          \
    cvtrace_dump_gpr_ldst(trace, CVT_LD_GPR, addr, val)
#define cvtrace_dump_gpr_store(trace, addr, val)         \
    cvtrace_dump_gpr_ldst(trace, CVT_ST_GPR, addr, val)

/*
 * Print the memory store to log file.
 */
void dump_store(CPUMIPSState *env, int opc, target_ulong addr,
        target_ulong value)
{

    if (likely(!(qemu_loglevel_mask(CPU_LOG_INSTR) |
                 qemu_loglevel_mask(CPU_LOG_CVTRACE))))
        return;
    if (qemu_loglevel_mask(CPU_LOG_CVTRACE)) {
        cvtrace_dump_gpr_store(&env->cvtrace, addr, value);
        return;
    }

    switch (opc) {
#if defined(TARGET_MIPS64)
    case OPC_SCD:
    case OPC_SD:
    case OPC_SDL: // FIXME: value printed is not correct for sdl!
    case OPC_SDR: // FIXME: value printed is not correct for sdr!

    case OPC_SDC1:
    case OPC_SDXC1:
    case OPC_SUXC1:

    case OPC_CSD:
    case OPC_CSTOREC:
    case OPC_CSCD:
        fprintf(qemu_logfile, "    Memory Write [" TARGET_FMT_lx "] = "
                TARGET_FMT_lx"\n", addr, value);
        break;
#endif
    case OPC_SC:
    case OPC_SW:
    case OPC_SWL: // FIXME: value printed is not correct for swl!
    case OPC_SWR: // FIXME: value printed is not correct for swr!

    case OPC_SWC1:
    case OPC_SWXC1:

    case OPC_CSW:
    case OPC_CSCW:
        fprintf(qemu_logfile, "    Memory Write [" TARGET_FMT_lx "] = %08x\n",
                addr, (uint32_t) value);
        break;
    case OPC_SH:

    case OPC_CSH:
    case OPC_CSCH:
        fprintf(qemu_logfile, "    Memory Write [" TARGET_FMT_lx "] = %04x\n",
                addr, (uint16_t) value);
        break;
    case OPC_SB:

    case OPC_CSB:
    case OPC_CSCB:
        fprintf(qemu_logfile, "    Memory Write [" TARGET_FMT_lx "] = %02x\n",
                addr, (uint8_t) value);
        break;
    default:
        fprintf(qemu_logfile, "    Memory op%u [" TARGET_FMT_lx "] = %08x\n",
                opc, addr, (uint32_t) value);
    }
}

/*
 * Print the memory load to log file.
 */
void helper_dump_load(CPUMIPSState *env, int opc, target_ulong addr,
        target_ulong value)
{
    if (opc == OPC_CLLD || opc == OPC_CLLWU || opc == OPC_CLLW ||
        opc == OPC_CLLHU || opc == OPC_CLLH || opc == OPC_CLLBU ||
        opc == OPC_CLLB) {
        env->lladdr = do_translate_address(env, addr, 0, GETPC());
        env->llval = value;
    }
    if (likely(!(qemu_loglevel_mask(CPU_LOG_INSTR) |
                 qemu_loglevel_mask(CPU_LOG_CVTRACE))))
        return;
    if (qemu_loglevel_mask(CPU_LOG_CVTRACE)) {
        cvtrace_dump_gpr_load(&env->cvtrace, addr, value);
        return;
    }

    switch (opc) {
#if defined(TARGET_MIPS64)
    case OPC_LD:
    case OPC_LDL:
    case OPC_LDR:
    case OPC_LDPC:

    case OPC_LDC1:
    case OPC_LDXC1:
    case OPC_LLD:
    case OPC_LUXC1:

    case OPC_CLD:
    case OPC_CLOADC:
    case OPC_CLLD:
        fprintf(qemu_logfile, "    Memory Read [" TARGET_FMT_lx "] = "
                TARGET_FMT_lx "\n", addr, value);
        break;
    case OPC_LWU:
#endif
    case OPC_LL:
    case OPC_LW:
    case OPC_LWPC:
    case OPC_LWL:
    case OPC_LWR:

    case OPC_LWC1:
    case OPC_LWXC1:

    case OPC_CLW:
    case OPC_CLWU:
    case OPC_CLLW:
    case OPC_CLLWU:
        fprintf(qemu_logfile, "    Memory Read [" TARGET_FMT_lx "] = %08x\n",
                addr, (uint32_t) value);
        break;
    case OPC_LH:
    case OPC_LHU:

    case OPC_CLH:
    case OPC_CLHU:
    case OPC_CLLH:
    case OPC_CLLHU:
        fprintf(qemu_logfile, "    Memory Read [" TARGET_FMT_lx "] = %04x\n",
                addr, (uint16_t) value);
        break;
    case OPC_LB:
    case OPC_LBU:

    case OPC_CLB:
    case OPC_CLBU:
    case OPC_CLLB:
    case OPC_CLLBU:
        fprintf(qemu_logfile, "    Memory Read [" TARGET_FMT_lx "] = %02x\n",
                addr, (uint8_t) value);
        break;
    }
}

void helper_dump_load32(CPUMIPSState *env, int opc, target_ulong addr,
        uint32_t value)
{

    helper_dump_load(env, opc, addr, (target_ulong)value);
}
#endif /* CONFIG_MIPS_LOG_INSTR */

/* Complex FPU operations which may need stack space. */

#define FLOAT_TWO32 make_float32(1 << 30)
#define FLOAT_TWO64 make_float64(1ULL << 62)

#define FP_TO_INT32_OVERFLOW 0x7fffffff
#define FP_TO_INT64_OVERFLOW 0x7fffffffffffffffULL

/* convert MIPS rounding mode in FCR31 to IEEE library */
unsigned int ieee_rm[] = {
    float_round_nearest_even,
    float_round_to_zero,
    float_round_up,
    float_round_down
};

target_ulong helper_cfc1(CPUMIPSState *env, uint32_t reg)
{
    target_ulong arg1 = 0;

    switch (reg) {
    case 0:
        arg1 = (int32_t)env->active_fpu.fcr0;
        break;
    case 1:
        /* UFR Support - Read Status FR */
        if (env->active_fpu.fcr0 & (1 << FCR0_UFRP)) {
            if (env->CP0_Config5 & (1 << CP0C5_UFR)) {
                arg1 = (int32_t)
                       ((env->CP0_Status & (1  << CP0St_FR)) >> CP0St_FR);
            } else {
                do_raise_exception(env, EXCP_RI, GETPC());
            }
        }
        break;
    case 5:
        /* FRE Support - read Config5.FRE bit */
        if (env->active_fpu.fcr0 & (1 << FCR0_FREP)) {
            if (env->CP0_Config5 & (1 << CP0C5_UFE)) {
                arg1 = (env->CP0_Config5 >> CP0C5_FRE) & 1;
            } else {
                helper_raise_exception(env, EXCP_RI);
            }
        }
        break;
    case 25:
        arg1 = ((env->active_fpu.fcr31 >> 24) & 0xfe) | ((env->active_fpu.fcr31 >> 23) & 0x1);
        break;
    case 26:
        arg1 = env->active_fpu.fcr31 & 0x0003f07c;
        break;
    case 28:
        arg1 = (env->active_fpu.fcr31 & 0x00000f83) | ((env->active_fpu.fcr31 >> 22) & 0x4);
        break;
    default:
        arg1 = (int32_t)env->active_fpu.fcr31;
        break;
    }

    return arg1;
}

void helper_ctc1(CPUMIPSState *env, target_ulong arg1, uint32_t fs, uint32_t rt)
{
    switch (fs) {
    case 1:
        /* UFR Alias - Reset Status FR */
        if (!((env->active_fpu.fcr0 & (1 << FCR0_UFRP)) && (rt == 0))) {
            return;
        }
        if (env->CP0_Config5 & (1 << CP0C5_UFR)) {
            env->CP0_Status &= ~(1 << CP0St_FR);
            compute_hflags(env);
        } else {
            do_raise_exception(env, EXCP_RI, GETPC());
        }
        break;
    case 4:
        /* UNFR Alias - Set Status FR */
        if (!((env->active_fpu.fcr0 & (1 << FCR0_UFRP)) && (rt == 0))) {
            return;
        }
        if (env->CP0_Config5 & (1 << CP0C5_UFR)) {
            env->CP0_Status |= (1 << CP0St_FR);
            compute_hflags(env);
        } else {
            do_raise_exception(env, EXCP_RI, GETPC());
        }
        break;
    case 5:
        /* FRE Support - clear Config5.FRE bit */
        if (!((env->active_fpu.fcr0 & (1 << FCR0_FREP)) && (rt == 0))) {
            return;
        }
        if (env->CP0_Config5 & (1 << CP0C5_UFE)) {
            env->CP0_Config5 &= ~(1 << CP0C5_FRE);
            compute_hflags(env);
        } else {
            helper_raise_exception(env, EXCP_RI);
        }
        break;
    case 6:
        /* FRE Support - set Config5.FRE bit */
        if (!((env->active_fpu.fcr0 & (1 << FCR0_FREP)) && (rt == 0))) {
            return;
        }
        if (env->CP0_Config5 & (1 << CP0C5_UFE)) {
            env->CP0_Config5 |= (1 << CP0C5_FRE);
            compute_hflags(env);
        } else {
            helper_raise_exception(env, EXCP_RI);
        }
        break;
    case 25:
        if ((env->insn_flags & ISA_MIPS32R6) || (arg1 & 0xffffff00)) {
            return;
        }
        env->active_fpu.fcr31 = (env->active_fpu.fcr31 & 0x017fffff) | ((arg1 & 0xfe) << 24) |
                     ((arg1 & 0x1) << 23);
        break;
    case 26:
        if (arg1 & 0x007c0000)
            return;
        env->active_fpu.fcr31 = (env->active_fpu.fcr31 & 0xfffc0f83) | (arg1 & 0x0003f07c);
        break;
    case 28:
        if (arg1 & 0x007c0000)
            return;
        env->active_fpu.fcr31 = (env->active_fpu.fcr31 & 0xfefff07c) | (arg1 & 0x00000f83) |
                     ((arg1 & 0x4) << 22);
        break;
    case 31:
        env->active_fpu.fcr31 = (arg1 & env->active_fpu.fcr31_rw_bitmask) |
               (env->active_fpu.fcr31 & ~(env->active_fpu.fcr31_rw_bitmask));
        break;
    default:
        return;
    }
    restore_fp_status(env);
    set_float_exception_flags(0, &env->active_fpu.fp_status);
    if ((GET_FP_ENABLE(env->active_fpu.fcr31) | 0x20) & GET_FP_CAUSE(env->active_fpu.fcr31))
        do_raise_exception(env, EXCP_FPE, GETPC());
}

int ieee_ex_to_mips(int xcpt)
{
    int ret = 0;
    if (xcpt) {
        if (xcpt & float_flag_invalid) {
            ret |= FP_INVALID;
        }
        if (xcpt & float_flag_overflow) {
            ret |= FP_OVERFLOW;
        }
        if (xcpt & float_flag_underflow) {
            ret |= FP_UNDERFLOW;
        }
        if (xcpt & float_flag_divbyzero) {
            ret |= FP_DIV0;
        }
        if (xcpt & float_flag_inexact) {
            ret |= FP_INEXACT;
        }
    }
    return ret;
}

static inline void update_fcr31(CPUMIPSState *env, uintptr_t pc)
{
    int tmp = ieee_ex_to_mips(get_float_exception_flags(&env->active_fpu.fp_status));

    SET_FP_CAUSE(env->active_fpu.fcr31, tmp);

    if (tmp) {
        set_float_exception_flags(0, &env->active_fpu.fp_status);

        if (GET_FP_ENABLE(env->active_fpu.fcr31) & tmp) {
            do_raise_exception(env, EXCP_FPE, pc);
        } else {
            UPDATE_FP_FLAGS(env->active_fpu.fcr31, tmp);
        }
    }
}

/* Float support.
   Single precition routines have a "s" suffix, double precision a
   "d" suffix, 32bit integer "w", 64bit integer "l", paired single "ps",
   paired single lower "pl", paired single upper "pu".  */

/* unary operations, modifying fp status  */
uint64_t helper_float_sqrt_d(CPUMIPSState *env, uint64_t fdt0)
{
    fdt0 = float64_sqrt(fdt0, &env->active_fpu.fp_status);
    update_fcr31(env, GETPC());
    return fdt0;
}

uint32_t helper_float_sqrt_s(CPUMIPSState *env, uint32_t fst0)
{
    fst0 = float32_sqrt(fst0, &env->active_fpu.fp_status);
    update_fcr31(env, GETPC());
    return fst0;
}

uint64_t helper_float_cvtd_s(CPUMIPSState *env, uint32_t fst0)
{
    uint64_t fdt2;

    fdt2 = float32_to_float64(fst0, &env->active_fpu.fp_status);
    update_fcr31(env, GETPC());
    return fdt2;
}

uint64_t helper_float_cvtd_w(CPUMIPSState *env, uint32_t wt0)
{
    uint64_t fdt2;

    fdt2 = int32_to_float64(wt0, &env->active_fpu.fp_status);
    update_fcr31(env, GETPC());
    return fdt2;
}

uint64_t helper_float_cvtd_l(CPUMIPSState *env, uint64_t dt0)
{
    uint64_t fdt2;

    fdt2 = int64_to_float64(dt0, &env->active_fpu.fp_status);
    update_fcr31(env, GETPC());
    return fdt2;
}

uint64_t helper_float_cvt_l_d(CPUMIPSState *env, uint64_t fdt0)
{
    uint64_t dt2;

    dt2 = float64_to_int64(fdt0, &env->active_fpu.fp_status);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
        & (float_flag_invalid | float_flag_overflow)) {
        dt2 = FP_TO_INT64_OVERFLOW;
    }
    update_fcr31(env, GETPC());
    return dt2;
}

uint64_t helper_float_cvt_l_s(CPUMIPSState *env, uint32_t fst0)
{
    uint64_t dt2;

    dt2 = float32_to_int64(fst0, &env->active_fpu.fp_status);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
        & (float_flag_invalid | float_flag_overflow)) {
        dt2 = FP_TO_INT64_OVERFLOW;
    }
    update_fcr31(env, GETPC());
    return dt2;
}

uint64_t helper_float_cvtps_pw(CPUMIPSState *env, uint64_t dt0)
{
    uint32_t fst2;
    uint32_t fsth2;

    fst2 = int32_to_float32(dt0 & 0XFFFFFFFF, &env->active_fpu.fp_status);
    fsth2 = int32_to_float32(dt0 >> 32, &env->active_fpu.fp_status);
    update_fcr31(env, GETPC());
    return ((uint64_t)fsth2 << 32) | fst2;
}

uint64_t helper_float_cvtpw_ps(CPUMIPSState *env, uint64_t fdt0)
{
    uint32_t wt2;
    uint32_t wth2;
    int excp, excph;

    wt2 = float32_to_int32(fdt0 & 0XFFFFFFFF, &env->active_fpu.fp_status);
    excp = get_float_exception_flags(&env->active_fpu.fp_status);
    if (excp & (float_flag_overflow | float_flag_invalid)) {
        wt2 = FP_TO_INT32_OVERFLOW;
    }

    set_float_exception_flags(0, &env->active_fpu.fp_status);
    wth2 = float32_to_int32(fdt0 >> 32, &env->active_fpu.fp_status);
    excph = get_float_exception_flags(&env->active_fpu.fp_status);
    if (excph & (float_flag_overflow | float_flag_invalid)) {
        wth2 = FP_TO_INT32_OVERFLOW;
    }

    set_float_exception_flags(excp | excph, &env->active_fpu.fp_status);
    update_fcr31(env, GETPC());

    return ((uint64_t)wth2 << 32) | wt2;
}

uint32_t helper_float_cvts_d(CPUMIPSState *env, uint64_t fdt0)
{
    uint32_t fst2;

    fst2 = float64_to_float32(fdt0, &env->active_fpu.fp_status);
    update_fcr31(env, GETPC());
    return fst2;
}

uint32_t helper_float_cvts_w(CPUMIPSState *env, uint32_t wt0)
{
    uint32_t fst2;

    fst2 = int32_to_float32(wt0, &env->active_fpu.fp_status);
    update_fcr31(env, GETPC());
    return fst2;
}

uint32_t helper_float_cvts_l(CPUMIPSState *env, uint64_t dt0)
{
    uint32_t fst2;

    fst2 = int64_to_float32(dt0, &env->active_fpu.fp_status);
    update_fcr31(env, GETPC());
    return fst2;
}

uint32_t helper_float_cvts_pl(CPUMIPSState *env, uint32_t wt0)
{
    uint32_t wt2;

    wt2 = wt0;
    update_fcr31(env, GETPC());
    return wt2;
}

uint32_t helper_float_cvts_pu(CPUMIPSState *env, uint32_t wth0)
{
    uint32_t wt2;

    wt2 = wth0;
    update_fcr31(env, GETPC());
    return wt2;
}

uint32_t helper_float_cvt_w_s(CPUMIPSState *env, uint32_t fst0)
{
    uint32_t wt2;

    wt2 = float32_to_int32(fst0, &env->active_fpu.fp_status);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
        & (float_flag_invalid | float_flag_overflow)) {
        wt2 = FP_TO_INT32_OVERFLOW;
    }
    update_fcr31(env, GETPC());
    return wt2;
}

uint32_t helper_float_cvt_w_d(CPUMIPSState *env, uint64_t fdt0)
{
    uint32_t wt2;

    wt2 = float64_to_int32(fdt0, &env->active_fpu.fp_status);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
        & (float_flag_invalid | float_flag_overflow)) {
        wt2 = FP_TO_INT32_OVERFLOW;
    }
    update_fcr31(env, GETPC());
    return wt2;
}

uint64_t helper_float_round_l_d(CPUMIPSState *env, uint64_t fdt0)
{
    uint64_t dt2;

    set_float_rounding_mode(float_round_nearest_even, &env->active_fpu.fp_status);
    dt2 = float64_to_int64(fdt0, &env->active_fpu.fp_status);
    restore_rounding_mode(env);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
        & (float_flag_invalid | float_flag_overflow)) {
        dt2 = FP_TO_INT64_OVERFLOW;
    }
    update_fcr31(env, GETPC());
    return dt2;
}

uint64_t helper_float_round_l_s(CPUMIPSState *env, uint32_t fst0)
{
    uint64_t dt2;

    set_float_rounding_mode(float_round_nearest_even, &env->active_fpu.fp_status);
    dt2 = float32_to_int64(fst0, &env->active_fpu.fp_status);
    restore_rounding_mode(env);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
        & (float_flag_invalid | float_flag_overflow)) {
        dt2 = FP_TO_INT64_OVERFLOW;
    }
    update_fcr31(env, GETPC());
    return dt2;
}

uint32_t helper_float_round_w_d(CPUMIPSState *env, uint64_t fdt0)
{
    uint32_t wt2;

    set_float_rounding_mode(float_round_nearest_even, &env->active_fpu.fp_status);
    wt2 = float64_to_int32(fdt0, &env->active_fpu.fp_status);
    restore_rounding_mode(env);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
        & (float_flag_invalid | float_flag_overflow)) {
        wt2 = FP_TO_INT32_OVERFLOW;
    }
    update_fcr31(env, GETPC());
    return wt2;
}

uint32_t helper_float_round_w_s(CPUMIPSState *env, uint32_t fst0)
{
    uint32_t wt2;

    set_float_rounding_mode(float_round_nearest_even, &env->active_fpu.fp_status);
    wt2 = float32_to_int32(fst0, &env->active_fpu.fp_status);
    restore_rounding_mode(env);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
        & (float_flag_invalid | float_flag_overflow)) {
        wt2 = FP_TO_INT32_OVERFLOW;
    }
    update_fcr31(env, GETPC());
    return wt2;
}

uint64_t helper_float_trunc_l_d(CPUMIPSState *env, uint64_t fdt0)
{
    uint64_t dt2;

    dt2 = float64_to_int64_round_to_zero(fdt0, &env->active_fpu.fp_status);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
        & (float_flag_invalid | float_flag_overflow)) {
        dt2 = FP_TO_INT64_OVERFLOW;
    }
    update_fcr31(env, GETPC());
    return dt2;
}

uint64_t helper_float_trunc_l_s(CPUMIPSState *env, uint32_t fst0)
{
    uint64_t dt2;

    dt2 = float32_to_int64_round_to_zero(fst0, &env->active_fpu.fp_status);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
        & (float_flag_invalid | float_flag_overflow)) {
        dt2 = FP_TO_INT64_OVERFLOW;
    }
    update_fcr31(env, GETPC());
    return dt2;
}

uint32_t helper_float_trunc_w_d(CPUMIPSState *env, uint64_t fdt0)
{
    uint32_t wt2;

    wt2 = float64_to_int32_round_to_zero(fdt0, &env->active_fpu.fp_status);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
        & (float_flag_invalid | float_flag_overflow)) {
        wt2 = FP_TO_INT32_OVERFLOW;
    }
    update_fcr31(env, GETPC());
    return wt2;
}

uint32_t helper_float_trunc_w_s(CPUMIPSState *env, uint32_t fst0)
{
    uint32_t wt2;

    wt2 = float32_to_int32_round_to_zero(fst0, &env->active_fpu.fp_status);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
        & (float_flag_invalid | float_flag_overflow)) {
        wt2 = FP_TO_INT32_OVERFLOW;
    }
    update_fcr31(env, GETPC());
    return wt2;
}

uint64_t helper_float_ceil_l_d(CPUMIPSState *env, uint64_t fdt0)
{
    uint64_t dt2;

    set_float_rounding_mode(float_round_up, &env->active_fpu.fp_status);
    dt2 = float64_to_int64(fdt0, &env->active_fpu.fp_status);
    restore_rounding_mode(env);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
        & (float_flag_invalid | float_flag_overflow)) {
        dt2 = FP_TO_INT64_OVERFLOW;
    }
    update_fcr31(env, GETPC());
    return dt2;
}

uint64_t helper_float_ceil_l_s(CPUMIPSState *env, uint32_t fst0)
{
    uint64_t dt2;

    set_float_rounding_mode(float_round_up, &env->active_fpu.fp_status);
    dt2 = float32_to_int64(fst0, &env->active_fpu.fp_status);
    restore_rounding_mode(env);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
        & (float_flag_invalid | float_flag_overflow)) {
        dt2 = FP_TO_INT64_OVERFLOW;
    }
    update_fcr31(env, GETPC());
    return dt2;
}

uint32_t helper_float_ceil_w_d(CPUMIPSState *env, uint64_t fdt0)
{
    uint32_t wt2;

    set_float_rounding_mode(float_round_up, &env->active_fpu.fp_status);
    wt2 = float64_to_int32(fdt0, &env->active_fpu.fp_status);
    restore_rounding_mode(env);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
        & (float_flag_invalid | float_flag_overflow)) {
        wt2 = FP_TO_INT32_OVERFLOW;
    }
    update_fcr31(env, GETPC());
    return wt2;
}

uint32_t helper_float_ceil_w_s(CPUMIPSState *env, uint32_t fst0)
{
    uint32_t wt2;

    set_float_rounding_mode(float_round_up, &env->active_fpu.fp_status);
    wt2 = float32_to_int32(fst0, &env->active_fpu.fp_status);
    restore_rounding_mode(env);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
        & (float_flag_invalid | float_flag_overflow)) {
        wt2 = FP_TO_INT32_OVERFLOW;
    }
    update_fcr31(env, GETPC());
    return wt2;
}

uint64_t helper_float_floor_l_d(CPUMIPSState *env, uint64_t fdt0)
{
    uint64_t dt2;

    set_float_rounding_mode(float_round_down, &env->active_fpu.fp_status);
    dt2 = float64_to_int64(fdt0, &env->active_fpu.fp_status);
    restore_rounding_mode(env);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
        & (float_flag_invalid | float_flag_overflow)) {
        dt2 = FP_TO_INT64_OVERFLOW;
    }
    update_fcr31(env, GETPC());
    return dt2;
}

uint64_t helper_float_floor_l_s(CPUMIPSState *env, uint32_t fst0)
{
    uint64_t dt2;

    set_float_rounding_mode(float_round_down, &env->active_fpu.fp_status);
    dt2 = float32_to_int64(fst0, &env->active_fpu.fp_status);
    restore_rounding_mode(env);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
        & (float_flag_invalid | float_flag_overflow)) {
        dt2 = FP_TO_INT64_OVERFLOW;
    }
    update_fcr31(env, GETPC());
    return dt2;
}

uint32_t helper_float_floor_w_d(CPUMIPSState *env, uint64_t fdt0)
{
    uint32_t wt2;

    set_float_rounding_mode(float_round_down, &env->active_fpu.fp_status);
    wt2 = float64_to_int32(fdt0, &env->active_fpu.fp_status);
    restore_rounding_mode(env);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
        & (float_flag_invalid | float_flag_overflow)) {
        wt2 = FP_TO_INT32_OVERFLOW;
    }
    update_fcr31(env, GETPC());
    return wt2;
}

uint32_t helper_float_floor_w_s(CPUMIPSState *env, uint32_t fst0)
{
    uint32_t wt2;

    set_float_rounding_mode(float_round_down, &env->active_fpu.fp_status);
    wt2 = float32_to_int32(fst0, &env->active_fpu.fp_status);
    restore_rounding_mode(env);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
        & (float_flag_invalid | float_flag_overflow)) {
        wt2 = FP_TO_INT32_OVERFLOW;
    }
    update_fcr31(env, GETPC());
    return wt2;
}

uint64_t helper_float_cvt_2008_l_d(CPUMIPSState *env, uint64_t fdt0)
{
    uint64_t dt2;

    dt2 = float64_to_int64(fdt0, &env->active_fpu.fp_status);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
            & float_flag_invalid) {
        if (float64_is_any_nan(fdt0)) {
            dt2 = 0;
        }
    }
    update_fcr31(env, GETPC());
    return dt2;
}

uint64_t helper_float_cvt_2008_l_s(CPUMIPSState *env, uint32_t fst0)
{
    uint64_t dt2;

    dt2 = float32_to_int64(fst0, &env->active_fpu.fp_status);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
            & float_flag_invalid) {
        if (float32_is_any_nan(fst0)) {
            dt2 = 0;
        }
    }
    update_fcr31(env, GETPC());
    return dt2;
}

uint32_t helper_float_cvt_2008_w_d(CPUMIPSState *env, uint64_t fdt0)
{
    uint32_t wt2;

    wt2 = float64_to_int32(fdt0, &env->active_fpu.fp_status);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
            & float_flag_invalid) {
        if (float64_is_any_nan(fdt0)) {
            wt2 = 0;
        }
    }
    update_fcr31(env, GETPC());
    return wt2;
}

uint32_t helper_float_cvt_2008_w_s(CPUMIPSState *env, uint32_t fst0)
{
    uint32_t wt2;

    wt2 = float32_to_int32(fst0, &env->active_fpu.fp_status);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
            & float_flag_invalid) {
        if (float32_is_any_nan(fst0)) {
            wt2 = 0;
        }
    }
    update_fcr31(env, GETPC());
    return wt2;
}

uint64_t helper_float_round_2008_l_d(CPUMIPSState *env, uint64_t fdt0)
{
    uint64_t dt2;

    set_float_rounding_mode(float_round_nearest_even,
            &env->active_fpu.fp_status);
    dt2 = float64_to_int64(fdt0, &env->active_fpu.fp_status);
    restore_rounding_mode(env);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
            & float_flag_invalid) {
        if (float64_is_any_nan(fdt0)) {
            dt2 = 0;
        }
    }
    update_fcr31(env, GETPC());
    return dt2;
}

uint64_t helper_float_round_2008_l_s(CPUMIPSState *env, uint32_t fst0)
{
    uint64_t dt2;

    set_float_rounding_mode(float_round_nearest_even,
            &env->active_fpu.fp_status);
    dt2 = float32_to_int64(fst0, &env->active_fpu.fp_status);
    restore_rounding_mode(env);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
            & float_flag_invalid) {
        if (float32_is_any_nan(fst0)) {
            dt2 = 0;
        }
    }
    update_fcr31(env, GETPC());
    return dt2;
}

uint32_t helper_float_round_2008_w_d(CPUMIPSState *env, uint64_t fdt0)
{
    uint32_t wt2;

    set_float_rounding_mode(float_round_nearest_even,
            &env->active_fpu.fp_status);
    wt2 = float64_to_int32(fdt0, &env->active_fpu.fp_status);
    restore_rounding_mode(env);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
            & float_flag_invalid) {
        if (float64_is_any_nan(fdt0)) {
            wt2 = 0;
        }
    }
    update_fcr31(env, GETPC());
    return wt2;
}

uint32_t helper_float_round_2008_w_s(CPUMIPSState *env, uint32_t fst0)
{
    uint32_t wt2;

    set_float_rounding_mode(float_round_nearest_even,
            &env->active_fpu.fp_status);
    wt2 = float32_to_int32(fst0, &env->active_fpu.fp_status);
    restore_rounding_mode(env);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
            & float_flag_invalid) {
        if (float32_is_any_nan(fst0)) {
            wt2 = 0;
        }
    }
    update_fcr31(env, GETPC());
    return wt2;
}

uint64_t helper_float_trunc_2008_l_d(CPUMIPSState *env, uint64_t fdt0)
{
    uint64_t dt2;

    dt2 = float64_to_int64_round_to_zero(fdt0, &env->active_fpu.fp_status);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
            & float_flag_invalid) {
        if (float64_is_any_nan(fdt0)) {
            dt2 = 0;
        }
    }
    update_fcr31(env, GETPC());
    return dt2;
}

uint64_t helper_float_trunc_2008_l_s(CPUMIPSState *env, uint32_t fst0)
{
    uint64_t dt2;

    dt2 = float32_to_int64_round_to_zero(fst0, &env->active_fpu.fp_status);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
            & float_flag_invalid) {
        if (float32_is_any_nan(fst0)) {
            dt2 = 0;
        }
    }
    update_fcr31(env, GETPC());
    return dt2;
}

uint32_t helper_float_trunc_2008_w_d(CPUMIPSState *env, uint64_t fdt0)
{
    uint32_t wt2;

    wt2 = float64_to_int32_round_to_zero(fdt0, &env->active_fpu.fp_status);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
            & float_flag_invalid) {
        if (float64_is_any_nan(fdt0)) {
            wt2 = 0;
        }
    }
    update_fcr31(env, GETPC());
    return wt2;
}

uint32_t helper_float_trunc_2008_w_s(CPUMIPSState *env, uint32_t fst0)
{
    uint32_t wt2;

    wt2 = float32_to_int32_round_to_zero(fst0, &env->active_fpu.fp_status);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
            & float_flag_invalid) {
        if (float32_is_any_nan(fst0)) {
            wt2 = 0;
        }
    }
    update_fcr31(env, GETPC());
    return wt2;
}

uint64_t helper_float_ceil_2008_l_d(CPUMIPSState *env, uint64_t fdt0)
{
    uint64_t dt2;

    set_float_rounding_mode(float_round_up, &env->active_fpu.fp_status);
    dt2 = float64_to_int64(fdt0, &env->active_fpu.fp_status);
    restore_rounding_mode(env);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
            & float_flag_invalid) {
        if (float64_is_any_nan(fdt0)) {
            dt2 = 0;
        }
    }
    update_fcr31(env, GETPC());
    return dt2;
}

uint64_t helper_float_ceil_2008_l_s(CPUMIPSState *env, uint32_t fst0)
{
    uint64_t dt2;

    set_float_rounding_mode(float_round_up, &env->active_fpu.fp_status);
    dt2 = float32_to_int64(fst0, &env->active_fpu.fp_status);
    restore_rounding_mode(env);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
            & float_flag_invalid) {
        if (float32_is_any_nan(fst0)) {
            dt2 = 0;
        }
    }
    update_fcr31(env, GETPC());
    return dt2;
}

uint32_t helper_float_ceil_2008_w_d(CPUMIPSState *env, uint64_t fdt0)
{
    uint32_t wt2;

    set_float_rounding_mode(float_round_up, &env->active_fpu.fp_status);
    wt2 = float64_to_int32(fdt0, &env->active_fpu.fp_status);
    restore_rounding_mode(env);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
            & float_flag_invalid) {
        if (float64_is_any_nan(fdt0)) {
            wt2 = 0;
        }
    }
    update_fcr31(env, GETPC());
    return wt2;
}

uint32_t helper_float_ceil_2008_w_s(CPUMIPSState *env, uint32_t fst0)
{
    uint32_t wt2;

    set_float_rounding_mode(float_round_up, &env->active_fpu.fp_status);
    wt2 = float32_to_int32(fst0, &env->active_fpu.fp_status);
    restore_rounding_mode(env);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
            & float_flag_invalid) {
        if (float32_is_any_nan(fst0)) {
            wt2 = 0;
        }
    }
    update_fcr31(env, GETPC());
    return wt2;
}

uint64_t helper_float_floor_2008_l_d(CPUMIPSState *env, uint64_t fdt0)
{
    uint64_t dt2;

    set_float_rounding_mode(float_round_down, &env->active_fpu.fp_status);
    dt2 = float64_to_int64(fdt0, &env->active_fpu.fp_status);
    restore_rounding_mode(env);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
            & float_flag_invalid) {
        if (float64_is_any_nan(fdt0)) {
            dt2 = 0;
        }
    }
    update_fcr31(env, GETPC());
    return dt2;
}

uint64_t helper_float_floor_2008_l_s(CPUMIPSState *env, uint32_t fst0)
{
    uint64_t dt2;

    set_float_rounding_mode(float_round_down, &env->active_fpu.fp_status);
    dt2 = float32_to_int64(fst0, &env->active_fpu.fp_status);
    restore_rounding_mode(env);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
            & float_flag_invalid) {
        if (float32_is_any_nan(fst0)) {
            dt2 = 0;
        }
    }
    update_fcr31(env, GETPC());
    return dt2;
}

uint32_t helper_float_floor_2008_w_d(CPUMIPSState *env, uint64_t fdt0)
{
    uint32_t wt2;

    set_float_rounding_mode(float_round_down, &env->active_fpu.fp_status);
    wt2 = float64_to_int32(fdt0, &env->active_fpu.fp_status);
    restore_rounding_mode(env);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
            & float_flag_invalid) {
        if (float64_is_any_nan(fdt0)) {
            wt2 = 0;
        }
    }
    update_fcr31(env, GETPC());
    return wt2;
}

uint32_t helper_float_floor_2008_w_s(CPUMIPSState *env, uint32_t fst0)
{
    uint32_t wt2;

    set_float_rounding_mode(float_round_down, &env->active_fpu.fp_status);
    wt2 = float32_to_int32(fst0, &env->active_fpu.fp_status);
    restore_rounding_mode(env);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
            & float_flag_invalid) {
        if (float32_is_any_nan(fst0)) {
            wt2 = 0;
        }
    }
    update_fcr31(env, GETPC());
    return wt2;
}

/* unary operations, not modifying fp status  */
#define FLOAT_UNOP(name)                                       \
uint64_t helper_float_ ## name ## _d(uint64_t fdt0)                \
{                                                              \
    return float64_ ## name(fdt0);                             \
}                                                              \
uint32_t helper_float_ ## name ## _s(uint32_t fst0)                \
{                                                              \
    return float32_ ## name(fst0);                             \
}                                                              \
uint64_t helper_float_ ## name ## _ps(uint64_t fdt0)               \
{                                                              \
    uint32_t wt0;                                              \
    uint32_t wth0;                                             \
                                                               \
    wt0 = float32_ ## name(fdt0 & 0XFFFFFFFF);                 \
    wth0 = float32_ ## name(fdt0 >> 32);                       \
    return ((uint64_t)wth0 << 32) | wt0;                       \
}
FLOAT_UNOP(abs)
FLOAT_UNOP(chs)
#undef FLOAT_UNOP

/* MIPS specific unary operations */
uint64_t helper_float_recip_d(CPUMIPSState *env, uint64_t fdt0)
{
    uint64_t fdt2;

    fdt2 = float64_div(float64_one, fdt0, &env->active_fpu.fp_status);
    update_fcr31(env, GETPC());
    return fdt2;
}

uint32_t helper_float_recip_s(CPUMIPSState *env, uint32_t fst0)
{
    uint32_t fst2;

    fst2 = float32_div(float32_one, fst0, &env->active_fpu.fp_status);
    update_fcr31(env, GETPC());
    return fst2;
}

uint64_t helper_float_rsqrt_d(CPUMIPSState *env, uint64_t fdt0)
{
    uint64_t fdt2;

    fdt2 = float64_sqrt(fdt0, &env->active_fpu.fp_status);
    fdt2 = float64_div(float64_one, fdt2, &env->active_fpu.fp_status);
    update_fcr31(env, GETPC());
    return fdt2;
}

uint32_t helper_float_rsqrt_s(CPUMIPSState *env, uint32_t fst0)
{
    uint32_t fst2;

    fst2 = float32_sqrt(fst0, &env->active_fpu.fp_status);
    fst2 = float32_div(float32_one, fst2, &env->active_fpu.fp_status);
    update_fcr31(env, GETPC());
    return fst2;
}

uint64_t helper_float_recip1_d(CPUMIPSState *env, uint64_t fdt0)
{
    uint64_t fdt2;

    fdt2 = float64_div(float64_one, fdt0, &env->active_fpu.fp_status);
    update_fcr31(env, GETPC());
    return fdt2;
}

uint32_t helper_float_recip1_s(CPUMIPSState *env, uint32_t fst0)
{
    uint32_t fst2;

    fst2 = float32_div(float32_one, fst0, &env->active_fpu.fp_status);
    update_fcr31(env, GETPC());
    return fst2;
}

uint64_t helper_float_recip1_ps(CPUMIPSState *env, uint64_t fdt0)
{
    uint32_t fst2;
    uint32_t fsth2;

    fst2 = float32_div(float32_one, fdt0 & 0XFFFFFFFF, &env->active_fpu.fp_status);
    fsth2 = float32_div(float32_one, fdt0 >> 32, &env->active_fpu.fp_status);
    update_fcr31(env, GETPC());
    return ((uint64_t)fsth2 << 32) | fst2;
}

uint64_t helper_float_rsqrt1_d(CPUMIPSState *env, uint64_t fdt0)
{
    uint64_t fdt2;

    fdt2 = float64_sqrt(fdt0, &env->active_fpu.fp_status);
    fdt2 = float64_div(float64_one, fdt2, &env->active_fpu.fp_status);
    update_fcr31(env, GETPC());
    return fdt2;
}

uint32_t helper_float_rsqrt1_s(CPUMIPSState *env, uint32_t fst0)
{
    uint32_t fst2;

    fst2 = float32_sqrt(fst0, &env->active_fpu.fp_status);
    fst2 = float32_div(float32_one, fst2, &env->active_fpu.fp_status);
    update_fcr31(env, GETPC());
    return fst2;
}

uint64_t helper_float_rsqrt1_ps(CPUMIPSState *env, uint64_t fdt0)
{
    uint32_t fst2;
    uint32_t fsth2;

    fst2 = float32_sqrt(fdt0 & 0XFFFFFFFF, &env->active_fpu.fp_status);
    fsth2 = float32_sqrt(fdt0 >> 32, &env->active_fpu.fp_status);
    fst2 = float32_div(float32_one, fst2, &env->active_fpu.fp_status);
    fsth2 = float32_div(float32_one, fsth2, &env->active_fpu.fp_status);
    update_fcr31(env, GETPC());
    return ((uint64_t)fsth2 << 32) | fst2;
}

#define FLOAT_RINT(name, bits)                                              \
uint ## bits ## _t helper_float_ ## name (CPUMIPSState *env,                \
                                          uint ## bits ## _t fs)            \
{                                                                           \
    uint ## bits ## _t fdret;                                               \
                                                                            \
    fdret = float ## bits ## _round_to_int(fs, &env->active_fpu.fp_status); \
    update_fcr31(env, GETPC());                                             \
    return fdret;                                                           \
}

FLOAT_RINT(rint_s, 32)
FLOAT_RINT(rint_d, 64)
#undef FLOAT_RINT

#define FLOAT_CLASS_SIGNALING_NAN      0x001
#define FLOAT_CLASS_QUIET_NAN          0x002
#define FLOAT_CLASS_NEGATIVE_INFINITY  0x004
#define FLOAT_CLASS_NEGATIVE_NORMAL    0x008
#define FLOAT_CLASS_NEGATIVE_SUBNORMAL 0x010
#define FLOAT_CLASS_NEGATIVE_ZERO      0x020
#define FLOAT_CLASS_POSITIVE_INFINITY  0x040
#define FLOAT_CLASS_POSITIVE_NORMAL    0x080
#define FLOAT_CLASS_POSITIVE_SUBNORMAL 0x100
#define FLOAT_CLASS_POSITIVE_ZERO      0x200

#define FLOAT_CLASS(name, bits)                                      \
uint ## bits ## _t float_ ## name (uint ## bits ## _t arg,           \
                                   float_status *status)             \
{                                                                    \
    if (float ## bits ## _is_signaling_nan(arg, status)) {           \
        return FLOAT_CLASS_SIGNALING_NAN;                            \
    } else if (float ## bits ## _is_quiet_nan(arg, status)) {        \
        return FLOAT_CLASS_QUIET_NAN;                                \
    } else if (float ## bits ## _is_neg(arg)) {                      \
        if (float ## bits ## _is_infinity(arg)) {                    \
            return FLOAT_CLASS_NEGATIVE_INFINITY;                    \
        } else if (float ## bits ## _is_zero(arg)) {                 \
            return FLOAT_CLASS_NEGATIVE_ZERO;                        \
        } else if (float ## bits ## _is_zero_or_denormal(arg)) {     \
            return FLOAT_CLASS_NEGATIVE_SUBNORMAL;                   \
        } else {                                                     \
            return FLOAT_CLASS_NEGATIVE_NORMAL;                      \
        }                                                            \
    } else {                                                         \
        if (float ## bits ## _is_infinity(arg)) {                    \
            return FLOAT_CLASS_POSITIVE_INFINITY;                    \
        } else if (float ## bits ## _is_zero(arg)) {                 \
            return FLOAT_CLASS_POSITIVE_ZERO;                        \
        } else if (float ## bits ## _is_zero_or_denormal(arg)) {     \
            return FLOAT_CLASS_POSITIVE_SUBNORMAL;                   \
        } else {                                                     \
            return FLOAT_CLASS_POSITIVE_NORMAL;                      \
        }                                                            \
    }                                                                \
}                                                                    \
                                                                     \
uint ## bits ## _t helper_float_ ## name (CPUMIPSState *env,         \
                                          uint ## bits ## _t arg)    \
{                                                                    \
    return float_ ## name(arg, &env->active_fpu.fp_status);          \
}

FLOAT_CLASS(class_s, 32)
FLOAT_CLASS(class_d, 64)
#undef FLOAT_CLASS

/* binary operations */
#define FLOAT_BINOP(name)                                          \
uint64_t helper_float_ ## name ## _d(CPUMIPSState *env,            \
                                     uint64_t fdt0, uint64_t fdt1) \
{                                                                  \
    uint64_t dt2;                                                  \
                                                                   \
    dt2 = float64_ ## name (fdt0, fdt1, &env->active_fpu.fp_status);     \
    update_fcr31(env, GETPC());                                    \
    return dt2;                                                    \
}                                                                  \
                                                                   \
uint32_t helper_float_ ## name ## _s(CPUMIPSState *env,            \
                                     uint32_t fst0, uint32_t fst1) \
{                                                                  \
    uint32_t wt2;                                                  \
                                                                   \
    wt2 = float32_ ## name (fst0, fst1, &env->active_fpu.fp_status);     \
    update_fcr31(env, GETPC());                                    \
    return wt2;                                                    \
}                                                                  \
                                                                   \
uint64_t helper_float_ ## name ## _ps(CPUMIPSState *env,           \
                                      uint64_t fdt0,               \
                                      uint64_t fdt1)               \
{                                                                  \
    uint32_t fst0 = fdt0 & 0XFFFFFFFF;                             \
    uint32_t fsth0 = fdt0 >> 32;                                   \
    uint32_t fst1 = fdt1 & 0XFFFFFFFF;                             \
    uint32_t fsth1 = fdt1 >> 32;                                   \
    uint32_t wt2;                                                  \
    uint32_t wth2;                                                 \
                                                                   \
    wt2 = float32_ ## name (fst0, fst1, &env->active_fpu.fp_status);     \
    wth2 = float32_ ## name (fsth0, fsth1, &env->active_fpu.fp_status);  \
    update_fcr31(env, GETPC());                                    \
    return ((uint64_t)wth2 << 32) | wt2;                           \
}

FLOAT_BINOP(add)
FLOAT_BINOP(sub)
FLOAT_BINOP(mul)
FLOAT_BINOP(div)
#undef FLOAT_BINOP

/* MIPS specific binary operations */
uint64_t helper_float_recip2_d(CPUMIPSState *env, uint64_t fdt0, uint64_t fdt2)
{
    fdt2 = float64_mul(fdt0, fdt2, &env->active_fpu.fp_status);
    fdt2 = float64_chs(float64_sub(fdt2, float64_one, &env->active_fpu.fp_status));
    update_fcr31(env, GETPC());
    return fdt2;
}

uint32_t helper_float_recip2_s(CPUMIPSState *env, uint32_t fst0, uint32_t fst2)
{
    fst2 = float32_mul(fst0, fst2, &env->active_fpu.fp_status);
    fst2 = float32_chs(float32_sub(fst2, float32_one, &env->active_fpu.fp_status));
    update_fcr31(env, GETPC());
    return fst2;
}

uint64_t helper_float_recip2_ps(CPUMIPSState *env, uint64_t fdt0, uint64_t fdt2)
{
    uint32_t fst0 = fdt0 & 0XFFFFFFFF;
    uint32_t fsth0 = fdt0 >> 32;
    uint32_t fst2 = fdt2 & 0XFFFFFFFF;
    uint32_t fsth2 = fdt2 >> 32;

    fst2 = float32_mul(fst0, fst2, &env->active_fpu.fp_status);
    fsth2 = float32_mul(fsth0, fsth2, &env->active_fpu.fp_status);
    fst2 = float32_chs(float32_sub(fst2, float32_one, &env->active_fpu.fp_status));
    fsth2 = float32_chs(float32_sub(fsth2, float32_one, &env->active_fpu.fp_status));
    update_fcr31(env, GETPC());
    return ((uint64_t)fsth2 << 32) | fst2;
}

uint64_t helper_float_rsqrt2_d(CPUMIPSState *env, uint64_t fdt0, uint64_t fdt2)
{
    fdt2 = float64_mul(fdt0, fdt2, &env->active_fpu.fp_status);
    fdt2 = float64_sub(fdt2, float64_one, &env->active_fpu.fp_status);
    fdt2 = float64_chs(float64_div(fdt2, FLOAT_TWO64, &env->active_fpu.fp_status));
    update_fcr31(env, GETPC());
    return fdt2;
}

uint32_t helper_float_rsqrt2_s(CPUMIPSState *env, uint32_t fst0, uint32_t fst2)
{
    fst2 = float32_mul(fst0, fst2, &env->active_fpu.fp_status);
    fst2 = float32_sub(fst2, float32_one, &env->active_fpu.fp_status);
    fst2 = float32_chs(float32_div(fst2, FLOAT_TWO32, &env->active_fpu.fp_status));
    update_fcr31(env, GETPC());
    return fst2;
}

uint64_t helper_float_rsqrt2_ps(CPUMIPSState *env, uint64_t fdt0, uint64_t fdt2)
{
    uint32_t fst0 = fdt0 & 0XFFFFFFFF;
    uint32_t fsth0 = fdt0 >> 32;
    uint32_t fst2 = fdt2 & 0XFFFFFFFF;
    uint32_t fsth2 = fdt2 >> 32;

    fst2 = float32_mul(fst0, fst2, &env->active_fpu.fp_status);
    fsth2 = float32_mul(fsth0, fsth2, &env->active_fpu.fp_status);
    fst2 = float32_sub(fst2, float32_one, &env->active_fpu.fp_status);
    fsth2 = float32_sub(fsth2, float32_one, &env->active_fpu.fp_status);
    fst2 = float32_chs(float32_div(fst2, FLOAT_TWO32, &env->active_fpu.fp_status));
    fsth2 = float32_chs(float32_div(fsth2, FLOAT_TWO32, &env->active_fpu.fp_status));
    update_fcr31(env, GETPC());
    return ((uint64_t)fsth2 << 32) | fst2;
}

uint64_t helper_float_addr_ps(CPUMIPSState *env, uint64_t fdt0, uint64_t fdt1)
{
    uint32_t fst0 = fdt0 & 0XFFFFFFFF;
    uint32_t fsth0 = fdt0 >> 32;
    uint32_t fst1 = fdt1 & 0XFFFFFFFF;
    uint32_t fsth1 = fdt1 >> 32;
    uint32_t fst2;
    uint32_t fsth2;

    fst2 = float32_add (fst0, fsth0, &env->active_fpu.fp_status);
    fsth2 = float32_add (fst1, fsth1, &env->active_fpu.fp_status);
    update_fcr31(env, GETPC());
    return ((uint64_t)fsth2 << 32) | fst2;
}

uint64_t helper_float_mulr_ps(CPUMIPSState *env, uint64_t fdt0, uint64_t fdt1)
{
    uint32_t fst0 = fdt0 & 0XFFFFFFFF;
    uint32_t fsth0 = fdt0 >> 32;
    uint32_t fst1 = fdt1 & 0XFFFFFFFF;
    uint32_t fsth1 = fdt1 >> 32;
    uint32_t fst2;
    uint32_t fsth2;

    fst2 = float32_mul (fst0, fsth0, &env->active_fpu.fp_status);
    fsth2 = float32_mul (fst1, fsth1, &env->active_fpu.fp_status);
    update_fcr31(env, GETPC());
    return ((uint64_t)fsth2 << 32) | fst2;
}

#define FLOAT_MINMAX(name, bits, minmaxfunc)                            \
uint ## bits ## _t helper_float_ ## name (CPUMIPSState *env,            \
                                          uint ## bits ## _t fs,        \
                                          uint ## bits ## _t ft)        \
{                                                                       \
    uint ## bits ## _t fdret;                                           \
                                                                        \
    fdret = float ## bits ## _ ## minmaxfunc(fs, ft,                    \
                                           &env->active_fpu.fp_status); \
    update_fcr31(env, GETPC());                                         \
    return fdret;                                                       \
}

FLOAT_MINMAX(max_s, 32, maxnum)
FLOAT_MINMAX(max_d, 64, maxnum)
FLOAT_MINMAX(maxa_s, 32, maxnummag)
FLOAT_MINMAX(maxa_d, 64, maxnummag)

FLOAT_MINMAX(min_s, 32, minnum)
FLOAT_MINMAX(min_d, 64, minnum)
FLOAT_MINMAX(mina_s, 32, minnummag)
FLOAT_MINMAX(mina_d, 64, minnummag)
#undef FLOAT_MINMAX

/* ternary operations */
#define UNFUSED_FMA(prefix, a, b, c, flags)                          \
{                                                                    \
    a = prefix##_mul(a, b, &env->active_fpu.fp_status);              \
    if ((flags) & float_muladd_negate_c) {                           \
        a = prefix##_sub(a, c, &env->active_fpu.fp_status);          \
    } else {                                                         \
        a = prefix##_add(a, c, &env->active_fpu.fp_status);          \
    }                                                                \
    if ((flags) & float_muladd_negate_result) {                      \
        a = prefix##_chs(a);                                         \
    }                                                                \
}

/* FMA based operations */
#define FLOAT_FMA(name, type)                                        \
uint64_t helper_float_ ## name ## _d(CPUMIPSState *env,              \
                                     uint64_t fdt0, uint64_t fdt1,   \
                                     uint64_t fdt2)                  \
{                                                                    \
    UNFUSED_FMA(float64, fdt0, fdt1, fdt2, type);                    \
    update_fcr31(env, GETPC());                                      \
    return fdt0;                                                     \
}                                                                    \
                                                                     \
uint32_t helper_float_ ## name ## _s(CPUMIPSState *env,              \
                                     uint32_t fst0, uint32_t fst1,   \
                                     uint32_t fst2)                  \
{                                                                    \
    UNFUSED_FMA(float32, fst0, fst1, fst2, type);                    \
    update_fcr31(env, GETPC());                                      \
    return fst0;                                                     \
}                                                                    \
                                                                     \
uint64_t helper_float_ ## name ## _ps(CPUMIPSState *env,             \
                                      uint64_t fdt0, uint64_t fdt1,  \
                                      uint64_t fdt2)                 \
{                                                                    \
    uint32_t fst0 = fdt0 & 0XFFFFFFFF;                               \
    uint32_t fsth0 = fdt0 >> 32;                                     \
    uint32_t fst1 = fdt1 & 0XFFFFFFFF;                               \
    uint32_t fsth1 = fdt1 >> 32;                                     \
    uint32_t fst2 = fdt2 & 0XFFFFFFFF;                               \
    uint32_t fsth2 = fdt2 >> 32;                                     \
                                                                     \
    UNFUSED_FMA(float32, fst0, fst1, fst2, type);                    \
    UNFUSED_FMA(float32, fsth0, fsth1, fsth2, type);                 \
    update_fcr31(env, GETPC());                                      \
    return ((uint64_t)fsth0 << 32) | fst0;                           \
}
FLOAT_FMA(madd, 0)
FLOAT_FMA(msub, float_muladd_negate_c)
FLOAT_FMA(nmadd, float_muladd_negate_result)
FLOAT_FMA(nmsub, float_muladd_negate_result | float_muladd_negate_c)
#undef FLOAT_FMA

#define FLOAT_FMADDSUB(name, bits, muladd_arg)                          \
uint ## bits ## _t helper_float_ ## name (CPUMIPSState *env,            \
                                          uint ## bits ## _t fs,        \
                                          uint ## bits ## _t ft,        \
                                          uint ## bits ## _t fd)        \
{                                                                       \
    uint ## bits ## _t fdret;                                           \
                                                                        \
    fdret = float ## bits ## _muladd(fs, ft, fd, muladd_arg,            \
                                     &env->active_fpu.fp_status);       \
    update_fcr31(env, GETPC());                                         \
    return fdret;                                                       \
}

FLOAT_FMADDSUB(maddf_s, 32, 0)
FLOAT_FMADDSUB(maddf_d, 64, 0)
FLOAT_FMADDSUB(msubf_s, 32, float_muladd_negate_product)
FLOAT_FMADDSUB(msubf_d, 64, float_muladd_negate_product)
#undef FLOAT_FMADDSUB

/* compare operations */
#define FOP_COND_D(op, cond)                                   \
void helper_cmp_d_ ## op(CPUMIPSState *env, uint64_t fdt0,     \
                         uint64_t fdt1, int cc)                \
{                                                              \
    int c;                                                     \
    c = cond;                                                  \
    update_fcr31(env, GETPC());                                \
    if (c)                                                     \
        SET_FP_COND(cc, env->active_fpu);                      \
    else                                                       \
        CLEAR_FP_COND(cc, env->active_fpu);                    \
}                                                              \
void helper_cmpabs_d_ ## op(CPUMIPSState *env, uint64_t fdt0,  \
                            uint64_t fdt1, int cc)             \
{                                                              \
    int c;                                                     \
    fdt0 = float64_abs(fdt0);                                  \
    fdt1 = float64_abs(fdt1);                                  \
    c = cond;                                                  \
    update_fcr31(env, GETPC());                                \
    if (c)                                                     \
        SET_FP_COND(cc, env->active_fpu);                      \
    else                                                       \
        CLEAR_FP_COND(cc, env->active_fpu);                    \
}

/* NOTE: the comma operator will make "cond" to eval to false,
 * but float64_unordered_quiet() is still called. */
FOP_COND_D(f,   (float64_unordered_quiet(fdt1, fdt0, &env->active_fpu.fp_status), 0))
FOP_COND_D(un,  float64_unordered_quiet(fdt1, fdt0, &env->active_fpu.fp_status))
FOP_COND_D(eq,  float64_eq_quiet(fdt0, fdt1, &env->active_fpu.fp_status))
FOP_COND_D(ueq, float64_unordered_quiet(fdt1, fdt0, &env->active_fpu.fp_status)  || float64_eq_quiet(fdt0, fdt1, &env->active_fpu.fp_status))
FOP_COND_D(olt, float64_lt_quiet(fdt0, fdt1, &env->active_fpu.fp_status))
FOP_COND_D(ult, float64_unordered_quiet(fdt1, fdt0, &env->active_fpu.fp_status)  || float64_lt_quiet(fdt0, fdt1, &env->active_fpu.fp_status))
FOP_COND_D(ole, float64_le_quiet(fdt0, fdt1, &env->active_fpu.fp_status))
FOP_COND_D(ule, float64_unordered_quiet(fdt1, fdt0, &env->active_fpu.fp_status)  || float64_le_quiet(fdt0, fdt1, &env->active_fpu.fp_status))
/* NOTE: the comma operator will make "cond" to eval to false,
 * but float64_unordered() is still called. */
FOP_COND_D(sf,  (float64_unordered(fdt1, fdt0, &env->active_fpu.fp_status), 0))
FOP_COND_D(ngle,float64_unordered(fdt1, fdt0, &env->active_fpu.fp_status))
FOP_COND_D(seq, float64_eq(fdt0, fdt1, &env->active_fpu.fp_status))
FOP_COND_D(ngl, float64_unordered(fdt1, fdt0, &env->active_fpu.fp_status)  || float64_eq(fdt0, fdt1, &env->active_fpu.fp_status))
FOP_COND_D(lt,  float64_lt(fdt0, fdt1, &env->active_fpu.fp_status))
FOP_COND_D(nge, float64_unordered(fdt1, fdt0, &env->active_fpu.fp_status)  || float64_lt(fdt0, fdt1, &env->active_fpu.fp_status))
FOP_COND_D(le,  float64_le(fdt0, fdt1, &env->active_fpu.fp_status))
FOP_COND_D(ngt, float64_unordered(fdt1, fdt0, &env->active_fpu.fp_status)  || float64_le(fdt0, fdt1, &env->active_fpu.fp_status))

#define FOP_COND_S(op, cond)                                   \
void helper_cmp_s_ ## op(CPUMIPSState *env, uint32_t fst0,     \
                         uint32_t fst1, int cc)                \
{                                                              \
    int c;                                                     \
    c = cond;                                                  \
    update_fcr31(env, GETPC());                                \
    if (c)                                                     \
        SET_FP_COND(cc, env->active_fpu);                      \
    else                                                       \
        CLEAR_FP_COND(cc, env->active_fpu);                    \
}                                                              \
void helper_cmpabs_s_ ## op(CPUMIPSState *env, uint32_t fst0,  \
                            uint32_t fst1, int cc)             \
{                                                              \
    int c;                                                     \
    fst0 = float32_abs(fst0);                                  \
    fst1 = float32_abs(fst1);                                  \
    c = cond;                                                  \
    update_fcr31(env, GETPC());                                \
    if (c)                                                     \
        SET_FP_COND(cc, env->active_fpu);                      \
    else                                                       \
        CLEAR_FP_COND(cc, env->active_fpu);                    \
}

/* NOTE: the comma operator will make "cond" to eval to false,
 * but float32_unordered_quiet() is still called. */
FOP_COND_S(f,   (float32_unordered_quiet(fst1, fst0, &env->active_fpu.fp_status), 0))
FOP_COND_S(un,  float32_unordered_quiet(fst1, fst0, &env->active_fpu.fp_status))
FOP_COND_S(eq,  float32_eq_quiet(fst0, fst1, &env->active_fpu.fp_status))
FOP_COND_S(ueq, float32_unordered_quiet(fst1, fst0, &env->active_fpu.fp_status)  || float32_eq_quiet(fst0, fst1, &env->active_fpu.fp_status))
FOP_COND_S(olt, float32_lt_quiet(fst0, fst1, &env->active_fpu.fp_status))
FOP_COND_S(ult, float32_unordered_quiet(fst1, fst0, &env->active_fpu.fp_status)  || float32_lt_quiet(fst0, fst1, &env->active_fpu.fp_status))
FOP_COND_S(ole, float32_le_quiet(fst0, fst1, &env->active_fpu.fp_status))
FOP_COND_S(ule, float32_unordered_quiet(fst1, fst0, &env->active_fpu.fp_status)  || float32_le_quiet(fst0, fst1, &env->active_fpu.fp_status))
/* NOTE: the comma operator will make "cond" to eval to false,
 * but float32_unordered() is still called. */
FOP_COND_S(sf,  (float32_unordered(fst1, fst0, &env->active_fpu.fp_status), 0))
FOP_COND_S(ngle,float32_unordered(fst1, fst0, &env->active_fpu.fp_status))
FOP_COND_S(seq, float32_eq(fst0, fst1, &env->active_fpu.fp_status))
FOP_COND_S(ngl, float32_unordered(fst1, fst0, &env->active_fpu.fp_status)  || float32_eq(fst0, fst1, &env->active_fpu.fp_status))
FOP_COND_S(lt,  float32_lt(fst0, fst1, &env->active_fpu.fp_status))
FOP_COND_S(nge, float32_unordered(fst1, fst0, &env->active_fpu.fp_status)  || float32_lt(fst0, fst1, &env->active_fpu.fp_status))
FOP_COND_S(le,  float32_le(fst0, fst1, &env->active_fpu.fp_status))
FOP_COND_S(ngt, float32_unordered(fst1, fst0, &env->active_fpu.fp_status)  || float32_le(fst0, fst1, &env->active_fpu.fp_status))

#define FOP_COND_PS(op, condl, condh)                           \
void helper_cmp_ps_ ## op(CPUMIPSState *env, uint64_t fdt0,     \
                          uint64_t fdt1, int cc)                \
{                                                               \
    uint32_t fst0, fsth0, fst1, fsth1;                          \
    int ch, cl;                                                 \
    fst0 = fdt0 & 0XFFFFFFFF;                                   \
    fsth0 = fdt0 >> 32;                                         \
    fst1 = fdt1 & 0XFFFFFFFF;                                   \
    fsth1 = fdt1 >> 32;                                         \
    cl = condl;                                                 \
    ch = condh;                                                 \
    update_fcr31(env, GETPC());                                 \
    if (cl)                                                     \
        SET_FP_COND(cc, env->active_fpu);                       \
    else                                                        \
        CLEAR_FP_COND(cc, env->active_fpu);                     \
    if (ch)                                                     \
        SET_FP_COND(cc + 1, env->active_fpu);                   \
    else                                                        \
        CLEAR_FP_COND(cc + 1, env->active_fpu);                 \
}                                                               \
void helper_cmpabs_ps_ ## op(CPUMIPSState *env, uint64_t fdt0,  \
                             uint64_t fdt1, int cc)             \
{                                                               \
    uint32_t fst0, fsth0, fst1, fsth1;                          \
    int ch, cl;                                                 \
    fst0 = float32_abs(fdt0 & 0XFFFFFFFF);                      \
    fsth0 = float32_abs(fdt0 >> 32);                            \
    fst1 = float32_abs(fdt1 & 0XFFFFFFFF);                      \
    fsth1 = float32_abs(fdt1 >> 32);                            \
    cl = condl;                                                 \
    ch = condh;                                                 \
    update_fcr31(env, GETPC());                                 \
    if (cl)                                                     \
        SET_FP_COND(cc, env->active_fpu);                       \
    else                                                        \
        CLEAR_FP_COND(cc, env->active_fpu);                     \
    if (ch)                                                     \
        SET_FP_COND(cc + 1, env->active_fpu);                   \
    else                                                        \
        CLEAR_FP_COND(cc + 1, env->active_fpu);                 \
}

/* NOTE: the comma operator will make "cond" to eval to false,
 * but float32_unordered_quiet() is still called. */
FOP_COND_PS(f,   (float32_unordered_quiet(fst1, fst0, &env->active_fpu.fp_status), 0),
                 (float32_unordered_quiet(fsth1, fsth0, &env->active_fpu.fp_status), 0))
FOP_COND_PS(un,  float32_unordered_quiet(fst1, fst0, &env->active_fpu.fp_status),
                 float32_unordered_quiet(fsth1, fsth0, &env->active_fpu.fp_status))
FOP_COND_PS(eq,  float32_eq_quiet(fst0, fst1, &env->active_fpu.fp_status),
                 float32_eq_quiet(fsth0, fsth1, &env->active_fpu.fp_status))
FOP_COND_PS(ueq, float32_unordered_quiet(fst1, fst0, &env->active_fpu.fp_status)    || float32_eq_quiet(fst0, fst1, &env->active_fpu.fp_status),
                 float32_unordered_quiet(fsth1, fsth0, &env->active_fpu.fp_status)  || float32_eq_quiet(fsth0, fsth1, &env->active_fpu.fp_status))
FOP_COND_PS(olt, float32_lt_quiet(fst0, fst1, &env->active_fpu.fp_status),
                 float32_lt_quiet(fsth0, fsth1, &env->active_fpu.fp_status))
FOP_COND_PS(ult, float32_unordered_quiet(fst1, fst0, &env->active_fpu.fp_status)    || float32_lt_quiet(fst0, fst1, &env->active_fpu.fp_status),
                 float32_unordered_quiet(fsth1, fsth0, &env->active_fpu.fp_status)  || float32_lt_quiet(fsth0, fsth1, &env->active_fpu.fp_status))
FOP_COND_PS(ole, float32_le_quiet(fst0, fst1, &env->active_fpu.fp_status),
                 float32_le_quiet(fsth0, fsth1, &env->active_fpu.fp_status))
FOP_COND_PS(ule, float32_unordered_quiet(fst1, fst0, &env->active_fpu.fp_status)    || float32_le_quiet(fst0, fst1, &env->active_fpu.fp_status),
                 float32_unordered_quiet(fsth1, fsth0, &env->active_fpu.fp_status)  || float32_le_quiet(fsth0, fsth1, &env->active_fpu.fp_status))
/* NOTE: the comma operator will make "cond" to eval to false,
 * but float32_unordered() is still called. */
FOP_COND_PS(sf,  (float32_unordered(fst1, fst0, &env->active_fpu.fp_status), 0),
                 (float32_unordered(fsth1, fsth0, &env->active_fpu.fp_status), 0))
FOP_COND_PS(ngle,float32_unordered(fst1, fst0, &env->active_fpu.fp_status),
                 float32_unordered(fsth1, fsth0, &env->active_fpu.fp_status))
FOP_COND_PS(seq, float32_eq(fst0, fst1, &env->active_fpu.fp_status),
                 float32_eq(fsth0, fsth1, &env->active_fpu.fp_status))
FOP_COND_PS(ngl, float32_unordered(fst1, fst0, &env->active_fpu.fp_status)    || float32_eq(fst0, fst1, &env->active_fpu.fp_status),
                 float32_unordered(fsth1, fsth0, &env->active_fpu.fp_status)  || float32_eq(fsth0, fsth1, &env->active_fpu.fp_status))
FOP_COND_PS(lt,  float32_lt(fst0, fst1, &env->active_fpu.fp_status),
                 float32_lt(fsth0, fsth1, &env->active_fpu.fp_status))
FOP_COND_PS(nge, float32_unordered(fst1, fst0, &env->active_fpu.fp_status)    || float32_lt(fst0, fst1, &env->active_fpu.fp_status),
                 float32_unordered(fsth1, fsth0, &env->active_fpu.fp_status)  || float32_lt(fsth0, fsth1, &env->active_fpu.fp_status))
FOP_COND_PS(le,  float32_le(fst0, fst1, &env->active_fpu.fp_status),
                 float32_le(fsth0, fsth1, &env->active_fpu.fp_status))
FOP_COND_PS(ngt, float32_unordered(fst1, fst0, &env->active_fpu.fp_status)    || float32_le(fst0, fst1, &env->active_fpu.fp_status),
                 float32_unordered(fsth1, fsth0, &env->active_fpu.fp_status)  || float32_le(fsth0, fsth1, &env->active_fpu.fp_status))

/* R6 compare operations */
#define FOP_CONDN_D(op, cond)                                       \
uint64_t helper_r6_cmp_d_ ## op(CPUMIPSState * env, uint64_t fdt0,  \
                         uint64_t fdt1)                             \
{                                                                   \
    uint64_t c;                                                     \
    c = cond;                                                       \
    update_fcr31(env, GETPC());                                     \
    if (c) {                                                        \
        return -1;                                                  \
    } else {                                                        \
        return 0;                                                   \
    }                                                               \
}

/* NOTE: the comma operator will make "cond" to eval to false,
 * but float64_unordered_quiet() is still called. */
FOP_CONDN_D(af,  (float64_unordered_quiet(fdt1, fdt0, &env->active_fpu.fp_status), 0))
FOP_CONDN_D(un,  (float64_unordered_quiet(fdt1, fdt0, &env->active_fpu.fp_status)))
FOP_CONDN_D(eq,  (float64_eq_quiet(fdt0, fdt1, &env->active_fpu.fp_status)))
FOP_CONDN_D(ueq, (float64_unordered_quiet(fdt1, fdt0, &env->active_fpu.fp_status)
                  || float64_eq_quiet(fdt0, fdt1, &env->active_fpu.fp_status)))
FOP_CONDN_D(lt,  (float64_lt_quiet(fdt0, fdt1, &env->active_fpu.fp_status)))
FOP_CONDN_D(ult, (float64_unordered_quiet(fdt1, fdt0, &env->active_fpu.fp_status)
                  || float64_lt_quiet(fdt0, fdt1, &env->active_fpu.fp_status)))
FOP_CONDN_D(le,  (float64_le_quiet(fdt0, fdt1, &env->active_fpu.fp_status)))
FOP_CONDN_D(ule, (float64_unordered_quiet(fdt1, fdt0, &env->active_fpu.fp_status)
                  || float64_le_quiet(fdt0, fdt1, &env->active_fpu.fp_status)))
/* NOTE: the comma operator will make "cond" to eval to false,
 * but float64_unordered() is still called. */
FOP_CONDN_D(saf,  (float64_unordered(fdt1, fdt0, &env->active_fpu.fp_status), 0))
FOP_CONDN_D(sun,  (float64_unordered(fdt1, fdt0, &env->active_fpu.fp_status)))
FOP_CONDN_D(seq,  (float64_eq(fdt0, fdt1, &env->active_fpu.fp_status)))
FOP_CONDN_D(sueq, (float64_unordered(fdt1, fdt0, &env->active_fpu.fp_status)
                   || float64_eq(fdt0, fdt1, &env->active_fpu.fp_status)))
FOP_CONDN_D(slt,  (float64_lt(fdt0, fdt1, &env->active_fpu.fp_status)))
FOP_CONDN_D(sult, (float64_unordered(fdt1, fdt0, &env->active_fpu.fp_status)
                   || float64_lt(fdt0, fdt1, &env->active_fpu.fp_status)))
FOP_CONDN_D(sle,  (float64_le(fdt0, fdt1, &env->active_fpu.fp_status)))
FOP_CONDN_D(sule, (float64_unordered(fdt1, fdt0, &env->active_fpu.fp_status)
                   || float64_le(fdt0, fdt1, &env->active_fpu.fp_status)))
FOP_CONDN_D(or,   (float64_le_quiet(fdt1, fdt0, &env->active_fpu.fp_status)
                   || float64_le_quiet(fdt0, fdt1, &env->active_fpu.fp_status)))
FOP_CONDN_D(une,  (float64_unordered_quiet(fdt1, fdt0, &env->active_fpu.fp_status)
                   || float64_lt_quiet(fdt1, fdt0, &env->active_fpu.fp_status)
                   || float64_lt_quiet(fdt0, fdt1, &env->active_fpu.fp_status)))
FOP_CONDN_D(ne,   (float64_lt_quiet(fdt1, fdt0, &env->active_fpu.fp_status)
                   || float64_lt_quiet(fdt0, fdt1, &env->active_fpu.fp_status)))
FOP_CONDN_D(sor,  (float64_le(fdt1, fdt0, &env->active_fpu.fp_status)
                   || float64_le(fdt0, fdt1, &env->active_fpu.fp_status)))
FOP_CONDN_D(sune, (float64_unordered(fdt1, fdt0, &env->active_fpu.fp_status)
                   || float64_lt(fdt1, fdt0, &env->active_fpu.fp_status)
                   || float64_lt(fdt0, fdt1, &env->active_fpu.fp_status)))
FOP_CONDN_D(sne,  (float64_lt(fdt1, fdt0, &env->active_fpu.fp_status)
                   || float64_lt(fdt0, fdt1, &env->active_fpu.fp_status)))

#define FOP_CONDN_S(op, cond)                                       \
uint32_t helper_r6_cmp_s_ ## op(CPUMIPSState * env, uint32_t fst0,  \
                         uint32_t fst1)                             \
{                                                                   \
    uint64_t c;                                                     \
    c = cond;                                                       \
    update_fcr31(env, GETPC());                                     \
    if (c) {                                                        \
        return -1;                                                  \
    } else {                                                        \
        return 0;                                                   \
    }                                                               \
}

/* NOTE: the comma operator will make "cond" to eval to false,
 * but float32_unordered_quiet() is still called. */
FOP_CONDN_S(af,   (float32_unordered_quiet(fst1, fst0, &env->active_fpu.fp_status), 0))
FOP_CONDN_S(un,   (float32_unordered_quiet(fst1, fst0, &env->active_fpu.fp_status)))
FOP_CONDN_S(eq,   (float32_eq_quiet(fst0, fst1, &env->active_fpu.fp_status)))
FOP_CONDN_S(ueq,  (float32_unordered_quiet(fst1, fst0, &env->active_fpu.fp_status)
                   || float32_eq_quiet(fst0, fst1, &env->active_fpu.fp_status)))
FOP_CONDN_S(lt,   (float32_lt_quiet(fst0, fst1, &env->active_fpu.fp_status)))
FOP_CONDN_S(ult,  (float32_unordered_quiet(fst1, fst0, &env->active_fpu.fp_status)
                   || float32_lt_quiet(fst0, fst1, &env->active_fpu.fp_status)))
FOP_CONDN_S(le,   (float32_le_quiet(fst0, fst1, &env->active_fpu.fp_status)))
FOP_CONDN_S(ule,  (float32_unordered_quiet(fst1, fst0, &env->active_fpu.fp_status)
                   || float32_le_quiet(fst0, fst1, &env->active_fpu.fp_status)))
/* NOTE: the comma operator will make "cond" to eval to false,
 * but float32_unordered() is still called. */
FOP_CONDN_S(saf,  (float32_unordered(fst1, fst0, &env->active_fpu.fp_status), 0))
FOP_CONDN_S(sun,  (float32_unordered(fst1, fst0, &env->active_fpu.fp_status)))
FOP_CONDN_S(seq,  (float32_eq(fst0, fst1, &env->active_fpu.fp_status)))
FOP_CONDN_S(sueq, (float32_unordered(fst1, fst0, &env->active_fpu.fp_status)
                   || float32_eq(fst0, fst1, &env->active_fpu.fp_status)))
FOP_CONDN_S(slt,  (float32_lt(fst0, fst1, &env->active_fpu.fp_status)))
FOP_CONDN_S(sult, (float32_unordered(fst1, fst0, &env->active_fpu.fp_status)
                   || float32_lt(fst0, fst1, &env->active_fpu.fp_status)))
FOP_CONDN_S(sle,  (float32_le(fst0, fst1, &env->active_fpu.fp_status)))
FOP_CONDN_S(sule, (float32_unordered(fst1, fst0, &env->active_fpu.fp_status)
                   || float32_le(fst0, fst1, &env->active_fpu.fp_status)))
FOP_CONDN_S(or,   (float32_le_quiet(fst1, fst0, &env->active_fpu.fp_status)
                   || float32_le_quiet(fst0, fst1, &env->active_fpu.fp_status)))
FOP_CONDN_S(une,  (float32_unordered_quiet(fst1, fst0, &env->active_fpu.fp_status)
                   || float32_lt_quiet(fst1, fst0, &env->active_fpu.fp_status)
                   || float32_lt_quiet(fst0, fst1, &env->active_fpu.fp_status)))
FOP_CONDN_S(ne,   (float32_lt_quiet(fst1, fst0, &env->active_fpu.fp_status)
                   || float32_lt_quiet(fst0, fst1, &env->active_fpu.fp_status)))
FOP_CONDN_S(sor,  (float32_le(fst1, fst0, &env->active_fpu.fp_status)
                   || float32_le(fst0, fst1, &env->active_fpu.fp_status)))
FOP_CONDN_S(sune, (float32_unordered(fst1, fst0, &env->active_fpu.fp_status)
                   || float32_lt(fst1, fst0, &env->active_fpu.fp_status)
                   || float32_lt(fst0, fst1, &env->active_fpu.fp_status)))
FOP_CONDN_S(sne,  (float32_lt(fst1, fst0, &env->active_fpu.fp_status)
                   || float32_lt(fst0, fst1, &env->active_fpu.fp_status)))

/* MSA */
/* Data format min and max values */
#define DF_BITS(df) (1 << ((df) + 3))

/* Element-by-element access macros */
#define DF_ELEMENTS(df) (MSA_WRLEN / DF_BITS(df))

#if !defined(CONFIG_USER_ONLY)
#define MEMOP_IDX(DF)                                           \
        TCGMemOpIdx oi = make_memop_idx(MO_TE | DF | MO_UNALN,  \
                                        cpu_mmu_index(env, false));
#else
#define MEMOP_IDX(DF)
#endif

#define MSA_LD_DF(DF, TYPE, LD_INSN, ...)                               \
void helper_msa_ld_ ## TYPE(CPUMIPSState *env, uint32_t wd,             \
                            target_ulong addr)                          \
{                                                                       \
    wr_t *pwd = &(env->active_fpu.fpr[wd].wr);                          \
    wr_t wx;                                                            \
    int i;                                                              \
    MEMOP_IDX(DF)                                                       \
    for (i = 0; i < DF_ELEMENTS(DF); i++) {                             \
        wx.TYPE[i] = LD_INSN(env, addr + (i << DF), ##__VA_ARGS__);     \
    }                                                                   \
    memcpy(pwd, &wx, sizeof(wr_t));                                     \
}

#if !defined(CONFIG_USER_ONLY)
MSA_LD_DF(DF_BYTE,   b, helper_ret_ldub_mmu, oi, GETPC())
MSA_LD_DF(DF_HALF,   h, helper_ret_lduw_mmu, oi, GETPC())
MSA_LD_DF(DF_WORD,   w, helper_ret_ldul_mmu, oi, GETPC())
MSA_LD_DF(DF_DOUBLE, d, helper_ret_ldq_mmu,  oi, GETPC())
#else
MSA_LD_DF(DF_BYTE,   b, cpu_ldub_data)
MSA_LD_DF(DF_HALF,   h, cpu_lduw_data)
MSA_LD_DF(DF_WORD,   w, cpu_ldl_data)
MSA_LD_DF(DF_DOUBLE, d, cpu_ldq_data)
#endif

#define MSA_PAGESPAN(x) \
        ((((x) & ~TARGET_PAGE_MASK) + MSA_WRLEN/8 - 1) >= TARGET_PAGE_SIZE)

static inline void ensure_writable_pages(CPUMIPSState *env,
                                         target_ulong addr,
                                         int mmu_idx,
                                         uintptr_t retaddr)
{
#if !defined(CONFIG_USER_ONLY)
    target_ulong page_addr;
    if (unlikely(MSA_PAGESPAN(addr))) {
        /* first page */
        probe_write(env, addr, 0, mmu_idx, retaddr);
        /* second page */
        page_addr = (addr & TARGET_PAGE_MASK) + TARGET_PAGE_SIZE;
        probe_write(env, page_addr, 0, mmu_idx, retaddr);
    }
#endif
}

#define MSA_ST_DF(DF, TYPE, ST_INSN, ...)                               \
void helper_msa_st_ ## TYPE(CPUMIPSState *env, uint32_t wd,             \
                            target_ulong addr)                          \
{                                                                       \
    wr_t *pwd = &(env->active_fpu.fpr[wd].wr);                          \
    int mmu_idx = cpu_mmu_index(env, false);				\
    int i;                                                              \
    MEMOP_IDX(DF)                                                       \
    ensure_writable_pages(env, addr, mmu_idx, GETPC());                 \
    for (i = 0; i < DF_ELEMENTS(DF); i++) {                             \
        ST_INSN(env, addr + (i << DF), pwd->TYPE[i], ##__VA_ARGS__);    \
    }                                                                   \
}

#if !defined(CONFIG_USER_ONLY)
MSA_ST_DF(DF_BYTE,   b, helper_ret_stb_mmu, oi, GETPC())
MSA_ST_DF(DF_HALF,   h, helper_ret_stw_mmu, oi, GETPC())
MSA_ST_DF(DF_WORD,   w, helper_ret_stl_mmu, oi, GETPC())
MSA_ST_DF(DF_DOUBLE, d, helper_ret_stq_mmu, oi, GETPC())
#else
MSA_ST_DF(DF_BYTE,   b, cpu_stb_data)
MSA_ST_DF(DF_HALF,   h, cpu_stw_data)
MSA_ST_DF(DF_WORD,   w, cpu_stl_data)
MSA_ST_DF(DF_DOUBLE, d, cpu_stq_data)
#endif

void helper_cache(CPUMIPSState *env, target_ulong addr, uint32_t op)
{
#ifndef CONFIG_USER_ONLY
    target_ulong index = addr & 0x1fffffff;
    if (op == 9) {
        /* Index Store Tag */
        memory_region_dispatch_write(env->itc_tag, index, env->CP0_TagLo,
                                     8, MEMTXATTRS_UNSPECIFIED);
    } else if (op == 5) {
        /* Index Load Tag */
        memory_region_dispatch_read(env->itc_tag, index, &env->CP0_TagLo,
                                    8, MEMTXATTRS_UNSPECIFIED);
    }
#endif
}

#ifdef TARGET_CHERI
target_ulong helper_rdhwr_statcounters_icount(CPUMIPSState *env)
{
    check_hwrena(env, 4, GETPC());
    return 0x12345;
}

target_ulong helper_rdhwr_statcounters_reset(CPUMIPSState *env)
{
    // TODO: actually implement this
    check_hwrena(env, 7, GETPC());
    return 0;
}

target_ulong helper_rdhwr_statcounters_ignored(CPUMIPSState *env, uint32_t num)
{
    check_hwrena(env, num, GETPC());
    return 0xdeadbeef;
}
#endif

/* Reduce the length so that addr + len doesn't cross a page boundary.  */
static inline target_ulong adj_len_to_page(target_ulong len, target_ulong addr)
{
#ifndef CONFIG_USER_ONLY
    target_ulong low_bits = (addr & ~TARGET_PAGE_MASK);
    if (low_bits + len - 1 >= TARGET_PAGE_SIZE) {
        return TARGET_PAGE_SIZE - low_bits;
    }
#endif
    return len;
}


#define MAGIC_LIBCALL_HELPER_CONTINUATION_FLAG UINT64_C(0xbadc0de)
#define MIPS_REGNUM_V0 2
#define MIPS_REGNUM_V1 3
#define MIPS_REGNUM_A0 4
#define MIPS_REGNUM_A1 5
#define MIPS_REGNUM_A2 6
#define MIPS_REGNUM_A3 7

#ifdef CONFIG_DEBUG_TCG
#define MAGIC_MEMSET_STATS 1
#else
#define MAGIC_MEMSET_STATS 0
#endif

#if MAGIC_MEMSET_STATS != 0
static bool memset_stats_dump_registered = false;

struct nop_stats {
    uint64_t kernel_mode_bytes;
    uint64_t kernel_mode_count;
    uint64_t user_mode_bytes;
    uint64_t user_mode_count;
};

static struct nop_stats magic_memset_zero_bytes;
static struct nop_stats magic_memset_nonzero_bytes;
static struct nop_stats magic_memmove_bytes;

static struct nop_stats magic_memcpy_bytes;
static struct nop_stats magic_memmove_bytes;
static struct nop_stats magic_bcopy_bytes;

static struct nop_stats magic_memmove_slowpath;

static inline void print_nop_stats(const char* msg, struct nop_stats* stats) {
    warn_report("%s in kernel mode: %" PRId64 " (%f MB) in %" PRId64 " calls\r", msg,
                stats->kernel_mode_bytes, stats->kernel_mode_bytes / (1024.0 * 1024.0), stats->kernel_mode_count);
    warn_report("%s in user   mode: %" PRId64 " (%f MB) in %" PRId64 " calls\r", msg,
                stats->user_mode_bytes, stats->user_mode_bytes / (1024.0 * 1024.0), stats->user_mode_count);
}

static void dump_memset_stats_on_exit() {
    print_nop_stats("memset (zero)    with magic nop", &magic_memset_zero_bytes);
    print_nop_stats("memset (nonzero) with magic nop", &magic_memset_nonzero_bytes);
    print_nop_stats("memcpy with magic nop", &magic_memcpy_bytes);
    print_nop_stats("memmove with magic nop", &magic_memmove_bytes);
    print_nop_stats("bcopy with magic nop", &magic_bcopy_bytes);
    print_nop_stats("memmove/memcpy/bcopy slowpath", &magic_memmove_slowpath);
}

static inline void collect_magic_nop_stats(CPUMIPSState *env, struct nop_stats* stats, target_ulong bytes) {
    if (!memset_stats_dump_registered) {
        // TODO: move this to CPU_init
        atexit(dump_memset_stats_on_exit);
        memset_stats_dump_registered = true;
    }
    if (in_kernel_mode(env)) {
        stats->kernel_mode_bytes += bytes;
        stats->kernel_mode_count++;
    } else {
        stats->user_mode_bytes += bytes;
        stats->user_mode_count++;
    }
}
#else
#define collect_magic_nop_stats(env, stats, bytes)
#endif


static inline void
store_byte_and_clear_tag(CPUMIPSState *env, target_ulong vaddr, uint8_t val,
                         TCGMemOpIdx oi, uintptr_t retaddr)
{
    helper_ret_stb_mmu(env, vaddr, val, oi, retaddr);
#ifdef TARGET_CHERI
    // If we returned (i.e. write was successful) we also need to invalidate the
    // tags bit to ensure we are consistent with sb
    cheri_tag_invalidate(env, vaddr, 1, retaddr);
#endif
}

#ifdef TARGET_CHERI
#define CHECK_AND_ADD_DDC(env, perms, ptr, len) check_ddc(env, perms, ptr, len, /*instavail=*/true);
#else
#define CHECK_AND_ADD_DDC(env, perms, ptr, len) ptr
#endif

static bool do_magic_memmove(CPUMIPSState *env, uint64_t ra, int dest_regnum, int src_regnum)
{
    tcg_debug_assert(dest_regnum != src_regnum);
    const target_ulong original_dest_ddc_offset = env->active_tc.gpr[dest_regnum]; // $a0 = dest
    const target_ulong original_src_ddc_offset = env->active_tc.gpr[src_regnum];  // $a1 = src
    const target_ulong original_len = env->active_tc.gpr[MIPS_REGNUM_A2];  // $a2 = len
    int mmu_idx = cpu_mmu_index(env, false);
    TCGMemOpIdx oi = make_memop_idx(MO_UB, mmu_idx);
    target_ulong len = original_len;
    target_ulong already_written = 0;
    const bool is_continuation = (env->active_tc.gpr[MIPS_REGNUM_V1] >> 32) == MAGIC_LIBCALL_HELPER_CONTINUATION_FLAG;
    if (is_continuation) {
        // This is a partial write -> $a0 is the original dest argument.
        // The updated dest (after the partial write) was stored in $v0 by the previous call
        already_written = env->active_tc.gpr[MIPS_REGNUM_V0];
        tcg_debug_assert(already_written < len);
        len -= already_written; // update the remaining length
#if 0
        fprintf(stderr, "--- %s: Got continuation for 0x" TARGET_FMT_lx " byte access at 0x" TARGET_FMT_plx
                        " -- current dest = 0x" TARGET_FMT_plx " -- current len = 0x" TARGET_FMT_lx "\r\n",
                        __func__, original_len, original_dest, dest, len);
#endif
    } else {
        // Not a partial write -> $v0 should be zero otherwise this is a usage error!
        if (env->active_tc.gpr[MIPS_REGNUM_V0] != 0) {
            error_report("ERROR: Attempted to call memset library function "
                          "with non-zero value in $v0 (0x" TARGET_FMT_lx
                          ") and continuation flag not set in $v1 (0x" TARGET_FMT_lx
                          ")!\n", env->active_tc.gpr[MIPS_REGNUM_V0], env->active_tc.gpr[MIPS_REGNUM_V1]);
            do_raise_exception(env, EXCP_RI, GETPC());
        }
    }
    const bool log_instr = qemu_loglevel_mask(CPU_LOG_INSTR | CPU_LOG_CVTRACE);
    if (len == 0 || original_src_ddc_offset == original_dest_ddc_offset) {
        goto success; // nothing to do
    }
    // Check capability bounds for the whole copy
    // If it is going to fail we don't bother doing a partial copy!
    const target_ulong original_src = CHECK_AND_ADD_DDC(env, CAP_PERM_LOAD, original_src_ddc_offset, original_len);
    const target_ulong original_dest = CHECK_AND_ADD_DDC(env, CAP_PERM_STORE, original_dest_ddc_offset, original_len);

    // Mark this as a continuation in $v1 (so that we continue sensibly if we get a tlb miss and longjump out)
    env->active_tc.gpr[MIPS_REGNUM_V1] = (MAGIC_LIBCALL_HELPER_CONTINUATION_FLAG << 32) | env->active_tc.gpr[3];

    const target_ulong dest_past_end = original_dest + original_len;
    const target_ulong src_past_end = original_src + original_len;
#if 0 // FIXME: for some reason this causes errors
    const bool dest_same_page = (original_dest & TARGET_PAGE_MASK) == ((dest_past_end - 1) & TARGET_PAGE_MASK);
    const bool src_same_page = (original_dest & TARGET_PAGE_MASK) == ((dest_past_end - 1) & TARGET_PAGE_MASK);
    // If neither src nor dest buffer cross a page boundary we can just do an address_space_read+write
    // Fast case: less than a page and neither of the buffers crosses a page boundary
    CPUState *cs = CPU(mips_env_get_cpu(env));
    if (dest_same_page && src_same_page) {
        tcg_debug_assert(already_written == 0);
        tcg_debug_assert(len <= TARGET_PAGE_SIZE);
        // The translation operation might trap and longjump out!
        hwaddr src_paddr = do_translate_address(env, original_src, MMU_DATA_LOAD, ra);
        hwaddr dest_paddr = do_translate_address(env, original_dest, MMU_DATA_STORE, ra);
#ifdef TARGET_CHERI
        if (dest_paddr <= env->lladdr && dest_paddr + len > env->lladdr) {
            // reset the linked flag if we touch the address with this write
            env->linkedflag = 0;
        }
#endif
        // Do a single load+store to update the MMU flags
        // uint8_t first_value = helper_ret_ldub_mmu(env, original_src, oi, ra);
        // Note: address_space_write will also clear the tag bits!
        MemTxResult result = MEMTX_ERROR;
        uint8_t buffer[TARGET_PAGE_SIZE];
        result = address_space_read(cs->as, src_paddr, MEMTXATTRS_UNSPECIFIED, buffer, len);
        if (result != MEMTX_OK) {
            warn_report("magic memmove: error reading %d bytes from paddr %"HWADDR_PRIx
                        ". Unmapped memory? Error code was %d\r", (int)len, src_paddr, result);
            // same ignored error would happen with normal loads/stores -> just continue
        }
        fprintf(stderr, "Used fast path to read %d bytes\r\n", (int)len);
        // do_hexdump(stderr, buffer, len, original_src);
        // fprintf(stderr, "\r");
        // also write one byte to the target buffer to ensure that the flags are updated
        // store_byte_and_clear_tag(env, original_dest, first_value, oi, ra); // might trap
        result = address_space_write(cs->as, dest_paddr, MEMTXATTRS_UNSPECIFIED, buffer, len);
        if (unlikely(log_instr)) {
            for (int i = 0; i < len; i++) {
                helper_dump_load(env, OPC_LBU, original_src + i, buffer[i]);
                dump_store(env, OPC_SB, original_dest + i, buffer[i]);
            }
        }
        if (result != MEMTX_OK) {
            warn_report("magic memmove: error writing %d bytes to paddr %"HWADDR_PRIx
                        ". Unmapped memory? Error code was %d\r", (int)len, dest_paddr, result);
            // same ignored error would happen with normal loads/stores -> just continue
        }
        already_written += len;
        env->active_tc.gpr[MIPS_REGNUM_V0] = already_written;
        goto success;
    }
#endif

    const bool has_overlap = MAX(original_dest, original_src) >= MAX(dest_past_end, src_past_end);
    if (has_overlap) {
        warn_report("Found multipage magic memmove with overlap: dst=" TARGET_FMT_plx " src=" TARGET_FMT_plx
                    " len=0x" TARGET_FMT_lx "\r", original_dest, original_src, original_len);
        // slow path: byte copies
    }

    const bool copy_backwards = original_src < original_dest;
    if (copy_backwards) {
        target_ulong current_dest_cursor = original_dest + len - 1;
        target_ulong current_src_cursor = original_src + len - 1;
        /* Slow path (probably attempt to do this to an I/O device or
         * similar, or clearing of a block of code we have translations
         * cached for). Just do a series of byte writes as the architecture
         * demands. It's not worth trying to use a cpu_physical_memory_map(),
         * memset(), unmap() sequence here because:
         *  + we'd need to account for the blocksize being larger than a page
         *  + the direct-RAM access case is almost always going to be dealt
         *    with in the fastpath code above, so there's no speed benefit
         *  + we would have to deal with the map returning NULL because the
         *    bounce buffer was in use
         */
        tcg_debug_assert(original_len - already_written == len);
        collect_magic_nop_stats(env, &magic_memmove_slowpath, len);
        while (already_written < original_len) {
            uint8_t value = helper_ret_ldub_mmu(env, current_src_cursor, oi, ra);
            if (unlikely(log_instr)) {
                helper_dump_load(env, OPC_LBU, current_src_cursor, value);
            }
            store_byte_and_clear_tag(env, current_dest_cursor, value, oi, ra); // might trap
            if (unlikely(log_instr)) {
                dump_store(env, OPC_SB, current_dest_cursor, value);
            }
            current_dest_cursor--;
            current_src_cursor--;
            already_written++;
            env->active_tc.gpr[MIPS_REGNUM_V0] = already_written;
        }
    } else {
        // copy forwards
        target_ulong current_dest_cursor = original_dest + already_written;
        target_ulong current_src_cursor = original_src + already_written;
        /* Slow path (probably attempt to do this to an I/O device or
         * similar, or clearing of a block of code we have translations
         * cached for). Just do a series of byte writes as the architecture
         * demands. It's not worth trying to use a cpu_physical_memory_map(),
         * memset(), unmap() sequence here because:
         *  + we'd need to account for the blocksize being larger than a page
         *  + the direct-RAM access case is almost always going to be dealt
         *    with in the fastpath code above, so there's no speed benefit
         *  + we would have to deal with the map returning NULL because the
         *    bounce buffer was in use
         */
        tcg_debug_assert(original_len - already_written == len);
        collect_magic_nop_stats(env, &magic_memmove_slowpath, len);
        while (already_written < original_len) {
            uint8_t value = helper_ret_ldub_mmu(env, current_src_cursor, oi, ra);
            if (unlikely(log_instr)) {
                helper_dump_load(env, OPC_LBU, current_src_cursor, value);
            }
            store_byte_and_clear_tag(env, current_dest_cursor, value, oi, ra); // might trap
            if (unlikely(log_instr)) {
                dump_store(env, OPC_SB, current_dest_cursor, value);
            }
            current_dest_cursor++;
            current_src_cursor++;
            already_written++;
            env->active_tc.gpr[MIPS_REGNUM_V0] = already_written;
        }
    }
success:
    if (already_written != original_len) {
        error_report("ERROR: %s: did not memmove all bytes to " TARGET_FMT_plx
                     ". Remainig len = " TARGET_FMT_plx "\r\n", __func__, original_dest, len);
        abort();
    }
    env->active_tc.gpr[MIPS_REGNUM_V0] = original_dest_ddc_offset; // return value of memcpy is the dest argument
    return true;
}

static uint8_t ZEROARRAY[TARGET_PAGE_SIZE];

static bool do_magic_memset(CPUMIPSState *env, uint64_t ra)
{
    // TODO: just use address_space_write?

    // See target/s390x/mem_helper.c and arm/helper.c HELPER(dc_zva)
    int mmu_idx = cpu_mmu_index(env, false);
    TCGMemOpIdx oi = make_memop_idx(MO_UB, mmu_idx);

    const target_ulong original_dest_ddc_offset = env->active_tc.gpr[MIPS_REGNUM_A0];      // $a0 = dest
    uint8_t value = (uint8_t)env->active_tc.gpr[MIPS_REGNUM_A1]; // $a1 = c
    const target_ulong original_len = env->active_tc.gpr[MIPS_REGNUM_A2];       // $a2 = len
    target_ulong dest = original_dest_ddc_offset;
    target_ulong len = original_len;
    const bool is_continuation = (env->active_tc.gpr[MIPS_REGNUM_V1] >> 32) == MAGIC_LIBCALL_HELPER_CONTINUATION_FLAG;
    if (is_continuation) {
        // This is a partial write -> $a0 is the original dest argument.
        // The updated dest (after the partial write) was stored in $v0 by the previous call
        dest = env->active_tc.gpr[MIPS_REGNUM_V0];
        tcg_debug_assert(dest >= original_dest_ddc_offset);
        target_ulong already_written = dest - original_dest_ddc_offset;
        len -= already_written; // update the remaining length
#if 0
        fprintf(stderr, "--- %s: Got continuation for 0x" TARGET_FMT_lx " byte access at 0x" TARGET_FMT_plx
                        " -- current dest = 0x" TARGET_FMT_plx " -- current len = 0x" TARGET_FMT_lx "\r\n",
                        __func__, original_len, original_dest, dest, len);
#endif
    } else {
        // Not a partial write -> $v0 should be zero otherwise this is a usage error!
        if (env->active_tc.gpr[MIPS_REGNUM_V0] != 0) {
            error_report("ERROR: Attempted to call memset library function "
                         "with non-zero value in $v0 (0x" TARGET_FMT_lx
                         ") and continuation flag not set in $v1 (0x" TARGET_FMT_lx
                         ")!\n", env->active_tc.gpr[MIPS_REGNUM_V0], env->active_tc.gpr[MIPS_REGNUM_V1]);
            do_raise_exception(env, EXCP_RI, GETPC());
        }
    }
    dest = CHECK_AND_ADD_DDC(env, CAP_PERM_STORE, dest, len);
    const target_ulong original_dest = CHECK_AND_ADD_DDC(env, CAP_PERM_STORE, original_dest_ddc_offset, original_len);

    tcg_debug_assert(dest + len == original_dest + original_len && "continuation broken?");
    const bool log_instr = qemu_loglevel_mask(CPU_LOG_INSTR | CPU_LOG_CVTRACE);
    if (len == 0) {
        goto success; // nothing to do
    }

    CPUState *cs = CPU(mips_env_get_cpu(env));

    while (len > 0) {
        assert(dest + len == original_dest + original_len && "continuation broken?");
        // probing for write access:
        // fprintf(stderr, "Probing for write access at " TARGET_FMT_plx "\r\n", dest);
        // fflush(stderr);
        // update $v0 to point to the updated dest in case probe_write_access takes a tlb fault:
        env->active_tc.gpr[MIPS_REGNUM_V0] = dest;
        // and mark this as a continuation in $v1 (so that we continue sensibly after the tlb miss was handled)
        env->active_tc.gpr[MIPS_REGNUM_V1] = (MAGIC_LIBCALL_HELPER_CONTINUATION_FLAG << 32) | env->active_tc.gpr[3];
        // If the host address is not in the tlb after the second write we are writing
        // to something strange so just fall back to the slowpath
        target_ulong l_adj = adj_len_to_page(len, dest);
        tcg_debug_assert(l_adj != 0);
        tcg_debug_assert(((dest + l_adj - 1) & TARGET_PAGE_MASK) == (dest & TARGET_PAGE_MASK) && "should not cross a page boundary!");
        void* hostaddr = NULL;
#if 0
        for (int try = 0; try < 2; try++) {
            hostaddr = tlb_vaddr_to_host(env, dest, 1, mmu_idx);
            if (hostaddr)
                break;
            /* OK, try a store and see if we can populate the tlb. This
             * might cause an exception if the memory isn't writable,
             * in which case we will longjmp out of here. We must for
             * this purpose use the actual register value passed to us
             * so that we get the fault address right.
             */
            store_byte_and_clear_tag(env, dest, 0xff, oi, ra);
            if (unlikely(log_instr)) {
                dump_store(env, OPC_SB, dest, 0xff);
            }
            // This should definitely fill the host TLB. If not we are probably writing to I/O memory
            // FIXME: tlb_vaddr_to_host() also returns null if the notdirty flag is set (not sure if that's a bug
            probe_write(env, dest, 1, mmu_idx, ra);
        }
#endif
        if (hostaddr) {
            /* If it's all in the TLB it's fair game for just writing to;
             * we know we don't need to update dirty status, etc.
             */
            tcg_debug_assert(dest + len == original_dest + original_len && "continuation broken?");
            // Do one store byte to update MMU flags (not sure this is necessary)
            store_byte_and_clear_tag(env, dest, value, oi, ra);
            memset(hostaddr, value, l_adj);
#ifdef TARGET_CHERI
            // We also need to invalidate the tags bits written by the memset
            // qemu_ram_addr_from_host is faster than using the v2r routines in cheri_tag_invalidate
            ram_addr_t ram_addr = qemu_ram_addr_from_host(hostaddr);
            if (ram_addr != RAM_ADDR_INVALID) {
                cheri_tag_phys_invalidate(ram_addr, l_adj);
            } else {
                cheri_tag_invalidate(env, dest, l_adj, ra);
            }
#endif
            if (unlikely(log_instr)) {
                // TODO: dump as a single big block?
                for (target_ulong i = 0; i < l_adj; i++)
                    dump_store(env, OPC_SB, dest + i, value);

                qemu_log("%s: Set " TARGET_FMT_ld " bytes to 0x%x at 0x" TARGET_FMT_plx "\n",
                          __func__, l_adj, value, dest);
            }
            // fprintf(stderr, "%s: Set " TARGET_FMT_ld " bytes to 0x%x at 0x" TARGET_FMT_plx "/%p\r\n", __func__, l_adj, value, dest, hostaddr);
            dest += l_adj;
            len -= l_adj;
        } else {
            // First try address_space_write and if that fails fall back to bytewise setting
            hwaddr paddr = do_translate_address(env, dest, MMU_DATA_STORE, ra);
#ifdef TARGET_CHERI
            if (paddr <= env->lladdr && paddr + l_adj > env->lladdr) {
                // reset the linked flag if we touch the address with this write
                env->linkedflag = 0;
            }
#endif
            // Note: address_space_write will also clear the tag bits!
            MemTxResult result = MEMTX_ERROR;
            if (value == 0) {
                 result = address_space_write(cs->as, paddr, MEMTXATTRS_UNSPECIFIED, ZEROARRAY, l_adj);
            } else {
                // create a buffer filled with the correct value and use that for the write
                uint8_t setbuffer[TARGET_PAGE_SIZE];
                tcg_debug_assert(l_adj <= sizeof(setbuffer));
                memset(setbuffer, value, l_adj);
                result = address_space_write(cs->as, paddr, MEMTXATTRS_UNSPECIFIED, setbuffer, l_adj);
            }
            if (result == MEMTX_OK) {
                dest += l_adj;
                len -= l_adj;
                if (unlikely(log_instr)) {
                    // TODO: dump as a single big block?
                    for (target_ulong i = 0; i < l_adj; i++)
                        dump_store(env, OPC_SB, dest + i, value);

                    qemu_log("%s: Set " TARGET_FMT_ld " bytes to 0x%x at 0x" TARGET_FMT_plx "\n",
                              __func__, l_adj, value, dest);
                }
                continue; // try again with next page
            } else {
                warn_report("address_space_write failed with error %d for %"HWADDR_PRIx "\r", result, paddr);
                // fall back to bytewise copy slow path
            }
            /* Slow path (probably attempt to do this to an I/O device or
             * similar, or clearing of a block of code we have translations
             * cached for). Just do a series of byte writes as the architecture
             * demands. It's not worth trying to use a cpu_physical_memory_map(),
             * memset(), unmap() sequence here because:
             *  + we'd need to account for the blocksize being larger than a page
             *  + the direct-RAM access case is almost always going to be dealt
             *    with in the fastpath code above, so there's no speed benefit
             *  + we would have to deal with the map returning NULL because the
             *    bounce buffer was in use
             */
            warn_report("%s: Falling back to memset slowpath for address " TARGET_FMT_plx
                        " (phys addr=%" HWADDR_PRIx", len=0x" TARGET_FMT_lx ")! I/O memory or QEMU TLB bug?\r",
                        __func__, dest, mips_cpu_get_phys_page_debug(CPU(mips_env_get_cpu(env)), dest), len);
            target_ulong end = original_dest + original_len;
            // in case we fault mark this as a continuation in $v1 (so that we continue sensibly after the tlb miss was handled)
            env->active_tc.gpr[MIPS_REGNUM_V1] = (MAGIC_LIBCALL_HELPER_CONTINUATION_FLAG << 32) | env->active_tc.gpr[MIPS_REGNUM_V1];
            while (dest < end) {
                tcg_debug_assert(dest + len == original_dest + original_len && "continuation broken?");
                // update $v0 to point to the updated dest in case probe_write_access takes a tlb fault:
                env->active_tc.gpr[MIPS_REGNUM_V0] = dest;
                store_byte_and_clear_tag(env, dest, value, oi, ra); // might trap
                if (unlikely(log_instr)) {
                    dump_store(env, OPC_SB, dest, value);
                }
                dest++;
                len--;
            }
        }
    }
    tcg_debug_assert(len == 0);
success:
    env->active_tc.gpr[MIPS_REGNUM_V0] = original_dest_ddc_offset; // return value of memset is the src argument
    // also update a0 and a2 to match what the kernel memset does (a0 -> buf end, a2 -> 0):
    env->active_tc.gpr[MIPS_REGNUM_A0] = dest;
    env->active_tc.gpr[MIPS_REGNUM_A2] = len;
#if MAGIC_MEMSET_STATS != 0
    collect_magic_nop_stats(env, value == 0 ? &magic_memset_zero_bytes : &magic_memset_nonzero_bytes, original_len);
#endif
    return true;
}

#define MAGIC_HELPER_DONE_FLAG 0xDEC0DED

enum {
    MAGIC_NOP_MEMSET = 1,
    MAGIC_NOP_MEMSET_C = 2,
    MAGIC_NOP_MEMCPY = 3,
    MAGIC_NOP_MEMCPY_C = 4,
    MAGIC_NOP_MEMMOVE = 5,
    MAGIC_NOP_MEMMOVE_C = 6,
    MAGIC_NOP_BCOPY = 7,
};



// Magic library function calls:
void helper_magic_library_function(CPUMIPSState *env, target_ulong which)
{
    qemu_log_mask(CPU_LOG_INSTR, "--- Calling magic library function 0x" TARGET_FMT_lx "\n", which);
    // High bits can be used by function to indicate continuation after TLB miss
    switch (which & UINT32_MAX) {
    case MAGIC_NOP_MEMSET:
        if (!do_magic_memset(env, GETPC()))
            return;
        // otherwise update $v1 to indicate success
        break;

    case MAGIC_NOP_MEMCPY:
        if (!do_magic_memmove(env, GETPC(), MIPS_REGNUM_A0, MIPS_REGNUM_A1))
            goto error;
        collect_magic_nop_stats(env, &magic_memcpy_bytes, env->active_tc.gpr[MIPS_REGNUM_A2]);
        break;

    case MAGIC_NOP_MEMMOVE:
        if (!do_magic_memmove(env, GETPC(), MIPS_REGNUM_A0, MIPS_REGNUM_A1))
            goto error;
        collect_magic_nop_stats(env, &magic_memmove_bytes, env->active_tc.gpr[MIPS_REGNUM_A2]);
        break;

    case MAGIC_NOP_BCOPY: // src + dest arguments swapped
        if (!do_magic_memmove(env, GETPC(), MIPS_REGNUM_A1, MIPS_REGNUM_A0))
            goto error;
        collect_magic_nop_stats(env, &magic_bcopy_bytes, env->active_tc.gpr[MIPS_REGNUM_A2]);
        break;

    case 0xf0:
    case 0xf1:
    {
        uint8_t buffer[TARGET_PAGE_SIZE];
        // to match memset/memcpy calling convention (use a0 and a2)
        target_ulong src = env->active_tc.gpr[MIPS_REGNUM_A0];
        target_ulong real_len = env->active_tc.gpr[MIPS_REGNUM_A2];
        fprintf(stderr, "--- Memory dump at %s(%s): " TARGET_FMT_lu " bytes at " TARGET_FMT_plx "\r\n",
                lookup_symbol(env->active_tc.PC), ((which & UINT32_MAX) == 0xf0 ? "entry" : "exit"), real_len, src);
        while (real_len > 0) {
            target_ulong len = adj_len_to_page(real_len, src);
            real_len -= len;
            if (len != env->active_tc.gpr[MIPS_REGNUM_A2]) {
                fprintf(stderr, "--- partial dump at %s(%s): " TARGET_FMT_lu " bytes at " TARGET_FMT_plx "\r\n",
                        lookup_symbol(env->active_tc.PC), ((which & UINT32_MAX) == 0xf0 ? "entry" : "exit"), len, src);
            }
            if (cpu_memory_rw_debug(ENV_GET_CPU(env), src, buffer, len, false) == 0) {
                bool have_nonzero = false;
                for (int i = 0; i < len; i++)
                    if (buffer[i] != 0)
                        have_nonzero = true;
                if (have_nonzero)
                    do_hexdump(stderr, buffer, len, src);
                else
                    fprintf(stderr, "   -- all zeroes\r\n");
            } else {
                fprintf(stderr, "--- Memory dump at %s(%s): Could not fetch" TARGET_FMT_lu " bytes at " TARGET_FMT_plx "\r\n",
                        lookup_symbol(env->active_tc.PC), ((which & UINT32_MAX) == 0xf0 ? "entry" : "exit"), len, src);
            }
        }
    }
    case 0xfe:
    case 0xff:
        // dump argument and return registers:
        warn_report("%s(%s): argument+return registers: \r\n"
                    "\tv0 = 0x" TARGET_FMT_lx "\tv1 = 0x" TARGET_FMT_lx "\r\n"
                    "\ta0 = 0x" TARGET_FMT_lx "\ta1 = 0x" TARGET_FMT_lx "\r\n"
                    "\ta2 = 0x" TARGET_FMT_lx "\ta3 = 0x" TARGET_FMT_lx "\r\n",
                    lookup_symbol(env->active_tc.PC), ((which & UINT32_MAX) == 0xfe ? "entry" : "exit"),
                    env->active_tc.gpr[2], env->active_tc.gpr[3],
                    env->active_tc.gpr[4], env->active_tc.gpr[5],
                    env->active_tc.gpr[6], env->active_tc.gpr[7]);
        break;
    case MAGIC_HELPER_DONE_FLAG:
        qemu_log_mask(CPU_LOG_INSTR, "ERROR: Attempted to call library function "
                                     "with success flag set in $v1!\n");
        do_raise_exception(env, EXCP_RI, GETPC());
    default:
        qemu_log_mask(CPU_LOG_INSTR, "ERROR: Attempted to call invalid library function "
                          TARGET_FMT_lx "\n", which);
        goto error;
    }
    // Indicate success by setting $v1 to 0xaffe
    env->active_tc.gpr[MIPS_REGNUM_V1] = MAGIC_HELPER_DONE_FLAG;
    return;
error:
    warn_report("%s: magic nop %d failed: \r\n"
                    "\tv0 = 0x" TARGET_FMT_lx "\tv1 = 0x" TARGET_FMT_lx "\r\n"
                    "\ta0 = 0x" TARGET_FMT_lx "\ta1 = 0x" TARGET_FMT_lx "\r\n"
                    "\ta2 = 0x" TARGET_FMT_lx "\ta3 = 0x" TARGET_FMT_lx "\r\n",
                    __func__, (int)(which & UINT32_MAX),
                    env->active_tc.gpr[MIPS_REGNUM_V0], env->active_tc.gpr[MIPS_REGNUM_V1],
                    env->active_tc.gpr[MIPS_REGNUM_A0], env->active_tc.gpr[MIPS_REGNUM_A1],
                    env->active_tc.gpr[MIPS_REGNUM_A2], env->active_tc.gpr[MIPS_REGNUM_A3]);
}
