/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2020 Alfredo Mazzinghi
 *
 * This software was developed by SRI International and the University of
 * Cambridge Computer Laboratory (Department of Computer Science and
 * Technology) under DARPA contract HR0011-18-C-0016 ("ECATS"), as part of the
 * DARPA SSITH research programme.
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

#include <stdint.h>

#include "qemu/osdep.h"
#include "qemu/range.h"
#include "qemu/log.h"
#include "cpu-param.h"
#include "cpu.h"
#include "exec/exec-all.h"
#include "exec/log.h"
#include "exec/helper-proto.h"
#include "exec/log_instr.h"
#include "exec/memop.h"
#include "disas/disas.h"

/*
 * CHERI common instruction logging.
 *
 * This is the central implementation of the CPU_LOG_INSTR tracing.
 * The same functions can be used by CHERI targets to append to the instruction
 * log buffer. Once the instruction is fully processed, the target commits the
 * log buffer and depending on the instruction operations and -dfilter options
 * we either flush the buffer or drop it.
 * A central desing goal is to reliably log multiple register updates and memory
 * accesses performed by an instruction. We also want to allow to log arbitary
 * events via special no-op instructions. Extra text debug output can also be
 * appended to the instruction log info.
 *
 * The output trace format can be easily changed by implementing a new set of
 * trace_fmt_hooks.
 *
 * The CPU_LOG_INSTR flag is used as a global enable to signal that logging is
 * active. Each CPU holds a private logging state, that can be controlled
 * individually.
 *
 * TODO(am2419): how do we deal with orderding in case multiple registers are updated?
 * This is critical to recognize which value goes in which register, and also how to
 * tie multiple memory accesses to the respective value/register.
 * We could add an explicit target-specific register ID handle in place of the
 * register name. This could be used also to fetch the register name and would
 * provide an identifier to external parsers.
 * Memory updates are harder to deal with, at least in the current format, perhaps
 * the semantic of the instruction is enough to recover the ordering from a trace.
 */

#ifdef CONFIG_TCG_LOG_INSTR

// #define CONFIG_DEBUG_TCG

#ifdef CONFIG_DEBUG_TCG
#define log_assert(x) assert((x))
#else
#define log_assert(x)
#endif

#ifndef TARGET_MAX_INSN_SIZE
#error "Target does not define TARGET_MAX_INSN_SIZE in cpu-param.h"
#endif

/*
 * -dfilter ranges in common logging implementation.
 */
extern GArray *debug_regions;

/*
 * Instruction log info associated with each committed log entry.
 * This is stored in the per-cpu log cpustate.
 */
typedef struct cpu_log_instr_info {
#define cpu_log_iinfo_startzero asid
    uint16_t asid;
    int flags;
/* Entry contains a synchronous exception */
#define LI_FLAG_INTR_TRAP 1
/* Entry contains an asynchronous exception */
#define LI_FLAG_INTR_ASYNC 2
#define LI_FLAG_INTR_MASK 0x3
/* Entry contains a CPU mode-switch and associated code */
#define LI_FLAG_MODE_SWITCH 4

    qemu_log_instr_cpu_mode_t next_cpu_mode;
    uint32_t intr_code;
    target_ulong intr_vector;
    target_ulong intr_faultaddr;

    target_ulong pc;
    /* Generic instruction opcode buffer */
    int insn_size;
    char insn_bytes[TARGET_MAX_INSN_SIZE];
#define cpu_log_iinfo_endzero mem
    /*
     * For now we allow multiple accesses to be tied to one instruction.
     * Some architectures may have multiple memory accesses
     * in the same instruction (e.g. x86-64 pop r/m64,
     * vector/matrix instructions, load/store pair). It is unclear
     * whether we would treat these as multiple trace "entities".
     *
     * Array of log_meminfo_t
     */
    GArray *mem;
    /* Register modifications. Array of log_reginfo_t */
    GArray *regs;
    /* Extra text-only log */
    GString *txt_buffer;
} cpu_log_instr_info_t;

/*
 * Register update info.
 * This records a CPU register update occurred during an instruction.
 */
typedef struct {
    uint16_t flags;
#define LRI_CAP_REG    1
#define LRI_HOLDS_CAP  2

    const char *name;
    union {
        target_ulong gpr;
#ifdef TARGET_CHERI
        cap_register_t cap;
#endif
    };
} log_reginfo_t;

#define reginfo_is_cap(ri) (ri->flags & LRI_CAP_REG)
#define reginfo_has_cap(ri) (reginfo_is_cap(ri) && (ri->flags & LRI_HOLDS_CAP))

/*
 * Memory access info.
 * This records a memory access occurred during an instruction.
 */
typedef struct {
    uint8_t flags;
#define LMI_LD 1
#define LMI_ST 2
#define LMI_CAP 4
    MemOp op;
    target_ulong addr;
    union {
        uint64_t value;
#ifdef TARGET_CHERI
        cap_register_t cap;
#endif
    };
} log_meminfo_t;

