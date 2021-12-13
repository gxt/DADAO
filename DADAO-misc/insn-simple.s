# Da Dao Zhi Jian
	.text
	.global _start
_start:
	swym
	# 0001-0000: MISC-Norm
	and	rg1, rg2, rg3
	orr	rg1, rg2, rg3
	xor	rg1, rg2, rg3
	shlu	rg1, rg2, rg3
	shrs	rg1, rg2, rg3
	shru	rg1, rg2, rg3
	exts	rg1, rg2, rg3
	extz	rg1, rg2, rg3
	not	rg1, rg2, rg3
	shlu	rg1, rg2, 3
	shrs	rg1, rg2, 3
	shru	rg1, rg2, 3
	exts	rg1, rg2, 3
	extz	rg1, rg2, 3
	not	rg1, rg2, 3
	cmps	rg1, rg2, rg3
	cmpu	rg1, rg2, rg3
	rg2rg	rg1, rg2, 3
	rg2rp	rp1, rg2, 3
	rg2rf	rf1, rg2, 3
	rp2rg	rg1, rp2, 3
	rp2rp	rp1, rp2, 3
	rf2rg	rg1, rf2, 3
	rf2rf	rf1, rf2, 3
	# 0001-0xxx
	add	rg1, 2
	add	rg1, rg2, rg3, rg4
	sub	rg1, rg2, rg3, rg4
	muls	rg1, rg2, rg3, rg4
	mulu	rg1, rg2, rg3, rg4
	divs	rg1, rg2, rg3, rg4
	divu	rg1, rg2, rg3, rg4
	# 0001-1xxx
	cmps	rg1, rg2, 3
	cmpu	rg1, rg2, 3
	setowh	rg1, 2
	setowj	rg1, 2
	setowk	rg1, 2
	setowl	rg1, 2
	setzwh	rg1, 2
	setzwj	rg1, 2
	setzwk	rg1, 2
	setzwl	rg1, 2
	orwh	rg1, 2
	orwj	rg1, 2
	orwk	rg1, 2
	orwl	rg1, 2
	andnwh	rg1, 2
	andnwj	rg1, 2
	andnwk	rg1, 2
	andnwl	rg1, 2

	ret
