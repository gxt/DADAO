# Da Dao Zhi Jian
	.text
	.global _start
_start:
	ldbs	rd1, rb1, 2047
	ldbs	rd1, rb1, -2048
	ldws	rd1, rb1, 2047
	ldws	rd1, rb1, -2048
	ldts	rd1, rb1, 2047
	ldts	rd1, rb1, -2048
	ldo	rd1, rb1, 2047
	ldo	rd1, rb1, -2048
	ldbu	rd1, rb1, 2047
	ldbu	rd1, rb1, -2048
	ldwu	rd1, rb1, 2047
	ldwu	rd1, rb1, -2048
	ldtu	rd1, rb1, 2047
	ldtu	rd1, rb1, -2048

	stb	rd1, rb1, 2047
	stb	rd1, rb1, -2048
	stw	rd1, rb1, 2047
	stw	rd1, rb1, -2048
	stt	rd1, rb1, 2047
	stt	rd1, rb1, -2048
	sto	rd1, rb1, 2047
	sto	rd1, rb1, -2048

	ldmbs	rd0, rb1, rd1, 63
	ldmws	rd0, rb1, rd1, 63
	ldmts	rd0, rb1, rd1, 63
	ldmo	rd0, rb1, rd1, 63
	ldmbu	rd0, rb1, rd1, 63
	ldmwu	rd0, rb1, rd1, 63
	ldmtu	rd0, rb1, rd1, 63

	stmb	rd0, rb1, rd1, 63
	stmw	rd0, rb1, rd1, 63
	stmt	rd0, rb1, rd1, 63
	stmo	rd0, rb1, rd1, 63

	ldrb	rb1, rb2, 2047
	ldrb	rb1, rb2, -2048
	strb	rb1, rb2, 2047
	strb	rb1, rb2, -2048

	ldmrb	rb0, rb1, rd1, 63
	stmrb	rb0, rb1, rd1, 63

	ldft	rf1, rb1, 2047
	ldft	rf1, rb1, -2048
	ldfo	rf1, rb1, 2047
	ldfo	rf1, rb1, -2048
	stft	rf1, rb1, 2047
	stft	rf1, rb1, -2048
	stfo	rf1, rb1, 2047
	stfo	rf1, rb1, -2048

	ldmft	rf0, rb1, rd1, 63
	ldmfo	rf0, rb1, rd1, 63
	stmft	rf0, rb1, rd1, 63
	stmfo	rf0, rb1, rd1, 63

	ldmra	ra0, rb1, rd1, 63
	stmra	ra0, rb1, rd1, 63
