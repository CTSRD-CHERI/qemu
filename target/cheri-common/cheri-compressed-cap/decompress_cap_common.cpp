/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2018 Alex Richardson
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
#include <cerrno>
#include <cinttypes>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <err.h>
#include <sysexits.h>

#include "cheri_compressed_cap.h"
using CCAPI = _CC_CONCAT(CompressedCap, CAP_FORMAT);
#ifdef DECOMPRESS_WITH_SAIL_GENERATED_CODE
#include "test/sail_wrapper.h"
using SailAPI = _CC_CONCAT(TestAPI, CAP_FORMAT);
#endif

static const char* otype_suffix(uint32_t otype) {
    // clang-format off
    switch (otype) {
#define CC_FORMAT_UPPER CAP_FORMAT_UPPER
#define OTYPE_CASE(Name, ...)                                                                                          \
    case _CC_N(Name): return " (" _CC_STRINGIFY(_CC_N(Name)) ")";
        _CC_N(LS_SPECIAL_OTYPES)(OTYPE_CASE, )
    default: return "";
#undef OTYPE_CASE
#undef CC_FORMAT_UPPER
    }
    // clang-format on
}

static void dump_cap_fields(const CCAPI::cap_t& result) {
    printf("Permissions: 0x%" PRIx32 "\n", result.permissions()); // TODO: decode perms
    printf("User Perms:  0x%" PRIx32 "\n", result.software_permissions());
    printf("Base:        0x%016" PRIx64 "\n", (uint64_t)result.base());
    printf("Offset:      0x%016" PRIx64 "\n", (int64_t)result.offset());
    printf("Cursor:      0x%016" PRIx64 "\n", (uint64_t)result.address());
    auto length = result.length();
    auto top_full = result.top();
    if constexpr (sizeof(length) > 8) {
        printf("Length:     0x%" PRIx64 "%016" PRIx64 " %s\n", (uint64_t)(length >> 64), (uint64_t)length,
               length > UINT64_MAX ? " (greater than UINT64_MAX)" : "");
        printf("Top:        0x%" PRIx64 "%016" PRIx64 " %s\n", (uint64_t)(top_full >> 64), (uint64_t)top_full,
               top_full > UINT64_MAX ? " (greater than UINT64_MAX)" : "");
    } else {
        printf("Length:     0x%09" PRIx64 " %s\n", (uint64_t)length,
               length > UINT32_MAX ? " (greater than UINT32_MAX)" : "");
        printf("Top:        0x%09" PRIx64 " %s\n", (uint64_t)top_full,
               top_full > UINT32_MAX ? " (greater than UINT32_MAX)" : "");
    }

    printf("Sealed:      %d\n", result.is_sealed() ? 1 : 0);
    auto otype = result.type();
    printf("OType:       0x%" PRIx32 "%s\n", otype, otype_suffix(otype));
    printf("Flags:       0x%" PRIx8 "\n", result.flags());
    printf("Reserved:    0x%" PRIx8 "\n", result.reserved_bits());
    printf("Valid decompress: %s", result.cr_bounds_valid ? "yes" : "no");
    printf("\n");
}

int main(int argc, char** argv) {
    if (argc < 3) {
        printf("Usage: %s PESBT CURSOR\n", argv[0]);
        return EXIT_FAILURE;
    }
    errno = 0;
    char* end;
    uint64_t pesbt = strtoull(argv[1], &end, 16);
    if (errno != 0 || !end || *end != '\0' || (uint64_t)(CCAPI::addr_t)pesbt != pesbt) {
        err(EX_DATAERR, "pesbt not a valid %zd-byte hex number: %s", sizeof(CCAPI::addr_t), argv[1]);
    }
    uint64_t cursor = strtoull(argv[2], &end, 16);
    if (errno != 0 || !end || *end != '\0' || (uint64_t)(CCAPI::addr_t)cursor != cursor) {
        err(EX_DATAERR, "cursor not a valid %zd-byte hex number: %s", sizeof(CCAPI::addr_t), argv[2]);
    }

    printf("Decompressing pesbt = %016" PRIx64 ", cursor = %016" PRIx64 "\n", pesbt, cursor);
#ifdef DECOMPRESS_WITH_SAIL_GENERATED_CODE
    CCAPI::cap_t result = SailAPI::decompress_mem(pesbt, cursor, false);
#else
    CCAPI::cap_t result = CCAPI::decompress_mem(pesbt, cursor, false);
#endif
    dump_cap_fields(result);
#ifdef DECOMPRESS_WITH_SAIL_GENERATED_CODE
    CCAPI::addr_t rt_pesbt = SailAPI::compress_mem(result);
#else
    CCAPI::addr_t rt_pesbt = CCAPI::compress_mem(result);
#endif
    printf("Re-compressed pesbt = %016" PRIx64 "%s\n", (uint64_t)rt_pesbt,
           pesbt == rt_pesbt ? "" : " - WAS DESTRUCTIVE");
    return EXIT_SUCCESS;
}
