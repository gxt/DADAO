# objdump: -dr
tmpdir/insn-arith.o:     file format elf64-dadao


Disassembly of section .text:

0000000000000000 <_start>:
   0:	20317200 	add	rg12, rg23, rg8
   4:	20317202 	add	rg12, rg23, rg8 << 2
   8:	21317fa0 	add	rg12, rg23, 4000
   c:	22317200 	addu	rg12, rg23, rg8
  10:	22317202 	addu	rg12, rg23, rg8 << 2
  14:	23317fa0 	addu	rg12, rg23, 4000
  18:	24317200 	sub	rg12, rg23, rg8
  1c:	24317202 	sub	rg12, rg23, rg8 << 2
  20:	25317fa0 	sub	rg12, rg23, 4000
  24:	26317200 	subu	rg12, rg23, rg8
  28:	26317202 	subu	rg12, rg23, rg8 << 2
  2c:	27317fa0 	subu	rg12, rg23, 4000
  30:	28317200 	mul	rg12, rg23, rg8
  34:	28317202 	mul	rg12, rg23, rg8 << 2
  38:	29317fa0 	mul	rg12, rg23, 4000
  3c:	2a317200 	mulu	rg12, rg23, rg8
  40:	2a317202 	mulu	rg12, rg23, rg8 << 2
  44:	2b317fa0 	mulu	rg12, rg23, 4000
  48:	2c317200 	div	rg12, rg23, rg8
  4c:	2c317202 	div	rg12, rg23, rg8 << 2
  50:	2d317fa0 	div	rg12, rg23, 4000
  54:	2e317200 	divu	rg12, rg23, rg8
  58:	2e317202 	divu	rg12, rg23, rg8 << 2
  5c:	2f317fa0 	divu	rg12, rg23, 4000
