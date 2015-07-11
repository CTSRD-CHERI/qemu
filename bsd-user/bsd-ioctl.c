/*
 *  BSD ioctl(2) emulation
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
#include "qemu/osdep.h"

#include <sys/types.h>
#include <sys/param.h>
#include <sys/disk.h>
#include <sys/ioccom.h>
#include <sys/ioctl.h>
#include <sys/sockio.h>
#if defined(__FreeBSD_version) && __FreeBSD_version > 900000
#include <sys/_termios.h>
#else
#include <sys/termios.h>
#endif
#include <sys/ttycom.h>
#include <sys/filio.h>
#include <sys/socket.h> /* needed for FreeBSD 8.4 */

#include <crypto/cryptodev.h>

#include <net/if.h>
#include <net/if_var.h>
#include <net/if_types.h>
#include <net/route.h>
#include <net/if_dl.h>
#include <net/if_gif.h>
#include <net/if_gre.h>
#include <net/if_media.h>
#include <netpfil/pf/pf.h>
#include <net/pfvar.h>
#include <net/if_pfsync.h>
#include <net/ethernet.h>
#include <netinet/icmp6.h>
#include <netinet/in.h>
#include <netinet/ip_carp.h>
#include <netinet6/in6_var.h>
#include <net80211/ieee80211_ioctl.h>

#include <stdio.h>

#include "qemu.h"
#include "qemu-common.h"

#include "bsd-ioctl.h"
#include "os-ioctl-cryptodev.h"
#include "os-ioctl-disk.h"
#include "os-ioctl-filio.h"
#include "os-ioctl-in6_var.h"
#include "os-ioctl-sockio.h"
#include "os-ioctl-ttycom.h"

static const bitmask_transtbl iflag_tbl[] = {
    { TARGET_IGNBRK, TARGET_IGNBRK, IGNBRK, IGNBRK },
    { TARGET_BRKINT, TARGET_BRKINT, BRKINT, BRKINT },
    { TARGET_IGNPAR, TARGET_IGNPAR, IGNPAR, IGNPAR },
    { TARGET_PARMRK, TARGET_PARMRK, PARMRK, PARMRK },
    { TARGET_INPCK, TARGET_INPCK, INPCK, INPCK },
    { TARGET_ISTRIP, TARGET_ISTRIP, ISTRIP, ISTRIP },
    { TARGET_INLCR, TARGET_INLCR, INLCR, INLCR },
    { TARGET_IGNCR, TARGET_IGNCR, IGNCR, IGNCR },
    { TARGET_ICRNL, TARGET_ICRNL, ICRNL, ICRNL },
    { TARGET_IXON, TARGET_IXON, IXON, IXON },
    { TARGET_IXOFF, TARGET_IXOFF, IXOFF, IXOFF },
#ifdef IXANY
    { TARGET_IXANY, TARGET_IXANY, IXANY, IXANY },
#endif
#ifdef IMAXBEL
    { TARGET_IMAXBEL, TARGET_IMAXBEL, IMAXBEL, IMAXBEL },
#endif
    { 0, 0, 0, 0 }
};

static const bitmask_transtbl oflag_tbl[] = {
    { TARGET_OPOST, TARGET_OPOST, OPOST, OPOST },
#ifdef ONLCR
    { TARGET_ONLCR, TARGET_ONLCR, ONLCR, ONLCR },
#endif
#ifdef TABDLY
    { TARGET_TABDLY, TARGET_TAB0, TABDLY, TAB0 },
    { TARGET_TABDLY, TARGET_TAB3, TABDLY, TAB3 },
#endif
#ifdef ONOEOT
    { TARGET_ONOEOT, TARGET_ONOEOT, ONOEOT, ONOEOT },
#endif
#ifdef OCRNL
    { TARGET_OCRNL, TARGET_OCRNL, OCRNL, OCRNL },
#endif
#ifdef ONOCR
    { TARGET_ONOCR, TARGET_ONOCR, ONOCR, ONOCR },
#endif
#ifdef ONLRET
    { TARGET_ONLRET, TARGET_ONLRET, ONLRET, ONLRET },
#endif
    { 0, 0, 0, 0 }
};

