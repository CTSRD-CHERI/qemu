/*
 *  MIPS emulation helpers for qemu.
 *
 *  Copyright (c) 2004-2005 Jocelyn Mayer
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */
#include "qemu/osdep.h"
#include "qemu/main-loop.h"
#include "cpu.h"
#include "internal.h"
#include "qemu/host-utils.h"
#include "qemu/error-report.h"
#include "exec/helper-proto.h"
#include "exec/exec-all.h"
#include "exec/cpu_ldst.h"
#include "sysemu/kvm.h"
#ifdef TARGET_CHERI
#include "disas/disas.h"
#include "disas/bfd.h"
#endif

// These constants are for a cheri concentrate format for 128. They give either 22 bits or 20 bits of precision depending on object size
// They give 20 bits for types

#define CC_L_IE_OFF 45
#define CC_L_LH_OFF 44
#define CC_L_B_OFF  0
#define CC_L_T_OFF  23
#define CC_L_S_OFF  46

#define CC_L_OHI_OFF 32
#define CC_L_OLO_OFF 11
#define CC_L_TYPES   24


#define CC_L_BWIDTH 23
#define CC_L_SEALED_BWIDTH (CC_L_BWIDTH - (CC_L_TYPES/2))
#define CC_L_EWIDTH 7
#define CC_L_LOWWIDTH (CC_L_EWIDTH >> 1)
#define CC_L_LOWMASK ((1 << CC_L_LOWWIDTH) - 1)

#define CAP_MAX_OTYPE ((1 << CC_L_TYPES) - 1)
/* Whatever NULL would encode to is this constant. We mask on store/load so this is invisibly keeps null 0 whatever we choose it to be */

#define NULL_XOR_MASK 0x200001000005

/*****************************************************************************/
/* Exceptions processing helpers */

void helper_raise_exception_err(CPUMIPSState *env, uint32_t exception,
                                int error_code)
{
    do_raise_exception_err(env, exception, error_code, 0);
}

void helper_raise_exception(CPUMIPSState *env, uint32_t exception)
{
    do_raise_exception(env, exception, GETPC());
}

void helper_raise_exception_debug(CPUMIPSState *env)
{
    do_raise_exception(env, EXCP_DEBUG, 0);
}

static void raise_exception(CPUMIPSState *env, uint32_t exception)
{
    do_raise_exception(env, exception, 0);
}

#if defined(TARGET_CHERI)
const char *causestr[] = {
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

/*
 * See section 4.4 of the CHERI Architecture.
 */
static inline QEMU_NORETURN void do_raise_c2_exception(CPUMIPSState *env,
        uint16_t cause, uint16_t reg)
{
    uint64_t pc = env->active_tc.PCC.cr_offset + env->active_tc.PCC.cr_base;

    /* fprintf(stderr, "C2 EXCEPTION: cause=%d(%s) reg=%d PCC=0x%016lx + 0x%016lx "
        "-> 0x%016lx PC=0x%016lx\n", cause, causestr[cause], reg,
         env->active_tc.PCC.cr_base, env->active_tc.PCC.cr_offset, pc,
         env->active_tc.PC); */
    qemu_log_mask(CPU_LOG_INSTR | CPU_LOG_INT, "C2 EXCEPTION: cause=%d(%s)"
       " reg=%d PCC=0x%016" PRIx64 " + 0x%016" PRIx64 " -> 0x" TARGET_FMT_lx
       " PC=0x" TARGET_FMT_lx "\n",
       cause, causestr[cause], reg, env->active_tc.PCC.cr_base,
       env->active_tc.PCC.cr_offset, pc, env->active_tc.PC);
    cpu_mips_store_capcause(env, reg, cause);
    env->active_tc.PC = pc;
    env->CP0_BadVAddr = pc;
    do_raise_exception(env, EXCP_C2E, pc);
}

static inline void do_raise_c2_exception_noreg(CPUMIPSState *env, uint16_t cause)
{
    do_raise_c2_exception(env, cause, 0xff);
}

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
                      howmuch, name, cr->cr_tag, cr->cr_sealed ? 1 : 0,
                      (((cr->cr_uperms & CAP_UPERMS_ALL) << CAP_UPERMS_MEM_SHFT) | (cr->cr_perms & CAP_PERMS_ALL)),
                      cr->cr_base, cr->cr_length, (int64_t)cr->cr_offset,
                      env->active_tc.PCC.cr_base + env->active_tc.PCC.cr_offset,
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
        #operation, env->active_tc.PCC.cr_base + env->active_tc.PCC.cr_offset, (unsigned)(env->CP0_EntryHi & 0xFF)); \
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
                (double)(100 * total_out_of_bounds) / (double)total);
    cpu_fprintf(f, "Total out of bounds %ss (excluding one past the end): %" PRIu64 " (%f%%)\n",
                name, total_out_of_bounds - after_bounds[0],
                (double)(100 * (total_out_of_bounds - after_bounds[0])) / (double)total);
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
#endif /* TARGET_CHERI */

#if defined(CONFIG_USER_ONLY)
#define HELPER_LD(name, insn, type)                                     \
static inline type do_##name(CPUMIPSState *env, target_ulong addr,      \
                             int mem_idx, uintptr_t retaddr)            \
{                                                                       \
    return (type) cpu_##insn##_data_ra(env, addr, retaddr);             \
}
#else
#define HELPER_LD(name, insn, type)                                     \
static inline type do_##name(CPUMIPSState *env, target_ulong addr,      \
                             int mem_idx, uintptr_t retaddr)            \
{                                                                       \
    switch (mem_idx)                                                    \
    {                                                                   \
    case 0: return (type) cpu_##insn##_kernel_ra(env, addr, retaddr);   \
    case 1: return (type) cpu_##insn##_super_ra(env, addr, retaddr);    \
    default:                                                            \
    case 2: return (type) cpu_##insn##_user_ra(env, addr, retaddr);     \
    case 3: return (type) cpu_##insn##_error_ra(env, addr, retaddr);    \
    }                                                                   \
}
#endif
HELPER_LD(lw, ldl, int32_t)
#if defined(TARGET_MIPS64)
HELPER_LD(ld, ldq, int64_t)
#endif
#undef HELPER_LD

#if defined(CONFIG_USER_ONLY)
#define HELPER_ST(name, insn, type)                                     \
static inline void do_##name(CPUMIPSState *env, target_ulong addr,      \
                             type val, int mem_idx, uintptr_t retaddr)  \
{                                                                       \
    cpu_##insn##_data_ra(env, addr, val, retaddr);                      \
}
#else
#define HELPER_ST(name, insn, type)                                     \
static inline void do_##name(CPUMIPSState *env, target_ulong addr,      \
                             type val, int mem_idx, uintptr_t retaddr)  \
{                                                                       \
    switch (mem_idx)                                                    \
    {                                                                   \
    case 0: cpu_##insn##_kernel_ra(env, addr, val, retaddr); break;     \
    case 1: cpu_##insn##_super_ra(env, addr, val, retaddr); break;      \
    default:                                                            \
    case 2: cpu_##insn##_user_ra(env, addr, val, retaddr); break;       \
    case 3:                                                             \
        cpu_##insn##_error_ra(env, addr, val, retaddr);                 \
        break;                                                          \
    }                                                                   \
}
#endif
HELPER_ST(sb, stb, uint8_t)
HELPER_ST(sw, stl, uint32_t)
#if defined(TARGET_MIPS64)
HELPER_ST(sd, stq, uint64_t)
#endif
#undef HELPER_ST

/* 64 bits arithmetic for 32 bits hosts */
static inline uint64_t get_HILO(CPUMIPSState *env)
{
    return ((uint64_t)(env->active_tc.HI[0]) << 32) | (uint32_t)env->active_tc.LO[0];
}

static inline target_ulong set_HIT0_LO(CPUMIPSState *env, uint64_t HILO)
{
    env->active_tc.LO[0] = (int32_t)(HILO & 0xFFFFFFFF);
    return env->active_tc.HI[0] = (int32_t)(HILO >> 32);
}

static inline target_ulong set_HI_LOT0(CPUMIPSState *env, uint64_t HILO)
{
    target_ulong tmp = env->active_tc.LO[0] = (int32_t)(HILO & 0xFFFFFFFF);
    env->active_tc.HI[0] = (int32_t)(HILO >> 32);
    return tmp;
}

/* Multiplication variants of the vr54xx. */
target_ulong helper_muls(CPUMIPSState *env, target_ulong arg1,
                         target_ulong arg2)
{
    return set_HI_LOT0(env, 0 - ((int64_t)(int32_t)arg1 *
                                 (int64_t)(int32_t)arg2));
}

target_ulong helper_mulsu(CPUMIPSState *env, target_ulong arg1,
                          target_ulong arg2)
{
    return set_HI_LOT0(env, 0 - (uint64_t)(uint32_t)arg1 *
                       (uint64_t)(uint32_t)arg2);
}

target_ulong helper_macc(CPUMIPSState *env, target_ulong arg1,
                         target_ulong arg2)
{
    return set_HI_LOT0(env, (int64_t)get_HILO(env) + (int64_t)(int32_t)arg1 *
                       (int64_t)(int32_t)arg2);
}

target_ulong helper_macchi(CPUMIPSState *env, target_ulong arg1,
                           target_ulong arg2)
{
    return set_HIT0_LO(env, (int64_t)get_HILO(env) + (int64_t)(int32_t)arg1 *
                       (int64_t)(int32_t)arg2);
}

target_ulong helper_maccu(CPUMIPSState *env, target_ulong arg1,
                          target_ulong arg2)
{
    return set_HI_LOT0(env, (uint64_t)get_HILO(env) +
                       (uint64_t)(uint32_t)arg1 * (uint64_t)(uint32_t)arg2);
}

target_ulong helper_macchiu(CPUMIPSState *env, target_ulong arg1,
                            target_ulong arg2)
{
    return set_HIT0_LO(env, (uint64_t)get_HILO(env) +
                       (uint64_t)(uint32_t)arg1 * (uint64_t)(uint32_t)arg2);
}

target_ulong helper_msac(CPUMIPSState *env, target_ulong arg1,
                         target_ulong arg2)
{
    return set_HI_LOT0(env, (int64_t)get_HILO(env) - (int64_t)(int32_t)arg1 *
                       (int64_t)(int32_t)arg2);
}

target_ulong helper_msachi(CPUMIPSState *env, target_ulong arg1,
                           target_ulong arg2)
{
    return set_HIT0_LO(env, (int64_t)get_HILO(env) - (int64_t)(int32_t)arg1 *
                       (int64_t)(int32_t)arg2);
}

target_ulong helper_msacu(CPUMIPSState *env, target_ulong arg1,
                          target_ulong arg2)
{
    return set_HI_LOT0(env, (uint64_t)get_HILO(env) -
                       (uint64_t)(uint32_t)arg1 * (uint64_t)(uint32_t)arg2);
}

target_ulong helper_msachiu(CPUMIPSState *env, target_ulong arg1,
                            target_ulong arg2)
{
    return set_HIT0_LO(env, (uint64_t)get_HILO(env) -
                       (uint64_t)(uint32_t)arg1 * (uint64_t)(uint32_t)arg2);
}

target_ulong helper_mulhi(CPUMIPSState *env, target_ulong arg1,
                          target_ulong arg2)
{
    return set_HIT0_LO(env, (int64_t)(int32_t)arg1 * (int64_t)(int32_t)arg2);
}

target_ulong helper_mulhiu(CPUMIPSState *env, target_ulong arg1,
                           target_ulong arg2)
{
    return set_HIT0_LO(env, (uint64_t)(uint32_t)arg1 *
                       (uint64_t)(uint32_t)arg2);
}

target_ulong helper_mulshi(CPUMIPSState *env, target_ulong arg1,
                           target_ulong arg2)
{
    return set_HIT0_LO(env, 0 - (int64_t)(int32_t)arg1 *
                       (int64_t)(int32_t)arg2);
}

target_ulong helper_mulshiu(CPUMIPSState *env, target_ulong arg1,
                            target_ulong arg2)
{
    return set_HIT0_LO(env, 0 - (uint64_t)(uint32_t)arg1 *
                       (uint64_t)(uint32_t)arg2);
}

static inline target_ulong bitswap(target_ulong v)
{
    v = ((v >> 1) & (target_ulong)0x5555555555555555ULL) |
              ((v & (target_ulong)0x5555555555555555ULL) << 1);
    v = ((v >> 2) & (target_ulong)0x3333333333333333ULL) |
              ((v & (target_ulong)0x3333333333333333ULL) << 2);
    v = ((v >> 4) & (target_ulong)0x0F0F0F0F0F0F0F0FULL) |
              ((v & (target_ulong)0x0F0F0F0F0F0F0F0FULL) << 4);
    return v;
}

#ifdef TARGET_MIPS64
target_ulong helper_dbitswap(target_ulong rt)
{
    return bitswap(rt);
}
#endif

target_ulong helper_bitswap(target_ulong rt)
{
    return (int32_t)bitswap(rt);
}

#ifndef CONFIG_USER_ONLY

static inline hwaddr do_translate_address(CPUMIPSState *env,
                                                      target_ulong address,
                                                      int rw, uintptr_t retaddr)
{
    hwaddr lladdr;
    CPUState *cs = CPU(mips_env_get_cpu(env));

    lladdr = cpu_mips_translate_address(env, address, rw);

    if (lladdr == -1LL) {
        cpu_loop_exit_restore(cs, retaddr);
    } else {
        return lladdr;
    }
}

#define HELPER_LD_ATOMIC(name, insn, almask)                                  \
target_ulong helper_##name(CPUMIPSState *env, target_ulong arg, int mem_idx)  \
{                                                                             \
    if (arg & almask) {                                                       \
        env->CP0_BadVAddr = arg;                                              \
        do_raise_exception(env, EXCP_AdEL, GETPC());                          \
    }                                                                         \
    env->lladdr = do_translate_address(env, arg, 0, GETPC());                 \
    env->llval = do_##insn(env, arg, mem_idx, GETPC());                       \
    return env->llval;                                                        \
}
HELPER_LD_ATOMIC(ll, lw, 0x3)
#ifdef TARGET_MIPS64
HELPER_LD_ATOMIC(lld, ld, 0x7)
#endif
#undef HELPER_LD_ATOMIC

#define HELPER_ST_ATOMIC(name, ld_insn, st_insn, almask)                      \
target_ulong helper_##name(CPUMIPSState *env, target_ulong arg1,              \
                           target_ulong arg2, int mem_idx)                    \
{                                                                             \
    target_long tmp;                                                          \
                                                                              \
    if (arg2 & almask) {                                                      \
        env->CP0_BadVAddr = arg2;                                             \
        do_raise_exception(env, EXCP_AdES, GETPC());                          \
    }                                                                         \
    if (do_translate_address(env, arg2, 1, GETPC()) == env->lladdr) {         \
        tmp = do_##ld_insn(env, arg2, mem_idx, GETPC());                      \
        if (tmp == env->llval) {                                              \
            do_##st_insn(env, arg2, arg1, mem_idx, GETPC());                  \
            return 1;                                                         \
        }                                                                     \
    }                                                                         \
    return 0;                                                                 \
}
HELPER_ST_ATOMIC(sc, lw, sw, 0x3)
#ifdef TARGET_MIPS64
HELPER_ST_ATOMIC(scd, ld, sd, 0x7)
#endif
#undef HELPER_ST_ATOMIC
#endif

#ifdef TARGET_WORDS_BIGENDIAN
#define GET_LMASK(v) ((v) & 3)
#define GET_OFFSET(addr, offset) (addr + (offset))
#else
#define GET_LMASK(v) (((v) & 3) ^ 3)
#define GET_OFFSET(addr, offset) (addr - (offset))
#endif

void helper_swl(CPUMIPSState *env, target_ulong arg1, target_ulong arg2,
                int mem_idx)
{
    do_sb(env, arg2, (uint8_t)(arg1 >> 24), mem_idx, GETPC());

    if (GET_LMASK(arg2) <= 2) {
        do_sb(env, GET_OFFSET(arg2, 1), (uint8_t)(arg1 >> 16), mem_idx,
              GETPC());
    }

    if (GET_LMASK(arg2) <= 1) {
        do_sb(env, GET_OFFSET(arg2, 2), (uint8_t)(arg1 >> 8), mem_idx,
              GETPC());
    }

    if (GET_LMASK(arg2) == 0) {
        do_sb(env, GET_OFFSET(arg2, 3), (uint8_t)arg1, mem_idx,
              GETPC());
    }
}

void helper_swr(CPUMIPSState *env, target_ulong arg1, target_ulong arg2,
                int mem_idx)
{
    do_sb(env, arg2, (uint8_t)arg1, mem_idx, GETPC());

    if (GET_LMASK(arg2) >= 1) {
        do_sb(env, GET_OFFSET(arg2, -1), (uint8_t)(arg1 >> 8), mem_idx,
              GETPC());
    }

    if (GET_LMASK(arg2) >= 2) {
        do_sb(env, GET_OFFSET(arg2, -2), (uint8_t)(arg1 >> 16), mem_idx,
              GETPC());
    }

    if (GET_LMASK(arg2) == 3) {
        do_sb(env, GET_OFFSET(arg2, -3), (uint8_t)(arg1 >> 24), mem_idx,
              GETPC());
    }
}

#if defined(TARGET_MIPS64)
/* "half" load and stores.  We must do the memory access inline,
   or fault handling won't work.  */

#ifdef TARGET_WORDS_BIGENDIAN
#define GET_LMASK64(v) ((v) & 7)
#else
#define GET_LMASK64(v) (((v) & 7) ^ 7)
#endif

void helper_sdl(CPUMIPSState *env, target_ulong arg1, target_ulong arg2,
                int mem_idx)
{
    do_sb(env, arg2, (uint8_t)(arg1 >> 56), mem_idx, GETPC());

    if (GET_LMASK64(arg2) <= 6) {
        do_sb(env, GET_OFFSET(arg2, 1), (uint8_t)(arg1 >> 48), mem_idx,
              GETPC());
    }

    if (GET_LMASK64(arg2) <= 5) {
        do_sb(env, GET_OFFSET(arg2, 2), (uint8_t)(arg1 >> 40), mem_idx,
              GETPC());
    }

    if (GET_LMASK64(arg2) <= 4) {
        do_sb(env, GET_OFFSET(arg2, 3), (uint8_t)(arg1 >> 32), mem_idx,
              GETPC());
    }

    if (GET_LMASK64(arg2) <= 3) {
        do_sb(env, GET_OFFSET(arg2, 4), (uint8_t)(arg1 >> 24), mem_idx,
              GETPC());
    }

    if (GET_LMASK64(arg2) <= 2) {
        do_sb(env, GET_OFFSET(arg2, 5), (uint8_t)(arg1 >> 16), mem_idx,
              GETPC());
    }

    if (GET_LMASK64(arg2) <= 1) {
        do_sb(env, GET_OFFSET(arg2, 6), (uint8_t)(arg1 >> 8), mem_idx,
              GETPC());
    }

    if (GET_LMASK64(arg2) <= 0) {
        do_sb(env, GET_OFFSET(arg2, 7), (uint8_t)arg1, mem_idx,
              GETPC());
    }
}

void helper_sdr(CPUMIPSState *env, target_ulong arg1, target_ulong arg2,
                int mem_idx)
{
    do_sb(env, arg2, (uint8_t)arg1, mem_idx, GETPC());

    if (GET_LMASK64(arg2) >= 1) {
        do_sb(env, GET_OFFSET(arg2, -1), (uint8_t)(arg1 >> 8), mem_idx,
              GETPC());
    }

    if (GET_LMASK64(arg2) >= 2) {
        do_sb(env, GET_OFFSET(arg2, -2), (uint8_t)(arg1 >> 16), mem_idx,
              GETPC());
    }

    if (GET_LMASK64(arg2) >= 3) {
        do_sb(env, GET_OFFSET(arg2, -3), (uint8_t)(arg1 >> 24), mem_idx,
              GETPC());
    }

    if (GET_LMASK64(arg2) >= 4) {
        do_sb(env, GET_OFFSET(arg2, -4), (uint8_t)(arg1 >> 32), mem_idx,
              GETPC());
    }

    if (GET_LMASK64(arg2) >= 5) {
        do_sb(env, GET_OFFSET(arg2, -5), (uint8_t)(arg1 >> 40), mem_idx,
              GETPC());
    }

    if (GET_LMASK64(arg2) >= 6) {
        do_sb(env, GET_OFFSET(arg2, -6), (uint8_t)(arg1 >> 48), mem_idx,
              GETPC());
    }

    if (GET_LMASK64(arg2) == 7) {
        do_sb(env, GET_OFFSET(arg2, -7), (uint8_t)(arg1 >> 56), mem_idx,
              GETPC());
    }
}
#endif /* TARGET_MIPS64 */

static const int multiple_regs[] = { 16, 17, 18, 19, 20, 21, 22, 23, 30 };

void helper_lwm(CPUMIPSState *env, target_ulong addr, target_ulong reglist,
                uint32_t mem_idx)
{
    target_ulong base_reglist = reglist & 0xf;
    target_ulong do_r31 = reglist & 0x10;

    if (base_reglist > 0 && base_reglist <= ARRAY_SIZE (multiple_regs)) {
        target_ulong i;

        for (i = 0; i < base_reglist; i++) {
            env->active_tc.gpr[multiple_regs[i]] =
                (target_long)do_lw(env, addr, mem_idx, GETPC());
            addr += 4;
        }
    }

    if (do_r31) {
        env->active_tc.gpr[31] = (target_long)do_lw(env, addr, mem_idx,
                                                    GETPC());
    }
}

void helper_swm(CPUMIPSState *env, target_ulong addr, target_ulong reglist,
                uint32_t mem_idx)
{
    target_ulong base_reglist = reglist & 0xf;
    target_ulong do_r31 = reglist & 0x10;

    if (base_reglist > 0 && base_reglist <= ARRAY_SIZE (multiple_regs)) {
        target_ulong i;

        for (i = 0; i < base_reglist; i++) {
            do_sw(env, addr, env->active_tc.gpr[multiple_regs[i]], mem_idx,
                  GETPC());
            addr += 4;
        }
    }

    if (do_r31) {
        do_sw(env, addr, env->active_tc.gpr[31], mem_idx, GETPC());
    }
}

#if defined(TARGET_MIPS64)
void helper_ldm(CPUMIPSState *env, target_ulong addr, target_ulong reglist,
                uint32_t mem_idx)
{
    target_ulong base_reglist = reglist & 0xf;
    target_ulong do_r31 = reglist & 0x10;

    if (base_reglist > 0 && base_reglist <= ARRAY_SIZE (multiple_regs)) {
        target_ulong i;

        for (i = 0; i < base_reglist; i++) {
            env->active_tc.gpr[multiple_regs[i]] = do_ld(env, addr, mem_idx,
                                                         GETPC());
            addr += 8;
        }
    }

    if (do_r31) {
        env->active_tc.gpr[31] = do_ld(env, addr, mem_idx, GETPC());
    }
}

void helper_sdm(CPUMIPSState *env, target_ulong addr, target_ulong reglist,
                uint32_t mem_idx)
{
    target_ulong base_reglist = reglist & 0xf;
    target_ulong do_r31 = reglist & 0x10;

    if (base_reglist > 0 && base_reglist <= ARRAY_SIZE (multiple_regs)) {
        target_ulong i;

        for (i = 0; i < base_reglist; i++) {
            do_sd(env, addr, env->active_tc.gpr[multiple_regs[i]], mem_idx,
                  GETPC());
            addr += 8;
        }
    }

    if (do_r31) {
        do_sd(env, addr, env->active_tc.gpr[31], mem_idx, GETPC());
    }
}
#endif

#ifndef CONFIG_USER_ONLY
/* SMP helpers.  */
static bool mips_vpe_is_wfi(MIPSCPU *c)
{
    CPUState *cpu = CPU(c);
    CPUMIPSState *env = &c->env;

    /* If the VPE is halted but otherwise active, it means it's waiting for
       an interrupt.  */
    return cpu->halted && mips_vpe_active(env);
}

static bool mips_vp_is_wfi(MIPSCPU *c)
{
    CPUState *cpu = CPU(c);
    CPUMIPSState *env = &c->env;

    return cpu->halted && mips_vp_active(env);
}

static inline void mips_vpe_wake(MIPSCPU *c)
{
    /* Don't set ->halted = 0 directly, let it be done via cpu_has_work
       because there might be other conditions that state that c should
       be sleeping.  */
    cpu_interrupt(CPU(c), CPU_INTERRUPT_WAKE);
}

static inline void mips_vpe_sleep(MIPSCPU *cpu)
{
    CPUState *cs = CPU(cpu);

    /* The VPE was shut off, really go to bed.
       Reset any old _WAKE requests.  */
    cs->halted = 1;
    cpu_reset_interrupt(cs, CPU_INTERRUPT_WAKE);
}

static inline void mips_tc_wake(MIPSCPU *cpu, int tc)
{
    CPUMIPSState *c = &cpu->env;

    /* FIXME: TC reschedule.  */
    if (mips_vpe_active(c) && !mips_vpe_is_wfi(cpu)) {
        mips_vpe_wake(cpu);
    }
}

static inline void mips_tc_sleep(MIPSCPU *cpu, int tc)
{
    CPUMIPSState *c = &cpu->env;

    /* FIXME: TC reschedule.  */
    if (!mips_vpe_active(c)) {
        mips_vpe_sleep(cpu);
    }
}

/**
 * mips_cpu_map_tc:
 * @env: CPU from which mapping is performed.
 * @tc: Should point to an int with the value of the global TC index.
 *
 * This function will transform @tc into a local index within the
 * returned #CPUMIPSState.
 */
/* FIXME: This code assumes that all VPEs have the same number of TCs,
          which depends on runtime setup. Can probably be fixed by
          walking the list of CPUMIPSStates.  */
static CPUMIPSState *mips_cpu_map_tc(CPUMIPSState *env, int *tc)
{
    MIPSCPU *cpu;
    CPUState *cs;
    CPUState *other_cs;
    int vpe_idx;
    int tc_idx = *tc;

    if (!(env->CP0_VPEConf0 & (1 << CP0VPEC0_MVP))) {
        /* Not allowed to address other CPUs.  */
        *tc = env->current_tc;
        return env;
    }

    cs = CPU(mips_env_get_cpu(env));
    vpe_idx = tc_idx / cs->nr_threads;
    *tc = tc_idx % cs->nr_threads;
    other_cs = qemu_get_cpu(vpe_idx);
    if (other_cs == NULL) {
        return env;
    }
    cpu = MIPS_CPU(other_cs);
    return &cpu->env;
}

/* The per VPE CP0_Status register shares some fields with the per TC
   CP0_TCStatus registers. These fields are wired to the same registers,
   so changes to either of them should be reflected on both registers.

   Also, EntryHi shares the bottom 8 bit ASID with TCStauts.

   These helper call synchronizes the regs for a given cpu.  */

/* Called for updates to CP0_Status.  Defined in "cpu.h" for gdbstub.c.  */
/* static inline void sync_c0_status(CPUMIPSState *env, CPUMIPSState *cpu,
                                     int tc);  */

/* Called for updates to CP0_TCStatus.  */
static void sync_c0_tcstatus(CPUMIPSState *cpu, int tc,
                             target_ulong v)
{
    uint32_t status;
    uint32_t tcu, tmx, tasid, tksu;
    uint32_t mask = ((1U << CP0St_CU3)
                       | (1 << CP0St_CU2)
                       | (1 << CP0St_CU1)
                       | (1 << CP0St_CU0)
                       | (1 << CP0St_MX)
                       | (3 << CP0St_KSU));

    tcu = (v >> CP0TCSt_TCU0) & 0xf;
    tmx = (v >> CP0TCSt_TMX) & 0x1;
    tasid = v & cpu->CP0_EntryHi_ASID_mask;
    tksu = (v >> CP0TCSt_TKSU) & 0x3;

    status = tcu << CP0St_CU0;
    status |= tmx << CP0St_MX;
    status |= tksu << CP0St_KSU;

    cpu->CP0_Status &= ~mask;
    cpu->CP0_Status |= status;

    /* Sync the TASID with EntryHi.  */
    cpu->CP0_EntryHi &= ~cpu->CP0_EntryHi_ASID_mask;
    cpu->CP0_EntryHi |= tasid;

    compute_hflags(cpu);
}

/* Called for updates to CP0_EntryHi.  */
static void sync_c0_entryhi(CPUMIPSState *cpu, int tc)
{
    int32_t *tcst;
    uint32_t asid, v = cpu->CP0_EntryHi;

    asid = v & cpu->CP0_EntryHi_ASID_mask;

    if (tc == cpu->current_tc) {
        tcst = &cpu->active_tc.CP0_TCStatus;
    } else {
        tcst = &cpu->tcs[tc].CP0_TCStatus;
    }

    *tcst &= ~cpu->CP0_EntryHi_ASID_mask;
    *tcst |= asid;
}

/* CP0 helpers */
target_ulong helper_mfc0_mvpcontrol(CPUMIPSState *env)
{
    return env->mvp->CP0_MVPControl;
}

target_ulong helper_mfc0_mvpconf0(CPUMIPSState *env)
{
    return env->mvp->CP0_MVPConf0;
}

target_ulong helper_mfc0_mvpconf1(CPUMIPSState *env)
{
    return env->mvp->CP0_MVPConf1;
}

target_ulong helper_mfc0_random(CPUMIPSState *env)
{
    return (int32_t)cpu_mips_get_random(env);
}

target_ulong helper_mfc0_tcstatus(CPUMIPSState *env)
{
    return env->active_tc.CP0_TCStatus;
}

target_ulong helper_mftc0_tcstatus(CPUMIPSState *env)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    if (other_tc == other->current_tc)
        return other->active_tc.CP0_TCStatus;
    else
        return other->tcs[other_tc].CP0_TCStatus;
}

target_ulong helper_mfc0_tcbind(CPUMIPSState *env)
{
    return env->active_tc.CP0_TCBind;
}

target_ulong helper_mftc0_tcbind(CPUMIPSState *env)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    if (other_tc == other->current_tc)
        return other->active_tc.CP0_TCBind;
    else
        return other->tcs[other_tc].CP0_TCBind;
}

target_ulong helper_mfc0_tcrestart(CPUMIPSState *env)
{
    return env->active_tc.PC;
}

target_ulong helper_mftc0_tcrestart(CPUMIPSState *env)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    if (other_tc == other->current_tc)
        return other->active_tc.PC;
    else
        return other->tcs[other_tc].PC;
}

target_ulong helper_mfc0_tchalt(CPUMIPSState *env)
{
    return env->active_tc.CP0_TCHalt;
}

target_ulong helper_mftc0_tchalt(CPUMIPSState *env)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    if (other_tc == other->current_tc)
        return other->active_tc.CP0_TCHalt;
    else
        return other->tcs[other_tc].CP0_TCHalt;
}

target_ulong helper_mfc0_tccontext(CPUMIPSState *env)
{
    return env->active_tc.CP0_TCContext;
}

target_ulong helper_mftc0_tccontext(CPUMIPSState *env)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    if (other_tc == other->current_tc)
        return other->active_tc.CP0_TCContext;
    else
        return other->tcs[other_tc].CP0_TCContext;
}

target_ulong helper_mfc0_tcschedule(CPUMIPSState *env)
{
    return env->active_tc.CP0_TCSchedule;
}

target_ulong helper_mftc0_tcschedule(CPUMIPSState *env)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    if (other_tc == other->current_tc)
        return other->active_tc.CP0_TCSchedule;
    else
        return other->tcs[other_tc].CP0_TCSchedule;
}

target_ulong helper_mfc0_tcschefback(CPUMIPSState *env)
{
    return env->active_tc.CP0_TCScheFBack;
}

target_ulong helper_mftc0_tcschefback(CPUMIPSState *env)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    if (other_tc == other->current_tc)
        return other->active_tc.CP0_TCScheFBack;
    else
        return other->tcs[other_tc].CP0_TCScheFBack;
}

target_ulong helper_mfc0_count(CPUMIPSState *env)
{
    int32_t count;
    qemu_mutex_lock_iothread();
    count = (int32_t) cpu_mips_get_count(env);
    qemu_mutex_unlock_iothread();
    return count;
}

target_ulong helper_mftc0_entryhi(CPUMIPSState *env)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    return other->CP0_EntryHi;
}

target_ulong helper_mftc0_cause(CPUMIPSState *env)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    int32_t tccause;
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    if (other_tc == other->current_tc) {
        tccause = other->CP0_Cause;
    } else {
        tccause = other->CP0_Cause;
    }

    return tccause;
}

target_ulong helper_mftc0_status(CPUMIPSState *env)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    return other->CP0_Status;
}

target_ulong helper_mfc0_lladdr(CPUMIPSState *env)
{
    return (int32_t)(env->lladdr >> env->CP0_LLAddr_shift);
}

target_ulong helper_mfc0_maar(CPUMIPSState *env)
{
    return (int32_t) env->CP0_MAAR[env->CP0_MAARI];
}

target_ulong helper_mfhc0_maar(CPUMIPSState *env)
{
    return env->CP0_MAAR[env->CP0_MAARI] >> 32;
}

target_ulong helper_mfc0_watchlo(CPUMIPSState *env, uint32_t sel)
{
    return (int32_t)env->CP0_WatchLo[sel];
}

target_ulong helper_mfc0_watchhi(CPUMIPSState *env, uint32_t sel)
{
    return env->CP0_WatchHi[sel];
}

target_ulong helper_mfc0_debug(CPUMIPSState *env)
{
    target_ulong t0 = env->CP0_Debug;
    if (env->hflags & MIPS_HFLAG_DM)
        t0 |= 1 << CP0DB_DM;

    return t0;
}

target_ulong helper_mftc0_debug(CPUMIPSState *env)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    int32_t tcstatus;
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    if (other_tc == other->current_tc)
        tcstatus = other->active_tc.CP0_Debug_tcstatus;
    else
        tcstatus = other->tcs[other_tc].CP0_Debug_tcstatus;

    /* XXX: Might be wrong, check with EJTAG spec. */
    return (other->CP0_Debug & ~((1 << CP0DB_SSt) | (1 << CP0DB_Halt))) |
            (tcstatus & ((1 << CP0DB_SSt) | (1 << CP0DB_Halt)));
}

#if defined(TARGET_MIPS64)
target_ulong helper_dmfc0_tcrestart(CPUMIPSState *env)
{
    return env->active_tc.PC;
}

target_ulong helper_dmfc0_tchalt(CPUMIPSState *env)
{
    return env->active_tc.CP0_TCHalt;
}

target_ulong helper_dmfc0_tccontext(CPUMIPSState *env)
{
    return env->active_tc.CP0_TCContext;
}

target_ulong helper_dmfc0_tcschedule(CPUMIPSState *env)
{
    return env->active_tc.CP0_TCSchedule;
}

target_ulong helper_dmfc0_tcschefback(CPUMIPSState *env)
{
    return env->active_tc.CP0_TCScheFBack;
}

target_ulong helper_dmfc0_lladdr(CPUMIPSState *env)
{
    return env->lladdr >> env->CP0_LLAddr_shift;
}

target_ulong helper_dmfc0_maar(CPUMIPSState *env)
{
    return env->CP0_MAAR[env->CP0_MAARI];
}

target_ulong helper_dmfc0_watchlo(CPUMIPSState *env, uint32_t sel)
{
    return env->CP0_WatchLo[sel];
}
#endif /* TARGET_MIPS64 */

void helper_mtc0_index(CPUMIPSState *env, target_ulong arg1)
{
    uint32_t index_p = env->CP0_Index & 0x80000000;
    uint32_t tlb_index = arg1 & 0x7fffffff;
    if (tlb_index < env->tlb->nb_tlb) {
        if (env->insn_flags & ISA_MIPS32R6) {
            index_p |= arg1 & 0x80000000;
        }
        env->CP0_Index = index_p | tlb_index;
    }
}

