/*
 * Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
 */
	.text
	.global _start
_start:
	call	_init
	call	main
	call	_fini

	# To exit, generate an exception by return from empty RAS
	ret
	swym

	.section .init,"ax",@progbits
	.global	_init
_init:
	rb2rb	rb2, rb1, 0
	addrb	rb2, -8

	.section .fini,"ax",@progbits
	.global	_fini
_fini:
	rb2rb	rb2, rb1, 0
	addrb	rb2, -8
