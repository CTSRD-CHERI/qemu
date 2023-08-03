/*
 * ARM AArch64 specific CPU for bsd-user
 *
 * Copyright (c) 2015 Stacey D. Son <sson at Freebsd>
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

#include <stdio.h>
#include "target_arch.h"

/* See cpu_set_user_tls() in arm64/arm64/vm_machdep.c */
void target_cpu_set_tls(CPUARMState *env, abi_uintptr_t newtls)
{

#ifdef TARGET_CHERI
    cheri_load(&env->cp15.tpidr_el[0].cap, &newtls);
#else
    set_aarch_reg_to_x(&env->cp15.tpidr_el[0], newtls);
#endif
}

abi_uintptr_t target_cpu_get_tls(CPUARMState *env)
{
#ifdef TARGET_CHERI
    abi_uintptr_t tls;

    cheri_store(&tls, &env->cp15.tpidr_el[0].cap);
    return tls;
#else
    return get_aarch_reg_as_x(&env->cp15.tpidr_el[0]);
#endif
}
