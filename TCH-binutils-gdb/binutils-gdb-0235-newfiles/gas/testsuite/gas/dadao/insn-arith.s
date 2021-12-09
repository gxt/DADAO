# Da Dao Zhi Jian
_start:
	# add/addu/sub/subu/mul
	add	rg1, rg2, 2047
	add	rg1, rg2, -2048
	add	rg1, rg2, rg3, rg4
	sub	rg1, rg2, rg3, rg4
	# muls/mulu/divs/divu
	muls	rg1, rg2, rg3, rg4
	mulu	rg1, rg2, rg3, rg4
	divs	rg1, rg2, rg3, rg4
	divu	rg1, rg2, rg3, rg4
	# and/orr/xor
	and	rg1, rg2, rg3
	orr	rg1, rg2, rg3
	xor	rg1, rg2, rg3
	# cmps/cmpu
	cmps	rg1, rg2, 2047
	cmps	rg1, rg2, -2048
	cmpu	rg1, rg2, 4095
	cmps	rg1, rg2, rg3
	cmpu	rg1, rg2, rg3