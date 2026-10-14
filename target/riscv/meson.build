# FIXME extra_args should accept files()
dir = meson.current_source_dir()

gen_nocheri = [
  decodetree.process('insn16.decode', extra_args: ['--static-decode=decode_insn16', '--insnwidth=16']),
  decodetree.process('insn32.decode', extra_args: [dir / 'insn32-rv128.decode', '--static-decode=decode_insn32']),
  decodetree.process('XVentanaCondOps.decode', extra_args: '--static-decode=decode_XVentanaCodeOps'),
]

# FIXME: would be nice to avoid this duplication but I don't see an easy way to
# do this since the TARGET_CHERI check is evaulated later.
# Work around meson generators not being able to depend on multiple files by
# making a copy of the decodetree() generator that has dependencies
fake_dep = custom_target('fake-cheri-insn-dep', command: [find_program('cat'), '@INPUT@'],
                         output: 'fake-output.decode', capture: true,
                         input: ['insn32.decode', 'insn32-cheri-std.decode',
                                 'insn32-cheri-v9.decode', 'insn32-cheri-v9-32.decode',
                                 'insn32-cheri-v9-64.decode'])
decodetree_w_cheri_deps = generator(
  decodetree_py,
  output: 'decode-@BASENAME@.c.inc',
  arguments: ['@INPUT@', '@EXTRA_ARGS@', '-o', '@OUTPUT@'],
  depends: fake_dep, # Workaround for bad dependency tracking
)
gen_xcheri32 = [
  decodetree.process('insn16.decode', extra_args: ['--static-decode=decode_insn16', '--insnwidth=16']),
  decodetree_w_cheri_deps.process('insn32.decode', extra_args: [dir / 'insn32-cheri-v9.decode', dir / 'insn32-cheri-v9-32.decode', '--static-decode=decode_insn32']),
  decodetree.process('XVentanaCondOps.decode', extra_args: '--static-decode=decode_XVentanaCodeOps'),
]
gen_xcheri64 = [
  decodetree.process('insn16.decode', extra_args: ['--static-decode=decode_insn16', '--insnwidth=16']),
  decodetree_w_cheri_deps.process('insn32.decode', extra_args: [dir / 'insn32-cheri-v9.decode', dir / 'insn32-cheri-v9-64.decode', '--static-decode=decode_insn32']),
  decodetree.process('XVentanaCondOps.decode', extra_args: '--static-decode=decode_XVentanaCodeOps'),
]
gen_stdcheri = [
  decodetree.process('insn16.decode', extra_args: ['--static-decode=decode_insn16', '--insnwidth=16']),
  decodetree_w_cheri_deps.process('insn32.decode', extra_args: [dir / 'insn32-cheri-std.decode', '--static-decode=decode_insn32']),
  decodetree.process('XVentanaCondOps.decode', extra_args: '--static-decode=decode_XVentanaCodeOps'),
]

cheri_ss = ss.source_set()
cheri_ss.add(when: ['TARGET_CHERI_RISCV_V9', 'TARGET_RISCV32'], if_true: gen_xcheri32)
cheri_ss.add(when: ['TARGET_CHERI_RISCV_V9', 'TARGET_RISCV64'], if_true: gen_xcheri64)
cheri_ss.add(when: 'TARGET_CHERI_RISCV_STD', if_true: gen_stdcheri)

riscv_ss = ss.source_set()
riscv_ss.add(when: 'TARGET_CHERI', if_false: gen_nocheri)
riscv_ss.add_all(when: 'TARGET_CHERI', if_true: cheri_ss)

riscv_ss.add(files(
  'cpu.c',
  'cpu_helper.c',
  'csr.c',
  'fpu_helper.c',
  'gdbstub.c',
  'op_helper.c',
  'vector_helper.c',
  'bitmanip_helper.c',
  'translate.c',
  'm128_helper.c',
  'crypto_helper.c'
))
riscv_ss.add(when: 'TARGET_CHERI', if_true: files('op_helper_cheri.c'))
riscv_ss.add(when: ['CONFIG_TCG', 'CONFIG_TCG_LOG_INSTR'], if_true: files('op_helper_log_instr.c'))
riscv_ss.add(when: 'CONFIG_KVM', if_true: files('kvm.c'), if_false: files('kvm-stub.c'))
riscv_ss.add(when: 'CONFIG_RVFI_DII', if_true: files('rvfi_dii.c'))

riscv_softmmu_ss = ss.source_set()
riscv_softmmu_ss.add(files(
  'arch_dump.c',
  'pmp.c',
  'debug.c',
  'monitor.c',
  'machine.c',
  'pmu.c',
  'time_helper.c'
))

target_arch += {'riscv': riscv_ss}
target_softmmu_arch += {'riscv': riscv_softmmu_ss}
