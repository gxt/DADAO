/*
 * Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
 */

# This is the crt0 equivalent for dadao, for setting up
# things for compiler-generated assembly-code and for setting up things
# between where the simulator calls and main, and shutting things down on
# the way back.  There's an actual crt0.o elsewhere, but that's a dummy.

# This little treasure (some contents) is required so the 32 lowest
# address bits of user data will not be zero.  Because of truncation,
# that would cause testcase gcc.c-torture/execute/980701-1.c to
# incorrectly fail.

	.data
	.p2align 3
dstart:
	.dd.o64	2009

	.text
	.global _start

# The __Stack_start symbol is provided by the link script.
stackpp:
	.dd.o64 __Stack_start
crtstxt:
	.dd.o64	_init
	.dd.o64	__etext

crtsdat:
	.dd.o64	 dstart
	.dd.o64	_end

_start:
	geta	rg63, crtstxt
	ldo	rg2, rp63, 0
#	andnwl	rg2, 0x7ff
	ldo	rg3, rp63, 8
	setwl	rg4, 2048
0:
	addu	rg2, rg2, rg4
	cmp	rg63, rg2, rg3
	br_n	rg63, 0b

	geta	rg63, crtsdat
	ldo	rg2, rp63, 0
#	andnwl	rg2, 0x7ff
	ldo	rg3, rp63, 8
0:
	addu	rg2, rg2, rg4
	cmp	rg63, rg2, rg3
	br_n	rg63,0b

# Initialize the stack pointer.  It is supposedly made a global
# zero-initialized (allowed to change) register in crtn.S; we use the
# explicit number.
	geta	rg63, stackpp
	ldo	rg62, rp63, 0

	call	_init

# Copy argc and argv from their initial position to argument registers
# where necessary.
	or	rg39, rg0, 0
	or	rg40, rg1, 0

	call	main
	call	exit

# Provide the first part of _init and _fini.  Save the return address on the
# register stack.  We eventually ignore the return address of these
# PUSHJ:s, so it doesn't matter that whether .init and .fini code calls
# functions or where they store rJ.  We shouldn't get there, so die
# (TRAP Halt) if that happens.

	.section .init,"ax",@progbits
	.global	_init
_init:
#	get.rs	rg0, rJ
	br_z	rg0, 0f
	setwl	rg63, 63
	trap	0
0:	trap	0

# Register _fini to be executed as the last atexit function.
	geta	rg39, _fini
	call	atexit

	.section .fini,"ax",@progbits
	.global	_fini
_fini:
#	get.rs	rg0, rJ
	br_z	rg0, 0f
	setwl	rg63, 63
	trap	0
0:	trap	0