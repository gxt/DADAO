# objdump: -dr
tmpdir/insn-seto.o:     file format elf64-dadao


Disassembly of section .text:

0000000000000000 <_start>:
   0:	3a05abcd 	setwk	\$1, 0xabcd
   4:	3b04abcd 	incwl	\$1, 0xabcd
   8:	3a0a0013 	setwj	\$2, 0x13
   c:	3b095792 	incwk	\$2, 0x5792
  10:	3b084680 	incwl	\$2, 0x4680
  14:	3a131234 	setwh	\$4, 0x1234
  18:	3b125678 	incwj	\$4, 0x5678
  1c:	3b111234 	incwk	\$4, 0x1234
  20:	3b105678 	incwl	\$4, 0x5678
  24:	3a140000 	setwl	\$5, 0x0
