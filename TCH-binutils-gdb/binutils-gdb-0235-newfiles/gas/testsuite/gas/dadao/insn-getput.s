# Da Dao Zhi Jian
	.text
	.global _start
_start:
	rd2rd	rd1, rd1, 63
	rd2rb	rb1, rd1, 63
	rb2rd	rd1, rb1, 63
	rb2rb	rb1, rb1, 63
	cmprb	rd1, rb1, rb2
	addrb	rb1, rb2, rd1
	subrb	rb1, rb2, rd1

	rd2rf	rf1, rd1, 63
	rf2rd	rd1, rf1, 63
	rf2rf	rf1, rf1, 63

	rd2ra	ra1, rd1, 63
	ra2rd	rd1, ra1, 63
