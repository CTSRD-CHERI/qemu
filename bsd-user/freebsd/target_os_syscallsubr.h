/*-
 * SPDX-License-Identifier: BSD-2-Clause-FreeBSD
 *
 * Copyright (c) 2002 Ian Dowse.  All rights reserved.
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
 *
 * $FreeBSD$
 */

/*
 * This file is a port of sys/sys/syscallsubr.h from CTSRD-CHERI/CheriBSD.
 */

#ifndef _TARGET_OS_SYSCALLSUBR_H_
#define _TARGET_OS_SYSCALLSUBR_H_

struct file;

typedef int (*mmap_check_fp_fn)(struct file *, int, int, int);

struct mmap_req {
    vm_offset_t      mr_hint;
    vm_offset_t      mr_max_addr;
    vm_size_t        mr_len;
    int              mr_prot;
    int              mr_flags;
    int              mr_fd;
    int              _int_pad;
    off_t            mr_pos;
    mmap_check_fp_fn mr_check_fp_fn;
#ifdef TARGET_CHERI
    cap_register_t mr_source_cap;
#endif
};

abi_long target_mmap_req(TaskState *ts, abi_syscallret_t retval,
    struct mmap_req *mrp);

#endif /* !_TARGET_OS_SYSCALLSUBR_H_ */
