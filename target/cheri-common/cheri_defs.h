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

#if defined CONFIG_DEBUG_TCG || defined QEMU_STATIC_ANALYSIS
# define cheri_debug_assert(X) do { assert(X); } while (0)
#else
# define cheri_debug_assert(X) ((void)0)
#endif

#ifdef TARGET_CHERI
#if (defined(CHERI_128) || defined(CHERI_64)) && !defined(CHERI_MAGIC128)
#define QEMU_USE_COMPRESSED_CHERI_CAPS 1
#else
#define QEMU_USE_COMPRESSED_CHERI_CAPS 0
#endif

#include "cheri-compressed-cap/cheri_compressed_cap.h"
#if defined(CHERI_64) || defined(CHERI_MAGIC64)
#error "Not implemented yet"
#elif defined(CHERI_128) || defined(CHERI_MAGIC128)
/* Magic 128 has the same set of permissions as 128 */
#define CAP_PERMS_ALL CC128_PERMS_ALL
#define CAP_UPERMS_ALL CC128_UPERMS_ALL
#define CAP_UPERMS_SHFT CC128_UPERMS_SHFT
#define CAP_MAX_UPERM CC128_MAX_UPERM
#define CAP_FLAGS_ALL_BITS CC128_FIELD_FLAGS_MASK_NOT_SHIFTED
#define CHERI_CAP_SIZE  16
#ifdef TARGET_WORDS_BIGENDIAN
#define CHERI_MEM_OFFSET_CURSOR 8
#define CHERI_MEM_OFFSET_METADATA 0
#else
#define CHERI_MEM_OFFSET_CURSOR 0
#define CHERI_MEM_OFFSET_METADATA 8
#endif
#else
#define CAP_PERMS_ALL CC256_PERMS_ALL_BITS
#define CAP_UPERMS_ALL CC256_UPERMS_ALL_BITS
#define CAP_UPERMS_SHFT CC256_UPERMS_SHFT
#define CAP_MAX_UPERM CC256_MAX_UPERM
#define CAP_FLAGS_ALL_BITS CC256_FLAGS_ALL_BITS
#define CHERI_CAP_SIZE  32
#ifdef TARGET_WORDS_BIGENDIAN
#define CHERI_MEM_OFFSET_METADATA 0
#define CHERI_MEM_OFFSET_CURSOR 8
#define CHERI_MEM_OFFSET_BASE 16
#define CHERI_MEM_OFFSET_LENGTH 24
#else
#error no little endian CHERI256
#endif
#endif

/* compressed capabilities use a 65-bit top, precise and magic 64-bits */
#if defined(CHERI_128)
#define CAP_MAX_LENGTH CC128_MAX_LENGTH
#define CAP_MAX_TOP CC128_MAX_TOP
#define CAP_MAX_REPRESENTABLE_OTYPE CC128_MAX_REPRESENTABLE_OTYPE
#define CAP_LAST_NONRESERVED_OTYPE CC128_LAST_NONRESERVED_OTYPE
#define CAP_OTYPE_UNSEALED CC128_OTYPE_UNSEALED
#define CAP_OTYPE_SENTRY CC128_OTYPE_SENTRY
#define CAP_FIRST_SPECIAL_OTYPE_SIGNED CC128_FIRST_SPECIAL_OTYPE_SIGNED
#define CAP_LAST_SPECIAL_OTYPE_SIGNED CC128_LAST_SPECIAL_OTYPE_SIGNED
#else
#error "64-bit capabilities not supported yet"
#endif

typedef signed __int128 cap_offset_t;
typedef unsigned __int128 cap_length_t;

typedef enum CheriPermissions {
    CAP_PERM_GLOBAL = (1 << 0),
    CAP_PERM_EXECUTE = (1 << 1),
    CAP_PERM_LOAD = (1 << 2),
    CAP_PERM_STORE = (1 << 3),
    CAP_PERM_LOAD_CAP = (1 << 4),
    CAP_PERM_STORE_CAP = (1 << 5),
    CAP_PERM_STORE_LOCAL = (1 << 6),
    CAP_PERM_SEAL = (1 << 7),
    CAP_PERM_CCALL = (1 << 8),
    CAP_PERM_UNSEAL = (1 << 9),
    CAP_ACCESS_SYS_REGS = (1 << 10),
    CAP_PERM_SETCID = (1 << 11),
    CAP_RESERVED4 = (1 << 12),
    CAP_RESERVED5 = (1 << 13),
    CAP_RESERVED6 = (1 << 14),
} CheriPermissions;

typedef enum CheriFlags {
    CHERI_FLAG_CAPMODE = (1 << 0),
} CheriFlags;

typedef enum CheriTbFlags {
    /* CHERI PCC is tagged, executable and unsealed */
    TB_FLAG_CHERI_PCC_VALID = (1 << 0),
    /* PCC.cr_flags == capmode */
    TB_FLAG_CHERI_CAPMODE = (1 << 1),
    /* DDC is tagged, unsealed and has PERM_LOAD */
    TB_FLAG_CHERI_DDC_READABLE = (1 << 2),
    /* DDC is tagged, unsealed and has PERM_STORE */
    TB_FLAG_CHERI_DDC_WRITABLE = (1 << 3),
    /* DDC is tagged, unsealed and base is zero */
    TB_FLAG_CHERI_DDC_BASE_ZERO = (1 << 4),
    /* DDC is tagged, unsealed and cursor (addresss) is zero */
    TB_FLAG_CHERI_DDC_CURSOR_ZERO = (1 << 5),
    /* DDC is tagged, unsealed and top is max_addr  */
    TB_FLAG_CHERI_DDC_TOP_MAX = (1 << 6),
    /* DDC spans the whole address space */
    TB_FLAG_CHERI_DDC_FULL_AS =
        (TB_FLAG_CHERI_DDC_BASE_ZERO | TB_FLAG_CHERI_DDC_TOP_MAX),
    /* No need to add a value to virtual address loads since DDC cursor and base
     * are zero. This handles both cases where interposition via DDC uses the
     * base or the cursor (in case we change our mind which one to use).
     * This is purely an optimization to skip an addition so it doesn't matter.
     */
    TB_FLAG_CHERI_DDC_NO_INTERPOSE =
        (TB_FLAG_CHERI_DDC_BASE_ZERO | TB_FLAG_CHERI_DDC_CURSOR_ZERO),
    /*
     * PCC spans the full adddress space and has base zero. This means we do
     * not need to perform bounds checks or subtract/add PCC.base
     */
    TB_FLAG_PCC_FULL_AS = (1 << 7)
} CheriTbFlags;
#endif // TARGET_CHERI
