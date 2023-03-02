# Da Dao Zhi Jian
	.text
	.global	_start
_start:
	shlu	rd1, rd2, rd3
	shrs	rd1, rd2, rd3
	shru	rd1, rd2, rd3
	exts	rd1, rd2, rd3
	extz	rd1, rd2, rd3
	xnor	rd1, rd2, rd3

	shlu	rd1, rd2, 63
	shrs	rd1, rd2, 63
	shru	rd1, rd2, 63
	exts	rd1, rd2, 63
	extz	rd1, rd2, 63
