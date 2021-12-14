GAS for DADAO .*/insn-arith.s 			page 1


   1              	# Da Dao Zhi Jian
   2              	_start:
   3              		# add/addu/sub/subu
   4 0000 1105ffff 		add	rg1, 262143
   5 0004 11070000 		add	rg1, -262144
   6 0008 120420c4 		add	rg1, rg2, rg3, rg4
   7 000c 130420c4 		sub	rg1, rg2, rg3, rg4
   8               		# muls/mulu/divs/divu
   9 0014 140420c4 		muls	rg1, rg2, rg3, rg4
  10 0018 150420c4 		mulu	rg1, rg2, rg3, rg4
  11 001c 160420c4 		divs	rg1, rg2, rg3, rg4
  12 0020 170420c4 		divu	rg1, rg2, rg3, rg4
  13               		# and/orr/xor
  14 0028 10201083 		and	rg1, rg2, rg3
  15 002c 10241083 		orr	rg1, rg2, rg3
  16 0030 10281083 		xor	rg1, rg2, rg3
  17              		# cmps/cmpu
  18 0038 1a0427ff 		cmps	rg1, rg2, 2047
  19 003c 1a042800 		cmps	rg1, rg2, -2048
  20 0040 1b042fff 		cmpu	rg1, rg2, 4095
  21 0044 10901083 		cmps	rg1, rg2, rg3
  22 0048 10941083    cmpu	rg1, rg2, rg3
GAS for DADAO .*/insn-arith.s 			page 2


DEFINED SYMBOLS
.*/insn-arith.s:2      .text:0000000000000000 _start

NO UNDEFINED SYMBOLS
