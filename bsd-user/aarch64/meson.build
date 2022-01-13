bsd_user_ss.add(when: 'TARGET_AARCH64', if_true: files(
  'target_arch_cpu.c',
))

bsd_user_ss.add(when: ['TARGET_AARCH64', 'TARGET_CHERI'], if_true: files(
  'cheri/cheri_exception.c',
  'cheri/cheri_machdep.c',
))
