/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2015-2016 Stacey Son <sson@FreeBSD.org>
 * Copyright (c) 2016-2018 Alfredo Mazzinghi <am2419@cl.cam.ac.uk>
 * Copyright (c) 2016-2018 Alex Richardson <Alexander.Richardson@cl.cam.ac.uk>
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
#include "qemu/osdep.h"
#include "qemu/main-loop.h"
#include "cpu.h"
#include "internal.h"
#include "qemu/host-utils.h"
#include "qemu/error-report.h"
#include "exec/exec-all.h"
#include "exec/cpu_ldst.h"
#include "exec/helper-proto.h"

#ifndef TARGET_CHERI
#error "This file should only be compiled for CHERI"
#endif

#include "disas/disas.h"
#include "disas/bfd.h"
/* Don't define the functions for CHERI256 (but we need CAP_MAX_OTYPE) */
#ifndef CHERI_128
#define CHERI_COMPRESSED_CONSTANTS_ONLY
#endif
/* Don't let cheri_compressed_cap define cap_register_t */
#define HAVE_CAP_REGISTER_T
#include "cheri-compressed-cap/cheri_compressed_cap.h"


const char *cp2_fault_causestr[] = {
    "None",
    "Length Violation",
    "Tag Violation",
    "Seal Violation",
    "Type Violation",
    "Call Trap",
    "Return Trap",
    "Underflow of Trusted System Stack",
    "User-defined Permission Violation",
    "TLB prohibits Store Capability",
    "Bounds Cannot Be Represented Exactly",
    "Reserved 0x0b",
    "Reserved 0x0c",
    "Reserved 0x0d",
    "Reserved 0x0e",
    "Reserved 0x0f",
    "Global Violation",
    "Permit_Execute Violation",
    "Permit_Load Violation",
    "Permit_Store Violation",
    "Permit_Load_Capability Violation",
    "Permit_Store_Capability Violation",
    "Permit_Store_Local_Capability Violation",
    "Permit_Seal Violation",
    "Access_Sys_Reg Violation",
    "Permit_CCall Violation",
    "Access_EPCC Violation",
    "Access_KDC Violation",
    "Access_KCC Violation",
    "Access_KR1C Violation",
    "Access_KR2C Violation"
};

#ifdef DO_CHERI_STATISTICS

struct bounds_bucket {
    uint64_t howmuch;
    const char* name;
};
struct bounds_bucket bounds_buckets[] = {
    {1, "1  "}, // 1
    {2, "2  "}, // 2
    {4, "4  "}, // 3
    {8, "8  "}, // 4
    {16, "16 "},
    {32, "32 "},
    {64, "64 "},
    {256, "256"},
    {1024, "1K "},
    {4096, "4K "},
    {64 * 1024, "64K"},
    {1024 * 1024, "1M "},
    {64 * 1024 * 1024, "64M"},
};

#define DEFINE_CHERI_STAT(op) \
    static uint64_t stat_num_##op = 0; \
    static uint64_t stat_num_##op##_after_bounds[ARRAY_SIZE(bounds_buckets) + 1]; \
    static uint64_t stat_num_##op##_before_bounds[ARRAY_SIZE(bounds_buckets) + 1]; \
    static uint64_t stat_num_##op##_out_of_bounds_unrep = 0;

DEFINE_CHERI_STAT(cincoffset)
DEFINE_CHERI_STAT(csetoffset)
DEFINE_CHERI_STAT(cgetpccsetoffset)
DEFINE_CHERI_STAT(cfromptr)