void helper_mtc0_mvpcontrol(CPUMIPSState *env, target_ulong arg1)
{
    uint32_t mask = 0;
    uint32_t newval;

    if (env->CP0_VPEConf0 & (1 << CP0VPEC0_MVP))
        mask |= (1 << CP0MVPCo_CPA) | (1 << CP0MVPCo_VPC) |
                (1 << CP0MVPCo_EVP);
    if (env->mvp->CP0_MVPControl & (1 << CP0MVPCo_VPC))
        mask |= (1 << CP0MVPCo_STLB);
    newval = (env->mvp->CP0_MVPControl & ~mask) | (arg1 & mask);

    // TODO: Enable/disable shared TLB, enable/disable VPEs.

    env->mvp->CP0_MVPControl = newval;
}

void helper_mtc0_vpecontrol(CPUMIPSState *env, target_ulong arg1)
{
    uint32_t mask;
    uint32_t newval;

    mask = (1 << CP0VPECo_YSI) | (1 << CP0VPECo_GSI) |
           (1 << CP0VPECo_TE) | (0xff << CP0VPECo_TargTC);
    newval = (env->CP0_VPEControl & ~mask) | (arg1 & mask);

    /* Yield scheduler intercept not implemented. */
    /* Gating storage scheduler intercept not implemented. */

    // TODO: Enable/disable TCs.

    env->CP0_VPEControl = newval;
}

void helper_mttc0_vpecontrol(CPUMIPSState *env, target_ulong arg1)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);
    uint32_t mask;
    uint32_t newval;

    mask = (1 << CP0VPECo_YSI) | (1 << CP0VPECo_GSI) |
           (1 << CP0VPECo_TE) | (0xff << CP0VPECo_TargTC);
    newval = (other->CP0_VPEControl & ~mask) | (arg1 & mask);

    /* TODO: Enable/disable TCs.  */

    other->CP0_VPEControl = newval;
}

target_ulong helper_mftc0_vpecontrol(CPUMIPSState *env)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);
    /* FIXME: Mask away return zero on read bits.  */
    return other->CP0_VPEControl;
}

target_ulong helper_mftc0_vpeconf0(CPUMIPSState *env)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    return other->CP0_VPEConf0;
}

void helper_mtc0_vpeconf0(CPUMIPSState *env, target_ulong arg1)
{
    uint32_t mask = 0;
    uint32_t newval;

    if (env->CP0_VPEConf0 & (1 << CP0VPEC0_MVP)) {
        if (env->CP0_VPEConf0 & (1 << CP0VPEC0_VPA))
            mask |= (0xff << CP0VPEC0_XTC);
        mask |= (1 << CP0VPEC0_MVP) | (1 << CP0VPEC0_VPA);
    }
    newval = (env->CP0_VPEConf0 & ~mask) | (arg1 & mask);

    // TODO: TC exclusive handling due to ERL/EXL.

    env->CP0_VPEConf0 = newval;
}

void helper_mttc0_vpeconf0(CPUMIPSState *env, target_ulong arg1)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);
    uint32_t mask = 0;
    uint32_t newval;

    mask |= (1 << CP0VPEC0_MVP) | (1 << CP0VPEC0_VPA);
    newval = (other->CP0_VPEConf0 & ~mask) | (arg1 & mask);

    /* TODO: TC exclusive handling due to ERL/EXL.  */
    other->CP0_VPEConf0 = newval;
}

void helper_mtc0_vpeconf1(CPUMIPSState *env, target_ulong arg1)
{
    uint32_t mask = 0;
    uint32_t newval;

    if (env->mvp->CP0_MVPControl & (1 << CP0MVPCo_VPC))
        mask |= (0xff << CP0VPEC1_NCX) | (0xff << CP0VPEC1_NCP2) |
                (0xff << CP0VPEC1_NCP1);
    newval = (env->CP0_VPEConf1 & ~mask) | (arg1 & mask);

    /* UDI not implemented. */
    /* CP2 not implemented. */

    // TODO: Handle FPU (CP1) binding.

    env->CP0_VPEConf1 = newval;
}

void helper_mtc0_yqmask(CPUMIPSState *env, target_ulong arg1)
{
    /* Yield qualifier inputs not implemented. */
    env->CP0_YQMask = 0x00000000;
}

void helper_mtc0_vpeopt(CPUMIPSState *env, target_ulong arg1)
{
    env->CP0_VPEOpt = arg1 & 0x0000ffff;
}

#define MTC0_ENTRYLO_MASK(env) ((env->PAMask >> 6) & 0x3FFFFFFF)

void helper_mtc0_entrylo0(CPUMIPSState *env, target_ulong arg1)
{
    /* 1k pages not implemented */
    target_ulong rxi = arg1 & (env->CP0_PageGrain & (3u << CP0PG_XIE));
    env->CP0_EntryLo0 = (arg1 & MTC0_ENTRYLO_MASK(env))
#if defined(TARGET_CHERI)
                        | (rxi << (CP0EnLo_L - 30));
#else
                        | (rxi << (CP0EnLo_XI - 30));
#endif /* TARGET_CHERI */
}

#if defined(TARGET_MIPS64)
#ifdef TARGET_CHERI
#define DMTC0_ENTRYLO_MASK(env) ((env->PAMask >> 6) | (1ull << CP0EnLo_S) | (1ull << CP0EnLo_L))
#else
#define DMTC0_ENTRYLO_MASK(env) (env->PAMask >> 6)
#endif /* TARGET_CHERI */

void helper_dmtc0_entrylo0(CPUMIPSState *env, uint64_t arg1)
{
    uint64_t rxi = arg1 & ((env->CP0_PageGrain & (3ull << CP0PG_XIE)) << 32);
    env->CP0_EntryLo0 = (arg1 & DMTC0_ENTRYLO_MASK(env)) | rxi;
}
#endif

void helper_mtc0_tcstatus(CPUMIPSState *env, target_ulong arg1)
{
    uint32_t mask = env->CP0_TCStatus_rw_bitmask;
    uint32_t newval;

    newval = (env->active_tc.CP0_TCStatus & ~mask) | (arg1 & mask);

    env->active_tc.CP0_TCStatus = newval;
    sync_c0_tcstatus(env, env->current_tc, newval);
}

void helper_mttc0_tcstatus(CPUMIPSState *env, target_ulong arg1)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    if (other_tc == other->current_tc)
        other->active_tc.CP0_TCStatus = arg1;
    else
        other->tcs[other_tc].CP0_TCStatus = arg1;
    sync_c0_tcstatus(other, other_tc, arg1);
}

void helper_mtc0_tcbind(CPUMIPSState *env, target_ulong arg1)
{
    uint32_t mask = (1 << CP0TCBd_TBE);
    uint32_t newval;

    if (env->mvp->CP0_MVPControl & (1 << CP0MVPCo_VPC))
        mask |= (1 << CP0TCBd_CurVPE);
    newval = (env->active_tc.CP0_TCBind & ~mask) | (arg1 & mask);
    env->active_tc.CP0_TCBind = newval;
}

void helper_mttc0_tcbind(CPUMIPSState *env, target_ulong arg1)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    uint32_t mask = (1 << CP0TCBd_TBE);
    uint32_t newval;
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    if (other->mvp->CP0_MVPControl & (1 << CP0MVPCo_VPC))
        mask |= (1 << CP0TCBd_CurVPE);
    if (other_tc == other->current_tc) {
        newval = (other->active_tc.CP0_TCBind & ~mask) | (arg1 & mask);
        other->active_tc.CP0_TCBind = newval;
    } else {
        newval = (other->tcs[other_tc].CP0_TCBind & ~mask) | (arg1 & mask);
        other->tcs[other_tc].CP0_TCBind = newval;
    }
}

void helper_mtc0_tcrestart(CPUMIPSState *env, target_ulong arg1)
{
    env->active_tc.PC = arg1;
    env->active_tc.CP0_TCStatus &= ~(1 << CP0TCSt_TDS);
    env->lladdr = 0ULL;
    /* MIPS16 not implemented. */
}

void helper_mttc0_tcrestart(CPUMIPSState *env, target_ulong arg1)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    if (other_tc == other->current_tc) {
        other->active_tc.PC = arg1;
        other->active_tc.CP0_TCStatus &= ~(1 << CP0TCSt_TDS);
        other->lladdr = 0ULL;
        /* MIPS16 not implemented. */
    } else {
        other->tcs[other_tc].PC = arg1;
        other->tcs[other_tc].CP0_TCStatus &= ~(1 << CP0TCSt_TDS);
        other->lladdr = 0ULL;
        /* MIPS16 not implemented. */
    }
}

void helper_mtc0_tchalt(CPUMIPSState *env, target_ulong arg1)
{
    MIPSCPU *cpu = mips_env_get_cpu(env);

    env->active_tc.CP0_TCHalt = arg1 & 0x1;

    // TODO: Halt TC / Restart (if allocated+active) TC.
    if (env->active_tc.CP0_TCHalt & 1) {
        mips_tc_sleep(cpu, env->current_tc);
    } else {
        mips_tc_wake(cpu, env->current_tc);
    }
}

void helper_mttc0_tchalt(CPUMIPSState *env, target_ulong arg1)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);
    MIPSCPU *other_cpu = mips_env_get_cpu(other);

    // TODO: Halt TC / Restart (if allocated+active) TC.

    if (other_tc == other->current_tc)
        other->active_tc.CP0_TCHalt = arg1;
    else
        other->tcs[other_tc].CP0_TCHalt = arg1;

    if (arg1 & 1) {
        mips_tc_sleep(other_cpu, other_tc);
    } else {
        mips_tc_wake(other_cpu, other_tc);
    }
}

void helper_mtc0_tccontext(CPUMIPSState *env, target_ulong arg1)
{
    env->active_tc.CP0_TCContext = arg1;
}

void helper_mttc0_tccontext(CPUMIPSState *env, target_ulong arg1)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    if (other_tc == other->current_tc)
        other->active_tc.CP0_TCContext = arg1;
    else
        other->tcs[other_tc].CP0_TCContext = arg1;
}

void helper_mtc0_tcschedule(CPUMIPSState *env, target_ulong arg1)
{
    env->active_tc.CP0_TCSchedule = arg1;
}

void helper_mttc0_tcschedule(CPUMIPSState *env, target_ulong arg1)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    if (other_tc == other->current_tc)
        other->active_tc.CP0_TCSchedule = arg1;
    else
        other->tcs[other_tc].CP0_TCSchedule = arg1;
}

void helper_mtc0_tcschefback(CPUMIPSState *env, target_ulong arg1)
{
    env->active_tc.CP0_TCScheFBack = arg1;
}

void helper_mttc0_tcschefback(CPUMIPSState *env, target_ulong arg1)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    if (other_tc == other->current_tc)
        other->active_tc.CP0_TCScheFBack = arg1;
    else
        other->tcs[other_tc].CP0_TCScheFBack = arg1;
}

void helper_mtc0_entrylo1(CPUMIPSState *env, target_ulong arg1)
{
    /* 1k pages not implemented */
    target_ulong rxi = arg1 & (env->CP0_PageGrain & (3u << CP0PG_XIE));
    env->CP0_EntryLo1 = (arg1 & MTC0_ENTRYLO_MASK(env))
#if defined(TARGET_CHERI)
                        | (rxi << (CP0EnLo_L - 30));
#else
                        | (rxi << (CP0EnLo_XI - 30));
#endif /* TARGET_CHERI */
}

#if defined(TARGET_MIPS64)
void helper_dmtc0_entrylo1(CPUMIPSState *env, uint64_t arg1)
{
    uint64_t rxi = arg1 & ((env->CP0_PageGrain & (3ull << CP0PG_XIE)) << 32);
    env->CP0_EntryLo1 = (arg1 & DMTC0_ENTRYLO_MASK(env)) | rxi;
}
#endif

void helper_mtc0_context(CPUMIPSState *env, target_ulong arg1)
{
    env->CP0_Context = (env->CP0_Context & 0x007FFFFF) | (arg1 & ~0x007FFFFF);
}

void helper_mtc0_pagemask(CPUMIPSState *env, target_ulong arg1)
{
    uint64_t mask = arg1 >> (TARGET_PAGE_BITS + 1);
    if (!(env->insn_flags & ISA_MIPS32R6) || (arg1 == ~0) ||
        (mask == 0x0000 || mask == 0x0003 || mask == 0x000F ||
         mask == 0x003F || mask == 0x00FF || mask == 0x03FF ||
         mask == 0x0FFF || mask == 0x3FFF || mask == 0xFFFF)) {
        env->CP0_PageMask = arg1 & (0x1FFFFFFF & (TARGET_PAGE_MASK << 1));
    }
}

void helper_mtc0_pagegrain(CPUMIPSState *env, target_ulong arg1)
{
    /* SmartMIPS not implemented */
    /* 1k pages not implemented */
    env->CP0_PageGrain = (arg1 & env->CP0_PageGrain_rw_bitmask) |
                         (env->CP0_PageGrain & ~env->CP0_PageGrain_rw_bitmask);
    compute_hflags(env);
    restore_pamask(env);
}

void helper_mtc0_segctl0(CPUMIPSState *env, target_ulong arg1)
{
    CPUState *cs = CPU(mips_env_get_cpu(env));

    env->CP0_SegCtl0 = arg1 & CP0SC0_MASK;
    tlb_flush(cs);
}

void helper_mtc0_segctl1(CPUMIPSState *env, target_ulong arg1)
{
    CPUState *cs = CPU(mips_env_get_cpu(env));

    env->CP0_SegCtl1 = arg1 & CP0SC1_MASK;
    tlb_flush(cs);
}

void helper_mtc0_segctl2(CPUMIPSState *env, target_ulong arg1)
{
    CPUState *cs = CPU(mips_env_get_cpu(env));

    env->CP0_SegCtl2 = arg1 & CP0SC2_MASK;
    tlb_flush(cs);
}

void helper_mtc0_wired(CPUMIPSState *env, target_ulong arg1)
{
    if (env->insn_flags & ISA_MIPS32R6) {
        if (arg1 < env->tlb->nb_tlb) {
            env->CP0_Wired = arg1;
        }
    } else {
        env->CP0_Wired = arg1 % env->tlb->nb_tlb;
    }
}

void helper_mtc0_srsconf0(CPUMIPSState *env, target_ulong arg1)
{
    env->CP0_SRSConf0 |= arg1 & env->CP0_SRSConf0_rw_bitmask;
}

void helper_mtc0_srsconf1(CPUMIPSState *env, target_ulong arg1)
{
    env->CP0_SRSConf1 |= arg1 & env->CP0_SRSConf1_rw_bitmask;
}

void helper_mtc0_srsconf2(CPUMIPSState *env, target_ulong arg1)
{
    env->CP0_SRSConf2 |= arg1 & env->CP0_SRSConf2_rw_bitmask;
}

void helper_mtc0_srsconf3(CPUMIPSState *env, target_ulong arg1)
{
    env->CP0_SRSConf3 |= arg1 & env->CP0_SRSConf3_rw_bitmask;
}

void helper_mtc0_srsconf4(CPUMIPSState *env, target_ulong arg1)
{
    env->CP0_SRSConf4 |= arg1 & env->CP0_SRSConf4_rw_bitmask;
}

void helper_mtc0_hwrena(CPUMIPSState *env, target_ulong arg1)
{
    uint32_t mask = 0x0000000F;
#if defined(TARGET_CHERI)
    /* Statcounters uses registers 4-14 */
    mask |= 0x7FF0;
#endif

    if ((env->CP0_Config1 & (1 << CP0C1_PC)) &&
        (env->insn_flags & ISA_MIPS32R6)) {
        mask |= (1 << 4);
    }
    if (env->insn_flags & ISA_MIPS32R6) {
        mask |= (1 << 5);
    }
    if (env->CP0_Config3 & (1 << CP0C3_ULRI)) {
        mask |= (1 << 29);

        if (arg1 & (1 << 29)) {
            env->hflags |= MIPS_HFLAG_HWRENA_ULR;
        } else {
            env->hflags &= ~MIPS_HFLAG_HWRENA_ULR;
        }
    }

    env->CP0_HWREna = arg1 & mask;
}

void helper_mtc0_count(CPUMIPSState *env, target_ulong arg1)
{
    qemu_mutex_lock_iothread();
    cpu_mips_store_count(env, arg1);
    qemu_mutex_unlock_iothread();
}

#ifdef TARGET_CHERI
uint64_t helper_mfc0_rtc64(CPUMIPSState *env)
{
    return cpu_mips_get_rtc64(env);
}

void helper_mtc0_rtc64(CPUMIPSState *env, uint64_t arg1)
{
    cpu_mips_set_rtc64(env, arg1);
}

/*
 * Return the CHERI/BERI CoreID Register:
 *
 *  31            16 15            0
 *  ----------------+---------------
 * |    MaxCoreID   |    CoreID     |
 *  ----------------+---------------
 *
 * Cores are numbered from 0 to MaxCoreID.
 *
 * See section 7.3.5 of BERI Hardware reference.
 */
target_ulong helper_mfc0_coreid(CPUMIPSState *env)
{
    MIPSCPU *cpu = mips_env_get_cpu(env);
    CPUState *cs = CPU(cpu);

    return (uint32_t)(((cs->nr_cores - 1) << 16) |
            (cs->cpu_index & 0xffff));
}
#endif /* TARGET_CHERI */

void helper_mtc0_entryhi(CPUMIPSState *env, target_ulong arg1)
{
    target_ulong old, val, mask;
    mask = (TARGET_PAGE_MASK << 1) | env->CP0_EntryHi_ASID_mask;
    if (((env->CP0_Config4 >> CP0C4_IE) & 0x3) >= 2) {
        mask |= 1 << CP0EnHi_EHINV;
    }

    /* 1k pages not implemented */
#if defined(TARGET_MIPS64)
    if (env->insn_flags & ISA_MIPS32R6) {
        int entryhi_r = extract64(arg1, 62, 2);
        int config0_at = extract32(env->CP0_Config0, 13, 2);
        bool no_supervisor = (env->CP0_Status_rw_bitmask & 0x8) == 0;
        if ((entryhi_r == 2) ||
            (entryhi_r == 1 && (no_supervisor || config0_at == 1))) {
            /* skip EntryHi.R field if new value is reserved */
            mask &= ~(0x3ull << 62);
        }
    }
    mask &= env->SEGMask;
#endif
    old = env->CP0_EntryHi;
    val = (arg1 & mask) | (old & ~mask);
    env->CP0_EntryHi = val;
    if (env->CP0_Config3 & (1 << CP0C3_MT)) {
        sync_c0_entryhi(env, env->current_tc);
    }
    /* If the ASID changes, flush qemu's TLB.  */
    if ((old & env->CP0_EntryHi_ASID_mask) !=
        (val & env->CP0_EntryHi_ASID_mask)) {
        tlb_flush(CPU(mips_env_get_cpu(env)));
    }
}

void helper_mttc0_entryhi(CPUMIPSState *env, target_ulong arg1)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    other->CP0_EntryHi = arg1;
    sync_c0_entryhi(other, other_tc);
}

void helper_mtc0_compare(CPUMIPSState *env, target_ulong arg1)
{
    qemu_mutex_lock_iothread();
    cpu_mips_store_compare(env, arg1);
    qemu_mutex_unlock_iothread();
}

void helper_mtc0_status(CPUMIPSState *env, target_ulong arg1)
{
    MIPSCPU *cpu = mips_env_get_cpu(env);
    uint32_t val, old;

    old = env->CP0_Status;
    cpu_mips_store_status(env, arg1);
    val = env->CP0_Status;

    if (qemu_loglevel_mask(CPU_LOG_EXEC)) {
        qemu_log("Status %08x (%08x) => %08x (%08x) Cause %08x",
                old, old & env->CP0_Cause & CP0Ca_IP_mask,
                val, val & env->CP0_Cause & CP0Ca_IP_mask,
                env->CP0_Cause);
        switch (cpu_mmu_index(env, false)) {
        case 3:
            qemu_log(", ERL\n");
            break;
        case MIPS_HFLAG_UM: qemu_log(", UM\n"); break;
        case MIPS_HFLAG_SM: qemu_log(", SM\n"); break;
        case MIPS_HFLAG_KM: qemu_log("\n"); break;
        default:
            cpu_abort(CPU(cpu), "Invalid MMU mode!\n");
            break;
        }
    }
}

void helper_mttc0_status(CPUMIPSState *env, target_ulong arg1)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    uint32_t mask = env->CP0_Status_rw_bitmask & ~0xf1000018;
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    other->CP0_Status = (other->CP0_Status & ~mask) | (arg1 & mask);
    sync_c0_status(env, other, other_tc);
}

void helper_mtc0_intctl(CPUMIPSState *env, target_ulong arg1)
{
    env->CP0_IntCtl = (env->CP0_IntCtl & ~0x000003e0) | (arg1 & 0x000003e0);
}

void helper_mtc0_srsctl(CPUMIPSState *env, target_ulong arg1)
{
    uint32_t mask = (0xf << CP0SRSCtl_ESS) | (0xf << CP0SRSCtl_PSS);
    env->CP0_SRSCtl = (env->CP0_SRSCtl & ~mask) | (arg1 & mask);
}

void helper_mtc0_cause(CPUMIPSState *env, target_ulong arg1)
{
    qemu_mutex_lock_iothread();
    cpu_mips_store_cause(env, arg1);
    qemu_mutex_unlock_iothread();
}

void helper_mttc0_cause(CPUMIPSState *env, target_ulong arg1)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    cpu_mips_store_cause(other, arg1);
}

target_ulong helper_mftc0_epc(CPUMIPSState *env)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    return other->CP0_EPC;
}

target_ulong helper_mftc0_ebase(CPUMIPSState *env)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    return other->CP0_EBase;
}

void helper_mtc0_ebase(CPUMIPSState *env, target_ulong arg1)
{
    target_ulong mask = 0x3FFFF000 | env->CP0_EBaseWG_rw_bitmask;
    if (arg1 & env->CP0_EBaseWG_rw_bitmask) {
        mask |= ~0x3FFFFFFF;
    }
    env->CP0_EBase = (env->CP0_EBase & ~mask) | (arg1 & mask);
}

void helper_mttc0_ebase(CPUMIPSState *env, target_ulong arg1)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);
    target_ulong mask = 0x3FFFF000 | env->CP0_EBaseWG_rw_bitmask;
    if (arg1 & env->CP0_EBaseWG_rw_bitmask) {
        mask |= ~0x3FFFFFFF;
    }
    other->CP0_EBase = (other->CP0_EBase & ~mask) | (arg1 & mask);
}

target_ulong helper_mftc0_configx(CPUMIPSState *env, target_ulong idx)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    switch (idx) {
    case 0: return other->CP0_Config0;
    case 1: return other->CP0_Config1;
    case 2: return other->CP0_Config2;
    case 3: return other->CP0_Config3;
    /* 4 and 5 are reserved.  */
    case 6: return other->CP0_Config6;
    case 7: return other->CP0_Config7;
    default:
        break;
    }
    return 0;
}

void helper_mtc0_config0(CPUMIPSState *env, target_ulong arg1)
{
    env->CP0_Config0 = (env->CP0_Config0 & 0x81FFFFF8) | (arg1 & 0x00000007);
}

void helper_mtc0_config2(CPUMIPSState *env, target_ulong arg1)
{
    /* tertiary/secondary caches not implemented */
    env->CP0_Config2 = (env->CP0_Config2 & 0x8FFF0FFF);
}

void helper_mtc0_config3(CPUMIPSState *env, target_ulong arg1)
{
    if (env->insn_flags & ASE_MICROMIPS) {
        env->CP0_Config3 = (env->CP0_Config3 & ~(1 << CP0C3_ISA_ON_EXC)) |
                           (arg1 & (1 << CP0C3_ISA_ON_EXC));
    }
}

void helper_mtc0_config4(CPUMIPSState *env, target_ulong arg1)
{
    env->CP0_Config4 = (env->CP0_Config4 & (~env->CP0_Config4_rw_bitmask)) |
                       (arg1 & env->CP0_Config4_rw_bitmask);
}

void helper_mtc0_config5(CPUMIPSState *env, target_ulong arg1)
{
    env->CP0_Config5 = (env->CP0_Config5 & (~env->CP0_Config5_rw_bitmask)) |
                       (arg1 & env->CP0_Config5_rw_bitmask);
    compute_hflags(env);
}

void helper_mtc0_lladdr(CPUMIPSState *env, target_ulong arg1)
{
    target_long mask = env->CP0_LLAddr_rw_bitmask;
    arg1 = arg1 << env->CP0_LLAddr_shift;
    env->lladdr = (env->lladdr & ~mask) | (arg1 & mask);
}

#define MTC0_MAAR_MASK(env) \
        ((0x1ULL << 63) | ((env->PAMask >> 4) & ~0xFFFull) | 0x3)

void helper_mtc0_maar(CPUMIPSState *env, target_ulong arg1)
{
    env->CP0_MAAR[env->CP0_MAARI] = arg1 & MTC0_MAAR_MASK(env);
}

void helper_mthc0_maar(CPUMIPSState *env, target_ulong arg1)
{
    env->CP0_MAAR[env->CP0_MAARI] =
        (((uint64_t) arg1 << 32) & MTC0_MAAR_MASK(env)) |
        (env->CP0_MAAR[env->CP0_MAARI] & 0x00000000ffffffffULL);
}

void helper_mtc0_maari(CPUMIPSState *env, target_ulong arg1)
{
    int index = arg1 & 0x3f;
    if (index == 0x3f) {
        /* Software may write all ones to INDEX to determine the
           maximum value supported. */
        env->CP0_MAARI = MIPS_MAAR_MAX - 1;
    } else if (index < MIPS_MAAR_MAX) {
        env->CP0_MAARI = index;
    }
    /* Other than the all ones, if the
       value written is not supported, then INDEX is unchanged
       from its previous value. */
}

void helper_mtc0_watchlo(CPUMIPSState *env, target_ulong arg1, uint32_t sel)
{
    /* Watch exceptions for instructions, data loads, data stores
       not implemented. */
    env->CP0_WatchLo[sel] = (arg1 & ~0x7);
}

void helper_mtc0_watchhi(CPUMIPSState *env, target_ulong arg1, uint32_t sel)
{
    int mask = 0x40000FF8 | (env->CP0_EntryHi_ASID_mask << CP0WH_ASID);
    env->CP0_WatchHi[sel] = arg1 & mask;
    env->CP0_WatchHi[sel] &= ~(env->CP0_WatchHi[sel] & arg1 & 0x7);
}

void helper_mtc0_xcontext(CPUMIPSState *env, target_ulong arg1)
{
    target_ulong mask = (1ULL << (env->SEGBITS - 7)) - 1;
    env->CP0_XContext = (env->CP0_XContext & mask) | (arg1 & ~mask);
}

void helper_mtc0_framemask(CPUMIPSState *env, target_ulong arg1)
{
    env->CP0_Framemask = arg1; /* XXX */
}

void helper_mtc0_debug(CPUMIPSState *env, target_ulong arg1)
{
    env->CP0_Debug = (env->CP0_Debug & 0x8C03FC1F) | (arg1 & 0x13300120);
    if (arg1 & (1 << CP0DB_DM))
        env->hflags |= MIPS_HFLAG_DM;
    else
        env->hflags &= ~MIPS_HFLAG_DM;

//cpu_loop_exit(CPU(mips_env_get_cpu(env))); /* CHERI: exit simulation */
//exit(0);
}

void helper_mttc0_debug(CPUMIPSState *env, target_ulong arg1)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    uint32_t val = arg1 & ((1 << CP0DB_SSt) | (1 << CP0DB_Halt));
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    /* XXX: Might be wrong, check with EJTAG spec. */
    if (other_tc == other->current_tc)
        other->active_tc.CP0_Debug_tcstatus = val;
    else
        other->tcs[other_tc].CP0_Debug_tcstatus = val;
    other->CP0_Debug = (other->CP0_Debug &
                     ((1 << CP0DB_SSt) | (1 << CP0DB_Halt))) |
                     (arg1 & ~((1 << CP0DB_SSt) | (1 << CP0DB_Halt)));
}

void helper_mtc0_performance0(CPUMIPSState *env, target_ulong arg1)
{
    env->CP0_Performance0 = arg1 & 0x000007ff;
}

void helper_mtc0_errctl(CPUMIPSState *env, target_ulong arg1)
{
    int32_t wst = arg1 & (1 << CP0EC_WST);
    int32_t spr = arg1 & (1 << CP0EC_SPR);
    int32_t itc = env->itc_tag ? (arg1 & (1 << CP0EC_ITC)) : 0;

    env->CP0_ErrCtl = wst | spr | itc;

    if (itc && !wst && !spr) {
        env->hflags |= MIPS_HFLAG_ITC_CACHE;
    } else {
        env->hflags &= ~MIPS_HFLAG_ITC_CACHE;
    }
}

void helper_mtc0_taglo(CPUMIPSState *env, target_ulong arg1)
{
    if (env->hflags & MIPS_HFLAG_ITC_CACHE) {
        /* If CACHE instruction is configured for ITC tags then make all
           CP0.TagLo bits writable. The actual write to ITC Configuration
           Tag will take care of the read-only bits. */
        env->CP0_TagLo = arg1;
    } else {
        env->CP0_TagLo = arg1 & 0xFFFFFCF6;
    }
}

#if defined(TARGET_CHERI)

static inline bool
is_cap_sealed(const cap_register_t *cp)
{
    return (cp->cr_sealed) ? true : false;
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

#ifdef CHERI_MAGIC128

#define CHERI_CAP_SIZE  16

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
#define CHERI_CAP_SIZE  16

/*
 * Define the following to do the is_representable() check by simply
 * compressing and decompressing the capability and checking to
 * see if it is the same.
 */
// #define SIMPLE_REPRESENT_CHECK

#ifndef SIMPLE_REPRESENT_CHECK
static inline bool all_ones(uint64_t offset, uint32_t e, uint32_t bwidth)
{
    uint64_t Itop;
    uint32_t shift = e + bwidth;

    if (shift >= 63)
        return false;
    Itop = offset >> shift;
    return Itop == (0xfffffffffffffffful >> shift);
}

static inline bool all_zeroes(uint64_t offset, uint32_t e, uint32_t bwidth)
{
    uint32_t shift = e + bwidth;
    uint64_t Itop;

    if (shift >= 63)
        Itop = 0ul;
    else
        Itop = offset >> shift;
    return Itop == 0ul;
}
#endif /* ! SIMPLE_REPRESENT_CHECK */

/* Returns the index of the most significant bit set in x */
static inline uint32_t idx_MSNZ(uint64_t x)
{
    /*
     * XXX For HOST_X86_64
     *
     * uint64_t r;
     *
     * asm("bsrq %1,%q0" : "+r" (r) : "rm" (x));
     *
     * return (uint32_t)r;
     *
     * XXX This isn't quite right. %q0 needs to be pushed/popped?
     */

/* floor(log2(x)) != floor(log2(y)) */
#define ld_neq(x, y) (((x)^(y)) > ((x)&(y)))

    uint32_t r = ld_neq(x, x & 0x5555555555555555ull)
        + (ld_neq(x, x & 0x3333333333333333ull) << 1)
        + (ld_neq(x, x & 0x0f0f0f0f0f0f0f0full) << 2)
        + (ld_neq(x, x & 0x00ff00ff00ff00ffull) << 3)
        + (ld_neq(x, x & 0x0000ffff0000ffffull) << 4)
        + (ld_neq(x, x & 0x00000000ffffffffull) << 5);

#undef ld_neq

    return r;
}

/*
 * e = idxMSNZ( (rlength + (rlength >> 6)) >> 19 )
 * where (rlength + (rlength >> 6)) needs to be a 65 bit integer
 */
static uint32_t compute_e(uint64_t rlength, uint32_t bwidth)
{
    if(rlength < (1 << (bwidth-1))) return 0;

    return (idx_MSNZ(rlength) - (bwidth-2));
}

static inline uint64_t getbits(uint64_t src, uint32_t str, uint32_t sz)
{

    return ((src >> str) & ((1ull << sz) - 1ull));
}

/*
 * These formats are from cheri concentrate, but I have added an extra sealing mode in order to allow precise sealing of zero offset objects
 * Unsealed CC-L:
 *  perms:    63-49 (15 bits)
 *  unused:   48    (1 bit)
 *  S:        47-46 (2 bits) = 0
 *  IE:       45    (1 bit)
 *  LH:       44    (1 bit)
 *  T:        43-23 (21 bit)
 *  B:        22-0  (23 bits)
 *
 * Sealed1 CC-L:
 *  perms:    63-49  (15 bits)
 *  unused:   48     (1 bit)
 *  S:        47-46  (2 bits) = 1
 *  IE:       45     (1 bit)
 *  LH:       44     (1 bit)
 *  otype.hi: 43-34  (10 bits)
 *  T:        33-23  (11 bits)
 *  otype.lo: 22-13  (10 bits)
 *  B:        12-0   (13 bits)
 *
 * Sealed2 CC-L:
 *  perms:    63-49 (15 bits)
 *  unused:   48    (1 bit)
 *  S:        47-46 (2 bits) = 2
 *  IE:       45    (1 bit)
 *  LH:       44    (1 bit)
 *  T:        43-23 (21 bits)
 *  otype     22-3  (20 bits)
 *  B:        2-0   (3 bits) (completely implied by cursor. Keep 3 bits for exponent)
 */

/*
 * Decompress a 128-bit capability.
 */
static void decompress_128cap(uint64_t pesbt, uint64_t cursor,
        cap_register_t *cdp)
{

    cdp->cr_pesbt = pesbt;

    pesbt ^= NULL_XOR_MASK;

    cdp->cr_perms = getbits(pesbt, 49, 11);
    cdp->cr_uperms = getbits(pesbt, 60, 4);

    uint8_t seal_mode = (uint8_t)getbits(pesbt, CC_L_S_OFF, 1);

    cdp->cr_sealed = seal_mode == 0 ? 0 : 1;

    uint32_t BWidth = seal_mode == 1 ? CC_L_SEALED_BWIDTH : CC_L_BWIDTH;
    uint32_t BMask = (1 << BWidth) - 1;
    uint32_t TMask = BMask >> 2;

    uint8_t IE = (uint8_t)getbits(pesbt, CC_L_IE_OFF, 1);
    uint8_t LH = (uint8_t)getbits(pesbt, CC_L_LH_OFF, 1);

    uint8_t E,L_msb;
    uint32_t B = getbits(pesbt, CC_L_B_OFF, BWidth);
    uint32_t T = getbits(pesbt, CC_L_T_OFF, BWidth-2);

    if(IE) {
        E = ((((LH << CC_L_LOWWIDTH) | (B & CC_L_LOWMASK)) << CC_L_LOWWIDTH) | (T & CC_L_LOWMASK)) + 1; // Offset by 1. We don't need to encode E=0
        E = MIN(64 - BWidth + 2, E);
        B &= ~CC_L_LOWMASK;
        T &= ~CC_L_LOWMASK;
        L_msb = 1;
    } else {
        E = 0;
        L_msb = LH;
    }

    uint32_t type = 0;

    if(seal_mode == 1) {
        type = (getbits(pesbt, CC_L_OHI_OFF, CC_L_TYPES/2) << (CC_L_TYPES/2)) | getbits(pesbt, CC_L_OLO_OFF, CC_L_TYPES/2);
    }

    cdp->cr_otype = type;

    uint8_t L_carry = T < (B & TMask) ? 1 : 0;
    uint8_t T_infer = ((B >> (BWidth-2)) + L_carry + L_msb) & 0x3;

    T |= ((uint32_t)T_infer) << (BWidth-2);

    uint32_t amid = (cursor >> E) & BMask;
    uint32_t r = (((B >> (BWidth-3)) - 1) << (BWidth-3)) & BMask;

    int64_t ct,cb;

    if(amid < r) {
        ct = T < r ? 0LL : -1LL;
        cb = B < r ? 0LL : -1LL;
    } else {
        ct = T < r ? 1LL : 0LL;
        cb = B < r ? 1LL : 0LL;
    }

    uint8_t shift = E + BWidth;

    uint64_t cursor_top = shift >= 64 ? 0 : cursor >> shift;

    uint64_t top  = (((cursor_top + (int64_t)ct) << BWidth) | (uint64_t)T) << E;
    uint64_t base = (((cursor_top + (int64_t)cb) << BWidth) | (uint64_t)B) << E;


    // top/length really should be 65 bits. If we get overflow length is actually max length

    cdp->cr_length = top < T ? (-1ULL) - base : top - base;
    cdp->cr_offset = cursor - base;
    cdp->cr_base = base;
}



/*
 * Compress a capability to 128 bits.
 */
static uint64_t compress_128cap(const cap_register_t *csp)
{
    bool is_sealed = is_cap_sealed(csp);

    uint8_t seal_mode = is_sealed ? 1 : 0;

    uint32_t BWidth = seal_mode == 1 ? CC_L_SEALED_BWIDTH : CC_L_BWIDTH;
    uint32_t BMask = (1 << BWidth) - 1;
    uint32_t TMask = BMask >> 2;

    uint64_t base = csp->cr_base;
    uint64_t top = csp->cr_base + csp->cr_length;
    uint64_t length = top - base;

    uint8_t IE, LH;
    uint32_t Te,Be;
    uint8_t E;

    if(top == -1ULL) {
        top = 0; // Actually 1 << 64
        length++;

        // Length of 0 is 1 << 64.
        if(length == 0) {
            E = 64 - BWidth + 2;
        } else {
            E = compute_e(length, BWidth);
        }

        Te = (1ULL << (64-E)) & TMask;
    } else {
        E = compute_e(length, BWidth);
        Te = (top >> E) & TMask;
    }

    Be = (base >> E) & BMask;
    IE = E == 0 ? 0 : 1;

    if (IE) {
        E -=1; // Don't need to encode E=0
        LH = E >> (2 * CC_L_LOWWIDTH);
        Be |= (E >> CC_L_LOWWIDTH) & CC_L_LOWMASK;
        Te |= E & CC_L_LOWMASK;
    } else {
        LH = (length >> (BWidth-2)) & 1;
    }

    if (seal_mode == 1) {
        uint64_t hi = ((uint64_t)csp->cr_otype >> (CC_L_TYPES/2)) & ((1 << (CC_L_TYPES/2))-1);
        uint64_t lo = (uint64_t)csp->cr_otype & ((1 << (CC_L_TYPES/2))-1);
        Te |= hi << (CC_L_SEALED_BWIDTH-2);
        Be |= lo << CC_L_SEALED_BWIDTH;
    }
    // assert(seal_mode < 3);

    uint64_t perms = ((uint64_t)csp->cr_uperms << 11) | (uint64_t)csp->cr_perms;
    uint64_t pesbt = ((((((((((perms << 3) |
                      (uint64_t)seal_mode) << 1) |
                      (uint64_t)IE) << 1) |
                      (uint64_t)LH) << (CC_L_BWIDTH-2)) |
                      (uint64_t)Te) << CC_L_BWIDTH) |
                      (uint64_t)Be);

    pesbt ^= NULL_XOR_MASK;

    return pesbt;
}

/*
 * Check to see if a memory region is representable by a compressed
 * capability. It is representable if:
 *
 *   representable = (inRange && inLimits) || (E >= 44)
 *
 * where:
 *
 *   E = compression exponent (see compute_e() above)
 *
 *   inRange = -s < i < s  where i is the increment (or offset)
 *   (In other words, all the bits of i<63, E+20> are the same.)
 *
 *   inLimits = (i < 0) ? (Imid >= (R - Amid)) && (R != Amid) : (R - Amid - 1)
 *   where Imid = i<E+19, E>, Amid = a<E+19, E>, R = B - 2^12 and a =
 *   base + offset.
 */
bool
is_representable(bool sealed, uint64_t base, uint64_t length, uint64_t offset,
        uint64_t new_offset)
{
    // I change the precision going between unsealed->sealed so the fast check doesn't work. Instead just compress/decompress.
    if(sealed) {

        cap_register_t c;
        uint64_t pesbt;

        /* Simply compress and uncompress to check. */

        #define MAGIC_TYPE 0b1011011101

        c.cr_base = base;
        c.cr_length = length;
        c.cr_offset = new_offset;
        c.cr_sealed = sealed;
        c.cr_otype = 0; // important to set as compress assumes this is in bounds

        pesbt = compress_128cap(&c);
        decompress_128cap(pesbt, base + new_offset, &c);

        if (c.cr_base != base || c.cr_length != length || c.cr_offset != new_offset)
            return false;

        return true;
    }

    uint32_t bwidth = CC_L_BWIDTH;
    uint32_t highest_exp = (64 - bwidth + 2);

    uint32_t e;

    // If top is 0xffff... we assume we meant it to be 1 << 64
    if(base + length == -1ULL) {
        length++;
        if(length == 0) {
            return true; // maximum length is always representable
        }
    }

    e = compute_e(length, bwidth);

    int64_t b, r, Imid, Amid;
    bool inRange, inLimits;
    int64_t inc = new_offset - offset;

#define MOD_MASK    ((1ul << bwidth) - 1ul)


    /* Check for the boundary cases. */

    b = (int64_t)((base >> e) & MOD_MASK);
    Imid = (int64_t)((inc >> e) & MOD_MASK);
    Amid = (int64_t)(((base + offset) >> e) & MOD_MASK);

    r = (((b >> (bwidth-3)) - 1) << (bwidth-3)) & MOD_MASK;

    /* inRange, test if bits are all the same */
    inRange = all_ones(inc, e, bwidth) || all_zeroes(inc, e, bwidth);

    /* inLimits */
    if (inc >= 0) {
        inLimits = ((uint64_t)Imid  < (((uint64_t)(r - Amid - 1l)) & MOD_MASK));
    } else {
        inLimits = ((uint64_t)Imid >= (((uint64_t)(r - Amid)) & MOD_MASK)) &&
            (r != Amid);
    }
#undef MOD_MASK

    return ((inRange && inLimits) || (e >= highest_exp));
}

extern bool cheri_c2e_on_unrepresentable;

static inline void
_became_unrepresentable(CPUMIPSState *env, uint16_t reg)
{
	if (cheri_c2e_on_unrepresentable)
		do_raise_c2_exception(env, CP2Ca_INEXACT, reg);
}

target_ulong helper_ccheck_imprecise(CPUMIPSState *env, target_ulong inc)
{
    cap_register_t *pcc = &env->active_tc.PCC;

    if (!is_representable(is_cap_sealed(pcc), pcc->cr_base, pcc->cr_length,
                pcc->cr_offset, inc)) {
        /*
         * Clear the tag and set the proper offset here. The capability
         * will be adjusted when handling the exception to avoid losing
         * information about the bounds before actually setting EPC.
         */
        pcc->cr_tag = 0;
        pcc->cr_offset = inc;
    }

    return (pcc->cr_base);
}

#else

#define CHERI_CAP_SIZE  32

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

static inline void dump_store(CPUMIPSState *env, int, target_ulong,
				target_ulong);

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
    default:
        return 1;
    }
}

