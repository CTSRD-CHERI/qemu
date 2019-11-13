/*
 * hostdep.h : things which are dependent on the host architecture
 *
 *  * Written by Peter Maydell <peter.maydell@linaro.org>
 *
 * Copyright (C) 2016 Linaro Limited
 *
 * This work is licensed under the terms of the GNU GPL, version 2 or later.
 * See the COPYING file in the top-level directory.
 */

#ifndef AARCH64_HOSTDEP_H
#define AARCH64_HOSTDEP_H

/* We have a safe-syscall.inc.S */
#define HAVE_SAFE_SYSCALL

#ifndef __ASSEMBLER__

/* These are defined by the safe-syscall.inc.S file */
extern char safe_syscall_start[];
extern char safe_syscall_end[];

/* FreeBSD-ism */
#define DEFINE_PCREG(puc)	&((ucontext_t *)(puc))->uc_mcontext.mc_gpregs.gp_lr

/* Adjust the signal context to rewind out of safe-syscall if we're in it */
static inline void rewind_if_in_safe_syscall(void *puc)
{
    __u64 *pcreg = DEFINE_PCREG(puc);

    if (*pcreg > (uintptr_t)safe_syscall_start
        && *pcreg < (uintptr_t)safe_syscall_end) {
        *pcreg = (uintptr_t)safe_syscall_start;
    }
}

#endif /* __ASSEMBLER__ */

#endif
