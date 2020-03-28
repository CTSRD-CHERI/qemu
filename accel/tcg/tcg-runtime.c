/*
 * Tiny Code Generator for QEMU
 *
 * Copyright (c) 2008 Fabrice Bellard
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include "qemu/osdep.h"
#include "qemu/host-utils.h"
#include "cpu.h"
#include "exec/helper-proto.h"
#include "exec/cpu_ldst.h"
#include "exec/exec-all.h"
#include "exec/tb-lookup.h"
#include "disas/disas.h"
#include "exec/log.h"
#include "tcg/tcg.h"
#ifdef TARGET_CHERI
#include "cheri-helper-utils.h"
#endif

/* 32-bit helpers */

int32_t HELPER(div_i32)(int32_t arg1, int32_t arg2)
{
    return arg1 / arg2;
}

int32_t HELPER(rem_i32)(int32_t arg1, int32_t arg2)
{
    return arg1 % arg2;
}

uint32_t HELPER(divu_i32)(uint32_t arg1, uint32_t arg2)
{
    return arg1 / arg2;
}

uint32_t HELPER(remu_i32)(uint32_t arg1, uint32_t arg2)
{
    return arg1 % arg2;
}

/* 64-bit helpers */

uint64_t HELPER(shl_i64)(uint64_t arg1, uint64_t arg2)
{
    return arg1 << arg2;
}

uint64_t HELPER(shr_i64)(uint64_t arg1, uint64_t arg2)
{
    return arg1 >> arg2;
}

int64_t HELPER(sar_i64)(int64_t arg1, int64_t arg2)
{
    return arg1 >> arg2;
}

int64_t HELPER(div_i64)(int64_t arg1, int64_t arg2)
{
    return arg1 / arg2;
}

int64_t HELPER(rem_i64)(int64_t arg1, int64_t arg2)
{
    return arg1 % arg2;
}

uint64_t HELPER(divu_i64)(uint64_t arg1, uint64_t arg2)
{
    return arg1 / arg2;
}

uint64_t HELPER(remu_i64)(uint64_t arg1, uint64_t arg2)
{
    return arg1 % arg2;
}

uint64_t HELPER(muluh_i64)(uint64_t arg1, uint64_t arg2)
{
    uint64_t l, h;
    mulu64(&l, &h, arg1, arg2);
    return h;
}

int64_t HELPER(mulsh_i64)(int64_t arg1, int64_t arg2)
{
    uint64_t l, h;
    muls64(&l, &h, arg1, arg2);
    return h;
}

uint32_t HELPER(clz_i32)(uint32_t arg, uint32_t zero_val)
{
    return arg ? clz32(arg) : zero_val;
}

uint32_t HELPER(ctz_i32)(uint32_t arg, uint32_t zero_val)
{
    return arg ? ctz32(arg) : zero_val;
}

uint64_t HELPER(clz_i64)(uint64_t arg, uint64_t zero_val)
{
    return arg ? clz64(arg) : zero_val;
}

uint64_t HELPER(ctz_i64)(uint64_t arg, uint64_t zero_val)
{
    return arg ? ctz64(arg) : zero_val;
}

uint32_t HELPER(clrsb_i32)(uint32_t arg)
{
    return clrsb32(arg);
}

uint64_t HELPER(clrsb_i64)(uint64_t arg)
{
    return clrsb64(arg);
}

uint32_t HELPER(ctpop_i32)(uint32_t arg)
{
    return ctpop32(arg);
}

uint64_t HELPER(ctpop_i64)(uint64_t arg)
{
    return ctpop64(arg);
}

void *HELPER(lookup_tb_ptr)(CPUArchState *env)
{
    CPUState *cpu = env_cpu(env);
    TranslationBlock *tb;
    target_ulong cs_base, cs_top = 0, pc;
    uint32_t cheri_flags = 0;
    uint32_t flags;

    tb = tb_lookup__cpu_state(cpu, &pc, &cs_base, &cs_top, &cheri_flags, &flags,
                              curr_cflags());
    if (tb == NULL) {
        return tcg_ctx->code_gen_epilogue;
    }
    qemu_log_mask_and_addr(CPU_LOG_EXEC, pc,
                           "Chain %d: %p [" TARGET_FMT_lx "/" TARGET_FMT_lx
                           "/" TARGET_FMT_lx "/%#x/%#x] %s\n",
                           cpu->cpu_index, tb->tc.ptr, cs_base, pc, cs_top,
                           cheri_flags, flags, lookup_symbol(pc));
    return tb->tc.ptr;
}

#if defined(CONFIG_MIPS_LOG_INSTR)
/*
 * Print the instruction to log file.
 */
