/*
 *  powerpc cpu related code
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, see <http://www.gnu.org/licenses/>.
 */
#include "target_arch.h"

static inline uint64_t cpu_ppc_get_tb(CPUPPCState *env)
{
    /* TO FIX */
    return 0;
}

uint64_t cpu_ppc_load_tbl(CPUPPCState *env)
{
    return cpu_ppc_get_tb(env);
}

uint32_t cpu_ppc_load_tbu(CPUPPCState *env)
{
    return cpu_ppc_get_tb(env) >> 32;
}

uint64_t cpu_ppc_load_atbl(CPUPPCState *env)
{
    return cpu_ppc_get_tb(env);
}

uint32_t cpu_ppc_load_atbu(CPUPPCState *env)
{
    return cpu_ppc_get_tb(env) >> 32;
}

uint32_t cpu_ppc601_load_rtcu(CPUPPCState *env)
__attribute__ (( alias ("cpu_ppc_load_tbu") ));

uint32_t cpu_ppc601_load_rtcl(CPUPPCState *env)
{
    return cpu_ppc_load_tbl(env) & 0x3FFFFF80;
}

/* XXX: to be fixed */
int ppc_dcr_read (ppc_dcr_t *dcr_env, int dcrn, uint32_t *valp)
{
    return -1;
}

int ppc_dcr_write (ppc_dcr_t *dcr_env, int dcrn, uint32_t val)
{
    return -1;
}
