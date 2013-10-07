/*
 *  FreeBSD sysctl() and sysarch() system call emulation
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

#include <sys/types.h>
#include <sys/param.h>
#include <sys/mount.h>
#include <sys/sysctl.h>
#include <string.h>

#include "qemu.h"

#include "target_arch_sysarch.h"
#include "target_os_vmparam.h"

/*
 * XXX The following should maybe go some place else.  Also, see the note
 * about using "thunk" for sysctl's that pass data using structures.
 */
/* From sys/mount.h: */
#define TARGET_MFSNAMELEN	16	/* length of type name including null */
struct target_xvfsconf {
	abi_ulong vfc_vfsops;		/* filesystem op vector - not used */
	char vfc_name[TARGET_MFSNAMELEN];	/* filesystem type name */
	int32_t  vfc_typenum;		/* historic fs type number */
	int32_t  vfc_refcount;		/* number mounted of this type */
	int32_t  vfc_flags;		/* permanent flags */
	abi_ulong vfc_next;		/* next int list - not used */
};

/* vfc_flag definitions */
#define TARGET_VFCF_STATIC     0x00010000  /* statically compiled into kernel */
#define TARGET_VFCF_NETWORK    0x00020000  /* may get data over the network */
#define TARGET_VFCF_READONLY   0x00040000  /* writes are not implemented */
#define TARGET_VFCF_SYNTHETIC  0x00080000  /* doesn't represent real files */
#define TARGET_VFCF_LOOPBACK   0x00100000  /* aliases some other mounted FS */
#define TARGET_VFCF_UNICODE    0x00200000  /* stores file names as Unicode */
#define TARGET_VFCF_JAIL       0x00400000  /* can be mounted within a jail */
#define TARGET_VFCF_DELEGADMIN 0x00800000  /* supports delegated admin */
#define TARGET_VFCF_SBDRY      0x01000000  /* defer stop requests */

static int
host_to_target_vfc_flags(int flags)
{
    int ret = 0;

    if (flags & VFCF_STATIC)
	ret |= TARGET_VFCF_STATIC;
    if (flags & VFCF_NETWORK)
	ret |= TARGET_VFCF_NETWORK;
    if (flags & VFCF_READONLY)
	ret |= TARGET_VFCF_READONLY;
    if (flags & VFCF_SYNTHETIC)
	ret |= TARGET_VFCF_SYNTHETIC;
    if (flags & VFCF_LOOPBACK)
	ret |= TARGET_VFCF_LOOPBACK;
    if (flags & VFCF_UNICODE)
	ret |= TARGET_VFCF_UNICODE;
    if (flags & VFCF_JAIL)
	ret |= TARGET_VFCF_JAIL;
    if (flags & VFCF_DELEGADMIN)
	ret |= TARGET_VFCF_DELEGADMIN;
#ifdef VFCF_SBRDY
    if (flags & VFCF_SBDRY)
	ret |= TARGET_VFCF_SBDRY;
#endif

    return ret;
}

/*
 * XXX this uses the undocumented oidfmt interface to find the kind of
 * a requested sysctl, see /sys/kern/kern_sysctl.c:sysctl_sysctl_oidfmt()
 * (compare to src/sbin/sysctl/sysctl.c)
 */
static int
oidfmt(int *oid, int len, char *fmt, uint32_t *kind)
{
    int qoid[CTL_MAXNAME+2];
    uint8_t buf[BUFSIZ];
    int i;
    size_t j;

    qoid[0] = 0;
    qoid[1] = 4;
    memcpy(qoid + 2, oid, len * sizeof(int));

    j = sizeof(buf);
    i = sysctl(qoid, len + 2, buf, &j, 0, 0);
    if (i) {
        return i;
    }

    if (kind) {
        *kind = *(uint32_t *)buf;
    }

    if (fmt) {
        strcpy(fmt, (char *)(buf + sizeof(uint32_t)));
    }
    return 0;
}

/*
 * try and convert sysctl return data for the target.
 * XXX doesn't handle CTLTYPE_OPAQUE and CTLTYPE_STRUCT.
 */
static int sysctl_oldcvt(void *holdp, size_t holdlen, uint32_t kind)
{
    switch (kind & CTLTYPE) {
    case CTLTYPE_INT:
    case CTLTYPE_UINT:
        *(uint32_t *)holdp = tswap32(*(uint32_t *)holdp);
        break;

#ifdef TARGET_ABI32
    case CTLTYPE_LONG:
    case CTLTYPE_ULONG:
        *(uint32_t *)holdp = tswap32(*(long *)holdp);
        break;
#else
    case CTLTYPE_LONG:
        *(uint64_t *)holdp = tswap64(*(long *)holdp);
    case CTLTYPE_ULONG:
        *(uint64_t *)holdp = tswap64(*(unsigned long *)holdp);
        break;
#endif
#if !defined(__FreeBSD_version) || __FreeBSD_version < 900031
    case CTLTYPE_QUAD:
#else
    case CTLTYPE_U64:
    case CTLTYPE_S64:
#endif
        *(uint64_t *)holdp = tswap64(*(uint64_t *)holdp);
        break;

    case CTLTYPE_STRING:
        break;

    default:
        /* XXX unhandled */
        return -1;
    }
    return 0;
}