static const bitmask_transtbl cflag_tbl[] = {
#ifdef CIGNORE
    { TARGET_CIGNORE, TARGET_CIGNORE, CIGNORE, CIGNORE },
#endif
    { TARGET_CSIZE, TARGET_CS5, CSIZE, CS5 },
    { TARGET_CSIZE, TARGET_CS6, CSIZE, CS6 },
    { TARGET_CSIZE, TARGET_CS7, CSIZE, CS7 },
    { TARGET_CSIZE, TARGET_CS8, CSIZE, CS8 },
    { TARGET_CSTOPB, TARGET_CSTOPB, CSTOPB, CSTOPB },
    { TARGET_CREAD, TARGET_CREAD, CREAD, CREAD },
    { TARGET_PARENB, TARGET_PARENB, PARENB, PARENB },
    { TARGET_PARODD, TARGET_PARODD, PARODD, PARODD },
    { TARGET_HUPCL, TARGET_HUPCL, HUPCL, HUPCL },
    { TARGET_CLOCAL, TARGET_CLOCAL, CLOCAL, CLOCAL },
#ifdef CCTS_OFLOW
    { TARGET_CCTS_OFLOW, TARGET_CCTS_OFLOW, CCTS_OFLOW, CCTS_OFLOW },
#endif
#ifdef CRTSCTS
    { TARGET_CRTSCTS, TARGET_CRTSCTS, CRTSCTS, CRTSCTS },
#endif
#ifdef CRTS_IFLOW
    { TARGET_CRTS_IFLOW, TARGET_CRTS_IFLOW, CRTS_IFLOW, CRTS_IFLOW },
#endif
#ifdef CDTS_IFLOW
    { TARGET_CDTR_IFLOW, TARGET_CDTR_IFLOW, CDTR_IFLOW, CDTR_IFLOW },
#endif
#ifdef CDSR_OFLOW
    { TARGET_CDSR_OFLOW, TARGET_CDSR_OFLOW, CDSR_OFLOW, CDSR_OFLOW },
#endif
#ifdef CCAR_OFLOW
    { TARGET_CCAR_OFLOW, TARGET_CCAR_OFLOW, CCAR_OFLOW, CCAR_OFLOW },
#endif
    { 0, 0, 0, 0 }
};

static const bitmask_transtbl lflag_tbl[] = {
#ifdef ECHOKE
    { TARGET_ECHOKE, TARGET_ECHOKE, ECHOKE, ECHOKE },
#endif
    { TARGET_ECHOE, TARGET_ECHOE, ECHOE, ECHOE },
    { TARGET_ECHOK, TARGET_ECHOK, ECHOK, ECHOK },
    { TARGET_ECHO, TARGET_ECHO, ECHO, ECHO },
    { TARGET_ECHONL, TARGET_ECHONL, ECHONL, ECHONL },
#ifdef ECHOPRT
    { TARGET_ECHOPRT, TARGET_ECHOPRT, ECHOPRT, ECHOPRT },
#endif
#ifdef ECHOCTL
    { TARGET_ECHOCTL, TARGET_ECHOCTL, ECHOCTL, ECHOCTL },
#endif
    { TARGET_ISIG, TARGET_ISIG, ISIG, ISIG },
    { TARGET_ICANON, TARGET_ICANON, ICANON, ICANON },
#ifdef ALTWERASE
    { TARGET_ALTWERASE, TARGET_ALTWERASE, ALTWERASE, ALTWERASE },
#endif
    { TARGET_IEXTEN, TARGET_IEXTEN, IEXTEN, IEXTEN },
    { TARGET_EXTPROC, TARGET_EXTPROC, EXTPROC, EXTPROC },
    { TARGET_TOSTOP, TARGET_TOSTOP, TOSTOP, TOSTOP },
#ifdef FLUSHO
    { TARGET_FLUSHO, TARGET_FLUSHO, FLUSHO, FLUSHO },
#endif
#ifdef NOKERNINFO
    { TARGET_NOKERNINFO, TARGET_NOKERNINFO, NOKERNINFO, NOKERNINFO },
#endif
#ifdef PENDIN
    { TARGET_PENDIN, TARGET_PENDIN, PENDIN, PENDIN },
#endif
    { TARGET_NOFLSH, TARGET_NOFLSH, NOFLSH, NOFLSH },
    { 0, 0, 0, 0 }
};

