# objdump: -dr
tmpdir/insn-getput.o:     file format elf64-dadao


Disassembly of section .text:

0000000000000000 <_start>:
   0:	10a0107f 	rd2rd	rd1, rd1, 0x3f
   4:	10a4107f 	rd2rb	rb1, rd1, 0x3f
   8:	10a8107f 	rb2rd	rd1, rb1, 0x3f
   c:	10ac107f 	rb2rb	rb1, rb1, 0x3f
  10:	10b41042 	cmp	rd1, rb1, rb2
  14:	10b81081 	add	rb1, rb2, rd1
  18:	10bc1081 	sub	rb1, rb2, rd1
  1c:	10c4107f 	rd2rf	rf1, rd1, 0x3f
  20:	10c8107f 	rf2rd	rd1, rf1, 0x3f
  24:	10cc107f 	rf2rf	rf1, rf1, 0x3f
  28:	10e4107f 	rd2ra	ra1, rd1, 0x3f
  2c:	10e8107f 	ra2rd	rd1, ra1, 0x3f
