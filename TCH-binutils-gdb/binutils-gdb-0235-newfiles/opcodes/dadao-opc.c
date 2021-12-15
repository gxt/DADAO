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
	/* General registers */
	{"zero", 0},
	{"datao1", 1},	/* internal reg for as, SHOULD removed at last */
	{"datao2", 2},	/* internal reg for as, SHOULD removed at last */
	{"datao3", 3},	/* internal reg for as, SHOULD removed at last */
	/* Pointer registers */
	//{"pc", 0x40 + 0},
	//{"sp", 0x40 + 1},
	//{"fp", 0x40 + 2},

	{NULL, 0}
 };

const struct dadao_opcode dadao_opcodes[] = {
#define	_OP(_op)	XCONCAT2 (dadao_operand_,_op)
#define	_T(_t)		XCONCAT2 (dadao_type_,_t)

#include "dadao-insn.opc"

	/* pseudo instructions */
	/* name		op-num	maj-op	min-op	fa		fb		fc		fd		insn-type */
	{"setrg",	2,	0,	0,	_OP(rg),	_OP(noop),	_OP(none),	_OP(none),	_T(pseudo)},

	{"setrp",	2,	1,	0,	_OP(rg),	_OP(noop),	_OP(none),	_OP(none),	_T(pseudo)},

	{NULL,		0,	0,	0,	_OP(noop),	_OP(noop),	_OP(noop),	_OP(noop),	_T(pseudo)},

#undef	_OP
#undef	_T
 };