static void target_to_host_termios(void *dst, const void *src)
{
    struct termios *host = dst;
    const struct target_termios *target = src;

    host->c_iflag = target_to_host_bitmask(tswap32(target->c_iflag), iflag_tbl);
    host->c_oflag = target_to_host_bitmask(tswap32(target->c_oflag), oflag_tbl);
    host->c_cflag = target_to_host_bitmask(tswap32(target->c_cflag), cflag_tbl);
    host->c_lflag = target_to_host_bitmask(tswap32(target->c_lflag), lflag_tbl);

    memset(host->c_cc, 0, sizeof(host->c_cc));
    host->c_cc[VEOF] = target->c_cc[TARGET_VEOF];
    host->c_cc[VEOL] = target->c_cc[TARGET_VEOL];
#ifdef VEOL2
    host->c_cc[VEOL2] = target->c_cc[TARGET_VEOL2];
#endif
    host->c_cc[VERASE] = target->c_cc[TARGET_VERASE];
#ifdef VWERASE
    host->c_cc[VWERASE] = target->c_cc[TARGET_VWERASE];
#endif
    host->c_cc[VKILL] = target->c_cc[TARGET_VKILL];
#ifdef VREPRINT
    host->c_cc[VREPRINT] = target->c_cc[TARGET_VREPRINT];
#endif
#ifdef VERASE2
    host->c_cc[VERASE2] = target->c_cc[TARGET_VERASE2];
#endif
    host->c_cc[VINTR] = target->c_cc[TARGET_VINTR];
    host->c_cc[VQUIT] = target->c_cc[TARGET_VQUIT];
    host->c_cc[VSUSP] = target->c_cc[TARGET_VSUSP];
#ifdef VDSUSP
    host->c_cc[VDSUSP] = target->c_cc[TARGET_VDSUSP];
#endif
    host->c_cc[VSTART] = target->c_cc[TARGET_VSTART];
    host->c_cc[VSTOP] = target->c_cc[TARGET_VSTOP];
#ifdef VLNEXT
    host->c_cc[VLNEXT] = target->c_cc[TARGET_VLNEXT];
#endif
#ifdef VDISCARD
    host->c_cc[VDISCARD] = target->c_cc[TARGET_VDISCARD];
#endif
    host->c_cc[VMIN] = target->c_cc[TARGET_VMIN];
    host->c_cc[VTIME] = target->c_cc[TARGET_VTIME];
#ifdef VSTATUS
    host->c_cc[VSTATUS] = target->c_cc[TARGET_VSTATUS];
#endif

    host->c_ispeed = tswap32(target->c_ispeed);
    host->c_ospeed = tswap32(target->c_ospeed);
}

static void host_to_target_termios(void *dst, const void *src)
{
    struct target_termios *target = dst;
    const struct termios *host = src;

    target->c_iflag = tswap32(host_to_target_bitmask(host->c_iflag, iflag_tbl));
    target->c_oflag = tswap32(host_to_target_bitmask(host->c_oflag, oflag_tbl));
    target->c_cflag = tswap32(host_to_target_bitmask(host->c_cflag, cflag_tbl));
    target->c_lflag = tswap32(host_to_target_bitmask(host->c_lflag, lflag_tbl));

    memset(target->c_cc, 0, sizeof(target->c_cc));
    target->c_cc[TARGET_VEOF] = host->c_cc[VEOF];
    target->c_cc[TARGET_VEOL] = host->c_cc[VEOL];
#ifdef VEOL2
    target->c_cc[TARGET_VEOL2] = host->c_cc[VEOL2];
#endif
    target->c_cc[TARGET_VERASE] = host->c_cc[VERASE];
#ifdef VWERASE
    target->c_cc[TARGET_VWERASE] = host->c_cc[VWERASE];
#endif
    target->c_cc[TARGET_VKILL] = host->c_cc[VKILL];
#ifdef VREPRINT
    target->c_cc[TARGET_VREPRINT] = host->c_cc[VREPRINT];
#endif
#ifdef VERASE2
    target->c_cc[TARGET_VERASE2] = host->c_cc[VERASE2];
#endif
    target->c_cc[TARGET_VINTR] = host->c_cc[VINTR];
    target->c_cc[TARGET_VQUIT] = host->c_cc[VQUIT];
    target->c_cc[TARGET_VSUSP] = host->c_cc[VSUSP];
#ifdef VDSUSP
    target->c_cc[TARGET_VDSUSP] = host->c_cc[VDSUSP];
#endif
    target->c_cc[TARGET_VSTART] = host->c_cc[VSTART];
    target->c_cc[TARGET_VSTOP] = host->c_cc[VSTOP];
#ifdef VLNEXT
    target->c_cc[TARGET_VLNEXT] = host->c_cc[VLNEXT];
#endif
#ifdef VDISCARD
    target->c_cc[TARGET_VDISCARD] = host->c_cc[VDISCARD];
#endif
    target->c_cc[TARGET_VMIN] = host->c_cc[VMIN];
    target->c_cc[TARGET_VTIME] = host->c_cc[VTIME];
#ifdef VSTATUS
    target->c_cc[TARGET_VSTATUS] = host->c_cc[VSTATUS];
#endif

    target->c_ispeed = tswap32(host->c_ispeed);
    target->c_ospeed = tswap32(host->c_ospeed);
}

