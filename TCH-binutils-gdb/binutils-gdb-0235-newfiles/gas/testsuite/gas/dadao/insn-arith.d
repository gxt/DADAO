insn-arith.d
# objdump: -dr
tmpdir/insn-arith.o:     file format elf64-dadao


Disassembly of section .text:

0000000000000000 <_start>:
   0:	1805ffff 	addrb	rb1, 131071
   4:	18060000 	addrb	rb1, -131072
   8:	1905ffff 	add	rd1, 131071
   c:	19060000 	add	rd1, -131072
  10:	1a0420c4 	add	rd1, rd2, rd3, rd4
  14:	1b0420c4 	sub	rd1, rd2, rd3, rd4
  18:	1c0420c4 	muls	rd1, rd2, rd3, rd4
  1c:	1d0420c4 	mulu	rd1, rd2, rd3, rd4
  20:	1e0420c4 	divs	rd1, rd2, rd3, rd4
  24:	1f0420c4 	divu	rd1, rd2, rd3, rd4
  28:	10201083 	and	rd1, rd2, rd3
  2c:	10241083 	orr	rd1, rd2, rd3
  30:	10281083 	xor	rd1, rd2, rd3
  34:	120427ff 	cmps	rd1, rd2, 2047
  38:	12042800 	cmps	rd1, rd2, -2048
  3c:	13042fff 	cmpu	rd1, rd2, 4095
  40:	10901083 	cmps	rd1, rd2, rd3
  44:	10941083 	cmpu	rd1, rd2, rd3
