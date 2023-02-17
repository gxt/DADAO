#ifndef _ENV_PHYSICAL_SINGLE_CORE_H
#define _ENV_PHYSICAL_SINGLE_CORE_H

#include "../encoding.h"

//-----------------------------------------------------------------------
// Begin Macro
//-----------------------------------------------------------------------

#define DDTEST_DDUII							\
	.macro init;							\
	.endm

#define INIT_DATAREG							\
	rd2rd	rd1, rd0, 0;						\
	rd2rd	rd2, rd0, 1;						\
	rd2rd	rd4, rd0, 3;						\
	rd2rd	rd8, rd0, 7;						\
	rd2rd	rd16, rd0, 15;						\
	rd2rd	rd32, rd0, 31;

#define INIT_FLOATREG							\
	setw	rf0, w3, 0x47ef;					\
	setw	rf0, w2, 0xffff;					\
	setw	rf0, w1, 0xe000;					\
	setw	rf0, w0, 0x0000;

#define DDTEST_CODE_BEGIN						\
	.section .text.init;						\
	.align 6;							\
	.weak stvec_handler;						\
	.weak mtvec_handler;						\
	.global _start;							\
_start:									\
	/* reset vector */						\
	move	rd8, trap_vector;					\
	cpwr	cp0, cr12, cr5, rd8; /* mtvec */			\
	jump	reset_vector;						\
	.align 2;							\
trap_vector:								\
	/* test whether the test came from pass/fail */			\
	move	rb32, 0x80001000;					\
	stt	TESTNUM, rb32, 0;					\
	stt	rd0, rb32, 4;						\
__wait_fromhost:							\
	ldtu	rd33, rb32, 0x40;	/* fromhost */			\
	brz	rd33, __wait_fromhost;					\
	stt	rd0, rb32, 0x40;					\
	cprd	cp0, cr13, cr1, rd33;	/* mepc */			\
	rd2rb	rb33, rd33, 0;						\
	jump	rb33, rd0, 0;		/* mret */			\
reset_vector:								\
	INIT_DATAREG;							\
	INIT_FLOATREG;							\
	rd2rd	TESTNUM, rd0, 0;					\
	init;								\
	swym; /* TODO */

//-----------------------------------------------------------------------
// Pass/Fail Macro
//-----------------------------------------------------------------------

#define DDTEST_CODE_END							\
	move	rd32, ((1337 << 1) | 1);				\
	move	rb32, 0x80001000;					\
	stt	rd32, rb32, 0;						\
__tohost_exit:								\
	jump	__tohost_exit;	/* noreturn */

//-----------------------------------------------------------------------
// Pass/Fail Macro
//-----------------------------------------------------------------------

#define DDTEST_PASS							\
	move	TESTNUM, 1;						\
	move	rd15, 93;						\
	trap	cp0, 0;

#define TESTNUM rd2
#define DDTEST_FAIL							\
	move rd15, 93;							\
	trap cp0, 0;

//-----------------------------------------------------------------------
// Data Section Macro
//-----------------------------------------------------------------------

#define DDTEST_DATA_BEGIN						\
	.pushsection .tohost,"aw",@progbits;				\
	.align 6; .global tohost; tohost: .dd.t32 0;			\
	.align 6; .global fromhost; fromhost: .dd.t32 0;		\
	.popsection;							\
	.align 4; .global begin_signature; begin_signature:

#define DDTEST_DATA_END .align 4; .global end_signature; end_signature:

#endif
