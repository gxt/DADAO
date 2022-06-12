# objdump: -dr
tmpdir/insn-set.o:     file format elf64-dadao


Disassembly of section .text:

0000000000000000 <_start>:
   0:	1604abcd 	setzwl	rd1, 0xabcd
   4:	1405abcd 	orwk	rd1, 0xabcd
   8:	16084680 	setzwl	rd2, 0x4680
   c:	14095792 	orwk	rd2, 0x5792
  10:	140a0013 	orwj	rd2, 0x13
  14:	16105678 	setzwl	rd4, 0x5678
  18:	14111234 	orwk	rd4, 0x1234
  1c:	14125678 	orwj	rd4, 0x5678
  20:	14131234 	orwh	rd4, 0x1234
  24:	16140000 	setzwl	rd5, 0x0
  28:	1718ffff 	setowl	rd6, 0xffff
  2c:	161cfffe 	setzwl	rd7, 0xfffe
  30:	141dffff 	orwk	rd7, 0xffff
  34:	1620ffff 	setzwl	rd8, 0xffff
  38:	1422ffff 	orwj	rd8, 0xffff
  3c:	17240000 	setowl	rd9, 0x0
  40:	15260000 	andnwj	rd9, 0x0
