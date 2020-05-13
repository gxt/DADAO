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

#define OP(y) XCONCAT2 (dadao_operands_,y)

/* Groups of instructions specified here must, if all are matching the
   same instruction, be consecutive, in order more-specific to
   less-specific match.  */

const struct dadao_opcode dadao_opcodes[] =
 {
   {"trap",	O (0),		OP (xyz_opt),		J},
   {"fcmp",	O (1),		OP (regs),		N},
   {"flot",	Z (8),		OP (roundregs_z),	N},

   {"fun",	O (2),		OP (regs),		N},
   {"feql",	O (3),		OP (regs),		N},
   {"flotu",	Z (10),		OP (roundregs_z),	N},

   {"fadd",	O (4),		OP (regs),		N},
   {"fix",	O (5),		OP (roundregs),		N},
   {"sflot",	Z (12),		OP (roundregs_z),	N},

   {"fsub",	O (6),		OP (regs),		N},
   {"fixu",	O (7),		OP (roundregs),		N},
   {"sflotu",	Z (14),		OP (roundregs_z),	N},

   {"fmul",	O (16),		OP (regs),		N},
   {"fcmpe",	O (17),		OP (regs),		N},

   {"fune",	O (18),		OP (regs),		N},
   {"feqle",	O (19),		OP (regs),		N},

   {"fdiv",	O (20),		OP (regs),		N},
   {"fsqrt",	O (21),		OP (roundregs),		N},

   {"frem",	O (22),		OP (regs),		N},
   {"fint",	O (23),		OP (roundregs),		N},

	{"add",		Z (0x20),	OP (rrs6_ri12),		D_A_BC},
	{"addu",	Z (0x22),	OP (rrs6_ri12),		D_A_BC},
	{"sub",		Z (0x24),	OP (rrs6_ri12),		D_A_BC},
	{"subu",	Z (0x26),	OP (rrs6_ri12),		D_A_BC},
	{"mul",		Z (0x28),	OP (rrs6_ri12),		D_A_BC},
	{"mulu",	Z (0x2A),	OP (rrs6_ri12),		D_A_BC},
	{"div",		Z (0x2C),	OP (rrs6_ri12),		D_A_BC},
	{"divu",	Z (0x2E),	OP (rrs6_ri12),		D_A_BC},

	{"cmp",		Z (0x30),	OP (rrs6_ri12),		D_A_BC},
	{"cmpu",	Z (0x32),	OP (rrs6_ri12),		D_A_BC},

   /* Synonym for ADDU.  Put after ADDU, since we don't prefer it for
      disassembly.  It's supposed to be used for addresses, so we make it
      a memory block reference for purposes of assembly.  */
   {"lda",	Z (0x22),	OP (regs_z_opt),	M},

   {"sl",	Z (0x38),	OP (regs_z),		N},

   {"slu",	Z (0x3a),	OP (regs_z),		N},

   {"sr",	Z (0x3c),	OP (regs_z),		N},

   {"sru",	Z (0x3e),	OP (regs_z),		N},

   {"bn",	Z (0x40),	OP (regaddr),		C},
   {"bnn",	Z (0x48),	OP (regaddr),		C},

   {"bz",	Z (0x42),	OP (regaddr),		C},
   {"bnz",	Z (0x4a),	OP (regaddr),		C},

   {"bp",	Z (0x44),	OP (regaddr),		C},
   {"bnp",	Z (0x4c),	OP (regaddr),		C},

   {"bod",	Z (0x46),	OP (regaddr),		C},
   {"bev",	Z (0x4e),	OP (regaddr),		C},

   {"pbn",	Z (0x50),	OP (regaddr),		C},
   {"pbnn",	Z (0x58),	OP (regaddr),		C},

   {"pbz",	Z (0x52),	OP (regaddr),		C},
   {"pbnz",	Z (0x5a),	OP (regaddr),		C},

   {"pbp",	Z (0x54),	OP (regaddr),		C},
   {"pbnp",	Z (0x5c),	OP (regaddr),		C},

   {"pbod",	Z (0x56),	OP (regaddr),		C},
   {"pbev",	Z (0x5e),	OP (regaddr),		C},

	{"csn",		Z (0x60),	OP (rrs6_ri12),		D_A_BC},
	{"csnn",	Z (0x68),	OP (rrs6_ri12),		D_A_BC},

	{"csz",		Z (0x62),	OP (rrs6_ri12),		D_A_BC},
	{"csnz",	Z (0x6a),	OP (rrs6_ri12),		D_A_BC},

	{"csp",		Z (0x64),	OP (rrs6_ri12),		D_A_BC},
	{"csnp",	Z (0x6c),	OP (rrs6_ri12),		D_A_BC},

	{"csod",	Z (0x66),	OP (rrs6_ri12),		D_A_BC},
	{"csev",	Z (0x6e),	OP (rrs6_ri12),		D_A_BC},

   {"zsn",	Z (0x70),	OP (regs_z),		N},
   {"zsnn",	Z (0x78),	OP (regs_z),		N},

   {"zsz",	Z (0x72),	OP (regs_z),		N},
   {"zsnz",	Z (0x7a),	OP (regs_z),		N},

   {"zsp",	Z (0x74),	OP (regs_z),		N},
   {"zsnp",	Z (0x7c),	OP (regs_z),		N},

   {"zsod",	Z (0x76),	OP (regs_z),		N},
   {"zsev",	Z (0x7e),	OP (regs_z),		N},

   {"ldb",	Z (0x80),	OP (regs_z_opt),	MB},
   {"ldt",	Z (0x88),	OP (regs_z_opt),	MT},

   {"ldbu",	Z (0x82),	OP (regs_z_opt),	MB},
   {"ldtu",	Z (0x8a),	OP (regs_z_opt),	MT},

   {"ldw",	Z (0x84),	OP (regs_z_opt),	MW},
   {"ldo",	Z (0x8c),	OP (regs_z_opt),	MO},

   {"ldwu",	Z (0x86),	OP (regs_z_opt),	MW},
   {"ldou",	Z (0x8e),	OP (regs_z_opt),	MO},

   {"ldsf",	Z (0x90),	OP (regs_z_opt),	MT},

   /* This doesn't seem to access memory, just the TLB.  */
   {"ldvts",	Z (0x98),	OP (regs_z_opt),	M},

   {"ldht",	Z (0x92),	OP (regs_z_opt),	MT},

   /* Neither does this per-se.  */
   {"preld",	Z (0x9a),	OP (x_regs_z),		N},

   {"cswap",	Z (0x94),	OP (regs_z_opt),	MO},
   {"prego",	Z (0x9c),	OP (x_regs_z),		N},

   {"ldunc",	Z (0x96),	OP (regs_z_opt),	MO},
   {"go",	Z (GO_INSN_BYTE),
				OP (regs_z_opt),	B},

   {"stb",	Z (0xa0),	OP (regs_z_opt),	MB},
   {"stt",	Z (0xa8),	OP (regs_z_opt),	MT},

   {"stbu",	Z (0xa2),	OP (regs_z_opt),	MB},
   {"sttu",	Z (0xaa),	OP (regs_z_opt),	MT},

   {"stw",	Z (0xa4),	OP (regs_z_opt),	MW},
   {"sto",	Z (0xac),	OP (regs_z_opt),	MO},

   {"stwu",	Z (0xa6),	OP (regs_z_opt),	MW},
   {"stou",	Z (0xae),	OP (regs_z_opt),	MO},

   {"stsf",	Z (0xb0),	OP (regs_z_opt),	MT},
   {"syncd",	Z (0xb8),	OP (x_regs_z),		M},

   {"stht",	Z (0xb2),	OP (regs_z_opt),	MT},
   {"prest",	Z (0xba),	OP (x_regs_z),		M},

   {"stco",	Z (0xb4),	OP (x_regs_z),		MO},
   {"syncid",	Z (0xbc),	OP (x_regs_z),		M},

   {"stunc",	Z (0xb6),	OP (regs_z_opt),	MO},
   {"pushgo",	Z (PUSHGO_INSN_BYTE),
				OP (pushgo),		J},

   /* Synonym for OR with a zero Z.  */
   {"set",	O (0xc1)
		  | 0xff,	OP (set),		N},

   {"or",	Z (0xc0),	OP (regs_z),		N},
   {"and",	Z (0xc8),	OP (regs_z),		N},

   {"orn",	Z (0xc2),	OP (regs_z),		N},
   {"andn",	Z (0xca),	OP (regs_z),		N},

   {"nor",	Z (0xc4),	OP (regs_z),		N},
   {"nand",	Z (0xcc),	OP (regs_z),		N},

   {"xor",	Z (0xc6),	OP (regs_z),		N},
   {"nxor",	Z (0xce),	OP (regs_z),		N},

   {"seth",	O (0xe0),	OP (reg_yz),		N},
   {"setmh",	O (0xe1),	OP (reg_yz),		N},
   {"orh",	O (0xe8),	OP (reg_yz),		N},
   {"ormh",	O (0xe9),	OP (reg_yz),		N},

   {"setml",	O (0xe2),	OP (reg_yz),		N},
   {"setl",	O (SETL_INSN_BYTE),
				OP (reg_yz),		N},
   {"orml",	O (0xea),	OP (reg_yz),		N},
   {"orl",	O (0xeb),	OP (reg_yz),		N},

   {"inch",	O (INCH_INSN_BYTE),
				OP (reg_yz),		N},
   {"incmh",	O (INCMH_INSN_BYTE),
				OP (reg_yz),		N},
   {"andnh",	O (0xec),	OP (reg_yz),		N},
   {"andnmh",	O (0xed),	OP (reg_yz),		N},

   {"incml",	O (INCML_INSN_BYTE),
				OP (reg_yz),		N},
   {"incl",	O (0xe7),	OP (reg_yz),		N},
   {"andnml",	O (0xee),	OP (reg_yz),		N},
   {"andnl",	O (0xef),	OP (reg_yz),		N},

   {"jmp",	Z (0xf0),	OP (jmp),		B},
   {"pop",	O (0xf8),	OP (pop),		B},
   {"resume",	O (0xf9)
		  | 0xffff00,	OP (resume),		B},

   {"pushj",	Z (0xf2),	OP (pushj),		J},
   {"save",	O (0xfa)
		  | 0xffff,	OP (save),		M},
   {"unsave",	O (0xfb)
		  | 0xffff00,	OP (unsave),		M},

   {"geta",	Z (0xf4),	OP (regaddr),		N},
   {"sync",	O (0xfc),	OP (sync),		N},
   {"swym",	O (SWYM_INSN_BYTE),
				OP (xyz_opt),		N},

   {"put", Z (0xf6) | 0xff00,	OP (put),		N},
   {"get", O (0xfe) | 0xffe0,	OP (get),		N},
   {"trip",	O (0xff),	OP (xyz_opt),		J},

	{"nop",	O (SWYM_INSN_BYTE),	OP (none),	N},

   {NULL, ~0, ~0, OP (none), N}
 };
