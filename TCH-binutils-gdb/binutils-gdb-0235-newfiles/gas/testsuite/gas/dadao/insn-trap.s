# Da Dao Zhi Jian
	.text
	.global _start
_start:
	trap	cp1, 262143
	cpco	cp1, 262143
	cprd	cp1, cr1, cr2, rd1
	cpwr	cp1, cr1, cr2, rd1
	cpld	cp1, rb1, 4095
	cpst	cp1, rb1, 4095