/*
 * Callbacks defined by a trace format implementation.
 * These are called to covert instruction tracing events to the corresponding
 * binary or text format.
 */
struct trace_fmt_hooks {
    void (*emit_header)(CPUArchState *env);
    void (*emit_start)(CPUArchState *env, target_ulong pc);
    void (*emit_stop)(CPUArchState *env, target_ulong pc);
    void (*emit_entry)(CPUArchState *env, cpu_log_instr_info_t *iinfo);
};
typedef struct trace_fmt_hooks trace_fmt_hooks_t;

/* Global trace format selector. Defaults to text tracing */
qemu_log_instr_fmt_t qemu_log_instr_format = QLI_FMT_TEXT;

/* Current format callbacks. */
static trace_fmt_hooks_t *trace_format = NULL;

/* Existing format callbacks list, indexed by qemu_log_instr_fmt_t */
static trace_fmt_hooks_t trace_formats[];

/*
 * CHERI binary trace format, originally used for MIPS only.
 * The format is limited to one entry per instruction, each
 * entry can hold at most one register modification and one
 * memory address.
 * Note that the CHERI format is the legacy MIPS format and
 * assumes big-endian byte order.
 */
typedef struct {
    uint8_t entry_type;
#define CTE_NO_REG  0   /* No register is changed. */
#define CTE_GPR     1   /* GPR change (val2) */
#define CTE_LD_GPR  2   /* Load into GPR (val2) from address (val1) */
#define CTE_ST_GPR  3   /* Store from GPR (val2) to address (val1) */
#define CTE_CAP     11  /* Cap change (val2,val3,val4,val5) */
#define CTE_LD_CAP  12  /* Load Cap (val2,val3,val4,val5) from addr (val1) */
#define CTE_ST_CAP  13  /* Store Cap (val2,val3,val4,val5) to addr (val1) */
    uint8_t exception;  /* 0=none, 1=TLB Mod, 2=TLB Load, 3=TLB Store, etc. */
#define CTE_EXCEPTION_NONE 31
    uint16_t cycles;    /* Currently not used. */
    uint32_t inst;      /* Encoded instruction. */
    uint64_t pc;        /* PC value of instruction. */
    uint64_t val1;      /* val1 is used for memory address. */
    uint64_t val2;      /* val2, val3, val4, val5 are used for reg content. */
    uint64_t val3;
    uint64_t val4;
    uint64_t val5;
    uint8_t thread;     /* Hardware thread/CPU (i.e. cpu->cpu_index ) */
    uint8_t asid;       /* Address Space ID */
} __attribute__((packed)) cheri_trace_entry_t;

/* Version 3 Cheri Stream Trace header info */
#define CTE_QEMU_VERSION    (0x80U + 3)
#define CTE_QEMU_MAGIC      "CheriTraceV03"

/*
 * Fetch the log state for a cpu.
 */
static inline cpu_log_instr_state_t *get_cpu_log_state(CPUArchState *env)
{
    return &env_cpu(env)->log_state;
}

/*
 * Fetch the given cpu current instruction info
 */
static inline cpu_log_instr_info_t *get_cpu_log_instr_info(CPUArchState *env)
{
    return get_cpu_log_state(env)->instr_info;
}

/* Text trace format emitters */

/*
 * Emit textual trace representation of memory access
 */
static inline void emit_text_ldst(log_meminfo_t *minfo, const char *direction)
{

#ifndef TARGET_CHERI
    log_assert((minfo->flags & LMI_CAP) == 0 &&
               "Capability memory access without CHERI support");
#else
    if (minfo->flags & LMI_CAP) {
        qemu_log("    Cap Memory %s [" TARGET_FMT_lx "] = v:%d PESBT:"
                 TARGET_FMT_lx " Cursor:" TARGET_FMT_lx "\n",
                 direction, minfo->addr, minfo->cap.cr_tag,
                 cc128_compress_mem(&minfo->cap),
                 cap_get_cursor(&minfo->cap));
    } else
#endif
    {
        switch (memop_size(minfo->op)) {
        default:
            qemu_log("    Unknown memory access width\n");
            /* fallthrough */
        case 8:
            qemu_log("    Memory %s [" TARGET_FMT_lx "] = " TARGET_FMT_plx "\n",
                     direction, minfo->addr, minfo->value);
            break;
        case 4:
            qemu_log("    Memory %s [" TARGET_FMT_lx "] = %08x\n",
                     direction, minfo->addr, (uint32_t)minfo->value);
            break;
        case 2:
            qemu_log("    Memory %s [" TARGET_FMT_lx "] = %04x\n",
                     direction, minfo->addr, (uint16_t)minfo->value);
            break;
        case 1:
            qemu_log("    Memory %s [" TARGET_FMT_lx "] = %02x\n",
                     direction, minfo->addr, (uint8_t)minfo->value);
            break;
        }
    }
}

