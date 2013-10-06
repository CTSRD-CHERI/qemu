#ifndef _TARGET_ARCH_VMPARAM_H_
#define _TARGET_ARCH_VMPARAM_H_

#include "cpu.h"

#define TARGET_MAXTSIZ  (1*1024*1024*1024)  /* max text size */
#define TARGET_DFLDSIZ  (128*1024*1024)     /* initial data size limit */
#define TARGET_MAXDSIZ  (1*1024*1024*1024)  /* max data size */
#define TARGET_DFLSSIZ  (128*1024*1024)     /* initial stack size limit */
#define TARGET_MAXSSIZ  (1*1024*1024*1024)  /* max stack size */
#define TARGET_SGROWSIZ (128*1024)      /* amount to grow stack */

#define TARGET_RESERVED_VA 0xf7000000

/* XXX this may not be right */
#define TARGET_VM_MAXUSER_ADDRESS   (0xc0000000 - (512 * 1024 * 1024))
#define TARGET_USRSTACK (TARGET_VM_MAXUSER_ADDRESS - TARGET_PAGE_SIZE)

#ifndef UREG_I6
#define UREG_I6        6
#endif
#ifndef UREG_FP
#define UREG_FP        UREG_I6
#endif

static inline abi_ulong get_sp_from_cpustate(CPUSPARCState *state)
{
    return state->regwptr[UREG_FP];
}

static inline void set_second_rval(CPUSPARCState *state, abi_ulong retval2)
{
    state->regwptr[1] = retval2;
}

#endif /* !_TARGET_ARCH_VMPARAM_H_ */

