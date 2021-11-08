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

#define	PAD_(t)	(sizeof(abi_syscallarg_t) <= sizeof(t) ? \
		0 : sizeof(abi_syscallarg_t) - sizeof(t))

#if BYTE_ORDER == LITTLE_ENDIAN
#define	PADL_(t)	0
#define	PADR_(t)	PAD_(t)
#else
#define	PADL_(t)	PAD_(t)
#define	PADR_(t)	0
#endif

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
struct target_chroot_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
};
struct target_msync_args {
	char addr_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t addr; char addr_r_[PADR_(abi_uintptr_t)];
	char len_l_[PADL_(size_t)]; size_t len; char len_r_[PADR_(size_t)];
	char flags_l_[PADL_(int)]; int flags; char flags_r_[PADR_(int)];
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
struct target_connect_args {
	char s_l_[PADL_(int)]; int s; char s_r_[PADR_(int)];
	char name_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t name; char name_r_[PADR_(abi_uintptr_t)];
	char namelen_l_[PADL_(__socklen_t)]; __socklen_t namelen; char namelen_r_[PADR_(__socklen_t)];
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
struct target_rename_args {
	char from_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t from; char from_r_[PADR_(abi_uintptr_t)];
	char to_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t to; char to_r_[PADR_(abi_uintptr_t)];
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
struct target_ntp_adjtime_args {
	char tp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t tp; char tp_r_[PADR_(abi_uintptr_t)];
};
struct target_pathconf_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
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
struct target_poll_args {
	char fds_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t fds; char fds_r_[PADR_(abi_uintptr_t)];
	char nfds_l_[PADL_(u_int)]; u_int nfds; char nfds_r_[PADR_(u_int)];
	char timeout_l_[PADL_(int)]; int timeout; char timeout_r_[PADR_(int)];
};
struct target_semop_args {
	char semid_l_[PADL_(int)]; int semid; char semid_r_[PADR_(int)];
	char sops_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t sops; char sops_r_[PADR_(abi_uintptr_t)];
	char nsops_l_[PADL_(size_t)]; size_t nsops; char nsops_r_[PADR_(size_t)];
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
struct target_modstat_args {
	char modid_l_[PADL_(int)]; int modid; char modid_r_[PADR_(int)];
	char stat_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t stat; char stat_r_[PADR_(abi_uintptr_t)];
};
struct target_modfind_args {
	char name_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t name; char name_r_[PADR_(abi_uintptr_t)];
};
struct target_kldload_args {
	char file_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t file; char file_r_[PADR_(abi_uintptr_t)];
};
struct target_kldfind_args {
	char file_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t file; char file_r_[PADR_(abi_uintptr_t)];
};
struct target_kldstat_args {
	char fileid_l_[PADL_(int)]; int fileid; char fileid_r_[PADR_(int)];
	char stat_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t stat; char stat_r_[PADR_(abi_uintptr_t)];
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
struct target_eaccess_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char amode_l_[PADL_(int)]; int amode; char amode_r_[PADR_(int)];
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
struct target_truncate_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char length_l_[PADL_(off_t)]; off_t length; char length_r_[PADR_(off_t)];
};
struct target_shm_unlink_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
};
struct target_cpuset_args {
	char setid_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t setid; char setid_r_[PADR_(abi_uintptr_t)];
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
struct target_cap_getmode_args {
	char modep_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t modep; char modep_r_[PADR_(abi_uintptr_t)];
};
struct target_pdfork_args {
	char fdp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t fdp; char fdp_r_[PADR_(abi_uintptr_t)];
	char flags_l_[PADL_(int)]; int flags; char flags_r_[PADR_(int)];
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
struct target_rpctls_syscall_args {
	char op_l_[PADL_(int)]; int op; char op_r_[PADR_(int)];
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
};
int	sys_read(struct thread *, struct target_read_args *);
int	sys_write(struct thread *, struct target_write_args *);
int	sys_open(struct thread *, struct target_open_args *);
int	sys_wait4(struct thread *, struct target_wait4_args *);
int	sys_link(struct thread *, struct target_link_args *);
int	sys_unlink(struct thread *, struct target_unlink_args *);
int	sys_chdir(struct thread *, struct target_chdir_args *);
int	sys_chmod(struct thread *, struct target_chmod_args *);
int	sys_chown(struct thread *, struct target_chown_args *);
int	sys_break(struct thread *, struct target_break_args *);
int	sys_mount(struct thread *, struct target_mount_args *);
int	sys_unmount(struct thread *, struct target_unmount_args *);
int	sys_ptrace(struct thread *, struct target_ptrace_args *);
int	sys_recvmsg(struct thread *, struct target_recvmsg_args *);
int	sys_sendmsg(struct thread *, struct target_sendmsg_args *);
int	sys_recvfrom(struct thread *, struct target_recvfrom_args *);
int	sys_accept(struct thread *, struct target_accept_args *);
int	sys_getpeername(struct thread *, struct target_getpeername_args *);
int	sys_getsockname(struct thread *, struct target_getsockname_args *);
int	sys_access(struct thread *, struct target_access_args *);
int	sys_chflags(struct thread *, struct target_chflags_args *);
int	sys_profil(struct thread *, struct target_profil_args *);
int	sys_ktrace(struct thread *, struct target_ktrace_args *);
int	sys_getlogin(struct thread *, struct target_getlogin_args *);
int	sys_setlogin(struct thread *, struct target_setlogin_args *);
int	sys_acct(struct thread *, struct target_acct_args *);
int	sys_sigaltstack(struct thread *, struct target_sigaltstack_args *);
int	sys_ioctl(struct thread *, struct target_ioctl_args *);
int	sys_revoke(struct thread *, struct target_revoke_args *);
int	sys_symlink(struct thread *, struct target_symlink_args *);
int	sys_readlink(struct thread *, struct target_readlink_args *);
int	sys_execve(struct thread *, struct target_execve_args *);
int	sys_chroot(struct thread *, struct target_chroot_args *);
int	sys_msync(struct thread *, struct target_msync_args *);
int	sys_munmap(struct thread *, struct target_munmap_args *);
int	sys_mprotect(struct thread *, struct target_mprotect_args *);
int	sys_madvise(struct thread *, struct target_madvise_args *);
int	sys_mincore(struct thread *, struct target_mincore_args *);
int	sys_getgroups(struct thread *, struct target_getgroups_args *);
int	sys_setgroups(struct thread *, struct target_setgroups_args *);
int	sys_setitimer(struct thread *, struct target_setitimer_args *);
int	sys_swapon(struct thread *, struct target_swapon_args *);
int	sys_getitimer(struct thread *, struct target_getitimer_args *);
int	sys_fcntl(struct thread *, struct target_fcntl_args *);
int	sys_select(struct thread *, struct target_select_args *);
int	sys_connect(struct thread *, struct target_connect_args *);
int	sys_bind(struct thread *, struct target_bind_args *);
int	sys_setsockopt(struct thread *, struct target_setsockopt_args *);
int	sys_gettimeofday(struct thread *, struct target_gettimeofday_args *);
int	sys_getrusage(struct thread *, struct target_getrusage_args *);
int	sys_getsockopt(struct thread *, struct target_getsockopt_args *);
int	sys_readv(struct thread *, struct target_readv_args *);
int	sys_writev(struct thread *, struct target_writev_args *);
int	sys_settimeofday(struct thread *, struct target_settimeofday_args *);
int	sys_rename(struct thread *, struct target_rename_args *);
int	sys_mkfifo(struct thread *, struct target_mkfifo_args *);
int	sys_sendto(struct thread *, struct target_sendto_args *);
int	sys_socketpair(struct thread *, struct target_socketpair_args *);
int	sys_mkdir(struct thread *, struct target_mkdir_args *);
int	sys_rmdir(struct thread *, struct target_rmdir_args *);
int	sys_utimes(struct thread *, struct target_utimes_args *);
int	sys_adjtime(struct thread *, struct target_adjtime_args *);
int	sys_quotactl(struct thread *, struct target_quotactl_args *);
int	sys_nlm_syscall(struct thread *, struct target_nlm_syscall_args *);
int	sys_nfssvc(struct thread *, struct target_nfssvc_args *);
int	sys_lgetfh(struct thread *, struct target_lgetfh_args *);
int	sys_getfh(struct thread *, struct target_getfh_args *);
int	sysarch(struct thread *, struct target_sysarch_args *);
int	sys_rtprio(struct thread *, struct target_rtprio_args *);
int	sys_semsys(struct thread *, struct target_semsys_args *);
int	sys_msgsys(struct thread *, struct target_msgsys_args *);
int	sys_shmsys(struct thread *, struct target_shmsys_args *);
int	sys_ntp_adjtime(struct thread *, struct target_ntp_adjtime_args *);
int	sys_pathconf(struct thread *, struct target_pathconf_args *);
int	sys_getrlimit(struct thread *, struct target___getrlimit_args *);
int	sys_setrlimit(struct thread *, struct target___setrlimit_args *);
int	sys___sysctl(struct thread *, struct target___sysctl_args *);
int	sys_mlock(struct thread *, struct target_mlock_args *);
int	sys_munlock(struct thread *, struct target_munlock_args *);
int	sys_undelete(struct thread *, struct target_undelete_args *);
int	sys_futimes(struct thread *, struct target_futimes_args *);
int	sys_poll(struct thread *, struct target_poll_args *);
int	sys_semop(struct thread *, struct target_semop_args *);
int	sys_msgsnd(struct thread *, struct target_msgsnd_args *);
int	sys_msgrcv(struct thread *, struct target_msgrcv_args *);
int	sys_shmat(struct thread *, struct target_shmat_args *);
int	sys_shmdt(struct thread *, struct target_shmdt_args *);
int	sys_clock_gettime(struct thread *, struct target_clock_gettime_args *);
int	sys_clock_settime(struct thread *, struct target_clock_settime_args *);
int	sys_clock_getres(struct thread *, struct target_clock_getres_args *);
int	sys_ktimer_create(struct thread *, struct target_ktimer_create_args *);
int	sys_ktimer_settime(struct thread *, struct target_ktimer_settime_args *);
int	sys_ktimer_gettime(struct thread *, struct target_ktimer_gettime_args *);
int	sys_nanosleep(struct thread *, struct target_nanosleep_args *);
int	sys_ffclock_getcounter(struct thread *, struct target_ffclock_getcounter_args *);
int	sys_ffclock_setestimate(struct thread *, struct target_ffclock_setestimate_args *);
int	sys_ffclock_getestimate(struct thread *, struct target_ffclock_getestimate_args *);
int	sys_clock_nanosleep(struct thread *, struct target_clock_nanosleep_args *);
int	sys_clock_getcpuclockid2(struct thread *, struct target_clock_getcpuclockid2_args *);
int	sys_ntp_gettime(struct thread *, struct target_ntp_gettime_args *);
int	sys_minherit(struct thread *, struct target_minherit_args *);
int	sys_lchown(struct thread *, struct target_lchown_args *);
int	sys_aio_read(struct thread *, struct target_aio_read_args *);
int	sys_aio_write(struct thread *, struct target_aio_write_args *);
int	sys_lio_listio(struct thread *, struct target_lio_listio_args *);
int	sys_kbounce(struct thread *, struct target_kbounce_args *);
int	sys_flag_captured(struct thread *, struct target_flag_captured_args *);
int	sys_lchmod(struct thread *, struct target_lchmod_args *);
int	sys_lutimes(struct thread *, struct target_lutimes_args *);
int	sys_preadv(struct thread *, struct target_preadv_args *);
int	sys_pwritev(struct thread *, struct target_pwritev_args *);
int	sys_fhopen(struct thread *, struct target_fhopen_args *);
int	sys_modstat(struct thread *, struct target_modstat_args *);
int	sys_modfind(struct thread *, struct target_modfind_args *);
int	sys_kldload(struct thread *, struct target_kldload_args *);
int	sys_kldfind(struct thread *, struct target_kldfind_args *);
int	sys_kldstat(struct thread *, struct target_kldstat_args *);
int	sys_aio_return(struct thread *, struct target_aio_return_args *);
int	sys_aio_suspend(struct thread *, struct target_aio_suspend_args *);
int	sys_aio_cancel(struct thread *, struct target_aio_cancel_args *);
int	sys_aio_error(struct thread *, struct target_aio_error_args *);
int	sys___getcwd(struct thread *, struct target___getcwd_args *);
int	sys_sched_setparam(struct thread *, struct target_sched_setparam_args *);
int	sys_sched_getparam(struct thread *, struct target_sched_getparam_args *);
int	sys_sched_setscheduler(struct thread *, struct target_sched_setscheduler_args *);
int	sys_sched_rr_get_interval(struct thread *, struct target_sched_rr_get_interval_args *);
int	sys_utrace(struct thread *, struct target_utrace_args *);
int	sys_kldsym(struct thread *, struct target_kldsym_args *);
int	sys_jail(struct thread *, struct target_jail_args *);
int	sys_nnpfs_syscall(struct thread *, struct target_nnpfs_syscall_args *);
int	sys_sigprocmask(struct thread *, struct target_sigprocmask_args *);
int	sys_sigsuspend(struct thread *, struct target_sigsuspend_args *);
int	sys_sigpending(struct thread *, struct target_sigpending_args *);
int	sys_sigtimedwait(struct thread *, struct target_sigtimedwait_args *);
int	sys_sigwaitinfo(struct thread *, struct target_sigwaitinfo_args *);
int	sys___acl_get_file(struct thread *, struct target___acl_get_file_args *);
int	sys___acl_set_file(struct thread *, struct target___acl_set_file_args *);
int	sys___acl_get_fd(struct thread *, struct target___acl_get_fd_args *);
int	sys___acl_set_fd(struct thread *, struct target___acl_set_fd_args *);
int	sys___acl_delete_file(struct thread *, struct target___acl_delete_file_args *);
int	sys___acl_aclcheck_file(struct thread *, struct target___acl_aclcheck_file_args *);
int	sys___acl_aclcheck_fd(struct thread *, struct target___acl_aclcheck_fd_args *);
int	sys_extattrctl(struct thread *, struct target_extattrctl_args *);
int	sys_extattr_set_file(struct thread *, struct target_extattr_set_file_args *);
int	sys_extattr_get_file(struct thread *, struct target_extattr_get_file_args *);
int	sys_extattr_delete_file(struct thread *, struct target_extattr_delete_file_args *);
int	sys_aio_waitcomplete(struct thread *, struct target_aio_waitcomplete_args *);
int	sys_getresuid(struct thread *, struct target_getresuid_args *);
int	sys_getresgid(struct thread *, struct target_getresgid_args *);
int	sys_extattr_set_fd(struct thread *, struct target_extattr_set_fd_args *);
int	sys_extattr_get_fd(struct thread *, struct target_extattr_get_fd_args *);
int	sys_extattr_delete_fd(struct thread *, struct target_extattr_delete_fd_args *);
int	sys_eaccess(struct thread *, struct target_eaccess_args *);
int	sys_nmount(struct thread *, struct target_nmount_args *);
int	sys___mac_get_proc(struct thread *, struct target___mac_get_proc_args *);
int	sys___mac_set_proc(struct thread *, struct target___mac_set_proc_args *);
int	sys___mac_get_fd(struct thread *, struct target___mac_get_fd_args *);
int	sys___mac_get_file(struct thread *, struct target___mac_get_file_args *);
int	sys___mac_set_fd(struct thread *, struct target___mac_set_fd_args *);
int	sys___mac_set_file(struct thread *, struct target___mac_set_file_args *);
int	sys_kenv(struct thread *, struct target_kenv_args *);
int	sys_lchflags(struct thread *, struct target_lchflags_args *);
int	sys_uuidgen(struct thread *, struct target_uuidgen_args *);
int	sys_sendfile(struct thread *, struct target_sendfile_args *);
int	sys_mac_syscall(struct thread *, struct target_mac_syscall_args *);
int	sys_ksem_init(struct thread *, struct target_ksem_init_args *);
int	sys_ksem_open(struct thread *, struct target_ksem_open_args *);
int	sys_ksem_unlink(struct thread *, struct target_ksem_unlink_args *);
int	sys_ksem_getvalue(struct thread *, struct target_ksem_getvalue_args *);
int	sys___mac_get_pid(struct thread *, struct target___mac_get_pid_args *);
int	sys___mac_get_link(struct thread *, struct target___mac_get_link_args *);
int	sys___mac_set_link(struct thread *, struct target___mac_set_link_args *);
int	sys_extattr_set_link(struct thread *, struct target_extattr_set_link_args *);
int	sys_extattr_get_link(struct thread *, struct target_extattr_get_link_args *);
int	sys_extattr_delete_link(struct thread *, struct target_extattr_delete_link_args *);
int	sys___mac_execve(struct thread *, struct target___mac_execve_args *);
int	sys_sigaction(struct thread *, struct target_sigaction_args *);
int	sys_sigreturn(struct thread *, struct target_sigreturn_args *);
int	sys_getcontext(struct thread *, struct target_getcontext_args *);
int	sys_setcontext(struct thread *, struct target_setcontext_args *);
int	sys_swapcontext(struct thread *, struct target_swapcontext_args *);
int	sys_swapoff(struct thread *, struct target_swapoff_args *);
int	sys___acl_get_link(struct thread *, struct target___acl_get_link_args *);
int	sys___acl_set_link(struct thread *, struct target___acl_set_link_args *);
int	sys___acl_delete_link(struct thread *, struct target___acl_delete_link_args *);
int	sys___acl_aclcheck_link(struct thread *, struct target___acl_aclcheck_link_args *);
int	sys_sigwait(struct thread *, struct target_sigwait_args *);
int	sys_thr_create(struct thread *, struct target_thr_create_args *);
int	sys_thr_exit(struct thread *, struct target_thr_exit_args *);
int	sys_thr_self(struct thread *, struct target_thr_self_args *);
int	sys_extattr_list_fd(struct thread *, struct target_extattr_list_fd_args *);
int	sys_extattr_list_file(struct thread *, struct target_extattr_list_file_args *);
int	sys_extattr_list_link(struct thread *, struct target_extattr_list_link_args *);
int	sys_ksem_timedwait(struct thread *, struct target_ksem_timedwait_args *);
int	sys_thr_suspend(struct thread *, struct target_thr_suspend_args *);
int	sys_audit(struct thread *, struct target_audit_args *);
int	sys_auditon(struct thread *, struct target_auditon_args *);
int	sys_getauid(struct thread *, struct target_getauid_args *);
int	sys_setauid(struct thread *, struct target_setauid_args *);
int	sys_getaudit(struct thread *, struct target_getaudit_args *);
int	sys_setaudit(struct thread *, struct target_setaudit_args *);
int	sys_getaudit_addr(struct thread *, struct target_getaudit_addr_args *);
int	sys_setaudit_addr(struct thread *, struct target_setaudit_addr_args *);
int	sys_auditctl(struct thread *, struct target_auditctl_args *);
int	sys__umtx_op(struct thread *, struct target__umtx_op_args *);
int	sys_thr_new(struct thread *, struct target_thr_new_args *);
int	sys_sigqueue(struct thread *, struct target_sigqueue_args *);
int	sys_kmq_open(struct thread *, struct target_kmq_open_args *);
int	sys_kmq_setattr(struct thread *, struct target_kmq_setattr_args *);
int	sys_kmq_timedreceive(struct thread *, struct target_kmq_timedreceive_args *);
int	sys_kmq_timedsend(struct thread *, struct target_kmq_timedsend_args *);
int	sys_kmq_notify(struct thread *, struct target_kmq_notify_args *);
int	sys_kmq_unlink(struct thread *, struct target_kmq_unlink_args *);
int	sys_abort2(struct thread *, struct target_abort2_args *);
int	sys_thr_set_name(struct thread *, struct target_thr_set_name_args *);
int	sys_aio_fsync(struct thread *, struct target_aio_fsync_args *);
int	sys_rtprio_thread(struct thread *, struct target_rtprio_thread_args *);
int	sys_sctp_generic_sendmsg(struct thread *, struct target_sctp_generic_sendmsg_args *);
int	sys_sctp_generic_sendmsg_iov(struct thread *, struct target_sctp_generic_sendmsg_iov_args *);
int	sys_sctp_generic_recvmsg(struct thread *, struct target_sctp_generic_recvmsg_args *);
int	sys_pread(struct thread *, struct target_pread_args *);
int	sys_pwrite(struct thread *, struct target_pwrite_args *);
int	sys_mmap(struct thread *, struct target_mmap_args *);
int	sys_truncate(struct thread *, struct target_truncate_args *);
int	sys_shm_unlink(struct thread *, struct target_shm_unlink_args *);
int	sys_cpuset(struct thread *, struct target_cpuset_args *);
int	sys_cpuset_getid(struct thread *, struct target_cpuset_getid_args *);
int	sys_cpuset_getaffinity(struct thread *, struct target_cpuset_getaffinity_args *);
int	sys_cpuset_setaffinity(struct thread *, struct target_cpuset_setaffinity_args *);
int	sys_faccessat(struct thread *, struct target_faccessat_args *);
int	sys_fchmodat(struct thread *, struct target_fchmodat_args *);
int	sys_fchownat(struct thread *, struct target_fchownat_args *);
int	sys_fexecve(struct thread *, struct target_fexecve_args *);
int	sys_futimesat(struct thread *, struct target_futimesat_args *);
int	sys_linkat(struct thread *, struct target_linkat_args *);
int	sys_mkdirat(struct thread *, struct target_mkdirat_args *);
int	sys_mkfifoat(struct thread *, struct target_mkfifoat_args *);
int	sys_openat(struct thread *, struct target_openat_args *);
int	sys_readlinkat(struct thread *, struct target_readlinkat_args *);
int	sys_renameat(struct thread *, struct target_renameat_args *);
int	sys_symlinkat(struct thread *, struct target_symlinkat_args *);
int	sys_unlinkat(struct thread *, struct target_unlinkat_args *);
int	sys_gssd_syscall(struct thread *, struct target_gssd_syscall_args *);
int	sys_jail_get(struct thread *, struct target_jail_get_args *);
int	sys_jail_set(struct thread *, struct target_jail_set_args *);
int	sys___semctl(struct thread *, struct target___semctl_args *);
int	sys_msgctl(struct thread *, struct target_msgctl_args *);
int	sys_shmctl(struct thread *, struct target_shmctl_args *);
int	sys_lpathconf(struct thread *, struct target_lpathconf_args *);
int	sys___cap_rights_get(struct thread *, struct target___cap_rights_get_args *);
int	sys_cap_getmode(struct thread *, struct target_cap_getmode_args *);
int	sys_pdfork(struct thread *, struct target_pdfork_args *);
int	sys_pdgetpid(struct thread *, struct target_pdgetpid_args *);
int	sys_pselect(struct thread *, struct target_pselect_args *);
int	sys_getloginclass(struct thread *, struct target_getloginclass_args *);
int	sys_setloginclass(struct thread *, struct target_setloginclass_args *);
int	sys_rctl_get_racct(struct thread *, struct target_rctl_get_racct_args *);
int	sys_rctl_get_rules(struct thread *, struct target_rctl_get_rules_args *);
int	sys_rctl_get_limits(struct thread *, struct target_rctl_get_limits_args *);
int	sys_rctl_add_rule(struct thread *, struct target_rctl_add_rule_args *);
int	sys_rctl_remove_rule(struct thread *, struct target_rctl_remove_rule_args *);
int	sys_wait6(struct thread *, struct target_wait6_args *);
int	sys_cap_rights_limit(struct thread *, struct target_cap_rights_limit_args *);
int	sys_cap_ioctls_limit(struct thread *, struct target_cap_ioctls_limit_args *);
int	sys_cap_ioctls_get(struct thread *, struct target_cap_ioctls_get_args *);
int	sys_cap_fcntls_get(struct thread *, struct target_cap_fcntls_get_args *);
int	sys_bindat(struct thread *, struct target_bindat_args *);
int	sys_connectat(struct thread *, struct target_connectat_args *);
int	sys_chflagsat(struct thread *, struct target_chflagsat_args *);
int	sys_accept4(struct thread *, struct target_accept4_args *);
int	sys_pipe2(struct thread *, struct target_pipe2_args *);
int	sys_aio_mlock(struct thread *, struct target_aio_mlock_args *);
int	sys_procctl(struct thread *, struct target_procctl_args *);
int	sys_ppoll(struct thread *, struct target_ppoll_args *);
int	sys_futimens(struct thread *, struct target_futimens_args *);
int	sys_utimensat(struct thread *, struct target_utimensat_args *);
int	sys_fstat(struct thread *, struct target_fstat_args *);
int	sys_fstatat(struct thread *, struct target_fstatat_args *);
int	sys_fhstat(struct thread *, struct target_fhstat_args *);
int	sys_getdirentries(struct thread *, struct target_getdirentries_args *);
int	sys_statfs(struct thread *, struct target_statfs_args *);
int	sys_fstatfs(struct thread *, struct target_fstatfs_args *);
int	sys_getfsstat(struct thread *, struct target_getfsstat_args *);
int	sys_fhstatfs(struct thread *, struct target_fhstatfs_args *);
int	sys_mknodat(struct thread *, struct target_mknodat_args *);
int	sys_kevent(struct thread *, struct target_kevent_args *);
int	sys_cpuset_getdomain(struct thread *, struct target_cpuset_getdomain_args *);
int	sys_cpuset_setdomain(struct thread *, struct target_cpuset_setdomain_args *);
int	sys_getrandom(struct thread *, struct target_getrandom_args *);
int	sys_getfhat(struct thread *, struct target_getfhat_args *);
int	sys_fhlink(struct thread *, struct target_fhlink_args *);
int	sys_fhlinkat(struct thread *, struct target_fhlinkat_args *);
int	sys_fhreadlink(struct thread *, struct target_fhreadlink_args *);
int	sys_funlinkat(struct thread *, struct target_funlinkat_args *);
int	sys_copy_file_range(struct thread *, struct target_copy_file_range_args *);
int	sys___sysctlbyname(struct thread *, struct target___sysctlbyname_args *);
int	sys_shm_open2(struct thread *, struct target_shm_open2_args *);
int	sys_shm_rename(struct thread *, struct target_shm_rename_args *);
int	sys_sigfastblock(struct thread *, struct target_sigfastblock_args *);
int	sys___realpathat(struct thread *, struct target___realpathat_args *);
int	sys_rpctls_syscall(struct thread *, struct target_rpctls_syscall_args *);

#ifdef COMPAT_43

struct target_ocreat_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char mode_l_[PADL_(int)]; int mode; char mode_r_[PADR_(int)];
};
struct target_ostat_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char ub_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t ub; char ub_r_[PADR_(abi_uintptr_t)];
};
struct target_olstat_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char ub_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t ub; char ub_r_[PADR_(abi_uintptr_t)];
};
struct target_osigaction_args {
	char signum_l_[PADL_(int)]; int signum; char signum_r_[PADR_(int)];
	char nsa_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t nsa; char nsa_r_[PADR_(abi_uintptr_t)];
	char osa_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t osa; char osa_r_[PADR_(abi_uintptr_t)];
};
struct target_ofstat_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char sb_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t sb; char sb_r_[PADR_(abi_uintptr_t)];
};
struct target_ogetkerninfo_args {
	char op_l_[PADL_(int)]; int op; char op_r_[PADR_(int)];
	char where_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t where; char where_r_[PADR_(abi_uintptr_t)];
	char size_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t size; char size_r_[PADR_(abi_uintptr_t)];
	char arg_l_[PADL_(int)]; int arg; char arg_r_[PADR_(int)];
};
struct target_ommap_args {
	char addr_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t addr; char addr_r_[PADR_(abi_uintptr_t)];
	char len_l_[PADL_(int)]; int len; char len_r_[PADR_(int)];
	char prot_l_[PADL_(int)]; int prot; char prot_r_[PADR_(int)];
	char flags_l_[PADL_(int)]; int flags; char flags_r_[PADR_(int)];
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char pos_l_[PADL_(long)]; long pos; char pos_r_[PADR_(long)];
};
struct target_ogethostname_args {
	char hostname_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t hostname; char hostname_r_[PADR_(abi_uintptr_t)];
	char len_l_[PADL_(u_int)]; u_int len; char len_r_[PADR_(u_int)];
};
struct target_osethostname_args {
	char hostname_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t hostname; char hostname_r_[PADR_(abi_uintptr_t)];
	char len_l_[PADL_(u_int)]; u_int len; char len_r_[PADR_(u_int)];
};
struct target_oaccept_args {
	char s_l_[PADL_(int)]; int s; char s_r_[PADR_(int)];
	char name_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t name; char name_r_[PADR_(abi_uintptr_t)];
	char anamelen_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t anamelen; char anamelen_r_[PADR_(abi_uintptr_t)];
};
struct target_osend_args {
	char s_l_[PADL_(int)]; int s; char s_r_[PADR_(int)];
	char buf_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t buf; char buf_r_[PADR_(abi_uintptr_t)];
	char len_l_[PADL_(int)]; int len; char len_r_[PADR_(int)];
	char flags_l_[PADL_(int)]; int flags; char flags_r_[PADR_(int)];
};
struct target_orecv_args {
	char s_l_[PADL_(int)]; int s; char s_r_[PADR_(int)];
	char buf_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t buf; char buf_r_[PADR_(abi_uintptr_t)];
	char len_l_[PADL_(int)]; int len; char len_r_[PADR_(int)];
	char flags_l_[PADL_(int)]; int flags; char flags_r_[PADR_(int)];
};
struct target_osigreturn_args {
	char sigcntxp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t sigcntxp; char sigcntxp_r_[PADR_(abi_uintptr_t)];
};
struct target_osigvec_args {
	char signum_l_[PADL_(int)]; int signum; char signum_r_[PADR_(int)];
	char nsv_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t nsv; char nsv_r_[PADR_(abi_uintptr_t)];
	char osv_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t osv; char osv_r_[PADR_(abi_uintptr_t)];
};
struct target_osigstack_args {
	char nss_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t nss; char nss_r_[PADR_(abi_uintptr_t)];
	char oss_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t oss; char oss_r_[PADR_(abi_uintptr_t)];
};
struct target_orecvmsg_args {
	char s_l_[PADL_(int)]; int s; char s_r_[PADR_(int)];
	char msg_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t msg; char msg_r_[PADR_(abi_uintptr_t)];
	char flags_l_[PADL_(int)]; int flags; char flags_r_[PADR_(int)];
};
struct target_osendmsg_args {
	char s_l_[PADL_(int)]; int s; char s_r_[PADR_(int)];
	char msg_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t msg; char msg_r_[PADR_(abi_uintptr_t)];
	char flags_l_[PADL_(int)]; int flags; char flags_r_[PADR_(int)];
};
struct target_otruncate_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char length_l_[PADL_(long)]; long length; char length_r_[PADR_(long)];
};
struct target_ogetpeername_args {
	char fdes_l_[PADL_(int)]; int fdes; char fdes_r_[PADR_(int)];
	char asa_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t asa; char asa_r_[PADR_(abi_uintptr_t)];
	char alen_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t alen; char alen_r_[PADR_(abi_uintptr_t)];
};
struct target_ogetrlimit_args {
	char which_l_[PADL_(u_int)]; u_int which; char which_r_[PADR_(u_int)];
	char rlp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t rlp; char rlp_r_[PADR_(abi_uintptr_t)];
};
struct target_osetrlimit_args {
	char which_l_[PADL_(u_int)]; u_int which; char which_r_[PADR_(u_int)];
	char rlp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t rlp; char rlp_r_[PADR_(abi_uintptr_t)];
};
struct target_ogetdirentries_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char buf_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t buf; char buf_r_[PADR_(abi_uintptr_t)];
	char count_l_[PADL_(u_int)]; u_int count; char count_r_[PADR_(u_int)];
	char basep_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t basep; char basep_r_[PADR_(abi_uintptr_t)];
};
int	ocreat(struct thread *, struct target_ocreat_args *);
int	ostat(struct thread *, struct target_ostat_args *);
int	olstat(struct thread *, struct target_olstat_args *);
int	osigaction(struct thread *, struct target_osigaction_args *);
int	ofstat(struct thread *, struct target_ofstat_args *);
int	ogetkerninfo(struct thread *, struct target_ogetkerninfo_args *);
int	ommap(struct thread *, struct target_ommap_args *);
int	ogethostname(struct thread *, struct target_ogethostname_args *);
int	osethostname(struct thread *, struct target_osethostname_args *);
int	oaccept(struct thread *, struct target_oaccept_args *);
int	osend(struct thread *, struct target_osend_args *);
int	orecv(struct thread *, struct target_orecv_args *);
int	osigreturn(struct thread *, struct target_osigreturn_args *);
int	osigvec(struct thread *, struct target_osigvec_args *);
int	osigstack(struct thread *, struct target_osigstack_args *);
int	orecvmsg(struct thread *, struct target_orecvmsg_args *);
int	osendmsg(struct thread *, struct target_osendmsg_args *);
int	orecvfrom(struct thread *, struct target_recvfrom_args *);
int	otruncate(struct thread *, struct target_otruncate_args *);
int	ogetpeername(struct thread *, struct target_ogetpeername_args *);
int	ogetrlimit(struct thread *, struct target_ogetrlimit_args *);
int	osetrlimit(struct thread *, struct target_osetrlimit_args *);
int	ogetsockname(struct thread *, struct target_getsockname_args *);
int	ogetdirentries(struct thread *, struct target_ogetdirentries_args *);

