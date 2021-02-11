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

extern const char * const cheri_gp_regnames[];
extern const char * const cheri_gp_int_regnames[];

#define CAP_TAG_GET_MANY_SHFT    2

// LETODO: All of these
#include "stdio.h"

static inline void QEMU_NORETURN raise_cheri_exception_impl(
        CPUArchState *env, CheriCapExcCause cause, unsigned regnum,
bool instavail, uintptr_t hostpc)
{
    fprintf(stderr, "Exception. Reg %u. Cause %x\n", regnum, cause);
    assert(0);
}

static inline void QEMU_NORETURN raise_load_tag_exception(
        CPUArchState *env, target_ulong va, int cb, uintptr_t retpc)
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

static inline bool validate_cjalr_target(CPUARMState *env,
                                         const cap_register_t *target,
                                         unsigned regnum,
                                         uintptr_t retpc)
{
    return true;
}
