/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2020 (holder)
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

/*
 * CPU-independant instruction logging configuration helpers.
 * These are generally used during initialization to setup
 * logging.
 */

#ifdef CONFIG_TCG_LOG_INSTR
/*
 * Instruction logging format
 */
typedef enum {
    QLI_FMT_TEXT = 0,
    QLI_FMT_CVTRACE = 1
} qemu_log_instr_fmt_t;

extern qemu_log_instr_fmt_t qemu_log_instr_format;

static inline void qemu_log_instr_set_format(qemu_log_instr_fmt_t fmt)
{
    qemu_log_instr_format = fmt;
}

static inline qemu_log_instr_fmt_t qemu_log_instr_get_format()
{
    return qemu_log_instr_format;
}

struct cpu_log_instr_info;

/*
 * Per-cpu logging state.
 */
typedef struct {
    /* CPU tracing user mode only enable flag */
    bool user_mode_tracing;
    /*
     * Opaque handle to the current instruction info
     * TODO(am2419): It would be interesting to have a ring buffer
     * of log_instr_info here, so that we can avoid dumping to file
     * all the time.
     */
    struct cpu_log_instr_info *instr_info;
} cpu_log_instr_state_t;

/*
 * Initialize instruction logging for a cpu.
 */
void qemu_log_instr_init(CPUState *env);

#else /* ! CONFIG_TCG_LOG_INSTR */
#define qemu_log_instr_set_format(fmt) ((void)0)
#endif /* ! CONFIG_TCG_LOG_INSTR */
