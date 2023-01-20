# See LICENSE for license details.

#include "encoding.h"

	.section ".text.init"
	.globl	_start
_start:
	rd2rd	rd1, rd0, 0
	rd2rd	rd2, rd0, 1
	rd2rd	rd4, rd0, 3
	rd2rd	rd8, rd0, 7
	rd2rd	rd16, rd0, 15
	rd2rd	rd32, rd0, 31

	move	rd8, trap_entry
	CPWR_mtvec(rd8)

	# give each core 128KB of stack
#define STKSHIFT 17
	move	rd8, 1
	shlu	rd8, rd8, STKSHIFT
	move	rb1, _end + 63
	andnw	rb1, w0, 63
	add	rb1, rb1, rd8

	jump	_init

	.align	2
trap_entry:
	addi	rb1, rb1, -512
	stmo	rd1, rb1, rd0, 63

	CPRD_mcause(rd16)
	CPRD_mepc(rd17)
	rb2rd	rd18, rb1, 0
	call	handle_trap
	// CPWR_mepc(rd16)

	ldmo	rd1, rb1, rd0, 63

	addi	rb1, rb1, 512
	rd2rb	rb7, rd16, 0
	jump	rb7, rd0, 0		/* mret */

.section ".tohost","aw",@progbits
	.align	6
	.globl	tohost
tohost:
	.dd.o64 0

	.align	6
	.globl	fromhost
fromhost:
	.dd.o64 0