#endif /* COMPAT_43 */


#ifdef COMPAT_FREEBSD4

struct target_freebsd4_getfsstat_args {
	char buf_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t buf; char buf_r_[PADR_(abi_uintptr_t)];
	char bufsize_l_[PADL_(long)]; long bufsize; char bufsize_r_[PADR_(long)];
	char mode_l_[PADL_(int)]; int mode; char mode_r_[PADR_(int)];
};
struct target_freebsd4_statfs_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char buf_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t buf; char buf_r_[PADR_(abi_uintptr_t)];
};
struct target_freebsd4_fstatfs_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char buf_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t buf; char buf_r_[PADR_(abi_uintptr_t)];
};
struct target_freebsd4_getdomainname_args {
	char domainname_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t domainname; char domainname_r_[PADR_(abi_uintptr_t)];
	char len_l_[PADL_(int)]; int len; char len_r_[PADR_(int)];
};
struct target_freebsd4_setdomainname_args {
	char domainname_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t domainname; char domainname_r_[PADR_(abi_uintptr_t)];
	char len_l_[PADL_(int)]; int len; char len_r_[PADR_(int)];
};
struct target_freebsd4_uname_args {
	char name_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t name; char name_r_[PADR_(abi_uintptr_t)];
};
struct target_freebsd4_fhstatfs_args {
	char u_fhp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t u_fhp; char u_fhp_r_[PADR_(abi_uintptr_t)];
	char buf_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t buf; char buf_r_[PADR_(abi_uintptr_t)];
};
struct target_freebsd4_sendfile_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char s_l_[PADL_(int)]; int s; char s_r_[PADR_(int)];
	char offset_l_[PADL_(off_t)]; off_t offset; char offset_r_[PADR_(off_t)];
	char nbytes_l_[PADL_(size_t)]; size_t nbytes; char nbytes_r_[PADR_(size_t)];
	char hdtr_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t hdtr; char hdtr_r_[PADR_(abi_uintptr_t)];
	char sbytes_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t sbytes; char sbytes_r_[PADR_(abi_uintptr_t)];
	char flags_l_[PADL_(int)]; int flags; char flags_r_[PADR_(int)];
};
struct target_freebsd4_sigaction_args {
	char sig_l_[PADL_(int)]; int sig; char sig_r_[PADR_(int)];
	char act_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t act; char act_r_[PADR_(abi_uintptr_t)];
	char oact_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t oact; char oact_r_[PADR_(abi_uintptr_t)];
};
struct target_freebsd4_sigreturn_args {
	char sigcntxp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t sigcntxp; char sigcntxp_r_[PADR_(abi_uintptr_t)];
};
int	freebsd4_getfsstat(struct thread *, struct target_freebsd4_getfsstat_args *);
int	freebsd4_statfs(struct thread *, struct target_freebsd4_statfs_args *);
int	freebsd4_fstatfs(struct thread *, struct target_freebsd4_fstatfs_args *);
int	freebsd4_getdomainname(struct thread *, struct target_freebsd4_getdomainname_args *);
int	freebsd4_setdomainname(struct thread *, struct target_freebsd4_setdomainname_args *);
int	freebsd4_uname(struct thread *, struct target_freebsd4_uname_args *);
int	freebsd4_fhstatfs(struct thread *, struct target_freebsd4_fhstatfs_args *);
int	freebsd4_sendfile(struct thread *, struct target_freebsd4_sendfile_args *);
int	freebsd4_sigaction(struct thread *, struct target_freebsd4_sigaction_args *);
int	freebsd4_sigreturn(struct thread *, struct target_freebsd4_sigreturn_args *);

