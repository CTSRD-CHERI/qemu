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

#pragma once

#include "hw/core/cpu.h"
#include "qemu/log_instr.h"
#include "cpu.h"

/*
 * Instruction logging interface for targets.
 *
 * # Adding a new target
 * The following functions must be defined by the target and declared in cpu.h:
 * - bool cpu_in_user_mode(env)
 *   return whether the cpu is in user mode
 * - uint16_t cpu_get_asid(env)
 *   return the hardware address space identifier
 * - const char *cpu_get_mode_name(mode)
 *   return the mode name associated with a qemu_log_instr_cpu_mode_t for printing.
 *
 * - Each target should implement their own register update logging helpers that
 *   call into qemu_log_instr_gpr(), qemu_log_instr_cap() and similar interface
 *   functions.
 *   Note that the target is responsible for producing a register name to use.
 * - It is recommended to use the helper_<target>_log_instr_{gpr,cap..}() naming
 *   convention to distinguish target-specific and generic qemu_log_instr
 *   interfaces.
 * - The discipline for checking whether instruction logging is enabled is
 *   the following:
 *   + TCG helpers should be emitted after checking the cached value
 *     of DisasContextBase::log_instr_enabled.
 *   + TCG logging helpers internally must check whether logging is enabled,
 *     this is because sometimes the DisasContext is not available to check
 *     the condition above.
 *   + qemu_log_instr_* functions do not internally check whether logging is
 *     enabled. This must be done by the caller or via a macro.
 *   + Use the macro qemu_log_instr_enabled to check whether instruction logging
 *     is enabled. This can be used in `if (...)` statements without adding
 *     `unlikely(...)` built-ins.
 *
 * TODO(am2419): decide whether to inline some of these.
 */

#define _glue_args(...) , ## __VA_ARGS__

/*
 * Helper to simplify checking for either instruction logging or
 * another loglevel enabled
 */
#define qemu_log_instr_or_mask_enabled(env, mask)       \
    ((unlikely(qemu_loglevel_mask(mask)) ||             \
      qemu_log_instr_enabled(env)) ? true : false)

/*
 * Helper to simplify emitting a message either to instruction
 * logging extra text buffer or when another loglevel is enabled
 */
#define qemu_log_instr_or_mask_msg(env, mask, msg, ...) do {            \
        if (qemu_loglevel_mask(mask)) {                                 \
            qemu_log(msg _glue_args(__VA_ARGS__));                      \
        } else if (qemu_log_instr_enabled(env)) {                       \
            qemu_log_instr_extra(env, msg _glue_args(__VA_ARGS__));     \
        }                                                               \
    } while (0)

/*
 * Helper version of qemu_log_instr_extra that checks whether logging is
 * enabled.
 */
#define qemu_maybe_log_instr_extra(env, msg, ...) do {                  \
        if (qemu_log_instr_enabled(env))                                \
            qemu_log_instr_extra(env, msg _glue_args(__VA_ARGS__));     \
    } while (0)

/*
 * Helper to append a changed register content to the extra debug info
 * for the current instruction.
 * Note that this can be ignored by the output trace format.
 */
#define qemu_log_instr_dbg_reg(env, name, value) do {                   \
        if (qemu_log_instr_enabled(env))                                \
            qemu_log_instr_extra(env, "    Write %s = " TARGET_FMT_lx "\n", \
                                 name, value);                          \
    } while (0)

/*
 * Helper to append a changed capability register content to the extra debug
 * info for the current instruction.
 * Note that this can be ignored by the output trace format.
 */
#define qemu_log_instr_dbg_cap(env, name, value) do {                   \
        if (qemu_log_instr_enabled(env))                                \
            qemu_log_instr_extra(                                       \
                env, "    Write %s|" PRINT_CAP_FMTSTR_L1 "\n"           \
                "             |" PRINT_CAP_FMTSTR_L2 "\n",              \
                name, PRINT_CAP_ARGS_L1(value),                         \
                PRINT_CAP_ARGS_L2(value));                              \
    } while (0)

#ifdef CONFIG_TCG_LOG_INSTR

/*
 * Request a flush of the TCG when changing loglevel outside of qemu_log_instr.
 * TODO(am2419): this should be removed from the interface.
 */
void qemu_log_instr_flush_tcg(bool request_stop);

/* Helper macro to check for instruction logging enabled */
#define	qemu_log_instr_enabled(env)                                     \
    (unlikely(qemu_log_instr_check_enabled((env))) ? true : false)

/*
 * Check whether instruction tracing is enabled.
 * Note: changes to the instruction logging flags result in a call to
 * tb_flush so we can do the logging checks at translate time as well.
 */
