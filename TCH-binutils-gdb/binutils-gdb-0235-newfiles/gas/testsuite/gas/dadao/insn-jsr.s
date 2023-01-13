# Da Dao Zhi Jian
	.text
	.p2align
	.global	_start
_start:
	call	_rec1
	ret	rd0, 0

_rec1:
	call	_rec2
	call	_rec2
	ret	rd0, 0

_rec2:
	call	_rec3
	call	_rec3
	call	_rec3
	call	_rec3
	ret	rd0, 0

_rec3:
	call	_rec1
	ret	rd0, 0
