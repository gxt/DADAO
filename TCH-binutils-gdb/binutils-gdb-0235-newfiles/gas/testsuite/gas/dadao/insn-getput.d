# objdump: -dr
tmpdir/insn-getput.o:     file format elf64-dadao


Disassembly of section .text:

0000000000000000 <_start>:
   0:	10a0107f 	rd2rd	rd1, rd1, 0x3f
   4:	10a4107f 	rd2rb	rb1, rd1, 0x3f
   8:	10a8107f 	rb2rd	rd1, rb1, 0x3f
   c:	10ac107f 	rb2rb	rb1, rb1, 0x3f
  10:	10c4107f 	rd2rf	rf1, rd1, 0x3f
  14:	10c8107f 	rf2rd	rd1, rf1, 0x3f
  18:	10cc107f 	rf2rf	rf1, rf1, 0x3f
  1c:	10e4107f 	rd2ra	ra1, rd1, 0x3f
  20:	10e8107f 	ra2rd	rd1, ra1, 0x3f
  24:	200420c4 	csn	rd1, rd2, rd3, rd4
  28:	220420c4 	csz	rd1, rd2, rd3, rd4
  2c:	240420c4 	csp	rd1, rd2, rd3, rd4
  30:	260420c4 	cseq	rd1, rd2, rd3, rd4
  34:	270420c4 	csne	rd1, rd2, rd3, rd4
