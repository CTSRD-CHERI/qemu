#ifndef _TARGET_OS_STACK_H_
#define _TARGET_OS_STACK_H_

#include "target_arch_sigtramp.h"

static inline int setup_initial_stack(struct bsd_binprm *bprm, abi_ulong *p)
{
    int i;
    abi_ulong stack_base;

    stack_base = (target_stkbas + target_stksiz) -
                  MAX_ARG_PAGES * TARGET_PAGE_SIZE;
    if (p) {
        *p = stack_base;
    }

    for (i = 0; i < MAX_ARG_PAGES; i++) {
        if (bprm->page[i]) {
            info->rss++;
            if (!memcpy_to_target(stack_base, bprm->page[i],
                        TARGET_PAGE_SIZE)) {
                errno = EFAULT;
                return -1;
            }
            g_free(bprm->page[i]);
        }
        stack_base += TARGET_PAGE_SIZE;
    }

    return 0;
}

#endif /* !_TARGET_OS_STACK_H_ */
