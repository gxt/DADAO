# objdump: -dr
tmpdir/insn-brcc.o:     file format elf64-dadao


Disassembly of section .text:

0000000000000000 <_start>:
   0:	c0040000 	br.n	rg1, 4 <_start\+0x4>
   4:	c00bffff 	br.n	rg2, 4 <_start\+0x4>
   8:	c00ffffd 	br.n	rg3, 0 <_start>
   c:	c2040000 	br.z	rg1, 10 <_start\+0x10>
  10:	c20bffff 	br.z	rg2, 10 <_start\+0x10>
  14:	c20ffffa 	br.z	rg3, 0 <_start>
  18:	c4040000 	br.p	rg1, 1c <_start\+0x1c>
  1c:	c40bffff 	br.p	rg2, 1c <_start\+0x1c>
  20:	c40ffff7 	br.p	rg3, 0 <_start>
  24:	c6040000 	br.od	rg1, 28 <_start\+0x28>
  28:	c60bffff 	br.od	rg2, 28 <_start\+0x28>
  2c:	c60ffff4 	br.od	rg3, 0 <_start>
  30:	c1040000 	br.nn	rg1, 34 <_start\+0x34>
  34:	c10bffff 	br.nn	rg2, 34 <_start\+0x34>
  38:	c10ffff1 	br.nn	rg3, 0 <_start>
  3c:	c3040000 	br.nz	rg1, 40 <_start\+0x40>
  40:	c30bffff 	br.nz	rg2, 40 <_start\+0x40>
  44:	c30fffee 	br.nz	rg3, 0 <_start>
  48:	c5040000 	br.np	rg1, 4c <_start\+0x4c>
  4c:	c50bffff 	br.np	rg2, 4c <_start\+0x4c>
  50:	c50fffeb 	br.np	rg3, 0 <_start>
  54:	c7040000 	br.ev	rg1, 58 <_start\+0x58>
  58:	c70bffff 	br.ev	rg2, 58 <_start\+0x58>
  5c:	c70fffe8 	br.ev	rg3, 0 <_start>