static inline int64_t _howmuch_out_of_bounds(CPUMIPSState *env, cap_register_t* cr, const char* name)
{
    if (!cr->cr_tag)
        return 0;  // We don't care about arithmetic on untagged things

    // FIXME: unsigned cr_offset is quite annoying, we should use cr_cursor
    if (cr->cr_offset == cr->cr_length) {
        // This case is very common so we should not print a message here
        return 1;
    } else if (cr->cr_offset > cr->cr_length) {
        // handle negative offsets:
        int64_t howmuch;
        if ((int64_t)cr->cr_offset < (int64_t)cr->cr_length)
            howmuch = (int64_t)cr->cr_offset;
        else
            howmuch = cr->cr_offset - cr->cr_length + 1;
        qemu_log_mask(CPU_LOG_INSTR | CPU_LOG_CHERI_BOUNDS,
                      "BOUNDS: Out of bounds capability (by %" PRId64 ") created using %s: v:%d s:%d"
                      " p:%08x b:%016" PRIx64 " l:%" PRId64 " o: %" PRId64 " pc=%016" PRIx64 " ASID=%u\n",
                      howmuch, name, cr->cr_tag, cap_is_sealed(cr),
                      (((cr->cr_uperms & CAP_UPERMS_ALL) << CAP_UPERMS_MEM_SHFT) | (cr->cr_perms & CAP_PERMS_ALL)),
                      cr->cr_base, cr->cr_length, (int64_t)cr->cr_offset,
                      cap_get_cursor(&env->active_tc.PCC),
                      (unsigned)(env->CP0_EntryHi & 0xFF));
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

#define check_out_of_bounds_stat(env, op, capreg) do { \
    int64_t howmuch = _howmuch_out_of_bounds(env, capreg, #op); \
    if (howmuch > 0) { \
        stat_num_##op##_after_bounds[out_of_bounds_stat_index(howmuch)]++; \
    } else if (howmuch < 0) { \
        stat_num_##op##_before_bounds[out_of_bounds_stat_index(llabs(howmuch))]++; \
    } \
} while (0)

// TODO: count how far it was out of bounds for this stat
#define became_unrepresentable(env, reg, operation) do { \
    /* unrepresentable implies more than one out of bounds: */ \
    stat_num_##operation##_out_of_bounds_unrep++; \
    qemu_log_mask(CPU_LOG_INSTR | CPU_LOG_CHERI_BOUNDS, \
         "BOUNDS: Unrepresentable capability created using %s, pc=%016" PRIx64 " ASID=%u\n", \
        #operation, cap_get_cursor(&env->active_tc.PCC), (unsigned)(env->CP0_EntryHi & 0xFF)); \
    _became_unrepresentable(env, reg); \
} while (0)

static void dump_out_of_bounds_stats(FILE* f, fprintf_function cpu_fprintf,
                                     const char* name, uint64_t total,
                                     uint64_t* after_bounds,
                                     uint64_t* before_bounds,
                                     uint64_t unrepresentable)
{

    cpu_fprintf(f, "Number of %ss: %" PRIu64 "\n", name, total);
    uint64_t total_out_of_bounds = after_bounds[0];
    // one past the end is fine according to ISO C
    cpu_fprintf(f, "  One past the end:           %" PRIu64 "\n", after_bounds[0]);
    assert(bounds_buckets[0].howmuch == 1);
    // All the others are invalid:
    for (int i = 1; i < ARRAY_SIZE(bounds_buckets); i++) {
        cpu_fprintf(f, "  Out of bounds by up to %s: %" PRIu64 "\n", bounds_buckets[i].name, after_bounds[i]);
        total_out_of_bounds += after_bounds[i];
    }
    cpu_fprintf(f, "  Out of bounds by over  %s: %" PRIu64 "\n",
        bounds_buckets[ARRAY_SIZE(bounds_buckets) - 1].name, after_bounds[ARRAY_SIZE(bounds_buckets)]);
    total_out_of_bounds += after_bounds[ARRAY_SIZE(bounds_buckets)];


    // One before the start is invalid though:
    for (int i = 0; i < ARRAY_SIZE(bounds_buckets); i++) {
        cpu_fprintf(f, "  Before bounds by up to -%s: %" PRIu64 "\n", bounds_buckets[i].name, before_bounds[i]);
        total_out_of_bounds += before_bounds[i];
    }
    cpu_fprintf(f, "  Before bounds by over  -%s: %" PRIu64 "\n",
        bounds_buckets[ARRAY_SIZE(bounds_buckets) - 1].name, before_bounds[ARRAY_SIZE(bounds_buckets)]);
    total_out_of_bounds += before_bounds[ARRAY_SIZE(bounds_buckets)];


    // unrepresentable, i.e. massively out of bounds:
    cpu_fprintf(f, "  Became unrepresentable due to out-of-bounds: %" PRIu64 "\n", unrepresentable);
    total_out_of_bounds += unrepresentable; // TODO: count how far it was out of bounds for this stat

    cpu_fprintf(f, "Total out of bounds %ss: %" PRIu64 " (%f%%)\n", name, total_out_of_bounds,
                total == 0 ? 0.0 : ((double)(100 * total_out_of_bounds) / (double)total));
    cpu_fprintf(f, "Total out of bounds %ss (excluding one past the end): %" PRIu64 " (%f%%)\n",
                name, total_out_of_bounds - after_bounds[0],
                total == 0 ? 0.0 : ((double)(100 * (total_out_of_bounds - after_bounds[0])) / (double)total));
}

#else /* !defined(DO_CHERI_STATISTICS) */

// Don't collect any statistics by default (it slows down QEMU)
#define check_out_of_bounds_stat(env, op, capreg) do { } while (0)
#define became_unrepresentable(env, reg, operation) _became_unrepresentable(env, reg)

#endif /* DO_CHERI_STATISTICS */

void cheri_cpu_dump_statistics(CPUState *cs, FILE*f,
                              fprintf_function cpu_fprintf, int flags)
{
#ifndef DO_CHERI_STATISTICS
    cpu_fprintf(f, "CPUSTATS DISABLED, RECOMPILE WITH -DDO_CHERI_STATISTICS\n");
#else
#define DUMP_CHERI_STAT(name, printname) \
    dump_out_of_bounds_stats(f, cpu_fprintf, printname, stat_num_##name, \
        stat_num_##name##_after_bounds, stat_num_##name##_before_bounds, \
        stat_num_##name##_out_of_bounds_unrep);

    DUMP_CHERI_STAT(cincoffset, "CIncOffset");
    DUMP_CHERI_STAT(csetoffset, "CSetOffset");
    DUMP_CHERI_STAT(cgetpccsetoffset, "CGetPCCSetOffset");
    DUMP_CHERI_STAT(cfromptr, "CFromPtr");
#undef DUMP_CHERI_STAT
#endif
}

static inline bool
is_cap_sealed(const cap_register_t *cp)
{
    return cap_is_sealed(cp);
}

/*
 * Convert 64-bit integer into a capability that holds the integer in
 * its offset field.
 *
 *       cap.base = 0, cap.tag = false, cap.offset = x
 *
 * The contents of other fields of int to cap depends on the capability
 * compression scheme in use (e.g. 256-bit capabilities or 128-bit
 * compressed capabilities). In particular, with 128-bit compressed
 * capabilities, length is not always zero. The length of a capability
 * created via int to cap is not semantically meaningful, and programs
 * should not rely on it having any particular value.
 */
static inline const cap_register_t*
int_to_cap(uint64_t x, cap_register_t *cr)
{

    (void)null_capability(cr);
    cr->cr_offset = x;
    return cr;
}

void print_capreg(FILE* f, const cap_register_t *cr, const char* prefix, const char* name) {
    fprintf(f, "%s%s|" PRINT_CAP_FMTSTR_L1 "\n",
            prefix, name, PRINT_CAP_ARGS_L1(cr));
    fprintf(qemu_logfile, "             |" PRINT_CAP_FMTSTR_L2 "\n", PRINT_CAP_ARGS_L2(cr));
}

#ifdef CHERI_MAGIC128

bool
is_representable(bool sealed, uint64_t base, uint64_t length, uint64_t offset,
        uint64_t inc)
{
    return true;
}

static inline void
_became_unrepresentable(CPUMIPSState *env, uint16_t reg)
{
}
#elif defined(CHERI_128)

bool
is_representable(bool sealed, uint64_t base, uint64_t length, uint64_t offset,
        uint64_t new_offset)
{
    return cc128_is_representable(sealed, base, length, offset, new_offset);
}

extern bool cheri_c2e_on_unrepresentable;
extern bool cheri_debugger_on_unrepresentable;

static inline void
_became_unrepresentable(CPUMIPSState *env, uint16_t reg)
{
    if (cheri_debugger_on_unrepresentable)
        helper_raise_exception_debug(env);

	if (cheri_c2e_on_unrepresentable)
		do_raise_c2_exception(env, CP2Ca_INEXACT, reg);
}

#else

bool
is_representable(bool sealed, uint64_t base, uint64_t length, uint64_t offset,
        uint64_t inc)
{
    return true;
}

static inline void
_became_unrepresentable(CPUMIPSState *env, uint16_t reg)
{
}
#endif /* ! 128-bit capabilities */

#ifdef CONFIG_MIPS_LOG_INSTR
void dump_store(CPUMIPSState *env, int, target_ulong, target_ulong);
#endif

static inline int align_of(int size, uint64_t addr)
{

    switch(size) {
    case 1:
        return 0;
    case 2:
        return (addr & 0x1);
    case 4:
        return (addr & 0x3);
    case 8:
        return (addr & 0x7);
    case 16:
        return (addr & 0xf);
    case 32:
        return (addr & 0x1f);
    case 64:
        return (addr & 0x3f);
    case 128:
        return (addr & 0x7f);
    default:
        return 1;
    }
}

static inline void check_cap(CPUMIPSState *env, const cap_register_t *cr,
        uint32_t perm, uint64_t addr, uint16_t regnum, uint32_t len, bool instavail)
{
    uint16_t cause;
    /*
     * See section 5.6 in CHERI Architecture.
     *
     * Capability checks (in order of priority):
     * (1) <ctag> must be set (CP2Ca_TAG Violation).
     * (2) Seal bit must be unset (CP2Ca_SEAL Violation).
     * (3) <perm> permission must be set (CP2Ca_PERM_EXE, CP2Ca_PERM_LD,
     * or CP2Ca_PERM_ST Violation).
     * (4) <addr> must be within bounds (CP2Ca_LENGTH Violation).
     */
    if (!cr->cr_tag) {
        cause = CP2Ca_TAG;
        // fprintf(qemu_logfile, "CAP Tag VIOLATION: ");
        goto do_exception;
    }
    if (is_cap_sealed(cr)) {
        cause = CP2Ca_SEAL;
        // fprintf(qemu_logfile, "CAP Seal VIOLATION: ");
        goto do_exception;
    }
    if ((cr->cr_perms & perm) != perm) {
        switch (perm) {
            case CAP_PERM_EXECUTE:
                cause = CP2Ca_PERM_EXE;
                // fprintf(qemu_logfile, "CAP Exe VIOLATION: ");
                goto do_exception;
            case CAP_PERM_LOAD:
                cause = CP2Ca_PERM_LD;
                // fprintf(qemu_logfile, "CAP LD VIOLATION: ");
                goto do_exception;
            case CAP_PERM_STORE:
                cause = CP2Ca_PERM_ST;
                // fprintf(qemu_logfile, "CAP ST VIOLATION: ");
                goto do_exception;
            default:
                break;
        }
    }
    // fprintf(stderr, "addr=%zx, len=%zd, cr_base=%zx, cr_len=%zd\n",
    //     (size_t)addr, (size_t)len, (size_t)cr->cr_base, (size_t)cr->cr_length);
    if (addr < cr->cr_base || (addr + len) > cap_get_top(cr)) {
        cause = CP2Ca_LENGTH;
        // fprintf(qemu_logfile, "CAP Len VIOLATION: ");
        goto do_exception;
    }

    return;

do_exception:
    env->CP0_BadVAddr = addr;
    if (!instavail)
        env->error_code |= EXCP_INST_NOTAVAIL;
    do_raise_c2_exception(env, cause, regnum);
}

static inline target_ulong
clear_tag_if_no_loadcap(CPUMIPSState *env, target_ulong tag, const cap_register_t* cbp) {
    if (tag && (env->TLB_L || !(cbp->cr_perms & CAP_PERM_LOAD_CAP))) {
        if (qemu_loglevel_mask(CPU_LOG_INSTR)) {
            qemu_log("Clearing tag bit due to missing %s",
                     env->TLB_L ? "TLB_L" : "CAP_PERM_LOAD_CAP");
        }
        return 0;
    }
    return tag;
}

void helper_candperm(CPUMIPSState *env, uint32_t cd, uint32_t cb,
        target_ulong rt)
{
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    /*
     * CAndPerm: Restrict Permissions
     */
    if (!cbp->cr_tag) {
        do_raise_c2_exception(env, CP2Ca_TAG, cb);
    } else if (is_cap_sealed(cbp)) {
        do_raise_c2_exception(env, CP2Ca_SEAL, cb);
    } else {
        uint32_t rt_perms = (uint32_t)rt & (CAP_PERMS_ALL);
        uint32_t rt_uperms = ((uint32_t)rt >> CAP_UPERMS_SHFT) &
            CAP_UPERMS_ALL;

        cap_register_t result = *cbp;
        result.cr_perms = cbp->cr_perms & rt_perms;
        result.cr_uperms = cbp->cr_uperms & rt_uperms;
        update_capreg(&env->active_tc, cd, &result);
    }
}

target_ulong helper_cbez(CPUMIPSState *env, uint32_t cb, uint32_t offset)
{
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    /*
     * CBEZ: Branch if NULL
     */
    /*
     * Compare the only semantically meaningful fields of int_to_cap(0)
     */
    if (cbp->cr_base == 0 && cbp->cr_tag == 0 && cbp->cr_offset == 0)
        return (target_ulong)1;
    else
        return (target_ulong)0;
}

target_ulong helper_cbnz(CPUMIPSState *env, uint32_t cb, uint32_t offset)
{
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    /*
     * CBEZ: Branch if not NULL
     */
    /*
     * Compare the only semantically meaningful fields of int_to_cap(0)
     */
    if (cbp->cr_base == 0 && cbp->cr_tag == 0 && cbp->cr_offset == 0)
        return (target_ulong)0;
    else
        return (target_ulong)1;
}

target_ulong helper_cbts(CPUMIPSState *env, uint32_t cb, uint32_t offset)
{
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    /*
     * CBTS: Branch if tag is set
     */
    return (target_ulong)cbp->cr_tag;
}

target_ulong helper_cbtu(CPUMIPSState *env, uint32_t cb, uint32_t offset)
{
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    /*
     * CBTU: Branch if tag is unset
     */
    return (target_ulong)!cbp->cr_tag;
}

static target_ulong ccall_common(CPUMIPSState *env, uint32_t cs, uint32_t cb, uint32_t selector)
{
    const cap_register_t *csp = get_readonly_capreg(&env->active_tc, cs);
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    /*
     * CCall: Call into a new security domain
     */
    if (!csp->cr_tag) {
        do_raise_c2_exception(env, CP2Ca_TAG, cs);
    } else if (!cbp->cr_tag) {
        do_raise_c2_exception(env, CP2Ca_TAG, cb);
    } else if (!is_cap_sealed(csp)) {
        do_raise_c2_exception(env, CP2Ca_SEAL, cs);
    } else if (!is_cap_sealed(cbp)) {
        do_raise_c2_exception(env, CP2Ca_SEAL, cb);
    } else if (csp->cr_otype != cbp->cr_otype) {
        do_raise_c2_exception(env, CP2Ca_TYPE, cs);
    } else if (!(csp->cr_perms & CAP_PERM_EXECUTE)) {
        do_raise_c2_exception(env, CP2Ca_PERM_EXE, cs);
    } else if (cbp->cr_perms & CAP_PERM_EXECUTE) {
        do_raise_c2_exception(env, CP2Ca_PERM_EXE, cb);
    } else if (csp->cr_offset >= csp->cr_length) {
        do_raise_c2_exception(env, CP2Ca_LENGTH, cs);
    } else {
        if (selector == CCALL_SELECTOR_0) {
            do_raise_c2_exception(env, CP2Ca_CALL, cs);
        } else if (!(csp->cr_perms & CAP_PERM_CCALL)){
            do_raise_c2_exception(env, CP2Ca_PERM_CCALL, cs);
        } else if (!(cbp->cr_perms & CAP_PERM_CCALL)){
            do_raise_c2_exception(env, CP2Ca_PERM_CCALL, cb);
        } else {
            cap_register_t idc = *cbp;
            idc.cr_sealed = 0;
            idc.cr_otype = 0;
            update_capreg(&env->active_tc, CP2CAP_IDC, &idc);
            // The capability register is loaded into PCC during delay slot
            env->active_tc.CapBranchTarget = *csp;
            // XXXAR: clearing these fields is not strictly needed since they
            // aren't copied from the CapBranchTarget to $pcc but it does make
            // the LOG_INSTR output less confusing.
            env->active_tc.CapBranchTarget.cr_sealed = 0;
            env->active_tc.CapBranchTarget.cr_otype = 0;
            // Return the branch target address
            return cap_get_cursor(csp);
        }
    }
    return (target_ulong)0;
}

void helper_ccall(CPUMIPSState *env, uint32_t cs, uint32_t cb)
{
    (void)ccall_common(env, cs, cb, CCALL_SELECTOR_0);
}

target_ulong helper_ccall_notrap(CPUMIPSState *env, uint32_t cs, uint32_t cb)
{
    return ccall_common(env, cs, cb, CCALL_SELECTOR_1);
}

void helper_cclearreg(CPUMIPSState *env, uint32_t mask)
{
    // Register zero means $ddc here since it is useful to clear $ddc on a
    // sandbox switch whereas clearing $NULL is useless
    if (mask & 0x1)
        (void)null_capability(&env->active_tc.CHWR.DDC);

    for (int creg = 1; creg < 32; creg++) {
        if (mask & (0x1 << creg))
            (void)null_capability(&env->active_tc._CGPR[creg]);
    }
}

void helper_creturn(CPUMIPSState *env)
{
    do_raise_c2_exception_noreg(env, CP2Ca_RETURN);
}

void helper_ccheckperm(CPUMIPSState *env, uint32_t cs, target_ulong rt)
{
    const cap_register_t *csp = get_readonly_capreg(&env->active_tc, cs);
    uint32_t rt_perms = (uint32_t)rt & (CAP_PERMS_ALL);
    uint32_t rt_uperms = ((uint32_t)rt >> CAP_UPERMS_SHFT) & CAP_UPERMS_ALL;
    /*
     * CCheckPerm: Raise exception if don't have permission
     */
    if (!csp->cr_tag) {
        do_raise_c2_exception(env, CP2Ca_TAG, cs);
    } else if ((csp->cr_perms & rt_perms) != rt_perms) {
        do_raise_c2_exception(env, CP2Ca_USRDEFINE, cs);
    } else if ((csp->cr_uperms & rt_uperms) != rt_uperms) {
        do_raise_c2_exception(env, CP2Ca_USRDEFINE, cs);
    } else if ((rt >> (16 + CAP_MAX_UPERM)) != 0UL) {
        do_raise_c2_exception(env, CP2Ca_USRDEFINE, cs);
    }
}

void helper_cchecktype(CPUMIPSState *env, uint32_t cs, uint32_t cb)
{
    const cap_register_t *csp = get_readonly_capreg(&env->active_tc, cs);
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    /*
     * CCheckType: Raise exception if otypes don't match
     */
    if (!csp->cr_tag) {
        do_raise_c2_exception(env, CP2Ca_TAG, cs);
    } else if (!cbp->cr_tag) {
        do_raise_c2_exception(env, CP2Ca_TAG, cb);
    } else if (!is_cap_sealed(csp)) {
        do_raise_c2_exception(env, CP2Ca_SEAL, cs);
    } else if (!is_cap_sealed(cbp)) {
        do_raise_c2_exception(env, CP2Ca_SEAL, cb);
    } else if (csp->cr_otype != cbp->cr_otype) {
        do_raise_c2_exception(env, CP2Ca_TYPE, cs);
    }
}

void helper_ccleartag(CPUMIPSState *env, uint32_t cd, uint32_t cb)
{
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    /*
     * CClearTag: Clear the tag bit
     */
    cap_register_t result = *cbp;
    result.cr_tag = 0;
#ifdef CHERI_128
    /* Save the compressed bits at the moment the tag was invalidated. */
    result.cr_pesbt = compress_128cap(&result);
#endif /* CHERI_128 */
    update_capreg(&env->active_tc, cd, &result);
}

void helper_cfromptr(CPUMIPSState *env, uint32_t cd, uint32_t cb,
        target_ulong rt)
{
#ifdef DO_CHERI_STATISTICS
    stat_num_cfromptr++;
#endif
    // CFromPtr traps on cbp == NULL so we use reg0 as $ddc to save encoding
    // space (and for backwards compat with old binaries).
    // Note: This is also still required for new binaries since clang assumes it
    // can use zero as $ddc in cfromptr/ctoptr
    const cap_register_t *cbp = get_capreg_0_is_ddc(&env->active_tc, cb);
    /*
     * CFromPtr: Create capability from pointer
     */
    if (rt == (target_ulong)0) {
        cap_register_t result;
        update_capreg(&env->active_tc, cd, null_capability(&result));
    } else if (!cbp->cr_tag) {
        do_raise_c2_exception(env, CP2Ca_TAG, cb);
    } else if (is_cap_sealed(cbp)) {
        do_raise_c2_exception(env, CP2Ca_SEAL, cb);
    } else {
        if (!is_representable(is_cap_sealed(cbp), cbp->cr_base,
                    cbp->cr_length, cbp->cr_offset, rt)) {
            became_unrepresentable(env, cd, cfromptr);
            cap_register_t result;
            update_capreg(&env->active_tc, cd,
                int_to_cap(cbp->cr_base + rt, &result));
        } else {
            cap_register_t result = *cbp;
            result.cr_offset = rt;
            update_capreg(&env->active_tc, cd, &result);
            check_out_of_bounds_stat(env, cfromptr, &result);
        }
    }
}

target_ulong helper_cgetaddr(CPUMIPSState *env, uint32_t cb)
{
    /*
     * CGetAddr: Move Virtual Address to a General-Purpose Register
     */
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    return (target_ulong)cap_get_cursor(cbp);
}

target_ulong helper_cloadtags(CPUMIPSState *env, uint32_t cb, uint64_t cbcursor)
{
    const cap_register_t *cbp = get_capreg_0_is_ddc(&env->active_tc, cb);

    if (!cbp->cr_tag) {
        do_raise_c2_exception(env, CP2Ca_TAG, cb);
    } else if (is_cap_sealed(cbp)) {
        do_raise_c2_exception(env, CP2Ca_SEAL, cb);
    } else if (!(cbp->cr_perms & CAP_PERM_LOAD)) {
        do_raise_c2_exception(env, CP2Ca_PERM_LD, cb);
    } else if (!(cbp->cr_perms & CAP_PERM_LOAD_CAP)) {
        do_raise_c2_exception(env, CP2Ca_PERM_LD_CAP, cb);
	} else {
    	return (target_ulong)cheri_tag_get_many(env, cbcursor, cb, NULL, GETPC());
	}
	return 0;
}

target_ulong helper_cgetbase(CPUMIPSState *env, uint32_t cb)
{
    /*
     * CGetBase: Move Base to a General-Purpose Register
     */
    return (target_ulong)get_readonly_capreg(&env->active_tc, cb)->cr_base;
}

target_ulong helper_cgetcause(CPUMIPSState *env)
{
    uint32_t perms = env->active_tc.PCC.cr_perms;
    /*
     * CGetCause: Move the Capability Exception Cause Register to a
     * General- Purpose Register
     */
    if (!(perms & CAP_ACCESS_SYS_REGS)) {
        do_raise_c2_exception_noreg(env, CP2Ca_ACCESS_SYS_REGS);
        return (target_ulong)0;
    } else {
        return (target_ulong)env->CP2_CapCause;
    }
}

target_ulong helper_cgetlen(CPUMIPSState *env, uint32_t cb)
{
    /*
     * CGetLen: Move Length to a General-Purpose Register
     */
    /* For 128-bit Capabilities we must check len >= 2^64 */
    return (target_ulong)get_readonly_capreg(&env->active_tc, cb)->cr_length;
}

target_ulong helper_cgetoffset(CPUMIPSState *env, uint32_t cb)
{
    /*
     * CGetOffset: Move Offset to a General-Purpose Register
     */
    // fprintf(qemu_logfile, "%s: offset(%d)=%016lx\n",
    //      __func__, cb, get_readonly_capreg(&env->active_tc, cb)->cr_offset);
    // return (target_ulong)(get_readonly_capreg(&env->active_tc, cb)->cr_cursor -
    //        get_readonly_capreg(&env->active_tc, cb)->cr_base);
    return (target_ulong)get_readonly_capreg(&env->active_tc, cb)->cr_offset;
}

void helper_cgetpcc(CPUMIPSState *env, uint32_t cd)
{
    /*
     * CGetPCC: Move PCC to capability register
     * See Chapter 4 in CHERI Architecture manual.
     */
    update_capreg(&env->active_tc, cd, &env->active_tc.PCC);
    /* Note that the offset(cursor) is updated by ccheck_pcc */
}

void helper_cgetpccsetoffset(CPUMIPSState *env, uint32_t cd, target_ulong rs)
{
#ifdef DO_CHERI_STATISTICS
    stat_num_cgetpccsetoffset++;
#endif
    cap_register_t *pccp = &env->active_tc.PCC;
    /*
     * CGetPCCSetOffset: Get PCC with new offset
     * See Chapter 5 in CHERI Architecture manual.
     */
    if (!is_representable(is_cap_sealed(pccp), pccp->cr_base,
                pccp->cr_length, pccp->cr_offset, rs)) {
        if (pccp->cr_tag)
            became_unrepresentable(env, cd, cgetpccsetoffset);
        cap_register_t result;
        update_capreg(&env->active_tc, cd,
            int_to_cap(pccp->cr_base + rs, &result));
    } else {
        cap_register_t result = *pccp;
        result.cr_offset = rs;
        update_capreg(&env->active_tc, cd, &result);
        check_out_of_bounds_stat(env, cgetpccsetoffset, &result);
        /* Note that the offset(cursor) is updated by ccheck_pcc */
    }
}

target_ulong helper_cgetperm(CPUMIPSState *env, uint32_t cb)
{
    /*
     * CGetPerm: Move Memory Permissions Field to a General-Purpose
     * Register
     */
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    uint64_t perms =  (uint64_t)
        ((cbp->cr_perms & CAP_PERMS_ALL) |
         ((cbp->cr_uperms & CAP_UPERMS_ALL) << CAP_UPERMS_SHFT));

    return (target_ulong)perms;
}

target_ulong helper_cgetsealed(CPUMIPSState *env, uint32_t cb)
{
    /*
     * CGetSealed: Move sealed bit to a General-Purpose Register
     */
    return (target_ulong)(is_cap_sealed(get_readonly_capreg(&env->active_tc, cb)) ? 1 : 0);
}

target_ulong helper_cgettag(CPUMIPSState *env, uint32_t cb)
{
    /*
     * CGetTag: Move Tag to a General-Purpose Register
     */
    return (target_ulong)get_readonly_capreg(&env->active_tc, cb)->cr_tag;
}

target_ulong helper_cgettype(CPUMIPSState *env, uint32_t cb)
{
    /*
     * CGetType: Move Object Type Field to a General-Purpose Register
     */
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    if (cbp->cr_sealed)
        return (target_ulong)(cbp->cr_otype & CAP_MAX_OTYPE);
    return (target_ulong)(target_long)-1;
}

void helper_cincbase(CPUMIPSState *env, uint32_t cd, uint32_t cb,
        target_ulong rt)
{
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    /*
     * CIncBase: Increase Base
     */
    if (!cbp->cr_tag && rt != 0) {
        do_raise_c2_exception(env, CP2Ca_TAG, cb);
    } else if (is_cap_sealed(cbp) && rt != 0) {
        do_raise_c2_exception(env, CP2Ca_SEAL, cb);
    } else if (rt > cbp->cr_length) {
        do_raise_c2_exception(env, CP2Ca_LENGTH, cb);
    } else {
        cap_register_t result = *cbp;
        result.cr_base = cbp->cr_base + rt;
        result.cr_length = cbp->cr_length - rt;
        update_capreg(&env->active_tc, cd, &result);
    }
}

void helper_cincoffset(CPUMIPSState *env, uint32_t cd, uint32_t cb,
        target_ulong rt)
{
#ifdef DO_CHERI_STATISTICS
    stat_num_cincoffset++;
#endif
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    /*
     * CIncOffset: Increase Offset
     */
    if (cbp->cr_tag && is_cap_sealed(cbp) && rt != 0) {
        do_raise_c2_exception(env, CP2Ca_SEAL, cb);
    } else {
        uint64_t cb_offset_plus_rt = cbp->cr_offset + rt;
        if (!is_representable(is_cap_sealed(cbp), cbp->cr_base, cbp->cr_length,
                    cbp->cr_offset, cb_offset_plus_rt)) {
            if (cbp->cr_tag) {
                became_unrepresentable(env, cd, cincoffset);
            }
            cap_register_t result;
            int_to_cap(cbp->cr_base + cb_offset_plus_rt, &result);
            update_capreg(&env->active_tc, cd, &result);
        } else {
            cap_register_t result = *cbp;
            result.cr_offset = cb_offset_plus_rt;
            update_capreg(&env->active_tc, cd, &result);
            check_out_of_bounds_stat(env, cincoffset, &result);
        }
    }
}

void helper_cmovz(CPUMIPSState *env, uint32_t cd, uint32_t cs, target_ulong rs)
{
    const cap_register_t *csp = get_readonly_capreg(&env->active_tc, cs);
    /*
     * CMOVZ: conditionally move capability on zero
     */
    if (rs == 0) {
        update_capreg(&env->active_tc, cd, csp);
    }
}

void helper_cmovn(CPUMIPSState *env, uint32_t cd, uint32_t cs, target_ulong rs)
{
    helper_cmovz(env, cd, cs, rs == 0);
}

target_ulong helper_cjalr(CPUMIPSState *env, uint32_t cd, uint32_t cb)
{
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    /*
     * CJALR: Jump and Link Capability Register
     */
    if (!cbp->cr_tag) {
        do_raise_c2_exception(env, CP2Ca_TAG, cb);
    } else if (is_cap_sealed(cbp)) {
        do_raise_c2_exception(env, CP2Ca_SEAL, cb);
    } else if (!(cbp->cr_perms & CAP_PERM_EXECUTE)) {
        do_raise_c2_exception(env, CP2Ca_PERM_EXE, cb);
    } else if (!(cbp->cr_perms & CAP_PERM_GLOBAL)) {
        do_raise_c2_exception(env, CP2Ca_GLOBAL, cb);
    } else if ((cbp->cr_offset + 4) > cbp->cr_length) {
        do_raise_c2_exception(env, CP2Ca_LENGTH, cb);
    } else if (align_of(4, cap_get_cursor(cbp))) {
        do_raise_c0_exception(env, EXCP_AdEL, cap_get_cursor(cbp));
    } else {
        cap_register_t result = env->active_tc.PCC;
        result.cr_offset += 8;
        update_capreg(&env->active_tc, cd, &result);
        // The capability register is loaded into PCC during delay slot
        env->active_tc.CapBranchTarget = *cbp;
        // Return the branch target address
        return cap_get_cursor(cbp);
    }

    return (target_ulong)0;
}

target_ulong helper_cjr(CPUMIPSState *env, uint32_t cb)
{
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    /*
     * CJR: Jump Capability Register
     */
    if (!cbp->cr_tag) {
        do_raise_c2_exception(env, CP2Ca_TAG, cb);
    } else if (is_cap_sealed(cbp)) {
        do_raise_c2_exception(env, CP2Ca_SEAL, cb);
    } else if (!(cbp->cr_perms & CAP_PERM_EXECUTE)) {
        do_raise_c2_exception(env, CP2Ca_PERM_EXE, cb);
    } else if (!(cbp->cr_perms & CAP_PERM_GLOBAL)) {
        do_raise_c2_exception(env, CP2Ca_GLOBAL, cb);
    } else if ((cbp->cr_offset + 4) > cbp->cr_length) {
        do_raise_c2_exception(env, CP2Ca_LENGTH, cb);
    } else if (align_of(4, cap_get_cursor(cbp))) {
        do_raise_c0_exception(env, EXCP_AdEL, cap_get_cursor(cbp));
    } else {
        // The capability register is loaded into PCC during delay slot
        env->active_tc.CapBranchTarget = *cbp;
        // Return the branch target address
        return cap_get_cursor(cbp);
    }

    return (target_ulong)0;
}

static void cseal_common(CPUMIPSState *env, uint32_t cd, uint32_t cs,
                         uint32_t ct, bool conditional)
{
    const cap_register_t *csp = get_readonly_capreg(&env->active_tc, cs);
    const cap_register_t *ctp = get_readonly_capreg(&env->active_tc, ct);
    uint64_t ct_base_plus_offset = cap_get_cursor(ctp);
    /*
     * CSeal: Seal a capability
     */
    if (!csp->cr_tag) {
        do_raise_c2_exception(env, CP2Ca_TAG, cs);
    } else if (!ctp->cr_tag) {
        if (conditional)
            update_capreg(&env->active_tc, cd, csp);
        else
            do_raise_c2_exception(env, CP2Ca_TAG, ct);
    } else if (conditional && cap_get_cursor(ctp) == -1) {
        update_capreg(&env->active_tc, cd, csp);
    } else if (is_cap_sealed(csp)) {
        do_raise_c2_exception(env, CP2Ca_SEAL, cs);
    } else if (is_cap_sealed(ctp)) {
        do_raise_c2_exception(env, CP2Ca_SEAL, ct);
    } else if (!(ctp->cr_perms & CAP_PERM_SEAL)) {
        do_raise_c2_exception(env, CP2Ca_PERM_SEAL, ct);
    } else if (ctp->cr_offset >= ctp->cr_length) {
        do_raise_c2_exception(env, CP2Ca_LENGTH, ct);
    } else if (ct_base_plus_offset > (uint64_t)CAP_MAX_OTYPE) {
        do_raise_c2_exception(env, CP2Ca_LENGTH, ct);
    } else if (!is_representable(true, csp->cr_base, csp->cr_length,
                csp->cr_offset, csp->cr_offset)) {
        do_raise_c2_exception(env, CP2Ca_INEXACT, cs);
    } else {
        cap_register_t result = *csp;
        result.cr_sealed = 1;
        result.cr_otype = (uint32_t)ct_base_plus_offset;
        update_capreg(&env->active_tc, cd, &result);
    }
}

void helper_cseal(CPUMIPSState *env, uint32_t cd, uint32_t cs,
        uint32_t ct)
{
    /*
     * CSeal: Seal a capability
     */
    cseal_common(env, cd, cs, ct, false);
}

void helper_ccseal(CPUMIPSState *env, uint32_t cd, uint32_t cs, uint32_t ct)
{
    /*
     * CCSeal: Conditionally seal a capability.
     */
    cseal_common(env, cd, cs, ct, true);
}

void helper_cbuildcap(CPUMIPSState *env, uint32_t cd, uint32_t cb, uint32_t ct)
{
    // CBuildCap traps on cbp == NULL so we use reg0 as $ddc. This saves encoding
    // space and also means a cbuildcap relative to $ddc can be one instr instead
    // of two.
    const cap_register_t *cbp = get_capreg_0_is_ddc(&env->active_tc, cb);
    const cap_register_t *ctp = get_readonly_capreg(&env->active_tc, ct);
    /*
     * CBuildCap: create capability from untagged register.
     * XXXAM: Note this is experimental and may change.
     */
    if (!cbp->cr_tag) {
        do_raise_c2_exception(env, CP2Ca_TAG, cb);
    } else if (is_cap_sealed(cbp)) {
        do_raise_c2_exception(env, CP2Ca_SEAL, cb);
    } else if (ctp->cr_base < cbp->cr_base) {
        do_raise_c2_exception(env, CP2Ca_LENGTH, cb);
    } else if (cap_get_top(ctp) > cap_get_top(cbp)) {
        do_raise_c2_exception(env, CP2Ca_LENGTH, cb);
    // } else if (ctp->cr_length < 0) {
    //    do_raise_c2_exception(env, CP2Ca_LENGTH, ct);
    } else if ((ctp->cr_perms & cbp->cr_perms) != ctp->cr_perms) {
        do_raise_c2_exception(env, CP2Ca_USRDEFINE, cb);
    } else if ((ctp->cr_uperms & cbp->cr_uperms) != ctp->cr_uperms) {
        do_raise_c2_exception(env, CP2Ca_USRDEFINE, cb);
    } else {
        /* XXXAM basic trivial implementation may not handle
         * compressed capabilities fully, does not perform renormalization.
         */
        // Without the temporary cap_register_t we would copy cb into cd
        // if cdp cd == ct (this was caught by testing cbuildcap $c3, $c1, $c3)
        cap_register_t result = *cbp;
        result.cr_base = ctp->cr_base;
        result.cr_length = ctp->cr_length;
        result.cr_perms = ctp->cr_perms;
        result.cr_uperms = ctp->cr_uperms;
        result.cr_offset = ctp->cr_offset;
        result.cr_sealed = 0;
        update_capreg(&env->active_tc, cd, &result);
    }
}

void helper_ccopytype(CPUMIPSState *env, uint32_t cd, uint32_t cb, uint32_t ct)
{
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    const cap_register_t *ctp = get_readonly_capreg(&env->active_tc, ct);
    /*
     * CCopyType: copy object type from untagged capability.
     * XXXAM: Note this is experimental and may change.
     */
    if (!cbp->cr_tag) {
        do_raise_c2_exception(env, CP2Ca_TAG, cb);
    } else if (is_cap_sealed(cbp)) {
        do_raise_c2_exception(env, CP2Ca_SEAL, cb);
    } else if (!is_cap_sealed(ctp)) {
        cap_register_t result;
        update_capreg(&env->active_tc, cd, int_to_cap(-1, &result));
    } else if (ctp->cr_otype < cbp->cr_base) {
        do_raise_c2_exception(env, CP2Ca_LENGTH, cb);
    } else if (ctp->cr_otype >= cap_get_top(cbp)) {
        do_raise_c2_exception(env, CP2Ca_LENGTH, cb);
    } else {
        cap_register_t result = *cbp;
        result.cr_offset = ctp->cr_otype - cbp->cr_base;
        update_capreg(&env->active_tc, cd, &result);
    }
}

static inline cap_register_t *
check_writable_cap_hwr_access(CPUMIPSState *env, enum CP2HWR hwr, target_ulong pc) {
    cheri_debug_assert((int)hwr >= (int)CP2HWR_BASE_INDEX);
    cheri_debug_assert((int)hwr < (int)(CP2HWR_BASE_INDEX + 32));
    bool access_sysregs = (env->active_tc.PCC.cr_perms & CAP_ACCESS_SYS_REGS) != 0;
    switch (hwr) {
    case CP2HWR_DDC: /* always accessible */
        return &env->active_tc.CHWR.DDC;
    case CP2HWR_USER_TLS:  /* always accessible */
        return &env->active_tc.CHWR.UserTlsCap;
    case CP2HWR_PRIV_TLS:
        if (!access_sysregs) {
            do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, hwr);
        }
        return &env->active_tc.CHWR.PrivTlsCap;
    case CP2HWR_K1RC:
        if (!in_kernel_mode(env)) {
            do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, hwr);
        }
        return &env->active_tc.CHWR.KR1C;
    case CP2HWR_K2RC:
        if (!in_kernel_mode(env)) {
            do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, hwr);
        }
        return &env->active_tc.CHWR.KR2C;
    case CP2HWR_ErrorEPCC:
#if 0
        if (!in_kernel_mode(env) || !access_sysregs) {
            do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, hwr);
        }

        return &env->active_tc.CHWR.ErrorEPCC;
#else
        break;
#endif
    case CP2HWR_KCC:
        if (!in_kernel_mode(env) || !access_sysregs) {
            do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, hwr);
        }
        return &env->active_tc.CHWR.KCC;
    case CP2HWR_KDC:
        if (!in_kernel_mode(env) || !access_sysregs) {
            do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, hwr);
        }
        return &env->active_tc.CHWR.KDC;
    case CP2HWR_EPCC:
        if (!in_kernel_mode(env) || !access_sysregs) {
            do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, hwr);
        }
        return &env->active_tc.CHWR.EPCC;
    }
    /* unknown cap hardware register */
    do_raise_exception(env, EXCP_RI, pc);
    return NULL;  // silence warning
}