/*
 * Emit textual trace representation of register modification
 */
static inline void emit_text_reg(log_reginfo_t *rinfo)
{
#ifndef TARGET_CHERI
    log_assert(!reginfo_is_cap(rinfo) && "Register marked as capability "
               "register whitout CHERI support");
#else
    if (reginfo_is_cap(rinfo)) {
        if (reginfo_has_cap(rinfo))
            qemu_log("    Write %s|" PRINT_CAP_FMTSTR_L1 "\n"
                     "             |" PRINT_CAP_FMTSTR_L2 "\n",
                     rinfo->name,
                     PRINT_CAP_ARGS_L1(&rinfo->cap),
                     PRINT_CAP_ARGS_L2(&rinfo->cap));
        else
            qemu_log("  %s <- " TARGET_FMT_lx " (setting integer value)\n",
                     rinfo->name, rinfo->gpr);
    } else
#endif
    {
        qemu_log("    Write %s = " TARGET_FMT_lx "\n", rinfo->name,
                 rinfo->gpr);
    }
}

/*
 * Emit textual trace entry to the log.
 */
static void emit_text_entry(CPUArchState *env, cpu_log_instr_info_t *iinfo)
{
    QemuLogFile *logfile;
    int i;

    /* Dump CPU-ID:ASID + address */
    qemu_log("[%d:%d] ", env_cpu(env)->cpu_index, iinfo->asid);

    /*
     * Instruction disassembly, note that we use the instruction info
     * opcode bytes, without accessing target memory here.
     */
    rcu_read_lock();
    logfile = atomic_rcu_read(&qemu_logfile);
    if (logfile) {
        target_disas_buf(logfile->fd, env_cpu(env), iinfo->insn_bytes,
                         sizeof(iinfo->insn_bytes), iinfo->pc, 1);
    }
    rcu_read_unlock();

    /*
     * TODO(am2419): what to do with injected instructions?
     * Is the rvfi_dii_trace state valid at log commit?
     */

    /* Dump mode switching info */
    if (iinfo->flags & LI_FLAG_MODE_SWITCH)
        qemu_log("-> Switch to %s mode\n", cpu_get_mode_name(iinfo->next_cpu_mode));
    /* Dump interrupt/exception info */
    switch (iinfo->flags & LI_FLAG_INTR_MASK) {
    case LI_FLAG_INTR_TRAP:
        qemu_log("-> Exception #%u vector 0x" TARGET_FMT_lx
                 " fault-addr 0x" TARGET_FMT_lx "\n",
                 iinfo->intr_code, iinfo->intr_vector, iinfo->intr_faultaddr);
        break;
    case LI_FLAG_INTR_ASYNC:
        qemu_log("-> Interrupt #%04x vector 0x" TARGET_FMT_lx "\n",
                 iinfo->intr_code, iinfo->intr_vector);
        break;
    default:
        /* No interrupt */
        break;
    }

    /* Dump memory access */
    for (i = 0; i < iinfo->mem->len; i++) {
        log_meminfo_t *minfo = &g_array_index(iinfo->mem, log_meminfo_t, i);
        if (minfo->flags & LMI_LD) {
            emit_text_ldst(minfo, "Read");
        } else if (minfo->flags & LMI_ST) {
            emit_text_ldst(minfo, "Write");
        }
    }

    /* Dump register changes and side-effects */
    for (i = 0; i < iinfo->regs->len; i++) {
        log_reginfo_t *rinfo = &g_array_index(iinfo->regs, log_reginfo_t, i);
        emit_text_reg(rinfo);
    }

    /* Dump extra logged messages, if any */
    if (iinfo->txt_buffer->len > 0)
        qemu_log("%s", iinfo->txt_buffer->str);
}

/*
 * Emit text tracing start event.
 */
static void emit_text_start(CPUArchState *env, target_ulong pc)
{
    cpu_log_instr_state_t *cpulog = get_cpu_log_state(env);

    if (cpulog->loglevel == QEMU_LOG_INSTR_LOGLEVEL_USER) {
        qemu_log("[%u:%u] Requested user-mode only instruction logging @ "
                 TARGET_FMT_lx " \n", env_cpu(env)->cpu_index,
                 cpu_get_asid(env), pc);
    } else {
        qemu_log("[%u:%u] Requested instruction logging @ " TARGET_FMT_lx " \n",
                 env_cpu(env)->cpu_index, cpu_get_asid(env), pc);
    }
}

/*
 * Emit text tracing stop event.
 */
