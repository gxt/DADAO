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
	{"pc", 0x40 + 0},
	{"sp", 0x40 + 1},
	{"fp", 0x40 + 2},

	{NULL, 0}
 };

/* Opcode-table.  In order to cut down on redundant contents, we use helper
   macros.  */

/* For easier overview of the table.  */
#define I(i)	(XCONCAT2 (DADAO_INSN_,i) >> 24)
#define W(w)	(XCONCAT2 (DADAO_WYDE_,w) >> 16)
#define OP(y)	XCONCAT2 (dadao_operands_,y)
#define T(t)	XCONCAT2 (dadao_type_,t)

/* Groups of instructions specified here must, if all are matching the
   same instruction, be consecutive, in order more-specific to
   less-specific match.  */

const struct dadao_opcode dadao_opcodes[] =
 {
	{"add",		0x20,		0,	1,	OP (rrii_rrri),		T (simm)},
	{"addu",	0x22,		0,	1,	OP (rrii_rrri),		T (uimm)},
	{"sub",		0x24,		0,	1,	OP (rrii_rrri),		T (simm)},
	{"subu",	0x26,		0,	1,	OP (rrii_rrri),		T (uimm)},

	{"add.rp",	0x28,		0,	1,	OP (rrii_rrri),		T (regp)},

	{"mul",		0x30,		0,	1,	OP (rrii_rrri),		T (simm)},
	{"mulu",	0x35,		0,	0,	OP (rrrr),		T (normal)},
	{"div",		0x36,		0,	0,	OP (rrrr),		T (normal)},
	{"divu",	0x37,		0,	0,	OP (rrrr),		T (normal)},

	{"setwl",	I (SETW),	W (WL),	0,	OP (rjii),		T (normal)},
	{"setwk",	I (SETW),	W (WK),	0,	OP (rjii),		T (normal)},
	{"setwj",	I (SETW),	W (WJ),	0,	OP (rjii),		T (normal)},
	{"setwh",	I (SETW),	W (WH),	0,	OP (rjii),		T (normal)},

	{"incwl",	I (INCW),	W (WL),	0,	OP (rjii),		T (normal)},
	{"incwk",	I (INCW),	W (WK),	0,	OP (rjii),		T (normal)},
	{"incwj",	I (INCW),	W (WJ),	0,	OP (rjii),		T (normal)},
	{"incwh",	I (INCW),	W (WH),	0,	OP (rjii),		T (normal)},

	{"slu",		0x3E,		1,	1,	OP (orri_orrr),		T (normal)},
	{"sr",		0x3E,		2,	1,	OP (orri_orrr),		T (normal)},
	{"sru",		0x3E,		3,	1,	OP (orri_orrr),		T (normal)},
	{"not",		0x3E,		4,	1,	OP (orri_orrr),		T (normal)},

	{"or",		0x40,		0,	1,	OP (rrii_rrri),		T (uimm)},
	{"and",		0x42,		0,	1,	OP (rrii_rrri),		T (uimm)},
	{"xor",		0x44,		0,	1,	OP (rrii_rrri),		T (uimm)},

	{"cs.n",	0x60,		0,	0,	OP (rrrr),		T (normal)},
	{"cs.nn",	0x61,		0,	0,	OP (rrrr),		T (normal)},
	{"cs.z",	0x62,		0,	0,	OP (rrrr),		T (normal)},
	{"cs.nz",	0x63,		0,	0,	OP (rrrr),		T (normal)},
	{"cs.p",	0x64,		0,	0,	OP (rrrr),		T (normal)},
	{"cs.np",	0x65,		0,	0,	OP (rrrr),		T (normal)},
	{"cs.od",	0x66,		0,	0,	OP (rrrr),		T (normal)},
	{"cs.ev",	0x67,		0,	0,	OP (rrrr),		T (normal)},

	{"cmp",		0x70,		0,	1,	OP (rrii_rrri),		T (simm)},
	{"cmpu",	0x72,		0,	1,	OP (rrii_rrri),		T (uimm)},

	{"ldb",		0x80,		0,	1,	OP (rrii_rrri),		T (dref)},
	{"ldw",		0x82,		0,	1,	OP (rrii_rrri),		T (dref)},
	{"ldt",		0x84,		0,	1,	OP (rrii_rrri),		T (dref)},
	{"ldo",		0x86,		0,	1,	OP (rrii_rrri),		T (dref)},
	{"ldo.rp",	0x88,		0,	1,	OP (rrii_rrri),		T (regp_dref)},
	{"ldt.rf",	0x8A,		0,	1,	OP (rrii_rrri),		T (regf_dref)},
	{"ldbu",	0x90,		0,	1,	OP (rrii_rrri),		T (dref)},
	{"ldwu",	0x92,		0,	1,	OP (rrii_rrri),		T (dref)},
	{"ldtu",	0x94,		0,	1,	OP (rrii_rrri),		T (dref)},

	{"stb",		0xA0,		0,	1,	OP (rrii_rrri),		T (dref)},
	{"stw",		0xA2,		0,	1,	OP (rrii_rrri),		T (dref)},
	{"stt",		0xA4,		0,	1,	OP (rrii_rrri),		T (dref)},
	{"sto",		0xA6,		0,	1,	OP (rrii_rrri),		T (dref)},
	{"sto.rp",	0xA8,		0,	1,	OP (rrii_rrri),		T (regp_dref)},
	{"stt.rf",	0xAA,		0,	1,	OP (rrii_rrri),		T (regf_dref)},

	{"br.n",	0xC0,		0,	0,	OP (riii),		T (condbranch)},
	{"br.nn",	0xC1,		0,	0,	OP (riii),		T (condbranch)},
	{"br.z",	0xC2,		0,	0,	OP (riii),		T (condbranch)},
	{"br.nz",	0xC3,		0,	0,	OP (riii),		T (condbranch)},
	{"br.p",	0xC4,		0,	0,	OP (riii),		T (condbranch)},
	{"br.np",	0xC5,		0,	0,	OP (riii),		T (condbranch)},
	{"br.od",	0xC6,		0,	0,	OP (riii),		T (condbranch)},
	{"br.ev",	0xC7,		0,	0,	OP (riii),		T (condbranch)},
	{"geta",	I (GETA),	0,	0,	OP (riii),		T (geta)},

	{"call",	I (CALL),	0,	1,	OP (iiii_rrii),		T (jsr)},
	{"jump",	I (JUMP),	0,	1,	OP (iiii_rrii),		T (branch)},

	{"swym",	0xDA,		0,	0,	OP (oiii),		T (normal)},

	{"mov.rp",	0xDA,		1,	0,	OP (orr0),		T (normal)},
	{"mov.rf",	0xDA,		2,	0,	OP (orr0),		T (normal)},
	{"mov.rv",	0xDA,		3,	0,	OP (orr0),		T (normal)},

	{"put.rp",	0xDA,		0x11,	0,	OP (orr0),		T (normal)},
	{"put.rf",	0xDA,		0x12,	0,	OP (orr0),		T (normal)},
	{"put.rv",	0xDA,		0x13,	0,	OP (orr0),		T (normal)},

	{"get.rp",	0xDA,		0x21,	0,	OP (orr0),		T (normal)},
	{"get.rf",	0xDA,		0x22,	0,	OP (orr0),		T (normal)},
	{"get.rv",	0xDA,		0x23,	0,	OP (orr0),		T (normal)},

	/* nop is 0xDADADADA, DA is 11011010, so minor-opcode is 110110 */
	{"nop",		0xDA,		0x36,	0,	OP (o000),		T (normal)},

	{"ret",		0xDA,		0x37,	0,	OP (o000),		T (jsr)},
	{"trip",	0xDA,		0x38,	0,	OP (oiii),		T (jsr)},
	{"trap",	0xDA,		0x3F,	0,	OP (oiii),		T (jsr)},

	{"fcmp",	I (FP),		1,	0,	OP (orrr),		T (regf_fbrg)},
	{"fun",		I (FP),		2,	0,	OP (orrr),		T (regf_fbrg)},
	{"feql",	I (FP),		3,	0,	OP (orrr),		T (regf_fbrg)},
	{"fcmpe",	I (FP),		17,	0,	OP (orrr),		T (regf_fbrg)},
	{"fune",	I (FP),		18,	0,	OP (orrr),		T (regf_fbrg)},
	{"feqle",	I (FP),		19,	0,	OP (orrr),		T (regf_fbrg)},

	{"fadd",	I (FP),		4,	0,	OP (orrr),		T (regf)},
	{"fsub",	I (FP),		6,	0,	OP (orrr),		T (regf)},
	{"fmul",	I (FP),		16,	0,	OP (orrr),		T (regf)},
	{"fdiv",	I (FP),		20,	0,	OP (orrr),		T (regf)},
	{"frem",	I (FP),		22,	0,	OP (orrr),		T (regf)},

	{"fix",		I (FP),		5,	0,	OP (orri),		T (regf_fbrg)},
	{"fixu",	I (FP),		7,	0,	OP (orri),		T (regf_fbrg)},
	{"fsqrt",	I (FP),		21,	0,	OP (orri),		T (regf)},
	{"fint",	I (FP),		23,	0,	OP (orri),		T (regf)},

	{"flot",	I (FP),		8,	0,	OP (orri),		T (regf_fcrg)},
	{"flotu",	I (FP),		10,	0,	OP (orri),		T (regf_fcrg)},
	{"sflot",	I (FP),		12,	0,	OP (orri),		T (regf_fcrg)},
	{"sflotu",	I (FP),		14,	0,	OP (orri),		T (regf_fcrg)},

	/* floating point instructions (single precision) */
	{"ft.add",	I (FT),		1,	0,	OP (orrr),		T (regf)},
	{"ft.sub",	I (FT),		2,	0,	OP (orrr),		T (regf)},
	{"ft.mul",	I (FT),		3,	0,	OP (orrr),		T (regf)},
	{"ft.div",	I (FT),		4,	0,	OP (orrr),		T (regf)},

	{"ft.abs",	I (FT),		8,	0,	OP (orr0),		T (regf)},
	{"ft.neg",	I (FT),		9,	0,	OP (orr0),		T (regf)},

	{"ft.f2i",	I (FT),		10,	0,	OP (orr0),		T (regf)},
	{"ft.i2f",	I (FT),		11,	0,	OP (orr0),		T (regf)},
	{"ft.2fo",	I (FT),		12,	0,	OP (orr0),		T (regf)},

	{"ftc.un",	I (FT),		32,	0,	OP (orrr),		T (regf)},
	{"ftc.or",	I (FT),		33,	0,	OP (orrr),		T (regf)},
	{"ftc.eq",	I (FT),		34,	0,	OP (orrr),		T (regf)},
	{"ftc.ne",	I (FT),		35,	0,	OP (orrr),		T (regf)},
	{"ftc.lt",	I (FT),		36,	0,	OP (orrr),		T (regf)},
	{"ftc.le",	I (FT),		37,	0,	OP (orrr),		T (regf)},
	{"ftc.gt",	I (FT),		38,	0,	OP (orrr),		T (regf)},
	{"ftc.ge",	I (FT),		39,	0,	OP (orrr),		T (regf)},

	/* floating point instructions (double precision) */
	{"fo.add",	I (FO),		1,	0,	OP (orrr),		T (regf)},
	{"fo.sub",	I (FO),		2,	0,	OP (orrr),		T (regf)},
	{"fo.mul",	I (FO),		3,	0,	OP (orrr),		T (regf)},
	{"fo.div",	I (FO),		4,	0,	OP (orrr),		T (regf)},

	{"fo.abs",	I (FO),		8,	0,	OP (orr0),		T (regf)},
	{"fo.neg",	I (FO),		9,	0,	OP (orr0),		T (regf)},

	{"fo.f2i",	I (FO),		10,	0,	OP (orr0),		T (regf)},
	{"fo.i2f",	I (FO),		11,	0,	OP (orr0),		T (regf)},
	{"fo.2ft",	I (FO),		12,	0,	OP (orr0),		T (regf)},

	{"foc.un",	I (FO),		32,	0,	OP (orrr),		T (regf)},
	{"foc.or",	I (FO),		33,	0,	OP (orrr),		T (regf)},
	{"foc.eq",	I (FO),		34,	0,	OP (orrr),		T (regf)},
	{"foc.ne",	I (FO),		35,	0,	OP (orrr),		T (regf)},
	{"foc.lt",	I (FO),		36,	0,	OP (orrr),		T (regf)},
	{"foc.le",	I (FO),		37,	0,	OP (orrr),		T (regf)},
	{"foc.gt",	I (FO),		38,	0,	OP (orrr),		T (regf)},
	{"foc.ge",	I (FO),		39,	0,	OP (orrr),		T (regf)},

	/* pseudo instructions */
	{"seto",	~0,		0,	0,	OP (ps_seto),		T (pseudo)},

	{NULL,		~0,		0,	0,	OP (none),		T (normal)}
 };