static inline const cap_register_t *
check_readonly_cap_hwr_access(CPUMIPSState *env, enum CP2HWR hwr, target_ulong pc) {
    // Currently there is no difference for access permissions between read
    // and write access but that may change in the future
    return check_writable_cap_hwr_access(env, hwr, pc);
}

target_ulong helper_mfc0_epc(CPUMIPSState *env)
{
    return get_CP0_EPC(env);
}

target_ulong helper_mfc0_error_epc(CPUMIPSState *env)
{
    return get_CP0_ErrorEPC(env);
}

void helper_mtc0_epc(CPUMIPSState *env, target_ulong arg)
{
    // Check that we can write to EPCC (should always be true since we would have got a trap when not in kernel mode)
    if (!in_kernel_mode(env)) {
        do_raise_exception(env, EXCP_RI, GETPC());
    } else if ((env->active_tc.PCC.cr_perms & CAP_ACCESS_SYS_REGS) == 0) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, CP2HWR_EPCC);
    }
    set_CP0_EPC(env, arg);
}

void helper_mtc0_error_epc(CPUMIPSState *env, target_ulong arg)
{
    // Check that we can write to ErrorEPCC (should always be true since we would have got a trap when not in kernel mode)
    if (!in_kernel_mode(env)) {
        do_raise_exception(env, EXCP_RI, GETPC());
    } else if ((env->active_tc.PCC.cr_perms & CAP_ACCESS_SYS_REGS) == 0) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, CP2HWR_ErrorEPCC);
    }
    set_CP0_ErrorEPC(env, arg);
}