static void emit_text_stop(CPUArchState *env, target_ulong pc)
{
    cpu_log_instr_state_t *cpulog = get_cpu_log_state(env);

    if (cpulog->loglevel == QEMU_LOG_INSTR_LOGLEVEL_USER) {
        qemu_log("[%u:%u] Disabled user-mode only instruction logging @ "
                 TARGET_FMT_lx " \n", env_cpu(env)->cpu_index,
                 cpu_get_asid(env), pc);
    } else {
        qemu_log("[%u:%u] Disabled instruction logging @ " TARGET_FMT_lx " \n",
                 env_cpu(env)->cpu_index, cpu_get_asid(env), pc);
    }
}

/* CHERI trace V3 format emitters */

/*
 * Emit cvtrace trace trace header. This is a magic byte + string
 */
static void emit_cvtrace_header(CPUArchState *env)
{
    FILE *logfile = qemu_log_lock();
    char buffer[sizeof(cheri_trace_entry_t)];

    buffer[0] = CTE_QEMU_VERSION;
    g_strlcpy(buffer + 1, CTE_QEMU_MAGIC, sizeof(buffer) - 2);
    fwrite(buffer, sizeof(buffer), 1, logfile);
    qemu_log_unlock(logfile);
}

/*
 * Emit cvtrace trace entry.
 * Note: this format is very MIPS-specific.
 */
static void emit_cvtrace_entry(CPUArchState *env, cpu_log_instr_info_t *iinfo)
{
    FILE *logfile;
    cheri_trace_entry_t entry;
    static uint16_t cycles = 0; // TODO(am2419): this should be a per-cpu counter.
    uint32_t *insn = (uint32_t *)&iinfo->insn_bytes[0];

    entry.entry_type = CTE_NO_REG;
    entry.thread = (uint8_t)env_cpu(env)->cpu_index;
    entry.asid = (uint8_t)iinfo->asid;
    entry.pc = cpu_to_be64(iinfo->pc);
    entry.cycles = cpu_to_be16(cycles++);
    /*
     * TODO(am2419): The instruction bytes are alread in target byte-order, however
     * cheritrace does not currently expect this.
     */
    entry.inst = cpu_to_be32(*insn);
    switch (iinfo->flags & LI_FLAG_INTR_MASK) {
    case LI_FLAG_INTR_TRAP:
        entry.exception = (uint8_t)(iinfo->intr_code & 0xff);
    case LI_FLAG_INTR_ASYNC:
        entry.exception = 0;
    default:
        entry.exception = CTE_EXCEPTION_NONE;
    }

    if (iinfo->regs->len) {
        log_reginfo_t *rinfo = &g_array_index(iinfo->regs, log_reginfo_t, 0);
#ifndef TARGET_CHERI
        log_assert(!reginfo_is_cap(rinfo) && "Capability register access "
                   "without CHERI support");
#else
        if (reginfo_is_cap(rinfo)) {
            cap_register_t intcap;
            cap_register_t *cr = &rinfo->cap;

            if (!reginfo_has_cap(rinfo)) {
                // cvtrace expects a null capability in the integer case
                cr = null_capability(&intcap);
            }
            uint64_t metadata = (
                ((uint64_t)cr->cr_tag << 63) | (cap_get_otype(cr) << 32) |
                (COMBINED_PERMS_VALUE(cr) << 1) |
                (uint64_t)(cap_is_unsealed(cr) ? 0 : 1));

            entry.entry_type = CTE_CAP;
            entry.val2 = cpu_to_be64(metadata);
            entry.val3 = cpu_to_be64(cap_get_cursor(cr));
            entry.val4 = cpu_to_be64(cap_get_base(cr));
            entry.val5 = cpu_to_be64(cap_get_length64(cr));
        } else
#endif
        {
            entry.entry_type = CTE_GPR;
            entry.val2 = cpu_to_be64(rinfo->gpr);
        }
    }

    if (iinfo->mem->len) {
        log_meminfo_t *minfo = &g_array_index(iinfo->mem, log_meminfo_t, 0);
#ifndef TARGET_CHERI
        log_assert((minfo->flags & LMI_CAP) == 0 && "Capability memory access "
                   "without CHERI support");
#endif
        entry.val1 = cpu_to_be64(minfo->addr);
        // Hack to avoid checking for GPR or CAP
        if (minfo->flags & LMI_LD)
            entry.entry_type += 1;
        else if (minfo->flags & LMI_ST)
            entry.entry_type += 2;
    }

    logfile = qemu_log_lock();
    fwrite(&entry, sizeof(entry), 1, logfile);
    qemu_log_unlock(logfile);
}

static void emit_cvtrace_start(CPUArchState *env, target_ulong pc)
{
    // TODO(am2419) Emit an event for instruction logging start
}

static void emit_cvtrace_stop(CPUArchState *env, target_ulong pc)
{
    // TODO(am2419) Emit an event for instruction logging stop
}

/* Core instruction logging implementation */