bool qemu_log_instr_check_enabled(CPUArchState *env);

/*
 * Start instruction tracing. Note that the instruction currently being
 * executed will be replaced by a trace start event.
 */
void qemu_log_instr_start(CPUArchState *env, target_ulong pc);

/*
 * Stop instruction tracing.
 */
void qemu_log_instr_stop(CPUArchState *env, target_ulong pc);

/*
 * Log a switch inc CPU modes.
 * This will also trigger pause and resume of user-only logging activity,
 * depending whether the mode parameter is QEMU_LOG_INSTR_CPU_USER or not.
 */
void qemu_log_instr_mode_switch(CPUArchState *env,
    qemu_log_instr_cpu_mode_t mode, target_ulong pc);

/*
 * Set the given CPU per-CPU log level.
 */
void qemu_log_instr_set_level(CPUArchState *env, qemu_log_instr_loglevel_t lvl);

/*
 * Set the per-CPU log level for all the CPUs.
 */
void qemu_log_instr_allcpu_set_level(qemu_log_instr_loglevel_t lvl);

/*
 * Drop the current buffered entry and ignore logging until next commit.
 */
void qemu_log_instr_drop(CPUArchState *env);

/*
 * Notify that we are done with this instruction and the buffer
 * can be sent to the output.
 */
void qemu_log_instr_commit(CPUArchState *env);

/*
 * Log changed general purpose register.
 */
void qemu_log_instr_reg(CPUArchState *env, const char *reg_name,
                        target_ulong value);

/*
 * Log integer memory load.
 */
void qemu_log_instr_ld_int(CPUArchState *env, target_ulong addr, MemOp op,
                           target_ulong value);

/*
 * Log integer memory store.
 */
void qemu_log_instr_st_int(CPUArchState *env, target_ulong addr, MemOp op,
                           target_ulong value);

#ifdef TARGET_CHERI
/*
 * Log changed capability register.
 */
void qemu_log_instr_cap(CPUArchState *env, const char *reg_name,
                        const cap_register_t *cr);

/*
 * Log changed capability register with integer value.
 */
void qemu_log_instr_cap_int(CPUArchState *env, const char *reg_name,
                            target_ulong value);

/*
 * Log capability memory load.
 */
void qemu_log_instr_ld_cap(CPUArchState *env, target_ulong addr,
                           const cap_register_t *value);

/*
 * Log capability memory store.
 */
void qemu_log_instr_st_cap(CPUArchState *env, target_ulong addr,
                           const cap_register_t *value);
#endif

/*
 * Log instruction pc and opcode.
 * Note that the insn buffer is expected to be in target byte-order.
 */
void qemu_log_instr(CPUArchState *env, target_ulong pc, const char *insn,
                    uint32_t size);

/*
 * Log Address Space IDentifier (also known as PCID).
 */
void qemu_log_instr_asid(CPUArchState *env, uint16_t asid);

/*
 * Log exception event.
 */
void qemu_log_instr_exception(CPUArchState *env, uint32_t code,
                              target_ulong vector, target_ulong faultaddr);

/*
 * Log interrupt event.
 */
void qemu_log_instr_interrupt(CPUArchState *env, uint32_t code, target_ulong vector);

/*
 * Log magic NOP event, we record a function number and 4 arguments.
 * Note that we have 6 bytes left in the cvtrace format, we may need
 * some trickery to reclaim those.
 */
void qemu_log_instr_evt(CPUArchState *env, uint16_t fn, target_ulong arg0,
                        target_ulong arg1, target_ulong arg2,
                        target_ulong arg3);

/*
 * Log extra information as a string. Some logging formats may
 * ignore this.
 */
void qemu_log_instr_extra(CPUArchState *env, const char *msg, ...);

#else /* ! CONFIG_TCG_LOG_INSTR */
#define	qemu_log_instr_enabled(cpu) false
#define	qemu_log_instr_start(env, mode, pc)
#define	qemu_log_instr_stop(env, mode, pc)
#define	qemu_log_instr_mode_switch(...)
#define	qemu_log_instr_reg(...)
#define	qemu_log_instr_cap(...)
#define	qemu_log_instr_mem(...)
#define	qemu_log_instr_instr(...)
#define	qemu_log_instr_hwtid(...)
#define	qemu_log_instr_asid(...)
#define	qemu_log_instr_exception(...)
#define	qemu_log_instr_interrupt(...)
#define	qemu_log_instr_env(...)
#define	qemu_log_instr_extra(...)
#define	qemu_log_instr_commit(...)
#endif /* ! CONFIG_TCG_LOG_INSTR */
