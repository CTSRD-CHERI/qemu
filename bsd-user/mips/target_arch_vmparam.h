/*
 *  mips VM parameters definitions
 *
 *  Copyright (c) 2013 Stacey D. Son
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

/* compare to sys/mips/include/vmparam.h */
#define TARGET_MAXTSIZ      (128UL*1024*1024)   /* max text size */
#define TARGET_DFLDSIZ      (128UL*1024*1024)   /* initial data size limit */
#define TARGET_MAXDSIZ      (1*1024UL*1024*1024)    /* max data size */
#define TARGET_DFLSSIZ      (8UL*1024*1024)     /* initial stack size limit */
#define TARGET_MAXSSIZ      (64UL*1024*1024)    /* max stack size */
#define TARGET_SGROWSIZ     (128UL*1024)        /* amount to grow stack */

/* MIPS only supports 31 bits of virtual address space for user space */
#define TARGET_RESERVED_VA  0x77000000

#define TARGET_VM_MINUSER_ADDRESS   (0x00000000)
#define TARGET_VM_MAXUSER_ADDRESS   (0x80000000)

// #define TARGET_USRSTACK (TARGET_VM_MAXUSER_ADDRESS - TARGET_PAGE_SIZE)
#define TARGET_USRSTACK (TARGET_RESERVED_VA - TARGET_PAGE_SIZE * 0x10)

static inline abi_ulong get_sp_from_cpustate(CPUMIPSState *state)
{
    return state->active_tc.gpr[29];
}

static inline void set_second_rval(CPUMIPSState *state, abi_ulong retval2)
{
    state->active_tc.gpr[3] = retval2;
}

#endif  /* ! _TARGET_ARCH_VMPARAM_H_ */
