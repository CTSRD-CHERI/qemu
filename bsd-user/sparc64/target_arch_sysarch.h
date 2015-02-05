/*
 *  SPARC64 sysarch() system call emulation
 *
 *  Copyright (c) 2013-15 Stacey D. Son
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

#ifndef __ARCH_SYSARCH_H_
#define __ARCH_SYSARCH_H_

#include "target_syscall.h"
#include "qemu-bsd.h"

#ifndef TARGET_UT_MAX
#define TARGET_UT_MAX 36
#endif
#ifndef TARGET_UTH_NOCHANGE
#define TARGET_UTH_NOCHANGE -1
#endif

extern abi_ulong target_sparc_utrap_precise[TARGET_UT_MAX];
extern abi_ulong target_sparc_sigtramp;

struct target_sparc_utrap_args {
    int32_t     type;
    abi_ulong   new_precise;
    abi_ulong   new_deferred;
    abi_ulong   old_precise;
    abi_ulong   old_deferred;
};

struct target_sparc_utrap_install_args {
    int32_t     num;
    abi_ulong   handlers;
};

static abi_long target_sparc_utrap_install(abi_ulong target_args)
{
    struct target_sparc_utrap_install_args *uia;
    struct target_sparc_utrap_args *ua;
    int32_t i, num;
    abi_ulong handlers;

    if (!lock_user_struct(VERIFY_READ, uia, target_args, 0))
        return -TARGET_EFAULT;
    __get_user(num, &uia->num);
    __get_user(handlers, &uia->handlers);
    unlock_user_struct(uia, target_args, 0);

    if (num < 0 || num > TARGET_UT_MAX || (handlers == 0 && num > 0))
        return -TARGET_EINVAL;
    for (i = 0; i < num; i++) {
        int32_t type;
        abi_ulong new_precise, new_deferred, old_precise, old_deferred;

        if (!lock_user_struct(VERIFY_READ, ua, handlers +
                    (i * sizeof(struct target_sparc_utrap_args)), 0))
            return -TARGET_EFAULT;
        __get_user(type, &ua->type);
        __get_user(new_precise, &ua->new_precise);
        __get_user(new_deferred, &ua->new_deferred);
        __get_user(old_precise, &ua->old_precise);
        __get_user(old_deferred, &ua->old_deferred);
        unlock_user_struct(uia, target_args, 0);

        if (type != TARGET_UTH_NOCHANGE &&
                (type < 0 || type >= TARGET_UT_MAX))
            return -TARGET_EINVAL;
        if (old_deferred != 0) {
            if (put_user_ual(0, old_deferred))
                return -TARGET_EFAULT;
        }
        if (old_precise != 0) {
            if (__put_user(target_sparc_utrap_precise[type], &ua->old_precise))
                return -TARGET_EFAULT;
        }
        if (type != TARGET_UTH_NOCHANGE) {
            target_sparc_utrap_precise[type] = new_precise;
        }

        // fprintf(stderr, "sparc_utrap_install: type = %d\n", type);
    }
    return 0;
}

struct target_sparc_sigtramp_install_args {
    abi_ulong   sia_new;
    abi_ulong   sia_old;
};

static abi_long target_sparc_sigtramp_install(abi_ulong target_args)
{
    struct target_sparc_sigtramp_install_args *sia;
    abi_ulong sia_old_addr;

    if (!lock_user_struct(VERIFY_READ, sia, target_args, 0)) {
        return -TARGET_EFAULT;
    }
    __get_user(sia_old_addr, &sia->sia_old);
    if (sia_old_addr != 0) {
        if(put_user_ual(target_sparc_sigtramp, sia_old_addr))
            return -TARGET_EFAULT;
    }
    __get_user(target_sparc_sigtramp, &sia->sia_new);
    unlock_user_struct(sia, target_args, 0);

    return 0;
}

static inline abi_long do_freebsd_arch_sysarch(void *env, int op,
        abi_ulong params)
{
    abi_long ret = 0;

    switch (op) {
    case TARGET_SPARC_SIGTRAMP_INSTALL:
        // fprintf(stderr, "SPARC_SIGTRAMP_INSTALL %s: %s:%d\n",
        //        __func__, __FILE__, __LINE__);
        ret = target_sparc_sigtramp_install(params);
        break;

    case TARGET_SPARC_UTRAP_INSTALL:
        /* XXX not currently handled */
        // fprintf(stderr, "SPARC_UTRAP_INSTALL %s: %s:%d\n",
        //        __func__, __FILE__, __LINE__);
        ret = target_sparc_utrap_install(params);
        break;

    default:
        ret = -TARGET_EINVAL;
        break;
    }

    return ret;
}

static inline void do_freebsd_arch_print_sysarch(
        const struct syscallname *name, abi_long arg1, abi_long arg2,
        abi_long arg3, abi_long arg4, abi_long arg5, abi_long arg6)
{

    gemu_log("%s(%d, " TARGET_ABI_FMT_lx ", " TARGET_ABI_FMT_lx ", "
        TARGET_ABI_FMT_lx ")", name->name, (int)arg1, arg2, arg3, arg4);
}

#endif /*!__ARCH_SYSARCH_H_ */