void helper_creadhwr(CPUMIPSState *env, uint32_t cd, uint32_t hwr)
{
    cap_register_t result = *check_readonly_cap_hwr_access(env, CP2HWR_BASE_INDEX + hwr, GETPC());
    update_capreg(&env->active_tc, cd, &result);
}

void helper_cwritehwr(CPUMIPSState *env, uint32_t cs, uint32_t hwr)
{
    const cap_register_t *csp = get_readonly_capreg(&env->active_tc, cs);
    cap_register_t *cdp = check_writable_cap_hwr_access(env, CP2HWR_BASE_INDEX + hwr, GETPC());
    *cdp = *csp;
}

void helper_csetbounds(CPUMIPSState *env, uint32_t cd, uint32_t cb,
        target_ulong rt)
{
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    uint64_t cursor = cap_get_cursor(cbp);
    uint64_t cursor_rt;
    uint64_t new_offset = 0ul;
#ifdef CHERI_128
    /*
     * With compressed capabilities we may need to increase the range of
     * memory addresses to be wider than requested so it is
     * representable.
     */
    uint64_t req_base = cursor;
    uint64_t req_top = cursor + rt;

    uint32_t BWidth = CC_L_BWIDTH;

    uint8_t E;

    uint64_t new_top = req_top;

    if(req_top == -1ULL) {
        new_top = 0; // actually 1 << 64
        rt++;
        if(rt == 0) { // actually 1 << 64
            E = 64 - BWidth + 2;
        } else {
          E = cc128_compute_e(rt, BWidth);
        }
    } else {
        E = cc128_compute_e(rt, BWidth);
    }

    if (E && (((rt >> E) & 0xF) == 0xF)) {
        new_top = ((new_top >> E) + 1) << E;
        E ++;
    }

    uint8_t need_zeros = E == 0 ? 0 : E + CC_L_LOWWIDTH;

    cursor = (cursor >> need_zeros) << need_zeros;
    new_top = ((new_top + ((UINT64_C(1) << need_zeros) - 1)) >> need_zeros) << need_zeros;

    if(new_top < req_top) new_top = -1ULL; // overflow happened somewhere

    rt = new_top - cursor;
    new_offset = req_base - cursor;

#endif /* CHERI_128 */
    cursor_rt = cursor + rt;

    /*
     * CSetBounds: Set Bounds
     */
    if (!cbp->cr_tag) {
        do_raise_c2_exception(env, CP2Ca_TAG, cb);
    } else if (is_cap_sealed(cbp)) {
        do_raise_c2_exception(env, CP2Ca_SEAL, cb);
    } else if (cursor < cbp->cr_base) {
        do_raise_c2_exception(env, CP2Ca_LENGTH, cb);
    } else if (cursor_rt < rt) {
        /* cursor + rt overflowed */
        do_raise_c2_exception(env, CP2Ca_LENGTH, cb);
    } else if (cursor_rt > cap_get_top(cbp)) {
        do_raise_c2_exception(env, CP2Ca_LENGTH, cb);
    } else {
        cap_register_t result = *cbp;
        result.cr_base = cursor;
        result.cr_length = rt;
        result.cr_offset = new_offset;
        update_capreg(&env->active_tc, cd, &result);
    }
}

void helper_csetboundsexact(CPUMIPSState *env, uint32_t cd, uint32_t cb,
        target_ulong rt)
{
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    uint64_t cursor = cap_get_cursor(cbp);
    uint64_t cursor_rt = cursor + rt;

#ifdef CHERI_128
    uint32_t bwidth = CC_L_BWIDTH;
    uint8_t e = cc128_compute_e(rt, bwidth);
    uint8_t need_zeros = e ? e + CC_L_LOWWIDTH : 0;
    uint64_t mask = (1ULL << need_zeros) - 1;

    bool representable = ((cursor | cursor_rt) & mask) == 0;
#else
    bool representable = true;
#endif

    /*
     * CSetBoundsExact: Set Bounds Exactly
     */
    if (!cbp->cr_tag) {
        do_raise_c2_exception(env, CP2Ca_TAG, cb);
    } else if (is_cap_sealed(cbp)) {
        do_raise_c2_exception(env, CP2Ca_SEAL, cb);
    } else if (cursor < cbp->cr_base) {
        do_raise_c2_exception(env, CP2Ca_LENGTH, cb);
    } else if (cursor_rt > cap_get_top(cbp)) {
        do_raise_c2_exception(env, CP2Ca_LENGTH, cb);
    } else if (!representable) {
        do_raise_c2_exception(env, CP2Ca_INEXACT, cb);
    } else {
        cap_register_t result = *cbp;
        result.cr_base = cursor;
        result.cr_length = rt;
        result.cr_offset = (target_ulong)0;
        update_capreg(&env->active_tc, cd, &result);
    }
}

//static inline bool cap_bounds_are_subset(const cap_register_t *first, const cap_register_t *second) {
//    return cap_get_base(first) <= cap_get_base(second) && cap_get_top(second) <= cap_get_top(first);
//}

target_ulong helper_csub(CPUMIPSState *env, uint32_t cb, uint32_t ct)
{
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    const cap_register_t *ctp = get_readonly_capreg(&env->active_tc, ct);

#if 0
    // This is very noisy, but may be interesting for C-compatibility analysis

    // If the capabilities are not subsets (i.e. at least one tagged and derived from different caps,
    // emit a warning to see how many subtractions are being performed that are invalid in ISO C
    if (cbp->cr_tag != ctp->cr_tag ||
        (cbp->cr_tag && !cap_bounds_are_subset(cbp, ctp) && !cap_bounds_are_subset(ctp, cbp))) {
        // Don't warn about subtracting NULL:
        if (!is_null_capability(ctp)) {
            warn_report("Subtraction between two capabilities that are not subsets: \r\n"
                    "\tLHS: " PRINT_CAP_FMTSTR "\r\n\tRHS: " PRINT_CAP_FMTSTR "\r",
                    PRINT_CAP_ARGS(cbp), PRINT_CAP_ARGS(ctp));
        }
    }
#endif
    /*
     * CSub: Subtract Capabilities
     */
    return (target_ulong)(cap_get_cursor(cbp) - cap_get_cursor(ctp));
}

void helper_csetcause(CPUMIPSState *env, target_ulong rt)
{
    uint32_t perms = env->active_tc.PCC.cr_perms;
    /*
     * CSetCause: Set the Capability Exception Cause Register
     */
    if (!(perms & CAP_ACCESS_SYS_REGS)) {
        do_raise_c2_exception_noreg(env, CP2Ca_ACCESS_SYS_REGS);
    } else {
        env->CP2_CapCause = (uint16_t)(rt & 0xffffUL);
    }
}

void helper_csetlen(CPUMIPSState *env, uint32_t cd, uint32_t cb,
        target_ulong rt)
{
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    /*
     * CSetLen: Set Length
     */
    if (!cbp->cr_tag) {
        do_raise_c2_exception(env, CP2Ca_TAG, cb);
    } else if (is_cap_sealed(cbp)) {
        do_raise_c2_exception(env, CP2Ca_SEAL, cb);
    } else if (rt > cbp->cr_length) {
        do_raise_c2_exception(env, CP2Ca_LENGTH, cb);
    } else {
        cap_register_t result = *cbp;
        result.cr_length = rt;
        update_capreg(&env->active_tc, cd, &result);
    }
}

void helper_csetoffset(CPUMIPSState *env, uint32_t cd, uint32_t cb,
        target_ulong rt)
{
#ifdef DO_CHERI_STATISTICS
    stat_num_csetoffset++;
#endif
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    /*
     * CSetOffset: Set cursor to an offset from base
     */
    if (cbp->cr_tag && is_cap_sealed(cbp)) {
        do_raise_c2_exception(env, CP2Ca_SEAL, cb);
    } else {
        if (!is_representable(is_cap_sealed(cbp), cbp->cr_base, cbp->cr_length,
                    cbp->cr_offset, rt)) {
            if (cbp->cr_tag)
                became_unrepresentable(env, cd, csetoffset);
            cap_register_t result;
            update_capreg(&env->active_tc, cd, int_to_cap(cbp->cr_base + rt, &result));
        } else {
            cap_register_t result = *cbp;
            result.cr_offset = rt;
            update_capreg(&env->active_tc, cd, &result);
            check_out_of_bounds_stat(env, csetoffset, &result);
        }
    }
}

target_ulong helper_ctoptr(CPUMIPSState *env, uint32_t cb, uint32_t ct)
{
    // CToPtr traps on ctp == NULL so we use reg0 as $ddc there. This means we
    // can have a CToPtr relative to $ddc as one instruction instead of two and
    // is required since clang still assumes it can use zero as $ddc in cfromptr/ctoptr
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    const cap_register_t *ctp = get_capreg_0_is_ddc(&env->active_tc, ct);
    uint64_t cb_cursor = cap_get_cursor(cbp);
    uint64_t ct_top = cap_get_top(ctp);
    /*
     * CToPtr: Capability to Pointer
     */
    if (!ctp->cr_tag) {
        do_raise_c2_exception(env, CP2Ca_TAG, ct);
    } else if (!cbp->cr_tag) {
        return (target_ulong)0;
    } else if ((cb_cursor < ctp->cr_base) || (cb_cursor > ct_top)) {
        /* XXX cb can not be wholly represented within ct. */
        return (target_ulong)0;
    } else if (ctp->cr_base > cb_cursor) {
        return (target_ulong)(ctp->cr_base - cb_cursor);
    } else {
        return (target_ulong)(cb_cursor - ctp->cr_base);
    }

    return (target_ulong)0;
}

void helper_cunseal(CPUMIPSState *env, uint32_t cd, uint32_t cs,
        uint32_t ct)
{
    const cap_register_t *csp = get_readonly_capreg(&env->active_tc, cs);
    const cap_register_t *ctp = get_readonly_capreg(&env->active_tc, ct);
    /*
     * CUnseal: Unseal a sealed capability
     */
    if (!csp->cr_tag) {
        do_raise_c2_exception(env, CP2Ca_TAG, cs);
    } else if (!ctp->cr_tag) {
        do_raise_c2_exception(env, CP2Ca_TAG, ct);
    } else if (!is_cap_sealed(csp)) {
        do_raise_c2_exception(env, CP2Ca_SEAL, cs);
    } else if (is_cap_sealed(ctp)) {
        do_raise_c2_exception(env, CP2Ca_SEAL, ct);
    } else if (cap_get_cursor(ctp) != csp->cr_otype) {
        do_raise_c2_exception(env, CP2Ca_TYPE, ct);
    } else if (!(ctp->cr_perms & CAP_PERM_UNSEAL)) {
        do_raise_c2_exception(env, CP2Ca_PERM_UNSEAL, ct);
    } else if (ctp->cr_offset >= ctp->cr_length) {
        do_raise_c2_exception(env, CP2Ca_LENGTH, ct);
    } else if (cap_get_cursor(ctp) >= CAP_MAX_OTYPE) {
        do_raise_c2_exception(env, CP2Ca_LENGTH, ct);
    } else {
        cap_register_t result = *csp;
        if ((csp->cr_perms & CAP_PERM_GLOBAL) &&
            (ctp->cr_perms & CAP_PERM_GLOBAL)) {
            result.cr_perms |= CAP_PERM_GLOBAL;
        } else {
            result.cr_perms &= ~CAP_PERM_GLOBAL;
        }
        result.cr_sealed = 0;
        result.cr_otype = 0;
        update_capreg(&env->active_tc, cd, &result);
    }
}

/*
 * CPtrCmp Instructions. Capability Pointer Compare.
 */
target_ulong helper_ceq(CPUMIPSState *env, uint32_t cb, uint32_t ct)
{
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    const cap_register_t *ctp = get_readonly_capreg(&env->active_tc, ct);
    gboolean equal = FALSE;
    /*
     * CEQ: Capability pointers equal
     */
{
        if (cbp->cr_tag != ctp->cr_tag) {
            equal = FALSE;
        } else {
            uint64_t cursor1 = cap_get_cursor(cbp);
            uint64_t cursor2 = cap_get_cursor(ctp);

            equal = (cursor1 == cursor2);
        }
    }
    return (target_ulong) equal;
}

