fsdev_ss = ss.source_set()
fsdev_ss.add(files('qemu-fsdev-opts.c', 'qemu-fsdev-throttle.c'))
fsdev_ss.add(when: 'CONFIG_ALL', if_true: files('qemu-fsdev-dummy.c'))
fsdev_ss.add(when: ['CONFIG_FSDEV_9P'], if_true: files(
  '9p-iov-marshal.c',
  '9p-marshal.c',
  'qemu-fsdev.c',
), if_false: files('qemu-fsdev-dummy.c'))

if host_machine.system() == 'darwin' or host_machine.system() == 'linux'
  softmmu_ss.add_all(fsdev_ss)
endif


have_virtfs_proxy_helper_deps = false
virtfs_proxy_helper_extra_sources = []
if host_machine.system() == 'darwin'
  # macOS systems always have the required libraries
  have_virtfs_proxy_helper_deps = true
  virtfs_proxy_helper_extra_sources = files('../hw/9pfs/9p-util-darwin.c')
elif host_machine.system() == 'linux'
  have_virtfs_proxy_helper_deps = libattr.found() and libcap_ng.found()
  virtfs_proxy_helper_extra_sources = files('../hw/9pfs/9p-util-linux.c')
else
  warning('virtfs_proxy_helper not support on ' + host_machine.system())
endif

have_virtfs_proxy_helper = have_tools and have_virtfs_proxy_helper_deps and 'CONFIG_VIRTFS' in config_host
if have_virtfs_proxy_helper
  virtfs_prox_helper_sources = files('virtfs-proxy-helper.c', '9p-marshal.c', '9p-iov-marshal.c')
  executable('virtfs-proxy-helper',
             virtfs_prox_helper_sources + virtfs_proxy_helper_extra_sources,
             dependencies: [qemuutil, libattr, libcap_ng],
             install: true,
             install_dir: get_option('libexecdir'))
endif
