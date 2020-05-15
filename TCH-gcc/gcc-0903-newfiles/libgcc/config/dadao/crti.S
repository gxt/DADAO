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

_start:	SETL	$63,32
	PUT	rG,$63

	geta	$63, crtstxt
	ldo	$2, $63, 0
	ANDNL	$2,#7ff
	ldo	$3, $63, 8
	SETL	$4,2048
0:
	addu	$2, $2, $4
	cmp	$63, $2, $3
	BN	$63,0b

	geta	$63, crtsdat
	ldo	$2, $63, 0
	ANDNL	$2,#7ff
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

	PUSHJ	$2,_init

#ifdef __DADAO_ABI_GNU__
# Copy argc and argv from their initial position to argument registers
# where necessary.
	SET	$39,$0
	SET	$40,$1
#else
# For the dadao ABI, we need to move arguments.  The return value will
# appear in $0.
	SET	$2,$1
	SET	$1,$0
#endif

	PUSHJ	$0,main
	jmp	$0, exit

# Provide the first part of _init and _fini.  Save the return address on the
# register stack.  We eventually ignore the return address of these
# PUSHJ:s, so it doesn't matter that whether .init and .fini code calls
# functions or where they store rJ.  We shouldn't get there, so die
# (TRAP Halt) if that happens.

	.section .init,"ax",@progbits
	.global	_init
_init:
	GET	$0,rJ
	PUSHJ	$1,0f
	SETL	$63,63
	trap	0
0:	trap	0

# Register _fini to be executed as the last atexit function.
#ifdef __DADAO_ABI_GNU__
	geta	$39, _fini
#else
	geta	$1, _fini
#endif
	PUSHJ	$0,atexit

	.section .fini,"ax",@progbits
	.global	_fini
_fini:
	GET	$0,rJ
	PUSHJ	$1,0f
	SETL	$63,63
	trap	0
0:	trap	0
