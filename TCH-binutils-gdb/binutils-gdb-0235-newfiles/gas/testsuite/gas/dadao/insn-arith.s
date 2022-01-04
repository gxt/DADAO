# Da Dao Zhi Jian
_start:
	# add/addu/sub/subu/mul
	add	rd1, 131071
	add	rd1, -131072
	add	rd1, rd2, rd3, rd4
	sub	rd1, rd2, rd3, rd4
	# muls/mulu/divs/divu
	muls	rd1, rd2, rd3, rd4
	mulu	rd1, rd2, rd3, rd4
	divs	rd1, rd2, rd3, rd4
	divu	rd1, rd2, rd3, rd4
	# and/orr/xor
	and	rd1, rd2, rd3
	orr	rd1, rd2, rd3
	xor	rd1, rd2, rd3
	# cmps/cmpu
	cmps	rd1, rd2, 2047
	cmps	rd1, rd2, -2048
	cmpu	rd1, rd2, 4095
	cmps	rd1, rd2, rd3
	cmpu	rd1, rd2, rd3