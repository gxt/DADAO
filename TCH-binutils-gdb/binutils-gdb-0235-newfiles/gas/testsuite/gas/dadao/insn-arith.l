GAS for DADAO .*/insn-arith.s 			page 1


   1              	# Da Dao Zhi Jian
   2              	_start:
   3              		# add/addu/sub/subu
   4 0000 1905ffff 		add	rg1, 131071
   5 0004 19060000 		add	rg1, -131072
   6 0008 1a0420c4 		add	rg1, rg2, rg3, rg4
   7 000c 1b0420c4 		sub	rg1, rg2, rg3, rg4
   8               		# muls/mulu/divs/divu
   9 0010 1c0420c4 		muls	rg1, rg2, rg3, rg4
  10 0014 1d0420c4 		mulu	rg1, rg2, rg3, rg4
  11 0018 1e0420c4 		divs	rg1, rg2, rg3, rg4
  12 001c 1f0420c4 		divu	rg1, rg2, rg3, rg4
  13               		# and/orr/xor
  14 0020 10201083 		and	rg1, rg2, rg3
  15 0024 10241083 		orr	rg1, rg2, rg3
  16 0028 10281083 		xor	rg1, rg2, rg3
  17              		# cmps/cmpu
  18 002c 120427ff 		cmps	rg1, rg2, 2047
  19 0030 12042800 		cmps	rg1, rg2, -2048
  20 0034 13042fff 		cmpu	rg1, rg2, 4095
  21 0038 10901083 		cmps	rg1, rg2, rg3
  22 003c 10941083    cmpu	rg1, rg2, rg3
GAS for DADAO .*/insn-arith.s 			page 2


DEFINED SYMBOLS
.*/insn-arith.s:2      .text:0000000000000000 _start

NO UNDEFINED SYMBOLS