#endif /* COMPAT_FREEBSD4 */


#ifdef COMPAT_FREEBSD6

struct target_freebsd6_pread_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char buf_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t buf; char buf_r_[PADR_(abi_uintptr_t)];
	char nbyte_l_[PADL_(size_t)]; size_t nbyte; char nbyte_r_[PADR_(size_t)];
	char pad_l_[PADL_(int)]; int pad; char pad_r_[PADR_(int)];
	char offset_l_[PADL_(off_t)]; off_t offset; char offset_r_[PADR_(off_t)];
};
struct target_freebsd6_pwrite_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char buf_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t buf; char buf_r_[PADR_(abi_uintptr_t)];
	char nbyte_l_[PADL_(size_t)]; size_t nbyte; char nbyte_r_[PADR_(size_t)];
	char pad_l_[PADL_(int)]; int pad; char pad_r_[PADR_(int)];
	char offset_l_[PADL_(off_t)]; off_t offset; char offset_r_[PADR_(off_t)];
};
struct target_freebsd6_mmap_args {
	char addr_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t addr; char addr_r_[PADR_(abi_uintptr_t)];
	char len_l_[PADL_(size_t)]; size_t len; char len_r_[PADR_(size_t)];
	char prot_l_[PADL_(int)]; int prot; char prot_r_[PADR_(int)];
	char flags_l_[PADL_(int)]; int flags; char flags_r_[PADR_(int)];
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char pad_l_[PADL_(int)]; int pad; char pad_r_[PADR_(int)];
	char pos_l_[PADL_(off_t)]; off_t pos; char pos_r_[PADR_(off_t)];
};
struct target_freebsd6_truncate_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char pad_l_[PADL_(int)]; int pad; char pad_r_[PADR_(int)];
	char length_l_[PADL_(off_t)]; off_t length; char length_r_[PADR_(off_t)];
};
struct target_freebsd6_aio_read_args {
	char aiocbp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t aiocbp; char aiocbp_r_[PADR_(abi_uintptr_t)];
};
struct target_freebsd6_aio_write_args {
	char aiocbp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t aiocbp; char aiocbp_r_[PADR_(abi_uintptr_t)];
};
struct target_freebsd6_lio_listio_args {
	char mode_l_[PADL_(int)]; int mode; char mode_r_[PADR_(int)];
	char acb_list_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t acb_list; char acb_list_r_[PADR_(abi_uintptr_t)];
	char nent_l_[PADL_(int)]; int nent; char nent_r_[PADR_(int)];
	char sig_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t sig; char sig_r_[PADR_(abi_uintptr_t)];
};
int	freebsd6_pread(struct thread *, struct target_freebsd6_pread_args *);
int	freebsd6_pwrite(struct thread *, struct target_freebsd6_pwrite_args *);
int	freebsd6_mmap(struct thread *, struct target_freebsd6_mmap_args *);
int	freebsd6_truncate(struct thread *, struct target_freebsd6_truncate_args *);
int	freebsd6_aio_read(struct thread *, struct target_freebsd6_aio_read_args *);
int	freebsd6_aio_write(struct thread *, struct target_freebsd6_aio_write_args *);
int	freebsd6_lio_listio(struct thread *, struct target_freebsd6_lio_listio_args *);

