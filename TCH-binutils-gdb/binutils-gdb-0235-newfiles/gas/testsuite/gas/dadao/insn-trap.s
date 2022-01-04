# Da Dao Zhi Jian
	.text
	.global _start
_start:
	trap	cp1, 262143
	cpcoop	cp1, 262143
	rd2cr	cp1, cr1, cr2, rd1
	cr2rd	cp1, rd1, cr1, cr2
	cpload	cp1, rb1, 4095
	cpstor	cp1, rb1, 4095
