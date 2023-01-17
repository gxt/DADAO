# Da Dao Zhi Jian
	.text
	.global	_start
_start:
	# addi/add/sub
	addi	rd1, rd2, 2047
	addi	rd1, rd2, -2048
	addi	rb1, rb2, 2047
	addi	rb1, rb2, -2048
	add	rd1, rd2, rd3, rd4
	add	rb1, rb1, rd1
	sub	rd1, rd2, rd3, rd4
	sub	rb1, rb1, rd1

	# muls/mulu/divs/divu
	muls	rd1, rd2, rd3, rd4
	mulu	rd1, rd2, rd3, rd4
	divs	rd1, rd2, rd3, rd4
	divu	rd1, rd2, rd3, rd4

	# and/orr/xor/andi
	and	rd1, rd2, rd3
	orr	rd1, rd2, rd3
	xor	rd1, rd2, rd3
	andi	rd1, rd2, 4095
	
	# cmps/cmpu/cmp
	cmps	rd1, rd2, 2047
	cmps	rd1, rd2, -2048
	cmpu	rd1, rd2, 4095
	cmps	rd1, rd2, rd3
	cmpu	rd1, rd2, rd3
	cmp	rd1, rb1, rb2

	# adrp
	adrp	rb1, 131071
	adrp	rb1, -131072
