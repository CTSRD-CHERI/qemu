/*
 * System call prototypes.
 *
 * DO NOT EDIT-- this file is automatically @generated.
 * $FreeBSD$
 */

#ifndef _TARGET_OS_PROTO_H_
#define	_TARGET_OS_PROTO_H_

#include <sys/signal.h>
#include <sys/acl.h>
#include <sys/cpuset.h>
#include <sys/domainset.h>
#include <sys/_ffcounter.h>
#include <sys/_semaphore.h>
#include <sys/ucontext.h>
#include <sys/wait.h>

#include <bsm/audit_kevents.h>

struct proc;

struct thread;

#define	PAD_(t)	(sizeof(abi_uintcap_t) <= sizeof(t) ? \
		0 : sizeof(abi_uintcap_t) - sizeof(t))

#if BYTE_ORDER == LITTLE_ENDIAN
#define	PADL_(t)	0
#define	PADR_(t)	PAD_(t)
#else
#define	PADL_(t)	PAD_(t)
#define	PADR_(t)	0
#endif

struct target_nosys_args {
	register_t dummy;
};
struct target_sys_exit_args {
	char rval_l_[PADL_(int)]; int rval; char rval_r_[PADR_(int)];
};
struct target_fork_args {
	register_t dummy;
};
struct target_read_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char buf_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t buf; char buf_r_[PADR_(abi_uintptr_t)];
	char nbyte_l_[PADL_(size_t)]; size_t nbyte; char nbyte_r_[PADR_(size_t)];
};
struct target_write_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char buf_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t buf; char buf_r_[PADR_(abi_uintptr_t)];
	char nbyte_l_[PADL_(size_t)]; size_t nbyte; char nbyte_r_[PADR_(size_t)];
};
struct target_open_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char flags_l_[PADL_(int)]; int flags; char flags_r_[PADR_(int)];
	char mode_l_[PADL_(mode_t)]; mode_t mode; char mode_r_[PADR_(mode_t)];
};
struct target_close_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
};
struct target_wait4_args {
	char pid_l_[PADL_(int)]; int pid; char pid_r_[PADR_(int)];
	char status_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t status; char status_r_[PADR_(abi_uintptr_t)];
	char options_l_[PADL_(int)]; int options; char options_r_[PADR_(int)];
	char rusage_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t rusage; char rusage_r_[PADR_(abi_uintptr_t)];
};
struct target_link_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char to_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t to; char to_r_[PADR_(abi_uintptr_t)];
};
struct target_unlink_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
};
struct target_chdir_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
};
struct target_fchdir_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
};
struct target_chmod_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char mode_l_[PADL_(mode_t)]; mode_t mode; char mode_r_[PADR_(mode_t)];
};
struct target_chown_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char uid_l_[PADL_(int)]; int uid; char uid_r_[PADR_(int)];
	char gid_l_[PADL_(int)]; int gid; char gid_r_[PADR_(int)];
};
struct target_break_args {
	char nsize_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t nsize; char nsize_r_[PADR_(abi_uintptr_t)];
};
struct target_getpid_args {
	register_t dummy;
};
struct target_mount_args {
	char type_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t type; char type_r_[PADR_(abi_uintptr_t)];
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char flags_l_[PADL_(int)]; int flags; char flags_r_[PADR_(int)];
	char data_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t data; char data_r_[PADR_(abi_uintptr_t)];
};
struct target_unmount_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char flags_l_[PADL_(int)]; int flags; char flags_r_[PADR_(int)];
};
struct target_setuid_args {
	char uid_l_[PADL_(uid_t)]; uid_t uid; char uid_r_[PADR_(uid_t)];
};
struct target_getuid_args {
	register_t dummy;
};
struct target_geteuid_args {
	register_t dummy;
};
struct target_ptrace_args {
	char req_l_[PADL_(int)]; int req; char req_r_[PADR_(int)];
	char pid_l_[PADL_(pid_t)]; pid_t pid; char pid_r_[PADR_(pid_t)];
	char addr_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t addr; char addr_r_[PADR_(abi_uintptr_t)];
	char data_l_[PADL_(int)]; int data; char data_r_[PADR_(int)];
};
struct target_recvmsg_args {
	char s_l_[PADL_(int)]; int s; char s_r_[PADR_(int)];
	char msg_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t msg; char msg_r_[PADR_(abi_uintptr_t)];
	char flags_l_[PADL_(int)]; int flags; char flags_r_[PADR_(int)];
};
struct target_sendmsg_args {
	char s_l_[PADL_(int)]; int s; char s_r_[PADR_(int)];
	char msg_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t msg; char msg_r_[PADR_(abi_uintptr_t)];
	char flags_l_[PADL_(int)]; int flags; char flags_r_[PADR_(int)];
};
struct target_recvfrom_args {
	char s_l_[PADL_(int)]; int s; char s_r_[PADR_(int)];
	char buf_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t buf; char buf_r_[PADR_(abi_uintptr_t)];
	char len_l_[PADL_(size_t)]; size_t len; char len_r_[PADR_(size_t)];
	char flags_l_[PADL_(int)]; int flags; char flags_r_[PADR_(int)];
	char from_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t from; char from_r_[PADR_(abi_uintptr_t)];
	char fromlenaddr_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t fromlenaddr; char fromlenaddr_r_[PADR_(abi_uintptr_t)];
};
struct target_accept_args {
	char s_l_[PADL_(int)]; int s; char s_r_[PADR_(int)];
	char name_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t name; char name_r_[PADR_(abi_uintptr_t)];
	char anamelen_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t anamelen; char anamelen_r_[PADR_(abi_uintptr_t)];
};
struct target_getpeername_args {
	char fdes_l_[PADL_(int)]; int fdes; char fdes_r_[PADR_(int)];
	char asa_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t asa; char asa_r_[PADR_(abi_uintptr_t)];
	char alen_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t alen; char alen_r_[PADR_(abi_uintptr_t)];
};
struct target_getsockname_args {
	char fdes_l_[PADL_(int)]; int fdes; char fdes_r_[PADR_(int)];
	char asa_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t asa; char asa_r_[PADR_(abi_uintptr_t)];
	char alen_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t alen; char alen_r_[PADR_(abi_uintptr_t)];
};
struct target_access_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char amode_l_[PADL_(int)]; int amode; char amode_r_[PADR_(int)];
};
struct target_chflags_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char flags_l_[PADL_(u_long)]; u_long flags; char flags_r_[PADR_(u_long)];
};
struct target_fchflags_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char flags_l_[PADL_(u_long)]; u_long flags; char flags_r_[PADR_(u_long)];
};
struct target_sync_args {
	register_t dummy;
};
struct target_kill_args {
	char pid_l_[PADL_(int)]; int pid; char pid_r_[PADR_(int)];
	char signum_l_[PADL_(int)]; int signum; char signum_r_[PADR_(int)];
};
struct target_getppid_args {
	register_t dummy;
};
struct target_dup_args {
	char fd_l_[PADL_(u_int)]; u_int fd; char fd_r_[PADR_(u_int)];
};
struct freebsd10_target_pipe_args {
	register_t dummy;
};
struct target_getegid_args {
	register_t dummy;
};
struct target_profil_args {
	char samples_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t samples; char samples_r_[PADR_(abi_uintptr_t)];
	char size_l_[PADL_(size_t)]; size_t size; char size_r_[PADR_(size_t)];
	char offset_l_[PADL_(size_t)]; size_t offset; char offset_r_[PADR_(size_t)];
	char scale_l_[PADL_(u_int)]; u_int scale; char scale_r_[PADR_(u_int)];
};
struct target_ktrace_args {
	char fname_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t fname; char fname_r_[PADR_(abi_uintptr_t)];
	char ops_l_[PADL_(int)]; int ops; char ops_r_[PADR_(int)];
	char facs_l_[PADL_(int)]; int facs; char facs_r_[PADR_(int)];
	char pid_l_[PADL_(int)]; int pid; char pid_r_[PADR_(int)];
};
struct target_getgid_args {
	register_t dummy;
};
struct target_getlogin_args {
	char namebuf_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t namebuf; char namebuf_r_[PADR_(abi_uintptr_t)];
	char namelen_l_[PADL_(u_int)]; u_int namelen; char namelen_r_[PADR_(u_int)];
};
struct target_setlogin_args {
	char namebuf_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t namebuf; char namebuf_r_[PADR_(abi_uintptr_t)];
};
struct target_acct_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
};
struct target_sigaltstack_args {
	char ss_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t ss; char ss_r_[PADR_(abi_uintptr_t)];
	char oss_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t oss; char oss_r_[PADR_(abi_uintptr_t)];
};
struct target_ioctl_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char com_l_[PADL_(u_long)]; u_long com; char com_r_[PADR_(u_long)];
	char data_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t data; char data_r_[PADR_(abi_uintptr_t)];
};
struct target_reboot_args {
	char opt_l_[PADL_(int)]; int opt; char opt_r_[PADR_(int)];
};
struct target_revoke_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
};
struct target_symlink_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char link_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t link; char link_r_[PADR_(abi_uintptr_t)];
};
struct target_readlink_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char buf_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t buf; char buf_r_[PADR_(abi_uintptr_t)];
	char count_l_[PADL_(size_t)]; size_t count; char count_r_[PADR_(size_t)];
};
struct target_execve_args {
	char fname_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t fname; char fname_r_[PADR_(abi_uintptr_t)];
	char argv_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t argv; char argv_r_[PADR_(abi_uintptr_t)];
	char envv_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t envv; char envv_r_[PADR_(abi_uintptr_t)];
};
struct target_umask_args {
	char newmask_l_[PADL_(mode_t)]; mode_t newmask; char newmask_r_[PADR_(mode_t)];
};
struct target_chroot_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
};
struct target_msync_args {
	char addr_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t addr; char addr_r_[PADR_(abi_uintptr_t)];
	char len_l_[PADL_(size_t)]; size_t len; char len_r_[PADR_(size_t)];
	char flags_l_[PADL_(int)]; int flags; char flags_r_[PADR_(int)];
};
struct target_vfork_args {
	register_t dummy;
};
struct target_sbrk_args {
	char incr_l_[PADL_(int)]; int incr; char incr_r_[PADR_(int)];
};
struct target_sstk_args {
	char incr_l_[PADL_(int)]; int incr; char incr_r_[PADR_(int)];
};
struct target_munmap_args {
	char addr_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t addr; char addr_r_[PADR_(abi_uintptr_t)];
	char len_l_[PADL_(size_t)]; size_t len; char len_r_[PADR_(size_t)];
};
struct target_mprotect_args {
	char addr_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t addr; char addr_r_[PADR_(abi_uintptr_t)];
	char len_l_[PADL_(size_t)]; size_t len; char len_r_[PADR_(size_t)];
	char prot_l_[PADL_(int)]; int prot; char prot_r_[PADR_(int)];
};
struct target_madvise_args {
	char addr_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t addr; char addr_r_[PADR_(abi_uintptr_t)];
	char len_l_[PADL_(size_t)]; size_t len; char len_r_[PADR_(size_t)];
	char behav_l_[PADL_(int)]; int behav; char behav_r_[PADR_(int)];
};
struct target_mincore_args {
	char addr_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t addr; char addr_r_[PADR_(abi_uintptr_t)];
	char len_l_[PADL_(size_t)]; size_t len; char len_r_[PADR_(size_t)];
	char vec_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t vec; char vec_r_[PADR_(abi_uintptr_t)];
};
struct target_getgroups_args {
	char gidsetsize_l_[PADL_(u_int)]; u_int gidsetsize; char gidsetsize_r_[PADR_(u_int)];
	char gidset_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t gidset; char gidset_r_[PADR_(abi_uintptr_t)];
};
struct target_setgroups_args {
	char gidsetsize_l_[PADL_(u_int)]; u_int gidsetsize; char gidsetsize_r_[PADR_(u_int)];
	char gidset_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t gidset; char gidset_r_[PADR_(abi_uintptr_t)];
};
struct target_getpgrp_args {
	register_t dummy;
};
struct target_setpgid_args {
	char pid_l_[PADL_(int)]; int pid; char pid_r_[PADR_(int)];
	char pgid_l_[PADL_(int)]; int pgid; char pgid_r_[PADR_(int)];
};
struct target_setitimer_args {
	char which_l_[PADL_(int)]; int which; char which_r_[PADR_(int)];
	char itv_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t itv; char itv_r_[PADR_(abi_uintptr_t)];
	char oitv_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t oitv; char oitv_r_[PADR_(abi_uintptr_t)];
};
struct target_swapon_args {
	char name_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t name; char name_r_[PADR_(abi_uintptr_t)];
};
struct target_getitimer_args {
	char which_l_[PADL_(int)]; int which; char which_r_[PADR_(int)];
	char itv_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t itv; char itv_r_[PADR_(abi_uintptr_t)];
};
struct target_getdtablesize_args {
	register_t dummy;
};
struct target_dup2_args {
	char from_l_[PADL_(u_int)]; u_int from; char from_r_[PADR_(u_int)];
	char to_l_[PADL_(u_int)]; u_int to; char to_r_[PADR_(u_int)];
};
struct target_fcntl_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char cmd_l_[PADL_(int)]; int cmd; char cmd_r_[PADR_(int)];
	char arg_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t arg; char arg_r_[PADR_(abi_uintptr_t)];
};
struct target_select_args {
	char nd_l_[PADL_(int)]; int nd; char nd_r_[PADR_(int)];
	char in_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t in; char in_r_[PADR_(abi_uintptr_t)];
	char ou_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t ou; char ou_r_[PADR_(abi_uintptr_t)];
	char ex_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t ex; char ex_r_[PADR_(abi_uintptr_t)];
	char tv_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t tv; char tv_r_[PADR_(abi_uintptr_t)];
};
struct target_fsync_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
};
struct target_setpriority_args {
	char which_l_[PADL_(int)]; int which; char which_r_[PADR_(int)];
	char who_l_[PADL_(int)]; int who; char who_r_[PADR_(int)];
	char prio_l_[PADL_(int)]; int prio; char prio_r_[PADR_(int)];
};
struct target_socket_args {
	char domain_l_[PADL_(int)]; int domain; char domain_r_[PADR_(int)];
	char type_l_[PADL_(int)]; int type; char type_r_[PADR_(int)];
	char protocol_l_[PADL_(int)]; int protocol; char protocol_r_[PADR_(int)];
};
struct target_connect_args {
	char s_l_[PADL_(int)]; int s; char s_r_[PADR_(int)];
	char name_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t name; char name_r_[PADR_(abi_uintptr_t)];
	char namelen_l_[PADL_(__socklen_t)]; __socklen_t namelen; char namelen_r_[PADR_(__socklen_t)];
};
struct target_getpriority_args {
	char which_l_[PADL_(int)]; int which; char which_r_[PADR_(int)];
	char who_l_[PADL_(int)]; int who; char who_r_[PADR_(int)];
};
struct target_bind_args {
	char s_l_[PADL_(int)]; int s; char s_r_[PADR_(int)];
	char name_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t name; char name_r_[PADR_(abi_uintptr_t)];
	char namelen_l_[PADL_(__socklen_t)]; __socklen_t namelen; char namelen_r_[PADR_(__socklen_t)];
};
struct target_setsockopt_args {
	char s_l_[PADL_(int)]; int s; char s_r_[PADR_(int)];
	char level_l_[PADL_(int)]; int level; char level_r_[PADR_(int)];
	char name_l_[PADL_(int)]; int name; char name_r_[PADR_(int)];
	char val_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t val; char val_r_[PADR_(abi_uintptr_t)];
	char valsize_l_[PADL_(__socklen_t)]; __socklen_t valsize; char valsize_r_[PADR_(__socklen_t)];
};
struct target_listen_args {
	char s_l_[PADL_(int)]; int s; char s_r_[PADR_(int)];
	char backlog_l_[PADL_(int)]; int backlog; char backlog_r_[PADR_(int)];
};
struct target_gettimeofday_args {
	char tp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t tp; char tp_r_[PADR_(abi_uintptr_t)];
	char tzp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t tzp; char tzp_r_[PADR_(abi_uintptr_t)];
};
struct target_getrusage_args {
	char who_l_[PADL_(int)]; int who; char who_r_[PADR_(int)];
	char rusage_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t rusage; char rusage_r_[PADR_(abi_uintptr_t)];
};
struct target_getsockopt_args {
	char s_l_[PADL_(int)]; int s; char s_r_[PADR_(int)];
	char level_l_[PADL_(int)]; int level; char level_r_[PADR_(int)];
	char name_l_[PADL_(int)]; int name; char name_r_[PADR_(int)];
	char val_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t val; char val_r_[PADR_(abi_uintptr_t)];
	char avalsize_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t avalsize; char avalsize_r_[PADR_(abi_uintptr_t)];
};
struct target_readv_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char iovp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t iovp; char iovp_r_[PADR_(abi_uintptr_t)];
	char iovcnt_l_[PADL_(u_int)]; u_int iovcnt; char iovcnt_r_[PADR_(u_int)];
};
struct target_writev_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char iovp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t iovp; char iovp_r_[PADR_(abi_uintptr_t)];
	char iovcnt_l_[PADL_(u_int)]; u_int iovcnt; char iovcnt_r_[PADR_(u_int)];
};
struct target_settimeofday_args {
	char tv_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t tv; char tv_r_[PADR_(abi_uintptr_t)];
	char tzp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t tzp; char tzp_r_[PADR_(abi_uintptr_t)];
};
struct target_fchown_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char uid_l_[PADL_(int)]; int uid; char uid_r_[PADR_(int)];
	char gid_l_[PADL_(int)]; int gid; char gid_r_[PADR_(int)];
};
struct target_fchmod_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char mode_l_[PADL_(mode_t)]; mode_t mode; char mode_r_[PADR_(mode_t)];
};
struct target_setreuid_args {
	char ruid_l_[PADL_(int)]; int ruid; char ruid_r_[PADR_(int)];
	char euid_l_[PADL_(int)]; int euid; char euid_r_[PADR_(int)];
};
struct target_setregid_args {
	char rgid_l_[PADL_(int)]; int rgid; char rgid_r_[PADR_(int)];
	char egid_l_[PADL_(int)]; int egid; char egid_r_[PADR_(int)];
};
struct target_rename_args {
	char from_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t from; char from_r_[PADR_(abi_uintptr_t)];
	char to_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t to; char to_r_[PADR_(abi_uintptr_t)];
};
struct target_flock_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char how_l_[PADL_(int)]; int how; char how_r_[PADR_(int)];
};
struct target_mkfifo_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char mode_l_[PADL_(mode_t)]; mode_t mode; char mode_r_[PADR_(mode_t)];
};
struct target_sendto_args {
	char s_l_[PADL_(int)]; int s; char s_r_[PADR_(int)];
	char buf_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t buf; char buf_r_[PADR_(abi_uintptr_t)];
	char len_l_[PADL_(size_t)]; size_t len; char len_r_[PADR_(size_t)];
	char flags_l_[PADL_(int)]; int flags; char flags_r_[PADR_(int)];
	char to_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t to; char to_r_[PADR_(abi_uintptr_t)];
	char tolen_l_[PADL_(__socklen_t)]; __socklen_t tolen; char tolen_r_[PADR_(__socklen_t)];
};
struct target_shutdown_args {
	char s_l_[PADL_(int)]; int s; char s_r_[PADR_(int)];
	char how_l_[PADL_(int)]; int how; char how_r_[PADR_(int)];
};
struct target_socketpair_args {
	char domain_l_[PADL_(int)]; int domain; char domain_r_[PADR_(int)];
	char type_l_[PADL_(int)]; int type; char type_r_[PADR_(int)];
	char protocol_l_[PADL_(int)]; int protocol; char protocol_r_[PADR_(int)];
	char rsv_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t rsv; char rsv_r_[PADR_(abi_uintptr_t)];
};
struct target_mkdir_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char mode_l_[PADL_(mode_t)]; mode_t mode; char mode_r_[PADR_(mode_t)];
};
struct target_rmdir_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
};
struct target_utimes_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char tptr_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t tptr; char tptr_r_[PADR_(abi_uintptr_t)];
};
struct target_adjtime_args {
	char delta_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t delta; char delta_r_[PADR_(abi_uintptr_t)];
	char olddelta_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t olddelta; char olddelta_r_[PADR_(abi_uintptr_t)];
};
struct target_setsid_args {
	register_t dummy;
};
struct target_quotactl_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char cmd_l_[PADL_(int)]; int cmd; char cmd_r_[PADR_(int)];
	char uid_l_[PADL_(int)]; int uid; char uid_r_[PADR_(int)];
	char arg_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t arg; char arg_r_[PADR_(abi_uintptr_t)];
};
struct target_nlm_syscall_args {
	char debug_level_l_[PADL_(int)]; int debug_level; char debug_level_r_[PADR_(int)];
	char grace_period_l_[PADL_(int)]; int grace_period; char grace_period_r_[PADR_(int)];
	char addr_count_l_[PADL_(int)]; int addr_count; char addr_count_r_[PADR_(int)];
	char addrs_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t addrs; char addrs_r_[PADR_(abi_uintptr_t)];
};
struct target_nfssvc_args {
	char flag_l_[PADL_(int)]; int flag; char flag_r_[PADR_(int)];
	char argp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t argp; char argp_r_[PADR_(abi_uintptr_t)];
};
struct target_lgetfh_args {
	char fname_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t fname; char fname_r_[PADR_(abi_uintptr_t)];
	char fhp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t fhp; char fhp_r_[PADR_(abi_uintptr_t)];
};
struct target_getfh_args {
	char fname_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t fname; char fname_r_[PADR_(abi_uintptr_t)];
	char fhp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t fhp; char fhp_r_[PADR_(abi_uintptr_t)];
};
struct target_sysarch_args {
	char op_l_[PADL_(int)]; int op; char op_r_[PADR_(int)];
	char parms_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t parms; char parms_r_[PADR_(abi_uintptr_t)];
};
struct target_rtprio_args {
	char function_l_[PADL_(int)]; int function; char function_r_[PADR_(int)];
	char pid_l_[PADL_(pid_t)]; pid_t pid; char pid_r_[PADR_(pid_t)];
	char rtp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t rtp; char rtp_r_[PADR_(abi_uintptr_t)];
};
struct target_semsys_args {
	char which_l_[PADL_(int)]; int which; char which_r_[PADR_(int)];
	char a2_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t a2; char a2_r_[PADR_(abi_uintptr_t)];
	char a3_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t a3; char a3_r_[PADR_(abi_uintptr_t)];
	char a4_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t a4; char a4_r_[PADR_(abi_uintptr_t)];
	char a5_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t a5; char a5_r_[PADR_(abi_uintptr_t)];
};
struct target_msgsys_args {
	char which_l_[PADL_(int)]; int which; char which_r_[PADR_(int)];
	char a2_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t a2; char a2_r_[PADR_(abi_uintptr_t)];
	char a3_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t a3; char a3_r_[PADR_(abi_uintptr_t)];
	char a4_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t a4; char a4_r_[PADR_(abi_uintptr_t)];
	char a5_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t a5; char a5_r_[PADR_(abi_uintptr_t)];
	char a6_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t a6; char a6_r_[PADR_(abi_uintptr_t)];
};
struct target_shmsys_args {
	char which_l_[PADL_(int)]; int which; char which_r_[PADR_(int)];
	char a2_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t a2; char a2_r_[PADR_(abi_uintptr_t)];
	char a3_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t a3; char a3_r_[PADR_(abi_uintptr_t)];
	char a4_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t a4; char a4_r_[PADR_(abi_uintptr_t)];
};
struct target_setfib_args {
	char fibnum_l_[PADL_(int)]; int fibnum; char fibnum_r_[PADR_(int)];
};
struct target_ntp_adjtime_args {
	char tp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t tp; char tp_r_[PADR_(abi_uintptr_t)];
};
struct target_setgid_args {
	char gid_l_[PADL_(gid_t)]; gid_t gid; char gid_r_[PADR_(gid_t)];
};
struct target_setegid_args {
	char egid_l_[PADL_(gid_t)]; gid_t egid; char egid_r_[PADR_(gid_t)];
};
struct target_seteuid_args {
	char euid_l_[PADL_(uid_t)]; uid_t euid; char euid_r_[PADR_(uid_t)];
};
struct target_pathconf_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char name_l_[PADL_(int)]; int name; char name_r_[PADR_(int)];
};
struct target_fpathconf_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char name_l_[PADL_(int)]; int name; char name_r_[PADR_(int)];
};
struct target___getrlimit_args {
	char which_l_[PADL_(u_int)]; u_int which; char which_r_[PADR_(u_int)];
	char rlp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t rlp; char rlp_r_[PADR_(abi_uintptr_t)];
};
struct target___setrlimit_args {
	char which_l_[PADL_(u_int)]; u_int which; char which_r_[PADR_(u_int)];
	char rlp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t rlp; char rlp_r_[PADR_(abi_uintptr_t)];
};
struct target___sysctl_args {
	char name_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t name; char name_r_[PADR_(abi_uintptr_t)];
	char namelen_l_[PADL_(u_int)]; u_int namelen; char namelen_r_[PADR_(u_int)];
	char old_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t old; char old_r_[PADR_(abi_uintptr_t)];
	char oldlenp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t oldlenp; char oldlenp_r_[PADR_(abi_uintptr_t)];
	char new_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t new; char new_r_[PADR_(abi_uintptr_t)];
	char newlen_l_[PADL_(size_t)]; size_t newlen; char newlen_r_[PADR_(size_t)];
};
struct target_mlock_args {
	char addr_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t addr; char addr_r_[PADR_(abi_uintptr_t)];
	char len_l_[PADL_(size_t)]; size_t len; char len_r_[PADR_(size_t)];
};
struct target_munlock_args {
	char addr_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t addr; char addr_r_[PADR_(abi_uintptr_t)];
	char len_l_[PADL_(size_t)]; size_t len; char len_r_[PADR_(size_t)];
};
struct target_undelete_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
};
struct target_futimes_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char tptr_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t tptr; char tptr_r_[PADR_(abi_uintptr_t)];
};
struct target_getpgid_args {
	char pid_l_[PADL_(pid_t)]; pid_t pid; char pid_r_[PADR_(pid_t)];
};
struct target_poll_args {
	char fds_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t fds; char fds_r_[PADR_(abi_uintptr_t)];
	char nfds_l_[PADL_(u_int)]; u_int nfds; char nfds_r_[PADR_(u_int)];
	char timeout_l_[PADL_(int)]; int timeout; char timeout_r_[PADR_(int)];
};
struct target_semget_args {
	char key_l_[PADL_(key_t)]; key_t key; char key_r_[PADR_(key_t)];
	char nsems_l_[PADL_(int)]; int nsems; char nsems_r_[PADR_(int)];
	char semflg_l_[PADL_(int)]; int semflg; char semflg_r_[PADR_(int)];
};
struct target_semop_args {
	char semid_l_[PADL_(int)]; int semid; char semid_r_[PADR_(int)];
	char sops_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t sops; char sops_r_[PADR_(abi_uintptr_t)];
	char nsops_l_[PADL_(size_t)]; size_t nsops; char nsops_r_[PADR_(size_t)];
};
struct target_msgget_args {
	char key_l_[PADL_(key_t)]; key_t key; char key_r_[PADR_(key_t)];
	char msgflg_l_[PADL_(int)]; int msgflg; char msgflg_r_[PADR_(int)];
};
struct target_msgsnd_args {
	char msqid_l_[PADL_(int)]; int msqid; char msqid_r_[PADR_(int)];
	char msgp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t msgp; char msgp_r_[PADR_(abi_uintptr_t)];
	char msgsz_l_[PADL_(size_t)]; size_t msgsz; char msgsz_r_[PADR_(size_t)];
	char msgflg_l_[PADL_(int)]; int msgflg; char msgflg_r_[PADR_(int)];
};
struct target_msgrcv_args {
	char msqid_l_[PADL_(int)]; int msqid; char msqid_r_[PADR_(int)];
	char msgp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t msgp; char msgp_r_[PADR_(abi_uintptr_t)];
	char msgsz_l_[PADL_(size_t)]; size_t msgsz; char msgsz_r_[PADR_(size_t)];
	char msgtyp_l_[PADL_(long)]; long msgtyp; char msgtyp_r_[PADR_(long)];
	char msgflg_l_[PADL_(int)]; int msgflg; char msgflg_r_[PADR_(int)];
};
struct target_shmat_args {
	char shmid_l_[PADL_(int)]; int shmid; char shmid_r_[PADR_(int)];
	char shmaddr_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t shmaddr; char shmaddr_r_[PADR_(abi_uintptr_t)];
	char shmflg_l_[PADL_(int)]; int shmflg; char shmflg_r_[PADR_(int)];
};
struct target_shmdt_args {
	char shmaddr_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t shmaddr; char shmaddr_r_[PADR_(abi_uintptr_t)];
};
struct target_shmget_args {
	char key_l_[PADL_(key_t)]; key_t key; char key_r_[PADR_(key_t)];
	char size_l_[PADL_(size_t)]; size_t size; char size_r_[PADR_(size_t)];
	char shmflg_l_[PADL_(int)]; int shmflg; char shmflg_r_[PADR_(int)];
};
struct target_clock_gettime_args {
	char clock_id_l_[PADL_(clockid_t)]; clockid_t clock_id; char clock_id_r_[PADR_(clockid_t)];
	char tp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t tp; char tp_r_[PADR_(abi_uintptr_t)];
};
struct target_clock_settime_args {
	char clock_id_l_[PADL_(clockid_t)]; clockid_t clock_id; char clock_id_r_[PADR_(clockid_t)];
	char tp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t tp; char tp_r_[PADR_(abi_uintptr_t)];
};
struct target_clock_getres_args {
	char clock_id_l_[PADL_(clockid_t)]; clockid_t clock_id; char clock_id_r_[PADR_(clockid_t)];
	char tp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t tp; char tp_r_[PADR_(abi_uintptr_t)];
};
struct target_ktimer_create_args {
	char clock_id_l_[PADL_(clockid_t)]; clockid_t clock_id; char clock_id_r_[PADR_(clockid_t)];
	char evp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t evp; char evp_r_[PADR_(abi_uintptr_t)];
	char timerid_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t timerid; char timerid_r_[PADR_(abi_uintptr_t)];
};
struct target_ktimer_delete_args {
	char timerid_l_[PADL_(int)]; int timerid; char timerid_r_[PADR_(int)];
};
struct target_ktimer_settime_args {
	char timerid_l_[PADL_(int)]; int timerid; char timerid_r_[PADR_(int)];
	char flags_l_[PADL_(int)]; int flags; char flags_r_[PADR_(int)];
	char value_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t value; char value_r_[PADR_(abi_uintptr_t)];
	char ovalue_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t ovalue; char ovalue_r_[PADR_(abi_uintptr_t)];
};
struct target_ktimer_gettime_args {
	char timerid_l_[PADL_(int)]; int timerid; char timerid_r_[PADR_(int)];
	char value_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t value; char value_r_[PADR_(abi_uintptr_t)];
};
struct target_ktimer_getoverrun_args {
	char timerid_l_[PADL_(int)]; int timerid; char timerid_r_[PADR_(int)];
};
struct target_nanosleep_args {
	char rqtp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t rqtp; char rqtp_r_[PADR_(abi_uintptr_t)];
	char rmtp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t rmtp; char rmtp_r_[PADR_(abi_uintptr_t)];
};
struct target_ffclock_getcounter_args {
	char ffcount_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t ffcount; char ffcount_r_[PADR_(abi_uintptr_t)];
};
struct target_ffclock_setestimate_args {
	char cest_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t cest; char cest_r_[PADR_(abi_uintptr_t)];
};
struct target_ffclock_getestimate_args {
	char cest_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t cest; char cest_r_[PADR_(abi_uintptr_t)];
};
struct target_clock_nanosleep_args {
	char clock_id_l_[PADL_(clockid_t)]; clockid_t clock_id; char clock_id_r_[PADR_(clockid_t)];
	char flags_l_[PADL_(int)]; int flags; char flags_r_[PADR_(int)];
	char rqtp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t rqtp; char rqtp_r_[PADR_(abi_uintptr_t)];
	char rmtp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t rmtp; char rmtp_r_[PADR_(abi_uintptr_t)];
};
struct target_clock_getcpuclockid2_args {
	char id_l_[PADL_(id_t)]; id_t id; char id_r_[PADR_(id_t)];
	char which_l_[PADL_(int)]; int which; char which_r_[PADR_(int)];
	char clock_id_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t clock_id; char clock_id_r_[PADR_(abi_uintptr_t)];
};
struct target_ntp_gettime_args {
	char ntvp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t ntvp; char ntvp_r_[PADR_(abi_uintptr_t)];
};
struct target_minherit_args {
	char addr_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t addr; char addr_r_[PADR_(abi_uintptr_t)];
	char len_l_[PADL_(size_t)]; size_t len; char len_r_[PADR_(size_t)];
	char inherit_l_[PADL_(int)]; int inherit; char inherit_r_[PADR_(int)];
};
struct target_rfork_args {
	char flags_l_[PADL_(int)]; int flags; char flags_r_[PADR_(int)];
};
struct target_issetugid_args {
	register_t dummy;
};
struct target_lchown_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char uid_l_[PADL_(int)]; int uid; char uid_r_[PADR_(int)];
	char gid_l_[PADL_(int)]; int gid; char gid_r_[PADR_(int)];
};
struct target_aio_read_args {
	char aiocbp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t aiocbp; char aiocbp_r_[PADR_(abi_uintptr_t)];
};
struct target_aio_write_args {
	char aiocbp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t aiocbp; char aiocbp_r_[PADR_(abi_uintptr_t)];
};
struct target_lio_listio_args {
	char mode_l_[PADL_(int)]; int mode; char mode_r_[PADR_(int)];
	char acb_list_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t acb_list; char acb_list_r_[PADR_(abi_uintptr_t)];
	char nent_l_[PADL_(int)]; int nent; char nent_r_[PADR_(int)];
	char sig_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t sig; char sig_r_[PADR_(abi_uintptr_t)];
};
struct target_kbounce_args {
	char src_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t src; char src_r_[PADR_(abi_uintptr_t)];
	char dst_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t dst; char dst_r_[PADR_(abi_uintptr_t)];
	char len_l_[PADL_(size_t)]; size_t len; char len_r_[PADR_(size_t)];
	char flags_l_[PADL_(int)]; int flags; char flags_r_[PADR_(int)];
};
struct target_flag_captured_args {
	char message_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t message; char message_r_[PADR_(abi_uintptr_t)];
	char key_l_[PADL_(uint32_t)]; uint32_t key; char key_r_[PADR_(uint32_t)];
};
struct target_lchmod_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char mode_l_[PADL_(mode_t)]; mode_t mode; char mode_r_[PADR_(mode_t)];
};
struct target_lutimes_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char tptr_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t tptr; char tptr_r_[PADR_(abi_uintptr_t)];
};
struct target_preadv_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char iovp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t iovp; char iovp_r_[PADR_(abi_uintptr_t)];
	char iovcnt_l_[PADL_(u_int)]; u_int iovcnt; char iovcnt_r_[PADR_(u_int)];
	char offset_l_[PADL_(off_t)]; off_t offset; char offset_r_[PADR_(off_t)];
};
struct target_pwritev_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char iovp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t iovp; char iovp_r_[PADR_(abi_uintptr_t)];
	char iovcnt_l_[PADL_(u_int)]; u_int iovcnt; char iovcnt_r_[PADR_(u_int)];
	char offset_l_[PADL_(off_t)]; off_t offset; char offset_r_[PADR_(off_t)];
};
struct target_fhopen_args {
	char u_fhp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t u_fhp; char u_fhp_r_[PADR_(abi_uintptr_t)];
	char flags_l_[PADL_(int)]; int flags; char flags_r_[PADR_(int)];
};
struct target_modnext_args {
	char modid_l_[PADL_(int)]; int modid; char modid_r_[PADR_(int)];
};
struct target_modstat_args {
	char modid_l_[PADL_(int)]; int modid; char modid_r_[PADR_(int)];
	char stat_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t stat; char stat_r_[PADR_(abi_uintptr_t)];
};
struct target_modfnext_args {
	char modid_l_[PADL_(int)]; int modid; char modid_r_[PADR_(int)];
};
struct target_modfind_args {
	char name_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t name; char name_r_[PADR_(abi_uintptr_t)];
};
struct target_kldload_args {
	char file_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t file; char file_r_[PADR_(abi_uintptr_t)];
};
struct target_kldunload_args {
	char fileid_l_[PADL_(int)]; int fileid; char fileid_r_[PADR_(int)];
};
struct target_kldfind_args {
	char file_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t file; char file_r_[PADR_(abi_uintptr_t)];
};
struct target_kldnext_args {
	char fileid_l_[PADL_(int)]; int fileid; char fileid_r_[PADR_(int)];
};
struct target_kldstat_args {
	char fileid_l_[PADL_(int)]; int fileid; char fileid_r_[PADR_(int)];
	char stat_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t stat; char stat_r_[PADR_(abi_uintptr_t)];
};
struct target_kldfirstmod_args {
	char fileid_l_[PADL_(int)]; int fileid; char fileid_r_[PADR_(int)];
};
struct target_getsid_args {
	char pid_l_[PADL_(pid_t)]; pid_t pid; char pid_r_[PADR_(pid_t)];
};
struct target_setresuid_args {
	char ruid_l_[PADL_(uid_t)]; uid_t ruid; char ruid_r_[PADR_(uid_t)];
	char euid_l_[PADL_(uid_t)]; uid_t euid; char euid_r_[PADR_(uid_t)];
	char suid_l_[PADL_(uid_t)]; uid_t suid; char suid_r_[PADR_(uid_t)];
};
struct target_setresgid_args {
	char rgid_l_[PADL_(gid_t)]; gid_t rgid; char rgid_r_[PADR_(gid_t)];
	char egid_l_[PADL_(gid_t)]; gid_t egid; char egid_r_[PADR_(gid_t)];
	char sgid_l_[PADL_(gid_t)]; gid_t sgid; char sgid_r_[PADR_(gid_t)];
};
struct target_aio_return_args {
	char aiocbp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t aiocbp; char aiocbp_r_[PADR_(abi_uintptr_t)];
};
struct target_aio_suspend_args {
	char aiocbp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t aiocbp; char aiocbp_r_[PADR_(abi_uintptr_t)];
	char nent_l_[PADL_(int)]; int nent; char nent_r_[PADR_(int)];
	char timeout_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t timeout; char timeout_r_[PADR_(abi_uintptr_t)];
};
struct target_aio_cancel_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char aiocbp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t aiocbp; char aiocbp_r_[PADR_(abi_uintptr_t)];
};
struct target_aio_error_args {
	char aiocbp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t aiocbp; char aiocbp_r_[PADR_(abi_uintptr_t)];
};
struct target_yield_args {
	register_t dummy;
};
struct target_mlockall_args {
	char how_l_[PADL_(int)]; int how; char how_r_[PADR_(int)];
};
struct target_munlockall_args {
	register_t dummy;
};
struct target___getcwd_args {
	char buf_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t buf; char buf_r_[PADR_(abi_uintptr_t)];
	char buflen_l_[PADL_(size_t)]; size_t buflen; char buflen_r_[PADR_(size_t)];
};
struct target_sched_setparam_args {
	char pid_l_[PADL_(pid_t)]; pid_t pid; char pid_r_[PADR_(pid_t)];
	char param_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t param; char param_r_[PADR_(abi_uintptr_t)];
};
struct target_sched_getparam_args {
	char pid_l_[PADL_(pid_t)]; pid_t pid; char pid_r_[PADR_(pid_t)];
	char param_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t param; char param_r_[PADR_(abi_uintptr_t)];
};
struct target_sched_setscheduler_args {
	char pid_l_[PADL_(pid_t)]; pid_t pid; char pid_r_[PADR_(pid_t)];
	char policy_l_[PADL_(int)]; int policy; char policy_r_[PADR_(int)];
	char param_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t param; char param_r_[PADR_(abi_uintptr_t)];
};
struct target_sched_getscheduler_args {
	char pid_l_[PADL_(pid_t)]; pid_t pid; char pid_r_[PADR_(pid_t)];
};
struct target_sched_yield_args {
	register_t dummy;
};
struct target_sched_get_priority_max_args {
	char policy_l_[PADL_(int)]; int policy; char policy_r_[PADR_(int)];
};
struct target_sched_get_priority_min_args {
	char policy_l_[PADL_(int)]; int policy; char policy_r_[PADR_(int)];
};
struct target_sched_rr_get_interval_args {
	char pid_l_[PADL_(pid_t)]; pid_t pid; char pid_r_[PADR_(pid_t)];
	char interval_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t interval; char interval_r_[PADR_(abi_uintptr_t)];
};
struct target_utrace_args {
	char addr_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t addr; char addr_r_[PADR_(abi_uintptr_t)];
	char len_l_[PADL_(size_t)]; size_t len; char len_r_[PADR_(size_t)];
};
struct target_kldsym_args {
	char fileid_l_[PADL_(int)]; int fileid; char fileid_r_[PADR_(int)];
	char cmd_l_[PADL_(int)]; int cmd; char cmd_r_[PADR_(int)];
	char data_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t data; char data_r_[PADR_(abi_uintptr_t)];
};
struct target_jail_args {
	char jailp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t jailp; char jailp_r_[PADR_(abi_uintptr_t)];
};
struct target_nnpfs_syscall_args {
	char operation_l_[PADL_(int)]; int operation; char operation_r_[PADR_(int)];
	char a_pathP_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t a_pathP; char a_pathP_r_[PADR_(abi_uintptr_t)];
	char a_opcode_l_[PADL_(int)]; int a_opcode; char a_opcode_r_[PADR_(int)];
	char a_paramsP_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t a_paramsP; char a_paramsP_r_[PADR_(abi_uintptr_t)];
	char a_followSymlinks_l_[PADL_(int)]; int a_followSymlinks; char a_followSymlinks_r_[PADR_(int)];
};
struct target_sigprocmask_args {
	char how_l_[PADL_(int)]; int how; char how_r_[PADR_(int)];
	char set_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t set; char set_r_[PADR_(abi_uintptr_t)];
	char oset_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t oset; char oset_r_[PADR_(abi_uintptr_t)];
};
struct target_sigsuspend_args {
	char sigmask_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t sigmask; char sigmask_r_[PADR_(abi_uintptr_t)];
};
struct target_sigpending_args {
	char set_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t set; char set_r_[PADR_(abi_uintptr_t)];
};
struct target_sigtimedwait_args {
	char set_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t set; char set_r_[PADR_(abi_uintptr_t)];
	char info_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t info; char info_r_[PADR_(abi_uintptr_t)];
	char timeout_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t timeout; char timeout_r_[PADR_(abi_uintptr_t)];
};
struct target_sigwaitinfo_args {
	char set_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t set; char set_r_[PADR_(abi_uintptr_t)];
	char info_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t info; char info_r_[PADR_(abi_uintptr_t)];
};
struct target___acl_get_file_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char type_l_[PADL_(acl_type_t)]; acl_type_t type; char type_r_[PADR_(acl_type_t)];
	char aclp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t aclp; char aclp_r_[PADR_(abi_uintptr_t)];
};
struct target___acl_set_file_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char type_l_[PADL_(acl_type_t)]; acl_type_t type; char type_r_[PADR_(acl_type_t)];
	char aclp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t aclp; char aclp_r_[PADR_(abi_uintptr_t)];
};
struct target___acl_get_fd_args {
	char filedes_l_[PADL_(int)]; int filedes; char filedes_r_[PADR_(int)];
	char type_l_[PADL_(acl_type_t)]; acl_type_t type; char type_r_[PADR_(acl_type_t)];
	char aclp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t aclp; char aclp_r_[PADR_(abi_uintptr_t)];
};
struct target___acl_set_fd_args {
	char filedes_l_[PADL_(int)]; int filedes; char filedes_r_[PADR_(int)];
	char type_l_[PADL_(acl_type_t)]; acl_type_t type; char type_r_[PADR_(acl_type_t)];
	char aclp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t aclp; char aclp_r_[PADR_(abi_uintptr_t)];
};
struct target___acl_delete_file_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char type_l_[PADL_(acl_type_t)]; acl_type_t type; char type_r_[PADR_(acl_type_t)];
};
struct target___acl_delete_fd_args {
	char filedes_l_[PADL_(int)]; int filedes; char filedes_r_[PADR_(int)];
	char type_l_[PADL_(acl_type_t)]; acl_type_t type; char type_r_[PADR_(acl_type_t)];
};
struct target___acl_aclcheck_file_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char type_l_[PADL_(acl_type_t)]; acl_type_t type; char type_r_[PADR_(acl_type_t)];
	char aclp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t aclp; char aclp_r_[PADR_(abi_uintptr_t)];
};
struct target___acl_aclcheck_fd_args {
	char filedes_l_[PADL_(int)]; int filedes; char filedes_r_[PADR_(int)];
	char type_l_[PADL_(acl_type_t)]; acl_type_t type; char type_r_[PADR_(acl_type_t)];
	char aclp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t aclp; char aclp_r_[PADR_(abi_uintptr_t)];
};
struct target_extattrctl_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char cmd_l_[PADL_(int)]; int cmd; char cmd_r_[PADR_(int)];
	char filename_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t filename; char filename_r_[PADR_(abi_uintptr_t)];
	char attrnamespace_l_[PADL_(int)]; int attrnamespace; char attrnamespace_r_[PADR_(int)];
	char attrname_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t attrname; char attrname_r_[PADR_(abi_uintptr_t)];
};
struct target_extattr_set_file_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char attrnamespace_l_[PADL_(int)]; int attrnamespace; char attrnamespace_r_[PADR_(int)];
	char attrname_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t attrname; char attrname_r_[PADR_(abi_uintptr_t)];
	char data_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t data; char data_r_[PADR_(abi_uintptr_t)];
	char nbytes_l_[PADL_(size_t)]; size_t nbytes; char nbytes_r_[PADR_(size_t)];
};
struct target_extattr_get_file_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char attrnamespace_l_[PADL_(int)]; int attrnamespace; char attrnamespace_r_[PADR_(int)];
	char attrname_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t attrname; char attrname_r_[PADR_(abi_uintptr_t)];
	char data_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t data; char data_r_[PADR_(abi_uintptr_t)];
	char nbytes_l_[PADL_(size_t)]; size_t nbytes; char nbytes_r_[PADR_(size_t)];
};
struct target_extattr_delete_file_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char attrnamespace_l_[PADL_(int)]; int attrnamespace; char attrnamespace_r_[PADR_(int)];
	char attrname_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t attrname; char attrname_r_[PADR_(abi_uintptr_t)];
};
struct target_aio_waitcomplete_args {
	char aiocbp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t aiocbp; char aiocbp_r_[PADR_(abi_uintptr_t)];
	char timeout_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t timeout; char timeout_r_[PADR_(abi_uintptr_t)];
};
struct target_getresuid_args {
	char ruid_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t ruid; char ruid_r_[PADR_(abi_uintptr_t)];
	char euid_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t euid; char euid_r_[PADR_(abi_uintptr_t)];
	char suid_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t suid; char suid_r_[PADR_(abi_uintptr_t)];
};
struct target_getresgid_args {
	char rgid_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t rgid; char rgid_r_[PADR_(abi_uintptr_t)];
	char egid_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t egid; char egid_r_[PADR_(abi_uintptr_t)];
	char sgid_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t sgid; char sgid_r_[PADR_(abi_uintptr_t)];
};
struct target_kqueue_args {
	register_t dummy;
};
struct target_extattr_set_fd_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char attrnamespace_l_[PADL_(int)]; int attrnamespace; char attrnamespace_r_[PADR_(int)];
	char attrname_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t attrname; char attrname_r_[PADR_(abi_uintptr_t)];
	char data_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t data; char data_r_[PADR_(abi_uintptr_t)];
	char nbytes_l_[PADL_(size_t)]; size_t nbytes; char nbytes_r_[PADR_(size_t)];
};
struct target_extattr_get_fd_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char attrnamespace_l_[PADL_(int)]; int attrnamespace; char attrnamespace_r_[PADR_(int)];
	char attrname_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t attrname; char attrname_r_[PADR_(abi_uintptr_t)];
	char data_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t data; char data_r_[PADR_(abi_uintptr_t)];
	char nbytes_l_[PADL_(size_t)]; size_t nbytes; char nbytes_r_[PADR_(size_t)];
};
struct target_extattr_delete_fd_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char attrnamespace_l_[PADL_(int)]; int attrnamespace; char attrnamespace_r_[PADR_(int)];
	char attrname_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t attrname; char attrname_r_[PADR_(abi_uintptr_t)];
};
struct target___setugid_args {
	char flag_l_[PADL_(int)]; int flag; char flag_r_[PADR_(int)];
};
struct target_eaccess_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char amode_l_[PADL_(int)]; int amode; char amode_r_[PADR_(int)];
};
struct target_afs3_syscall_args {
	char syscall_l_[PADL_(long)]; long syscall; char syscall_r_[PADR_(long)];
	char parm1_l_[PADL_(long)]; long parm1; char parm1_r_[PADR_(long)];
	char parm2_l_[PADL_(long)]; long parm2; char parm2_r_[PADR_(long)];
	char parm3_l_[PADL_(long)]; long parm3; char parm3_r_[PADR_(long)];
	char parm4_l_[PADL_(long)]; long parm4; char parm4_r_[PADR_(long)];
	char parm5_l_[PADL_(long)]; long parm5; char parm5_r_[PADR_(long)];
	char parm6_l_[PADL_(long)]; long parm6; char parm6_r_[PADR_(long)];
};
struct target_nmount_args {
	char iovp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t iovp; char iovp_r_[PADR_(abi_uintptr_t)];
	char iovcnt_l_[PADL_(unsigned int)]; unsigned int iovcnt; char iovcnt_r_[PADR_(unsigned int)];
	char flags_l_[PADL_(int)]; int flags; char flags_r_[PADR_(int)];
};
struct target___mac_get_proc_args {
	char mac_p_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t mac_p; char mac_p_r_[PADR_(abi_uintptr_t)];
};
struct target___mac_set_proc_args {
	char mac_p_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t mac_p; char mac_p_r_[PADR_(abi_uintptr_t)];
};
struct target___mac_get_fd_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char mac_p_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t mac_p; char mac_p_r_[PADR_(abi_uintptr_t)];
};
struct target___mac_get_file_args {
	char path_p_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path_p; char path_p_r_[PADR_(abi_uintptr_t)];
	char mac_p_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t mac_p; char mac_p_r_[PADR_(abi_uintptr_t)];
};
struct target___mac_set_fd_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char mac_p_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t mac_p; char mac_p_r_[PADR_(abi_uintptr_t)];
};
struct target___mac_set_file_args {
	char path_p_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path_p; char path_p_r_[PADR_(abi_uintptr_t)];
	char mac_p_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t mac_p; char mac_p_r_[PADR_(abi_uintptr_t)];
};
struct target_kenv_args {
	char what_l_[PADL_(int)]; int what; char what_r_[PADR_(int)];
	char name_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t name; char name_r_[PADR_(abi_uintptr_t)];
	char value_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t value; char value_r_[PADR_(abi_uintptr_t)];
	char len_l_[PADL_(int)]; int len; char len_r_[PADR_(int)];
};
struct target_lchflags_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char flags_l_[PADL_(u_long)]; u_long flags; char flags_r_[PADR_(u_long)];
};
struct target_uuidgen_args {
	char store_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t store; char store_r_[PADR_(abi_uintptr_t)];
	char count_l_[PADL_(int)]; int count; char count_r_[PADR_(int)];
};
struct target_sendfile_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char s_l_[PADL_(int)]; int s; char s_r_[PADR_(int)];
	char offset_l_[PADL_(off_t)]; off_t offset; char offset_r_[PADR_(off_t)];
	char nbytes_l_[PADL_(size_t)]; size_t nbytes; char nbytes_r_[PADR_(size_t)];
	char hdtr_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t hdtr; char hdtr_r_[PADR_(abi_uintptr_t)];
	char sbytes_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t sbytes; char sbytes_r_[PADR_(abi_uintptr_t)];
	char flags_l_[PADL_(int)]; int flags; char flags_r_[PADR_(int)];
};
struct target_mac_syscall_args {
	char policy_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t policy; char policy_r_[PADR_(abi_uintptr_t)];
	char call_l_[PADL_(int)]; int call; char call_r_[PADR_(int)];
	char arg_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t arg; char arg_r_[PADR_(abi_uintptr_t)];
};
struct target_ksem_close_args {
	char id_l_[PADL_(semid_t)]; semid_t id; char id_r_[PADR_(semid_t)];
};
struct target_ksem_post_args {
	char id_l_[PADL_(semid_t)]; semid_t id; char id_r_[PADR_(semid_t)];
};
struct target_ksem_wait_args {
	char id_l_[PADL_(semid_t)]; semid_t id; char id_r_[PADR_(semid_t)];
};
struct target_ksem_trywait_args {
	char id_l_[PADL_(semid_t)]; semid_t id; char id_r_[PADR_(semid_t)];
};
struct target_ksem_init_args {
	char idp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t idp; char idp_r_[PADR_(abi_uintptr_t)];
	char value_l_[PADL_(unsigned int)]; unsigned int value; char value_r_[PADR_(unsigned int)];
};
struct target_ksem_open_args {
	char idp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t idp; char idp_r_[PADR_(abi_uintptr_t)];
	char name_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t name; char name_r_[PADR_(abi_uintptr_t)];
	char oflag_l_[PADL_(int)]; int oflag; char oflag_r_[PADR_(int)];
	char mode_l_[PADL_(mode_t)]; mode_t mode; char mode_r_[PADR_(mode_t)];
	char value_l_[PADL_(unsigned int)]; unsigned int value; char value_r_[PADR_(unsigned int)];
};
struct target_ksem_unlink_args {
	char name_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t name; char name_r_[PADR_(abi_uintptr_t)];
};
struct target_ksem_getvalue_args {
	char id_l_[PADL_(semid_t)]; semid_t id; char id_r_[PADR_(semid_t)];
	char val_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t val; char val_r_[PADR_(abi_uintptr_t)];
};
struct target_ksem_destroy_args {
	char id_l_[PADL_(semid_t)]; semid_t id; char id_r_[PADR_(semid_t)];
};
struct target___mac_get_pid_args {
	char pid_l_[PADL_(pid_t)]; pid_t pid; char pid_r_[PADR_(pid_t)];
	char mac_p_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t mac_p; char mac_p_r_[PADR_(abi_uintptr_t)];
};
struct target___mac_get_link_args {
	char path_p_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path_p; char path_p_r_[PADR_(abi_uintptr_t)];
	char mac_p_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t mac_p; char mac_p_r_[PADR_(abi_uintptr_t)];
};
struct target___mac_set_link_args {
	char path_p_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path_p; char path_p_r_[PADR_(abi_uintptr_t)];
	char mac_p_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t mac_p; char mac_p_r_[PADR_(abi_uintptr_t)];
};
struct target_extattr_set_link_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char attrnamespace_l_[PADL_(int)]; int attrnamespace; char attrnamespace_r_[PADR_(int)];
	char attrname_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t attrname; char attrname_r_[PADR_(abi_uintptr_t)];
	char data_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t data; char data_r_[PADR_(abi_uintptr_t)];
	char nbytes_l_[PADL_(size_t)]; size_t nbytes; char nbytes_r_[PADR_(size_t)];
};
struct target_extattr_get_link_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char attrnamespace_l_[PADL_(int)]; int attrnamespace; char attrnamespace_r_[PADR_(int)];
	char attrname_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t attrname; char attrname_r_[PADR_(abi_uintptr_t)];
	char data_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t data; char data_r_[PADR_(abi_uintptr_t)];
	char nbytes_l_[PADL_(size_t)]; size_t nbytes; char nbytes_r_[PADR_(size_t)];
};
struct target_extattr_delete_link_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char attrnamespace_l_[PADL_(int)]; int attrnamespace; char attrnamespace_r_[PADR_(int)];
	char attrname_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t attrname; char attrname_r_[PADR_(abi_uintptr_t)];
};
struct target___mac_execve_args {
	char fname_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t fname; char fname_r_[PADR_(abi_uintptr_t)];
	char argv_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t argv; char argv_r_[PADR_(abi_uintptr_t)];
	char envv_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t envv; char envv_r_[PADR_(abi_uintptr_t)];
	char mac_p_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t mac_p; char mac_p_r_[PADR_(abi_uintptr_t)];
};
struct target_sigaction_args {
	char sig_l_[PADL_(int)]; int sig; char sig_r_[PADR_(int)];
	char act_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t act; char act_r_[PADR_(abi_uintptr_t)];
	char oact_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t oact; char oact_r_[PADR_(abi_uintptr_t)];
};
struct target_sigreturn_args {
	char sigcntxp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t sigcntxp; char sigcntxp_r_[PADR_(abi_uintptr_t)];
};
struct target_getcontext_args {
	char ucp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t ucp; char ucp_r_[PADR_(abi_uintptr_t)];
};
struct target_setcontext_args {
	char ucp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t ucp; char ucp_r_[PADR_(abi_uintptr_t)];
};
struct target_swapcontext_args {
	char oucp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t oucp; char oucp_r_[PADR_(abi_uintptr_t)];
	char ucp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t ucp; char ucp_r_[PADR_(abi_uintptr_t)];
};
struct target_swapoff_args {
	char name_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t name; char name_r_[PADR_(abi_uintptr_t)];
};
struct target___acl_get_link_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char type_l_[PADL_(acl_type_t)]; acl_type_t type; char type_r_[PADR_(acl_type_t)];
	char aclp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t aclp; char aclp_r_[PADR_(abi_uintptr_t)];
};
struct target___acl_set_link_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char type_l_[PADL_(acl_type_t)]; acl_type_t type; char type_r_[PADR_(acl_type_t)];
	char aclp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t aclp; char aclp_r_[PADR_(abi_uintptr_t)];
};
struct target___acl_delete_link_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char type_l_[PADL_(acl_type_t)]; acl_type_t type; char type_r_[PADR_(acl_type_t)];
};
struct target___acl_aclcheck_link_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char type_l_[PADL_(acl_type_t)]; acl_type_t type; char type_r_[PADR_(acl_type_t)];
	char aclp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t aclp; char aclp_r_[PADR_(abi_uintptr_t)];
};
struct target_sigwait_args {
	char set_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t set; char set_r_[PADR_(abi_uintptr_t)];
	char sig_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t sig; char sig_r_[PADR_(abi_uintptr_t)];
};
struct target_thr_create_args {
	char ctx_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t ctx; char ctx_r_[PADR_(abi_uintptr_t)];
	char id_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t id; char id_r_[PADR_(abi_uintptr_t)];
	char flags_l_[PADL_(int)]; int flags; char flags_r_[PADR_(int)];
};
struct target_thr_exit_args {
	char state_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t state; char state_r_[PADR_(abi_uintptr_t)];
};
struct target_thr_self_args {
	char id_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t id; char id_r_[PADR_(abi_uintptr_t)];
};
struct target_thr_kill_args {
	char id_l_[PADL_(long)]; long id; char id_r_[PADR_(long)];
	char sig_l_[PADL_(int)]; int sig; char sig_r_[PADR_(int)];
};
struct target_jail_attach_args {
	char jid_l_[PADL_(int)]; int jid; char jid_r_[PADR_(int)];
};
struct target_extattr_list_fd_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char attrnamespace_l_[PADL_(int)]; int attrnamespace; char attrnamespace_r_[PADR_(int)];
	char data_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t data; char data_r_[PADR_(abi_uintptr_t)];
	char nbytes_l_[PADL_(size_t)]; size_t nbytes; char nbytes_r_[PADR_(size_t)];
};
struct target_extattr_list_file_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char attrnamespace_l_[PADL_(int)]; int attrnamespace; char attrnamespace_r_[PADR_(int)];
	char data_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t data; char data_r_[PADR_(abi_uintptr_t)];
	char nbytes_l_[PADL_(size_t)]; size_t nbytes; char nbytes_r_[PADR_(size_t)];
};
struct target_extattr_list_link_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char attrnamespace_l_[PADL_(int)]; int attrnamespace; char attrnamespace_r_[PADR_(int)];
	char data_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t data; char data_r_[PADR_(abi_uintptr_t)];
	char nbytes_l_[PADL_(size_t)]; size_t nbytes; char nbytes_r_[PADR_(size_t)];
};
struct target_ksem_timedwait_args {
	char id_l_[PADL_(semid_t)]; semid_t id; char id_r_[PADR_(semid_t)];
	char abstime_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t abstime; char abstime_r_[PADR_(abi_uintptr_t)];
};
struct target_thr_suspend_args {
	char timeout_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t timeout; char timeout_r_[PADR_(abi_uintptr_t)];
};
struct target_thr_wake_args {
	char id_l_[PADL_(long)]; long id; char id_r_[PADR_(long)];
};
struct target_kldunloadf_args {
	char fileid_l_[PADL_(int)]; int fileid; char fileid_r_[PADR_(int)];
	char flags_l_[PADL_(int)]; int flags; char flags_r_[PADR_(int)];
};
struct target_audit_args {
	char record_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t record; char record_r_[PADR_(abi_uintptr_t)];
	char length_l_[PADL_(u_int)]; u_int length; char length_r_[PADR_(u_int)];
};
struct target_auditon_args {
	char cmd_l_[PADL_(int)]; int cmd; char cmd_r_[PADR_(int)];
	char data_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t data; char data_r_[PADR_(abi_uintptr_t)];
	char length_l_[PADL_(u_int)]; u_int length; char length_r_[PADR_(u_int)];
};
struct target_getauid_args {
	char auid_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t auid; char auid_r_[PADR_(abi_uintptr_t)];
};
struct target_setauid_args {
	char auid_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t auid; char auid_r_[PADR_(abi_uintptr_t)];
};
struct target_getaudit_args {
	char auditinfo_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t auditinfo; char auditinfo_r_[PADR_(abi_uintptr_t)];
};
struct target_setaudit_args {
	char auditinfo_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t auditinfo; char auditinfo_r_[PADR_(abi_uintptr_t)];
};
struct target_getaudit_addr_args {
	char auditinfo_addr_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t auditinfo_addr; char auditinfo_addr_r_[PADR_(abi_uintptr_t)];
	char length_l_[PADL_(u_int)]; u_int length; char length_r_[PADR_(u_int)];
};
struct target_setaudit_addr_args {
	char auditinfo_addr_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t auditinfo_addr; char auditinfo_addr_r_[PADR_(abi_uintptr_t)];
	char length_l_[PADL_(u_int)]; u_int length; char length_r_[PADR_(u_int)];
};
struct target_auditctl_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
};
struct target__umtx_op_args {
	char obj_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t obj; char obj_r_[PADR_(abi_uintptr_t)];
	char op_l_[PADL_(int)]; int op; char op_r_[PADR_(int)];
	char val_l_[PADL_(u_long)]; u_long val; char val_r_[PADR_(u_long)];
	char uaddr1_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t uaddr1; char uaddr1_r_[PADR_(abi_uintptr_t)];
	char uaddr2_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t uaddr2; char uaddr2_r_[PADR_(abi_uintptr_t)];
};
struct target_thr_new_args {
	char param_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t param; char param_r_[PADR_(abi_uintptr_t)];
	char param_size_l_[PADL_(int)]; int param_size; char param_size_r_[PADR_(int)];
};
struct target_sigqueue_args {
	char pid_l_[PADL_(pid_t)]; pid_t pid; char pid_r_[PADR_(pid_t)];
	char signum_l_[PADL_(int)]; int signum; char signum_r_[PADR_(int)];
	char value_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t value; char value_r_[PADR_(abi_uintptr_t)];
};
struct target_kmq_open_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char flags_l_[PADL_(int)]; int flags; char flags_r_[PADR_(int)];
	char mode_l_[PADL_(mode_t)]; mode_t mode; char mode_r_[PADR_(mode_t)];
	char attr_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t attr; char attr_r_[PADR_(abi_uintptr_t)];
};
struct target_kmq_setattr_args {
	char mqd_l_[PADL_(int)]; int mqd; char mqd_r_[PADR_(int)];
	char attr_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t attr; char attr_r_[PADR_(abi_uintptr_t)];
	char oattr_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t oattr; char oattr_r_[PADR_(abi_uintptr_t)];
};
struct target_kmq_timedreceive_args {
	char mqd_l_[PADL_(int)]; int mqd; char mqd_r_[PADR_(int)];
	char msg_ptr_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t msg_ptr; char msg_ptr_r_[PADR_(abi_uintptr_t)];
	char msg_len_l_[PADL_(size_t)]; size_t msg_len; char msg_len_r_[PADR_(size_t)];
	char msg_prio_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t msg_prio; char msg_prio_r_[PADR_(abi_uintptr_t)];
	char abs_timeout_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t abs_timeout; char abs_timeout_r_[PADR_(abi_uintptr_t)];
};
struct target_kmq_timedsend_args {
	char mqd_l_[PADL_(int)]; int mqd; char mqd_r_[PADR_(int)];
	char msg_ptr_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t msg_ptr; char msg_ptr_r_[PADR_(abi_uintptr_t)];
	char msg_len_l_[PADL_(size_t)]; size_t msg_len; char msg_len_r_[PADR_(size_t)];
	char msg_prio_l_[PADL_(unsigned)]; unsigned msg_prio; char msg_prio_r_[PADR_(unsigned)];
	char abs_timeout_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t abs_timeout; char abs_timeout_r_[PADR_(abi_uintptr_t)];
};
struct target_kmq_notify_args {
	char mqd_l_[PADL_(int)]; int mqd; char mqd_r_[PADR_(int)];
	char sigev_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t sigev; char sigev_r_[PADR_(abi_uintptr_t)];
};
struct target_kmq_unlink_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
};
struct target_abort2_args {
	char why_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t why; char why_r_[PADR_(abi_uintptr_t)];
	char nargs_l_[PADL_(int)]; int nargs; char nargs_r_[PADR_(int)];
	char args_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t args; char args_r_[PADR_(abi_uintptr_t)];
};
struct target_thr_set_name_args {
	char id_l_[PADL_(long)]; long id; char id_r_[PADR_(long)];
	char name_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t name; char name_r_[PADR_(abi_uintptr_t)];
};
struct target_aio_fsync_args {
	char op_l_[PADL_(int)]; int op; char op_r_[PADR_(int)];
	char aiocbp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t aiocbp; char aiocbp_r_[PADR_(abi_uintptr_t)];
};
struct target_rtprio_thread_args {
	char function_l_[PADL_(int)]; int function; char function_r_[PADR_(int)];
	char lwpid_l_[PADL_(lwpid_t)]; lwpid_t lwpid; char lwpid_r_[PADR_(lwpid_t)];
	char rtp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t rtp; char rtp_r_[PADR_(abi_uintptr_t)];
};
struct target_sctp_peeloff_args {
	char sd_l_[PADL_(int)]; int sd; char sd_r_[PADR_(int)];
	char name_l_[PADL_(uint32_t)]; uint32_t name; char name_r_[PADR_(uint32_t)];
};
struct target_sctp_generic_sendmsg_args {
	char sd_l_[PADL_(int)]; int sd; char sd_r_[PADR_(int)];
	char msg_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t msg; char msg_r_[PADR_(abi_uintptr_t)];
	char mlen_l_[PADL_(int)]; int mlen; char mlen_r_[PADR_(int)];
	char to_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t to; char to_r_[PADR_(abi_uintptr_t)];
	char tolen_l_[PADL_(__socklen_t)]; __socklen_t tolen; char tolen_r_[PADR_(__socklen_t)];
	char sinfo_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t sinfo; char sinfo_r_[PADR_(abi_uintptr_t)];
	char flags_l_[PADL_(int)]; int flags; char flags_r_[PADR_(int)];
};
struct target_sctp_generic_sendmsg_iov_args {
	char sd_l_[PADL_(int)]; int sd; char sd_r_[PADR_(int)];
	char iov_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t iov; char iov_r_[PADR_(abi_uintptr_t)];
	char iovlen_l_[PADL_(int)]; int iovlen; char iovlen_r_[PADR_(int)];
	char to_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t to; char to_r_[PADR_(abi_uintptr_t)];
	char tolen_l_[PADL_(__socklen_t)]; __socklen_t tolen; char tolen_r_[PADR_(__socklen_t)];
	char sinfo_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t sinfo; char sinfo_r_[PADR_(abi_uintptr_t)];
	char flags_l_[PADL_(int)]; int flags; char flags_r_[PADR_(int)];
};
struct target_sctp_generic_recvmsg_args {
	char sd_l_[PADL_(int)]; int sd; char sd_r_[PADR_(int)];
	char iov_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t iov; char iov_r_[PADR_(abi_uintptr_t)];
	char iovlen_l_[PADL_(int)]; int iovlen; char iovlen_r_[PADR_(int)];
	char from_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t from; char from_r_[PADR_(abi_uintptr_t)];
	char fromlenaddr_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t fromlenaddr; char fromlenaddr_r_[PADR_(abi_uintptr_t)];
	char sinfo_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t sinfo; char sinfo_r_[PADR_(abi_uintptr_t)];
	char msg_flags_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t msg_flags; char msg_flags_r_[PADR_(abi_uintptr_t)];
};
struct target_pread_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char buf_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t buf; char buf_r_[PADR_(abi_uintptr_t)];
	char nbyte_l_[PADL_(size_t)]; size_t nbyte; char nbyte_r_[PADR_(size_t)];
	char offset_l_[PADL_(off_t)]; off_t offset; char offset_r_[PADR_(off_t)];
};
struct target_pwrite_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char buf_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t buf; char buf_r_[PADR_(abi_uintptr_t)];
	char nbyte_l_[PADL_(size_t)]; size_t nbyte; char nbyte_r_[PADR_(size_t)];
	char offset_l_[PADL_(off_t)]; off_t offset; char offset_r_[PADR_(off_t)];
};
struct target_mmap_args {
	char addr_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t addr; char addr_r_[PADR_(abi_uintptr_t)];
	char len_l_[PADL_(size_t)]; size_t len; char len_r_[PADR_(size_t)];
	char prot_l_[PADL_(int)]; int prot; char prot_r_[PADR_(int)];
	char flags_l_[PADL_(int)]; int flags; char flags_r_[PADR_(int)];
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char pos_l_[PADL_(off_t)]; off_t pos; char pos_r_[PADR_(off_t)];
};
struct target_lseek_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char offset_l_[PADL_(off_t)]; off_t offset; char offset_r_[PADR_(off_t)];
	char whence_l_[PADL_(int)]; int whence; char whence_r_[PADR_(int)];
};
struct target_truncate_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char length_l_[PADL_(off_t)]; off_t length; char length_r_[PADR_(off_t)];
};
struct target_ftruncate_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char length_l_[PADL_(off_t)]; off_t length; char length_r_[PADR_(off_t)];
};
struct target_thr_kill2_args {
	char pid_l_[PADL_(pid_t)]; pid_t pid; char pid_r_[PADR_(pid_t)];
	char id_l_[PADL_(long)]; long id; char id_r_[PADR_(long)];
	char sig_l_[PADL_(int)]; int sig; char sig_r_[PADR_(int)];
};
struct target_shm_unlink_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
};
struct target_cpuset_args {
	char setid_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t setid; char setid_r_[PADR_(abi_uintptr_t)];
};
struct target_cpuset_setid_args {
	char which_l_[PADL_(cpuwhich_t)]; cpuwhich_t which; char which_r_[PADR_(cpuwhich_t)];
	char id_l_[PADL_(id_t)]; id_t id; char id_r_[PADR_(id_t)];
	char setid_l_[PADL_(cpusetid_t)]; cpusetid_t setid; char setid_r_[PADR_(cpusetid_t)];
};
struct target_cpuset_getid_args {
	char level_l_[PADL_(cpulevel_t)]; cpulevel_t level; char level_r_[PADR_(cpulevel_t)];
	char which_l_[PADL_(cpuwhich_t)]; cpuwhich_t which; char which_r_[PADR_(cpuwhich_t)];
	char id_l_[PADL_(id_t)]; id_t id; char id_r_[PADR_(id_t)];
	char setid_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t setid; char setid_r_[PADR_(abi_uintptr_t)];
};
struct target_cpuset_getaffinity_args {
	char level_l_[PADL_(cpulevel_t)]; cpulevel_t level; char level_r_[PADR_(cpulevel_t)];
	char which_l_[PADL_(cpuwhich_t)]; cpuwhich_t which; char which_r_[PADR_(cpuwhich_t)];
	char id_l_[PADL_(id_t)]; id_t id; char id_r_[PADR_(id_t)];
	char cpusetsize_l_[PADL_(size_t)]; size_t cpusetsize; char cpusetsize_r_[PADR_(size_t)];
	char mask_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t mask; char mask_r_[PADR_(abi_uintptr_t)];
};
struct target_cpuset_setaffinity_args {
	char level_l_[PADL_(cpulevel_t)]; cpulevel_t level; char level_r_[PADR_(cpulevel_t)];
	char which_l_[PADL_(cpuwhich_t)]; cpuwhich_t which; char which_r_[PADR_(cpuwhich_t)];
	char id_l_[PADL_(id_t)]; id_t id; char id_r_[PADR_(id_t)];
	char cpusetsize_l_[PADL_(size_t)]; size_t cpusetsize; char cpusetsize_r_[PADR_(size_t)];
	char mask_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t mask; char mask_r_[PADR_(abi_uintptr_t)];
};
struct target_faccessat_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char amode_l_[PADL_(int)]; int amode; char amode_r_[PADR_(int)];
	char flag_l_[PADL_(int)]; int flag; char flag_r_[PADR_(int)];
};
struct target_fchmodat_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char mode_l_[PADL_(mode_t)]; mode_t mode; char mode_r_[PADR_(mode_t)];
	char flag_l_[PADL_(int)]; int flag; char flag_r_[PADR_(int)];
};
struct target_fchownat_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char uid_l_[PADL_(uid_t)]; uid_t uid; char uid_r_[PADR_(uid_t)];
	char gid_l_[PADL_(gid_t)]; gid_t gid; char gid_r_[PADR_(gid_t)];
	char flag_l_[PADL_(int)]; int flag; char flag_r_[PADR_(int)];
};
struct target_fexecve_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char argv_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t argv; char argv_r_[PADR_(abi_uintptr_t)];
	char envv_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t envv; char envv_r_[PADR_(abi_uintptr_t)];
};
struct target_futimesat_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char times_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t times; char times_r_[PADR_(abi_uintptr_t)];
};
struct target_linkat_args {
	char fd1_l_[PADL_(int)]; int fd1; char fd1_r_[PADR_(int)];
	char path1_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path1; char path1_r_[PADR_(abi_uintptr_t)];
	char fd2_l_[PADL_(int)]; int fd2; char fd2_r_[PADR_(int)];
	char path2_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path2; char path2_r_[PADR_(abi_uintptr_t)];
	char flag_l_[PADL_(int)]; int flag; char flag_r_[PADR_(int)];
};
struct target_mkdirat_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char mode_l_[PADL_(mode_t)]; mode_t mode; char mode_r_[PADR_(mode_t)];
};
struct target_mkfifoat_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char mode_l_[PADL_(mode_t)]; mode_t mode; char mode_r_[PADR_(mode_t)];
};
struct target_openat_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char flag_l_[PADL_(int)]; int flag; char flag_r_[PADR_(int)];
	char mode_l_[PADL_(mode_t)]; mode_t mode; char mode_r_[PADR_(mode_t)];
};
struct target_readlinkat_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char buf_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t buf; char buf_r_[PADR_(abi_uintptr_t)];
	char bufsize_l_[PADL_(size_t)]; size_t bufsize; char bufsize_r_[PADR_(size_t)];
};
struct target_renameat_args {
	char oldfd_l_[PADL_(int)]; int oldfd; char oldfd_r_[PADR_(int)];
	char old_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t old; char old_r_[PADR_(abi_uintptr_t)];
	char newfd_l_[PADL_(int)]; int newfd; char newfd_r_[PADR_(int)];
	char new_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t new; char new_r_[PADR_(abi_uintptr_t)];
};
struct target_symlinkat_args {
	char path1_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path1; char path1_r_[PADR_(abi_uintptr_t)];
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char path2_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path2; char path2_r_[PADR_(abi_uintptr_t)];
};
struct target_unlinkat_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char flag_l_[PADL_(int)]; int flag; char flag_r_[PADR_(int)];
};
struct target_posix_openpt_args {
	char flags_l_[PADL_(int)]; int flags; char flags_r_[PADR_(int)];
};
struct target_gssd_syscall_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
};
struct target_jail_get_args {
	char iovp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t iovp; char iovp_r_[PADR_(abi_uintptr_t)];
	char iovcnt_l_[PADL_(unsigned int)]; unsigned int iovcnt; char iovcnt_r_[PADR_(unsigned int)];
	char flags_l_[PADL_(int)]; int flags; char flags_r_[PADR_(int)];
};
struct target_jail_set_args {
	char iovp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t iovp; char iovp_r_[PADR_(abi_uintptr_t)];
	char iovcnt_l_[PADL_(unsigned int)]; unsigned int iovcnt; char iovcnt_r_[PADR_(unsigned int)];
	char flags_l_[PADL_(int)]; int flags; char flags_r_[PADR_(int)];
};
struct target_jail_remove_args {
	char jid_l_[PADL_(int)]; int jid; char jid_r_[PADR_(int)];
};
struct target___semctl_args {
	char semid_l_[PADL_(int)]; int semid; char semid_r_[PADR_(int)];
	char semnum_l_[PADL_(int)]; int semnum; char semnum_r_[PADR_(int)];
	char cmd_l_[PADL_(int)]; int cmd; char cmd_r_[PADR_(int)];
	char arg_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t arg; char arg_r_[PADR_(abi_uintptr_t)];
};
struct target_msgctl_args {
	char msqid_l_[PADL_(int)]; int msqid; char msqid_r_[PADR_(int)];
	char cmd_l_[PADL_(int)]; int cmd; char cmd_r_[PADR_(int)];
	char buf_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t buf; char buf_r_[PADR_(abi_uintptr_t)];
};
struct target_shmctl_args {
	char shmid_l_[PADL_(int)]; int shmid; char shmid_r_[PADR_(int)];
	char cmd_l_[PADL_(int)]; int cmd; char cmd_r_[PADR_(int)];
	char buf_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t buf; char buf_r_[PADR_(abi_uintptr_t)];
};
struct target_lpathconf_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char name_l_[PADL_(int)]; int name; char name_r_[PADR_(int)];
};
struct target___cap_rights_get_args {
	char version_l_[PADL_(int)]; int version; char version_r_[PADR_(int)];
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char rightsp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t rightsp; char rightsp_r_[PADR_(abi_uintptr_t)];
};
struct target_cap_enter_args {
	register_t dummy;
};
struct target_cap_getmode_args {
	char modep_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t modep; char modep_r_[PADR_(abi_uintptr_t)];
};
struct target_pdfork_args {
	char fdp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t fdp; char fdp_r_[PADR_(abi_uintptr_t)];
	char flags_l_[PADL_(int)]; int flags; char flags_r_[PADR_(int)];
};
struct target_pdkill_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char signum_l_[PADL_(int)]; int signum; char signum_r_[PADR_(int)];
};
struct target_pdgetpid_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char pidp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t pidp; char pidp_r_[PADR_(abi_uintptr_t)];
};
struct target_pselect_args {
	char nd_l_[PADL_(int)]; int nd; char nd_r_[PADR_(int)];
	char in_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t in; char in_r_[PADR_(abi_uintptr_t)];
	char ou_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t ou; char ou_r_[PADR_(abi_uintptr_t)];
	char ex_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t ex; char ex_r_[PADR_(abi_uintptr_t)];
	char ts_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t ts; char ts_r_[PADR_(abi_uintptr_t)];
	char sm_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t sm; char sm_r_[PADR_(abi_uintptr_t)];
};
struct target_getloginclass_args {
	char namebuf_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t namebuf; char namebuf_r_[PADR_(abi_uintptr_t)];
	char namelen_l_[PADL_(size_t)]; size_t namelen; char namelen_r_[PADR_(size_t)];
};
struct target_setloginclass_args {
	char namebuf_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t namebuf; char namebuf_r_[PADR_(abi_uintptr_t)];
};
struct target_rctl_get_racct_args {
	char inbufp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t inbufp; char inbufp_r_[PADR_(abi_uintptr_t)];
	char inbuflen_l_[PADL_(size_t)]; size_t inbuflen; char inbuflen_r_[PADR_(size_t)];
	char outbufp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t outbufp; char outbufp_r_[PADR_(abi_uintptr_t)];
	char outbuflen_l_[PADL_(size_t)]; size_t outbuflen; char outbuflen_r_[PADR_(size_t)];
};
struct target_rctl_get_rules_args {
	char inbufp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t inbufp; char inbufp_r_[PADR_(abi_uintptr_t)];
	char inbuflen_l_[PADL_(size_t)]; size_t inbuflen; char inbuflen_r_[PADR_(size_t)];
	char outbufp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t outbufp; char outbufp_r_[PADR_(abi_uintptr_t)];
	char outbuflen_l_[PADL_(size_t)]; size_t outbuflen; char outbuflen_r_[PADR_(size_t)];
};
struct target_rctl_get_limits_args {
	char inbufp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t inbufp; char inbufp_r_[PADR_(abi_uintptr_t)];
	char inbuflen_l_[PADL_(size_t)]; size_t inbuflen; char inbuflen_r_[PADR_(size_t)];
	char outbufp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t outbufp; char outbufp_r_[PADR_(abi_uintptr_t)];
	char outbuflen_l_[PADL_(size_t)]; size_t outbuflen; char outbuflen_r_[PADR_(size_t)];
};
struct target_rctl_add_rule_args {
	char inbufp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t inbufp; char inbufp_r_[PADR_(abi_uintptr_t)];
	char inbuflen_l_[PADL_(size_t)]; size_t inbuflen; char inbuflen_r_[PADR_(size_t)];
	char outbufp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t outbufp; char outbufp_r_[PADR_(abi_uintptr_t)];
	char outbuflen_l_[PADL_(size_t)]; size_t outbuflen; char outbuflen_r_[PADR_(size_t)];
};
struct target_rctl_remove_rule_args {
	char inbufp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t inbufp; char inbufp_r_[PADR_(abi_uintptr_t)];
	char inbuflen_l_[PADL_(size_t)]; size_t inbuflen; char inbuflen_r_[PADR_(size_t)];
	char outbufp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t outbufp; char outbufp_r_[PADR_(abi_uintptr_t)];
	char outbuflen_l_[PADL_(size_t)]; size_t outbuflen; char outbuflen_r_[PADR_(size_t)];
};
struct target_posix_fallocate_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char offset_l_[PADL_(off_t)]; off_t offset; char offset_r_[PADR_(off_t)];
	char len_l_[PADL_(off_t)]; off_t len; char len_r_[PADR_(off_t)];
};
struct target_posix_fadvise_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char offset_l_[PADL_(off_t)]; off_t offset; char offset_r_[PADR_(off_t)];
	char len_l_[PADL_(off_t)]; off_t len; char len_r_[PADR_(off_t)];
	char advice_l_[PADL_(int)]; int advice; char advice_r_[PADR_(int)];
};
struct target_wait6_args {
	char idtype_l_[PADL_(idtype_t)]; idtype_t idtype; char idtype_r_[PADR_(idtype_t)];
	char id_l_[PADL_(id_t)]; id_t id; char id_r_[PADR_(id_t)];
	char status_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t status; char status_r_[PADR_(abi_uintptr_t)];
	char options_l_[PADL_(int)]; int options; char options_r_[PADR_(int)];
	char wrusage_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t wrusage; char wrusage_r_[PADR_(abi_uintptr_t)];
	char info_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t info; char info_r_[PADR_(abi_uintptr_t)];
};
struct target_cap_rights_limit_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char rightsp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t rightsp; char rightsp_r_[PADR_(abi_uintptr_t)];
};
struct target_cap_ioctls_limit_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char cmds_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t cmds; char cmds_r_[PADR_(abi_uintptr_t)];
	char ncmds_l_[PADL_(size_t)]; size_t ncmds; char ncmds_r_[PADR_(size_t)];
};
struct target_cap_ioctls_get_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char cmds_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t cmds; char cmds_r_[PADR_(abi_uintptr_t)];
	char maxcmds_l_[PADL_(size_t)]; size_t maxcmds; char maxcmds_r_[PADR_(size_t)];
};
struct target_cap_fcntls_limit_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char fcntlrights_l_[PADL_(uint32_t)]; uint32_t fcntlrights; char fcntlrights_r_[PADR_(uint32_t)];
};
struct target_cap_fcntls_get_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char fcntlrightsp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t fcntlrightsp; char fcntlrightsp_r_[PADR_(abi_uintptr_t)];
};
struct target_bindat_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char s_l_[PADL_(int)]; int s; char s_r_[PADR_(int)];
	char name_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t name; char name_r_[PADR_(abi_uintptr_t)];
	char namelen_l_[PADL_(__socklen_t)]; __socklen_t namelen; char namelen_r_[PADR_(__socklen_t)];
};
struct target_connectat_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char s_l_[PADL_(int)]; int s; char s_r_[PADR_(int)];
	char name_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t name; char name_r_[PADR_(abi_uintptr_t)];
	char namelen_l_[PADL_(__socklen_t)]; __socklen_t namelen; char namelen_r_[PADR_(__socklen_t)];
};
struct target_chflagsat_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char flags_l_[PADL_(u_long)]; u_long flags; char flags_r_[PADR_(u_long)];
	char atflag_l_[PADL_(int)]; int atflag; char atflag_r_[PADR_(int)];
};
struct target_accept4_args {
	char s_l_[PADL_(int)]; int s; char s_r_[PADR_(int)];
	char name_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t name; char name_r_[PADR_(abi_uintptr_t)];
	char anamelen_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t anamelen; char anamelen_r_[PADR_(abi_uintptr_t)];
	char flags_l_[PADL_(int)]; int flags; char flags_r_[PADR_(int)];
};
struct target_pipe2_args {
	char fildes_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t fildes; char fildes_r_[PADR_(abi_uintptr_t)];
	char flags_l_[PADL_(int)]; int flags; char flags_r_[PADR_(int)];
};
struct target_aio_mlock_args {
	char aiocbp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t aiocbp; char aiocbp_r_[PADR_(abi_uintptr_t)];
};
struct target_procctl_args {
	char idtype_l_[PADL_(idtype_t)]; idtype_t idtype; char idtype_r_[PADR_(idtype_t)];
	char id_l_[PADL_(id_t)]; id_t id; char id_r_[PADR_(id_t)];
	char com_l_[PADL_(int)]; int com; char com_r_[PADR_(int)];
	char data_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t data; char data_r_[PADR_(abi_uintptr_t)];
};
struct target_ppoll_args {
	char fds_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t fds; char fds_r_[PADR_(abi_uintptr_t)];
	char nfds_l_[PADL_(u_int)]; u_int nfds; char nfds_r_[PADR_(u_int)];
	char ts_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t ts; char ts_r_[PADR_(abi_uintptr_t)];
	char set_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t set; char set_r_[PADR_(abi_uintptr_t)];
};
struct target_futimens_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char times_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t times; char times_r_[PADR_(abi_uintptr_t)];
};
struct target_utimensat_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char times_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t times; char times_r_[PADR_(abi_uintptr_t)];
	char flag_l_[PADL_(int)]; int flag; char flag_r_[PADR_(int)];
};
struct target_fdatasync_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
};
struct target_fstat_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char sb_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t sb; char sb_r_[PADR_(abi_uintptr_t)];
};
struct target_fstatat_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char buf_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t buf; char buf_r_[PADR_(abi_uintptr_t)];
	char flag_l_[PADL_(int)]; int flag; char flag_r_[PADR_(int)];
};
struct target_fhstat_args {
	char u_fhp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t u_fhp; char u_fhp_r_[PADR_(abi_uintptr_t)];
	char sb_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t sb; char sb_r_[PADR_(abi_uintptr_t)];
};
struct target_getdirentries_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char buf_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t buf; char buf_r_[PADR_(abi_uintptr_t)];
	char count_l_[PADL_(size_t)]; size_t count; char count_r_[PADR_(size_t)];
	char basep_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t basep; char basep_r_[PADR_(abi_uintptr_t)];
};
struct target_statfs_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char buf_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t buf; char buf_r_[PADR_(abi_uintptr_t)];
};
struct target_fstatfs_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char buf_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t buf; char buf_r_[PADR_(abi_uintptr_t)];
};
struct target_getfsstat_args {
	char buf_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t buf; char buf_r_[PADR_(abi_uintptr_t)];
	char bufsize_l_[PADL_(long)]; long bufsize; char bufsize_r_[PADR_(long)];
	char mode_l_[PADL_(int)]; int mode; char mode_r_[PADR_(int)];
};
struct target_fhstatfs_args {
	char u_fhp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t u_fhp; char u_fhp_r_[PADR_(abi_uintptr_t)];
	char buf_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t buf; char buf_r_[PADR_(abi_uintptr_t)];
};
struct target_mknodat_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char mode_l_[PADL_(mode_t)]; mode_t mode; char mode_r_[PADR_(mode_t)];
	char dev_l_[PADL_(dev_t)]; dev_t dev; char dev_r_[PADR_(dev_t)];
};
struct target_kevent_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char changelist_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t changelist; char changelist_r_[PADR_(abi_uintptr_t)];
	char nchanges_l_[PADL_(int)]; int nchanges; char nchanges_r_[PADR_(int)];
	char eventlist_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t eventlist; char eventlist_r_[PADR_(abi_uintptr_t)];
	char nevents_l_[PADL_(int)]; int nevents; char nevents_r_[PADR_(int)];
	char timeout_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t timeout; char timeout_r_[PADR_(abi_uintptr_t)];
};
struct target_cpuset_getdomain_args {
	char level_l_[PADL_(cpulevel_t)]; cpulevel_t level; char level_r_[PADR_(cpulevel_t)];
	char which_l_[PADL_(cpuwhich_t)]; cpuwhich_t which; char which_r_[PADR_(cpuwhich_t)];
	char id_l_[PADL_(id_t)]; id_t id; char id_r_[PADR_(id_t)];
	char domainsetsize_l_[PADL_(size_t)]; size_t domainsetsize; char domainsetsize_r_[PADR_(size_t)];
	char mask_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t mask; char mask_r_[PADR_(abi_uintptr_t)];
	char policy_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t policy; char policy_r_[PADR_(abi_uintptr_t)];
};
struct target_cpuset_setdomain_args {
	char level_l_[PADL_(cpulevel_t)]; cpulevel_t level; char level_r_[PADR_(cpulevel_t)];
	char which_l_[PADL_(cpuwhich_t)]; cpuwhich_t which; char which_r_[PADR_(cpuwhich_t)];
	char id_l_[PADL_(id_t)]; id_t id; char id_r_[PADR_(id_t)];
	char domainsetsize_l_[PADL_(size_t)]; size_t domainsetsize; char domainsetsize_r_[PADR_(size_t)];
	char mask_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t mask; char mask_r_[PADR_(abi_uintptr_t)];
	char policy_l_[PADL_(int)]; int policy; char policy_r_[PADR_(int)];
};
struct target_getrandom_args {
	char buf_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t buf; char buf_r_[PADR_(abi_uintptr_t)];
	char buflen_l_[PADL_(size_t)]; size_t buflen; char buflen_r_[PADR_(size_t)];
	char flags_l_[PADL_(unsigned int)]; unsigned int flags; char flags_r_[PADR_(unsigned int)];
};
struct target_getfhat_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char fhp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t fhp; char fhp_r_[PADR_(abi_uintptr_t)];
	char flags_l_[PADL_(int)]; int flags; char flags_r_[PADR_(int)];
};
struct target_fhlink_args {
	char fhp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t fhp; char fhp_r_[PADR_(abi_uintptr_t)];
	char to_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t to; char to_r_[PADR_(abi_uintptr_t)];
};
struct target_fhlinkat_args {
	char fhp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t fhp; char fhp_r_[PADR_(abi_uintptr_t)];
	char tofd_l_[PADL_(int)]; int tofd; char tofd_r_[PADR_(int)];
	char to_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t to; char to_r_[PADR_(abi_uintptr_t)];
};
struct target_fhreadlink_args {
	char fhp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t fhp; char fhp_r_[PADR_(abi_uintptr_t)];
	char buf_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t buf; char buf_r_[PADR_(abi_uintptr_t)];
	char bufsize_l_[PADL_(size_t)]; size_t bufsize; char bufsize_r_[PADR_(size_t)];
};
struct target_funlinkat_args {
	char dfd_l_[PADL_(int)]; int dfd; char dfd_r_[PADR_(int)];
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char flag_l_[PADL_(int)]; int flag; char flag_r_[PADR_(int)];
};
struct target_copy_file_range_args {
	char infd_l_[PADL_(int)]; int infd; char infd_r_[PADR_(int)];
	char inoffp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t inoffp; char inoffp_r_[PADR_(abi_uintptr_t)];
	char outfd_l_[PADL_(int)]; int outfd; char outfd_r_[PADR_(int)];
	char outoffp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t outoffp; char outoffp_r_[PADR_(abi_uintptr_t)];
	char len_l_[PADL_(size_t)]; size_t len; char len_r_[PADR_(size_t)];
	char flags_l_[PADL_(unsigned int)]; unsigned int flags; char flags_r_[PADR_(unsigned int)];
};
struct target___sysctlbyname_args {
	char name_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t name; char name_r_[PADR_(abi_uintptr_t)];
	char namelen_l_[PADL_(size_t)]; size_t namelen; char namelen_r_[PADR_(size_t)];
	char old_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t old; char old_r_[PADR_(abi_uintptr_t)];
	char oldlenp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t oldlenp; char oldlenp_r_[PADR_(abi_uintptr_t)];
	char new_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t new; char new_r_[PADR_(abi_uintptr_t)];
	char newlen_l_[PADL_(size_t)]; size_t newlen; char newlen_r_[PADR_(size_t)];
};
struct target_shm_open2_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char flags_l_[PADL_(int)]; int flags; char flags_r_[PADR_(int)];
	char mode_l_[PADL_(mode_t)]; mode_t mode; char mode_r_[PADR_(mode_t)];
	char shmflags_l_[PADL_(int)]; int shmflags; char shmflags_r_[PADR_(int)];
	char name_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t name; char name_r_[PADR_(abi_uintptr_t)];
};
struct target_shm_rename_args {
	char path_from_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path_from; char path_from_r_[PADR_(abi_uintptr_t)];
	char path_to_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path_to; char path_to_r_[PADR_(abi_uintptr_t)];
	char flags_l_[PADL_(int)]; int flags; char flags_r_[PADR_(int)];
};
struct target_sigfastblock_args {
	char cmd_l_[PADL_(int)]; int cmd; char cmd_r_[PADR_(int)];
	char ptr_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t ptr; char ptr_r_[PADR_(abi_uintptr_t)];
};
struct target___realpathat_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char buf_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t buf; char buf_r_[PADR_(abi_uintptr_t)];
	char size_l_[PADL_(size_t)]; size_t size; char size_r_[PADR_(size_t)];
	char flags_l_[PADL_(int)]; int flags; char flags_r_[PADR_(int)];
};
struct target_close_range_args {
	char lowfd_l_[PADL_(u_int)]; u_int lowfd; char lowfd_r_[PADR_(u_int)];
	char highfd_l_[PADL_(u_int)]; u_int highfd; char highfd_r_[PADR_(u_int)];
	char flags_l_[PADL_(int)]; int flags; char flags_r_[PADR_(int)];
};
struct target_rpctls_syscall_args {
	char op_l_[PADL_(int)]; int op; char op_r_[PADR_(int)];
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
};

