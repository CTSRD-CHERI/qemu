/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2018 Lawrence Esswood
 * Copyright (c) 2018 Alex Richardson
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

#include <cinttypes>
#include "../cheri_compressed_cap.h"
#include "sail_wrapper.h"
#include "FuzzedDataProvider.h"

static void dump_cap_fields(const cap_register_t& result) {
    fprintf(stderr, "Permissions: 0x%" PRIx32 "\n", result.cr_perms); // TODO: decode perms
    fprintf(stderr, "User Perms:  0x%" PRIx32 "\n", result.cr_uperms);
    fprintf(stderr, "Base:        0x%016" PRIx64 "\n", result.cr_base);
    fprintf(stderr, "Offset:      0x%016" PRIx64 "\n", (uint64_t)result.offset());
    fprintf(stderr, "Cursor:      0x%016" PRIx64 "\n", result.address());
    fprintf(stderr, "Length:      0x%" PRIx64 "%016" PRIx64 " %s\n", (uint64_t)(result.length() >> 64),
        (uint64_t)result.length() , result.length()  > UINT64_MAX ? " (greater than UINT64_MAX)" : "");
    cc128_length_t top_full = result.top();
    fprintf(stderr, "Top:         0x%" PRIx64 "%016" PRIx64 " %s\n", (uint64_t)(top_full >> 64), (uint64_t)top_full,
        top_full > UINT64_MAX ? " (greater than UINT64_MAX)" : "");
    fprintf(stderr, "Sealed:      %d\n", (int)cc128_is_cap_sealed(&result));
    fprintf(stderr, "OType:       0x%" PRIx32 "\n", result.cr_otype);
    fprintf(stderr, "\n");
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    if (size != 16) {
        return 0; // Need 128 bits of data
    }
    FuzzedDataProvider fuzzData(data, size);
    uint64_t pesbt = fuzzData.ConsumeIntegral<uint64_t>();
    uint64_t cursor = fuzzData.ConsumeIntegral<uint64_t>();
    cap_register_t result;
    cap_register_t sail_result;
    memset(&result, 0, sizeof(result));
    memset(&sail_result, 0, sizeof(sail_result));
    cc128_decompress_mem(pesbt, cursor, false, &result);
    sail_decode_128_mem(pesbt, cursor, false, &sail_result);
    if (memcmp(&result, &sail_result, sizeof(result)) != 0) {
        fprintf(stderr, "DECODE FROM MEM FAILED\n");
        fprintf(stderr, "PESBT  0x%016" PRIx64 "\n", pesbt);
        fprintf(stderr, "Cursor 0x%016" PRIx64 "\n", cursor);
        dump_cap_fields(result);
        dump_cap_fields(sail_result);
        abort();
    }

    memset(&result, 0, sizeof(result));
    memset(&sail_result, 0, sizeof(sail_result));
    cc128_decompress_raw(pesbt, cursor, false, &result);
    sail_decode_128_raw(pesbt, cursor, false, &sail_result);
    if (memcmp(&result, &sail_result, sizeof(result)) != 0) {
        fprintf(stderr, "DECODE ALREADY XORED FAILED\n");
        fprintf(stderr, "PESBT  0x%016" PRIx64 "\n", pesbt);
        fprintf(stderr, "Cursor 0x%016" PRIx64 "\n", cursor);
        dump_cap_fields(result);
        dump_cap_fields(sail_result);
        abort();
    }

    return 0;  // Non-zero return values are reserved for future use.
}
