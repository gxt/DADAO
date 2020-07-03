# objdump: -dr
tmpdir/insn-arith.o:     file format elf64-dadao


Disassembly of section .text:

0000000000000000 <_start>:
   0:	21317200 	add	rg12, rg23, rg8
   4:	21317202 	add	rg12, rg23, rg8 << 2
   8:	203177d0 	add	rg12, rg23, 2000
   c:	23317200 	addu	rg12, rg23, rg8
  10:	23317202 	addu	rg12, rg23, rg8 << 2
  14:	223177d0 	addu	rg12, rg23, 2000
  18:	25317200 	sub	rg12, rg23, rg8
  1c:	25317202 	sub	rg12, rg23, rg8 << 2
  20:	243177d0 	sub	rg12, rg23, 2000
  24:	27317200 	subu	rg12, rg23, rg8
  28:	27317202 	subu	rg12, rg23, rg8 << 2
  2c:	263177d0 	subu	rg12, rg23, 2000
  30:	31317200 	mul	rg12, rg23, rg8
  34:	31317202 	mul	rg12, rg23, rg8 << 2
  38:	303177d0 	mul	rg12, rg23, 2000
  3c:	35317204 	mulu	rg12, rg23, rg8, rg4
  40:	36317204 	div	rg12, rg23, rg8, rg4
  44:	37317204 	divu	rg12, rg23, rg8, rg4
