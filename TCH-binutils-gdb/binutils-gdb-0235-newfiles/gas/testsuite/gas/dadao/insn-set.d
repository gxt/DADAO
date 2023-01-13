# objdump: -dr
tmpdir/insn-set.o:     file format elf64-dadao


Disassembly of section .text:

0000000000000000 <_start>:
   0:	1604abcd 	setzw	rd1, w0, 0xabcd
   4:	1405abcd 	orw	rd1, w1, 0xabcd
   8:	16084680 	setzw	rd2, w0, 0x4680
   c:	14095792 	orw	rd2, w1, 0x5792
  10:	140a0013 	orw	rd2, w2, 0x13
  14:	16105678 	setzw	rd4, w0, 0x5678
  18:	14111234 	orw	rd4, w1, 0x1234
  1c:	14125678 	orw	rd4, w2, 0x5678
  20:	14131234 	orw	rd4, w3, 0x1234
  24:	16140000 	setzw	rd5, w0, 0x0
  28:	1718ffff 	setow	rd6, w0, 0xffff
  2c:	161cfffe 	setzw	rd7, w0, 0xfffe
  30:	141dffff 	orw	rd7, w1, 0xffff
  34:	1620ffff 	setzw	rd8, w0, 0xffff
  38:	1422ffff 	orw	rd8, w2, 0xffff
  3c:	17240000 	setow	rd9, w0, 0x0
  40:	1526ffff 	andnw	rd9, w2, 0xffff