#endif /* COMPAT_FREEBSD6 */


#ifdef COMPAT_FREEBSD7

struct target_freebsd7___semctl_args {
	char semid_l_[PADL_(int)]; int semid; char semid_r_[PADR_(int)];
	char semnum_l_[PADL_(int)]; int semnum; char semnum_r_[PADR_(int)];
	char cmd_l_[PADL_(int)]; int cmd; char cmd_r_[PADR_(int)];
	char arg_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t arg; char arg_r_[PADR_(abi_uintptr_t)];
};
struct target_freebsd7_msgctl_args {
	char msqid_l_[PADL_(int)]; int msqid; char msqid_r_[PADR_(int)];
	char cmd_l_[PADL_(int)]; int cmd; char cmd_r_[PADR_(int)];
	char buf_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t buf; char buf_r_[PADR_(abi_uintptr_t)];
};
struct target_freebsd7_shmctl_args {
	char shmid_l_[PADL_(int)]; int shmid; char shmid_r_[PADR_(int)];
	char cmd_l_[PADL_(int)]; int cmd; char cmd_r_[PADR_(int)];
	char buf_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t buf; char buf_r_[PADR_(abi_uintptr_t)];
};
int	freebsd7___semctl(struct thread *, struct target_freebsd7___semctl_args *);
int	freebsd7_msgctl(struct thread *, struct target_freebsd7_msgctl_args *);
int	freebsd7_shmctl(struct thread *, struct target_freebsd7_shmctl_args *);

