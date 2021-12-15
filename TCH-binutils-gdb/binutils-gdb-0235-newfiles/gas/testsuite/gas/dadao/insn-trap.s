# Da Dao Zhi Jian
	.text
	.global _start
_start:
	trap	cp1, 262143
	cpcoop	cp1, 262143
	rg2cr	cp1, cr1, cr2, rg1
	cr2rg	cp1, rg1, cr1, cr2
	cpload	cp1, rp1, 4096
	cpstor	cp1, rp1, 4096
