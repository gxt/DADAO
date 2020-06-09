GAS for DADAO .*/insn-arith.s 			page 1


   1              	# Da Dao Zhi Jian
   2              	_start:
   3              		# add/addu/sub/subu/mul/mulu/div/divu
   4 0000 20317200 		add	rg12, rg23, rg8
   5 0004 20317202 		add	rg12, rg23, rg8 << 2
   6 0008 21317FA0 		add	rg12, rg23, 4000
   7 000c 22317200 		addu	rg12, rg23, rg8
   8 0010 22317202 		addu	rg12, rg23, rg8 << 2
   9 0014 23317FA0 		addu	rg12, rg23, 4000
  10 0018 24317200 		sub	rg12, rg23, rg8
  11 001c 24317202 		sub	rg12, rg23, rg8 << 2
  12 0020 25317FA0 		sub	rg12, rg23, 4000
  13 0024 26317200 		subu	rg12, rg23, rg8
  14 0028 26317202 		subu	rg12, rg23, rg8 << 2
  15 002c 27317FA0 		subu	rg12, rg23, 4000
  16 0030 28317200 		mul	rg12, rg23, rg8
  17 0034 28317202 		mul	rg12, rg23, rg8 << 2
  18 0038 29317FA0 		mul	rg12, rg23, 4000
  19 003c 2A317200 		mulu	rg12, rg23, rg8
  20 0040 2A317202 		mulu	rg12, rg23, rg8 << 2
  21 0044 2B317FA0 		mulu	rg12, rg23, 4000
  22 0048 2C317200 		div	rg12, rg23, rg8
  23 004c 2C317202 		div	rg12, rg23, rg8 << 2
  24 0050 2D317FA0 		div	rg12, rg23, 4000
  25 0054 2E317200 		divu	rg12, rg23, rg8
  26 0058 2E317202 		divu	rg12, rg23, rg8 << 2
  27 005c 2F317FA0 		divu	rg12, rg23, 4000
GAS for DADAO .*/insn-arith.s 			page 2


DEFINED SYMBOLS
.*/insn-arith.s:2      .text:0000000000000000 _start

NO UNDEFINED SYMBOLS
