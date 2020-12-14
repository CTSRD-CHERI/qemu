mips_ss = ss.source_set()
mips_ss.add(files(
  'cpu.c',
  'dsp_helper.c',
  'fpu_helper.c',
  'gdbstub.c',
  'helper.c',
  'lmmi_helper.c',
  'msa_helper.c',
  'op_helper.c',
  'op_helper_beri.c',
  'translate.c',
))
mips_ss.add(when: 'CONFIG_KVM', if_true: files('kvm.c'))

mips_ss.add(when: 'TARGET_CHERI', if_true: files('op_helper_cheri.c'))
mips_ss.add(when: ['CONFIG_TCG', 'CONFIG_TCG_LOG_INSTR'], if_true: files('op_helper_log_instr.c'))

mips_softmmu_ss = ss.source_set()
mips_softmmu_ss.add(files(
  'addr.c',
  'cp0_helper.c',
  'cp0_timer.c',
  'machine.c',
  'mips-semi.c',
))

target_arch += {'mips': mips_ss}
target_softmmu_arch += {'mips': mips_softmmu_ss}
