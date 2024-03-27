/* int setjmp (jmp_buf);  */
  .globl  setjmp
  .type   setjmp, @function
setjmp:
	rd2rb	rb8, rd16, 1
	setzw	rd16, w0, 0

	#stmrb	rb1, rb8, rd16, 2
	strb	rb1, rb8, 0
	strb	rb2, rb8, 8
	addi	rd16, rd16, 2*8

	stmo	rd32, rb8, rd16, 32
	addi	rd16, rd16, 32*8
	stmrb	rb32, rb8, rd16, 32
	addi	rd16, rd16, 32*8
	stmfo	rf32, rb8, rd16, 32
	addi	rd16, rd16, 32*8

	stmra	ra1, rb8, rd16, 63
	addi	rd16, rd16, 64*8

	ret		rd31, 0

/* volatile void longjmp (jmp_buf, int);  */
  .globl  longjmp
  .type   longjmp, @function
longjmp:
	rd2rb	rb8, rd16, 1
	setzw	rd16, w0, 0

	#ldmrb	rb1, rb8, rd16, 2
	ldrb	rb1, rb8, 0
	ldrb	rb2, rb8, 8
	addi	rd16, rd16, 2*8

	ldmo	rd32, rb8, rd16, 32
	addi	rd16, rd16, 32*8
	ldmrb	rb32, rb8, rd16, 32
	addi	rd16, rd16, 32*8
	ldmfo	rf32, rb8, rd16, 32
	addi	rd16, rd16, 32*8

	ldmra	ra1, rb8, rd16, 63
	addi	rd16, rd16, 64*8

	# rd31 = (rd17 == 0) ? 1 : rd17
	setzw	rd31, w0, 1
	csne	rd17, rd0, rd31, rd17
	ret		rd0, 0