/* Reset instruction info buffer for next instruction */
static void reset_log_buffer(cpu_log_instr_state_t *cpulog,
                             cpu_log_instr_info_t *iinfo)
{
    memset(&iinfo->cpu_log_iinfo_startzero, 0,
           ((char *)&iinfo->cpu_log_iinfo_endzero -
            (char *)&iinfo->cpu_log_iinfo_startzero));
    g_array_remove_range(iinfo->regs, 0, iinfo->regs->len);
    g_array_remove_range(iinfo->mem, 0, iinfo->mem->len);
    g_string_erase(iinfo->txt_buffer, 0, -1);
    cpulog->force_drop = false;
    cpulog->starting = false;
}

/* Common instruction commit implementation */
static void do_instr_commit(CPUArchState *env)
{
    cpu_log_instr_state_t *cpulog = get_cpu_log_state(env);
    cpu_log_instr_info_t *iinfo = get_cpu_log_instr_info(env);

    log_assert(cpulog != NULL && "Invalid log state");
    log_assert(iinfo != NULL && "Invalid log buffer");

    if (cpulog->force_drop)
        return;

    if (cpulog->starting) {
        cpulog->starting = false;
        trace_format->emit_start(env, cpu_get_recent_pc(env));
        return;
    }

    /* Check for dfilter matches in this instruction */
    if (debug_regions) {
        int i, j;
        bool match = false;
        for (i = 0; !match && i < debug_regions->len; i++) {
            Range *range = &g_array_index(debug_regions, Range, i);
            match = range_contains(range, iinfo->pc);
            if (match)
                break;

            for (j = 0; j < iinfo->mem->len; j++) {
                log_meminfo_t *minfo = &g_array_index(iinfo->mem, log_meminfo_t, j);
                match = range_contains(range, minfo->addr);
                if (match)
                    break;
            }
        }
        if (match)
            trace_format->emit_entry(env, iinfo);
    } else {
        /* dfilter disabled, always log */
        trace_format->emit_entry(env, iinfo);
    }
}

/*
 * Perform the actual work to change per-CPU log level.
 * This runs in the CPU exclusive context.
 *
 * Note:
 * If we start logging, we delay emitting the start event until the next commit.
 * This is because on the path from the exclusive context to the translation
 * loop we may get an interrupt/exception causing a switch in CPU mode, causing
 * to stop logging. This would result in a pointless start/stop sequence with
 * no instructions executed in beteween.
 */
static void do_cpu_loglevel_switch(CPUState *cpu, run_on_cpu_data data)
{
    CPUArchState *env = cpu->env_ptr;
    cpu_log_instr_state_t *cpulog = get_cpu_log_state(env);
    cpu_log_instr_info_t *iinfo = get_cpu_log_instr_info(env);
    qemu_log_instr_loglevel_t prev_level = cpulog->loglevel;
    bool prev_level_active = cpulog->loglevel_active;
    qemu_log_instr_loglevel_t next_level = data.host_int;
    bool next_level_active;

    log_assert(qemu_loglevel_mask(CPU_LOG_INSTR));

    /* Decide whether we have to pause/resume logging */
    switch (next_level) {
    case QEMU_LOG_INSTR_LOGLEVEL_NONE:
        next_level_active = false;
        break;
    case QEMU_LOG_INSTR_LOGLEVEL_ALL:
        next_level_active = true;
        break;
    case QEMU_LOG_INSTR_LOGLEVEL_USER:
        /*
         * Assume iinfo holds the mode switch that caused cpu_loglevel_switch
         * to be called
         */
        if (iinfo->flags & LI_FLAG_MODE_SWITCH)
            next_level_active = (iinfo->next_cpu_mode == QEMU_LOG_INSTR_CPU_USER);
        else
            next_level_active = cpu_in_user_mode(env);
        break;
    default:
        log_assert(false && "Invalid cpu instruction log level");
        warn_report("Invalid cpu %d instruction log level\r",
                    cpu->cpu_index);
    }

    /* Update level */
    cpulog->loglevel = next_level;
    cpulog->loglevel_active = next_level_active;

    /* Check if this was a no-op */
    if (next_level == prev_level && prev_level_active == next_level_active)
        return;
    tb_flush(cpu);
    /* Emit start/stop events */
    if (prev_level_active) {
        if (cpulog->starting) {
            reset_log_buffer(cpulog, iinfo);
            return;
        }
        do_instr_commit(env);
        trace_format->emit_stop(env, cpu_get_recent_pc(env));
        reset_log_buffer(cpulog, iinfo);
    }
    if (next_level_active) {
        cpulog->starting = true;
    }
}

static void cpu_loglevel_switch(CPUArchState *env,
    qemu_log_instr_loglevel_t level)
{
    async_safe_run_on_cpu(env_cpu(env), do_cpu_loglevel_switch,
        RUN_ON_CPU_HOST_INT(level));
}

/*
 * Interface function to trigger a change in the CPU log level
 * while the global CPU_LOG_INSTR flag is being modified.
 */
