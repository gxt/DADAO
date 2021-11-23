GAS for DADAO .*/insn-arith.s 			page 1


   1              	# Da Dao Zhi Jian
   2              	_start:
   3              		# add/addu/sub/subu/mul
   4 0000 210420C0 		add	rg1, rg2, rg3
   5 0004 210420DF 		add	rg1, rg2, rg3 << 31
   6 0008 200427FF 		add	rg1, rg2, 2047
   7 000c 20042800 		add	rg1, rg2, -2048
   8 0010 230420C0 		addu	rg1, rg2, rg3
   9 0014 230420DF 		addu	rg1, rg2, rg3 << 31
  10 0018 220427FF 		addu	rg1, rg2, 2047
  11 001c 22042FFF 		addu	rg1, rg2, 4095
  12 0020 250420C0 		sub	rg1, rg2, rg3
  13 0024 250420DF 		sub	rg1, rg2, rg3 << 31
  14 0028 240427FF 		sub	rg1, rg2, 2047
  15 002c 24042800 		sub	rg1, rg2, -2048
  16 0030 270420C0 		subu	rg1, rg2, rg3
  17 0034 270420DF 		subu	rg1, rg2, rg3 << 31
  18 0038 260427FF 		subu	rg1, rg2, 2047
  19 003c 26042FFF 		subu	rg1, rg2, 4095
  20 0040 310420C0 		mul	rg1, rg2, rg3
  21 0044 310420DF 		mul	rg1, rg2, rg3 << 31
  22 0048 300427FF 		mul	rg1, rg2, 2047
  23 004c 30042800 		mul	rg1, rg2, -2048
  24              		# mulu/div/divu
  25 0050 350420C4 		mulu	rg1, rg2, rg3, rg4
  26 0054 360420C4 		div	rg1, rg2, rg3, rg4
  27 0058 370420C4 		divu	rg1, rg2, rg3, rg4
  28              		# or/and/xor
  29 005c 410420C0 		or	rg1, rg2, rg3
  30 0060 410420DF 		or	rg1, rg2, rg3 << 31
  31 0064 400427FF 		or	rg1, rg2, 2047
  32 0068 40042FFF 		or	rg1, rg2, 4095
  33 006c 430420C0 		and	rg1, rg2, rg3
  34 0070 430420DF 		and	rg1, rg2, rg3 << 31
  35 0074 420427FF 		and	rg1, rg2, 2047
  36 0078 42042FFF 		and	rg1, rg2, 4095
  37 007c 450420C0 		xor	rg1, rg2, rg3
  38 0080 450420DF 		xor	rg1, rg2, rg3 << 31
  39 0084 440427FF 		xor	rg1, rg2, 2047
  40 0088 44042FFF 		xor	rg1, rg2, 4095
  41              		# cmp/cmpu
  42 008c 710420C0 		cmp	rg1, rg2, rg3
  43 0090 710420DF 		cmp	rg1, rg2, rg3 << 31
  44 0094 700427FF 		cmp	rg1, rg2, 2047
  45 0098 70042800 		cmp	rg1, rg2, -2048
  46 009c 730420C0 		cmpu	rg1, rg2, rg3
  47 00a0 730420DF 		cmpu	rg1, rg2, rg3 << 31
  48 00a4 720427FF 		cmpu	rg1, rg2, 2047
  49 00a8 72042FFF 		cmpu	rg1, rg2, 4095
GAS for DADAO .*/insn-arith.s 			page 2


DEFINED SYMBOLS
.*/insn-arith.s:2      .text:0000000000000000 _start

NO UNDEFINED SYMBOLS
