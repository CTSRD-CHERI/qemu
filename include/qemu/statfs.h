/*
 * Host statfs header abstraction
 *
 * This work is licensed under the terms of the GNU GPL, version 2, or any
 * later version.  See the COPYING file in the top-level directory.
 *
 */
#ifndef QEMU_STATFS_H
#define QEMU_STATFS_H

#ifdef CONFIG_LINUX
# include <sys/vfs.h>
#endif
#ifdef CONFIG_DARWIN
# include <sys/param.h>
# include <sys/mount.h>
#endif

#endif
