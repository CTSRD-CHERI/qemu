/*
 *  miscellaneous OpenBSD system call shims
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
#ifndef __OS_MISC_H_
#define __OS_MISC_H_

/*
 * XXX To support FreeBSD binaries on OpenBSD these syscalls will need
 * to be emulated.
 */

/* sched_setparam(2) */
static inline abi_long do_freebsd_sched_setparam(pid_t pid,
        abi_ulong target_sp_addr)
{

    qemu_log("qemu: Unsupported syscall sched_setparam()\n");
    return -TARGET_ENOSYS;
}

/* sched_get_param(2) */
static inline abi_long do_freebsd_sched_getparam(pid_t pid,
        abi_ulong target_sp_addr)
{

    qemu_log("qemu: Unsupported syscall sched_getparam()\n");
    return -TARGET_ENOSYS;
}

/* sched_setscheduler(2) */
static inline abi_long do_freebsd_sched_setscheduler(pid_t pid, int policy,
        abi_ulong target_sp_addr)
{

    qemu_log("qemu: Unsupported syscall sched_setscheduler()\n");
    return -TARGET_ENOSYS;
}

/* sched_getscheduler(2) */
static inline abi_long do_freebsd_sched_getscheduler(pid_t pid)
{

    qemu_log("qemu: Unsupported syscall sched_getscheduler()\n");
    return -TARGET_ENOSYS;
}

/* sched_getscheduler(2) */
static inline abi_long do_freebsd_sched_rr_get_interval(pid_t pid,
        abi_ulong target_ts_addr)
{

    qemu_log("qemu: Unsupported syscall sched_rr_get_interval()\n");
    return -TARGET_ENOSYS;
}

/* cpuset(2) */
static inline abi_long do_freebsd_cpuset(abi_ulong target_cpuid)
{

    qemu_log("qemu: Unsupported syscall cpuset()\n");
    return -TARGET_ENOSYS;
}

/* cpuset_setid(2) */
static inline abi_long do_freebsd_cpuset_setid(void *cpu_env, abi_long arg1,
        abi_ulong arg2, abi_ulong arg3, abi_ulong arg4, abi_ulong arg5)
{

    qemu_log("qemu: Unsupported syscall cpuset_setid()\n");
    return -TARGET_ENOSYS;
}

/* cpuset_getid(2) */
static inline abi_long do_freebsd_cpuset_getid(abi_long arg1, abi_ulong arg2,
        abi_ulong arg3, abi_ulong arg4, abi_ulong arg5)
{

    qemu_log("qemu: Unsupported syscall cpuset_getid()\n");
    return -TARGET_ENOSYS;
}

/* cpuset_getaffinity(2) */
static inline abi_long do_freebsd_cpuset_getaffinity(abi_long arg1,
        abi_ulong arg2, abi_ulong arg3, abi_ulong arg4, abi_ulong arg5,
        abi_ulong arg6)
{

    qemu_log("qemu: Unsupported syscall cpuset_getaffinity()\n");
    return -TARGET_ENOSYS;
}

/* cpuset_setaffinity(2) */
static inline abi_long do_freebsd_cpuset_setaffinity(abi_long arg1,
        abi_ulong arg2, abi_ulong arg3, abi_ulong arg4, abi_ulong arg5,
        abi_ulong arg6)
{

    qemu_log("qemu: Unsupported syscall cpuset_setaffinity()\n");
    return -TARGET_ENOSYS;
}


/* modfnext(2) */
static inline abi_long do_freebsd_modfnext(abi_long modid)
{

    qemu_log("qemu: Unsupported syscall modfnext()\n");
    return -TARGET_ENOSYS;
}

/* modfind(2) */
static inline abi_long do_freebsd_modfind(abi_ulong target_name)
{

    qemu_log("qemu: Unsupported syscall modfind()\n");
    return -TARGET_ENOSYS;
}

/* kldload(2) */
static inline abi_long do_freebsd_kldload(abi_ulong target_name)
{

    qemu_log("qemu: Unsupported syscall kldload()\n");
    return -TARGET_ENOSYS;
}

/* kldunload(2) */
static inline abi_long do_freebsd_kldunload(abi_long fileid)
{

    qemu_log("qemu: Unsupported syscall kldunload()\n");
    return -TARGET_ENOSYS;
}

/* kldunloadf(2) */
static inline abi_long do_freebsd_kldunloadf(abi_long fileid, abi_long flags)
{

    qemu_log("qemu: Unsupported syscall kldunloadf()\n");
    return -TARGET_ENOSYS;
}

/* kldfind(2) */
static inline abi_long do_freebsd_kldfind(abi_ulong target_name)
{

    qemu_log("qemu: Unsupported syscall kldfind()\n");
    return -TARGET_ENOSYS;
}

/* kldnext(2) */
static inline abi_long do_freebsd_kldnext(abi_long fileid)
{

    qemu_log("qemu: Unsupported syscall kldnext()\n");
    return -TARGET_ENOSYS;
}


/* kldstat(2) */
static inline abi_long do_freebsd_kldstat(abi_long fileid,
        abi_ulong target_stat)
{

    qemu_log("qemu: Unsupported syscall kldstat()\n");
    return -TARGET_ENOSYS;
}

/* kldfirstmod(2) */
static inline abi_long do_freebsd_kldfirstmod(abi_long fileid)
{

    qemu_log("qemu: Unsupported syscall kldfirstmod()\n");
    return -TARGET_ENOSYS;
}

/* kldsym(2) */
static inline abi_long do_freebsd_kldsym(abi_long fileid, abi_long cmd,
        abi_ulong target_data)
{

    qemu_log("qemu: Unsupported syscall kldsym()\n");
    return -TARGET_ENOSYS;
}

