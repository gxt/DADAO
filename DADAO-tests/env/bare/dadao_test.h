#ifndef _ENV_PHYSICAL_SINGLE_CORE_H
#define _ENV_PHYSICAL_SINGLE_CORE_H

#include "../encoding.h"

//-----------------------------------------------------------------------
// Begin Macro
//-----------------------------------------------------------------------

#define DDTEST_DDUII							\
	.macro init;							\
	.endm

#define INIT_RD_RB_RF							\
	rd2rd	rd1, rd0, 1;						\
	rd2rd	rd2, rd0, 2;						\
	rd2rd	rd4, rd0, 4;						\
	rd2rd	rd8, rd0, 8;						\
	rd2rd	rd16, rd0, 16;						\
	rd2rd	rd32, rd0, 32;						\
	rd2rb	rb1, rd1, 63;						\
	rd2rf	rf1, rd1, 63;

#define DDTEST_CODE_BEGIN						\
	.section .text.init;						\
	.align 6;							\
	.weak stvec_handler;						\
	.weak mtvec_handler;						\
	.global _start;							\
_start:									\
	/* reset vector */						\
	INIT_RD_RB_RF;							\
	CPRD_mhartid(rd8);						\
1:	brnz	rd8, 1b;						\
	setrd	TESTNUM, 0;						\
	setrd	rd8, trap_vector;					\
	CPWR_mtvec(rd8);						\
	CPWR_mstatus(rd0);						\
	init;								\
	setrd	rd8, _test_start;					\
	CPWR_mepc(rd8);							\
	escape	cp0, 0; /* should goto _test_start */			\
trap_vector:								\
	/* test whether the test came from pass/fail */			\
	CPRD_mcause(rd8);						\
	setrd	rd9, CAUSE_USER_ECALL;					\
	breq	rd9, rd8, write_tohost;					\
	setrd	rd9, CAUSE_SUPERVISOR_ECALL;				\
	breq	rd9, rd8, write_tohost;					\
	setrd	rd9, CAUSE_MACHINE_ECALL;				\
	breq	rd9, rd8, write_tohost;					\
	/* some unhandlable exception occurred */			\
	orw	TESTNUM, w0, 1337;					\
write_tohost:								\
	setrb	rb8, tohost;						\
	stt	TESTNUM, rb8, 0;					\
	stt	rd0, rb8, 4;						\
	jump	write_tohost;						\
_test_start:

//-----------------------------------------------------------------------
// Pass/Fail Macro
//-----------------------------------------------------------------------

#define DDTEST_CODE_END							\
	unimp	0;							\
	/* SHOULD cause illegal instruction */

//-----------------------------------------------------------------------
// Pass/Fail Macro
//-----------------------------------------------------------------------

#define DDTEST_PASS							\
	fence	0;							\
	setrd	TESTNUM, 1;						\
	setrd	rd15, 93;						\
	setrd	rd16, 0;						\
	trap	cp0, 0;

#define TESTNUM rd2
#define DDTEST_FAIL							\
	fence	0;							\
1:	breq	rd0, TESTNUM, 1b;					\
	shlu	TESTNUM, TESTNUM, 1;					\
	orw	TESTNUM, w0, 1;						\
	setrd	rd15, 93;						\
	rd2rd	rd16, TESTNUM, 1;					\
	trap	cp0, 0;

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
