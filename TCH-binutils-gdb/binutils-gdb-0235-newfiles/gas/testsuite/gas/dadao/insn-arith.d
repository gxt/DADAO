# objdump: -dr
tmpdir/insn-arith.o:     file format elf64-dadao


Disassembly of section .text:

0000000000000000 <_start>:
   0:	1905ffff 	add	rd1, 131071
   4:	19060000 	add	rd1, -131072
   8:	1a0420c4 	add	rd1, rd2, rd3, rd4
   c:	1b0420c4 	sub	rd1, rd2, rd3, rd4
  10:	1c0420c4 	muls	rd1, rd2, rd3, rd4
  14:	1d0420c4 	mulu	rd1, rd2, rd3, rd4
  18:	1e0420c4 	divs	rd1, rd2, rd3, rd4
  1c:	1f0420c4 	divu	rd1, rd2, rd3, rd4
  20:	10201083 	and	rd1, rd2, rd3
  24:	10241083 	orr	rd1, rd2, rd3
  28:	10281083 	xor	rd1, rd2, rd3
  2c:	120427ff 	cmps	rd1, rd2, 2047
  30:	12042800 	cmps  rd1, rd2, -2048
  34:	13042fff 	cmpu  rd1, rd2, 4095
  38:	10901083 	cmps  rd1, rd2, rd3
  3c:	10941083 	cmpu	rd1, rd2, rd3