static inline void check_cap(CPUMIPSState *env, const cap_register_t *cr,
        uint32_t perm, uint64_t addr, uint16_t regnum, uint32_t len)
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
    if (addr < cr->cr_base || (addr + len) > (cr->cr_base + cr->cr_length)) {
        cause = CP2Ca_LENGTH;
        // fprintf(qemu_logfile, "CAP Len VIOLATION: ");
        goto do_exception;
    }

    return;

do_exception:
    env->CP0_BadVAddr = addr;
    // env->active_tc.C[CP2CAP_EPCC] = *cr;
    do_raise_c2_exception(env, cause, regnum);
}

static inline int creg_inaccessible(uint32_t perms, uint32_t creg)
{
    // FIXME: remove this check now that we no longer mirror special regs
    /*
     * Check to see if the capability register is inaccessible.
     * See Section 5.4 in CHERI Architecture manual.
     */
    if (!(perms & CAP_ACCESS_SYS_REGS) && (creg == CP2CAP_EPCC ||
                creg == CP2CAP_KDC || creg == CP2CAP_KCC ||
                creg == CP2CAP_KR1C  || creg == CP2CAP_KR2C)) {
        return 1;
    } else {
        return 0;
    }
}

void helper_check_access_idc(CPUMIPSState *env, uint32_t reg)
{
    /*
     * IDC access in a CCall (selector 1) delay slot
     */
    if (reg == CP2CAP_IDC) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_CCALL_IDC, reg);
    }
}

void helper_candperm(CPUMIPSState *env, uint32_t cd, uint32_t cb,
        target_ulong rt)
{
    uint32_t perms = env->active_tc.PCC.cr_perms;
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    /*
     * CAndPerm: Restrict Permissions
     */
    if (creg_inaccessible(perms, cd)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cd);
    } else if (creg_inaccessible(perms, cb)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cb);
    } else if (!cbp->cr_tag) {
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
    cap_register_t *pccp = &env->active_tc.PCC;
    uint32_t perms = pccp->cr_perms;
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    /*
     * CBEZ: Branch if NULL
     */
    if (creg_inaccessible(perms, cb)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cb);
        return (target_ulong)0;
    } else {
        /*
         * Compare the only semantically meaningful fields of int_to_cap(0)
         */
        if (cbp->cr_base == 0 && cbp->cr_tag == 0 && cbp->cr_offset == 0)
            return (target_ulong)1;
        else
            return (target_ulong)0;
    }
}

target_ulong helper_cbnz(CPUMIPSState *env, uint32_t cb, uint32_t offset)
{
    cap_register_t *pccp = &env->active_tc.PCC;
    uint32_t perms = pccp->cr_perms;
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    /*
     * CBEZ: Branch if not NULL
     */
    if (creg_inaccessible(perms, cb)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cb);
        return (target_ulong)0;
    } else {
        /*
         * Compare the only semantically meaningful fields of int_to_cap(0)
         */
        if (cbp->cr_base == 0 && cbp->cr_tag == 0 && cbp->cr_offset == 0)
            return (target_ulong)0;
        else
            return (target_ulong)1;
    }
}

target_ulong helper_cbts(CPUMIPSState *env, uint32_t cb, uint32_t offset)
{
    cap_register_t *pccp = &env->active_tc.PCC;
    uint32_t perms = pccp->cr_perms;
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    /*
     * CBTS: Branch if tag is set
     */
    if (creg_inaccessible(perms, cb)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cb);
        return (target_ulong)0;
    } else {
        return (target_ulong)cbp->cr_tag;
    }
}

target_ulong helper_cbtu(CPUMIPSState *env, uint32_t cb, uint32_t offset)
{
    cap_register_t *pccp = &env->active_tc.PCC;
    uint32_t perms = pccp->cr_perms;
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    /*
     * CBTU: Branch if tag is unset
     */
    if (creg_inaccessible(perms, cb)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cb);
        return (target_ulong)0;
    } else {
        return (target_ulong)!cbp->cr_tag;
    }
}

static target_ulong ccall_common(CPUMIPSState *env, uint32_t cs, uint32_t cb, uint32_t selector)
{
    uint32_t perms = env->active_tc.PCC.cr_perms;
    const cap_register_t *csp = get_readonly_capreg(&env->active_tc, cs);
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    /*
     * CCall: Call into a new security domain
     */
    if (creg_inaccessible(perms, cs)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cs);
    } else if (creg_inaccessible(perms, cb)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cb);
    } else if (!csp->cr_tag) {
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
        if (selector == 0) {
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
            return csp->cr_base + csp->cr_offset;
        }
    }
    return (target_ulong)0;
}

void helper_ccall(CPUMIPSState *env, uint32_t cs, uint32_t cb)
{
    (void)ccall_common(env, cs, cb, 0);
}

target_ulong helper_ccall_notrap(CPUMIPSState *env, uint32_t cs, uint32_t cb)
{
    return ccall_common(env, cs, cb, 1);
}

void helper_cclearreg(CPUMIPSState *env, uint32_t mask)
{
    uint32_t perms = env->active_tc.PCC.cr_perms;

    // The creg_inaccessible check must be performed before changing any regs
    for (int creg = 0; creg < 32; creg++) {
        if ((mask & (0x1 << creg)) && creg_inaccessible(perms, creg)) {
            /* raise exception and bail without clearing registers */
            do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, creg);
            return;
        }
    }
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
    uint32_t perms = env->active_tc.PCC.cr_perms;
    const cap_register_t *csp = get_readonly_capreg(&env->active_tc, cs);
    uint32_t rt_perms = (uint32_t)rt & (CAP_PERMS_ALL);
    uint32_t rt_uperms = ((uint32_t)rt >> CAP_UPERMS_SHFT) & CAP_UPERMS_ALL;
    /*
     * CCheckPerm: Raise exception if don't have permission
     */
    if (creg_inaccessible(perms, cs)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cs);
    } else if (!csp->cr_tag) {
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
    uint32_t perms = env->active_tc.PCC.cr_perms;
    const cap_register_t *csp = get_readonly_capreg(&env->active_tc, cs);
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    /*
     * CCheckType: Raise exception if otypes don't match
     */
    if (creg_inaccessible(perms, cs)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cs);
    } else if (creg_inaccessible(perms, cb)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cb);
    } else if (!csp->cr_tag) {
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
    uint32_t perms = env->active_tc.PCC.cr_perms;
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    /*
     * CClearTag: Clear the tag bit
     */
    if (creg_inaccessible(perms, cd)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cd);
    } else if (creg_inaccessible(perms, cb)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cb);
    } else {
        cap_register_t result = *cbp;
        result.cr_tag = 0;
#ifdef CHERI_128
        /* Save the compressed bits at the moment the tag was invalidated. */
        result.cr_pesbt = compress_128cap(&result);
#endif /* CHERI_128 */
        update_capreg(&env->active_tc, cd, &result);
    }
}

void helper_cfromptr(CPUMIPSState *env, uint32_t cd, uint32_t cb,
        target_ulong rt)
{
#ifdef DO_CHERI_STATISTICS
    stat_num_cfromptr++;
#endif
    uint32_t perms = env->active_tc.PCC.cr_perms;
    // CFromPtr traps on cbp == NULL so we use reg0 as $ddc to save encoding
    // space (and for backwards compat with old binaries).
    // Note: This is also still required for new binaries since clang assumes it
    // can use zero as $ddc in cfromptr/ctoptr
    const cap_register_t *cbp = get_capreg_0_is_ddc(&env->active_tc, cb);
    /*
     * CFromPtr: Create capability from pointer
     */
    if (creg_inaccessible(perms, cd)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cd);
    } else if (creg_inaccessible(perms, cb)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cb);
    } else if (rt == (target_ulong)0) {
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
    uint32_t perms = env->active_tc.PCC.cr_perms;
    /*
     * CGetAddr: Move Virtual Address to a General-Purpose Register
     */
    if (creg_inaccessible(perms, cb)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cb);
        return (target_ulong)0;
    } else {
        const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
        return (target_ulong)(cbp->cr_base + cbp->cr_offset);
    }
}

target_ulong helper_cgetbase(CPUMIPSState *env, uint32_t cb)
{
    uint32_t perms = env->active_tc.PCC.cr_perms;
    /*
     * CGetBase: Move Base to a General-Purpose Register
     */
    if (creg_inaccessible(perms, cb)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cb);
        return (target_ulong)0;
    } else {
        return (target_ulong)get_readonly_capreg(&env->active_tc, cb)->cr_base;
    }
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
    uint32_t perms = env->active_tc.PCC.cr_perms;
    /*
     * CGetLen: Move Length to a General-Purpose Register
     */
    if (creg_inaccessible(perms, cb)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cb);
        return (target_ulong)0;
    } else {
        /* For 128-bit Capabilities we must check len >= 2^64 */
        return (target_ulong)get_readonly_capreg(&env->active_tc, cb)->cr_length;
    }
}

target_ulong helper_cgetoffset(CPUMIPSState *env, uint32_t cb)
{
    uint32_t perms = env->active_tc.PCC.cr_perms;
    /*
     * CGetOffset: Move Offset to a General-Purpose Register
     */
    if (creg_inaccessible(perms, cb)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cb);
    } else {
        // fprintf(qemu_logfile, "%s: offset(%d)=%016lx\n",
        //      __func__, cb, get_readonly_capreg(&env->active_tc, cb)->cr_offset);
        // return (target_ulong)(get_readonly_capreg(&env->active_tc, cb)->cr_cursor -
        //        get_readonly_capreg(&env->active_tc, cb)->cr_base);
        return (target_ulong)get_readonly_capreg(&env->active_tc, cb)->cr_offset;
    }
    return (target_ulong)0;
}

void helper_cgetpcc(CPUMIPSState *env, uint32_t cd)
{
    uint32_t perms = env->active_tc.PCC.cr_perms;

    /*
     * CGetPCC: Move PCC to capability register
     * See Chapter 4 in CHERI Architecture manual.
     */
    if (creg_inaccessible(perms, cd)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cd);
    } else {
        update_capreg(&env->active_tc, cd, &env->active_tc.PCC);
        /* Note that the offset(cursor) is updated by ccheck_pcc */
    }
}

void helper_cgetpccsetoffset(CPUMIPSState *env, uint32_t cd, target_ulong rs)
{
#ifdef DO_CHERI_STATISTICS
    stat_num_cgetpccsetoffset++;
#endif
    cap_register_t *pccp = &env->active_tc.PCC;
    uint32_t perms = pccp->cr_perms;
    /*
     * CGetPCCSetOffset: Get PCC with new offset
     * See Chapter 5 in CHERI Architecture manual.
     */
    if (creg_inaccessible(perms, cd)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cd);
    } else if (!is_representable(is_cap_sealed(pccp), pccp->cr_base,
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
    uint32_t perms = env->active_tc.PCC.cr_perms;
    /*
     * CGetPerm: Move Memory Permissions Field to a General-Purpose
     * Register
     */
    if (creg_inaccessible(perms, cb)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cb);
        return (target_ulong)0;
    } else {
        const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
        uint64_t perms =  (uint64_t)
            ((cbp->cr_perms & CAP_PERMS_ALL) |
             ((cbp->cr_uperms & CAP_UPERMS_ALL) << CAP_UPERMS_SHFT));

        return (target_ulong)perms;
    }
}

target_ulong helper_cgetsealed(CPUMIPSState *env, uint32_t cb)
{
    uint32_t perms = env->active_tc.PCC.cr_perms;
    /*
     * CGetSealed: Move sealed bit to a General-Purpose Register
     */
    if (creg_inaccessible(perms, cb)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cb);
        return (target_ulong)0;
    } else {
        return (target_ulong)(is_cap_sealed(get_readonly_capreg(&env->active_tc, cb)) ? 1 : 0);
    }
}

target_ulong helper_cgettag(CPUMIPSState *env, uint32_t cb)
{
    uint32_t perms = env->active_tc.PCC.cr_perms;
    /*
     * CGetTag: Move Tag to a General-Purpose Register
     */
    if (creg_inaccessible(perms, cb)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cb);
        return (target_ulong)0;
    } else {
        return (target_ulong)get_readonly_capreg(&env->active_tc, cb)->cr_tag;
    }
}

target_ulong helper_cgettype(CPUMIPSState *env, uint32_t cb)
{
    uint32_t perms = env->active_tc.PCC.cr_perms;
    /*
     * CGetType: Move Object Type Field to a General-Purpose Register
     */
    if (creg_inaccessible(perms, cb)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cb);
        return (target_ulong)0;
    } else {
        const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
        if (cbp->cr_sealed)
            return (target_ulong)(cbp->cr_otype & CAP_MAX_OTYPE);
        return (target_ulong)-1;
    }
}

void helper_cincbase(CPUMIPSState *env, uint32_t cd, uint32_t cb,
        target_ulong rt)
{
    uint32_t perms = env->active_tc.PCC.cr_perms;
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    /*
     * CIncBase: Increase Base
     */
    if (creg_inaccessible(perms, cd)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cd);
    } else if (creg_inaccessible(perms, cb)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cb);
    } else if (!cbp->cr_tag && rt != 0) {
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
    uint32_t perms = env->active_tc.PCC.cr_perms;
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    /*
     * CIncOffset: Increase Offset
     */
    if (creg_inaccessible(perms, cd)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cd);
    } else if (creg_inaccessible(perms, cb)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cb);
    } else if (cbp->cr_tag && is_cap_sealed(cbp) && rt != 0) {
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
    uint32_t perms = env->active_tc.PCC.cr_perms;
    const cap_register_t *csp = get_readonly_capreg(&env->active_tc, cs);
    /*
     * CMOVZ: conditionally move capability on zero
     */
    if (creg_inaccessible(perms, cd)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cd);
    } else if (creg_inaccessible(perms, cs)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cs);
    } else if (rs == 0) {
        update_capreg(&env->active_tc, cd, csp);
    }
}

void helper_cmovn(CPUMIPSState *env, uint32_t cd, uint32_t cs, target_ulong rs)
{
    helper_cmovz(env, cd, cs, rs == 0);
}

target_ulong helper_cjalr(CPUMIPSState *env, uint32_t cd, uint32_t cb)
{
    uint32_t perms = env->active_tc.PCC.cr_perms;
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    /*
     * CJALR: Jump and Link Capability Register
     */
    if (creg_inaccessible(perms, cd)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cd);
    } else if (creg_inaccessible(perms, cb)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cb);
    } else if (!cbp->cr_tag) {
        do_raise_c2_exception(env, CP2Ca_TAG, cb);
    } else if (is_cap_sealed(cbp)) {
        do_raise_c2_exception(env, CP2Ca_SEAL, cb);
    } else if (!(cbp->cr_perms & CAP_PERM_EXECUTE)) {
        do_raise_c2_exception(env, CP2Ca_PERM_EXE, cb);
    } else if (!(cbp->cr_perms & CAP_PERM_GLOBAL)) {
        do_raise_c2_exception(env, CP2Ca_GLOBAL, cb);
    } else if ((cbp->cr_offset + 4) > cbp->cr_length) {
        do_raise_c2_exception(env, CP2Ca_LENGTH, cb);
    } else if (align_of(4, (cbp->cr_base + cbp->cr_offset))) {
        do_raise_c0_exception(env, EXCP_AdEL, (cbp->cr_base + cbp->cr_offset));
    } else {
        cap_register_t result = env->active_tc.PCC;
        result.cr_offset += 8;
        update_capreg(&env->active_tc, cd, &result);
        // The capability register is loaded into PCC during delay slot
        // Return the branch target address
        return cbp->cr_offset + cbp->cr_base;
    }

    return (target_ulong)0;
}

target_ulong helper_cjr(CPUMIPSState *env, uint32_t cb)
{
    uint32_t perms = env->active_tc.PCC.cr_perms;
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    /*
     * CJR: Jump Capability Register
     */
    if (creg_inaccessible(perms, cb)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cb);
    } else if (!cbp->cr_tag) {
        do_raise_c2_exception(env, CP2Ca_TAG, cb);
    } else if (is_cap_sealed(cbp)) {
        do_raise_c2_exception(env, CP2Ca_SEAL, cb);
    } else if (!(cbp->cr_perms & CAP_PERM_EXECUTE)) {
        do_raise_c2_exception(env, CP2Ca_PERM_EXE, cb);
    } else if (!(cbp->cr_perms & CAP_PERM_GLOBAL)) {
        do_raise_c2_exception(env, CP2Ca_GLOBAL, cb);
    } else if ((cbp->cr_offset + 4) > cbp->cr_length) {
        do_raise_c2_exception(env, CP2Ca_LENGTH, cb);
    } else if (align_of(4, (cbp->cr_base + cbp->cr_offset))) {
        do_raise_c0_exception(env, EXCP_AdEL, (cbp->cr_base + cbp->cr_offset));
    } else {
        // The capability register is loaded into PCC during delay slot
        // Return the branch target address
        return cbp->cr_offset + cbp->cr_base;
    }

    return (target_ulong)0;
}

static void cseal_common(CPUMIPSState *env, uint32_t cd, uint32_t cs,
                         uint32_t ct, bool conditional)
{
    uint32_t perms = env->active_tc.PCC.cr_perms;
    const cap_register_t *csp = get_readonly_capreg(&env->active_tc, cs);
    const cap_register_t *ctp = get_readonly_capreg(&env->active_tc, ct);
    uint64_t ct_base_plus_offset = ctp->cr_base + ctp->cr_offset;
    /*
     * CSeal: Seal a capability
     */
    if (creg_inaccessible(perms, cd)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cd);
    } else if (creg_inaccessible(perms, cs)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cs);
    } else if (creg_inaccessible(perms, ct)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, ct);
    } else if (!csp->cr_tag) {
        do_raise_c2_exception(env, CP2Ca_TAG, cs);
    } else if (!ctp->cr_tag) {
        if (conditional)
            update_capreg(&env->active_tc, cd, csp);
        else
            do_raise_c2_exception(env, CP2Ca_TAG, ct);
    } else if (conditional && ctp->cr_base + ctp->cr_offset == -1) {
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
    uint32_t perms = env->active_tc.PCC.cr_perms;
    // CBuildCap traps on cbp == NULL so we use reg0 as $ddc. This saves encoding
    // space and also means a cbuildcap relative to $ddc can be one instr instead
    // of two.
    const cap_register_t *cbp = get_capreg_0_is_ddc(&env->active_tc, cb);
    const cap_register_t *ctp = get_readonly_capreg(&env->active_tc, ct);
    /*
     * CBuildCap: create capability from untagged register.
     * XXXAM: Note this is experimental and may change.
     */
    if (creg_inaccessible(perms, cd)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cd);
    } else if (creg_inaccessible(perms, cb)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cb);
    } else if (creg_inaccessible(perms, ct)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, ct);
    } else if (!cbp->cr_tag) {
        do_raise_c2_exception(env, CP2Ca_TAG, cb);
    } else if (is_cap_sealed(cbp)) {
        do_raise_c2_exception(env, CP2Ca_SEAL, cb);
    } else if (ctp->cr_base < cbp->cr_base) {
        do_raise_c2_exception(env, CP2Ca_LENGTH, cb);
    } else if (ctp->cr_base + ctp->cr_length > cbp->cr_base + cbp->cr_length) {
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
    uint32_t perms = env->active_tc.PCC.cr_perms;
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    const cap_register_t *ctp = get_readonly_capreg(&env->active_tc, ct);
    /*
     * CCopyType: copy object type from untagged capability.
     * XXXAM: Note this is experimental and may change.
     */
    if (creg_inaccessible(perms, cd)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cd);
    } else if (creg_inaccessible(perms, cb)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cb);
    } else if (creg_inaccessible(perms, ct)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, ct);
    } else if (!cbp->cr_tag) {
        do_raise_c2_exception(env, CP2Ca_TAG, cb);
    } else if (is_cap_sealed(cbp)) {
        do_raise_c2_exception(env, CP2Ca_SEAL, cb);
    } else if (!is_cap_sealed(ctp)) {
        cap_register_t result;
        update_capreg(&env->active_tc, cd, int_to_cap(-1, &result));
    } else if (ctp->cr_otype < cbp->cr_base) {
        do_raise_c2_exception(env, CP2Ca_LENGTH, cb);
    } else if (ctp->cr_otype >= cbp->cr_base + cbp->cr_length) {
        do_raise_c2_exception(env, CP2Ca_LENGTH, cb);
    } else {
        cap_register_t result = *cbp;
        result.cr_offset = ctp->cr_otype - cbp->cr_base;
        update_capreg(&env->active_tc, cd, &result);
    }
}


static inline bool in_kernel_mode(CPUMIPSState *env) {
    // TODO: what about env->CP0_Debug & (1 << CP0DB_DM)
    // If ERL or EXL is set we have taken an exception and are in the kernel
    if ((env->CP0_Status & BIT(CP0St_ERL)) || (env->CP0_Status & BIT(CP0St_EXL))) {
        return true;
    }
    uint32_t ksu = extract32(env->CP0_Status, CP0St_KSU, 2);
    // KSU = 0 -> kernel, 1 -> supervisor, 2 -> user
    if (ksu == 0 || ksu == 1) {
        return true;
    }
    return false;
}

static inline cap_register_t *
check_writable_cap_hwr_access(CPUMIPSState *env, enum CP2HWR hwr) {
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
    // XXXAR: Must use do_raise_c0_exception and not do_raise_exception here!
    do_raise_c0_exception(env, EXCP_RI, 0);
    return NULL;  // silence warning
}

static inline const cap_register_t *
check_readonly_cap_hwr_access(CPUMIPSState *env, enum CP2HWR hwr) {
    // Currently there is no difference for access permissions between read
    // and write access but that may change in the future
    return check_writable_cap_hwr_access(env, hwr);
}

void helper_creadhwr(CPUMIPSState *env, uint32_t cd, uint32_t hwr)
{
    uint32_t perms = env->active_tc.PCC.cr_perms;
    if (creg_inaccessible(perms, cd)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cd);
        return;
    }
    const cap_register_t *csp = check_readonly_cap_hwr_access(env, hwr);
    update_capreg(&env->active_tc, cd, csp);
}

void helper_cwritehwr(CPUMIPSState *env, uint32_t cs, uint32_t hwr)
{
    uint32_t perms = env->active_tc.PCC.cr_perms;
    if (creg_inaccessible(perms, cs)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cs);
        return;
    }
    const cap_register_t *csp = get_readonly_capreg(&env->active_tc, cs);
    cap_register_t *cdp = check_writable_cap_hwr_access(env, hwr);
    *cdp = *csp;
}

void helper_csetbounds(CPUMIPSState *env, uint32_t cd, uint32_t cb,
        target_ulong rt)
{
    uint32_t perms = env->active_tc.PCC.cr_perms;
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    uint64_t cursor = cbp->cr_base + cbp->cr_offset;
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
          E = compute_e(rt, BWidth);
        }
    } else {
        E = compute_e(rt, BWidth);
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
    if (creg_inaccessible(perms, cd)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cd);
    } else if (creg_inaccessible(perms, cb)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cb);
    } else if (!cbp->cr_tag) {
        do_raise_c2_exception(env, CP2Ca_TAG, cb);
    } else if (is_cap_sealed(cbp)) {
        do_raise_c2_exception(env, CP2Ca_SEAL, cb);
    } else if (cursor < cbp->cr_base) {
        do_raise_c2_exception(env, CP2Ca_LENGTH, cb);
    } else if (cursor_rt < rt) {
        /* cursor + rt overflowed */
        do_raise_c2_exception(env, CP2Ca_LENGTH, cb);
    } else if (cursor_rt > (cbp->cr_base + cbp->cr_length)) {
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
    uint32_t perms = env->active_tc.PCC.cr_perms;
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    uint64_t cursor = cbp->cr_base + cbp->cr_offset;
    uint64_t cursor_rt = cursor + rt;

#ifdef CHERI_128
    uint32_t bwidth = CC_L_BWIDTH;
    uint8_t e = compute_e(rt, bwidth);
    uint8_t need_zeros = e ? e + CC_L_LOWWIDTH : 0;
    uint64_t mask = (1ULL << need_zeros) - 1;

    bool representable = ((cursor | cursor_rt) & mask) == 0;
#else
    bool representable = true;
#endif

    /*
     * CSetBoundsExact: Set Bounds Exactly
     */
    if (creg_inaccessible(perms, cd)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cd);
    } else if (creg_inaccessible(perms, cb)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cb);
    } else if (!cbp->cr_tag) {
        do_raise_c2_exception(env, CP2Ca_TAG, cb);
    } else if (is_cap_sealed(cbp)) {
        do_raise_c2_exception(env, CP2Ca_SEAL, cb);
    } else if (cursor < cbp->cr_base) {
        do_raise_c2_exception(env, CP2Ca_LENGTH, cb);
    } else if (cursor_rt > (cbp->cr_base + cbp->cr_length)) {
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

target_ulong helper_csub(CPUMIPSState *env, uint32_t cb, uint32_t ct)
{
    uint32_t perms = env->active_tc.PCC.cr_perms;
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    const cap_register_t *ctp = get_readonly_capreg(&env->active_tc, ct);
    /*
     * CSub: Subtract Capabilities
     */
    if (creg_inaccessible(perms, cb)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cb);
        return (target_ulong)0;
    } else if (creg_inaccessible(perms, ct)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, ct);
        return (target_ulong)0;
    } else {
        return (target_ulong)(cbp->cr_base + cbp->cr_offset -
                ctp->cr_base - ctp->cr_offset);
    }
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
    uint32_t perms = env->active_tc.PCC.cr_perms;
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    /*
     * CSetLen: Set Length
     */
    if (creg_inaccessible(perms, cd)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cd);
    } else if (creg_inaccessible(perms, cb)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cb);
    } else if (!cbp->cr_tag) {
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
    uint32_t perms = env->active_tc.PCC.cr_perms;
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    /*
     * CSetOffset: Set cursor to an offset from base
     */
    if (creg_inaccessible(perms, cd)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cd);
    } else if (creg_inaccessible(perms, cb)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cb);
    } else if (cbp->cr_tag && is_cap_sealed(cbp)) {
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
    uint32_t perms = env->active_tc.PCC.cr_perms;
    // CToPtr traps on ctp == NULL so we use reg0 as $ddc there. This means we
    // can have a CToPtr relative to $ddc as one instruction instead of two and
    // is required since clang still assumes it can use zero as $ddc in cfromptr/ctoptr
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    const cap_register_t *ctp = get_capreg_0_is_ddc(&env->active_tc, ct);
    uint64_t cb_cursor = cbp->cr_base + cbp->cr_offset;
    uint64_t ct_top = ctp->cr_base + ctp->cr_length;
    /*
     * CToPtr: Capability to Pointer
     */
    if (creg_inaccessible(perms, cb)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cb);
    } else if (creg_inaccessible(perms, ct)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, ct);
    } else if (!ctp->cr_tag) {
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
    uint32_t perms = env->active_tc.PCC.cr_perms;
    const cap_register_t *csp = get_readonly_capreg(&env->active_tc, cs);
    const cap_register_t *ctp = get_readonly_capreg(&env->active_tc, ct);
    /*
     * CUnseal: Unseal a sealed capability
     */
    if (creg_inaccessible(perms, cd)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cd);
    } else if (creg_inaccessible(perms, cs)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cs);
    } else if (creg_inaccessible(perms, ct)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, ct);
    } else if (!csp->cr_tag) {
        do_raise_c2_exception(env, CP2Ca_TAG, cs);
    } else if (!ctp->cr_tag) {
        do_raise_c2_exception(env, CP2Ca_TAG, ct);
    } else if (!is_cap_sealed(csp)) {
        do_raise_c2_exception(env, CP2Ca_SEAL, cs);
    } else if (is_cap_sealed(ctp)) {
        do_raise_c2_exception(env, CP2Ca_SEAL, ct);
    } else if ((ctp->cr_base + ctp->cr_offset) != csp->cr_otype) {
        do_raise_c2_exception(env, CP2Ca_TYPE, ct);
    } else if (!(ctp->cr_perms & CAP_PERM_UNSEAL)) {
        do_raise_c2_exception(env, CP2Ca_PERM_UNSEAL, ct);
    } else if (ctp->cr_offset >= ctp->cr_length) {
        do_raise_c2_exception(env, CP2Ca_LENGTH, ct);
    } else if ((ctp->cr_base + ctp->cr_offset) >= CAP_MAX_OTYPE) {
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
    uint32_t perms = env->active_tc.PCC.cr_perms;
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    const cap_register_t *ctp = get_readonly_capreg(&env->active_tc, ct);
    gboolean equal = FALSE;
    /*
     * CEQ: Capability pointers equal
     */
    if (creg_inaccessible(perms, cb)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cb);
    } else if (creg_inaccessible(perms, ct)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, ct);
    } else {
        if (cbp->cr_tag != ctp->cr_tag) {
            equal = FALSE;
        } else {
            uint64_t cursor1 = cbp->cr_base + cbp->cr_offset;
            uint64_t cursor2 = ctp->cr_base + ctp->cr_offset;

            equal = (cursor1 == cursor2);
        }
    }
    return (target_ulong) equal;
}

target_ulong helper_cne(CPUMIPSState *env, uint32_t cb, uint32_t ct)
{
    uint32_t perms = env->active_tc.PCC.cr_perms;
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    const cap_register_t *ctp = get_readonly_capreg(&env->active_tc, ct);
    gboolean equal = FALSE;
    /*
     * CNE: Capability pointers not equal
     */
    if (creg_inaccessible(perms, cb)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cb);
    } else if (creg_inaccessible(perms, ct)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, ct);
    } else {
        if (cbp->cr_tag != ctp->cr_tag) {
            equal = FALSE;
        } else {
            uint64_t cursor1 = cbp->cr_base + cbp->cr_offset;
            uint64_t cursor2 = ctp->cr_base + ctp->cr_offset;

            equal = (cursor1 == cursor2);
        }
    }
    return (target_ulong) !equal;
}

