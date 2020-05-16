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
   {"fcmp",	O (1),		0,	OP (regs),		N},
   {"flot",	Z (8),		0,	OP (roundregs_z),	N},

   {"fun",	O (2),		0,	OP (regs),		N},
   {"feql",	O (3),		0,	OP (regs),		N},
   {"flotu",	Z (10),		0,	OP (roundregs_z),	N},

   {"fadd",	O (4),		0,	OP (regs),		N},
   {"fix",	O (5),		0,	OP (roundregs),		N},
   {"sflot",	Z (12),		0,	OP (roundregs_z),	N},

   {"fsub",	O (6),		0,	OP (regs),		N},
   {"fixu",	O (7),		0,	OP (roundregs),		N},
   {"sflotu",	Z (14),		0,	OP (roundregs_z),	N},

   {"fmul",	O (16),		0,	OP (regs),		N},
   {"fcmpe",	O (17),		0,	OP (regs),		N},

   {"fune",	O (18),		0,	OP (regs),		N},
   {"feqle",	O (19),		0,	OP (regs),		N},

   {"fdiv",	O (20),		0,	OP (regs),		N},
   {"fsqrt",	O (21),		0,	OP (roundregs),		N},

   {"frem",	O (22),		0,	OP (regs),		N},
   {"fint",	O (23),		0,	OP (roundregs),		N},

	{"add",		Z (0x20),	0,	OP (fdfa_reg_fbc_rs6_i12),	D_A_BC},
	{"addu",	Z (0x22),	0,	OP (fdfa_reg_fbc_rs6_i12),	D_A_BC},
	{"sub",		Z (0x24),	0,	OP (fdfa_reg_fbc_rs6_i12),	D_A_BC},
	{"subu",	Z (0x26),	0,	OP (fdfa_reg_fbc_rs6_i12),	D_A_BC},
	{"mul",		Z (0x28),	0,	OP (fdfa_reg_fbc_rs6_i12),	D_A_BC},
	{"mulu",	Z (0x2A),	0,	OP (fdfa_reg_fbc_rs6_i12),	D_A_BC},
	{"div",		Z (0x2C),	0,	OP (fdfa_reg_fbc_rs6_i12),	D_A_BC},
	{"divu",	Z (0x2E),	0,	OP (fdfa_reg_fbc_rs6_i12),	D_A_BC},

	{"cmp",		Z (0x30),	0,	OP (fdfa_reg_fbc_rs6_i12),	D_A_BC},
	{"cmpu",	Z (0x32),	0,	OP (fdfa_reg_fbc_rs6_i12),	D_A_BC},

	/* Synonym for ADDU.  Put after ADDU, since we don't prefer it for disassembly. */
	{"lda",		Z (0x22),	0,	OP (fdfa_reg_fbc_rs6_i12_or_sym),	D_A_BC},

	{"slu",		Z (0x3E),	1,	OP (fa_op_fdfb_reg_fc_i6),	D_B_C},
	{"sr",		Z (0x3E),	2,	OP (fa_op_fdfb_reg_fc_i6),	D_B_C},
	{"sru",		Z (0x3E),	3,	OP (fa_op_fdfb_reg_fc_i6),	D_B_C},
	{"not",		Z (0x3E),	4,	OP (fa_op_fdfb_reg_fc_i6),	D_B_C},

	{"bn",		Z (0x40),	0,	OP (fa_reg_fbcd_i18),		COND_BR},
	{"bnn",		Z (0x48),	0,	OP (fa_reg_fbcd_i18),		COND_BR},
	{"bz",		Z (0x42),	0,	OP (fa_reg_fbcd_i18),		COND_BR},
	{"bnz",		Z (0x4a),	0,	OP (fa_reg_fbcd_i18),		COND_BR},
	{"bp",		Z (0x44),	0,	OP (fa_reg_fbcd_i18),		COND_BR},
	{"bnp",		Z (0x4c),	0,	OP (fa_reg_fbcd_i18),		COND_BR},
	{"bod",		Z (0x46),	0,	OP (fa_reg_fbcd_i18),		COND_BR},
	{"bev",		Z (0x4e),	0,	OP (fa_reg_fbcd_i18),		COND_BR},

	{"csn",		Z (0x60),	0,	OP (fdfa_reg_fbc_rs6_i12),	D_A_BC},
	{"csnn",	Z (0x68),	0,	OP (fdfa_reg_fbc_rs6_i12),	D_A_BC},

	{"csz",		Z (0x62),	0,	OP (fdfa_reg_fbc_rs6_i12),	D_A_BC},
	{"csnz",	Z (0x6a),	0,	OP (fdfa_reg_fbc_rs6_i12),	D_A_BC},

	{"csp",		Z (0x64),	0,	OP (fdfa_reg_fbc_rs6_i12),	D_A_BC},
	{"csnp",	Z (0x6c),	0,	OP (fdfa_reg_fbc_rs6_i12),	D_A_BC},

	{"csod",	Z (0x66),	0,	OP (fdfa_reg_fbc_rs6_i12),	D_A_BC},
	{"csev",	Z (0x6e),	0,	OP (fdfa_reg_fbc_rs6_i12),	D_A_BC},

	{"ldb",		Z (0x80),	0,	OP (fdfa_reg_fbc_rs6_i12_or_sym),	MB},
	{"ldbu",	Z (0x82),	0,	OP (fdfa_reg_fbc_rs6_i12_or_sym),	MB},
	{"ldw",		Z (0x84),	0,	OP (fdfa_reg_fbc_rs6_i12_or_sym),	MW},
	{"ldwu",	Z (0x86),	0,	OP (fdfa_reg_fbc_rs6_i12_or_sym),	MW},
	{"ldt",		Z (0x88),	0,	OP (fdfa_reg_fbc_rs6_i12_or_sym),	MT},
	{"ldtu",	Z (0x8A),	0,	OP (fdfa_reg_fbc_rs6_i12_or_sym),	MT},
	{"ldo",		Z (0x8C),	0,	OP (fdfa_reg_fbc_rs6_i12_or_sym),	MO},

	{"stb",		Z (0xA0),	0,	OP (fdfa_reg_fbc_rs6_i12_or_sym),	MB},
	{"stw",		Z (0xA2),	0,	OP (fdfa_reg_fbc_rs6_i12_or_sym),	MW},
	{"stt",		Z (0xA4),	0,	OP (fdfa_reg_fbc_rs6_i12_or_sym),	MT},
	{"sto",		Z (0xA6),	0,	OP (fdfa_reg_fbc_rs6_i12_or_sym),	MO},

   {"ldsf",	Z (0x90),	0,	OP (regs_z_opt),	MT},

   /* This doesn't seem to access memory, just the TLB.  */
   {"ldvts",	Z (0x98),	0,	OP (regs_z_opt),	M},

   {"cswap",	Z (0x94),	0,	OP (regs_z_opt),	MO},

   {"go",	Z (GO_INSN_BYTE),
				0,	OP (regs_z_opt),	B},

   {"stsf",	Z (0xb0),	0,	OP (regs_z_opt),	MT},
   {"syncd",	Z (0xb8),	0,	OP (x_regs_z),		M},

   {"stco",	Z (0xb4),	0,	OP (x_regs_z),		MO},
   {"syncid",	Z (0xbc),	0,	OP (x_regs_z),		M},

   {"pushgo",	Z (PUSHGO_INSN_BYTE),
				0,	OP (pushgo),		J},

   /* Synonym for OR with a zero Z.  */
   {"set",	O (0xc1)
		  | 0xff,	0,	OP (set),		N},

	{"or",		Z (0xC0),	0,	OP (fdfa_reg_fbc_rs6_i12),	D_A_BC},
	{"and",		Z (0xC2),	0,	OP (fdfa_reg_fbc_rs6_i12),	D_A_BC},
	{"xor",		Z (0xC4),	0,	OP (fdfa_reg_fbc_rs6_i12),	D_A_BC},

	{"swym",	O (0xDA),	0,	OP (fa_op_fbcd_i18),		N},
	{"trip",	O (0xDA),	0x04,	OP (fa_op_fbcd_i18),		N},
	{"trap",	O (0xDA),	0x20,	OP (fa_op_fbcd_i18),		N},
	{"nop",		O (0xDA),	0x36,	OP (none),			N},

   {"seth",	O (0xe0),	0,	OP (reg_yz),		N},
   {"setmh",	O (0xe1),	0,	OP (reg_yz),		N},
   {"orh",	O (0xe8),	0,	OP (reg_yz),		N},
   {"ormh",	O (0xe9),	0,	OP (reg_yz),		N},

   {"setml",	O (0xe2),	0,	OP (reg_yz),		N},
   {"setl",	O (SETL_INSN_BYTE),
				0,	OP (reg_yz),		N},
   {"orml",	O (0xea),	0,	OP (reg_yz),		N},
   {"orl",	O (0xeb),	0,	OP (reg_yz),		N},

   {"inch",	O (INCH_INSN_BYTE),
				0,	OP (reg_yz),		N},
   {"incmh",	O (INCMH_INSN_BYTE),
				0,	OP (reg_yz),		N},
   {"andnh",	O (0xec),	0,	OP (reg_yz),		N},
   {"andnmh",	O (0xed),	0,	OP (reg_yz),		N},

   {"incml",	O (INCML_INSN_BYTE),
				0,	OP (reg_yz),		N},
   {"incl",	O (0xe7),	0,	OP (reg_yz),		N},
   {"andnml",	O (0xee),	0,	OP (reg_yz),		N},
   {"andnl",	O (0xef),	0,	OP (reg_yz),		N},

	{"jmp",		Z (0xf0),	0,	OP (fa_reg_fbcd_i18),	dadao_type_jmp},
   {"pop",	O (0xf8),	0,	OP (pop),		B},

   {"pushj",	Z (0xf2),	0,	OP (pushj),		J},

	{"geta",	Z (0xf4),	0,	OP (fa_reg_fbcd_i18),	dadao_type_geta},

   {"sync",	O (0xfc),	0,	OP (sync),		N},

   {"put", Z (0xf6) | 0xff00,	0,	OP (put),		N},
   {"get", O (0xfe) | 0xffe0,	0,	OP (get),		N},


	{NULL,	~0,	~0,	0,	OP (none),		N}
 };
