# objdump: -dr
tmpdir/insn-brcc.o:     file format elf64-dadao


Disassembly of section .text:

0000000000000000 <_start>:
   0:	28040002 	brn	rd1, 8 <_rec1>
   4:	280bffff 	brn	rd2, 0 <_start>

0000000000000008 <_rec1>:
   8:	2a040000 	brz	rd1, 8 <_rec1>
   c:	2a080001 	brz	rd2, 10 <_rec2>

0000000000000010 <_rec2>:
  10:	2c040000 	brp	rd1, 10 <_rec2>
  14:	2c080001 	brp	rd2, 18 <_rec3>

0000000000000018 <_rec3>:
  18:	2e040000 	brod	rd1, 18 <_rec3>
  1c:	2e080001 	brod	rd2, 20 <_rec4>

0000000000000020 <_rec4>:
  20:	29040000 	brnn	rd1, 20 <_rec4>
  24:	29080001 	brnn	rd2, 28 <_rec5>

0000000000000028 <_rec5>:
  28:	2b040000 	brnz	rd1, 28 <_rec5>
  2c:	2b080001 	brnz	rd2, 30 <_rec6>

0000000000000030 <_rec6>:
  30:	2d040000 	brnp	rd1, 30 <_rec6>
  34:	2d080001 	brnp	rd2, 38 <_rec7>

0000000000000038 <_rec7>:
  38:	2f040001 	brev	rd1, 3c <_rec8>

000000000000003c <_rec8>:
  3c:	2f0bfff1 	brev	rd2, 0 <_start>