target_ulong helper_clt(CPUMIPSState *env, uint32_t cb, uint32_t ct)
{
    uint32_t perms = env->active_tc.PCC.cr_perms;
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    const cap_register_t *ctp = get_readonly_capreg(&env->active_tc, ct);
    gboolean signed_less = FALSE;
    /*
     * CLT: Capability pointers less than (signed)
     */
    if (creg_inaccessible(perms, cb)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cb);
    } else if (creg_inaccessible(perms, ct)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, ct);
    } else {
        if (cbp->cr_tag != ctp->cr_tag) {
            if (cbp->cr_tag) {
                signed_less = FALSE;
            } else {
                signed_less = TRUE;
            }
        } else {
            int64_t cursor1 = (int64_t)(cbp->cr_base + cbp->cr_offset);
            int64_t cursor2 = (int64_t)(ctp->cr_base + ctp->cr_offset);

            signed_less = (cursor1 < cursor2);
        }
    }
    return (target_ulong) signed_less;
}

target_ulong helper_cle(CPUMIPSState *env, uint32_t cb, uint32_t ct)
{
    uint32_t perms = env->active_tc.PCC.cr_perms;
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    const cap_register_t *ctp = get_readonly_capreg(&env->active_tc, ct);
    gboolean signed_lte = FALSE;
    /*
     * CLE: Capability pointers less than equal (signed)
     */
    if (creg_inaccessible(perms, cb)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cb);
    } else if (creg_inaccessible(perms, ct)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, ct);
    } else {
        if (cbp->cr_tag != ctp->cr_tag) {
            if (cbp->cr_tag) {
                signed_lte = FALSE;
            } else {
                signed_lte = TRUE;
            }
        } else {
            int64_t cursor1 = (int64_t)(cbp->cr_base + cbp->cr_offset);
            int64_t cursor2 = (int64_t)(ctp->cr_base + ctp->cr_offset);

            signed_lte = (cursor1 <= cursor2);
        }
    }
    return (target_ulong) signed_lte;
}

target_ulong helper_cltu(CPUMIPSState *env, uint32_t cb, uint32_t ct)
{
    uint32_t perms = env->active_tc.PCC.cr_perms;
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    const cap_register_t *ctp = get_readonly_capreg(&env->active_tc, ct);
    gboolean ltu = FALSE;
    /*
     * CLTU: Capability pointers less than (unsigned)
     */
    if (creg_inaccessible(perms, cb)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cb);
    } else if (creg_inaccessible(perms, ct)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, ct);
    } else {
        if (cbp->cr_tag != ctp->cr_tag) {
            if (cbp->cr_tag) {
                ltu = FALSE;
            } else {
                ltu = TRUE;
            }
        } else {
            uint64_t cursor1 = cbp->cr_base + cbp->cr_offset;
            uint64_t cursor2 = ctp->cr_base + ctp->cr_offset;

            ltu = (cursor1 < cursor2);
        }
    }
    return (target_ulong) ltu;
}

target_ulong helper_cleu(CPUMIPSState *env, uint32_t cb, uint32_t ct)
{
    uint32_t perms = env->active_tc.PCC.cr_perms;
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    const cap_register_t *ctp = get_readonly_capreg(&env->active_tc, ct);
    gboolean leu = FALSE;
    /*
     * CLEU: Capability pointers less than equal (unsigned)
     */
    if (creg_inaccessible(perms, cb)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cb);
    } else if (creg_inaccessible(perms, ct)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, ct);
    } else {
        if (cbp->cr_tag != ctp->cr_tag) {
            if (cbp->cr_tag) {
                leu = FALSE;
            } else {
                leu = TRUE;
            }
        } else {
            uint64_t cursor1 = cbp->cr_base + cbp->cr_offset;
            uint64_t cursor2 = ctp->cr_base + ctp->cr_offset;

            leu = (cursor1 <= cursor2);
        }
    }
    return (target_ulong) leu;
}

