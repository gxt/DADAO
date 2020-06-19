# Da Dao Zhi Jian
	.text
	.p2align
	.global	_start
_start:
1:	br.n	rg1, 1f
1:	br.n	rg2, 1b
	br.n	rg3, _start
2:	br.z	rg1, 2f
2:	br.z	rg2, 2b
	br.z	rg3, _start
3:	br.p	rg1, 3f
3:	br.p	rg2, 3b
	br.p	rg3, _start
4:	br.od	rg1, 4f
4:	br.od	rg2, 4b
	br.od	rg3, _start
5:	br.nn	rg1, 5f
5:	br.nn	rg2, 5b
	br.nn	rg3, _start
6:	br.nz	rg1, 6f
6:	br.nz	rg2, 6b
	br.nz	rg3, _start
7:	br.np	rg1, 7f
7:	br.np	rg2, 7b
	br.np	rg3, _start
8:	br.ev	rg1, 8f
8:	br.ev	rg2, 8b
	br.ev	rg3, _start
