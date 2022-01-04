# objdump: -dr
tmpdir/insn-brcc.o:     file format elf64-dadao


Disassembly of section .text:

0000000000000000 <_start>:
   0:	28040000 	brn	rd1, 4 <_start+0x4>
   4:	280bffff 	brn	rd2, 4 <_start+0x4>
   8:	280ffffd 	brn	rd3, 0 <_start>
   c:	2a040000 	brz	rd1, 10 <_start+0x10>
  10:	2a0bffff 	brz	rd2, 10 <_start+0x10>
  14:	2a0ffffa 	brz	rd3, 0 <_start>
  18:	2c040000 	brp	rd1, 1c <_start+0x1c>
  1c:	2c0bffff 	brp	rd2, 1c <_start+0x1c>
  20:	2c0ffff7 	brp	rd3, 0 <_start>
  24:	2e040000 	brod	rd1, 28 <_start+0x28>
  28:	2e0bffff 	brod	rd2, 28 <_start+0x28>
  2c:	2e0ffff4 	brod	rd3, 0 <_start>
  30:	29040000 	brnn	rd1, 34 <_start+0x34>
  34:	290bffff 	brnn	rd2, 34 <_start+0x34>
  38:	290ffff1 	brnn	rd3, 0 <_start>
  3c:	2b040000 	brnz	rd1, 40 <_start+0x40>
  40:	2b0bffff 	brnz	rd2, 40 <_start+0x40>
  44:	2b0fffee 	brnz	rd3, 0 <_start>
  48:	2d040000 	brnp	rd1, 4c <_start+0x4c>
  4c:	2d0bffff 	brnp	rd2, 4c <_start+0x4c>
  50:	2d0fffeb 	brnp	rd3, 0 <_start>
  54:	2f040000 	brev	rd1, 58 <_start+0x58>
  58:	2f0bffff 	brev	rd2, 58 <_start+0x58>
  5c:	2f0fffe8 	brev	rd3, 0 <_start>
