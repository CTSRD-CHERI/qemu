/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2020 Alex Richardson
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
#include "contrib/sail.h"
#include "contrib/sail_failure.h"
#include "sail_wrapper.h"

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wsometimes-uninitialized"
#pragma clang diagnostic ignored "-Wstrict-prototypes"
#ifdef HAVE_UNUSED_BUT_SET_VARIABLE
#pragma clang diagnostic ignored "-Wunused-but-set-variable"
#endif
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#ifdef HAVE_UNUSED_BUT_SET_VARIABLE
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif
#endif
#include SAIL_COMPRESSION_GENERATED_C_FILE
#ifdef __clang__
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#define CC_FORMAT_LOWER SAIL_WRAPPER_CC_FORMAT_LOWER
#define CC_FORMAT_UPPER SAIL_WRAPPER_CC_FORMAT_UPPER

__attribute__((constructor, used)) static void sail_startup(void) { model_init(); }
__attribute__((destructor, used)) static void sail_cleanup(void) { model_fini(); }

int process_arguments(int argc, char** argv) { abort(); }

void setup_rts(void) {
    // Avoid double initialization of sail.c data
    static int done_setup = false;
    if (!done_setup) {
        setup_library();
        done_setup = true;
    }
}

void cleanup_rts(void) {
    // Avoid double free of sail.c data
    static int done_cleanup = false;
    if (!done_cleanup) {
        cleanup_library();
        done_cleanup = true;
    }
}

static inline uint64_t extract_bits(lbits op, uint64_t start, uint64_t len) {
    sail_int start_sail;
    CREATE_OF(sail_int, mach_int)(&start_sail, start);
    sail_int len_sail;
    CREATE_OF(sail_int, mach_int)(&len_sail, len);
    lbits slice_bits;
    CREATE(lbits)(&slice_bits);
    slice(&slice_bits, op, start_sail, len_sail);
    KILL(sail_int)(&start_sail);
    KILL(sail_int)(&len_sail);
    uint64_t result = CONVERT_OF(fbits, lbits)(slice_bits, true);
    KILL(lbits)(&slice_bits);
    return result;
}
/* CHERI-64 uses uint64_t, CHERI-128 uses lbits */
#if _CC_N(CAP_BITS) == 64
typedef uint64_t sail_cap_bits;
static void CREATE(sail_cap_bits)(sail_cap_bits* bits) { *bits = 0; }
static void KILL(sail_cap_bits)(sail_cap_bits* bits) { *bits = UINT64_MAX; }
static void pesbt_and_addr_to_sail_cap_bits(sail_cap_bits* out, uint32_t pesbt, uint32_t cursor) {
    *out = (sail_cap_bits)pesbt << 32 | (sail_cap_bits)cursor;
}
static uint64_t cc64_getbits(uint64_t bits, uint32_t start, uint32_t size);
static uint64_t extract_sail_cap_bits(sail_cap_bits* bits, uint64_t start, uint64_t len) {
    return cc64_getbits(*bits, start, len);
}
#elif _CC_N(CAP_BITS) == 128
typedef lbits sail_cap_bits;
#define SAIL_COMPRESSION_INDIRECT_BITS
static void create_sail_cap_bits(sail_cap_bits* bits) { create_lbits(bits); }
static void kill_sail_cap_bits(sail_cap_bits* bits) { kill_lbits(bits); }
static void pesbt_and_addr_to_sail_cap_bits(sail_cap_bits* out, uint64_t pesbt, uint64_t cursor) {
    lbits sail_pesbt;
    lbits sail_cursor;
    CREATE_OF(lbits, fbits)(&sail_pesbt, pesbt, 64, true);
    CREATE_OF(lbits, fbits)(&sail_cursor, cursor, 64, true);
    CREATE(lbits)(out);
    append(out, sail_pesbt, sail_cursor);
    KILL(lbits)(&sail_pesbt);
    KILL(lbits)(&sail_cursor);
}
static inline uint64_t extract_sail_cap_bits(sail_cap_bits* bits, uint64_t start, uint64_t len) {
    return extract_bits(*bits, start, len);
}
#else
#error Invalid capability size
#endif

static void cc_length_t_to_sail_cap_bits(sail_cap_bits* out, _cc_length_t len) {
#if _CC_LEN_WIDTH > 64
    lbits len_high;
    lbits len_low;
    CREATE_OF(lbits, fbits)(&len_high, len >> 64, 1, true);
    CREATE_OF(lbits, fbits)(&len_low, (uint64_t)len, 64, true);
    append(out, len_high, len_low);
    KILL(lbits)(&len_high);
    KILL(lbits)(&len_low);
#else
    *out = len;
#endif
}

#ifndef SAIL_WRAPPER_CC_IS_MORELLO

static inline void sail_dump_cap(const char* msg, struct zCapability cap) {
    sail_string zcap_str;
    CREATE(sail_string)(&zcap_str);
    sailgen_capToString(&zcap_str, cap);
    fprintf(stderr, "%s: %s\n", msg, zcap_str);
    KILL(sail_string)(&zcap_str);
}

static inline void set_top_base_from_sail(const struct zCapability* sail, _cc_cap_t* c);
static inline uint64_t extract_sail_cap_bits(sail_cap_bits* bits, uint64_t start, uint64_t len);
static struct zCapability cap_t_to_sail_cap(const _cc_cap_t* c);
static _cc_cap_t sail_cap_to_cap_t(const struct zCapability* sail);
static _cc_addr_t _compress_sailcap_raw(struct zCapability sailcap);

#ifdef SAIL_WRAPPER_CC_IS_V9
#include "sail_wrapper_common_v9.c"
#endif

bool _cc_sail(precise_is_representable)(const _cc_cap_t* cap, _cc_addr_t new_addr) {
    struct zCapability sailcap = cap_t_to_sail_cap(cap);
    return sailgen_setCapAddr(sailcap, new_addr).ztup0;
}

_cc_addr_t _cc_sail(null_pesbt)(void) { return _compress_sailcap_raw(znull_cap); }

_cc_addr_t _cc_sail(reset_pesbt)(void) { return _compress_sailcap_raw(SAIL_INFINITE_CAP); }

bool _cc_sail(setbounds)(_cc_cap_t* cap, _cc_length_t req_len) {
    struct zCapability sailcap = cap_t_to_sail_cap(cap);
    sail_cap_bits sail_req_len;
    CREATE(sail_cap_bits)(&sail_req_len);
    cc_length_t_to_sail_cap_bits(&sail_req_len, req_len);
    __typeof__(sailgen_doCSetBounds(sailcap, sail_req_len)) result = sailgen_doCSetBounds(sailcap, sail_req_len);
    KILL(sail_cap_bits)(&sail_req_len);
    bool exact = result.ztup0;
    *cap = sail_cap_to_cap_t(&result.ztup1);
    return exact;
}

_cc_cap_t _cc_sail(reset_capability)(void) { return sail_cap_to_cap_t(&SAIL_INFINITE_CAP); }

#endif // SAIL_WRAPPER_CC_IS_MORELLO
