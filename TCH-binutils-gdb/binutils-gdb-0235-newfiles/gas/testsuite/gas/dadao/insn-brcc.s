# Da Dao Zhi Jian
	.text
	.p2align
	.global	_start
_start:
1:	brn		rg1, 1f
1:	brn		rg2, 1b
	brn		rg3, _start
2:	brz		rg1, 2f
2:	brz		rg2, 2b
	brz		rg3, _start
3:	brp		rg1, 3f
3:	brp		rg2, 3b
	brp		rg3, _start
4:	brod	rg1, 4f
4:	brod	rg2, 4b
	brod	rg3, _start
5:	brnn	rg1, 5f
5:	brnn	rg2, 5b
	brnn	rg3, _start
6:	brnz	rg1, 6f
6:	brnz	rg2, 6b
	brnz	rg3, _start
7:	brnp	rg1, 7f
7:	brnp	rg2, 7b
	brnp	rg3, _start
8:	brev	rg1, 8f
8:	brev	rg2, 8b
	brev	rg3, _start
