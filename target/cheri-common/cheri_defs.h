/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2020 Alex Richardson
 * Copyright (c) 2021 Microsoft <robert.norton@microsoft.com>
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

#include "cheri-compressed-cap/cheri_compressed_cap.h"

#if TARGET_LONG_BITS == 32
#  define CHERI_CAP_SIZE 8
#  define CHERI_CAP_BITS 64
#elif TARGET_LONG_BITS == 64
#  define CHERI_CAP_SIZE 16
#  define CHERI_CAP_BITS 128
#else
#  error "Unknown CHERI capability format"
#endif

#ifdef TARGET_WORDS_BIGENDIAN
#  define CHERI_MEM_OFFSET_CURSOR TARGET_LONG_SIZE
#  define CHERI_MEM_OFFSET_METADATA 0
#else
#  define CHERI_MEM_OFFSET_CURSOR 0
#  define CHERI_MEM_OFFSET_METADATA TARGET_LONG_SIZE
#endif

/*
 * Optional suffix intended for Morello to differentiate its 128-bit
 * compression format from the CHERI-MIPS and CHERI-RISC-V CHERI-128 format,
 * allowing e.g. cc128 and cc128m to coexist. Unused for now.
 */
#define CHERI_CAP_VARIANT_LOWER
#define CHERI_CAP_VARIANT_UPPER

#define _CHERI_CAP_FORMAT(size, variant) size ## variant
#define _XCHERI_CAP_FORMAT(size, variant) _CHERI_CAP_FORMAT(size, variant)
#define CHERI_CAP_FORMAT(case) _XCHERI_CAP_FORMAT(CHERI_CAP_BITS, CHERI_CAP_VARIANT ## _ ## case)

#define _CAP_NAMESPACED(prefix, format, id) prefix ## format ## _ ## id
#define _XCAP_NAMESPACED(prefix, format, id) _CAP_NAMESPACED(prefix, format, id)
#define CAP_NAMESPACED(case, prefix, id) _XCAP_NAMESPACED(prefix, CHERI_CAP_FORMAT(case), id)

#define CAP_cc(id) CAP_NAMESPACED(LOWER, cc, id)
#define CAP_CC(id) CAP_NAMESPACED(UPPER, CC, id)

#define CAP_PERMS_ALL CAP_CC(PERMS_ALL)
#define CAP_UPERMS_ALL CAP_CC(UPERMS_ALL)
#define CAP_UPERMS_SHFT CAP_CC(UPERMS_SHFT)
#define CAP_MAX_UPERM CAP_CC(MAX_UPERM)
#define CAP_MAX_REPRESENTABLE_OTYPE CAP_CC(MAX_REPRESENTABLE_OTYPE)
#define CAP_LAST_NONRESERVED_OTYPE CAP_CC(LAST_NONRESERVED_OTYPE)
#define CAP_OTYPE_UNSEALED CAP_CC(OTYPE_UNSEALED)
#define CAP_OTYPE_UNSEALED_SIGNED CAP_CC(OTYPE_UNSEALED_SIGNED)
#define CAP_OTYPE_SENTRY CAP_CC(OTYPE_SENTRY)
#define CAP_FIRST_SPECIAL_OTYPE_SIGNED CAP_CC(FIRST_SPECIAL_OTYPE_SIGNED)
#define CAP_LAST_SPECIAL_OTYPE_SIGNED CAP_CC(LAST_SPECIAL_OTYPE_SIGNED)
#define CAP_FLAGS_ALL_BITS CAP_CC(FIELD_FLAGS_MASK_NOT_SHIFTED)

/* compressed capabilities use an (XLEN+1)-bit top */
#define CAP_MAX_LENGTH CAP_CC(MAX_LENGTH)
#define CAP_MAX_TOP CAP_CC(MAX_TOP)

#define CAP_NULL_PESBT CAP_CC(NULL_PESBT)
#define CAP_NULL_XOR_MASK CAP_CC(NULL_XOR_MASK)

#define CAP_VERSION_UNVERSIONED CAP_CC(VERSION_UNVERSIONED)
#define CAP_MAX_VERSION CAP_CC(MAX_VERSION)
/* size of memory version granule. In principle this is independent of CAP_SIZE
    but in practice it will probably be the same. */
#  define CHERI_VERSION_GRANULE_SIZE (CHERI_CAP_SIZE)
/* Integer type big enough to hold a memory granule version. The actually 
   supported number of versions might be smaller e.g. 4-bits */
typedef uint8_t cap_version_t;

typedef CAP_cc(cap_t) cap_register_t;
typedef CAP_cc(offset_t) cap_offset_t;
typedef CAP_cc(length_t) cap_length_t;

typedef enum CheriPermissions {
    CAP_PERM_GLOBAL = CAP_CC(PERM_GLOBAL),
    CAP_PERM_EXECUTE = CAP_CC(PERM_EXECUTE),
    CAP_PERM_LOAD = CAP_CC(PERM_LOAD),
    CAP_PERM_STORE = CAP_CC(PERM_STORE),
    CAP_PERM_LOAD_CAP = CAP_CC(PERM_LOAD_CAP),
    CAP_PERM_STORE_CAP = CAP_CC(PERM_STORE_CAP),
    CAP_PERM_STORE_LOCAL = CAP_CC(PERM_STORE_LOCAL),
    CAP_PERM_SEAL = CAP_CC(PERM_SEAL),
    CAP_PERM_CINVOKE = CAP_CC(PERM_CINVOKE),
    CAP_PERM_UNSEAL = CAP_CC(PERM_UNSEAL),
    CAP_ACCESS_SYS_REGS = CAP_CC(PERM_ACCESS_SYS_REGS),
    CAP_PERM_SETCID = CAP_CC(PERM_SETCID),
} CheriPermissions;

typedef enum CheriFlags {
    CHERI_FLAG_CAPMODE = (1 << 0),
} CheriFlags;

typedef enum CheriTbFlags {
    /* CHERI PCC is tagged, executable and unsealed */
    TB_FLAG_CHERI_PCC_EXECUTABLE = (1 << 0),
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
     * PCC spans the full address space and has base zero. This means we do
     * not need to perform bounds checks or subtract/add PCC.base
     */
    TB_FLAG_CHERI_PCC_FULL_AS = (1 << 7),
    TB_FLAG_CHERI_PCC_READABLE = (1 << 8),
    /* DDC is tagged, unsealed and unversioned */
    TB_FLAG_CHERI_DDC_UNVERSIONED = (1 << 9),

    /* Useful for CHERI-specific flags on various platforms if the normal flags
       overflowed */
    TB_FLAG_CHERI_SPARE_INDEX_START = 16,
} CheriTbFlags;

#endif // TARGET_CHERI