target_ulong helper_cne(CPUMIPSState *env, uint32_t cb, uint32_t ct)
{
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    const cap_register_t *ctp = get_readonly_capreg(&env->active_tc, ct);
    gboolean equal = FALSE;
    /*
     * CNE: Capability pointers not equal
     */
{
        if (cbp->cr_tag != ctp->cr_tag) {
            equal = FALSE;
        } else {
            uint64_t cursor1 = cap_get_cursor(cbp);
            uint64_t cursor2 = cap_get_cursor(ctp);

            equal = (cursor1 == cursor2);
        }
    }
    return (target_ulong) !equal;
}

target_ulong helper_clt(CPUMIPSState *env, uint32_t cb, uint32_t ct)
{
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    const cap_register_t *ctp = get_readonly_capreg(&env->active_tc, ct);
    gboolean signed_less = FALSE;
    /*
     * CLT: Capability pointers less than (signed)
     */
{
        if (cbp->cr_tag != ctp->cr_tag) {
            if (cbp->cr_tag) {
                signed_less = FALSE;
            } else {
                signed_less = TRUE;
            }
        } else {
            int64_t cursor1 = (int64_t)cap_get_cursor(cbp);
            int64_t cursor2 = (int64_t)cap_get_cursor(ctp);

            signed_less = (cursor1 < cursor2);
        }
    }
    return (target_ulong) signed_less;
}

target_ulong helper_cle(CPUMIPSState *env, uint32_t cb, uint32_t ct)
{
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    const cap_register_t *ctp = get_readonly_capreg(&env->active_tc, ct);
    gboolean signed_lte = FALSE;
    /*
     * CLE: Capability pointers less than equal (signed)
     */
{
        if (cbp->cr_tag != ctp->cr_tag) {
            if (cbp->cr_tag) {
                signed_lte = FALSE;
            } else {
                signed_lte = TRUE;
            }
        } else {
            int64_t cursor1 = (int64_t)cap_get_cursor(cbp);
            int64_t cursor2 = (int64_t)cap_get_cursor(ctp);

            signed_lte = (cursor1 <= cursor2);
        }
    }
    return (target_ulong) signed_lte;
}

target_ulong helper_cltu(CPUMIPSState *env, uint32_t cb, uint32_t ct)
{
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    const cap_register_t *ctp = get_readonly_capreg(&env->active_tc, ct);
    gboolean ltu = FALSE;
    /*
     * CLTU: Capability pointers less than (unsigned)
     */
{
        if (cbp->cr_tag != ctp->cr_tag) {
            if (cbp->cr_tag) {
                ltu = FALSE;
            } else {
                ltu = TRUE;
            }
        } else {
            uint64_t cursor1 = cap_get_cursor(cbp);
            uint64_t cursor2 = cap_get_cursor(ctp);

            ltu = (cursor1 < cursor2);
        }
    }
    return (target_ulong) ltu;
}

target_ulong helper_cleu(CPUMIPSState *env, uint32_t cb, uint32_t ct)
{
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    const cap_register_t *ctp = get_readonly_capreg(&env->active_tc, ct);
    gboolean leu = FALSE;
    /*
     * CLEU: Capability pointers less than equal (unsigned)
     */
{
        if (cbp->cr_tag != ctp->cr_tag) {
            if (cbp->cr_tag) {
                leu = FALSE;
            } else {
                leu = TRUE;
            }
        } else {
            uint64_t cursor1 = cap_get_cursor(cbp);
            uint64_t cursor2 = cap_get_cursor(ctp);

            leu = (cursor1 <= cursor2);
        }
    }
    return (target_ulong) leu;
}

target_ulong helper_cexeq(CPUMIPSState *env, uint32_t cb, uint32_t ct)
{
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    const cap_register_t *ctp = get_readonly_capreg(&env->active_tc, ct);
    gboolean equal = FALSE;
    /*
     * CEXEQ: Capability pointers equal (all fields)
     */
    {
        if (cbp->cr_tag != ctp->cr_tag) {
            equal = FALSE;
        } else if (cbp->cr_base != ctp->cr_base) {
            equal = FALSE;
        } else if (cbp->cr_offset != ctp->cr_offset) {
            equal = FALSE;
        } else if (cbp->cr_length != ctp->cr_length) {
            equal = FALSE;
        } else if (cbp->cr_otype != ctp->cr_otype) {
            equal = FALSE;
        } else if (cbp->cr_perms != ctp->cr_perms) {
            equal = FALSE;
        } else {
            equal = TRUE;
        }
    }
    return (target_ulong) equal;
}

target_ulong helper_cnexeq(CPUMIPSState *env, uint32_t cb, uint32_t ct)
{
    gboolean not_equal = helper_cexeq(env, cb, ct) ? FALSE : TRUE;

    return (target_ulong) not_equal;
}

target_ulong helper_ctestsubset(CPUMIPSState *env, uint32_t cb, uint32_t ct)
{
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    const cap_register_t *ctp = get_readonly_capreg(&env->active_tc, ct);
    gboolean is_subset = FALSE;
    /*
     * CTestSubset: Test if capability is a subset of another
     */
    {
        if (cbp->cr_tag == ctp->cr_tag &&
            /* is_cap_sealed(cbp) == is_cap_sealed(ctp) && */
            cap_get_base(cbp) <= cap_get_base(ctp) &&
            cap_get_top(ctp) <= cap_get_top(cbp) &&
            (ctp->cr_perms & cbp->cr_perms) == ctp->cr_perms &&
            (ctp->cr_uperms & cbp->cr_uperms) == ctp->cr_uperms) {
            is_subset = TRUE;
        }
    }
    return (target_ulong) is_subset;
}

/*
 * Load Via Capability Register
 */
target_ulong helper_cload(CPUMIPSState *env, uint32_t cb, target_ulong rt,
        uint32_t offset, uint32_t size)
{
    // CL[BHWD][U] traps on cbp == NULL so we use reg0 as $ddc to save encoding
    // space and increase code density since loading relative to $ddc is common
    // in the hybrid ABI (and also for backwards compat with old binaries).
    const cap_register_t *cbp = get_capreg_0_is_ddc(&env->active_tc, cb);

    if (!cbp->cr_tag) {
        do_raise_c2_exception(env, CP2Ca_TAG, cb);
    } else if (is_cap_sealed(cbp)) {
        do_raise_c2_exception(env, CP2Ca_SEAL, cb);
    } else if (!(cbp->cr_perms & CAP_PERM_LOAD)) {
        do_raise_c2_exception(env, CP2Ca_PERM_LD, cb);
    } else {
        uint64_t cursor = cap_get_cursor(cbp);
        uint64_t addr = cursor + rt + (int32_t)offset;

        if ((addr + size) > cap_get_top(cbp)) {
            do_raise_c2_exception(env, CP2Ca_LENGTH, cb);
        } else if (addr < cbp->cr_base) {
            do_raise_c2_exception(env, CP2Ca_LENGTH, cb);
        } else if (align_of(size, addr)) {
#if defined(CHERI_UNALIGNED)
            qemu_log_mask(CPU_LOG_INSTR, "Allowing unaligned %d-byte load of "
                "address 0x%" PRIx64 "\n", size, addr);
            return addr;
#else
            // TODO: is this actually needed? tcg_gen_qemu_st_tl() should
            // check for alignment already.
            do_raise_c0_exception(env, EXCP_AdEL, addr);
#endif
        } else {
            return addr;
        }
    }
    return 0;
}

/*
 * Load Linked Via Capability Register
 */
target_ulong helper_cloadlinked(CPUMIPSState *env, uint32_t cb, uint32_t size)
{
    // CLL[BHWD][U] traps on cbp == NULL so we use reg0 as $ddc to save encoding
    // space and increase code density since loading relative to $ddc is common
    // in the hybrid ABI (and also for backwards compat with old binaries).
    const cap_register_t *cbp = get_capreg_0_is_ddc(&env->active_tc, cb);
    uint64_t addr = cap_get_cursor(cbp);

    env->linkedflag = 0;
    if (!cbp->cr_tag) {
        do_raise_c2_exception(env, CP2Ca_TAG, cb);
    } else if (is_cap_sealed(cbp)) {
        do_raise_c2_exception(env, CP2Ca_SEAL, cb);
    } else if (!(cbp->cr_perms & CAP_PERM_LOAD)) {
        do_raise_c2_exception(env, CP2Ca_PERM_LD, cb);
    } else if ((addr + size) > cap_get_top(cbp)) {
        do_raise_c2_exception(env, CP2Ca_LENGTH, cb);
    } else if (addr < cbp->cr_base) {
        do_raise_c2_exception(env, CP2Ca_LENGTH, cb);
    } else if (align_of(size, addr)) {
        // TODO: should #if (CHERI_UNALIGNED) also disable this check?
        do_raise_c0_exception(env, EXCP_AdEL, addr);
    } else {
        env->linkedflag = 1;
        return addr;
    }
    return 0;
}

/*
 * Store Conditional Via Capability Register
 */
target_ulong helper_cstorecond(CPUMIPSState *env, uint32_t cb, uint32_t size)
{
    // CSC[BHWD] traps on cbp == NULL so we use reg0 as $ddc to save encoding
    // space and increase code density since storing relative to $ddc is common
    // in the hybrid ABI (and also for backwards compat with old binaries).
    const cap_register_t *cbp = get_capreg_0_is_ddc(&env->active_tc, cb);
    uint64_t addr = cap_get_cursor(cbp);

    if (!cbp->cr_tag) {
        do_raise_c2_exception(env, CP2Ca_TAG, cb);
    } else if (is_cap_sealed(cbp)) {
        do_raise_c2_exception(env, CP2Ca_SEAL, cb);
    } else if (!(cbp->cr_perms & CAP_PERM_STORE)) {
        do_raise_c2_exception(env, CP2Ca_PERM_ST, cb);
    } else if ((addr + size) > cap_get_top(cbp)) {
        do_raise_c2_exception(env, CP2Ca_LENGTH, cb);
    } else if (addr < cbp->cr_base) {
        do_raise_c2_exception(env, CP2Ca_LENGTH, cb);
    } else if (align_of(size, addr)) {
        // TODO: should #if (CHERI_UNALIGNED) also disable this check?
        do_raise_c0_exception(env, EXCP_AdES, addr);
    } else {
        // Can't do this here.  It might miss in the TLB.
        // cheri_tag_invalidate(env, addr, size);
        // Also, rd is set by the actual store conditional operation.
        return addr;
    }
    return 0;
}

/*
 * Store Via Capability Register
 */
target_ulong helper_cstore(CPUMIPSState *env, uint32_t cb, target_ulong rt,
        uint32_t offset, uint32_t size)
{
    // CS[BHWD][U] traps on cbp == NULL so we use reg0 as $ddc to save encoding
    // space and increase code density since storing relative to $ddc is common
    // in the hybrid ABI (and also for backwards compat with old binaries).
    const cap_register_t *cbp = get_capreg_0_is_ddc(&env->active_tc, cb);

    if (!cbp->cr_tag) {
        do_raise_c2_exception(env, CP2Ca_TAG, cb);
    } else if (is_cap_sealed(cbp)) {
        do_raise_c2_exception(env, CP2Ca_SEAL, cb);
    } else if (!(cbp->cr_perms & CAP_PERM_STORE)) {
        do_raise_c2_exception(env, CP2Ca_PERM_ST, cb);
    } else {
        uint64_t cursor = cap_get_cursor(cbp);
        uint64_t addr = cursor + rt + (int32_t)offset;

        if ((addr + size) > cap_get_top(cbp)) {
            do_raise_c2_exception(env, CP2Ca_LENGTH, cb);
        } else if (addr < cbp->cr_base) {
            do_raise_c2_exception(env, CP2Ca_LENGTH, cb);
        } else if (align_of(size, addr)) {
#if defined(CHERI_UNALIGNED)
            qemu_log_mask(CPU_LOG_INSTR, "Allowing unaligned %d-byte store to "
                "address 0x%" PRIx64 "\n", size, addr);
            // Can't do this here.  It might miss in the TLB.
            // cheri_tag_invalidate(env, addr, size);
            return addr;
#else
            // TODO: is this actually needed? tcg_gen_qemu_st_tl() should
            // check for alignment already.
            do_raise_c0_exception(env, EXCP_AdES, addr);
#endif
        } else {
            // Can't do this here.  It might miss in the TLB.
            // cheri_tag_invalidate(env, addr, size);
            return addr;
        }
    }
    return 0;
}

target_ulong helper_clc_addr(CPUMIPSState *env, uint32_t cd, uint32_t cb,
        target_ulong rt, uint32_t offset)
{
    // CLC traps on cbp == NULL so we use reg0 as $ddc to save encoding
    // space and increase code density since loading relative to $ddc is common
    // in the hybrid ABI (and also for backwards compat with old binaries).
    const cap_register_t *cbp = get_capreg_0_is_ddc(&env->active_tc, cb);

    if (!cbp->cr_tag) {
        do_raise_c2_exception(env, CP2Ca_TAG, cb);
        return (target_ulong)0;
    } else if (is_cap_sealed(cbp)) {
        do_raise_c2_exception(env, CP2Ca_SEAL, cb);
        return (target_ulong)0;
    } else if (!(cbp->cr_perms & CAP_PERM_LOAD)) {
        do_raise_c2_exception(env, CP2Ca_PERM_LD, cb);
        return (target_ulong)0;
    } else {
        uint64_t cursor = cap_get_cursor(cbp);
        uint64_t addr = (uint64_t)((cursor + rt) + (int32_t)offset);
        /* uint32_t tag = cheri_tag_get(env, addr, cd, NULL); */

        if ((addr + CHERI_CAP_SIZE) > cap_get_top(cbp)) {
            do_raise_c2_exception(env, CP2Ca_LENGTH, cb);
            return (target_ulong)0;
        } else if (addr < cbp->cr_base) {
            do_raise_c2_exception(env, CP2Ca_LENGTH, cb);
            return (target_ulong)0;
        } else if (align_of(CHERI_CAP_SIZE, addr)) {
            do_raise_c0_exception(env, EXCP_AdEL, addr);
            return (target_ulong)0;
        }

        /*
         * XXX Don't chance taking the TLB missing in cheri_tag_get().
         * Do the first load of the capability and then get the tag in
         * helper_bytes2cap_op() below.
        tag = cheri_tag_get(env, addr, cd, NULL);
        if (env->TLB_L)
            tag = 0;
        cdp->cr_tag = tag;
        */
        return (target_ulong)addr;
    }
}

