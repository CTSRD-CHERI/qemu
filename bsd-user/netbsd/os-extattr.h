/*
 *  NetBSD extended attributes and ACL system call support
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

/* XXX To support FreeBSD targets the following will need to be added. */

/* extattrctl() */
static inline abi_long do_freebsd_extattrctl(abi_ulong arg1, abi_ulong arg2,
        abi_ulong arg3, abi_ulong arg4, abi_ulong arg5)
{

    qemu_log("qemu: Unsupported syscall extattrctl()\n");
    return -TARGET_ENOSYS;
}

/* extattr_set_file(2) */
static inline abi_long do_freebsd_extattr_set_file(abi_ulong arg1,
        abi_long arg2, abi_ulong arg3, abi_ulong arg4, abi_ulong arg5)
{

    qemu_log("qemu: Unsupported syscall extattr_set_file()\n");
    return -TARGET_ENOSYS;
}

/* extattr_get_file(2) */
static inline abi_long do_freebsd_extattr_get_file(abi_ulong arg1,
        abi_long arg2, abi_ulong arg3, abi_ulong arg4, abi_ulong arg5)
{

    qemu_log("qemu: Unsupported syscall extattr_get_file()\n");
    return -TARGET_ENOSYS;
}

/* extattr_delete_file(2) */
static inline abi_long do_freebsd_extattr_delete_file(abi_ulong arg1,
        abi_long arg2, abi_ulong arg3)
{

    qemu_log("qemu: Unsupported syscall extattr_delete_file()\n");
    return -TARGET_ENOSYS;
}

/* extattr_set_fd(2) */
static inline abi_long do_freebsd_extattr_set_fd(abi_long arg1,
        abi_long arg2, abi_ulong arg3, abi_ulong arg4, abi_ulong arg5)
{

    qemu_log("qemu: Unsupported syscall extattr_set_fd()\n");
    return -TARGET_ENOSYS;
}

/* extattr_get_fd(2) */
static inline abi_long do_freebsd_extattr_get_fd(abi_long arg1,
        abi_long arg2, abi_ulong arg3, abi_ulong arg4, abi_ulong arg5)
{

    qemu_log("qemu: Unsupported syscall extattr_get_fd()\n");
    return -TARGET_ENOSYS;
}

/* extattr_delete_fd(2) */
static inline abi_long do_freebsd_extattr_delete_fd(abi_long arg1,
        abi_long arg2, abi_ulong arg3)
{

    qemu_log("qemu: Unsupported syscall extattr_delete_fd()\n");
    return -TARGET_ENOSYS;
}

/* extattr_get_link(2) */
static inline abi_long do_freebsd_extattr_get_link(abi_ulong arg1,
        abi_long arg2, abi_ulong arg3, abi_ulong arg4, abi_ulong arg5)
{

    qemu_log("qemu: Unsupported syscall extattr_get_link()\n");
    return -TARGET_ENOSYS;
}

/* extattr_set_link(2) */
static inline abi_long do_freebsd_extattr_set_link(abi_ulong arg1,
        abi_long arg2, abi_ulong arg3, abi_ulong arg4, abi_ulong arg5)
{

    qemu_log("qemu: Unsupported syscall extattr_set_link()\n");
    return -TARGET_ENOSYS;
}

/* extattr_delete_link(2) */
static inline abi_long do_freebsd_extattr_delete_link(abi_ulong arg1,
        abi_long arg2, abi_ulong arg3)
{

    qemu_log("qemu: Unsupported syscall extattr_delete_link()\n");
    return -TARGET_ENOSYS;
}

/* extattr_list_fd(2) */
static inline abi_long do_freebsd_extattr_list_fd(abi_long arg1, abi_long arg2,
        abi_ulong arg3, abi_ulong arg4)
{

    qemu_log("qemu: Unsupported syscall exattr_list_fd()\n");
    return -TARGET_ENOSYS;
}

/* extattr_list_file(2) */
static inline abi_long do_freebsd_extattr_list_file(abi_long arg1,
        abi_long arg2, abi_ulong arg3, abi_ulong arg4)
{

    qemu_log("qemu: Unsupported syscall extattr_list_file()\n");
    return -TARGET_ENOSYS;
}

