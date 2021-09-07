#ifndef TARGET_RISCV_CPU_USER_H
#define TARGET_RISCV_CPU_USER_H

#define xNULL 0   /* NULL pointer */
#define xRA   1   /* return address (aka link register) */
#define xSP   2   /* stack pointer */
#define xGP   3   /* global pointer */
#define xTP   4   /* thread pointer */

/*
 * Temporary registers.
 */
#define xT0   5   /* temporary/alternate link register */
#define xT1   6
#define xT2   7
#define xT3   28
#define xT4   29
#define xT5   30
#define xT6   31

/*
 * Saved registers.
 */
#define xS0   8   /* saved register/frame pointer */
#define xS1   9
#define xS2   18
#define xS3   19
#define xS4   20
#define xS5   21
#define xS6   22
#define xS7   23
#define xS8   24
#define xS9   25
#define xS10  26
#define xS11  27

/*
 * Function arguments.
 */
#define xA0   10  /* function argument/return value */
#define xA1   11  /* function argument/return value */
#define xA2   12
#define xA3   13
#define xA4   14
#define xA5   15
#define xA6   16
#define xA7   17  /* syscall number for RVI ABI */

#endif