void qemu_log_instr_flush_tcg(bool request_stop)
{
#ifdef CONFIG_DEBUG_TCG
    warn_report("Calling real %s\r", __func__);
    int cpu_index = 0;
#endif
    CPUState *cpu;
    CPU_FOREACH(cpu) {
#ifdef CONFIG_DEBUG_TCG
        warn_report("Flushing TCG for CPU %d\r", cpu_index++);
#endif
        if (request_stop) {
            cpu_loglevel_switch(cpu->env_ptr, QEMU_LOG_INSTR_LOGLEVEL_NONE);
        } else {
            cpu_loglevel_switch(cpu->env_ptr, QEMU_LOG_INSTR_LOGLEVEL_ALL);
        }
    }
}

/* Start global logging flag if it was disabled */
static void global_loglevel_enable()
{
    if (!qemu_loglevel_mask(CPU_LOG_INSTR))
        qemu_set_log_noflush(qemu_loglevel | CPU_LOG_INSTR);
}

/*
 * This must be called upon cpu creation.
 */
void qemu_log_instr_init(CPUState *cpu)
{
    cpu_log_instr_state_t *cpulog = &cpu->log_state;
    cpu_log_instr_info_t *iinfo = g_malloc0(sizeof(cpu_log_instr_info_t));

    iinfo->txt_buffer = g_string_new(NULL);
    iinfo->regs = g_array_new(FALSE, TRUE, sizeof(log_reginfo_t));
    iinfo->mem = g_array_new(FALSE, TRUE, sizeof(log_meminfo_t));

    cpulog->loglevel = QEMU_LOG_INSTR_LOGLEVEL_NONE;
    cpulog->loglevel_active = false;
    cpulog->instr_info = iinfo;
    reset_log_buffer(cpulog, iinfo);

    // Make sure we are using the correct trace format.
    if (trace_format == NULL) {
        trace_format = &trace_formats[qemu_log_instr_format];
        // Only emit header on first init
        if (trace_format->emit_header)
            trace_format->emit_header(cpu->env_ptr);
    }

    /* If we are starting with instruction logging enabled, switch it on now */
    if (qemu_loglevel_mask(CPU_LOG_INSTR))
        do_cpu_loglevel_switch(cpu,
            RUN_ON_CPU_HOST_INT(QEMU_LOG_INSTR_LOGLEVEL_ALL));
}

/*
 * Check whether instruction logging is enabled on this CPU.
 */
bool qemu_log_instr_check_enabled(CPUArchState *env)
{
    return (qemu_loglevel_mask(CPU_LOG_INSTR) &&
            get_cpu_log_state(env)->loglevel_active);
}

/*
 * Record a change in CPU mode.
 * Any instruction calling this should exit the TB.
 * This will also trigger pause and resume of user-only logging activity.
 *
 * We flush the TCG buffer when we have to change logging level, this
 * will cause an exit from the cpu_exec() loop, the bulk of the log level
 * switching is performed in exclusive context during the TCG flush
 * initiated here.
 */
void qemu_log_instr_mode_switch(CPUArchState *env,
    qemu_log_instr_cpu_mode_t mode, target_ulong pc)
{
    cpu_log_instr_state_t *cpulog = get_cpu_log_state(env);
    cpu_log_instr_info_t *iinfo = get_cpu_log_instr_info(env);

    log_assert(cpulog != NULL && "Invalid log state");
    log_assert(iinfo != NULL && "Invalid log info");

    iinfo->flags |= LI_FLAG_MODE_SWITCH;
    iinfo->next_cpu_mode = mode;

    /* If we are not logging in user-only mode, bail */
    if (!qemu_loglevel_mask(CPU_LOG_INSTR) ||
        cpulog->loglevel != QEMU_LOG_INSTR_LOGLEVEL_USER)
        return;

    /* Check if we are switching to an interesting mode */
    if ((mode == QEMU_LOG_INSTR_CPU_USER) != cpulog->loglevel_active) {
        cpu_loglevel_switch(env, cpulog->loglevel);
    }
}

void qemu_log_instr_drop(CPUArchState *env)
{
    cpu_log_instr_state_t *cpulog = get_cpu_log_state(env);

    log_assert(cpulog != NULL && "Invalid log state");

    cpulog->force_drop = true;
}

void qemu_log_instr_commit(CPUArchState *env)
{
    cpu_log_instr_state_t *cpulog = get_cpu_log_state(env);
    cpu_log_instr_info_t *iinfo = get_cpu_log_instr_info(env);

    log_assert(cpulog != NULL && "Invalid log state");
    log_assert(iinfo != NULL && "Invalid log info");

    do_instr_commit(env);
    reset_log_buffer(cpulog, iinfo);
}

