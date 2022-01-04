# Da Dao Zhi Jian
	.text
	.global _start
_start:
	rd2rd	rd1, rd1, 0
	rd2rb	rb1, rd1, 1
	rb2rd	rd1, rb1, 2
	rb2rb	rb1, rb1, 3
	cmprb	rd1, rb1, rb2
	addrb	rb1, rb2, rd1
	subrb	rb1, rb2, rd1

	rd2rf	rf1, rd1, 0
	rf2rd	rd1, rf1, 1
	rf2rf	rf1, rf1, 2

	rd2ra	ra1, rd1, 3
	ra2rd	rd1, ra1, 4