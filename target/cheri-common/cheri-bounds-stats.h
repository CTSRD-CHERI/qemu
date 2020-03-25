/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2016-2020 Alex Richardson <Alexander.Richardson@cl.cam.ac.uk>
 * All rights reserved.
 *
 * This software was developed by SRI International and the University of
 * Cambridge Computer Laboratory under DARPA/AFRL contract FA8750-10-C-0237
 * ("CTSRD"), as part of the DARPA CRASH research programme.
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
#include "cheri_utils.h"
#include "qemu/qemu-print.h"

#if QEMU_USE_COMPRESSED_CHERI_CAPS

extern bool cheri_c2e_on_unrepresentable;
extern bool cheri_debugger_on_unrepresentable;

static inline void
_became_unrepresentable(CPUArchState *env, uint16_t reg, uintptr_t retpc)
{
    env->statcounters_unrepresentable_caps++;
#ifdef TARGET_MIPS
    if (cheri_debugger_on_unrepresentable)
        do_raise_exception(env, EXCP_DEBUG, retpc);
#elif defined(TARGET_RISCV)
    if (cheri_debugger_on_unrepresentable)
        riscv_raise_exception(env, EXCP_DEBUG, retpc);
#else
#error "Unknonwn CHERI target"
#endif
    if (cheri_c2e_on_unrepresentable)
        raise_cheri_exception_impl(env, CapEx_InexactBounds, reg, false, retpc);
}

#else
static inline void
_became_unrepresentable(CPUArchState *env, uint16_t reg, uintptr_t retpc)
{
    assert(false && "THIS SHOULD NOT BE CALLED");
}

#endif /* ! 128-bit capabilities */

#ifdef DO_CHERI_STATISTICS

struct bounds_bucket {
    uint64_t howmuch;
    const char* name;
};
#define NUM_BOUNDS_BUCKETS 13
extern struct bounds_bucket bounds_buckets[NUM_BOUNDS_BUCKETS];

struct oob_stats_info {
    const char* operation;
    uint64_t num_uses;
    uint64_t unrepresentable; // Number of OOB caps that were unrepresentable
    uint64_t after_bounds[ARRAY_SIZE(bounds_buckets) + 1]; // Number of OOB caps created pointing to after end
    uint64_t before_bounds[ARRAY_SIZE(bounds_buckets) + 1];  // Number of OOB caps created pointing to before start
};

#define DEFINE_CHERI_STAT(op) \
    struct oob_stats_info oob_info_##op = { \
        .operation = #op, \
    }
#define DECLARE_CHERI_STAT(op) extern struct oob_stats_info oob_info_##op;
#define OOB_INFO(op) (&oob_info_##op)

static inline int64_t _howmuch_out_of_bounds(CPUArchState *env,
                                             const cap_register_t *cr,
                                             const char *name, uintptr_t retpc)
{
    if (!cr->cr_tag)
        return 0;  // We don't care about arithmetic on untagged things

    const cap_offset_t offset = cap_get_offset(cr);
    const uint64_t addr = cap_get_cursor(cr);
    if (addr == cap_get_top65(cr)) {
        // This case is very common so we should not print a message here
        return 1;
    } else if (offset < 0 || offset > cap_get_length65(cr)) {
        // handle negative offsets:
        int64_t howmuch;
        if (offset < 0)
            howmuch = (int64_t)offset;
        else
            howmuch = offset - cap_get_length65(cr) + 1;
        qemu_log_mask(CPU_LOG_INSTR | CPU_LOG_CHERI_BOUNDS,
                      "BOUNDS: Out of bounds capability (by %" PRId64
                      ") created using %s: " PRINT_CAP_FMTSTR ", "
                      " pc=%016" PRIx64 " ASID=%u\n",
                      howmuch, name, PRINT_CAP_ARGS(cr),
                      cpu_get_current_pc(env, retpc), cheri_get_asid(env));
        return howmuch;
    }
    return 0;
}

static inline int out_of_bounds_stat_index(uint64_t howmuch) {

    for (int i = 0; i < ARRAY_SIZE(bounds_buckets); i++) {
        if (howmuch <= bounds_buckets[i].howmuch)
            return i;
    }
    return ARRAY_SIZE(bounds_buckets); // more than 64MB
}

static inline void check_out_of_bounds_stat(CPUArchState *env,
                                            struct oob_stats_info *info,
                                            const cap_register_t *capreg,
                                            uintptr_t retpc)
{
    int64_t howmuch =
        _howmuch_out_of_bounds(env, capreg, info->operation, retpc);
    if (howmuch > 0) {
        info->after_bounds[out_of_bounds_stat_index(howmuch)]++;
    } else if (howmuch < 0) {
        info->before_bounds[out_of_bounds_stat_index(llabs(howmuch))]++;
    }
}

