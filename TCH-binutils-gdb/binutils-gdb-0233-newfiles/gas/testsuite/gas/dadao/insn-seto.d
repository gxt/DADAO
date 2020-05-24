# objdump: -dr
tmpdir/insn-seto.o:     file format elf64-dadao


Disassembly of section .text:

0000000000000000 <_start>:
   0:	3a05abcd 	setml	\$1, 0xabcd
   4:	3b04abcd 	incl	\$1, 0xabcd
   8:	3a0a0013 	setmh	\$2, 0x13
   c:	3b095792 	incml	\$2, 0x5792
  10:	3b084680 	incl	\$2, 0x4680
  14:	3a131234 	seth	\$4, 0x1234
  18:	3b125678 	incmh	\$4, 0x5678
  1c:	3b111234 	incml	\$4, 0x1234
  20:	3b105678 	incl	\$4, 0x5678
  24:	3a140000 	setl	\$5, 0x0
