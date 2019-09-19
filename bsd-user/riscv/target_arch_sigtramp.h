/*
 * RISC-V sigcode
 *
 * Copyright (c) 2019 Mark Corbin <mark.corbin@embecosm.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _TARGET_ARCH_SIGTRAMP_H_
#define _TARGET_ARCH_SIGTRAMP_H_

/* Compare with sigcode() in riscv/riscv/locore.S */
static inline abi_long setup_sigtramp(abi_ulong offset, unsigned sigf_uc,
        unsigned sys_sigreturn)
{
    int i;
    uint32_t sys_exit = TARGET_FREEBSD_NR_exit;

    uint32_t sigtramp_code[] = {
    /* 1 */ 0x00010513,                         /* mv a0, sp */
    /* 2 */ 0x00050513 + (sigf_uc << 20),       /* addi a0, a0, sigf_uc */
    /* 3 */ 0x00000293 + (sys_sigreturn << 20), /* li t0, sys_sigreturn */
    /* 4 */ 0x00000073,                         /* ecall */
    /* 5 */ 0x00000293 + (sys_exit << 20),      /* li t0, sys_exit */
    /* 6 */ 0x00000073,                         /* ecall */
    /* 7 */ 0xFF1FF06F                          /* b -16 */
    };

    for (i = 0; i < 7; i++) {
        tswap32s(&sigtramp_code[i]);
    }

    return memcpy_to_target(offset, sigtramp_code, TARGET_SZSIGCODE);
}
#endif /* _TARGET_ARCH_SIGTRAMP_H_ */