#endif /* COMPAT_FREEBSD7 */


#ifdef COMPAT_FREEBSD10


#endif /* COMPAT_FREEBSD10 */


#ifdef COMPAT_FREEBSD11

struct target_freebsd11_mknod_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char mode_l_[PADL_(int)]; int mode; char mode_r_[PADR_(int)];
	char dev_l_[PADL_(uint32_t)]; uint32_t dev; char dev_r_[PADR_(uint32_t)];
};
struct target_freebsd11_stat_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char ub_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t ub; char ub_r_[PADR_(abi_uintptr_t)];
};
struct target_freebsd11_fstat_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char sb_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t sb; char sb_r_[PADR_(abi_uintptr_t)];
};
struct target_freebsd11_lstat_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char ub_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t ub; char ub_r_[PADR_(abi_uintptr_t)];
};
struct target_freebsd11_getdirentries_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char buf_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t buf; char buf_r_[PADR_(abi_uintptr_t)];
	char count_l_[PADL_(u_int)]; u_int count; char count_r_[PADR_(u_int)];
	char basep_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t basep; char basep_r_[PADR_(abi_uintptr_t)];
};
struct target_freebsd11_getdents_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char buf_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t buf; char buf_r_[PADR_(abi_uintptr_t)];
	char count_l_[PADL_(size_t)]; size_t count; char count_r_[PADR_(size_t)];
};
struct target_freebsd11_nstat_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char ub_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t ub; char ub_r_[PADR_(abi_uintptr_t)];
};
struct target_freebsd11_nfstat_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char sb_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t sb; char sb_r_[PADR_(abi_uintptr_t)];
};
struct target_freebsd11_nlstat_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char ub_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t ub; char ub_r_[PADR_(abi_uintptr_t)];
};
struct target_freebsd11_fhstat_args {
	char u_fhp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t u_fhp; char u_fhp_r_[PADR_(abi_uintptr_t)];
	char sb_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t sb; char sb_r_[PADR_(abi_uintptr_t)];
};
struct target_freebsd11_kevent_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char changelist_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t changelist; char changelist_r_[PADR_(abi_uintptr_t)];
	char nchanges_l_[PADL_(int)]; int nchanges; char nchanges_r_[PADR_(int)];
	char eventlist_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t eventlist; char eventlist_r_[PADR_(abi_uintptr_t)];
	char nevents_l_[PADL_(int)]; int nevents; char nevents_r_[PADR_(int)];
	char timeout_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t timeout; char timeout_r_[PADR_(abi_uintptr_t)];
};
struct target_freebsd11_getfsstat_args {
	char buf_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t buf; char buf_r_[PADR_(abi_uintptr_t)];
	char bufsize_l_[PADL_(long)]; long bufsize; char bufsize_r_[PADR_(long)];
	char mode_l_[PADL_(int)]; int mode; char mode_r_[PADR_(int)];
};
struct target_freebsd11_statfs_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char buf_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t buf; char buf_r_[PADR_(abi_uintptr_t)];
};
struct target_freebsd11_fstatfs_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char buf_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t buf; char buf_r_[PADR_(abi_uintptr_t)];
};
struct target_freebsd11_fhstatfs_args {
	char u_fhp_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t u_fhp; char u_fhp_r_[PADR_(abi_uintptr_t)];
	char buf_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t buf; char buf_r_[PADR_(abi_uintptr_t)];
};
struct target_freebsd11_fstatat_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char buf_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t buf; char buf_r_[PADR_(abi_uintptr_t)];
	char flag_l_[PADL_(int)]; int flag; char flag_r_[PADR_(int)];
};
struct target_freebsd11_mknodat_args {
	char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char mode_l_[PADL_(mode_t)]; mode_t mode; char mode_r_[PADR_(mode_t)];
	char dev_l_[PADL_(uint32_t)]; uint32_t dev; char dev_r_[PADR_(uint32_t)];
};
int	freebsd11_mknod(struct thread *, struct target_freebsd11_mknod_args *);
int	freebsd11_stat(struct thread *, struct target_freebsd11_stat_args *);
int	freebsd11_fstat(struct thread *, struct target_freebsd11_fstat_args *);
int	freebsd11_lstat(struct thread *, struct target_freebsd11_lstat_args *);
int	freebsd11_getdirentries(struct thread *, struct target_freebsd11_getdirentries_args *);
int	freebsd11_getdents(struct thread *, struct target_freebsd11_getdents_args *);
int	freebsd11_nstat(struct thread *, struct target_freebsd11_nstat_args *);
int	freebsd11_nfstat(struct thread *, struct target_freebsd11_nfstat_args *);
int	freebsd11_nlstat(struct thread *, struct target_freebsd11_nlstat_args *);
int	freebsd11_fhstat(struct thread *, struct target_freebsd11_fhstat_args *);
int	freebsd11_kevent(struct thread *, struct target_freebsd11_kevent_args *);
int	freebsd11_getfsstat(struct thread *, struct target_freebsd11_getfsstat_args *);
int	freebsd11_statfs(struct thread *, struct target_freebsd11_statfs_args *);
int	freebsd11_fstatfs(struct thread *, struct target_freebsd11_fstatfs_args *);
int	freebsd11_fhstatfs(struct thread *, struct target_freebsd11_fhstatfs_args *);
int	freebsd11_fstatat(struct thread *, struct target_freebsd11_fstatat_args *);
int	freebsd11_mknodat(struct thread *, struct target_freebsd11_mknodat_args *);

#endif /* COMPAT_FREEBSD11 */


#ifdef COMPAT_FREEBSD12

struct target_freebsd12_shm_open_args {
	char path_l_[PADL_(abi_uintptr_t)]; abi_uintptr_t path; char path_r_[PADR_(abi_uintptr_t)];
	char flags_l_[PADL_(int)]; int flags; char flags_r_[PADR_(int)];
	char mode_l_[PADL_(mode_t)]; mode_t mode; char mode_r_[PADR_(mode_t)];
};
int	freebsd12_shm_open(struct thread *, struct target_freebsd12_shm_open_args *);

#endif /* COMPAT_FREEBSD12 */

