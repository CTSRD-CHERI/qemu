
#ifndef _TARGET_ARCH_SIGTRAMP_H_
#define _TARGET_ARCH_SIGTRAMP_H_

static inline abi_long setup_sigtramp(abi_ulong offset, unsigned sigf_uc,
        unsigned sys_sigreturn)
{

    return -TARGET_EOPNOTSUPP;
}
#endif /* _TARGET_ARCH_SIGTRAMP_H_ */
