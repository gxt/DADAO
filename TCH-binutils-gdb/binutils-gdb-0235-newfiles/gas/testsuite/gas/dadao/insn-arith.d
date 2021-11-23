# objdump: -dr
tmpdir/insn-arith.o:     file format elf64-dadao


Disassembly of section .text:

0000000000000000 <_start>:
   0:	210420c0 	add	rg1, rg2, rg3
   4:	210420df 	add	rg1, rg2, rg3 << 31
   8:	200427ff 	add	rg1, rg2, 2047
   c:	20042800 	add	rg1, rg2, -2048
  10:	230420c0 	addu	rg1, rg2, rg3
  14:	230420df 	addu	rg1, rg2, rg3 << 31
  18:	220427ff 	addu	rg1, rg2, 2047
  1c:	22042fff 	addu	rg1, rg2, 4095
  20:	250420c0 	sub	rg1, rg2, rg3
  24:	250420df 	sub	rg1, rg2, rg3 << 31
  28:	240427ff 	sub	rg1, rg2, 2047
  2c:	24042800 	sub	rg1, rg2, -2048
  30:	270420c0 	subu	rg1, rg2, rg3
  34:	270420df 	subu	rg1, rg2, rg3 << 31
  38:	260427ff 	subu	rg1, rg2, 2047
  3c:	26042fff 	subu	rg1, rg2, 4095
  40:	310420c0 	mul	rg1, rg2, rg3
  44:	310420df 	mul	rg1, rg2, rg3 << 31
  48:	300427ff 	mul	rg1, rg2, 2047
  4c:	30042800 	mul	rg1, rg2, -2048
  50:	350420c4 	mulu	rg1, rg2, rg3, rg4
  54:	360420c4 	div	rg1, rg2, rg3, rg4
  58:	370420c4 	divu	rg1, rg2, rg3, rg4
  5c:	410420c0 	or	rg1, rg2, rg3
  60:	410420df 	or	rg1, rg2, rg3 << 31
  64:	400427ff 	or	rg1, rg2, 2047
  68:	40042fff 	or	rg1, rg2, 4095
  6c:	430420c0 	and	rg1, rg2, rg3
  70:	430420df 	and	rg1, rg2, rg3 << 31
  74:	420427ff 	and	rg1, rg2, 2047
  78:	42042fff 	and	rg1, rg2, 4095
  7c:	450420c0 	xor	rg1, rg2, rg3
  80:	450420df 	xor	rg1, rg2, rg3 << 31
  84:	440427ff 	xor	rg1, rg2, 2047
  88:	44042fff 	xor	rg1, rg2, 4095
  8c:	710420c0 	cmp	rg1, rg2, rg3
  90:	710420df 	cmp	rg1, rg2, rg3 << 31
  94:	700427ff 	cmp	rg1, rg2, 2047
  98:	70042800 	cmp	rg1, rg2, -2048
  9c:	730420c0 	cmpu	rg1, rg2, rg3
  a0:	730420df 	cmpu	rg1, rg2, rg3 << 31
  a4:	720427ff 	cmpu	rg1, rg2, 2047
  a8:	72042fff 	cmpu	rg1, rg2, 4095
