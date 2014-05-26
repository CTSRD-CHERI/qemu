/*
 *  powerpc VM parameters definitions
 *
 *  Copyright (c) 2014 Justin Hibbits
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

/* compare to sys/powerpc/include/vmparam.h */
#define TARGET_MAXTSIZ      (64UL*1024*1024)   /* max text size */
#define TARGET_DFLDSIZ      (128UL*1024*1024)   /* initial data size limit */
#define TARGET_MAXDSIZ      (1UL*1024*1024*1024)    /* max data size */
#define TARGET_DFLSSIZ      (8UL*1024*1024)     /* initial stack size limit */
#define TARGET_MAXSSIZ      (64UL*1024*1024)    /* max stack size */
#define TARGET_SGROWSIZ     (128UL*1024)        /* amount to grow stack */

#ifndef TARGET_PPC64
#define TARGET_RESERVED_VA  0xfffff000
#endif

                /* KERNBASE - 512 MB */
#if defined(TARGET_PPC64) && !defined(TARGET_ABI32)
#define TARGET_VM_MAXUSER_ADDRESS	0x00007fffff000000ULL
#else
#define TARGET_VM_MAXUSER_ADDRESS   0xfffff000UL
#endif
#define TARGET_USRSTACK             (TARGET_VM_MAXUSER_ADDRESS - TARGET_PAGE_SIZE * 0x10)

static inline abi_ulong get_sp_from_cpustate(CPUPPCState *state)
{
    return state->gpr[1]; /* sp */
}

static inline void set_second_rval(CPUPPCState *state, abi_ulong retval2)
{
    state->gpr[4] = retval2;
}

#endif  /* ! _TARGET_ARCH_VMPARAM_H_ */
