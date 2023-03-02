# Da Dao Zhi Jian
	.text
	.p2align
	.global	_start
_start:
	call	_rec1
	jump	_rec1
	swym	0
	ret	rd0, 0

_rec1:
	call	_rec2
	call	_rec2
	jump	_rec2
	ret	rd1, 131071

_rec2:
	call	_rec3
	call	_rec3
	call	_rec3
	call	_rec3
	jump	_rec3
	ret	rd1, -131072

_rec3:
	call	_rec1
	jump	_rec1
	jump	rb1, rd1, 2047
	jump	rb1, rd1, -2048
	ret	rd0, 0
