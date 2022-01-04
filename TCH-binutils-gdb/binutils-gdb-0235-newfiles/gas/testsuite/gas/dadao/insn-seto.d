# objdump: -dr
tmpdir/insn-seto.o:     file format elf64-dadao


Disassembly of section .text:

0000000000000000 <_start>:
   0:	1504abcd 	setzwl	rd1, 0xabcd
   4:	1605abcd 	orwk	rd1, 0xabcd
   8:	15084680 	setzwl	rd2, 0x4680
   c:	16095792 	orwk	rd2, 0x5792
  10:	160a0013 	orwj	rd2, 0x13
  14:	15105678 	setzwl	rd4, 0x5678
  18:	16111234 	orwk	rd4, 0x1234
  1c:	16125678 	orwj	rd4, 0x5678
  20:	16131234 	orwh	rd4, 0x1234
  24:	15140000 	setzwl	rd5, 0x0
  28:	1418ffff 	setowl	rd6, 0xffff
  2c:	151cfffe 	setzwl	rd7, 0xfffe
  30:	161dffff 	orwk	rd7, 0xffff
  34:	1520ffff 	setzwl	rd8, 0xffff
  38:	1622ffff 	orwj	rd8, 0xffff
  3c:	14240000 	setowl	rd9, 0x0
  40:	17260000 	andnwj	rd9, 0x0