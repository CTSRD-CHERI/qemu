/*
 * System call argument to DTrace register array converstion.
 *
 * DO NOT EDIT-- this file is automatically @generated.
 * $FreeBSD$
 * This file is part of the DTrace syscall provider.
 */

static void
systrace_args(int sysnum, void *params, uint64_t *uarg, int *n_args)
{
	int64_t *iarg  = (int64_t *) uarg;
	switch (sysnum) {
	/* nosys */
	case 0: {
		*n_args = 0;
		break;
	}
	/* sys_exit */
	case 1: {
		struct target_sys_exit_args *p = params;
		iarg[0] = p->rval; /* int */
		*n_args = 1;
		break;
	}
	/* fork */
	case 2: {
		*n_args = 0;
		break;
	}
	/* read */
	case 3: {
		struct target_read_args *p = params;
		iarg[0] = p->fd; /* int */
		uarg[1] = (intptr_t) p->buf; /* abi_uintptr_t */
		uarg[2] = p->nbyte; /* size_t */
		*n_args = 3;
		break;
	}
	/* write */
	case 4: {
		struct target_write_args *p = params;
		iarg[0] = p->fd; /* int */
		uarg[1] = (intptr_t) p->buf; /* abi_uintptr_t */
		uarg[2] = p->nbyte; /* size_t */
		*n_args = 3;
		break;
	}
	/* open */
	case 5: {
		struct target_open_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		iarg[1] = p->flags; /* int */
		iarg[2] = p->mode; /* mode_t */
		*n_args = 3;
		break;
	}
	/* close */
	case 6: {
		struct target_close_args *p = params;
		iarg[0] = p->fd; /* int */
		*n_args = 1;
		break;
	}
	/* wait4 */
	case 7: {
		struct target_wait4_args *p = params;
		iarg[0] = p->pid; /* int */
		uarg[1] = (intptr_t) p->status; /* abi_uintptr_t */
		iarg[2] = p->options; /* int */
		uarg[3] = (intptr_t) p->rusage; /* abi_uintptr_t */
		*n_args = 4;
		break;
	}
	/* link */
	case 9: {
		struct target_link_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		uarg[1] = (intptr_t) p->to; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* unlink */
	case 10: {
		struct target_unlink_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		*n_args = 1;
		break;
	}
	/* chdir */
	case 12: {
		struct target_chdir_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		*n_args = 1;
		break;
	}
	/* fchdir */
	case 13: {
		struct target_fchdir_args *p = params;
		iarg[0] = p->fd; /* int */
		*n_args = 1;
		break;
	}
	/* chmod */
	case 15: {
		struct target_chmod_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		iarg[1] = p->mode; /* mode_t */
		*n_args = 2;
		break;
	}
	/* chown */
	case 16: {
		struct target_chown_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		iarg[1] = p->uid; /* int */
		iarg[2] = p->gid; /* int */
		*n_args = 3;
		break;
	}
	/* break */
	case 17: {
		struct target_break_args *p = params;
		uarg[0] = (intptr_t) p->nsize; /* abi_uintptr_t */
		*n_args = 1;
		break;
	}
	/* getpid */
	case 20: {
		*n_args = 0;
		break;
	}
	/* mount */
	case 21: {
		struct target_mount_args *p = params;
		uarg[0] = (intptr_t) p->type; /* abi_uintptr_t */
		uarg[1] = (intptr_t) p->path; /* abi_uintptr_t */
		iarg[2] = p->flags; /* int */
		uarg[3] = (intptr_t) p->data; /* abi_uintptr_t */
		*n_args = 4;
		break;
	}
	/* unmount */
	case 22: {
		struct target_unmount_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		iarg[1] = p->flags; /* int */
		*n_args = 2;
		break;
	}
	/* setuid */
	case 23: {
		struct target_setuid_args *p = params;
		uarg[0] = p->uid; /* uid_t */
		*n_args = 1;
		break;
	}
	/* getuid */
	case 24: {
		*n_args = 0;
		break;
	}
	/* geteuid */
	case 25: {
		*n_args = 0;
		break;
	}
	/* ptrace */
	case 26: {
		struct target_ptrace_args *p = params;
		iarg[0] = p->req; /* int */
		iarg[1] = p->pid; /* pid_t */
		uarg[2] = (intptr_t) p->addr; /* abi_uintptr_t */
		iarg[3] = p->data; /* int */
		*n_args = 4;
		break;
	}
	/* recvmsg */
	case 27: {
		struct target_recvmsg_args *p = params;
		iarg[0] = p->s; /* int */
		uarg[1] = (intptr_t) p->msg; /* abi_uintptr_t */
		iarg[2] = p->flags; /* int */
		*n_args = 3;
		break;
	}
	/* sendmsg */
	case 28: {
		struct target_sendmsg_args *p = params;
		iarg[0] = p->s; /* int */
		uarg[1] = (intptr_t) p->msg; /* abi_uintptr_t */
		iarg[2] = p->flags; /* int */
		*n_args = 3;
		break;
	}
	/* recvfrom */
	case 29: {
		struct target_recvfrom_args *p = params;
		iarg[0] = p->s; /* int */
		uarg[1] = (intptr_t) p->buf; /* abi_uintptr_t */
		uarg[2] = p->len; /* size_t */
		iarg[3] = p->flags; /* int */
		uarg[4] = (intptr_t) p->from; /* abi_uintptr_t */
		uarg[5] = (intptr_t) p->fromlenaddr; /* abi_uintptr_t */
		*n_args = 6;
		break;
	}
	/* accept */
	case 30: {
		struct target_accept_args *p = params;
		iarg[0] = p->s; /* int */
		uarg[1] = (intptr_t) p->name; /* abi_uintptr_t */
		uarg[2] = (intptr_t) p->anamelen; /* abi_uintptr_t */
		*n_args = 3;
		break;
	}
	/* getpeername */
	case 31: {
		struct target_getpeername_args *p = params;
		iarg[0] = p->fdes; /* int */
		uarg[1] = (intptr_t) p->asa; /* abi_uintptr_t */
		uarg[2] = (intptr_t) p->alen; /* abi_uintptr_t */
		*n_args = 3;
		break;
	}
	/* getsockname */
	case 32: {
		struct target_getsockname_args *p = params;
		iarg[0] = p->fdes; /* int */
		uarg[1] = (intptr_t) p->asa; /* abi_uintptr_t */
		uarg[2] = (intptr_t) p->alen; /* abi_uintptr_t */
		*n_args = 3;
		break;
	}
	/* access */
	case 33: {
		struct target_access_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		iarg[1] = p->amode; /* int */
		*n_args = 2;
		break;
	}
	/* chflags */
	case 34: {
		struct target_chflags_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		uarg[1] = p->flags; /* u_long */
		*n_args = 2;
		break;
	}
	/* fchflags */
	case 35: {
		struct target_fchflags_args *p = params;
		iarg[0] = p->fd; /* int */
		uarg[1] = p->flags; /* u_long */
		*n_args = 2;
		break;
	}
	/* sync */
	case 36: {
		*n_args = 0;
		break;
	}
	/* kill */
	case 37: {
		struct target_kill_args *p = params;
		iarg[0] = p->pid; /* int */
		iarg[1] = p->signum; /* int */
		*n_args = 2;
		break;
	}
	/* getppid */
	case 39: {
		*n_args = 0;
		break;
	}
	/* dup */
	case 41: {
		struct target_dup_args *p = params;
		uarg[0] = p->fd; /* u_int */
		*n_args = 1;
		break;
	}
	/* getegid */
	case 43: {
		*n_args = 0;
		break;
	}
	/* profil */
	case 44: {
		struct target_profil_args *p = params;
		uarg[0] = (intptr_t) p->samples; /* abi_uintptr_t */
		uarg[1] = p->size; /* size_t */
		uarg[2] = p->offset; /* size_t */
		uarg[3] = p->scale; /* u_int */
		*n_args = 4;
		break;
	}
	/* ktrace */
	case 45: {
		struct target_ktrace_args *p = params;
		uarg[0] = (intptr_t) p->fname; /* abi_uintptr_t */
		iarg[1] = p->ops; /* int */
		iarg[2] = p->facs; /* int */
		iarg[3] = p->pid; /* int */
		*n_args = 4;
		break;
	}
	/* getgid */
	case 47: {
		*n_args = 0;
		break;
	}
	/* getlogin */
	case 49: {
		struct target_getlogin_args *p = params;
		uarg[0] = (intptr_t) p->namebuf; /* abi_uintptr_t */
		uarg[1] = p->namelen; /* u_int */
		*n_args = 2;
		break;
	}
	/* setlogin */
	case 50: {
		struct target_setlogin_args *p = params;
		uarg[0] = (intptr_t) p->namebuf; /* abi_uintptr_t */
		*n_args = 1;
		break;
	}
	/* acct */
	case 51: {
		struct target_acct_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		*n_args = 1;
		break;
	}
	/* sigaltstack */
	case 53: {
		struct target_sigaltstack_args *p = params;
		uarg[0] = (intptr_t) p->ss; /* abi_uintptr_t */
		uarg[1] = (intptr_t) p->oss; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* ioctl */
	case 54: {
		struct target_ioctl_args *p = params;
		iarg[0] = p->fd; /* int */
		uarg[1] = p->com; /* u_long */
		uarg[2] = (intptr_t) p->data; /* abi_uintptr_t */
		*n_args = 3;
		break;
	}
	/* reboot */
	case 55: {
		struct target_reboot_args *p = params;
		iarg[0] = p->opt; /* int */
		*n_args = 1;
		break;
	}
	/* revoke */
	case 56: {
		struct target_revoke_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		*n_args = 1;
		break;
	}
	/* symlink */
	case 57: {
		struct target_symlink_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		uarg[1] = (intptr_t) p->link; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* readlink */
	case 58: {
		struct target_readlink_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		uarg[1] = (intptr_t) p->buf; /* abi_uintptr_t */
		uarg[2] = p->count; /* size_t */
		*n_args = 3;
		break;
	}
	/* execve */
	case 59: {
		struct target_execve_args *p = params;
		uarg[0] = (intptr_t) p->fname; /* abi_uintptr_t */
		uarg[1] = (intptr_t) p->argv; /* abi_uintptr_t */
		uarg[2] = (intptr_t) p->envv; /* abi_uintptr_t */
		*n_args = 3;
		break;
	}
	/* umask */
	case 60: {
		struct target_umask_args *p = params;
		iarg[0] = p->newmask; /* mode_t */
		*n_args = 1;
		break;
	}
	/* chroot */
	case 61: {
		struct target_chroot_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		*n_args = 1;
		break;
	}
	/* msync */
	case 65: {
		struct target_msync_args *p = params;
		uarg[0] = (intptr_t) p->addr; /* abi_uintptr_t */
		uarg[1] = p->len; /* size_t */
		iarg[2] = p->flags; /* int */
		*n_args = 3;
		break;
	}
	/* vfork */
	case 66: {
		*n_args = 0;
		break;
	}
	/* sbrk */
	case 69: {
		struct target_sbrk_args *p = params;
		iarg[0] = p->incr; /* int */
		*n_args = 1;
		break;
	}
	/* sstk */
	case 70: {
		struct target_sstk_args *p = params;
		iarg[0] = p->incr; /* int */
		*n_args = 1;
		break;
	}
	/* munmap */
	case 73: {
		struct target_munmap_args *p = params;
		uarg[0] = (intptr_t) p->addr; /* abi_uintptr_t */
		uarg[1] = p->len; /* size_t */
		*n_args = 2;
		break;
	}
	/* mprotect */
	case 74: {
		struct target_mprotect_args *p = params;
		uarg[0] = (intptr_t) p->addr; /* abi_uintptr_t */
		uarg[1] = p->len; /* size_t */
		iarg[2] = p->prot; /* int */
		*n_args = 3;
		break;
	}
	/* madvise */
	case 75: {
		struct target_madvise_args *p = params;
		uarg[0] = (intptr_t) p->addr; /* abi_uintptr_t */
		uarg[1] = p->len; /* size_t */
		iarg[2] = p->behav; /* int */
		*n_args = 3;
		break;
	}
	/* mincore */
	case 78: {
		struct target_mincore_args *p = params;
		uarg[0] = (intptr_t) p->addr; /* abi_uintptr_t */
		uarg[1] = p->len; /* size_t */
		uarg[2] = (intptr_t) p->vec; /* abi_uintptr_t */
		*n_args = 3;
		break;
	}
	/* getgroups */
	case 79: {
		struct target_getgroups_args *p = params;
		uarg[0] = p->gidsetsize; /* u_int */
		uarg[1] = (intptr_t) p->gidset; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* setgroups */
	case 80: {
		struct target_setgroups_args *p = params;
		uarg[0] = p->gidsetsize; /* u_int */
		uarg[1] = (intptr_t) p->gidset; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* getpgrp */
	case 81: {
		*n_args = 0;
		break;
	}
	/* setpgid */
	case 82: {
		struct target_setpgid_args *p = params;
		iarg[0] = p->pid; /* int */
		iarg[1] = p->pgid; /* int */
		*n_args = 2;
		break;
	}
	/* setitimer */
	case 83: {
		struct target_setitimer_args *p = params;
		iarg[0] = p->which; /* int */
		uarg[1] = (intptr_t) p->itv; /* abi_uintptr_t */
		uarg[2] = (intptr_t) p->oitv; /* abi_uintptr_t */
		*n_args = 3;
		break;
	}
	/* swapon */
	case 85: {
		struct target_swapon_args *p = params;
		uarg[0] = (intptr_t) p->name; /* abi_uintptr_t */
		*n_args = 1;
		break;
	}
	/* getitimer */
	case 86: {
		struct target_getitimer_args *p = params;
		iarg[0] = p->which; /* int */
		uarg[1] = (intptr_t) p->itv; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* getdtablesize */
	case 89: {
		*n_args = 0;
		break;
	}
	/* dup2 */
	case 90: {
		struct target_dup2_args *p = params;
		uarg[0] = p->from; /* u_int */
		uarg[1] = p->to; /* u_int */
		*n_args = 2;
		break;
	}
	/* fcntl */
	case 92: {
		struct target_fcntl_args *p = params;
		iarg[0] = p->fd; /* int */
		iarg[1] = p->cmd; /* int */
		uarg[2] = (intptr_t) p->arg; /* abi_uintptr_t */
		*n_args = 3;
		break;
	}
	/* select */
	case 93: {
		struct target_select_args *p = params;
		iarg[0] = p->nd; /* int */
		uarg[1] = (intptr_t) p->in; /* abi_uintptr_t */
		uarg[2] = (intptr_t) p->ou; /* abi_uintptr_t */
		uarg[3] = (intptr_t) p->ex; /* abi_uintptr_t */
		uarg[4] = (intptr_t) p->tv; /* abi_uintptr_t */
		*n_args = 5;
		break;
	}
	/* fsync */
	case 95: {
		struct target_fsync_args *p = params;
		iarg[0] = p->fd; /* int */
		*n_args = 1;
		break;
	}
	/* setpriority */
	case 96: {
		struct target_setpriority_args *p = params;
		iarg[0] = p->which; /* int */
		iarg[1] = p->who; /* int */
		iarg[2] = p->prio; /* int */
		*n_args = 3;
		break;
	}
	/* socket */
	case 97: {
		struct target_socket_args *p = params;
		iarg[0] = p->domain; /* int */
		iarg[1] = p->type; /* int */
		iarg[2] = p->protocol; /* int */
		*n_args = 3;
		break;
	}
	/* connect */
	case 98: {
		struct target_connect_args *p = params;
		iarg[0] = p->s; /* int */
		uarg[1] = (intptr_t) p->name; /* abi_uintptr_t */
		iarg[2] = p->namelen; /* __socklen_t */
		*n_args = 3;
		break;
	}
	/* getpriority */
	case 100: {
		struct target_getpriority_args *p = params;
		iarg[0] = p->which; /* int */
		iarg[1] = p->who; /* int */
		*n_args = 2;
		break;
	}
	/* bind */
	case 104: {
		struct target_bind_args *p = params;
		iarg[0] = p->s; /* int */
		uarg[1] = (intptr_t) p->name; /* abi_uintptr_t */
		iarg[2] = p->namelen; /* __socklen_t */
		*n_args = 3;
		break;
	}
	/* setsockopt */
	case 105: {
		struct target_setsockopt_args *p = params;
		iarg[0] = p->s; /* int */
		iarg[1] = p->level; /* int */
		iarg[2] = p->name; /* int */
		uarg[3] = (intptr_t) p->val; /* abi_uintptr_t */
		iarg[4] = p->valsize; /* __socklen_t */
		*n_args = 5;
		break;
	}
	/* listen */
	case 106: {
		struct target_listen_args *p = params;
		iarg[0] = p->s; /* int */
		iarg[1] = p->backlog; /* int */
		*n_args = 2;
		break;
	}
	/* gettimeofday */
	case 116: {
		struct target_gettimeofday_args *p = params;
		uarg[0] = (intptr_t) p->tp; /* abi_uintptr_t */
		uarg[1] = (intptr_t) p->tzp; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* getrusage */
	case 117: {
		struct target_getrusage_args *p = params;
		iarg[0] = p->who; /* int */
		uarg[1] = (intptr_t) p->rusage; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* getsockopt */
	case 118: {
		struct target_getsockopt_args *p = params;
		iarg[0] = p->s; /* int */
		iarg[1] = p->level; /* int */
		iarg[2] = p->name; /* int */
		uarg[3] = (intptr_t) p->val; /* abi_uintptr_t */
		uarg[4] = (intptr_t) p->avalsize; /* abi_uintptr_t */
		*n_args = 5;
		break;
	}
	/* readv */
	case 120: {
		struct target_readv_args *p = params;
		iarg[0] = p->fd; /* int */
		uarg[1] = (intptr_t) p->iovp; /* abi_uintptr_t */
		uarg[2] = p->iovcnt; /* u_int */
		*n_args = 3;
		break;
	}
	/* writev */
	case 121: {
		struct target_writev_args *p = params;
		iarg[0] = p->fd; /* int */
		uarg[1] = (intptr_t) p->iovp; /* abi_uintptr_t */
		uarg[2] = p->iovcnt; /* u_int */
		*n_args = 3;
		break;
	}
	/* settimeofday */
	case 122: {
		struct target_settimeofday_args *p = params;
		uarg[0] = (intptr_t) p->tv; /* abi_uintptr_t */
		uarg[1] = (intptr_t) p->tzp; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* fchown */
	case 123: {
		struct target_fchown_args *p = params;
		iarg[0] = p->fd; /* int */
		iarg[1] = p->uid; /* int */
		iarg[2] = p->gid; /* int */
		*n_args = 3;
		break;
	}
	/* fchmod */
	case 124: {
		struct target_fchmod_args *p = params;
		iarg[0] = p->fd; /* int */
		iarg[1] = p->mode; /* mode_t */
		*n_args = 2;
		break;
	}
	/* setreuid */
	case 126: {
		struct target_setreuid_args *p = params;
		iarg[0] = p->ruid; /* int */
		iarg[1] = p->euid; /* int */
		*n_args = 2;
		break;
	}
	/* setregid */
	case 127: {
		struct target_setregid_args *p = params;
		iarg[0] = p->rgid; /* int */
		iarg[1] = p->egid; /* int */
		*n_args = 2;
		break;
	}
	/* rename */
	case 128: {
		struct target_rename_args *p = params;
		uarg[0] = (intptr_t) p->from; /* abi_uintptr_t */
		uarg[1] = (intptr_t) p->to; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* flock */
	case 131: {
		struct target_flock_args *p = params;
		iarg[0] = p->fd; /* int */
		iarg[1] = p->how; /* int */
		*n_args = 2;
		break;
	}
	/* mkfifo */
	case 132: {
		struct target_mkfifo_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		iarg[1] = p->mode; /* mode_t */
		*n_args = 2;
		break;
	}
	/* sendto */
	case 133: {
		struct target_sendto_args *p = params;
		iarg[0] = p->s; /* int */
		uarg[1] = (intptr_t) p->buf; /* abi_uintptr_t */
		uarg[2] = p->len; /* size_t */
		iarg[3] = p->flags; /* int */
		uarg[4] = (intptr_t) p->to; /* abi_uintptr_t */
		iarg[5] = p->tolen; /* __socklen_t */
		*n_args = 6;
		break;
	}
	/* shutdown */
	case 134: {
		struct target_shutdown_args *p = params;
		iarg[0] = p->s; /* int */
		iarg[1] = p->how; /* int */
		*n_args = 2;
		break;
	}
	/* socketpair */
	case 135: {
		struct target_socketpair_args *p = params;
		iarg[0] = p->domain; /* int */
		iarg[1] = p->type; /* int */
		iarg[2] = p->protocol; /* int */
		uarg[3] = (intptr_t) p->rsv; /* abi_uintptr_t */
		*n_args = 4;
		break;
	}
	/* mkdir */
	case 136: {
		struct target_mkdir_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		iarg[1] = p->mode; /* mode_t */
		*n_args = 2;
		break;
	}
	/* rmdir */
	case 137: {
		struct target_rmdir_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		*n_args = 1;
		break;
	}
	/* utimes */
	case 138: {
		struct target_utimes_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		uarg[1] = (intptr_t) p->tptr; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* adjtime */
	case 140: {
		struct target_adjtime_args *p = params;
		uarg[0] = (intptr_t) p->delta; /* abi_uintptr_t */
		uarg[1] = (intptr_t) p->olddelta; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* setsid */
	case 147: {
		*n_args = 0;
		break;
	}
	/* quotactl */
	case 148: {
		struct target_quotactl_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		iarg[1] = p->cmd; /* int */
		iarg[2] = p->uid; /* int */
		uarg[3] = (intptr_t) p->arg; /* abi_uintptr_t */
		*n_args = 4;
		break;
	}
	/* nlm_syscall */
	case 154: {
		struct target_nlm_syscall_args *p = params;
		iarg[0] = p->debug_level; /* int */
		iarg[1] = p->grace_period; /* int */
		iarg[2] = p->addr_count; /* int */
		uarg[3] = (intptr_t) p->addrs; /* abi_uintptr_t */
		*n_args = 4;
		break;
	}
	/* nfssvc */
	case 155: {
		struct target_nfssvc_args *p = params;
		iarg[0] = p->flag; /* int */
		uarg[1] = (intptr_t) p->argp; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* lgetfh */
	case 160: {
		struct target_lgetfh_args *p = params;
		uarg[0] = (intptr_t) p->fname; /* abi_uintptr_t */
		uarg[1] = (intptr_t) p->fhp; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* getfh */
	case 161: {
		struct target_getfh_args *p = params;
		uarg[0] = (intptr_t) p->fname; /* abi_uintptr_t */
		uarg[1] = (intptr_t) p->fhp; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* sysarch */
	case 165: {
		struct target_sysarch_args *p = params;
		iarg[0] = p->op; /* int */
		uarg[1] = (intptr_t) p->parms; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* rtprio */
	case 166: {
		struct target_rtprio_args *p = params;
		iarg[0] = p->function; /* int */
		iarg[1] = p->pid; /* pid_t */
		uarg[2] = (intptr_t) p->rtp; /* abi_uintptr_t */
		*n_args = 3;
		break;
	}
	/* semsys */
	case 169: {
		struct target_semsys_args *p = params;
		iarg[0] = p->which; /* int */
		uarg[1] = (intptr_t) p->a2; /* abi_uintptr_t */
		uarg[2] = (intptr_t) p->a3; /* abi_uintptr_t */
		uarg[3] = (intptr_t) p->a4; /* abi_uintptr_t */
		uarg[4] = (intptr_t) p->a5; /* abi_uintptr_t */
		*n_args = 5;
		break;
	}
	/* msgsys */
	case 170: {
		struct target_msgsys_args *p = params;
		iarg[0] = p->which; /* int */
		uarg[1] = (intptr_t) p->a2; /* abi_uintptr_t */
		uarg[2] = (intptr_t) p->a3; /* abi_uintptr_t */
		uarg[3] = (intptr_t) p->a4; /* abi_uintptr_t */
		uarg[4] = (intptr_t) p->a5; /* abi_uintptr_t */
		uarg[5] = (intptr_t) p->a6; /* abi_uintptr_t */
		*n_args = 6;
		break;
	}
	/* shmsys */
	case 171: {
		struct target_shmsys_args *p = params;
		iarg[0] = p->which; /* int */
		uarg[1] = (intptr_t) p->a2; /* abi_uintptr_t */
		uarg[2] = (intptr_t) p->a3; /* abi_uintptr_t */
		uarg[3] = (intptr_t) p->a4; /* abi_uintptr_t */
		*n_args = 4;
		break;
	}
	/* setfib */
	case 175: {
		struct target_setfib_args *p = params;
		iarg[0] = p->fibnum; /* int */
		*n_args = 1;
		break;
	}
	/* ntp_adjtime */
	case 176: {
		struct target_ntp_adjtime_args *p = params;
		uarg[0] = (intptr_t) p->tp; /* abi_uintptr_t */
		*n_args = 1;
		break;
	}
	/* setgid */
	case 181: {
		struct target_setgid_args *p = params;
		iarg[0] = p->gid; /* gid_t */
		*n_args = 1;
		break;
	}
	/* setegid */
	case 182: {
		struct target_setegid_args *p = params;
		iarg[0] = p->egid; /* gid_t */
		*n_args = 1;
		break;
	}
	/* seteuid */
	case 183: {
		struct target_seteuid_args *p = params;
		uarg[0] = p->euid; /* uid_t */
		*n_args = 1;
		break;
	}
	/* pathconf */
	case 191: {
		struct target_pathconf_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		iarg[1] = p->name; /* int */
		*n_args = 2;
		break;
	}
	/* fpathconf */
	case 192: {
		struct target_fpathconf_args *p = params;
		iarg[0] = p->fd; /* int */
		iarg[1] = p->name; /* int */
		*n_args = 2;
		break;
	}
	/* getrlimit */
	case 194: {
		struct target___getrlimit_args *p = params;
		uarg[0] = p->which; /* u_int */
		uarg[1] = (intptr_t) p->rlp; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* setrlimit */
	case 195: {
		struct target___setrlimit_args *p = params;
		uarg[0] = p->which; /* u_int */
		uarg[1] = (intptr_t) p->rlp; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* nosys */
	case 198: {
		*n_args = 0;
		break;
	}
	/* __sysctl */
	case 202: {
		struct target___sysctl_args *p = params;
		uarg[0] = (intptr_t) p->name; /* abi_uintptr_t */
		uarg[1] = p->namelen; /* u_int */
		uarg[2] = (intptr_t) p->old; /* abi_uintptr_t */
		uarg[3] = (intptr_t) p->oldlenp; /* abi_uintptr_t */
		uarg[4] = (intptr_t) p->new; /* abi_uintptr_t */
		uarg[5] = p->newlen; /* size_t */
		*n_args = 6;
		break;
	}
	/* mlock */
	case 203: {
		struct target_mlock_args *p = params;
		uarg[0] = (intptr_t) p->addr; /* abi_uintptr_t */
		uarg[1] = p->len; /* size_t */
		*n_args = 2;
		break;
	}
	/* munlock */
	case 204: {
		struct target_munlock_args *p = params;
		uarg[0] = (intptr_t) p->addr; /* abi_uintptr_t */
		uarg[1] = p->len; /* size_t */
		*n_args = 2;
		break;
	}
	/* undelete */
	case 205: {
		struct target_undelete_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		*n_args = 1;
		break;
	}
	/* futimes */
	case 206: {
		struct target_futimes_args *p = params;
		iarg[0] = p->fd; /* int */
		uarg[1] = (intptr_t) p->tptr; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* getpgid */
	case 207: {
		struct target_getpgid_args *p = params;
		iarg[0] = p->pid; /* pid_t */
		*n_args = 1;
		break;
	}
	/* poll */
	case 209: {
		struct target_poll_args *p = params;
		uarg[0] = (intptr_t) p->fds; /* abi_uintptr_t */
		uarg[1] = p->nfds; /* u_int */
		iarg[2] = p->timeout; /* int */
		*n_args = 3;
		break;
	}
	/* lkmnosys */
	case 210: {
		*n_args = 0;
		break;
	}
	/* lkmnosys */
	case 211: {
		*n_args = 0;
		break;
	}
	/* lkmnosys */
	case 212: {
		*n_args = 0;
		break;
	}
	/* lkmnosys */
	case 213: {
		*n_args = 0;
		break;
	}
	/* lkmnosys */
	case 214: {
		*n_args = 0;
		break;
	}
	/* lkmnosys */
	case 215: {
		*n_args = 0;
		break;
	}
	/* lkmnosys */
	case 216: {
		*n_args = 0;
		break;
	}
	/* lkmnosys */
	case 217: {
		*n_args = 0;
		break;
	}
	/* lkmnosys */
	case 218: {
		*n_args = 0;
		break;
	}
	/* lkmnosys */
	case 219: {
		*n_args = 0;
		break;
	}
	/* semget */
	case 221: {
		struct target_semget_args *p = params;
		iarg[0] = p->key; /* key_t */
		iarg[1] = p->nsems; /* int */
		iarg[2] = p->semflg; /* int */
		*n_args = 3;
		break;
	}
	/* semop */
	case 222: {
		struct target_semop_args *p = params;
		iarg[0] = p->semid; /* int */
		uarg[1] = (intptr_t) p->sops; /* abi_uintptr_t */
		uarg[2] = p->nsops; /* size_t */
		*n_args = 3;
		break;
	}
	/* msgget */
	case 225: {
		struct target_msgget_args *p = params;
		iarg[0] = p->key; /* key_t */
		iarg[1] = p->msgflg; /* int */
		*n_args = 2;
		break;
	}
	/* msgsnd */
	case 226: {
		struct target_msgsnd_args *p = params;
		iarg[0] = p->msqid; /* int */
		uarg[1] = (intptr_t) p->msgp; /* abi_uintptr_t */
		uarg[2] = p->msgsz; /* size_t */
		iarg[3] = p->msgflg; /* int */
		*n_args = 4;
		break;
	}
	/* msgrcv */
	case 227: {
		struct target_msgrcv_args *p = params;
		iarg[0] = p->msqid; /* int */
		uarg[1] = (intptr_t) p->msgp; /* abi_uintptr_t */
		uarg[2] = p->msgsz; /* size_t */
		iarg[3] = p->msgtyp; /* long */
		iarg[4] = p->msgflg; /* int */
		*n_args = 5;
		break;
	}
	/* shmat */
	case 228: {
		struct target_shmat_args *p = params;
		iarg[0] = p->shmid; /* int */
		uarg[1] = (intptr_t) p->shmaddr; /* abi_uintptr_t */
		iarg[2] = p->shmflg; /* int */
		*n_args = 3;
		break;
	}
	/* shmdt */
	case 230: {
		struct target_shmdt_args *p = params;
		uarg[0] = (intptr_t) p->shmaddr; /* abi_uintptr_t */
		*n_args = 1;
		break;
	}
	/* shmget */
	case 231: {
		struct target_shmget_args *p = params;
		iarg[0] = p->key; /* key_t */
		uarg[1] = p->size; /* size_t */
		iarg[2] = p->shmflg; /* int */
		*n_args = 3;
		break;
	}
	/* clock_gettime */
	case 232: {
		struct target_clock_gettime_args *p = params;
		iarg[0] = p->clock_id; /* clockid_t */
		uarg[1] = (intptr_t) p->tp; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* clock_settime */
	case 233: {
		struct target_clock_settime_args *p = params;
		iarg[0] = p->clock_id; /* clockid_t */
		uarg[1] = (intptr_t) p->tp; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* clock_getres */
	case 234: {
		struct target_clock_getres_args *p = params;
		iarg[0] = p->clock_id; /* clockid_t */
		uarg[1] = (intptr_t) p->tp; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* ktimer_create */
	case 235: {
		struct target_ktimer_create_args *p = params;
		iarg[0] = p->clock_id; /* clockid_t */
		uarg[1] = (intptr_t) p->evp; /* abi_uintptr_t */
		uarg[2] = (intptr_t) p->timerid; /* abi_uintptr_t */
		*n_args = 3;
		break;
	}
	/* ktimer_delete */
	case 236: {
		struct target_ktimer_delete_args *p = params;
		iarg[0] = p->timerid; /* int */
		*n_args = 1;
		break;
	}
	/* ktimer_settime */
	case 237: {
		struct target_ktimer_settime_args *p = params;
		iarg[0] = p->timerid; /* int */
		iarg[1] = p->flags; /* int */
		uarg[2] = (intptr_t) p->value; /* abi_uintptr_t */
		uarg[3] = (intptr_t) p->ovalue; /* abi_uintptr_t */
		*n_args = 4;
		break;
	}
	/* ktimer_gettime */
	case 238: {
		struct target_ktimer_gettime_args *p = params;
		iarg[0] = p->timerid; /* int */
		uarg[1] = (intptr_t) p->value; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* ktimer_getoverrun */
	case 239: {
		struct target_ktimer_getoverrun_args *p = params;
		iarg[0] = p->timerid; /* int */
		*n_args = 1;
		break;
	}
	/* nanosleep */
	case 240: {
		struct target_nanosleep_args *p = params;
		uarg[0] = (intptr_t) p->rqtp; /* abi_uintptr_t */
		uarg[1] = (intptr_t) p->rmtp; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* ffclock_getcounter */
	case 241: {
		struct target_ffclock_getcounter_args *p = params;
		uarg[0] = (intptr_t) p->ffcount; /* abi_uintptr_t */
		*n_args = 1;
		break;
	}
	/* ffclock_setestimate */
	case 242: {
		struct target_ffclock_setestimate_args *p = params;
		uarg[0] = (intptr_t) p->cest; /* abi_uintptr_t */
		*n_args = 1;
		break;
	}
	/* ffclock_getestimate */
	case 243: {
		struct target_ffclock_getestimate_args *p = params;
		uarg[0] = (intptr_t) p->cest; /* abi_uintptr_t */
		*n_args = 1;
		break;
	}
	/* clock_nanosleep */
	case 244: {
		struct target_clock_nanosleep_args *p = params;
		iarg[0] = p->clock_id; /* clockid_t */
		iarg[1] = p->flags; /* int */
		uarg[2] = (intptr_t) p->rqtp; /* abi_uintptr_t */
		uarg[3] = (intptr_t) p->rmtp; /* abi_uintptr_t */
		*n_args = 4;
		break;
	}
	/* clock_getcpuclockid2 */
	case 247: {
		struct target_clock_getcpuclockid2_args *p = params;
		iarg[0] = p->id; /* id_t */
		iarg[1] = p->which; /* int */
		uarg[2] = (intptr_t) p->clock_id; /* abi_uintptr_t */
		*n_args = 3;
		break;
	}
	/* ntp_gettime */
	case 248: {
		struct target_ntp_gettime_args *p = params;
		uarg[0] = (intptr_t) p->ntvp; /* abi_uintptr_t */
		*n_args = 1;
		break;
	}
	/* minherit */
	case 250: {
		struct target_minherit_args *p = params;
		uarg[0] = (intptr_t) p->addr; /* abi_uintptr_t */
		uarg[1] = p->len; /* size_t */
		iarg[2] = p->inherit; /* int */
		*n_args = 3;
		break;
	}
	/* rfork */
	case 251: {
		struct target_rfork_args *p = params;
		iarg[0] = p->flags; /* int */
		*n_args = 1;
		break;
	}
	/* issetugid */
	case 253: {
		*n_args = 0;
		break;
	}
	/* lchown */
	case 254: {
		struct target_lchown_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		iarg[1] = p->uid; /* int */
		iarg[2] = p->gid; /* int */
		*n_args = 3;
		break;
	}
	/* aio_read */
	case 255: {
		struct target_aio_read_args *p = params;
		uarg[0] = (intptr_t) p->aiocbp; /* abi_uintptr_t */
		*n_args = 1;
		break;
	}
	/* aio_write */
	case 256: {
		struct target_aio_write_args *p = params;
		uarg[0] = (intptr_t) p->aiocbp; /* abi_uintptr_t */
		*n_args = 1;
		break;
	}
	/* lio_listio */
	case 257: {
		struct target_lio_listio_args *p = params;
		iarg[0] = p->mode; /* int */
		uarg[1] = (intptr_t) p->acb_list; /* abi_uintptr_t */
		iarg[2] = p->nent; /* int */
		uarg[3] = (intptr_t) p->sig; /* abi_uintptr_t */
		*n_args = 4;
		break;
	}
	/* kbounce */
	case 258: {
		struct target_kbounce_args *p = params;
		uarg[0] = (intptr_t) p->src; /* abi_uintptr_t */
		uarg[1] = (intptr_t) p->dst; /* abi_uintptr_t */
		uarg[2] = p->len; /* size_t */
		iarg[3] = p->flags; /* int */
		*n_args = 4;
		break;
	}
	/* flag_captured */
	case 259: {
		struct target_flag_captured_args *p = params;
		uarg[0] = (intptr_t) p->message; /* abi_uintptr_t */
		uarg[1] = p->key; /* uint32_t */
		*n_args = 2;
		break;
	}
	/* lchmod */
	case 274: {
		struct target_lchmod_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		iarg[1] = p->mode; /* mode_t */
		*n_args = 2;
		break;
	}
	/* lutimes */
	case 276: {
		struct target_lutimes_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		uarg[1] = (intptr_t) p->tptr; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* preadv */
	case 289: {
		struct target_preadv_args *p = params;
		iarg[0] = p->fd; /* int */
		uarg[1] = (intptr_t) p->iovp; /* abi_uintptr_t */
		uarg[2] = p->iovcnt; /* u_int */
		iarg[3] = p->offset; /* off_t */
		*n_args = 4;
		break;
	}
	/* pwritev */
	case 290: {
		struct target_pwritev_args *p = params;
		iarg[0] = p->fd; /* int */
		uarg[1] = (intptr_t) p->iovp; /* abi_uintptr_t */
		uarg[2] = p->iovcnt; /* u_int */
		iarg[3] = p->offset; /* off_t */
		*n_args = 4;
		break;
	}
	/* fhopen */
	case 298: {
		struct target_fhopen_args *p = params;
		uarg[0] = (intptr_t) p->u_fhp; /* abi_uintptr_t */
		iarg[1] = p->flags; /* int */
		*n_args = 2;
		break;
	}
	/* modnext */
	case 300: {
		struct target_modnext_args *p = params;
		iarg[0] = p->modid; /* int */
		*n_args = 1;
		break;
	}
	/* modstat */
	case 301: {
		struct target_modstat_args *p = params;
		iarg[0] = p->modid; /* int */
		uarg[1] = (intptr_t) p->stat; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* modfnext */
	case 302: {
		struct target_modfnext_args *p = params;
		iarg[0] = p->modid; /* int */
		*n_args = 1;
		break;
	}
	/* modfind */
	case 303: {
		struct target_modfind_args *p = params;
		uarg[0] = (intptr_t) p->name; /* abi_uintptr_t */
		*n_args = 1;
		break;
	}
	/* kldload */
	case 304: {
		struct target_kldload_args *p = params;
		uarg[0] = (intptr_t) p->file; /* abi_uintptr_t */
		*n_args = 1;
		break;
	}
	/* kldunload */
	case 305: {
		struct target_kldunload_args *p = params;
		iarg[0] = p->fileid; /* int */
		*n_args = 1;
		break;
	}
	/* kldfind */
	case 306: {
		struct target_kldfind_args *p = params;
		uarg[0] = (intptr_t) p->file; /* abi_uintptr_t */
		*n_args = 1;
		break;
	}
	/* kldnext */
	case 307: {
		struct target_kldnext_args *p = params;
		iarg[0] = p->fileid; /* int */
		*n_args = 1;
		break;
	}
	/* kldstat */
	case 308: {
		struct target_kldstat_args *p = params;
		iarg[0] = p->fileid; /* int */
		uarg[1] = (intptr_t) p->stat; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* kldfirstmod */
	case 309: {
		struct target_kldfirstmod_args *p = params;
		iarg[0] = p->fileid; /* int */
		*n_args = 1;
		break;
	}
	/* getsid */
	case 310: {
		struct target_getsid_args *p = params;
		iarg[0] = p->pid; /* pid_t */
		*n_args = 1;
		break;
	}
	/* setresuid */
	case 311: {
		struct target_setresuid_args *p = params;
		uarg[0] = p->ruid; /* uid_t */
		uarg[1] = p->euid; /* uid_t */
		uarg[2] = p->suid; /* uid_t */
		*n_args = 3;
		break;
	}
	/* setresgid */
	case 312: {
		struct target_setresgid_args *p = params;
		iarg[0] = p->rgid; /* gid_t */
		iarg[1] = p->egid; /* gid_t */
		iarg[2] = p->sgid; /* gid_t */
		*n_args = 3;
		break;
	}
	/* aio_return */
	case 314: {
		struct target_aio_return_args *p = params;
		uarg[0] = (intptr_t) p->aiocbp; /* abi_uintptr_t */
		*n_args = 1;
		break;
	}
	/* aio_suspend */
	case 315: {
		struct target_aio_suspend_args *p = params;
		uarg[0] = (intptr_t) p->aiocbp; /* abi_uintptr_t */
		iarg[1] = p->nent; /* int */
		uarg[2] = (intptr_t) p->timeout; /* abi_uintptr_t */
		*n_args = 3;
		break;
	}
	/* aio_cancel */
	case 316: {
		struct target_aio_cancel_args *p = params;
		iarg[0] = p->fd; /* int */
		uarg[1] = (intptr_t) p->aiocbp; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* aio_error */
	case 317: {
		struct target_aio_error_args *p = params;
		uarg[0] = (intptr_t) p->aiocbp; /* abi_uintptr_t */
		*n_args = 1;
		break;
	}
	/* yield */
	case 321: {
		*n_args = 0;
		break;
	}
	/* mlockall */
	case 324: {
		struct target_mlockall_args *p = params;
		iarg[0] = p->how; /* int */
		*n_args = 1;
		break;
	}
	/* munlockall */
	case 325: {
		*n_args = 0;
		break;
	}
	/* __getcwd */
	case 326: {
		struct target___getcwd_args *p = params;
		uarg[0] = (intptr_t) p->buf; /* abi_uintptr_t */
		uarg[1] = p->buflen; /* size_t */
		*n_args = 2;
		break;
	}
	/* sched_setparam */
	case 327: {
		struct target_sched_setparam_args *p = params;
		iarg[0] = p->pid; /* pid_t */
		uarg[1] = (intptr_t) p->param; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* sched_getparam */
	case 328: {
		struct target_sched_getparam_args *p = params;
		iarg[0] = p->pid; /* pid_t */
		uarg[1] = (intptr_t) p->param; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* sched_setscheduler */
	case 329: {
		struct target_sched_setscheduler_args *p = params;
		iarg[0] = p->pid; /* pid_t */
		iarg[1] = p->policy; /* int */
		uarg[2] = (intptr_t) p->param; /* abi_uintptr_t */
		*n_args = 3;
		break;
	}
	/* sched_getscheduler */
	case 330: {
		struct target_sched_getscheduler_args *p = params;
		iarg[0] = p->pid; /* pid_t */
		*n_args = 1;
		break;
	}
	/* sched_yield */
	case 331: {
		*n_args = 0;
		break;
	}
	/* sched_get_priority_max */
	case 332: {
		struct target_sched_get_priority_max_args *p = params;
		iarg[0] = p->policy; /* int */
		*n_args = 1;
		break;
	}
	/* sched_get_priority_min */
	case 333: {
		struct target_sched_get_priority_min_args *p = params;
		iarg[0] = p->policy; /* int */
		*n_args = 1;
		break;
	}
	/* sched_rr_get_interval */
	case 334: {
		struct target_sched_rr_get_interval_args *p = params;
		iarg[0] = p->pid; /* pid_t */
		uarg[1] = (intptr_t) p->interval; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* utrace */
	case 335: {
		struct target_utrace_args *p = params;
		uarg[0] = (intptr_t) p->addr; /* abi_uintptr_t */
		uarg[1] = p->len; /* size_t */
		*n_args = 2;
		break;
	}
	/* kldsym */
	case 337: {
		struct target_kldsym_args *p = params;
		iarg[0] = p->fileid; /* int */
		iarg[1] = p->cmd; /* int */
		uarg[2] = (intptr_t) p->data; /* abi_uintptr_t */
		*n_args = 3;
		break;
	}
	/* jail */
	case 338: {
		struct target_jail_args *p = params;
		uarg[0] = (intptr_t) p->jailp; /* abi_uintptr_t */
		*n_args = 1;
		break;
	}
	/* nnpfs_syscall */
	case 339: {
		struct target_nnpfs_syscall_args *p = params;
		iarg[0] = p->operation; /* int */
		uarg[1] = (intptr_t) p->a_pathP; /* abi_uintptr_t */
		iarg[2] = p->a_opcode; /* int */
		uarg[3] = (intptr_t) p->a_paramsP; /* abi_uintptr_t */
		iarg[4] = p->a_followSymlinks; /* int */
		*n_args = 5;
		break;
	}
	/* sigprocmask */
	case 340: {
		struct target_sigprocmask_args *p = params;
		iarg[0] = p->how; /* int */
		uarg[1] = (intptr_t) p->set; /* abi_uintptr_t */
		uarg[2] = (intptr_t) p->oset; /* abi_uintptr_t */
		*n_args = 3;
		break;
	}
	/* sigsuspend */
	case 341: {
		struct target_sigsuspend_args *p = params;
		uarg[0] = (intptr_t) p->sigmask; /* abi_uintptr_t */
		*n_args = 1;
		break;
	}
	/* sigpending */
	case 343: {
		struct target_sigpending_args *p = params;
		uarg[0] = (intptr_t) p->set; /* abi_uintptr_t */
		*n_args = 1;
		break;
	}
	/* sigtimedwait */
	case 345: {
		struct target_sigtimedwait_args *p = params;
		uarg[0] = (intptr_t) p->set; /* abi_uintptr_t */
		uarg[1] = (intptr_t) p->info; /* abi_uintptr_t */
		uarg[2] = (intptr_t) p->timeout; /* abi_uintptr_t */
		*n_args = 3;
		break;
	}
	/* sigwaitinfo */
	case 346: {
		struct target_sigwaitinfo_args *p = params;
		uarg[0] = (intptr_t) p->set; /* abi_uintptr_t */
		uarg[1] = (intptr_t) p->info; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* __acl_get_file */
	case 347: {
		struct target___acl_get_file_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		iarg[1] = p->type; /* acl_type_t */
		uarg[2] = (intptr_t) p->aclp; /* abi_uintptr_t */
		*n_args = 3;
		break;
	}
	/* __acl_set_file */
	case 348: {
		struct target___acl_set_file_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		iarg[1] = p->type; /* acl_type_t */
		uarg[2] = (intptr_t) p->aclp; /* abi_uintptr_t */
		*n_args = 3;
		break;
	}
	/* __acl_get_fd */
	case 349: {
		struct target___acl_get_fd_args *p = params;
		iarg[0] = p->filedes; /* int */
		iarg[1] = p->type; /* acl_type_t */
		uarg[2] = (intptr_t) p->aclp; /* abi_uintptr_t */
		*n_args = 3;
		break;
	}
	/* __acl_set_fd */
	case 350: {
		struct target___acl_set_fd_args *p = params;
		iarg[0] = p->filedes; /* int */
		iarg[1] = p->type; /* acl_type_t */
		uarg[2] = (intptr_t) p->aclp; /* abi_uintptr_t */
		*n_args = 3;
		break;
	}
	/* __acl_delete_file */
	case 351: {
		struct target___acl_delete_file_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		iarg[1] = p->type; /* acl_type_t */
		*n_args = 2;
		break;
	}
	/* __acl_delete_fd */
	case 352: {
		struct target___acl_delete_fd_args *p = params;
		iarg[0] = p->filedes; /* int */
		iarg[1] = p->type; /* acl_type_t */
		*n_args = 2;
		break;
	}
	/* __acl_aclcheck_file */
	case 353: {
		struct target___acl_aclcheck_file_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		iarg[1] = p->type; /* acl_type_t */
		uarg[2] = (intptr_t) p->aclp; /* abi_uintptr_t */
		*n_args = 3;
		break;
	}
	/* __acl_aclcheck_fd */
	case 354: {
		struct target___acl_aclcheck_fd_args *p = params;
		iarg[0] = p->filedes; /* int */
		iarg[1] = p->type; /* acl_type_t */
		uarg[2] = (intptr_t) p->aclp; /* abi_uintptr_t */
		*n_args = 3;
		break;
	}
	/* extattrctl */
	case 355: {
		struct target_extattrctl_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		iarg[1] = p->cmd; /* int */
		uarg[2] = (intptr_t) p->filename; /* abi_uintptr_t */
		iarg[3] = p->attrnamespace; /* int */
		uarg[4] = (intptr_t) p->attrname; /* abi_uintptr_t */
		*n_args = 5;
		break;
	}
	/* extattr_set_file */
	case 356: {
		struct target_extattr_set_file_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		iarg[1] = p->attrnamespace; /* int */
		uarg[2] = (intptr_t) p->attrname; /* abi_uintptr_t */
		uarg[3] = (intptr_t) p->data; /* abi_uintptr_t */
		uarg[4] = p->nbytes; /* size_t */
		*n_args = 5;
		break;
	}
	/* extattr_get_file */
	case 357: {
		struct target_extattr_get_file_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		iarg[1] = p->attrnamespace; /* int */
		uarg[2] = (intptr_t) p->attrname; /* abi_uintptr_t */
		uarg[3] = (intptr_t) p->data; /* abi_uintptr_t */
		uarg[4] = p->nbytes; /* size_t */
		*n_args = 5;
		break;
	}
	/* extattr_delete_file */
	case 358: {
		struct target_extattr_delete_file_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		iarg[1] = p->attrnamespace; /* int */
		uarg[2] = (intptr_t) p->attrname; /* abi_uintptr_t */
		*n_args = 3;
		break;
	}
	/* aio_waitcomplete */
	case 359: {
		struct target_aio_waitcomplete_args *p = params;
		uarg[0] = (intptr_t) p->aiocbp; /* abi_uintptr_t */
		uarg[1] = (intptr_t) p->timeout; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* getresuid */
	case 360: {
		struct target_getresuid_args *p = params;
		uarg[0] = (intptr_t) p->ruid; /* abi_uintptr_t */
		uarg[1] = (intptr_t) p->euid; /* abi_uintptr_t */
		uarg[2] = (intptr_t) p->suid; /* abi_uintptr_t */
		*n_args = 3;
		break;
	}
	/* getresgid */
	case 361: {
		struct target_getresgid_args *p = params;
		uarg[0] = (intptr_t) p->rgid; /* abi_uintptr_t */
		uarg[1] = (intptr_t) p->egid; /* abi_uintptr_t */
		uarg[2] = (intptr_t) p->sgid; /* abi_uintptr_t */
		*n_args = 3;
		break;
	}
	/* kqueue */
	case 362: {
		*n_args = 0;
		break;
	}
	/* extattr_set_fd */
	case 371: {
		struct target_extattr_set_fd_args *p = params;
		iarg[0] = p->fd; /* int */
		iarg[1] = p->attrnamespace; /* int */
		uarg[2] = (intptr_t) p->attrname; /* abi_uintptr_t */
		uarg[3] = (intptr_t) p->data; /* abi_uintptr_t */
		uarg[4] = p->nbytes; /* size_t */
		*n_args = 5;
		break;
	}
	/* extattr_get_fd */
	case 372: {
		struct target_extattr_get_fd_args *p = params;
		iarg[0] = p->fd; /* int */
		iarg[1] = p->attrnamespace; /* int */
		uarg[2] = (intptr_t) p->attrname; /* abi_uintptr_t */
		uarg[3] = (intptr_t) p->data; /* abi_uintptr_t */
		uarg[4] = p->nbytes; /* size_t */
		*n_args = 5;
		break;
	}
	/* extattr_delete_fd */
	case 373: {
		struct target_extattr_delete_fd_args *p = params;
		iarg[0] = p->fd; /* int */
		iarg[1] = p->attrnamespace; /* int */
		uarg[2] = (intptr_t) p->attrname; /* abi_uintptr_t */
		*n_args = 3;
		break;
	}
	/* __setugid */
	case 374: {
		struct target___setugid_args *p = params;
		iarg[0] = p->flag; /* int */
		*n_args = 1;
		break;
	}
	/* eaccess */
	case 376: {
		struct target_eaccess_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		iarg[1] = p->amode; /* int */
		*n_args = 2;
		break;
	}
	/* afs3_syscall */
	case 377: {
		struct target_afs3_syscall_args *p = params;
		iarg[0] = p->syscall; /* long */
		iarg[1] = p->parm1; /* long */
		iarg[2] = p->parm2; /* long */
		iarg[3] = p->parm3; /* long */
		iarg[4] = p->parm4; /* long */
		iarg[5] = p->parm5; /* long */
		iarg[6] = p->parm6; /* long */
		*n_args = 7;
		break;
	}
	/* nmount */
	case 378: {
		struct target_nmount_args *p = params;
		uarg[0] = (intptr_t) p->iovp; /* abi_uintptr_t */
		uarg[1] = p->iovcnt; /* unsigned int */
		iarg[2] = p->flags; /* int */
		*n_args = 3;
		break;
	}
	/* __mac_get_proc */
	case 384: {
		struct target___mac_get_proc_args *p = params;
		uarg[0] = (intptr_t) p->mac_p; /* abi_uintptr_t */
		*n_args = 1;
		break;
	}
	/* __mac_set_proc */
	case 385: {
		struct target___mac_set_proc_args *p = params;
		uarg[0] = (intptr_t) p->mac_p; /* abi_uintptr_t */
		*n_args = 1;
		break;
	}
	/* __mac_get_fd */
	case 386: {
		struct target___mac_get_fd_args *p = params;
		iarg[0] = p->fd; /* int */
		uarg[1] = (intptr_t) p->mac_p; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* __mac_get_file */
	case 387: {
		struct target___mac_get_file_args *p = params;
		uarg[0] = (intptr_t) p->path_p; /* abi_uintptr_t */
		uarg[1] = (intptr_t) p->mac_p; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* __mac_set_fd */
	case 388: {
		struct target___mac_set_fd_args *p = params;
		iarg[0] = p->fd; /* int */
		uarg[1] = (intptr_t) p->mac_p; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* __mac_set_file */
	case 389: {
		struct target___mac_set_file_args *p = params;
		uarg[0] = (intptr_t) p->path_p; /* abi_uintptr_t */
		uarg[1] = (intptr_t) p->mac_p; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* kenv */
	case 390: {
		struct target_kenv_args *p = params;
		iarg[0] = p->what; /* int */
		uarg[1] = (intptr_t) p->name; /* abi_uintptr_t */
		uarg[2] = (intptr_t) p->value; /* abi_uintptr_t */
		iarg[3] = p->len; /* int */
		*n_args = 4;
		break;
	}
	/* lchflags */
	case 391: {
		struct target_lchflags_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		uarg[1] = p->flags; /* u_long */
		*n_args = 2;
		break;
	}
	/* uuidgen */
	case 392: {
		struct target_uuidgen_args *p = params;
		uarg[0] = (intptr_t) p->store; /* abi_uintptr_t */
		iarg[1] = p->count; /* int */
		*n_args = 2;
		break;
	}
	/* sendfile */
	case 393: {
		struct target_sendfile_args *p = params;
		iarg[0] = p->fd; /* int */
		iarg[1] = p->s; /* int */
		iarg[2] = p->offset; /* off_t */
		uarg[3] = p->nbytes; /* size_t */
		uarg[4] = (intptr_t) p->hdtr; /* abi_uintptr_t */
		uarg[5] = (intptr_t) p->sbytes; /* abi_uintptr_t */
		iarg[6] = p->flags; /* int */
		*n_args = 7;
		break;
	}
	/* mac_syscall */
	case 394: {
		struct target_mac_syscall_args *p = params;
		uarg[0] = (intptr_t) p->policy; /* abi_uintptr_t */
		iarg[1] = p->call; /* int */
		uarg[2] = (intptr_t) p->arg; /* abi_uintptr_t */
		*n_args = 3;
		break;
	}
	/* ksem_close */
	case 400: {
		struct target_ksem_close_args *p = params;
		iarg[0] = p->id; /* semid_t */
		*n_args = 1;
		break;
	}
	/* ksem_post */
	case 401: {
		struct target_ksem_post_args *p = params;
		iarg[0] = p->id; /* semid_t */
		*n_args = 1;
		break;
	}
	/* ksem_wait */
	case 402: {
		struct target_ksem_wait_args *p = params;
		iarg[0] = p->id; /* semid_t */
		*n_args = 1;
		break;
	}
	/* ksem_trywait */
	case 403: {
		struct target_ksem_trywait_args *p = params;
		iarg[0] = p->id; /* semid_t */
		*n_args = 1;
		break;
	}
	/* ksem_init */
	case 404: {
		struct target_ksem_init_args *p = params;
		uarg[0] = (intptr_t) p->idp; /* abi_uintptr_t */
		uarg[1] = p->value; /* unsigned int */
		*n_args = 2;
		break;
	}
	/* ksem_open */
	case 405: {
		struct target_ksem_open_args *p = params;
		uarg[0] = (intptr_t) p->idp; /* abi_uintptr_t */
		uarg[1] = (intptr_t) p->name; /* abi_uintptr_t */
		iarg[2] = p->oflag; /* int */
		iarg[3] = p->mode; /* mode_t */
		uarg[4] = p->value; /* unsigned int */
		*n_args = 5;
		break;
	}
	/* ksem_unlink */
	case 406: {
		struct target_ksem_unlink_args *p = params;
		uarg[0] = (intptr_t) p->name; /* abi_uintptr_t */
		*n_args = 1;
		break;
	}
	/* ksem_getvalue */
	case 407: {
		struct target_ksem_getvalue_args *p = params;
		iarg[0] = p->id; /* semid_t */
		uarg[1] = (intptr_t) p->val; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* ksem_destroy */
	case 408: {
		struct target_ksem_destroy_args *p = params;
		iarg[0] = p->id; /* semid_t */
		*n_args = 1;
		break;
	}
	/* __mac_get_pid */
	case 409: {
		struct target___mac_get_pid_args *p = params;
		iarg[0] = p->pid; /* pid_t */
		uarg[1] = (intptr_t) p->mac_p; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* __mac_get_link */
	case 410: {
		struct target___mac_get_link_args *p = params;
		uarg[0] = (intptr_t) p->path_p; /* abi_uintptr_t */
		uarg[1] = (intptr_t) p->mac_p; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* __mac_set_link */
	case 411: {
		struct target___mac_set_link_args *p = params;
		uarg[0] = (intptr_t) p->path_p; /* abi_uintptr_t */
		uarg[1] = (intptr_t) p->mac_p; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* extattr_set_link */
	case 412: {
		struct target_extattr_set_link_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		iarg[1] = p->attrnamespace; /* int */
		uarg[2] = (intptr_t) p->attrname; /* abi_uintptr_t */
		uarg[3] = (intptr_t) p->data; /* abi_uintptr_t */
		uarg[4] = p->nbytes; /* size_t */
		*n_args = 5;
		break;
	}
	/* extattr_get_link */
	case 413: {
		struct target_extattr_get_link_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		iarg[1] = p->attrnamespace; /* int */
		uarg[2] = (intptr_t) p->attrname; /* abi_uintptr_t */
		uarg[3] = (intptr_t) p->data; /* abi_uintptr_t */
		uarg[4] = p->nbytes; /* size_t */
		*n_args = 5;
		break;
	}
	/* extattr_delete_link */
	case 414: {
		struct target_extattr_delete_link_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		iarg[1] = p->attrnamespace; /* int */
		uarg[2] = (intptr_t) p->attrname; /* abi_uintptr_t */
		*n_args = 3;
		break;
	}
	/* __mac_execve */
	case 415: {
		struct target___mac_execve_args *p = params;
		uarg[0] = (intptr_t) p->fname; /* abi_uintptr_t */
		uarg[1] = (intptr_t) p->argv; /* abi_uintptr_t */
		uarg[2] = (intptr_t) p->envv; /* abi_uintptr_t */
		uarg[3] = (intptr_t) p->mac_p; /* abi_uintptr_t */
		*n_args = 4;
		break;
	}
	/* sigaction */
	case 416: {
		struct target_sigaction_args *p = params;
		iarg[0] = p->sig; /* int */
		uarg[1] = (intptr_t) p->act; /* abi_uintptr_t */
		uarg[2] = (intptr_t) p->oact; /* abi_uintptr_t */
		*n_args = 3;
		break;
	}
	/* sigreturn */
	case 417: {
		struct target_sigreturn_args *p = params;
		uarg[0] = (intptr_t) p->sigcntxp; /* abi_uintptr_t */
		*n_args = 1;
		break;
	}
	/* getcontext */
	case 421: {
		struct target_getcontext_args *p = params;
		uarg[0] = (intptr_t) p->ucp; /* abi_uintptr_t */
		*n_args = 1;
		break;
	}
	/* setcontext */
	case 422: {
		struct target_setcontext_args *p = params;
		uarg[0] = (intptr_t) p->ucp; /* abi_uintptr_t */
		*n_args = 1;
		break;
	}
	/* swapcontext */
	case 423: {
		struct target_swapcontext_args *p = params;
		uarg[0] = (intptr_t) p->oucp; /* abi_uintptr_t */
		uarg[1] = (intptr_t) p->ucp; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* swapoff */
	case 424: {
		struct target_swapoff_args *p = params;
		uarg[0] = (intptr_t) p->name; /* abi_uintptr_t */
		*n_args = 1;
		break;
	}
	/* __acl_get_link */
	case 425: {
		struct target___acl_get_link_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		iarg[1] = p->type; /* acl_type_t */
		uarg[2] = (intptr_t) p->aclp; /* abi_uintptr_t */
		*n_args = 3;
		break;
	}
	/* __acl_set_link */
	case 426: {
		struct target___acl_set_link_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		iarg[1] = p->type; /* acl_type_t */
		uarg[2] = (intptr_t) p->aclp; /* abi_uintptr_t */
		*n_args = 3;
		break;
	}
	/* __acl_delete_link */
	case 427: {
		struct target___acl_delete_link_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		iarg[1] = p->type; /* acl_type_t */
		*n_args = 2;
		break;
	}
	/* __acl_aclcheck_link */
	case 428: {
		struct target___acl_aclcheck_link_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		iarg[1] = p->type; /* acl_type_t */
		uarg[2] = (intptr_t) p->aclp; /* abi_uintptr_t */
		*n_args = 3;
		break;
	}
	/* sigwait */
	case 429: {
		struct target_sigwait_args *p = params;
		uarg[0] = (intptr_t) p->set; /* abi_uintptr_t */
		uarg[1] = (intptr_t) p->sig; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* thr_create */
	case 430: {
		struct target_thr_create_args *p = params;
		uarg[0] = (intptr_t) p->ctx; /* abi_uintptr_t */
		uarg[1] = (intptr_t) p->id; /* abi_uintptr_t */
		iarg[2] = p->flags; /* int */
		*n_args = 3;
		break;
	}
	/* thr_exit */
	case 431: {
		struct target_thr_exit_args *p = params;
		uarg[0] = (intptr_t) p->state; /* abi_uintptr_t */
		*n_args = 1;
		break;
	}
	/* thr_self */
	case 432: {
		struct target_thr_self_args *p = params;
		uarg[0] = (intptr_t) p->id; /* abi_uintptr_t */
		*n_args = 1;
		break;
	}
	/* thr_kill */
	case 433: {
		struct target_thr_kill_args *p = params;
		iarg[0] = p->id; /* long */
		iarg[1] = p->sig; /* int */
		*n_args = 2;
		break;
	}
	/* jail_attach */
	case 436: {
		struct target_jail_attach_args *p = params;
		iarg[0] = p->jid; /* int */
		*n_args = 1;
		break;
	}
	/* extattr_list_fd */
	case 437: {
		struct target_extattr_list_fd_args *p = params;
		iarg[0] = p->fd; /* int */
		iarg[1] = p->attrnamespace; /* int */
		uarg[2] = (intptr_t) p->data; /* abi_uintptr_t */
		uarg[3] = p->nbytes; /* size_t */
		*n_args = 4;
		break;
	}
	/* extattr_list_file */
	case 438: {
		struct target_extattr_list_file_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		iarg[1] = p->attrnamespace; /* int */
		uarg[2] = (intptr_t) p->data; /* abi_uintptr_t */
		uarg[3] = p->nbytes; /* size_t */
		*n_args = 4;
		break;
	}
	/* extattr_list_link */
	case 439: {
		struct target_extattr_list_link_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		iarg[1] = p->attrnamespace; /* int */
		uarg[2] = (intptr_t) p->data; /* abi_uintptr_t */
		uarg[3] = p->nbytes; /* size_t */
		*n_args = 4;
		break;
	}
	/* ksem_timedwait */
	case 441: {
		struct target_ksem_timedwait_args *p = params;
		iarg[0] = p->id; /* semid_t */
		uarg[1] = (intptr_t) p->abstime; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* thr_suspend */
	case 442: {
		struct target_thr_suspend_args *p = params;
		uarg[0] = (intptr_t) p->timeout; /* abi_uintptr_t */
		*n_args = 1;
		break;
	}
	/* thr_wake */
	case 443: {
		struct target_thr_wake_args *p = params;
		iarg[0] = p->id; /* long */
		*n_args = 1;
		break;
	}
	/* kldunloadf */
	case 444: {
		struct target_kldunloadf_args *p = params;
		iarg[0] = p->fileid; /* int */
		iarg[1] = p->flags; /* int */
		*n_args = 2;
		break;
	}
	/* audit */
	case 445: {
		struct target_audit_args *p = params;
		uarg[0] = (intptr_t) p->record; /* abi_uintptr_t */
		uarg[1] = p->length; /* u_int */
		*n_args = 2;
		break;
	}
	/* auditon */
	case 446: {
		struct target_auditon_args *p = params;
		iarg[0] = p->cmd; /* int */
		uarg[1] = (intptr_t) p->data; /* abi_uintptr_t */
		uarg[2] = p->length; /* u_int */
		*n_args = 3;
		break;
	}
	/* getauid */
	case 447: {
		struct target_getauid_args *p = params;
		uarg[0] = (intptr_t) p->auid; /* abi_uintptr_t */
		*n_args = 1;
		break;
	}
	/* setauid */
	case 448: {
		struct target_setauid_args *p = params;
		uarg[0] = (intptr_t) p->auid; /* abi_uintptr_t */
		*n_args = 1;
		break;
	}
	/* getaudit */
	case 449: {
		struct target_getaudit_args *p = params;
		uarg[0] = (intptr_t) p->auditinfo; /* abi_uintptr_t */
		*n_args = 1;
		break;
	}
	/* setaudit */
	case 450: {
		struct target_setaudit_args *p = params;
		uarg[0] = (intptr_t) p->auditinfo; /* abi_uintptr_t */
		*n_args = 1;
		break;
	}
	/* getaudit_addr */
	case 451: {
		struct target_getaudit_addr_args *p = params;
		uarg[0] = (intptr_t) p->auditinfo_addr; /* abi_uintptr_t */
		uarg[1] = p->length; /* u_int */
		*n_args = 2;
		break;
	}
	/* setaudit_addr */
	case 452: {
		struct target_setaudit_addr_args *p = params;
		uarg[0] = (intptr_t) p->auditinfo_addr; /* abi_uintptr_t */
		uarg[1] = p->length; /* u_int */
		*n_args = 2;
		break;
	}
	/* auditctl */
	case 453: {
		struct target_auditctl_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		*n_args = 1;
		break;
	}
	/* _umtx_op */
	case 454: {
		struct target__umtx_op_args *p = params;
		uarg[0] = (intptr_t) p->obj; /* abi_uintptr_t */
		iarg[1] = p->op; /* int */
		uarg[2] = p->val; /* u_long */
		uarg[3] = (intptr_t) p->uaddr1; /* abi_uintptr_t */
		uarg[4] = (intptr_t) p->uaddr2; /* abi_uintptr_t */
		*n_args = 5;
		break;
	}
	/* thr_new */
	case 455: {
		struct target_thr_new_args *p = params;
		uarg[0] = (intptr_t) p->param; /* abi_uintptr_t */
		iarg[1] = p->param_size; /* int */
		*n_args = 2;
		break;
	}
	/* sigqueue */
	case 456: {
		struct target_sigqueue_args *p = params;
		iarg[0] = p->pid; /* pid_t */
		iarg[1] = p->signum; /* int */
		uarg[2] = (intptr_t) p->value; /* abi_uintptr_t */
		*n_args = 3;
		break;
	}
	/* kmq_open */
	case 457: {
		struct target_kmq_open_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		iarg[1] = p->flags; /* int */
		iarg[2] = p->mode; /* mode_t */
		uarg[3] = (intptr_t) p->attr; /* abi_uintptr_t */
		*n_args = 4;
		break;
	}
	/* kmq_setattr */
	case 458: {
		struct target_kmq_setattr_args *p = params;
		iarg[0] = p->mqd; /* int */
		uarg[1] = (intptr_t) p->attr; /* abi_uintptr_t */
		uarg[2] = (intptr_t) p->oattr; /* abi_uintptr_t */
		*n_args = 3;
		break;
	}
	/* kmq_timedreceive */
	case 459: {
		struct target_kmq_timedreceive_args *p = params;
		iarg[0] = p->mqd; /* int */
		uarg[1] = (intptr_t) p->msg_ptr; /* abi_uintptr_t */
		uarg[2] = p->msg_len; /* size_t */
		uarg[3] = (intptr_t) p->msg_prio; /* abi_uintptr_t */
		uarg[4] = (intptr_t) p->abs_timeout; /* abi_uintptr_t */
		*n_args = 5;
		break;
	}
	/* kmq_timedsend */
	case 460: {
		struct target_kmq_timedsend_args *p = params;
		iarg[0] = p->mqd; /* int */
		uarg[1] = (intptr_t) p->msg_ptr; /* abi_uintptr_t */
		uarg[2] = p->msg_len; /* size_t */
		uarg[3] = p->msg_prio; /* unsigned */
		uarg[4] = (intptr_t) p->abs_timeout; /* abi_uintptr_t */
		*n_args = 5;
		break;
	}
	/* kmq_notify */
	case 461: {
		struct target_kmq_notify_args *p = params;
		iarg[0] = p->mqd; /* int */
		uarg[1] = (intptr_t) p->sigev; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* kmq_unlink */
	case 462: {
		struct target_kmq_unlink_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		*n_args = 1;
		break;
	}
	/* abort2 */
	case 463: {
		struct target_abort2_args *p = params;
		uarg[0] = (intptr_t) p->why; /* abi_uintptr_t */
		iarg[1] = p->nargs; /* int */
		uarg[2] = (intptr_t) p->args; /* abi_uintptr_t */
		*n_args = 3;
		break;
	}
	/* thr_set_name */
	case 464: {
		struct target_thr_set_name_args *p = params;
		iarg[0] = p->id; /* long */
		uarg[1] = (intptr_t) p->name; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* aio_fsync */
	case 465: {
		struct target_aio_fsync_args *p = params;
		iarg[0] = p->op; /* int */
		uarg[1] = (intptr_t) p->aiocbp; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* rtprio_thread */
	case 466: {
		struct target_rtprio_thread_args *p = params;
		iarg[0] = p->function; /* int */
		iarg[1] = p->lwpid; /* lwpid_t */
		uarg[2] = (intptr_t) p->rtp; /* abi_uintptr_t */
		*n_args = 3;
		break;
	}
	/* sctp_peeloff */
	case 471: {
		struct target_sctp_peeloff_args *p = params;
		iarg[0] = p->sd; /* int */
		uarg[1] = p->name; /* uint32_t */
		*n_args = 2;
		break;
	}
	/* sctp_generic_sendmsg */
	case 472: {
		struct target_sctp_generic_sendmsg_args *p = params;
		iarg[0] = p->sd; /* int */
		uarg[1] = (intptr_t) p->msg; /* abi_uintptr_t */
		iarg[2] = p->mlen; /* int */
		uarg[3] = (intptr_t) p->to; /* abi_uintptr_t */
		iarg[4] = p->tolen; /* __socklen_t */
		uarg[5] = (intptr_t) p->sinfo; /* abi_uintptr_t */
		iarg[6] = p->flags; /* int */
		*n_args = 7;
		break;
	}
	/* sctp_generic_sendmsg_iov */
	case 473: {
		struct target_sctp_generic_sendmsg_iov_args *p = params;
		iarg[0] = p->sd; /* int */
		uarg[1] = (intptr_t) p->iov; /* abi_uintptr_t */
		iarg[2] = p->iovlen; /* int */
		uarg[3] = (intptr_t) p->to; /* abi_uintptr_t */
		iarg[4] = p->tolen; /* __socklen_t */
		uarg[5] = (intptr_t) p->sinfo; /* abi_uintptr_t */
		iarg[6] = p->flags; /* int */
		*n_args = 7;
		break;
	}
	/* sctp_generic_recvmsg */
	case 474: {
		struct target_sctp_generic_recvmsg_args *p = params;
		iarg[0] = p->sd; /* int */
		uarg[1] = (intptr_t) p->iov; /* abi_uintptr_t */
		iarg[2] = p->iovlen; /* int */
		uarg[3] = (intptr_t) p->from; /* abi_uintptr_t */
		uarg[4] = (intptr_t) p->fromlenaddr; /* abi_uintptr_t */
		uarg[5] = (intptr_t) p->sinfo; /* abi_uintptr_t */
		uarg[6] = (intptr_t) p->msg_flags; /* abi_uintptr_t */
		*n_args = 7;
		break;
	}
	/* pread */
	case 475: {
		struct target_pread_args *p = params;
		iarg[0] = p->fd; /* int */
		uarg[1] = (intptr_t) p->buf; /* abi_uintptr_t */
		uarg[2] = p->nbyte; /* size_t */
		iarg[3] = p->offset; /* off_t */
		*n_args = 4;
		break;
	}
	/* pwrite */
	case 476: {
		struct target_pwrite_args *p = params;
		iarg[0] = p->fd; /* int */
		uarg[1] = (intptr_t) p->buf; /* abi_uintptr_t */
		uarg[2] = p->nbyte; /* size_t */
		iarg[3] = p->offset; /* off_t */
		*n_args = 4;
		break;
	}
	/* mmap */
	case 477: {
		struct target_mmap_args *p = params;
		uarg[0] = (intptr_t) p->addr; /* abi_uintptr_t */
		uarg[1] = p->len; /* size_t */
		iarg[2] = p->prot; /* int */
		iarg[3] = p->flags; /* int */
		iarg[4] = p->fd; /* int */
		iarg[5] = p->pos; /* off_t */
		*n_args = 6;
		break;
	}
	/* lseek */
	case 478: {
		struct target_lseek_args *p = params;
		iarg[0] = p->fd; /* int */
		iarg[1] = p->offset; /* off_t */
		iarg[2] = p->whence; /* int */
		*n_args = 3;
		break;
	}
	/* truncate */
	case 479: {
		struct target_truncate_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		iarg[1] = p->length; /* off_t */
		*n_args = 2;
		break;
	}
	/* ftruncate */
	case 480: {
		struct target_ftruncate_args *p = params;
		iarg[0] = p->fd; /* int */
		iarg[1] = p->length; /* off_t */
		*n_args = 2;
		break;
	}
	/* thr_kill2 */
	case 481: {
		struct target_thr_kill2_args *p = params;
		iarg[0] = p->pid; /* pid_t */
		iarg[1] = p->id; /* long */
		iarg[2] = p->sig; /* int */
		*n_args = 3;
		break;
	}
	/* shm_unlink */
	case 483: {
		struct target_shm_unlink_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		*n_args = 1;
		break;
	}
	/* cpuset */
	case 484: {
		struct target_cpuset_args *p = params;
		uarg[0] = (intptr_t) p->setid; /* abi_uintptr_t */
		*n_args = 1;
		break;
	}
	/* cpuset_setid */
	case 485: {
		struct target_cpuset_setid_args *p = params;
		iarg[0] = p->which; /* cpuwhich_t */
		iarg[1] = p->id; /* id_t */
		iarg[2] = p->setid; /* cpusetid_t */
		*n_args = 3;
		break;
	}
	/* cpuset_getid */
	case 486: {
		struct target_cpuset_getid_args *p = params;
		iarg[0] = p->level; /* cpulevel_t */
		iarg[1] = p->which; /* cpuwhich_t */
		iarg[2] = p->id; /* id_t */
		uarg[3] = (intptr_t) p->setid; /* abi_uintptr_t */
		*n_args = 4;
		break;
	}
	/* cpuset_getaffinity */
	case 487: {
		struct target_cpuset_getaffinity_args *p = params;
		iarg[0] = p->level; /* cpulevel_t */
		iarg[1] = p->which; /* cpuwhich_t */
		iarg[2] = p->id; /* id_t */
		uarg[3] = p->cpusetsize; /* size_t */
		uarg[4] = (intptr_t) p->mask; /* abi_uintptr_t */
		*n_args = 5;
		break;
	}
	/* cpuset_setaffinity */
	case 488: {
		struct target_cpuset_setaffinity_args *p = params;
		iarg[0] = p->level; /* cpulevel_t */
		iarg[1] = p->which; /* cpuwhich_t */
		iarg[2] = p->id; /* id_t */
		uarg[3] = p->cpusetsize; /* size_t */
		uarg[4] = (intptr_t) p->mask; /* abi_uintptr_t */
		*n_args = 5;
		break;
	}
	/* faccessat */
	case 489: {
		struct target_faccessat_args *p = params;
		iarg[0] = p->fd; /* int */
		uarg[1] = (intptr_t) p->path; /* abi_uintptr_t */
		iarg[2] = p->amode; /* int */
		iarg[3] = p->flag; /* int */
		*n_args = 4;
		break;
	}
	/* fchmodat */
	case 490: {
		struct target_fchmodat_args *p = params;
		iarg[0] = p->fd; /* int */
		uarg[1] = (intptr_t) p->path; /* abi_uintptr_t */
		iarg[2] = p->mode; /* mode_t */
		iarg[3] = p->flag; /* int */
		*n_args = 4;
		break;
	}
	/* fchownat */
	case 491: {
		struct target_fchownat_args *p = params;
		iarg[0] = p->fd; /* int */
		uarg[1] = (intptr_t) p->path; /* abi_uintptr_t */
		uarg[2] = p->uid; /* uid_t */
		iarg[3] = p->gid; /* gid_t */
		iarg[4] = p->flag; /* int */
		*n_args = 5;
		break;
	}
	/* fexecve */
	case 492: {
		struct target_fexecve_args *p = params;
		iarg[0] = p->fd; /* int */
		uarg[1] = (intptr_t) p->argv; /* abi_uintptr_t */
		uarg[2] = (intptr_t) p->envv; /* abi_uintptr_t */
		*n_args = 3;
		break;
	}
	/* futimesat */
	case 494: {
		struct target_futimesat_args *p = params;
		iarg[0] = p->fd; /* int */
		uarg[1] = (intptr_t) p->path; /* abi_uintptr_t */
		uarg[2] = (intptr_t) p->times; /* abi_uintptr_t */
		*n_args = 3;
		break;
	}
	/* linkat */
	case 495: {
		struct target_linkat_args *p = params;
		iarg[0] = p->fd1; /* int */
		uarg[1] = (intptr_t) p->path1; /* abi_uintptr_t */
		iarg[2] = p->fd2; /* int */
		uarg[3] = (intptr_t) p->path2; /* abi_uintptr_t */
		iarg[4] = p->flag; /* int */
		*n_args = 5;
		break;
	}
	/* mkdirat */
	case 496: {
		struct target_mkdirat_args *p = params;
		iarg[0] = p->fd; /* int */
		uarg[1] = (intptr_t) p->path; /* abi_uintptr_t */
		iarg[2] = p->mode; /* mode_t */
		*n_args = 3;
		break;
	}
	/* mkfifoat */
	case 497: {
		struct target_mkfifoat_args *p = params;
		iarg[0] = p->fd; /* int */
		uarg[1] = (intptr_t) p->path; /* abi_uintptr_t */
		iarg[2] = p->mode; /* mode_t */
		*n_args = 3;
		break;
	}
	/* openat */
	case 499: {
		struct target_openat_args *p = params;
		iarg[0] = p->fd; /* int */
		uarg[1] = (intptr_t) p->path; /* abi_uintptr_t */
		iarg[2] = p->flag; /* int */
		iarg[3] = p->mode; /* mode_t */
		*n_args = 4;
		break;
	}
	/* readlinkat */
	case 500: {
		struct target_readlinkat_args *p = params;
		iarg[0] = p->fd; /* int */
		uarg[1] = (intptr_t) p->path; /* abi_uintptr_t */
		uarg[2] = (intptr_t) p->buf; /* abi_uintptr_t */
		uarg[3] = p->bufsize; /* size_t */
		*n_args = 4;
		break;
	}
	/* renameat */
	case 501: {
		struct target_renameat_args *p = params;
		iarg[0] = p->oldfd; /* int */
		uarg[1] = (intptr_t) p->old; /* abi_uintptr_t */
		iarg[2] = p->newfd; /* int */
		uarg[3] = (intptr_t) p->new; /* abi_uintptr_t */
		*n_args = 4;
		break;
	}
	/* symlinkat */
	case 502: {
		struct target_symlinkat_args *p = params;
		uarg[0] = (intptr_t) p->path1; /* abi_uintptr_t */
		iarg[1] = p->fd; /* int */
		uarg[2] = (intptr_t) p->path2; /* abi_uintptr_t */
		*n_args = 3;
		break;
	}
	/* unlinkat */
	case 503: {
		struct target_unlinkat_args *p = params;
		iarg[0] = p->fd; /* int */
		uarg[1] = (intptr_t) p->path; /* abi_uintptr_t */
		iarg[2] = p->flag; /* int */
		*n_args = 3;
		break;
	}
	/* posix_openpt */
	case 504: {
		struct target_posix_openpt_args *p = params;
		iarg[0] = p->flags; /* int */
		*n_args = 1;
		break;
	}
	/* gssd_syscall */
	case 505: {
		struct target_gssd_syscall_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		*n_args = 1;
		break;
	}
	/* jail_get */
	case 506: {
		struct target_jail_get_args *p = params;
		uarg[0] = (intptr_t) p->iovp; /* abi_uintptr_t */
		uarg[1] = p->iovcnt; /* unsigned int */
		iarg[2] = p->flags; /* int */
		*n_args = 3;
		break;
	}
	/* jail_set */
	case 507: {
		struct target_jail_set_args *p = params;
		uarg[0] = (intptr_t) p->iovp; /* abi_uintptr_t */
		uarg[1] = p->iovcnt; /* unsigned int */
		iarg[2] = p->flags; /* int */
		*n_args = 3;
		break;
	}
	/* jail_remove */
	case 508: {
		struct target_jail_remove_args *p = params;
		iarg[0] = p->jid; /* int */
		*n_args = 1;
		break;
	}
	/* __semctl */
	case 510: {
		struct target___semctl_args *p = params;
		iarg[0] = p->semid; /* int */
		iarg[1] = p->semnum; /* int */
		iarg[2] = p->cmd; /* int */
		uarg[3] = (intptr_t) p->arg; /* abi_uintptr_t */
		*n_args = 4;
		break;
	}
	/* msgctl */
	case 511: {
		struct target_msgctl_args *p = params;
		iarg[0] = p->msqid; /* int */
		iarg[1] = p->cmd; /* int */
		uarg[2] = (intptr_t) p->buf; /* abi_uintptr_t */
		*n_args = 3;
		break;
	}
	/* shmctl */
	case 512: {
		struct target_shmctl_args *p = params;
		iarg[0] = p->shmid; /* int */
		iarg[1] = p->cmd; /* int */
		uarg[2] = (intptr_t) p->buf; /* abi_uintptr_t */
		*n_args = 3;
		break;
	}
	/* lpathconf */
	case 513: {
		struct target_lpathconf_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		iarg[1] = p->name; /* int */
		*n_args = 2;
		break;
	}
	/* __cap_rights_get */
	case 515: {
		struct target___cap_rights_get_args *p = params;
		iarg[0] = p->version; /* int */
		iarg[1] = p->fd; /* int */
		uarg[2] = (intptr_t) p->rightsp; /* abi_uintptr_t */
		*n_args = 3;
		break;
	}
	/* cap_enter */
	case 516: {
		*n_args = 0;
		break;
	}
	/* cap_getmode */
	case 517: {
		struct target_cap_getmode_args *p = params;
		uarg[0] = (intptr_t) p->modep; /* abi_uintptr_t */
		*n_args = 1;
		break;
	}
	/* pdfork */
	case 518: {
		struct target_pdfork_args *p = params;
		uarg[0] = (intptr_t) p->fdp; /* abi_uintptr_t */
		iarg[1] = p->flags; /* int */
		*n_args = 2;
		break;
	}
	/* pdkill */
	case 519: {
		struct target_pdkill_args *p = params;
		iarg[0] = p->fd; /* int */
		iarg[1] = p->signum; /* int */
		*n_args = 2;
		break;
	}
	/* pdgetpid */
	case 520: {
		struct target_pdgetpid_args *p = params;
		iarg[0] = p->fd; /* int */
		uarg[1] = (intptr_t) p->pidp; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* pselect */
	case 522: {
		struct target_pselect_args *p = params;
		iarg[0] = p->nd; /* int */
		uarg[1] = (intptr_t) p->in; /* abi_uintptr_t */
		uarg[2] = (intptr_t) p->ou; /* abi_uintptr_t */
		uarg[3] = (intptr_t) p->ex; /* abi_uintptr_t */
		uarg[4] = (intptr_t) p->ts; /* abi_uintptr_t */
		uarg[5] = (intptr_t) p->sm; /* abi_uintptr_t */
		*n_args = 6;
		break;
	}
	/* getloginclass */
	case 523: {
		struct target_getloginclass_args *p = params;
		uarg[0] = (intptr_t) p->namebuf; /* abi_uintptr_t */
		uarg[1] = p->namelen; /* size_t */
		*n_args = 2;
		break;
	}
	/* setloginclass */
	case 524: {
		struct target_setloginclass_args *p = params;
		uarg[0] = (intptr_t) p->namebuf; /* abi_uintptr_t */
		*n_args = 1;
		break;
	}
	/* rctl_get_racct */
	case 525: {
		struct target_rctl_get_racct_args *p = params;
		uarg[0] = (intptr_t) p->inbufp; /* abi_uintptr_t */
		uarg[1] = p->inbuflen; /* size_t */
		uarg[2] = (intptr_t) p->outbufp; /* abi_uintptr_t */
		uarg[3] = p->outbuflen; /* size_t */
		*n_args = 4;
		break;
	}
	/* rctl_get_rules */
	case 526: {
		struct target_rctl_get_rules_args *p = params;
		uarg[0] = (intptr_t) p->inbufp; /* abi_uintptr_t */
		uarg[1] = p->inbuflen; /* size_t */
		uarg[2] = (intptr_t) p->outbufp; /* abi_uintptr_t */
		uarg[3] = p->outbuflen; /* size_t */
		*n_args = 4;
		break;
	}
	/* rctl_get_limits */
	case 527: {
		struct target_rctl_get_limits_args *p = params;
		uarg[0] = (intptr_t) p->inbufp; /* abi_uintptr_t */
		uarg[1] = p->inbuflen; /* size_t */
		uarg[2] = (intptr_t) p->outbufp; /* abi_uintptr_t */
		uarg[3] = p->outbuflen; /* size_t */
		*n_args = 4;
		break;
	}
	/* rctl_add_rule */
	case 528: {
		struct target_rctl_add_rule_args *p = params;
		uarg[0] = (intptr_t) p->inbufp; /* abi_uintptr_t */
		uarg[1] = p->inbuflen; /* size_t */
		uarg[2] = (intptr_t) p->outbufp; /* abi_uintptr_t */
		uarg[3] = p->outbuflen; /* size_t */
		*n_args = 4;
		break;
	}
	/* rctl_remove_rule */
	case 529: {
		struct target_rctl_remove_rule_args *p = params;
		uarg[0] = (intptr_t) p->inbufp; /* abi_uintptr_t */
		uarg[1] = p->inbuflen; /* size_t */
		uarg[2] = (intptr_t) p->outbufp; /* abi_uintptr_t */
		uarg[3] = p->outbuflen; /* size_t */
		*n_args = 4;
		break;
	}
	/* posix_fallocate */
	case 530: {
		struct target_posix_fallocate_args *p = params;
		iarg[0] = p->fd; /* int */
		iarg[1] = p->offset; /* off_t */
		iarg[2] = p->len; /* off_t */
		*n_args = 3;
		break;
	}
	/* posix_fadvise */
	case 531: {
		struct target_posix_fadvise_args *p = params;
		iarg[0] = p->fd; /* int */
		iarg[1] = p->offset; /* off_t */
		iarg[2] = p->len; /* off_t */
		iarg[3] = p->advice; /* int */
		*n_args = 4;
		break;
	}
	/* wait6 */
	case 532: {
		struct target_wait6_args *p = params;
		iarg[0] = p->idtype; /* idtype_t */
		iarg[1] = p->id; /* id_t */
		uarg[2] = (intptr_t) p->status; /* abi_uintptr_t */
		iarg[3] = p->options; /* int */
		uarg[4] = (intptr_t) p->wrusage; /* abi_uintptr_t */
		uarg[5] = (intptr_t) p->info; /* abi_uintptr_t */
		*n_args = 6;
		break;
	}
	/* cap_rights_limit */
	case 533: {
		struct target_cap_rights_limit_args *p = params;
		iarg[0] = p->fd; /* int */
		uarg[1] = (intptr_t) p->rightsp; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* cap_ioctls_limit */
	case 534: {
		struct target_cap_ioctls_limit_args *p = params;
		iarg[0] = p->fd; /* int */
		uarg[1] = (intptr_t) p->cmds; /* abi_uintptr_t */
		uarg[2] = p->ncmds; /* size_t */
		*n_args = 3;
		break;
	}
	/* cap_ioctls_get */
	case 535: {
		struct target_cap_ioctls_get_args *p = params;
		iarg[0] = p->fd; /* int */
		uarg[1] = (intptr_t) p->cmds; /* abi_uintptr_t */
		uarg[2] = p->maxcmds; /* size_t */
		*n_args = 3;
		break;
	}
	/* cap_fcntls_limit */
	case 536: {
		struct target_cap_fcntls_limit_args *p = params;
		iarg[0] = p->fd; /* int */
		uarg[1] = p->fcntlrights; /* uint32_t */
		*n_args = 2;
		break;
	}
	/* cap_fcntls_get */
	case 537: {
		struct target_cap_fcntls_get_args *p = params;
		iarg[0] = p->fd; /* int */
		uarg[1] = (intptr_t) p->fcntlrightsp; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* bindat */
	case 538: {
		struct target_bindat_args *p = params;
		iarg[0] = p->fd; /* int */
		iarg[1] = p->s; /* int */
		uarg[2] = (intptr_t) p->name; /* abi_uintptr_t */
		iarg[3] = p->namelen; /* __socklen_t */
		*n_args = 4;
		break;
	}
	/* connectat */
	case 539: {
		struct target_connectat_args *p = params;
		iarg[0] = p->fd; /* int */
		iarg[1] = p->s; /* int */
		uarg[2] = (intptr_t) p->name; /* abi_uintptr_t */
		iarg[3] = p->namelen; /* __socklen_t */
		*n_args = 4;
		break;
	}
	/* chflagsat */
	case 540: {
		struct target_chflagsat_args *p = params;
		iarg[0] = p->fd; /* int */
		uarg[1] = (intptr_t) p->path; /* abi_uintptr_t */
		uarg[2] = p->flags; /* u_long */
		iarg[3] = p->atflag; /* int */
		*n_args = 4;
		break;
	}
	/* accept4 */
	case 541: {
		struct target_accept4_args *p = params;
		iarg[0] = p->s; /* int */
		uarg[1] = (intptr_t) p->name; /* abi_uintptr_t */
		uarg[2] = (intptr_t) p->anamelen; /* abi_uintptr_t */
		iarg[3] = p->flags; /* int */
		*n_args = 4;
		break;
	}
	/* pipe2 */
	case 542: {
		struct target_pipe2_args *p = params;
		uarg[0] = (intptr_t) p->fildes; /* abi_uintptr_t */
		iarg[1] = p->flags; /* int */
		*n_args = 2;
		break;
	}
	/* aio_mlock */
	case 543: {
		struct target_aio_mlock_args *p = params;
		uarg[0] = (intptr_t) p->aiocbp; /* abi_uintptr_t */
		*n_args = 1;
		break;
	}
	/* procctl */
	case 544: {
		struct target_procctl_args *p = params;
		iarg[0] = p->idtype; /* idtype_t */
		iarg[1] = p->id; /* id_t */
		iarg[2] = p->com; /* int */
		uarg[3] = (intptr_t) p->data; /* abi_uintptr_t */
		*n_args = 4;
		break;
	}
	/* ppoll */
	case 545: {
		struct target_ppoll_args *p = params;
		uarg[0] = (intptr_t) p->fds; /* abi_uintptr_t */
		uarg[1] = p->nfds; /* u_int */
		uarg[2] = (intptr_t) p->ts; /* abi_uintptr_t */
		uarg[3] = (intptr_t) p->set; /* abi_uintptr_t */
		*n_args = 4;
		break;
	}
	/* futimens */
	case 546: {
		struct target_futimens_args *p = params;
		iarg[0] = p->fd; /* int */
		uarg[1] = (intptr_t) p->times; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* utimensat */
	case 547: {
		struct target_utimensat_args *p = params;
		iarg[0] = p->fd; /* int */
		uarg[1] = (intptr_t) p->path; /* abi_uintptr_t */
		uarg[2] = (intptr_t) p->times; /* abi_uintptr_t */
		iarg[3] = p->flag; /* int */
		*n_args = 4;
		break;
	}
	/* fdatasync */
	case 550: {
		struct target_fdatasync_args *p = params;
		iarg[0] = p->fd; /* int */
		*n_args = 1;
		break;
	}
	/* fstat */
	case 551: {
		struct target_fstat_args *p = params;
		iarg[0] = p->fd; /* int */
		uarg[1] = (intptr_t) p->sb; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* fstatat */
	case 552: {
		struct target_fstatat_args *p = params;
		iarg[0] = p->fd; /* int */
		uarg[1] = (intptr_t) p->path; /* abi_uintptr_t */
		uarg[2] = (intptr_t) p->buf; /* abi_uintptr_t */
		iarg[3] = p->flag; /* int */
		*n_args = 4;
		break;
	}
	/* fhstat */
	case 553: {
		struct target_fhstat_args *p = params;
		uarg[0] = (intptr_t) p->u_fhp; /* abi_uintptr_t */
		uarg[1] = (intptr_t) p->sb; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* getdirentries */
	case 554: {
		struct target_getdirentries_args *p = params;
		iarg[0] = p->fd; /* int */
		uarg[1] = (intptr_t) p->buf; /* abi_uintptr_t */
		uarg[2] = p->count; /* size_t */
		uarg[3] = (intptr_t) p->basep; /* abi_uintptr_t */
		*n_args = 4;
		break;
	}
	/* statfs */
	case 555: {
		struct target_statfs_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		uarg[1] = (intptr_t) p->buf; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* fstatfs */
	case 556: {
		struct target_fstatfs_args *p = params;
		iarg[0] = p->fd; /* int */
		uarg[1] = (intptr_t) p->buf; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* getfsstat */
	case 557: {
		struct target_getfsstat_args *p = params;
		uarg[0] = (intptr_t) p->buf; /* abi_uintptr_t */
		iarg[1] = p->bufsize; /* long */
		iarg[2] = p->mode; /* int */
		*n_args = 3;
		break;
	}
	/* fhstatfs */
	case 558: {
		struct target_fhstatfs_args *p = params;
		uarg[0] = (intptr_t) p->u_fhp; /* abi_uintptr_t */
		uarg[1] = (intptr_t) p->buf; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* mknodat */
	case 559: {
		struct target_mknodat_args *p = params;
		iarg[0] = p->fd; /* int */
		uarg[1] = (intptr_t) p->path; /* abi_uintptr_t */
		iarg[2] = p->mode; /* mode_t */
		iarg[3] = p->dev; /* dev_t */
		*n_args = 4;
		break;
	}
	/* kevent */
	case 560: {
		struct target_kevent_args *p = params;
		iarg[0] = p->fd; /* int */
		uarg[1] = (intptr_t) p->changelist; /* abi_uintptr_t */
		iarg[2] = p->nchanges; /* int */
		uarg[3] = (intptr_t) p->eventlist; /* abi_uintptr_t */
		iarg[4] = p->nevents; /* int */
		uarg[5] = (intptr_t) p->timeout; /* abi_uintptr_t */
		*n_args = 6;
		break;
	}
	/* cpuset_getdomain */
	case 561: {
		struct target_cpuset_getdomain_args *p = params;
		iarg[0] = p->level; /* cpulevel_t */
		iarg[1] = p->which; /* cpuwhich_t */
		iarg[2] = p->id; /* id_t */
		uarg[3] = p->domainsetsize; /* size_t */
		uarg[4] = (intptr_t) p->mask; /* abi_uintptr_t */
		uarg[5] = (intptr_t) p->policy; /* abi_uintptr_t */
		*n_args = 6;
		break;
	}
	/* cpuset_setdomain */
	case 562: {
		struct target_cpuset_setdomain_args *p = params;
		iarg[0] = p->level; /* cpulevel_t */
		iarg[1] = p->which; /* cpuwhich_t */
		iarg[2] = p->id; /* id_t */
		uarg[3] = p->domainsetsize; /* size_t */
		uarg[4] = (intptr_t) p->mask; /* abi_uintptr_t */
		iarg[5] = p->policy; /* int */
		*n_args = 6;
		break;
	}
	/* getrandom */
	case 563: {
		struct target_getrandom_args *p = params;
		uarg[0] = (intptr_t) p->buf; /* abi_uintptr_t */
		uarg[1] = p->buflen; /* size_t */
		uarg[2] = p->flags; /* unsigned int */
		*n_args = 3;
		break;
	}
	/* getfhat */
	case 564: {
		struct target_getfhat_args *p = params;
		iarg[0] = p->fd; /* int */
		uarg[1] = (intptr_t) p->path; /* abi_uintptr_t */
		uarg[2] = (intptr_t) p->fhp; /* abi_uintptr_t */
		iarg[3] = p->flags; /* int */
		*n_args = 4;
		break;
	}
	/* fhlink */
	case 565: {
		struct target_fhlink_args *p = params;
		uarg[0] = (intptr_t) p->fhp; /* abi_uintptr_t */
		uarg[1] = (intptr_t) p->to; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* fhlinkat */
	case 566: {
		struct target_fhlinkat_args *p = params;
		uarg[0] = (intptr_t) p->fhp; /* abi_uintptr_t */
		iarg[1] = p->tofd; /* int */
		uarg[2] = (intptr_t) p->to; /* abi_uintptr_t */
		*n_args = 3;
		break;
	}
	/* fhreadlink */
	case 567: {
		struct target_fhreadlink_args *p = params;
		uarg[0] = (intptr_t) p->fhp; /* abi_uintptr_t */
		uarg[1] = (intptr_t) p->buf; /* abi_uintptr_t */
		uarg[2] = p->bufsize; /* size_t */
		*n_args = 3;
		break;
	}
	/* funlinkat */
	case 568: {
		struct target_funlinkat_args *p = params;
		iarg[0] = p->dfd; /* int */
		uarg[1] = (intptr_t) p->path; /* abi_uintptr_t */
		iarg[2] = p->fd; /* int */
		iarg[3] = p->flag; /* int */
		*n_args = 4;
		break;
	}
	/* copy_file_range */
	case 569: {
		struct target_copy_file_range_args *p = params;
		iarg[0] = p->infd; /* int */
		uarg[1] = (intptr_t) p->inoffp; /* abi_uintptr_t */
		iarg[2] = p->outfd; /* int */
		uarg[3] = (intptr_t) p->outoffp; /* abi_uintptr_t */
		uarg[4] = p->len; /* size_t */
		uarg[5] = p->flags; /* unsigned int */
		*n_args = 6;
		break;
	}
	/* __sysctlbyname */
	case 570: {
		struct target___sysctlbyname_args *p = params;
		uarg[0] = (intptr_t) p->name; /* abi_uintptr_t */
		uarg[1] = p->namelen; /* size_t */
		uarg[2] = (intptr_t) p->old; /* abi_uintptr_t */
		uarg[3] = (intptr_t) p->oldlenp; /* abi_uintptr_t */
		uarg[4] = (intptr_t) p->new; /* abi_uintptr_t */
		uarg[5] = p->newlen; /* size_t */
		*n_args = 6;
		break;
	}
	/* shm_open2 */
	case 571: {
		struct target_shm_open2_args *p = params;
		uarg[0] = (intptr_t) p->path; /* abi_uintptr_t */
		iarg[1] = p->flags; /* int */
		iarg[2] = p->mode; /* mode_t */
		iarg[3] = p->shmflags; /* int */
		uarg[4] = (intptr_t) p->name; /* abi_uintptr_t */
		*n_args = 5;
		break;
	}
	/* shm_rename */
	case 572: {
		struct target_shm_rename_args *p = params;
		uarg[0] = (intptr_t) p->path_from; /* abi_uintptr_t */
		uarg[1] = (intptr_t) p->path_to; /* abi_uintptr_t */
		iarg[2] = p->flags; /* int */
		*n_args = 3;
		break;
	}
	/* sigfastblock */
	case 573: {
		struct target_sigfastblock_args *p = params;
		iarg[0] = p->cmd; /* int */
		uarg[1] = (intptr_t) p->ptr; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	/* __realpathat */
	case 574: {
		struct target___realpathat_args *p = params;
		iarg[0] = p->fd; /* int */
		uarg[1] = (intptr_t) p->path; /* abi_uintptr_t */
		uarg[2] = (intptr_t) p->buf; /* abi_uintptr_t */
		uarg[3] = p->size; /* size_t */
		iarg[4] = p->flags; /* int */
		*n_args = 5;
		break;
	}
	/* close_range */
	case 575: {
		struct target_close_range_args *p = params;
		uarg[0] = p->lowfd; /* u_int */
		uarg[1] = p->highfd; /* u_int */
		iarg[2] = p->flags; /* int */
		*n_args = 3;
		break;
	}
	/* rpctls_syscall */
	case 576: {
		struct target_rpctls_syscall_args *p = params;
		iarg[0] = p->op; /* int */
		uarg[1] = (intptr_t) p->path; /* abi_uintptr_t */
		*n_args = 2;
		break;
	}
	default:
		*n_args = 0;
		break;
	};
}
static void
systrace_entry_setargdesc(int sysnum, int ndx, char *desc, size_t descsz)
{
	const char *p = NULL;
	switch (sysnum) {
	/* nosys */
	case 0:
		break;
	/* sys_exit */
	case 1:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* fork */
	case 2:
		break;
	/* read */
	case 3:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "size_t";
			break;
		default:
			break;
		};
		break;
	/* write */
	case 4:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "size_t";
			break;
		default:
			break;
		};
		break;
	/* open */
	case 5:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "int";
			break;
		case 2:
			p = "mode_t";
			break;
		default:
			break;
		};
		break;
	/* close */
	case 6:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* wait4 */
	case 7:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "int";
			break;
		case 3:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* link */
	case 9:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* unlink */
	case 10:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* chdir */
	case 12:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* fchdir */
	case 13:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* chmod */
	case 15:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "mode_t";
			break;
		default:
			break;
		};
		break;
	/* chown */
	case 16:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "int";
			break;
		case 2:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* break */
	case 17:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* getpid */
	case 20:
		break;
	/* mount */
	case 21:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "int";
			break;
		case 3:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* unmount */
	case 22:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* setuid */
	case 23:
		switch(ndx) {
		case 0:
			p = "uid_t";
			break;
		default:
			break;
		};
		break;
	/* getuid */
	case 24:
		break;
	/* geteuid */
	case 25:
		break;
	/* ptrace */
	case 26:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "pid_t";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		case 3:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* recvmsg */
	case 27:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* sendmsg */
	case 28:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* recvfrom */
	case 29:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "size_t";
			break;
		case 3:
			p = "int";
			break;
		case 4:
			p = "abi_uintptr_t";
			break;
		case 5:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* accept */
	case 30:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* getpeername */
	case 31:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* getsockname */
	case 32:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* access */
	case 33:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* chflags */
	case 34:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "u_long";
			break;
		default:
			break;
		};
		break;
	/* fchflags */
	case 35:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "u_long";
			break;
		default:
			break;
		};
		break;
	/* sync */
	case 36:
		break;
	/* kill */
	case 37:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* getppid */
	case 39:
		break;
	/* dup */
	case 41:
		switch(ndx) {
		case 0:
			p = "u_int";
			break;
		default:
			break;
		};
		break;
	/* getegid */
	case 43:
		break;
	/* profil */
	case 44:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "size_t";
			break;
		case 2:
			p = "size_t";
			break;
		case 3:
			p = "u_int";
			break;
		default:
			break;
		};
		break;
	/* ktrace */
	case 45:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "int";
			break;
		case 2:
			p = "int";
			break;
		case 3:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* getgid */
	case 47:
		break;
	/* getlogin */
	case 49:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "u_int";
			break;
		default:
			break;
		};
		break;
	/* setlogin */
	case 50:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* acct */
	case 51:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* sigaltstack */
	case 53:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* ioctl */
	case 54:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "u_long";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* reboot */
	case 55:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* revoke */
	case 56:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* symlink */
	case 57:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* readlink */
	case 58:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "size_t";
			break;
		default:
			break;
		};
		break;
	/* execve */
	case 59:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* umask */
	case 60:
		switch(ndx) {
		case 0:
			p = "mode_t";
			break;
		default:
			break;
		};
		break;
	/* chroot */
	case 61:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* msync */
	case 65:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "size_t";
			break;
		case 2:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* vfork */
	case 66:
		break;
	/* sbrk */
	case 69:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* sstk */
	case 70:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* munmap */
	case 73:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "size_t";
			break;
		default:
			break;
		};
		break;
	/* mprotect */
	case 74:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "size_t";
			break;
		case 2:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* madvise */
	case 75:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "size_t";
			break;
		case 2:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* mincore */
	case 78:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "size_t";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* getgroups */
	case 79:
		switch(ndx) {
		case 0:
			p = "u_int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* setgroups */
	case 80:
		switch(ndx) {
		case 0:
			p = "u_int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* getpgrp */
	case 81:
		break;
	/* setpgid */
	case 82:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* setitimer */
	case 83:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* swapon */
	case 85:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* getitimer */
	case 86:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* getdtablesize */
	case 89:
		break;
	/* dup2 */
	case 90:
		switch(ndx) {
		case 0:
			p = "u_int";
			break;
		case 1:
			p = "u_int";
			break;
		default:
			break;
		};
		break;
	/* fcntl */
	case 92:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "int";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* select */
	case 93:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		case 3:
			p = "abi_uintptr_t";
			break;
		case 4:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* fsync */
	case 95:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* setpriority */
	case 96:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "int";
			break;
		case 2:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* socket */
	case 97:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "int";
			break;
		case 2:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* connect */
	case 98:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "__socklen_t";
			break;
		default:
			break;
		};
		break;
	/* getpriority */
	case 100:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* bind */
	case 104:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "__socklen_t";
			break;
		default:
			break;
		};
		break;
	/* setsockopt */
	case 105:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "int";
			break;
		case 2:
			p = "int";
			break;
		case 3:
			p = "abi_uintptr_t";
			break;
		case 4:
			p = "__socklen_t";
			break;
		default:
			break;
		};
		break;
	/* listen */
	case 106:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* gettimeofday */
	case 116:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* getrusage */
	case 117:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* getsockopt */
	case 118:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "int";
			break;
		case 2:
			p = "int";
			break;
		case 3:
			p = "abi_uintptr_t";
			break;
		case 4:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* readv */
	case 120:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "u_int";
			break;
		default:
			break;
		};
		break;
	/* writev */
	case 121:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "u_int";
			break;
		default:
			break;
		};
		break;
	/* settimeofday */
	case 122:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* fchown */
	case 123:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "int";
			break;
		case 2:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* fchmod */
	case 124:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "mode_t";
			break;
		default:
			break;
		};
		break;
	/* setreuid */
	case 126:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* setregid */
	case 127:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* rename */
	case 128:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* flock */
	case 131:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* mkfifo */
	case 132:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "mode_t";
			break;
		default:
			break;
		};
		break;
	/* sendto */
	case 133:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "size_t";
			break;
		case 3:
			p = "int";
			break;
		case 4:
			p = "abi_uintptr_t";
			break;
		case 5:
			p = "__socklen_t";
			break;
		default:
			break;
		};
		break;
	/* shutdown */
	case 134:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* socketpair */
	case 135:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "int";
			break;
		case 2:
			p = "int";
			break;
		case 3:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* mkdir */
	case 136:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "mode_t";
			break;
		default:
			break;
		};
		break;
	/* rmdir */
	case 137:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* utimes */
	case 138:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* adjtime */
	case 140:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* setsid */
	case 147:
		break;
	/* quotactl */
	case 148:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "int";
			break;
		case 2:
			p = "int";
			break;
		case 3:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* nlm_syscall */
	case 154:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "int";
			break;
		case 2:
			p = "int";
			break;
		case 3:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* nfssvc */
	case 155:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* lgetfh */
	case 160:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* getfh */
	case 161:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* sysarch */
	case 165:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* rtprio */
	case 166:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "pid_t";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* semsys */
	case 169:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		case 3:
			p = "abi_uintptr_t";
			break;
		case 4:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* msgsys */
	case 170:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		case 3:
			p = "abi_uintptr_t";
			break;
		case 4:
			p = "abi_uintptr_t";
			break;
		case 5:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* shmsys */
	case 171:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		case 3:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* setfib */
	case 175:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* ntp_adjtime */
	case 176:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* setgid */
	case 181:
		switch(ndx) {
		case 0:
			p = "gid_t";
			break;
		default:
			break;
		};
		break;
	/* setegid */
	case 182:
		switch(ndx) {
		case 0:
			p = "gid_t";
			break;
		default:
			break;
		};
		break;
	/* seteuid */
	case 183:
		switch(ndx) {
		case 0:
			p = "uid_t";
			break;
		default:
			break;
		};
		break;
	/* pathconf */
	case 191:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* fpathconf */
	case 192:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* getrlimit */
	case 194:
		switch(ndx) {
		case 0:
			p = "u_int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* setrlimit */
	case 195:
		switch(ndx) {
		case 0:
			p = "u_int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* nosys */
	case 198:
		break;
	/* __sysctl */
	case 202:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "u_int";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		case 3:
			p = "abi_uintptr_t";
			break;
		case 4:
			p = "abi_uintptr_t";
			break;
		case 5:
			p = "size_t";
			break;
		default:
			break;
		};
		break;
	/* mlock */
	case 203:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "size_t";
			break;
		default:
			break;
		};
		break;
	/* munlock */
	case 204:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "size_t";
			break;
		default:
			break;
		};
		break;
	/* undelete */
	case 205:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* futimes */
	case 206:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* getpgid */
	case 207:
		switch(ndx) {
		case 0:
			p = "pid_t";
			break;
		default:
			break;
		};
		break;
	/* poll */
	case 209:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "u_int";
			break;
		case 2:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* lkmnosys */
	case 210:
		break;
	/* lkmnosys */
	case 211:
		break;
	/* lkmnosys */
	case 212:
		break;
	/* lkmnosys */
	case 213:
		break;
	/* lkmnosys */
	case 214:
		break;
	/* lkmnosys */
	case 215:
		break;
	/* lkmnosys */
	case 216:
		break;
	/* lkmnosys */
	case 217:
		break;
	/* lkmnosys */
	case 218:
		break;
	/* lkmnosys */
	case 219:
		break;
	/* semget */
	case 221:
		switch(ndx) {
		case 0:
			p = "key_t";
			break;
		case 1:
			p = "int";
			break;
		case 2:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* semop */
	case 222:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "size_t";
			break;
		default:
			break;
		};
		break;
	/* msgget */
	case 225:
		switch(ndx) {
		case 0:
			p = "key_t";
			break;
		case 1:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* msgsnd */
	case 226:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "size_t";
			break;
		case 3:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* msgrcv */
	case 227:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "size_t";
			break;
		case 3:
			p = "long";
			break;
		case 4:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* shmat */
	case 228:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* shmdt */
	case 230:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* shmget */
	case 231:
		switch(ndx) {
		case 0:
			p = "key_t";
			break;
		case 1:
			p = "size_t";
			break;
		case 2:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* clock_gettime */
	case 232:
		switch(ndx) {
		case 0:
			p = "clockid_t";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* clock_settime */
	case 233:
		switch(ndx) {
		case 0:
			p = "clockid_t";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* clock_getres */
	case 234:
		switch(ndx) {
		case 0:
			p = "clockid_t";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* ktimer_create */
	case 235:
		switch(ndx) {
		case 0:
			p = "clockid_t";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* ktimer_delete */
	case 236:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* ktimer_settime */
	case 237:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "int";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		case 3:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* ktimer_gettime */
	case 238:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* ktimer_getoverrun */
	case 239:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* nanosleep */
	case 240:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* ffclock_getcounter */
	case 241:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* ffclock_setestimate */
	case 242:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* ffclock_getestimate */
	case 243:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* clock_nanosleep */
	case 244:
		switch(ndx) {
		case 0:
			p = "clockid_t";
			break;
		case 1:
			p = "int";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		case 3:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* clock_getcpuclockid2 */
	case 247:
		switch(ndx) {
		case 0:
			p = "id_t";
			break;
		case 1:
			p = "int";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* ntp_gettime */
	case 248:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* minherit */
	case 250:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "size_t";
			break;
		case 2:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* rfork */
	case 251:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* issetugid */
	case 253:
		break;
	/* lchown */
	case 254:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "int";
			break;
		case 2:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* aio_read */
	case 255:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* aio_write */
	case 256:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* lio_listio */
	case 257:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "int";
			break;
		case 3:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* kbounce */
	case 258:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "size_t";
			break;
		case 3:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* flag_captured */
	case 259:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "uint32_t";
			break;
		default:
			break;
		};
		break;
	/* lchmod */
	case 274:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "mode_t";
			break;
		default:
			break;
		};
		break;
	/* lutimes */
	case 276:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* preadv */
	case 289:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "u_int";
			break;
		case 3:
			p = "off_t";
			break;
		default:
			break;
		};
		break;
	/* pwritev */
	case 290:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "u_int";
			break;
		case 3:
			p = "off_t";
			break;
		default:
			break;
		};
		break;
	/* fhopen */
	case 298:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* modnext */
	case 300:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* modstat */
	case 301:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* modfnext */
	case 302:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* modfind */
	case 303:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* kldload */
	case 304:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* kldunload */
	case 305:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* kldfind */
	case 306:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* kldnext */
	case 307:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* kldstat */
	case 308:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* kldfirstmod */
	case 309:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* getsid */
	case 310:
		switch(ndx) {
		case 0:
			p = "pid_t";
			break;
		default:
			break;
		};
		break;
	/* setresuid */
	case 311:
		switch(ndx) {
		case 0:
			p = "uid_t";
			break;
		case 1:
			p = "uid_t";
			break;
		case 2:
			p = "uid_t";
			break;
		default:
			break;
		};
		break;
	/* setresgid */
	case 312:
		switch(ndx) {
		case 0:
			p = "gid_t";
			break;
		case 1:
			p = "gid_t";
			break;
		case 2:
			p = "gid_t";
			break;
		default:
			break;
		};
		break;
	/* aio_return */
	case 314:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* aio_suspend */
	case 315:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "int";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* aio_cancel */
	case 316:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* aio_error */
	case 317:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* yield */
	case 321:
		break;
	/* mlockall */
	case 324:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* munlockall */
	case 325:
		break;
	/* __getcwd */
	case 326:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "size_t";
			break;
		default:
			break;
		};
		break;
	/* sched_setparam */
	case 327:
		switch(ndx) {
		case 0:
			p = "pid_t";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* sched_getparam */
	case 328:
		switch(ndx) {
		case 0:
			p = "pid_t";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* sched_setscheduler */
	case 329:
		switch(ndx) {
		case 0:
			p = "pid_t";
			break;
		case 1:
			p = "int";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* sched_getscheduler */
	case 330:
		switch(ndx) {
		case 0:
			p = "pid_t";
			break;
		default:
			break;
		};
		break;
	/* sched_yield */
	case 331:
		break;
	/* sched_get_priority_max */
	case 332:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* sched_get_priority_min */
	case 333:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* sched_rr_get_interval */
	case 334:
		switch(ndx) {
		case 0:
			p = "pid_t";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* utrace */
	case 335:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "size_t";
			break;
		default:
			break;
		};
		break;
	/* kldsym */
	case 337:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "int";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* jail */
	case 338:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* nnpfs_syscall */
	case 339:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "int";
			break;
		case 3:
			p = "abi_uintptr_t";
			break;
		case 4:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* sigprocmask */
	case 340:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* sigsuspend */
	case 341:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* sigpending */
	case 343:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* sigtimedwait */
	case 345:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* sigwaitinfo */
	case 346:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* __acl_get_file */
	case 347:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "acl_type_t";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* __acl_set_file */
	case 348:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "acl_type_t";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* __acl_get_fd */
	case 349:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "acl_type_t";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* __acl_set_fd */
	case 350:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "acl_type_t";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* __acl_delete_file */
	case 351:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "acl_type_t";
			break;
		default:
			break;
		};
		break;
	/* __acl_delete_fd */
	case 352:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "acl_type_t";
			break;
		default:
			break;
		};
		break;
	/* __acl_aclcheck_file */
	case 353:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "acl_type_t";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* __acl_aclcheck_fd */
	case 354:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "acl_type_t";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* extattrctl */
	case 355:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "int";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		case 3:
			p = "int";
			break;
		case 4:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* extattr_set_file */
	case 356:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "int";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		case 3:
			p = "abi_uintptr_t";
			break;
		case 4:
			p = "size_t";
			break;
		default:
			break;
		};
		break;
	/* extattr_get_file */
	case 357:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "int";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		case 3:
			p = "abi_uintptr_t";
			break;
		case 4:
			p = "size_t";
			break;
		default:
			break;
		};
		break;
	/* extattr_delete_file */
	case 358:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "int";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* aio_waitcomplete */
	case 359:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* getresuid */
	case 360:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* getresgid */
	case 361:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* kqueue */
	case 362:
		break;
	/* extattr_set_fd */
	case 371:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "int";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		case 3:
			p = "abi_uintptr_t";
			break;
		case 4:
			p = "size_t";
			break;
		default:
			break;
		};
		break;
	/* extattr_get_fd */
	case 372:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "int";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		case 3:
			p = "abi_uintptr_t";
			break;
		case 4:
			p = "size_t";
			break;
		default:
			break;
		};
		break;
	/* extattr_delete_fd */
	case 373:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "int";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* __setugid */
	case 374:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* eaccess */
	case 376:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* afs3_syscall */
	case 377:
		switch(ndx) {
		case 0:
			p = "long";
			break;
		case 1:
			p = "long";
			break;
		case 2:
			p = "long";
			break;
		case 3:
			p = "long";
			break;
		case 4:
			p = "long";
			break;
		case 5:
			p = "long";
			break;
		case 6:
			p = "long";
			break;
		default:
			break;
		};
		break;
	/* nmount */
	case 378:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "unsigned int";
			break;
		case 2:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* __mac_get_proc */
	case 384:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* __mac_set_proc */
	case 385:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* __mac_get_fd */
	case 386:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* __mac_get_file */
	case 387:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* __mac_set_fd */
	case 388:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* __mac_set_file */
	case 389:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* kenv */
	case 390:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		case 3:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* lchflags */
	case 391:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "u_long";
			break;
		default:
			break;
		};
		break;
	/* uuidgen */
	case 392:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* sendfile */
	case 393:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "int";
			break;
		case 2:
			p = "off_t";
			break;
		case 3:
			p = "size_t";
			break;
		case 4:
			p = "abi_uintptr_t";
			break;
		case 5:
			p = "abi_uintptr_t";
			break;
		case 6:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* mac_syscall */
	case 394:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "int";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* ksem_close */
	case 400:
		switch(ndx) {
		case 0:
			p = "semid_t";
			break;
		default:
			break;
		};
		break;
	/* ksem_post */
	case 401:
		switch(ndx) {
		case 0:
			p = "semid_t";
			break;
		default:
			break;
		};
		break;
	/* ksem_wait */
	case 402:
		switch(ndx) {
		case 0:
			p = "semid_t";
			break;
		default:
			break;
		};
		break;
	/* ksem_trywait */
	case 403:
		switch(ndx) {
		case 0:
			p = "semid_t";
			break;
		default:
			break;
		};
		break;
	/* ksem_init */
	case 404:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "unsigned int";
			break;
		default:
			break;
		};
		break;
	/* ksem_open */
	case 405:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "int";
			break;
		case 3:
			p = "mode_t";
			break;
		case 4:
			p = "unsigned int";
			break;
		default:
			break;
		};
		break;
	/* ksem_unlink */
	case 406:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* ksem_getvalue */
	case 407:
		switch(ndx) {
		case 0:
			p = "semid_t";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* ksem_destroy */
	case 408:
		switch(ndx) {
		case 0:
			p = "semid_t";
			break;
		default:
			break;
		};
		break;
	/* __mac_get_pid */
	case 409:
		switch(ndx) {
		case 0:
			p = "pid_t";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* __mac_get_link */
	case 410:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* __mac_set_link */
	case 411:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* extattr_set_link */
	case 412:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "int";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		case 3:
			p = "abi_uintptr_t";
			break;
		case 4:
			p = "size_t";
			break;
		default:
			break;
		};
		break;
	/* extattr_get_link */
	case 413:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "int";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		case 3:
			p = "abi_uintptr_t";
			break;
		case 4:
			p = "size_t";
			break;
		default:
			break;
		};
		break;
	/* extattr_delete_link */
	case 414:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "int";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* __mac_execve */
	case 415:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		case 3:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* sigaction */
	case 416:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* sigreturn */
	case 417:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* getcontext */
	case 421:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* setcontext */
	case 422:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* swapcontext */
	case 423:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* swapoff */
	case 424:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* __acl_get_link */
	case 425:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "acl_type_t";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* __acl_set_link */
	case 426:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "acl_type_t";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* __acl_delete_link */
	case 427:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "acl_type_t";
			break;
		default:
			break;
		};
		break;
	/* __acl_aclcheck_link */
	case 428:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "acl_type_t";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* sigwait */
	case 429:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* thr_create */
	case 430:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* thr_exit */
	case 431:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* thr_self */
	case 432:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* thr_kill */
	case 433:
		switch(ndx) {
		case 0:
			p = "long";
			break;
		case 1:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* jail_attach */
	case 436:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* extattr_list_fd */
	case 437:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "int";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		case 3:
			p = "size_t";
			break;
		default:
			break;
		};
		break;
	/* extattr_list_file */
	case 438:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "int";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		case 3:
			p = "size_t";
			break;
		default:
			break;
		};
		break;
	/* extattr_list_link */
	case 439:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "int";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		case 3:
			p = "size_t";
			break;
		default:
			break;
		};
		break;
	/* ksem_timedwait */
	case 441:
		switch(ndx) {
		case 0:
			p = "semid_t";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* thr_suspend */
	case 442:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* thr_wake */
	case 443:
		switch(ndx) {
		case 0:
			p = "long";
			break;
		default:
			break;
		};
		break;
	/* kldunloadf */
	case 444:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* audit */
	case 445:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "u_int";
			break;
		default:
			break;
		};
		break;
	/* auditon */
	case 446:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "u_int";
			break;
		default:
			break;
		};
		break;
	/* getauid */
	case 447:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* setauid */
	case 448:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* getaudit */
	case 449:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* setaudit */
	case 450:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* getaudit_addr */
	case 451:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "u_int";
			break;
		default:
			break;
		};
		break;
	/* setaudit_addr */
	case 452:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "u_int";
			break;
		default:
			break;
		};
		break;
	/* auditctl */
	case 453:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* _umtx_op */
	case 454:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "int";
			break;
		case 2:
			p = "u_long";
			break;
		case 3:
			p = "abi_uintptr_t";
			break;
		case 4:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* thr_new */
	case 455:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* sigqueue */
	case 456:
		switch(ndx) {
		case 0:
			p = "pid_t";
			break;
		case 1:
			p = "int";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* kmq_open */
	case 457:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "int";
			break;
		case 2:
			p = "mode_t";
			break;
		case 3:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* kmq_setattr */
	case 458:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* kmq_timedreceive */
	case 459:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "size_t";
			break;
		case 3:
			p = "abi_uintptr_t";
			break;
		case 4:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* kmq_timedsend */
	case 460:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "size_t";
			break;
		case 3:
			p = "unsigned";
			break;
		case 4:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* kmq_notify */
	case 461:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* kmq_unlink */
	case 462:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* abort2 */
	case 463:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "int";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* thr_set_name */
	case 464:
		switch(ndx) {
		case 0:
			p = "long";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* aio_fsync */
	case 465:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* rtprio_thread */
	case 466:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "lwpid_t";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* sctp_peeloff */
	case 471:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "uint32_t";
			break;
		default:
			break;
		};
		break;
	/* sctp_generic_sendmsg */
	case 472:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "int";
			break;
		case 3:
			p = "abi_uintptr_t";
			break;
		case 4:
			p = "__socklen_t";
			break;
		case 5:
			p = "abi_uintptr_t";
			break;
		case 6:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* sctp_generic_sendmsg_iov */
	case 473:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "int";
			break;
		case 3:
			p = "abi_uintptr_t";
			break;
		case 4:
			p = "__socklen_t";
			break;
		case 5:
			p = "abi_uintptr_t";
			break;
		case 6:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* sctp_generic_recvmsg */
	case 474:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "int";
			break;
		case 3:
			p = "abi_uintptr_t";
			break;
		case 4:
			p = "abi_uintptr_t";
			break;
		case 5:
			p = "abi_uintptr_t";
			break;
		case 6:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* pread */
	case 475:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "size_t";
			break;
		case 3:
			p = "off_t";
			break;
		default:
			break;
		};
		break;
	/* pwrite */
	case 476:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "size_t";
			break;
		case 3:
			p = "off_t";
			break;
		default:
			break;
		};
		break;
	/* mmap */
	case 477:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "size_t";
			break;
		case 2:
			p = "int";
			break;
		case 3:
			p = "int";
			break;
		case 4:
			p = "int";
			break;
		case 5:
			p = "off_t";
			break;
		default:
			break;
		};
		break;
	/* lseek */
	case 478:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "off_t";
			break;
		case 2:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* truncate */
	case 479:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "off_t";
			break;
		default:
			break;
		};
		break;
	/* ftruncate */
	case 480:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "off_t";
			break;
		default:
			break;
		};
		break;
	/* thr_kill2 */
	case 481:
		switch(ndx) {
		case 0:
			p = "pid_t";
			break;
		case 1:
			p = "long";
			break;
		case 2:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* shm_unlink */
	case 483:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* cpuset */
	case 484:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* cpuset_setid */
	case 485:
		switch(ndx) {
		case 0:
			p = "cpuwhich_t";
			break;
		case 1:
			p = "id_t";
			break;
		case 2:
			p = "cpusetid_t";
			break;
		default:
			break;
		};
		break;
	/* cpuset_getid */
	case 486:
		switch(ndx) {
		case 0:
			p = "cpulevel_t";
			break;
		case 1:
			p = "cpuwhich_t";
			break;
		case 2:
			p = "id_t";
			break;
		case 3:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* cpuset_getaffinity */
	case 487:
		switch(ndx) {
		case 0:
			p = "cpulevel_t";
			break;
		case 1:
			p = "cpuwhich_t";
			break;
		case 2:
			p = "id_t";
			break;
		case 3:
			p = "size_t";
			break;
		case 4:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* cpuset_setaffinity */
	case 488:
		switch(ndx) {
		case 0:
			p = "cpulevel_t";
			break;
		case 1:
			p = "cpuwhich_t";
			break;
		case 2:
			p = "id_t";
			break;
		case 3:
			p = "size_t";
			break;
		case 4:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* faccessat */
	case 489:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "int";
			break;
		case 3:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* fchmodat */
	case 490:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "mode_t";
			break;
		case 3:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* fchownat */
	case 491:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "uid_t";
			break;
		case 3:
			p = "gid_t";
			break;
		case 4:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* fexecve */
	case 492:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* futimesat */
	case 494:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* linkat */
	case 495:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "int";
			break;
		case 3:
			p = "abi_uintptr_t";
			break;
		case 4:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* mkdirat */
	case 496:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "mode_t";
			break;
		default:
			break;
		};
		break;
	/* mkfifoat */
	case 497:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "mode_t";
			break;
		default:
			break;
		};
		break;
	/* openat */
	case 499:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "int";
			break;
		case 3:
			p = "mode_t";
			break;
		default:
			break;
		};
		break;
	/* readlinkat */
	case 500:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		case 3:
			p = "size_t";
			break;
		default:
			break;
		};
		break;
	/* renameat */
	case 501:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "int";
			break;
		case 3:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* symlinkat */
	case 502:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "int";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* unlinkat */
	case 503:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* posix_openpt */
	case 504:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* gssd_syscall */
	case 505:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* jail_get */
	case 506:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "unsigned int";
			break;
		case 2:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* jail_set */
	case 507:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "unsigned int";
			break;
		case 2:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* jail_remove */
	case 508:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* __semctl */
	case 510:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "int";
			break;
		case 2:
			p = "int";
			break;
		case 3:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* msgctl */
	case 511:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "int";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* shmctl */
	case 512:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "int";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* lpathconf */
	case 513:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* __cap_rights_get */
	case 515:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "int";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* cap_enter */
	case 516:
		break;
	/* cap_getmode */
	case 517:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* pdfork */
	case 518:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* pdkill */
	case 519:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* pdgetpid */
	case 520:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* pselect */
	case 522:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		case 3:
			p = "abi_uintptr_t";
			break;
		case 4:
			p = "abi_uintptr_t";
			break;
		case 5:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* getloginclass */
	case 523:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "size_t";
			break;
		default:
			break;
		};
		break;
	/* setloginclass */
	case 524:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* rctl_get_racct */
	case 525:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "size_t";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		case 3:
			p = "size_t";
			break;
		default:
			break;
		};
		break;
	/* rctl_get_rules */
	case 526:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "size_t";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		case 3:
			p = "size_t";
			break;
		default:
			break;
		};
		break;
	/* rctl_get_limits */
	case 527:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "size_t";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		case 3:
			p = "size_t";
			break;
		default:
			break;
		};
		break;
	/* rctl_add_rule */
	case 528:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "size_t";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		case 3:
			p = "size_t";
			break;
		default:
			break;
		};
		break;
	/* rctl_remove_rule */
	case 529:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "size_t";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		case 3:
			p = "size_t";
			break;
		default:
			break;
		};
		break;
	/* posix_fallocate */
	case 530:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "off_t";
			break;
		case 2:
			p = "off_t";
			break;
		default:
			break;
		};
		break;
	/* posix_fadvise */
	case 531:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "off_t";
			break;
		case 2:
			p = "off_t";
			break;
		case 3:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* wait6 */
	case 532:
		switch(ndx) {
		case 0:
			p = "idtype_t";
			break;
		case 1:
			p = "id_t";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		case 3:
			p = "int";
			break;
		case 4:
			p = "abi_uintptr_t";
			break;
		case 5:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* cap_rights_limit */
	case 533:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* cap_ioctls_limit */
	case 534:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "size_t";
			break;
		default:
			break;
		};
		break;
	/* cap_ioctls_get */
	case 535:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "size_t";
			break;
		default:
			break;
		};
		break;
	/* cap_fcntls_limit */
	case 536:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "uint32_t";
			break;
		default:
			break;
		};
		break;
	/* cap_fcntls_get */
	case 537:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* bindat */
	case 538:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "int";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		case 3:
			p = "__socklen_t";
			break;
		default:
			break;
		};
		break;
	/* connectat */
	case 539:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "int";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		case 3:
			p = "__socklen_t";
			break;
		default:
			break;
		};
		break;
	/* chflagsat */
	case 540:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "u_long";
			break;
		case 3:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* accept4 */
	case 541:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		case 3:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* pipe2 */
	case 542:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* aio_mlock */
	case 543:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* procctl */
	case 544:
		switch(ndx) {
		case 0:
			p = "idtype_t";
			break;
		case 1:
			p = "id_t";
			break;
		case 2:
			p = "int";
			break;
		case 3:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* ppoll */
	case 545:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "u_int";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		case 3:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* futimens */
	case 546:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* utimensat */
	case 547:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		case 3:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* fdatasync */
	case 550:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* fstat */
	case 551:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* fstatat */
	case 552:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		case 3:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* fhstat */
	case 553:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* getdirentries */
	case 554:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "size_t";
			break;
		case 3:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* statfs */
	case 555:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* fstatfs */
	case 556:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* getfsstat */
	case 557:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "long";
			break;
		case 2:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* fhstatfs */
	case 558:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* mknodat */
	case 559:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "mode_t";
			break;
		case 3:
			p = "dev_t";
			break;
		default:
			break;
		};
		break;
	/* kevent */
	case 560:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "int";
			break;
		case 3:
			p = "abi_uintptr_t";
			break;
		case 4:
			p = "int";
			break;
		case 5:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* cpuset_getdomain */
	case 561:
		switch(ndx) {
		case 0:
			p = "cpulevel_t";
			break;
		case 1:
			p = "cpuwhich_t";
			break;
		case 2:
			p = "id_t";
			break;
		case 3:
			p = "size_t";
			break;
		case 4:
			p = "abi_uintptr_t";
			break;
		case 5:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* cpuset_setdomain */
	case 562:
		switch(ndx) {
		case 0:
			p = "cpulevel_t";
			break;
		case 1:
			p = "cpuwhich_t";
			break;
		case 2:
			p = "id_t";
			break;
		case 3:
			p = "size_t";
			break;
		case 4:
			p = "abi_uintptr_t";
			break;
		case 5:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* getrandom */
	case 563:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "size_t";
			break;
		case 2:
			p = "unsigned int";
			break;
		default:
			break;
		};
		break;
	/* getfhat */
	case 564:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		case 3:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* fhlink */
	case 565:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* fhlinkat */
	case 566:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "int";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* fhreadlink */
	case 567:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "size_t";
			break;
		default:
			break;
		};
		break;
	/* funlinkat */
	case 568:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "int";
			break;
		case 3:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* copy_file_range */
	case 569:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "int";
			break;
		case 3:
			p = "abi_uintptr_t";
			break;
		case 4:
			p = "size_t";
			break;
		case 5:
			p = "unsigned int";
			break;
		default:
			break;
		};
		break;
	/* __sysctlbyname */
	case 570:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "size_t";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		case 3:
			p = "abi_uintptr_t";
			break;
		case 4:
			p = "abi_uintptr_t";
			break;
		case 5:
			p = "size_t";
			break;
		default:
			break;
		};
		break;
	/* shm_open2 */
	case 571:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "int";
			break;
		case 2:
			p = "mode_t";
			break;
		case 3:
			p = "int";
			break;
		case 4:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* shm_rename */
	case 572:
		switch(ndx) {
		case 0:
			p = "abi_uintptr_t";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* sigfastblock */
	case 573:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	/* __realpathat */
	case 574:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		case 2:
			p = "abi_uintptr_t";
			break;
		case 3:
			p = "size_t";
			break;
		case 4:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* close_range */
	case 575:
		switch(ndx) {
		case 0:
			p = "u_int";
			break;
		case 1:
			p = "u_int";
			break;
		case 2:
			p = "int";
			break;
		default:
			break;
		};
		break;
	/* rpctls_syscall */
	case 576:
		switch(ndx) {
		case 0:
			p = "int";
			break;
		case 1:
			p = "abi_uintptr_t";
			break;
		default:
			break;
		};
		break;
	default:
		break;
	};
	if (p != NULL)
		strlcpy(desc, p, descsz);
}
static void
systrace_return_setargdesc(int sysnum, int ndx, char *desc, size_t descsz)
{
	const char *p = NULL;
	switch (sysnum) {
	/* nosys */
	case 0:
	/* sys_exit */
	case 1:
		if (ndx == 0 || ndx == 1)
			p = "void";
		break;
	/* fork */
	case 2:
	/* read */
	case 3:
		if (ndx == 0 || ndx == 1)
			p = "ssize_t";
		break;
	/* write */
	case 4:
		if (ndx == 0 || ndx == 1)
			p = "ssize_t";
		break;
	/* open */
	case 5:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* close */
	case 6:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* wait4 */
	case 7:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* link */
	case 9:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* unlink */
	case 10:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* chdir */
	case 12:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* fchdir */
	case 13:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* chmod */
	case 15:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* chown */
	case 16:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* break */
	case 17:
		if (ndx == 0 || ndx == 1)
			p = "void *";
		break;
	/* getpid */
	case 20:
	/* mount */
	case 21:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* unmount */
	case 22:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* setuid */
	case 23:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* getuid */
	case 24:
	/* geteuid */
	case 25:
	/* ptrace */
	case 26:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* recvmsg */
	case 27:
		if (ndx == 0 || ndx == 1)
			p = "ssize_t";
		break;
	/* sendmsg */
	case 28:
		if (ndx == 0 || ndx == 1)
			p = "ssize_t";
		break;
	/* recvfrom */
	case 29:
		if (ndx == 0 || ndx == 1)
			p = "ssize_t";
		break;
	/* accept */
	case 30:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* getpeername */
	case 31:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* getsockname */
	case 32:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* access */
	case 33:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* chflags */
	case 34:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* fchflags */
	case 35:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* sync */
	case 36:
	/* kill */
	case 37:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* getppid */
	case 39:
	/* dup */
	case 41:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* getegid */
	case 43:
	/* profil */
	case 44:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* ktrace */
	case 45:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* getgid */
	case 47:
	/* getlogin */
	case 49:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* setlogin */
	case 50:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* acct */
	case 51:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* sigaltstack */
	case 53:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* ioctl */
	case 54:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* reboot */
	case 55:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* revoke */
	case 56:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* symlink */
	case 57:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* readlink */
	case 58:
		if (ndx == 0 || ndx == 1)
			p = "ssize_t";
		break;
	/* execve */
	case 59:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* umask */
	case 60:
		if (ndx == 0 || ndx == 1)
			p = "mode_t";
		break;
	/* chroot */
	case 61:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* msync */
	case 65:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* vfork */
	case 66:
	/* sbrk */
	case 69:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* sstk */
	case 70:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* munmap */
	case 73:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* mprotect */
	case 74:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* madvise */
	case 75:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* mincore */
	case 78:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* getgroups */
	case 79:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* setgroups */
	case 80:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* getpgrp */
	case 81:
	/* setpgid */
	case 82:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* setitimer */
	case 83:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* swapon */
	case 85:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* getitimer */
	case 86:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* getdtablesize */
	case 89:
	/* dup2 */
	case 90:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* fcntl */
	case 92:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* select */
	case 93:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* fsync */
	case 95:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* setpriority */
	case 96:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* socket */
	case 97:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* connect */
	case 98:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* getpriority */
	case 100:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* bind */
	case 104:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* setsockopt */
	case 105:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* listen */
	case 106:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* gettimeofday */
	case 116:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* getrusage */
	case 117:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* getsockopt */
	case 118:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* readv */
	case 120:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* writev */
	case 121:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* settimeofday */
	case 122:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* fchown */
	case 123:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* fchmod */
	case 124:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* setreuid */
	case 126:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* setregid */
	case 127:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* rename */
	case 128:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* flock */
	case 131:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* mkfifo */
	case 132:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* sendto */
	case 133:
		if (ndx == 0 || ndx == 1)
			p = "ssize_t";
		break;
	/* shutdown */
	case 134:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* socketpair */
	case 135:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* mkdir */
	case 136:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* rmdir */
	case 137:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* utimes */
	case 138:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* adjtime */
	case 140:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* setsid */
	case 147:
	/* quotactl */
	case 148:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* nlm_syscall */
	case 154:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* nfssvc */
	case 155:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* lgetfh */
	case 160:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* getfh */
	case 161:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* sysarch */
	case 165:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* rtprio */
	case 166:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* semsys */
	case 169:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* msgsys */
	case 170:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* shmsys */
	case 171:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* setfib */
	case 175:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* ntp_adjtime */
	case 176:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* setgid */
	case 181:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* setegid */
	case 182:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* seteuid */
	case 183:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* pathconf */
	case 191:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* fpathconf */
	case 192:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* getrlimit */
	case 194:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* setrlimit */
	case 195:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* nosys */
	case 198:
	/* __sysctl */
	case 202:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* mlock */
	case 203:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* munlock */
	case 204:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* undelete */
	case 205:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* futimes */
	case 206:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* getpgid */
	case 207:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* poll */
	case 209:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* lkmnosys */
	case 210:
	/* lkmnosys */
	case 211:
	/* lkmnosys */
	case 212:
	/* lkmnosys */
	case 213:
	/* lkmnosys */
	case 214:
	/* lkmnosys */
	case 215:
	/* lkmnosys */
	case 216:
	/* lkmnosys */
	case 217:
	/* lkmnosys */
	case 218:
	/* lkmnosys */
	case 219:
	/* semget */
	case 221:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* semop */
	case 222:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* msgget */
	case 225:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* msgsnd */
	case 226:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* msgrcv */
	case 227:
		if (ndx == 0 || ndx == 1)
			p = "ssize_t";
		break;
	/* shmat */
	case 228:
		if (ndx == 0 || ndx == 1)
			p = "void *";
		break;
	/* shmdt */
	case 230:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* shmget */
	case 231:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* clock_gettime */
	case 232:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* clock_settime */
	case 233:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* clock_getres */
	case 234:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* ktimer_create */
	case 235:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* ktimer_delete */
	case 236:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* ktimer_settime */
	case 237:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* ktimer_gettime */
	case 238:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* ktimer_getoverrun */
	case 239:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* nanosleep */
	case 240:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* ffclock_getcounter */
	case 241:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* ffclock_setestimate */
	case 242:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* ffclock_getestimate */
	case 243:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* clock_nanosleep */
	case 244:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* clock_getcpuclockid2 */
	case 247:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* ntp_gettime */
	case 248:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* minherit */
	case 250:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* rfork */
	case 251:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* issetugid */
	case 253:
	/* lchown */
	case 254:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* aio_read */
	case 255:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* aio_write */
	case 256:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* lio_listio */
	case 257:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* kbounce */
	case 258:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* flag_captured */
	case 259:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* lchmod */
	case 274:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* lutimes */
	case 276:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* preadv */
	case 289:
		if (ndx == 0 || ndx == 1)
			p = "ssize_t";
		break;
	/* pwritev */
	case 290:
		if (ndx == 0 || ndx == 1)
			p = "ssize_t";
		break;
	/* fhopen */
	case 298:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* modnext */
	case 300:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* modstat */
	case 301:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* modfnext */
	case 302:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* modfind */
	case 303:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* kldload */
	case 304:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* kldunload */
	case 305:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* kldfind */
	case 306:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* kldnext */
	case 307:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* kldstat */
	case 308:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* kldfirstmod */
	case 309:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* getsid */
	case 310:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* setresuid */
	case 311:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* setresgid */
	case 312:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* aio_return */
	case 314:
		if (ndx == 0 || ndx == 1)
			p = "ssize_t";
		break;
	/* aio_suspend */
	case 315:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* aio_cancel */
	case 316:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* aio_error */
	case 317:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* yield */
	case 321:
	/* mlockall */
	case 324:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* munlockall */
	case 325:
	/* __getcwd */
	case 326:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* sched_setparam */
	case 327:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* sched_getparam */
	case 328:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* sched_setscheduler */
	case 329:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* sched_getscheduler */
	case 330:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* sched_yield */
	case 331:
	/* sched_get_priority_max */
	case 332:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* sched_get_priority_min */
	case 333:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* sched_rr_get_interval */
	case 334:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* utrace */
	case 335:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* kldsym */
	case 337:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* jail */
	case 338:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* nnpfs_syscall */
	case 339:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* sigprocmask */
	case 340:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* sigsuspend */
	case 341:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* sigpending */
	case 343:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* sigtimedwait */
	case 345:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* sigwaitinfo */
	case 346:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* __acl_get_file */
	case 347:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* __acl_set_file */
	case 348:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* __acl_get_fd */
	case 349:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* __acl_set_fd */
	case 350:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* __acl_delete_file */
	case 351:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* __acl_delete_fd */
	case 352:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* __acl_aclcheck_file */
	case 353:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* __acl_aclcheck_fd */
	case 354:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* extattrctl */
	case 355:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* extattr_set_file */
	case 356:
		if (ndx == 0 || ndx == 1)
			p = "ssize_t";
		break;
	/* extattr_get_file */
	case 357:
		if (ndx == 0 || ndx == 1)
			p = "ssize_t";
		break;
	/* extattr_delete_file */
	case 358:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* aio_waitcomplete */
	case 359:
		if (ndx == 0 || ndx == 1)
			p = "ssize_t";
		break;
	/* getresuid */
	case 360:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* getresgid */
	case 361:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* kqueue */
	case 362:
	/* extattr_set_fd */
	case 371:
		if (ndx == 0 || ndx == 1)
			p = "ssize_t";
		break;
	/* extattr_get_fd */
	case 372:
		if (ndx == 0 || ndx == 1)
			p = "ssize_t";
		break;
	/* extattr_delete_fd */
	case 373:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* __setugid */
	case 374:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* eaccess */
	case 376:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* afs3_syscall */
	case 377:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* nmount */
	case 378:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* __mac_get_proc */
	case 384:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* __mac_set_proc */
	case 385:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* __mac_get_fd */
	case 386:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* __mac_get_file */
	case 387:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* __mac_set_fd */
	case 388:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* __mac_set_file */
	case 389:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* kenv */
	case 390:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* lchflags */
	case 391:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* uuidgen */
	case 392:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* sendfile */
	case 393:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* mac_syscall */
	case 394:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* ksem_close */
	case 400:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* ksem_post */
	case 401:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* ksem_wait */
	case 402:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* ksem_trywait */
	case 403:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* ksem_init */
	case 404:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* ksem_open */
	case 405:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* ksem_unlink */
	case 406:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* ksem_getvalue */
	case 407:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* ksem_destroy */
	case 408:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* __mac_get_pid */
	case 409:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* __mac_get_link */
	case 410:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* __mac_set_link */
	case 411:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* extattr_set_link */
	case 412:
		if (ndx == 0 || ndx == 1)
			p = "ssize_t";
		break;
	/* extattr_get_link */
	case 413:
		if (ndx == 0 || ndx == 1)
			p = "ssize_t";
		break;
	/* extattr_delete_link */
	case 414:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* __mac_execve */
	case 415:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* sigaction */
	case 416:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* sigreturn */
	case 417:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* getcontext */
	case 421:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* setcontext */
	case 422:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* swapcontext */
	case 423:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* swapoff */
	case 424:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* __acl_get_link */
	case 425:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* __acl_set_link */
	case 426:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* __acl_delete_link */
	case 427:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* __acl_aclcheck_link */
	case 428:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* sigwait */
	case 429:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* thr_create */
	case 430:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* thr_exit */
	case 431:
		if (ndx == 0 || ndx == 1)
			p = "void";
		break;
	/* thr_self */
	case 432:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* thr_kill */
	case 433:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* jail_attach */
	case 436:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* extattr_list_fd */
	case 437:
		if (ndx == 0 || ndx == 1)
			p = "ssize_t";
		break;
	/* extattr_list_file */
	case 438:
		if (ndx == 0 || ndx == 1)
			p = "ssize_t";
		break;
	/* extattr_list_link */
	case 439:
		if (ndx == 0 || ndx == 1)
			p = "ssize_t";
		break;
	/* ksem_timedwait */
	case 441:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* thr_suspend */
	case 442:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* thr_wake */
	case 443:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* kldunloadf */
	case 444:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* audit */
	case 445:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* auditon */
	case 446:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* getauid */
	case 447:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* setauid */
	case 448:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* getaudit */
	case 449:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* setaudit */
	case 450:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* getaudit_addr */
	case 451:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* setaudit_addr */
	case 452:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* auditctl */
	case 453:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* _umtx_op */
	case 454:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* thr_new */
	case 455:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* sigqueue */
	case 456:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* kmq_open */
	case 457:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* kmq_setattr */
	case 458:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* kmq_timedreceive */
	case 459:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* kmq_timedsend */
	case 460:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* kmq_notify */
	case 461:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* kmq_unlink */
	case 462:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* abort2 */
	case 463:
		if (ndx == 0 || ndx == 1)
			p = "void";
		break;
	/* thr_set_name */
	case 464:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* aio_fsync */
	case 465:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* rtprio_thread */
	case 466:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* sctp_peeloff */
	case 471:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* sctp_generic_sendmsg */
	case 472:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* sctp_generic_sendmsg_iov */
	case 473:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* sctp_generic_recvmsg */
	case 474:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* pread */
	case 475:
		if (ndx == 0 || ndx == 1)
			p = "ssize_t";
		break;
	/* pwrite */
	case 476:
		if (ndx == 0 || ndx == 1)
			p = "ssize_t";
		break;
	/* mmap */
	case 477:
		if (ndx == 0 || ndx == 1)
			p = "void *";
		break;
	/* lseek */
	case 478:
		if (ndx == 0 || ndx == 1)
			p = "off_t";
		break;
	/* truncate */
	case 479:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* ftruncate */
	case 480:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* thr_kill2 */
	case 481:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* shm_unlink */
	case 483:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* cpuset */
	case 484:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* cpuset_setid */
	case 485:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* cpuset_getid */
	case 486:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* cpuset_getaffinity */
	case 487:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* cpuset_setaffinity */
	case 488:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* faccessat */
	case 489:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* fchmodat */
	case 490:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* fchownat */
	case 491:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* fexecve */
	case 492:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* futimesat */
	case 494:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* linkat */
	case 495:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* mkdirat */
	case 496:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* mkfifoat */
	case 497:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* openat */
	case 499:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* readlinkat */
	case 500:
		if (ndx == 0 || ndx == 1)
			p = "ssize_t";
		break;
	/* renameat */
	case 501:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* symlinkat */
	case 502:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* unlinkat */
	case 503:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* posix_openpt */
	case 504:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* gssd_syscall */
	case 505:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* jail_get */
	case 506:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* jail_set */
	case 507:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* jail_remove */
	case 508:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* __semctl */
	case 510:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* msgctl */
	case 511:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* shmctl */
	case 512:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* lpathconf */
	case 513:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* __cap_rights_get */
	case 515:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* cap_enter */
	case 516:
	/* cap_getmode */
	case 517:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* pdfork */
	case 518:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* pdkill */
	case 519:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* pdgetpid */
	case 520:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* pselect */
	case 522:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* getloginclass */
	case 523:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* setloginclass */
	case 524:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* rctl_get_racct */
	case 525:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* rctl_get_rules */
	case 526:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* rctl_get_limits */
	case 527:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* rctl_add_rule */
	case 528:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* rctl_remove_rule */
	case 529:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* posix_fallocate */
	case 530:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* posix_fadvise */
	case 531:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* wait6 */
	case 532:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* cap_rights_limit */
	case 533:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* cap_ioctls_limit */
	case 534:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* cap_ioctls_get */
	case 535:
		if (ndx == 0 || ndx == 1)
			p = "ssize_t";
		break;
	/* cap_fcntls_limit */
	case 536:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* cap_fcntls_get */
	case 537:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* bindat */
	case 538:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* connectat */
	case 539:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* chflagsat */
	case 540:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* accept4 */
	case 541:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* pipe2 */
	case 542:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* aio_mlock */
	case 543:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* procctl */
	case 544:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* ppoll */
	case 545:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* futimens */
	case 546:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* utimensat */
	case 547:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* fdatasync */
	case 550:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* fstat */
	case 551:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* fstatat */
	case 552:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* fhstat */
	case 553:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* getdirentries */
	case 554:
		if (ndx == 0 || ndx == 1)
			p = "ssize_t";
		break;
	/* statfs */
	case 555:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* fstatfs */
	case 556:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* getfsstat */
	case 557:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* fhstatfs */
	case 558:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* mknodat */
	case 559:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* kevent */
	case 560:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* cpuset_getdomain */
	case 561:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* cpuset_setdomain */
	case 562:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* getrandom */
	case 563:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* getfhat */
	case 564:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* fhlink */
	case 565:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* fhlinkat */
	case 566:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* fhreadlink */
	case 567:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* funlinkat */
	case 568:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* copy_file_range */
	case 569:
		if (ndx == 0 || ndx == 1)
			p = "ssize_t";
		break;
	/* __sysctlbyname */
	case 570:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* shm_open2 */
	case 571:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* shm_rename */
	case 572:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* sigfastblock */
	case 573:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* __realpathat */
	case 574:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* close_range */
	case 575:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	/* rpctls_syscall */
	case 576:
		if (ndx == 0 || ndx == 1)
			p = "int";
		break;
	default:
		break;
	};
	if (p != NULL)
		strlcpy(desc, p, descsz);
}
