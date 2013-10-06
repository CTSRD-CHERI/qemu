#ifndef _TARGET_OS_VMPARAM_H_
#define _TARGET_OS_VMPARAM_H_

#include "target_arch_vmparam.h"

#define TARGET_SPACE_USRSPACE   4096
#define TARGET_ARG_MAX          262144

/* Compare to sys/exec.h */
struct target_ps_strings {
    abi_ulong ps_argvstr;
    uint32_t ps_nargvstr;
    abi_ulong ps_envstr;
    uint32_t ps_nenvstr;
};

extern abi_ulong target_stkbas;
extern abi_ulong target_stksiz;

#define TARGET_PS_STRINGS  ((target_stkbas + target_stksiz) - \
		sizeof(struct target_ps_strings))

#endif /* !TARGET_OS_VMPARAM_H_ */
