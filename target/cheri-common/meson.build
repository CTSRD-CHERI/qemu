specific_ss.add(when: 'TARGET_CHERI', if_true: files(
  'cheri_gdbstub.c',
  'op_helper_cheri_common.c',
))
specific_ss.add(when: ['TARGET_CHERI', 'CONFIG_SOFTMMU'], if_true: files(
  'cheri_tagmem.c',
))

specific_ss.add(when: ['TARGET_CHERI', 'CONFIG_USER_ONLY'], if_true: files(
  'cheri_tagtree.c',
))