target_ulong helper_cexeq(CPUMIPSState *env, uint32_t cb, uint32_t ct)
{
    uint32_t perms = env->active_tc.PCC.cr_perms;
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    const cap_register_t *ctp = get_readonly_capreg(&env->active_tc, ct);
    gboolean equal = FALSE;
    /*
     * CEXEQ: Capability pointers equal (all fields)
     */
    if (creg_inaccessible(perms, cb)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cb);
    } else if (creg_inaccessible(perms, ct)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, ct);
    } else {
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
    uint32_t perms = env->active_tc.PCC.cr_perms;
    const cap_register_t *cbp = get_readonly_capreg(&env->active_tc, cb);
    const cap_register_t *ctp = get_readonly_capreg(&env->active_tc, ct);
    gboolean is_subset = FALSE;
    /*
     * CTestSubset: Test if capability is a subset of another
     */
    if (creg_inaccessible(perms, cb)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cb);
    } else if (creg_inaccessible(perms, ct)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, ct);
    } else {
        if (cbp->cr_tag == ctp->cr_tag &&
            /* is_cap_sealed(cbp) == is_cap_sealed(ctp) && */
            cbp->cr_base <= ctp->cr_base &&
            ctp->cr_base + ctp->cr_length <= cbp->cr_base + cbp->cr_length &&
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
    uint32_t perms = env->active_tc.PCC.cr_perms;
    // CL[BHWD][U] traps on cbp == NULL so we use reg0 as $ddc to save encoding
    // space and increase code density since loading relative to $ddc is common
    // in the hybrid ABI (and also for backwards compat with old binaries).
    const cap_register_t *cbp = get_capreg_0_is_ddc(&env->active_tc, cb);

    if (creg_inaccessible(perms, cb)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cb);
    } else if (!cbp->cr_tag) {
        do_raise_c2_exception(env, CP2Ca_TAG, cb);
    } else if (is_cap_sealed(cbp)) {
        do_raise_c2_exception(env, CP2Ca_SEAL, cb);
    } else if (!(cbp->cr_perms & CAP_PERM_LOAD)) {
        do_raise_c2_exception(env, CP2Ca_PERM_LD, cb);
    } else {
        uint64_t cursor = cbp->cr_base + cbp->cr_offset;
        uint64_t addr = cursor + rt + (int32_t)offset;

        if ((addr + size) > (cbp->cr_base + cbp->cr_length)) {
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
    uint32_t perms = env->active_tc.PCC.cr_perms;
    // CLL[BHWD][U] traps on cbp == NULL so we use reg0 as $ddc to save encoding
    // space and increase code density since loading relative to $ddc is common
    // in the hybrid ABI (and also for backwards compat with old binaries).
    const cap_register_t *cbp = get_capreg_0_is_ddc(&env->active_tc, cb);
    uint64_t addr = cbp->cr_base + cbp->cr_offset;

    env->linkedflag = 0;
    if (creg_inaccessible(perms, cb)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cb);
    } else if (!cbp->cr_tag) {
        do_raise_c2_exception(env, CP2Ca_TAG, cb);
    } else if (is_cap_sealed(cbp)) {
        do_raise_c2_exception(env, CP2Ca_SEAL, cb);
    } else if (!(cbp->cr_perms & CAP_PERM_LOAD)) {
        do_raise_c2_exception(env, CP2Ca_PERM_LD, cb);
    } else if ((addr + size) > (cbp->cr_base + cbp->cr_length)) {
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
    uint32_t perms = env->active_tc.PCC.cr_perms;
    // CSC[BHWD] traps on cbp == NULL so we use reg0 as $ddc to save encoding
    // space and increase code density since storing relative to $ddc is common
    // in the hybrid ABI (and also for backwards compat with old binaries).
    const cap_register_t *cbp = get_capreg_0_is_ddc(&env->active_tc, cb);
    uint64_t addr = cbp->cr_base + cbp->cr_offset;

    if (creg_inaccessible(perms, cb)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cb);
    } else if (!cbp->cr_tag) {
        do_raise_c2_exception(env, CP2Ca_TAG, cb);
    } else if (is_cap_sealed(cbp)) {
        do_raise_c2_exception(env, CP2Ca_SEAL, cb);
    } else if (!(cbp->cr_perms & CAP_PERM_STORE)) {
        do_raise_c2_exception(env, CP2Ca_PERM_ST, cb);
    } else if ((addr + size) > (cbp->cr_base + cbp->cr_length)) {
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
    uint32_t perms = env->active_tc.PCC.cr_perms;
    // CS[BHWD][U] traps on cbp == NULL so we use reg0 as $ddc to save encoding
    // space and increase code density since storing relative to $ddc is common
    // in the hybrid ABI (and also for backwards compat with old binaries).
    const cap_register_t *cbp = get_capreg_0_is_ddc(&env->active_tc, cb);

    if (creg_inaccessible(perms, cb)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cb);
    } else if (!cbp->cr_tag) {
        do_raise_c2_exception(env, CP2Ca_TAG, cb);
    } else if (is_cap_sealed(cbp)) {
        do_raise_c2_exception(env, CP2Ca_SEAL, cb);
    } else if (!(cbp->cr_perms & CAP_PERM_STORE)) {
        do_raise_c2_exception(env, CP2Ca_PERM_ST, cb);
    } else {
        uint64_t cursor = cbp->cr_base + cbp->cr_offset;
        uint64_t addr = cursor + rt + (int32_t)offset;

        if ((addr + size) > (cbp->cr_base + cbp->cr_length)) {
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
    uint32_t perms = env->active_tc.PCC.cr_perms;
    // CLC traps on cbp == NULL so we use reg0 as $ddc to save encoding
    // space and increase code density since loading relative to $ddc is common
    // in the hybrid ABI (and also for backwards compat with old binaries).
    const cap_register_t *cbp = get_capreg_0_is_ddc(&env->active_tc, cb);

    if (creg_inaccessible(perms, cd)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cd);
        return (target_ulong)0;
    } else if (creg_inaccessible(perms, cb)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cb);
        return (target_ulong)0;
    } else if (!cbp->cr_tag) {
        do_raise_c2_exception(env, CP2Ca_TAG, cb);
        return (target_ulong)0;
    } else if (is_cap_sealed(cbp)) {
        do_raise_c2_exception(env, CP2Ca_SEAL, cb);
        return (target_ulong)0;
    } else if (!(cbp->cr_perms & CAP_PERM_LOAD)) {
        do_raise_c2_exception(env, CP2Ca_PERM_LD, cb);
        return (target_ulong)0;
    } else {
        uint64_t cursor = cbp->cr_base + cbp->cr_offset;
        uint64_t addr = (uint64_t)((cursor + rt) + (int32_t)offset);
        /* uint32_t tag = cheri_tag_get(env, addr, cd, NULL); */

        if ((addr + CHERI_CAP_SIZE) > (cbp->cr_base + cbp->cr_length)) {
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
    uint32_t perms = env->active_tc.PCC.cr_perms;
    // CLLC traps on cbp == NULL so we use reg0 as $ddc to save encoding
    // space and increase code density since loading relative to $ddc is common
    // in the hybrid ABI (and also for backwards compat with old binaries).
    const cap_register_t *cbp = get_capreg_0_is_ddc(&env->active_tc, cb);
    uint64_t addr = cbp->cr_base + cbp->cr_offset;

    env->linkedflag = 0;
    if (creg_inaccessible(perms, cd)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cd);
        return (target_ulong)0;
    } else if (creg_inaccessible(perms, cb)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cb);
        return (target_ulong)0;
    } else if (!cbp->cr_tag) {
        do_raise_c2_exception(env, CP2Ca_TAG, cb);
        return (target_ulong)0;
    } else if (is_cap_sealed(cbp)) {
        do_raise_c2_exception(env, CP2Ca_SEAL, cb);
        return (target_ulong)0;
    } else if (!(cbp->cr_perms & CAP_PERM_LOAD)) {
        do_raise_c2_exception(env, CP2Ca_PERM_LD, cb);
        return (target_ulong)0;
    } else if ((addr + CHERI_CAP_SIZE) > (cbp->cr_base + cbp->cr_length)) {
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
    uint32_t perms = env->active_tc.PCC.cr_perms;
    // CSC traps on cbp == NULL so we use reg0 as $ddc to save encoding
    // space and increase code density since storing relative to $ddc is common
    // in the hybrid ABI (and also for backwards compat with old binaries).
    const cap_register_t *cbp = get_capreg_0_is_ddc(&env->active_tc, cb);
    const cap_register_t *csp = get_readonly_capreg(&env->active_tc, cs);

    if (creg_inaccessible(perms, cs)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cs);
        return (target_ulong)0;
    } else if (creg_inaccessible(perms, cb)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cb);
        return (target_ulong)0;
    } else if (!cbp->cr_tag) {
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
        uint64_t cursor = cbp->cr_base + cbp->cr_offset;
        uint64_t addr = (uint64_t)((int64_t)(cursor + rt) + (int32_t)offset);

        if ((addr + CHERI_CAP_SIZE) > (cbp->cr_base + cbp->cr_length)) {
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
    uint32_t perms = env->active_tc.PCC.cr_perms;
    // CSCC traps on cbp == NULL so we use reg0 as $ddc to save encoding
    // space and increase code density since storing relative to $ddc is common
    // in the hybrid ABI (and also for backwards compat with old binaries).
    const cap_register_t *cbp = get_capreg_0_is_ddc(&env->active_tc, cb);
    const cap_register_t *csp = get_readonly_capreg(&env->active_tc, cs);
    uint64_t addr = cbp->cr_base + cbp->cr_offset;

    if (creg_inaccessible(perms, cs)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cs);
        return (target_ulong)0;
    } else if (creg_inaccessible(perms, cb)) {
        do_raise_c2_exception(env, CP2Ca_ACCESS_SYS_REGS, cb);
        return (target_ulong)0;
    } else if (!cbp->cr_tag) {
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
    } else if ((addr + CHERI_CAP_SIZE) > (cbp->cr_base + cbp->cr_length)) {
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

extern int cl_default_trace_format;


#define USER_TRACE_DEBUG 0
#if USER_TRACE_DEBUG
#define user_trace_dbg(...) qemu_log("=== " __VA_ARGS__)
#else
#define user_trace_dbg(...)
#endif

/* Start instruction trace logging. */
void helper_instr_start(CPUMIPSState *env, target_ulong pc)
{
    env->trace_explicitly_disabled = false;
    /* Don't turn on tracing if user-mode only is selected and we are in the kernel */
    if (env->user_only_tracing_enabled && !IN_USERSPACE(env)) {
        assert(qemu_loglevel_mask(CPU_LOG_USER_ONLY));
        user_trace_dbg("Delaying tracing request at 0x%lx "
            "until next switch to user mode, ASID %lu\n",
            pc, env->CP0_EntryHi & 0xFF);
        env->tracing_suspended = true;
    } else {
        qemu_set_log(qemu_loglevel | cl_default_trace_format);
        user_trace_dbg("Switching on tracing @ 0x%lx ASID %lu\n",
            pc, env->CP0_EntryHi & 0xFF);
        env->tracing_suspended = false;
    }
}

/* Stop instruction trace logging. */
void helper_instr_stop(CPUMIPSState *env, target_ulong pc)
{
    user_trace_dbg("Switching off tracing @ 0x%lx ASID %lu\n",
        pc, env->CP0_EntryHi & 0xFF);
    qemu_set_log(qemu_loglevel & ~cl_default_trace_format);
    /* Make sure a kernel -> user switch does not turn on tracing */
    env->tracing_suspended = false;
    /* don't turn on on next kernel -> userspace change */
    env->trace_explicitly_disabled = true;
}

/* Set instruction trace logging to user mode only. */
void helper_instr_start_user_mode_only(CPUMIPSState *env, target_ulong pc)
{
    /*
     * Make sure that qemu_loglevel doesn't get set to zero when we
     * suspend tracing because otherwise qemu will close the logfile.
     */
    qemu_set_log(qemu_loglevel | CPU_LOG_USER_ONLY);
    user_trace_dbg("User-mode only tracing enabled at 0x%lx, ASID %lu\n",
        pc, env->CP0_EntryHi & 0xFF);
    env->user_only_tracing_enabled = true;
    /* Disable tracing if we are not currently in user mode */
    if (!IN_USERSPACE(env)) {
        qemu_set_log(qemu_loglevel & ~cl_default_trace_format);
        env->tracing_suspended = true;
    } else {
        env->tracing_suspended = false;
    }
}

/* Stop instruction trace logging to user mode only. */
void helper_instr_stop_user_mode_only(CPUMIPSState *env, target_ulong pc)
{
    /* Disable user-mode only and restore the previous tracing level */
    if (env->tracing_suspended && !env->trace_explicitly_disabled) {
        user_trace_dbg("User-only trace turned off -> Restoring old trace level"
            " at 0x%lx, ASID %lu\n", pc, env->CP0_EntryHi & 0xFF);
        qemu_set_log(qemu_loglevel | cl_default_trace_format);
    }
    env->tracing_suspended = false;
    env->user_only_tracing_enabled = false;
    user_trace_dbg("User-mode only tracing disabled at 0x%lx, ASID %lu\n",
        pc, env->CP0_EntryHi & 0xFF);
    qemu_set_log(qemu_loglevel & ~CPU_LOG_USER_ONLY);
}

static void do_hexdump(FILE* f, uint8_t* buffer, target_ulong length, target_ulong vaddr) {
    char ascii_chars[17] = { 0 };
    target_ulong line_start = vaddr & ~0xf;
    target_ulong addr;

    /* print leading empty space to always start with an aligned address */
    if (line_start != vaddr) {
        fprintf(f, "    " TARGET_FMT_lx" : ", line_start);
        for (addr = line_start; addr < vaddr; addr++) {
            if ((addr % 4) == 0) {
                fprintf(f, "   ");
            } else {
                fprintf(f, "  ");
            }
            ascii_chars[addr % 16] = ' ';
        }
    }
    ascii_chars[16] = '\0';
    for (addr = vaddr; addr < vaddr + length; addr++) {
        if ((addr % 16) == 0) {
            fprintf(f, "    " TARGET_FMT_lx ": ", line_start);
        }
        if ((addr % 4) == 0) {
            fprintf(f, " ");
        }
        unsigned char c = (unsigned char)buffer[addr - vaddr];
        fprintf(f, "%02x", c);
        ascii_chars[addr % 16] = isprint(c) ? c : '.';
        if ((addr % 16) == 15) {
            fprintf(f, "  %s\n", ascii_chars);
            line_start += 16;
        }
    }
    if (line_start != vaddr + length) {
        const target_ulong hexdump_end_addr = (vaddr + length) | 0xf;
        for (addr = vaddr + length; addr <= hexdump_end_addr; addr++) {
            if ((addr % 4) == 0) {
                fprintf(f, "   ");
            } else {
                fprintf(f, "  ");
            }
            ascii_chars[addr % 16] = ' ';
        }
        fprintf(f, "  %s\n", ascii_chars);
    }
}


/* Stop instruction trace logging to user mode only. */
void helper_cheri_debug_message(struct CPUMIPSState* env, uint64_t pc)
{
    uint32_t mode = qemu_loglevel & (CPU_LOG_CVTRACE | CPU_LOG_INSTR);
    if (!mode && env->tracing_suspended) {
        /* Always print these messages even if user-space only tracing is on */
        mode = cl_default_trace_format;
    }
    if (!mode) {
        return;
    }
    uint8_t buffer[4096];
    /* Address loaded from a0, length from a1, print mode in a2 */
    typedef enum _PrintMode {
        DEBUG_MESSAGE_CSTRING = 0,
        DEBUG_MESSAGE_HEXDUMP = 1
    } PrintMode;
    target_ulong vaddr = env->active_tc.gpr[4];
    target_ulong length = MIN(sizeof(buffer), env->active_tc.gpr[5]);
    PrintMode print_mode = (PrintMode)env->active_tc.gpr[6];

    int ret = cpu_memory_rw_debug(ENV_GET_CPU(env), vaddr, buffer, sizeof(buffer), false);
    if (ret != 0) {
        warn_report("CHERI DEBUG HELPER: Could not write " TARGET_FMT_ld
                    " bytes at vaddr 0x" TARGET_FMT_lx "\n", length, vaddr);
    }
    if (mode & CPU_LOG_INSTR) {
        qemu_log("DEBUG MESSAGE @ 0x" TARGET_FMT_lx "\n", pc);
        if (print_mode == DEBUG_MESSAGE_CSTRING) {
            /* XXXAR: Escape newlines, etc.? */
            qemu_log("    message = \"%s\"\n", buffer);
        } else if (print_mode == DEBUG_MESSAGE_HEXDUMP) {
            qemu_log("   Dumping " TARGET_FMT_lu " bytes starting at 0x"
                     TARGET_FMT_lx "\n", length, vaddr);
            do_hexdump(qemu_logfile, buffer, length, vaddr);
        }
    } else if (mode & CPU_LOG_CVTRACE) {
        warn_report("NOT IMPLEMENTED: CVTRACE debug message nop at 0x"
                    TARGET_FMT_lx "\n", pc);
    } else {
        assert(false && "logic error");
    }
}

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

/*
 * dump non-capability data to cvtrace entry
 */
static inline void cvtrace_dump_gpr_ldst(cvtrace_t *cvtrace, uint8_t version,
        uint64_t addr, uint64_t value)
{
    if (qemu_loglevel_mask(CPU_LOG_CVTRACE)) {
        cvtrace->version = version;
        cvtrace->val1 = tswap64(addr);
        cvtrace->val2 = tswap64(value);
    }
}
#define cvtrace_dump_gpr_load(trace, addr, val)          \
    cvtrace_dump_gpr_ldst(trace, CVT_LD_GPR, addr, val)
#define cvtrace_dump_gpr_store(trace, addr, val)         \
    cvtrace_dump_gpr_ldst(trace, CVT_ST_GPR, addr, val)

static inline void cvtrace_dump_gpr(cvtrace_t *cvtrace, uint64_t value)
{
    if (qemu_loglevel_mask(CPU_LOG_CVTRACE)) {
        if (cvtrace->version == CVT_NO_REG)
            cvtrace->version = CVT_GPR;
        cvtrace->val2 = tswap64(value);
    }
}

#ifdef CHERI_128
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


target_ulong helper_bytes2cap_128_tag_get(CPUMIPSState *env, uint32_t cd,
        uint32_t cb, target_ulong addr)
{
    /* This could be done in helper_bytes2cap_128 but TCG limits the number
     * of arguments to 5 so we have to have a separate helper to handle the tag.
     */
    // Since this is used by cl* we need to treat cb == 0 as $ddc
    const cap_register_t *cbp = get_capreg_0_is_ddc(&env->active_tc, cb);
    target_ulong tag = cheri_tag_get(env, addr, cd, NULL);

    if (env->TLB_L || !(cbp->cr_perms & CAP_PERM_LOAD_CAP))
        tag = 0;
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
    /* Log memory read, if needed. */
    dump_cap_load(addr, cdp->cr_pesbt, cursor, tag);
    cvtrace_dump_cap_load(&env->cvtrace, addr, cdp);
    cvtrace_dump_cap_cbl(&env->cvtrace, cdp);
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

    /* Log memory cap write, if needed. */
    dump_cap_store(vaddr, ret, csp->cr_offset + csp->cr_base, csp->cr_tag);
    cvtrace_dump_cap_store(&env->cvtrace, vaddr, csp);
    cvtrace_dump_cap_cbl(&env->cvtrace, csp);

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
        cheri_tag_set(env, vaddr, cs);
    else
        cheri_tag_invalidate(env, vaddr, CHERI_CAP_SIZE);

    env->hflags = save_hflags;

    return ret;
}

#elif defined(CHERI_MAGIC128)
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

void helper_bytes2cap_m128(CPUMIPSState *env, uint32_t cd, target_ulong base,
                           target_ulong cursor, target_ulong addr)
{
    uint64_t tps, length;
    cap_register_t *cdp = get_writable_capreg_raw(&env->active_tc, cd);
    /* fetch tps and length */
    cheri_tag_get_m128(env, addr, cd, &tps, &length);

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

    uint32_t tag = cheri_tag_get_m128(env, addr, cd, &tps, &length);

    if (env->TLB_L || !(cbp->cr_perms & CAP_PERM_LOAD_CAP))
        tag = 0;
    cdp->cr_tag = tag;

    /* Log memory read, if needed. */
    dump_cap_load(addr, cursor, cdp->cr_base, tag);
    cvtrace_dump_cap_load(&env->cvtrace, addr, cdp);
    cvtrace_dump_cap_cbl(&env->cvtrace, cdp);
}

target_ulong helper_cap2bytes_m128c(CPUMIPSState *env, uint32_t cs,
        target_ulong vaddr)
{
    const cap_register_t *csp = get_readonly_capreg(&env->active_tc, cs);
    target_ulong ret;

    ret = csp->cr_offset + csp->cr_base;

    /* Log memory cap write, if needed. */
    dump_cap_store(vaddr, ret, csp->cr_base, csp->cr_tag);
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

    cheri_tag_set_m128(env, vaddr, cs, csp->cr_tag, tps, length);

    /* Log memory cap write, if needed. */
    cvtrace_dump_cap_store(&env->cvtrace, vaddr, csp);
    cvtrace_dump_cap_cbl(&env->cvtrace, csp);

    env->hflags = save_hflags;

    return ret;
}

#else /* ! CHERI_MAGIC128 */

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

void helper_bytes2cap_op(CPUMIPSState *env, uint32_t cb, uint32_t cd, target_ulong otype,
        target_ulong addr)
{
    // Since this is used by cl* we need to treat cb == 0 as $ddc
    const cap_register_t *cbp = get_capreg_0_is_ddc(&env->active_tc, cb);
    cap_register_t *cdp = get_writable_capreg_raw(&env->active_tc, cd);
    uint32_t tag = cheri_tag_get(env, addr, cd, NULL);
    uint32_t perms;

    if (env->TLB_L || !(cbp->cr_perms & CAP_PERM_LOAD_CAP))
        tag = 0;
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

    /* Log memory read, if needed. */
    dump_cap_load_op(addr, otype, tag);
    cvtrace_dump_cap_load(&env->cvtrace, addr, cdp);
}

void helper_bytes2cap_opll(CPUMIPSState *env, uint32_t cb, uint32_t cd, target_ulong otype,
        target_ulong addr)
{
    // Since this is used by cl* we need to treat cb == 0 as $ddc
    const cap_register_t *cbp = get_capreg_0_is_ddc(&env->active_tc, cb);
    cap_register_t *cdp = get_writable_capreg_raw(&env->active_tc, cd);
    uint32_t tag = cheri_tag_get(env, addr, cd, &env->lladdr);
    uint32_t perms;

    if (env->TLB_L || !(cbp->cr_perms & CAP_PERM_LOAD_CAP))
        tag = 0;
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

    /* Log memory read, if needed. */
    dump_cap_load_op(addr, otype, tag);
    cvtrace_dump_cap_load(&env->cvtrace, addr, cdp);
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

    /* Log memory cap write, if needed. */
    dump_cap_store_op(vaddr, ret, csp->cr_tag);
    cvtrace_dump_cap_store(&env->cvtrace, vaddr, csp);

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

    /* Log memory reads, if needed. */
    dump_cap_load_cbl(cursor, base, length);
    cvtrace_dump_cap_cbl(&env->cvtrace, cdp);
}

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
        cheri_tag_set(env, vaddr, cs);
    else
        cheri_tag_invalidate(env, vaddr, CHERI_CAP_SIZE);

    ret = csp->cr_offset + csp->cr_base;
    /* Log memory cap write, if needed. */
    dump_cap_store_cursor(ret);
    cvtrace_dump_cap_cursor(&env->cvtrace, ret);


    env->hflags = save_hflags;

    return (ret);
}

target_ulong helper_cap2bytes_base(CPUMIPSState *env, uint32_t cs)
{
    const cap_register_t *csp = get_readonly_capreg(&env->active_tc, cs);

    /* Log memory cap write, if needed. */
    dump_cap_store_base(csp->cr_base);
    cvtrace_dump_cap_base(&env->cvtrace, csp->cr_base);

    return (csp->cr_base);
}

target_ulong helper_cap2bytes_length(CPUMIPSState *env, uint32_t cs)
{
    const cap_register_t *csp = get_readonly_capreg(&env->active_tc, cs);

    /* Log memory cap write, if needed. */
    dump_cap_store_length(csp->cr_length);
    cvtrace_dump_cap_length(&env->cvtrace, csp->cr_length);

    return (csp->cr_length ^ -1UL);
}
#endif /* ! CHERI_MAGIC128 */

/*
 * Print the instruction to log file.
 */
static inline void log_instruction(CPUMIPSState *env, target_ulong pc, int isa)
{
    if (unlikely(qemu_loglevel_mask(CPU_LOG_INSTR))) {
        MIPSCPU *cpu = mips_env_get_cpu(env);
        CPUState *cs = CPU(cpu);

        /* Disassemble and print instruction. */
        if (isa == 0) {
            target_disas(qemu_logfile, cs, pc, 4);
        } else {
            target_disas(qemu_logfile, cs, pc, 2);
        }
    }

    if (unlikely(qemu_loglevel_mask(CPU_LOG_CVTRACE))) {
        static uint16_t cycles = 0;  /* XXX */
        uint32_t opcode;
        MIPSCPU *cpu = mips_env_get_cpu(env);
        CPUState *cs = CPU(cpu);

        /* if the logfile is empty we need to emit the cvt magic */
        if (env->cvtrace.version != 0 && ftell(qemu_logfile) != 0) {
            /* Write previous instruction trace to log. */
            fwrite(&env->cvtrace, sizeof(env->cvtrace), 1, qemu_logfile);
        } else {
            char buffer[sizeof(env->cvtrace)];

            buffer[0] = CVT_QEMU_VERSION;
            g_strlcpy(buffer+1, CVT_QEMU_MAGIC, sizeof(env->cvtrace)-2);
            fwrite(buffer, sizeof(env->cvtrace), 1, qemu_logfile);
            cycles = 0;
        }
        bzero(&env->cvtrace, sizeof(env->cvtrace));
        env->cvtrace.version = CVT_NO_REG;
        env->cvtrace.pc = tswap64(pc);
        env->cvtrace.cycles = tswap16(cycles++);
        env->cvtrace.thread = (uint8_t)cs->cpu_index;
        env->cvtrace.asid = (uint8_t)(env->active_tc.CP0_TCStatus & 0xff);
        env->cvtrace.exception = 31;

        /* Fetch opcode. */
        if (isa == 0) {
            /* mips32/mips64 instruction. */
            opcode = cpu_ldl_code(env, pc);
        } else {
            /* micromips or mips16. */
            opcode = cpu_lduw_code(env, pc);
            if (isa == 1) {
                /* micromips */
                switch (opcode >> 10) {
                case 0x01: case 0x02: case 0x03: case 0x09:
                case 0x0a: case 0x0b:
                case 0x11: case 0x12: case 0x13: case 0x19:
                case 0x1a: case 0x1b:
                case 0x20: case 0x21: case 0x22: case 0x23:
                case 0x28: case 0x29: case 0x2a: case 0x2b:
                case 0x30: case 0x31: case 0x32: case 0x33:
                case 0x38: case 0x39: case 0x3a: case 0x3b:
                    break;
                default:
                    opcode <<= 16;
                    opcode |= cpu_lduw_code(env, pc + 2);
                    break;
                }
            } else {
                /* mips16 */
                switch (opcode >> 11) {
                case 0x03:
                case 0x1e:
                    opcode <<= 16;
                    opcode |= cpu_lduw_code(env, pc + 2);
                    break;
                }
            }
        }
        env->cvtrace.inst = opcode;  /* XXX need bswapped? */
    }
}

void helper_log_instruction(CPUMIPSState *env, uint64_t pc, int isa)
{
    log_instruction(env, pc, isa);
}

void helper_log_registers(CPUMIPSState *env)
{
    /* Print changed state: GPR, HI/LO, COP0. */
    if (unlikely(qemu_loglevel_mask(CPU_LOG_CVTRACE | CPU_LOG_INSTR) ||
        env->user_only_tracing_enabled))
        mips_dump_changed_state(env);
}

void helper_ccheck_pc(CPUMIPSState *env, uint64_t pc, int isa)
{
    cap_register_t *pcc = &env->active_tc.PCC;
    CPUState *cs = CPU(mips_env_get_cpu(env));

    // TODO: increment icount?
    /* Decrement the startup breakcount, if set. */
    if (unlikely(cs->breakcount)) {
        cs->breakcount--;
        if (cs->breakcount == 0UL) {
            helper_raise_exception(env, EXCP_DEBUG);
        }
    }

#ifdef CHERI_128
    /* Check tag before updating offset. */
    if (!pcc->cr_tag) {
        do_raise_c2_exception(env, CP2Ca_TAG, 0xff);
    }
#endif /* CHERI_128 */

    /* Update the offset */
    pcc->cr_offset = pc - pcc->cr_base;

    check_cap(env, &env->active_tc.PCC, CAP_PERM_EXECUTE, pc, 0xff, 4);
    // fprintf(qemu_logfile, "PC:%016lx\n", pc);
}

target_ulong helper_ccheck_store_right(CPUMIPSState *env, target_ulong offset, uint32_t len)
{
#ifndef TARGET_WORDS_BIGENDIAN
#error "This check is only valid for big endian targets, for little endian the load/store left instructions need to be checked"
#endif
    // For swr/sdr we store all bytes if offset & 3/7 == 0 we store only first byte, if all low bits are set we store the full amount
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

target_ulong helper_ccheck_store(CPUMIPSState *env, target_ulong offset, uint32_t len)
{
    const cap_register_t *ddc = &env->active_tc.CHWR.DDC;
    target_ulong addr = offset + ddc->cr_offset + ddc->cr_base;

    // fprintf(qemu_logfile, "ST(%u):%016lx\n", len, addr);
    check_cap(env, ddc, CAP_PERM_STORE, addr, 0, len);

    return (addr);
}

target_ulong helper_ccheck_load(CPUMIPSState *env, target_ulong offset, uint32_t len)
{
    const cap_register_t *ddc = &env->active_tc.CHWR.DDC;
    target_ulong addr = offset + ddc->cr_offset + ddc->cr_base;

    // fprintf(qemu_logfile, "LD(%u):%016lx\n", len, addr);
    check_cap(env, ddc, CAP_PERM_LOAD, addr, 0, len);

    return (addr);
}

void helper_cinvalidate_tag(CPUMIPSState *env, target_ulong addr, uint32_t len,
    uint32_t opc, target_ulong value)
{

    /* Log write, if enabled. */
    dump_store(env, opc, addr, value);

    cheri_tag_invalidate(env, addr, len);
}

void helper_cinvalidate_tag32(CPUMIPSState *env, target_ulong addr, uint32_t len,
    uint32_t opc, uint32_t value)
{

    /* Log write, if enabled. */
    dump_store(env, opc, addr, (target_ulong)value);

    cheri_tag_invalidate(env, addr, len);
}


static void simple_dump_state(CPUMIPSState *env, FILE *f,
        fprintf_function cpu_fprintf)
{

/* gxemul compat:
    cpu_fprintf(f, "pc = 0x" TARGET_FMT_lx "\n", env->active_tc.PC);
    cpu_fprintf(f, "hi = 0x" TARGET_FMT_lx "    lo = 0x" TARGET_FMT_lx "\n",
            env->active_tc.HI[0], env->active_tc.LO[0]);
    cpu_fprintf(f, "                       ""    s0 = 0x" TARGET_FMT_lx "\n",
            env->active_tc.gpr[16]);
    cpu_fprintf(f, "at = 0x" TARGET_FMT_lx "    s1 = 0x" TARGET_FMT_lx "\n",
            env->active_tc.gpr[1], env->active_tc.gpr[17]);
    cpu_fprintf(f, "v0 = 0x" TARGET_FMT_lx "    s2 = 0x" TARGET_FMT_lx "\n",
            env->active_tc.gpr[2], env->active_tc.gpr[18]);
    cpu_fprintf(f, "v1 = 0x" TARGET_FMT_lx "    s3 = 0x" TARGET_FMT_lx "\n",
            env->active_tc.gpr[3], env->active_tc.gpr[19]);
    cpu_fprintf(f, "a0 = 0x" TARGET_FMT_lx "    s4 = 0x" TARGET_FMT_lx "\n",
            env->active_tc.gpr[4], env->active_tc.gpr[20]);
    cpu_fprintf(f, "a1 = 0x" TARGET_FMT_lx "    s5 = 0x" TARGET_FMT_lx "\n",
            env->active_tc.gpr[5], env->active_tc.gpr[21]);
    cpu_fprintf(f, "a2 = 0x" TARGET_FMT_lx "    s6 = 0x" TARGET_FMT_lx "\n",
            env->active_tc.gpr[6], env->active_tc.gpr[22]);
    cpu_fprintf(f, "a3 = 0x" TARGET_FMT_lx "    s7 = 0x" TARGET_FMT_lx "\n",
            env->active_tc.gpr[7], env->active_tc.gpr[23]);
    cpu_fprintf(f, "t0 = 0x" TARGET_FMT_lx "    t8 = 0x" TARGET_FMT_lx "\n",
            env->active_tc.gpr[8], env->active_tc.gpr[24]);
    cpu_fprintf(f, "t1 = 0x" TARGET_FMT_lx "    t9 = 0x" TARGET_FMT_lx "\n",
            env->active_tc.gpr[9], env->active_tc.gpr[25]);
    cpu_fprintf(f, "t2 = 0x" TARGET_FMT_lx "    k0 = 0x" TARGET_FMT_lx "\n",
            env->active_tc.gpr[10], env->active_tc.gpr[26]);
    cpu_fprintf(f, "t3 = 0x" TARGET_FMT_lx "    k1 = 0x" TARGET_FMT_lx "\n",
            env->active_tc.gpr[11], env->active_tc.gpr[27]);
    cpu_fprintf(f, "t4 = 0x" TARGET_FMT_lx "    gp = 0x" TARGET_FMT_lx "\n",
            env->active_tc.gpr[12], env->active_tc.gpr[28]);
    cpu_fprintf(f, "t5 = 0x" TARGET_FMT_lx "    sp = 0x" TARGET_FMT_lx "\n",
            env->active_tc.gpr[13], env->active_tc.gpr[29]);
    cpu_fprintf(f, "t6 = 0x" TARGET_FMT_lx "    fp = 0x" TARGET_FMT_lx "\n",
            env->active_tc.gpr[14], env->active_tc.gpr[30]);
    cpu_fprintf(f, "t7 = 0x" TARGET_FMT_lx "    ra = 0x" TARGET_FMT_lx "\n",
            env->active_tc.gpr[15], env->active_tc.gpr[31]);
*/

    /* sim compatible register dump: */
    cpu_fprintf(f, "DEBUG MIPS COREID 0\n");
    cpu_fprintf(f, "DEBUG MIPS PC 0x" TARGET_FMT_lx "\n", env->active_tc.PC);
    cpu_fprintf(f, "DEBUG MIPS REG 00 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[0]);
    cpu_fprintf(f, "DEBUG MIPS REG 01 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[1]);
    cpu_fprintf(f, "DEBUG MIPS REG 02 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[2]);
    cpu_fprintf(f, "DEBUG MIPS REG 03 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[3]);
    cpu_fprintf(f, "DEBUG MIPS REG 04 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[4]);
    cpu_fprintf(f, "DEBUG MIPS REG 05 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[5]);
    cpu_fprintf(f, "DEBUG MIPS REG 06 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[6]);
    cpu_fprintf(f, "DEBUG MIPS REG 07 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[7]);
    cpu_fprintf(f, "DEBUG MIPS REG 08 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[8]);
    cpu_fprintf(f, "DEBUG MIPS REG 09 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[9]);
    cpu_fprintf(f, "DEBUG MIPS REG 10 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[10]);
    cpu_fprintf(f, "DEBUG MIPS REG 11 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[11]);
    cpu_fprintf(f, "DEBUG MIPS REG 12 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[12]);
    cpu_fprintf(f, "DEBUG MIPS REG 13 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[13]);
    cpu_fprintf(f, "DEBUG MIPS REG 14 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[14]);
    cpu_fprintf(f, "DEBUG MIPS REG 15 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[15]);
    cpu_fprintf(f, "DEBUG MIPS REG 16 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[16]);
    cpu_fprintf(f, "DEBUG MIPS REG 17 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[17]);
    cpu_fprintf(f, "DEBUG MIPS REG 18 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[18]);
    cpu_fprintf(f, "DEBUG MIPS REG 19 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[19]);
    cpu_fprintf(f, "DEBUG MIPS REG 20 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[20]);
    cpu_fprintf(f, "DEBUG MIPS REG 21 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[21]);
    cpu_fprintf(f, "DEBUG MIPS REG 22 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[22]);
    cpu_fprintf(f, "DEBUG MIPS REG 23 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[23]);
    cpu_fprintf(f, "DEBUG MIPS REG 24 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[24]);
    cpu_fprintf(f, "DEBUG MIPS REG 25 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[25]);
    cpu_fprintf(f, "DEBUG MIPS REG 26 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[26]);
    cpu_fprintf(f, "DEBUG MIPS REG 27 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[27]);
    cpu_fprintf(f, "DEBUG MIPS REG 28 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[28]);
    cpu_fprintf(f, "DEBUG MIPS REG 29 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[29]);
    cpu_fprintf(f, "DEBUG MIPS REG 30 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[30]);
    cpu_fprintf(f, "DEBUG MIPS REG 31 0x" TARGET_FMT_lx "\n", env->active_tc.gpr[31]);

}

void helper_mtc0_dumpstate(CPUMIPSState *env, target_ulong arg1)
{
#if 0
    cpu_dump_state(CPU(mips_env_get_cpu(env)),
            (qemu_logfile == NULL) ? stderr : qemu_logfile,
            fprintf, CPU_DUMP_CODE);
#else
    simple_dump_state(env, (qemu_logfile == NULL) ? stderr : qemu_logfile,
            fprintf);
#endif
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
            crp->cr_base, crp->cr_length, (crp->cr_offset + crp->cr_base));
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
    cpu_fprintf(f, "DEBUG CAP %s t:%d s:%d perms:0x%08x type:0x%06x "
            "offset:0x%016lx base:0x%016lx length:0x%016lx\n",
            name, crp->cr_tag, is_cap_sealed(crp),
#endif
            ((crp->cr_uperms & CAP_UPERMS_ALL) << CAP_UPERMS_SHFT) |
            (crp->cr_perms & CAP_PERMS_ALL),
            crp->cr_otype, crp->cr_offset, crp->cr_base, crp->cr_length);
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
#endif /* TARGET_CHERI */



void helper_mtc0_datalo(CPUMIPSState *env, target_ulong arg1)
{
    env->CP0_DataLo = arg1; /* XXX */
}

void helper_mtc0_taghi(CPUMIPSState *env, target_ulong arg1)
{
    env->CP0_TagHi = arg1; /* XXX */
}

void helper_mtc0_datahi(CPUMIPSState *env, target_ulong arg1)
{
    env->CP0_DataHi = arg1; /* XXX */
}

/* MIPS MT functions */
target_ulong helper_mftgpr(CPUMIPSState *env, uint32_t sel)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    if (other_tc == other->current_tc)
        return other->active_tc.gpr[sel];
    else
        return other->tcs[other_tc].gpr[sel];
}

target_ulong helper_mftlo(CPUMIPSState *env, uint32_t sel)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    if (other_tc == other->current_tc)
        return other->active_tc.LO[sel];
    else
        return other->tcs[other_tc].LO[sel];
}

target_ulong helper_mfthi(CPUMIPSState *env, uint32_t sel)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    if (other_tc == other->current_tc)
        return other->active_tc.HI[sel];
    else
        return other->tcs[other_tc].HI[sel];
}

target_ulong helper_mftacx(CPUMIPSState *env, uint32_t sel)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    if (other_tc == other->current_tc)
        return other->active_tc.ACX[sel];
    else
        return other->tcs[other_tc].ACX[sel];
}

target_ulong helper_mftdsp(CPUMIPSState *env)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    if (other_tc == other->current_tc)
        return other->active_tc.DSPControl;
    else
        return other->tcs[other_tc].DSPControl;
}

void helper_mttgpr(CPUMIPSState *env, target_ulong arg1, uint32_t sel)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    if (other_tc == other->current_tc)
        other->active_tc.gpr[sel] = arg1;
    else
        other->tcs[other_tc].gpr[sel] = arg1;
}

void helper_mttlo(CPUMIPSState *env, target_ulong arg1, uint32_t sel)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    if (other_tc == other->current_tc)
        other->active_tc.LO[sel] = arg1;
    else
        other->tcs[other_tc].LO[sel] = arg1;
}

void helper_mtthi(CPUMIPSState *env, target_ulong arg1, uint32_t sel)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    if (other_tc == other->current_tc)
        other->active_tc.HI[sel] = arg1;
    else
        other->tcs[other_tc].HI[sel] = arg1;
}

void helper_mttacx(CPUMIPSState *env, target_ulong arg1, uint32_t sel)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    if (other_tc == other->current_tc)
        other->active_tc.ACX[sel] = arg1;
    else
        other->tcs[other_tc].ACX[sel] = arg1;
}

void helper_mttdsp(CPUMIPSState *env, target_ulong arg1)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    CPUMIPSState *other = mips_cpu_map_tc(env, &other_tc);

    if (other_tc == other->current_tc)
        other->active_tc.DSPControl = arg1;
    else
        other->tcs[other_tc].DSPControl = arg1;
}

/* MIPS MT functions */
target_ulong helper_dmt(void)
{
    // TODO
     return 0;
}

target_ulong helper_emt(void)
{
    // TODO
    return 0;
}

target_ulong helper_dvpe(CPUMIPSState *env)
{
    CPUState *other_cs = first_cpu;
    target_ulong prev = env->mvp->CP0_MVPControl;

    CPU_FOREACH(other_cs) {
        MIPSCPU *other_cpu = MIPS_CPU(other_cs);
        /* Turn off all VPEs except the one executing the dvpe.  */
        if (&other_cpu->env != env) {
            other_cpu->env.mvp->CP0_MVPControl &= ~(1 << CP0MVPCo_EVP);
            mips_vpe_sleep(other_cpu);
        }
    }
    return prev;
}

target_ulong helper_evpe(CPUMIPSState *env)
{
    CPUState *other_cs = first_cpu;
    target_ulong prev = env->mvp->CP0_MVPControl;

    CPU_FOREACH(other_cs) {
        MIPSCPU *other_cpu = MIPS_CPU(other_cs);

        if (&other_cpu->env != env
            /* If the VPE is WFI, don't disturb its sleep.  */
            && !mips_vpe_is_wfi(other_cpu)) {
            /* Enable the VPE.  */
            other_cpu->env.mvp->CP0_MVPControl |= (1 << CP0MVPCo_EVP);
            mips_vpe_wake(other_cpu); /* And wake it up.  */
        }
    }
    return prev;
}
#endif /* !CONFIG_USER_ONLY */

void helper_fork(target_ulong arg1, target_ulong arg2)
{
    // arg1 = rt, arg2 = rs
    // TODO: store to TC register
}

target_ulong helper_yield(CPUMIPSState *env, target_ulong arg)
{
    target_long arg1 = arg;

    if (arg1 < 0) {
        /* No scheduling policy implemented. */
        if (arg1 != -2) {
            if (env->CP0_VPEControl & (1 << CP0VPECo_YSI) &&
                env->active_tc.CP0_TCStatus & (1 << CP0TCSt_DT)) {
                env->CP0_VPEControl &= ~(0x7 << CP0VPECo_EXCPT);
                env->CP0_VPEControl |= 4 << CP0VPECo_EXCPT;
                do_raise_exception(env, EXCP_THREAD, GETPC());
            }
        }
    } else if (arg1 == 0) {
        if (0 /* TODO: TC underflow */) {
            env->CP0_VPEControl &= ~(0x7 << CP0VPECo_EXCPT);
            do_raise_exception(env, EXCP_THREAD, GETPC());
        } else {
            // TODO: Deallocate TC
        }
    } else if (arg1 > 0) {
        /* Yield qualifier inputs not implemented. */
        env->CP0_VPEControl &= ~(0x7 << CP0VPECo_EXCPT);
        env->CP0_VPEControl |= 2 << CP0VPECo_EXCPT;
        do_raise_exception(env, EXCP_THREAD, GETPC());
    }
    return env->CP0_YQMask;
}

/* R6 Multi-threading */
#ifndef CONFIG_USER_ONLY
target_ulong helper_dvp(CPUMIPSState *env)
{
    CPUState *other_cs = first_cpu;
    target_ulong prev = env->CP0_VPControl;

    if (!((env->CP0_VPControl >> CP0VPCtl_DIS) & 1)) {
        CPU_FOREACH(other_cs) {
            MIPSCPU *other_cpu = MIPS_CPU(other_cs);
            /* Turn off all VPs except the one executing the dvp. */
            if (&other_cpu->env != env) {
                mips_vpe_sleep(other_cpu);
            }
        }
        env->CP0_VPControl |= (1 << CP0VPCtl_DIS);
    }
    return prev;
}

target_ulong helper_evp(CPUMIPSState *env)
{
    CPUState *other_cs = first_cpu;
    target_ulong prev = env->CP0_VPControl;

    if ((env->CP0_VPControl >> CP0VPCtl_DIS) & 1) {
        CPU_FOREACH(other_cs) {
            MIPSCPU *other_cpu = MIPS_CPU(other_cs);
            if ((&other_cpu->env != env) && !mips_vp_is_wfi(other_cpu)) {
                /* If the VP is WFI, don't disturb its sleep.
                 * Otherwise, wake it up. */
                mips_vpe_wake(other_cpu);
            }
        }
        env->CP0_VPControl &= ~(1 << CP0VPCtl_DIS);
    }
    return prev;
}
#endif /* !CONFIG_USER_ONLY */

#ifndef CONFIG_USER_ONLY
/* TLB management */
static void r4k_mips_tlb_flush_extra (CPUMIPSState *env, int first)
{
    /* Discard entries from env->tlb[first] onwards.  */
    while (env->tlb->tlb_in_use > first) {
        r4k_invalidate_tlb(env, --env->tlb->tlb_in_use, 0);
    }
}

static inline uint64_t get_tlb_pfn_from_entrylo(uint64_t entrylo)
{
#if defined(TARGET_MIPS64)
    return extract64(entrylo, 6, 54);
#else
    return extract64(entrylo, 6, 24) | /* PFN */
           (extract64(entrylo, 32, 32) << 24); /* PFNX */
#endif
}

static void r4k_fill_tlb(CPUMIPSState *env, int idx)
{
    r4k_tlb_t *tlb;
    uint64_t mask = env->CP0_PageMask >> (TARGET_PAGE_BITS + 1);

    /* XXX: detect conflicting TLBs and raise a MCHECK exception when needed */
    tlb = &env->tlb->mmu.r4k.tlb[idx];
    if (env->CP0_EntryHi & (1 << CP0EnHi_EHINV)) {
        tlb->EHINV = 1;
        return;
    }
    tlb->EHINV = 0;
    tlb->VPN = env->CP0_EntryHi & (TARGET_PAGE_MASK << 1);
#if defined(TARGET_MIPS64)
    tlb->VPN &= env->SEGMask;
#endif
    tlb->ASID = env->CP0_EntryHi & env->CP0_EntryHi_ASID_mask;
    tlb->PageMask = env->CP0_PageMask;
    tlb->G = env->CP0_EntryLo0 & env->CP0_EntryLo1 & 1;
    tlb->V0 = (env->CP0_EntryLo0 & 2) != 0;
    tlb->D0 = (env->CP0_EntryLo0 & 4) != 0;
    tlb->C0 = (env->CP0_EntryLo0 >> 3) & 0x7;
#if defined(TARGET_CHERI)
    tlb->L0 = (env->CP0_EntryLo0 >> CP0EnLo_L) & 1;
    tlb->S0 = (env->CP0_EntryLo0 >> CP0EnLo_S) & 1;
#else
    tlb->XI0 = (env->CP0_EntryLo0 >> CP0EnLo_XI) & 1;
    tlb->RI0 = (env->CP0_EntryLo0 >> CP0EnLo_RI) & 1;
#endif /* TARGET_CHERI */
    tlb->PFN[0] = (get_tlb_pfn_from_entrylo(env->CP0_EntryLo0) & ~mask) << 12;
    tlb->V1 = (env->CP0_EntryLo1 & 2) != 0;
    tlb->D1 = (env->CP0_EntryLo1 & 4) != 0;
    tlb->C1 = (env->CP0_EntryLo1 >> 3) & 0x7;
#if defined(TARGET_CHERI)
    tlb->L1 = (env->CP0_EntryLo1 >> CP0EnLo_L) & 1;
    tlb->S1 = (env->CP0_EntryLo1 >> CP0EnLo_S) & 1;
#else
    tlb->XI1 = (env->CP0_EntryLo1 >> CP0EnLo_XI) & 1;
    tlb->RI1 = (env->CP0_EntryLo1 >> CP0EnLo_RI) & 1;
#endif /* TARGET_CHERI */
    tlb->PFN[1] = (get_tlb_pfn_from_entrylo(env->CP0_EntryLo1) & ~mask) << 12;
}

#ifdef TARGET_CHERI
static void r4k_dump_tlb(CPUMIPSState *env, int idx)
{
    r4k_tlb_t *tlb = &env->tlb->mmu.r4k.tlb[idx];
    unsigned pagemask, hi, lo0, lo1;

    if (tlb->EHINV) {
        pagemask = 0;
        hi  = 1 << CP0EnHi_EHINV;
        lo0 = 0;
        lo1 = 0;
    } else {
        pagemask = tlb->PageMask;
        hi = tlb->VPN | tlb->ASID;
        lo0 = tlb->G | (tlb->V0 << 1) | (tlb->D0 << 2) |
#if 1
            ((target_ulong)tlb->S0 << CP0EnLo_S) |
            ((target_ulong)tlb->L0 << CP0EnLo_L) |
#else
            ((target_ulong)tlb->RI0 << CP0EnLo_RI) |
            ((target_ulong)tlb->XI0 << CP0EnLo_XI) |
#endif
            (tlb->C0 << 3) | (tlb->PFN[0] >> 6);
        lo1 = tlb->G | (tlb->V1 << 1) | (tlb->D1 << 2) |
#if 1
            ((target_ulong)tlb->S1 << CP0EnLo_S) |
            ((target_ulong)tlb->L1 << CP0EnLo_L) |
#else
            ((target_ulong)tlb->RI1 << CP0EnLo_RI) |
            ((target_ulong)tlb->XI1 << CP0EnLo_XI) |
#endif
            (tlb->C1 << 3) | (tlb->PFN[1] >> 6);
    }
    fprintf(qemu_logfile, "    Write TLB[%u] = pgmsk:%08x hi:%08x lo0:%08x lo1:%08x\n",
            idx, pagemask, hi, lo0, lo1);
}
#endif /* TARGET_CHERI */

void r4k_helper_tlbinv(CPUMIPSState *env)
{
    int idx;
    r4k_tlb_t *tlb;
    uint16_t ASID = env->CP0_EntryHi & env->CP0_EntryHi_ASID_mask;

    for (idx = 0; idx < env->tlb->nb_tlb; idx++) {
        tlb = &env->tlb->mmu.r4k.tlb[idx];
        if (!tlb->G && tlb->ASID == ASID) {
            tlb->EHINV = 1;
        }
    }
    cpu_mips_tlb_flush(env);
}

void r4k_helper_tlbinvf(CPUMIPSState *env)
{
    int idx;

    for (idx = 0; idx < env->tlb->nb_tlb; idx++) {
        env->tlb->mmu.r4k.tlb[idx].EHINV = 1;
    }
    cpu_mips_tlb_flush(env);
}

void r4k_helper_tlbwi(CPUMIPSState *env)
{
    r4k_tlb_t *tlb;
    int idx;
    target_ulong VPN;
    uint16_t ASID;
    bool EHINV, G, V0, D0, V1, D1;
#if defined(TARGET_CHERI)
    bool S0, S1, L0, L1;
#else
    bool XI0, XI1, RI0, RI1;
#endif
    idx = (env->CP0_Index & ~0x80000000) % env->tlb->nb_tlb;
    tlb = &env->tlb->mmu.r4k.tlb[idx];
    VPN = env->CP0_EntryHi & (TARGET_PAGE_MASK << 1);
#if defined(TARGET_MIPS64)
    VPN &= env->SEGMask;
#endif
    ASID = env->CP0_EntryHi & env->CP0_EntryHi_ASID_mask;
    EHINV = (env->CP0_EntryHi & (1 << CP0EnHi_EHINV)) != 0;
    G = env->CP0_EntryLo0 & env->CP0_EntryLo1 & 1;
    V0 = (env->CP0_EntryLo0 & 2) != 0;
    D0 = (env->CP0_EntryLo0 & 4) != 0;
#if defined(TARGET_CHERI)
    S0 = (env->CP0_EntryLo0 >> CP0EnLo_S) &1;
    L0 = (env->CP0_EntryLo0 >> CP0EnLo_L) &1;
#else
    XI0 = (env->CP0_EntryLo0 >> CP0EnLo_XI) &1;
    RI0 = (env->CP0_EntryLo0 >> CP0EnLo_RI) &1;
#endif
    V1 = (env->CP0_EntryLo1 & 2) != 0;
    D1 = (env->CP0_EntryLo1 & 4) != 0;
#if defined(TARGET_CHERI)
    S1 = (env->CP0_EntryLo1 >> CP0EnLo_S) &1;
    L1 = (env->CP0_EntryLo1 >> CP0EnLo_L) &1;
#else
    XI1 = (env->CP0_EntryLo1 >> CP0EnLo_XI) &1;
    RI1 = (env->CP0_EntryLo1 >> CP0EnLo_RI) &1;
#endif

    /* Discard cached TLB entries, unless tlbwi is just upgrading access
       permissions on the current entry. */
    if (tlb->VPN != VPN || tlb->ASID != ASID || tlb->G != G ||
        (!tlb->EHINV && EHINV) ||
        (tlb->V0 && !V0) || (tlb->D0 && !D0) ||
#if defined(TARGET_CHERI)
        (!tlb->S0 && S0) || (!tlb->L0 && L0) ||
#else
        (!tlb->XI0 && XI0) || (!tlb->RI0 && RI0) ||
#endif
        (tlb->V1 && !V1) || (tlb->D1 && !D1) ||
#if defined(TARGET_CHERI)
        (!tlb->S1 && S1) || (!tlb->L1 && L1)) {
#else
        (!tlb->XI1 && XI1) || (!tlb->RI1 && RI1)) {
#endif
        r4k_mips_tlb_flush_extra(env, env->tlb->nb_tlb);
    }

    r4k_invalidate_tlb(env, idx, 0);
    r4k_fill_tlb(env, idx);
#ifdef TARGET_CHERI
    if (qemu_loglevel_mask(CPU_LOG_INSTR))
        r4k_dump_tlb(env, idx);
#endif /* TARGET_CHERI */
}

void r4k_helper_tlbwr(CPUMIPSState *env)
{
    int r = cpu_mips_get_random(env);

    r4k_invalidate_tlb(env, r, 1);
    r4k_fill_tlb(env, r);
#ifdef TARGET_CHERI
    if (qemu_loglevel_mask(CPU_LOG_INSTR))
        r4k_dump_tlb(env, r);
#endif /* TARGET_CHERI */
}

void r4k_helper_tlbp(CPUMIPSState *env)
{
    r4k_tlb_t *tlb;
    target_ulong mask;
    target_ulong tag;
    target_ulong VPN;
    uint16_t ASID;
    int i;

    ASID = env->CP0_EntryHi & env->CP0_EntryHi_ASID_mask;
    for (i = 0; i < env->tlb->nb_tlb; i++) {
        tlb = &env->tlb->mmu.r4k.tlb[i];
        /* 1k pages are not supported. */
        mask = tlb->PageMask | ~(TARGET_PAGE_MASK << 1);
        tag = env->CP0_EntryHi & ~mask;
        VPN = tlb->VPN & ~mask;
#if defined(TARGET_MIPS64)
        tag &= env->SEGMask;
#endif
        /* Check ASID, virtual page number & size */
        if ((tlb->G == 1 || tlb->ASID == ASID) && VPN == tag && !tlb->EHINV) {
            /* TLB match */
            env->CP0_Index = i;
            break;
        }
    }
    if (i == env->tlb->nb_tlb) {
        /* No match.  Discard any shadow entries, if any of them match.  */
        for (i = env->tlb->nb_tlb; i < env->tlb->tlb_in_use; i++) {
            tlb = &env->tlb->mmu.r4k.tlb[i];
            /* 1k pages are not supported. */
            mask = tlb->PageMask | ~(TARGET_PAGE_MASK << 1);
            tag = env->CP0_EntryHi & ~mask;
            VPN = tlb->VPN & ~mask;
#if defined(TARGET_MIPS64)
            tag &= env->SEGMask;
#endif
            /* Check ASID, virtual page number & size */
            if ((tlb->G == 1 || tlb->ASID == ASID) && VPN == tag) {
                r4k_mips_tlb_flush_extra (env, i);
                break;
            }
        }

        env->CP0_Index |= 0x80000000;
    }
}

static inline uint64_t get_entrylo_pfn_from_tlb(uint64_t tlb_pfn)
{
#if defined(TARGET_MIPS64)
    return tlb_pfn << 6;
#else
    return (extract64(tlb_pfn, 0, 24) << 6) | /* PFN */
           (extract64(tlb_pfn, 24, 32) << 32); /* PFNX */
#endif
}

void r4k_helper_tlbr(CPUMIPSState *env)
{
    r4k_tlb_t *tlb;
    uint16_t ASID;
    int idx;

    ASID = env->CP0_EntryHi & env->CP0_EntryHi_ASID_mask;
    idx = (env->CP0_Index & ~0x80000000) % env->tlb->nb_tlb;
    tlb = &env->tlb->mmu.r4k.tlb[idx];

    /* If this will change the current ASID, flush qemu's TLB.  */
    if (ASID != tlb->ASID)
        cpu_mips_tlb_flush(env);

    r4k_mips_tlb_flush_extra(env, env->tlb->nb_tlb);

    if (tlb->EHINV) {
        env->CP0_EntryHi = 1 << CP0EnHi_EHINV;
        env->CP0_PageMask = 0;
        env->CP0_EntryLo0 = 0;
        env->CP0_EntryLo1 = 0;
    } else {
        env->CP0_EntryHi = tlb->VPN | tlb->ASID;
        env->CP0_PageMask = tlb->PageMask;
        env->CP0_EntryLo0 = tlb->G | (tlb->V0 << 1) | (tlb->D0 << 2) |
#if defined(TARGET_CHERI)
                        ((uint64_t)tlb->L0 << CP0EnLo_L) |
                        ((uint64_t)tlb->S0 << CP0EnLo_S) | (tlb->C0 << 3) |
#else
                        ((uint64_t)tlb->RI0 << CP0EnLo_RI) |
                        ((uint64_t)tlb->XI0 << CP0EnLo_XI) | (tlb->C0 << 3) |
#endif /* TARGET_CHERI */
                        get_entrylo_pfn_from_tlb(tlb->PFN[0] >> 12);
        env->CP0_EntryLo1 = tlb->G | (tlb->V1 << 1) | (tlb->D1 << 2) |
#if defined(TARGET_CHERI)
                        ((uint64_t)tlb->L1 << CP0EnLo_L) |
                        ((uint64_t)tlb->S1 << CP0EnLo_S) | (tlb->C0 << 3) |
#else
                        ((uint64_t)tlb->RI1 << CP0EnLo_RI) |
                        ((uint64_t)tlb->XI1 << CP0EnLo_XI) | (tlb->C1 << 3) |
#endif /* TARGET_CHERI */
                        get_entrylo_pfn_from_tlb(tlb->PFN[1] >> 12);
    }
}

void helper_tlbwi(CPUMIPSState *env)
{
    env->tlb->helper_tlbwi(env);
}

void helper_tlbwr(CPUMIPSState *env)
{
    env->tlb->helper_tlbwr(env);
}

void helper_tlbp(CPUMIPSState *env)
{
    env->tlb->helper_tlbp(env);
}

void helper_tlbr(CPUMIPSState *env)
{
    env->tlb->helper_tlbr(env);
}

void helper_tlbinv(CPUMIPSState *env)
{
    env->tlb->helper_tlbinv(env);
}

void helper_tlbinvf(CPUMIPSState *env)
{
    env->tlb->helper_tlbinvf(env);
}

/* Specials */
target_ulong helper_di(CPUMIPSState *env)
{
    target_ulong t0 = env->CP0_Status;

    env->CP0_Status = t0 & ~(1 << CP0St_IE);
    return t0;
}

target_ulong helper_ei(CPUMIPSState *env)
{
    target_ulong t0 = env->CP0_Status;

    env->CP0_Status = t0 | (1 << CP0St_IE);
    return t0;
}

static void debug_pre_eret(CPUMIPSState *env)
{
    if (qemu_loglevel_mask(CPU_LOG_EXEC)) {
        qemu_log("ERET: PC " TARGET_FMT_lx " EPC " TARGET_FMT_lx,
                env->active_tc.PC, env->CP0_EPC);
        if (env->CP0_Status & (1 << CP0St_ERL))
            qemu_log(" ErrorEPC " TARGET_FMT_lx, env->CP0_ErrorEPC);
        if (env->hflags & MIPS_HFLAG_DM)
            qemu_log(" DEPC " TARGET_FMT_lx, env->CP0_DEPC);
        qemu_log("\n");
    }
}

static void debug_post_eret(CPUMIPSState *env)
{
    MIPSCPU *cpu = mips_env_get_cpu(env);

    if (qemu_loglevel_mask(CPU_LOG_EXEC)) {
        qemu_log("  =>  PC " TARGET_FMT_lx " EPC " TARGET_FMT_lx,
                env->active_tc.PC, env->CP0_EPC);
        if (env->CP0_Status & (1 << CP0St_ERL))
            qemu_log(" ErrorEPC " TARGET_FMT_lx, env->CP0_ErrorEPC);
        if (env->hflags & MIPS_HFLAG_DM)
            qemu_log(" DEPC " TARGET_FMT_lx, env->CP0_DEPC);
        switch (cpu_mmu_index(env, false)) {
        case 3:
            qemu_log(", ERL\n");
            break;
        case MIPS_HFLAG_UM: qemu_log(", UM\n"); break;
        case MIPS_HFLAG_SM: qemu_log(", SM\n"); break;
        case MIPS_HFLAG_KM: qemu_log("\n"); break;
        default:
            cpu_abort(CPU(cpu), "Invalid MMU mode!\n");
            break;
        }
    }
}

static void set_pc(CPUMIPSState *env, target_ulong error_pc)
{
    env->active_tc.PC = error_pc & ~(target_ulong)1;
    if (error_pc & 1) {
        env->hflags |= MIPS_HFLAG_M16;
    } else {
        env->hflags &= ~(MIPS_HFLAG_M16);
    }
}

static inline void exception_return(CPUMIPSState *env)
{
    debug_pre_eret(env);
#ifdef TARGET_CHERI
    // qemu_log_mask(CPU_LOG_INSTR, "%s: PCC <- EPCC\n", __func__);
    if (unlikely(qemu_loglevel_mask(CPU_LOG_INSTR))) {
         // Print the new PCC value for debugging traces (compare to null
         // so that we always print it)
         cap_register_t null_cap;
         null_capability(&null_cap);
         dump_changed_capreg(env, &env->active_tc.PCC, &null_cap, "PCC");
    }
    env->active_tc.PCC = env->active_tc.CHWR.EPCC;
#endif /* TARGET_CHERI */
    if (env->CP0_Status & (1 << CP0St_ERL)) {
#ifdef TARGET_CHERI
        set_pc(env, env->CP0_ErrorEPC + env->active_tc.PCC.cr_base);
#else
        set_pc(env, env->CP0_ErrorEPC);
#endif /* TARGET_CHERI */
        env->CP0_Status &= ~(1 << CP0St_ERL);
    } else {
#ifdef TARGET_CHERI
        set_pc(env, env->CP0_EPC + env->active_tc.PCC.cr_base);
#else
        set_pc(env, env->CP0_EPC);
#endif /* TARGET_CHERI */
        env->CP0_Status &= ~(1 << CP0St_EXL);
    }
    compute_hflags(env);
    debug_post_eret(env);
}

void helper_eret(CPUMIPSState *env)
{
    exception_return(env);
    env->lladdr = 1;
#ifdef TARGET_CHERI
    env->linkedflag = 0;
#endif /* TARGET_CHERI */
}

void helper_eretnc(CPUMIPSState *env)
{
    exception_return(env);
}

void helper_deret(CPUMIPSState *env)
{
    debug_pre_eret(env);
    set_pc(env, env->CP0_DEPC);

    env->hflags &= ~MIPS_HFLAG_DM;
    compute_hflags(env);
    debug_post_eret(env);
}
#endif /* !CONFIG_USER_ONLY */

static inline void check_hwrena(CPUMIPSState *env, int reg, uintptr_t pc)
{
    if ((env->hflags & MIPS_HFLAG_CP0) || (env->CP0_HWREna & (1 << reg))) {
        return;
    }
    do_raise_exception(env, EXCP_RI, pc);
}

target_ulong helper_rdhwr_cpunum(CPUMIPSState *env)
{
    check_hwrena(env, 0, GETPC());
    return env->CP0_EBase & 0x3ff;
}

target_ulong helper_rdhwr_synci_step(CPUMIPSState *env)
{
    check_hwrena(env, 1, GETPC());
    return env->SYNCI_Step;
}

target_ulong helper_rdhwr_cc(CPUMIPSState *env)
{
    int32_t count;
    check_hwrena(env, 2, GETPC());
#ifdef CONFIG_USER_ONLY
    count = env->CP0_Count;
#else
    qemu_mutex_lock_iothread();
    count = (int32_t)cpu_mips_get_count(env);
    qemu_mutex_unlock_iothread();
#endif
    return count;
}

target_ulong helper_rdhwr_ccres(CPUMIPSState *env)
{
    check_hwrena(env, 3, GETPC());
    return env->CCRes;
}

target_ulong helper_rdhwr_performance(CPUMIPSState *env)
{
    check_hwrena(env, 4, GETPC());
    return env->CP0_Performance0;
}

target_ulong helper_rdhwr_xnp(CPUMIPSState *env)
{
    check_hwrena(env, 5, GETPC());
    return (env->CP0_Config5 >> CP0C5_XNP) & 1;
}

#if defined(TARGET_CHERI)
target_ulong helper_rdhwr_statcounters_icount(CPUMIPSState *env)
{
    check_hwrena(env, 4, GETPC());
    return 0x12345;
}

target_ulong helper_rdhwr_statcounters_reset(CPUMIPSState *env)
{
    // TODO: actually implement this
    check_hwrena(env, 7, GETPC());
    return 0;
}

target_ulong helper_rdhwr_statcounters_ignored(CPUMIPSState *env, uint32_t num)
{
    check_hwrena(env, num, GETPC());
    return 0xdeadbeef;
}
#endif

void helper_pmon(CPUMIPSState *env, int function)
{
    function /= 2;
    switch (function) {
    case 2: /* TODO: char inbyte(int waitflag); */
        if (env->active_tc.gpr[4] == 0)
            env->active_tc.gpr[2] = -1;
        /* Fall through */
    case 11: /* TODO: char inbyte (void); */
        env->active_tc.gpr[2] = -1;
        break;
    case 3:
    case 12:
        printf("%c", (char)(env->active_tc.gpr[4] & 0xFF));
        break;
    case 17:
        break;
    case 158:
        {
            unsigned char *fmt = (void *)(uintptr_t)env->active_tc.gpr[4];
            printf("%s", fmt);
        }
        break;
    }
}

void helper_wait(CPUMIPSState *env)
{
    CPUState *cs = CPU(mips_env_get_cpu(env));

    cs->halted = 1;
    cpu_reset_interrupt(cs, CPU_INTERRUPT_WAKE);
    /* Last instruction in the block, PC was updated before
       - no need to recover PC and icount */
    raise_exception(env, EXCP_HLT);
}

#if !defined(CONFIG_USER_ONLY)

void mips_cpu_do_unaligned_access(CPUState *cs, vaddr addr,
                                  MMUAccessType access_type,
                                  int mmu_idx, uintptr_t retaddr)
{
    MIPSCPU *cpu = MIPS_CPU(cs);
    CPUMIPSState *env = &cpu->env;
    int error_code = 0;
    int excp;

    env->CP0_BadVAddr = addr;

    if (access_type == MMU_DATA_STORE) {
        excp = EXCP_AdES;
    } else {
        excp = EXCP_AdEL;
        if (access_type == MMU_INST_FETCH) {
            error_code |= EXCP_INST_NOTAVAIL;
        }
    }

    do_raise_exception_err(env, excp, error_code, retaddr);
}

void tlb_fill(CPUState *cs, target_ulong addr, int size,
              MMUAccessType access_type, int mmu_idx, uintptr_t retaddr)
{
    int ret;

    ret = mips_cpu_handle_mmu_fault(cs, addr, size, access_type, mmu_idx);
    if (ret) {
        MIPSCPU *cpu = MIPS_CPU(cs);
        CPUMIPSState *env = &cpu->env;

        do_raise_exception_err(env, cs->exception_index,
                               env->error_code, retaddr);
    }
}

void mips_cpu_unassigned_access(CPUState *cs, hwaddr addr,
                                bool is_write, bool is_exec, int unused,
                                unsigned size)
{
    MIPSCPU *cpu = MIPS_CPU(cs);
    CPUMIPSState *env = &cpu->env;

    /*
     * Raising an exception with KVM enabled will crash because it won't be from
     * the main execution loop so the longjmp won't have a matching setjmp.
     * Until we can trigger a bus error exception through KVM lets just ignore
     * the access.
     */
    if (kvm_enabled()) {
        return;
    }

    if (is_exec) {
        raise_exception(env, EXCP_IBE);
    } else {
        raise_exception(env, EXCP_DBE);
    }
}

#ifdef TARGET_CHERI

/*
 * Print changed kernel/user/debug mode.
 */
static const char* mips_cpu_get_changed_mode(CPUMIPSState *env)
{
    const char *kernel0, *kernel1, *mode;

#if defined(TARGET_MIPS64)
    if (env->CP0_Status & (1 << CP0St_KX)) {
        kernel0 = "Kernel mode (ERL=0, KX=1)";
        kernel1 = "Kernel mode (ERL=1, KX=1)";
    } else {
        kernel0 = "Kernel mode (ERL=0, KX=0)";
        kernel1 = "Kernel mode (ERL=1, KX=0)";
    }
#else
    kernel0 = "Kernel mode (ERL=0)";
    kernel1 = "Kernel mode (ERL=1)";
#endif

    if (env->CP0_Debug & (1 << CP0DB_DM)) {
        mode = "Debug mode";
    } else if (env->CP0_Status & (1 << CP0St_ERL)) {
        mode = kernel1;
    } else if (env->CP0_Status & (1 << CP0St_EXL)) {
        mode = kernel0;
    } else {
        switch (extract32(env->CP0_Status, CP0St_KSU, 2)) {
        case 0:  mode = kernel0;           break;
        case 1:  mode = "Supervisor mode"; break;
        default: mode = TRACE_MODE_USER;   break;
        }
    }
    return mode;
}

/*
 * Names of coprocessor 0 registers.
 */
static const char *cop0_name[32*8] = {
/*0*/   "Index",        "MVPControl",   "MVPConf0",     "MVPConf1",
        0,              0,              0,              0,
/*1*/   "Random",       "VPEControl",   "VPEConf0",     "VPEConf1",
        "YQMask",       "VPESchedule",  "VPEScheFBack", "VPEOpt",
/*2*/   "EntryLo0",     "TCStatus",     "TCBind",       "TCRestart",
        "TCHalt",       "TCContext",    "TCSchedule",   "TCScheFBack",
/*3*/   "EntryLo1",     0,              0,              0,
        0,              0,              0,              "TCOpt",
/*4*/   "Context",      "ContextConfig","UserLocal",    "XContextConfig",
        0,              0,              0,              0,
/*5*/   "PageMask",     "PageGrain",    "SegCtl0",      "SegCtl1",
        "SegCtl2",      0,              0,              0,
/*6*/   "Wired",        "SRSConf0",     "SRSConf1",     "SRSConf2",
        "SRSConf3",     "SRSConf4",     0,              0,
/*7*/   "HWREna",       0,              0,              0,
        0,              0,              0,              0,
/*8*/   "BadVAddr",     "BadInstr",     "BadInstrP",    0,
        0,              0,              0,              0,
/*9*/   "Count",        0,              0,              0,
        0,              0,              0,              0,
/*10*/  "EntryHi",      0,              0,              0,
        0,              "MSAAccess",    "MSASave",      "MSARequest",
/*11*/  "Compare",      0,              0,              0,
        0,              0,              0,              0,
/*12*/  "Status",       "IntCtl",       "SRSCtl",       "SRSMap",
        "ViewIPL",      "SRSMap2",      0,              0,
/*13*/  "Cause",        0,              0,              0,
        "ViewRIPL",     "NestedExc",    0,              0,
/*14*/  "EPC",          0,              "NestedEPC",    0,
        0,              0,              0,              0,
/*15*/  "PRId",         "EBase",        "CDMMBase",     "CMGCRBase",
        0,              0,              0,              0,
/*16*/  "Config",       "Config1",      "Config2",      "Config3",
        "Config4",      "Config5",      "Config6",      "Config7",
/*17*/  "LLAddr",       0,              0,              0,
        0,              0,              0,              0,
/*18*/  "WatchLo",      "WatchLo1",     "WatchLo2",     "WatchLo3",
        "WatchLo4",     "WatchLo5",     "WatchLo6",     "WatchLo7",
/*19*/  "WatchHi",      "WatchHi1",     "WatchHi2",     "WatchHi3",
        "WatchHi4",     "WatchHi5",     "WatchHi6",     "WatchHi7",
/*20*/  "XContext",     0,              0,              0,
        0,              0,              0,              0,
/*21*/  0,              0,              0,              0,
        0,              0,              0,              0,
/*22*/  0,              0,              0,              0,
        0,              0,              0,              0,
/*23*/  "Debug",        "TraceControl", "TraceControl2","UserTraceData",
        "TraceIBPC",    "TraceDBPC",    "Debug2",       0,
/*24*/  "DEPC",         0,              "TraceControl3","UserTraceData2",
        0,              0,              0,              0,
/*25*/  "PerfCnt",      "PerfCnt1",     "PerfCnt2",     "PerfCnt3",
        "PerfCnt4",     "PerfCnt5",     "PerfCnt6",     "PerfCnt7",
/*26*/  "ErrCtl",       0,              0,              0,
        0,              0,              0,              0,
/*27*/  "CacheErr",     0,              0,              0,
        0,              0,              0,              0,
/*28*/  "ITagLo",       "IDataLo",      "DTagLo",       "DDataLo",
        "L23TagLo",     "L23DataLo",    0,              0,
/*29*/  "ITagHi",       "IDataHi",      "DTagHi",       0,
        0,              "L23DataHi",    0,              0,
/*30*/  "ErrorEPC",     0,              0,              0,
        0,              0,              0,              0,
/*31*/  "DESAVE",       0,              "KScratch1",    "KScratch2",
        "KScratch3",    "KScratch4",    "KScratch5",    "KScratch6",
};

/*
 * Print changed values of COP0 registers.
 */
static void dump_changed_cop0_reg(CPUMIPSState *env, int idx,
        target_ulong value)
{
    if (value != env->last_cop0[idx]) {
        env->last_cop0[idx] = value;
        if (cop0_name[idx])
            fprintf(qemu_logfile, "    Write %s = " TARGET_FMT_lx "\n",
                    cop0_name[idx], value);
        else
            fprintf(qemu_logfile, "    Write (idx=%d) = " TARGET_FMT_lx "\n",
                    idx, value);

    }
}

/*
 * Print changed values of COP0 registers.
 */
static void dump_changed_cop0(CPUMIPSState *env)
{
    dump_changed_cop0_reg(env, 0*8 + 0, env->CP0_Index);
    if (env->CP0_Config3 & (1 << CP0C3_MT)) {
        dump_changed_cop0_reg(env, 0*8 + 1, env->mvp->CP0_MVPControl);
        dump_changed_cop0_reg(env, 0*8 + 2, env->mvp->CP0_MVPConf0);
        dump_changed_cop0_reg(env, 0*8 + 3, env->mvp->CP0_MVPConf1);

        dump_changed_cop0_reg(env, 1*8 + 1, env->CP0_VPEControl);
        dump_changed_cop0_reg(env, 1*8 + 2, env->CP0_VPEConf0);
        dump_changed_cop0_reg(env, 1*8 + 3, env->CP0_VPEConf1);
        dump_changed_cop0_reg(env, 1*8 + 4, env->CP0_YQMask);
        dump_changed_cop0_reg(env, 1*8 + 5, env->CP0_VPESchedule);
        dump_changed_cop0_reg(env, 1*8 + 6, env->CP0_VPEScheFBack);
        dump_changed_cop0_reg(env, 1*8 + 7, env->CP0_VPEOpt);
    }

    dump_changed_cop0_reg(env, 2*8 + 0, env->CP0_EntryLo0);
    if (env->CP0_Config3 & (1 << CP0C3_MT)) {
        dump_changed_cop0_reg(env, 2*8 + 1, env->active_tc.CP0_TCStatus);
        dump_changed_cop0_reg(env, 2*8 + 2, env->active_tc.CP0_TCBind);
        dump_changed_cop0_reg(env, 2*8 + 3, env->active_tc.PC);
        dump_changed_cop0_reg(env, 2*8 + 4, env->active_tc.CP0_TCHalt);
        dump_changed_cop0_reg(env, 2*8 + 5, env->active_tc.CP0_TCContext);
        dump_changed_cop0_reg(env, 2*8 + 6, env->active_tc.CP0_TCSchedule);
        dump_changed_cop0_reg(env, 2*8 + 7, env->active_tc.CP0_TCScheFBack);
    }

    dump_changed_cop0_reg(env, 3*8 + 0, env->CP0_EntryLo1);

    dump_changed_cop0_reg(env, 4*8 + 0, env->CP0_Context);
    /* 4/1 not implemented - ContextConfig */
    dump_changed_cop0_reg(env, 4*8 + 2, env->active_tc.CP0_UserLocal);
    /* 4/3 not implemented - XContextConfig */

    dump_changed_cop0_reg(env, 5*8 + 0, env->CP0_PageMask);
    dump_changed_cop0_reg(env, 5*8 + 1, env->CP0_PageGrain);

    dump_changed_cop0_reg(env, 6*8 + 0, env->CP0_Wired);
    dump_changed_cop0_reg(env, 6*8 + 1, env->CP0_SRSConf0);
    dump_changed_cop0_reg(env, 6*8 + 2, env->CP0_SRSConf1);
    dump_changed_cop0_reg(env, 6*8 + 3, env->CP0_SRSConf2);
    dump_changed_cop0_reg(env, 6*8 + 4, env->CP0_SRSConf3);
    dump_changed_cop0_reg(env, 6*8 + 5, env->CP0_SRSConf4);

    dump_changed_cop0_reg(env, 7*8 + 0, env->CP0_HWREna);

    dump_changed_cop0_reg(env, 8*8 + 0, env->CP0_BadVAddr);
    if (env->CP0_Config3 & (1 << CP0C3_BI))
        dump_changed_cop0_reg(env, 8*8 + 1, env->CP0_BadInstr);
    if (env->CP0_Config3 & (1 << CP0C3_BP))
        dump_changed_cop0_reg(env, 8*8 + 2, env->CP0_BadInstrP);

    dump_changed_cop0_reg(env, 10*8 + 0, env->CP0_EntryHi);

    dump_changed_cop0_reg(env, 11*8 + 0, env->CP0_Compare);

    dump_changed_cop0_reg(env, 12*8 + 0, env->CP0_Status);
    dump_changed_cop0_reg(env, 12*8 + 1, env->CP0_IntCtl);
    dump_changed_cop0_reg(env, 12*8 + 2, env->CP0_SRSCtl);
    dump_changed_cop0_reg(env, 12*8 + 3, env->CP0_SRSMap);

    dump_changed_cop0_reg(env, 13*8 + 0, env->CP0_Cause);

    dump_changed_cop0_reg(env, 14*8 + 0, env->CP0_EPC);

    dump_changed_cop0_reg(env, 15*8 + 0, env->CP0_PRid);
    dump_changed_cop0_reg(env, 15*8 + 1, env->CP0_EBase);

    dump_changed_cop0_reg(env, 16*8 + 0, env->CP0_Config0);
    dump_changed_cop0_reg(env, 16*8 + 1, env->CP0_Config1);
    dump_changed_cop0_reg(env, 16*8 + 2, env->CP0_Config2);
    dump_changed_cop0_reg(env, 16*8 + 3, env->CP0_Config3);
    dump_changed_cop0_reg(env, 16*8 + 4, env->CP0_Config4);
    dump_changed_cop0_reg(env, 16*8 + 5, env->CP0_Config5);
    dump_changed_cop0_reg(env, 16*8 + 6, env->CP0_Config6);
    dump_changed_cop0_reg(env, 16*8 + 7, env->CP0_Config7);

    dump_changed_cop0_reg(env, 17*8 + 0, env->lladdr >> env->CP0_LLAddr_shift);

    dump_changed_cop0_reg(env, 18*8 + 0, env->CP0_WatchLo[0]);
    dump_changed_cop0_reg(env, 18*8 + 1, env->CP0_WatchLo[1]);
    dump_changed_cop0_reg(env, 18*8 + 2, env->CP0_WatchLo[2]);
    dump_changed_cop0_reg(env, 18*8 + 3, env->CP0_WatchLo[3]);
    dump_changed_cop0_reg(env, 18*8 + 4, env->CP0_WatchLo[4]);
    dump_changed_cop0_reg(env, 18*8 + 5, env->CP0_WatchLo[5]);
    dump_changed_cop0_reg(env, 18*8 + 6, env->CP0_WatchLo[6]);
    dump_changed_cop0_reg(env, 18*8 + 7, env->CP0_WatchLo[7]);

    dump_changed_cop0_reg(env, 19*8 + 0, env->CP0_WatchHi[0]);
    dump_changed_cop0_reg(env, 19*8 + 1, env->CP0_WatchHi[1]);
    dump_changed_cop0_reg(env, 19*8 + 2, env->CP0_WatchHi[2]);
    dump_changed_cop0_reg(env, 19*8 + 3, env->CP0_WatchHi[3]);
    dump_changed_cop0_reg(env, 19*8 + 4, env->CP0_WatchHi[4]);
    dump_changed_cop0_reg(env, 19*8 + 5, env->CP0_WatchHi[5]);
    dump_changed_cop0_reg(env, 19*8 + 6, env->CP0_WatchHi[6]);
    dump_changed_cop0_reg(env, 19*8 + 7, env->CP0_WatchHi[7]);

#if defined(TARGET_MIPS64)
    dump_changed_cop0_reg(env, 20*8 + 0, env->CP0_XContext);
#endif

    dump_changed_cop0_reg(env, 21*8 + 0, env->CP0_Framemask);

    /* 22/x not defined */

    dump_changed_cop0_reg(env, 23*8 + 0, helper_mfc0_debug(env));

    dump_changed_cop0_reg(env, 24*8 + 0, env->CP0_DEPC);

    dump_changed_cop0_reg(env, 25*8 + 0, env->CP0_Performance0);

    /* 26/0 - ErrCtl */
    dump_changed_cop0_reg(env, 25*8 + 0, env->CP0_ErrCtl);

    /* 27/0 not implemented - CacheErr */

    dump_changed_cop0_reg(env, 28*8 + 0, env->CP0_TagLo);
    dump_changed_cop0_reg(env, 28*8 + 1, env->CP0_DataLo);

    dump_changed_cop0_reg(env, 29*8 + 0, env->CP0_TagHi);
    dump_changed_cop0_reg(env, 29*8 + 1, env->CP0_DataHi);

    dump_changed_cop0_reg(env, 30*8 + 0, env->CP0_ErrorEPC);

    dump_changed_cop0_reg(env, 31*8 + 0, env->CP0_DESAVE);
    dump_changed_cop0_reg(env, 31*8 + 2, env->CP0_KScratch[0]);
    dump_changed_cop0_reg(env, 31*8 + 3, env->CP0_KScratch[1]);
    dump_changed_cop0_reg(env, 31*8 + 4, env->CP0_KScratch[2]);
    dump_changed_cop0_reg(env, 31*8 + 5, env->CP0_KScratch[3]);
    dump_changed_cop0_reg(env, 31*8 + 6, env->CP0_KScratch[4]);
    dump_changed_cop0_reg(env, 31*8 + 7, env->CP0_KScratch[5]);
}
#endif /* TARGET_CHERI */
#endif /* !CONFIG_USER_ONLY */

#ifdef TARGET_CHERI

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
            // TODO: allow printing a string instead of C%d
            fprintf(qemu_logfile,
                    "    Write %s|v:%d s:%d p:%08x b:%016" PRIx64
                        " l:%016" PRIx64 "\n", name, cr->cr_tag,
                    is_cap_sealed(cr) ? 1 : 0,
                    (((cr->cr_uperms & CAP_UPERMS_ALL) <<
                                                       CAP_UPERMS_MEM_SHFT) |
                     (cr->cr_perms & CAP_PERMS_ALL)),
                    cr->cr_base, cr->cr_length);
            fprintf(qemu_logfile, "             |o:%016" PRIx64 " t:%x\n",
                    cr->cr_offset, cr->cr_otype);
        }
    }
}

/*
 * Print changed values of GPR, HI/LO and DSPControl registers.
 */
static void dump_changed_regs(CPUMIPSState *env)
{
    TCState *cur = &env->active_tc;
    static const char * const gpr_name[] = {
        "r0", "at", "v0", "v1", "a0", "a1", "a2", "a3",
        "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
        "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
        "t8", "t9", "k0", "k1", "gp", "sp", "s8", "ra",
    };
    int i;

    for (i=1; i<32; i++) {
        if (cur->gpr[i] != env->last_gpr[i]) {
            env->last_gpr[i] = cur->gpr[i];
            cvtrace_dump_gpr(&env->cvtrace, cur->gpr[i]);
            if (qemu_loglevel_mask(CPU_LOG_INSTR)) {
                fprintf(qemu_logfile, "    Write %s = " TARGET_FMT_lx "\n",
                        gpr_name[i], cur->gpr[i]);
            }
        }
    }
    static const char * const capreg_name[] = {
        "C00", "C01", "C02", "C03", "C04", "C05", "C06", "C07",
        "C08", "C09", "C10", "C11", "C12", "C13", "C14", "C15",
        "C16", "C17", "C18", "C19", "C20", "C21", "C22", "C23",
        "C24", "C25", "C26", "C27", "C28", "C29", "C30", "C31",
    };
    for (i=0; i<32; i++) {
        dump_changed_capreg(env, &cur->_CGPR[i], &env->last_C[i], capreg_name[i]);
    }
    dump_changed_capreg(env, &cur->CHWR.DDC, &env->last_CHWR.DDC, "DDC");
    dump_changed_capreg(env, &cur->CHWR.UserTlsCap, &env->last_CHWR.UserTlsCap, "UserTlsCap");
    dump_changed_capreg(env, &cur->CHWR.PrivTlsCap, &env->last_CHWR.PrivTlsCap, "PrivTlsCap");
    dump_changed_capreg(env, &cur->CHWR.KR1C, &env->last_CHWR.KR1C, "ChwrKR1C");
    dump_changed_capreg(env, &cur->CHWR.KR2C, &env->last_CHWR.KR2C, "ChwrKR1C");
    dump_changed_capreg(env, &cur->CHWR.KCC, &env->last_CHWR.KCC, "KCC");
    dump_changed_capreg(env, &cur->CHWR.KDC, &env->last_CHWR.KDC, "KDC");
    dump_changed_capreg(env, &cur->CHWR.EPCC, &env->last_CHWR.EPCC, "EPCC");
}


static void update_tracing_on_mode_change(CPUMIPSState *env, const char* new_mode)
{
    if (!env->user_only_tracing_enabled) {
        return;
    }
    if (IN_USERSPACE(env)) {
        assert(strcmp(new_mode, TRACE_MODE_USER) != 0);
        /* When changing from user mode to kernel mode disable tracing */
        user_trace_dbg("%s -> %s: 0x%lx ASID %lu -- switching off tracing \n",
            env->last_mode, new_mode, env->active_tc.PC, env->CP0_EntryHi & 0xFF);
        env->tracing_suspended = true;
        qemu_set_log(qemu_loglevel & ~cl_default_trace_format);
    } else if (strcmp(new_mode, TRACE_MODE_USER) == 0) {
        /* When changing back to user mode restore instruction tracing */
        assert(!IN_USERSPACE(env));
        if (env->trace_explicitly_disabled) {
            user_trace_dbg("Not turning on tracing on switch %s -> %s 0x%lx. "
                "Tracing was explicitly disabled, ASID=%lu\n",
                env->last_mode, new_mode, env->active_tc.PC, env->CP0_EntryHi & 0xFF);
        } else if (env->tracing_suspended) {
            qemu_set_log(qemu_loglevel | cl_default_trace_format);
            user_trace_dbg("%s -> %s 0x%lx ASID %lu -- switching on tracing\n",
                env->last_mode, new_mode, env->active_tc.PC, env->CP0_EntryHi & 0xFF);
            env->tracing_suspended = false;
        }
    }
}

/*
 * Print the changed processor state.
 */
void mips_dump_changed_state(CPUMIPSState *env)
{
    const char* new_mode = mips_cpu_get_changed_mode(env);
    /* Testing pointer equality is fine, it always points to the same constants */
    if (new_mode != env->last_mode) {
        update_tracing_on_mode_change(env, new_mode);
        env->last_mode = new_mode;
        qemu_log_mask(CPU_LOG_INSTR, "--- %s\n", new_mode);
    }

    if (qemu_loglevel_mask(CPU_LOG_INSTR | CPU_LOG_CVTRACE)) {
        /* Print changed state: GPR, Cap. */
        dump_changed_regs(env);
    }

    if (qemu_loglevel_mask(CPU_LOG_INSTR)) {
        /* Print change state: HI/LO COP0 (not included in CVTRACE) */
        dump_changed_cop0(env);
    }
}

enum {
    /* Load and stores */
    OPC_LDL      = (0x1A << 26),
    OPC_LDR      = (0x1B << 26),
    OPC_LB       = (0x20 << 26),
    OPC_LH       = (0x21 << 26),
    OPC_LWL      = (0x22 << 26),
    OPC_LW       = (0x23 << 26),
    OPC_LWPC     = OPC_LW | 0x5,
    OPC_LBU      = (0x24 << 26),
    OPC_LHU      = (0x25 << 26),
    OPC_LWR      = (0x26 << 26),
    OPC_LWU      = (0x27 << 26),
    OPC_SB       = (0x28 << 26),
    OPC_SH       = (0x29 << 26),
    OPC_SWL      = (0x2A << 26),
    OPC_SW       = (0x2B << 26),
    OPC_SDL      = (0x2C << 26),
    OPC_SDR      = (0x2D << 26),
    OPC_SWR      = (0x2E << 26),
    OPC_LL       = (0x30 << 26),

    OPC_LWC1     = (0x31 << 26),
    OPC_LDC1     = (0x35 << 26),
    OPC_SWC1     = (0x39 << 26),
    OPC_SDC1     = (0x3D << 26),

    OPC_LWXC1   = 0x00 | (0x13 << 26),
    OPC_LDXC1   = 0x01 | (0x13 << 26),
    OPC_LUXC1   = 0x05 | (0x13 << 26),
    OPC_SWXC1   = 0x08 | (0x13 << 26),
    OPC_SDXC1   = 0x09 | (0x13 << 26),
    OPC_SUXC1   = 0x0D | (0x13 << 26),

    OPC_CSCB     = (0x12 << 26) | (0x10 << 21) | (0x0),
    OPC_CSCH     = (0x12 << 26) | (0x10 << 21) | (0x1),
    OPC_CSCW     = (0x12 << 26) | (0x10 << 21) | (0x2),
    OPC_CSCD     = (0x12 << 26) | (0x10 << 21) | (0x3),

    OPC_CSCC     = (0x12 << 26) | (0x10 << 21) | (0x7),

    OPC_CLLB     = (0x12 << 26) | (0x10 << 21) | (0x8),
    OPC_CLLH     = (0x12 << 26) | (0x10 << 21) | (0x9),
    OPC_CLLW     = (0x12 << 26) | (0x10 << 21) | (0xa),
    OPC_CLLD     = (0x12 << 26) | (0x10 << 21) | (0xb),
    OPC_CLLBU    = (0x12 << 26) | (0x10 << 21) | (0xc),
    OPC_CLLHU    = (0x12 << 26) | (0x10 << 21) | (0xd),
    OPC_CLLWU    = (0x12 << 26) | (0x10 << 21) | (0xe),

    OPC_CLLC     = (0x12 << 26) | (0x10 << 21) | (0xf),

    OPC_CLBU     = (0x32 << 26) | (0x0),
    OPC_CLHU     = (0x32 << 26) | (0x1),
    OPC_CLWU     = (0x32 << 26) | (0x2),
    OPC_CLDU     = (0x32 << 26) | (0x3),
    OPC_CLB      = (0x32 << 26) | (0x4),
    OPC_CLH      = (0x32 << 26) | (0x5),
    OPC_CLW      = (0x32 << 26) | (0x6),
    OPC_CLD      = (0x32 << 26) | (0x7),

    OPC_CLOADC   = (0x36 << 26),

    OPC_CSB      = (0x3A << 26) | (0x0),
    OPC_CSH      = (0x3A << 26) | (0x1),
    OPC_CSW      = (0x3A << 26) | (0x2),
    OPC_CSD      = (0x3A << 26) | (0x3),

    OPC_CSTOREC  = (0x3E << 26),

    OPC_LLD      = (0x34 << 26),
    OPC_LD       = (0x37 << 26),
    OPC_LDPC     = OPC_LD | 0x5,
    OPC_SC       = (0x38 << 26),
    OPC_SCD      = (0x3C << 26),
    OPC_SD       = (0x3F << 26),
};

/*
 * Print the memory store to log file.
 */
static inline void dump_store(CPUMIPSState *env, int opc, target_ulong addr,
        target_ulong value)
{

    if (likely(!(qemu_loglevel_mask(CPU_LOG_INSTR) |
                 qemu_loglevel_mask(CPU_LOG_CVTRACE))))
        return;
    if (qemu_loglevel_mask(CPU_LOG_CVTRACE)) {
        cvtrace_dump_gpr_store(&env->cvtrace, addr, value);
        return;
    }

    switch (opc) {
#if defined(TARGET_MIPS64)
    case OPC_SCD:
    case OPC_SD:
    case OPC_SDL:
    case OPC_SDR:

    case OPC_SDC1:
    case OPC_SDXC1:
    case OPC_SUXC1:

    case OPC_CSD:
    case OPC_CSTOREC:
    case OPC_CSCD:
        fprintf(qemu_logfile, "    Memory Write [" TARGET_FMT_lx "] = "
                TARGET_FMT_lx"\n", addr, value);
        break;
#endif
    case OPC_SC:
    case OPC_SW:
    case OPC_SWL:
    case OPC_SWR:

    case OPC_SWC1:
    case OPC_SWXC1:

    case OPC_CSW:
    case OPC_CSCW:
        fprintf(qemu_logfile, "    Memory Write [" TARGET_FMT_lx "] = %08x\n",
                addr, (uint32_t) value);
        break;
    case OPC_SH:

    case OPC_CSH:
    case OPC_CSCH:
        fprintf(qemu_logfile, "    Memory Write [" TARGET_FMT_lx "] = %04x\n",
                addr, (uint16_t) value);
        break;
    case OPC_SB:

    case OPC_CSB:
    case OPC_CSCB:
        fprintf(qemu_logfile, "    Memory Write [" TARGET_FMT_lx "] = %02x\n",
                addr, (uint8_t) value);
        break;
    default:
        fprintf(qemu_logfile, "    Memory op%u [" TARGET_FMT_lx "] = %08x\n",
                opc, addr, (uint32_t) value);
    }
}

/*
 * Print the memory load to log file.
 */
void helper_dump_load(CPUMIPSState *env, int opc, target_ulong addr,
        target_ulong value)
{
    if (opc == OPC_CLLD || opc == OPC_CLLWU || opc == OPC_CLLW ||
        opc == OPC_CLLHU || opc == OPC_CLLH || opc == OPC_CLLBU ||
        opc == OPC_CLLB) {
        env->lladdr = do_translate_address(env, addr, 0, GETPC());
        env->llval = value;
    }
    if (likely(!(qemu_loglevel_mask(CPU_LOG_INSTR) |
                 qemu_loglevel_mask(CPU_LOG_CVTRACE))))
        return;
    if (qemu_loglevel_mask(CPU_LOG_CVTRACE)) {
        cvtrace_dump_gpr_load(&env->cvtrace, addr, value);
        return;
    }

    switch (opc) {
#if defined(TARGET_MIPS64)
    case OPC_LD:
    case OPC_LDL:
    case OPC_LDR:
    case OPC_LDPC:

    case OPC_LDC1:
    case OPC_LDXC1:
    case OPC_LLD:
    case OPC_LUXC1:

    case OPC_CLD:
    case OPC_CLOADC:
    case OPC_CLLD:
        fprintf(qemu_logfile, "    Memory Read [" TARGET_FMT_lx "] = "
                TARGET_FMT_lx "\n", addr, value);
        break;
    case OPC_LWU:
#endif
    case OPC_LL:
    case OPC_LW:
    case OPC_LWPC:
    case OPC_LWL:
    case OPC_LWR:

    case OPC_LWC1:
    case OPC_LWXC1:

    case OPC_CLW:
    case OPC_CLWU:
    case OPC_CLLW:
    case OPC_CLLWU:
        fprintf(qemu_logfile, "    Memory Read [" TARGET_FMT_lx "] = %08x\n",
                addr, (uint32_t) value);
        break;
    case OPC_LH:
    case OPC_LHU:

    case OPC_CLH:
    case OPC_CLHU:
    case OPC_CLLH:
    case OPC_CLLHU:
        fprintf(qemu_logfile, "    Memory Read [" TARGET_FMT_lx "] = %04x\n",
                addr, (uint16_t) value);
        break;
    case OPC_LB:
    case OPC_LBU:

    case OPC_CLB:
    case OPC_CLBU:
    case OPC_CLLB:
    case OPC_CLLBU:
        fprintf(qemu_logfile, "    Memory Read [" TARGET_FMT_lx "] = %02x\n",
                addr, (uint8_t) value);
        break;
    }
}

void helper_dump_load32(CPUMIPSState *env, int opc, target_ulong addr,
        uint32_t value)
{

    helper_dump_load(env, opc, addr, (target_ulong)value);
}
#endif /* TARGET_CHERI */

/* Complex FPU operations which may need stack space. */

#define FLOAT_TWO32 make_float32(1 << 30)
#define FLOAT_TWO64 make_float64(1ULL << 62)

#define FP_TO_INT32_OVERFLOW 0x7fffffff
#define FP_TO_INT64_OVERFLOW 0x7fffffffffffffffULL

/* convert MIPS rounding mode in FCR31 to IEEE library */
unsigned int ieee_rm[] = {
    float_round_nearest_even,
    float_round_to_zero,
    float_round_up,
    float_round_down
};

target_ulong helper_cfc1(CPUMIPSState *env, uint32_t reg)
{
    target_ulong arg1 = 0;

    switch (reg) {
    case 0:
        arg1 = (int32_t)env->active_fpu.fcr0;
        break;
    case 1:
        /* UFR Support - Read Status FR */
        if (env->active_fpu.fcr0 & (1 << FCR0_UFRP)) {
            if (env->CP0_Config5 & (1 << CP0C5_UFR)) {
                arg1 = (int32_t)
                       ((env->CP0_Status & (1  << CP0St_FR)) >> CP0St_FR);
            } else {
                do_raise_exception(env, EXCP_RI, GETPC());
            }
        }
        break;
    case 5:
        /* FRE Support - read Config5.FRE bit */
        if (env->active_fpu.fcr0 & (1 << FCR0_FREP)) {
            if (env->CP0_Config5 & (1 << CP0C5_UFE)) {
                arg1 = (env->CP0_Config5 >> CP0C5_FRE) & 1;
            } else {
                helper_raise_exception(env, EXCP_RI);
            }
        }
        break;
    case 25:
        arg1 = ((env->active_fpu.fcr31 >> 24) & 0xfe) | ((env->active_fpu.fcr31 >> 23) & 0x1);
        break;
    case 26:
        arg1 = env->active_fpu.fcr31 & 0x0003f07c;
        break;
    case 28:
        arg1 = (env->active_fpu.fcr31 & 0x00000f83) | ((env->active_fpu.fcr31 >> 22) & 0x4);
        break;
    default:
        arg1 = (int32_t)env->active_fpu.fcr31;
        break;
    }

    return arg1;
}

void helper_ctc1(CPUMIPSState *env, target_ulong arg1, uint32_t fs, uint32_t rt)
{
    switch (fs) {
    case 1:
        /* UFR Alias - Reset Status FR */
        if (!((env->active_fpu.fcr0 & (1 << FCR0_UFRP)) && (rt == 0))) {
            return;
        }
        if (env->CP0_Config5 & (1 << CP0C5_UFR)) {
            env->CP0_Status &= ~(1 << CP0St_FR);
            compute_hflags(env);
        } else {
            do_raise_exception(env, EXCP_RI, GETPC());
        }
        break;
    case 4:
        /* UNFR Alias - Set Status FR */
        if (!((env->active_fpu.fcr0 & (1 << FCR0_UFRP)) && (rt == 0))) {
            return;
        }
        if (env->CP0_Config5 & (1 << CP0C5_UFR)) {
            env->CP0_Status |= (1 << CP0St_FR);
            compute_hflags(env);
        } else {
            do_raise_exception(env, EXCP_RI, GETPC());
        }
        break;
    case 5:
        /* FRE Support - clear Config5.FRE bit */
        if (!((env->active_fpu.fcr0 & (1 << FCR0_FREP)) && (rt == 0))) {
            return;
        }
        if (env->CP0_Config5 & (1 << CP0C5_UFE)) {
            env->CP0_Config5 &= ~(1 << CP0C5_FRE);
            compute_hflags(env);
        } else {
            helper_raise_exception(env, EXCP_RI);
        }
        break;
    case 6:
        /* FRE Support - set Config5.FRE bit */
        if (!((env->active_fpu.fcr0 & (1 << FCR0_FREP)) && (rt == 0))) {
            return;
        }
        if (env->CP0_Config5 & (1 << CP0C5_UFE)) {
            env->CP0_Config5 |= (1 << CP0C5_FRE);
            compute_hflags(env);
        } else {
            helper_raise_exception(env, EXCP_RI);
        }
        break;
    case 25:
        if ((env->insn_flags & ISA_MIPS32R6) || (arg1 & 0xffffff00)) {
            return;
        }
        env->active_fpu.fcr31 = (env->active_fpu.fcr31 & 0x017fffff) | ((arg1 & 0xfe) << 24) |
                     ((arg1 & 0x1) << 23);
        break;
    case 26:
        if (arg1 & 0x007c0000)
            return;
        env->active_fpu.fcr31 = (env->active_fpu.fcr31 & 0xfffc0f83) | (arg1 & 0x0003f07c);
        break;
    case 28:
        if (arg1 & 0x007c0000)
            return;
        env->active_fpu.fcr31 = (env->active_fpu.fcr31 & 0xfefff07c) | (arg1 & 0x00000f83) |
                     ((arg1 & 0x4) << 22);
        break;
    case 31:
        env->active_fpu.fcr31 = (arg1 & env->active_fpu.fcr31_rw_bitmask) |
               (env->active_fpu.fcr31 & ~(env->active_fpu.fcr31_rw_bitmask));
        break;
    default:
        return;
    }
    restore_fp_status(env);
    set_float_exception_flags(0, &env->active_fpu.fp_status);
    if ((GET_FP_ENABLE(env->active_fpu.fcr31) | 0x20) & GET_FP_CAUSE(env->active_fpu.fcr31))
        do_raise_exception(env, EXCP_FPE, GETPC());
}

int ieee_ex_to_mips(int xcpt)
{
    int ret = 0;
    if (xcpt) {
        if (xcpt & float_flag_invalid) {
            ret |= FP_INVALID;
        }
        if (xcpt & float_flag_overflow) {
            ret |= FP_OVERFLOW;
        }
        if (xcpt & float_flag_underflow) {
            ret |= FP_UNDERFLOW;
        }
        if (xcpt & float_flag_divbyzero) {
            ret |= FP_DIV0;
        }
        if (xcpt & float_flag_inexact) {
            ret |= FP_INEXACT;
        }
    }
    return ret;
}

static inline void update_fcr31(CPUMIPSState *env, uintptr_t pc)
{
    int tmp = ieee_ex_to_mips(get_float_exception_flags(&env->active_fpu.fp_status));

    SET_FP_CAUSE(env->active_fpu.fcr31, tmp);

    if (tmp) {
        set_float_exception_flags(0, &env->active_fpu.fp_status);

        if (GET_FP_ENABLE(env->active_fpu.fcr31) & tmp) {
            do_raise_exception(env, EXCP_FPE, pc);
        } else {
            UPDATE_FP_FLAGS(env->active_fpu.fcr31, tmp);
        }
    }
}

/* Float support.
   Single precition routines have a "s" suffix, double precision a
   "d" suffix, 32bit integer "w", 64bit integer "l", paired single "ps",
   paired single lower "pl", paired single upper "pu".  */

/* unary operations, modifying fp status  */
uint64_t helper_float_sqrt_d(CPUMIPSState *env, uint64_t fdt0)
{
    fdt0 = float64_sqrt(fdt0, &env->active_fpu.fp_status);
    update_fcr31(env, GETPC());
    return fdt0;
}

uint32_t helper_float_sqrt_s(CPUMIPSState *env, uint32_t fst0)
{
    fst0 = float32_sqrt(fst0, &env->active_fpu.fp_status);
    update_fcr31(env, GETPC());
    return fst0;
}

uint64_t helper_float_cvtd_s(CPUMIPSState *env, uint32_t fst0)
{
    uint64_t fdt2;

    fdt2 = float32_to_float64(fst0, &env->active_fpu.fp_status);
    update_fcr31(env, GETPC());
    return fdt2;
}

uint64_t helper_float_cvtd_w(CPUMIPSState *env, uint32_t wt0)
{
    uint64_t fdt2;

    fdt2 = int32_to_float64(wt0, &env->active_fpu.fp_status);
    update_fcr31(env, GETPC());
    return fdt2;
}

uint64_t helper_float_cvtd_l(CPUMIPSState *env, uint64_t dt0)
{
    uint64_t fdt2;

    fdt2 = int64_to_float64(dt0, &env->active_fpu.fp_status);
    update_fcr31(env, GETPC());
    return fdt2;
}

uint64_t helper_float_cvt_l_d(CPUMIPSState *env, uint64_t fdt0)
{
    uint64_t dt2;

    dt2 = float64_to_int64(fdt0, &env->active_fpu.fp_status);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
        & (float_flag_invalid | float_flag_overflow)) {
        dt2 = FP_TO_INT64_OVERFLOW;
    }
    update_fcr31(env, GETPC());
    return dt2;
}

uint64_t helper_float_cvt_l_s(CPUMIPSState *env, uint32_t fst0)
{
    uint64_t dt2;

    dt2 = float32_to_int64(fst0, &env->active_fpu.fp_status);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
        & (float_flag_invalid | float_flag_overflow)) {
        dt2 = FP_TO_INT64_OVERFLOW;
    }
    update_fcr31(env, GETPC());
    return dt2;
}

uint64_t helper_float_cvtps_pw(CPUMIPSState *env, uint64_t dt0)
{
    uint32_t fst2;
    uint32_t fsth2;

    fst2 = int32_to_float32(dt0 & 0XFFFFFFFF, &env->active_fpu.fp_status);
    fsth2 = int32_to_float32(dt0 >> 32, &env->active_fpu.fp_status);
    update_fcr31(env, GETPC());
    return ((uint64_t)fsth2 << 32) | fst2;
}

uint64_t helper_float_cvtpw_ps(CPUMIPSState *env, uint64_t fdt0)
{
    uint32_t wt2;
    uint32_t wth2;
    int excp, excph;

    wt2 = float32_to_int32(fdt0 & 0XFFFFFFFF, &env->active_fpu.fp_status);
    excp = get_float_exception_flags(&env->active_fpu.fp_status);
    if (excp & (float_flag_overflow | float_flag_invalid)) {
        wt2 = FP_TO_INT32_OVERFLOW;
    }

    set_float_exception_flags(0, &env->active_fpu.fp_status);
    wth2 = float32_to_int32(fdt0 >> 32, &env->active_fpu.fp_status);
    excph = get_float_exception_flags(&env->active_fpu.fp_status);
    if (excph & (float_flag_overflow | float_flag_invalid)) {
        wth2 = FP_TO_INT32_OVERFLOW;
    }

    set_float_exception_flags(excp | excph, &env->active_fpu.fp_status);
    update_fcr31(env, GETPC());

    return ((uint64_t)wth2 << 32) | wt2;
}

uint32_t helper_float_cvts_d(CPUMIPSState *env, uint64_t fdt0)
{
    uint32_t fst2;

    fst2 = float64_to_float32(fdt0, &env->active_fpu.fp_status);
    update_fcr31(env, GETPC());
    return fst2;
}

uint32_t helper_float_cvts_w(CPUMIPSState *env, uint32_t wt0)
{
    uint32_t fst2;

    fst2 = int32_to_float32(wt0, &env->active_fpu.fp_status);
    update_fcr31(env, GETPC());
    return fst2;
}

uint32_t helper_float_cvts_l(CPUMIPSState *env, uint64_t dt0)
{
    uint32_t fst2;

    fst2 = int64_to_float32(dt0, &env->active_fpu.fp_status);
    update_fcr31(env, GETPC());
    return fst2;
}

uint32_t helper_float_cvts_pl(CPUMIPSState *env, uint32_t wt0)
{
    uint32_t wt2;

    wt2 = wt0;
    update_fcr31(env, GETPC());
    return wt2;
}

uint32_t helper_float_cvts_pu(CPUMIPSState *env, uint32_t wth0)
{
    uint32_t wt2;

    wt2 = wth0;
    update_fcr31(env, GETPC());
    return wt2;
}

uint32_t helper_float_cvt_w_s(CPUMIPSState *env, uint32_t fst0)
{
    uint32_t wt2;

    wt2 = float32_to_int32(fst0, &env->active_fpu.fp_status);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
        & (float_flag_invalid | float_flag_overflow)) {
        wt2 = FP_TO_INT32_OVERFLOW;
    }
    update_fcr31(env, GETPC());
    return wt2;
}

uint32_t helper_float_cvt_w_d(CPUMIPSState *env, uint64_t fdt0)
{
    uint32_t wt2;

    wt2 = float64_to_int32(fdt0, &env->active_fpu.fp_status);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
        & (float_flag_invalid | float_flag_overflow)) {
        wt2 = FP_TO_INT32_OVERFLOW;
    }
    update_fcr31(env, GETPC());
    return wt2;
}

uint64_t helper_float_round_l_d(CPUMIPSState *env, uint64_t fdt0)
{
    uint64_t dt2;

    set_float_rounding_mode(float_round_nearest_even, &env->active_fpu.fp_status);
    dt2 = float64_to_int64(fdt0, &env->active_fpu.fp_status);
    restore_rounding_mode(env);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
        & (float_flag_invalid | float_flag_overflow)) {
        dt2 = FP_TO_INT64_OVERFLOW;
    }
    update_fcr31(env, GETPC());
    return dt2;
}

uint64_t helper_float_round_l_s(CPUMIPSState *env, uint32_t fst0)
{
    uint64_t dt2;

    set_float_rounding_mode(float_round_nearest_even, &env->active_fpu.fp_status);
    dt2 = float32_to_int64(fst0, &env->active_fpu.fp_status);
    restore_rounding_mode(env);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
        & (float_flag_invalid | float_flag_overflow)) {
        dt2 = FP_TO_INT64_OVERFLOW;
    }
    update_fcr31(env, GETPC());
    return dt2;
}

uint32_t helper_float_round_w_d(CPUMIPSState *env, uint64_t fdt0)
{
    uint32_t wt2;

    set_float_rounding_mode(float_round_nearest_even, &env->active_fpu.fp_status);
    wt2 = float64_to_int32(fdt0, &env->active_fpu.fp_status);
    restore_rounding_mode(env);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
        & (float_flag_invalid | float_flag_overflow)) {
        wt2 = FP_TO_INT32_OVERFLOW;
    }
    update_fcr31(env, GETPC());
    return wt2;
}

uint32_t helper_float_round_w_s(CPUMIPSState *env, uint32_t fst0)
{
    uint32_t wt2;

    set_float_rounding_mode(float_round_nearest_even, &env->active_fpu.fp_status);
    wt2 = float32_to_int32(fst0, &env->active_fpu.fp_status);
    restore_rounding_mode(env);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
        & (float_flag_invalid | float_flag_overflow)) {
        wt2 = FP_TO_INT32_OVERFLOW;
    }
    update_fcr31(env, GETPC());
    return wt2;
}

uint64_t helper_float_trunc_l_d(CPUMIPSState *env, uint64_t fdt0)
{
    uint64_t dt2;

    dt2 = float64_to_int64_round_to_zero(fdt0, &env->active_fpu.fp_status);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
        & (float_flag_invalid | float_flag_overflow)) {
        dt2 = FP_TO_INT64_OVERFLOW;
    }
    update_fcr31(env, GETPC());
    return dt2;
}

uint64_t helper_float_trunc_l_s(CPUMIPSState *env, uint32_t fst0)
{
    uint64_t dt2;

    dt2 = float32_to_int64_round_to_zero(fst0, &env->active_fpu.fp_status);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
        & (float_flag_invalid | float_flag_overflow)) {
        dt2 = FP_TO_INT64_OVERFLOW;
    }
    update_fcr31(env, GETPC());
    return dt2;
}

uint32_t helper_float_trunc_w_d(CPUMIPSState *env, uint64_t fdt0)
{
    uint32_t wt2;

    wt2 = float64_to_int32_round_to_zero(fdt0, &env->active_fpu.fp_status);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
        & (float_flag_invalid | float_flag_overflow)) {
        wt2 = FP_TO_INT32_OVERFLOW;
    }
    update_fcr31(env, GETPC());
    return wt2;
}

uint32_t helper_float_trunc_w_s(CPUMIPSState *env, uint32_t fst0)
{
    uint32_t wt2;

    wt2 = float32_to_int32_round_to_zero(fst0, &env->active_fpu.fp_status);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
        & (float_flag_invalid | float_flag_overflow)) {
        wt2 = FP_TO_INT32_OVERFLOW;
    }
    update_fcr31(env, GETPC());
    return wt2;
}

uint64_t helper_float_ceil_l_d(CPUMIPSState *env, uint64_t fdt0)
{
    uint64_t dt2;

    set_float_rounding_mode(float_round_up, &env->active_fpu.fp_status);
    dt2 = float64_to_int64(fdt0, &env->active_fpu.fp_status);
    restore_rounding_mode(env);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
        & (float_flag_invalid | float_flag_overflow)) {
        dt2 = FP_TO_INT64_OVERFLOW;
    }
    update_fcr31(env, GETPC());
    return dt2;
}

uint64_t helper_float_ceil_l_s(CPUMIPSState *env, uint32_t fst0)
{
    uint64_t dt2;

    set_float_rounding_mode(float_round_up, &env->active_fpu.fp_status);
    dt2 = float32_to_int64(fst0, &env->active_fpu.fp_status);
    restore_rounding_mode(env);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
        & (float_flag_invalid | float_flag_overflow)) {
        dt2 = FP_TO_INT64_OVERFLOW;
    }
    update_fcr31(env, GETPC());
    return dt2;
}

uint32_t helper_float_ceil_w_d(CPUMIPSState *env, uint64_t fdt0)
{
    uint32_t wt2;

    set_float_rounding_mode(float_round_up, &env->active_fpu.fp_status);
    wt2 = float64_to_int32(fdt0, &env->active_fpu.fp_status);
    restore_rounding_mode(env);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
        & (float_flag_invalid | float_flag_overflow)) {
        wt2 = FP_TO_INT32_OVERFLOW;
    }
    update_fcr31(env, GETPC());
    return wt2;
}

uint32_t helper_float_ceil_w_s(CPUMIPSState *env, uint32_t fst0)
{
    uint32_t wt2;

    set_float_rounding_mode(float_round_up, &env->active_fpu.fp_status);
    wt2 = float32_to_int32(fst0, &env->active_fpu.fp_status);
    restore_rounding_mode(env);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
        & (float_flag_invalid | float_flag_overflow)) {
        wt2 = FP_TO_INT32_OVERFLOW;
    }
    update_fcr31(env, GETPC());
    return wt2;
}

uint64_t helper_float_floor_l_d(CPUMIPSState *env, uint64_t fdt0)
{
    uint64_t dt2;

    set_float_rounding_mode(float_round_down, &env->active_fpu.fp_status);
    dt2 = float64_to_int64(fdt0, &env->active_fpu.fp_status);
    restore_rounding_mode(env);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
        & (float_flag_invalid | float_flag_overflow)) {
        dt2 = FP_TO_INT64_OVERFLOW;
    }
    update_fcr31(env, GETPC());
    return dt2;
}

uint64_t helper_float_floor_l_s(CPUMIPSState *env, uint32_t fst0)
{
    uint64_t dt2;

    set_float_rounding_mode(float_round_down, &env->active_fpu.fp_status);
    dt2 = float32_to_int64(fst0, &env->active_fpu.fp_status);
    restore_rounding_mode(env);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
        & (float_flag_invalid | float_flag_overflow)) {
        dt2 = FP_TO_INT64_OVERFLOW;
    }
    update_fcr31(env, GETPC());
    return dt2;
}

uint32_t helper_float_floor_w_d(CPUMIPSState *env, uint64_t fdt0)
{
    uint32_t wt2;

    set_float_rounding_mode(float_round_down, &env->active_fpu.fp_status);
    wt2 = float64_to_int32(fdt0, &env->active_fpu.fp_status);
    restore_rounding_mode(env);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
        & (float_flag_invalid | float_flag_overflow)) {
        wt2 = FP_TO_INT32_OVERFLOW;
    }
    update_fcr31(env, GETPC());
    return wt2;
}

uint32_t helper_float_floor_w_s(CPUMIPSState *env, uint32_t fst0)
{
    uint32_t wt2;

    set_float_rounding_mode(float_round_down, &env->active_fpu.fp_status);
    wt2 = float32_to_int32(fst0, &env->active_fpu.fp_status);
    restore_rounding_mode(env);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
        & (float_flag_invalid | float_flag_overflow)) {
        wt2 = FP_TO_INT32_OVERFLOW;
    }
    update_fcr31(env, GETPC());
    return wt2;
}

uint64_t helper_float_cvt_2008_l_d(CPUMIPSState *env, uint64_t fdt0)
{
    uint64_t dt2;

    dt2 = float64_to_int64(fdt0, &env->active_fpu.fp_status);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
            & float_flag_invalid) {
        if (float64_is_any_nan(fdt0)) {
            dt2 = 0;
        }
    }
    update_fcr31(env, GETPC());
    return dt2;
}