void HELPER(log_instruction)(CPUArchState *env, target_ulong pc)
{
    if (unlikely(qemu_loglevel_mask(CPU_LOG_INSTR) && qemu_log_in_addr_range(pc))) {
#if defined(CONFIG_RVFI_DII) && defined(TARGET_RISCV)
        if (env->rvfi_dii_have_injected_insn) {
            uint32_t insn = env->rvfi_dii_trace.rvfi_dii_insn;
            target_disas_buf(stderr, env_cpu(env), &insn, sizeof(insn),
                             PC_ADDR(env), 1);
        } else
#endif
        {
            /* Disassemble and print one instruction. */
            log_target_disas(env_cpu(env), pc, -1);
        }
    }
}

#ifdef TARGET_MIPS
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
#endif

/*
 * Print the memory store to log file.
 */
void HELPER(dump_store64)(CPUArchState *env, target_ulong addr, uint64_t value, MemOp op)
{

    if (likely(!(qemu_loglevel_mask(CPU_LOG_INSTR | CPU_LOG_CVTRACE))))
        return;

#ifdef TARGET_CHERI
    // Try not to dump all stores when -dfilter is enabled
    // Note: we check both PC and memory location in -dfilter
    if (likely(!should_log_mem_access(env, CPU_LOG_INSTR | CPU_LOG_CVTRACE, addr)))
        return;
#endif

#ifdef TARGET_MIPS
    if (qemu_loglevel_mask(CPU_LOG_CVTRACE)) {
        cvtrace_dump_gpr_store(&env->cvtrace, addr, value);
        return;
    }
#endif

    // FIXME: value printed is not correct for sdl!
    // FIXME: value printed is not correct for sdr!
    // FIXME: value printed is not correct for swl!
    // FIXME: value printed is not correct for swr!
    switch (memop_size(op)) {
    case 8:
        qemu_log("    Memory Write [" TARGET_FMT_lx "] = " TARGET_FMT_plx "\n",
                 addr, value);
        break;
    case 4:
        qemu_log("    Memory Write [" TARGET_FMT_lx "] = %08x\n", addr,
                 (uint32_t)value);
        break;
    case 2:
        qemu_log("    Memory Write [" TARGET_FMT_lx "] = %04x\n", addr,
                 (uint16_t)value);
        break;
    case 1:
        qemu_log("    Memory Write [" TARGET_FMT_lx "] = %02x\n", addr,
                 (uint8_t)value);
        break;
    default:
        tcg_abort();
    }
}

void HELPER(dump_store32)(CPUArchState *env, target_ulong addr, uint32_t value, MemOp op)
{
    helper_dump_store64(env, addr, (uint64_t)value, op);
}

/*
 * Print the memory load to log file.
 */
void HELPER(dump_load64)(CPUArchState *env, target_ulong addr, uint64_t value, MemOp op)
{
    if (likely(!(qemu_loglevel_mask(CPU_LOG_INSTR | CPU_LOG_CVTRACE))))
        return;

#ifdef TARGET_CHERI
    // Try not to dump all loads when -dfilter is enabled
    // Note: we check both PC and memory location in -dfilter
    if (likely(!should_log_mem_access(env, CPU_LOG_INSTR | CPU_LOG_CVTRACE, addr)))
        return;
#endif

#ifdef TARGET_MIPS
    if (qemu_loglevel_mask(CPU_LOG_CVTRACE)) {
        cvtrace_dump_gpr_load(&env->cvtrace, addr, value);
        return;
    }
#endif
    // FIXME: cloadtags not correct
    switch (memop_size(op)) {
    case 8:
        qemu_log("    Memory Read [" TARGET_FMT_lx "] = " TARGET_FMT_plx "\n",
                 addr, value);
        break;
    case 4:
        qemu_log("    Memory Read [" TARGET_FMT_lx "] = %08x\n", addr,
                 (uint32_t)value);
        break;
    case 2:
        qemu_log("    Memory Read [" TARGET_FMT_lx "] = %04x\n", addr,
                 (uint16_t)value);
        break;
    case 1:

        qemu_log("    Memory Read [" TARGET_FMT_lx "] = %02x\n", addr,
                 (uint8_t)value);
        break;
    default:
        tcg_abort();
    }
}

void HELPER(dump_load32)(CPUArchState *env, target_ulong addr, uint32_t value, MemOp op)
{
    helper_dump_load64(env, addr, (uint64_t)value, op);
}
#endif


void HELPER(exit_atomic)(CPUArchState *env)
{
    cpu_loop_exit_atomic(env_cpu(env), GETPC());
}
