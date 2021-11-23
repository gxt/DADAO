# Da Dao Zhi Jian
_start:
	# add/addu/sub/subu/mul
	add	rg1, rg2, rg3
	add	rg1, rg2, rg3 << 31
	add	rg1, rg2, 2047
	add	rg1, rg2, -2048
	addu	rg1, rg2, rg3
	addu	rg1, rg2, rg3 << 31
	addu	rg1, rg2, 2047
	addu	rg1, rg2, 4095
	sub	rg1, rg2, rg3
	sub	rg1, rg2, rg3 << 31
	sub	rg1, rg2, 2047
	sub	rg1, rg2, -2048
	subu	rg1, rg2, rg3
	subu	rg1, rg2, rg3 << 31
	subu	rg1, rg2, 2047
	subu	rg1, rg2, 4095
	mul	rg1, rg2, rg3
	mul	rg1, rg2, rg3 << 31
	mul	rg1, rg2, 2047
	mul	rg1, rg2, -2048
	# mulu/div/divu
	mulu	rg1, rg2, rg3, rg4
	div	rg1, rg2, rg3, rg4
	divu	rg1, rg2, rg3, rg4
	# or/and/xor
	or	rg1, rg2, rg3
	or	rg1, rg2, rg3 << 31
	or	rg1, rg2, 2047
	or	rg1, rg2, 4095
	and	rg1, rg2, rg3
	and	rg1, rg2, rg3 << 31
	and	rg1, rg2, 2047
	and	rg1, rg2, 4095
	xor	rg1, rg2, rg3
	xor	rg1, rg2, rg3 << 31
	xor	rg1, rg2, 2047
	xor	rg1, rg2, 4095
	# cmp/cmpu
	cmp	rg1, rg2, rg3
	cmp	rg1, rg2, rg3 << 31
	cmp	rg1, rg2, 2047
	cmp	rg1, rg2, -2048
	cmpu	rg1, rg2, rg3
	cmpu	rg1, rg2, rg3 << 31
	cmpu	rg1, rg2, 2047
	cmpu	rg1, rg2, 4095
