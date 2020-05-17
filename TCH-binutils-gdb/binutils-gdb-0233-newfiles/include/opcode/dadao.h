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
   dadao_type_fd_eq_fa_op_bc,
   dadao_type_fd_eq_fb_op_fc,
   dadao_type_branch,
   dadao_type_condbranch,
   dadao_type_jmp,
   dadao_type_geta,
   dadao_type_memaccess_octa,
   dadao_type_memaccess_tetra,
   dadao_type_memaccess_wyde,
   dadao_type_memaccess_byte,
   dadao_type_memaccess_block,
   dadao_type_jsr
 };

/* Type of operands an instruction takes.  Use when parsing assembly code
   and disassembling.  */
enum dadao_operands_type
 {
   dadao_operands_none = 0,

   /* "$X,YZ", like SETH.  */
   dadao_operands_reg_yz,

	/* All operands are registers: "regd, regb, regc".  */
	dadao_operands_fa_op_fbcd_reg,

	/* The regular "rega, imm18" */
	dadao_operands_fa_reg_fbcd_i18,

	/* The regular "regd, imm16", 2-bit in fc used to indicate H/MH/ML/L */
	dadao_operands_fd_reg_fabc_i16,

	/* The regular "regd, rega, regb << shift6" or "regd, rega, imm12" */
	dadao_operands_fdfa_reg_fbc_rs6_i12,

	/* The regular "regd, rega, regb << shift6" or "regd, rega, imm12" or "regd, sym */
	dadao_operands_fdfa_reg_fbc_rs6_i12_or_sym,

	/* The regular "regd, regb, regc", or "regd, regb, imm6" */
	dadao_operands_fa_op_fdfb_reg_fc_i6,

	/* ONLY "imm18" accepted, for swym/trip/trap/nop */
	dadao_operands_fa_op_fbcd_i18,

   /* "X,YZ", POP.  Unless GNU syntax, zero or one operand is allowed.  */
   dadao_operands_pop,

   /* "$X|X,Address, like PUSHJ.  */
   dadao_operands_pushj,

   /* "$X,spec_reg"; GET.  */
   dadao_operands_get,

   /* "spec_reg,$Z|Z"; PUT.  */
   dadao_operands_put,
 };

struct dadao_opcode
 {
   const char *name;
   unsigned long match;
   unsigned long lose;
	unsigned char fa_as_opcode;	/* Some insns use fa as op code */
   enum dadao_operands_type operands;

   /* This is used by the disassembly function.  */
   enum dadao_insn_type type;
 };

/* Declare the actual tables.  */
extern const struct dadao_opcode dadao_opcodes[];

/* This one is terminated with an entry with a NULL name.  */
extern const struct dadao_spec_reg dadao_spec_regs[];

/* Some insn values we use when padding and synthesizing address loads.  */
#define IMM_OFFSET_BIT 1
#define COND_INV_BIT 0x8
#define PRED_INV_BIT 0x10

#define GO_INSN_BYTE 0x9e
#define JMP_INSN_BYTE 0xf0

#define	DADAO_WYDE_L			0
#define	DADAO_WYDE_ML			1
#define	DADAO_WYDE_MH			2
#define	DADAO_WYDE_H			3

#define	DADAO_INSN_SETW			0xE0
#define	DADAO_INSN_INCW			0xE1
#define	DADAO_INSN_ORW			0xE2
#define	DADAO_INSN_ANDNW		0xE3

/* Dadao bit-field definition:
 *   OP: 8-bit, [31..24]
 *   FA: 6-bit, [23..18]
 *   FB: 6-bit, [17..12]
 *   FC: 6-bit, [11..6]
 *   FD: 6-bit, [5..0]
 */
#define DDOP_CHECK_BIT(ddop_fx, bit_count)							\
	do {											\
		if ((ddop_fx) < 0)								\
			as_bad_where(__FILE__, __LINE__, "negative");				\
		if ((ddop_fx) > ((1 << (bit_count)) - 1))					\
			as_bad_where(__FILE__, __LINE__, "bigger than %d bits", bit_count );	\
	} while (0)

#define DDOP_CHECK_2_BIT(ddop_fx) DDOP_CHECK_BIT(ddop_fx, 2)
#define DDOP_CHECK_16_BIT(ddop_fx) DDOP_CHECK_BIT(ddop_fx, 16)

