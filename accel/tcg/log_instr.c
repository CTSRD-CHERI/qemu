/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2020 Alfredo Mazzinghi
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

#include <stdint.h>

#include "qemu/osdep.h"
#include "qemu/log.h"
#include "cpu.h"
#include "exec/log.h"
#include "exec/helper-proto.h"
#include "exec/log_instr.h"
#include "exec/memop.h"
#include "disas/disas.h"

#ifdef TARGET_CHERI
// cheri-common includes?
#endif

/*
 * CHERI common instruction logging.
 *
 * This is the central implementation of the CPU_LOG_INSTR, CPU_LOG_CVTRACE
 * and CPU_LOG_USER_ONLY methods.
 * The same functions can be used by CHERI targets to append to the instruction
 * log buffer. Once the instruction is fully processed, the target commits the
 * log buffer and depending on the instruction operations and -dfilter options
 * we either flush the buffer or drop it.
 * A central design goal is to deterministically output the main side effect of
 * the instruction (generally the destination register value), other side
 * effects, such as multiple register updates, may be implemented by emitting
 * multiple trace entries, but this is currently unimplemented.
 * Another design goal is to allow tracing of arbitrary events via nops in
 * both the text and binary format.
 */

#ifdef CONFIG_TCG_LOG_INSTR

#ifdef CONFIG_DEBUG_TCG
#define log_assert(x) assert((x))
#else
#define log_assert(x)
#endif

struct log_reginfo {
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
};
typedef struct log_reginfo log_reginfo_t;

#define reginfo_is_cap(ri) (ri->flags & LRI_CAP_REG)
#define reginfo_has_cap(ri) (reginfo_is_cap(ri) && (ri->flags & LRI_HOLDS_CAP))

struct log_meminfo {
    uint8_t flags;
#define LMI_NOMEM 0
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
};
typedef struct log_meminfo log_meminfo_t;

extern int cl_default_trace_format;

static cpu_log_instr_info_t *cpu_get_log(CPUState *cpu)
{
    return &cpu->log_info;
}

static cpu_log_instr_info_t *target_cpu_get_log(CPUArchState *env)
{
    return &env_cpu(env)->log_info;
}

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
        qemu_log("na    Cap Memory %s [" TARGET_FMT_lx "] = v:%d PESBT:"
                 TARGET_FMT_lx " Cursor:" TARGET_FMT_lx "\n",
                 direction, minfo->addr, minfo->cap.cr_tag,
                 compress_128cap(&minfo->cap),
                 cap_get_cursor(&minfo->cap));
    } else