void qemu_log_instr_reg(CPUArchState *env, const char *reg_name, target_ulong value)
{
    cpu_log_instr_info_t *iinfo = get_cpu_log_instr_info(env);
    log_reginfo_t r;

    r.flags = 0;
    r.name = reg_name;
    r.gpr = value;
    g_array_append_val(iinfo->regs, r);
}

#ifdef TARGET_CHERI
void qemu_log_instr_cap(CPUArchState *env, const char *reg_name,
                         const cap_register_t *cr)
{
    cpu_log_instr_info_t *iinfo = get_cpu_log_instr_info(env);
    log_reginfo_t r;

    r.flags = LRI_CAP_REG | LRI_HOLDS_CAP;
    r.name = reg_name;
    r.cap = *cr;
    g_array_append_val(iinfo->regs, r);
}

void qemu_log_instr_cap_int(CPUArchState *env, const char *reg_name,
                             target_ulong value)
{
    cpu_log_instr_info_t *iinfo = get_cpu_log_instr_info(env);
    log_reginfo_t r;

    r.flags = LRI_CAP_REG;
    r.name = reg_name;
    r.gpr = value;
    g_array_append_val(iinfo->regs, r);
}
#endif

static inline void qemu_log_instr_mem_int(
    CPUArchState *env, target_ulong addr, int flags,
    MemOp op, target_ulong value)
{
    cpu_log_instr_info_t *iinfo = get_cpu_log_instr_info(env);
    log_meminfo_t m;

    m.flags = flags;
    m.op = op;
    m.addr = addr;
    m.value = value;
    g_array_append_val(iinfo->mem, m);
}

void qemu_log_instr_ld_int(CPUArchState *env, target_ulong addr, MemOp op,
                           target_ulong value)
{
    qemu_log_instr_mem_int(env, addr, LMI_LD, op, value);
}

void qemu_log_instr_st_int(CPUArchState *env, target_ulong addr, MemOp op,
                           target_ulong value)
{
    qemu_log_instr_mem_int(env, addr, LMI_ST, op, value);
}

#ifdef TARGET_CHERI
/*
 * Note: logging the value here may be redundant as the capability is
 * generally loaded to a register and we get the value in the reginfo
 * as well. Need to think whether there is value to keep logging what
 * was loaded directly.
 */
static inline void qemu_log_instr_mem_cap(
    CPUArchState *env, target_ulong addr, int flags,
    const cap_register_t *value)
{
    cpu_log_instr_info_t *iinfo = get_cpu_log_instr_info(env);
    log_meminfo_t m;

    m.flags = flags;
    m.op = 0;
    m.addr = addr;
    m.cap = *value;
    g_array_append_val(iinfo->mem, m);
}

void qemu_log_instr_ld_cap(CPUArchState *env, target_ulong addr,
                           const cap_register_t *value)
{
    qemu_log_instr_mem_cap(env, addr, LMI_LD | LMI_CAP, value);
}
void qemu_log_instr_st_cap(CPUArchState *env, target_ulong addr,
                           const cap_register_t *value)
{
    qemu_log_instr_mem_cap(env, addr, LMI_ST | LMI_CAP, value);
}
#endif

void qemu_log_instr(CPUArchState *env, target_ulong pc, const char *insn,
                    uint32_t size)
{
    cpu_log_instr_info_t *iinfo = get_cpu_log_instr_info(env);

    iinfo->pc = pc;
    iinfo->insn_size = size;
    memcpy(iinfo->insn_bytes, insn, size);
}

void qemu_log_instr_asid(CPUArchState *env, uint16_t asid)
{
    cpu_log_instr_info_t *iinfo = get_cpu_log_instr_info(env);

    iinfo->asid = asid;
}

void qemu_log_instr_exception(CPUArchState *env, uint32_t code,
                              target_ulong vector, target_ulong faultaddr)
{
    cpu_log_instr_info_t *iinfo = get_cpu_log_instr_info(env);

    iinfo->flags |= LI_FLAG_INTR_TRAP;
    iinfo->intr_code = code;
    iinfo->intr_vector = vector;
    iinfo->intr_faultaddr = faultaddr;
}

void qemu_log_instr_interrupt(CPUArchState *env, uint32_t code,
                              target_ulong vector)
{
    cpu_log_instr_info_t *iinfo = get_cpu_log_instr_info(env);

    iinfo->flags |= LI_FLAG_INTR_ASYNC;
    iinfo->intr_code = code;
    iinfo->intr_vector = vector;
}

void qemu_log_instr_evt(CPUArchState *env, uint16_t fn, target_ulong arg0,
                         target_ulong arg1, target_ulong arg2,
                         target_ulong arg3)
{
    /* iinfo->cv_buffer.entry_type = CVT_EVT; */
    /* iinfo->cv_buffer.val5 = fn; */
    /* iinfo->cv_buffer.val1 = arg0; */
    /* iinfo->cv_buffer.val2 = arg1; */
    /* iinfo->cv_buffer.val3 = arg2; */
    /* iinfo->cv_buffer.val4 = arg3; */
}

