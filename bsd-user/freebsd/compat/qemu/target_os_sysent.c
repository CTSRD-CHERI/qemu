/*
 * System call switch table.
 *
 * DO NOT EDIT-- this file is automatically @generated.
 * $FreeBSD$
 */

#include <sys/param.h>
#include <sys/sysent.h>
#include <sys/sysproto.h>
#include <syscall_defs.h>

#define AS(name) (sizeof(struct name) / sizeof(abi_uintcap_t))

#ifdef COMPAT_FREEBSD10
#define compat10(n, name) .sy_narg = n, .sy_call = (sy_call_t *)__CONCAT(freebsd10_,name)
#else
#define compat10(n, name) .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys
#endif

#ifdef COMPAT_FREEBSD11
#define compat11(n, name) .sy_narg = n, .sy_call = (sy_call_t *)__CONCAT(freebsd11_,name)
#else
#define compat11(n, name) .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys
#endif

#ifdef COMPAT_FREEBSD12
#define compat12(n, name) .sy_narg = n, .sy_call = (sy_call_t *)__CONCAT(freebsd12_,name)
#else
#define compat12(n, name) .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys
#endif

/* The casts are bogus but will do for now. */
struct sysent target_sysent[] = {
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },		/* 0 = syscall */
	{ .sy_narg = AS(target_sys_exit_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_EXIT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 1 = exit */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_FORK, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },		/* 2 = fork */
	{ .sy_narg = AS(target_read_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_READ, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 3 = read */
	{ .sy_narg = AS(target_write_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_WRITE, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 4 = write */
	{ .sy_narg = AS(target_open_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_OPEN_RWTC, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 5 = open */
	{ .sy_narg = AS(target_close_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_CLOSE, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 6 = close */
	{ .sy_narg = AS(target_wait4_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_WAIT4, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 7 = wait4 */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 8 = obsolete ocreat */
	{ .sy_narg = AS(target_link_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_LINK, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 9 = link */
	{ .sy_narg = AS(target_unlink_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_UNLINK, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 10 = unlink */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 11 = obsolete execv */
	{ .sy_narg = AS(target_chdir_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_CHDIR, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 12 = chdir */
	{ .sy_narg = AS(target_fchdir_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_FCHDIR, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 13 = fchdir */
	{ compat11(AS(freebsd11_target_mknod_args),mknod), .sy_auevent = AUE_MKNOD, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 14 = freebsd11 mknod */
	{ .sy_narg = AS(target_chmod_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_CHMOD, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 15 = chmod */
	{ .sy_narg = AS(target_chown_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_CHOWN, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 16 = chown */
	{ .sy_narg = AS(target_break_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 17 = break */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 18 = obsolete freebsd4_getfsstat */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 19 = obsolete olseek */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_GETPID, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 20 = getpid */
	{ .sy_narg = AS(target_mount_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_MOUNT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 21 = mount */
	{ .sy_narg = AS(target_unmount_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_UMOUNT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 22 = unmount */
	{ .sy_narg = AS(target_setuid_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SETUID, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 23 = setuid */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_GETUID, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 24 = getuid */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_GETEUID, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 25 = geteuid */
	{ .sy_narg = AS(target_ptrace_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_PTRACE, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 26 = ptrace */
	{ .sy_narg = AS(target_recvmsg_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_RECVMSG, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 27 = recvmsg */
	{ .sy_narg = AS(target_sendmsg_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SENDMSG, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 28 = sendmsg */
	{ .sy_narg = AS(target_recvfrom_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_RECVFROM, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 29 = recvfrom */
	{ .sy_narg = AS(target_accept_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_ACCEPT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 30 = accept */
	{ .sy_narg = AS(target_getpeername_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_GETPEERNAME, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 31 = getpeername */
	{ .sy_narg = AS(target_getsockname_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_GETSOCKNAME, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 32 = getsockname */
	{ .sy_narg = AS(target_access_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_ACCESS, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 33 = access */
	{ .sy_narg = AS(target_chflags_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_CHFLAGS, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 34 = chflags */
	{ .sy_narg = AS(target_fchflags_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_FCHFLAGS, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 35 = fchflags */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SYNC, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },		/* 36 = sync */
	{ .sy_narg = AS(target_kill_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_KILL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 37 = kill */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 38 = obsolete ostat */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_GETPPID, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 39 = getppid */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 40 = obsolete olstat */
	{ .sy_narg = AS(target_dup_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_DUP, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 41 = dup */
	{ compat10(0,pipe), .sy_auevent = AUE_PIPE, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },			/* 42 = freebsd10 pipe */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_GETEGID, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 43 = getegid */
	{ .sy_narg = AS(target_profil_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_PROFILE, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 44 = profil */
	{ .sy_narg = AS(target_ktrace_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_KTRACE, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 45 = ktrace */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 46 = obsolete osigaction */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_GETGID, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 47 = getgid */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 48 = obsolete osigprocmask */
	{ .sy_narg = AS(target_getlogin_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_GETLOGIN, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 49 = getlogin */
	{ .sy_narg = AS(target_setlogin_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SETLOGIN, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 50 = setlogin */
	{ .sy_narg = AS(target_acct_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_ACCT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 51 = acct */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 52 = obsolete osigpending */
	{ .sy_narg = AS(target_sigaltstack_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SIGALTSTACK, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 53 = sigaltstack */
	{ .sy_narg = AS(target_ioctl_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_IOCTL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 54 = ioctl */
	{ .sy_narg = AS(target_reboot_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_REBOOT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 55 = reboot */
	{ .sy_narg = AS(target_revoke_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_REVOKE, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 56 = revoke */
	{ .sy_narg = AS(target_symlink_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SYMLINK, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 57 = symlink */
	{ .sy_narg = AS(target_readlink_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_READLINK, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 58 = readlink */
	{ .sy_narg = AS(target_execve_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_EXECVE, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 59 = execve */
	{ .sy_narg = AS(target_umask_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_UMASK, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 60 = umask */
	{ .sy_narg = AS(target_chroot_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_CHROOT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 61 = chroot */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 62 = obsolete ofstat */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 63 = obsolete ogetkerninfo */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 64 = obsolete ogetpagesize */
	{ .sy_narg = AS(target_msync_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_MSYNC, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 65 = msync */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_VFORK, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 66 = vfork */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 67 = obsolete vread */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 68 = obsolete vwrite */
	{ .sy_narg = AS(target_sbrk_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SBRK, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 69 = sbrk */
	{ .sy_narg = AS(target_sstk_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SSTK, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 70 = sstk */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 71 = obsolete ommap */
	{ compat11(AS(freebsd11_target_vadvise_args),vadvise), .sy_auevent = AUE_O_VADVISE, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 72 = freebsd11 vadvise */
	{ .sy_narg = AS(target_munmap_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_MUNMAP, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 73 = munmap */
	{ .sy_narg = AS(target_mprotect_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_MPROTECT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 74 = mprotect */
	{ .sy_narg = AS(target_madvise_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_MADVISE, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 75 = madvise */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 76 = obsolete vhangup */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 77 = obsolete vlimit */
	{ .sy_narg = AS(target_mincore_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_MINCORE, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 78 = mincore */
	{ .sy_narg = AS(target_getgroups_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_GETGROUPS, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 79 = getgroups */
	{ .sy_narg = AS(target_setgroups_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SETGROUPS, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 80 = setgroups */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_GETPGRP, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 81 = getpgrp */
	{ .sy_narg = AS(target_setpgid_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SETPGRP, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 82 = setpgid */
	{ .sy_narg = AS(target_setitimer_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SETITIMER, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 83 = setitimer */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 84 = obsolete owait */
	{ .sy_narg = AS(target_swapon_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SWAPON, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 85 = swapon */
	{ .sy_narg = AS(target_getitimer_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_GETITIMER, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 86 = getitimer */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 87 = obsolete ogethostname */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 88 = obsolete osethostname */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_GETDTABLESIZE, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 89 = getdtablesize */
	{ .sy_narg = AS(target_dup2_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_DUP2, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 90 = dup2 */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 91 = getdopt */
	{ .sy_narg = AS(target_fcntl_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_FCNTL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 92 = fcntl */
	{ .sy_narg = AS(target_select_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SELECT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 93 = select */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 94 = setdopt */
	{ .sy_narg = AS(target_fsync_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_FSYNC, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 95 = fsync */
	{ .sy_narg = AS(target_setpriority_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SETPRIORITY, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 96 = setpriority */
	{ .sy_narg = AS(target_socket_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SOCKET, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 97 = socket */
	{ .sy_narg = AS(target_connect_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_CONNECT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 98 = connect */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 99 = obsolete oaccept */
	{ .sy_narg = AS(target_getpriority_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_GETPRIORITY, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 100 = getpriority */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 101 = obsolete osend */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 102 = obsolete orecv */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 103 = obsolete osigreturn */
	{ .sy_narg = AS(target_bind_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_BIND, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 104 = bind */
	{ .sy_narg = AS(target_setsockopt_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SETSOCKOPT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 105 = setsockopt */
	{ .sy_narg = AS(target_listen_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_LISTEN, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 106 = listen */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 107 = obsolete vtimes */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 108 = obsolete osigvec */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 109 = obsolete osigblock */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 110 = obsolete osigsetmask */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 111 = obsolete osigsuspend */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 112 = obsolete osigstack */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 113 = obsolete orecvmsg */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 114 = obsolete osendmsg */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 115 = obsolete vtrace */
	{ .sy_narg = AS(target_gettimeofday_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_GETTIMEOFDAY, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 116 = gettimeofday */
	{ .sy_narg = AS(target_getrusage_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_GETRUSAGE, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 117 = getrusage */
	{ .sy_narg = AS(target_getsockopt_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_GETSOCKOPT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 118 = getsockopt */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 119 = resuba */
	{ .sy_narg = AS(target_readv_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_READV, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 120 = readv */
	{ .sy_narg = AS(target_writev_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_WRITEV, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 121 = writev */
	{ .sy_narg = AS(target_settimeofday_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SETTIMEOFDAY, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 122 = settimeofday */
	{ .sy_narg = AS(target_fchown_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_FCHOWN, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 123 = fchown */
	{ .sy_narg = AS(target_fchmod_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_FCHMOD, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 124 = fchmod */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 125 = obsolete orecvfrom */
	{ .sy_narg = AS(target_setreuid_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SETREUID, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 126 = setreuid */
	{ .sy_narg = AS(target_setregid_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SETREGID, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 127 = setregid */
	{ .sy_narg = AS(target_rename_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_RENAME, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 128 = rename */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 129 = obsolete otruncate */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 130 = obsolete oftruncate */
	{ .sy_narg = AS(target_flock_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_FLOCK, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 131 = flock */
	{ .sy_narg = AS(target_mkfifo_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_MKFIFO, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 132 = mkfifo */
	{ .sy_narg = AS(target_sendto_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SENDTO, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 133 = sendto */
	{ .sy_narg = AS(target_shutdown_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SHUTDOWN, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 134 = shutdown */
	{ .sy_narg = AS(target_socketpair_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SOCKETPAIR, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 135 = socketpair */
	{ .sy_narg = AS(target_mkdir_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_MKDIR, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 136 = mkdir */
	{ .sy_narg = AS(target_rmdir_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_RMDIR, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 137 = rmdir */
	{ .sy_narg = AS(target_utimes_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_UTIMES, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 138 = utimes */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 139 = obsolete 4.2 sigreturn */
	{ .sy_narg = AS(target_adjtime_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_ADJTIME, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 140 = adjtime */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 141 = obsolete ogetpeername */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 142 = obsolete ogethostid */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 143 = obsolete osethostid */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 144 = obsolete ogetrlimit */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 145 = obsolete osetrlimit */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 146 = obsolete okillpg */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SETSID, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 147 = setsid */
	{ .sy_narg = AS(target_quotactl_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_QUOTACTL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 148 = quotactl */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 149 = obsolete oquota */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 150 = obsolete ogetsockname */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 151 = sem_lock */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 152 = sem_wakeup */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 153 = asyncdaemon */
	{ .sy_narg = AS(target_nlm_syscall_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },	/* 154 = nlm_syscall */
	{ .sy_narg = AS(target_nfssvc_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },	/* 155 = nfssvc */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 156 = obsolete ogetdirentries */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 157 = obsolete freebsd4_statfs */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 158 = obsolete freebsd4_fstatfs */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 159 = nosys */
	{ .sy_narg = AS(target_lgetfh_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_LGETFH, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 160 = lgetfh */
	{ .sy_narg = AS(target_getfh_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NFS_GETFH, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 161 = getfh */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 162 = obsolete freebsd4_getdomainname */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 163 = obsolete freebsd4_setdomainname */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 164 = obsolete freebsd4_uname */
	{ .sy_narg = AS(target_sysarch_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SYSARCH, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 165 = sysarch */
	{ .sy_narg = AS(target_rtprio_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_RTPRIO, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 166 = rtprio */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 167 = nosys */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 168 = nosys */
	{ .sy_narg = AS(target_semsys_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },	/* 169 = semsys */
	{ .sy_narg = AS(target_msgsys_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },	/* 170 = msgsys */
	{ .sy_narg = AS(target_shmsys_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },	/* 171 = shmsys */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 172 = nosys */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 173 = obsolete freebsd6_pread */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 174 = obsolete freebsd6_pwrite */
	{ .sy_narg = AS(target_setfib_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SETFIB, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 175 = setfib */
	{ .sy_narg = AS(target_ntp_adjtime_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NTP_ADJTIME, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 176 = ntp_adjtime */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 177 = sfork */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 178 = getdescriptor */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 179 = setdescriptor */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 180 = nosys */
	{ .sy_narg = AS(target_setgid_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SETGID, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 181 = setgid */
	{ .sy_narg = AS(target_setegid_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SETEGID, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 182 = setegid */
	{ .sy_narg = AS(target_seteuid_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SETEUID, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 183 = seteuid */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 184 = obsolete lfs_bmapv */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 185 = obsolete lfs_markv */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 186 = obsolete lfs_segclean */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 187 = obsolete lfs_segwait */
	{ compat11(AS(freebsd11_target_stat_args),stat), .sy_auevent = AUE_STAT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 188 = freebsd11 stat */
	{ compat11(AS(freebsd11_target_fstat_args),fstat), .sy_auevent = AUE_FSTAT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 189 = freebsd11 fstat */
	{ compat11(AS(freebsd11_target_lstat_args),lstat), .sy_auevent = AUE_LSTAT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 190 = freebsd11 lstat */
	{ .sy_narg = AS(target_pathconf_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_PATHCONF, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 191 = pathconf */
	{ .sy_narg = AS(target_fpathconf_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_FPATHCONF, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 192 = fpathconf */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 193 = nosys */
	{ .sy_narg = AS(target___getrlimit_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_GETRLIMIT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 194 = getrlimit */
	{ .sy_narg = AS(target___setrlimit_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SETRLIMIT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 195 = setrlimit */
	{ compat11(AS(freebsd11_target_getdirentries_args),getdirentries), .sy_auevent = AUE_GETDIRENTRIES, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 196 = freebsd11 getdirentries */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 197 = obsolete freebsd6_mmap */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },		/* 198 = __syscall */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 199 = obsolete freebsd6_lseek */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 200 = obsolete freebsd6_truncate */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 201 = obsolete freebsd6_ftruncate */
	{ .sy_narg = AS(target___sysctl_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SYSCTL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 202 = __sysctl */
	{ .sy_narg = AS(target_mlock_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_MLOCK, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 203 = mlock */
	{ .sy_narg = AS(target_munlock_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_MUNLOCK, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 204 = munlock */
	{ .sy_narg = AS(target_undelete_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_UNDELETE, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 205 = undelete */
	{ .sy_narg = AS(target_futimes_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_FUTIMES, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 206 = futimes */
	{ .sy_narg = AS(target_getpgid_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_GETPGID, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 207 = getpgid */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 208 = nosys */
	{ .sy_narg = AS(target_poll_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_POLL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 209 = poll */
	{ .sy_narg = AS(target_nosys_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },	/* 210 = lkmnosys */
	{ .sy_narg = AS(target_nosys_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },	/* 211 = lkmnosys */
	{ .sy_narg = AS(target_nosys_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },	/* 212 = lkmnosys */
	{ .sy_narg = AS(target_nosys_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },	/* 213 = lkmnosys */
	{ .sy_narg = AS(target_nosys_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },	/* 214 = lkmnosys */
	{ .sy_narg = AS(target_nosys_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },	/* 215 = lkmnosys */
	{ .sy_narg = AS(target_nosys_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },	/* 216 = lkmnosys */
	{ .sy_narg = AS(target_nosys_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },	/* 217 = lkmnosys */
	{ .sy_narg = AS(target_nosys_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },	/* 218 = lkmnosys */
	{ .sy_narg = AS(target_nosys_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },	/* 219 = lkmnosys */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 220 = obsolete freebsd7___semctl */
	{ .sy_narg = AS(target_semget_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },	/* 221 = semget */
	{ .sy_narg = AS(target_semop_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },	/* 222 = semop */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 223 = obsolete semconfig */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 224 = obsolete freebsd7_msgctl */
	{ .sy_narg = AS(target_msgget_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },	/* 225 = msgget */
	{ .sy_narg = AS(target_msgsnd_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },	/* 226 = msgsnd */
	{ .sy_narg = AS(target_msgrcv_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },	/* 227 = msgrcv */
	{ .sy_narg = AS(target_shmat_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },	/* 228 = shmat */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 229 = obsolete freebsd7_shmctl */
	{ .sy_narg = AS(target_shmdt_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },	/* 230 = shmdt */
	{ .sy_narg = AS(target_shmget_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },	/* 231 = shmget */
	{ .sy_narg = AS(target_clock_gettime_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 232 = clock_gettime */
	{ .sy_narg = AS(target_clock_settime_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_CLOCK_SETTIME, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 233 = clock_settime */
	{ .sy_narg = AS(target_clock_getres_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 234 = clock_getres */
	{ .sy_narg = AS(target_ktimer_create_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 235 = ktimer_create */
	{ .sy_narg = AS(target_ktimer_delete_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 236 = ktimer_delete */
	{ .sy_narg = AS(target_ktimer_settime_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 237 = ktimer_settime */
	{ .sy_narg = AS(target_ktimer_gettime_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 238 = ktimer_gettime */
	{ .sy_narg = AS(target_ktimer_getoverrun_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 239 = ktimer_getoverrun */
	{ .sy_narg = AS(target_nanosleep_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 240 = nanosleep */
	{ .sy_narg = AS(target_ffclock_getcounter_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 241 = ffclock_getcounter */
	{ .sy_narg = AS(target_ffclock_setestimate_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 242 = ffclock_setestimate */
	{ .sy_narg = AS(target_ffclock_getestimate_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 243 = ffclock_getestimate */
	{ .sy_narg = AS(target_clock_nanosleep_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 244 = clock_nanosleep */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 245 = nosys */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 246 = nosys */
	{ .sy_narg = AS(target_clock_getcpuclockid2_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 247 = clock_getcpuclockid2 */
	{ .sy_narg = AS(target_ntp_gettime_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 248 = ntp_gettime */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 249 = nosys */
	{ .sy_narg = AS(target_minherit_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_MINHERIT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 250 = minherit */
	{ .sy_narg = AS(target_rfork_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_RFORK, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 251 = rfork */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 252 = obsolete openbsd_poll */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_ISSETUGID, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 253 = issetugid */
	{ .sy_narg = AS(target_lchown_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_LCHOWN, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 254 = lchown */
	{ .sy_narg = AS(target_aio_read_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_AIO_READ, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 255 = aio_read */
	{ .sy_narg = AS(target_aio_write_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_AIO_WRITE, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 256 = aio_write */
	{ .sy_narg = AS(target_lio_listio_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_LIO_LISTIO, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 257 = lio_listio */
	{ .sy_narg = AS(target_kbounce_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 258 = kbounce */
	{ .sy_narg = AS(target_flag_captured_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 259 = flag_captured */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 260 = nosys */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 261 = nosys */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 262 = nosys */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 263 = nosys */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 264 = nosys */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 265 = nosys */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 266 = nosys */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 267 = nosys */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 268 = nosys */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 269 = nosys */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 270 = nosys */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 271 = nosys */
	{ compat11(AS(freebsd11_target_getdents_args),getdents), .sy_auevent = AUE_O_GETDENTS, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 272 = freebsd11 getdents */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 273 = nosys */
	{ .sy_narg = AS(target_lchmod_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_LCHMOD, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 274 = lchmod */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 275 = obsolete netbsd_lchown */
	{ .sy_narg = AS(target_lutimes_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_LUTIMES, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 276 = lutimes */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 277 = obsolete netbsd_msync */
	{ compat11(AS(freebsd11_target_nstat_args),nstat), .sy_auevent = AUE_STAT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 278 = freebsd11 nstat */
	{ compat11(AS(freebsd11_target_nfstat_args),nfstat), .sy_auevent = AUE_FSTAT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 279 = freebsd11 nfstat */
	{ compat11(AS(freebsd11_target_nlstat_args),nlstat), .sy_auevent = AUE_LSTAT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 280 = freebsd11 nlstat */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 281 = nosys */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 282 = nosys */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 283 = nosys */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 284 = nosys */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 285 = nosys */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 286 = nosys */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 287 = nosys */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 288 = nosys */
	{ .sy_narg = AS(target_preadv_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_PREADV, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 289 = preadv */
	{ .sy_narg = AS(target_pwritev_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_PWRITEV, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 290 = pwritev */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 291 = nosys */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 292 = nosys */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 293 = nosys */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 294 = nosys */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 295 = nosys */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 296 = nosys */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 297 = obsolete freebsd4_fhstatfs */
	{ .sy_narg = AS(target_fhopen_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_FHOPEN, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 298 = fhopen */
	{ compat11(AS(freebsd11_target_fhstat_args),fhstat), .sy_auevent = AUE_FHSTAT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 299 = freebsd11 fhstat */
	{ .sy_narg = AS(target_modnext_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 300 = modnext */
	{ .sy_narg = AS(target_modstat_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 301 = modstat */
	{ .sy_narg = AS(target_modfnext_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 302 = modfnext */
	{ .sy_narg = AS(target_modfind_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 303 = modfind */
	{ .sy_narg = AS(target_kldload_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_MODLOAD, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 304 = kldload */
	{ .sy_narg = AS(target_kldunload_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_MODUNLOAD, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 305 = kldunload */
	{ .sy_narg = AS(target_kldfind_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 306 = kldfind */
	{ .sy_narg = AS(target_kldnext_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 307 = kldnext */
	{ .sy_narg = AS(target_kldstat_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 308 = kldstat */
	{ .sy_narg = AS(target_kldfirstmod_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 309 = kldfirstmod */
	{ .sy_narg = AS(target_getsid_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_GETSID, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 310 = getsid */
	{ .sy_narg = AS(target_setresuid_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SETRESUID, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 311 = setresuid */
	{ .sy_narg = AS(target_setresgid_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SETRESGID, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 312 = setresgid */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 313 = obsolete signanosleep */
	{ .sy_narg = AS(target_aio_return_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_AIO_RETURN, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 314 = aio_return */
	{ .sy_narg = AS(target_aio_suspend_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_AIO_SUSPEND, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 315 = aio_suspend */
	{ .sy_narg = AS(target_aio_cancel_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_AIO_CANCEL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 316 = aio_cancel */
	{ .sy_narg = AS(target_aio_error_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_AIO_ERROR, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 317 = aio_error */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 318 = obsolete freebsd6_aio_read */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 319 = obsolete freebsd6_aio_write */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 320 = obsolete freebsd6_lio_listio */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },		/* 321 = yield */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 322 = obsolete thr_sleep */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 323 = obsolete thr_wakeup */
	{ .sy_narg = AS(target_mlockall_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_MLOCKALL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 324 = mlockall */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_MUNLOCKALL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 325 = munlockall */
	{ .sy_narg = AS(target___getcwd_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_GETCWD, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 326 = __getcwd */
	{ .sy_narg = AS(target_sched_setparam_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 327 = sched_setparam */
	{ .sy_narg = AS(target_sched_getparam_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 328 = sched_getparam */
	{ .sy_narg = AS(target_sched_setscheduler_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 329 = sched_setscheduler */
	{ .sy_narg = AS(target_sched_getscheduler_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 330 = sched_getscheduler */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 331 = sched_yield */
	{ .sy_narg = AS(target_sched_get_priority_max_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 332 = sched_get_priority_max */
	{ .sy_narg = AS(target_sched_get_priority_min_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 333 = sched_get_priority_min */
	{ .sy_narg = AS(target_sched_rr_get_interval_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 334 = sched_rr_get_interval */
	{ .sy_narg = AS(target_utrace_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 335 = utrace */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 336 = obsolete freebsd4_sendfile */
	{ .sy_narg = AS(target_kldsym_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 337 = kldsym */
	{ .sy_narg = AS(target_jail_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_JAIL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 338 = jail */
	{ .sy_narg = AS(target_nnpfs_syscall_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },	/* 339 = nnpfs_syscall */
	{ .sy_narg = AS(target_sigprocmask_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SIGPROCMASK, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 340 = sigprocmask */
	{ .sy_narg = AS(target_sigsuspend_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SIGSUSPEND, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 341 = sigsuspend */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 342 = obsolete freebsd4_sigaction */
	{ .sy_narg = AS(target_sigpending_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SIGPENDING, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 343 = sigpending */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 344 = obsolete freebsd4_sigreturn */
	{ .sy_narg = AS(target_sigtimedwait_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SIGWAIT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 345 = sigtimedwait */
	{ .sy_narg = AS(target_sigwaitinfo_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 346 = sigwaitinfo */
	{ .sy_narg = AS(target___acl_get_file_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_ACL_GET_FILE, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 347 = __acl_get_file */
	{ .sy_narg = AS(target___acl_set_file_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_ACL_SET_FILE, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 348 = __acl_set_file */
	{ .sy_narg = AS(target___acl_get_fd_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_ACL_GET_FD, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 349 = __acl_get_fd */
	{ .sy_narg = AS(target___acl_set_fd_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_ACL_SET_FD, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 350 = __acl_set_fd */
	{ .sy_narg = AS(target___acl_delete_file_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_ACL_DELETE_FILE, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 351 = __acl_delete_file */
	{ .sy_narg = AS(target___acl_delete_fd_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_ACL_DELETE_FD, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 352 = __acl_delete_fd */
	{ .sy_narg = AS(target___acl_aclcheck_file_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_ACL_CHECK_FILE, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 353 = __acl_aclcheck_file */
	{ .sy_narg = AS(target___acl_aclcheck_fd_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_ACL_CHECK_FD, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 354 = __acl_aclcheck_fd */
	{ .sy_narg = AS(target_extattrctl_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_EXTATTRCTL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 355 = extattrctl */
	{ .sy_narg = AS(target_extattr_set_file_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_EXTATTR_SET_FILE, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 356 = extattr_set_file */
	{ .sy_narg = AS(target_extattr_get_file_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_EXTATTR_GET_FILE, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 357 = extattr_get_file */
	{ .sy_narg = AS(target_extattr_delete_file_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_EXTATTR_DELETE_FILE, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 358 = extattr_delete_file */
	{ .sy_narg = AS(target_aio_waitcomplete_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_AIO_WAITCOMPLETE, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 359 = aio_waitcomplete */
	{ .sy_narg = AS(target_getresuid_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_GETRESUID, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 360 = getresuid */
	{ .sy_narg = AS(target_getresgid_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_GETRESGID, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 361 = getresgid */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_KQUEUE, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 362 = kqueue */
	{ compat11(AS(freebsd11_target_kevent_args),kevent), .sy_auevent = AUE_KEVENT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 363 = freebsd11 kevent */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 364 = obsolete __cap_get_proc */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 365 = obsolete __cap_set_proc */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 366 = obsolete __cap_get_fd */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 367 = obsolete __cap_get_file */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 368 = obsolete __cap_set_fd */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 369 = obsolete __cap_set_file */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 370 = nosys */
	{ .sy_narg = AS(target_extattr_set_fd_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_EXTATTR_SET_FD, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 371 = extattr_set_fd */
	{ .sy_narg = AS(target_extattr_get_fd_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_EXTATTR_GET_FD, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 372 = extattr_get_fd */
	{ .sy_narg = AS(target_extattr_delete_fd_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_EXTATTR_DELETE_FD, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 373 = extattr_delete_fd */
	{ .sy_narg = AS(target___setugid_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SETUGID, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 374 = __setugid */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 375 = obsolete nfsclnt */
	{ .sy_narg = AS(target_eaccess_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_EACCESS, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 376 = eaccess */
	{ .sy_narg = AS(target_afs3_syscall_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },	/* 377 = afs3_syscall */
	{ .sy_narg = AS(target_nmount_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NMOUNT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 378 = nmount */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 379 = obsolete kse_exit */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 380 = obsolete kse_wakeup */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 381 = obsolete kse_create */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 382 = obsolete kse_thr_interrupt */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 383 = obsolete kse_release */
	{ .sy_narg = AS(target___mac_get_proc_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 384 = __mac_get_proc */
	{ .sy_narg = AS(target___mac_set_proc_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 385 = __mac_set_proc */
	{ .sy_narg = AS(target___mac_get_fd_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 386 = __mac_get_fd */
	{ .sy_narg = AS(target___mac_get_file_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 387 = __mac_get_file */
	{ .sy_narg = AS(target___mac_set_fd_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 388 = __mac_set_fd */
	{ .sy_narg = AS(target___mac_set_file_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 389 = __mac_set_file */
	{ .sy_narg = AS(target_kenv_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 390 = kenv */
	{ .sy_narg = AS(target_lchflags_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_LCHFLAGS, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 391 = lchflags */
	{ .sy_narg = AS(target_uuidgen_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 392 = uuidgen */
	{ .sy_narg = AS(target_sendfile_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SENDFILE, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 393 = sendfile */
	{ .sy_narg = AS(target_mac_syscall_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 394 = mac_syscall */
	{ compat11(AS(freebsd11_target_getfsstat_args),getfsstat), .sy_auevent = AUE_GETFSSTAT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 395 = freebsd11 getfsstat */
	{ compat11(AS(freebsd11_target_statfs_args),statfs), .sy_auevent = AUE_STATFS, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 396 = freebsd11 statfs */
	{ compat11(AS(freebsd11_target_fstatfs_args),fstatfs), .sy_auevent = AUE_FSTATFS, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 397 = freebsd11 fstatfs */
	{ compat11(AS(freebsd11_target_fhstatfs_args),fhstatfs), .sy_auevent = AUE_FHSTATFS, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 398 = freebsd11 fhstatfs */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 399 = nosys */
	{ .sy_narg = AS(target_ksem_close_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },	/* 400 = ksem_close */
	{ .sy_narg = AS(target_ksem_post_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },	/* 401 = ksem_post */
	{ .sy_narg = AS(target_ksem_wait_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },	/* 402 = ksem_wait */
	{ .sy_narg = AS(target_ksem_trywait_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },	/* 403 = ksem_trywait */
	{ .sy_narg = AS(target_ksem_init_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },	/* 404 = ksem_init */
	{ .sy_narg = AS(target_ksem_open_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },	/* 405 = ksem_open */
	{ .sy_narg = AS(target_ksem_unlink_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },	/* 406 = ksem_unlink */
	{ .sy_narg = AS(target_ksem_getvalue_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },	/* 407 = ksem_getvalue */
	{ .sy_narg = AS(target_ksem_destroy_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },	/* 408 = ksem_destroy */
	{ .sy_narg = AS(target___mac_get_pid_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 409 = __mac_get_pid */
	{ .sy_narg = AS(target___mac_get_link_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 410 = __mac_get_link */
	{ .sy_narg = AS(target___mac_set_link_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 411 = __mac_set_link */
	{ .sy_narg = AS(target_extattr_set_link_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_EXTATTR_SET_LINK, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 412 = extattr_set_link */
	{ .sy_narg = AS(target_extattr_get_link_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_EXTATTR_GET_LINK, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 413 = extattr_get_link */
	{ .sy_narg = AS(target_extattr_delete_link_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_EXTATTR_DELETE_LINK, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 414 = extattr_delete_link */
	{ .sy_narg = AS(target___mac_execve_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 415 = __mac_execve */
	{ .sy_narg = AS(target_sigaction_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SIGACTION, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 416 = sigaction */
	{ .sy_narg = AS(target_sigreturn_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SIGRETURN, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 417 = sigreturn */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 418 = __xstat */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 419 = __xfstat */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 420 = __xlstat */
	{ .sy_narg = AS(target_getcontext_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 421 = getcontext */
	{ .sy_narg = AS(target_setcontext_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 422 = setcontext */
	{ .sy_narg = AS(target_swapcontext_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 423 = swapcontext */
	{ .sy_narg = AS(target_swapoff_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SWAPOFF, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 424 = swapoff */
	{ .sy_narg = AS(target___acl_get_link_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_ACL_GET_LINK, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 425 = __acl_get_link */
	{ .sy_narg = AS(target___acl_set_link_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_ACL_SET_LINK, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 426 = __acl_set_link */
	{ .sy_narg = AS(target___acl_delete_link_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_ACL_DELETE_LINK, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 427 = __acl_delete_link */
	{ .sy_narg = AS(target___acl_aclcheck_link_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_ACL_CHECK_LINK, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 428 = __acl_aclcheck_link */
	{ .sy_narg = AS(target_sigwait_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SIGWAIT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 429 = sigwait */
	{ .sy_narg = AS(target_thr_create_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_THR_CREATE, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 430 = thr_create */
	{ .sy_narg = AS(target_thr_exit_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_THR_EXIT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 431 = thr_exit */
	{ .sy_narg = AS(target_thr_self_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 432 = thr_self */
	{ .sy_narg = AS(target_thr_kill_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_THR_KILL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 433 = thr_kill */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 434 = nosys */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 435 = nosys */
	{ .sy_narg = AS(target_jail_attach_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_JAIL_ATTACH, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 436 = jail_attach */
	{ .sy_narg = AS(target_extattr_list_fd_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_EXTATTR_LIST_FD, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 437 = extattr_list_fd */
	{ .sy_narg = AS(target_extattr_list_file_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_EXTATTR_LIST_FILE, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 438 = extattr_list_file */
	{ .sy_narg = AS(target_extattr_list_link_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_EXTATTR_LIST_LINK, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 439 = extattr_list_link */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 440 = obsolete kse_switchin */
	{ .sy_narg = AS(target_ksem_timedwait_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },	/* 441 = ksem_timedwait */
	{ .sy_narg = AS(target_thr_suspend_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 442 = thr_suspend */
	{ .sy_narg = AS(target_thr_wake_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 443 = thr_wake */
	{ .sy_narg = AS(target_kldunloadf_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_MODUNLOAD, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 444 = kldunloadf */
	{ .sy_narg = AS(target_audit_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_AUDIT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 445 = audit */
	{ .sy_narg = AS(target_auditon_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_AUDITON, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 446 = auditon */
	{ .sy_narg = AS(target_getauid_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_GETAUID, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 447 = getauid */
	{ .sy_narg = AS(target_setauid_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SETAUID, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 448 = setauid */
	{ .sy_narg = AS(target_getaudit_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_GETAUDIT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 449 = getaudit */
	{ .sy_narg = AS(target_setaudit_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SETAUDIT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 450 = setaudit */
	{ .sy_narg = AS(target_getaudit_addr_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_GETAUDIT_ADDR, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 451 = getaudit_addr */
	{ .sy_narg = AS(target_setaudit_addr_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SETAUDIT_ADDR, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 452 = setaudit_addr */
	{ .sy_narg = AS(target_auditctl_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_AUDITCTL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 453 = auditctl */
	{ .sy_narg = AS(target__umtx_op_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 454 = _umtx_op */
	{ .sy_narg = AS(target_thr_new_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_THR_NEW, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 455 = thr_new */
	{ .sy_narg = AS(target_sigqueue_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 456 = sigqueue */
	{ .sy_narg = AS(target_kmq_open_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },	/* 457 = kmq_open */
	{ .sy_narg = AS(target_kmq_setattr_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },	/* 458 = kmq_setattr */
	{ .sy_narg = AS(target_kmq_timedreceive_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },	/* 459 = kmq_timedreceive */
	{ .sy_narg = AS(target_kmq_timedsend_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },	/* 460 = kmq_timedsend */
	{ .sy_narg = AS(target_kmq_notify_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },	/* 461 = kmq_notify */
	{ .sy_narg = AS(target_kmq_unlink_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },	/* 462 = kmq_unlink */
	{ .sy_narg = AS(target_abort2_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 463 = abort2 */
	{ .sy_narg = AS(target_thr_set_name_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 464 = thr_set_name */
	{ .sy_narg = AS(target_aio_fsync_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_AIO_FSYNC, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 465 = aio_fsync */
	{ .sy_narg = AS(target_rtprio_thread_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_RTPRIO, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 466 = rtprio_thread */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 467 = nosys */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 468 = nosys */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 469 = __getpath_fromfd */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 470 = __getpath_fromaddr */
	{ .sy_narg = AS(target_sctp_peeloff_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },	/* 471 = sctp_peeloff */
	{ .sy_narg = AS(target_sctp_generic_sendmsg_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },	/* 472 = sctp_generic_sendmsg */
	{ .sy_narg = AS(target_sctp_generic_sendmsg_iov_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },	/* 473 = sctp_generic_sendmsg_iov */
	{ .sy_narg = AS(target_sctp_generic_recvmsg_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },	/* 474 = sctp_generic_recvmsg */
	{ .sy_narg = AS(target_pread_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_PREAD, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 475 = pread */
	{ .sy_narg = AS(target_pwrite_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_PWRITE, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 476 = pwrite */
	{ .sy_narg = AS(target_mmap_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_MMAP, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 477 = mmap */
	{ .sy_narg = AS(target_lseek_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_LSEEK, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 478 = lseek */
	{ .sy_narg = AS(target_truncate_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_TRUNCATE, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 479 = truncate */
	{ .sy_narg = AS(target_ftruncate_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_FTRUNCATE, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 480 = ftruncate */
	{ .sy_narg = AS(target_thr_kill2_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_THR_KILL2, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 481 = thr_kill2 */
	{ compat12(AS(freebsd12_target_shm_open_args),shm_open), .sy_auevent = AUE_SHMOPEN, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 482 = freebsd12 shm_open */
	{ .sy_narg = AS(target_shm_unlink_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SHMUNLINK, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 483 = shm_unlink */
	{ .sy_narg = AS(target_cpuset_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 484 = cpuset */
	{ .sy_narg = AS(target_cpuset_setid_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 485 = cpuset_setid */
	{ .sy_narg = AS(target_cpuset_getid_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 486 = cpuset_getid */
	{ .sy_narg = AS(target_cpuset_getaffinity_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 487 = cpuset_getaffinity */
	{ .sy_narg = AS(target_cpuset_setaffinity_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 488 = cpuset_setaffinity */
	{ .sy_narg = AS(target_faccessat_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_FACCESSAT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 489 = faccessat */
	{ .sy_narg = AS(target_fchmodat_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_FCHMODAT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 490 = fchmodat */
	{ .sy_narg = AS(target_fchownat_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_FCHOWNAT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 491 = fchownat */
	{ .sy_narg = AS(target_fexecve_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_FEXECVE, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 492 = fexecve */
	{ compat11(AS(freebsd11_target_fstatat_args),fstatat), .sy_auevent = AUE_FSTATAT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 493 = freebsd11 fstatat */
	{ .sy_narg = AS(target_futimesat_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_FUTIMESAT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 494 = futimesat */
	{ .sy_narg = AS(target_linkat_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_LINKAT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 495 = linkat */
	{ .sy_narg = AS(target_mkdirat_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_MKDIRAT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 496 = mkdirat */
	{ .sy_narg = AS(target_mkfifoat_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_MKFIFOAT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 497 = mkfifoat */
	{ compat11(AS(freebsd11_target_mknodat_args),mknodat), .sy_auevent = AUE_MKNODAT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 498 = freebsd11 mknodat */
	{ .sy_narg = AS(target_openat_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_OPENAT_RWTC, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 499 = openat */
	{ .sy_narg = AS(target_readlinkat_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_READLINKAT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 500 = readlinkat */
	{ .sy_narg = AS(target_renameat_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_RENAMEAT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 501 = renameat */
	{ .sy_narg = AS(target_symlinkat_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SYMLINKAT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 502 = symlinkat */
	{ .sy_narg = AS(target_unlinkat_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_UNLINKAT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 503 = unlinkat */
	{ .sy_narg = AS(target_posix_openpt_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_POSIX_OPENPT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 504 = posix_openpt */
	{ .sy_narg = AS(target_gssd_syscall_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },	/* 505 = gssd_syscall */
	{ .sy_narg = AS(target_jail_get_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_JAIL_GET, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 506 = jail_get */
	{ .sy_narg = AS(target_jail_set_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_JAIL_SET, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 507 = jail_set */
	{ .sy_narg = AS(target_jail_remove_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_JAIL_REMOVE, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 508 = jail_remove */
	{ compat12(AS(freebsd12_target_closefrom_args),closefrom), .sy_auevent = AUE_CLOSEFROM, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 509 = freebsd12 closefrom */
	{ .sy_narg = AS(target___semctl_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },	/* 510 = __semctl */
	{ .sy_narg = AS(target_msgctl_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },	/* 511 = msgctl */
	{ .sy_narg = AS(target_shmctl_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },	/* 512 = shmctl */
	{ .sy_narg = AS(target_lpathconf_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_LPATHCONF, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 513 = lpathconf */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 514 = obsolete cap_new */
	{ .sy_narg = AS(target___cap_rights_get_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_CAP_RIGHTS_GET, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 515 = __cap_rights_get */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_CAP_ENTER, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 516 = cap_enter */
	{ .sy_narg = AS(target_cap_getmode_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_CAP_GETMODE, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 517 = cap_getmode */
	{ .sy_narg = AS(target_pdfork_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_PDFORK, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 518 = pdfork */
	{ .sy_narg = AS(target_pdkill_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_PDKILL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 519 = pdkill */
	{ .sy_narg = AS(target_pdgetpid_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_PDGETPID, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 520 = pdgetpid */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 521 = pdwait4 */
	{ .sy_narg = AS(target_pselect_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SELECT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 522 = pselect */
	{ .sy_narg = AS(target_getloginclass_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_GETLOGINCLASS, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 523 = getloginclass */
	{ .sy_narg = AS(target_setloginclass_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SETLOGINCLASS, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 524 = setloginclass */
	{ .sy_narg = AS(target_rctl_get_racct_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 525 = rctl_get_racct */
	{ .sy_narg = AS(target_rctl_get_rules_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 526 = rctl_get_rules */
	{ .sy_narg = AS(target_rctl_get_limits_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 527 = rctl_get_limits */
	{ .sy_narg = AS(target_rctl_add_rule_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 528 = rctl_add_rule */
	{ .sy_narg = AS(target_rctl_remove_rule_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 529 = rctl_remove_rule */
	{ .sy_narg = AS(target_posix_fallocate_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_POSIX_FALLOCATE, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 530 = posix_fallocate */
	{ .sy_narg = AS(target_posix_fadvise_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_POSIX_FADVISE, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 531 = posix_fadvise */
	{ .sy_narg = AS(target_wait6_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_WAIT6, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 532 = wait6 */
	{ .sy_narg = AS(target_cap_rights_limit_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_CAP_RIGHTS_LIMIT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 533 = cap_rights_limit */
	{ .sy_narg = AS(target_cap_ioctls_limit_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_CAP_IOCTLS_LIMIT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 534 = cap_ioctls_limit */
	{ .sy_narg = AS(target_cap_ioctls_get_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_CAP_IOCTLS_GET, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 535 = cap_ioctls_get */
	{ .sy_narg = AS(target_cap_fcntls_limit_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_CAP_FCNTLS_LIMIT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 536 = cap_fcntls_limit */
	{ .sy_narg = AS(target_cap_fcntls_get_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_CAP_FCNTLS_GET, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 537 = cap_fcntls_get */
	{ .sy_narg = AS(target_bindat_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_BINDAT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 538 = bindat */
	{ .sy_narg = AS(target_connectat_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_CONNECTAT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 539 = connectat */
	{ .sy_narg = AS(target_chflagsat_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_CHFLAGSAT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 540 = chflagsat */
	{ .sy_narg = AS(target_accept4_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_ACCEPT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 541 = accept4 */
	{ .sy_narg = AS(target_pipe2_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_PIPE, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 542 = pipe2 */
	{ .sy_narg = AS(target_aio_mlock_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_AIO_MLOCK, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 543 = aio_mlock */
	{ .sy_narg = AS(target_procctl_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_PROCCTL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 544 = procctl */
	{ .sy_narg = AS(target_ppoll_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_POLL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 545 = ppoll */
	{ .sy_narg = AS(target_futimens_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_FUTIMES, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 546 = futimens */
	{ .sy_narg = AS(target_utimensat_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_FUTIMESAT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 547 = utimensat */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 548 = obsolete numa_getaffinity */
	{ .sy_narg = 0, .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },			/* 549 = obsolete numa_setaffinity */
	{ .sy_narg = AS(target_fdatasync_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_FSYNC, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 550 = fdatasync */
	{ .sy_narg = AS(target_fstat_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_FSTAT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 551 = fstat */
	{ .sy_narg = AS(target_fstatat_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_FSTATAT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 552 = fstatat */
	{ .sy_narg = AS(target_fhstat_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_FHSTAT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 553 = fhstat */
	{ .sy_narg = AS(target_getdirentries_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_GETDIRENTRIES, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 554 = getdirentries */
	{ .sy_narg = AS(target_statfs_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_STATFS, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 555 = statfs */
	{ .sy_narg = AS(target_fstatfs_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_FSTATFS, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 556 = fstatfs */
	{ .sy_narg = AS(target_getfsstat_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_GETFSSTAT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 557 = getfsstat */
	{ .sy_narg = AS(target_fhstatfs_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_FHSTATFS, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 558 = fhstatfs */
	{ .sy_narg = AS(target_mknodat_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_MKNODAT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 559 = mknodat */
	{ .sy_narg = AS(target_kevent_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_KEVENT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 560 = kevent */
	{ .sy_narg = AS(target_cpuset_getdomain_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 561 = cpuset_getdomain */
	{ .sy_narg = AS(target_cpuset_setdomain_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 562 = cpuset_setdomain */
	{ .sy_narg = AS(target_getrandom_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 563 = getrandom */
	{ .sy_narg = AS(target_getfhat_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 564 = getfhat */
	{ .sy_narg = AS(target_fhlink_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 565 = fhlink */
	{ .sy_narg = AS(target_fhlinkat_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 566 = fhlinkat */
	{ .sy_narg = AS(target_fhreadlink_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 567 = fhreadlink */
	{ .sy_narg = AS(target_funlinkat_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_UNLINKAT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 568 = funlinkat */
	{ .sy_narg = AS(target_copy_file_range_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 569 = copy_file_range */
	{ .sy_narg = AS(target___sysctlbyname_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SYSCTL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 570 = __sysctlbyname */
	{ .sy_narg = AS(target_shm_open2_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SHMOPEN, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 571 = shm_open2 */
	{ .sy_narg = AS(target_shm_rename_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_SHMRENAME, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 572 = shm_rename */
	{ .sy_narg = AS(target_sigfastblock_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 573 = sigfastblock */
	{ .sy_narg = AS(target___realpathat_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_REALPATHAT, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 574 = __realpathat */
	{ .sy_narg = AS(target_close_range_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_CLOSERANGE, .sy_flags = 0, .sy_thrcnt = SY_THR_STATIC },	/* 575 = close_range */
	{ .sy_narg = AS(target_rpctls_syscall_args), .sy_call = (sy_call_t *)do_bsd_nosys, .sy_auevent = AUE_NULL, .sy_flags = 0, .sy_thrcnt = SY_THR_ABSENT },	/* 576 = rpctls_syscall */
};
