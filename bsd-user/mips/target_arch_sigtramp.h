
#ifndef _TARGET_ARCH_SIGTRAMP_H_
#define _TARGET_ARCH_SIGTRAMP_H_

/* Compare to mips/mips/locore.S sigcode() */
static inline abi_long setup_sigtramp(abi_ulong offset, unsigned sigf_uc,
        unsigned sys_sigreturn)
{
    int i;
    uint32_t sigtramp_code[TARGET_SZSIGCODE/TARGET_INSN_SIZE] = {
    /* 1 */ 0x27A40000 + sigf_uc,       /* addu   $a0, $sp, (sigf_uc) */
    /* 2 */ 0x24020000 + sys_sigreturn, /* li      $v0, (sys_sigreturn) */
    /* 3 */ 0x0000000C,                 /* syscall */
    /* 4 */ 0x0000000D                  /* break */
    };

    for (i = 0; i < 4; i++) {
        tswap32s(&sigtramp_code[i]);
    }

    return memcpy_to_target(offset, sigtramp_code, TARGET_SZSIGCODE);
}
#endif /* _TARGET_ARCH_SIGTRAMP_H_ */
