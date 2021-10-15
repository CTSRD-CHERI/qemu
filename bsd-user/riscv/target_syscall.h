/*
 *  RISC-V system call definitions
 *
 *  Copyright (c) Mark Corbin <mark.corbin@embecosm.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __ARCH_SYSCALL_H_
#define __ARCH_SYSCALL_H_

/*
 * struct target_pt_regs defines the way the registers are stored on the stack
 * during a system call.
 */

struct target_pt_regs {
#ifdef TARGET_CHERI
    cap_register_t regs[32];
    cap_register_t sepc;
#else
    abi_ulong regs[32];
    abi_ulong sepc;
#endif
};

#define UNAME_MACHINE "riscv64"

#define TARGET_HW_MACHINE       "riscv64"
#define TARGET_HW_MACHINE_ARCH  UNAME_MACHINE

#endif /* !__ARCH_SYSCALL_H_ */
