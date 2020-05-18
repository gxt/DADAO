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
#define N dadao_type_normal
#define B dadao_type_branch
#define MB dadao_type_memaccess_byte
#define MW dadao_type_memaccess_wyde
#define MT dadao_type_memaccess_tetra
#define MO dadao_type_memaccess_octa
#define M dadao_type_memaccess_block
#define J dadao_type_jsr
#define COND_BR	dadao_type_condbranch
#define D_A_BC	dadao_type_fd_eq_fa_op_bc
#define D_B_C	dadao_type_fd_eq_fb_op_fc

#define I(i)	XCONCAT2 (DADAO_INSN_,i)
#define W(w)	XCONCAT2 (DADAO_WYDE_,w)
#define OP(y)	XCONCAT2 (dadao_operands_,y)

/* Groups of instructions specified here must, if all are matching the
   same instruction, be consecutive, in order more-specific to
   less-specific match.  */

const struct dadao_opcode dadao_opcodes[] =
 {
	{"add",		0x20,		0,	1,	OP (riir_rrir),		D_A_BC},
	{"addu",	0x22,		0,	1,	OP (riir_rrir),		D_A_BC},
	{"sub",		0x24,		0,	1,	OP (riir_rrir),		D_A_BC},
	{"subu",	0x26,		0,	1,	OP (riir_rrir),		D_A_BC},
	{"mul",		0x28,		0,	1,	OP (riir_rrir),		D_A_BC},
	{"mulu",	0x2A,		0,	1,	OP (riir_rrir),		D_A_BC},
	{"div",		0x2C,		0,	1,	OP (riir_rrir),		D_A_BC},
	{"divu",	0x2E,		0,	1,	OP (riir_rrir),		D_A_BC},

	/* Synonym for ADDU.  Put after ADDU, since we don't prefer it for disassembly. */
	{"lda",		0x22,		0,	1,	OP (riir_rrir_or_sym),	D_A_BC},

	{"cmp",		0x30,		0,	1,	OP (riir_rrir),		D_A_BC},
	{"cmpu",	0x32,		0,	1,	OP (riir_rrir),		D_A_BC},
	{"or",		0x34,		0,	1,	OP (riir_rrir),		D_A_BC},
	{"and",		0x36,		0,	1,	OP (riir_rrir),		D_A_BC},
	{"xor",		0x38,		0,	1,	OP (riir_rrir),		D_A_BC},

	{"setl",	I (SETW),	W (L),	0,	OP (iijr),		N},
	{"setml",	I (SETW),	W (ML),	0,	OP (iijr),		N},
	{"setmh",	I (SETW),	W (MH),	0,	OP (iijr),		N},
	{"seth",	I (SETW),	W (H),	0,	OP (iijr),		N},

	{"incl",	I (INCW),	W (L),	0,	OP (iijr),		N},
	{"incml",	I (INCW),	W (ML),	0,	OP (iijr),		N},
	{"incmh",	I (INCW),	W (MH),	0,	OP (iijr),		N},
	{"inch",	I (INCW),	W (H),	0,	OP (iijr),		N},

	{"orl",		I (ORW),	W (L),	0,	OP (iijr),		N},
	{"orml",	I (ORW),	W (ML),	0,	OP (iijr),		N},
	{"ormh",	I (ORW),	W (MH),	0,	OP (iijr),		N},
	{"orh",		I (ORW),	W (H),	0,	OP (iijr),		N},

	{"andnl",	I (ANDNW),	W (L),	0,	OP (iijr),		N},
	{"andnml",	I (ANDNW),	W (ML),	0,	OP (iijr),		N},
	{"andnmh",	I (ANDNW),	W (MH),	0,	OP (iijr),		N},
	{"andnh",	I (ANDNW),	W (H),	0,	OP (iijr),		N},

	{"slu",		0x3E,		1,	1,	OP (orir_orrr),		D_B_C},
	{"sr",		0x3E,		2,	1,	OP (orir_orrr),		D_B_C},
	{"sru",		0x3E,		3,	1,	OP (orir_orrr),		D_B_C},
	{"not",		0x3E,		4,	1,	OP (orir_orrr),		D_B_C},

	{"csn",		0x40,		0,	1,	OP (riir_rrir),		D_A_BC},
	{"csz",		0x42,		0,	1,	OP (riir_rrir),		D_A_BC},
	{"csp",		0x44,		0,	1,	OP (riir_rrir),		D_A_BC},
	{"csod",	0x46,		0,	1,	OP (riir_rrir),		D_A_BC},
	{"csnn",	0x48,		0,	1,	OP (riir_rrir),		D_A_BC},
	{"csnz",	0x4A,		0,	1,	OP (riir_rrir),		D_A_BC},
	{"csnp",	0x4C,		0,	1,	OP (riir_rrir),		D_A_BC},
	{"csev",	0x4E,		0,	1,	OP (riir_rrir),		D_A_BC},

	{"ldb",		0x80,		0,	1,	OP (riir_rrir_or_sym),	MB},
	{"ldbu",	0x82,		0,	1,	OP (riir_rrir_or_sym),	MB},
	{"ldw",		0x84,		0,	1,	OP (riir_rrir_or_sym),	MW},
	{"ldwu",	0x86,		0,	1,	OP (riir_rrir_or_sym),	MW},
	{"ldt",		0x88,		0,	1,	OP (riir_rrir_or_sym),	MT},
	{"ldtu",	0x8A,		0,	1,	OP (riir_rrir_or_sym),	MT},
	{"ldo",		0x8C,		0,	1,	OP (riir_rrir_or_sym),	MO},
	{"ldsf",	0x8E,		0,	1,	OP (riir_rrir_or_sym),	MT},

	{"stb",		0x90,		0,	1,	OP (riir_rrir_or_sym),	MB},
	{"stw",		0x92,		0,	1,	OP (riir_rrir_or_sym),	MW},
	{"stt",		0x94,		0,	1,	OP (riir_rrir_or_sym),	MT},
	{"sto",		0x96,		0,	1,	OP (riir_rrir_or_sym),	MO},
	{"stsf",	0x9E,		0,	1,	OP (riir_rrir_or_sym),	MT},

	{"bn",		0xC0,		0,	1,	OP (riii),		COND_BR},
	{"bz",		0xC2,		0,	1,	OP (riii),		COND_BR},
	{"bp",		0xC4,		0,	1,	OP (riii),		COND_BR},
	{"bod",		0xC6,		0,	1,	OP (riii),		COND_BR},
	{"bnn",		0xC8,		0,	1,	OP (riii),		COND_BR},
	{"bnz",		0xCA,		0,	1,	OP (riii),		COND_BR},
	{"bnp",		0xCC,		0,	1,	OP (riii),		COND_BR},
	{"bev",		0xCE,		0,	1,	OP (riii),		COND_BR},

	{"jmp",		I (JUMP),	0,	1,	OP (riii),		dadao_type_jmp},
	{"call",	I (CALL),	0,	1,	OP (riii_rrii),		J},
	{"geta",	0xD4,		0,	1,	OP (riii),		dadao_type_geta},

	{"swym",	0xDA,		0,	0,	OP (oiii),		N},
	{"ret",		0xDA,		0x01,	0,	OP (o000),		B},
	{"trip",	0xDA,		0x04,	0,	OP (oiii),		N},
	{"trap",	0xDA,		0x20,	0,	OP (oiii),		N},
	{"nop",		0xDA,		0x36,	0,	OP (o000),		N},

	{"put",		0xDA,		0x37,	0,	OP (or0r_put),		N},
	{"get",		0xDA,		0x38,	0,	OP (or0r_get),		N},

	{"fcmp",	I (FP),		1,	0,	OP (orrr),		N},
	{"fun",		I (FP),		2,	0,	OP (orrr),		N},
	{"feql",	I (FP),		3,	0,	OP (orrr),		N},
	{"fadd",	I (FP),		4,	0,	OP (orrr),		N},
	{"fsub",	I (FP),		6,	0,	OP (orrr),		N},
	{"fmul",	I (FP),		16,	0,	OP (orrr),		N},
	{"fcmpe",	I (FP),		17,	0,	OP (orrr),		N},
	{"fune",	I (FP),		18,	0,	OP (orrr),		N},
	{"feqle",	I (FP),		19,	0,	OP (orrr),		N},
	{"fdiv",	I (FP),		20,	0,	OP (orrr),		N},
	{"frem",	I (FP),		22,	0,	OP (orrr),		N},

	{"fix",		I (FP),		5,	0,	OP (orir),		N},
	{"fixu",	I (FP),		7,	0,	OP (orir),		N},
	{"fsqrt",	I (FP),		21,	0,	OP (orir),		N},
	{"fint",	I (FP),		23,	0,	OP (orir),		N},

	{"flot",	I (FP),		8,	0,	OP (orir),		N},
	{"flotu",	I (FP),		10,	0,	OP (orir),		N},
	{"sflot",	I (FP),		12,	0,	OP (orir),		N},
	{"sflotu",	I (FP),		14,	0,	OP (orir),		N},

	{NULL,		~0,		0,	0,	OP (o000),		N}
 };
