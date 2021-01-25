/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2020 Lawrence Esswood
 *
 * This work was supported by Innovate UK project 105694, "Digital Security
 * by Design (DSbD) Technology Platform Prototype".
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

#ifndef QEMU_LOG_INSTR_EARLY_H
#define QEMU_LOG_INSTR_EARLY_H

// Format specifiers for TCGv and TCGv_ptr
#if TARGET_LONG_BITS == 64
#define QLP_TCGV    "d"
#else
#define QLP_TCGV    "w"
#endif
#if UINTPTR_MAX == UINT32_MAX
#define QLP_TCGP    "w"
#else
#define QLP_TCGP    "d"
#endif

// Max printf args
#define QEMU_LOG_PRINTF_ARG_MAX              8
// Max printf's before flush
#define QEMU_LOG_PRINTF_BUF_DEPTH            32
// Early flush if buffer gets this full
#define QEMU_LOG_PRINTF_FLUSH_BARRIER        32

typedef union {
    char                charv;
    short               shortv;
    unsigned short      ushortv;
    int                 intv;
    unsigned int        uintv;
    long                longv;
    unsigned long       ulongv;
    long long           longlongv;
    unsigned long long  ulonglongv;
    float               floatv;
    double              doublev;
    void*               ptrv;
} qemu_log_arg_t;

// Stick one of these in your CPUState.
typedef struct {
    qemu_log_arg_t args[QEMU_LOG_PRINTF_ARG_MAX*QEMU_LOG_PRINTF_BUF_DEPTH]; // arguments to printf calls
    const char* fmts[QEMU_LOG_PRINTF_BUF_DEPTH]; // the printf fmts
    uint64_t valid_entries; // bitmap of which entries are valid
} qemu_log_printf_buf_t;

#endif //QEMU_LOG_INSTR_EARLY_H
