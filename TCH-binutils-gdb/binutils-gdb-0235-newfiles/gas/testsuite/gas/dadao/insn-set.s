# Da Dao Zhi Jian
	.text
	.global _start
_start:
	move	rd1, 0xabcdabcd
	move	rd2, 0x1357924680
	move	rd4, 0x1234567812345678
	move	rd5, 0
	move	rd6, -1
	move	rd7, 0xFFFFFFFE
	move	rd8, 0xFFFF0000FFFF
	move	rd9, 0xFFFF0000FFFF0000

_rec:
	move	rb1, _start
