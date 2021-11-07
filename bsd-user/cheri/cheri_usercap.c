/*-
 * Copyright (c) 2011-2017 Robert N. M. Watson
 * All rights reserved.
 *
 * This software was developed by SRI International and the University of
 * Cambridge Computer Laboratory under DARPA/AFRL contract (FA8750-10-C-0237)
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
 * This file is a port of sys/cheri/cheri_usercap.c from CTSRD-CHERI/CheriBSD.
 */

#include "qemu.h"

#include "cheri/cheric.h"
#include "cheri/cheri.h"
#include "cheri/cherireg.h"

#include "machine/cherireg.h"

cap_register_t userspace_cap;
cap_register_t userspace_sealcap;

/*
 * Build a new userspace capability derived from userspace_cap.
 * The resulting capability may include both read and execute permissions,
 * but not write, and will be a sentry capability. For architectures that use
 * flags, the flags for the resulting capability will be set based on what is
 * expected by userspace for the specified thread.
 */
cap_register_t *
cheri_capability_build_user_code(cap_register_t *cap, uint32_t perms,
    abi_ulong basep, size_t length, off_t off)
{

    assert((perms & ~CHERI_CAP_USER_CODE_PERMS) == 0);

    cap = cheri_capability_build_user_rwx(cap,
        perms & CHERI_CAP_USER_CODE_PERMS, basep, length, off);
#ifdef CHERI_FLAGS_CAP_MODE
    /* XXXKW: Check the current thread for SV_CHERI. */
    cap = cheri_setflags(cap, CHERI_FLAGS_CAP_MODE);
#endif

    /* XXXKW: Seal the capability. */
    return (cap);
}

/*
 * Build a new userspace capability derived from userspace_cap.
 * The resulting capability may include read, write, and execute permissions.
 *
 * This function violates W^X and its use is discouraged and the reason for
 * use should be documented in a comment when it is used.
 */
cap_register_t *
cheri_capability_build_user_rwx(cap_register_t *cap, uint32_t perms,
    abi_ulong basep, size_t length, off_t off)
{

    *cap = userspace_cap;
    cap = cheri_setoffset(cheri_andperm(cheri_setbounds(
        cheri_setoffset(cap, basep), length), perms), off);

    assert(cheri_getlen(cap) == length);

    return (cap);
}
