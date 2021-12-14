# objdump: -dr
tmpdir/insn-brcc.o:     file format elf64-dadao


Disassembly of section .text:

0000000000000000 <_start>:
   0:	28040000 	brn	rg1, 4 <_start+0x4>
   4:	280bffff 	brn	rg2, 4 <_start+0x4>
   8:	280ffffd 	brn	rg3, 0 <_start>
   c:	2a040000 	brz	rg1, 10 <_start+0x10>
  10:	2a0bffff 	brz	rg2, 10 <_start+0x10>
  14:	2a0ffffa 	brz	rg3, 0 <_start>
  18:	2c040000 	brp	rg1, 1c <_start+0x1c>
  1c:	2c0bffff 	brp	rg2, 1c <_start+0x1c>
  20:	2c0ffff7 	brp	rg3, 0 <_start>
  24:	2e040000 	brod	rg1, 28 <_start+0x28>
  28:	2e0bffff 	brod	rg2, 28 <_start+0x28>
  2c:	2e0ffff4 	brod	rg3, 0 <_start>
  30:	29040000 	brnn	rg1, 34 <_start+0x34>
  34:	290bffff 	brnn	rg2, 34 <_start+0x34>
  38:	290ffff1 	brnn	rg3, 0 <_start>
  3c:	2b040000 	brnz	rg1, 40 <_start+0x40>
  40:	2b0bffff 	brnz	rg2, 40 <_start+0x40>
  44:	2b0fffee 	brnz	rg3, 0 <_start>
  48:	2d040000 	brnp	rg1, 4c <_start+0x4c>
  4c:	2d0bffff 	brnp	rg2, 4c <_start+0x4c>
  50:	2d0fffeb 	brnp	rg3, 0 <_start>
  54:	2f040000 	brev	rg1, 58 <_start+0x58>
  58:	2f0bffff 	brev	rg2, 58 <_start+0x58>
  5c:	2f0fffe8 	brev	rg3, 0 <_start>
