/*
 *  FreeBSD disk.h definitions for ioctl(2) emulation
 *
 *  Copyright (c) 2015 Stacey D. Son
 *  All rights reserved.
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
#ifndef _OS_IOCTL_DISK_H_
#define _OS_IOCTL_DISK_H_

/* See sys/disk.h */

#define TARGET_DIOCGSECTORSIZE      TARGET_IOR('d', 128, uint32_t)
#define TARGET_DIOCGMEDIASIZE       TARGET_IOR('d', 129, int64_t)
#define TARGET_DIOCGFWSECTORS       TARGET_IOR('d', 130, uint32_t)
#define TARGET_DIOCGFWHEADS         TARGET_IOR('d', 131, uint32_t)
#define TARGET_DIOCSKERNELDUMP      TARGET_IOW('d', 133, uint32_t)
#define TARGET_DIOCGFRONTSTUFF      TARGET_IOR('d', 134, int64_t)
#define TARGET_DIOCGFLUSH           TARGET_IO('d', 135)
#define TARGET_DIOCGDELETE          TARGET_IOW('d', 136, int64_t[2])
#define TARGET_DISK_IDENT_SIZE 256
#define TARGET_DIOCGIDENT           TARGET_IOR('d', 137,            \
                                        char[TARGET_DISK_IDENT_SIZE])
#define TARGET_DIOCGPROVIDERNAME    TARGET_IOR('d', 138, char[MAXPATHLEN])
#define TARGET_DIOCGSTRIPESIZE      TARGET_IOR('d', 139, int64_t)
#define TARGET_DIOCGSTRIPEOFFSET    TARGET_IOR('d', 140, int64_t)
#define TARGET_DIOCGPHYSPATH        TARGET_IOR('d', 141, char[MAXPATHLEN])

struct target_diocgattr_arg {
        char name[64];
        int32_t len;
        union {
                char str[TARGET_DISK_IDENT_SIZE];
                int64_t off;
                int32_t i;
        } value;
};

#define TARGET_DIOCGATTR    TARGET_IOWR('d', 142, struct target_diocgattr_arg)

#endif /* _OS_IOCTL_DISK_H_ */
