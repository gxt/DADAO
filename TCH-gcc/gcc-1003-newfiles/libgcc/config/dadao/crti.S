/*
 * Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
 */
	.text
	.global _start
_start:
	addi	rb1, rb1, -8
	strb	rb2, rb1, 0
	rb2rb	rb2, rb1, 1
	addi	rb2, rb2, 8

	call	_init
	call	main

	# rd5 as caller saved
	rd2rd	rd32, rd31, 1

	call	_fini

	ldrb    rb2, rb1, 0
	addi	rb1, rb1, 8

	# across the procedure call of _fini
	rd2rd	rd16, rd32, 1
	jump	_exit

	.section .init,"ax",@progbits
	.global	_init
_init:
	swym	0

	.section .fini,"ax",@progbits
	.global	_fini
_fini:
	swym	0