#define	TARGET_FREEBSD_NR_AUE_read	AUE_READ
#define	TARGET_FREEBSD_NR_AUE_write	AUE_WRITE
#define	TARGET_FREEBSD_NR_AUE_open	AUE_OPEN_RWTC
#define	TARGET_FREEBSD_NR_AUE_wait4	AUE_WAIT4
#define	TARGET_FREEBSD_NR_AUE_ocreat	AUE_CREAT
#define	TARGET_FREEBSD_NR_AUE_link	AUE_LINK
#define	TARGET_FREEBSD_NR_AUE_unlink	AUE_UNLINK
#define	TARGET_FREEBSD_NR_AUE_chdir	AUE_CHDIR
#define	TARGET_FREEBSD_NR_AUE_freebsd11_mknod	AUE_MKNOD
#define	TARGET_FREEBSD_NR_AUE_chmod	AUE_CHMOD
#define	TARGET_FREEBSD_NR_AUE_chown	AUE_CHOWN
#define	TARGET_FREEBSD_NR_AUE_break	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_freebsd4_getfsstat	AUE_GETFSSTAT
#define	TARGET_FREEBSD_NR_AUE_mount	AUE_MOUNT
#define	TARGET_FREEBSD_NR_AUE_unmount	AUE_UMOUNT
#define	TARGET_FREEBSD_NR_AUE_ptrace	AUE_PTRACE
#define	TARGET_FREEBSD_NR_AUE_recvmsg	AUE_RECVMSG
#define	TARGET_FREEBSD_NR_AUE_sendmsg	AUE_SENDMSG
#define	TARGET_FREEBSD_NR_AUE_recvfrom	AUE_RECVFROM
#define	TARGET_FREEBSD_NR_AUE_accept	AUE_ACCEPT
#define	TARGET_FREEBSD_NR_AUE_getpeername	AUE_GETPEERNAME
#define	TARGET_FREEBSD_NR_AUE_getsockname	AUE_GETSOCKNAME
#define	TARGET_FREEBSD_NR_AUE_access	AUE_ACCESS
#define	TARGET_FREEBSD_NR_AUE_chflags	AUE_CHFLAGS
#define	TARGET_FREEBSD_NR_AUE_ostat	AUE_STAT
#define	TARGET_FREEBSD_NR_AUE_olstat	AUE_LSTAT
#define	TARGET_FREEBSD_NR_AUE_profil	AUE_PROFILE
#define	TARGET_FREEBSD_NR_AUE_ktrace	AUE_KTRACE
#define	TARGET_FREEBSD_NR_AUE_osigaction	AUE_SIGACTION
#define	TARGET_FREEBSD_NR_AUE_getlogin	AUE_GETLOGIN
#define	TARGET_FREEBSD_NR_AUE_setlogin	AUE_SETLOGIN
#define	TARGET_FREEBSD_NR_AUE_acct	AUE_ACCT
#define	TARGET_FREEBSD_NR_AUE_sigaltstack	AUE_SIGALTSTACK
#define	TARGET_FREEBSD_NR_AUE_ioctl	AUE_IOCTL
#define	TARGET_FREEBSD_NR_AUE_revoke	AUE_REVOKE
#define	TARGET_FREEBSD_NR_AUE_symlink	AUE_SYMLINK
#define	TARGET_FREEBSD_NR_AUE_readlink	AUE_READLINK
#define	TARGET_FREEBSD_NR_AUE_execve	AUE_EXECVE
#define	TARGET_FREEBSD_NR_AUE_chroot	AUE_CHROOT
#define	TARGET_FREEBSD_NR_AUE_ofstat	AUE_FSTAT
#define	TARGET_FREEBSD_NR_AUE_ogetkerninfo	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_msync	AUE_MSYNC
#define	TARGET_FREEBSD_NR_AUE_ommap	AUE_MMAP
#define	TARGET_FREEBSD_NR_AUE_munmap	AUE_MUNMAP
#define	TARGET_FREEBSD_NR_AUE_mprotect	AUE_MPROTECT
#define	TARGET_FREEBSD_NR_AUE_madvise	AUE_MADVISE
#define	TARGET_FREEBSD_NR_AUE_mincore	AUE_MINCORE
#define	TARGET_FREEBSD_NR_AUE_getgroups	AUE_GETGROUPS
#define	TARGET_FREEBSD_NR_AUE_setgroups	AUE_SETGROUPS
#define	TARGET_FREEBSD_NR_AUE_setitimer	AUE_SETITIMER
#define	TARGET_FREEBSD_NR_AUE_swapon	AUE_SWAPON
#define	TARGET_FREEBSD_NR_AUE_getitimer	AUE_GETITIMER
#define	TARGET_FREEBSD_NR_AUE_ogethostname	AUE_SYSCTL
#define	TARGET_FREEBSD_NR_AUE_osethostname	AUE_SYSCTL
#define	TARGET_FREEBSD_NR_AUE_fcntl	AUE_FCNTL
#define	TARGET_FREEBSD_NR_AUE_select	AUE_SELECT
#define	TARGET_FREEBSD_NR_AUE_connect	AUE_CONNECT
#define	TARGET_FREEBSD_NR_AUE_oaccept	AUE_ACCEPT
#define	TARGET_FREEBSD_NR_AUE_osend	AUE_SEND
#define	TARGET_FREEBSD_NR_AUE_orecv	AUE_RECV
#define	TARGET_FREEBSD_NR_AUE_osigreturn	AUE_SIGRETURN
#define	TARGET_FREEBSD_NR_AUE_bind	AUE_BIND
#define	TARGET_FREEBSD_NR_AUE_setsockopt	AUE_SETSOCKOPT
#define	TARGET_FREEBSD_NR_AUE_osigvec	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_osigstack	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_orecvmsg	AUE_RECVMSG
#define	TARGET_FREEBSD_NR_AUE_osendmsg	AUE_SENDMSG
#define	TARGET_FREEBSD_NR_AUE_gettimeofday	AUE_GETTIMEOFDAY
#define	TARGET_FREEBSD_NR_AUE_getrusage	AUE_GETRUSAGE
#define	TARGET_FREEBSD_NR_AUE_getsockopt	AUE_GETSOCKOPT
#define	TARGET_FREEBSD_NR_AUE_readv	AUE_READV
#define	TARGET_FREEBSD_NR_AUE_writev	AUE_WRITEV
#define	TARGET_FREEBSD_NR_AUE_settimeofday	AUE_SETTIMEOFDAY
#define	TARGET_FREEBSD_NR_AUE_orecvfrom	AUE_RECVFROM
#define	TARGET_FREEBSD_NR_AUE_rename	AUE_RENAME
#define	TARGET_FREEBSD_NR_AUE_otruncate	AUE_TRUNCATE
#define	TARGET_FREEBSD_NR_AUE_mkfifo	AUE_MKFIFO
#define	TARGET_FREEBSD_NR_AUE_sendto	AUE_SENDTO
#define	TARGET_FREEBSD_NR_AUE_socketpair	AUE_SOCKETPAIR
#define	TARGET_FREEBSD_NR_AUE_mkdir	AUE_MKDIR
#define	TARGET_FREEBSD_NR_AUE_rmdir	AUE_RMDIR
#define	TARGET_FREEBSD_NR_AUE_utimes	AUE_UTIMES
#define	TARGET_FREEBSD_NR_AUE_adjtime	AUE_ADJTIME
#define	TARGET_FREEBSD_NR_AUE_ogetpeername	AUE_GETPEERNAME
#define	TARGET_FREEBSD_NR_AUE_ogetrlimit	AUE_GETRLIMIT
#define	TARGET_FREEBSD_NR_AUE_osetrlimit	AUE_SETRLIMIT
#define	TARGET_FREEBSD_NR_AUE_quotactl	AUE_QUOTACTL
#define	TARGET_FREEBSD_NR_AUE_ogetsockname	AUE_GETSOCKNAME
#define	TARGET_FREEBSD_NR_AUE_nlm_syscall	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_nfssvc	AUE_NFS_SVC
#define	TARGET_FREEBSD_NR_AUE_ogetdirentries	AUE_GETDIRENTRIES
#define	TARGET_FREEBSD_NR_AUE_freebsd4_statfs	AUE_STATFS
#define	TARGET_FREEBSD_NR_AUE_freebsd4_fstatfs	AUE_FSTATFS
#define	TARGET_FREEBSD_NR_AUE_lgetfh	AUE_LGETFH
#define	TARGET_FREEBSD_NR_AUE_getfh	AUE_NFS_GETFH
#define	TARGET_FREEBSD_NR_AUE_freebsd4_getdomainname	AUE_SYSCTL
#define	TARGET_FREEBSD_NR_AUE_freebsd4_setdomainname	AUE_SYSCTL
#define	TARGET_FREEBSD_NR_AUE_freebsd4_uname	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_sysarch	AUE_SYSARCH
#define	TARGET_FREEBSD_NR_AUE_rtprio	AUE_RTPRIO
#define	TARGET_FREEBSD_NR_AUE_semsys	AUE_SEMSYS
#define	TARGET_FREEBSD_NR_AUE_msgsys	AUE_MSGSYS
#define	TARGET_FREEBSD_NR_AUE_shmsys	AUE_SHMSYS
#define	TARGET_FREEBSD_NR_AUE_freebsd6_pread	AUE_PREAD
#define	TARGET_FREEBSD_NR_AUE_freebsd6_pwrite	AUE_PWRITE
#define	TARGET_FREEBSD_NR_AUE_ntp_adjtime	AUE_NTP_ADJTIME
#define	TARGET_FREEBSD_NR_AUE_freebsd11_stat	AUE_STAT
#define	TARGET_FREEBSD_NR_AUE_freebsd11_fstat	AUE_FSTAT
#define	TARGET_FREEBSD_NR_AUE_freebsd11_lstat	AUE_LSTAT
#define	TARGET_FREEBSD_NR_AUE_pathconf	AUE_PATHCONF
#define	TARGET_FREEBSD_NR_AUE_getrlimit	AUE_GETRLIMIT
#define	TARGET_FREEBSD_NR_AUE_setrlimit	AUE_SETRLIMIT
#define	TARGET_FREEBSD_NR_AUE_freebsd11_getdirentries	AUE_GETDIRENTRIES
#define	TARGET_FREEBSD_NR_AUE_freebsd6_mmap	AUE_MMAP
#define	TARGET_FREEBSD_NR_AUE_freebsd6_truncate	AUE_TRUNCATE
#define	TARGET_FREEBSD_NR_AUE___sysctl	AUE_SYSCTL
#define	TARGET_FREEBSD_NR_AUE_mlock	AUE_MLOCK
#define	TARGET_FREEBSD_NR_AUE_munlock	AUE_MUNLOCK
#define	TARGET_FREEBSD_NR_AUE_undelete	AUE_UNDELETE
#define	TARGET_FREEBSD_NR_AUE_futimes	AUE_FUTIMES
#define	TARGET_FREEBSD_NR_AUE_poll	AUE_POLL
#define	TARGET_FREEBSD_NR_AUE_freebsd7___semctl	AUE_SEMCTL
#define	TARGET_FREEBSD_NR_AUE_semop	AUE_SEMOP
#define	TARGET_FREEBSD_NR_AUE_freebsd7_msgctl	AUE_MSGCTL
#define	TARGET_FREEBSD_NR_AUE_msgsnd	AUE_MSGSND
#define	TARGET_FREEBSD_NR_AUE_msgrcv	AUE_MSGRCV
#define	TARGET_FREEBSD_NR_AUE_shmat	AUE_SHMAT
#define	TARGET_FREEBSD_NR_AUE_freebsd7_shmctl	AUE_SHMCTL
#define	TARGET_FREEBSD_NR_AUE_shmdt	AUE_SHMDT
#define	TARGET_FREEBSD_NR_AUE_clock_gettime	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_clock_settime	AUE_CLOCK_SETTIME
#define	TARGET_FREEBSD_NR_AUE_clock_getres	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_ktimer_create	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_ktimer_settime	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_ktimer_gettime	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_nanosleep	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_ffclock_getcounter	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_ffclock_setestimate	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_ffclock_getestimate	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_clock_nanosleep	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_clock_getcpuclockid2	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_ntp_gettime	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_minherit	AUE_MINHERIT
#define	TARGET_FREEBSD_NR_AUE_lchown	AUE_LCHOWN
#define	TARGET_FREEBSD_NR_AUE_aio_read	AUE_AIO_READ
#define	TARGET_FREEBSD_NR_AUE_aio_write	AUE_AIO_WRITE
#define	TARGET_FREEBSD_NR_AUE_lio_listio	AUE_LIO_LISTIO
#define	TARGET_FREEBSD_NR_AUE_kbounce	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_flag_captured	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_freebsd11_getdents	AUE_O_GETDENTS
#define	TARGET_FREEBSD_NR_AUE_lchmod	AUE_LCHMOD
#define	TARGET_FREEBSD_NR_AUE_lutimes	AUE_LUTIMES
#define	TARGET_FREEBSD_NR_AUE_freebsd11_nstat	AUE_STAT
#define	TARGET_FREEBSD_NR_AUE_freebsd11_nfstat	AUE_FSTAT
#define	TARGET_FREEBSD_NR_AUE_freebsd11_nlstat	AUE_LSTAT
#define	TARGET_FREEBSD_NR_AUE_preadv	AUE_PREADV
#define	TARGET_FREEBSD_NR_AUE_pwritev	AUE_PWRITEV
#define	TARGET_FREEBSD_NR_AUE_freebsd4_fhstatfs	AUE_FHSTATFS
#define	TARGET_FREEBSD_NR_AUE_fhopen	AUE_FHOPEN
#define	TARGET_FREEBSD_NR_AUE_freebsd11_fhstat	AUE_FHSTAT
#define	TARGET_FREEBSD_NR_AUE_modstat	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_modfind	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_kldload	AUE_MODLOAD
#define	TARGET_FREEBSD_NR_AUE_kldfind	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_kldstat	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_aio_return	AUE_AIO_RETURN
#define	TARGET_FREEBSD_NR_AUE_aio_suspend	AUE_AIO_SUSPEND
#define	TARGET_FREEBSD_NR_AUE_aio_cancel	AUE_AIO_CANCEL
#define	TARGET_FREEBSD_NR_AUE_aio_error	AUE_AIO_ERROR
#define	TARGET_FREEBSD_NR_AUE_freebsd6_aio_read	AUE_AIO_READ
#define	TARGET_FREEBSD_NR_AUE_freebsd6_aio_write	AUE_AIO_WRITE
#define	TARGET_FREEBSD_NR_AUE_freebsd6_lio_listio	AUE_LIO_LISTIO
#define	TARGET_FREEBSD_NR_AUE___getcwd	AUE_GETCWD
#define	TARGET_FREEBSD_NR_AUE_sched_setparam	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_sched_getparam	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_sched_setscheduler	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_sched_rr_get_interval	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_utrace	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_freebsd4_sendfile	AUE_SENDFILE
#define	TARGET_FREEBSD_NR_AUE_kldsym	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_jail	AUE_JAIL
#define	TARGET_FREEBSD_NR_AUE_nnpfs_syscall	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_sigprocmask	AUE_SIGPROCMASK
#define	TARGET_FREEBSD_NR_AUE_sigsuspend	AUE_SIGSUSPEND
#define	TARGET_FREEBSD_NR_AUE_freebsd4_sigaction	AUE_SIGACTION
#define	TARGET_FREEBSD_NR_AUE_sigpending	AUE_SIGPENDING
#define	TARGET_FREEBSD_NR_AUE_freebsd4_sigreturn	AUE_SIGRETURN
#define	TARGET_FREEBSD_NR_AUE_sigtimedwait	AUE_SIGWAIT
#define	TARGET_FREEBSD_NR_AUE_sigwaitinfo	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE___acl_get_file	AUE_ACL_GET_FILE
#define	TARGET_FREEBSD_NR_AUE___acl_set_file	AUE_ACL_SET_FILE
#define	TARGET_FREEBSD_NR_AUE___acl_get_fd	AUE_ACL_GET_FD
#define	TARGET_FREEBSD_NR_AUE___acl_set_fd	AUE_ACL_SET_FD
#define	TARGET_FREEBSD_NR_AUE___acl_delete_file	AUE_ACL_DELETE_FILE
#define	TARGET_FREEBSD_NR_AUE___acl_aclcheck_file	AUE_ACL_CHECK_FILE
#define	TARGET_FREEBSD_NR_AUE___acl_aclcheck_fd	AUE_ACL_CHECK_FD
#define	TARGET_FREEBSD_NR_AUE_extattrctl	AUE_EXTATTRCTL
#define	TARGET_FREEBSD_NR_AUE_extattr_set_file	AUE_EXTATTR_SET_FILE
#define	TARGET_FREEBSD_NR_AUE_extattr_get_file	AUE_EXTATTR_GET_FILE
#define	TARGET_FREEBSD_NR_AUE_extattr_delete_file	AUE_EXTATTR_DELETE_FILE
#define	TARGET_FREEBSD_NR_AUE_aio_waitcomplete	AUE_AIO_WAITCOMPLETE
#define	TARGET_FREEBSD_NR_AUE_getresuid	AUE_GETRESUID
#define	TARGET_FREEBSD_NR_AUE_getresgid	AUE_GETRESGID
#define	TARGET_FREEBSD_NR_AUE_freebsd11_kevent	AUE_KEVENT
#define	TARGET_FREEBSD_NR_AUE_extattr_set_fd	AUE_EXTATTR_SET_FD
#define	TARGET_FREEBSD_NR_AUE_extattr_get_fd	AUE_EXTATTR_GET_FD
#define	TARGET_FREEBSD_NR_AUE_extattr_delete_fd	AUE_EXTATTR_DELETE_FD
#define	TARGET_FREEBSD_NR_AUE_eaccess	AUE_EACCESS
#define	TARGET_FREEBSD_NR_AUE_nmount	AUE_NMOUNT
#define	TARGET_FREEBSD_NR_AUE___mac_get_proc	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE___mac_set_proc	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE___mac_get_fd	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE___mac_get_file	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE___mac_set_fd	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE___mac_set_file	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_kenv	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_lchflags	AUE_LCHFLAGS
#define	TARGET_FREEBSD_NR_AUE_uuidgen	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_sendfile	AUE_SENDFILE
#define	TARGET_FREEBSD_NR_AUE_mac_syscall	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_freebsd11_getfsstat	AUE_GETFSSTAT
#define	TARGET_FREEBSD_NR_AUE_freebsd11_statfs	AUE_STATFS
#define	TARGET_FREEBSD_NR_AUE_freebsd11_fstatfs	AUE_FSTATFS
#define	TARGET_FREEBSD_NR_AUE_freebsd11_fhstatfs	AUE_FHSTATFS
#define	TARGET_FREEBSD_NR_AUE_ksem_init	AUE_SEMINIT
#define	TARGET_FREEBSD_NR_AUE_ksem_open	AUE_SEMOPEN
#define	TARGET_FREEBSD_NR_AUE_ksem_unlink	AUE_SEMUNLINK
#define	TARGET_FREEBSD_NR_AUE_ksem_getvalue	AUE_SEMGETVALUE
#define	TARGET_FREEBSD_NR_AUE___mac_get_pid	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE___mac_get_link	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE___mac_set_link	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_extattr_set_link	AUE_EXTATTR_SET_LINK
#define	TARGET_FREEBSD_NR_AUE_extattr_get_link	AUE_EXTATTR_GET_LINK
#define	TARGET_FREEBSD_NR_AUE_extattr_delete_link	AUE_EXTATTR_DELETE_LINK
#define	TARGET_FREEBSD_NR_AUE___mac_execve	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_sigaction	AUE_SIGACTION
#define	TARGET_FREEBSD_NR_AUE_sigreturn	AUE_SIGRETURN
#define	TARGET_FREEBSD_NR_AUE_getcontext	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_setcontext	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_swapcontext	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_swapoff	AUE_SWAPOFF
#define	TARGET_FREEBSD_NR_AUE___acl_get_link	AUE_ACL_GET_LINK
#define	TARGET_FREEBSD_NR_AUE___acl_set_link	AUE_ACL_SET_LINK
#define	TARGET_FREEBSD_NR_AUE___acl_delete_link	AUE_ACL_DELETE_LINK
#define	TARGET_FREEBSD_NR_AUE___acl_aclcheck_link	AUE_ACL_CHECK_LINK
#define	TARGET_FREEBSD_NR_AUE_sigwait	AUE_SIGWAIT
#define	TARGET_FREEBSD_NR_AUE_thr_create	AUE_THR_CREATE
#define	TARGET_FREEBSD_NR_AUE_thr_exit	AUE_THR_EXIT
#define	TARGET_FREEBSD_NR_AUE_thr_self	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_extattr_list_fd	AUE_EXTATTR_LIST_FD
#define	TARGET_FREEBSD_NR_AUE_extattr_list_file	AUE_EXTATTR_LIST_FILE
#define	TARGET_FREEBSD_NR_AUE_extattr_list_link	AUE_EXTATTR_LIST_LINK
#define	TARGET_FREEBSD_NR_AUE_ksem_timedwait	AUE_SEMWAIT
#define	TARGET_FREEBSD_NR_AUE_thr_suspend	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_audit	AUE_AUDIT
#define	TARGET_FREEBSD_NR_AUE_auditon	AUE_AUDITON
#define	TARGET_FREEBSD_NR_AUE_getauid	AUE_GETAUID
#define	TARGET_FREEBSD_NR_AUE_setauid	AUE_SETAUID
#define	TARGET_FREEBSD_NR_AUE_getaudit	AUE_GETAUDIT
#define	TARGET_FREEBSD_NR_AUE_setaudit	AUE_SETAUDIT
#define	TARGET_FREEBSD_NR_AUE_getaudit_addr	AUE_GETAUDIT_ADDR
#define	TARGET_FREEBSD_NR_AUE_setaudit_addr	AUE_SETAUDIT_ADDR
#define	TARGET_FREEBSD_NR_AUE_auditctl	AUE_AUDITCTL
#define	TARGET_FREEBSD_NR_AUE__umtx_op	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_thr_new	AUE_THR_NEW
#define	TARGET_FREEBSD_NR_AUE_sigqueue	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_kmq_open	AUE_MQ_OPEN
#define	TARGET_FREEBSD_NR_AUE_kmq_setattr	AUE_MQ_SETATTR
#define	TARGET_FREEBSD_NR_AUE_kmq_timedreceive	AUE_MQ_TIMEDRECEIVE
#define	TARGET_FREEBSD_NR_AUE_kmq_timedsend	AUE_MQ_TIMEDSEND
#define	TARGET_FREEBSD_NR_AUE_kmq_notify	AUE_MQ_NOTIFY
#define	TARGET_FREEBSD_NR_AUE_kmq_unlink	AUE_MQ_UNLINK
#define	TARGET_FREEBSD_NR_AUE_abort2	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_thr_set_name	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_aio_fsync	AUE_AIO_FSYNC
#define	TARGET_FREEBSD_NR_AUE_rtprio_thread	AUE_RTPRIO
#define	TARGET_FREEBSD_NR_AUE_sctp_generic_sendmsg	AUE_SCTP_GENERIC_SENDMSG
#define	TARGET_FREEBSD_NR_AUE_sctp_generic_sendmsg_iov	AUE_SCTP_GENERIC_SENDMSG_IOV
#define	TARGET_FREEBSD_NR_AUE_sctp_generic_recvmsg	AUE_SCTP_GENERIC_RECVMSG
#define	TARGET_FREEBSD_NR_AUE_pread	AUE_PREAD
#define	TARGET_FREEBSD_NR_AUE_pwrite	AUE_PWRITE
#define	TARGET_FREEBSD_NR_AUE_mmap	AUE_MMAP
#define	TARGET_FREEBSD_NR_AUE_truncate	AUE_TRUNCATE
#define	TARGET_FREEBSD_NR_AUE_freebsd12_shm_open	AUE_SHMOPEN
#define	TARGET_FREEBSD_NR_AUE_shm_unlink	AUE_SHMUNLINK
#define	TARGET_FREEBSD_NR_AUE_cpuset	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_cpuset_getid	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_cpuset_getaffinity	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_cpuset_setaffinity	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_faccessat	AUE_FACCESSAT
#define	TARGET_FREEBSD_NR_AUE_fchmodat	AUE_FCHMODAT
#define	TARGET_FREEBSD_NR_AUE_fchownat	AUE_FCHOWNAT
#define	TARGET_FREEBSD_NR_AUE_fexecve	AUE_FEXECVE
#define	TARGET_FREEBSD_NR_AUE_freebsd11_fstatat	AUE_FSTATAT
#define	TARGET_FREEBSD_NR_AUE_futimesat	AUE_FUTIMESAT
#define	TARGET_FREEBSD_NR_AUE_linkat	AUE_LINKAT
#define	TARGET_FREEBSD_NR_AUE_mkdirat	AUE_MKDIRAT
#define	TARGET_FREEBSD_NR_AUE_mkfifoat	AUE_MKFIFOAT
#define	TARGET_FREEBSD_NR_AUE_freebsd11_mknodat	AUE_MKNODAT
#define	TARGET_FREEBSD_NR_AUE_openat	AUE_OPENAT_RWTC
#define	TARGET_FREEBSD_NR_AUE_readlinkat	AUE_READLINKAT
#define	TARGET_FREEBSD_NR_AUE_renameat	AUE_RENAMEAT
#define	TARGET_FREEBSD_NR_AUE_symlinkat	AUE_SYMLINKAT
#define	TARGET_FREEBSD_NR_AUE_unlinkat	AUE_UNLINKAT
#define	TARGET_FREEBSD_NR_AUE_gssd_syscall	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_jail_get	AUE_JAIL_GET
#define	TARGET_FREEBSD_NR_AUE_jail_set	AUE_JAIL_SET
#define	TARGET_FREEBSD_NR_AUE___semctl	AUE_SEMCTL
#define	TARGET_FREEBSD_NR_AUE_msgctl	AUE_MSGCTL
#define	TARGET_FREEBSD_NR_AUE_shmctl	AUE_SHMCTL
#define	TARGET_FREEBSD_NR_AUE_lpathconf	AUE_LPATHCONF
#define	TARGET_FREEBSD_NR_AUE___cap_rights_get	AUE_CAP_RIGHTS_GET
#define	TARGET_FREEBSD_NR_AUE_cap_getmode	AUE_CAP_GETMODE
#define	TARGET_FREEBSD_NR_AUE_pdfork	AUE_PDFORK
#define	TARGET_FREEBSD_NR_AUE_pdgetpid	AUE_PDGETPID
#define	TARGET_FREEBSD_NR_AUE_pselect	AUE_SELECT
#define	TARGET_FREEBSD_NR_AUE_getloginclass	AUE_GETLOGINCLASS
#define	TARGET_FREEBSD_NR_AUE_setloginclass	AUE_SETLOGINCLASS
#define	TARGET_FREEBSD_NR_AUE_rctl_get_racct	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_rctl_get_rules	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_rctl_get_limits	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_rctl_add_rule	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_rctl_remove_rule	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_wait6	AUE_WAIT6
#define	TARGET_FREEBSD_NR_AUE_cap_rights_limit	AUE_CAP_RIGHTS_LIMIT
#define	TARGET_FREEBSD_NR_AUE_cap_ioctls_limit	AUE_CAP_IOCTLS_LIMIT
#define	TARGET_FREEBSD_NR_AUE_cap_ioctls_get	AUE_CAP_IOCTLS_GET
#define	TARGET_FREEBSD_NR_AUE_cap_fcntls_get	AUE_CAP_FCNTLS_GET
#define	TARGET_FREEBSD_NR_AUE_bindat	AUE_BINDAT
#define	TARGET_FREEBSD_NR_AUE_connectat	AUE_CONNECTAT
#define	TARGET_FREEBSD_NR_AUE_chflagsat	AUE_CHFLAGSAT
#define	TARGET_FREEBSD_NR_AUE_accept4	AUE_ACCEPT
#define	TARGET_FREEBSD_NR_AUE_pipe2	AUE_PIPE
#define	TARGET_FREEBSD_NR_AUE_aio_mlock	AUE_AIO_MLOCK
#define	TARGET_FREEBSD_NR_AUE_procctl	AUE_PROCCTL
#define	TARGET_FREEBSD_NR_AUE_ppoll	AUE_POLL
#define	TARGET_FREEBSD_NR_AUE_futimens	AUE_FUTIMES
#define	TARGET_FREEBSD_NR_AUE_utimensat	AUE_FUTIMESAT
#define	TARGET_FREEBSD_NR_AUE_fstat	AUE_FSTAT
#define	TARGET_FREEBSD_NR_AUE_fstatat	AUE_FSTATAT
#define	TARGET_FREEBSD_NR_AUE_fhstat	AUE_FHSTAT
#define	TARGET_FREEBSD_NR_AUE_getdirentries	AUE_GETDIRENTRIES
#define	TARGET_FREEBSD_NR_AUE_statfs	AUE_STATFS
#define	TARGET_FREEBSD_NR_AUE_fstatfs	AUE_FSTATFS
#define	TARGET_FREEBSD_NR_AUE_getfsstat	AUE_GETFSSTAT
#define	TARGET_FREEBSD_NR_AUE_fhstatfs	AUE_FHSTATFS
#define	TARGET_FREEBSD_NR_AUE_mknodat	AUE_MKNODAT
#define	TARGET_FREEBSD_NR_AUE_kevent	AUE_KEVENT
#define	TARGET_FREEBSD_NR_AUE_cpuset_getdomain	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_cpuset_setdomain	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_getrandom	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_getfhat	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_fhlink	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_fhlinkat	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_fhreadlink	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE_funlinkat	AUE_UNLINKAT
#define	TARGET_FREEBSD_NR_AUE_copy_file_range	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE___sysctlbyname	AUE_SYSCTL
#define	TARGET_FREEBSD_NR_AUE_shm_open2	AUE_SHMOPEN
#define	TARGET_FREEBSD_NR_AUE_shm_rename	AUE_SHMRENAME
#define	TARGET_FREEBSD_NR_AUE_sigfastblock	AUE_NULL
#define	TARGET_FREEBSD_NR_AUE___realpathat	AUE_REALPATHAT
#define	TARGET_FREEBSD_NR_AUE_rpctls_syscall	AUE_NULL

#undef PAD_
#undef PADL_
#undef PADR_

#endif /* !_TARGET_OS_PROTO_H_ */
