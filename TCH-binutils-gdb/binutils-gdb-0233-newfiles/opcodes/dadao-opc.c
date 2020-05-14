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
#define C dadao_type_condbranch
#define MB dadao_type_memaccess_byte
#define MW dadao_type_memaccess_wyde
#define MT dadao_type_memaccess_tetra
#define MO dadao_type_memaccess_octa
#define M dadao_type_memaccess_block
#define J dadao_type_jsr
#define D_A_BC	dadao_type_fd_eq_fa_op_bc
#define D_B_C	dadao_type_fd_eq_fb_op_fc

#define OP(y) XCONCAT2 (dadao_operands_,y)

/* Groups of instructions specified here must, if all are matching the
   same instruction, be consecutive, in order more-specific to
   less-specific match.  */

const struct dadao_opcode dadao_opcodes[] =
 {
   {"trap",	O (0),		0,	OP (xyz_opt),		J},
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

	{"add",		Z (0x20),	0,	OP (rrs6_ri12),		D_A_BC},
	{"addu",	Z (0x22),	0,	OP (rrs6_ri12),		D_A_BC},
	{"sub",		Z (0x24),	0,	OP (rrs6_ri12),		D_A_BC},
	{"subu",	Z (0x26),	0,	OP (rrs6_ri12),		D_A_BC},
	{"mul",		Z (0x28),	0,	OP (rrs6_ri12),		D_A_BC},
	{"mulu",	Z (0x2A),	0,	OP (rrs6_ri12),		D_A_BC},
	{"div",		Z (0x2C),	0,	OP (rrs6_ri12),		D_A_BC},
	{"divu",	Z (0x2E),	0,	OP (rrs6_ri12),		D_A_BC},

	{"cmp",		Z (0x30),	0,	OP (rrs6_ri12),		D_A_BC},
	{"cmpu",	Z (0x32),	0,	OP (rrs6_ri12),		D_A_BC},

   /* Synonym for ADDU.  Put after ADDU, since we don't prefer it for
      disassembly.  It's supposed to be used for addresses, so we make it
      a memory block reference for purposes of assembly.  */
   {"lda",	Z (0x22),	0,	OP (regs_z_opt),	M},

	{"slu",		Z (0x3E),	1,	OP (rr_ri6),		D_B_C},
	{"sr",		Z (0x3E),	2,	OP (rr_ri6),		D_B_C},
	{"sru",		Z (0x3E),	3,	OP (rr_ri6),		D_B_C},
	{"not",		Z (0x3E),	4,	OP (rr_ri6),		D_B_C},

   {"bn",	Z (0x40),	0,	OP (regaddr),		C},
   {"bnn",	Z (0x48),	0,	OP (regaddr),		C},

   {"bz",	Z (0x42),	0,	OP (regaddr),		C},
   {"bnz",	Z (0x4a),	0,	OP (regaddr),		C},

   {"bp",	Z (0x44),	0,	OP (regaddr),		C},
   {"bnp",	Z (0x4c),	0,	OP (regaddr),		C},

   {"bod",	Z (0x46),	0,	OP (regaddr),		C},
   {"bev",	Z (0x4e),	0,	OP (regaddr),		C},

   {"pbn",	Z (0x50),	0,	OP (regaddr),		C},
   {"pbnn",	Z (0x58),	0,	OP (regaddr),		C},

   {"pbz",	Z (0x52),	0,	OP (regaddr),		C},
   {"pbnz",	Z (0x5a),	0,	OP (regaddr),		C},

   {"pbp",	Z (0x54),	0,	OP (regaddr),		C},
   {"pbnp",	Z (0x5c),	0,	OP (regaddr),		C},

   {"pbod",	Z (0x56),	0,	OP (regaddr),		C},
   {"pbev",	Z (0x5e),	0,	OP (regaddr),		C},

	{"csn",		Z (0x60),	0,	OP (rrs6_ri12),		D_A_BC},
	{"csnn",	Z (0x68),	0,	OP (rrs6_ri12),		D_A_BC},

	{"csz",		Z (0x62),	0,	OP (rrs6_ri12),		D_A_BC},
	{"csnz",	Z (0x6a),	0,	OP (rrs6_ri12),		D_A_BC},

	{"csp",		Z (0x64),	0,	OP (rrs6_ri12),		D_A_BC},
	{"csnp",	Z (0x6c),	0,	OP (rrs6_ri12),		D_A_BC},

	{"csod",	Z (0x66),	0,	OP (rrs6_ri12),		D_A_BC},
	{"csev",	Z (0x6e),	0,	OP (rrs6_ri12),		D_A_BC},

   {"ldb",	Z (0x80),	0,	OP (regs_z_opt),	MB},
   {"ldt",	Z (0x88),	0,	OP (regs_z_opt),	MT},

   {"ldbu",	Z (0x82),	0,	OP (regs_z_opt),	MB},
   {"ldtu",	Z (0x8a),	0,	OP (regs_z_opt),	MT},

   {"ldw",	Z (0x84),	0,	OP (regs_z_opt),	MW},
   {"ldo",	Z (0x8c),	0,	OP (regs_z_opt),	MO},

   {"ldwu",	Z (0x86),	0,	OP (regs_z_opt),	MW},
   {"ldou",	Z (0x8e),	0,	OP (regs_z_opt),	MO},

   {"ldsf",	Z (0x90),	0,	OP (regs_z_opt),	MT},

   /* This doesn't seem to access memory, just the TLB.  */
   {"ldvts",	Z (0x98),	0,	OP (regs_z_opt),	M},

   {"ldht",	Z (0x92),	0,	OP (regs_z_opt),	MT},

   /* Neither does this per-se.  */
   {"preld",	Z (0x9a),	0,	OP (x_regs_z),		N},

   {"cswap",	Z (0x94),	0,	OP (regs_z_opt),	MO},
   {"prego",	Z (0x9c),	0,	OP (x_regs_z),		N},

   {"go",	Z (GO_INSN_BYTE),
				0,	OP (regs_z_opt),	B},

   {"stb",	Z (0xa0),	0,	OP (regs_z_opt),	MB},
   {"stt",	Z (0xa8),	0,	OP (regs_z_opt),	MT},

   {"stbu",	Z (0xa2),	0,	OP (regs_z_opt),	MB},
   {"sttu",	Z (0xaa),	0,	OP (regs_z_opt),	MT},

   {"stw",	Z (0xa4),	0,	OP (regs_z_opt),	MW},
   {"sto",	Z (0xac),	0,	OP (regs_z_opt),	MO},

   {"stwu",	Z (0xa6),	0,	OP (regs_z_opt),	MW},
   {"stou",	Z (0xae),	0,	OP (regs_z_opt),	MO},

   {"stsf",	Z (0xb0),	0,	OP (regs_z_opt),	MT},
   {"syncd",	Z (0xb8),	0,	OP (x_regs_z),		M},

   {"stht",	Z (0xb2),	0,	OP (regs_z_opt),	MT},
   {"prest",	Z (0xba),	0,	OP (x_regs_z),		M},

   {"stco",	Z (0xb4),	0,	OP (x_regs_z),		MO},
   {"syncid",	Z (0xbc),	0,	OP (x_regs_z),		M},

   {"pushgo",	Z (PUSHGO_INSN_BYTE),
				0,	OP (pushgo),		J},

   /* Synonym for OR with a zero Z.  */
   {"set",	O (0xc1)
		  | 0xff,	0,	OP (set),		N},

	{"or",		Z (0xC0),	0,	OP (rrs6_ri12),		D_A_BC},
	{"and",		Z (0xC2),	0,	OP (rrs6_ri12),		D_A_BC},
	{"xor",		Z (0xC4),	0,	OP (rrs6_ri12),		D_A_BC},

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

   {"jmp",	Z (0xf0),	0,	OP (jmp),		B},
   {"pop",	O (0xf8),	0,	OP (pop),		B},
   {"resume",	O (0xf9)
		  | 0xffff00,	0,	OP (resume),		B},

   {"pushj",	Z (0xf2),	0,	OP (pushj),		J},

   {"geta",	Z (0xf4),	0,	OP (regaddr),		N},
   {"sync",	O (0xfc),	0,	OP (sync),		N},
   {"swym",	O (SWYM_INSN_BYTE),
				0,	OP (xyz_opt),		N},

   {"put", Z (0xf6) | 0xff00,	0,	OP (put),		N},
   {"get", O (0xfe) | 0xffe0,	0,	OP (get),		N},
   {"trip",	O (0xff),	0,	OP (xyz_opt),		J},

	{"nop",	O (SWYM_INSN_BYTE),	0,	OP (none),	N},

	{NULL,	~0,	~0,	0,	OP (none),		N}
 };