target_ulong helper_cllc_addr(CPUMIPSState *env, uint32_t cd, uint32_t cb)
{
    // CLLC traps on cbp == NULL so we use reg0 as $ddc to save encoding
    // space and increase code density since loading relative to $ddc is common
    // in the hybrid ABI (and also for backwards compat with old binaries).
    const cap_register_t *cbp = get_capreg_0_is_ddc(&env->active_tc, cb);
    uint64_t addr = cap_get_cursor(cbp);

    env->linkedflag = 0;
    if (!cbp->cr_tag) {
        do_raise_c2_exception(env, CP2Ca_TAG, cb);
        return (target_ulong)0;
    } else if (is_cap_sealed(cbp)) {
        do_raise_c2_exception(env, CP2Ca_SEAL, cb);
        return (target_ulong)0;
    } else if (!(cbp->cr_perms & CAP_PERM_LOAD)) {
        do_raise_c2_exception(env, CP2Ca_PERM_LD, cb);
        return (target_ulong)0;
    } else if ((addr + CHERI_CAP_SIZE) > cap_get_top(cbp)) {
        do_raise_c2_exception(env, CP2Ca_LENGTH, cb);
        return (target_ulong)0;
    } else if (addr < cbp->cr_base) {
        do_raise_c2_exception(env, CP2Ca_LENGTH, cb);
        return (target_ulong)0;
    } else if (align_of(CHERI_CAP_SIZE, addr)) {
         do_raise_c0_exception(env, EXCP_AdEL, addr);
         return (target_ulong)0;
    }

    /*
     * XXX Don't chance taking the TLB missing in cheri_tag_get().
     * Do the first load of the capability and then get the tag in
     * helper_bytes2cap_opll() below.
    tag = cheri_tag_get(env, addr, cd, &env->lladdr);
    if (env->TLB_L)
        tag = 0;
    cdp->cr_tag = tag;
    */

    env->linkedflag = 1;

    return (target_ulong)addr;
}

target_ulong helper_csc_addr(CPUMIPSState *env, uint32_t cs, uint32_t cb,
        target_ulong rt, uint32_t offset)
{
    // CSC traps on cbp == NULL so we use reg0 as $ddc to save encoding
    // space and increase code density since storing relative to $ddc is common
    // in the hybrid ABI (and also for backwards compat with old binaries).
    const cap_register_t *cbp = get_capreg_0_is_ddc(&env->active_tc, cb);
    const cap_register_t *csp = get_readonly_capreg(&env->active_tc, cs);

    if (!cbp->cr_tag) {
        do_raise_c2_exception(env, CP2Ca_TAG, cb);
        return (target_ulong)0;
    } else if (is_cap_sealed(cbp)) {
        do_raise_c2_exception(env, CP2Ca_SEAL, cb);
        return (target_ulong)0;
    } else if (!(cbp->cr_perms & CAP_PERM_STORE)) {
        do_raise_c2_exception(env, CP2Ca_PERM_ST, cb);
        return (target_ulong)0;
    } else if (!(cbp->cr_perms & CAP_PERM_STORE_CAP)) {
        do_raise_c2_exception(env, CP2Ca_PERM_ST_CAP, cb);
        return (target_ulong)0;
    } else if (!(cbp->cr_perms & CAP_PERM_STORE_LOCAL) && csp->cr_tag &&
            !(csp->cr_perms & CAP_PERM_GLOBAL)) {
        do_raise_c2_exception(env, CP2Ca_PERM_ST_LC_CAP, cb);
        return (target_ulong)0;
    } else {
        uint64_t cursor = cap_get_cursor(cbp);
        uint64_t addr = (uint64_t)((int64_t)(cursor + rt) + (int32_t)offset);

        if ((addr + CHERI_CAP_SIZE) > cap_get_top(cbp)) {
            do_raise_c2_exception(env, CP2Ca_LENGTH, cb);
            return (target_ulong)0;
        } else if (addr < cbp->cr_base) {
            do_raise_c2_exception(env, CP2Ca_LENGTH, cb);
            return (target_ulong)0;
        } else if (align_of(CHERI_CAP_SIZE, addr)) {
            do_raise_c0_exception(env, EXCP_AdES, addr);
            return (target_ulong)0;
        }

        return (target_ulong)addr;
    }
}

target_ulong helper_cscc_addr(CPUMIPSState *env, uint32_t cs, uint32_t cb)
{
    // CSCC traps on cbp == NULL so we use reg0 as $ddc to save encoding
    // space and increase code density since storing relative to $ddc is common
    // in the hybrid ABI (and also for backwards compat with old binaries).
    const cap_register_t *cbp = get_capreg_0_is_ddc(&env->active_tc, cb);
    const cap_register_t *csp = get_readonly_capreg(&env->active_tc, cs);
    uint64_t addr = cap_get_cursor(cbp);

    if (!cbp->cr_tag) {
        do_raise_c2_exception(env, CP2Ca_TAG, cb);
        return (target_ulong)0;
    } else if (is_cap_sealed(cbp)) {
        do_raise_c2_exception(env, CP2Ca_SEAL, cb);
        return (target_ulong)0;
    } else if (!(cbp->cr_perms & CAP_PERM_STORE)) {
        do_raise_c2_exception(env, CP2Ca_PERM_ST, cb);
        return (target_ulong)0;
    } else if (!(cbp->cr_perms & CAP_PERM_STORE_CAP)) {
        do_raise_c2_exception(env, CP2Ca_PERM_ST_CAP, cb);
        return (target_ulong)0;
    } else if (!(cbp->cr_perms & CAP_PERM_STORE_LOCAL) && csp->cr_tag &&
            !(csp->cr_perms & CAP_PERM_GLOBAL)) {
        do_raise_c2_exception(env, CP2Ca_PERM_ST_LC_CAP, cb);
        return (target_ulong)0;
    } else if ((addr + CHERI_CAP_SIZE) > cap_get_top(cbp)) {
        do_raise_c2_exception(env, CP2Ca_LENGTH, cb);
        return (target_ulong)0;
    } else if (addr < cbp->cr_base) {
        do_raise_c2_exception(env, CP2Ca_LENGTH, cb);
        return (target_ulong)0;
    } else if (align_of(CHERI_CAP_SIZE, addr)) {
        do_raise_c0_exception(env, EXCP_AdES, addr);
        return (target_ulong)0;
    }

    return (target_ulong)addr;
}

#ifdef CONFIG_MIPS_LOG_INSTR
/*
 * Dump cap tag, otype, permissions and seal bit to cvtrace entry
 */
static inline void
cvtrace_dump_cap_perms(cvtrace_t *cvtrace, cap_register_t *cr)
{
    if (unlikely(qemu_loglevel_mask(CPU_LOG_CVTRACE))) {
        cvtrace->val2 = tswap64(((uint64_t)cr->cr_tag << 63) |
            ((uint64_t)(cr->cr_otype & CAP_MAX_OTYPE)<< 32) |
            ((((cr->cr_uperms & CAP_UPERMS_ALL) << CAP_UPERMS_SHFT) |
              (cr->cr_perms & CAP_PERMS_ALL)) << 1) |
            (uint64_t)(is_cap_sealed(cr) ? 1 : 0));
    }
}

/*
 * Dump capability cursor, base and length to cvtrace entry
 */
static inline void cvtrace_dump_cap_cbl(cvtrace_t *cvtrace, const cap_register_t *cr)
{
    if (unlikely(qemu_loglevel_mask(CPU_LOG_CVTRACE))) {
        cvtrace->val3 = tswap64(cr->cr_offset + cr->cr_base);
        cvtrace->val4 = tswap64(cr->cr_base);
        cvtrace->val5 = tswap64(cr->cr_length);
    }
}

void dump_changed_capreg(CPUMIPSState *env, cap_register_t *cr,
        cap_register_t *old_reg, const char* name)
{
    if (memcmp(cr, old_reg, sizeof(cap_register_t))) {
        *old_reg = *cr;
        if (qemu_loglevel_mask(CPU_LOG_CVTRACE)) {
            if (env->cvtrace.version == CVT_NO_REG ||
                env->cvtrace.version == CVT_GPR)
                env->cvtrace.version = CVT_CAP;
            if (env->cvtrace.version == CVT_ST_GPR)
                env->cvtrace.version = CVT_ST_CAP;
            cvtrace_dump_cap_perms(&env->cvtrace, cr);
            cvtrace_dump_cap_cbl(&env->cvtrace, cr);
        }
        if (qemu_loglevel_mask(CPU_LOG_INSTR)) {
            print_capreg(qemu_logfile, cr, "    Write ", name);
        }
    }
}


void dump_changed_cop2(CPUMIPSState *env, TCState *cur) {
    static const char * const capreg_name[] = {
        "C00", "C01", "C02", "C03", "C04", "C05", "C06", "C07",
        "C08", "C09", "C10", "C11", "C12", "C13", "C14", "C15",
        "C16", "C17", "C18", "C19", "C20", "C21", "C22", "C23",
        "C24", "C25", "C26", "C27", "C28", "C29", "C30", "C31",
    };
    for (int i=0; i<32; i++) {
        dump_changed_capreg(env, &cur->_CGPR[i], &env->last_C[i], capreg_name[i]);
    }
    dump_changed_capreg(env, &cur->CHWR.DDC, &env->last_CHWR.DDC, "DDC");
    dump_changed_capreg(env, &cur->CHWR.UserTlsCap, &env->last_CHWR.UserTlsCap, "UserTlsCap");
    dump_changed_capreg(env, &cur->CHWR.PrivTlsCap, &env->last_CHWR.PrivTlsCap, "PrivTlsCap");
    dump_changed_capreg(env, &cur->CHWR.KR1C, &env->last_CHWR.KR1C, "ChwrKR1C");
    dump_changed_capreg(env, &cur->CHWR.KR2C, &env->last_CHWR.KR2C, "ChwrKR1C");
    dump_changed_capreg(env, &cur->CHWR.ErrorEPCC, &env->last_CHWR.ErrorEPCC, "ErrorEPCC");
    dump_changed_capreg(env, &cur->CHWR.KCC, &env->last_CHWR.KCC, "KCC");
    dump_changed_capreg(env, &cur->CHWR.KDC, &env->last_CHWR.KDC, "KDC");
    dump_changed_capreg(env, &cur->CHWR.EPCC, &env->last_CHWR.EPCC, "EPCC");
    dump_changed_capreg(env, &cur->CapBranchTarget, &env->last_CapBranchTarget, "CapBranchTarget");
}

/*
 * Dump cap load or store to cvtrace
 */
static inline void cvtrace_dump_cap_ldst(cvtrace_t *cvtrace, uint8_t version,
        uint64_t addr, const cap_register_t *cr)
{
    if (unlikely(qemu_loglevel_mask(CPU_LOG_CVTRACE))) {
        cvtrace->version = version;
        cvtrace->val1 = tswap64(addr);
        cvtrace->val2 = tswap64(((uint64_t)cr->cr_tag << 63) |
            ((uint64_t)(cr->cr_otype & CAP_MAX_OTYPE)<< 32) |
            ((((cr->cr_uperms & CAP_UPERMS_ALL) << CAP_UPERMS_SHFT) |
              (cr->cr_perms & CAP_PERMS_ALL)) << 1) |
            (uint64_t)(is_cap_sealed(cr) ? 1 : 0));
    }
}
#define cvtrace_dump_cap_load(trace, addr, cr)          \
    cvtrace_dump_cap_ldst(trace, CVT_LD_CAP, addr, cr)
#define cvtrace_dump_cap_store(trace, addr, cr)         \
    cvtrace_dump_cap_ldst(trace, CVT_ST_CAP, addr, cr)

#endif // CONFIG_MIPS_LOG_INSTR

#ifdef CHERI_128

#ifdef CONFIG_MIPS_LOG_INSTR
/*
 * Print capability load from memory to log file.
 */
static inline void dump_cap_load(uint64_t addr, uint64_t pesbt,
        uint64_t cursor, uint8_t tag)
{

    if (unlikely(qemu_loglevel_mask(CPU_LOG_INSTR))) {
        fprintf(qemu_logfile, "    Cap Memory Read [" TARGET_FMT_lx
                "] = v:%d PESBT:" TARGET_FMT_lx " Cursor:" TARGET_FMT_lx "\n",
                addr, tag, pesbt, cursor);
    }
}

/*
 * Print capability store to memory to log file.
 */
static inline void dump_cap_store(uint64_t addr, uint64_t pesbt,
        uint64_t cursor, uint8_t tag)
{

    if (unlikely(qemu_loglevel_mask(CPU_LOG_INSTR))) {
        fprintf(qemu_logfile, "    Cap Memory Write [" TARGET_FMT_lx
                "] = v:%d PESBT:" TARGET_FMT_lx " Cursor:" TARGET_FMT_lx "\n",
                addr, tag, pesbt, cursor);
    }
}
#endif // CONFIG_MIPS_LOG_INSTR


target_ulong helper_bytes2cap_128_tag_get(CPUMIPSState *env, uint32_t cd,
        uint32_t cb, target_ulong addr)
{
    /* This could be done in helper_bytes2cap_128 but TCG limits the number
     * of arguments to 5 so we have to have a separate helper to handle the tag.
     */
    // Since this is used by cl* we need to treat cb == 0 as $ddc
    const cap_register_t *cbp = get_capreg_0_is_ddc(&env->active_tc, cb);
    target_ulong tag = cheri_tag_get(env, addr, cd, NULL, GETPC());
    tag = clear_tag_if_no_loadcap(env, tag, cbp);
    return tag;
}

void helper_bytes2cap_128(CPUMIPSState *env, uint32_t cd, target_ulong pesbt,
        target_ulong cursor)
{
    cap_register_t *cdp = get_writable_capreg_raw(&env->active_tc, cd);

    decompress_128cap(pesbt, cursor, cdp);
}

void helper_bytes2cap_128_tag_set(CPUMIPSState *env, uint32_t cd,
        target_ulong tag, target_ulong addr, target_ulong cursor)
{
    /* This could be done in helper_bytes2cap_128 but TCG limits the number
     * of arguments to 5 so we have to have a separate helper to handle the tag.
     */
    cap_register_t *cdp = get_writable_capreg_raw(&env->active_tc, cd);

    cdp->cr_tag = tag;
#ifdef CONFIG_MIPS_LOG_INSTR
    /* Log memory read, if needed. */
    dump_cap_load(addr, cdp->cr_pesbt, cursor, tag);
    cvtrace_dump_cap_load(&env->cvtrace, addr, cdp);
    cvtrace_dump_cap_cbl(&env->cvtrace, cdp);
#endif
}

