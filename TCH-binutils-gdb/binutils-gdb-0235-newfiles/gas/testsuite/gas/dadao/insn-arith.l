GAS for DADAO .*/insn-arith.s 			page 1


   1              	# Da Dao Zhi Jian
   2              	_start:
   3              		# add/addu/sub/subu
   4 0000 1905ffff 		add	rd1, 131071
   5 0004 19060000 		add	rd1, -131072
   6 0008 1a0420c4 		add	rd1, rd2, rd3, rd4
   7 000c 1b0420c4 		sub	rd1, rd2, rd3, rd4
   8               		# muls/mulu/divs/divu
   9 0010 1c0420c4 		muls	rd1, rd2, rd3, rd4
  10 0014 1d0420c4 		mulu	rd1, rd2, rd3, rd4
  11 0018 1e0420c4 		divs	rd1, rd2, rd3, rd4
  12 001c 1f0420c4 		divu	rd1, rd2, rd3, rd4
  13               		# and/orr/xor
  14 0020 10201083 		and	rd1, rd2, rd3
  15 0024 10241083 		orr	rd1, rd2, rd3
  16 0028 10281083 		xor	rd1, rd2, rd3
  17              		# cmps/cmpu
  18 002c 120427ff 		cmps	rd1, rd2, 2047
  19 0030 12042800 		cmps	rd1, rd2, -2048
  20 0034 13042fff 		cmpu	rd1, rd2, 4095
  21 0038 10901083 		cmps	rd1, rd2, rd3
  22 003c 10941083    cmpu	rd1, rd2, rd3
GAS for DADAO .*/insn-arith.s 			page 2


DEFINED SYMBOLS
.*/insn-arith.s:2      .text:0000000000000000 _start

NO UNDEFINED SYMBOLS
