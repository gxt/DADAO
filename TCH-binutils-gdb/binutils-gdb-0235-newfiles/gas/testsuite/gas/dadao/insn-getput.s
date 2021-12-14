# Da Dao Zhi Jian
	.text
	.global _start
_start:
	rg2rg	rg1, rg1, 0
	rg2rp	rp1, rg1, 1
	rg2rf	rf1, rg1, 2
	rp2rg	rg1, rp1, 3
	rp2rp	rp1, rp1, 4
	rf2rg	rg1, rf1, 5
	rf2rf	rf1, rf1, 6