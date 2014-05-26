
#ifndef _TARGET_ARCH_SIGTRAMP_H_
#define _TARGET_ARCH_SIGTRAMP_H_

/* Compare to powerpc/powerpc/locore.S sigcode() */
static inline abi_long setup_sigtramp(abi_ulong offset, unsigned sigf_uc,
        unsigned sys_sigreturn)
{
    int i;
#if defined(TARGET_PPC64) && !defined(TARGET_ABI32)
    uint32_t sigtramp_code[TARGET_SZSIGCODE/TARGET_INSN_SIZE] = {
    /*  1 */ 0x3821FF90,   /* addi r1,r1,-112 */
    /*  2 */ 0x7C4802A6,   /* mflr r2 */
    /*  3 */ 0xE8020000,   /* ld r0,0(r2) */
    /*  4 */ 0xE8420008,   /* ld r2,8(r2) */
    /*  5 */ 0x7C0803A6,   /* mtlr r0 */
    /*  6 */ 0x4E800021,   /* blrl */
    /*  7 */ 0x38610070 + sigf_uc,   /* addi r3,r1,112+SF_UC */
    /*  8 */ 0x38000000 + sys_sigreturn, /* li r0,SYS_sigreturn */
    /*  9 */ 0x44000002,   /* sc */
    /* 10 */ 0x38000001,   /* li r0,1 */
    /* 11 */ 0x44000002,   /* sc */
    /* 12 */ 0x60000000    /* nop */
    };
#else
    uint32_t sigtramp_code[TARGET_SZSIGCODE/TARGET_INSN_SIZE] = {
    /* 1 */ 0x3821FFEC,   /* addi r1,r1,-20 */
    /* 2 */ 0x4E800021,   /* blrl */
    /* 3 */ 0x38610014 + sigf_uc,   /* addi r3,r1,20+SF_UC */
    /* 4 */ 0x38000000 + sys_sigreturn, /* li r0,SYS_sigreturn */
    /* 5 */ 0x44000002,   /* sc */
    /* 6 */ 0x38000001,   /* li r0,1 */
    /* 7 */ 0x44000002    /* sc */
    };
#endif

    for (i = 0; i < TARGET_SZSIGCODE/TARGET_INSN_SIZE; i++) {
        tswap32s(&sigtramp_code[i]);
    }

    return memcpy_to_target(offset, sigtramp_code, TARGET_SZSIGCODE);
}
#endif /* _TARGET_ARCH_SIGTRAMP_H_ */
