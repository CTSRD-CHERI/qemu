/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2018 SRI International
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

/*
 * This file is a port of sys/kern/sys_kbounce.c from CTSRD-CHERI/CheriBSD.
 */

#include <sys/param.h>

#include "qemu.h"

#include "target_os_systm.h"

abi_long
do_freebsd_kbounce(abi_syscallret_t retval, abi_syscallarg_t ua_src,
    abi_syscallarg_t ua_dst, abi_syscallarg_t ua_len, abi_syscallarg_t ua_flags)
{
    struct target_kbounce_args ua;
    void *bounce;
    abi_long error;

    ua.src = syscallarg_uintptr(ua_src);
    ua.dst = syscallarg_uintptr(ua_dst);
    ua.len = syscallarg_value(ua_len);
    ua.flags = syscallarg_value(ua_flags);

    bounce = NULL;

    if (ua.len > TARGET_IOSIZE_MAX) {
        error = -TARGET_EINVAL;
        goto error;
    }
    if (ua.flags != 0) {
        error = -TARGET_EINVAL;
        goto error;
    }
    if (uintptr_vaddr(ua.src) == 0 || uintptr_vaddr(ua.dst) == 0) {
        error = -TARGET_EINVAL;
        goto error;
    }

    bounce = calloc(1, ua.len);
    if (bounce == NULL) {
        switch (errno) {
        case EINVAL:
            error = -TARGET_EINVAL;
            break;
        case ENOMEM:
            error = -TARGET_ENOMEM;
            break;
        default:
            error = -TARGET_ENOMEM;
            break;
        }
        goto error;
    }

    error = copy_from_user(bounce, uintptr_vaddr(ua.src), ua.len);
    if (error != 0)
        goto error;
    error = copy_to_user(uintptr_vaddr(ua.dst), bounce, ua.len);
error:
    free(bounce);
#ifdef TARGET_CHERI
    *retval = *cheri_fromint(error);
#else
    *retval = error;
#endif
    return (error);
}
