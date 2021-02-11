/*
 *  Copyright (c) 2020 Lawrence Esswood
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

#pragma once

#include "cheri-archspecific-early.h"

extern const char *const cheri_gp_regnames[];
extern const char *const cheri_gp_int_regnames[];

#define CAP_TAG_GET_MANY_SHFT 2

// LETODO: All of these
#include "stdio.h"

static inline void QEMU_NORETURN
raise_cheri_exception_impl(CPUArchState *env, CheriCapExcCause cause,
                           unsigned regnum, bool instavail, uintptr_t hostpc)
{
    fprintf(stderr, "Exception. Reg %u. Cause %x\n", regnum, cause);
    assert(0);
}

static inline void QEMU_NORETURN raise_load_tag_exception(CPUArchState *env,
                                                          target_ulong va,
                                                          int cb,
                                                          uintptr_t retpc)
{
    assert(0);
}

static inline void QEMU_NORETURN raise_unaligned_load_exception(
    CPUArchState *env, target_ulong addr, uintptr_t retpc)
{
    assert(0);
}

static inline void QEMU_NORETURN raise_unaligned_store_exception(
    CPUArchState *env, target_ulong addr, uintptr_t retpc)
{
    assert(0);
}

static inline bool validate_jump_target(CPUARMState *env,
                                        const cap_register_t *target,
                                        target_ulong addr, unsigned regnum,
                                        uintptr_t retpc)
{
    return true;
}

#include "exec/helper-proto.h"

static inline void update_next_pcc_for_tcg(CPUARMState *env,
                                           const cap_register_t *target,
                                           uint32_t cjalr_flags)
{
    bool system_changed =
        ((env->pc.cr_perms ^ target->cr_perms) & CAP_ACCESS_SYS_REGS) != 0;
    bool executive_changed =
        ((env->pc.cr_perms ^ target->cr_perms) & CAP_PERM_EXECUTIVE) != 0;
    bool c64_changed =
        (target->_cr_cursor & 1) != ((env->pstate & PSTATE_C64) != 0);
    bool was_executive = (env->pc.cr_perms & CAP_PERM_EXECUTIVE);

    unsigned int cur_el;

    env->pc = *target;
    env->pc._cr_cursor &= ~1;

    if (executive_changed) {
        // Branches back to executive are allowed
        if (!(cjalr_flags & CJALR_CAN_BRANCH_RESTRICTED) && was_executive) {
            env->pc.cr_tag = 0;
        }
        cur_el = arm_current_el(env);
        aarch64_save_sp(env, cur_el);
    }

    // Switch C64. Branches will have already ended a translation block.
    if (c64_changed)
        env->pstate ^= PSTATE_C64;

    // Rebuild cached target-specific cheriflags if anything changed
    if (system_changed || executive_changed || c64_changed)
        arm_rebuild_chflags(env);

    // Executive change can result in a change of stack / DDC
    if (executive_changed)
        aarch64_restore_sp(env, cur_el);
}
