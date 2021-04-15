/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2021 Lawrence Esswood
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

#ifndef QEMU_CHERI_TAGMEM_EX_LOCKS_H
#define QEMU_CHERI_TAGMEM_EX_LOCKS_H

typedef void *tag_lock_t;

// These will likely be TCG globals when we try use fewer helpers.
// We can actually tell what kind of lock a lock is as reads/writes are
// exclusive, So we do not actually need two different queues.
#define MAX_CHERI_EXCEPTION_LOCKS 2
typedef struct cheri_exception_locks_t {
    tag_lock_t locks[MAX_CHERI_EXCEPTION_LOCKS];
    // note: this index is just for push/pop. On an exception it is ignored,
    // and any non-null value in the lock array is freed.
    int fill;
} cheri_exception_locks_t;

#define CHERI_EXCEPTION_LOCKS cheri_exception_locks_t cheri_exception_locks;

#endif // QEMU_CHERI_TAGMEM_EX_LOCKS_H
