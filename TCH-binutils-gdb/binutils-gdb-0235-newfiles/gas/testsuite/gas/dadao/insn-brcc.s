# Da Dao Zhi Jian
	.text
	.p2align
	.global	_start
_start:
1:	brn		rd1, 1f
1:	brn		rd2, 1b
	brn		rd3, _start
2:	brz		rd1, 2f
2:	brz		rd2, 2b
	brz		rd3, _start
3:	brp		rd1, 3f
3:	brp		rd2, 3b
	brp		rd3, _start
4:	brod	rd1, 4f
4:	brod	rd2, 4b
	brod	rd3, _start
5:	brnn	rd1, 5f
5:	brnn	rd2, 5b
	brnn	rd3, _start
6:	brnz	rd1, 6f
6:	brnz	rd2, 6b
	brnz	rd3, _start
7:	brnp	rd1, 7f
7:	brnp	rd2, 7b
	brnp	rd3, _start
8:	brev	rd1, 8f
8:	brev	rd2, 8b
	brev	rd3, _start