uint64_t helper_float_cvt_2008_l_s(CPUMIPSState *env, uint32_t fst0)
{
    uint64_t dt2;

    dt2 = float32_to_int64(fst0, &env->active_fpu.fp_status);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
            & float_flag_invalid) {
        if (float32_is_any_nan(fst0)) {
            dt2 = 0;
        }
    }
    update_fcr31(env, GETPC());
    return dt2;
}

uint32_t helper_float_cvt_2008_w_d(CPUMIPSState *env, uint64_t fdt0)
{
    uint32_t wt2;

    wt2 = float64_to_int32(fdt0, &env->active_fpu.fp_status);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
            & float_flag_invalid) {
        if (float64_is_any_nan(fdt0)) {
            wt2 = 0;
        }
    }
    update_fcr31(env, GETPC());
    return wt2;
}

uint32_t helper_float_cvt_2008_w_s(CPUMIPSState *env, uint32_t fst0)
{
    uint32_t wt2;

    wt2 = float32_to_int32(fst0, &env->active_fpu.fp_status);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
            & float_flag_invalid) {
        if (float32_is_any_nan(fst0)) {
            wt2 = 0;
        }
    }
    update_fcr31(env, GETPC());
    return wt2;
}

uint64_t helper_float_round_2008_l_d(CPUMIPSState *env, uint64_t fdt0)
{
    uint64_t dt2;

    set_float_rounding_mode(float_round_nearest_even,
            &env->active_fpu.fp_status);
    dt2 = float64_to_int64(fdt0, &env->active_fpu.fp_status);
    restore_rounding_mode(env);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
            & float_flag_invalid) {
        if (float64_is_any_nan(fdt0)) {
            dt2 = 0;
        }
    }
    update_fcr31(env, GETPC());
    return dt2;
}

