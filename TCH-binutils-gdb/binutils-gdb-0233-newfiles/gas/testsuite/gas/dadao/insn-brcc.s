# Da Dao Zhi Jian
	.text
	.p2align
	.global	_start
_start:
1:	bn	rg1, 1f
1:	bn	rg2, 1b
	bn	rg3, _start
2:	bz	rg1, 2f
2:	bz	rg2, 2b
	bz	rg3, _start
3:	bp	rg1, 3f
3:	bp	rg2, 3b
	bp	rg3, _start
4:	bod	rg1, 4f
4:	bod	rg2, 4b
	bod	rg3, _start
5:	bnn	rg1, 5f
5:	bnn	rg2, 5b
	bnn	rg3, _start
6:	bnz	rg1, 6f
6:	bnz	rg2, 6b
	bnz	rg3, _start
7:	bnp	rg1, 7f
7:	bnp	rg2, 7b
	bnp	rg3, _start
8:	bev	rg1, 8f
8:	bev	rg2, 8b
	bev	rg3, _start
