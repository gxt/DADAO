# objdump: -dr
tmpdir/insn-getput.o:     file format elf64-dadao


Disassembly of section .text:

0000000000000000 <_start>:
   0:	10a0003f 	rd2rd	rd0, rd0, 0x3f
   4:	10a4003f 	rd2rb	rb0, rd0, 0x3f
   8:	10a8003f 	rb2rd	rd0, rb0, 0x3f
   c:	10ac003f 	rb2rb	rb0, rb0, 0x3f
  10:	10c4003f 	rd2rf	rf0, rd0, 0x3f
  14:	10c8003f 	rf2rd	rd0, rf0, 0x3f
  18:	10cc003f 	rf2rf	rf0, rf0, 0x3f
  1c:	10e4003f 	rd2ra	ra0, rd0, 0x3f
  20:	10e8003f 	ra2rd	rd0, ra0, 0x3f
  24:	200420c4 	csn	rd1, rd2, rd3, rd4
  28:	220420c4 	csz	rd1, rd2, rd3, rd4
  2c:	240420c4 	csp	rd1, rd2, rd3, rd4
  30:	260420c4 	cseq	rd1, rd2, rd3, rd4
  34:	270420c4 	csne	rd1, rd2, rd3, rd4
