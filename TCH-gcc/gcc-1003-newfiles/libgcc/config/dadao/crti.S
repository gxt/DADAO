/*
 * Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
 */
	.text
	.global _start
_start:
	addi	rb1, -8
	strb	rb2, rb1, 0
	rb2rb	rb2, rb1, 0
	addi	rb2, 8

	call	_init
	call	main
	call	_fini

	ldrb    rb2, rb1, 0
	addi	rb1, 8
	# To exit, generate an exception by return from empty RAS
	ret
	swym

	.section .init,"ax",@progbits
	.global	_init
_init:
#	rb2rb	rb2, rb1, 0
#	addi	rb2, -8
	swym

	.section .fini,"ax",@progbits
	.global	_fini
_fini:
#	rb2rb	rb2, rb1, 0
#	addi	rb2, -8
	swym
