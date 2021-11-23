# Da Dao Zhi Jian
	.text
	.p2align
	.global	_start
_start:
	call	_rec1
	ret

_rec1:
	call	_rec2
	call	_rec2
	ret

_rec2:
	call	_rec3
	call	_rec3
	call	_rec3
	call	_rec3
	ret

_rec3:
	call	_rec1
	ret
