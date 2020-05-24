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

/* Register-name-table for special registers.  */
const struct dadao_spec_reg dadao_spec_regs[] =
 {
   /* Keep rJ at top; it's the most frequently used one.  */
   {"rJ", 4},
   {"rA", 21},
   {"rB", 0},
   {"rC", 8},
   {"rD", 1},
   {"rE", 2},
   {"rF", 22},
   {"rG", 19},
   {"rH", 3},
   {"rI", 12},
   {"rK", 15},
   {"rL", 20},
   {"rM", 5},
   {"rN", 9},
   {"rO", 10},
   {"rP", 23},
   {"rQ", 16},
   {"rR", 6},
   {"rS", 11},
   {"rT", 13},
   {"rU", 17},
   {"rV", 18},
   {"rW", 24},
   {"rX", 25},
   {"rY", 26},
   {"rZ", 27},
   {"rBB", 7},
   {"rTT", 14},
   {"rWW", 28},
   {"rXX", 29},
   {"rYY", 30},
   {"rZZ", 31},
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
	{"add",		0x20,		0,	1,	OP (rrii_rrri),		T (normal)},
	{"addu",	0x22,		0,	1,	OP (rrii_rrri),		T (normal)},
	{"sub",		0x24,		0,	1,	OP (rrii_rrri),		T (normal)},
	{"subu",	0x26,		0,	1,	OP (rrii_rrri),		T (normal)},
	{"mul",		0x28,		0,	1,	OP (rrii_rrri),		T (normal)},
	{"mulu",	0x2A,		0,	1,	OP (rrii_rrri),		T (normal)},
	{"div",		0x2C,		0,	1,	OP (rrii_rrri),		T (normal)},
	{"divu",	0x2E,		0,	1,	OP (rrii_rrri),		T (normal)},

	/* Synonym for ADDU.  Put after ADDU, since we don't prefer it for disassembly. */
	{"lda",		0x22,		0,	1,	OP (rrii_rrri_or_sym),	T (normal)},

	{"cmp",		0x30,		0,	1,	OP (rrii_rrri),		T (normal)},
	{"cmpu",	0x32,		0,	1,	OP (rrii_rrri),		T (normal)},
	{"or",		0x34,		0,	1,	OP (rrii_rrri),		T (normal)},
	{"and",		0x36,		0,	1,	OP (rrii_rrri),		T (normal)},
	{"xor",		0x38,		0,	1,	OP (rrii_rrri),		T (normal)},

	{"setl",	I (SETW),	W (L),	0,	OP (rjii),		T (normal)},
	{"setml",	I (SETW),	W (ML),	0,	OP (rjii),		T (normal)},
	{"setmh",	I (SETW),	W (MH),	0,	OP (rjii),		T (normal)},
	{"seth",	I (SETW),	W (H),	0,	OP (rjii),		T (normal)},

	{"incl",	I (INCW),	W (L),	0,	OP (rjii),		T (normal)},
	{"incml",	I (INCW),	W (ML),	0,	OP (rjii),		T (normal)},
	{"incmh",	I (INCW),	W (MH),	0,	OP (rjii),		T (normal)},
	{"inch",	I (INCW),	W (H),	0,	OP (rjii),		T (normal)},

	{"orl",		I (ORW),	W (L),	0,	OP (rjii),		T (normal)},
	{"orml",	I (ORW),	W (ML),	0,	OP (rjii),		T (normal)},
	{"ormh",	I (ORW),	W (MH),	0,	OP (rjii),		T (normal)},
	{"orh",		I (ORW),	W (H),	0,	OP (rjii),		T (normal)},

	{"andnl",	I (ANDNW),	W (L),	0,	OP (rjii),		T (normal)},
	{"andnml",	I (ANDNW),	W (ML),	0,	OP (rjii),		T (normal)},
	{"andnmh",	I (ANDNW),	W (MH),	0,	OP (rjii),		T (normal)},
	{"andnh",	I (ANDNW),	W (H),	0,	OP (rjii),		T (normal)},

	{"slu",		0x3E,		1,	1,	OP (orri_orrr),		T (normal)},
	{"sr",		0x3E,		2,	1,	OP (orri_orrr),		T (normal)},
	{"sru",		0x3E,		3,	1,	OP (orri_orrr),		T (normal)},
	{"not",		0x3E,		4,	1,	OP (orri_orrr),		T (normal)},

	{"csn",		0x40,		0,	1,	OP (rrii_rrri),		T (normal)},
	{"csz",		0x42,		0,	1,	OP (rrii_rrri),		T (normal)},
	{"csp",		0x44,		0,	1,	OP (rrii_rrri),		T (normal)},
	{"csod",	0x46,		0,	1,	OP (rrii_rrri),		T (normal)},
	{"csnn",	0x48,		0,	1,	OP (rrii_rrri),		T (normal)},
	{"csnz",	0x4A,		0,	1,	OP (rrii_rrri),		T (normal)},
	{"csnp",	0x4C,		0,	1,	OP (rrii_rrri),		T (normal)},
	{"csev",	0x4E,		0,	1,	OP (rrii_rrri),		T (normal)},

	{"ldb",		0x80,		0,	1,	OP (rrii_rrri_or_sym),	T (dref)},
	{"ldw",		0x82,		0,	1,	OP (rrii_rrri_or_sym),	T (dref)},
	{"ldt",		0x84,		0,	1,	OP (rrii_rrri_or_sym),	T (dref)},
	{"ldo",		0x86,		0,	1,	OP (rrii_rrri_or_sym),	T (dref)},
	{"ldbu",	0x88,		0,	1,	OP (rrii_rrri_or_sym),	T (dref)},
	{"ldwu",	0x8A,		0,	1,	OP (rrii_rrri_or_sym),	T (dref)},
	{"ldtu",	0x8C,		0,	1,	OP (rrii_rrri_or_sym),	T (dref)},
	{"ldsf",	0x8E,		0,	1,	OP (rrii_rrri_or_sym),	T (dref)},

	{"stb",		0x90,		0,	1,	OP (rrii_rrri_or_sym),	T (dref)},
	{"stw",		0x92,		0,	1,	OP (rrii_rrri_or_sym),	T (dref)},
	{"stt",		0x94,		0,	1,	OP (rrii_rrri_or_sym),	T (dref)},
	{"sto",		0x96,		0,	1,	OP (rrii_rrri_or_sym),	T (dref)},
	{"stsf",	0x9E,		0,	1,	OP (rrii_rrri_or_sym),	T (dref)},

	{"bn",		0xC0,		0,	0,	OP (riii),		T (condbranch)},
	{"bnn",		0xC1,		0,	0,	OP (riii),		T (condbranch)},
	{"bz",		0xC2,		0,	0,	OP (riii),		T (condbranch)},
	{"bnz",		0xC3,		0,	0,	OP (riii),		T (condbranch)},
	{"bp",		0xC4,		0,	0,	OP (riii),		T (condbranch)},
	{"bnp",		0xC5,		0,	0,	OP (riii),		T (condbranch)},
	{"bod",		0xC6,		0,	0,	OP (riii),		T (condbranch)},
	{"bev",		0xC7,		0,	0,	OP (riii),		T (condbranch)},
	{"geta",	I (GETA),	0,	0,	OP (riii),		T (geta)},

	{"call",	I (CALL),	0,	1,	OP (iiii_riii),		T (jsr)},
	{"jump",	I (JUMP),	0,	1,	OP (riii_rrii),		T (branch)},

	{"swym",	0xDA,		0,	0,	OP (oiii),		T (normal)},
	{"ret",		0xDA,		0x01,	0,	OP (o000),		T (jsr)},
	{"trip",	0xDA,		0x04,	0,	OP (oiii),		T (jsr)},
	{"trap",	0xDA,		0x20,	0,	OP (oiii),		T (jsr)},
	{"nop",		0xDA,		0x36,	0,	OP (o000),		T (normal)},

	{"put",		0xDA,		0x37,	0,	OP (orr0_put),		T (normal)},
	{"get",		0xDA,		0x38,	0,	OP (orr0_get),		T (normal)},

	{"fcmp",	I (FP),		1,	0,	OP (orrr),		T (normal)},
	{"fun",		I (FP),		2,	0,	OP (orrr),		T (normal)},
	{"feql",	I (FP),		3,	0,	OP (orrr),		T (normal)},
	{"fadd",	I (FP),		4,	0,	OP (orrr),		T (normal)},
	{"fsub",	I (FP),		6,	0,	OP (orrr),		T (normal)},
	{"fmul",	I (FP),		16,	0,	OP (orrr),		T (normal)},
	{"fcmpe",	I (FP),		17,	0,	OP (orrr),		T (normal)},
	{"fune",	I (FP),		18,	0,	OP (orrr),		T (normal)},
	{"feqle",	I (FP),		19,	0,	OP (orrr),		T (normal)},
	{"fdiv",	I (FP),		20,	0,	OP (orrr),		T (normal)},
	{"frem",	I (FP),		22,	0,	OP (orrr),		T (normal)},

	{"fix",		I (FP),		5,	0,	OP (orri),		T (normal)},
	{"fixu",	I (FP),		7,	0,	OP (orri),		T (normal)},
	{"fsqrt",	I (FP),		21,	0,	OP (orri),		T (normal)},
	{"fint",	I (FP),		23,	0,	OP (orri),		T (normal)},

	{"flot",	I (FP),		8,	0,	OP (orri),		T (normal)},
	{"flotu",	I (FP),		10,	0,	OP (orri),		T (normal)},
	{"sflot",	I (FP),		12,	0,	OP (orri),		T (normal)},
	{"sflotu",	I (FP),		14,	0,	OP (orri),		T (normal)},

	{"seto",	~0,		0,	0,	OP (ps_seto),		T (pseudo)},

	{NULL,		~0,		0,	0,	OP (none),		T (normal)}
 };
