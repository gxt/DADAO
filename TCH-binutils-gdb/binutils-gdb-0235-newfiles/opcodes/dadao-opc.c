/*
 * dadao-opc.c -- Definitions for dadao opcodes.
 * Copyright (C) 2019-2033 Guan Xuetao (AT) Peking Univ.
 *
 * Contributed by:
 *   2019:
 *	Guan Xuetao <gxt@pku.edu.cn>
 */
#include <stdio.h>
#include "opcode/dadao.h"
#include "symcat.h"

/* Register-name-table */
const struct dadao_reg_alias dadao_reg_aliases[] =
 {
	/* Data registers */
	{"rdzero", 0},
	{"rderrno", 1},
	{"rdt0", 8},
	{"rdt1", 9},
	{"rdt2", 10},
	{"rdt3", 11},
	{"rdt4", 12},
	{"rdt5", 13},
	{"rdt6", 14},
	{"rdt7", 15},
	{"rda0", 16},
	{"rda1", 17},
	{"rda2", 18},
	{"rda3", 19},
	{"rda4", 20},
	{"rda5", 21},
	{"rda6", 22},
	{"rda7", 23},
	{"rda8", 24},
	{"rda9", 25},
	{"rda10", 26},
	{"rda11", 27},
	{"rda12", 28},
	{"rda13", 29},
	{"rda14", 30},
	{"rda15", 31},
	/* Base registers */
	{"rbip", 0x40 + 0},
	{"rbsp", 0x40 + 1},
	{"rbfp", 0x40 + 2},
	{"rbgp", 0x40 + 3},
	{"rbtp", 0x40 + 4},
	{"rbt0", 0x40 + 8},
	{"rbt1", 0x40 + 9},
	{"rbt2", 0x40 + 10},
	{"rbt3", 0x40 + 11},
	{"rbt4", 0x40 + 12},
	{"rbt5", 0x40 + 13},
	{"rbt6", 0x40 + 14},
	{"rbt7", 0x40 + 15},
	{NULL, 0}
 };

const struct dadao_opcode dadao_opcodes[] = {
#define	_OP(_op)	XCONCAT2 (dadao_operand_,_op)
#define	_T(_t)		XCONCAT2 (dadao_type_,_t)

#include "dadao-insn.opc"

	/* pseudo instructions */
	/* name		op-num	maj-op	min-op	fa		fb		fc		fd		insn-type */
	{"move",	2,	0,	0,	_OP(rd),	_OP(noop),	_OP(none),	_OP(none),	_T(pseudo)},

	{"move",	2,	1,	0,	_OP(rb),	_OP(noop),	_OP(none),	_OP(none),	_T(pseudo)},

	{NULL,		0,	0,	0,	_OP(noop),	_OP(noop),	_OP(noop),	_OP(noop),	_T(pseudo)},

#undef	_OP
#undef	_T
 };
