/*
 * 9p utilities (Darwin Implementation)
 *
 * This work is licensed under the terms of the GNU GPL, version 2 or later.
 * See the COPYING file in the top-level directory.
 */

#include "qemu/osdep.h"
#include "qemu/xattr.h"
#include "9p-util.h"

ssize_t fgetxattrat_nofollow(int dirfd, const char *filename, const char *name,
                             void *value, size_t size)
{
    int ret;
    int fd = openat_file(dirfd, filename,
                         O_RDONLY | O_PATH_9P_UTIL | O_NOFOLLOW, 0);
    if (fd == -1) {
        return -1;
    }
    ret = fgetxattr(fd, name, value, size, 0, 0);
    close_preserve_errno(fd);
    return ret;
}

ssize_t flistxattrat_nofollow(int dirfd, const char *filename,
                              char *list, size_t size)
{
    int ret;
    int fd = openat_file(dirfd, filename,
                         O_RDONLY | O_PATH_9P_UTIL | O_NOFOLLOW, 0);
    if (fd == -1) {
        return -1;
    }
    ret = flistxattr(fd, list, size, 0);
    close_preserve_errno(fd);
    return ret;
}

ssize_t fremovexattrat_nofollow(int dirfd, const char *filename,
                                const char *name)
{
    int ret;
    int fd = openat_file(dirfd, filename, O_PATH_9P_UTIL | O_NOFOLLOW, 0);
    if (fd == -1) {
        return -1;
    }
    ret = fremovexattr(fd, name, 0);
    close_preserve_errno(fd);
    return ret;
}

int fsetxattrat_nofollow(int dirfd, const char *filename, const char *name,
                         void *value, size_t size, int flags)
{
    int ret;
    int fd = openat_file(dirfd, filename, O_PATH_9P_UTIL | O_NOFOLLOW, 0);
    if (fd == -1) {
        return -1;
    }
    ret = fsetxattr(fd, name, value, size, 0, flags);
    close_preserve_errno(fd);
    return ret;
}

#ifndef __has_builtin
#define __has_builtin(x) 0
#endif

static int update_times_from_stat(int fd, struct timespec times[2],
                                  int update0, int update1)
{
    struct stat buf;
    int ret = fstat(fd, &buf);
    if (ret == -1) {
        return ret;
    }
    if (update0) {
        times[0] = buf.st_atimespec;
    }
    if (update1) {
        times[1] = buf.st_mtimespec;
    }
    return 0;
}

int utimensat_nofollow(int dirfd, const char *filename,
                       const struct timespec times_in[2])
{
    int ret, fd;
    int special0, special1;
    struct timeval futimes_buf[2];
    struct timespec times[2];
    memcpy(times, times_in, 2 * sizeof(struct timespec));

/* Check whether we have an SDK version that defines utimensat */
#if defined(__MAC_10_13)
# if __MAC_OS_X_VERSION_MIN_REQUIRED >= __MAC_10_13
#  define UTIMENSAT_AVAILABLE 1
# elif __has_builtin(__builtin_available)
#  define UTIMENSAT_AVAILABLE __builtin_available(macos 10.13, *)
# else
#  define UTIMENSAT_AVAILABLE 0
# endif
    if (UTIMENSAT_AVAILABLE) {
        return utimensat(dirfd, filename, times, AT_SYMLINK_NOFOLLOW);
    }
#endif

    /* utimensat not available. Use futimes. */
    fd = openat_file(dirfd, filename, O_PATH_9P_UTIL | O_NOFOLLOW, 0);
    if (fd == -1) {
        return -1;
    }

    special0 = times[0].tv_nsec == UTIME_OMIT;
    special1 = times[1].tv_nsec == UTIME_OMIT;
    if (special0 || special1) {
        /* If both are set, nothing to do */
        if (special0 && special1) {
            ret = 0;
            goto done;
        }

        ret = update_times_from_stat(fd, times, special0, special1);
        if (ret < 0) {
            goto done;
        }
    }

    special0 = times[0].tv_nsec == UTIME_NOW;
    special1 = times[1].tv_nsec == UTIME_NOW;
    if (special0 || special1) {
        ret = futimes(fd, NULL);
        if (ret < 0) {
            goto done;
        }

        /* If both are set, we are done */
        if (special0 && special1) {
            ret = 0;
            goto done;
        }

        ret = update_times_from_stat(fd, times, special0, special1);
        if (ret < 0) {
            goto done;
        }
    }

    futimes_buf[0].tv_sec = times[0].tv_sec;
    futimes_buf[0].tv_usec = times[0].tv_nsec / 1000;
    futimes_buf[1].tv_sec = times[1].tv_sec;
    futimes_buf[1].tv_usec = times[1].tv_nsec / 1000;
    ret = futimes(fd, futimes_buf);

done:
    close_preserve_errno(fd);
    return ret;
}
