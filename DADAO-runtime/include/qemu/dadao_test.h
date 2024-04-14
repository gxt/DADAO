#ifndef _RUNTIME_QEMU_DDTEST_H_
#define _RUNTIME_QEMU_DDTEST_H_

#include "simrisc/ddtest.h"
#include "simrisc/encoding.h"

#define DDTEST_DDUII

#define DDTEST_CODE_BEGIN						\
	.text;								\
	.globl _start;							\
	.align 4;							\
_start:									\
	setrd	RD_FLAG, 0;					\
	setrd	RD_NUMR, 0;					\
	setrd	RD_PASS, 0;					\
	setrd	RD_EXP1, 0;					\
	setrd	RD_EXP2, 0;					\
	setrd	RD_RET1, 0;					\
	setrd	RD_RET2, 0;

#define DDTEST_CODE_END							\
	brnz	RD_FLAG, ___fail;					\
	/* SHOULD handle pass first */				\
	setrd	rd16, 1;							\
	setrd	rd17, __passstr;					\
	setrd	rd18, __passlen;					\
	setrd	rd15, 64; /* write */				\
	trap	cp0, 0;								\
	setrd	rd16, 0;							\
	setrd	rd15, 93; /* exit */				\
	trap	cp0, 0;								\
___fail:										\
	setrd	rd16, 1;							\
	setrd	rd17, __failstr;					\
	setrd	rd18, __faillen;					\
	setrd	rd15, 64; /* write */				\
	trap	cp0, 0;								\
	rd2rd	rd16, TESTNUM, 1;					\
	setrd	rd15, 93; /* exit */				\
	trap	cp0, 0;								\
__passstr:										\
	.ascii  "PASS";								\
	.equ __passlen, . - __passstr;				\
	.type __passstr, object;					\
	.size __passstr, __passlen;					\
__failstr:										\
	.ascii  "FAIL\n";							\
	.equ __faillen, . - __failstr;				\
	.type __failstr, object;					\
	.size __failstr, __faillen;					\

#define DDTEST_DATA_BEGIN						\
	.section data;								\

#define DDTEST_DATA_END

#endif
