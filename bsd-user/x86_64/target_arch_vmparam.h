#ifndef _TARGET_ARCH_VMPARAM_H_
#define _TARGET_ARCH_VMPARAM_H_

#include "cpu.h"

/* compare to amd64/include/vmparam.h */
#define TARGET_MAXTSIZ  (128UL*1024*1024)   /* max text size */
#define TARGET_DFLDSIZ  (32768UL*1024*1024) /* initial data size limit */
#define TARGET_MAXDSIZ  (32768UL*1024*1024) /* max data size */
#define TARGET_DFLSSIZ  (8UL*1024*1024)     /* initial stack size limit */
#define TARGET_MAXSSIZ  (512UL*1024*1024)   /* max stack size */
#define TARGET_SGROWSIZ (128UL*1024)        /* amount to grow stack */

#define TARGET_VM_MAXUSER_ADDRESS   (0x00007fffff000000UL)

#define TARGET_USRSTACK (TARGET_VM_MAXUSER_ADDRESS - TARGET_PAGE_SIZE)

static inline abi_ulong get_sp_from_cpustate(CPUX86State *state)
{
    return state->regs[R_ESP];
}

static inline void set_second_rval(CPUX86State *state, abi_ulong retval2)
{
    state->regs[R_EDX] = retval2;
}

#endif /* !_TARGET_ARCH_VMPARAM_H_ */