/*
 * Resource limits (undocumented except for rctl(8) and rctl.conf(5) )
 */
/* rctl_get_racct() */
static inline abi_long do_freebsd_rctl_get_racct(abi_ulong target_inbufp,
        abi_ulong inbuflen, abi_ulong target_outbufp, abi_ulong outbuflen)
{

    qemu_log("qemu: Unsupported syscall rctl_get_racct()\n");
    return -TARGET_ENOSYS;
}

/* rctl_get_rules() */
static inline abi_long do_freebsd_rctl_get_rules(abi_ulong target_inbufp,
        abi_ulong inbuflen, abi_ulong target_outbufp, abi_ulong outbuflen)
{

    qemu_log("qemu: Unsupported syscall rctl_get_rules()\n");
    return -TARGET_ENOSYS;
}

/* rctl_add_rule() */
static inline abi_long do_freebsd_rctl_add_rule(abi_ulong target_inbufp,
        abi_ulong inbuflen, abi_ulong target_outbufp, abi_ulong outbuflen)
{

    qemu_log("qemu: Unsupported syscall rctl_add_rule()\n");
    return -TARGET_ENOSYS;
}

/* rctl_remove_rule() */
static inline abi_long do_freebsd_rctl_remove_rule(abi_ulong target_inbufp,
        abi_ulong inbuflen, abi_ulong target_outbufp, abi_ulong outbuflen)
{

    qemu_log("qemu: Unsupported syscall rctl_remove_rule()\n");
    return -TARGET_ENOSYS;
}

/* rctl_get_limits() */
static inline abi_long do_freebsd_rctl_get_limits(abi_ulong target_inbufp,
        abi_ulong inbuflen, abi_ulong target_outbufp, abi_ulong outbuflen)
{

    qemu_log("qemu: Unsupported syscall rctl_get_limits()\n");
    return -TARGET_ENOSYS;
}

/*
 * Kernel environment
 */

/* kenv(2) */
static inline abi_long do_freebsd_kenv(abi_long what, abi_ulong target_name,
        abi_ulong target_value, abi_long len)
{

    qemu_log("qemu: Unsupported syscall kenv()\n");
    return -TARGET_ENOSYS;
}


/*
 *  Mandatory Access Control
 */

/* __mac_get_proc */
static inline abi_long do_freebsd___mac_get_proc(abi_ulong target_mac)
{

    qemu_log("qemu: Unsupported syscall mac_get_proc()\n");
    return -TARGET_ENOSYS;
}

/* __mac_set_proc */
static inline abi_long do_freebsd___mac_set_proc(abi_ulong target_mac)
{

    qemu_log("qemu: Unsupported syscall mac_set_proc()\n");
    return -TARGET_ENOSYS;
}


/* __mac_get_fd */
static inline abi_long do_freebsd___mac_get_fd(abi_long fd,
        abi_ulong target_mac)
{

    qemu_log("qemu: Unsupported syscall mac_get_fd()\n");
    return -TARGET_ENOSYS;
}

/* __mac_set_fd */
static inline abi_long do_freebsd___mac_set_fd(abi_long fd,
        abi_ulong target_mac)
{

    qemu_log("qemu: Unsupported syscall mac_set_fd()\n");
    return -TARGET_ENOSYS;
}

/* __mac_get_file */
static inline abi_long do_freebsd___mac_get_file(abi_ulong target_path,
        abi_ulong target_mac)
{

    qemu_log("qemu: Unsupported syscall mac_get_file()\n");
    return -TARGET_ENOSYS;
}

/* __mac_set_file */
static inline abi_long do_freebsd___mac_set_file(abi_ulong target_path,
        abi_ulong target_mac)
{

    qemu_log("qemu: Unsupported syscall mac_set_file()\n");
    return -TARGET_ENOSYS;
}

/* __mac_get_link */
static inline abi_long do_freebsd___mac_get_link(abi_ulong target_path,
        abi_ulong target_mac)
{

    qemu_log("qemu: Unsupported syscall mac_get_link()\n");
    return -TARGET_ENOSYS;
}

/* __mac_set_link */
static inline abi_long do_freebsd___mac_set_link(abi_ulong target_path,
        abi_ulong target_mac)
{

    qemu_log("qemu: Unsupported syscall mac_set_link()\n");
    return -TARGET_ENOSYS;
}

/* mac_syscall */
static inline abi_long do_freebsd_mac_syscall(abi_ulong target_policy,
        abi_long call, abi_ulong target_arg)
{

    qemu_log("qemu: Unsupported syscall mac_syscall()\n");
    return -TARGET_ENOSYS;
}


/*
 * New posix calls
 */
/* posix_fallocate(2) */
static inline abi_long do_freebsd_posix_fallocate(abi_long fd, abi_ulong offset,
        abi_ulong len)
{

    qemu_log("qemu: Unsupported syscall posix_fallocate()\n");
    return -TARGET_ENOSYS;
}

/* posix_openpt(2) */
static inline abi_long do_freebsd_posix_openpt(abi_long flags)
{

    qemu_log("qemu: Unsupported syscall posix_openpt()\n");
    return -TARGET_ENOSYS;
}

/* posix_fadvise(2) */
static inline abi_long do_freebsd_posix_fadvise(abi_long fd, abi_ulong offset,
        abi_ulong len, abi_long advise)
{

    qemu_log("qemu: Unsupported syscall posix_fadvise()\n");
    return -TARGET_ENOSYS;
}

#endif /* ! __OS_MISC_H_ */
