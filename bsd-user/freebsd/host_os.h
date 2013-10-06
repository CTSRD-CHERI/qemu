/*
 *  FreeBSD host dependent code and definitions
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

#ifndef __HOST_OS_H_
#define __HOST_OS_H_

#include <stdio.h>
#include <sys/sysctl.h>

#include "qemu.h"

#define HOST_DEFAULT_BSD_TYPE target_freebsd

static inline void save_proc_pathname(char *argv0)
{
    int mib[4];
    size_t len;

    mib[0] = CTL_KERN;
    mib[1] = KERN_PROC;
    mib[2] = KERN_PROC_PATHNAME;
    mib[3] = -1;

    len = PATH_MAX;
    if (sysctl(mib, 4, qemu_proc_pathname, &len, NULL, 0)) {
        perror("sysctl");
    }
}

#endif /*!__HOST_OS_H_ */
