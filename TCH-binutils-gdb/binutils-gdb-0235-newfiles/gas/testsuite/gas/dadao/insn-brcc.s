# Da Dao Zhi Jian
	.text
	.p2align
	.global	_start
_start:
	brn		rd1, _rec1
	brn		rd2, _start
_rec1:
	brz		rd1, _rec1
	brz		rd2, _rec2
_rec2:
	brp		rd1, _rec2
	brp		rd2, _rec3
_rec3:
	brod	rd1, _rec3
	brod	rd2, _rec4
_rec4:
	brnn	rd1, _rec4
	brnn	rd2, _rec5
_rec5:
	brnz	rd1, _rec5
	brnz	rd2, _rec6
_rec6:
	brnp	rd1, _rec6
	brnp	rd2, _rec7
_rec7:
	brev	rd1, _rec8
_rec8:
	brev	rd2, _start