target_ulong helper_cap2bytes_128b(CPUMIPSState *env, uint32_t cs,
        target_ulong vaddr)
{
    const cap_register_t *csp = get_readonly_capreg(&env->active_tc, cs);
    target_ulong ret;

    if (csp->cr_tag)
        ret = compress_128cap(csp);
    else
        ret = csp->cr_pesbt;

#ifdef CONFIG_MIPS_LOG_INSTR
    /* Log memory cap write, if needed. */
    dump_cap_store(vaddr, ret, csp->cr_offset + csp->cr_base, csp->cr_tag);
    cvtrace_dump_cap_store(&env->cvtrace, vaddr, csp);
    cvtrace_dump_cap_cbl(&env->cvtrace, csp);
#endif

    return ret;
}

target_ulong helper_cap2bytes_128c(CPUMIPSState *env, uint32_t cs,
        uint32_t bdoffset, target_ulong vaddr)
{
    const cap_register_t *csp = get_readonly_capreg(&env->active_tc, cs);
    target_ulong ret;
    uint32_t save_hflags = env->hflags;

    /* Are we in a branch delay slot? */
    switch(bdoffset) {
    case 4:
        env->hflags |= MIPS_HFLAG_BDS32;
        break;
    case 2:
        env->hflags |= MIPS_HFLAG_BDS16;
        break;
    default:
        break;
    }

    ret = csp->cr_offset + csp->cr_base;

    /* Set the tag bit in memory, if set in the register. */
    if (csp->cr_tag)
        cheri_tag_set(env, vaddr, cs, GETPC());
    else
        cheri_tag_invalidate(env, vaddr, CHERI_CAP_SIZE, GETPC());

    env->hflags = save_hflags;

    return ret;
}

#elif defined(CHERI_MAGIC128)
#ifdef CONFIG_MIPS_LOG_INSTR
/*
 * Print capability load from memory to log file.
 */
static inline void dump_cap_load(uint64_t addr, uint64_t cursor,
        uint64_t base, uint8_t tag)
{

    if (unlikely(qemu_loglevel_mask(CPU_LOG_INSTR))) {
       fprintf(qemu_logfile, "    Cap Memory Read [" TARGET_FMT_lx "] = v:%d c:"
               TARGET_FMT_lx " b:" TARGET_FMT_lx "\n", addr, tag, cursor, base);
    }
}

/*
 * Print capability store to memory to log file.
 */
static inline void dump_cap_store(uint64_t addr, uint64_t cursor,
        uint64_t base, uint8_t tag)
{

    if (unlikely(qemu_loglevel_mask(CPU_LOG_INSTR))) {
      fprintf(qemu_logfile, "    Cap Memory Write [" TARGET_FMT_lx "] = v:%d c:"
              TARGET_FMT_lx " b:" TARGET_FMT_lx "\n", addr, tag, cursor, base);
    }
}
#endif // CONFIG_MIPS_LOG_INSTR

void helper_bytes2cap_m128(CPUMIPSState *env, uint32_t cd, target_ulong base,
                           target_ulong cursor, target_ulong addr)
{
    uint64_t tps, length;
    cap_register_t *cdp = get_writable_capreg_raw(&env->active_tc, cd);
    /* fetch tps and length */
    cheri_tag_get_m128(env, addr, cd, &tps, &length, GETPC());

    cdp->cr_otype = (uint32_t)(tps >> 32);
    cdp->cr_perms = (uint32_t)((tps >> 1) & CAP_PERMS_ALL);
    cdp->cr_uperms = (uint32_t)(((tps >> 1) >> CAP_UPERMS_SHFT) &
            CAP_UPERMS_ALL);
    if (tps & 1ULL)
        cdp->cr_sealed = 1;
    else
        cdp->cr_sealed = 0;
    cdp->cr_length = length ^ -1UL;
    cdp->cr_base = base;
    cdp->cr_offset = cursor - base;
}

void helper_bytes2cap_m128_tag(CPUMIPSState *env, uint32_t cb, uint32_t cd,
                               target_ulong cursor, target_ulong addr)
{
    /* unused but needed to fetch the tag */
    uint64_t tps, length;
    // Since this is used by cl* we need to treat cb == 0 as $ddc
    const cap_register_t *cbp = get_capreg_0_is_ddc(&env->active_tc, cb);
    cap_register_t *cdp = get_writable_capreg_raw(&env->active_tc, cd);

    target_ulong tag = cheri_tag_get_m128(env, addr, cd, &tps, &length, GETPC());
    tag = clear_tag_if_no_loadcap(env, tag, cbp);
    cdp->cr_tag = tag;

#ifdef CONFIG_MIPS_LOG_INSTR
    /* Log memory read, if needed. */
    dump_cap_load(addr, cursor, cdp->cr_base, tag);
    cvtrace_dump_cap_load(&env->cvtrace, addr, cdp);
    cvtrace_dump_cap_cbl(&env->cvtrace, cdp);
#endif
}

target_ulong helper_cap2bytes_m128c(CPUMIPSState *env, uint32_t cs,
        target_ulong vaddr)
{
    const cap_register_t *csp = get_readonly_capreg(&env->active_tc, cs);
    target_ulong ret;

    ret = cap_get_cursor(csp);

    /* Log memory cap write, if needed. */
#ifdef CONFIG_MIPS_LOG_INSTR
    dump_cap_store(vaddr, ret, csp->cr_base, csp->cr_tag);
#endif
    return ret;
}

target_ulong helper_cap2bytes_m128b(CPUMIPSState *env, uint32_t cs,
        uint32_t bdoffset, target_ulong vaddr)
{
    const cap_register_t *csp = get_readonly_capreg(&env->active_tc, cs);
    target_ulong ret;
    uint64_t tps, length, perms;
    uint32_t save_hflags = env->hflags;

    /* Are we in a branch delay slot? */
    switch(bdoffset) {
    case 4:
        env->hflags |= MIPS_HFLAG_BDS32;
        break;
    case 2:
        env->hflags |= MIPS_HFLAG_BDS16;
        break;
    default:
        break;
    }

    ret = csp->cr_base;

    perms = (uint64_t)(((csp->cr_uperms & CAP_UPERMS_ALL) << CAP_UPERMS_SHFT) |
        (csp->cr_perms & CAP_PERMS_ALL));

    tps = ((uint64_t)csp->cr_otype << 32) |
        (perms << 1) | (is_cap_sealed(csp) ? 1UL : 0UL);

    length = csp->cr_length ^ -1UL;

    cheri_tag_set_m128(env, vaddr, cs, csp->cr_tag, tps, length, GETPC());

#ifdef CONFIG_MIPS_LOG_INSTR
    /* Log memory cap write, if needed. */
    cvtrace_dump_cap_store(&env->cvtrace, vaddr, csp);
    cvtrace_dump_cap_cbl(&env->cvtrace, csp);
#endif

    env->hflags = save_hflags;

    return ret;
}

#else /* ! CHERI_MAGIC128 */

#ifdef CONFIG_MIPS_LOG_INSTR
/*
 * Print capability load from memory to log file.
 */
static inline void dump_cap_load_op(uint64_t addr, uint64_t perm_type,
        uint8_t tag)
{

    if (unlikely(qemu_loglevel_mask(CPU_LOG_INSTR))) {
        fprintf(qemu_logfile, "    Cap Memory Read [" TARGET_FMT_lx
             "] = v:%d tps:" TARGET_FMT_lx "\n", addr, tag, perm_type);
    }
}

static inline void dump_cap_load_cbl(uint64_t cursor, uint64_t base,
        uint64_t length)
{

    if (unlikely(qemu_loglevel_mask(CPU_LOG_INSTR))) {
        fprintf(qemu_logfile, "    c:" TARGET_FMT_lx " b:" TARGET_FMT_lx " l:"
                TARGET_FMT_lx "\n", cursor, base, length);
    }
}

/*
 * Print capability store to memory to log file.
 */
static inline void dump_cap_store_op(uint64_t addr, uint64_t perm_type,
        uint8_t tag)
{

    if (unlikely(qemu_loglevel_mask(CPU_LOG_INSTR))) {
        fprintf(qemu_logfile, "    Cap Memory Write [" TARGET_FMT_lx
                "] = v:%d tps:" TARGET_FMT_lx "\n", addr, tag, perm_type);
    }
}

static inline void dump_cap_store_cursor(uint64_t cursor)
{

    if (unlikely(qemu_loglevel_mask(CPU_LOG_INSTR))) {
        fprintf(qemu_logfile, "    c:" TARGET_FMT_lx, cursor);
    }
}

static inline void dump_cap_store_base(uint64_t base)
{

    if (unlikely(qemu_loglevel_mask(CPU_LOG_INSTR))) {
        fprintf(qemu_logfile, " b:" TARGET_FMT_lx, base);
    }
}

static inline void dump_cap_store_length(uint64_t length)
{

    if (unlikely(qemu_loglevel_mask(CPU_LOG_INSTR))) {
        fprintf(qemu_logfile, " l:" TARGET_FMT_lx "\n", length);
    }
}

#endif // CONFIG_MIPS_LOG_INSTR

void helper_bytes2cap_op(CPUMIPSState *env, uint32_t cb, uint32_t cd, target_ulong otype,
        target_ulong addr)
{
    // Since this is used by cl* we need to treat cb == 0 as $ddc
    const cap_register_t *cbp = get_capreg_0_is_ddc(&env->active_tc, cb);
    cap_register_t *cdp = get_writable_capreg_raw(&env->active_tc, cd);
    target_ulong tag = cheri_tag_get(env, addr, cd, NULL, GETPC());
    tag = clear_tag_if_no_loadcap(env, tag, cbp);
    uint32_t perms;

    cdp->cr_tag = tag;

    cdp->cr_otype = (uint32_t)(otype >> 32);
    perms = (uint32_t)(otype >> 1);
    uint64_t store_mem_perms = tag ? CAP_PERMS_ALL : CAP_HW_PERMS_ALL_MEM;
    cdp->cr_perms = perms & store_mem_perms;
    cdp->cr_uperms = (perms >> CAP_UPERMS_SHFT) & CAP_UPERMS_ALL;
    if (otype & 1ULL)
        cdp->cr_sealed = 1;
    else
        cdp->cr_sealed = 0;

#ifdef CONFIG_MIPS_LOG_INSTR
    /* Log memory read, if needed. */
    dump_cap_load_op(addr, otype, tag);
    cvtrace_dump_cap_load(&env->cvtrace, addr, cdp);
#endif
}

void helper_bytes2cap_opll(CPUMIPSState *env, uint32_t cb, uint32_t cd, target_ulong otype,
        target_ulong addr)
{
    // Since this is used by cl* we need to treat cb == 0 as $ddc
    const cap_register_t *cbp = get_capreg_0_is_ddc(&env->active_tc, cb);
    cap_register_t *cdp = get_writable_capreg_raw(&env->active_tc, cd);
    target_ulong tag = cheri_tag_get(env, addr, cd, &env->lladdr, GETPC());
    tag = clear_tag_if_no_loadcap(env, tag, cbp);
    uint32_t perms;
    cdp->cr_tag = tag;

    cdp->cr_otype = (uint32_t)(otype >> 32);
    perms = (uint32_t)(otype >> 1);
    uint64_t store_mem_perms = tag ? CAP_PERMS_ALL : CAP_HW_PERMS_ALL_MEM;
    cdp->cr_perms = perms & store_mem_perms;
    cdp->cr_uperms = (perms >> CAP_UPERMS_SHFT) & CAP_UPERMS_ALL;
    if (otype & 1ULL)
        cdp->cr_sealed = 1;
    else
        cdp->cr_sealed = 0;

#ifdef CONFIG_MIPS_LOG_INSTR
    /* Log memory read, if needed. */
    dump_cap_load_op(addr, otype, tag);
    cvtrace_dump_cap_load(&env->cvtrace, addr, cdp);
#endif
}

target_ulong helper_cap2bytes_op(CPUMIPSState *env, uint32_t cs,
        target_ulong vaddr)
{
    const cap_register_t *csp = get_readonly_capreg(&env->active_tc, cs);
    target_ulong ret;
    uint64_t perms;

    // If the value is tagged we only store the actually available bits otherwise
    // just store back the raw bits that we originally loaded.
    uint64_t store_mem_perms = csp->cr_tag ? CAP_PERMS_ALL : CAP_HW_PERMS_ALL_MEM;

    perms = (uint64_t)(((csp->cr_uperms & CAP_UPERMS_ALL) << CAP_UPERMS_SHFT) |
        (csp->cr_perms & store_mem_perms));

    ret = ((uint64_t)csp->cr_otype << 32) |
        (perms << 1) | (is_cap_sealed(csp) ? 1UL : 0UL);

#ifdef CONFIG_MIPS_LOG_INSTR
    /* Log memory cap write, if needed. */
    dump_cap_store_op(vaddr, ret, csp->cr_tag);
    cvtrace_dump_cap_store(&env->cvtrace, vaddr, csp);
#endif

    return ret;
}

void helper_bytes2cap_cbl(CPUMIPSState *env, uint32_t cd, target_ulong cursor,
        target_ulong base, target_ulong length)
{
    cap_register_t *cdp = get_writable_capreg_raw(&env->active_tc, cd);

    length = length ^ -1UL;
    cdp->cr_length = length;
    cdp->cr_base = base;
    cdp->cr_offset = cursor - base;

#ifdef CONFIG_MIPS_LOG_INSTR
    /* Log memory reads, if needed. */
    dump_cap_load_cbl(cursor, base, length);
    cvtrace_dump_cap_cbl(&env->cvtrace, cdp);
#endif
}

#ifdef CONFIG_MIPS_LOG_INSTR
static inline void cvtrace_dump_cap_cursor(cvtrace_t *cvtrace, uint64_t cursor)
{
    if (unlikely(qemu_loglevel_mask(CPU_LOG_CVTRACE))) {
        cvtrace->val3 = tswap64(cursor);
    }
}

static inline void cvtrace_dump_cap_base(cvtrace_t *cvtrace, uint64_t base)
{
    if (unlikely(qemu_loglevel_mask(CPU_LOG_CVTRACE))) {
        cvtrace->val4 = tswap64(base);
    }
}

static inline void cvtrace_dump_cap_length(cvtrace_t *cvtrace, uint64_t length)
{
    if (unlikely(qemu_loglevel_mask(CPU_LOG_CVTRACE))) {
        cvtrace->val5 = tswap64(length);
    }
}
#endif // CONFIG_MIPS_LOG_INSTR