uint64_t helper_float_round_2008_l_s(CPUMIPSState *env, uint32_t fst0)
{
    uint64_t dt2;

    set_float_rounding_mode(float_round_nearest_even,
            &env->active_fpu.fp_status);
    dt2 = float32_to_int64(fst0, &env->active_fpu.fp_status);
    restore_rounding_mode(env);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
            & float_flag_invalid) {
        if (float32_is_any_nan(fst0)) {
            dt2 = 0;
        }
    }
    update_fcr31(env, GETPC());
    return dt2;
}

uint32_t helper_float_round_2008_w_d(CPUMIPSState *env, uint64_t fdt0)
{
    uint32_t wt2;

    set_float_rounding_mode(float_round_nearest_even,
            &env->active_fpu.fp_status);
    wt2 = float64_to_int32(fdt0, &env->active_fpu.fp_status);
    restore_rounding_mode(env);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
            & float_flag_invalid) {
        if (float64_is_any_nan(fdt0)) {
            wt2 = 0;
        }
    }
    update_fcr31(env, GETPC());
    return wt2;
}

uint32_t helper_float_round_2008_w_s(CPUMIPSState *env, uint32_t fst0)
{
    uint32_t wt2;

    set_float_rounding_mode(float_round_nearest_even,
            &env->active_fpu.fp_status);
    wt2 = float32_to_int32(fst0, &env->active_fpu.fp_status);
    restore_rounding_mode(env);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
            & float_flag_invalid) {
        if (float32_is_any_nan(fst0)) {
            wt2 = 0;
        }
    }
    update_fcr31(env, GETPC());
    return wt2;
}

uint64_t helper_float_trunc_2008_l_d(CPUMIPSState *env, uint64_t fdt0)
{
    uint64_t dt2;

    dt2 = float64_to_int64_round_to_zero(fdt0, &env->active_fpu.fp_status);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
            & float_flag_invalid) {
        if (float64_is_any_nan(fdt0)) {
            dt2 = 0;
        }
    }
    update_fcr31(env, GETPC());
    return dt2;
}

uint64_t helper_float_trunc_2008_l_s(CPUMIPSState *env, uint32_t fst0)
{
    uint64_t dt2;

    dt2 = float32_to_int64_round_to_zero(fst0, &env->active_fpu.fp_status);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
            & float_flag_invalid) {
        if (float32_is_any_nan(fst0)) {
            dt2 = 0;
        }
    }
    update_fcr31(env, GETPC());
    return dt2;
}

uint32_t helper_float_trunc_2008_w_d(CPUMIPSState *env, uint64_t fdt0)
{
    uint32_t wt2;

    wt2 = float64_to_int32_round_to_zero(fdt0, &env->active_fpu.fp_status);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
            & float_flag_invalid) {
        if (float64_is_any_nan(fdt0)) {
            wt2 = 0;
        }
    }
    update_fcr31(env, GETPC());
    return wt2;
}

uint32_t helper_float_trunc_2008_w_s(CPUMIPSState *env, uint32_t fst0)
{
    uint32_t wt2;

    wt2 = float32_to_int32_round_to_zero(fst0, &env->active_fpu.fp_status);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
            & float_flag_invalid) {
        if (float32_is_any_nan(fst0)) {
            wt2 = 0;
        }
    }
    update_fcr31(env, GETPC());
    return wt2;
}

uint64_t helper_float_ceil_2008_l_d(CPUMIPSState *env, uint64_t fdt0)
{
    uint64_t dt2;

    set_float_rounding_mode(float_round_up, &env->active_fpu.fp_status);
    dt2 = float64_to_int64(fdt0, &env->active_fpu.fp_status);
    restore_rounding_mode(env);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
            & float_flag_invalid) {
        if (float64_is_any_nan(fdt0)) {
            dt2 = 0;
        }
    }
    update_fcr31(env, GETPC());
    return dt2;
}

uint64_t helper_float_ceil_2008_l_s(CPUMIPSState *env, uint32_t fst0)
{
    uint64_t dt2;

    set_float_rounding_mode(float_round_up, &env->active_fpu.fp_status);
    dt2 = float32_to_int64(fst0, &env->active_fpu.fp_status);
    restore_rounding_mode(env);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
            & float_flag_invalid) {
        if (float32_is_any_nan(fst0)) {
            dt2 = 0;
        }
    }
    update_fcr31(env, GETPC());
    return dt2;
}

uint32_t helper_float_ceil_2008_w_d(CPUMIPSState *env, uint64_t fdt0)
{
    uint32_t wt2;

    set_float_rounding_mode(float_round_up, &env->active_fpu.fp_status);
    wt2 = float64_to_int32(fdt0, &env->active_fpu.fp_status);
    restore_rounding_mode(env);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
            & float_flag_invalid) {
        if (float64_is_any_nan(fdt0)) {
            wt2 = 0;
        }
    }
    update_fcr31(env, GETPC());
    return wt2;
}

uint32_t helper_float_ceil_2008_w_s(CPUMIPSState *env, uint32_t fst0)
{
    uint32_t wt2;

    set_float_rounding_mode(float_round_up, &env->active_fpu.fp_status);
    wt2 = float32_to_int32(fst0, &env->active_fpu.fp_status);
    restore_rounding_mode(env);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
            & float_flag_invalid) {
        if (float32_is_any_nan(fst0)) {
            wt2 = 0;
        }
    }
    update_fcr31(env, GETPC());
    return wt2;
}

uint64_t helper_float_floor_2008_l_d(CPUMIPSState *env, uint64_t fdt0)
{
    uint64_t dt2;

    set_float_rounding_mode(float_round_down, &env->active_fpu.fp_status);
    dt2 = float64_to_int64(fdt0, &env->active_fpu.fp_status);
    restore_rounding_mode(env);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
            & float_flag_invalid) {
        if (float64_is_any_nan(fdt0)) {
            dt2 = 0;
        }
    }
    update_fcr31(env, GETPC());
    return dt2;
}

uint64_t helper_float_floor_2008_l_s(CPUMIPSState *env, uint32_t fst0)
{
    uint64_t dt2;

    set_float_rounding_mode(float_round_down, &env->active_fpu.fp_status);
    dt2 = float32_to_int64(fst0, &env->active_fpu.fp_status);
    restore_rounding_mode(env);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
            & float_flag_invalid) {
        if (float32_is_any_nan(fst0)) {
            dt2 = 0;
        }
    }
    update_fcr31(env, GETPC());
    return dt2;
}

uint32_t helper_float_floor_2008_w_d(CPUMIPSState *env, uint64_t fdt0)
{
    uint32_t wt2;

    set_float_rounding_mode(float_round_down, &env->active_fpu.fp_status);
    wt2 = float64_to_int32(fdt0, &env->active_fpu.fp_status);
    restore_rounding_mode(env);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
            & float_flag_invalid) {
        if (float64_is_any_nan(fdt0)) {
            wt2 = 0;
        }
    }
    update_fcr31(env, GETPC());
    return wt2;
}

uint32_t helper_float_floor_2008_w_s(CPUMIPSState *env, uint32_t fst0)
{
    uint32_t wt2;

    set_float_rounding_mode(float_round_down, &env->active_fpu.fp_status);
    wt2 = float32_to_int32(fst0, &env->active_fpu.fp_status);
    restore_rounding_mode(env);
    if (get_float_exception_flags(&env->active_fpu.fp_status)
            & float_flag_invalid) {
        if (float32_is_any_nan(fst0)) {
            wt2 = 0;
        }
    }
    update_fcr31(env, GETPC());
    return wt2;
}

/* unary operations, not modifying fp status  */
#define FLOAT_UNOP(name)                                       \
uint64_t helper_float_ ## name ## _d(uint64_t fdt0)                \
{                                                              \
    return float64_ ## name(fdt0);                             \
}                                                              \
uint32_t helper_float_ ## name ## _s(uint32_t fst0)                \
{                                                              \
    return float32_ ## name(fst0);                             \
}                                                              \
uint64_t helper_float_ ## name ## _ps(uint64_t fdt0)               \
{                                                              \
    uint32_t wt0;                                              \
    uint32_t wth0;                                             \
                                                               \
    wt0 = float32_ ## name(fdt0 & 0XFFFFFFFF);                 \
    wth0 = float32_ ## name(fdt0 >> 32);                       \
    return ((uint64_t)wth0 << 32) | wt0;                       \
}
FLOAT_UNOP(abs)
FLOAT_UNOP(chs)
#undef FLOAT_UNOP

/* MIPS specific unary operations */
uint64_t helper_float_recip_d(CPUMIPSState *env, uint64_t fdt0)
{
    uint64_t fdt2;

    fdt2 = float64_div(float64_one, fdt0, &env->active_fpu.fp_status);
    update_fcr31(env, GETPC());
    return fdt2;
}

uint32_t helper_float_recip_s(CPUMIPSState *env, uint32_t fst0)
{
    uint32_t fst2;

    fst2 = float32_div(float32_one, fst0, &env->active_fpu.fp_status);
    update_fcr31(env, GETPC());
    return fst2;
}

uint64_t helper_float_rsqrt_d(CPUMIPSState *env, uint64_t fdt0)
{
    uint64_t fdt2;

    fdt2 = float64_sqrt(fdt0, &env->active_fpu.fp_status);
    fdt2 = float64_div(float64_one, fdt2, &env->active_fpu.fp_status);
    update_fcr31(env, GETPC());
    return fdt2;
}

uint32_t helper_float_rsqrt_s(CPUMIPSState *env, uint32_t fst0)
{
    uint32_t fst2;

    fst2 = float32_sqrt(fst0, &env->active_fpu.fp_status);
    fst2 = float32_div(float32_one, fst2, &env->active_fpu.fp_status);
    update_fcr31(env, GETPC());
    return fst2;
}

uint64_t helper_float_recip1_d(CPUMIPSState *env, uint64_t fdt0)
{
    uint64_t fdt2;

    fdt2 = float64_div(float64_one, fdt0, &env->active_fpu.fp_status);
    update_fcr31(env, GETPC());
    return fdt2;
}

uint32_t helper_float_recip1_s(CPUMIPSState *env, uint32_t fst0)
{
    uint32_t fst2;

    fst2 = float32_div(float32_one, fst0, &env->active_fpu.fp_status);
    update_fcr31(env, GETPC());
    return fst2;
}

uint64_t helper_float_recip1_ps(CPUMIPSState *env, uint64_t fdt0)
{
    uint32_t fst2;
    uint32_t fsth2;

    fst2 = float32_div(float32_one, fdt0 & 0XFFFFFFFF, &env->active_fpu.fp_status);
    fsth2 = float32_div(float32_one, fdt0 >> 32, &env->active_fpu.fp_status);
    update_fcr31(env, GETPC());
    return ((uint64_t)fsth2 << 32) | fst2;
}

uint64_t helper_float_rsqrt1_d(CPUMIPSState *env, uint64_t fdt0)
{
    uint64_t fdt2;

    fdt2 = float64_sqrt(fdt0, &env->active_fpu.fp_status);
    fdt2 = float64_div(float64_one, fdt2, &env->active_fpu.fp_status);
    update_fcr31(env, GETPC());
    return fdt2;
}

uint32_t helper_float_rsqrt1_s(CPUMIPSState *env, uint32_t fst0)
{
    uint32_t fst2;

    fst2 = float32_sqrt(fst0, &env->active_fpu.fp_status);
    fst2 = float32_div(float32_one, fst2, &env->active_fpu.fp_status);
    update_fcr31(env, GETPC());
    return fst2;
}

uint64_t helper_float_rsqrt1_ps(CPUMIPSState *env, uint64_t fdt0)
{
    uint32_t fst2;
    uint32_t fsth2;

    fst2 = float32_sqrt(fdt0 & 0XFFFFFFFF, &env->active_fpu.fp_status);
    fsth2 = float32_sqrt(fdt0 >> 32, &env->active_fpu.fp_status);
    fst2 = float32_div(float32_one, fst2, &env->active_fpu.fp_status);
    fsth2 = float32_div(float32_one, fsth2, &env->active_fpu.fp_status);
    update_fcr31(env, GETPC());
    return ((uint64_t)fsth2 << 32) | fst2;
}

#define FLOAT_RINT(name, bits)                                              \
uint ## bits ## _t helper_float_ ## name (CPUMIPSState *env,                \
                                          uint ## bits ## _t fs)            \
{                                                                           \
    uint ## bits ## _t fdret;                                               \
                                                                            \
    fdret = float ## bits ## _round_to_int(fs, &env->active_fpu.fp_status); \
    update_fcr31(env, GETPC());                                             \
    return fdret;                                                           \
}

FLOAT_RINT(rint_s, 32)
FLOAT_RINT(rint_d, 64)
#undef FLOAT_RINT

#define FLOAT_CLASS_SIGNALING_NAN      0x001
#define FLOAT_CLASS_QUIET_NAN          0x002
#define FLOAT_CLASS_NEGATIVE_INFINITY  0x004
#define FLOAT_CLASS_NEGATIVE_NORMAL    0x008
#define FLOAT_CLASS_NEGATIVE_SUBNORMAL 0x010
#define FLOAT_CLASS_NEGATIVE_ZERO      0x020
#define FLOAT_CLASS_POSITIVE_INFINITY  0x040
#define FLOAT_CLASS_POSITIVE_NORMAL    0x080
#define FLOAT_CLASS_POSITIVE_SUBNORMAL 0x100
#define FLOAT_CLASS_POSITIVE_ZERO      0x200

#define FLOAT_CLASS(name, bits)                                      \
uint ## bits ## _t float_ ## name (uint ## bits ## _t arg,           \
                                   float_status *status)             \
{                                                                    \
    if (float ## bits ## _is_signaling_nan(arg, status)) {           \
        return FLOAT_CLASS_SIGNALING_NAN;                            \
    } else if (float ## bits ## _is_quiet_nan(arg, status)) {        \
        return FLOAT_CLASS_QUIET_NAN;                                \
    } else if (float ## bits ## _is_neg(arg)) {                      \
        if (float ## bits ## _is_infinity(arg)) {                    \
            return FLOAT_CLASS_NEGATIVE_INFINITY;                    \
        } else if (float ## bits ## _is_zero(arg)) {                 \
            return FLOAT_CLASS_NEGATIVE_ZERO;                        \
        } else if (float ## bits ## _is_zero_or_denormal(arg)) {     \
            return FLOAT_CLASS_NEGATIVE_SUBNORMAL;                   \
        } else {                                                     \
            return FLOAT_CLASS_NEGATIVE_NORMAL;                      \
        }                                                            \
    } else {                                                         \
        if (float ## bits ## _is_infinity(arg)) {                    \
            return FLOAT_CLASS_POSITIVE_INFINITY;                    \
        } else if (float ## bits ## _is_zero(arg)) {                 \
            return FLOAT_CLASS_POSITIVE_ZERO;                        \
        } else if (float ## bits ## _is_zero_or_denormal(arg)) {     \
            return FLOAT_CLASS_POSITIVE_SUBNORMAL;                   \
        } else {                                                     \
            return FLOAT_CLASS_POSITIVE_NORMAL;                      \
        }                                                            \
    }                                                                \
}                                                                    \
                                                                     \
uint ## bits ## _t helper_float_ ## name (CPUMIPSState *env,         \
                                          uint ## bits ## _t arg)    \
{                                                                    \
    return float_ ## name(arg, &env->active_fpu.fp_status);          \
}

FLOAT_CLASS(class_s, 32)
FLOAT_CLASS(class_d, 64)
#undef FLOAT_CLASS

/* binary operations */
#define FLOAT_BINOP(name)                                          \
uint64_t helper_float_ ## name ## _d(CPUMIPSState *env,            \
                                     uint64_t fdt0, uint64_t fdt1) \
{                                                                  \
    uint64_t dt2;                                                  \
                                                                   \
    dt2 = float64_ ## name (fdt0, fdt1, &env->active_fpu.fp_status);     \
    update_fcr31(env, GETPC());                                    \
    return dt2;                                                    \
}                                                                  \
                                                                   \
uint32_t helper_float_ ## name ## _s(CPUMIPSState *env,            \
                                     uint32_t fst0, uint32_t fst1) \
{                                                                  \
    uint32_t wt2;                                                  \
                                                                   \
    wt2 = float32_ ## name (fst0, fst1, &env->active_fpu.fp_status);     \
    update_fcr31(env, GETPC());                                    \
    return wt2;                                                    \
}                                                                  \
                                                                   \
uint64_t helper_float_ ## name ## _ps(CPUMIPSState *env,           \
                                      uint64_t fdt0,               \
                                      uint64_t fdt1)               \
{                                                                  \
    uint32_t fst0 = fdt0 & 0XFFFFFFFF;                             \
    uint32_t fsth0 = fdt0 >> 32;                                   \
    uint32_t fst1 = fdt1 & 0XFFFFFFFF;                             \
    uint32_t fsth1 = fdt1 >> 32;                                   \
    uint32_t wt2;                                                  \
    uint32_t wth2;                                                 \
                                                                   \
    wt2 = float32_ ## name (fst0, fst1, &env->active_fpu.fp_status);     \
    wth2 = float32_ ## name (fsth0, fsth1, &env->active_fpu.fp_status);  \
    update_fcr31(env, GETPC());                                    \
    return ((uint64_t)wth2 << 32) | wt2;                           \
}

FLOAT_BINOP(add)
FLOAT_BINOP(sub)
FLOAT_BINOP(mul)
FLOAT_BINOP(div)
#undef FLOAT_BINOP

/* MIPS specific binary operations */
uint64_t helper_float_recip2_d(CPUMIPSState *env, uint64_t fdt0, uint64_t fdt2)
{
    fdt2 = float64_mul(fdt0, fdt2, &env->active_fpu.fp_status);
    fdt2 = float64_chs(float64_sub(fdt2, float64_one, &env->active_fpu.fp_status));
    update_fcr31(env, GETPC());
    return fdt2;
}

uint32_t helper_float_recip2_s(CPUMIPSState *env, uint32_t fst0, uint32_t fst2)
{
    fst2 = float32_mul(fst0, fst2, &env->active_fpu.fp_status);
    fst2 = float32_chs(float32_sub(fst2, float32_one, &env->active_fpu.fp_status));
    update_fcr31(env, GETPC());
    return fst2;
}

