# objdump: -dr
tmpdir/insn-getput.o:     file format elf64-dadao


Disassembly of section .text:

0000000000000000 <_start>:
   0:	da803040 	get.rp	rg3, sp
   4:	da843080 	get.rf	rg3, rf2
   8:	da8830c0 	get.rv	rg3, rv3
   c:	da8c3100 	get.rs	rg3, rJ
  10:	da401080 	put.rp	sp, rg2
  14:	da442080 	put.rf	rf2, rg2
  18:	da483080 	put.rv	rv3, rg2
  1c:	da4c4080 	put.rs	rJ, rg2
