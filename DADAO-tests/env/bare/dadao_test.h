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

#define DDTEST_CODE_BEGIN						\
	.section .text.init;						\
	.align 6;							\
	.weak stvec_handler;						\
	.weak mtvec_handler;						\
	.global _start;							\
_start:									\
	/* reset vector */						\
	jump reset_vector;						\
	.align 2;							\
trap_vector:								\
	/* TODO */							\
reset_vector:								\
	INIT_DATAREG;							\
	rd2rd	TESTNUM, rd0, 0;					\
	init;								\
	swym; /* TODO */

//-----------------------------------------------------------------------
// Pass/Fail Macro
//-----------------------------------------------------------------------

#define DDTEST_CODE_END							\
//	unimp;
	swym; /* TODO */

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
	trap cp0, 0; /* TODO */

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
