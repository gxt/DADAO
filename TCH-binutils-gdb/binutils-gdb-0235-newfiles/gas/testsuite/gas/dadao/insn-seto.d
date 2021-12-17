# objdump: -dr
tmpdir/insn-seto.o:     file format elf64-dadao


Disassembly of section .text:

0000000000000000 <_start>:
   0:	1504abcd 	setzwl	rg1, 0xabcd
   4:	1605abcd 	orwk	rg1, 0xabcd
   8:	15084680 	setzwl	rg2, 0x4680
   c:	16095792 	orwk	rg2, 0x5792
  10:	160a0013 	orwj	rg2, 0x13
  14:	15105678 	setzwl	rg4, 0x5678
  18:	16111234 	orwk	rg4, 0x1234
  1c:	16125678 	orwj	rg4, 0x5678
  20:	16131234 	orwh	rg4, 0x1234
  24:	15140000 	setzwl	rg5, 0x0
  28:	1418ffff 	setowl	rg6, 0xffff
  2c:	151cfffe 	setzwl	rg7, 0xfffe
  30:	161dffff 	orwk	rg7, 0xffff
  34:	1520ffff 	setzwl	rg8, 0xffff
  38:	1622ffff 	orwj	rg8, 0xffff
  3c:	14240000 	setowl	rg9, 0x0
  40:	17260000 	andnwj	rg9, 0x0