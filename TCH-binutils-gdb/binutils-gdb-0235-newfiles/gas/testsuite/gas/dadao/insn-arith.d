# objdump: -dr
tmpdir/insn-arith.o:     file format elf64-dadao


Disassembly of section .text:

0000000000000000 <_start>:
   0:	190427ff 	addi	rd1, rd2, 2047
   4:	19042800 	addi	rd1, rd2, -2048
   8:	490427ff 	addi	rb1, rb2, 2047
   c:	49042800 	addi	rb1, rb2, -2048
  10:	1a0420c4 	add	rd1, rd2, rd3, rd4
  14:	10b81041 	add	rb1, rb1, rd1
  18:	1b0420c4 	sub	rd1, rd2, rd3, rd4
  1c:	10bc1041 	sub	rb1, rb1, rd1
  20:	1c0420c4 	muls	rd1, rd2, rd3, rd4
  24:	1d0420c4 	mulu	rd1, rd2, rd3, rd4
  28:	1e0420c4 	divs	rd1, rd2, rd3, rd4
  2c:	1f0420c4 	divu	rd1, rd2, rd3, rd4
  30:	10201083 	and	rd1, rd2, rd3
  34:	10241083 	orr	rd1, rd2, rd3
  38:	10281083 	xor	rd1, rd2, rd3
  3c:	18042fff 	andi	rd1, rd2, 0xfff
  40:	120427ff 	cmps	rd1, rd2, 2047
  44:	12042800 	cmps	rd1, rd2, -2048
  48:	13042fff 	cmpu	rd1, rd2, 0xfff
  4c:	10901083 	cmps	rd1, rd2, rd3
  50:	10941083 	cmpu	rd1, rd2, rd3
  54:	10b41042 	cmp	rd1, rb1, rb2
  58:	4805ffff 	adrp	rb1, 131071
  5c:	48060000 	adrp	rb1, -131072
