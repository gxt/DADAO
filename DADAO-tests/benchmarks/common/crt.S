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
	cpwr	cp0, cr12, cr5, rd8	/* mtvec */

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

	cprd	cp0, cr13, cr2, rd16	/* mcause: 0x342 */
	cprd	cp0, cr13, cr1, rd17	/* mepc: 0x341 */
	rb2rd	rd18, rb1, 0
	call	handle_trap
//	cpwr	cp0, cr13, cr1, rd16	/* mepc: 0x341 */

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