static const StructEntry struct_termios_def = {
    .convert = { host_to_target_termios, target_to_host_termios },
    .size = { sizeof(struct target_termios), sizeof(struct termios) },
    .align = { __alignof__(struct target_termios),
        __alignof__(struct termios) },
};


/* ioctl structure type definitions */
#define STRUCT(name, ...) STRUCT_ ## name,
#define STRUCT_SPECIAL(name) STRUCT_ ## name,
enum {
#include "os-ioctl-types.h"
STRUCT_MAX
};
#undef STRUCT
#undef STRUCT_SPECIAL

#define STRUCT(name, ...) \
    static const argtype struct_ ## name ## _def[] = { __VA_ARGS__, TYPE_NULL };
#define STRUCT_SPECIAL(name)
#include "os-ioctl-types.h"
#undef STRUCT
#undef STRUCT_SPECIAL


struct IOCTLEntry;

typedef abi_long do_ioctl_fn(const struct IOCTLEntry *ie, uint8_t *buf_temp,
                int fd, abi_long cmd, abi_long arg);

struct IOCTLEntry {
    unsigned int target_cmd;
    unsigned int host_cmd;
    const char *name;
    int access;
    do_ioctl_fn *do_ioctl;
    const argtype arg_type[5];
};
typedef struct IOCTLEntry IOCTLEntry;

#define MAX_STRUCT_SIZE 4096

static abi_long do_ioctl_unsupported(__unused const IOCTLEntry *ie,
		__unused uint8_t *buf_temp,  __unused int fd,
		__unused abi_long cmd, __unused abi_long arg);

