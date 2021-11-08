/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2020 SRI International
 * All rights reserved.
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

/*
 * This file is a port of sys/kern/kern_flag.c from CTSRD-CHERI/CheriBSD.
 */

#include <sys/param.h>

#include "qemu.h"

static uint64_t security__flags_captured;
static uint64_t security__flags_captured_key;
static uint64_t security__kernel_flags_captured;
static uint64_t security__kernel_flags_captured_key;

static abi_long
do_freebsd_sysctl_flag_captured(CPUArchState *env, uint64_t value,
    void *holdp, size_t *holdlenp, void *hnewp, size_t newlen)
{

    if (hnewp != NULL) {
        errno = EPERM;
        return (-1);
    }
    if (holdp == NULL || holdlenp == NULL || *holdlenp < sizeof(value)) {
        errno = EINVAL;
        return (-1);
    }

    value = tswap64(value);
    *holdlenp = sizeof(value);
    (void)memcpy(holdp, &value, *holdlenp);

    return (0);
}

abi_long
do_freebsd_sysctl_oid_handle_security__flags_captured(CPUArchState *env,
    void *holdp, size_t *holdlenp, void *hnewp, size_t newlen)
{

    return (do_freebsd_sysctl_flag_captured(env, security__flags_captured,
        holdp, holdlenp, hnewp, newlen));
}

abi_long
do_freebsd_sysctl_oid_handle_security__flags_captured_key(CPUArchState *env,
    void *holdp, size_t *holdlenp, void *hnewp, size_t newlen)
{

    return (do_freebsd_sysctl_flag_captured(env, security__flags_captured_key,
        holdp, holdlenp, hnewp, newlen));
}

abi_long
do_freebsd_sysctl_oid_handle_security__kernel_flags_captured(CPUArchState *env,
    void *holdp, size_t *holdlenp, void *hnewp, size_t newlen)
{

    return (do_freebsd_sysctl_flag_captured(env,
        security__kernel_flags_captured, holdp, holdlenp, hnewp, newlen));
}

abi_long
do_freebsd_sysctl_oid_handle_security__kernel_flags_captured_key(
    CPUArchState *env, void *holdp, size_t *holdlenp, void *hnewp,
    size_t newlen)
{

    return (do_freebsd_sysctl_flag_captured(env,
        security__kernel_flags_captured_key, holdp, holdlenp, hnewp, newlen));
}

abi_long
do_freebsd_flag_captured(abi_syscallret_t retval, abi_syscallarg_t ua_message,
    abi_syscallarg_t ua_key, const char *source)
{
    struct target_flag_captured_args ua;
    char msg_buf[256];
    char src_buf[32];
    char key_buf[32];
    int error;

    ua.message = syscallarg_uintptr(ua_message);
    ua.key = syscallarg_int(ua_key);

    security__flags_captured++;

    /*
     * Try to copy message if it exists but don't return an error
     * if the pointer is bad (we need to log the event regardless).
     */
    if (uintptr_vaddr(ua.message) != 0) {
        error = copy_from_user(msg_buf, uintptr_vaddr(ua.message),
            sizeof(msg_buf));
        if (error != 0) {
            snprintf(msg_buf, sizeof(msg_buf),
                "error %d copying from %p", error,
                (const char *)uintptr_vaddr(ua.message));
        }
    } else
        strlcpy(msg_buf, "<null>", sizeof(msg_buf));

    if (source != NULL)
        strlcpy(src_buf, source, sizeof(src_buf));
    else
        strlcpy(src_buf, "<null>", sizeof(src_buf));

    if (ua.key == 0xfe77c0de) {
        security__flags_captured_key++;
        strlcpy(key_buf, "correct", sizeof(key_buf));
    } else {
        snprintf(key_buf, sizeof(key_buf), "incorrect (0x%x)", ua.key);
    }

    qemu_log("captured flag source: (%s) message: (%s) key: %s\n",
        src_buf, msg_buf, key_buf);

    *retval = *cheri_fromint(0);
    return (0);
}
