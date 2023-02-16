# Da Dao Zhi Jian
	.text
	.global _start
_start:
	rd2rd	rd0, rd0, 63
	rd2rb	rb0, rd0, 63
	rb2rd	rd0, rb0, 63
	rb2rb	rb0, rb0, 63

	rd2rf	rf0, rd0, 63
	rf2rd	rd0, rf0, 63
	rf2rf	rf0, rf0, 63

	rd2ra	ra0, rd0, 63
	ra2rd	rd0, ra0, 63

	csn	rd1, rd2, rd3, rd4
	csz	rd1, rd2, rd3, rd4
	csp	rd1, rd2, rd3, rd4
	cseq	rd1, rd2, rd3, rd4
	csne	rd1, rd2, rd3, rd4