/* extattr_list_link(2) */
static inline abi_long do_freebsd_extattr_list_link(abi_long arg1,
        abi_long arg2, abi_ulong arg3, abi_ulong arg4)
{

    qemu_log("qemu: Unsupported syscall extattr_list_link()\n");
    return -TARGET_ENOSYS;
}

/*
 *  Access Control Lists
 */

/* __acl_aclcheck_fd(int filedes, acl_type_t type, struct acl *aclp); */
static inline abi_long do_freebsd__acl_aclcheck_fd(abi_long arg1, abi_long arg2,
       abi_ulong arg3)
{

    qemu_log("qemu: Unsupported syscall __acl_aclcheck_fd()\n");
    return -TARGET_ENOSYS;
}

/* __acl_aclcheck_file(const char *path, acl_type_t type, struct acl *aclp); */
static inline abi_long do_freebsd__acl_aclcheck_file(abi_ulong arg1,
        abi_long arg2, abi_ulong arg3)
{

    qemu_log("qemu: Unsupported syscall __acl_aclcheck_file()\n");
    return -TARGET_ENOSYS;
}

/* __acl_aclcheck_link(const char *path, acl_type_t type, struct acl *aclp); */
static inline abi_long do_freebsd__acl_aclcheck_link(abi_ulong arg1,
        abi_long arg2, abi_ulong arg3)
{

    qemu_log("qemu: Unsupported syscall __acl_aclcheck_link()\n");
    return -TARGET_ENOSYS;
}

/* int __acl_delete_fd(int filedes, acl_type_t type); */
static inline abi_long do_freebsd__acl_delete_fd(abi_long arg1, abi_long arg2)
{

    qemu_log("qemu: Unsupported syscall __acl_delete_fd()\n");
    return -TARGET_ENOSYS;
}

/* int __acl_delete_file(const char *path, acl_type_t type); */
static inline abi_long do_freebsd__acl_delete_file(abi_ulong arg1,
        abi_long arg2)
{

    qemu_log("qemu: Unsupported syscall __acl_delete_fil()\n");
    return -TARGET_ENOSYS;
}

/* int __acl_delete_link(const char *path, acl_type_t type); */
static inline abi_long do_freebsd__acl_delete_link(abi_ulong arg1,
        abi_long arg2)
{

    qemu_log("qemu: Unsupported syscall __acl_delete_link()\n");
    return -TARGET_ENOSYS;
}

/* int __acl_get_fd(int filedes, acl_type_t type, struct acl *aclp); */
static inline abi_long do_freebsd__acl_get_fd(abi_long arg1, abi_long arg2,
        abi_ulong arg3)
{

    qemu_log("qemu: Unsupported syscall __acl_get_fd()\n");
    return -TARGET_ENOSYS;
}

/* __acl_get_file(const char *path, acl_type_t type, struct acl *aclp); */
static inline abi_long do_freebsd__acl_get_file(abi_ulong arg1, abi_long arg2,
       abi_ulong arg3)
{

    qemu_log("qemu: Unsupported syscall __acl_get_file()\n");
    return -TARGET_ENOSYS;
}

/* int __acl_get_link(const char *path, acl_type_t type, struct acl *aclp); */
static inline abi_long do_freebsd__acl_get_link(abi_ulong arg1, abi_long arg2,
       abi_ulong arg3)
{

    qemu_log("qemu: Unsupported syscall _acl_get_link()\n");
    return -TARGET_ENOSYS;
}

/* int __acl_get_fd(int filedes, acl_type_t type, struct acl *aclp); */
static inline abi_long do_freebsd__acl_set_fd(abi_long arg1, abi_long arg2,
        abi_ulong arg3)
{

    qemu_log("qemu: Unsupported syscall __acl_set_fd()\n");
    return -TARGET_ENOSYS;
}

/* int __acl_set_file(const char *path, acl_type_t type, struct acl *aclp); */
static inline abi_long do_freebsd__acl_set_file(abi_ulong arg1, abi_long arg2,
       abi_ulong arg3)
{

    qemu_log("qemu: Unsupported syscall __acl_set_file()\n");
    return -TARGET_ENOSYS;
}

/* int __acl_set_link(const char *path, acl_type_t type, struct acl *aclp); */
static inline abi_long do_freebsd__acl_set_link(abi_ulong arg1, abi_long arg2,
       abi_ulong arg3)
{

    qemu_log("qemu: Unsupported syscall __acl_set_link()\n");
    return -TARGET_ENOSYS;
}