/*
 * Convert the undocmented name2oid sysctl data for the target.
 */
static inline void sysctl_name2oid(uint32_t *holdp, size_t holdlen)
{
    size_t i, num = holdlen / sizeof(uint32_t);

    for (i = 0; i < num; i++) {
        holdp[i] = tswap32(holdp[i]);
    }
}

static inline void sysctl_oidfmt(uint32_t *holdp)
{
    /* byte swap the kind */
    holdp[0] = tswap32(holdp[0]);
}

abi_long do_freebsd_sysctl(CPUArchState *env, abi_ulong namep, int32_t namelen,
        abi_ulong oldp, abi_ulong oldlenp, abi_ulong newp, abi_ulong newlen)
{
    abi_long ret;
    void *hnamep, *holdp = NULL, *hnewp = NULL;
    size_t holdlen;
    abi_ulong oldlen = 0;
    int32_t *snamep = g_malloc(sizeof(int32_t) * namelen), *p, *q, i;
    uint32_t kind = 0;
    CPUState *cpu = ENV_GET_CPU(env);
    TaskState *ts = (TaskState *)cpu->opaque;

    if (oldlenp) {
        if (get_user_ual(oldlen, oldlenp)) {
            return -TARGET_EFAULT;
        }
    }
    hnamep = lock_user(VERIFY_READ, namep, namelen, 1);
    if (hnamep == NULL) {
        return -TARGET_EFAULT;
    }
    if (newp) {
        hnewp = lock_user(VERIFY_READ, newp, newlen, 1);
        if (hnewp == NULL) {
            return -TARGET_EFAULT;
        }
    }
    if (oldp) {
        holdp = lock_user(VERIFY_WRITE, oldp, oldlen, 0);
        if (holdp == NULL) {
            return -TARGET_EFAULT;
        }
    }
    holdlen = oldlen;
    for (p = hnamep, q = snamep, i = 0; i < namelen; p++, i++) {
        *q++ = tswap32(*p);
    }
    oidfmt(snamep, namelen, NULL, &kind);

    /* Handle some arch/emulator dependent sysctl()'s here. */
    switch (snamep[0]) {
    case CTL_KERN:
        switch (snamep[1]) {
        case KERN_USRSTACK:
#if TARGET_USRSTACK != 0
            if (oldlen) {
                (*(abi_ulong *)holdp) = tswapal(TARGET_USRSTACK);
            }
            holdlen = sizeof(abi_ulong);
            ret = 0;
#else
            ret = -TARGET_ENOENT;
#endif
            goto out;

        case KERN_PS_STRINGS:
#if defined(TARGET_PS_STRINGS)
            if (oldlen) {
                (*(abi_ulong *)holdp) = tswapal(TARGET_PS_STRINGS);
            }
            holdlen = sizeof(abi_ulong);
            ret = 0;
#else
            ret = -TARGET_ENOENT;
#endif
            goto out;

        case KERN_PROC:
            switch (snamep[2]) {
            case KERN_PROC_PATHNAME:
                holdlen = strlen(ts->bprm->fullpath) + 1;
                if (holdp) {
                    if (oldlen < holdlen) {
                        ret = -TARGET_EINVAL;
                        goto out;
                    }
                    strlcpy(holdp, ts->bprm->fullpath, oldlen);
                }
                ret = 0;
                goto out;

            default:
                break;
            }
            break;

        default:
            break;
        }
        break;

    case CTL_VFS:
	{
	    static int oid_vfs_conflist;

	    if (!oid_vfs_conflist) {
	        int real_oid[CTL_MAXNAME+2];
		size_t len = sizeof(real_oid) / sizeof(int);

		if (sysctlnametomib("vfs.conflist", real_oid, &len) >= 0)
		    oid_vfs_conflist = real_oid[1];
	    }

	    if (oid_vfs_conflist && snamep[1] == oid_vfs_conflist) {
		struct xvfsconf *xvfsp;
		struct target_xvfsconf *txp;
		int cnt, i;

		if (sysctlbyname("vfs.conflist", NULL, &holdlen, NULL, 0) < 0) {
		    ret = -1;
		    goto out;
		}
		if (!holdp) {
		    ret = 0;
		    goto out;
		}
		xvfsp = (struct xvfsconf *)g_malloc(holdlen);
		if (xvfsp == NULL) {
		    ret = -TARGET_ENOMEM;
		    goto out;
		}
		if (sysctlbyname("vfs.conflist", xvfsp, &holdlen, NULL, 0) < 0){
		    g_free(xvfsp);
		    ret = -1;
		    goto out;
		}
		cnt = holdlen / sizeof(struct xvfsconf);
		holdlen = cnt * sizeof(struct target_xvfsconf);
		txp = (struct target_xvfsconf *)holdp;
		for (i = 0; i < cnt; i++) {
		    txp[i].vfc_vfsops = 0;
		    strlcpy(txp[i].vfc_name, xvfsp[i].vfc_name,
			TARGET_MFSNAMELEN);
		    txp[i].vfc_typenum = tswap32(xvfsp[i].vfc_typenum);
		    txp[i].vfc_refcount = tswap32(xvfsp[i].vfc_refcount);
		    txp[i].vfc_flags = tswap32(
			host_to_target_vfc_flags(xvfsp[i].vfc_flags));
		    txp[i].vfc_next = 0;
	        }
		g_free(xvfsp);
		ret = 0;
		goto out;
	    }
	}
	break;

    case CTL_HW:
        switch (snamep[1]) {
        case HW_MACHINE:
	    holdlen = sizeof(TARGET_HW_MACHINE);
	    if (holdp)
		strlcpy(holdp, TARGET_HW_MACHINE, oldlen);
            ret = 0;
            goto out;

        case HW_MACHINE_ARCH:
	    holdlen = sizeof(TARGET_HW_MACHINE_ARCH);
	    if (holdp)
		strlcpy(holdp, TARGET_HW_MACHINE_ARCH, oldlen);
            ret = 0;
            goto out;

#if TARGET_ABI_BITS != HOST_LONG_BITS
	case HW_PHYSMEM:
	    holdlen = sizeof(abi_ulong);
	    ret = 0;

	    if (oldlen) {
		unsigned long lvalue;
		size_t len = sizeof(lvalue);

		if (sysctlbyname("hw.physmem", &lvalue, &len, NULL, 0)
		    == -1) {
			ret = -1;
		} else {
			abi_ulong maxmem = -0x100c000;
			if (((unsigned long)maxmem) < lvalue)
			    lvalue = maxmem;
			    (*(abi_ulong *)holdp) = lvalue;
		}
	    }
	    goto out;
#endif

        default:
            {
                static int oid_hw_availpages;
                static int oid_hw_pagesizes;

                if (!oid_hw_availpages) {
                    int real_oid[CTL_MAXNAME+2];
                    size_t len = sizeof(real_oid) / sizeof(int);

                    if (sysctlnametomib("hw.availpages", real_oid, &len) >= 0)
                        oid_hw_availpages = real_oid[1];
                }
                if (!oid_hw_pagesizes) {
                    int real_oid[CTL_MAXNAME+2];
                    size_t len = sizeof(real_oid) / sizeof(int);

                    if (sysctlnametomib("hw.pagesizes", real_oid, &len) >= 0)
                        oid_hw_pagesizes = real_oid[1];
                }

                if (oid_hw_availpages && snamep[1] == oid_hw_availpages) {
                    long lvalue;
                    size_t len = sizeof(lvalue);

                    if (sysctlbyname("hw.availpages", &lvalue, &len, NULL, 0)
                            == -1) {
                        ret = -1;
                    } else {
                        if (oldlen) {
                            (*(abi_ulong *)holdp) = tswapal((abi_ulong)lvalue);
                        }
                        holdlen = sizeof(abi_ulong);
                        ret = 0;
                    }
                    goto out;
                }

                if (oid_hw_pagesizes && snamep[1] == oid_hw_pagesizes) {
                    // XXX some targets do superpages now too... */
                    if (oldlen) {
                        (*(abi_ulong *)holdp) = tswapal((abi_ulong)getpagesize());
                        ((abi_ulong *)holdp)[1] = 0;
                    }
                    holdlen = sizeof(abi_ulong) * 2;
                    ret = 0;
                    goto out;
                }
                break;
            }
        }
    default:
        break;
    }

    ret = get_errno(sysctl(snamep, namelen, holdp, &holdlen, hnewp, newlen));
    if (!ret && (holdp != 0 && holdlen != 0)) {
        if (0 == snamep[0] && (2 == snamep[1] || 3 == snamep[1] ||
                    4 == snamep[1])) {
            switch (snamep[1]) {
            case 2:
            case 3:
                /* Handle the undocumented name2oid special case. */
                sysctl_name2oid(holdp, holdlen);
                break;

            case 4:
            default:
                /* Handle oidfmt */
                sysctl_oidfmt(holdp);
                break;
            }
        } else {
            sysctl_oldcvt(holdp, holdlen, kind);
        }
    }
#ifdef DEBUG
    else {
        printf("sysctl(mib[0]=%d, mib[1]=%d, mib[3]=%d...) returned %d\n",
        snamep[0], snamep[1], snamep[2], (int)ret);
    }
#endif

out:
    if (oldlenp) {
        put_user_ual(holdlen, oldlenp);
    }
    unlock_user(hnamep, namep, 0);
    unlock_user(holdp, oldp, holdlen);
    if (hnewp) {
        unlock_user(hnewp, newp, 0);
    }
    g_free(snamep);
    return ret;
}

/* sysarch() is architecture dependent. */
abi_long do_freebsd_sysarch(void *cpu_env, abi_long arg1, abi_long arg2)
{

    return do_freebsd_arch_sysarch(cpu_env, arg1, arg2);
}