uint64_t helper_float_recip2_ps(CPUMIPSState *env, uint64_t fdt0, uint64_t fdt2)
{
    uint32_t fst0 = fdt0 & 0XFFFFFFFF;
    uint32_t fsth0 = fdt0 >> 32;
    uint32_t fst2 = fdt2 & 0XFFFFFFFF;
    uint32_t fsth2 = fdt2 >> 32;

    fst2 = float32_mul(fst0, fst2, &env->active_fpu.fp_status);
    fsth2 = float32_mul(fsth0, fsth2, &env->active_fpu.fp_status);
    fst2 = float32_chs(float32_sub(fst2, float32_one, &env->active_fpu.fp_status));
    fsth2 = float32_chs(float32_sub(fsth2, float32_one, &env->active_fpu.fp_status));
    update_fcr31(env, GETPC());
    return ((uint64_t)fsth2 << 32) | fst2;
}

uint64_t helper_float_rsqrt2_d(CPUMIPSState *env, uint64_t fdt0, uint64_t fdt2)
{
    fdt2 = float64_mul(fdt0, fdt2, &env->active_fpu.fp_status);
    fdt2 = float64_sub(fdt2, float64_one, &env->active_fpu.fp_status);
    fdt2 = float64_chs(float64_div(fdt2, FLOAT_TWO64, &env->active_fpu.fp_status));
    update_fcr31(env, GETPC());
    return fdt2;
}

uint32_t helper_float_rsqrt2_s(CPUMIPSState *env, uint32_t fst0, uint32_t fst2)
{
    fst2 = float32_mul(fst0, fst2, &env->active_fpu.fp_status);
    fst2 = float32_sub(fst2, float32_one, &env->active_fpu.fp_status);
    fst2 = float32_chs(float32_div(fst2, FLOAT_TWO32, &env->active_fpu.fp_status));
    update_fcr31(env, GETPC());
    return fst2;
}

uint64_t helper_float_rsqrt2_ps(CPUMIPSState *env, uint64_t fdt0, uint64_t fdt2)
{
    uint32_t fst0 = fdt0 & 0XFFFFFFFF;
    uint32_t fsth0 = fdt0 >> 32;
    uint32_t fst2 = fdt2 & 0XFFFFFFFF;
    uint32_t fsth2 = fdt2 >> 32;

    fst2 = float32_mul(fst0, fst2, &env->active_fpu.fp_status);
    fsth2 = float32_mul(fsth0, fsth2, &env->active_fpu.fp_status);
    fst2 = float32_sub(fst2, float32_one, &env->active_fpu.fp_status);
    fsth2 = float32_sub(fsth2, float32_one, &env->active_fpu.fp_status);
    fst2 = float32_chs(float32_div(fst2, FLOAT_TWO32, &env->active_fpu.fp_status));
    fsth2 = float32_chs(float32_div(fsth2, FLOAT_TWO32, &env->active_fpu.fp_status));
    update_fcr31(env, GETPC());
    return ((uint64_t)fsth2 << 32) | fst2;
}

uint64_t helper_float_addr_ps(CPUMIPSState *env, uint64_t fdt0, uint64_t fdt1)
{
    uint32_t fst0 = fdt0 & 0XFFFFFFFF;
    uint32_t fsth0 = fdt0 >> 32;
    uint32_t fst1 = fdt1 & 0XFFFFFFFF;
    uint32_t fsth1 = fdt1 >> 32;
    uint32_t fst2;
    uint32_t fsth2;

    fst2 = float32_add (fst0, fsth0, &env->active_fpu.fp_status);
    fsth2 = float32_add (fst1, fsth1, &env->active_fpu.fp_status);
    update_fcr31(env, GETPC());
    return ((uint64_t)fsth2 << 32) | fst2;
}

uint64_t helper_float_mulr_ps(CPUMIPSState *env, uint64_t fdt0, uint64_t fdt1)
{
    uint32_t fst0 = fdt0 & 0XFFFFFFFF;
    uint32_t fsth0 = fdt0 >> 32;
    uint32_t fst1 = fdt1 & 0XFFFFFFFF;
    uint32_t fsth1 = fdt1 >> 32;
    uint32_t fst2;
    uint32_t fsth2;

    fst2 = float32_mul (fst0, fsth0, &env->active_fpu.fp_status);
    fsth2 = float32_mul (fst1, fsth1, &env->active_fpu.fp_status);
    update_fcr31(env, GETPC());
    return ((uint64_t)fsth2 << 32) | fst2;
}

#define FLOAT_MINMAX(name, bits, minmaxfunc)                            \
uint ## bits ## _t helper_float_ ## name (CPUMIPSState *env,            \
                                          uint ## bits ## _t fs,        \
                                          uint ## bits ## _t ft)        \
{                                                                       \
    uint ## bits ## _t fdret;                                           \
                                                                        \
    fdret = float ## bits ## _ ## minmaxfunc(fs, ft,                    \
                                           &env->active_fpu.fp_status); \
    update_fcr31(env, GETPC());                                         \
    return fdret;                                                       \
}

FLOAT_MINMAX(max_s, 32, maxnum)
FLOAT_MINMAX(max_d, 64, maxnum)
FLOAT_MINMAX(maxa_s, 32, maxnummag)
FLOAT_MINMAX(maxa_d, 64, maxnummag)

FLOAT_MINMAX(min_s, 32, minnum)
FLOAT_MINMAX(min_d, 64, minnum)
FLOAT_MINMAX(mina_s, 32, minnummag)
FLOAT_MINMAX(mina_d, 64, minnummag)
#undef FLOAT_MINMAX

/* ternary operations */
#define UNFUSED_FMA(prefix, a, b, c, flags)                          \
{                                                                    \
    a = prefix##_mul(a, b, &env->active_fpu.fp_status);              \
    if ((flags) & float_muladd_negate_c) {                           \
        a = prefix##_sub(a, c, &env->active_fpu.fp_status);          \
    } else {                                                         \
        a = prefix##_add(a, c, &env->active_fpu.fp_status);          \
    }                                                                \
    if ((flags) & float_muladd_negate_result) {                      \
        a = prefix##_chs(a);                                         \
    }                                                                \
}

/* FMA based operations */
#define FLOAT_FMA(name, type)                                        \
uint64_t helper_float_ ## name ## _d(CPUMIPSState *env,              \
                                     uint64_t fdt0, uint64_t fdt1,   \
                                     uint64_t fdt2)                  \
{                                                                    \
    UNFUSED_FMA(float64, fdt0, fdt1, fdt2, type);                    \
    update_fcr31(env, GETPC());                                      \
    return fdt0;                                                     \
}                                                                    \
                                                                     \
uint32_t helper_float_ ## name ## _s(CPUMIPSState *env,              \
                                     uint32_t fst0, uint32_t fst1,   \
                                     uint32_t fst2)                  \
{                                                                    \
    UNFUSED_FMA(float32, fst0, fst1, fst2, type);                    \
    update_fcr31(env, GETPC());                                      \
    return fst0;                                                     \
}                                                                    \
                                                                     \
uint64_t helper_float_ ## name ## _ps(CPUMIPSState *env,             \
                                      uint64_t fdt0, uint64_t fdt1,  \
                                      uint64_t fdt2)                 \
{                                                                    \
    uint32_t fst0 = fdt0 & 0XFFFFFFFF;                               \
    uint32_t fsth0 = fdt0 >> 32;                                     \
    uint32_t fst1 = fdt1 & 0XFFFFFFFF;                               \
    uint32_t fsth1 = fdt1 >> 32;                                     \
    uint32_t fst2 = fdt2 & 0XFFFFFFFF;                               \
    uint32_t fsth2 = fdt2 >> 32;                                     \
                                                                     \
    UNFUSED_FMA(float32, fst0, fst1, fst2, type);                    \
    UNFUSED_FMA(float32, fsth0, fsth1, fsth2, type);                 \
    update_fcr31(env, GETPC());                                      \
    return ((uint64_t)fsth0 << 32) | fst0;                           \
}
FLOAT_FMA(madd, 0)
FLOAT_FMA(msub, float_muladd_negate_c)
FLOAT_FMA(nmadd, float_muladd_negate_result)
FLOAT_FMA(nmsub, float_muladd_negate_result | float_muladd_negate_c)
#undef FLOAT_FMA

#define FLOAT_FMADDSUB(name, bits, muladd_arg)                          \
uint ## bits ## _t helper_float_ ## name (CPUMIPSState *env,            \
                                          uint ## bits ## _t fs,        \
                                          uint ## bits ## _t ft,        \
                                          uint ## bits ## _t fd)        \
{                                                                       \
    uint ## bits ## _t fdret;                                           \
                                                                        \
    fdret = float ## bits ## _muladd(fs, ft, fd, muladd_arg,            \
                                     &env->active_fpu.fp_status);       \
    update_fcr31(env, GETPC());                                         \
    return fdret;                                                       \
}

FLOAT_FMADDSUB(maddf_s, 32, 0)
FLOAT_FMADDSUB(maddf_d, 64, 0)
FLOAT_FMADDSUB(msubf_s, 32, float_muladd_negate_product)
FLOAT_FMADDSUB(msubf_d, 64, float_muladd_negate_product)
#undef FLOAT_FMADDSUB

/* compare operations */
#define FOP_COND_D(op, cond)                                   \
void helper_cmp_d_ ## op(CPUMIPSState *env, uint64_t fdt0,     \
                         uint64_t fdt1, int cc)                \
{                                                              \
    int c;                                                     \
    c = cond;                                                  \
    update_fcr31(env, GETPC());                                \
    if (c)                                                     \
        SET_FP_COND(cc, env->active_fpu);                      \
    else                                                       \
        CLEAR_FP_COND(cc, env->active_fpu);                    \
}                                                              \
void helper_cmpabs_d_ ## op(CPUMIPSState *env, uint64_t fdt0,  \
                            uint64_t fdt1, int cc)             \
{                                                              \
    int c;                                                     \
    fdt0 = float64_abs(fdt0);                                  \
    fdt1 = float64_abs(fdt1);                                  \
    c = cond;                                                  \
    update_fcr31(env, GETPC());                                \
    if (c)                                                     \
        SET_FP_COND(cc, env->active_fpu);                      \
    else                                                       \
        CLEAR_FP_COND(cc, env->active_fpu);                    \
}

/* NOTE: the comma operator will make "cond" to eval to false,
 * but float64_unordered_quiet() is still called. */
FOP_COND_D(f,   (float64_unordered_quiet(fdt1, fdt0, &env->active_fpu.fp_status), 0))
FOP_COND_D(un,  float64_unordered_quiet(fdt1, fdt0, &env->active_fpu.fp_status))
FOP_COND_D(eq,  float64_eq_quiet(fdt0, fdt1, &env->active_fpu.fp_status))
FOP_COND_D(ueq, float64_unordered_quiet(fdt1, fdt0, &env->active_fpu.fp_status)  || float64_eq_quiet(fdt0, fdt1, &env->active_fpu.fp_status))
FOP_COND_D(olt, float64_lt_quiet(fdt0, fdt1, &env->active_fpu.fp_status))
FOP_COND_D(ult, float64_unordered_quiet(fdt1, fdt0, &env->active_fpu.fp_status)  || float64_lt_quiet(fdt0, fdt1, &env->active_fpu.fp_status))
FOP_COND_D(ole, float64_le_quiet(fdt0, fdt1, &env->active_fpu.fp_status))
FOP_COND_D(ule, float64_unordered_quiet(fdt1, fdt0, &env->active_fpu.fp_status)  || float64_le_quiet(fdt0, fdt1, &env->active_fpu.fp_status))
/* NOTE: the comma operator will make "cond" to eval to false,
 * but float64_unordered() is still called. */
FOP_COND_D(sf,  (float64_unordered(fdt1, fdt0, &env->active_fpu.fp_status), 0))
FOP_COND_D(ngle,float64_unordered(fdt1, fdt0, &env->active_fpu.fp_status))
FOP_COND_D(seq, float64_eq(fdt0, fdt1, &env->active_fpu.fp_status))
FOP_COND_D(ngl, float64_unordered(fdt1, fdt0, &env->active_fpu.fp_status)  || float64_eq(fdt0, fdt1, &env->active_fpu.fp_status))
FOP_COND_D(lt,  float64_lt(fdt0, fdt1, &env->active_fpu.fp_status))
FOP_COND_D(nge, float64_unordered(fdt1, fdt0, &env->active_fpu.fp_status)  || float64_lt(fdt0, fdt1, &env->active_fpu.fp_status))
FOP_COND_D(le,  float64_le(fdt0, fdt1, &env->active_fpu.fp_status))
FOP_COND_D(ngt, float64_unordered(fdt1, fdt0, &env->active_fpu.fp_status)  || float64_le(fdt0, fdt1, &env->active_fpu.fp_status))

#define FOP_COND_S(op, cond)                                   \
void helper_cmp_s_ ## op(CPUMIPSState *env, uint32_t fst0,     \
                         uint32_t fst1, int cc)                \
{                                                              \
    int c;                                                     \
    c = cond;                                                  \
    update_fcr31(env, GETPC());                                \
    if (c)                                                     \
        SET_FP_COND(cc, env->active_fpu);                      \
    else                                                       \
        CLEAR_FP_COND(cc, env->active_fpu);                    \
}                                                              \
void helper_cmpabs_s_ ## op(CPUMIPSState *env, uint32_t fst0,  \
                            uint32_t fst1, int cc)             \
{                                                              \
    int c;                                                     \
    fst0 = float32_abs(fst0);                                  \
    fst1 = float32_abs(fst1);                                  \
    c = cond;                                                  \
    update_fcr31(env, GETPC());                                \
    if (c)                                                     \
        SET_FP_COND(cc, env->active_fpu);                      \
    else                                                       \
        CLEAR_FP_COND(cc, env->active_fpu);                    \
}

/* NOTE: the comma operator will make "cond" to eval to false,
 * but float32_unordered_quiet() is still called. */
FOP_COND_S(f,   (float32_unordered_quiet(fst1, fst0, &env->active_fpu.fp_status), 0))
FOP_COND_S(un,  float32_unordered_quiet(fst1, fst0, &env->active_fpu.fp_status))
FOP_COND_S(eq,  float32_eq_quiet(fst0, fst1, &env->active_fpu.fp_status))
FOP_COND_S(ueq, float32_unordered_quiet(fst1, fst0, &env->active_fpu.fp_status)  || float32_eq_quiet(fst0, fst1, &env->active_fpu.fp_status))
FOP_COND_S(olt, float32_lt_quiet(fst0, fst1, &env->active_fpu.fp_status))
FOP_COND_S(ult, float32_unordered_quiet(fst1, fst0, &env->active_fpu.fp_status)  || float32_lt_quiet(fst0, fst1, &env->active_fpu.fp_status))
FOP_COND_S(ole, float32_le_quiet(fst0, fst1, &env->active_fpu.fp_status))
FOP_COND_S(ule, float32_unordered_quiet(fst1, fst0, &env->active_fpu.fp_status)  || float32_le_quiet(fst0, fst1, &env->active_fpu.fp_status))
/* NOTE: the comma operator will make "cond" to eval to false,
 * but float32_unordered() is still called. */
FOP_COND_S(sf,  (float32_unordered(fst1, fst0, &env->active_fpu.fp_status), 0))
FOP_COND_S(ngle,float32_unordered(fst1, fst0, &env->active_fpu.fp_status))
FOP_COND_S(seq, float32_eq(fst0, fst1, &env->active_fpu.fp_status))
FOP_COND_S(ngl, float32_unordered(fst1, fst0, &env->active_fpu.fp_status)  || float32_eq(fst0, fst1, &env->active_fpu.fp_status))
FOP_COND_S(lt,  float32_lt(fst0, fst1, &env->active_fpu.fp_status))
FOP_COND_S(nge, float32_unordered(fst1, fst0, &env->active_fpu.fp_status)  || float32_lt(fst0, fst1, &env->active_fpu.fp_status))
FOP_COND_S(le,  float32_le(fst0, fst1, &env->active_fpu.fp_status))
FOP_COND_S(ngt, float32_unordered(fst1, fst0, &env->active_fpu.fp_status)  || float32_le(fst0, fst1, &env->active_fpu.fp_status))

#define FOP_COND_PS(op, condl, condh)                           \
void helper_cmp_ps_ ## op(CPUMIPSState *env, uint64_t fdt0,     \
                          uint64_t fdt1, int cc)                \
{                                                               \
    uint32_t fst0, fsth0, fst1, fsth1;                          \
    int ch, cl;                                                 \
    fst0 = fdt0 & 0XFFFFFFFF;                                   \
    fsth0 = fdt0 >> 32;                                         \
    fst1 = fdt1 & 0XFFFFFFFF;                                   \
    fsth1 = fdt1 >> 32;                                         \
    cl = condl;                                                 \
    ch = condh;                                                 \
    update_fcr31(env, GETPC());                                 \
    if (cl)                                                     \
        SET_FP_COND(cc, env->active_fpu);                       \
    else                                                        \
        CLEAR_FP_COND(cc, env->active_fpu);                     \
    if (ch)                                                     \
        SET_FP_COND(cc + 1, env->active_fpu);                   \
    else                                                        \
        CLEAR_FP_COND(cc + 1, env->active_fpu);                 \
}                                                               \
void helper_cmpabs_ps_ ## op(CPUMIPSState *env, uint64_t fdt0,  \
                             uint64_t fdt1, int cc)             \
{                                                               \
    uint32_t fst0, fsth0, fst1, fsth1;                          \
    int ch, cl;                                                 \
    fst0 = float32_abs(fdt0 & 0XFFFFFFFF);                      \
    fsth0 = float32_abs(fdt0 >> 32);                            \
    fst1 = float32_abs(fdt1 & 0XFFFFFFFF);                      \
    fsth1 = float32_abs(fdt1 >> 32);                            \
    cl = condl;                                                 \
    ch = condh;                                                 \
    update_fcr31(env, GETPC());                                 \
    if (cl)                                                     \
        SET_FP_COND(cc, env->active_fpu);                       \
    else                                                        \
        CLEAR_FP_COND(cc, env->active_fpu);                     \
    if (ch)                                                     \
        SET_FP_COND(cc + 1, env->active_fpu);                   \
    else                                                        \
        CLEAR_FP_COND(cc + 1, env->active_fpu);                 \
}

/* NOTE: the comma operator will make "cond" to eval to false,
 * but float32_unordered_quiet() is still called. */
FOP_COND_PS(f,   (float32_unordered_quiet(fst1, fst0, &env->active_fpu.fp_status), 0),
                 (float32_unordered_quiet(fsth1, fsth0, &env->active_fpu.fp_status), 0))
FOP_COND_PS(un,  float32_unordered_quiet(fst1, fst0, &env->active_fpu.fp_status),
                 float32_unordered_quiet(fsth1, fsth0, &env->active_fpu.fp_status))
FOP_COND_PS(eq,  float32_eq_quiet(fst0, fst1, &env->active_fpu.fp_status),
                 float32_eq_quiet(fsth0, fsth1, &env->active_fpu.fp_status))
FOP_COND_PS(ueq, float32_unordered_quiet(fst1, fst0, &env->active_fpu.fp_status)    || float32_eq_quiet(fst0, fst1, &env->active_fpu.fp_status),
                 float32_unordered_quiet(fsth1, fsth0, &env->active_fpu.fp_status)  || float32_eq_quiet(fsth0, fsth1, &env->active_fpu.fp_status))
FOP_COND_PS(olt, float32_lt_quiet(fst0, fst1, &env->active_fpu.fp_status),
                 float32_lt_quiet(fsth0, fsth1, &env->active_fpu.fp_status))
FOP_COND_PS(ult, float32_unordered_quiet(fst1, fst0, &env->active_fpu.fp_status)    || float32_lt_quiet(fst0, fst1, &env->active_fpu.fp_status),
                 float32_unordered_quiet(fsth1, fsth0, &env->active_fpu.fp_status)  || float32_lt_quiet(fsth0, fsth1, &env->active_fpu.fp_status))
FOP_COND_PS(ole, float32_le_quiet(fst0, fst1, &env->active_fpu.fp_status),
                 float32_le_quiet(fsth0, fsth1, &env->active_fpu.fp_status))
FOP_COND_PS(ule, float32_unordered_quiet(fst1, fst0, &env->active_fpu.fp_status)    || float32_le_quiet(fst0, fst1, &env->active_fpu.fp_status),
                 float32_unordered_quiet(fsth1, fsth0, &env->active_fpu.fp_status)  || float32_le_quiet(fsth0, fsth1, &env->active_fpu.fp_status))
/* NOTE: the comma operator will make "cond" to eval to false,
 * but float32_unordered() is still called. */
FOP_COND_PS(sf,  (float32_unordered(fst1, fst0, &env->active_fpu.fp_status), 0),
                 (float32_unordered(fsth1, fsth0, &env->active_fpu.fp_status), 0))
FOP_COND_PS(ngle,float32_unordered(fst1, fst0, &env->active_fpu.fp_status),
                 float32_unordered(fsth1, fsth0, &env->active_fpu.fp_status))
FOP_COND_PS(seq, float32_eq(fst0, fst1, &env->active_fpu.fp_status),
                 float32_eq(fsth0, fsth1, &env->active_fpu.fp_status))
FOP_COND_PS(ngl, float32_unordered(fst1, fst0, &env->active_fpu.fp_status)    || float32_eq(fst0, fst1, &env->active_fpu.fp_status),
                 float32_unordered(fsth1, fsth0, &env->active_fpu.fp_status)  || float32_eq(fsth0, fsth1, &env->active_fpu.fp_status))
FOP_COND_PS(lt,  float32_lt(fst0, fst1, &env->active_fpu.fp_status),
                 float32_lt(fsth0, fsth1, &env->active_fpu.fp_status))
FOP_COND_PS(nge, float32_unordered(fst1, fst0, &env->active_fpu.fp_status)    || float32_lt(fst0, fst1, &env->active_fpu.fp_status),
                 float32_unordered(fsth1, fsth0, &env->active_fpu.fp_status)  || float32_lt(fsth0, fsth1, &env->active_fpu.fp_status))
FOP_COND_PS(le,  float32_le(fst0, fst1, &env->active_fpu.fp_status),
                 float32_le(fsth0, fsth1, &env->active_fpu.fp_status))
FOP_COND_PS(ngt, float32_unordered(fst1, fst0, &env->active_fpu.fp_status)    || float32_le(fst0, fst1, &env->active_fpu.fp_status),
                 float32_unordered(fsth1, fsth0, &env->active_fpu.fp_status)  || float32_le(fsth0, fsth1, &env->active_fpu.fp_status))

/* R6 compare operations */
#define FOP_CONDN_D(op, cond)                                       \
uint64_t helper_r6_cmp_d_ ## op(CPUMIPSState * env, uint64_t fdt0,  \
                         uint64_t fdt1)                             \
{                                                                   \
    uint64_t c;                                                     \
    c = cond;                                                       \
    update_fcr31(env, GETPC());                                     \
    if (c) {                                                        \
        return -1;                                                  \
    } else {                                                        \
        return 0;                                                   \
    }                                                               \
}

/* NOTE: the comma operator will make "cond" to eval to false,
 * but float64_unordered_quiet() is still called. */
FOP_CONDN_D(af,  (float64_unordered_quiet(fdt1, fdt0, &env->active_fpu.fp_status), 0))
FOP_CONDN_D(un,  (float64_unordered_quiet(fdt1, fdt0, &env->active_fpu.fp_status)))
FOP_CONDN_D(eq,  (float64_eq_quiet(fdt0, fdt1, &env->active_fpu.fp_status)))
FOP_CONDN_D(ueq, (float64_unordered_quiet(fdt1, fdt0, &env->active_fpu.fp_status)
                  || float64_eq_quiet(fdt0, fdt1, &env->active_fpu.fp_status)))
FOP_CONDN_D(lt,  (float64_lt_quiet(fdt0, fdt1, &env->active_fpu.fp_status)))
FOP_CONDN_D(ult, (float64_unordered_quiet(fdt1, fdt0, &env->active_fpu.fp_status)
                  || float64_lt_quiet(fdt0, fdt1, &env->active_fpu.fp_status)))
FOP_CONDN_D(le,  (float64_le_quiet(fdt0, fdt1, &env->active_fpu.fp_status)))
FOP_CONDN_D(ule, (float64_unordered_quiet(fdt1, fdt0, &env->active_fpu.fp_status)
                  || float64_le_quiet(fdt0, fdt1, &env->active_fpu.fp_status)))
/* NOTE: the comma operator will make "cond" to eval to false,
 * but float64_unordered() is still called. */
FOP_CONDN_D(saf,  (float64_unordered(fdt1, fdt0, &env->active_fpu.fp_status), 0))
FOP_CONDN_D(sun,  (float64_unordered(fdt1, fdt0, &env->active_fpu.fp_status)))
FOP_CONDN_D(seq,  (float64_eq(fdt0, fdt1, &env->active_fpu.fp_status)))
FOP_CONDN_D(sueq, (float64_unordered(fdt1, fdt0, &env->active_fpu.fp_status)
                   || float64_eq(fdt0, fdt1, &env->active_fpu.fp_status)))
FOP_CONDN_D(slt,  (float64_lt(fdt0, fdt1, &env->active_fpu.fp_status)))
FOP_CONDN_D(sult, (float64_unordered(fdt1, fdt0, &env->active_fpu.fp_status)
                   || float64_lt(fdt0, fdt1, &env->active_fpu.fp_status)))
FOP_CONDN_D(sle,  (float64_le(fdt0, fdt1, &env->active_fpu.fp_status)))
FOP_CONDN_D(sule, (float64_unordered(fdt1, fdt0, &env->active_fpu.fp_status)
                   || float64_le(fdt0, fdt1, &env->active_fpu.fp_status)))
FOP_CONDN_D(or,   (float64_le_quiet(fdt1, fdt0, &env->active_fpu.fp_status)
                   || float64_le_quiet(fdt0, fdt1, &env->active_fpu.fp_status)))
FOP_CONDN_D(une,  (float64_unordered_quiet(fdt1, fdt0, &env->active_fpu.fp_status)
                   || float64_lt_quiet(fdt1, fdt0, &env->active_fpu.fp_status)
                   || float64_lt_quiet(fdt0, fdt1, &env->active_fpu.fp_status)))
FOP_CONDN_D(ne,   (float64_lt_quiet(fdt1, fdt0, &env->active_fpu.fp_status)
                   || float64_lt_quiet(fdt0, fdt1, &env->active_fpu.fp_status)))
FOP_CONDN_D(sor,  (float64_le(fdt1, fdt0, &env->active_fpu.fp_status)
                   || float64_le(fdt0, fdt1, &env->active_fpu.fp_status)))
FOP_CONDN_D(sune, (float64_unordered(fdt1, fdt0, &env->active_fpu.fp_status)
                   || float64_lt(fdt1, fdt0, &env->active_fpu.fp_status)
                   || float64_lt(fdt0, fdt1, &env->active_fpu.fp_status)))
FOP_CONDN_D(sne,  (float64_lt(fdt1, fdt0, &env->active_fpu.fp_status)
                   || float64_lt(fdt0, fdt1, &env->active_fpu.fp_status)))

#define FOP_CONDN_S(op, cond)                                       \
uint32_t helper_r6_cmp_s_ ## op(CPUMIPSState * env, uint32_t fst0,  \
                         uint32_t fst1)                             \
{                                                                   \
    uint64_t c;                                                     \
    c = cond;                                                       \
    update_fcr31(env, GETPC());                                     \
    if (c) {                                                        \
        return -1;                                                  \
    } else {                                                        \
        return 0;                                                   \
    }                                                               \
}

/* NOTE: the comma operator will make "cond" to eval to false,
 * but float32_unordered_quiet() is still called. */
FOP_CONDN_S(af,   (float32_unordered_quiet(fst1, fst0, &env->active_fpu.fp_status), 0))
FOP_CONDN_S(un,   (float32_unordered_quiet(fst1, fst0, &env->active_fpu.fp_status)))
FOP_CONDN_S(eq,   (float32_eq_quiet(fst0, fst1, &env->active_fpu.fp_status)))
FOP_CONDN_S(ueq,  (float32_unordered_quiet(fst1, fst0, &env->active_fpu.fp_status)
                   || float32_eq_quiet(fst0, fst1, &env->active_fpu.fp_status)))
FOP_CONDN_S(lt,   (float32_lt_quiet(fst0, fst1, &env->active_fpu.fp_status)))
FOP_CONDN_S(ult,  (float32_unordered_quiet(fst1, fst0, &env->active_fpu.fp_status)
                   || float32_lt_quiet(fst0, fst1, &env->active_fpu.fp_status)))
FOP_CONDN_S(le,   (float32_le_quiet(fst0, fst1, &env->active_fpu.fp_status)))
FOP_CONDN_S(ule,  (float32_unordered_quiet(fst1, fst0, &env->active_fpu.fp_status)
                   || float32_le_quiet(fst0, fst1, &env->active_fpu.fp_status)))
/* NOTE: the comma operator will make "cond" to eval to false,
 * but float32_unordered() is still called. */
FOP_CONDN_S(saf,  (float32_unordered(fst1, fst0, &env->active_fpu.fp_status), 0))
FOP_CONDN_S(sun,  (float32_unordered(fst1, fst0, &env->active_fpu.fp_status)))
FOP_CONDN_S(seq,  (float32_eq(fst0, fst1, &env->active_fpu.fp_status)))
FOP_CONDN_S(sueq, (float32_unordered(fst1, fst0, &env->active_fpu.fp_status)
                   || float32_eq(fst0, fst1, &env->active_fpu.fp_status)))
FOP_CONDN_S(slt,  (float32_lt(fst0, fst1, &env->active_fpu.fp_status)))
FOP_CONDN_S(sult, (float32_unordered(fst1, fst0, &env->active_fpu.fp_status)
                   || float32_lt(fst0, fst1, &env->active_fpu.fp_status)))
FOP_CONDN_S(sle,  (float32_le(fst0, fst1, &env->active_fpu.fp_status)))
FOP_CONDN_S(sule, (float32_unordered(fst1, fst0, &env->active_fpu.fp_status)
                   || float32_le(fst0, fst1, &env->active_fpu.fp_status)))
FOP_CONDN_S(or,   (float32_le_quiet(fst1, fst0, &env->active_fpu.fp_status)
                   || float32_le_quiet(fst0, fst1, &env->active_fpu.fp_status)))
FOP_CONDN_S(une,  (float32_unordered_quiet(fst1, fst0, &env->active_fpu.fp_status)
                   || float32_lt_quiet(fst1, fst0, &env->active_fpu.fp_status)
                   || float32_lt_quiet(fst0, fst1, &env->active_fpu.fp_status)))
FOP_CONDN_S(ne,   (float32_lt_quiet(fst1, fst0, &env->active_fpu.fp_status)
                   || float32_lt_quiet(fst0, fst1, &env->active_fpu.fp_status)))
FOP_CONDN_S(sor,  (float32_le(fst1, fst0, &env->active_fpu.fp_status)
                   || float32_le(fst0, fst1, &env->active_fpu.fp_status)))
FOP_CONDN_S(sune, (float32_unordered(fst1, fst0, &env->active_fpu.fp_status)
                   || float32_lt(fst1, fst0, &env->active_fpu.fp_status)
                   || float32_lt(fst0, fst1, &env->active_fpu.fp_status)))
FOP_CONDN_S(sne,  (float32_lt(fst1, fst0, &env->active_fpu.fp_status)
                   || float32_lt(fst0, fst1, &env->active_fpu.fp_status)))

/* MSA */
/* Data format min and max values */
#define DF_BITS(df) (1 << ((df) + 3))

/* Element-by-element access macros */
#define DF_ELEMENTS(df) (MSA_WRLEN / DF_BITS(df))

#if !defined(CONFIG_USER_ONLY)
#define MEMOP_IDX(DF)                                           \
        TCGMemOpIdx oi = make_memop_idx(MO_TE | DF | MO_UNALN,  \
                                        cpu_mmu_index(env, false));
#else
#define MEMOP_IDX(DF)
#endif

#define MSA_LD_DF(DF, TYPE, LD_INSN, ...)                               \
void helper_msa_ld_ ## TYPE(CPUMIPSState *env, uint32_t wd,             \
                            target_ulong addr)                          \
{                                                                       \
    wr_t *pwd = &(env->active_fpu.fpr[wd].wr);                          \
    wr_t wx;                                                            \
    int i;                                                              \
    MEMOP_IDX(DF)                                                       \
    for (i = 0; i < DF_ELEMENTS(DF); i++) {                             \
        wx.TYPE[i] = LD_INSN(env, addr + (i << DF), ##__VA_ARGS__);     \
    }                                                                   \
    memcpy(pwd, &wx, sizeof(wr_t));                                     \
}

#if !defined(CONFIG_USER_ONLY)
MSA_LD_DF(DF_BYTE,   b, helper_ret_ldub_mmu, oi, GETPC())
MSA_LD_DF(DF_HALF,   h, helper_ret_lduw_mmu, oi, GETPC())
MSA_LD_DF(DF_WORD,   w, helper_ret_ldul_mmu, oi, GETPC())
MSA_LD_DF(DF_DOUBLE, d, helper_ret_ldq_mmu,  oi, GETPC())
#else
MSA_LD_DF(DF_BYTE,   b, cpu_ldub_data)
MSA_LD_DF(DF_HALF,   h, cpu_lduw_data)
MSA_LD_DF(DF_WORD,   w, cpu_ldl_data)
MSA_LD_DF(DF_DOUBLE, d, cpu_ldq_data)
#endif

#define MSA_PAGESPAN(x) \
        ((((x) & ~TARGET_PAGE_MASK) + MSA_WRLEN/8 - 1) >= TARGET_PAGE_SIZE)

static inline void ensure_writable_pages(CPUMIPSState *env,
                                         target_ulong addr,
                                         int mmu_idx,
                                         uintptr_t retaddr)
{
#if !defined(CONFIG_USER_ONLY)
    target_ulong page_addr;
    if (unlikely(MSA_PAGESPAN(addr))) {
        /* first page */
        probe_write(env, addr, 0, mmu_idx, retaddr);
        /* second page */
        page_addr = (addr & TARGET_PAGE_MASK) + TARGET_PAGE_SIZE;
        probe_write(env, page_addr, 0, mmu_idx, retaddr);
    }
#endif
}

#define MSA_ST_DF(DF, TYPE, ST_INSN, ...)                               \
void helper_msa_st_ ## TYPE(CPUMIPSState *env, uint32_t wd,             \
                            target_ulong addr)                          \
{                                                                       \
    wr_t *pwd = &(env->active_fpu.fpr[wd].wr);                          \
    int mmu_idx = cpu_mmu_index(env, false);				\
    int i;                                                              \
    MEMOP_IDX(DF)                                                       \
    ensure_writable_pages(env, addr, mmu_idx, GETPC());                 \
    for (i = 0; i < DF_ELEMENTS(DF); i++) {                             \
        ST_INSN(env, addr + (i << DF), pwd->TYPE[i], ##__VA_ARGS__);    \
    }                                                                   \
}

#if !defined(CONFIG_USER_ONLY)
MSA_ST_DF(DF_BYTE,   b, helper_ret_stb_mmu, oi, GETPC())
MSA_ST_DF(DF_HALF,   h, helper_ret_stw_mmu, oi, GETPC())
MSA_ST_DF(DF_WORD,   w, helper_ret_stl_mmu, oi, GETPC())
MSA_ST_DF(DF_DOUBLE, d, helper_ret_stq_mmu, oi, GETPC())
#else
MSA_ST_DF(DF_BYTE,   b, cpu_stb_data)
MSA_ST_DF(DF_HALF,   h, cpu_stw_data)
MSA_ST_DF(DF_WORD,   w, cpu_stl_data)
MSA_ST_DF(DF_DOUBLE, d, cpu_stq_data)
#endif

void helper_cache(CPUMIPSState *env, target_ulong addr, uint32_t op)
{
#ifndef CONFIG_USER_ONLY
    target_ulong index = addr & 0x1fffffff;
    if (op == 9) {
        /* Index Store Tag */
        memory_region_dispatch_write(env->itc_tag, index, env->CP0_TagLo,
                                     8, MEMTXATTRS_UNSPECIFIED);
    } else if (op == 5) {
        /* Index Load Tag */
        memory_region_dispatch_read(env->itc_tag, index, &env->CP0_TagLo,
                                    8, MEMTXATTRS_UNSPECIFIED);
    }
#endif
}