#endif
    {
        switch (memop_size(minfo->op)) {
        default:
            qemu_log("    Unknown memory access width\n");
            /* fallthrough */
        case 8:
            qemu_log("na    Memory %s [" TARGET_FMT_lx "] = " TARGET_FMT_plx "\n",
                     direction, minfo->addr, minfo->value);
            break;
        case 4:
            qemu_log("na    Memory %s [" TARGET_FMT_lx "] = %08x\n",
                     direction, minfo->addr, (uint32_t)minfo->value);
            break;
        case 2:
            qemu_log("na    Memory %s [" TARGET_FMT_lx "] = %04x\n",
                     direction, minfo->addr, (uint16_t)minfo->value);
            break;
        case 1:
            qemu_log("na    Memory %s [" TARGET_FMT_lx "] = %02x\n",
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
            qemu_log("na    Write %s|" PRINT_CAP_FMTSTR_L1 "\n"
                     "             |" PRINT_CAP_FMTSTR_L2 "\n",
                     rinfo->name,
                     PRINT_CAP_ARGS_L1(&rinfo->cap),
                     PRINT_CAP_ARGS_L2(&rinfo->cap));
        else
            qemu_log("na  %s <- " TARGET_FMT_lx " (setting integer value)\n",
                     rinfo->name, rinfo->gpr);
    } else
#endif
    {
        qemu_log("na    Write %s = " TARGET_FMT_lx "\n", rinfo->name,
                 rinfo->gpr);
    }
}

/*
 * Emit textual trace entry to the log.
 */
static void emit_text_trace(CPUArchState *env, cpu_log_instr_info_t *log)
{
    int i;

    /* Dump main instruction log */
#if defined(TARGET_RISCV) && defined(CONFIG_RVFI_DII)
    /*
     * TODO(am2419): what to do with injected instructions?
     * Is the rvfi_dii_trace state valid at log commit?
     */
    if (env->rvfi_dii_have_injected_insn) {
        uint32_t insn = env->rvfi_dii_trace.rvfi_dii_insn;
        target_disas_buf(stderr, env_cpu(env), &insn, sizeof(insn),
                         PC_ADDR(env), 1);
    } else
#endif
    {
        log_target_disas(env_cpu(env), log->pc, /*only one*/-1);
    }

    /* Dump memory access */
    for (i = 0; i < log->mem->len; i++) {
        log_meminfo_t *minfo = &g_array_index(log->mem, log_meminfo_t, i);
        if (minfo->flags & LMI_LD) {
            emit_text_ldst(minfo, "Read");
        } else if (minfo->flags & LMI_ST) {
            emit_text_ldst(minfo, "Write");
        }
    }

    /* Dump register changes and side-effects */
    for (i = 0; i < log->regs->len; i++) {
        log_reginfo_t *rinfo = &g_array_index(log->regs, log_reginfo_t, i);
        emit_text_reg(rinfo);
    }

    /* Dump extra logged messages, if any */
    if (log->txt_buffer->len > 0)
        qemu_log("%s", log->txt_buffer->str);
}

/*
 * Emit binary trace entry to the log.
 */
static void emit_cvtrace(cpu_log_instr_info_t *log)
{
    // TODO(am2419): how do we get the opcode from pc? this is a machine-dependant
    // thing, maybe we can reuse disas or have an hook to determine the opcode length
    // can we get it from next instruction pc?

    /*     if (qemu_loglevel_mask(CPU_LOG_INSTR)) { */
    /*     g_string_append_printf( */
    /*         "    Write %s|" PRINT_CAP_FMTSTR_L1 "\n |" PRINT_CAP_FMTSTR_L2 "\n", */
    /*         reg_name, PRINT_CAP_ARGS_L1(cr), PRINT_CAP_ARGS_L2(cr)); */
    /* } else if (qemu_loglevel_mask(CPU_LOG_CVTRACE)) { */
    /*     uint64_t metadata = (((uint64_t)cr->cr_tag << 63) | */
    /*                          (cap_get_otype(cr) << 32) | */
    /*                          (COMBINED_PERMS_VALUE(cr) << 1) | */
    /*                          (uint64_t)(cap_is_unsealed(cr) ? 0 : 1)); */

    /*     log->cv_buffer.entry_type |= CVT_CAP; */
    /*     log->cv_buffer.val2 = metadata; */
    /*     log->cv_buffer.val3 = cap_get_cursor(cr); */
    /*     log->cv_buffer.val4 = cap_get_base(cr); */
    /*     log->cv_buffer.val5 = cap_get_length64(cr); */
    /* } */
}

static void emit_log_start(CPUArchState *env, target_ulong pc)
{
    if (qemu_loglevel_mask(CPU_LOG_INSTR)) {
        qemu_log("Requested instruction logging @ " TARGET_FMT_lx
                 " ASID %u\n", pc, cheri_get_asid(env));
        qemu_log("User-mode only tracing enabled at " TARGET_FMT_lx
                 ", ASID %u\n", pc, cheri_get_asid(env));
    } else if (qemu_loglevel_mask(CPU_LOG_CVTRACE)) {
        // TODO(am2419) Emit an event for instruction logging start
    }
}

static void emit_log_stop(CPUArchState *env, target_ulong pc)
{
    if (qemu_loglevel_mask(CPU_LOG_INSTR)) {
        if (qemu_loglevel_mask(CPU_LOG_USER_ONLY))
            qemu_log("User-mode only tracing disabled at " TARGET_FMT_lx
                     ", ASID %u\n", pc, cheri_get_asid(env));
        else
            qemu_log("Disabled instruction logging @ " TARGET_FMT_lx
                     " ASID %u\n", pc, cheri_get_asid(env));
    } else if (qemu_loglevel_mask(CPU_LOG_CVTRACE)) {
        // TODO(am2419) Emit an event for instruction logging stop
    }
}

static void reset_log_buffer(cpu_log_instr_info_t *log)
{
    g_array_remove_range(log->regs, 0, log->regs->len);
    g_array_remove_range(log->mem, 0, log->mem->len);
    g_string_erase(log->txt_buffer, 0, -1);
    log->force_drop = false;
}

void qemu_log_instr_init(CPUArchState *env)
{
    cpu_log_instr_info_t *log = target_cpu_get_log(env);

    memset((void *)log, 0, sizeof(*log));
    log->txt_buffer = g_string_new(NULL);
    log->regs = g_array_new(FALSE, TRUE, sizeof(log_reginfo_t));
    log->mem = g_array_new(FALSE, TRUE, sizeof(log_meminfo_t));
    log->force_drop = true;
}

/*
 * Callback invoked by tcg when flushing on state changed
 * TODO(am2419): should we move the flush callback here? it seems to be
 *  instr logging-related and only used for this.
 */
/*
void flush_tcg_on_log_instr_chage(void);
void flush_tcg_on_log_instr_chage(void) {
    warn_report("Calling real %s\r", __func__);
    CPUState *cpu;
    int cpu_index = 0;
    CPU_FOREACH(cpu) {
        warn_report("Flushing TCG for CPU %d\r", cpu_index++);
        tb_flush(cpu);
    }
}
*/

/*
 * Check for the instruction logging flags in qemu_loglevel.
 * If we are tracing user-mode, then we need to check the per-cpu
 * flag as multiple CPUs may be in different modes.
 */
bool qemu_log_instr_check_enabled(CPUArchState *env)
{
    if (qemu_loglevel_mask(CPU_LOG_USER_ONLY)) {
        return target_cpu_get_log(env)->user_mode_tracing;
    }

    return (qemu_loglevel_mask(INSTR_LOG_MASK));
}

/*
 * Start user tracing in the given mode.
 * Note:
 * Don't turn on tracing if user-mode only is selected and we are in the kernel.
 * Make sure that qemu_loglevel doesn't get set to zero when we
 * suspend tracing because otherwise qemu will close the logfile.
 *
 * Log start events are logged regardless of -dfilter and without waiting for
 * the commit() call.
 *
 * We do not emit a start event to the trace if the tracing is enabled from
 * command line or from the qemu monitor. This is because there is no associated
 * instruction/ASID to pin the event to.
 * TODO(am2419): Not sure if this is sensible, would be useful to have the event
 * for consistency anyway, but what to do with multiple cpus?
 */
void qemu_log_instr_start(CPUArchState *env, uint32_t mode, target_ulong pc)
{
    cpu_log_instr_info_t *log = target_cpu_get_log(env);
    uint32_t enabled = qemu_loglevel & INSTR_LOG_MASK;

    log_assert(log != NULL && "Invalid log buffer");
    log_assert((mode & CPU_LOG_INSTR) ^ (mode & CPU_LOG_CVTRACE) &&
        "Instruction logging must be enabled in either INSTR or CVTRACE mode");

    if ((mode & CPU_LOG_USER_ONLY)) {
        // TODO(am2419): looks like translator_ops::tb_in_user_mode(db, cpu) does
        // a very similar check, maybe there is a way to reuse it?
        if (cpu_in_user_mode(env)) {
            log->user_mode_tracing = true;
        } else {
            /* Delay enabling user-mode tracing */
            if ((mode & CPU_LOG_CVTRACE) == 0)
                qemu_log("Delaying tracing request at " TARGET_FMT_lx
                         " until next switch to user mode, ASID %u\n",
                         pc, cheri_get_asid(env));
            log->user_mode_tracing = false;
        }
    }

    qemu_set_log((qemu_loglevel & ~(INSTR_LOG_MASK)) | mode);

    /* If mode bits were not in enabled, we changed state */
    if ((mode & enabled) == 0)
        emit_log_start(env, pc);
    reset_log_buffer(log);
}

/*
 * Log stop events are logged regardless of -dfilter and without waiting for
 * the commit() call.
 */
void qemu_log_instr_stop(CPUArchState *env, uint32_t mode, target_ulong pc)
{
    cpu_log_instr_info_t *log = target_cpu_get_log(env);
    uint32_t enabled = qemu_loglevel & INSTR_LOG_MASK;

    log_assert(log != NULL && "Invalid log buffer");

    if (mode & CPU_LOG_USER_ONLY)
        log->user_mode_tracing = false;

    if (mode & enabled)
        emit_log_stop(env, pc);
    qemu_set_log(qemu_loglevel & ~(mode));
    reset_log_buffer(log);
}

/*
 * Toggle tracing if we are switching mode.
 * Note: this does not flush the tcg instruction buffer, this must be
 * done by the caller, if needed.
 * TODO(am2419): we need a version that flushes the tcg as well, we do not
 * want targets to care.
 */
void qemu_log_instr_mode_switch(CPUArchState *env, bool enable,
                                 target_ulong pc)
{
    cpu_log_instr_info_t *log = target_cpu_get_log(env);

    log_assert(log != NULL && "Invalid log info");

    if (qemu_loglevel_mask(CPU_LOG_USER_ONLY)) {
        log->user_mode_tracing = enable;
        // TODO(am2419): emit logging start/stop events.
    }
}

void qemu_log_instr_drop(CPUArchState *env)
{
    cpu_log_instr_info_t *log = target_cpu_get_log(env);

    log_assert(log != NULL && "Invalid log buffer");

    log->force_drop = true;
}

void qemu_log_instr_commit(CPUArchState *env)
{
    cpu_log_instr_info_t *log = target_cpu_get_log(env);

    log_assert(log != NULL && "Invalid log buffer");

    if (log->force_drop)
        goto out;
    // TODO(am2419) handle dfilter: need to check if enabled and if the entry matched

    if (qemu_loglevel_mask(CPU_LOG_INSTR))
        emit_text_trace(env, log);
    else if (qemu_loglevel_mask(CPU_LOG_CVTRACE))
        emit_cvtrace(log);

out:
    reset_log_buffer(log);
}

/*
 * TODO(am2419): how do we deal with orderding in case multiple registers are updated?
 * Maybe add a log_instr_reg_extra to distinguish from primary side-effect and other
 * side effects? (e.g. primary side effect is to update destination register, secondary
 * might be to clear status flag registers).
 * This is however an half-solution to the limitations imposed by the binary trace format,
 * we may just discard the assumption that there is any ordering at all and all side-effects
 * are granted equal rights under the constitution.
 */
void qemu_log_instr_reg(CPUArchState *env, const char *reg_name, target_ulong value)
{
    cpu_log_instr_info_t *log = target_cpu_get_log(env);
    log_reginfo_t r;

    r.flags = 0;
    r.name = reg_name;
    r.gpr = value;
    g_array_append_val(log->regs, r);
}

#ifdef TARGET_CHERI
void qemu_log_instr_cap(CPUArchState *env, const char *reg_name,
                         const cap_register_t *cr)
{
    cpu_log_instr_info_t *log = target_cpu_get_log(env);
    log_reginfo_t r;

    r.flags = LRI_CAP_REG | LRI_HOLDS_CAP;
    r.name = reg_name;
    r.cap = *cr;
    g_array_append_val(log->regs, r);
}

void qemu_log_instr_cap_int(CPUArchState *env, const char *reg_name,
                             target_ulong value)
{
    cpu_log_instr_info_t *log = target_cpu_get_log(env);
    log_reginfo_t r;

    r.flags = LRI_CAP_REG;
    r.name = reg_name;
    r.gpr = value;
    g_array_append_val(log->regs, r);
}
#endif

static inline void qemu_log_instr_mem_int(
    CPUArchState *env, target_ulong addr, int flags,
    MemOp op, target_ulong value)
{
    cpu_log_instr_info_t *log = target_cpu_get_log(env);
    log_meminfo_t m;

    m.flags = flags;
    m.op = op;
    m.addr = addr;
    m.value = value;
    g_array_append_val(log->mem, m);
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
    cpu_log_instr_info_t *log = target_cpu_get_log(env);
    log_meminfo_t m;

    m.flags = flags;
    m.op = 0;
    m.addr = addr;
    m.cap = *value;
    g_array_append_val(log->mem, m);
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

void qemu_log_instr_pc(CPUArchState *env, target_ulong pc)
{
    cpu_log_instr_info_t *log = target_cpu_get_log(env);

    log->pc = pc;
}

void qemu_log_instr_hwtid(CPUArchState *env, uint8_t tid)
{

    /* log->cv_buffer.thread = tid; */
}

void qemu_log_instr_asid(CPUArchState *env, uint8_t asid)
{

    /* log->cv_buffer.asid = asid; */
}

void qemu_log_instr_exception(CPUArchState *env, uint8_t code)
{

    /* log->cv_buffer.exception = code; */
}

void qemu_log_instr_evt(CPUArchState *env, uint16_t fn, target_ulong arg0,
                         target_ulong arg1, target_ulong arg2,
                         target_ulong arg3)
{
    /* log->cv_buffer.entry_type = CVT_EVT; */
    /* log->cv_buffer.val5 = fn; */
    /* log->cv_buffer.val1 = arg0; */
    /* log->cv_buffer.val2 = arg1; */
    /* log->cv_buffer.val3 = arg2; */
    /* log->cv_buffer.val4 = arg3; */
}

void qemu_log_instr_extra(CPUArchState *env, const char *msg, ...)
{
    cpu_log_instr_info_t *log = target_cpu_get_log(env);
    va_list va;

    va_start(va, msg);
    g_string_append_vprintf(log->txt_buffer, msg, va);
    va_end(va);
}

/* Instruction logging helpers */

void helper_qemu_log_instr_start(CPUArchState *env, target_ulong pc)
{
    cpu_log_instr_info_t *log = target_cpu_get_log(env);

    qemu_log_instr_start(env, cl_default_trace_format, pc);
    /* Skip this instruction commit */
    log->force_drop = true;
}

void helper_qemu_log_instr_stop(CPUArchState *env, target_ulong pc)
{
    qemu_log_instr_stop(env, cl_default_trace_format, pc);
}

void helper_qemu_log_instr_user_start(CPUArchState *env, target_ulong pc)
{
    cpu_log_instr_info_t *log = target_cpu_get_log(env);

    qemu_log_instr_start(env, cl_default_trace_format | CPU_LOG_USER_ONLY, pc);
    /* Skip this instruction commit */
    log->force_drop = true;
}

void helper_qemu_log_instr_user_stop(CPUArchState *env, target_ulong pc)
{
    qemu_log_instr_stop(env, cl_default_trace_format | CPU_LOG_USER_ONLY, pc);
}

void helper_qemu_log_instr_commit(CPUArchState *env)
{
    if (qemu_log_instr_enabled(env))
        qemu_log_instr_commit(env);
}

void helper_qemu_log_instr(CPUArchState *env, target_ulong pc)
{
    if (qemu_log_instr_enabled(env))
        qemu_log_instr_pc(env, pc);
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

#endif /* CONFIG_TCG_LOG_INSTR */
