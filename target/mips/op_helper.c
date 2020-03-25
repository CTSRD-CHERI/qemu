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
 *
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
#include "exec/memop.h"
#ifdef CONFIG_MIPS_LOG_INSTR
#include "exec/log.h"
#endif
#ifdef TARGET_CHERI
#include "cheri_tagmem.h"
#include "cheri-helper-utils.h"
#endif
#include "sysemu/kvm.h"


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

void helper_check_breakcount(struct CPUMIPSState* env)
{
    CPUState *cs = env_cpu(env);
    /* Decrement the startup breakcount, if set. */
    if (unlikely(cs->breakcount)) {
        cs->breakcount--;
        if (cs->breakcount == 0UL) {
            qemu_log_mask(CPU_LOG_INSTR | CPU_LOG_INT | CPU_LOG_EXEC, "Reached breakcount!\n");
            helper_raise_exception_debug(env);
        }
    }
}

/* 64 bits arithmetic for 32 bits hosts */
static inline uint64_t get_HILO(CPUMIPSState *env)
{
    return ((uint64_t)(env->active_tc.HI[0]) << 32) |
           (uint32_t)env->active_tc.LO[0];
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

target_ulong helper_rotx(target_ulong rs, uint32_t shift, uint32_t shiftx,
                        uint32_t stripe)
{
    int i;
    uint64_t tmp0 = ((uint64_t)rs) << 32 | ((uint64_t)rs & 0xffffffff);
    uint64_t tmp1 = tmp0;
    for (i = 0; i <= 46; i++) {
        int s;
        if (i & 0x8) {
            s = shift;
        } else {
            s = shiftx;
        }

        if (stripe != 0 && !(i & 0x4)) {
            s = ~s;
        }
        if (s & 0x10) {
            if (tmp0 & (1LL << (i + 16))) {
                tmp1 |= 1LL << i;
            } else {
                tmp1 &= ~(1LL << i);
            }
        }
    }

    uint64_t tmp2 = tmp1;
    for (i = 0; i <= 38; i++) {
        int s;
        if (i & 0x4) {
            s = shift;
        } else {
            s = shiftx;
        }

        if (s & 0x8) {
            if (tmp1 & (1LL << (i + 8))) {
                tmp2 |= 1LL << i;
            } else {
                tmp2 &= ~(1LL << i);
            }
        }
    }

    uint64_t tmp3 = tmp2;
    for (i = 0; i <= 34; i++) {
        int s;
        if (i & 0x2) {
            s = shift;
        } else {
            s = shiftx;
        }
        if (s & 0x4) {
            if (tmp2 & (1LL << (i + 4))) {
                tmp3 |= 1LL << i;
            } else {
                tmp3 &= ~(1LL << i);
            }
        }
    }

    uint64_t tmp4 = tmp3;
    for (i = 0; i <= 32; i++) {
        int s;
        if (i & 0x1) {
            s = shift;
        } else {
            s = shiftx;
        }
        if (s & 0x2) {
            if (tmp3 & (1LL << (i + 2))) {
                tmp4 |= 1LL << i;
            } else {
                tmp4 &= ~(1LL << i);
            }
        }
    }

    uint64_t tmp5 = tmp4;
    for (i = 0; i <= 31; i++) {
        int s;
        s = shift;
        if (s & 0x1) {
            if (tmp4 & (1LL << (i + 1))) {
                tmp5 |= 1LL << i;
            } else {
                tmp5 &= ~(1LL << i);
            }
        }
    }

    return (int64_t)(int32_t)(uint32_t)tmp5;
}

#ifndef CONFIG_USER_ONLY

hwaddr do_translate_address(CPUMIPSState *env, target_ulong address, int rw,
                            uintptr_t retaddr) {
    hwaddr paddr;
    CPUState *cs = env_cpu(env);

    paddr = cpu_mips_translate_address(env, address, rw);

    if (paddr == -1LL) {
        cpu_loop_exit_restore(cs, retaddr);
    } else {
        return paddr;
    }
}

#define HELPER_LD_ATOMIC(name, insn, almask, do_cast)                         \
target_ulong helper_##name(CPUMIPSState *env, target_ulong arg, int mem_idx)  \
{                                                                             \
    if (arg & almask) {                                                       \
        if (!(env->hflags & MIPS_HFLAG_DM)) {                                 \
            env->CP0_BadVAddr = arg;                                          \
        }                                                                     \
        do_raise_exception(env, EXCP_AdEL, GETPC());                          \
    }                                                                         \
    env->CP0_LLAddr = do_translate_address(env, arg, 0, GETPC());             \
    env->lladdr = arg;                                                        \
    env->llval = do_cast cpu_##insn##_mmuidx_ra(env, arg, mem_idx, GETPC());  \
    return env->llval;                                                        \
}
HELPER_LD_ATOMIC(ll, ldl, 0x3, (target_long)(int32_t))
#ifdef TARGET_MIPS64
HELPER_LD_ATOMIC(lld, ldq, 0x7, (target_ulong))
#endif
#undef HELPER_LD_ATOMIC
#endif

#ifdef TARGET_WORDS_BIGENDIAN
#define GET_LMASK(v) ((v) & 3)
#define GET_OFFSET(addr, offset) (addr + (offset))
#else
#define GET_LMASK(v) (((v) & 3) ^ 3)
#define GET_OFFSET(addr, offset) (addr - (offset))
#endif

#ifdef TARGET_CHERI
static inline target_ulong ccheck_store_right(CPUMIPSState *env, target_ulong offset, uint32_t len, uintptr_t retpc)
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
    return check_ddc(env, CAP_PERM_STORE, write_offset, stored_bytes, retpc) + low_bits;
}
#endif

static inline void invalidate_tags_store_left_right(CPUMIPSState *env,
                                                    target_ulong addr,
                                                    uintptr_t retpc) {
#ifdef TARGET_CHERI
    // swr/sdr/swl/sdl will never invalidate more than one capability
    cheri_tag_invalidate(env, addr, 1, retpc);
#endif
}



void helper_swl(CPUMIPSState *env, target_ulong arg1, target_ulong arg2,
                int mem_idx)
{
#ifdef TARGET_CHERI
    const int num_bytes = 4 - GET_LMASK(arg2);
    arg2 = check_ddc(env, CAP_PERM_STORE, arg2, num_bytes, GETPC());
#endif
    cpu_stb_mmuidx_ra(env, arg2, (uint8_t)(arg1 >> 24), mem_idx, GETPC());

    if (GET_LMASK(arg2) <= 2) {
        cpu_stb_mmuidx_ra(env, GET_OFFSET(arg2, 1), (uint8_t)(arg1 >> 16),
                          mem_idx, GETPC());
    }

    if (GET_LMASK(arg2) <= 1) {
        cpu_stb_mmuidx_ra(env, GET_OFFSET(arg2, 2), (uint8_t)(arg1 >> 8),
                          mem_idx, GETPC());
    }

    if (GET_LMASK(arg2) == 0) {
        cpu_stb_mmuidx_ra(env, GET_OFFSET(arg2, 3), (uint8_t)arg1,
                          mem_idx, GETPC());
    }
    invalidate_tags_store_left_right(env, arg2, GETPC());
}

void helper_swr(CPUMIPSState *env, target_ulong arg1, target_ulong arg2,
                int mem_idx) {
#ifdef TARGET_CHERI
    arg2 = ccheck_store_right(env, arg2, 4, GETPC());
#endif
    cpu_stb_mmuidx_ra(env, arg2, (uint8_t)arg1, mem_idx, GETPC());

    if (GET_LMASK(arg2) >= 1) {
        cpu_stb_mmuidx_ra(env, GET_OFFSET(arg2, -1), (uint8_t)(arg1 >> 8),
                          mem_idx, GETPC());
    }

    if (GET_LMASK(arg2) >= 2) {
        cpu_stb_mmuidx_ra(env, GET_OFFSET(arg2, -2), (uint8_t)(arg1 >> 16),
                          mem_idx, GETPC());
    }

    if (GET_LMASK(arg2) == 3) {
        cpu_stb_mmuidx_ra(env, GET_OFFSET(arg2, -3), (uint8_t)(arg1 >> 24),
                          mem_idx, GETPC());
    }
    invalidate_tags_store_left_right(env, arg2, GETPC());
}

#if defined(TARGET_MIPS64)
/*
 * "half" load and stores.  We must do the memory access inline,
 * or fault handling won't work.
 */
#ifdef TARGET_WORDS_BIGENDIAN
#define GET_LMASK64(v) ((v) & 7)
#else
#define GET_LMASK64(v) (((v) & 7) ^ 7)
#endif

void helper_sdl(CPUMIPSState *env, target_ulong arg1, target_ulong arg2,
                int mem_idx)
{
#ifdef TARGET_CHERI
    const int num_bytes = 4 - GET_LMASK(arg2);
    arg2 = check_ddc(env, CAP_PERM_STORE, arg2, num_bytes, GETPC());
#endif
    cpu_stb_mmuidx_ra(env, arg2, (uint8_t)(arg1 >> 56), mem_idx, GETPC());

    if (GET_LMASK64(arg2) <= 6) {
        cpu_stb_mmuidx_ra(env, GET_OFFSET(arg2, 1), (uint8_t)(arg1 >> 48),
                          mem_idx, GETPC());
    }

    if (GET_LMASK64(arg2) <= 5) {
        cpu_stb_mmuidx_ra(env, GET_OFFSET(arg2, 2), (uint8_t)(arg1 >> 40),
                          mem_idx, GETPC());
    }

    if (GET_LMASK64(arg2) <= 4) {
        cpu_stb_mmuidx_ra(env, GET_OFFSET(arg2, 3), (uint8_t)(arg1 >> 32),
                          mem_idx, GETPC());
    }

    if (GET_LMASK64(arg2) <= 3) {
        cpu_stb_mmuidx_ra(env, GET_OFFSET(arg2, 4), (uint8_t)(arg1 >> 24),
                          mem_idx, GETPC());
    }

    if (GET_LMASK64(arg2) <= 2) {
        cpu_stb_mmuidx_ra(env, GET_OFFSET(arg2, 5), (uint8_t)(arg1 >> 16),
                          mem_idx, GETPC());
    }

    if (GET_LMASK64(arg2) <= 1) {
        cpu_stb_mmuidx_ra(env, GET_OFFSET(arg2, 6), (uint8_t)(arg1 >> 8),
                          mem_idx, GETPC());
    }

    if (GET_LMASK64(arg2) <= 0) {
        cpu_stb_mmuidx_ra(env, GET_OFFSET(arg2, 7), (uint8_t)arg1,
                          mem_idx, GETPC());
    }
    invalidate_tags_store_left_right(env, arg2, GETPC());
}

void helper_sdr(CPUMIPSState *env, target_ulong arg1, target_ulong arg2,
                int mem_idx)
{
#ifdef TARGET_CHERI
    arg2 = ccheck_store_right(env, arg2, 8, GETPC());
#endif
    cpu_stb_mmuidx_ra(env, arg2, (uint8_t)arg1, mem_idx, GETPC());

    if (GET_LMASK64(arg2) >= 1) {
        cpu_stb_mmuidx_ra(env, GET_OFFSET(arg2, -1), (uint8_t)(arg1 >> 8),
                          mem_idx, GETPC());
    }

    if (GET_LMASK64(arg2) >= 2) {
        cpu_stb_mmuidx_ra(env, GET_OFFSET(arg2, -2), (uint8_t)(arg1 >> 16),
                          mem_idx, GETPC());
    }

    if (GET_LMASK64(arg2) >= 3) {
        cpu_stb_mmuidx_ra(env, GET_OFFSET(arg2, -3), (uint8_t)(arg1 >> 24),
                          mem_idx, GETPC());
    }

    if (GET_LMASK64(arg2) >= 4) {
        cpu_stb_mmuidx_ra(env, GET_OFFSET(arg2, -4), (uint8_t)(arg1 >> 32),
                          mem_idx, GETPC());
    }

    if (GET_LMASK64(arg2) >= 5) {
        cpu_stb_mmuidx_ra(env, GET_OFFSET(arg2, -5), (uint8_t)(arg1 >> 40),
                          mem_idx, GETPC());
    }

    if (GET_LMASK64(arg2) >= 6) {
        cpu_stb_mmuidx_ra(env, GET_OFFSET(arg2, -6), (uint8_t)(arg1 >> 48),
                          mem_idx, GETPC());
    }

    if (GET_LMASK64(arg2) == 7) {
        cpu_stb_mmuidx_ra(env, GET_OFFSET(arg2, -7), (uint8_t)(arg1 >> 56),
                          mem_idx, GETPC());
    }
    invalidate_tags_store_left_right(env, arg2, GETPC());
}
#endif /* TARGET_MIPS64 */

static const int multiple_regs[] = { 16, 17, 18, 19, 20, 21, 22, 23, 30 };

