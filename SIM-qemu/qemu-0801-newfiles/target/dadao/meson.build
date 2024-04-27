gen = decodetree.process('insns.decode', extra_args: '--decode=disas_dadao')

dadao_ss = ss.source_set()
dadao_ss.add(gen)
dadao_ss.add(files(
  'cpu.c',
  'helper.c',
  'helper-fpu.c',
  'softmmu.c',
  'translate.c',
))

dadao_softmmu_ss = ss.source_set()
dadao_softmmu_ss.add(files(
  
))

target_arch += {'dadao': dadao_ss}
target_softmmu_arch += {'dadao': dadao_softmmu_ss}