target_ulong helper_cap2bytes_cursor(CPUMIPSState *env, uint32_t cs,
        uint32_t bdoffset, target_ulong vaddr)
{
    const cap_register_t *csp = get_readonly_capreg(&env->active_tc, cs);
    target_ulong ret;
    uint32_t save_hflags = env->hflags;

    /* Are we in a branch delay slot? */
    switch(bdoffset) {
    case 4:
        env->hflags |= MIPS_HFLAG_BDS32;
        break;
    case 2:
        env->hflags |= MIPS_HFLAG_BDS16;
        break;
    default:
        break;
    }

    if (csp->cr_tag)
        cheri_tag_set(env, vaddr, cs, GETPC());
    else
        cheri_tag_invalidate(env, vaddr, CHERI_CAP_SIZE, GETPC());

    ret = csp->cr_offset + csp->cr_base;
#ifdef CONFIG_MIPS_LOG_INSTR
    /* Log memory cap write, if needed. */
    dump_cap_store_cursor(ret);
    cvtrace_dump_cap_cursor(&env->cvtrace, ret);
#endif

    env->hflags = save_hflags;

    return (ret);
}

target_ulong helper_cap2bytes_base(CPUMIPSState *env, uint32_t cs)
{
    const cap_register_t *csp = get_readonly_capreg(&env->active_tc, cs);

#ifdef CONFIG_MIPS_LOG_INSTR
    /* Log memory cap write, if needed. */
    dump_cap_store_base(csp->cr_base);
    cvtrace_dump_cap_base(&env->cvtrace, csp->cr_base);
#endif

    return (csp->cr_base);
}

target_ulong helper_cap2bytes_length(CPUMIPSState *env, uint32_t cs)
{
    const cap_register_t *csp = get_readonly_capreg(&env->active_tc, cs);

#ifdef CONFIG_MIPS_LOG_INSTR
    /* Log memory cap write, if needed. */
    dump_cap_store_length(csp->cr_length);
    cvtrace_dump_cap_length(&env->cvtrace, csp->cr_length);
#endif

    return (csp->cr_length ^ -1UL);
}
#endif /* ! CHERI_MAGIC128 */

void helper_ccheck_btarget(CPUMIPSState *env)
{
    // Check whether the branch target is within $pcc and if not raise an exception
    // qemu_log_mask(CPU_LOG_INSTR, "%s: env->pc=0x" TARGET_FMT_lx " hflags=0x%x, btarget=0x" TARGET_FMT_lx "\n",
    //    __func__, env->active_tc.PC, env->hflags, env->btarget);
    check_cap(env, &env->active_tc.PCC, CAP_PERM_EXECUTE, env->btarget, 0xff, 4, /*instavail=*/false);
}

void helper_ccheck_pc(CPUMIPSState *env, uint64_t pc)
{
    cap_register_t *pcc = &env->active_tc.PCC;

#ifdef CONFIG_MIPS_LOG_INSTR
    /* Print changed state before advancing to the next instruction: GPR, HI/LO, COP0. */
    const bool should_log_instr =
        qemu_loglevel_mask(CPU_LOG_CVTRACE | CPU_LOG_INSTR | CPU_LOG_USER_ONLY) || env->user_only_tracing_enabled;
    if (unlikely(should_log_instr))
        helper_dump_changed_state(env);
#endif

    /* Update statcounters icount */
    env->statcounters_icount++;

    // branch instructions have already checked the validity of the target,
    // but we still need to check if the next instruction is accessible.
    // In order to ensure that EPC is set correctly we must set the offset
    // before checking the bounds.
    pcc->cr_offset = pc - pcc->cr_base;
    check_cap(env, pcc, CAP_PERM_EXECUTE, pc, 0xff, 4, /*instavail=*/false);
    // fprintf(qemu_logfile, "PC:%016lx\n", pc);

#ifdef CONFIG_MIPS_LOG_INSTR
    // Finally, log the instruction that will be executed next
    if (unlikely(should_log_instr)) {
        helper_log_instruction(env, pc);
    }
#endif
}

target_ulong helper_ccheck_store_right(CPUMIPSState *env, target_ulong offset, uint32_t len)
{
#ifndef TARGET_WORDS_BIGENDIAN
#error "This check is only valid for big endian targets, for little endian the load/store left instructions need to be checked"
#endif
    // For swr/sdr if offset & 3/7 == 0 we store only first byte, if all low bits are set we store the full amount
    uint32_t low_bits = (uint32_t)offset & (len - 1);
    uint32_t stored_bytes = low_bits + 1;
    // From spec:
    //if BigEndianMem = 1 then
    //  pAddr <- pAddr(PSIZE-1)..3 || 000 (for ldr), 00 for lwr
    //endif
    // clear the low bits in offset to perform the length check
    target_ulong write_offset = offset & ~((target_ulong)len - 1);
    // fprintf(stderr, "%s: len=%d, offset=%zd, write_offset=%zd: will touch %d bytes\n",
    //    __func__, len, (size_t)offset, (size_t)write_offset, stored_bytes);
    // return the actual address by adding the low bits (this is expected by translate.c
    return helper_ccheck_store(env, write_offset, stored_bytes) + low_bits;
}


target_ulong helper_ccheck_load_right(CPUMIPSState *env, target_ulong offset, uint32_t len)
{
#ifndef TARGET_WORDS_BIGENDIAN
#error "This check is only valid for big endian targets, for little endian the load/store left instructions need to be checked"
#endif
    // For lwr/ldr we load all bytes if offset & 3/7 == 0 we load only the first byte, if all low bits are set we load the full amount
    uint32_t low_bits = (uint32_t)offset & (len - 1);
    uint32_t loaded_bytes = low_bits + 1;
    // From spec:
    //if BigEndianMem = 1 then
    //  pAddr <- pAddr(PSIZE-1)..3 || 000 (for ldr), 00 for lwr
    //endif
    // clear the low bits in offset to perform the length check
    target_ulong read_offset = offset & ~((target_ulong)len - 1);
    // fprintf(stderr, "%s: len=%d, offset=%zd, read_offset=%zd: will touch %d bytes\n",
    //      __func__, len, (size_t)offset, (size_t)read_offset, loaded_bytes);
    // return the actual address by adding the low bits (this is expected by translate.c
    return helper_ccheck_load(env, read_offset, loaded_bytes) + low_bits;
}

target_ulong check_ddc(CPUMIPSState *env, uint32_t perm, uint64_t ddc_offset, uint32_t len, bool instavail)
{
    const cap_register_t *ddc = &env->active_tc.CHWR.DDC;
    target_ulong addr = ddc_offset + cap_get_cursor(ddc);
    // fprintf(qemu_logfile, "ST(%u):%016lx\n", len, addr);
    // FIXME: should regnum be 32 instead?
    check_cap(env, ddc, perm, addr, /*regnum=*/0, len, instavail);
    return addr;
}


target_ulong helper_ccheck_store(CPUMIPSState *env, target_ulong offset, uint32_t len)
{
    return check_ddc(env, CAP_PERM_STORE, offset, len, /*instavail=*/true);
}

target_ulong helper_ccheck_load(CPUMIPSState *env, target_ulong offset, uint32_t len)
{
    return check_ddc(env, CAP_PERM_LOAD, offset, len, /*instavail=*/true);
}

void helper_cinvalidate_tag_left_right(CPUMIPSState *env, target_ulong addr, uint32_t len,
    uint32_t opc, target_ulong value)
{
#ifdef CONFIG_MIPS_LOG_INSTR
    /* Log write, if enabled */
    dump_store(env, opc, addr, value);
#endif
    // swr/sdr/swl/sdl will never invalidate more than one capability
    cheri_tag_invalidate(env, addr, 1, GETPC());
}

void helper_cinvalidate_tag(CPUMIPSState *env, target_ulong addr, uint32_t len,
    uint32_t opc, target_ulong value)
{
#ifdef CONFIG_MIPS_LOG_INSTR
    /* Log write, if enabled. */
    dump_store(env, opc, addr, value);
#endif

    cheri_tag_invalidate(env, addr, len, GETPC());
}

void helper_cinvalidate_tag32(CPUMIPSState *env, target_ulong addr, uint32_t len,
    uint32_t opc, uint32_t value)
{
#ifdef CONFIG_MIPS_LOG_INSTR
    /* Log write, if enabled. */
    dump_store(env, opc, addr, (target_ulong)value);
#endif

    cheri_tag_invalidate(env, addr, len, GETPC());
}


static const char *cheri_cap_reg[] = {
  "DDC",  "",   "",      "",     "",    "",    "",    "",  /* C00 - C07 */
     "",  "",   "",      "",     "",    "",    "",    "",  /* C08 - C15 */
     "",  "",   "",      "",     "",    "",    "",    "",  /* C16 - C23 */
  "RCC",  "", "IDC", "KR1C", "KR2C", "KCC", "KDC", "EPCC"  /* C24 - C31 */
};


static void cheri_dump_creg(const cap_register_t *crp, const char *name,
        const char *alias, FILE *f, fprintf_function cpu_fprintf)
{

#if 0
    if (crp->cr_tag) {
        cpu_fprintf(f, "%s: bas=%016lx len=%016lx cur=%016lx\n", name,
            // crp->cr_base, crp->cr_length, crp->cr_cursor);
            crp->cr_base, crp->cr_length, cap_get_cursor(crp));
        cpu_fprintf(f, "%-4s off=%016lx otype=%06x seal=%d "
		    "perms=%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",
            // alias, (crp->cr_cursor - crp->cr_base), crp->cr_otype,
            alias, crp->cr_offset, crp->cr_otype,
            is_cap_sealed(crp) ? 1 : 0,
            (crp->cr_perms & CAP_PERM_GLOBAL) ? 'G' : '-',
            (crp->cr_perms & CAP_PERM_EXECUTE) ? 'e' : '-',
            (crp->cr_perms & CAP_PERM_LOAD) ? 'l' : '-',
            (crp->cr_perms & CAP_PERM_STORE) ? 's' : '-',
            (crp->cr_perms & CAP_PERM_LOAD_CAP) ? 'L' : '-',
            (crp->cr_perms & CAP_PERM_STORE_CAP) ? 'S' : '-',
            (crp->cr_perms & CAP_PERM_STORE_LOCAL) ? '&' : '-',
            (crp->cr_perms & CAP_PERM_SEAL) ? '$' : '-',
            (crp->cr_perms & CAP_RESERVED1) ? 'R' : '-',
            (crp->cr_perms & CAP_RESERVED2) ? 'R' : '-',
            (crp->cr_perms & CAP_ACCESS_SYS_REGS) ? 'r' : '-');
    } else {
        cpu_fprintf(f, "%s: (not valid - tag not set)\n");
        cpu_fprintf(f, "%-4s\n", alias);
    }
#else
    /*
    cpu_fprintf(f, "DEBUG %s: v:%d s:%d p:%08x b:%016lx l:%016lx o:%016lx t:%08x\n",
            name, crp->cr_tag, is_cap_sealed(crp) ? 1 : 0,
            ((crp->cr_uperms & CAP_UPERMS_ALL) << CAP_UPERMS_SHFT) |
            (crp->cr_perms & CAP_PERMS_ALL), crp->cr_base, crp->cr_length,
            crp->cr_offset, crp->cr_otype);
    */

/* #define OLD_DEBUG_CAP */

#ifdef OLD_DEBUG_CAP
    cpu_fprintf(f, "DEBUG CAP %s u:%d perms:0x%08x type:0x%06x "
            "offset:0x%016lx base:0x%016lx length:0x%016lx\n",
            name, is_cap_sealed(crp),
#else
    cpu_fprintf(f, "DEBUG CAP %s t:%d s:%d perms:0x%08x type:0x%016" PRIx64 " "
            "offset:0x%016lx base:0x%016lx length:0x%016lx\n",
            name, crp->cr_tag, is_cap_sealed(crp),
#endif
            ((crp->cr_uperms & CAP_UPERMS_ALL) << CAP_UPERMS_SHFT) |
            (crp->cr_perms & CAP_PERMS_ALL),
            (uint64_t)(is_cap_sealed(crp) ? crp->cr_otype : UINT64_MAX), /* testsuite wants -1 for unsealed */
            crp->cr_offset, crp->cr_base, crp->cr_length);
#endif
}

static void cheri_dump_state(CPUState *cs, FILE *f, fprintf_function cpu_fprintf,
        int flags)
{
    MIPSCPU *cpu = MIPS_CPU(cs);
    CPUMIPSState *env = &cpu->env;
    int i;
    char name[8];

    cpu_fprintf(f, "DEBUG CAP COREID 0\n");
    cheri_dump_creg(&env->active_tc.PCC, "PCC", "", f, cpu_fprintf);
    for (i = 0; i < 32; i++) {
        // snprintf(name, sizeof(name), "C%02d", i);
        snprintf(name, sizeof(name), "REG %02d", i);
        cheri_dump_creg(&env->active_tc._CGPR[i], name, cheri_cap_reg[i], f,
                cpu_fprintf);
    }
    cheri_dump_creg(&env->active_tc.CHWR.DDC,        "HWREG 00 (DDC)", "", f, cpu_fprintf);
    cheri_dump_creg(&env->active_tc.CHWR.UserTlsCap, "HWREG 01 (CTLSU)", "", f, cpu_fprintf);
    cheri_dump_creg(&env->active_tc.CHWR.PrivTlsCap, "HWREG 08 (CTLSP)", "", f, cpu_fprintf);
    cheri_dump_creg(&env->active_tc.CHWR.KR1C,       "HWREG 22 (KR1C)", "", f, cpu_fprintf);
    cheri_dump_creg(&env->active_tc.CHWR.KR2C,       "HWREG 23 (KR2C)", "", f, cpu_fprintf);
    // TODO: dump this once we have a number: cheri_dump_creg(&env->active_tc.CHWR.ErrorEPCC,  "HWREG 28 (ErrorEPC)", "", f, cpu_fprintf);
    cheri_dump_creg(&env->active_tc.CHWR.KCC,        "HWREG 29 (KCC)", "", f, cpu_fprintf);
    cheri_dump_creg(&env->active_tc.CHWR.KDC,        "HWREG 30 (KDC)", "", f, cpu_fprintf);
    cheri_dump_creg(&env->active_tc.CHWR.EPCC,       "HWREG 31 (EPCC)", "", f, cpu_fprintf);

    cpu_fprintf(f, "\n");
}

void helper_mtc2_dumpcstate(CPUMIPSState *env, target_ulong arg1)
{
    cheri_dump_state(CPU(mips_env_get_cpu(env)),
            (qemu_logfile == NULL) ? stderr : qemu_logfile,
            fprintf, CPU_DUMP_CODE);
}