void helper_lwm(CPUMIPSState *env, target_ulong addr, target_ulong reglist,
                uint32_t mem_idx)
{
    target_ulong base_reglist = reglist & 0xf;
    target_ulong do_r31 = reglist & 0x10;

    if (base_reglist > 0 && base_reglist <= ARRAY_SIZE(multiple_regs)) {
        target_ulong i;

        for (i = 0; i < base_reglist; i++) {
            env->active_tc.gpr[multiple_regs[i]] =
                (target_long)cpu_ldl_mmuidx_ra(env, addr, mem_idx, GETPC());
            addr += 4;
        }
    }

    if (do_r31) {
        env->active_tc.gpr[31] =
            (target_long)cpu_ldl_mmuidx_ra(env, addr, mem_idx, GETPC());
    }
}

void helper_swm(CPUMIPSState *env, target_ulong addr, target_ulong reglist,
                uint32_t mem_idx)
{
    target_ulong base_reglist = reglist & 0xf;
    target_ulong do_r31 = reglist & 0x10;

    if (base_reglist > 0 && base_reglist <= ARRAY_SIZE(multiple_regs)) {
        target_ulong i;

        for (i = 0; i < base_reglist; i++) {
            cpu_stw_mmuidx_ra(env, addr, env->active_tc.gpr[multiple_regs[i]],
                              mem_idx, GETPC());
            addr += 4;
        }
    }

    if (do_r31) {
        cpu_stw_mmuidx_ra(env, addr, env->active_tc.gpr[31], mem_idx, GETPC());
    }
}

#if defined(TARGET_MIPS64)
void helper_ldm(CPUMIPSState *env, target_ulong addr, target_ulong reglist,
                uint32_t mem_idx)
{
    target_ulong base_reglist = reglist & 0xf;
    target_ulong do_r31 = reglist & 0x10;

    if (base_reglist > 0 && base_reglist <= ARRAY_SIZE(multiple_regs)) {
        target_ulong i;

        for (i = 0; i < base_reglist; i++) {
            env->active_tc.gpr[multiple_regs[i]] =
                cpu_ldq_mmuidx_ra(env, addr, mem_idx, GETPC());
            addr += 8;
        }
    }

    if (do_r31) {
        env->active_tc.gpr[31] =
            cpu_ldq_mmuidx_ra(env, addr, mem_idx, GETPC());
    }
}

void helper_sdm(CPUMIPSState *env, target_ulong addr, target_ulong reglist,
                uint32_t mem_idx)
{
    target_ulong base_reglist = reglist & 0xf;
    target_ulong do_r31 = reglist & 0x10;

    if (base_reglist > 0 && base_reglist <= ARRAY_SIZE(multiple_regs)) {
        target_ulong i;

        for (i = 0; i < base_reglist; i++) {
            cpu_stq_mmuidx_ra(env, addr, env->active_tc.gpr[multiple_regs[i]],
                              mem_idx, GETPC());
            addr += 8;
        }
    }

    if (do_r31) {
        cpu_stq_mmuidx_ra(env, addr, env->active_tc.gpr[31], mem_idx, GETPC());
    }
}
#endif


void helper_fork(target_ulong arg1, target_ulong arg2)
{
    /*
     * arg1 = rt, arg2 = rs
     * TODO: store to TC register
     */
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
        if (0) {
            /* TODO: TC underflow */
            env->CP0_VPEControl &= ~(0x7 << CP0VPECo_EXCPT);
            do_raise_exception(env, EXCP_THREAD, GETPC());
        } else {
            /* TODO: Deallocate TC */
        }
    } else if (arg1 > 0) {
        /* Yield qualifier inputs not implemented. */
        env->CP0_VPEControl &= ~(0x7 << CP0VPECo_EXCPT);
        env->CP0_VPEControl |= 2 << CP0VPECo_EXCPT;
        do_raise_exception(env, EXCP_THREAD, GETPC());
    }
    return env->CP0_YQMask;
}

#ifndef CONFIG_USER_ONLY
/* TLB management */
static void r4k_mips_tlb_flush_extra(CPUMIPSState *env, int first)
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
    tlb->MMID = env->CP0_MemoryMapID;
    tlb->PageMask = env->CP0_PageMask;
    tlb->G = env->CP0_EntryLo0 & env->CP0_EntryLo1 & 1;
    tlb->V0 = (env->CP0_EntryLo0 & 2) != 0;
    tlb->D0 = (env->CP0_EntryLo0 & 4) != 0;
    tlb->C0 = (env->CP0_EntryLo0 >> 3) & 0x7;
#if defined(TARGET_CHERI)
    tlb->L0 = (env->CP0_EntryLo0 >> CP0EnLo_L) & 1;
    tlb->S0 = (env->CP0_EntryLo0 >> CP0EnLo_S) & 1;
    tlb->CLG0 = (env->CP0_EntryLo0 >> CP0EnLo_CLG) & 1;
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
    tlb->CLG1 = (env->CP0_EntryLo1 >> CP0EnLo_CLG) & 1;
#else
    tlb->XI1 = (env->CP0_EntryLo1 >> CP0EnLo_XI) & 1;
    tlb->RI1 = (env->CP0_EntryLo1 >> CP0EnLo_RI) & 1;
#endif /* TARGET_CHERI */
    tlb->PFN[1] = (get_tlb_pfn_from_entrylo(env->CP0_EntryLo1) & ~mask) << 12;
}

