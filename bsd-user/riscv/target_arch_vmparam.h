/*
 *  RISC-V VM parameters definitions
 *
 *  Copyright (c) 2019 Mark Corbin <mark.corbin@embecosm.com>
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
 
#ifndef _TARGET_ARCH_VMPARAM_H_
#define _TARGET_ARCH_VMPARAM_H_

#include "cpu.h"

#include "helper_utils.h"

#ifndef TARGET_VM_NRESERVLEVEL
#define TARGET_VM_NRESERVLEVEL  1
#endif

#ifndef TARGET_VM_LEVEL_0_ORDER
#define TARGET_VM_LEVEL_0_ORDER 9
#endif

/* Compare with riscv/include/vmparam.h */
#define TARGET_MAXTSIZ      (1*1024UL*1024*1024)   /* max text size */
#define TARGET_DFLDSIZ      (128UL*1024*1024)   /* initial data size limit */
#define TARGET_MAXDSIZ      (1*1024UL*1024*1024)    /* max data size */
#define TARGET_DFLSSIZ      (128UL*1024*1024)     /* initial stack size limit */
#define TARGET_MAXSSIZ      (1*1024UL*1024*1024)    /* max stack size */
#define TARGET_SGROWSIZ     (128UL*1024)        /* amount to grow stack */

#define TARGET_VM_MINUSER_ADDRESS   (0x0000000000000000UL)
#define TARGET_VM_MAXUSER_ADDRESS   (0x0000004000000000UL)

#define TARGET_SHAREDPAGE   (TARGET_VM_MAXUSER_ADDRESS - TARGET_PAGE_SIZE)

#ifdef TARGET_CHERI
#define TARGET_USRSTACK     (TARGET_SHAREDPAGE - (255 * TARGET_PAGE_SIZE))
#else
#define TARGET_USRSTACK     TARGET_SHAREDPAGE
#endif

static inline abi_ulong get_sp_from_cpustate(CPURISCVState *state)
{
    return gpr_int_value(state, xSP);
}

static inline void set_second_rval(CPURISCVState *state, abi_ulong retval2)
{
    gpr_set_int_value(state, xA1, retval2);
}

#endif  /* ! _TARGET_ARCH_VMPARAM_H_ */