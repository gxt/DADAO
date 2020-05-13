GAS for DADAO .*/list-insns.s 			page 1


   1              	#
   2              	# Somewhat complete instruction set and operand type check.  No
   3              	# relocations or deferred register definitions here.
   4              	#
   5              	_start:
   6              		# add/addu/sub/subu/mul/mulu/div/divu
   7 0000 205C800C 		add	\$12, \$23, \$8
   8 0004 205C808C 		add	\$12, \$23, \$8 << 2
   9 0008 215FE80C 		add	\$12, \$23, 4000
  10 000c 225C800C 		addu	\$12, \$23, \$8
  11 0010 225C808C 		addu	\$12, \$23, \$8 << 2
  12 0014 235FE80C 		addu	\$12, \$23, 4000
  13 0018 245C800C 		sub	\$12, \$23, \$8
  14 001c 245C808C 		sub	\$12, \$23, \$8 << 2
  15 0020 255FE80C 		sub	\$12, \$23, 4000
  16 0024 265C800C 		subu	\$12, \$23, \$8
  17 0028 265C808C 		subu	\$12, \$23, \$8 << 2
  18 002c 275FE80C 		subu	\$12, \$23, 4000
  19 0030 285C800C 		mul	\$12, \$23, \$8
  20 0034 285C808C 		mul	\$12, \$23, \$8 << 2
  21 0038 295FE80C 		mul	\$12, \$23, 4000
  22 003c 2A5C800C 		mulu	\$12, \$23, \$8
  23 0040 2A5C808C 		mulu	\$12, \$23, \$8 << 2
  24 0044 2B5FE80C 		mulu	\$12, \$23, 4000
  25 0048 2C5C800C 		div	\$12, \$23, \$8
  26 004c 2C5C808C 		div	\$12, \$23, \$8 << 2
  27 0050 2D5FE80C 		div	\$12, \$23, 4000
  28 0054 2E5C800C 		divu	\$12, \$23, \$8
  29 0058 2E5C808C 		divu	\$12, \$23, \$8 << 2
  30 005c 2F5FE80C 		divu	\$12, \$23, 4000
  31              		# swym
  32 0060 FD000000 		swym	0, 0, 0
  33              		# nop
  34 0064 FD000000 		nop
GAS for DADAO .*/list-insns.s 			page 2


DEFINED SYMBOLS
.*/list-insns.s:5      .text:0000000000000000 _start

NO UNDEFINED SYMBOLS