void qemu_log_instr_extra(CPUArchState *env, const char *msg, ...)
{
    cpu_log_instr_info_t *iinfo = get_cpu_log_instr_info(env);
    va_list va;

    va_start(va, msg);
    g_string_append_vprintf(iinfo->txt_buffer, msg, va);
    va_end(va);
}

/* Instruction logging helpers */

/* Start logging all instructions on the current CPU */
void helper_qemu_log_instr_start(CPUArchState *env, target_ulong pc)
{
    cpu_log_instr_state_t *cpulog = get_cpu_log_state(env);

    log_assert(cpulog != NULL && "Invalid log state");
    global_loglevel_enable();

    /* If we are already started in the correct mode, bail */
    if (cpulog->loglevel == QEMU_LOG_INSTR_LOGLEVEL_ALL &&
        cpulog->loglevel_active)
        return;

    cpu_loglevel_switch(env, QEMU_LOG_INSTR_LOGLEVEL_ALL);
}

/* Start logging user-only instructions on the current CPU */
void helper_qemu_log_instr_user_start(CPUArchState *env, target_ulong pc)
{
    cpu_log_instr_state_t *cpulog = get_cpu_log_state(env);

    log_assert(cpulog != NULL && "Invalid log state");
    global_loglevel_enable();

    /* If we are already in the correct mode, bail */
    if (cpulog->loglevel == QEMU_LOG_INSTR_LOGLEVEL_USER)
        return;

    cpu_loglevel_switch(env, QEMU_LOG_INSTR_LOGLEVEL_USER);
}

/* Stop logging on the current CPU */
void helper_qemu_log_instr_stop(CPUArchState *env, target_ulong pc)
{

    cpu_loglevel_switch(env, QEMU_LOG_INSTR_LOGLEVEL_NONE);
}

/* Start logging all instructions on all CPUs */
void helper_qemu_log_instr_allcpu_start()
{
    CPUState *cpu;

    CPU_FOREACH(cpu) {
        helper_qemu_log_instr_start(cpu->env_ptr, 0);
    }
}

/* Start logging user-only instructions on all CPUs */
void helper_qemu_log_instr_allcpu_user_start()
{
    CPUState *cpu;

    CPU_FOREACH(cpu) {
        helper_qemu_log_instr_user_start(cpu->env_ptr, 0);
    }
}

/* Stop logging instructions on all CPUs */
void helper_qemu_log_instr_allcpu_stop()
{
    CPUState *cpu;

    CPU_FOREACH(cpu) {
        helper_qemu_log_instr_stop(cpu->env_ptr, 0);
    }
}

void helper_qemu_log_instr_commit(CPUArchState *env)
{
    qemu_log_instr_commit(env);
}

void helper_qemu_log_instr_load64(CPUArchState *env, target_ulong addr,
                                  uint64_t value, MemOp op)
{
    if (qemu_log_instr_enabled(env))
        qemu_log_instr_mem_int(env, addr, LMI_LD, op, value);
}

void helper_qemu_log_instr_store64(CPUArchState *env, target_ulong addr,
                                   uint64_t value, MemOp op)
{
    if (qemu_log_instr_enabled(env))
        qemu_log_instr_mem_int(env, addr, LMI_ST, op, value);
}

void helper_qemu_log_instr_load32(CPUArchState *env, target_ulong addr,
                                  uint32_t value, MemOp op)
{
    if (qemu_log_instr_enabled(env))
        qemu_log_instr_mem_int(env, addr, LMI_LD, op, (uint64_t)value);
}

void helper_qemu_log_instr_store32(CPUArchState *env, target_ulong addr,
                                   uint32_t value, MemOp op)
{
    if (qemu_log_instr_enabled(env))
        qemu_log_instr_mem_int(env, addr, LMI_ST, op, (uint64_t)value);
}

static void emit_nop_start(CPUArchState *env, target_ulong pc) {}
static void emit_nop_stop(CPUArchState *env, target_ulong pc) {}
static void emit_nop_entry(CPUArchState *env, cpu_log_instr_info_t *iinfo) {}

/*
 * Hooks for each trace format
 */
static trace_fmt_hooks_t trace_formats[] = {
    {
        .emit_header = NULL,
        .emit_start = emit_text_start,
        .emit_stop = emit_text_stop,
        .emit_entry = emit_text_entry
    },
    {
        .emit_header = emit_cvtrace_header,
        .emit_start = emit_cvtrace_start,
        .emit_stop = emit_cvtrace_stop,
        .emit_entry = emit_cvtrace_entry
    },
    {
        .emit_header = NULL,
        .emit_start = emit_nop_start,
        .emit_stop = emit_nop_stop,
        .emit_entry = emit_nop_entry
    }
};

#endif /* CONFIG_TCG_LOG_INSTR */
