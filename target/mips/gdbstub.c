/*
 * MIPS gdb server stub
 *
 * Copyright (c) 2003-2005 Fabrice Bellard
 * Copyright (c) 2013 SUSE LINUX Products GmbH
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
#include "qemu-common.h"
#include "cpu.h"
#include "internal.h"
#include "exec/gdbstub.h"

int mips_cpu_gdb_read_register(CPUState *cs, uint8_t *mem_buf, int n)
{
    MIPSCPU *cpu = MIPS_CPU(cs);
    CPUMIPSState *env = &cpu->env;

    if (n < 32) {
        return gdb_get_regl(mem_buf, env->active_tc.gpr[n]);
    }
    if (env->CP0_Config1 & (1 << CP0C1_FP) && n >= 38 && n < 72) {
        switch (n) {
        case 70:
            return gdb_get_regl(mem_buf, (int32_t)env->active_fpu.fcr31);
        case 71:
            return gdb_get_regl(mem_buf, (int32_t)env->active_fpu.fcr0);
        default:
            if (env->CP0_Status & (1 << CP0St_FR)) {
                return gdb_get_regl(mem_buf,
                    env->active_fpu.fpr[n - 38].d);
            } else {
                return gdb_get_regl(mem_buf,
                    env->active_fpu.fpr[n - 38].w[FP_ENDIAN_IDX]);
            }
        }
    }
    switch (n) {
    case 32:
        return gdb_get_regl(mem_buf, (int32_t)env->CP0_Status);
    case 33:
        return gdb_get_regl(mem_buf, env->active_tc.LO[0]);
    case 34:
        return gdb_get_regl(mem_buf, env->active_tc.HI[0]);
    case 35:
        return gdb_get_regl(mem_buf, env->CP0_BadVAddr);
    case 36:
        return gdb_get_regl(mem_buf, (int32_t)env->CP0_Cause);
    case 37:
        return gdb_get_regl(mem_buf, env->active_tc.PC |
                                     !!(env->hflags & MIPS_HFLAG_M16));
    }

    return 0;
}

int mips_cpu_gdb_write_register(CPUState *cs, uint8_t *mem_buf, int n)
{
    MIPSCPU *cpu = MIPS_CPU(cs);
    CPUMIPSState *env = &cpu->env;
    target_ulong tmp;

    tmp = ldtul_p(mem_buf);

    if (n < 32) {
        env->active_tc.gpr[n] = tmp;
        return sizeof(target_ulong);
    }
    if (env->CP0_Config1 & (1 << CP0C1_FP) && n >= 38 && n < 72) {
        switch (n) {
        case 70:
            env->active_fpu.fcr31 = (tmp & env->active_fpu.fcr31_rw_bitmask) |
                  (env->active_fpu.fcr31 & ~(env->active_fpu.fcr31_rw_bitmask));
            restore_fp_status(env);
            break;
        case 71:
            /* FIR is read-only.  Ignore writes.  */
            break;
        default:
            if (env->CP0_Status & (1 << CP0St_FR)) {
                env->active_fpu.fpr[n - 38].d = tmp;
            } else {
                env->active_fpu.fpr[n - 38].w[FP_ENDIAN_IDX] = tmp;
            }
            break;
        }
        return sizeof(target_ulong);
    }
    switch (n) {
    case 32:
#ifndef CONFIG_USER_ONLY
        cpu_mips_store_status(env, tmp);
#endif
        break;
    case 33:
        env->active_tc.LO[0] = tmp;
        break;
    case 34:
        env->active_tc.HI[0] = tmp;
        break;
    case 35:
        env->CP0_BadVAddr = tmp;
        break;
    case 36:
#ifndef CONFIG_USER_ONLY
        cpu_mips_store_cause(env, tmp);
#endif
        break;
    case 37:
        env->active_tc.PC = tmp & ~(target_ulong)1;
        if (tmp & 1) {
            env->hflags |= MIPS_HFLAG_M16;
        } else {
            env->hflags &= ~(MIPS_HFLAG_M16);
        }
        break;
    default:
        if (n > 72) {
            return 0;
        }
        /* Other registers are readonly.  Ignore writes.  */
        break;
    }

    return sizeof(target_ulong);
}

int mips_gdb_get_sys_reg(CPUMIPSState *env, uint8_t *mem_buf, int n)
{
    if (n == 0)
        return gdb_get_regl(mem_buf, (int32_t)env->CP0_PRid);

    return 0;
}

int mips_gdb_set_sys_reg(CPUMIPSState *env, uint8_t *mem_buf, int n)
{
    /* System registers are readonly.  Ignore writes.  */
    if (n == 0)
	return sizeof(target_ulong);

    return 0;
}

#if defined(TARGET_CHERI)
static int gdb_get_capreg(uint8_t *mem_buf, cap_register_t *cap)
{
#ifdef CHERI_128
    stq_p(mem_buf, cap->cr_pesbt);
    stq_p(mem_buf + 8, cap->cr_base + cap->cr_offset);
    return 16;
#elif defined(CHERI_MAGIC128)
    /* XXX: Would need to generate pesbt. */
    stq_p(mem_buf, 0);
    stq_p(mem_buf + 8, cap->cr_base + cap->cr_offset);
    return 16;
#else
    target_ulong ret;
    uint64_t perms;

    perms = (uint64_t)(((cap->cr_uperms & CAP_UPERMS_ALL) << CAP_UPERMS_SHFT) |
        (cap->cr_perms & CAP_PERMS_ALL));

    ret = ((uint64_t)cap->cr_otype << 32) |
        (perms << 1) | (cap->cr_sealed ? 1UL : 0UL);
	
    stq_p(mem_buf, ret);
    stq_p(mem_buf + 8, cap->cr_base + cap->cr_offset);
    stq_p(mem_buf + 16, cap->cr_base);
    stq_p(mem_buf + 24, cap->cr_length);
    return 32;
#endif
}

int mips_gdb_get_cheri_reg(CPUMIPSState *env, uint8_t *mem_buf, int n)
{
    if (n < 32)
	return gdb_get_capreg(mem_buf, &env->active_tc._CGPR[n]);
    switch (n) {
    case 32:
	return gdb_get_capreg(mem_buf, &env->active_tc.PCC);
    case 33:
	return gdb_get_regl(mem_buf, env->CP2_CapCause);
    case 34: {
	uint64_t cap_valid;
	int i;

	cap_valid = 0;
	for (i = 0; i < 32; i++) {
	    if (env->active_tc._CGPR[i].cr_tag)
		cap_valid |= ((uint64_t)1 << i);
	}
	if (env->active_tc.PCC.cr_tag)
	    cap_valid |= ((uint64_t)1 << 32);
	return gdb_get_regl(mem_buf, cap_valid);
    }
    }

    return 0;
}

int mips_gdb_set_cheri_reg(CPUMIPSState *env, uint8_t *mem_buf, int n)
{
    /* All CHERI registers are read-only currently.  */
    if (n < 33)
#if defined(CHERI_128) || defined(CHERI_MAGIC128)
	return 16;
#else
        return 32;
#endif
    if (n == 33 || n == 34)
	return 8;

    return 0;
}
#endif
