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

#pragma once

#include "cpu_log_instr.h"
#include "cpu.h"

/*
 * Instruction logging interface for targets.
 */

#ifdef CONFIG_CHERI_LOG_INSTR

#define CHERI_LOG_MASK (CPU_LOG_INST | CPU_LOG_CVTRACE | CPU_LOG_USER_ONLY)

/*
 * Initialize instruction logging for a cpu.
 */
void qemu_log_instr_init(CPUArchState *env);

/*
 * Start instruction tracing. Note that the instruction currently being
 * executed will be replaced by a trace start event.
 */
void qemu_log_instr_start(CPUArchState *env, uint32_t mode, target_ulong pc);

/*
 * Stop instruction tracing. Note that the instruction currently being
 * executed will be replaced by a trace stop event.
 * Mode is the mask of log modes to clear.
 */
void qemu_log_instr_stop(CPUArchState *env, uint32_t mode, target_ulong pc);

#define qemu_maybe_log_instr(op, ...) do {                      \
        if (unlikely(qemu_loglevel_mask(CHERI_LOG_MASK)))       \
            op(__VA_ARGS__);                                    \
    } while (0)

#define qemu_log_instr_mode_switch(...)                                 \
    qemu_maybe_log_instr(_qemu_log_instr_mode_switch, __VA_ARGS__)

#define qemu_log_instr_reg(...)                                 \
    qemu_maybe_log_instr(_qemu_log_instr_reg, __VA_ARGS__)

#ifdef TARGET_CHERI
#define qemu_log_instr_cap(...)                                 \
    qemu_maybe_log_instr(_qemu_log_instr_cap, __VA_ARGS__)
#else
#define qemu_log_instr_cap(...)
#endif

#define qemu_log_instr_pc(...)                                 \
    qemu_maybe_log_instr(_qemu_log_instr_pc, __VA_ARGS__)

#define qemu_log_instr_mem(...)                                 \
    qemu_maybe_log_instr(_qemu_log_instr_mem, __VA_ARGS__)

#define qemu_log_instr_instr(...)                               \
    qemu_maybe_log_instr(_qemu_log_instr_instr, __VA_ARGS__)

#define qemu_log_instr_hwtid(...)                               \
    qemu_maybe_log_instr(_qemu_log_instr_hwtid, __VA_ARGS__)

#define qemu_log_instr_asid(...)                                \
    qemu_maybe_log_instr(_qemu_log_instr_asid, __VA_ARGS__)

#define qemu_log_instr_exception(...)                           \
    qemu_maybe_log_instr(_qemu_log_instr_exception, __VA_ARGS__)

#define qemu_log_instr_env(...)                                 \
    qemu_maybe_log_instr(_qemu_log_instr_env, __VA_ARGS__)

#define qemu_log_instr_extra(...)                               \
    qemu_maybe_log_instr(_qemu_log_instr_extra, __VA_ARGS__)

#define qemu_log_instr_commit(...)                              \
    qemu_maybe_log_instr(_qemu_log_instr_commit, __VA_ARGS__)

/* TODO(am2419): decide whether to inline these */

/*
 * Switch user/kernel modes. Note this could be extended to support other rings
 * if needed.
 */
void _qemu_log_instr_mode_switch(CPUArchState *env, bool user, target_ulong pc);

/*
 * Drop the current buffered entry and ignore logging until next commit.
 */
void _qemu_log_instr_drop(CPUArchState *env);

/*
 * Notify that we are done with this instruction and the buffer
 * can be sent to the output.
 */
void _qemu_log_instr_commit(CPUArchState *env);

/*
 * Log changed general purpose register.
 */
void _qemu_log_instr_reg(CPUArchState *env, const char *reg_name,
                         target_ulong value);

#ifdef TARGET_CHERI
/*
 * Log changed capability register.
 */
void _qemu_log_instr_cap(CPUArchState *env, const char *reg_name,
                         cap_register_t *cr);
#endif

/*
 * Log instruction pc.
 */
void _qemu_log_instr_pc(CPUArchState *env, target_ulong pc);

/*
 * Log memory access performed by instruction
 */
void _qemu_log_instr_mem(CPUArchState *env, target_ulong addr);

/*
 * Log instruction opcode. Note that currently cvtrace only supports
 * 32-bit instructions.
 */
void _qemu_log_instr_instr(CPUArchState *env, target_ulong opcode_start);

/*
 * Log hardware thread id.
 * This is currently the cpu index in qemu.
 */
void _qemu_log_instr_hwtid(CPUArchState *env, uint8_t tid);

/*
 * Log ASID. Note on some architectures this is larger, we
 * may want to account for this.
 */
void _qemu_log_instr_asid(CPUArchState *env, uint8_t asid);

/*
 * Log exception code. Note on some architectures this is larger,
 * we may want to account for this.
 */
void _qemu_log_instr_exception(CPUArchState *env, uint8_t code);

/*
 * Log magic NOP event, we record a function number and 4 arguments.
 * Note that we have 6 bytes left in the cvtrace format, we may need
 * some trickery to reclaim those.
 */
void _qemu_log_instr_evt(CPUArchState *env, uint16_t fn, target_ulong arg0,
                         target_ulong arg1, target_ulong arg2,
                         target_ulong arg3);

/*
 * Log extra information as a string. Some logging formats may
 * ignore this.
 */
void _qemu_log_instr_extra(CPUArchState *env, const char *msg, ...);

#else /* ! defined(CONFIG_CHERI_LOG_INSTR) */
#define	qemu_log_instr_init(env)
#define	qemu_log_instr_start(env, mode, pc)
#define	qemu_log_instr_stop(env, mode, pc)
#define	qemu_log_instr_mode_switch(...)
#define	qemu_log_instr_reg(...)
#define	qemu_log_instr_cap(...)
#define	qemu_log_instr_pc(...)
#define	qemu_log_instr_mem(...)
#define	qemu_log_instr_instr(...)
#define	qemu_log_instr_hwtid(...)
#define	qemu_log_instr_asid(...)
#define	qemu_log_instr_exception(...)
#define	qemu_log_instr_env(...)
#define	qemu_log_instr_extra(...)
#define	qemu_log_instr_commit(...)
#endif /* ! defined(CONFIG_CHERI_LOG_INSTR) */
