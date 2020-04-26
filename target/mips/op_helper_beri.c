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
#include "exec/exec-all.h"
#include "exec/helper-proto.h"
#include "cpu.h"
#include "internal.h"

uint64_t helper_mfc0_rtc64(CPUMIPSState *env)
{
    if (!is_beri_or_cheri(env)) {
        qemu_maybe_log_instr_extra(env, "Error: Attempted to use BERI RTC64",
            " register for non-BERI CPU %s\n", env->cpu_model->name);
        do_raise_exception(env, EXCP_RI, GETPC());
    }
    return cpu_mips_get_rtc64(env);
}

void helper_mtc0_rtc64(CPUMIPSState *env, uint64_t arg1)
{
    if (!is_beri_or_cheri(env)) {
        qemu_maybe_log_instr_extra(env, "Error: Attempted to use BERI RTC64"
            " register for non-BERI CPU %s\n", env->cpu_model->name);
        do_raise_exception(env, EXCP_RI, GETPC());
    }
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
    if (!is_beri_or_cheri(env)) {
        qemu_maybe_log_instr_extra(env, "Error: Attempted to use BERI CoreID"
            " register for non-BERI CPU %s\n", env->cpu_model->name);
        do_raise_exception(env, EXCP_RI, GETPC());
    }
    CPUState *cs = env_cpu(env);

    return (uint32_t)(((cs->nr_cores - 1) << 16) |
        (cs->cpu_index & 0xffff));
}

target_ulong helper_rdhwr_statcounters_icount(CPUMIPSState *env, uint32_t sel)
{
    qemu_maybe_log_instr_extra(env, "%s\n", __func__);
    check_hwrena(env, 4, GETPC());
    switch (sel) {
    case 0: return env->statcounters_icount_user + env->statcounters_icount_kernel;
    case 1: return env->statcounters_icount_user;
    case 2: return env->statcounters_icount_kernel;
#ifdef TARGET_CHERI
    case 3: return env->statcounters_imprecise_setbounds;
    case 4: return env->statcounters_unrepresentable_caps;
#endif
    default: return 0xdeadbeef;
    }
}

#if defined(TARGET_CHERI)
target_ulong helper_rdhwr_statcounters_itlb_miss(CPUMIPSState *env)
{
    qemu_maybe_log_instr_extra(env, "%s\n", __func__);
    check_hwrena(env, 5, GETPC());
    return env->statcounters_itlb_miss;
}

target_ulong helper_rdhwr_statcounters_dtlb_miss(CPUMIPSState *env)
{
    qemu_maybe_log_instr_extra(env, "%s\n", __func__);
    check_hwrena(env, 6, GETPC());
    return env->statcounters_dtlb_miss;
}

target_ulong helper_rdhwr_statcounters_memory(CPUMIPSState *env, uint32_t sel)
{
    qemu_maybe_log_instr_extra(env, "%s(%d)\n", __func__, sel);
    check_hwrena(env, 11, GETPC());
    switch (sel) {
    case 2: return env->statcounters_icount_user;
    case 4: return env->statcounters_icount_kernel;
    case 8: return env->statcounters_cap_read;
    case 9: return env->statcounters_cap_write;
    case 10: return env->statcounters_cap_read_tagged;
    case 11: return env->statcounters_cap_write_tagged;
    default: return 0xdeadbeef;
    }
}

target_ulong helper_rdhwr_statcounters_reset(CPUMIPSState *env)
{
    // TODO: actually implement this
    qemu_maybe_log_instr_extra(env, "%s\n", __func__);
    check_hwrena(env, 7, GETPC());
    return 0;
}

target_ulong helper_rdhwr_statcounters_ignored(CPUMIPSState *env, uint32_t num)
{
    qemu_maybe_log_instr_extra(env, "%s\n", __func__);
    check_hwrena(env, num, GETPC());
    return 0xdeadbeef;
}
#endif
