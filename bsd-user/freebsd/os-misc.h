/*
 *  miscellaneous FreeBSD system call shims
 *
 *  Copyright (c) 2013-14 Stacey D. Son
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

#include <sys/cpuset.h>
#include <sys/random.h>
#include <sched.h>

/* sched_setparam(2) */
static inline abi_long do_freebsd_sched_setparam(pid_t pid,
        abi_ulong target_sp_addr)
{
    abi_long ret;
    struct sched_param host_sp;

    ret = get_user_s32(host_sp.sched_priority, target_sp_addr);
    if (!is_error(ret)) {
        ret = get_errno(sched_setparam(pid, &host_sp));
    }
    return ret;
}

/* sched_get_param(2) */
static inline abi_long do_freebsd_sched_getparam(pid_t pid,
        abi_ulong target_sp_addr)
{
    abi_long ret;
    struct sched_param host_sp;

    ret = get_errno(sched_getparam(pid, &host_sp));
    if (!is_error(ret)) {
        ret = put_user_s32(host_sp.sched_priority, target_sp_addr);
    }
    return ret;
}

/* sched_setscheduler(2) */
static inline abi_long do_freebsd_sched_setscheduler(pid_t pid, int policy,
        abi_ulong target_sp_addr)
{
    abi_long ret;
    struct sched_param host_sp;

    ret = get_user_s32(host_sp.sched_priority, target_sp_addr);
    if (!is_error(ret)) {
        ret = get_errno(sched_setscheduler(pid, policy, &host_sp));
    }
    return ret;
}

/* sched_getscheduler(2) */
static inline abi_long do_freebsd_sched_getscheduler(pid_t pid)
{

    return get_errno(sched_getscheduler(pid));
}

/* sched_getscheduler(2) */
static inline abi_long do_freebsd_sched_rr_get_interval(pid_t pid,
        abi_ulong target_ts_addr)
{
    abi_long ret;
    struct timespec host_ts;

    ret = get_errno(sched_rr_get_interval(pid, &host_ts));
    if (!is_error(ret)) {
        ret = h2t_freebsd_timespec(target_ts_addr, &host_ts);
    }
    return ret;
}

/* cpuset(2) */
static inline abi_long do_freebsd_cpuset(abi_ulong target_cpuid)
{
    abi_long ret;
    cpusetid_t setid;

    ret = get_errno(cpuset(&setid));
    if (is_error(ret)) {
        return ret;
    }
    return put_user_s32(setid, target_cpuid);
}

#define target_to_host_cpuset_which(hp, t) { \
    (*hp) = t;                               \
} while (0)

#define target_to_host_cpuset_level(hp, t) { \
    (*hp) = t;                               \
} while (0)

/* cpuset_setid(2) */
static inline abi_long do_freebsd_cpuset_setid(void *cpu_env, abi_long arg1,
        abi_ulong arg2, abi_ulong arg3, abi_ulong arg4, abi_ulong arg5)
{
    id_t id;    /* 64-bit value */
    cpusetid_t setid;
    cpuwhich_t which;

    target_to_host_cpuset_which(&which, arg1);
#if TARGET_ABI_BITS == 32
    /* See if we need to align the register pairs */
    if (regpairs_aligned(cpu_env)) {
        id = target_arg64(arg3, arg4);
        setid = arg5;
    } else {
        id = target_arg64(arg2, arg3);
        setid = arg4;
    }
#else
    id = arg2;
    setid = arg3;
#endif
    return get_errno(cpuset_setid(which, id, setid));
}

/* cpuset_getid(2) */
static inline abi_long do_freebsd_cpuset_getid(abi_long arg1, abi_ulong arg2,
        abi_ulong arg3, abi_ulong arg4, abi_ulong arg5)
{
    abi_long ret;
    id_t id;    /* 64-bit value */
    cpusetid_t setid;
    cpuwhich_t which;
    cpulevel_t level;
    abi_ulong target_setid;

    target_to_host_cpuset_which(&which, arg1)
	;
    target_to_host_cpuset_level(&level, arg2);
#if TARGET_ABI_BITS == 32
    id = target_arg64(arg3, arg4);
    target_setid = arg5;
#else
    id = arg3;
    target_setid = arg4;
#endif
    ret = get_errno(cpuset_getid(level, which, id, &setid));
    if (is_error(ret)) {
        return ret;
    }
    return put_user_s32(setid, target_setid);
}

static abi_ulong copy_from_user_cpuset_mask(cpuset_t *mask,
	abi_ulong target_mask_addr)
{
	int i, j, k;
	abi_ulong b, *target_mask;

	target_mask = lock_user(VERIFY_READ, target_mask_addr,
					(CPU_SETSIZE / 8), 1);
	if (target_mask == NULL) {
		return -TARGET_EFAULT;
	}
	CPU_ZERO(mask);
	k = 0;
	for (i = 0; i < ((CPU_SETSIZE/8)/sizeof(abi_ulong)); i++) {
		__get_user(b, &target_mask[i]);
		for (j = 0; j < TARGET_ABI_BITS; j++) {
			if ((b >> j) & 1) {
				CPU_SET(k, mask);
			}
			k++;
		}
	}
	unlock_user(target_mask, target_mask_addr, 0);

	return 0;
}