#ifdef COMPAT_43


#endif /* COMPAT_43 */


#ifdef COMPAT_FREEBSD4


#endif /* COMPAT_FREEBSD4 */


#ifdef COMPAT_FREEBSD6


#endif /* COMPAT_FREEBSD6 */


#ifdef COMPAT_FREEBSD7


#endif /* COMPAT_FREEBSD7 */


#ifdef COMPAT_FREEBSD10


#endif /* COMPAT_FREEBSD10 */


#ifdef COMPAT_FREEBSD11

struct freebsd11_target_mknod_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char mode_l_[PADL_(int)]; int mode; char mode_r_[PADR_(int)];
	char dev_l_[PADL_(uint32_t)]; uint32_t dev; char dev_r_[PADR_(uint32_t)];
};
struct freebsd11_target_vadvise_args {
	char anom_l_[PADL_(int)]; int anom; char anom_r_[PADR_(int)];
};
struct freebsd11_target_stat_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char ub_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t ub; char ub_r_[PADR_(abi_uintptr_t)];
};
struct freebsd11_target_fstat_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char sb_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t sb; char sb_r_[PADR_(abi_uintptr_t)];
};
struct freebsd11_target_lstat_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char ub_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t ub; char ub_r_[PADR_(abi_uintptr_t)];
};
struct freebsd11_target_getdirentries_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char buf_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t buf; char buf_r_[PADR_(abi_uintptr_t)];
	char count_l_[PADL_(u_int)]; u_int count; char count_r_[PADR_(u_int)];
	char basep_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t basep; char basep_r_[PADR_(abi_uintptr_t)];
};
struct freebsd11_target_getdents_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char buf_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t buf; char buf_r_[PADR_(abi_uintptr_t)];
	char count_l_[PADL_(size_t)]; size_t count; char count_r_[PADR_(size_t)];
};
struct freebsd11_target_nstat_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char ub_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t ub; char ub_r_[PADR_(abi_uintptr_t)];
};
struct freebsd11_target_nfstat_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char sb_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t sb; char sb_r_[PADR_(abi_uintptr_t)];
};
struct freebsd11_target_nlstat_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char ub_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t ub; char ub_r_[PADR_(abi_uintptr_t)];
};
struct freebsd11_target_fhstat_args {
	char u_fhp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t u_fhp; char u_fhp_r_[PADR_(abi_uintptr_t)];
	char sb_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t sb; char sb_r_[PADR_(abi_uintptr_t)];
};
struct freebsd11_target_kevent_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char changelist_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t changelist; char changelist_r_[PADR_(abi_uintptr_t)];
	char nchanges_l_[PADL_(int)]; int nchanges; char nchanges_r_[PADR_(int)];
	char eventlist_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t eventlist; char eventlist_r_[PADR_(abi_uintptr_t)];
	char nevents_l_[PADL_(int)]; int nevents; char nevents_r_[PADR_(int)];
	char timeout_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t timeout; char timeout_r_[PADR_(abi_uintptr_t)];
};
struct freebsd11_target_getfsstat_args {
	char buf_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t buf; char buf_r_[PADR_(abi_uintptr_t)];
	char bufsize_l_[PADL_(long)]; long bufsize; char bufsize_r_[PADR_(long)];
	char mode_l_[PADL_(int)]; int mode; char mode_r_[PADR_(int)];
};
struct freebsd11_target_statfs_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char buf_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t buf; char buf_r_[PADR_(abi_uintptr_t)];
};
struct freebsd11_target_fstatfs_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char buf_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t buf; char buf_r_[PADR_(abi_uintptr_t)];
};
struct freebsd11_target_fhstatfs_args {
	char u_fhp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t u_fhp; char u_fhp_r_[PADR_(abi_uintptr_t)];
	char buf_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t buf; char buf_r_[PADR_(abi_uintptr_t)];
};
struct freebsd11_target_fstatat_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char buf_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t buf; char buf_r_[PADR_(abi_uintptr_t)];
	char flag_l_[PADL_(int)]; int flag; char flag_r_[PADR_(int)];
};
struct freebsd11_target_mknodat_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char mode_l_[PADL_(mode_t)]; mode_t mode; char mode_r_[PADR_(mode_t)];
	char dev_l_[PADL_(uint32_t)]; uint32_t dev; char dev_r_[PADR_(uint32_t)];
};

#endif /* COMPAT_FREEBSD11 */


#ifdef COMPAT_FREEBSD12

struct freebsd12_target_shm_open_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char flags_l_[PADL_(int)]; int flags; char flags_r_[PADR_(int)];
	char mode_l_[PADL_(mode_t)]; mode_t mode; char mode_r_[PADR_(mode_t)];
};
struct freebsd12_target_closefrom_args {
	char lowfd_l_[PADL_(int)]; int lowfd; char lowfd_r_[PADR_(int)];
};

#endif /* COMPAT_FREEBSD12 */


#undef PAD_
#undef PADL_
#undef PADR_

#endif /* !_TARGET_OS_PROTO_H_ */
