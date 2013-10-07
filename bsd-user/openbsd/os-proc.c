/*
 * XXX To support FreeBSD binaries on NetBSD the following will need to be
 * emulated.
 */
abi_long freebsd_exec_common(abi_ulong path_or_fd, abi_ulong guest_argp,
        abi_ulong guest_envp, int do_fexec)
{

    qemu_log("qemu: Unsupported %s\n", __func__);
    return -TARGET_ENOSYS;
}
