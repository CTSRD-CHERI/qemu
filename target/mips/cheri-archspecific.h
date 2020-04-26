/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2015-2016 Stacey Son <sson@FreeBSD.org>
 * Copyright (c) 2016-2018 Alfredo Mazzinghi <am2419@cl.cam.ac.uk>
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

#include "cheri_defs.h"
#include "internal.h"

static inline const char* cheri_cause_str(CheriCapExcCause cause);

extern bool cheri_debugger_on_trap;

static inline QEMU_NORETURN void do_raise_c2_exception_impl(CPUMIPSState *env,
                                                            uint16_t cause,
                                                            uint16_t reg,
                                                            uintptr_t hostpc)
{
    if (qemu_log_instr_or_mask_enabled(env, CPU_LOG_INT)) {
        qemu_log_instr_or_mask_msg(env, CPU_LOG_INT,
            "C2 EXCEPTION: cause=%d(%s) reg=%d PCC=" PRINT_CAP_FMTSTR
            " -> host PC: 0x%jx\n", cause, cheri_cause_str(cause), reg,
            PRINT_CAP_ARGS(cheri_get_current_pcc_fetch_from_tcg(env, hostpc)),
            (uintmax_t)hostpc);
    }
#ifdef DEBUG_KERNEL_CP2_VIOLATION
    if (in_kernel_mode(env)) {
        // Print some debug information for CheriBSD kernel crashes
        error_report("C2 EXCEPTION: cause=%d(%s) reg=%d\r", cause, cheri_fault_str(cause), reg);
        if (reg < 32) {
            const cap_register_t* cr = get_readonly_capreg(env, reg);
            error_report("Caused by: "PRINT_CAP_FMTSTR "\r", PRINT_CAP_ARGS(cr));
        }
        char buf[4096];
        FILE* buf_file = fmemopen(buf, sizeof(buf), "w");
        cheri_dump_state(env_cpu(env), buf_file, fprintf, CPU_DUMP_CODE);
        error_report("%s\r\n", buf);
        sleep(1); // to flush the write buffer
        fclose(buf_file);
    }
#endif
    cpu_mips_store_capcause(env, reg, cause);
    // Allow drop into debugger on first CHERI trap:
    // FIXME: allow c command to work by adding another boolean flag to skip
    // this breakpoint when GDB asks to continue
    if (cheri_debugger_on_trap)
        do_raise_exception(env, EXCP_DEBUG, hostpc);
    do_raise_exception(env, EXCP_C2E, hostpc);
}

static inline QEMU_NORETURN void do_raise_c2_exception_noreg(CPUMIPSState *env, uint16_t cause, uintptr_t pc)
{
    do_raise_c2_exception_impl(env, cause, 0xff, pc);
}


static inline void QEMU_NORETURN raise_cheri_exception_impl(
    CPUArchState *env, CheriCapExcCause cause, unsigned regnum,
    bool instavail, uintptr_t hostpc)
{
    if (!instavail)
        env->error_code |= EXCP_INST_NOTAVAIL;
    do_raise_c2_exception_impl(env, cause, regnum, hostpc);
}

static inline bool
cheri_tag_prot_clear_or_trap(CPUMIPSState *env, target_ulong va,
                             int cb, const cap_register_t* cbp,
                             int prot, uintptr_t retpc, target_ulong tag)
{
    if (tag && ((prot & PAGE_LC_CLEAR) || !(cbp->cr_perms & CAP_PERM_LOAD_CAP))) {
        qemu_maybe_log_instr_extra(env, "Clearing tag loaded from " TARGET_FMT_lx
            " due to %s\n", va, prot & PAGE_LC_CLEAR ?
            "asserted TLB_LI" : "missing CAP_PERM_LOAD_CAP");
        return 0;
    }
    if (tag && (prot & PAGE_LC_TRAP)) {
      env->CP0_BadVAddr = va;
      do_raise_c2_exception_impl(env, CapEx_CapLoadGen, cb, retpc);
    }
    return tag;
}

static inline void QEMU_NORETURN raise_unaligned_load_exception(
    CPUArchState *env, target_ulong addr, uintptr_t retpc)
{
    do_raise_c0_exception_impl(env, EXCP_AdEL, addr, retpc);
}

static inline void QEMU_NORETURN raise_unaligned_store_exception(
    CPUArchState *env, target_ulong addr, uintptr_t retpc)
{
    do_raise_c0_exception_impl(env, EXCP_AdES, addr, retpc);
}

static inline bool validate_cjalr_target(CPUMIPSState *env,
                                         const cap_register_t *target,
                                         unsigned regnum,
                                         uintptr_t retpc)
{
    if (!QEMU_IS_ALIGNED(cap_get_cursor(target), 4)) {
        do_raise_c0_exception_impl(env, EXCP_AdEL, cap_get_cursor(target),
                                   retpc);
    }
    return true;
}
