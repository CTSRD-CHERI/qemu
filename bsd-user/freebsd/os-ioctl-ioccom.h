/*
 *  FreeBSD ioccom definitions for ioctl(2) emulation
 *
 *  Copyright (c) 2013 Stacey D. Son
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

#ifndef _IOCTL_IOCCOM_H_
#define _IOCTL_IOCCOM_H_
/*
 * Ioctl's have the command encoded in the lower word, and the size of
 * any in or out parameters in the upper word.  The high 3 bits of the
 * upper word are used to encode the in/out status of the parameter.
 */
/* number of bits for ioctl size */
#define TARGET_IOCPARM_SHIFT    13

/* parameter length mask */
#define TARGET_IOCPARM_MASK ((1 << TARGET_IOCPARM_SHIFT) - 1)

#define TARGET_IOCPARM_LEN(x)   (((x) >> 16) & TARGET_IOCPARM_MASK)
#define TARGET_IOCBASECMD(x)    ((x) & ~(TARGET_IOCPARM_MASK << 16))
#define TARGET_IOCGROUP(x)  (((x) >> 8) & 0xff)

#define TARGET_IOCPARM_MAX  (1 << TARGET_IOCPARM_SHIFT) /* max size of ioctl */
#define TARGET_IOC_VOID     0x20000000  /* no parameters */
#define TARGET_IOC_OUT      0x40000000  /* copy out parameters */
#define TARGET_IOC_IN       0x80000000  /* copy in parameters */
#define TARGET_IOC_INOUT    (TARGET_IOC_IN|TARGET_IOC_OUT)
#define TARGET_IOC_DIRMASK  (TARGET_IOC_VOID|TARGET_IOC_OUT|TARGET_IOC_IN)

#define TARGET_IOC(inout, group, num, len) ((abi_ulong) \
    ((inout) | (((len) & TARGET_IOCPARM_MASK) << 16) | ((group) << 8) \
     | (num)))
#define TARGET_IO(g, n)       TARGET_IOC(IOC_VOID,    (g), (n), 0)
#define TARGET_IOWINT(g, n)   TARGET_IOC(IOC_VOID,    (g), (n), sizeof(int))
#define TARGET_IOR(g, n, t)   TARGET_IOC(IOC_OUT, (g), (n), sizeof(t))
#define TARGET_IOW(g, n, t)   TARGET_IOC(IOC_IN,  (g), (n), sizeof(t))
/* this should be _IORW, but stdio got there first */
#define TARGET_IOWR(g, n, t)  TARGET_IOC(IOC_INOUT,   (g), (n), sizeof(t))

#endif /* !_IOCTL_IOCCOM_H_ */
