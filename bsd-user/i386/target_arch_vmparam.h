#ifndef _TARGET_ARCH_VMPARAM_H_
#define _TARGET_ARCH_VMPARAM_H_

#include "cpu.h"

/* compare to i386/include/vmparam.h */
#define TARGET_MAXTSIZ  (128UL*1024*1024)   /* max text size */
#define TARGET_DFLDSIZ  (128UL*1024*1024)   /* initial data size limit */
#define TARGET_MAXDSIZ  (512UL*1024*1024)   /* max data size */
#define TARGET_DFLSSIZ  (8UL*1024*1024)     /* initial stack size limit */
#define TARGET_MAXSSIZ  (64UL*1024*1024)    /* max stack size */
#define TARGET_SGROWSIZ (128UL*1024)        /* amount to grow stack */

#define TARGET_RESERVED_VA 0xf7000000

#define TARGET_USRSTACK (0xbfc00000)

static inline abi_ulong get_sp_from_cpustate(CPUX86State *state)
{
    return state->regs[R_ESP];
}

static inline void set_second_rval(CPUX86State *state, abi_ulong retval2)
{
    state->regs[R_EDX] = retval2;
}

#endif /* !_TARGET_ARCH_VMPARAM_H_ */
