/*
 * Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
 */
	.text
	.global _start
_start:
	call	main

	.section .init,"ax",@progbits
	.global	_init
_init:
	rb2rd	rd7, rb1, 0
	add	rd7, -8
	rd2rb	rb2, rd7, 0

	.section .fini,"ax",@progbits
	.global	_fini
_fini:
	rb2rd    rd7, rb1, 0
        add     rd7, -8
        rd2rb   rb2, rd7, 0
