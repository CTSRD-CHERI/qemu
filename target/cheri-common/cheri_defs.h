/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2020 Alex Richardson
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
#pragma once


#if (defined(CHERI_128) || defined(CHERI_64)) && !defined(CHERI_MAGIC128)
#define QEMU_USE_COMPRESSED_CHERI_CAPS 1
#else
#define QEMU_USE_COMPRESSED_CHERI_CAPS 0
#endif

#include "cheri-compressed-cap/cheri_compressed_cap.h"
#if defined(CHERI_128) || defined(CHERI_MAGIC128)
/* Magic 128 has the same set of permissions as 128 */
#define CAP_PERMS_ALL CC128_PERMS_ALL
#define CAP_UPERMS_ALL CC128_UPERMS_ALL
#define CAP_UPERMS_SHFT CC128_UPERMS_SHFT
#define CAP_MAX_UPERM CC128_MAX_UPERM
#else
#define CAP_PERMS_ALL CC256_PERMS_ALL_BITS
#define CAP_UPERMS_ALL CC256_UPERMS_ALL_BITS
#define CAP_UPERMS_SHFT CC256_UPERMS_SHFT
#define CAP_MAX_UPERM CC256_MAX_UPERM
#endif

/* compressed capabilities use a 65-bit top, precise and magic 64-bits */
#if defined(CHERI_128)
#define CAP_MAX_LENGTH CC128_NULL_LENGTH
#define CAP_MAX_TOP CC128_NULL_TOP
#else
#define CAP_MAX_LENGTH CC256_NULL_LENGTH
#define CAP_MAX_TOP CC256_NULL_TOP
#endif

typedef signed __int128 cap_offset_t;
typedef unsigned __int128 cap_length_t;

#define CAP_PERM_GLOBAL         (1 << 0)
#define CAP_PERM_EXECUTE        (1 << 1)
#define CAP_PERM_LOAD           (1 << 2)
#define CAP_PERM_STORE          (1 << 3)
#define CAP_PERM_LOAD_CAP       (1 << 4)
#define CAP_PERM_STORE_CAP      (1 << 5)
#define CAP_PERM_STORE_LOCAL    (1 << 6)
#define CAP_PERM_SEAL           (1 << 7)
#define CAP_PERM_CCALL          (1 << 8)
#define CAP_PERM_UNSEAL         (1 << 9)
#define CAP_ACCESS_SYS_REGS     (1 << 10)
#define CAP_PERM_SETCID         (1 << 11)
#define CAP_RESERVED4           (1 << 12)
#define CAP_RESERVED5           (1 << 13)
#define CAP_RESERVED6           (1 << 14)