static abi_ulong copy_to_user_cpuset_mask(abi_ulong target_mask_addr,
	cpuset_t *mask)
{
	int i, j, k;
	abi_ulong b, *target_mask;

	target_mask = lock_user(VERIFY_WRITE, target_mask_addr,
					(CPU_SETSIZE / 8), 0);
	if (target_mask == NULL) {
		return -TARGET_EFAULT;
	}
	k = 0;
	for (i = 0; i < ((CPU_SETSIZE/8)/sizeof(abi_ulong)); i++) {
		b = 0;
		for (j = 0; j < TARGET_ABI_BITS; j++) {
			b |= ((CPU_ISSET(k, mask) != 0) << j);
			k++;
		}
		__put_user(b, &target_mask[i]);
	}
	unlock_user(target_mask, target_mask_addr, (CPU_SETSIZE / 8));

	return 0;
}

/* cpuset_getaffinity(2) */
/* cpuset_getaffinity(cpulevel_t, cpuwhich_t, id_t, size_t, cpuset_t *); */
static inline abi_long do_freebsd_cpuset_getaffinity(cpulevel_t level,
        cpuwhich_t which, abi_ulong arg3, abi_ulong arg4, abi_ulong arg5,
        abi_ulong arg6)
{
	cpuset_t mask;
	abi_long ret;
    id_t id;    /* 64-bit */
    abi_ulong setsize, target_mask;

#if TARGET_ABI_BITS == 32
    id = (id_t)target_arg64(arg3, arg4);
    setsize = arg5;
    target_mask = arg6;
#else
    id = (id_t)arg3;
    setsize = arg4;
    target_mask = arg5;
#endif

	ret = get_errno(cpuset_getaffinity(level, which, id, setsize, &mask));
	if (ret == 0) {
		ret = copy_to_user_cpuset_mask(target_mask, &mask);
	}

    return ret;
}

/* cpuset_setaffinity(2) */
/* cpuset_setaffinity(cpulevel_t, cpuwhich_t, id_t, size_t, const cpuset_t *);*/
static inline abi_long do_freebsd_cpuset_setaffinity(cpulevel_t level,
        cpuwhich_t which, abi_ulong arg3, abi_ulong arg4, abi_ulong arg5,
        abi_ulong arg6)
{
	cpuset_t mask;
	abi_long ret;
    id_t id; /* 64-bit */
    abi_ulong setsize, target_mask;

#if TARGET_ABI_BITS == 32
    id = (id_t)target_arg64(arg3, arg4);
    setsize = arg5;
    target_mask = arg6;
#else
    id = (id_t)arg3;
    setsize = arg4;
    target_mask = arg5;
#endif

	ret = copy_from_user_cpuset_mask(&mask, target_mask);
	if (ret == 0) {
		ret = get_errno(cpuset_setaffinity(level, which, id, setsize, &mask));
	}

	return ret;
}

/* cpuset_getdomain(2) */
/* cpuset_getdomain(cpulevel_t, cpuwhich_t, id_t, size_t, const cpuset_t *);*/
static inline abi_long do_freebsd_cpuset_getdomain(cpulevel_t level,
        cpuwhich_t which, abi_ulong arg3, abi_ulong arg4, abi_ulong arg5,
        abi_long arg6)
{
    qemu_log("qemu: Unsupported syscall cpuset_getdomain()\n");
    return -TARGET_ENOSYS;
}

/* cpuset_setdomain(2) */
/* cpuset_setdomain(cpulevel_t, cpuwhich_t, id_t, size_t, const cpuset_t *);*/
static inline abi_long do_freebsd_cpuset_setdomain(cpulevel_t level,
        cpuwhich_t which, abi_ulong arg3, abi_ulong arg4, abi_ulong arg5,
        abi_long arg6)
{
    qemu_log("qemu: Unsupported syscall cpuset_getdomain()\n");
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

#if TARGET_ABI_BITS == 32
static inline uint64_t target_offset64(uint32_t word0, uint32_t word1)
{
#ifdef TARGET_WORDS_BIGENDIAN
    return ((uint64_t)word0 << 32) | word1;
#else
    return ((uint64_t)word1 << 32) | word0;
#endif
}
#else /* TARGET_ABI_BITS == 32 */
static inline uint64_t target_offset64(uint64_t word0, uint64_t word1)
{
    return word0;
}
#endif /* TARGET_ABI_BITS != 32 */

/* posix_fallocate(2) */
static inline abi_long do_freebsd_posix_fallocate(abi_long arg1, abi_long arg2, abi_long arg3, abi_long arg4, abi_long arg5, abi_long arg6)
{

#if TARGET_ABI_BITS == 32                           
    return get_errno(posix_fallocate(arg1, target_offset64(arg3, arg4),
        target_offset64(arg5, arg6)));
#else
    return get_errno(posix_fallocate(arg1, arg2, arg3));
#endif
}

/* posix_openpt(2) */
static inline abi_long do_freebsd_posix_openpt(abi_long flags)
{

    return get_errno(posix_openpt(flags));
}

/* posix_fadvise(2) */
static inline abi_long do_freebsd_posix_fadvise(abi_long fd, abi_ulong offset,
        abi_ulong len, abi_long advise)
{

    qemu_log("qemu: Unsupported syscall posix_fadvise()\n");
    return -TARGET_ENOSYS;
}

#if defined(HAVE_GETRANDOM)
static inline abi_long do_freebsd_getrandom(void *buf, abi_ulong buflen,
        abi_ulong flags)
{
    abi_long ret;
    void *p;

    p = lock_user(VERIFY_WRITE, buf, buflen, 0);
    if (p == NULL) {
        return -TARGET_EFAULT;
    }
    ret = get_errno(getrandom(p, buflen, flags));
    unlock_user(p, buf, ret);

    return ret;
}
#endif

#endif /* ! __OS_MISC_H_ */
