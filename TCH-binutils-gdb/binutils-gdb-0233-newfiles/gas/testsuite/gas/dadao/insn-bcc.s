# Da Dao Zhi Jian
	.text
	.p2align
	.global	_start
_start:
1:	bn	$1, 1f
1:	bn	$2, 1b
	bn	$3, _start
2:	bz	$1, 2f
2:	bz	$2, 2b
	bz	$3, _start
3:	bp	$1, 3f
3:	bp	$2, 3b
	bp	$3, _start
4:	bod	$1, 4f
4:	bod	$2, 4b
	bod	$3, _start
5:	bnn	$1, 5f
5:	bnn	$2, 5b
	bnn	$3, _start
6:	bnz	$1, 6f
6:	bnz	$2, 6b
	bnz	$3, _start
7:	bnp	$1, 7f
7:	bnp	$2, 7b
	bnp	$3, _start
8:	bev	$1, 8f
8:	bev	$2, 8b
	bev	$3, _start
