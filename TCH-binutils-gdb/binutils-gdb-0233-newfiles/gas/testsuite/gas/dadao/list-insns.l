GAS for DADAO .*/list-insns.s 			page 1


   1              	#
   2              	# Somewhat complete instruction set and operand type check.  No
   3              	# relocations or deferred register definitions here.
   4              	#
   5 0000 00000003 	Main tetra 3
   6              		# add/addu/sub/subu
   7 0004 205C500C 		add	\$12, \$23, \$5
   8 0008 205C808C 		add	\$12, \$23, \$8 << 2
   9 000c 205C8F8C 		add	\$12, \$23, \$8 << 62
  10 0010 21503FD6 		add	\$22, \$20, 255
  11 0014 2153E816 		add	\$22, \$20, 4000
  12 0018 225C808C 		addu	\$12, \$23, \$8 << 2
  13 001c 2353E816 		addu	\$22, \$20, 4000
  14 0020 245C808C 		sub	\$12, \$23, \$8 << 2
  15 0024 2553E816 		sub	\$22, \$20, 4000
  16 0028 265C808C 		subu	\$12, \$23, \$8 << 2
  17 002c 2753E816 		subu	\$22, \$20, 4000
  18              		# swym
  19 0030 FD000000 		swym	0, 0, 0
GAS for DADAO .*/list-insns.s 			page 2


DEFINED SYMBOLS
.*/list-insns.s:5      .text:0000000000000000 Main

NO UNDEFINED SYMBOLS
