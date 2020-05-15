#
# Somewhat complete instruction set and operand type check.  No
# relocations or deferred register definitions here.
#
_start:
	# add/addu/sub/subu/mul/mulu/div/divu
	add	$12, $23, $8
	add	$12, $23, $8 << 2
	add	$12, $23, 4000
	addu	$12, $23, $8
	addu	$12, $23, $8 << 2
	addu	$12, $23, 4000
	sub	$12, $23, $8
	sub	$12, $23, $8 << 2
	sub	$12, $23, 4000
	subu	$12, $23, $8
	subu	$12, $23, $8 << 2
	subu	$12, $23, 4000
	mul	$12, $23, $8
	mul	$12, $23, $8 << 2
	mul	$12, $23, 4000
	mulu	$12, $23, $8
	mulu	$12, $23, $8 << 2
	mulu	$12, $23, 4000
	div	$12, $23, $8
	div	$12, $23, $8 << 2
	div	$12, $23, 4000
	divu	$12, $23, $8
	divu	$12, $23, $8 << 2
	divu	$12, $23, 4000
	# swym
	swym	0
	# nop
	nop
