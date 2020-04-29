#
# Somewhat complete instruction set and operand type check.  No
# relocations or deferred register definitions here.
#
Main tetra 3
	# add/addu/sub/subu
	add	$12, $23, $5
	add	$12, $23, $8 << 2
	add	$12, $23, $8 << 62
	add	$22, $20, 255
	add	$22, $20, 4000
	addu	$12, $23, $8 << 2
	addu	$22, $20, 4000
	sub	$12, $23, $8 << 2
	sub	$22, $20, 4000
	subu	$12, $23, $8 << 2
	subu	$22, $20, 4000
	# swym
	swym	0, 0, 0
