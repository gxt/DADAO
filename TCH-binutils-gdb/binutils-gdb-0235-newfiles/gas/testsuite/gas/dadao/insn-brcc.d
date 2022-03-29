# objdump: -dr
tmpdir/insn-brcc.o:     file format elf64-dadao


Disassembly of section .text:

0000000000000000 <_start>:
   0:	28040001 	brn	rd1, 4 <_start+0x4>
   4:	28080000 	brn	rd2, 4 <_start+0x4>
   8:	280ffffe 	brn	rd3, 0 <_start>
   c:	2a040001 	brz	rd1, 10 <_start+0x10>
  10:	2a080000 	brz	rd2, 10 <_start+0x10>
  14:	2a0ffffb 	brz	rd3, 0 <_start>
  18:	2c040001 	brp	rd1, 1c <_start+0x1c>
  1c:	2c080000 	brp	rd2, 1c <_start+0x1c>
  20:	2c0ffff8 	brp	rd3, 0 <_start>
  24:	2e040001 	brod	rd1, 28 <_start+0x28>
  28:	2e080000 	brod	rd2, 28 <_start+0x28>
  2c:	2e0ffff5 	brod	rd3, 0 <_start>
  30:	29040001 	brnn	rd1, 34 <_start+0x34>
  34:	29080000 	brnn	rd2, 34 <_start+0x34>
  38:	290ffff2 	brnn	rd3, 0 <_start>
  3c:	2b040001 	brnz	rd1, 40 <_start+0x40>
  40:	2b080000 	brnz	rd2, 40 <_start+0x40>
  44:	2b0fffef 	brnz	rd3, 0 <_start>
  48:	2d040001 	brnp	rd1, 4c <_start+0x4c>
  4c:	2d080000 	brnp	rd2, 4c <_start+0x4c>
  50:	2d0fffec 	brnp	rd3, 0 <_start>
  54:	2f040001 	brev	rd1, 58 <_start+0x58>
  58:	2f080000 	brev	rd2, 58 <_start+0x58>
  5c:	2f0fffe9 	brev	rd3, 0 <_start>
