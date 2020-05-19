GAS for DADAO .*/list-insns.s 			page 1


   1              	#
   2              	# Somewhat complete instruction set and operand type check.  No
   3              	# relocations or deferred register definitions here.
   4              	#
   5              	_start:
   6              		# add/addu/sub/subu/mul/mulu/div/divu
   7 0000 20317200 		add	\$12, \$23, \$8
   8 0004 20317202 		add	\$12, \$23, \$8 << 2
   9 0008 21317FA0 		add	\$12, \$23, 4000
  10 000c 22317200 		addu	\$12, \$23, \$8
  11 0010 22317202 		addu	\$12, \$23, \$8 << 2
  12 0014 23317FA0 		addu	\$12, \$23, 4000
  13 0018 24317200 		sub	\$12, \$23, \$8
  14 001c 24317202 		sub	\$12, \$23, \$8 << 2
  15 0020 25317FA0 		sub	\$12, \$23, 4000
  16 0024 26317200 		subu	\$12, \$23, \$8
  17 0028 26317202 		subu	\$12, \$23, \$8 << 2
  18 002c 27317FA0 		subu	\$12, \$23, 4000
  19 0030 28317200 		mul	\$12, \$23, \$8
  20 0034 28317202 		mul	\$12, \$23, \$8 << 2
  21 0038 29317FA0 		mul	\$12, \$23, 4000
  22 003c 2A317200 		mulu	\$12, \$23, \$8
  23 0040 2A317202 		mulu	\$12, \$23, \$8 << 2
  24 0044 2B317FA0 		mulu	\$12, \$23, 4000
  25 0048 2C317200 		div	\$12, \$23, \$8
  26 004c 2C317202 		div	\$12, \$23, \$8 << 2
  27 0050 2D317FA0 		div	\$12, \$23, 4000
  28 0054 2E317200 		divu	\$12, \$23, \$8
  29 0058 2E317202 		divu	\$12, \$23, \$8 << 2
  30 005c 2F317FA0 		divu	\$12, \$23, 4000
  31              		# swym
  32 0060 DA000000 		swym	0
  33              		# nop
  34 0064 DADADADA 		nop
GAS for DADAO .*/list-insns.s 			page 2


DEFINED SYMBOLS
.*/list-insns.s:5      .text:0000000000000000 _start

NO UNDEFINED SYMBOLS
