# objdump: -dr
tmpdir/insn-arith.o:     file format elf64-dadao


Disassembly of section .text:

0000000000000000 <_start>:
   0:	1105ffff 	add	rg1, 131071
   4:	11060000 	add	rg1, -131072
   8:	120420c4 	add	rg1, rg2, rg3, rg4
   c:	130420c4 	sub	rg1, rg2, rg3, rg4
  10:	140420c4 	muls	rg1, rg2, rg3, rg4
  14:	150420c4 	mulu	rg1, rg2, rg3, rg4
  18:	160420c4 	divs	rg1, rg2, rg3, rg4
  1c:	170420c4 	divu	rg1, rg2, rg3, rg4
  20:	10201083 	and	rg1, rg2, rg3
  24:	10241083 	orr	rg1, rg2, rg3
  28:	10281083 	xor	rg1, rg2, rg3
  2c:	1a0427ff 	cmps	rg1, rg2, 2047
  30:	1a042800 	cmps  rg1, rg2, -2048
  34:	1b042fff 	cmpu  rg1, rg2, 4095
  38:	10901083 	cmps  rg1, rg2, rg3
  3c:	10941083 	cmpu	rg1, rg2, rg3