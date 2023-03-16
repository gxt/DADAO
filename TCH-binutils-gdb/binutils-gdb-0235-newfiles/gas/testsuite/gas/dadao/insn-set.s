# Da Dao Zhi Jian
	.text
	.global _start
_start:
	setrd	rd1, 0xabcdabcd
	setrd	rd2, 0x1357924680
	setrd	rd4, 0x1234567812345678
	setrd	rd5, 0
	setrd	rd6, -1
	setrd	rd7, 0xfffffffe
	setrd	rd8, 0xffff0000ffff
	setrd	rd9, 0xffff0000ffff0000
