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

#include "cpu-param.h"

#ifdef CONFIG_TCG_LOG_INSTR

#ifndef TARGET_MAX_INSN_SIZE
#error "Target does not define TARGET_MAX_INSN_SIZE in cpu-param.h"
#endif

/*
 * The log buffer is associated with the cpu arch state, each target should
 * define the cpu_get_log_buffer() function/macro to retrieve a pointer to
 * the log buffer. The per-cpu log buffer structure is defined below.
 */
typedef struct cpu_log_instr_info {
    bool user_mode_tracing;
#define cpu_log_iinfo_startzero force_drop
    bool force_drop;
    uint16_t asid;
    int flags;
#define LI_FLAG_INTR_TRAP 1
#define LI_FLAG_INTR_ASYNC 2
#define LI_FLAG_INTR_MASK 0x3
    uint32_t intr_code;
    // TODO(am2419): should be target_ulong
    uint64_t intr_vector;
    uint64_t intr_faultaddr;

    uint64_t pc;
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
#endif /* CONFIG_TCG_LOG_INSTR */
