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

/* All bits in the opcode-byte are significant.  Add "| ..." expressions
   to add zero-bits.  */
#undef O
#define O(m) ((unsigned long) (m) << 24UL), ((~(unsigned long) (m) & 255) << 24)

/* Bits 7..1 of the opcode are significant.  */
#undef Z
#define Z(m) ((unsigned long) (m) << 24), ((~(unsigned long) (m) & 254) << 24)

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

#define OP(y) XCONCAT2 (dadao_operands_,y)

/* Groups of instructions specified here must, if all are matching the
   same instruction, be consecutive, in order more-specific to
   less-specific match.  */

const struct dadao_opcode dadao_opcodes[] =
 {
	{"add",		Z (0x20),	0,	OP (fdfa_reg_fbc_rs6_i12),	D_A_BC},
	{"addu",	Z (0x22),	0,	OP (fdfa_reg_fbc_rs6_i12),	D_A_BC},
	{"sub",		Z (0x24),	0,	OP (fdfa_reg_fbc_rs6_i12),	D_A_BC},
	{"subu",	Z (0x26),	0,	OP (fdfa_reg_fbc_rs6_i12),	D_A_BC},
	{"mul",		Z (0x28),	0,	OP (fdfa_reg_fbc_rs6_i12),	D_A_BC},
	{"mulu",	Z (0x2A),	0,	OP (fdfa_reg_fbc_rs6_i12),	D_A_BC},
	{"div",		Z (0x2C),	0,	OP (fdfa_reg_fbc_rs6_i12),	D_A_BC},
	{"divu",	Z (0x2E),	0,	OP (fdfa_reg_fbc_rs6_i12),	D_A_BC},

	/* Synonym for ADDU.  Put after ADDU, since we don't prefer it for disassembly. */
	{"lda",		Z (0x22),	0,	OP (fdfa_reg_fbc_rs6_i12_or_sym),	D_A_BC},

	{"cmp",		Z (0x30),	0,	OP (fdfa_reg_fbc_rs6_i12),	D_A_BC},
	{"cmpu",	Z (0x32),	0,	OP (fdfa_reg_fbc_rs6_i12),	D_A_BC},
	{"or",		Z (0x34),	0,	OP (fdfa_reg_fbc_rs6_i12),	D_A_BC},
	{"and",		Z (0x36),	0,	OP (fdfa_reg_fbc_rs6_i12),	D_A_BC},
	{"xor",		Z (0x38),	0,	OP (fdfa_reg_fbc_rs6_i12),	D_A_BC},

	{"setl",	O (DADAO_INSN_SETW),	DADAO_WYDE_L,	OP (fd_reg_fabc_i16),		N},
	{"setml",	O (DADAO_INSN_SETW),	DADAO_WYDE_ML,	OP (fd_reg_fabc_i16),		N},
	{"setmh",	O (DADAO_INSN_SETW),	DADAO_WYDE_MH,	OP (fd_reg_fabc_i16),		N},
	{"seth",	O (DADAO_INSN_SETW),	DADAO_WYDE_H,	OP (fd_reg_fabc_i16),		N},

	{"incl",	O (DADAO_INSN_INCW),	DADAO_WYDE_L,	OP (fd_reg_fabc_i16),		N},
	{"incml",	O (DADAO_INSN_INCW),	DADAO_WYDE_ML,	OP (fd_reg_fabc_i16),		N},
	{"incmh",	O (DADAO_INSN_INCW),	DADAO_WYDE_MH,	OP (fd_reg_fabc_i16),		N},
	{"inch",	O (DADAO_INSN_INCW),	DADAO_WYDE_H,	OP (fd_reg_fabc_i16),		N},

	{"orl",		O (DADAO_INSN_ORW),	DADAO_WYDE_L,	OP (fd_reg_fabc_i16),		N},
	{"orml",	O (DADAO_INSN_ORW),	DADAO_WYDE_ML,	OP (fd_reg_fabc_i16),		N},
	{"ormh",	O (DADAO_INSN_ORW),	DADAO_WYDE_MH,	OP (fd_reg_fabc_i16),		N},
	{"orh",		O (DADAO_INSN_ORW),	DADAO_WYDE_H,	OP (fd_reg_fabc_i16),		N},

	{"andnl",	O (DADAO_INSN_ANDNW),	DADAO_WYDE_L,	OP (fd_reg_fabc_i16),		N},
	{"andnml",	O (DADAO_INSN_ANDNW),	DADAO_WYDE_ML,	OP (fd_reg_fabc_i16),		N},
	{"andnmh",	O (DADAO_INSN_ANDNW),	DADAO_WYDE_MH,	OP (fd_reg_fabc_i16),		N},
	{"andnh",	O (DADAO_INSN_ANDNW),	DADAO_WYDE_H,	OP (fd_reg_fabc_i16),		N},

	{"slu",		Z (0x3E),	1,	OP (fa_op_fdfb_reg_fc_reg_i6),	D_B_C},
	{"sr",		Z (0x3E),	2,	OP (fa_op_fdfb_reg_fc_reg_i6),	D_B_C},
	{"sru",		Z (0x3E),	3,	OP (fa_op_fdfb_reg_fc_reg_i6),	D_B_C},
	{"not",		Z (0x3E),	4,	OP (fa_op_fdfb_reg_fc_reg_i6),	D_B_C},

	{"bn",		Z (0x40),	0,	OP (fa_reg_fbcd_i18),		COND_BR},
	{"bz",		Z (0x42),	0,	OP (fa_reg_fbcd_i18),		COND_BR},
	{"bp",		Z (0x44),	0,	OP (fa_reg_fbcd_i18),		COND_BR},
	{"bod",		Z (0x46),	0,	OP (fa_reg_fbcd_i18),		COND_BR},
	{"bnn",		Z (0x48),	0,	OP (fa_reg_fbcd_i18),		COND_BR},
	{"bnz",		Z (0x4a),	0,	OP (fa_reg_fbcd_i18),		COND_BR},
	{"bnp",		Z (0x4c),	0,	OP (fa_reg_fbcd_i18),		COND_BR},
	{"bev",		Z (0x4e),	0,	OP (fa_reg_fbcd_i18),		COND_BR},

	{"csn",		Z (0x60),	0,	OP (fdfa_reg_fbc_rs6_i12),	D_A_BC},
	{"csz",		Z (0x62),	0,	OP (fdfa_reg_fbc_rs6_i12),	D_A_BC},
	{"csp",		Z (0x64),	0,	OP (fdfa_reg_fbc_rs6_i12),	D_A_BC},
	{"csod",	Z (0x66),	0,	OP (fdfa_reg_fbc_rs6_i12),	D_A_BC},
	{"csnn",	Z (0x68),	0,	OP (fdfa_reg_fbc_rs6_i12),	D_A_BC},
	{"csnz",	Z (0x6a),	0,	OP (fdfa_reg_fbc_rs6_i12),	D_A_BC},
	{"csnp",	Z (0x6c),	0,	OP (fdfa_reg_fbc_rs6_i12),	D_A_BC},
	{"csev",	Z (0x6e),	0,	OP (fdfa_reg_fbc_rs6_i12),	D_A_BC},

	{"ldb",		Z (0x80),	0,	OP (fdfa_reg_fbc_rs6_i12_or_sym),	MB},
	{"ldbu",	Z (0x82),	0,	OP (fdfa_reg_fbc_rs6_i12_or_sym),	MB},
	{"ldw",		Z (0x84),	0,	OP (fdfa_reg_fbc_rs6_i12_or_sym),	MW},
	{"ldwu",	Z (0x86),	0,	OP (fdfa_reg_fbc_rs6_i12_or_sym),	MW},
	{"ldt",		Z (0x88),	0,	OP (fdfa_reg_fbc_rs6_i12_or_sym),	MT},
	{"ldtu",	Z (0x8A),	0,	OP (fdfa_reg_fbc_rs6_i12_or_sym),	MT},
	{"ldo",		Z (0x8C),	0,	OP (fdfa_reg_fbc_rs6_i12_or_sym),	MO},
	{"ldsf",	Z (0x8E),	0,	OP (fdfa_reg_fbc_rs6_i12_or_sym),	MT},

	{"stb",		Z (0xA0),	0,	OP (fdfa_reg_fbc_rs6_i12_or_sym),	MB},
	{"stw",		Z (0xA2),	0,	OP (fdfa_reg_fbc_rs6_i12_or_sym),	MW},
	{"stt",		Z (0xA4),	0,	OP (fdfa_reg_fbc_rs6_i12_or_sym),	MT},
	{"sto",		Z (0xA6),	0,	OP (fdfa_reg_fbc_rs6_i12_or_sym),	MO},
	{"stsf",	Z (0xAE),	0,	OP (fdfa_reg_fbc_rs6_i12_or_sym),	MT},

	{"swym",	O (0xDA),	0,	OP (fa_op_fbcd_i18),		N},
	{"ret",		O (0xDA),	0x01,	OP (none),			B},
	{"trip",	O (0xDA),	0x04,	OP (fa_op_fbcd_i18),		N},
	{"trap",	O (0xDA),	0x20,	OP (fa_op_fbcd_i18),		N},
	{"nop",		O (0xDA),	0x36,	OP (none),			N},

	{"put",		O (0xDA),	0x37,	OP (fa_op_fdfb_reg_fc_0_put),		N},
	{"get",		O (0xDA),	0x38,	OP (fa_op_fdfb_reg_fc_0_get),		N},

	{"fcmp",	O (DADAO_INSN_FP),	1,	OP (fa_op_fbcd_reg),		N},
	{"fun",		O (DADAO_INSN_FP),	2,	OP (fa_op_fbcd_reg),		N},
	{"feql",	O (DADAO_INSN_FP),	3,	OP (fa_op_fbcd_reg),		N},
	{"fadd",	O (DADAO_INSN_FP),	4,	OP (fa_op_fbcd_reg),		N},
	{"fsub",	O (DADAO_INSN_FP),	6,	OP (fa_op_fbcd_reg),		N},
	{"fmul",	O (DADAO_INSN_FP),	16,	OP (fa_op_fbcd_reg),		N},
	{"fcmpe",	O (DADAO_INSN_FP),	17,	OP (fa_op_fbcd_reg),		N},
	{"fune",	O (DADAO_INSN_FP),	18,	OP (fa_op_fbcd_reg),		N},
	{"feqle",	O (DADAO_INSN_FP),	19,	OP (fa_op_fbcd_reg),		N},
	{"fdiv",	O (DADAO_INSN_FP),	20,	OP (fa_op_fbcd_reg),		N},
	{"frem",	O (DADAO_INSN_FP),	22,	OP (fa_op_fbcd_reg),		N},

	{"fix",		O (DADAO_INSN_FP),	5,	OP (fa_op_fdfb_reg_fc_i6),	N},
	{"fixu",	O (DADAO_INSN_FP),	7,	OP (fa_op_fdfb_reg_fc_i6),	N},
	{"fsqrt",	O (DADAO_INSN_FP),	21,	OP (fa_op_fdfb_reg_fc_i6),	N},
	{"fint",	O (DADAO_INSN_FP),	23,	OP (fa_op_fdfb_reg_fc_i6),	N},

	{"flot",	O (DADAO_INSN_FP),	8,	OP (fa_op_fdfb_reg_fc_i6),	N},
	{"flotu",	O (DADAO_INSN_FP),	10,	OP (fa_op_fdfb_reg_fc_i6),	N},
	{"sflot",	O (DADAO_INSN_FP),	12,	OP (fa_op_fdfb_reg_fc_i6),	N},
	{"sflotu",	O (DADAO_INSN_FP),	14,	OP (fa_op_fdfb_reg_fc_i6),	N},

	{"jmp",		Z (0xf0),	0,	OP (fa_reg_fbcd_i18),	dadao_type_jmp},
	{"call",	Z (DADAO_INSN_CALL),	0,	OP (fa_reg_fbcd_i18_ri12),	J},
	{"geta",	Z (0xf4),	0,	OP (fa_reg_fbcd_i18),	dadao_type_geta},

	{NULL,	~0,	~0,	0,	OP (none),		N}
 };
