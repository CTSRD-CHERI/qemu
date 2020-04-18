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
#include "disas/disas.h"

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

#ifdef CONFIG_CHERI_LOG_INSTR

struct log_reginfo {
    bool is_capability;
    const char *name;
    union {
        target_ulong gpr;
#ifdef TARGET_CHERI
        cap_register_t cap;
#endif
    };
};
typedef struct log_reginfo log_reginfo_t;

extern int cl_default_trace_format;

/*
 * Emit textual trace entry to the log.
 */
static void emit_text_trace(CPUArchState *env, cpu_log_buffer_t *log)
{
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
        log_target_disas(env_cpu(env), log->instr_begin,
                         /*only one*/-1);
    }

    /* Dump register changes and side-effects */


    /* Dump extra logged messages, if any */
    if (log->txt_buffer->len > 0)
        qemu_log("%s", log->txt_buffer->str);
}

/*
 * Emit binary trace entry to the log.
 */
static void emit_cvtrace(cpu_log_buffer_t *log)
{
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
        qemu_log("Requested instruction logging @ " TARGET_FMT_plx
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
            qemu_log("Disabled instruction logging @ " TARGET_FMT_plx " ASID %u\n",
                     pc, cheri_get_asid(env));
    } else if (qemu_loglevel_mask(CPU_LOG_CVTRACE)) {
        // TODO(am2419) Emit an event for instruction logging stop
    }
}

static void reset_log_buffer(cpu_log_buffer_t *log)
{
    g_array_remove_range(log->regs, 0, log->regs->len);
    g_string_erase(log->txt_buffer, 0, -1);
    log->force_drop = false;
}

void helper_qemu_log_instr_start(CPUArchState *env, target_ulong pc)
{
    cpu_log_buffer_t *log = cpu_get_log_buffer(env);

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
    cpu_log_buffer_t *log = cpu_get_log_buffer(env);

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
    _qemu_log_instr_commit(env);
}

void helper_qemu_log_instr(CPUArchState *env, target_ulong pc)
{
    _qemu_log_instr_instr(env, pc);
}

void qemu_log_instr_init(CPUArchState *env)
{
    cpu_log_buffer_t *log = cpu_get_log_buffer(env);

    memset((void *)log, 0, sizeof(*log));
    log->txt_buffer = g_string_new(NULL);
    log->regs = g_array_new(FALSE, TRUE, sizeof(log_reginfo_t));
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
    cpu_log_buffer_t *log = cpu_get_log_buffer(env);
    uint32_t enabled = qemu_loglevel & INSTR_LOG_MASK;

    assert(log != NULL && "Invalid log buffer");
    assert((mode & CPU_LOG_INSTR) ^ (mode & CPU_LOG_CVTRACE) &&
        "Instruction logging must be enabled in either INSTR or CVTRACE mode");

    if ((mode & CPU_LOG_USER_ONLY) && !cpu_in_user_mode(env)) {
        if ((mode & CPU_LOG_CVTRACE) == 0)
            qemu_log("Delaying tracing request at " TARGET_FMT_lx
                     " until next switch to user mode, ASID %u\n",
                     pc, cheri_get_asid(env));
        /* Delay enabling user-mode tracing */
        mode &= ~(CPU_LOG_INSTR | CPU_LOG_CVTRACE);
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
    cpu_log_buffer_t *log = cpu_get_log_buffer(env);
    uint32_t enabled = qemu_loglevel & INSTR_LOG_MASK;

    assert(log != NULL && "Invalid log buffer");

    if (mode & enabled)
        emit_log_stop(env, pc);
    qemu_set_log(qemu_loglevel & ~(mode));
    reset_log_buffer(log);
}

/*
 * Toggle tracing if we are switching mode.
 */
void _qemu_log_instr_mode_switch(CPUArchState *env, bool user, target_ulong pc)
{
    if (qemu_loglevel_mask(CPU_LOG_USER_ONLY)) {
        if (cpu_in_user_mode(env)) {
            qemu_set_log(qemu_loglevel | cl_default_trace_format |
                         CPU_LOG_USER_ONLY);
        } else {
            qemu_set_log((qemu_loglevel & ~(cl_default_trace_format)) |
                         CPU_LOG_USER_ONLY);
        }
    }
}

void _qemu_log_instr_drop(CPUArchState *env)
{
    cpu_log_buffer_t *log = cpu_get_log_buffer(env);

    assert(log != NULL && "Invalid log buffer");

    log->force_drop = true;
}

void _qemu_log_instr_commit(CPUArchState *env)
{
    cpu_log_buffer_t *log = cpu_get_log_buffer(env);

    assert(log != NULL && "Invalid log buffer");

    if (log->force_drop)
        goto out;
    // TODO(am2419) handle dfilter: need to check if enabled and if the entry matched

    if (qemu_loglevel_mask(CPU_LOG_INSTR))
        emit_text_trace(env, log);
    else if (qemu_loglevel_mask(CPU_LOG_CVTRACE))
        emit_cvtrace(log);

out:
    g_array_remove_range(log->regs, 0, log->regs->len);
    g_string_erase(log->txt_buffer, 0, -1);
    log->force_drop = false;
}

void _qemu_log_instr_reg(CPUArchState *env, const char *reg_name, target_ulong value)
{
    cpu_log_buffer_t *log = cpu_get_log_buffer(env);
    log_reginfo_t r;

    r.is_capability = false;
    r.name = reg_name;
    r.gpr = value;
    g_array_append_val(log->regs, r);
}

#ifdef TARGET_CHERI
void _qemu_log_instr_cap(CPUArchState *env, const char *reg_name, cap_register_t *cr)
{
    cpu_log_buffer_t *log = cpu_get_log_buffer(env);
    log_reginfo_t r;

    r.is_capability = true;
    r.name = reg_name;
    r.cap = *cr;
    g_array_append_val(log->regs, r);
}
#endif

void _qemu_log_instr_pc(CPUArchState *env, target_ulong pc)
{
    cpu_log_buffer_t *log = cpu_get_log_buffer(env);

    log->pc = pc;
}

void _qemu_log_instr_mem(CPUArchState *env, target_ulong addr)
{
    cpu_log_buffer_t *log = cpu_get_log_buffer(env);

    /* log->mem = addr; */
}

void _qemu_log_instr_instr(CPUArchState *env, target_ulong opcode_start)
{
    cpu_log_buffer_t *log = cpu_get_log_buffer(env);

    log->instr_begin = opcode_start;
}

void _qemu_log_instr_hwtid(CPUArchState *env, uint8_t tid)
{
    cpu_log_buffer_t *log = cpu_get_log_buffer(env);

    /* log->cv_buffer.thread = tid; */
}

void _qemu_log_instr_asid(CPUArchState *env, uint8_t asid)
{
    cpu_log_buffer_t *log = cpu_get_log_buffer(env);

    /* log->cv_buffer.asid = asid; */
}

void _qemu_log_instr_exception(CPUArchState *env, uint8_t code)
{
    cpu_log_buffer_t *log = cpu_get_log_buffer(env);

    /* log->cv_buffer.exception = code; */
}

void _qemu_log_instr_evt(CPUArchState *env, uint16_t fn, target_ulong arg0,
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

void _qemu_log_instr_extra(CPUArchState *env, const char *msg, ...)
{
    cpu_log_buffer_t *log = cpu_get_log_buffer(env);
    va_list va;

    va_start(va, msg);
    g_string_append_vprintf(log->txt_buffer, msg, va);
    va_end(va);
}

#endif /* CONFIG_CHERI_LOG_INSTR */
