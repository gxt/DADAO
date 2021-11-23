# objdump: -dr
tmpdir/insn-getput.o:     file format elf64-dadao


Disassembly of section .text:

0000000000000000 <_start>:
   0:	da843040 	get.rp	rg3, sp
   4:	da883080 	get.rf	rg3, rf2
   8:	da8c30c0 	get.rv	rg3, rv3
   c:	da903100 	get.rs	rg3, rJ
  10:	da441080 	put.rp	sp, rg2
  14:	da482080 	put.rf	rf2, rg2
  18:	da4c3080 	put.rv	rv3, rg2
  1c:	da504080 	put.rs	rJ, rg2
