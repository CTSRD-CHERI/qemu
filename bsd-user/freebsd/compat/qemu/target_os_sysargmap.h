/*
 * System call argument map.
 *
 * DO NOT EDIT-- this file is automatically @generated.
 * $FreeBSD$
 */

#ifndef _SYS_SYSARGMAP_H_
#define	_SYS_SYSARGMAP_H_

static int target_sysargmask[] = {
	[TARGET_FREEBSD_NR_syscall] = (0x0),
	[TARGET_FREEBSD_NR_exit] = (0x0),
	[TARGET_FREEBSD_NR_fork] = (0x0),
	[TARGET_FREEBSD_NR_read] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_write] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_open] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_close] = (0x0),
	[TARGET_FREEBSD_NR_wait4] = (0x0 | 0x2 | 0x8),
	[TARGET_FREEBSD_NR_link] = (0x0 | 0x1 | 0x2),
	[TARGET_FREEBSD_NR_unlink] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_chdir] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_fchdir] = (0x0),
	[TARGET_FREEBSD_NR_chmod] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_chown] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_break] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_getpid] = (0x0),
	[TARGET_FREEBSD_NR_mount] = (0x0 | 0x1 | 0x2 | 0x8),
	[TARGET_FREEBSD_NR_unmount] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_setuid] = (0x0),
	[TARGET_FREEBSD_NR_getuid] = (0x0),
	[TARGET_FREEBSD_NR_geteuid] = (0x0),
	[TARGET_FREEBSD_NR_ptrace] = (0x0 | 0x4),
	[TARGET_FREEBSD_NR_recvmsg] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_sendmsg] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_recvfrom] = (0x0 | 0x2 | 0x10 | 0x20),
	[TARGET_FREEBSD_NR_accept] = (0x0 | 0x2 | 0x4),
	[TARGET_FREEBSD_NR_getpeername] = (0x0 | 0x2 | 0x4),
	[TARGET_FREEBSD_NR_getsockname] = (0x0 | 0x2 | 0x4),
	[TARGET_FREEBSD_NR_access] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_chflags] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_fchflags] = (0x0),
	[TARGET_FREEBSD_NR_sync] = (0x0),
	[TARGET_FREEBSD_NR_kill] = (0x0),
	[TARGET_FREEBSD_NR_getppid] = (0x0),
	[TARGET_FREEBSD_NR_dup] = (0x0),
	[TARGET_FREEBSD_NR_getegid] = (0x0),
	[TARGET_FREEBSD_NR_profil] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_ktrace] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_getgid] = (0x0),
	[TARGET_FREEBSD_NR_getlogin] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_setlogin] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_acct] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_sigaltstack] = (0x0 | 0x1 | 0x2),
	[TARGET_FREEBSD_NR_ioctl] = (0x0 | 0x4),
	[TARGET_FREEBSD_NR_reboot] = (0x0),
	[TARGET_FREEBSD_NR_revoke] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_symlink] = (0x0 | 0x1 | 0x2),
	[TARGET_FREEBSD_NR_readlink] = (0x0 | 0x1 | 0x2),
	[TARGET_FREEBSD_NR_execve] = (0x0 | 0x1 | 0x2 | 0x4),
	[TARGET_FREEBSD_NR_umask] = (0x0),
	[TARGET_FREEBSD_NR_chroot] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_msync] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_vfork] = (0x0),
	[TARGET_FREEBSD_NR_sbrk] = (0x0),
	[TARGET_FREEBSD_NR_sstk] = (0x0),
	[TARGET_FREEBSD_NR_munmap] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_mprotect] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_madvise] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_mincore] = (0x0 | 0x1 | 0x4),
	[TARGET_FREEBSD_NR_getgroups] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_setgroups] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_getpgrp] = (0x0),
	[TARGET_FREEBSD_NR_setpgid] = (0x0),
	[TARGET_FREEBSD_NR_setitimer] = (0x0 | 0x2 | 0x4),
	[TARGET_FREEBSD_NR_swapon] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_getitimer] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_getdtablesize] = (0x0),
	[TARGET_FREEBSD_NR_dup2] = (0x0),
	[TARGET_FREEBSD_NR_fcntl] = (0x0 | 0x4),
	[TARGET_FREEBSD_NR_select] = (0x0 | 0x2 | 0x4 | 0x8 | 0x10),
	[TARGET_FREEBSD_NR_fsync] = (0x0),
	[TARGET_FREEBSD_NR_setpriority] = (0x0),
	[TARGET_FREEBSD_NR_socket] = (0x0),
	[TARGET_FREEBSD_NR_connect] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_getpriority] = (0x0),
	[TARGET_FREEBSD_NR_bind] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_setsockopt] = (0x0 | 0x8),
	[TARGET_FREEBSD_NR_listen] = (0x0),
	[TARGET_FREEBSD_NR_gettimeofday] = (0x0 | 0x1 | 0x2),
	[TARGET_FREEBSD_NR_getrusage] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_getsockopt] = (0x0 | 0x8 | 0x10),
	[TARGET_FREEBSD_NR_readv] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_writev] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_settimeofday] = (0x0 | 0x1 | 0x2),
	[TARGET_FREEBSD_NR_fchown] = (0x0),
	[TARGET_FREEBSD_NR_fchmod] = (0x0),
	[TARGET_FREEBSD_NR_setreuid] = (0x0),
	[TARGET_FREEBSD_NR_setregid] = (0x0),
	[TARGET_FREEBSD_NR_rename] = (0x0 | 0x1 | 0x2),
	[TARGET_FREEBSD_NR_flock] = (0x0),
	[TARGET_FREEBSD_NR_mkfifo] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_sendto] = (0x0 | 0x2 | 0x10),
	[TARGET_FREEBSD_NR_shutdown] = (0x0),
	[TARGET_FREEBSD_NR_socketpair] = (0x0 | 0x8),
	[TARGET_FREEBSD_NR_mkdir] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_rmdir] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_utimes] = (0x0 | 0x1 | 0x2),
	[TARGET_FREEBSD_NR_adjtime] = (0x0 | 0x1 | 0x2),
	[TARGET_FREEBSD_NR_setsid] = (0x0),
	[TARGET_FREEBSD_NR_quotactl] = (0x0 | 0x1 | 0x8),
	[TARGET_FREEBSD_NR_nlm_syscall] = (0x0 | 0x8),
	[TARGET_FREEBSD_NR_nfssvc] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_lgetfh] = (0x0 | 0x1 | 0x2),
	[TARGET_FREEBSD_NR_getfh] = (0x0 | 0x1 | 0x2),
	[TARGET_FREEBSD_NR_sysarch] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_rtprio] = (0x0 | 0x4),
	[TARGET_FREEBSD_NR_semsys] = (0x0 | 0x2 | 0x4 | 0x8 | 0x10),
	[TARGET_FREEBSD_NR_msgsys] = (0x0 | 0x2 | 0x4 | 0x8 | 0x10 | 0x20),
	[TARGET_FREEBSD_NR_shmsys] = (0x0 | 0x2 | 0x4 | 0x8),
	[TARGET_FREEBSD_NR_setfib] = (0x0),
	[TARGET_FREEBSD_NR_ntp_adjtime] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_setgid] = (0x0),
	[TARGET_FREEBSD_NR_setegid] = (0x0),
	[TARGET_FREEBSD_NR_seteuid] = (0x0),
	[TARGET_FREEBSD_NR_pathconf] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_fpathconf] = (0x0),
	[TARGET_FREEBSD_NR_getrlimit] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_setrlimit] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR___sysctl] = (0x0 | 0x1 | 0x4 | 0x8 | 0x10),
	[TARGET_FREEBSD_NR_mlock] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_munlock] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_undelete] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_futimes] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_getpgid] = (0x0),
	[TARGET_FREEBSD_NR_poll] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_semget] = (0x0),
	[TARGET_FREEBSD_NR_semop] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_msgget] = (0x0),
	[TARGET_FREEBSD_NR_msgsnd] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_msgrcv] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_shmat] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_shmdt] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_shmget] = (0x0),
	[TARGET_FREEBSD_NR_clock_gettime] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_clock_settime] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_clock_getres] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_ktimer_create] = (0x0 | 0x2 | 0x4),
	[TARGET_FREEBSD_NR_ktimer_delete] = (0x0),
	[TARGET_FREEBSD_NR_ktimer_settime] = (0x0 | 0x4 | 0x8),
	[TARGET_FREEBSD_NR_ktimer_gettime] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_ktimer_getoverrun] = (0x0),
	[TARGET_FREEBSD_NR_nanosleep] = (0x0 | 0x1 | 0x2),
	[TARGET_FREEBSD_NR_ffclock_getcounter] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_ffclock_setestimate] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_ffclock_getestimate] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_clock_nanosleep] = (0x0 | 0x4 | 0x8),
	[TARGET_FREEBSD_NR_clock_getcpuclockid2] = (0x0 | 0x4),
	[TARGET_FREEBSD_NR_ntp_gettime] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_minherit] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_rfork] = (0x0),
	[TARGET_FREEBSD_NR_issetugid] = (0x0),
	[TARGET_FREEBSD_NR_lchown] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_aio_read] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_aio_write] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_lio_listio] = (0x0 | 0x2 | 0x8),
	[TARGET_FREEBSD_NR_kbounce] = (0x0 | 0x1 | 0x2),
	[TARGET_FREEBSD_NR_flag_captured] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_lchmod] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_lutimes] = (0x0 | 0x1 | 0x2),
	[TARGET_FREEBSD_NR_preadv] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_pwritev] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_fhopen] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_modnext] = (0x0),
	[TARGET_FREEBSD_NR_modstat] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_modfnext] = (0x0),
	[TARGET_FREEBSD_NR_modfind] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_kldload] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_kldunload] = (0x0),
	[TARGET_FREEBSD_NR_kldfind] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_kldnext] = (0x0),
	[TARGET_FREEBSD_NR_kldstat] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_kldfirstmod] = (0x0),
	[TARGET_FREEBSD_NR_getsid] = (0x0),
	[TARGET_FREEBSD_NR_setresuid] = (0x0),
	[TARGET_FREEBSD_NR_setresgid] = (0x0),
	[TARGET_FREEBSD_NR_aio_return] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_aio_suspend] = (0x0 | 0x1 | 0x4),
	[TARGET_FREEBSD_NR_aio_cancel] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_aio_error] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_yield] = (0x0),
	[TARGET_FREEBSD_NR_mlockall] = (0x0),
	[TARGET_FREEBSD_NR_munlockall] = (0x0),
	[TARGET_FREEBSD_NR___getcwd] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_sched_setparam] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_sched_getparam] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_sched_setscheduler] = (0x0 | 0x4),
	[TARGET_FREEBSD_NR_sched_getscheduler] = (0x0),
	[TARGET_FREEBSD_NR_sched_yield] = (0x0),
	[TARGET_FREEBSD_NR_sched_get_priority_max] = (0x0),
	[TARGET_FREEBSD_NR_sched_get_priority_min] = (0x0),
	[TARGET_FREEBSD_NR_sched_rr_get_interval] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_utrace] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_kldsym] = (0x0 | 0x4),
	[TARGET_FREEBSD_NR_jail] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_nnpfs_syscall] = (0x0 | 0x2 | 0x8),
	[TARGET_FREEBSD_NR_sigprocmask] = (0x0 | 0x2 | 0x4),
	[TARGET_FREEBSD_NR_sigsuspend] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_sigpending] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_sigtimedwait] = (0x0 | 0x1 | 0x2 | 0x4),
	[TARGET_FREEBSD_NR_sigwaitinfo] = (0x0 | 0x1 | 0x2),
	[TARGET_FREEBSD_NR___acl_get_file] = (0x0 | 0x1 | 0x4),
	[TARGET_FREEBSD_NR___acl_set_file] = (0x0 | 0x1 | 0x4),
	[TARGET_FREEBSD_NR___acl_get_fd] = (0x0 | 0x4),
	[TARGET_FREEBSD_NR___acl_set_fd] = (0x0 | 0x4),
	[TARGET_FREEBSD_NR___acl_delete_file] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR___acl_delete_fd] = (0x0),
	[TARGET_FREEBSD_NR___acl_aclcheck_file] = (0x0 | 0x1 | 0x4),
	[TARGET_FREEBSD_NR___acl_aclcheck_fd] = (0x0 | 0x4),
	[TARGET_FREEBSD_NR_extattrctl] = (0x0 | 0x1 | 0x4 | 0x10),
	[TARGET_FREEBSD_NR_extattr_set_file] = (0x0 | 0x1 | 0x4 | 0x8),
	[TARGET_FREEBSD_NR_extattr_get_file] = (0x0 | 0x1 | 0x4 | 0x8),
	[TARGET_FREEBSD_NR_extattr_delete_file] = (0x0 | 0x1 | 0x4),
	[TARGET_FREEBSD_NR_aio_waitcomplete] = (0x0 | 0x1 | 0x2),
	[TARGET_FREEBSD_NR_getresuid] = (0x0 | 0x1 | 0x2 | 0x4),
	[TARGET_FREEBSD_NR_getresgid] = (0x0 | 0x1 | 0x2 | 0x4),
	[TARGET_FREEBSD_NR_kqueue] = (0x0),
	[TARGET_FREEBSD_NR_extattr_set_fd] = (0x0 | 0x4 | 0x8),
	[TARGET_FREEBSD_NR_extattr_get_fd] = (0x0 | 0x4 | 0x8),
	[TARGET_FREEBSD_NR_extattr_delete_fd] = (0x0 | 0x4),
	[TARGET_FREEBSD_NR___setugid] = (0x0),
	[TARGET_FREEBSD_NR_eaccess] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_afs3_syscall] = (0x0),
	[TARGET_FREEBSD_NR_nmount] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR___mac_get_proc] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR___mac_set_proc] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR___mac_get_fd] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR___mac_get_file] = (0x0 | 0x1 | 0x2),
	[TARGET_FREEBSD_NR___mac_set_fd] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR___mac_set_file] = (0x0 | 0x1 | 0x2),
	[TARGET_FREEBSD_NR_kenv] = (0x0 | 0x2 | 0x4),
	[TARGET_FREEBSD_NR_lchflags] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_uuidgen] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_sendfile] = (0x0 | 0x10 | 0x20),
	[TARGET_FREEBSD_NR_mac_syscall] = (0x0 | 0x1 | 0x4),
	[TARGET_FREEBSD_NR_ksem_close] = (0x0),
	[TARGET_FREEBSD_NR_ksem_post] = (0x0),
	[TARGET_FREEBSD_NR_ksem_wait] = (0x0),
	[TARGET_FREEBSD_NR_ksem_trywait] = (0x0),
	[TARGET_FREEBSD_NR_ksem_init] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_ksem_open] = (0x0 | 0x1 | 0x2),
	[TARGET_FREEBSD_NR_ksem_unlink] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_ksem_getvalue] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_ksem_destroy] = (0x0),
	[TARGET_FREEBSD_NR___mac_get_pid] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR___mac_get_link] = (0x0 | 0x1 | 0x2),
	[TARGET_FREEBSD_NR___mac_set_link] = (0x0 | 0x1 | 0x2),
	[TARGET_FREEBSD_NR_extattr_set_link] = (0x0 | 0x1 | 0x4 | 0x8),
	[TARGET_FREEBSD_NR_extattr_get_link] = (0x0 | 0x1 | 0x4 | 0x8),
	[TARGET_FREEBSD_NR_extattr_delete_link] = (0x0 | 0x1 | 0x4),
	[TARGET_FREEBSD_NR___mac_execve] = (0x0 | 0x1 | 0x2 | 0x4 | 0x8),
	[TARGET_FREEBSD_NR_sigaction] = (0x0 | 0x2 | 0x4),
	[TARGET_FREEBSD_NR_sigreturn] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_getcontext] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_setcontext] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_swapcontext] = (0x0 | 0x1 | 0x2),
	[TARGET_FREEBSD_NR_swapoff] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR___acl_get_link] = (0x0 | 0x1 | 0x4),
	[TARGET_FREEBSD_NR___acl_set_link] = (0x0 | 0x1 | 0x4),
	[TARGET_FREEBSD_NR___acl_delete_link] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR___acl_aclcheck_link] = (0x0 | 0x1 | 0x4),
	[TARGET_FREEBSD_NR_sigwait] = (0x0 | 0x1 | 0x2),
	[TARGET_FREEBSD_NR_thr_create] = (0x0 | 0x1 | 0x2),
	[TARGET_FREEBSD_NR_thr_exit] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_thr_self] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_thr_kill] = (0x0),
	[TARGET_FREEBSD_NR_jail_attach] = (0x0),
	[TARGET_FREEBSD_NR_extattr_list_fd] = (0x0 | 0x4),
	[TARGET_FREEBSD_NR_extattr_list_file] = (0x0 | 0x1 | 0x4),
	[TARGET_FREEBSD_NR_extattr_list_link] = (0x0 | 0x1 | 0x4),
	[TARGET_FREEBSD_NR_ksem_timedwait] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_thr_suspend] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_thr_wake] = (0x0),
	[TARGET_FREEBSD_NR_kldunloadf] = (0x0),
	[TARGET_FREEBSD_NR_audit] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_auditon] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_getauid] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_setauid] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_getaudit] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_setaudit] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_getaudit_addr] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_setaudit_addr] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_auditctl] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR__umtx_op] = (0x0 | 0x1 | 0x8 | 0x10),
	[TARGET_FREEBSD_NR_thr_new] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_sigqueue] = (0x0 | 0x4),
	[TARGET_FREEBSD_NR_kmq_open] = (0x0 | 0x1 | 0x8),
	[TARGET_FREEBSD_NR_kmq_setattr] = (0x0 | 0x2 | 0x4),
	[TARGET_FREEBSD_NR_kmq_timedreceive] = (0x0 | 0x2 | 0x8 | 0x10),
	[TARGET_FREEBSD_NR_kmq_timedsend] = (0x0 | 0x2 | 0x10),
	[TARGET_FREEBSD_NR_kmq_notify] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_kmq_unlink] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_abort2] = (0x0 | 0x1 | 0x4),
	[TARGET_FREEBSD_NR_thr_set_name] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_aio_fsync] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_rtprio_thread] = (0x0 | 0x4),
	[TARGET_FREEBSD_NR_sctp_peeloff] = (0x0),
	[TARGET_FREEBSD_NR_sctp_generic_sendmsg] = (0x0 | 0x2 | 0x8 | 0x20),
	[TARGET_FREEBSD_NR_sctp_generic_sendmsg_iov] = (0x0 | 0x2 | 0x8 | 0x20),
	[TARGET_FREEBSD_NR_sctp_generic_recvmsg] = (0x0 | 0x2 | 0x8 | 0x10 | 0x20 | 0x40),
	[TARGET_FREEBSD_NR_pread] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_pwrite] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_mmap] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_lseek] = (0x0),
	[TARGET_FREEBSD_NR_truncate] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_ftruncate] = (0x0),
	[TARGET_FREEBSD_NR_thr_kill2] = (0x0),
	[TARGET_FREEBSD_NR_shm_unlink] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_cpuset] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_cpuset_setid] = (0x0),
	[TARGET_FREEBSD_NR_cpuset_getid] = (0x0 | 0x8),
	[TARGET_FREEBSD_NR_cpuset_getaffinity] = (0x0 | 0x10),
	[TARGET_FREEBSD_NR_cpuset_setaffinity] = (0x0 | 0x10),
	[TARGET_FREEBSD_NR_faccessat] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_fchmodat] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_fchownat] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_fexecve] = (0x0 | 0x2 | 0x4),
	[TARGET_FREEBSD_NR_futimesat] = (0x0 | 0x2 | 0x4),
	[TARGET_FREEBSD_NR_linkat] = (0x0 | 0x2 | 0x8),
	[TARGET_FREEBSD_NR_mkdirat] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_mkfifoat] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_openat] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_readlinkat] = (0x0 | 0x2 | 0x4),
	[TARGET_FREEBSD_NR_renameat] = (0x0 | 0x2 | 0x8),
	[TARGET_FREEBSD_NR_symlinkat] = (0x0 | 0x1 | 0x4),
	[TARGET_FREEBSD_NR_unlinkat] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_posix_openpt] = (0x0),
	[TARGET_FREEBSD_NR_gssd_syscall] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_jail_get] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_jail_set] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_jail_remove] = (0x0),
	[TARGET_FREEBSD_NR___semctl] = (0x0 | 0x8),
	[TARGET_FREEBSD_NR_msgctl] = (0x0 | 0x4),
	[TARGET_FREEBSD_NR_shmctl] = (0x0 | 0x4),
	[TARGET_FREEBSD_NR_lpathconf] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR___cap_rights_get] = (0x0 | 0x4),
	[TARGET_FREEBSD_NR_cap_enter] = (0x0),
	[TARGET_FREEBSD_NR_cap_getmode] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_pdfork] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_pdkill] = (0x0),
	[TARGET_FREEBSD_NR_pdgetpid] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_pselect] = (0x0 | 0x2 | 0x4 | 0x8 | 0x10 | 0x20),
	[TARGET_FREEBSD_NR_getloginclass] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_setloginclass] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_rctl_get_racct] = (0x0 | 0x1 | 0x4),
	[TARGET_FREEBSD_NR_rctl_get_rules] = (0x0 | 0x1 | 0x4),
	[TARGET_FREEBSD_NR_rctl_get_limits] = (0x0 | 0x1 | 0x4),
	[TARGET_FREEBSD_NR_rctl_add_rule] = (0x0 | 0x1 | 0x4),
	[TARGET_FREEBSD_NR_rctl_remove_rule] = (0x0 | 0x1 | 0x4),
	[TARGET_FREEBSD_NR_posix_fallocate] = (0x0),
	[TARGET_FREEBSD_NR_posix_fadvise] = (0x0),
	[TARGET_FREEBSD_NR_wait6] = (0x0 | 0x4 | 0x10 | 0x20),
	[TARGET_FREEBSD_NR_cap_rights_limit] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_cap_ioctls_limit] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_cap_ioctls_get] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_cap_fcntls_limit] = (0x0),
	[TARGET_FREEBSD_NR_cap_fcntls_get] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_bindat] = (0x0 | 0x4),
	[TARGET_FREEBSD_NR_connectat] = (0x0 | 0x4),
	[TARGET_FREEBSD_NR_chflagsat] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_accept4] = (0x0 | 0x2 | 0x4),
	[TARGET_FREEBSD_NR_pipe2] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_aio_mlock] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_procctl] = (0x0 | 0x8),
	[TARGET_FREEBSD_NR_ppoll] = (0x0 | 0x1 | 0x4 | 0x8),
	[TARGET_FREEBSD_NR_futimens] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_utimensat] = (0x0 | 0x2 | 0x4),
	[TARGET_FREEBSD_NR_fdatasync] = (0x0),
	[TARGET_FREEBSD_NR_fstat] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_fstatat] = (0x0 | 0x2 | 0x4),
	[TARGET_FREEBSD_NR_fhstat] = (0x0 | 0x1 | 0x2),
	[TARGET_FREEBSD_NR_getdirentries] = (0x0 | 0x2 | 0x8),
	[TARGET_FREEBSD_NR_statfs] = (0x0 | 0x1 | 0x2),
	[TARGET_FREEBSD_NR_fstatfs] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_getfsstat] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_fhstatfs] = (0x0 | 0x1 | 0x2),
	[TARGET_FREEBSD_NR_mknodat] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_kevent] = (0x0 | 0x2 | 0x8 | 0x20),
	[TARGET_FREEBSD_NR_cpuset_getdomain] = (0x0 | 0x10 | 0x20),
	[TARGET_FREEBSD_NR_cpuset_setdomain] = (0x0 | 0x10),
	[TARGET_FREEBSD_NR_getrandom] = (0x0 | 0x1),
	[TARGET_FREEBSD_NR_getfhat] = (0x0 | 0x2 | 0x4),
	[TARGET_FREEBSD_NR_fhlink] = (0x0 | 0x1 | 0x2),
	[TARGET_FREEBSD_NR_fhlinkat] = (0x0 | 0x1 | 0x4),
	[TARGET_FREEBSD_NR_fhreadlink] = (0x0 | 0x1 | 0x2),
	[TARGET_FREEBSD_NR_funlinkat] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR_copy_file_range] = (0x0 | 0x2 | 0x8),
	[TARGET_FREEBSD_NR___sysctlbyname] = (0x0 | 0x1 | 0x4 | 0x8 | 0x10),
	[TARGET_FREEBSD_NR_shm_open2] = (0x0 | 0x1 | 0x10),
	[TARGET_FREEBSD_NR_shm_rename] = (0x0 | 0x1 | 0x2),
	[TARGET_FREEBSD_NR_sigfastblock] = (0x0 | 0x2),
	[TARGET_FREEBSD_NR___realpathat] = (0x0 | 0x2 | 0x4),
	[TARGET_FREEBSD_NR_close_range] = (0x0),
	[TARGET_FREEBSD_NR_rpctls_syscall] = (0x0 | 0x2),
};

#endif /* !_SYS_SYSARGMAP_H_ */