void r4k_helper_tlbinv(CPUMIPSState *env)
{
    bool mi = !!((env->CP0_Config5 >> CP0C5_MI) & 1);
    uint16_t ASID = env->CP0_EntryHi & env->CP0_EntryHi_ASID_mask;
    uint32_t MMID = env->CP0_MemoryMapID;
    uint32_t tlb_mmid;
    r4k_tlb_t *tlb;
    int idx;

    MMID = mi ? MMID : (uint32_t) ASID;
    for (idx = 0; idx < env->tlb->nb_tlb; idx++) {
        tlb = &env->tlb->mmu.r4k.tlb[idx];
        tlb_mmid = mi ? tlb->MMID : (uint32_t) tlb->ASID;
        if (!tlb->G && tlb_mmid == MMID) {
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
    bool mi = !!((env->CP0_Config5 >> CP0C5_MI) & 1);
    target_ulong VPN;
    uint16_t ASID = env->CP0_EntryHi & env->CP0_EntryHi_ASID_mask;
    uint32_t MMID = env->CP0_MemoryMapID;
    uint32_t tlb_mmid;
    bool EHINV, G, V0, D0, V1, D1;
#if defined(TARGET_CHERI)
    bool S0, S1, L0, L1, CLG0, CLG1;
#else
    bool XI0, XI1, RI0, RI1;
#endif
    r4k_tlb_t *tlb;
    int idx;

    MMID = mi ? MMID : (uint32_t) ASID;

    idx = (env->CP0_Index & ~0x80000000) % env->tlb->nb_tlb;
    tlb = &env->tlb->mmu.r4k.tlb[idx];
    VPN = env->CP0_EntryHi & (TARGET_PAGE_MASK << 1);
#if defined(TARGET_MIPS64)
    VPN &= env->SEGMask;
#endif
    EHINV = (env->CP0_EntryHi & (1 << CP0EnHi_EHINV)) != 0;
    G = env->CP0_EntryLo0 & env->CP0_EntryLo1 & 1;
    V0 = (env->CP0_EntryLo0 & 2) != 0;
    D0 = (env->CP0_EntryLo0 & 4) != 0;
#if defined(TARGET_CHERI)
    S0 = (env->CP0_EntryLo0 >> CP0EnLo_S) &1;
    L0 = (env->CP0_EntryLo0 >> CP0EnLo_L) &1;
    CLG0 = (env->CP0_EntryLo0 >> CP0EnLo_CLG) &1;
#else
    XI0 = (env->CP0_EntryLo0 >> CP0EnLo_XI) &1;
    RI0 = (env->CP0_EntryLo0 >> CP0EnLo_RI) &1;
#endif
    V1 = (env->CP0_EntryLo1 & 2) != 0;
    D1 = (env->CP0_EntryLo1 & 4) != 0;
#if defined(TARGET_CHERI)
    S1 = (env->CP0_EntryLo1 >> CP0EnLo_S) &1;
    L1 = (env->CP0_EntryLo1 >> CP0EnLo_L) &1;
    CLG1 = (env->CP0_EntryLo1 >> CP0EnLo_CLG) &1;
#else
    XI1 = (env->CP0_EntryLo1 >> CP0EnLo_XI) &1;
    RI1 = (env->CP0_EntryLo1 >> CP0EnLo_RI) &1;
#endif

    tlb_mmid = mi ? tlb->MMID : (uint32_t) tlb->ASID;
    /*
     * Discard cached TLB entries, unless tlbwi is just upgrading access
     * permissions on the current entry.
     */
    if (tlb->VPN != VPN || tlb_mmid != MMID || tlb->G != G ||
        (!tlb->EHINV && EHINV) ||
        (tlb->V0 && !V0) || (tlb->D0 && !D0) ||
#if defined(TARGET_CHERI)
        (!tlb->S0 && S0) || (!tlb->L0 && L0) || (tlb->CLG0 != CLG0) ||
#else
        (!tlb->XI0 && XI0) || (!tlb->RI0 && RI0) ||
#endif
        (tlb->V1 && !V1) || (tlb->D1 && !D1) ||
#if defined(TARGET_CHERI)
        (!tlb->S1 && S1) || (!tlb->L1 && L1) || (tlb->CLG1 != CLG1)) {
#else
        (!tlb->XI1 && XI1) || (!tlb->RI1 && RI1)) {
#endif
        r4k_mips_tlb_flush_extra(env, env->tlb->nb_tlb);
    }

    r4k_invalidate_tlb(env, idx, 0);
    r4k_fill_tlb(env, idx);
#ifdef CONFIG_MIPS_LOG_INSTR
    if (qemu_loglevel_mask(CPU_LOG_INSTR))
        r4k_dump_tlb(env, idx);
#endif /* TARGET_CHERI */
}

void r4k_helper_tlbwr(CPUMIPSState *env)
{
    int r = cpu_mips_get_random(env);

    r4k_invalidate_tlb(env, r, 1);
    r4k_fill_tlb(env, r);
#ifdef CONFIG_MIPS_LOG_INSTR
    if (qemu_loglevel_mask(CPU_LOG_INSTR))
        r4k_dump_tlb(env, r);
#endif /* TARGET_CHERI */
}

void r4k_helper_tlbp(CPUMIPSState *env)
{
    bool mi = !!((env->CP0_Config5 >> CP0C5_MI) & 1);
    r4k_tlb_t *tlb;
    target_ulong mask;
    target_ulong tag;
    target_ulong VPN;
    uint16_t ASID = env->CP0_EntryHi & env->CP0_EntryHi_ASID_mask;
    uint32_t MMID = env->CP0_MemoryMapID;
    uint32_t tlb_mmid;
    int i;

    MMID = mi ? MMID : (uint32_t) ASID;
    for (i = 0; i < env->tlb->nb_tlb; i++) {
        tlb = &env->tlb->mmu.r4k.tlb[i];
        /* 1k pages are not supported. */
        mask = tlb->PageMask | ~(TARGET_PAGE_MASK << 1);
        tag = env->CP0_EntryHi & ~mask;
        VPN = tlb->VPN & ~mask;
#if defined(TARGET_MIPS64)
        tag &= env->SEGMask;
#endif
        tlb_mmid = mi ? tlb->MMID : (uint32_t) tlb->ASID;
        /* Check ASID/MMID, virtual page number & size */
        if ((tlb->G == 1 || tlb_mmid == MMID) && VPN == tag && !tlb->EHINV) {
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
            tlb_mmid = mi ? tlb->MMID : (uint32_t) tlb->ASID;
            /* Check ASID/MMID, virtual page number & size */
            if ((tlb->G == 1 || tlb_mmid == MMID) && VPN == tag) {
                r4k_mips_tlb_flush_extra(env, i);
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
    bool mi = !!((env->CP0_Config5 >> CP0C5_MI) & 1);
    uint16_t ASID = env->CP0_EntryHi & env->CP0_EntryHi_ASID_mask;
    uint32_t MMID = env->CP0_MemoryMapID;
    uint32_t tlb_mmid;
    r4k_tlb_t *tlb;
    int idx;

    MMID = mi ? MMID : (uint32_t) ASID;
    idx = (env->CP0_Index & ~0x80000000) % env->tlb->nb_tlb;
    tlb = &env->tlb->mmu.r4k.tlb[idx];

    tlb_mmid = mi ? tlb->MMID : (uint32_t) tlb->ASID;
    /* If this will change the current ASID/MMID, flush qemu's TLB.  */
    if (MMID != tlb_mmid) {
        cpu_mips_tlb_flush(env);
    }

    r4k_mips_tlb_flush_extra(env, env->tlb->nb_tlb);

#ifdef CPU_CHERI
	uint64_t save_clg = env->CP0_EntryHi &
		((1 << CP0EnHi_CLGU) | (1 << CP0EnHi_CLGS) | (1 << CP0EnHi_CLGK));
#endif

    if (tlb->EHINV) {
        env->CP0_EntryHi = 1 << CP0EnHi_EHINV;
        env->CP0_PageMask = 0;
        env->CP0_EntryLo0 = 0;
        env->CP0_EntryLo1 = 0;
    } else {
        env->CP0_EntryHi = mi ? tlb->VPN : tlb->VPN | tlb->ASID;
        env->CP0_MemoryMapID = tlb->MMID;
        env->CP0_PageMask = tlb->PageMask;
        env->CP0_EntryLo0 = tlb->G | (tlb->V0 << 1) | (tlb->D0 << 2) |
#if defined(TARGET_CHERI)
                        ((uint64_t)tlb->L0 << CP0EnLo_L) |
                        ((uint64_t)tlb->S0 << CP0EnLo_S) |
                        ((uint64_t)tlb->CLG0 << CP0EnLo_CLG) |
#else
                        ((uint64_t)tlb->RI0 << CP0EnLo_RI) |
                        ((uint64_t)tlb->XI0 << CP0EnLo_XI) |
#endif /* TARGET_CHERI */
                        (tlb->C0 << 3) |
                        get_entrylo_pfn_from_tlb(tlb->PFN[0] >> 12);
        env->CP0_EntryLo1 = tlb->G | (tlb->V1 << 1) | (tlb->D1 << 2) |
#if defined(TARGET_CHERI)
                        ((uint64_t)tlb->L1 << CP0EnLo_L) |
                        ((uint64_t)tlb->S1 << CP0EnLo_S) |
                        ((uint64_t)tlb->CLG1 << CP0EnLo_CLG) |
#else
                        ((uint64_t)tlb->RI1 << CP0EnLo_RI) |
                        ((uint64_t)tlb->XI1 << CP0EnLo_XI) |
#endif /* TARGET_CHERI */
                        (tlb->C1 << 3) |
                        get_entrylo_pfn_from_tlb(tlb->PFN[1] >> 12);
    }

#ifdef CPU_CHERI
	env->CP0_EntryHi |= save_clg;
#endif
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

static void global_invalidate_tlb(CPUMIPSState *env,
                           uint32_t invMsgVPN2,
                           uint8_t invMsgR,
                           uint32_t invMsgMMid,
                           bool invAll,
                           bool invVAMMid,
                           bool invMMid,
                           bool invVA)
{

    int idx;
    r4k_tlb_t *tlb;
    bool VAMatch;
    bool MMidMatch;

    for (idx = 0; idx < env->tlb->nb_tlb; idx++) {
        tlb = &env->tlb->mmu.r4k.tlb[idx];
        VAMatch =
            (((tlb->VPN & ~tlb->PageMask) == (invMsgVPN2 & ~tlb->PageMask))
#ifdef TARGET_MIPS64
            &&
            (extract64(env->CP0_EntryHi, 62, 2) == invMsgR)
#endif
            );
        MMidMatch = tlb->MMID == invMsgMMid;
        if ((invAll && (idx > env->CP0_Wired)) ||
            (VAMatch && invVAMMid && (tlb->G || MMidMatch)) ||
            (VAMatch && invVA) ||
            (MMidMatch && !(tlb->G) && invMMid)) {
            tlb->EHINV = 1;
        }
    }
    cpu_mips_tlb_flush(env);
}

void helper_ginvt(CPUMIPSState *env, target_ulong arg, uint32_t type)
{
    bool invAll = type == 0;
    bool invVA = type == 1;
    bool invMMid = type == 2;
    bool invVAMMid = type == 3;
    uint32_t invMsgVPN2 = arg & (TARGET_PAGE_MASK << 1);
    uint8_t invMsgR = 0;
    uint32_t invMsgMMid = env->CP0_MemoryMapID;
    CPUState *other_cs = first_cpu;

#ifdef TARGET_MIPS64
    invMsgR = extract64(arg, 62, 2);
#endif

    CPU_FOREACH(other_cs) {
        MIPSCPU *other_cpu = MIPS_CPU(other_cs);
        global_invalidate_tlb(&other_cpu->env, invMsgVPN2, invMsgR, invMsgMMid,
                              invAll, invVAMMid, invMMid, invVA);
    }
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
    if (qemu_loglevel_mask(CPU_LOG_EXEC | CPU_LOG_INSTR)) {
        qemu_log("ERET: PC " TARGET_FMT_lx " EPC " TARGET_FMT_lx,
                 PC_ADDR(env), get_CP0_EPC(env));
        if (should_use_error_epc(env)) {
          qemu_log(" ErrorEPC " TARGET_FMT_lx, get_CP0_ErrorEPC(env));
        }
        if (env->hflags & MIPS_HFLAG_DM) {
            qemu_log(" DEPC " TARGET_FMT_lx, env->CP0_DEPC);
        }
        qemu_log("\n");
    }
}

static void debug_post_eret(CPUMIPSState *env)
{
    if (qemu_loglevel_mask(CPU_LOG_EXEC)) {
        qemu_log("  =>  PC " TARGET_FMT_lx " EPC " TARGET_FMT_lx,
                 PC_ADDR(env), get_CP0_EPC(env));
        if (should_use_error_epc(env)) {
          qemu_log(" ErrorEPC " TARGET_FMT_lx, get_CP0_ErrorEPC(env));
        }
        if (env->hflags & MIPS_HFLAG_DM) {
            qemu_log(" DEPC " TARGET_FMT_lx, env->CP0_DEPC);
        }
        switch (cpu_mmu_index(env, false)) {
        case 3:
            qemu_log(", ERL\n");
            break;
        case MIPS_HFLAG_UM:
            qemu_log(", UM\n");
            break;
        case MIPS_HFLAG_SM:
            qemu_log(", SM\n");
            break;
        case MIPS_HFLAG_KM:
            qemu_log("\n");
            break;
        default:
            cpu_abort(env_cpu(env), "Invalid MMU mode!\n");
            break;
        }
    }
}

#ifdef TARGET_CHERI
static void set_pc(CPUMIPSState *env, cap_register_t* error_pcc)
#else
static void set_pc(CPUMIPSState *env, target_ulong error_pc)
#endif
{
#ifdef TARGET_CHERI
    env->active_tc.PCC = *error_pcc;
    // Sentry capabilities in EPCC should be unsealed on ERET
    if (cap_is_sealed_entry(&env->active_tc.PCC)) {
        cap_unseal_entry(&env->active_tc.PCC);
    }
    target_ulong error_pc = cap_get_cursor(error_pcc);
#endif
    mips_update_pc(env, error_pc & ~(target_ulong)1);
    if (error_pc & 1) {
#if defined(TARGET_CHERI)
        warn_report("Got target pc with low bit set, but QEMU-CHERI does not"
                    " support microMIPS: 0x%" PRIx64, error_pc);
#else
        env->hflags |= MIPS_HFLAG_M16;
#endif
    } else {
        env->hflags &= ~(MIPS_HFLAG_M16);
    }
}

static inline void exception_return(CPUMIPSState *env)
{
    debug_pre_eret(env);
#ifdef TARGET_CHERI
    // qemu_log_mask(CPU_LOG_INSTR, "%s: PCC <- EPCC\n", __func__);
#ifdef CONFIG_MIPS_LOG_INSTR
    if (unlikely(qemu_loglevel_mask(CPU_LOG_INSTR))) {
         // Print the new PCC value for debugging traces (compare to null
         // so that we always print it)
         cap_register_t null_cap;
         null_capability(&null_cap);
         dump_changed_capreg(env, &env->active_tc.PCC, &null_cap, "PCC");
         null_capability(&null_cap);
         dump_changed_capreg(env, &env->active_tc.CHWR.EPCC, &null_cap, "EPCC");
         null_capability(&null_cap);
         dump_changed_capreg(env, &env->active_tc.CHWR.ErrorEPCC, &null_cap, "ErrorEPCC");
    }
#endif // CONFIG_MIPS_LOG_INSTR
#endif /* TARGET_CHERI */
    if (env->CP0_Status & (1 << CP0St_ERL)) {
#ifdef TARGET_CHERI
        set_pc(env, &env->active_tc.CHWR.ErrorEPCC);
#else
        set_pc(env, env->CP0_ErrorEPC);
#endif
        env->CP0_Status &= ~(1 << CP0St_ERL);
    } else {
#ifdef TARGET_CHERI
        set_pc(env, &env->active_tc.CHWR.EPCC);
#else
        set_pc(env, env->CP0_EPC);
#endif
        env->CP0_Status &= ~(1 << CP0St_EXL);
    }
    compute_hflags(env);
    debug_post_eret(env);
}

void helper_eret(CPUMIPSState *env)
{
    exception_return(env);
    env->CP0_LLAddr = 1;
    env->lladdr = 1;
#ifdef TARGET_CHERI
    env->linkedflag = 0;
#endif /* TARGET_CHERI */
}

void helper_eretnc(CPUMIPSState *env)
{
#ifdef TARGET_CHERI
    do_raise_exception(env, EXCP_RI, GETPC()); /* This does not unset linkedflag? */
#endif
    exception_return(env);
}

void helper_deret(CPUMIPSState *env)
{
#ifdef TARGET_CHERI
    do_raise_exception(env, EXCP_RI, GETPC()); /* This ignores EPCC */
#else
    debug_pre_eret(env);

    env->hflags &= ~MIPS_HFLAG_DM;
    compute_hflags(env);

    set_pc(env, env->CP0_DEPC);

    debug_post_eret(env);
#endif
}
#endif /* !CONFIG_USER_ONLY */

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
    check_hwrena(env, 2, GETPC());
#ifdef CONFIG_USER_ONLY
    return env->CP0_Count;
#else
    return (int32_t)cpu_mips_get_count(env);
#endif
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

void helper_pmon(CPUMIPSState *env, int function)
{
    function /= 2;
    switch (function) {
    case 2: /* TODO: char inbyte(int waitflag); */
        if (env->active_tc.gpr[4] == 0) {
            env->active_tc.gpr[2] = -1;
        }
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
    CPUState *cs = env_cpu(env);

    cs->halted = 1;
    cpu_reset_interrupt(cs, CPU_INTERRUPT_WAKE);
    /*
     * Last instruction in the block, PC was updated before
     * - no need to recover PC and icount.
     */
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

    if (!(env->hflags & MIPS_HFLAG_DM)) {
        env->CP0_BadVAddr = addr;
    }

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

void mips_cpu_do_transaction_failed(CPUState *cs, hwaddr physaddr,
                                    vaddr addr, unsigned size,
                                    MMUAccessType access_type,
                                    int mmu_idx, MemTxAttrs attrs,
                                    MemTxResult response, uintptr_t retaddr)
{
    MIPSCPU *cpu = MIPS_CPU(cs);
    CPUMIPSState *env = &cpu->env;

    if (access_type == MMU_INST_FETCH) {
        do_raise_exception(env, EXCP_IBE, retaddr);
    } else {
        do_raise_exception(env, EXCP_DBE, retaddr);
    }
}
#endif /* !CONFIG_USER_ONLY */

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

void helper_msa_ld_b(CPUMIPSState *env, uint32_t wd,
                     target_ulong addr)
{
    wr_t *pwd = &(env->active_fpu.fpr[wd].wr);
    MEMOP_IDX(DF_BYTE)
#if !defined(CONFIG_USER_ONLY)
#if !defined(HOST_WORDS_BIGENDIAN)
    pwd->b[0]  = helper_ret_ldub_mmu(env, addr + (0  << DF_BYTE), oi, GETPC());
    pwd->b[1]  = helper_ret_ldub_mmu(env, addr + (1  << DF_BYTE), oi, GETPC());
    pwd->b[2]  = helper_ret_ldub_mmu(env, addr + (2  << DF_BYTE), oi, GETPC());
    pwd->b[3]  = helper_ret_ldub_mmu(env, addr + (3  << DF_BYTE), oi, GETPC());
    pwd->b[4]  = helper_ret_ldub_mmu(env, addr + (4  << DF_BYTE), oi, GETPC());
    pwd->b[5]  = helper_ret_ldub_mmu(env, addr + (5  << DF_BYTE), oi, GETPC());
    pwd->b[6]  = helper_ret_ldub_mmu(env, addr + (6  << DF_BYTE), oi, GETPC());
    pwd->b[7]  = helper_ret_ldub_mmu(env, addr + (7  << DF_BYTE), oi, GETPC());
    pwd->b[8]  = helper_ret_ldub_mmu(env, addr + (8  << DF_BYTE), oi, GETPC());
    pwd->b[9]  = helper_ret_ldub_mmu(env, addr + (9  << DF_BYTE), oi, GETPC());
    pwd->b[10] = helper_ret_ldub_mmu(env, addr + (10 << DF_BYTE), oi, GETPC());
    pwd->b[11] = helper_ret_ldub_mmu(env, addr + (11 << DF_BYTE), oi, GETPC());
    pwd->b[12] = helper_ret_ldub_mmu(env, addr + (12 << DF_BYTE), oi, GETPC());
    pwd->b[13] = helper_ret_ldub_mmu(env, addr + (13 << DF_BYTE), oi, GETPC());
    pwd->b[14] = helper_ret_ldub_mmu(env, addr + (14 << DF_BYTE), oi, GETPC());
    pwd->b[15] = helper_ret_ldub_mmu(env, addr + (15 << DF_BYTE), oi, GETPC());
#else
    pwd->b[0]  = helper_ret_ldub_mmu(env, addr + (7  << DF_BYTE), oi, GETPC());
    pwd->b[1]  = helper_ret_ldub_mmu(env, addr + (6  << DF_BYTE), oi, GETPC());
    pwd->b[2]  = helper_ret_ldub_mmu(env, addr + (5  << DF_BYTE), oi, GETPC());
    pwd->b[3]  = helper_ret_ldub_mmu(env, addr + (4  << DF_BYTE), oi, GETPC());
    pwd->b[4]  = helper_ret_ldub_mmu(env, addr + (3  << DF_BYTE), oi, GETPC());
    pwd->b[5]  = helper_ret_ldub_mmu(env, addr + (2  << DF_BYTE), oi, GETPC());
    pwd->b[6]  = helper_ret_ldub_mmu(env, addr + (1  << DF_BYTE), oi, GETPC());
    pwd->b[7]  = helper_ret_ldub_mmu(env, addr + (0  << DF_BYTE), oi, GETPC());
    pwd->b[8]  = helper_ret_ldub_mmu(env, addr + (15 << DF_BYTE), oi, GETPC());
    pwd->b[9]  = helper_ret_ldub_mmu(env, addr + (14 << DF_BYTE), oi, GETPC());
    pwd->b[10] = helper_ret_ldub_mmu(env, addr + (13 << DF_BYTE), oi, GETPC());
    pwd->b[11] = helper_ret_ldub_mmu(env, addr + (12 << DF_BYTE), oi, GETPC());
    pwd->b[12] = helper_ret_ldub_mmu(env, addr + (11 << DF_BYTE), oi, GETPC());
    pwd->b[13] = helper_ret_ldub_mmu(env, addr + (10 << DF_BYTE), oi, GETPC());
    pwd->b[14] = helper_ret_ldub_mmu(env, addr + (9  << DF_BYTE), oi, GETPC());
    pwd->b[15] = helper_ret_ldub_mmu(env, addr + (8  << DF_BYTE), oi, GETPC());
#endif
#else
#if !defined(HOST_WORDS_BIGENDIAN)
    pwd->b[0]  = cpu_ldub_data(env, addr + (0  << DF_BYTE));
    pwd->b[1]  = cpu_ldub_data(env, addr + (1  << DF_BYTE));
    pwd->b[2]  = cpu_ldub_data(env, addr + (2  << DF_BYTE));
    pwd->b[3]  = cpu_ldub_data(env, addr + (3  << DF_BYTE));
    pwd->b[4]  = cpu_ldub_data(env, addr + (4  << DF_BYTE));
    pwd->b[5]  = cpu_ldub_data(env, addr + (5  << DF_BYTE));
    pwd->b[6]  = cpu_ldub_data(env, addr + (6  << DF_BYTE));
    pwd->b[7]  = cpu_ldub_data(env, addr + (7  << DF_BYTE));
    pwd->b[8]  = cpu_ldub_data(env, addr + (8  << DF_BYTE));
    pwd->b[9]  = cpu_ldub_data(env, addr + (9  << DF_BYTE));
    pwd->b[10] = cpu_ldub_data(env, addr + (10 << DF_BYTE));
    pwd->b[11] = cpu_ldub_data(env, addr + (11 << DF_BYTE));
    pwd->b[12] = cpu_ldub_data(env, addr + (12 << DF_BYTE));
    pwd->b[13] = cpu_ldub_data(env, addr + (13 << DF_BYTE));
    pwd->b[14] = cpu_ldub_data(env, addr + (14 << DF_BYTE));
    pwd->b[15] = cpu_ldub_data(env, addr + (15 << DF_BYTE));
#else
    pwd->b[0]  = cpu_ldub_data(env, addr + (7  << DF_BYTE));
    pwd->b[1]  = cpu_ldub_data(env, addr + (6  << DF_BYTE));
    pwd->b[2]  = cpu_ldub_data(env, addr + (5  << DF_BYTE));
    pwd->b[3]  = cpu_ldub_data(env, addr + (4  << DF_BYTE));
    pwd->b[4]  = cpu_ldub_data(env, addr + (3  << DF_BYTE));
    pwd->b[5]  = cpu_ldub_data(env, addr + (2  << DF_BYTE));
    pwd->b[6]  = cpu_ldub_data(env, addr + (1  << DF_BYTE));
    pwd->b[7]  = cpu_ldub_data(env, addr + (0  << DF_BYTE));
    pwd->b[8]  = cpu_ldub_data(env, addr + (15 << DF_BYTE));
    pwd->b[9]  = cpu_ldub_data(env, addr + (14 << DF_BYTE));
    pwd->b[10] = cpu_ldub_data(env, addr + (13 << DF_BYTE));
    pwd->b[11] = cpu_ldub_data(env, addr + (12 << DF_BYTE));
    pwd->b[12] = cpu_ldub_data(env, addr + (11 << DF_BYTE));
    pwd->b[13] = cpu_ldub_data(env, addr + (10 << DF_BYTE));
    pwd->b[14] = cpu_ldub_data(env, addr + (9 << DF_BYTE));
    pwd->b[15] = cpu_ldub_data(env, addr + (8 << DF_BYTE));
#endif
#endif
}

void helper_msa_ld_h(CPUMIPSState *env, uint32_t wd,
                     target_ulong addr)
{
    wr_t *pwd = &(env->active_fpu.fpr[wd].wr);
    MEMOP_IDX(DF_HALF)
#if !defined(CONFIG_USER_ONLY)
#if !defined(HOST_WORDS_BIGENDIAN)
    pwd->h[0] = helper_ret_lduw_mmu(env, addr + (0 << DF_HALF), oi, GETPC());
    pwd->h[1] = helper_ret_lduw_mmu(env, addr + (1 << DF_HALF), oi, GETPC());
    pwd->h[2] = helper_ret_lduw_mmu(env, addr + (2 << DF_HALF), oi, GETPC());
    pwd->h[3] = helper_ret_lduw_mmu(env, addr + (3 << DF_HALF), oi, GETPC());
    pwd->h[4] = helper_ret_lduw_mmu(env, addr + (4 << DF_HALF), oi, GETPC());
    pwd->h[5] = helper_ret_lduw_mmu(env, addr + (5 << DF_HALF), oi, GETPC());
    pwd->h[6] = helper_ret_lduw_mmu(env, addr + (6 << DF_HALF), oi, GETPC());
    pwd->h[7] = helper_ret_lduw_mmu(env, addr + (7 << DF_HALF), oi, GETPC());
#else
    pwd->h[0] = helper_ret_lduw_mmu(env, addr + (3 << DF_HALF), oi, GETPC());
    pwd->h[1] = helper_ret_lduw_mmu(env, addr + (2 << DF_HALF), oi, GETPC());
    pwd->h[2] = helper_ret_lduw_mmu(env, addr + (1 << DF_HALF), oi, GETPC());
    pwd->h[3] = helper_ret_lduw_mmu(env, addr + (0 << DF_HALF), oi, GETPC());
    pwd->h[4] = helper_ret_lduw_mmu(env, addr + (7 << DF_HALF), oi, GETPC());
    pwd->h[5] = helper_ret_lduw_mmu(env, addr + (6 << DF_HALF), oi, GETPC());
    pwd->h[6] = helper_ret_lduw_mmu(env, addr + (5 << DF_HALF), oi, GETPC());
    pwd->h[7] = helper_ret_lduw_mmu(env, addr + (4 << DF_HALF), oi, GETPC());
#endif
#else
#if !defined(HOST_WORDS_BIGENDIAN)
    pwd->h[0] = cpu_lduw_data(env, addr + (0 << DF_HALF));
    pwd->h[1] = cpu_lduw_data(env, addr + (1 << DF_HALF));
    pwd->h[2] = cpu_lduw_data(env, addr + (2 << DF_HALF));
    pwd->h[3] = cpu_lduw_data(env, addr + (3 << DF_HALF));
    pwd->h[4] = cpu_lduw_data(env, addr + (4 << DF_HALF));
    pwd->h[5] = cpu_lduw_data(env, addr + (5 << DF_HALF));
    pwd->h[6] = cpu_lduw_data(env, addr + (6 << DF_HALF));
    pwd->h[7] = cpu_lduw_data(env, addr + (7 << DF_HALF));
#else
    pwd->h[0] = cpu_lduw_data(env, addr + (3 << DF_HALF));
    pwd->h[1] = cpu_lduw_data(env, addr + (2 << DF_HALF));
    pwd->h[2] = cpu_lduw_data(env, addr + (1 << DF_HALF));
    pwd->h[3] = cpu_lduw_data(env, addr + (0 << DF_HALF));
    pwd->h[4] = cpu_lduw_data(env, addr + (7 << DF_HALF));
    pwd->h[5] = cpu_lduw_data(env, addr + (6 << DF_HALF));
    pwd->h[6] = cpu_lduw_data(env, addr + (5 << DF_HALF));
    pwd->h[7] = cpu_lduw_data(env, addr + (4 << DF_HALF));
#endif
#endif
}

void helper_msa_ld_w(CPUMIPSState *env, uint32_t wd,
                     target_ulong addr)
{
    wr_t *pwd = &(env->active_fpu.fpr[wd].wr);
    MEMOP_IDX(DF_WORD)
#if !defined(CONFIG_USER_ONLY)
#if !defined(HOST_WORDS_BIGENDIAN)
    pwd->w[0] = helper_ret_ldul_mmu(env, addr + (0 << DF_WORD), oi, GETPC());
    pwd->w[1] = helper_ret_ldul_mmu(env, addr + (1 << DF_WORD), oi, GETPC());
    pwd->w[2] = helper_ret_ldul_mmu(env, addr + (2 << DF_WORD), oi, GETPC());
    pwd->w[3] = helper_ret_ldul_mmu(env, addr + (3 << DF_WORD), oi, GETPC());
#else
    pwd->w[0] = helper_ret_ldul_mmu(env, addr + (1 << DF_WORD), oi, GETPC());
    pwd->w[1] = helper_ret_ldul_mmu(env, addr + (0 << DF_WORD), oi, GETPC());
    pwd->w[2] = helper_ret_ldul_mmu(env, addr + (3 << DF_WORD), oi, GETPC());
    pwd->w[3] = helper_ret_ldul_mmu(env, addr + (2 << DF_WORD), oi, GETPC());
#endif
#else
#if !defined(HOST_WORDS_BIGENDIAN)
    pwd->w[0] = cpu_ldl_data(env, addr + (0 << DF_WORD));
    pwd->w[1] = cpu_ldl_data(env, addr + (1 << DF_WORD));
    pwd->w[2] = cpu_ldl_data(env, addr + (2 << DF_WORD));
    pwd->w[3] = cpu_ldl_data(env, addr + (3 << DF_WORD));
#else
    pwd->w[0] = cpu_ldl_data(env, addr + (1 << DF_WORD));
    pwd->w[1] = cpu_ldl_data(env, addr + (0 << DF_WORD));
    pwd->w[2] = cpu_ldl_data(env, addr + (3 << DF_WORD));
    pwd->w[3] = cpu_ldl_data(env, addr + (2 << DF_WORD));
#endif
#endif
}

void helper_msa_ld_d(CPUMIPSState *env, uint32_t wd,
                     target_ulong addr)
{
    wr_t *pwd = &(env->active_fpu.fpr[wd].wr);
    MEMOP_IDX(DF_DOUBLE)
#if !defined(CONFIG_USER_ONLY)
    pwd->d[0] = helper_ret_ldq_mmu(env, addr + (0 << DF_DOUBLE), oi, GETPC());
    pwd->d[1] = helper_ret_ldq_mmu(env, addr + (1 << DF_DOUBLE), oi, GETPC());
#else
    pwd->d[0] = cpu_ldq_data(env, addr + (0 << DF_DOUBLE));
    pwd->d[1] = cpu_ldq_data(env, addr + (1 << DF_DOUBLE));
#endif
}

#define MSA_PAGESPAN(x) \
        ((((x) & ~TARGET_PAGE_MASK) + MSA_WRLEN / 8 - 1) >= TARGET_PAGE_SIZE)

static inline void ensure_writable_pages(CPUMIPSState *env,
                                         target_ulong addr,
                                         int mmu_idx,
                                         uintptr_t retaddr)
{
    /* FIXME: Probe the actual accesses (pass and use a size) */
    if (unlikely(MSA_PAGESPAN(addr))) {
        /* first page */
        probe_write(env, addr, 0, mmu_idx, retaddr);
        /* second page */
        addr = (addr & TARGET_PAGE_MASK) + TARGET_PAGE_SIZE;
        probe_write(env, addr, 0, mmu_idx, retaddr);
    }
}

void helper_msa_st_b(CPUMIPSState *env, uint32_t wd,
                     target_ulong addr)
{
    wr_t *pwd = &(env->active_fpu.fpr[wd].wr);
    int mmu_idx = cpu_mmu_index(env, false);

    MEMOP_IDX(DF_BYTE)
    ensure_writable_pages(env, addr, mmu_idx, GETPC());
#if !defined(CONFIG_USER_ONLY)
#if !defined(HOST_WORDS_BIGENDIAN)
    helper_ret_stb_mmu(env, addr + (0  << DF_BYTE), pwd->b[0],  oi, GETPC());
    helper_ret_stb_mmu(env, addr + (1  << DF_BYTE), pwd->b[1],  oi, GETPC());
    helper_ret_stb_mmu(env, addr + (2  << DF_BYTE), pwd->b[2],  oi, GETPC());
    helper_ret_stb_mmu(env, addr + (3  << DF_BYTE), pwd->b[3],  oi, GETPC());
    helper_ret_stb_mmu(env, addr + (4  << DF_BYTE), pwd->b[4],  oi, GETPC());
    helper_ret_stb_mmu(env, addr + (5  << DF_BYTE), pwd->b[5],  oi, GETPC());
    helper_ret_stb_mmu(env, addr + (6  << DF_BYTE), pwd->b[6],  oi, GETPC());
    helper_ret_stb_mmu(env, addr + (7  << DF_BYTE), pwd->b[7],  oi, GETPC());
    helper_ret_stb_mmu(env, addr + (8  << DF_BYTE), pwd->b[8],  oi, GETPC());
    helper_ret_stb_mmu(env, addr + (9  << DF_BYTE), pwd->b[9],  oi, GETPC());
    helper_ret_stb_mmu(env, addr + (10 << DF_BYTE), pwd->b[10], oi, GETPC());
    helper_ret_stb_mmu(env, addr + (11 << DF_BYTE), pwd->b[11], oi, GETPC());
    helper_ret_stb_mmu(env, addr + (12 << DF_BYTE), pwd->b[12], oi, GETPC());
    helper_ret_stb_mmu(env, addr + (13 << DF_BYTE), pwd->b[13], oi, GETPC());
    helper_ret_stb_mmu(env, addr + (14 << DF_BYTE), pwd->b[14], oi, GETPC());
    helper_ret_stb_mmu(env, addr + (15 << DF_BYTE), pwd->b[15], oi, GETPC());
#else
    helper_ret_stb_mmu(env, addr + (7  << DF_BYTE), pwd->b[0],  oi, GETPC());
    helper_ret_stb_mmu(env, addr + (6  << DF_BYTE), pwd->b[1],  oi, GETPC());
    helper_ret_stb_mmu(env, addr + (5  << DF_BYTE), pwd->b[2],  oi, GETPC());
    helper_ret_stb_mmu(env, addr + (4  << DF_BYTE), pwd->b[3],  oi, GETPC());
    helper_ret_stb_mmu(env, addr + (3  << DF_BYTE), pwd->b[4],  oi, GETPC());
    helper_ret_stb_mmu(env, addr + (2  << DF_BYTE), pwd->b[5],  oi, GETPC());
    helper_ret_stb_mmu(env, addr + (1  << DF_BYTE), pwd->b[6],  oi, GETPC());
    helper_ret_stb_mmu(env, addr + (0  << DF_BYTE), pwd->b[7],  oi, GETPC());
    helper_ret_stb_mmu(env, addr + (15 << DF_BYTE), pwd->b[8],  oi, GETPC());
    helper_ret_stb_mmu(env, addr + (14 << DF_BYTE), pwd->b[9],  oi, GETPC());
    helper_ret_stb_mmu(env, addr + (13 << DF_BYTE), pwd->b[10], oi, GETPC());
    helper_ret_stb_mmu(env, addr + (12 << DF_BYTE), pwd->b[11], oi, GETPC());
    helper_ret_stb_mmu(env, addr + (11 << DF_BYTE), pwd->b[12], oi, GETPC());
    helper_ret_stb_mmu(env, addr + (10 << DF_BYTE), pwd->b[13], oi, GETPC());
    helper_ret_stb_mmu(env, addr + (9  << DF_BYTE), pwd->b[14], oi, GETPC());
    helper_ret_stb_mmu(env, addr + (8  << DF_BYTE), pwd->b[15], oi, GETPC());
#endif
#else
#if !defined(HOST_WORDS_BIGENDIAN)
    cpu_stb_data(env, addr + (0  << DF_BYTE), pwd->b[0]);
    cpu_stb_data(env, addr + (1  << DF_BYTE), pwd->b[1]);
    cpu_stb_data(env, addr + (2  << DF_BYTE), pwd->b[2]);
    cpu_stb_data(env, addr + (3  << DF_BYTE), pwd->b[3]);
    cpu_stb_data(env, addr + (4  << DF_BYTE), pwd->b[4]);
    cpu_stb_data(env, addr + (5  << DF_BYTE), pwd->b[5]);
    cpu_stb_data(env, addr + (6  << DF_BYTE), pwd->b[6]);
    cpu_stb_data(env, addr + (7  << DF_BYTE), pwd->b[7]);
    cpu_stb_data(env, addr + (8  << DF_BYTE), pwd->b[8]);
    cpu_stb_data(env, addr + (9  << DF_BYTE), pwd->b[9]);
    cpu_stb_data(env, addr + (10 << DF_BYTE), pwd->b[10]);
    cpu_stb_data(env, addr + (11 << DF_BYTE), pwd->b[11]);
    cpu_stb_data(env, addr + (12 << DF_BYTE), pwd->b[12]);
    cpu_stb_data(env, addr + (13 << DF_BYTE), pwd->b[13]);
    cpu_stb_data(env, addr + (14 << DF_BYTE), pwd->b[14]);
    cpu_stb_data(env, addr + (15 << DF_BYTE), pwd->b[15]);
#else
    cpu_stb_data(env, addr + (7  << DF_BYTE), pwd->b[0]);
    cpu_stb_data(env, addr + (6  << DF_BYTE), pwd->b[1]);
    cpu_stb_data(env, addr + (5  << DF_BYTE), pwd->b[2]);
    cpu_stb_data(env, addr + (4  << DF_BYTE), pwd->b[3]);
    cpu_stb_data(env, addr + (3  << DF_BYTE), pwd->b[4]);
    cpu_stb_data(env, addr + (2  << DF_BYTE), pwd->b[5]);
    cpu_stb_data(env, addr + (1  << DF_BYTE), pwd->b[6]);
    cpu_stb_data(env, addr + (0  << DF_BYTE), pwd->b[7]);
    cpu_stb_data(env, addr + (15 << DF_BYTE), pwd->b[8]);
    cpu_stb_data(env, addr + (14 << DF_BYTE), pwd->b[9]);
    cpu_stb_data(env, addr + (13 << DF_BYTE), pwd->b[10]);
    cpu_stb_data(env, addr + (12 << DF_BYTE), pwd->b[11]);
    cpu_stb_data(env, addr + (11 << DF_BYTE), pwd->b[12]);
    cpu_stb_data(env, addr + (10 << DF_BYTE), pwd->b[13]);
    cpu_stb_data(env, addr + (9  << DF_BYTE), pwd->b[14]);
    cpu_stb_data(env, addr + (8  << DF_BYTE), pwd->b[15]);
#endif
#endif
}

void helper_msa_st_h(CPUMIPSState *env, uint32_t wd,
                     target_ulong addr)
{
    wr_t *pwd = &(env->active_fpu.fpr[wd].wr);
    int mmu_idx = cpu_mmu_index(env, false);

    MEMOP_IDX(DF_HALF)
    ensure_writable_pages(env, addr, mmu_idx, GETPC());
#if !defined(CONFIG_USER_ONLY)
#if !defined(HOST_WORDS_BIGENDIAN)
    helper_ret_stw_mmu(env, addr + (0 << DF_HALF), pwd->h[0], oi, GETPC());
    helper_ret_stw_mmu(env, addr + (1 << DF_HALF), pwd->h[1], oi, GETPC());
    helper_ret_stw_mmu(env, addr + (2 << DF_HALF), pwd->h[2], oi, GETPC());
    helper_ret_stw_mmu(env, addr + (3 << DF_HALF), pwd->h[3], oi, GETPC());
    helper_ret_stw_mmu(env, addr + (4 << DF_HALF), pwd->h[4], oi, GETPC());
    helper_ret_stw_mmu(env, addr + (5 << DF_HALF), pwd->h[5], oi, GETPC());
    helper_ret_stw_mmu(env, addr + (6 << DF_HALF), pwd->h[6], oi, GETPC());
    helper_ret_stw_mmu(env, addr + (7 << DF_HALF), pwd->h[7], oi, GETPC());
#else
    helper_ret_stw_mmu(env, addr + (3 << DF_HALF), pwd->h[0], oi, GETPC());
    helper_ret_stw_mmu(env, addr + (2 << DF_HALF), pwd->h[1], oi, GETPC());
    helper_ret_stw_mmu(env, addr + (1 << DF_HALF), pwd->h[2], oi, GETPC());
    helper_ret_stw_mmu(env, addr + (0 << DF_HALF), pwd->h[3], oi, GETPC());
    helper_ret_stw_mmu(env, addr + (7 << DF_HALF), pwd->h[4], oi, GETPC());
    helper_ret_stw_mmu(env, addr + (6 << DF_HALF), pwd->h[5], oi, GETPC());
    helper_ret_stw_mmu(env, addr + (5 << DF_HALF), pwd->h[6], oi, GETPC());
    helper_ret_stw_mmu(env, addr + (4 << DF_HALF), pwd->h[7], oi, GETPC());
#endif
#else
#if !defined(HOST_WORDS_BIGENDIAN)
    cpu_stw_data(env, addr + (0 << DF_HALF), pwd->h[0]);
    cpu_stw_data(env, addr + (1 << DF_HALF), pwd->h[1]);
    cpu_stw_data(env, addr + (2 << DF_HALF), pwd->h[2]);
    cpu_stw_data(env, addr + (3 << DF_HALF), pwd->h[3]);
    cpu_stw_data(env, addr + (4 << DF_HALF), pwd->h[4]);
    cpu_stw_data(env, addr + (5 << DF_HALF), pwd->h[5]);
    cpu_stw_data(env, addr + (6 << DF_HALF), pwd->h[6]);
    cpu_stw_data(env, addr + (7 << DF_HALF), pwd->h[7]);
#else
    cpu_stw_data(env, addr + (3 << DF_HALF), pwd->h[0]);
    cpu_stw_data(env, addr + (2 << DF_HALF), pwd->h[1]);
    cpu_stw_data(env, addr + (1 << DF_HALF), pwd->h[2]);
    cpu_stw_data(env, addr + (0 << DF_HALF), pwd->h[3]);
    cpu_stw_data(env, addr + (7 << DF_HALF), pwd->h[4]);
    cpu_stw_data(env, addr + (6 << DF_HALF), pwd->h[5]);
    cpu_stw_data(env, addr + (5 << DF_HALF), pwd->h[6]);
    cpu_stw_data(env, addr + (4 << DF_HALF), pwd->h[7]);
#endif
#endif
}

void helper_msa_st_w(CPUMIPSState *env, uint32_t wd,
                     target_ulong addr)
{
    wr_t *pwd = &(env->active_fpu.fpr[wd].wr);
    int mmu_idx = cpu_mmu_index(env, false);

    MEMOP_IDX(DF_WORD)
    ensure_writable_pages(env, addr, mmu_idx, GETPC());
#if !defined(CONFIG_USER_ONLY)
#if !defined(HOST_WORDS_BIGENDIAN)
    helper_ret_stl_mmu(env, addr + (0 << DF_WORD), pwd->w[0], oi, GETPC());
    helper_ret_stl_mmu(env, addr + (1 << DF_WORD), pwd->w[1], oi, GETPC());
    helper_ret_stl_mmu(env, addr + (2 << DF_WORD), pwd->w[2], oi, GETPC());
    helper_ret_stl_mmu(env, addr + (3 << DF_WORD), pwd->w[3], oi, GETPC());
#else
    helper_ret_stl_mmu(env, addr + (1 << DF_WORD), pwd->w[0], oi, GETPC());
    helper_ret_stl_mmu(env, addr + (0 << DF_WORD), pwd->w[1], oi, GETPC());
    helper_ret_stl_mmu(env, addr + (3 << DF_WORD), pwd->w[2], oi, GETPC());
    helper_ret_stl_mmu(env, addr + (2 << DF_WORD), pwd->w[3], oi, GETPC());
#endif
#else
#if !defined(HOST_WORDS_BIGENDIAN)
    cpu_stl_data(env, addr + (0 << DF_WORD), pwd->w[0]);
    cpu_stl_data(env, addr + (1 << DF_WORD), pwd->w[1]);
    cpu_stl_data(env, addr + (2 << DF_WORD), pwd->w[2]);
    cpu_stl_data(env, addr + (3 << DF_WORD), pwd->w[3]);
#else
    cpu_stl_data(env, addr + (1 << DF_WORD), pwd->w[0]);
    cpu_stl_data(env, addr + (0 << DF_WORD), pwd->w[1]);
    cpu_stl_data(env, addr + (3 << DF_WORD), pwd->w[2]);
    cpu_stl_data(env, addr + (2 << DF_WORD), pwd->w[3]);
#endif
#endif
}

void helper_msa_st_d(CPUMIPSState *env, uint32_t wd,
                     target_ulong addr)
{
    wr_t *pwd = &(env->active_fpu.fpr[wd].wr);
    int mmu_idx = cpu_mmu_index(env, false);

    MEMOP_IDX(DF_DOUBLE)
    ensure_writable_pages(env, addr, mmu_idx, GETPC());
#if !defined(CONFIG_USER_ONLY)
    helper_ret_stq_mmu(env, addr + (0 << DF_DOUBLE), pwd->d[0], oi, GETPC());
    helper_ret_stq_mmu(env, addr + (1 << DF_DOUBLE), pwd->d[1], oi, GETPC());
#else
    cpu_stq_data(env, addr + (0 << DF_DOUBLE), pwd->d[0]);
    cpu_stq_data(env, addr + (1 << DF_DOUBLE), pwd->d[1]);
#endif
}

void helper_cache(CPUMIPSState *env, target_ulong addr, uint32_t op)
{
#ifndef CONFIG_USER_ONLY
    target_ulong index = addr & 0x1fffffff;
    if (op == 9) {
        /* Index Store Tag */
        memory_region_dispatch_write(env->itc_tag, index, env->CP0_TagLo,
                                     MO_64, MEMTXATTRS_UNSPECIFIED);
    } else if (op == 5) {
        /* Index Load Tag */
        memory_region_dispatch_read(env->itc_tag, index, &env->CP0_TagLo,
                                    MO_64, MEMTXATTRS_UNSPECIFIED);
    }
#endif
}

/* Reduce the length so that addr + len doesn't cross a page boundary.  */
static inline target_ulong adj_len_to_page(target_ulong len, target_ulong addr)
{
#ifndef CONFIG_USER_ONLY
    target_ulong low_bits = (addr & ~TARGET_PAGE_MASK);
    if (low_bits + len - 1 >= TARGET_PAGE_SIZE) {
        return TARGET_PAGE_SIZE - low_bits;
    }
#endif
    return len;
}


#define MAGIC_LIBCALL_HELPER_CONTINUATION_FLAG UINT64_C(0xbadc0de)
#define MIPS_REGNUM_V0 2
#define MIPS_REGNUM_V1 3
#define MIPS_REGNUM_A0 4
#define MIPS_REGNUM_A1 5
#define MIPS_REGNUM_A2 6
#define MIPS_REGNUM_A3 7

#ifdef CONFIG_DEBUG_TCG
#define MAGIC_MEMSET_STATS 1
#else
#define MAGIC_MEMSET_STATS 0
#endif

#if MAGIC_MEMSET_STATS != 0
static bool memset_stats_dump_registered = false;

struct nop_stats {
    uint64_t kernel_mode_bytes;
    uint64_t kernel_mode_count;
    uint64_t user_mode_bytes;
    uint64_t user_mode_count;
};

static struct nop_stats magic_memset_zero_bytes;
static struct nop_stats magic_memset_nonzero_bytes;

static struct nop_stats magic_memcpy_bytes;
static struct nop_stats magic_memmove_bytes;
static struct nop_stats magic_bcopy_bytes;

static struct nop_stats magic_memmove_slowpath;

static inline void print_nop_stats(const char* msg, struct nop_stats* stats) {
    warn_report("%s in kernel mode: %" PRId64 " (%f MB) in %" PRId64 " calls\r", msg,
                stats->kernel_mode_bytes, stats->kernel_mode_bytes / (1024.0 * 1024.0), stats->kernel_mode_count);
    warn_report("%s in user   mode: %" PRId64 " (%f MB) in %" PRId64 " calls\r", msg,
                stats->user_mode_bytes, stats->user_mode_bytes / (1024.0 * 1024.0), stats->user_mode_count);
}

static void dump_memset_stats_on_exit(void) {
    print_nop_stats("memset (zero)    with magic nop", &magic_memset_zero_bytes);
    print_nop_stats("memset (nonzero) with magic nop", &magic_memset_nonzero_bytes);
    print_nop_stats("memcpy with magic nop", &magic_memcpy_bytes);
    print_nop_stats("memmove with magic nop", &magic_memmove_bytes);
    print_nop_stats("bcopy with magic nop", &magic_bcopy_bytes);
    print_nop_stats("memmove/memcpy/bcopy slowpath", &magic_memmove_slowpath);
}

static inline void collect_magic_nop_stats(CPUMIPSState *env, struct nop_stats* stats, target_ulong bytes) {
    if (!memset_stats_dump_registered) {
        // TODO: move this to CPU_init
        atexit(dump_memset_stats_on_exit);
        memset_stats_dump_registered = true;
    }
    if (in_kernel_mode(env)) {
        stats->kernel_mode_bytes += bytes;
        stats->kernel_mode_count++;
    } else {
        stats->user_mode_bytes += bytes;
        stats->user_mode_count++;
    }
}
#else
#define collect_magic_nop_stats(env, stats, bytes)
#endif


static inline void
store_byte_and_clear_tag(CPUMIPSState *env, target_ulong vaddr, uint8_t val,
                         TCGMemOpIdx oi, uintptr_t retaddr)
{
    helper_ret_stb_mmu(env, vaddr, val, oi, retaddr);
#ifdef TARGET_CHERI
    // If we returned (i.e. write was successful) we also need to invalidate the
    // tags bit to ensure we are consistent with sb
    cheri_tag_invalidate(env, vaddr, 1, retaddr);
#endif
}

static inline void
store_u32_and_clear_tag(CPUMIPSState *env, target_ulong vaddr, uint32_t val,
                         TCGMemOpIdx oi, uintptr_t retaddr)
{
    helper_ret_stw_mmu(env, vaddr, val, oi, retaddr);
#ifdef TARGET_CHERI
    // If we returned (i.e. write was successful) we also need to invalidate the
    // tags bit to ensure we are consistent with sb
    cheri_tag_invalidate(env, vaddr, 4, retaddr);
#endif
}

#ifdef TARGET_CHERI
#define CHECK_AND_ADD_DDC(env, perms, ptr, len, retpc) check_ddc(env, perms, ptr, len, retpc);
#else
#define CHECK_AND_ADD_DDC(env, perms, ptr, len, retpc) ptr
#endif

static bool do_magic_memmove(CPUMIPSState *env, uint64_t ra, int dest_regnum, int src_regnum)
{
    tcg_debug_assert(dest_regnum != src_regnum);
    const target_ulong original_dest_ddc_offset = env->active_tc.gpr[dest_regnum]; // $a0 = dest
    const target_ulong original_src_ddc_offset = env->active_tc.gpr[src_regnum];  // $a1 = src
    const target_ulong original_len = env->active_tc.gpr[MIPS_REGNUM_A2];  // $a2 = len
    int mmu_idx = cpu_mmu_index(env, false);
    TCGMemOpIdx oi = make_memop_idx(MO_UB, mmu_idx);
    target_ulong len = original_len;
    target_ulong already_written = 0;
    const bool is_continuation = (env->active_tc.gpr[MIPS_REGNUM_V1] >> 32) == MAGIC_LIBCALL_HELPER_CONTINUATION_FLAG;
    if (is_continuation) {
        // This is a partial write -> $a0 is the original dest argument.
        // The already written bytes (from the partial write) was stored in $v0 by the previous call
        already_written = env->active_tc.gpr[MIPS_REGNUM_V0];
        tcg_debug_assert(already_written < len);
        len -= already_written; // update the remaining length
#if 0
        fprintf(stderr, "--- %s: Got continuation for 0x" TARGET_FMT_lx " byte access at 0x" TARGET_FMT_plx
                        " -- current dest = 0x" TARGET_FMT_plx " -- current len = 0x" TARGET_FMT_lx "\r\n",
                        __func__, original_len, original_dest, dest, len);
#endif
    } else {
        // Not a partial write -> $v0 should be zero otherwise this is a usage error!
        if (env->active_tc.gpr[MIPS_REGNUM_V0] != 0) {
            error_report("ERROR: Attempted to call memset library function "
                          "with non-zero value in $v0 (0x" TARGET_FMT_lx
                          ") and continuation flag not set in $v1 (0x" TARGET_FMT_lx
                          ")!\n", env->active_tc.gpr[MIPS_REGNUM_V0], env->active_tc.gpr[MIPS_REGNUM_V1]);
            do_raise_exception(env, EXCP_RI, GETPC());
        }
    }
    if (len == 0) {
        goto success; // nothing to do
    }
    if (original_src_ddc_offset == original_dest_ddc_offset) {
        already_written = original_len;
        goto success; // nothing to do
    }
    // Check capability bounds for the whole copy
    // If it is going to fail we don't bother doing a partial copy!
    const target_ulong original_src = CHECK_AND_ADD_DDC(env, CAP_PERM_LOAD, original_src_ddc_offset, original_len, ra);
    const target_ulong original_dest = CHECK_AND_ADD_DDC(env, CAP_PERM_STORE, original_dest_ddc_offset, original_len, ra);

    // Mark this as a continuation in $v1 (so that we continue sensibly if we get a tlb miss and longjump out)
    env->active_tc.gpr[MIPS_REGNUM_V1] = (MAGIC_LIBCALL_HELPER_CONTINUATION_FLAG << 32) | env->active_tc.gpr[3];

    const target_ulong dest_past_end = original_dest + original_len;
    const target_ulong src_past_end = original_src + original_len;
#if 0 // FIXME: for some reason this causes errors
    const bool log_instr = qemu_loglevel_mask(CPU_LOG_INSTR | CPU_LOG_CVTRACE);
    const bool dest_same_page = (original_dest & TARGET_PAGE_MASK) == ((dest_past_end - 1) & TARGET_PAGE_MASK);
    const bool src_same_page = (original_dest & TARGET_PAGE_MASK) == ((dest_past_end - 1) & TARGET_PAGE_MASK);
    // If neither src nor dest buffer cross a page boundary we can just do an address_space_read+write
    // Fast case: less than a page and neither of the buffers crosses a page boundary
    CPUState *cs = env_cpu(env);
    if (dest_same_page && src_same_page) {
        tcg_debug_assert(already_written == 0);
        tcg_debug_assert(len <= TARGET_PAGE_SIZE);
        // The translation operation might trap and longjump out!
        hwaddr src_paddr = do_translate_address(env, original_src, MMU_DATA_LOAD, ra);
        hwaddr dest_paddr = do_translate_address(env, original_dest, MMU_DATA_STORE, ra);
#ifdef TARGET_CHERI
        if (dest_paddr <= env->CP0_LLAddr && dest_paddr + len > env->CP0_LLAddr) {
            // reset the linked flag if we touch the address with this write
            env->linkedflag = 0;
            env->lladdr = 0;
        }
#endif
        // Do a single load+store to update the MMU flags
        // uint8_t first_value = helper_ret_ldub_mmu(env, original_src, oi, ra);
        // Note: address_space_write will also clear the tag bits!
        MemTxResult result = MEMTX_ERROR;
        uint8_t buffer[TARGET_PAGE_SIZE];
        result = address_space_read(cs->as, src_paddr, MEMTXATTRS_UNSPECIFIED, buffer, len);
        if (result != MEMTX_OK) {
            warn_report("magic memmove: error reading %d bytes from paddr %"HWADDR_PRIx
                        ". Unmapped memory? Error code was %d\r", (int)len, src_paddr, result);
            // same ignored error would happen with normal loads/stores -> just continue
        }
        fprintf(stderr, "Used fast path to read %d bytes\r\n", (int)len);
        // do_hexdump(stderr, buffer, len, original_src);
        // fprintf(stderr, "\r");
        // also write one byte to the target buffer to ensure that the flags are updated
        // store_byte_and_clear_tag(env, original_dest, first_value, oi, ra); // might trap
        result = address_space_write(cs->as, dest_paddr, MEMTXATTRS_UNSPECIFIED, buffer, len);
        if (unlikely(log_instr)) {
            for (int i = 0; i < len; i++) {
                helper_dump_load64(env, original_src + i, buffer[i], MO_8);
                helper_dump_store64(env, original_dest + i, buffer[i], MO_8);
            }
        }
        if (result != MEMTX_OK) {
            warn_report("magic memmove: error writing %d bytes to paddr %"HWADDR_PRIx
                        ". Unmapped memory? Error code was %d\r", (int)len, dest_paddr, result);
            // same ignored error would happen with normal loads/stores -> just continue
        }
        already_written += len;
        env->active_tc.gpr[MIPS_REGNUM_V0] = already_written;
        goto success;
    }
#endif

    const bool has_overlap = MAX(original_dest, original_src) >= MAX(dest_past_end, src_past_end);
    if (has_overlap) {
        warn_report("Found multipage magic memmove with overlap: dst=" TARGET_FMT_plx " src=" TARGET_FMT_plx
                    " len=0x" TARGET_FMT_lx "\r", original_dest, original_src, original_len);
        // slow path: byte copies
    }

    const bool copy_backwards = original_src < original_dest;
    if (copy_backwards) {
        target_ulong current_dest_cursor = original_dest + len - 1;
        target_ulong current_src_cursor = original_src + len - 1;
        /* Slow path (probably attempt to do this to an I/O device or
         * similar, or clearing of a block of code we have translations
         * cached for). Just do a series of byte writes as the architecture
         * demands. It's not worth trying to use a cpu_physical_memory_map(),
         * memset(), unmap() sequence here because:
         *  + we'd need to account for the blocksize being larger than a page
         *  + the direct-RAM access case is almost always going to be dealt
         *    with in the fastpath code above, so there's no speed benefit
         *  + we would have to deal with the map returning NULL because the
         *    bounce buffer was in use
         */
        tcg_debug_assert(original_len - already_written == len);
        collect_magic_nop_stats(env, &magic_memmove_slowpath, len);
        while (already_written < original_len) {
            uint8_t value = helper_ret_ldub_mmu(env, current_src_cursor, oi, ra);
            store_byte_and_clear_tag(env, current_dest_cursor, value, oi, ra); // might trap
             current_dest_cursor--;
            current_src_cursor--;
            already_written++;
            env->active_tc.gpr[MIPS_REGNUM_V0] = already_written;
        }
    } else {
        // copy forwards
        target_ulong current_dest_cursor = original_dest + already_written;
        target_ulong current_src_cursor = original_src + already_written;
        /* Slow path (probably attempt to do this to an I/O device or
         * similar, or clearing of a block of code we have translations
         * cached for). Just do a series of byte writes as the architecture
         * demands. It's not worth trying to use a cpu_physical_memory_map(),
         * memset(), unmap() sequence here because:
         *  + we'd need to account for the blocksize being larger than a page
         *  + the direct-RAM access case is almost always going to be dealt
         *    with in the fastpath code above, so there's no speed benefit
         *  + we would have to deal with the map returning NULL because the
         *    bounce buffer was in use
         */
        tcg_debug_assert(original_len - already_written == len);
        collect_magic_nop_stats(env, &magic_memmove_slowpath, len);
        while (already_written < original_len) {
            uint8_t value = helper_ret_ldub_mmu(env, current_src_cursor, oi, ra);
            store_byte_and_clear_tag(env, current_dest_cursor, value, oi, ra); // might trap
            current_dest_cursor++;
            current_src_cursor++;
            already_written++;
            env->active_tc.gpr[MIPS_REGNUM_V0] = already_written;
        }
    }
success:
    if (unlikely(already_written != original_len)) {
        error_report("ERROR: %s: failed to memmove all bytes to " TARGET_FMT_plx " (" TARGET_FMT_plx " with $ddc added).\r\n"
                     "Remainig len = " TARGET_FMT_plx ", full len = " TARGET_FMT_plx ".\r\n"
                     "Source address = " TARGET_FMT_plx " (" TARGET_FMT_plx " with $ddc added)\r\n",
                     __func__, original_dest_ddc_offset, original_dest, len, original_len, original_src_ddc_offset, original_src);
        error_report("$a0: " TARGET_FMT_plx "\r\n", env->active_tc.gpr[MIPS_REGNUM_A0]);
        error_report("$a1: " TARGET_FMT_plx "\r\n", env->active_tc.gpr[MIPS_REGNUM_A1]);
        error_report("$a2: " TARGET_FMT_plx "\r\n", env->active_tc.gpr[MIPS_REGNUM_A2]);
        error_report("$v0: " TARGET_FMT_plx "\r\n", env->active_tc.gpr[MIPS_REGNUM_V0]);
        error_report("$v1: " TARGET_FMT_plx "\r\n", env->active_tc.gpr[MIPS_REGNUM_V1]);
        abort();
    }
    env->active_tc.gpr[MIPS_REGNUM_V0] = original_dest_ddc_offset; // return value of memcpy is the dest argument
    return true;
}

static uint8_t ZEROARRAY[TARGET_PAGE_SIZE];

static void do_memset_pattern_hostaddr(void* hostaddr, uint64_t value, uint64_t nitems, unsigned pattern_length, uint64_t ra) {
    if (pattern_length == 1) {
        memset(hostaddr, value, nitems);
    } else if (pattern_length == 4) {
        uint32_t* ptr = hostaddr;
        uint32_t target_value = tswap32((uint32_t)value);
        for (target_ulong i = 0; i < nitems; i++) {
            *ptr = target_value;
            ptr++;
        }
    } else {
        assert(false && "unsupported memset pattern length");
    }
}

static bool do_magic_memset(CPUMIPSState *env, uint64_t ra, uint pattern_length)
{
    // TODO: just use address_space_write?

    // See target/s390x/mem_helper.c and arm/helper.c HELPER(dc_zva)
    int mmu_idx = cpu_mmu_index(env, false);
    TCGMemOpIdx oi = make_memop_idx(MO_UB, mmu_idx);

    const target_ulong original_dest_ddc_offset = env->active_tc.gpr[MIPS_REGNUM_A0];      // $a0 = dest
    uint64_t value = env->active_tc.gpr[MIPS_REGNUM_A1]; // $a1 = c
    const target_ulong original_len_nitems = env->active_tc.gpr[MIPS_REGNUM_A2];       // $a2 = len
    const target_ulong original_len_bytes = original_len_nitems * pattern_length;
    target_ulong dest = original_dest_ddc_offset;
    target_ulong len_nitems = original_len_nitems;
    const bool is_continuation = (env->active_tc.gpr[MIPS_REGNUM_V1] >> 32) == MAGIC_LIBCALL_HELPER_CONTINUATION_FLAG;
    if (is_continuation) {
        // This is a partial write -> $a0 is the original dest argument.
        // The updated dest (after the partial write) was stored in $v0 by the previous call
        dest = env->active_tc.gpr[MIPS_REGNUM_V0];
        if (dest < original_dest_ddc_offset || dest >= original_dest_ddc_offset + original_len_bytes) {
            error_report("ERROR: Attempted to call memset library function "
                         "with invalid dest in $v0 (0x" TARGET_FMT_lx
                         ") and continuation flag set. orig dest = 0x" TARGET_FMT_lx
                         " and orig bytes = 0x" TARGET_FMT_lx "!\n",
                         env->active_tc.gpr[MIPS_REGNUM_V0], env->active_tc.gpr[MIPS_REGNUM_A0],
                         env->active_tc.gpr[MIPS_REGNUM_A2]);
            do_raise_exception(env, EXCP_RI, ra);
        }
        target_ulong already_written = dest - original_dest_ddc_offset;
        len_nitems -= already_written / pattern_length; // update the remaining length
        assert((already_written % pattern_length) == 0);
#if 0
        fprintf(stderr, "--- %s: Got continuation for 0x" TARGET_FMT_lx " byte access at 0x" TARGET_FMT_plx
                        " -- current dest = 0x" TARGET_FMT_plx " -- current len = 0x" TARGET_FMT_lx "\r\n",
                        __func__, original_len, original_dest, dest, len_nitems);
#endif
    } else {
        // Not a partial write -> $v0 should be zero otherwise this is a usage error!
        if (env->active_tc.gpr[MIPS_REGNUM_V0] != 0) {
            error_report("ERROR: Attempted to call memset library function "
                         "with non-zero value in $v0 (0x" TARGET_FMT_lx
                         ") and continuation flag not set in $v1 (0x" TARGET_FMT_lx
                         ")!\n", env->active_tc.gpr[MIPS_REGNUM_V0], env->active_tc.gpr[MIPS_REGNUM_V1]);
            do_raise_exception(env, EXCP_RI, ra);
        }
    }
    dest = CHECK_AND_ADD_DDC(env, CAP_PERM_STORE, dest, len_nitems * pattern_length, ra);
    const target_ulong original_dest = CHECK_AND_ADD_DDC(env, CAP_PERM_STORE, original_dest_ddc_offset, original_len_nitems, ra);

    tcg_debug_assert(dest + (len_nitems * pattern_length) == original_dest + original_len_bytes && "continuation broken?");
    const bool log_instr = qemu_loglevel_mask(CPU_LOG_INSTR | CPU_LOG_CVTRACE);
    if (len_nitems == 0) {
        goto success; // nothing to do
    }

    CPUState *cs = env_cpu(env);

    while (len_nitems > 0) {
        const target_ulong total_len_nbytes = len_nitems * pattern_length;
        assert(dest + total_len_nbytes == original_dest + original_len_bytes && "continuation broken?");
        // probing for write access:
        // fprintf(stderr, "Probing for write access at " TARGET_FMT_plx "\r\n", dest);
        // fflush(stderr);
        // update $v0 to point to the updated dest in case probe_write_access takes a tlb fault:
        env->active_tc.gpr[MIPS_REGNUM_V0] = dest;
        // and mark this as a continuation in $v1 (so that we continue sensibly after the tlb miss was handled)
        env->active_tc.gpr[MIPS_REGNUM_V1] = (MAGIC_LIBCALL_HELPER_CONTINUATION_FLAG << 32) | env->active_tc.gpr[3];
        // If the host address is not in the tlb after the second write we are writing
        // to something strange so just fall back to the slowpath
        target_ulong l_adj_bytes = adj_len_to_page(total_len_nbytes, dest);
        target_ulong l_adj_nitems = l_adj_bytes;
        if (unlikely(pattern_length != 1)) {
            l_adj_nitems = l_adj_bytes / pattern_length;
        }
        tcg_debug_assert(l_adj_nitems != 0);
        tcg_debug_assert(((dest + l_adj_bytes - 1) & TARGET_PAGE_MASK) == (dest & TARGET_PAGE_MASK) && "should not cross a page boundary!");
        void* hostaddr = NULL;
#if 0
        for (int try = 0; try < 2; try++) {
            hostaddr = tlb_vaddr_to_host(env, dest, 1, mmu_idx);
            if (hostaddr)
                break;
            /* OK, try a store and see if we can populate the tlb. This
             * might cause an exception if the memory isn't writable,
             * in which case we will longjmp out of here. We must for
             * this purpose use the actual register value passed to us
             * so that we get the fault address right.
             */
            store_byte_and_clear_tag(env, dest, 0xff, oi, ra);
            // This should definitely fill the host TLB. If not we are probably writing to I/O memory
            // FIXME: tlb_vaddr_to_host() also returns null if the notdirty flag is set (not sure if that's a bug
            probe_write(env, dest, 1, mmu_idx, ra);
        }
#endif
        if (hostaddr) {
            /* If it's all in the TLB it's fair game for just writing to;
             * we know we don't need to update dirty status, etc.
             */
            tcg_debug_assert(dest + total_len_nbytes == original_dest + original_len_bytes && "continuation broken?");
            // Do one store byte to update MMU flags (not sure this is necessary)
            store_byte_and_clear_tag(env, dest, value, oi, ra);
            do_memset_pattern_hostaddr(hostaddr, value, l_adj_nitems, pattern_length, ra);
#ifdef TARGET_CHERI
            // We also need to invalidate the tags bits written by the memset
            // qemu_ram_addr_from_host is faster than using the v2r routines in cheri_tag_invalidate
            ram_addr_t ram_addr = qemu_ram_addr_from_host(hostaddr);
            if (ram_addr != RAM_ADDR_INVALID) {
                cheri_tag_phys_invalidate(env, ram_addr, l_adj_bytes, NULL);
            } else {
                cheri_tag_invalidate(env, dest, l_adj_bytes, ra);
            }
#endif
            if (unlikely(log_instr)) {
                // TODO: dump as a single big block?
                for (target_ulong i = 0; i < l_adj_nitems; i++) {
                    if (pattern_length == 1)
                        helper_dump_store64(env, dest + i, value, MO_8);
                    else if (pattern_length == 4)
                        helper_dump_store64(env, dest + (i * pattern_length), value, MO_32);
                    else
                        assert(false && "invalid pattern length");
                }
                qemu_log_mask(CPU_LOG_INSTR, "%s: Set " TARGET_FMT_ld
                    " %d-byte items to 0x%" PRIx64 " at 0x" TARGET_FMT_plx "\n",
                    __func__, l_adj_nitems, pattern_length, value, dest);
            }
            // fprintf(stderr, "%s: Set " TARGET_FMT_ld " bytes to 0x%x at 0x" TARGET_FMT_plx "/%p\r\n", __func__, l_adj, value, dest, hostaddr);
            dest += l_adj_bytes;
            len_nitems -= l_adj_nitems;
        } else {
            // First try address_space_write and if that fails fall back to bytewise setting
            hwaddr paddr = do_translate_address(env, dest, MMU_DATA_STORE, ra);
#ifdef TARGET_CHERI
            if (paddr <= env->CP0_LLAddr && paddr + l_adj_bytes > env->CP0_LLAddr) {
                // reset the linked flag if we touch the address with this write
                env->linkedflag = 0;
                env->lladdr = 0;
            }
#endif
            // Note: address_space_write will also clear the tag bits!
            MemTxResult result = MEMTX_ERROR;
            if (value == 0) {
                 result = address_space_write(cs->as, paddr, MEMTXATTRS_UNSPECIFIED, ZEROARRAY, l_adj_bytes);
            } else {
                // create a buffer filled with the correct value and use that for the write
                uint8_t setbuffer[TARGET_PAGE_SIZE];
                tcg_debug_assert(l_adj_bytes <= sizeof(setbuffer));
                do_memset_pattern_hostaddr(setbuffer, value, l_adj_nitems, pattern_length, ra);
                result = address_space_write(cs->as, paddr, MEMTXATTRS_UNSPECIFIED, setbuffer, l_adj_bytes);
            }
            if (result == MEMTX_OK) {
                dest += l_adj_bytes;
                len_nitems -= l_adj_nitems;
                if (unlikely(log_instr)) {
                    // TODO: dump as a single big block?
                    for (target_ulong i = 0; i < l_adj_nitems; i++) {
                        if (pattern_length == 1)
                            helper_dump_store64(env, dest + i, value, MO_8);
                        else if (pattern_length == 4)
                            helper_dump_store64(env, dest + (i * pattern_length), value, MO_32);
                        else
                            assert(false && "invalid pattern length");
                    }
                    qemu_log_mask(CPU_LOG_INSTR, "%s: Set " TARGET_FMT_ld
                        " %d-byte items to 0x%" PRIx64 " at 0x" TARGET_FMT_plx "\n",
                        __func__, l_adj_nitems, pattern_length, value, dest);
                }
                continue; // try again with next page
            } else {
                warn_report("address_space_write failed with error %d for %"HWADDR_PRIx "\r", result, paddr);
                // fall back to bytewise copy slow path
            }
            /* Slow path (probably attempt to do this to an I/O device or
             * similar, or clearing of a block of code we have translations
             * cached for). Just do a series of byte writes as the architecture
             * demands. It's not worth trying to use a cpu_physical_memory_map(),
             * memset(), unmap() sequence here because:
             *  + we'd need to account for the blocksize being larger than a page
             *  + the direct-RAM access case is almost always going to be dealt
             *    with in the fastpath code above, so there's no speed benefit
             *  + we would have to deal with the map returning NULL because the
             *    bounce buffer was in use
             */
            warn_report("%s: Falling back to memset slowpath for address "
                        TARGET_FMT_plx " (phys addr=%" HWADDR_PRIx", len_nitems=0x"
                        TARGET_FMT_lx ")! I/O memory or QEMU TLB bug?\r",
                        __func__, dest, mips_cpu_get_phys_page_debug(env_cpu(env), dest), len_nitems);
            target_ulong end = original_dest + original_len_bytes;
            tcg_debug_assert(((end - dest) % pattern_length) == 0);
            // in case we fault mark this as a continuation in $v1 (so that we continue sensibly after the tlb miss was handled)
            env->active_tc.gpr[MIPS_REGNUM_V1] = (MAGIC_LIBCALL_HELPER_CONTINUATION_FLAG << 32) | env->active_tc.gpr[MIPS_REGNUM_V1];
            while (dest < end) {
                tcg_debug_assert(dest + (len_nitems * pattern_length) == original_dest + original_len_bytes && "continuation broken?");
                // update $v0 to point to the updated dest in case probe_write_access takes a tlb fault:
                env->active_tc.gpr[MIPS_REGNUM_V0] = dest;
                if (pattern_length == 1) {
                    store_byte_and_clear_tag(env, dest, value, oi, ra); // might trap
                } else if (pattern_length == 4) {
                    store_u32_and_clear_tag(env, dest, value, oi, ra); // might trap
                } else {
                    assert(false && "invalid pattern length");
                }
                if (unlikely(log_instr)) {
                    if (pattern_length == 1)
                        helper_dump_store64(env, dest, value, MO_8);
                    else if (pattern_length == 4)
                        helper_dump_store64(env, dest, value, MO_32);
                    else
                        assert(false && "invalid pattern length");
                    helper_dump_store64(env, dest, value, MO_8);
                }
                dest += pattern_length;
                len_nitems--;
            }
        }
    }
    tcg_debug_assert(len_nitems == 0);
success:
    env->active_tc.gpr[MIPS_REGNUM_V0] = original_dest_ddc_offset; // return value of memset is the src argument
    // also update a0 and a2 to match what the kernel memset does (a0 -> buf end, a2 -> 0):
    env->active_tc.gpr[MIPS_REGNUM_A0] = dest;
    env->active_tc.gpr[MIPS_REGNUM_A2] = len_nitems;
#if MAGIC_MEMSET_STATS != 0
    collect_magic_nop_stats(env, value == 0 ? &magic_memset_zero_bytes : &magic_memset_nonzero_bytes, original_len_bytes);
#endif
    return true;
}

#define MAGIC_HELPER_DONE_FLAG 0xDEC0DED

enum {
    MAGIC_NOP_MEMSET = 1,
    MAGIC_NOP_MEMSET_C = 2,
    MAGIC_NOP_MEMCPY = 3,
    MAGIC_NOP_MEMCPY_C = 4,
    MAGIC_NOP_MEMMOVE = 5,
    MAGIC_NOP_MEMMOVE_C = 6,
    MAGIC_NOP_BCOPY = 7,
    MAGIC_NOP_U32_MEMSET = 8,
};



// Magic library function calls:
void helper_magic_library_function(CPUMIPSState *env, target_ulong which)
{
    qemu_log_mask(CPU_LOG_INSTR, "--- Calling magic library function 0x" TARGET_FMT_lx "\n", which);
    // High bits can be used by function to indicate continuation after TLB miss
    switch (which & UINT32_MAX) {
    case MAGIC_NOP_MEMSET:
        if (!do_magic_memset(env, GETPC(), /*pattern_size=*/1))
            return;
        // otherwise update $v1 to indicate success
        break;

    case MAGIC_NOP_U32_MEMSET:
        if (!do_magic_memset(env, GETPC(), /*pattern_size=*/4))
            return;
        // otherwise update $v1 to indicate success
        break;

    case MAGIC_NOP_MEMCPY:
        if (!do_magic_memmove(env, GETPC(), MIPS_REGNUM_A0, MIPS_REGNUM_A1))
            goto error;
        collect_magic_nop_stats(env, &magic_memcpy_bytes, env->active_tc.gpr[MIPS_REGNUM_A2]);
        break;

    case MAGIC_NOP_MEMMOVE:
        if (!do_magic_memmove(env, GETPC(), MIPS_REGNUM_A0, MIPS_REGNUM_A1))
            goto error;
        collect_magic_nop_stats(env, &magic_memmove_bytes, env->active_tc.gpr[MIPS_REGNUM_A2]);
        break;

    case MAGIC_NOP_BCOPY: // src + dest arguments swapped
        if (!do_magic_memmove(env, GETPC(), MIPS_REGNUM_A1, MIPS_REGNUM_A0))
            goto error;
        collect_magic_nop_stats(env, &magic_bcopy_bytes, env->active_tc.gpr[MIPS_REGNUM_A2]);
        break;

    case 0xf0:
    case 0xf1:
    {
        uint8_t buffer[TARGET_PAGE_SIZE];
        // to match memset/memcpy calling convention (use a0 and a2)
        target_ulong src = env->active_tc.gpr[MIPS_REGNUM_A0];
        target_ulong real_len = env->active_tc.gpr[MIPS_REGNUM_A2];
        fprintf(stderr, "--- Memory dump at %s(%s): " TARGET_FMT_lu " bytes at " TARGET_FMT_plx "\r\n",
                lookup_symbol(PC_ADDR(env)), ((which & UINT32_MAX) == 0xf0 ? "entry" : "exit"), real_len, src);
        while (real_len > 0) {
            target_ulong len = adj_len_to_page(real_len, src);
            real_len -= len;
            if (len != env->active_tc.gpr[MIPS_REGNUM_A2]) {
                fprintf(stderr, "--- partial dump at %s(%s): " TARGET_FMT_lu " bytes at " TARGET_FMT_plx "\r\n",
                        lookup_symbol(PC_ADDR(env)), ((which & UINT32_MAX) == 0xf0 ? "entry" : "exit"), len, src);
            }
            if (cpu_memory_rw_debug(env_cpu(env), src, buffer, len, false) == 0) {
                bool have_nonzero = false;
                for (int i = 0; i < len; i++)
                    if (buffer[i] != 0)
                        have_nonzero = true;
                if (have_nonzero)
                    do_hexdump(stderr, buffer, len, src);
                else
                    fprintf(stderr, "   -- all zeroes\r\n");
            } else {
                fprintf(stderr, "--- Memory dump at %s(%s): Could not fetch" TARGET_FMT_lu " bytes at " TARGET_FMT_plx "\r\n",
                        lookup_symbol(PC_ADDR(env)), ((which & UINT32_MAX) == 0xf0 ? "entry" : "exit"), len, src);
            }
        }
    }
    case 0xfe:
    case 0xff:
        // dump argument and return registers:
        warn_report("%s(%s): argument+return registers: \r\n"
                    "\tv0 = 0x" TARGET_FMT_lx "\tv1 = 0x" TARGET_FMT_lx "\r\n"
                    "\ta0 = 0x" TARGET_FMT_lx "\ta1 = 0x" TARGET_FMT_lx "\r\n"
                    "\ta2 = 0x" TARGET_FMT_lx "\ta3 = 0x" TARGET_FMT_lx "\r\n",
                    lookup_symbol(PC_ADDR(env)), ((which & UINT32_MAX) == 0xfe ? "entry" : "exit"),
                    env->active_tc.gpr[2], env->active_tc.gpr[3],
                    env->active_tc.gpr[4], env->active_tc.gpr[5],
                    env->active_tc.gpr[6], env->active_tc.gpr[7]);
        break;
    case MAGIC_HELPER_DONE_FLAG:
        qemu_log_mask(CPU_LOG_INSTR, "ERROR: Attempted to call library function "
                                     "with success flag set in $v1!\n");
        do_raise_exception(env, EXCP_RI, GETPC());
    default:
        qemu_log_mask(CPU_LOG_INSTR, "ERROR: Attempted to call invalid library function "
                          TARGET_FMT_lx "\n", which);
        return;
    }
    // Indicate success by setting $v1 to 0xaffe
    env->active_tc.gpr[MIPS_REGNUM_V1] = MAGIC_HELPER_DONE_FLAG;
    return;
error:
    warn_report("%s: magic nop %d failed: \r\n"
                    "\tv0 = 0x" TARGET_FMT_lx "\tv1 = 0x" TARGET_FMT_lx "\r\n"
                    "\ta0 = 0x" TARGET_FMT_lx "\ta1 = 0x" TARGET_FMT_lx "\r\n"
                    "\ta2 = 0x" TARGET_FMT_lx "\ta3 = 0x" TARGET_FMT_lx "\r\n",
                    __func__, (int)(which & UINT32_MAX),
                    env->active_tc.gpr[MIPS_REGNUM_V0], env->active_tc.gpr[MIPS_REGNUM_V1],
                    env->active_tc.gpr[MIPS_REGNUM_A0], env->active_tc.gpr[MIPS_REGNUM_A1],
                    env->active_tc.gpr[MIPS_REGNUM_A2], env->active_tc.gpr[MIPS_REGNUM_A3]);
}
