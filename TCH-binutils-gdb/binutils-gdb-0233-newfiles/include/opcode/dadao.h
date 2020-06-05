/*
 * Definitions for decoding the dadao opcode table.
 * Copyright (C) 2019-2033 Guan Xuetao (AT) Peking Univ.
 *
 * Contributed by:
 *   2019:
 *	Guan Xuetao <gxt@pku.edu.cn>
 */

/* We could have just a char*[] table indexed by the register number, but
   that would not allow for synonyms.  The table is terminated with an
   entry with a NULL name.  */
struct dadao_spec_reg
{
  const char *name;
  unsigned int number;
};

/* General indication of the type of instruction.  */
enum dadao_insn_type
 {
	dadao_type_normal,
	dadao_type_pseudo,
	dadao_type_condbranch,
	dadao_type_branch,
	dadao_type_geta,
	dadao_type_dref,
	dadao_type_jsr
 };

/* Type of operands an instruction takes.  Use when parsing assembly code
   and disassembling.  */
enum dadao_operands_type {
	dadao_operands_none = 0,

	dadao_operands_ps_seto,		/* seto pseudo insn */

	dadao_operands_iiii_rrii,	/* call/jump insn, "imm24" or "ra, rb, imm12" */
	dadao_operands_o000,		/* no operand, for nop/ret */
	dadao_operands_oiii,		/* ONLY "imm18" accepted, for swym/trip/trap/nop */
	dadao_operands_orr0_get,	/* "rb, spec_reg"; GET.  */
	dadao_operands_orr0_put,	/* "spec_reg, rc"; PUT.  */
	dadao_operands_orri,		/* The regular "rb, rc, imm6" */
	dadao_operands_orri_orrr,	/* The regular "rb, rc, rd", or "rb, rc, imm6" */
	dadao_operands_orrr,		/* The regular "rb, rc, rd".  */
	dadao_operands_riii,		/* The regular "ra, imm18" */
	dadao_operands_rjii,		/* The regular "ra, imm16", 2-bit in fc used to indicate WH/WJ/WK/WL */
	dadao_operands_rrii_rrri,	/* The regular "ra, rb, imm12" or "ra, rb, rc << imm6" */
	dadao_operands_rsym_rrii_rrri	/* The regular "ra, rb, imm12" or "ra, rb, rc << imm6" with symbol support */
 };

struct dadao_opcode
 {
	const char *name;
	unsigned int major_opcode;
	unsigned int minor_opcode;
	unsigned int double_modes;	/* 0 means one operand mode, 1 means two operand modes */
	enum dadao_operands_type operands;
	enum dadao_insn_type type;
 };

/* Declare the actual tables.  */
extern const struct dadao_opcode dadao_opcodes[];

/* This one is terminated with an entry with a NULL name.  */
extern const struct dadao_spec_reg dadao_spec_regs[];

#define	DADAO_BAD_INSN(msg)									\
	do {											\
		as_bad_where(__FILE__, __LINE__, "(%s %s) %s",					\
			 instruction->name, operands, (msg));					\
	} while (0)

/* Some insn values we use when padding and synthesizing address loads.  */
#define	DADAO_INSN_ALTMODE		(1 << 24)	/* The alternative addressing mode, generally bigger immediate */

#define	DADAO_REGP_PC			0		/* No. 0 reg is pc, read only */
#define	DADAO_REGP_TAO			3		/* No. 3 reg is reserved for insn expand */

/* One octa, four wydes, named wh/wj/wk/wl */
#define	DADAO_WYDE_WH			(3 << 16)
#define	DADAO_WYDE_WJ			(2 << 16)
#define	DADAO_WYDE_WK			(1 << 16)
#define	DADAO_WYDE_WL			(0 << 16)

#define	DADAO_INSN_SETW			((unsigned int)(0x3A << 24))
#define	DADAO_INSN_INCW			((unsigned int)(0x3B << 24))
#define	DADAO_INSN_ORW			((unsigned int)(0x3C << 24))
#define	DADAO_INSN_ANDNW		((unsigned int)(0x3D << 24))

#define	DADAO_INSN_GETA			((unsigned int)(0xCA << 24))
#define	DADAO_INSN_CALL			((unsigned int)(0xD0 << 24))
#define	DADAO_INSN_JUMP			((unsigned int)(0xD2 << 24))

#define	DADAO_INSN_FP			((unsigned int)(0xDB << 24))

/* Dadao bit-field definition:
 *   OP: 8-bit, [31..24]
 *   FA: 6-bit, [23..18]
 *   FB: 6-bit, [17..12]
 *   FC: 6-bit, [11..6]
 *   FD: 6-bit, [5..0]
 */
#define DDOP_CHECK_BIT_COUNT(ddop_fx, bit_count)						\
	do {											\
		if (((ddop_fx) < 0) || ((ddop_fx) > ((1 << (bit_count)) - 1)))			\
			DADAO_BAD_INSN("bit count is too big");					\
	} while (0)

#define DDOP_EXP_MUST_BE_REG(ddop_exp)								\
	do {											\
		if (ddop_exp.X_op != O_register)						\
			DADAO_BAD_INSN("exp should be register");				\
		DDOP_CHECK_BIT_COUNT(ddop_exp.X_add_number, 8);	/* FIXME: shoule be 6 */	\
	} while (0)

#define DDOP_EXP_MUST_BE_UIMM(ddop_exp, bit_count)						\
	do {											\
		if (ddop_exp.X_op != O_constant)						\
			DADAO_BAD_INSN("exp should be const");					\
		DDOP_CHECK_BIT_COUNT(ddop_exp.X_add_number, (bit_count));			\
	} while (0)

#define	DDOP_SET_INSN_ALTMODE(ddop_insn_p)							\
	do {											\
		char *ddop_insn_char_p = (char *)ddop_insn_p;					\
		ddop_insn_char_p[0] |= 1;							\
	} while (0)

#define DDOP_SET_FA(ddop_insn_p, ddop_fa)							\
	do {											\
		char *ddop_insn_char_p = (char *)ddop_insn_p;					\
		ddop_insn_char_p[1] |= (((ddop_fa) & 0x3F) << 2);				\
	} while (0)

#define DDOP_SET_FB(ddop_insn_p, ddop_fb)							\
	do {											\
		char *ddop_insn_char_p = (char *)ddop_insn_p;					\
		ddop_insn_char_p[1] |= (((ddop_fb) & 0x3F) >> 4);				\
		ddop_insn_char_p[2] |= (((ddop_fb) & 0xF) << 4);				\
	} while (0)

#define DDOP_SET_FC(ddop_insn_p, ddop_fc)							\
	do {											\
		char *ddop_insn_char_p = (char *)ddop_insn_p;					\
		ddop_insn_char_p[2] |= (((ddop_fc) & 0x3F) >> 2);				\
		ddop_insn_char_p[3] |= (((ddop_fc) & 0x3) << 6);				\
	} while (0)

#define DDOP_SET_FD(ddop_insn_p, ddop_fd)							\
	do {											\
		char *ddop_insn_char_p = (char *)ddop_insn_p;					\
		ddop_insn_char_p[3] |= ((ddop_fd) & 0x3F);					\
	} while (0)