static IOCTLEntry ioctl_entries[] = {
#define IOC_    0x0000
#define IOC_R   0x0001
#define IOC_W   0x0002
#define IOC_RW  (IOC_R | IOC_W)
#define IOCTL(cmd, access, ...) \
    { TARGET_ ## cmd, cmd, #cmd, access, 0, { __VA_ARGS__ } },
#define IOCTL_SPECIAL(cmd, access, dofn, ...) \
    { TARGET_ ## cmd, cmd, #cmd, access, dofn, { __VA_ARGS__ } },
#include "os-ioctl-cmds.h"
    { 0, 0 },
};

static void log_unsupported_ioctl(unsigned long cmd)
{
	gemu_log("cmd=0x%08lx dir=", cmd);
	switch (cmd & IOC_DIRMASK) {
	case IOC_VOID:
		gemu_log("VOID ");
		break;
	case IOC_OUT:
		gemu_log("OUT ");
		break;
	case IOC_IN:
		gemu_log("IN  ");
		break;
	case IOC_INOUT:
		gemu_log("INOUT");
		break;
	default:
		gemu_log("%01lx ???", (cmd & IOC_DIRMASK) >> 29);
		break;
	}
	gemu_log(" '%c' %3d %lu\n", (char)IOCGROUP(cmd), (int)(cmd & 0xff), IOCPARM_LEN(cmd));
}

static abi_long do_ioctl_unsupported(__unused const IOCTLEntry *ie,
		__unused uint8_t *buf_temp,  __unused int fd,
		__unused abi_long cmd, __unused abi_long arg)
{

	return -TARGET_ENXIO;
}

abi_long do_bsd_ioctl(int fd, abi_long cmd, abi_long arg)
{
    const IOCTLEntry *ie;
    const argtype *arg_type;
    abi_long ret;
    uint8_t buf_temp[MAX_STRUCT_SIZE];
    int target_size;
    void *argptr;

    ie = ioctl_entries;
    for (;;) {
        if (ie->target_cmd == 0) {
	    gemu_log("Qemu unsupported ioctl: ");
	    log_unsupported_ioctl(cmd);
            return -TARGET_ENOSYS;
        }
        if (ie->target_cmd == cmd) {
            break;
        }
        ie++;
    }
    arg_type = ie->arg_type;
#if defined(DEBUG)
    gemu_log("ioctl: cmd=0x%04lx (%s)\n", (long)cmd, ie->name);
#endif
    if (ie->do_ioctl) {
        return ie->do_ioctl(ie, buf_temp, fd, cmd, arg);
    }

    switch (arg_type[0]) {
    case TYPE_NULL:
        /* no argument */
        ret = get_errno(ioctl(fd, ie->host_cmd));
        break;

    case TYPE_PTRVOID:
    case TYPE_INT:
        /* int argument */
        ret = get_errno(ioctl(fd, ie->host_cmd, arg));
        break;

    case TYPE_PTR:
        arg_type++;
        target_size = thunk_type_size(arg_type, 0);
        switch (ie->access) {
        case IOC_R:
            ret = get_errno(ioctl(fd, ie->host_cmd, buf_temp));
            if (!is_error(ret)) {
                argptr = lock_user(VERIFY_WRITE, arg,
                    target_size, 0);
                if (!argptr) {
                    return -TARGET_EFAULT;
                }
                thunk_convert(argptr, buf_temp, arg_type,
                    THUNK_TARGET);
                unlock_user(argptr, arg, target_size);
            }
            break;

        case IOC_W:
            argptr = lock_user(VERIFY_READ, arg, target_size, 1);
            if (!argptr) {
                return -TARGET_EFAULT;
            }
            thunk_convert(buf_temp, argptr, arg_type, THUNK_HOST);
            unlock_user(argptr, arg, 0);
            ret = get_errno(ioctl(fd, ie->host_cmd, buf_temp));
            break;

        case IOC_RW:
            /* fallthrough */
        default:
            argptr = lock_user(VERIFY_READ, arg, target_size, 1);
            if (!argptr) {
                return -TARGET_EFAULT;
            }
            thunk_convert(buf_temp, argptr, arg_type, THUNK_HOST);
            unlock_user(argptr, arg, 0);
            ret = get_errno(ioctl(fd, ie->host_cmd, buf_temp));
            if (!is_error(ret)) {
                argptr = lock_user(VERIFY_WRITE, arg, target_size, 0);
                if (!argptr) {
                    return -TARGET_EFAULT;
                }
                thunk_convert(argptr, buf_temp, arg_type, THUNK_TARGET);
                unlock_user(argptr, arg, target_size);
            }
            break;
        }
        break;

    default:
	gemu_log("Qemu unknown ioctl: type=%d ", arg_type[0]);
	log_unsupported_ioctl(cmd);
        ret = -TARGET_ENOSYS;
        break;
    }
    return ret;
}

void init_bsd_ioctl(void)
{
    IOCTLEntry *ie;
    const argtype *arg_type;
    int size;

    thunk_init(STRUCT_MAX);

#define STRUCT(name, ...) \
 thunk_register_struct(STRUCT_ ## name, #name, struct_ ## name ## _def);
#define STRUCT_SPECIAL(name) \
 thunk_register_struct_direct(STRUCT_ ## name, #name, &struct_ ## name ## _def);
#include "os-ioctl-types.h"
#undef STRUCT
#undef STRUCT_SPECIAL

    /*
     * Patch the ioctl size if necessary using the fact that no
     * ioctl has all the bits at '1' in the size field
     * (IOCPARM_MAX - 1).
     */
    ie = ioctl_entries;
    while (ie->target_cmd != 0) {
        if (((ie->target_cmd >> TARGET_IOCPARM_SHIFT) &
                    TARGET_IOCPARM_MASK) == TARGET_IOCPARM_MASK) {
            arg_type = ie->arg_type;
            if (arg_type[0] != TYPE_PTR) {
                fprintf(stderr, "cannot patch size for ioctl 0x%x\n",
                        ie->target_cmd);
                exit(1);
            }
            arg_type++;
            size = thunk_type_size(arg_type, 0);
            ie->target_cmd = (ie->target_cmd &
                    ~(TARGET_IOCPARM_MASK << TARGET_IOCPARM_SHIFT)) |
                (size << TARGET_IOCPARM_SHIFT);
        }
        ie++;
    }

}