static inline void became_unrepresentable(CPUArchState *env, uint16_t reg,
                                          struct oob_stats_info *info,
                                          uintptr_t retpc) {
    const cap_register_t *capreg = get_readonly_capreg(env, reg);
    /* unrepresentable implies more than one out of bounds: */
    check_out_of_bounds_stat(env, info, capreg, retpc);
    info->unrepresentable++;
    qemu_log_mask(
        CPU_LOG_INSTR | CPU_LOG_CHERI_BOUNDS,
        "BOUNDS: Unrepresentable capability created using %s, pc=%016" PRIx64
        " ASID=%u\n", info->operation, cheri_get_current_pc(env, retpc)),
        cheri_get_asid(env));
    _became_unrepresentable(env, reg, retpc);
}

static inline void dump_out_of_bounds_stats(FILE* f, const struct oob_stats_info *info)
{

    qemu_fprintf(f, "Number of %ss: %" PRIu64 "\n", info->operation, info->num_uses);
    uint64_t total_out_of_bounds = info->after_bounds[0];
    // one past the end is fine according to ISO C
    qemu_fprintf(f, "  One past the end:           %" PRIu64 "\n", info->after_bounds[0]);
    assert(bounds_buckets[0].howmuch == 1);
    // All the others are invalid:
    for (int i = 1; i < ARRAY_SIZE(bounds_buckets); i++) {
        qemu_fprintf(f, "  Out of bounds by up to %s: %" PRIu64 "\n", bounds_buckets[i].name, info->after_bounds[i]);
        total_out_of_bounds += info->after_bounds[i];
    }
    qemu_fprintf(f, "  Out of bounds by over  %s: %" PRIu64 "\n",
        bounds_buckets[ARRAY_SIZE(bounds_buckets) - 1].name, info->after_bounds[ARRAY_SIZE(bounds_buckets)]);
    total_out_of_bounds += info->after_bounds[ARRAY_SIZE(bounds_buckets)];


    // One before the start is invalid though:
    for (int i = 0; i < ARRAY_SIZE(bounds_buckets); i++) {
        qemu_fprintf(f, "  Before bounds by up to -%s: %" PRIu64 "\n", bounds_buckets[i].name, info->before_bounds[i]);
        total_out_of_bounds += info->before_bounds[i];
    }
    qemu_fprintf(f, "  Before bounds by over  -%s: %" PRIu64 "\n",
        bounds_buckets[ARRAY_SIZE(bounds_buckets) - 1].name, info->before_bounds[ARRAY_SIZE(bounds_buckets)]);
    total_out_of_bounds += info->before_bounds[ARRAY_SIZE(bounds_buckets)];


    // unrepresentable, i.e. massively out of bounds:
    qemu_fprintf(f, "  Became unrepresentable due to out-of-bounds: %" PRIu64 "\n", info->unrepresentable);
    total_out_of_bounds += info->unrepresentable; // TODO: count how far it was out of bounds for this stat

    qemu_fprintf(f, "Total out of bounds %ss: %" PRIu64 " (%f%%)\n", info->operation, total_out_of_bounds,
        info->num_uses == 0 ? 0.0 : ((double)(100 * total_out_of_bounds) / (double)info->num_uses));
    qemu_fprintf(f, "Total out of bounds %ss (excluding one past the end): %" PRIu64 " (%f%%)\n",
        info->operation, total_out_of_bounds - info->after_bounds[0],
        info->num_uses == 0 ? 0.0 : ((double)(100 * (total_out_of_bounds - info->after_bounds[0])) / (double)info->num_uses));
}

// Common cross-architecture stats:
DECLARE_CHERI_STAT(candaddr)
DECLARE_CHERI_STAT(cfromptr)
DECLARE_CHERI_STAT(cincoffset)
DECLARE_CHERI_STAT(csetaddr)
DECLARE_CHERI_STAT(csetoffset)

#else /* !defined(DO_CHERI_STATISTICS) */

// Don't collect any statistics by default (it slows down QEMU)
struct oob_stats_info;
#define OOB_INFO(op) NULL
#define check_out_of_bounds_stat(env, op, capreg, retpc)                       \
    do {                                                                       \
    } while (0)
#define became_unrepresentable(env, reg, operation, retpc) _became_unrepresentable(env, reg, retpc)

#endif /* DO_CHERI_STATISTICS */