#define DDOP_CHECK_8_BIT(ddop_fx)								\
	do {											\
		if ((ddop_fx) < 0)								\
			as_bad_where(__FILE__, __LINE__, "negative");				\
		if ((ddop_fx) > 0xFF)								\
			as_bad_where(__FILE__, __LINE__, "bigger than 255");			\
	} while (0)

#define DDOP_CHECK_18_BIT(ddop_fx)								\
	do {											\
		if ((ddop_fx) < 0)								\
			as_bad_where(__FILE__, __LINE__, "negative");				\
		if ((ddop_fx) > 0x3FFFF)								\
			as_bad_where(__FILE__, __LINE__, "bigger than 18-bit uint");			\
	} while (0)

/* FIXME: after gcc works */
#define DDOP_CHECK_6_BIT(x) DDOP_CHECK_8_BIT(x)
#if 0
#define DDOP_CHECK_6_BIT(ddop_fx)								\
	do {											\
		if ((ddop_fx) < 0)								\
			as_bad_where(__FILE__, __LINE__, "negative");				\
		if ((ddop_fx) > 0x3F)								\
			as_bad_where(__FILE__, __LINE__, "bigger than 63, FIXME!");		\
	} while (0)
#endif

#define DDOP_EXP_MUST_BE_REG(ddop_exp)								\
	do {											\
		if (ddop_exp.X_op != O_register)						\
			as_bad_where(__FILE__, __LINE__, "Exp should be register");		\
		DDOP_CHECK_6_BIT(ddop_exp.X_add_number);					\
	} while (0)

#define DDOP_EXP_MUST_BE_UIMM6(ddop_exp)							\
	do {											\
		if (ddop_exp.X_op != O_constant)						\
			as_bad_where(__FILE__, __LINE__, "Exp should be 6-bit const");		\
		DDOP_CHECK_6_BIT(ddop_exp.X_add_number);					\
	} while (0)

#define DDOP_EXP_MUST_BE_UIMM12(ddop_exp)							\
	do {											\
		if (ddop_exp.X_op != O_constant)						\
			as_bad_where(__FILE__, __LINE__, "Exp should be 12-bit const");		\
		if ((ddop_exp.X_add_number) < 0)						\
			as_bad_where(__FILE__, __LINE__, "negative");				\
		if ((ddop_exp.X_add_number) > 0xFFF)						\
			as_bad_where(__FILE__, __LINE__, "bigger than 12-bit");			\
	} while (0)

#define DDOP_SET_FA(ddop_insn_p, ddop_fa)							\
	do {											\
		char *ddop_insn_char_p = (char *)ddop_insn_p;					\
		DDOP_CHECK_6_BIT(ddop_fa);							\
		ddop_insn_char_p[1] |= (((ddop_fa) & 0x3F) << 2);				\
	} while (0)

#define DDOP_SET_FB(ddop_insn_p, ddop_fb)							\
	do {											\
		char *ddop_insn_char_p = (char *)ddop_insn_p;					\
		DDOP_CHECK_6_BIT(ddop_fb);							\
		ddop_insn_char_p[1] |= (((ddop_fb) & 0x3F) >> 4);				\
		ddop_insn_char_p[2] |= (((ddop_fb) & 0xF) << 4);				\
	} while (0)

#define DDOP_SET_FC(ddop_insn_p, ddop_fc)							\
	do {											\
		char *ddop_insn_char_p = (char *)ddop_insn_p;					\
		DDOP_CHECK_6_BIT(ddop_fc);							\
		ddop_insn_char_p[2] |= (((ddop_fc) & 0x3F) >> 2);				\
		ddop_insn_char_p[3] |= (((ddop_fc) & 0x3) << 6);				\
	} while (0)

#define DDOP_SET_FD(ddop_insn_p, ddop_fd)							\
	do {											\
		char *ddop_insn_char_p = (char *)ddop_insn_p;					\
		DDOP_CHECK_6_BIT(ddop_fd);							\
		ddop_insn_char_p[3] |= ((ddop_fd) & 0x3F);					\
	} while (0)

#define DDOP_SET_FB_FC(ddop_insn_p, ddop_fb_fc)							\
	do {											\
		DDOP_SET_FB(ddop_insn_p, ((ddop_fb_fc) >> 6));					\
		DDOP_SET_FC(ddop_insn_p, ((ddop_fb_fc) & 0x3F));				\
	} while (0)

/* We can have 256 - 32 (local registers) - 1 ($255 is not allocatable)
   global registers.  */
#define MAX_GREGS 223
