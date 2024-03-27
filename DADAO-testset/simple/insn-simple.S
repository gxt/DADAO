# Da Dao Zhi Jian
	.text
	.global _start
_start:
	swym	0
	# 0001-0000: MISC-Norm
	and	rd1, rd2, rd3
	orr	rd1, rd2, rd3
	xor	rd1, rd2, rd3
	shlu	rd1, rd2, rd3
	shrs	rd1, rd2, rd3
	shru	rd1, rd2, rd3
	exts	rd1, rd2, rd3
	extz	rd1, rd2, rd3
	not	rd1, rd2, rd3
	shlu	rd1, rd2, 3
	shrs	rd1, rd2, 3
	shru	rd1, rd2, 3
	exts	rd1, rd2, 3
	extz	rd1, rd2, 3
	not	rd1, rd2, 3
	cmps	rd1, rd2, rd3
	cmpu	rd1, rd2, rd3
	rd2rd	rd1, rd2, 4
	rd2rb	rb1, rd2, 4
	rb2rd	rd1, rb2, 4
	rb2rb	rb1, rb2, 4
	cmprb	rd1, rb2, rb3
	addrb	rb1, rb2, rd3
	subrb	rb1, rb2, rd3
	rd2rf	rf1, rd2, 4
	rf2rd	rd1, rf2, 4
	rf2rf	rf1, rf2, 4
	rd2ra	ra1, rd2, 4
	ra2rd	rd1, ra2, 4
	# 0001-0xxx
	addi	rd1, 2
	add	rd1, rd2, rd3, rd4
	sub	rd1, rd2, rd3, rd4
	muls	rd1, rd2, rd3, rd4
	mulu	rd1, rd2, rd3, rd4
	divs	rd1, rd2, rd3, rd4
	divu	rd1, rd2, rd3, rd4
	# 0001-1xxx
	cmps	rd1, rd2, 3
	cmpu	rd1, rd2, 3
	orw	rd1, w3, 2
	orw	rd1, w2, 2
	orw	rd1, w1, 2
	orw	rd1, w0, 2
	andnw	rd1, w3, 2
	andnw	rd1, w2, 2
	andnw	rd1, w1, 2
	andnw	rd1, w0, 2
	setzw	rd1, w3, 2
	setzw	rd1, w2, 2
	setzw	rd1, w1, 2
	setzw	rd1, w0, 2
	setow	rd1, w3, 2
	setow	rd1, w2, 2
	setow	rd1, w1, 2
	setow	rd1, w0, 2
	# 0010-0xxx
	csn	rd1, rd2, rd3, rd4
	csnn	rd1, rd2, rd3, rd4
	csz	rd1, rd2, rd3, rd4
	csnz	rd1, rd2, rd3, rd4
	csp	rd1, rd2, rd3, rd4
	csnp	rd1, rd2, rd3, rd4
	csod	rd1, rd2, rd3, rd4
	csev	rd1, rd2, rd3, rd4
	# 0010-1xxx
	brn	rd1, _start
	brnn	rd1, _start
	brz	rd1, _start
	brnz	rd1, _start
	brp	rd1, _start
	brnp	rd1, _start
	brod	rd1, _start
	brev	rd1, _start
	# 0011-0xxx
	ldbs	rd1, rb2, 3
	ldws	rd1, rb2, 3
	ldts	rd1, rb2, 3
	ldo	rd1, rb2, 3
	ldmbs	rd1, rb2, rd3, 5
	ldmws	rd1, rb2, rd3, 5
	ldmts	rd1, rb2, rd3, 5
	ldmo	rd1, rb2, rd3, 5
	# 0011-1xxx
	stb	rd1, rb2, 3
	stw	rd1, rb2, 3
	stt	rd1, rb2, 3
	sto	rd1, rb2, 3
	stmb	rd1, rb2, rd3, 5
	stmw	rd1, rb2, rd3, 5
	stmt	rd1, rb2, rd3, 5
	stmo	rd1, rb2, rd3, 5
	# 0100-0xxx
	ldbu	rd1, rb2, 3
	ldwu	rd1, rb2, 3
	ldtu	rd1, rb2, 3
	ldrb	rb1, rb2, 3
	ldmbu	rd1, rb2, rd3, 5
	ldmwu	rd1, rb2, rd3, 5
	ldmtu	rd1, rb2, rd3, 5
	ldmrb	rb1, rb2, rd3, 5
	# 0100-1xxx
	addi	rb1, 1
	strb	rb1, rb2, 2
	orw	rb1, w3, 3
	orw	rb1, w2, 3
	orw	rb1, w1, 3
	orw	rb1, w0, 3
	andnw	rb1, w3, 3
	andnw	rb1, w2, 3
	andnw	rb1, w1, 3
	andnw	rb1, w0, 3
	setzw	rb1, w3, 3
	setzw	rb1, w2, 3
	setzw	rb1, w1, 3
	setzw	rb1, w0, 3
	stmrb	rb1, rb2, rd3, 5
	# 0101-0000L MISC-RF
	ft2fo	rf1, rf2, 3
	fo2ft	rf1, rf2, 3
	ft2rd	rd1, rf2, 3
	fo2rd	rd1, rf2, 3
	rd2fo	rf1, rd2, 3
	rd2ft	rf1, rd2, 3
	ftadd	rf1, rf2, rf3
	ftsub	rf1, rf2, rf3
	ftmul	rf1, rf2, rf3
	ftdiv	rf1, rf2, rf3
	ftabs	rf1, rf2, 3
	ftneg	rf1, rf2, 3
	ftsqrt	rf1, rf2, 3
	ftcun	rd1, rf2, rf3
	ftcor	rd1, rf2, rf3
	ftcne	rd1, rf2, rf3
	ftceq	rd1, rf2, rf3
	ftclt	rd1, rf2, rf3
	ftcge	rd1, rf2, rf3
	ftcgt	rd1, rf2, rf3
	ftcle	rd1, rf2, rf3
	foadd	rf1, rf2, rf3
	fosub	rf1, rf2, rf3
	fomul	rf1, rf2, rf3
	fodiv	rf1, rf2, rf3
	foabs	rf1, rf2, 3
	foneg	rf1, rf2, 3
	fosqrt	rf1, rf2, 3
	focun	rd1, rf2, rf3
	focor	rd1, rf2, rf3
	focne	rd1, rf2, rf3
	foceq	rd1, rf2, rf3
	foclt	rd1, rf2, rf3
	focge	rd1, rf2, rf3
	focgt	rd1, rf2, rf3
	focle	rd1, rf2, rf3
	# 0101-0xxx
	ldft	rf1, rb2, 3
	ldfo	rf1, rb2, 3
	setw	rf1, w3, 1
	setw	rf1, w2, 2
	setw	rf1, w1, 3
	setw	rf1, w0, 4
	ldmft	rf1, rb2, rd3, 5
	ldmfo	rf1, rb2, rd3, 5
	stft	rf1, rb2, 3
	stfo	rf1, rb2, 3
	stmft	rf1, rb2, rd3, 5
	stmfo	rf1, rb2, rd3, 5
	# 0110-0xxx
	jump	_start
	call	rb1, rd2, 3
	ldmra	ra1, rb2, rd3, 5
	# 0110-1xxx
	call	_start
	call	rb1, rd2, 3
	ret	rd0, 0
	stmra	ra1, rb2, rd3, 5
	# 0111-0xxx
	trap	cp0, 1
	cpcoop	cp1, 2
	rd2cr	cp1, cr2, cr3, rd4
	cr2rd	cp1, rd2, cr3, cr4
	cpload	cp1, rb2, 3
	cpstor	cp1, rb2, 3
	# pseudo insn
	setrd	rd1, 0xabcdabcdabcdabcd
	setrd	rd2, -1
	setrd	rd3, _start
	setrb	rb1, 0xabcdabcdabcdabcd
	setrb	rb2, -1
	setrb	rb3, _start
