
#ifndef _TARGET_ARCH_H_
#define _TARGET_ARCH_H_

#include "qemu.h"

void target_cpu_set_tls(CPUMIPSState *env, target_ulong newtls);
target_ulong target_cpu_get_tls(CPUMIPSState *env);

#endif /* !_TARGET_ARCH_H_ */
