# Da Dao Zhi Jian
	.text
	.global _start
_start:
	ft2fo	rf0, rf0, 63
	fo2ft	rf0, rf0, 63
	ft2rd	rd0, rf0, 63
	fo2rd	rd0, rf0, 63
	rd2ft	rf0, rd0, 63
	rd2fo	rf0, rd0, 63

	ftadd	rf1, rf2, rf3
	ftsub	rf1, rf2, rf3
	ftmul	rf1, rf2, rf3
	ftdiv	rf1, rf2, rf3
	foadd	rf1, rf2, rf3
	fosub	rf1, rf2, rf3
	fomul	rf1, rf2, rf3
	fodiv	rf1, rf2, rf3

	ftabs	rf0, rf0, 63
	ftneg	rf0, rf0, 63
	ftsqrt	rf0, rf0, 63
	foabs	rf0, rf0, 63
	foneg	rf0, rf0, 63
	fosqrt	rf0, rf0, 63

	ftcun	rd1, rf1, rf2
	ftcor	rd1, rf1, rf2
	ftcne	rd1, rf1, rf2
	ftceq	rd1, rf1, rf2
	ftclt	rd1, rf1, rf2
	ftcge	rd1, rf1, rf2
	ftcgt	rd1, rf1, rf2
	ftcle	rd1, rf1, rf2

	focun	rd1, rf1, rf2
	focor	rd1, rf1, rf2
	focne	rd1, rf1, rf2
	foceq	rd1, rf1, rf2
	foclt	rd1, rf1, rf2
	focge	rd1, rf1, rf2
	focgt	rd1, rf1, rf2
	focle	rd1, rf1, rf2
