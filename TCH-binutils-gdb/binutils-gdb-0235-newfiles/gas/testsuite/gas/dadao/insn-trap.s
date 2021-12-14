# Da Dao Zhi Jian
	.text
	.global _start
_start:
	trap	cp1, 262144
	cmd 	cp1, 262144
	rg2cr	cp1, cr1, cr2, rg1
	cr2rg	cp1, rg1, cr1, cr2
    cpld    cp1, rp1, 4096
    cpst    cp1, rp1, 4096