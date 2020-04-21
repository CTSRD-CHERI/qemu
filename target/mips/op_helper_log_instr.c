/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2015-2016 Stacey Son <sson@FreeBSD.org>
 * Copyright (c) 2016-2018 Alfredo Mazzinghi <am2419@cl.cam.ac.uk>
 * Copyright (c) 2016-2018 Alex Richardson <Alexander.Richardson@cl.cam.ac.uk>
 * All rights reserved.
 *
 * This software was developed by SRI International and the University of
 * Cambridge Computer Laboratory under DARPA/AFRL contract FA8750-10-C-0237
 * ("CTSRD"), as part of the DARPA CRASH research programme.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#include "qemu/osdep.h"
#include "exec/exec-all.h"
#include "exec/cpu_ldst.h"
#include "exec/helper-proto.h"
#include "exec/log.h"
#include "cpu.h"
#include "internal.h"
#include "exec/log_instr.h"

#ifdef TARGET_CHERI
#include "cheri-helper-utils.h"
#endif

#ifdef CONFIG_CHERI_LOG_INSTR

/* TODO(am2419): New logging API helpers */

static const char* mips_cpu_get_changed_mode(CPUMIPSState *env);

/*
 * Log general purpose register modification.
 */
void helper_mips_log_instr_gpr(CPUArchState *env, uint32_t reg,
                               target_ulong value)
{
    if (unlikely(qemu_log_instr_enabled(env_cpu(env))))
        qemu_log_instr_reg(env, mips_gp_regnames[reg], value);
}

/*
 * Log Cop0 register modification.
 * Most of the mtc0 logging is done in cp0_helper, this takes care
 * of logging updates that do not go though helpers in cp0_helper.
 * TODO(am2419): may be merged with the log_instr_cop0_update in an header.
 */
void helper_mips_log_instr_cop0(CPUArchState *env, uint32_t reg, uint32_t sel,
                                target_ulong value)
{
    if (unlikely(qemu_log_instr_enabled(env_cpu(env))))
        qemu_log_instr_reg(env, mips_cop0_regnames[reg * 8 + sel], value);
}

static void dump_changed_regs(CPUMIPSState *env);

static void dump_changed_cop0(CPUMIPSState *env);
/*
 * Print the changed processor state.
 * Note: if this is emitted, tracing was enabled for this translation block,
 * so we do not repeat the check.
 */
void helper_mips_log_instr_changed_state(CPUMIPSState *env, target_ulong pc)
{
    const char *new_mode = mips_cpu_get_changed_mode(env);
    /* Testing pointer equality is fine, it always points to the same constants */
    if (new_mode != env->last_mode) {
        env->last_mode = new_mode;
        qemu_log_instr_extra(env, "--- %s\n", new_mode);
        qemu_log_instr_mode_switch(env, /*enable*/cpu_in_user_mode(env), pc);
    }

    /* TODO(am2419): remove below, just for testing. */
    /* Print changed state: GPR, Cap. */
    dump_changed_regs(env);
    /* if (qemu_loglevel_mask(CPU_LOG_INSTR)) { */
    /*     /\* Print change state: HI/LO COP0 (not included in CVTRACE) *\/ */
    /*     dump_changed_cop0(env); */
    /* } */
}

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

/* TODO(am2419): Old Stuff */

extern int cl_default_trace_format;


#define USER_TRACE_DEBUG 0
#if USER_TRACE_DEBUG
#define user_trace_dbg(...) qemu_log("=== " __VA_ARGS__)
#else
#define user_trace_dbg(...)
#endif

