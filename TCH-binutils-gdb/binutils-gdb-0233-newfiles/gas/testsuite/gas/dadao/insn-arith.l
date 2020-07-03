GAS for DADAO .*/insn-arith.s 			page 1


   1              	# Da Dao Zhi Jian
   2              	_start:
   3              		# add/addu/sub/subu/mul/mulu/div/divu
   4 0000 21317200 		add	rg12, rg23, rg8
   5 0004 21317202 		add	rg12, rg23, rg8 << 2
   6 0008 203177D0 		add	rg12, rg23, 2000
   7 000c 23317200 		addu	rg12, rg23, rg8
   8 0010 23317202 		addu	rg12, rg23, rg8 << 2
   9 0014 223177D0 		addu	rg12, rg23, 2000
  10 0018 25317200 		sub	rg12, rg23, rg8
  11 001c 25317202 		sub	rg12, rg23, rg8 << 2
  12 0020 243177D0 		sub	rg12, rg23, 2000
  13 0024 27317200 		subu	rg12, rg23, rg8
  14 0028 27317202 		subu	rg12, rg23, rg8 << 2
  15 002c 263177D0 		subu	rg12, rg23, 2000
  16 0030 31317200 		mul	rg12, rg23, rg8
  17 0034 31317202 		mul	rg12, rg23, rg8 << 2
  18 0038 303177D0 		mul	rg12, rg23, 2000
  19 003c 35317204 		mulu	rg12, rg23, rg8, rg4
  20 0040 36317204 		div	rg12, rg23, rg8, rg4
  21 0044 37317204 		divu	rg12, rg23, rg8, rg4
GAS for DADAO .*/insn-arith.s 			page 2


DEFINED SYMBOLS
.*/insn-arith.s:2      .text:0000000000000000 _start

NO UNDEFINED SYMBOLS
