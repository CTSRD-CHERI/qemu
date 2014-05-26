/*
 *  powerpc system call definitions
 *
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
#ifndef _POWERPC_SYSCALL_H_
#define _POWERPC_SYSCALL_H_

/*
 * struct target_pt_regs defines the way the registers are stored on the stack
 * during a system call.
 */

struct target_pt_regs {
	abi_ulong gpr[32];
	abi_ulong lr;
	abi_ulong cr;
	abi_ulong xer;
	abi_ulong ctr;
	abi_ulong nip;
	abi_ulong srr1;
	abi_ulong exc;
	union {
		struct {
			abi_ulong dar;		/* Fault registers */
			abi_ulong dsisr;
		} aim;
		struct {
			abi_ulong dear;
			abi_ulong esr;
			abi_ulong dbcr0;
		} booke;
	} cpu;
};


#define UNAME_MACHINE			"powerpc"
#define TARGET_HW_MACHINE       "powerpc"
#if defined(TARGET_PPC64) && !defined(TARGET_ABI32)
#define TARGET_HW_MACHINE_ARCH  UNAME_MACHINE"64"
#else
#define TARGET_HW_MACHINE_ARCH  UNAME_MACHINE
#endif

#endif /* !_POWERPC_SYSCALL_H_ */