void do_hexdump(FILE* f, uint8_t* buffer, target_ulong length, target_ulong vaddr) {
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
    if (!mode) {
        /* Always print these messages even if user-space only tracing is on */
        mode = cl_default_trace_format;
    }

    if (!mode && qemu_loglevel_mask(CPU_LOG_GUEST_DEBUG_MSG))
        mode = CPU_LOG_INSTR;

    if (!qemu_loglevel_mask(CPU_LOG_GUEST_DEBUG_MSG) || !qemu_log_enabled())
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
            qemu_log_mask(CPU_LOG_INSTR, "   ptr = 0x" TARGET_FMT_lx "\r\n", vaddr);
        }
        return;
    } else if (print_mode == DEBUG_MESSAGE_DECIMAL) {
        if (mode & CPU_LOG_INSTR) {
            qemu_log_mask(CPU_LOG_INSTR, "   value = " TARGET_FMT_ld "\r\n", vaddr);
        }
        return;
    }
    // Otherwise we meed to fetch the memory referenced by vaddr+length
    // NB: length has already been capped at sizeof(buffer)
    int ret = cpu_memory_rw_debug(env_cpu(env), vaddr, buffer, length, false);
    if (ret != 0) {
        warn_report("CHERI DEBUG HELPER: Could not read " TARGET_FMT_ld
                    " bytes at vaddr 0x" TARGET_FMT_lx "\r\n", length, vaddr);
    }
    if ((mode & CPU_LOG_INSTR) || qemu_log_enabled()) {
        qemu_log_mask(CPU_LOG_INSTR, "DEBUG MESSAGE @ 0x" TARGET_FMT_lx " addr=0x"
            TARGET_FMT_lx " len=" TARGET_FMT_ld "\r\n", pc, vaddr, length);
        if (print_mode == DEBUG_MESSAGE_CSTRING) {
            /* XXXAR: Escape newlines, etc.? */
            qemu_log_mask(CPU_LOG_INSTR, "    message = \"%*.*s\"\n", (int)length, (int)length, buffer);
        } else if (print_mode == DEBUG_MESSAGE_HEXDUMP) {
            qemu_log_mask(CPU_LOG_INSTR, "   Dumping " TARGET_FMT_lu
                     " bytes starting at 0x"
                     TARGET_FMT_lx "\r\n", length, vaddr);
            FILE* logfile = qemu_log_lock();
            do_hexdump(logfile, buffer, length, vaddr);
            qemu_log_unlock(logfile);
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

void helper_log_value(CPUMIPSState *env, const void* ptr, uint64_t value)
{
    qemu_log_mask(CPU_LOG_INSTR, "%s: " TARGET_FMT_plx "\n", ptr, value);
}

/*
 * Print the instruction to cvtrace log file.
 */
void helper_mips_cvtrace_log_instruction(CPUMIPSState *env, target_ulong pc)
{
    if (unlikely(should_log_instr(env, CPU_LOG_CVTRACE))) {
        int isa = (env->hflags & MIPS_HFLAG_M16) == 0 ? 0 : (env->insn_flags & ASE_MICROMIPS) ? 1 : 2;
        static uint16_t cycles = 0;  /* XXX */
        uint32_t opcode;
        CPUState *cs = env_cpu(env);

        FILE* logfile = qemu_log_lock();
        /* if the logfile is empty we need to emit the cvt magic */
        if (env->cvtrace.version != 0 && ftell(logfile) != 0) {
            /* Write previous instruction trace to log. */
            fwrite(&env->cvtrace, sizeof(env->cvtrace), 1, logfile);
        } else {
            char buffer[sizeof(env->cvtrace)];

            buffer[0] = CVT_QEMU_VERSION;
            g_strlcpy(buffer+1, CVT_QEMU_MAGIC, sizeof(env->cvtrace)-2);
            fwrite(buffer, sizeof(env->cvtrace), 1, logfile);
            cycles = 0;
        }
        qemu_log_unlock(logfile);
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

#ifndef CONFIG_USER_ONLY

void r4k_dump_tlb(CPUMIPSState *env, int idx)
{
    r4k_tlb_t *tlb = &env->tlb->mmu.r4k.tlb[idx];
    target_ulong pagemask, hi, lo0, lo1;

    if (tlb->EHINV) {
        pagemask = 0;
        hi  = 1 << CP0EnHi_EHINV;
        lo0 = 0;
        lo1 = 0;
    } else {
        pagemask = tlb->PageMask;
        hi = tlb->VPN | tlb->ASID;
        lo0 = tlb->G | (tlb->V0 << 1) | (tlb->D0 << 2) |
#ifdef TARGET_CHERI
            ((target_ulong)tlb->S0 << CP0EnLo_S) |
            ((target_ulong)tlb->L0 << CP0EnLo_L) |
            ((target_ulong)tlb->CLG0 << CP0EnLo_CLG) |
#else
            ((target_ulong)tlb->RI0 << CP0EnLo_RI) |
            ((target_ulong)tlb->XI0 << CP0EnLo_XI) |
#endif
            (tlb->C0 << 3) | (tlb->PFN[0] >> 6);
        lo1 = tlb->G | (tlb->V1 << 1) | (tlb->D1 << 2) |
#ifdef TARGET_CHERI
            ((target_ulong)tlb->S1 << CP0EnLo_S) |
            ((target_ulong)tlb->L1 << CP0EnLo_L) |
            ((target_ulong)tlb->CLG1 << CP0EnLo_CLG) |
#else
            ((target_ulong)tlb->RI1 << CP0EnLo_RI) |
            ((target_ulong)tlb->XI1 << CP0EnLo_XI) |
#endif
            (tlb->C1 << 3) | (tlb->PFN[1] >> 6);
    }
    qemu_log("    Write TLB[%u] = pgmsk:%08lx hi:%08lx lo0:%08lx lo1:%08lx\n",
            idx, (long)pagemask, (long)hi, (long)lo0, (long)lo1);
}

/*
 * Print changed values of COP0 registers.
 */
static void dump_changed_cop0_reg(CPUMIPSState *env, int idx,
        target_ulong value)
{
    if (value != env->last_cop0[idx]) {
        env->last_cop0[idx] = value;
        if (mips_cop0_regnames[idx])
            qemu_log("    Write %s = " TARGET_FMT_lx "\n",
                     mips_cop0_regnames[idx], value);
        else
            qemu_log("    Write (idx=%d) = " TARGET_FMT_lx "\n",
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
        dump_changed_cop0_reg(env, 2*8 + 3, PC_ADDR(env));
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

    dump_changed_cop0_reg(env, 14*8 + 0, get_CP0_EPC(env));

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

    dump_changed_cop0_reg(env, 17*8 + 0, env->CP0_LLAddr >> env->CP0_LLAddr_shift);
    dump_changed_cop0_reg(env, 17*8 + 1, env->lladdr);
    dump_changed_cop0_reg(env, 17*8 + 2, env->llval);
#ifdef TARGET_CHERI
    dump_changed_cop0_reg(env, 17*8 + 3, env->linkedflag);
#endif

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

    dump_changed_cop0_reg(env, 30*8 + 0, get_CP0_ErrorEPC(env));

    dump_changed_cop0_reg(env, 31*8 + 0, env->CP0_DESAVE);
    dump_changed_cop0_reg(env, 31*8 + 2, env->CP0_KScratch[0]);
    dump_changed_cop0_reg(env, 31*8 + 3, env->CP0_KScratch[1]);
    dump_changed_cop0_reg(env, 31*8 + 4, env->CP0_KScratch[2]);
    dump_changed_cop0_reg(env, 31*8 + 5, env->CP0_KScratch[3]);
    dump_changed_cop0_reg(env, 31*8 + 6, env->CP0_KScratch[4]);
    dump_changed_cop0_reg(env, 31*8 + 7, env->CP0_KScratch[5]);
}
#endif /* !CONFIG_USER_ONLY */


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
            qemu_log_mask(CPU_LOG_INSTR, "    Write %s = " TARGET_FMT_lx "\n",
                          gpr_name[i], cur->gpr[i]);
        }
    }
#ifdef TARGET_CHERI
    dump_changed_cop2(env, cur);
#endif
}


#endif // CONFIG_CHERI_LOG_INSTR

static void simple_dump_state(CPUMIPSState *env, FILE *f,
                              fprintf_function cpu_fprintf)
{

/* gxemul compat:
    cpu_fprintf(f, "pc = 0x" TARGET_FMT_lx "\n", PC_ADDR(env));
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
    cpu_fprintf(f, "DEBUG MIPS PC 0x" TARGET_FMT_lx "\n", PC_ADDR(env));
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
    FILE* logfile = qemu_log_enabled() ? qemu_log_lock() : stderr;
#if 0
    cpu_dump_state(env_cpu(env), logfile, fprintf, CPU_DUMP_CODE);
#else
    simple_dump_state(env, logfile, fprintf);
#endif
    if (logfile != stderr)
        qemu_log_unlock(logfile);
}
