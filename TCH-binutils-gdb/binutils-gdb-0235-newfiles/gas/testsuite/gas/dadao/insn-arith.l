GAS for DADAO .*/insn-arith.s 			page 1


   1              	# Da Dao Zhi Jian
   2              	_start:
   3              		# addi/add/sub
   4 0000 1905ffff 		addi	rd1, 131071
   5 0004 19060000 		addi	rd1, -131072
   6 0008 4905ffff    addi	rb1, 131071
   7 000c 49060000 	  addi	rb1, -131072
   8 0010 1a0420c4 		add	rd1, rd2, rd3, rd4
   9 0014 1b0420c4 		sub	rd1, rd2, rd3, rd4
  10               	
  11                  # muls/mulu/divs/divu
  12 0018 1c0420c4 		muls	rd1, rd2, rd3, rd4
  13 001c 1d0420c4 		mulu	rd1, rd2, rd3, rd4
  14 0020 1e0420c4 		divs	rd1, rd2, rd3, rd4
  15 0024 1f0420c4 		divu	rd1, rd2, rd3, rd4
  16               		
  17                  # and/orr/xor
  18 0028 10201083 		and	rd1, rd2, rd3
  19 002c 10241083 		orr	rd1, rd2, rd3
  20 0030 10281083 		xor	rd1, rd2, rd3
  21              		
  22                  # cmps/cmpu
  23 0034 120427ff 		cmps	rd1, rd2, 2047
  24 0038 12042800 		cmps	rd1, rd2, -2048
  25 003c 13042fff 		cmpu	rd1, rd2, 4095
  26 0040 10901083 		cmps	rd1, rd2, rd3
  27 0044 10941083    cmpu	rd1, rd2, rd3
GAS for DADAO .*/insn-arith.s 			page 2


DEFINED SYMBOLS
.*/insn-arith.s:2      .text:0000000000000000 _start

NO UNDEFINED SYMBOLS
