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
	.dd.octa 2009

	.text
	.global _start

# The __Stack_start symbol is provided by the link script.
stackpp:
	.dd.octa __Stack_start
crtstxt:
	.dd.octa _init
	.dd.octa __etext

crtsdat:
	.dd.octa dstart
	.dd.octa _end

_start:	setwl	$63, 32
	put	rG, $63

	geta	$63, crtstxt
	ldo	$2, $63, 0
	andnwl	$2, 0x7ff
	ldo	$3, $63, 8
	setwl	$4, 2048
0:
	addu	$2, $2, $4
	cmp	$63, $2, $3
	BN	$63,0b

	geta	$63, crtsdat
	ldo	$2, $63, 0
	andnwl	$2, 0x7ff
	ldo	$3, $63, 8
0:
	addu	$2, $2, $4
	cmp	$63, $2, $3
	BN	$63,0b

# Initialize the stack pointer.  It is supposedly made a global
# zero-initialized (allowed to change) register in crtn.S; we use the
# explicit number.
	geta	$63, stackpp
	ldo	$62, $63, 0

	call	_init

# Copy argc and argv from their initial position to argument registers
# where necessary.
	or	$39, $0, 0
	or	$40, $1, 0

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
	get	$0, rJ
	bz	$0, 0f
	setwl	$63, 63
	trap	0
0:	trap	0

# Register _fini to be executed as the last atexit function.
	geta	$39, _fini
	call	atexit

	.section .fini,"ax",@progbits
	.global	_fini
_fini:
	get	$0, rJ
	bz	$0, 0f
	setwl	$63, 63
	trap	0
0:	trap	0
