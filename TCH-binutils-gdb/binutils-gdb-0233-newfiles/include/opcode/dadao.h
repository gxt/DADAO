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

   /* All operands are registers: "$X,$Y,$Z".  */
   dadao_operands_regs,

   /* "$X,YZ", like SETH.  */
   dadao_operands_reg_yz,

   /* The regular "$X,$Y,$Z|Z".
      The Z is optional; if only "$X,$Y" is given, then "$X,$Y,0" is
      assumed.  */
   dadao_operands_regs_z_opt,

   /* The regular "$X,$Y,$Z|Z".  */
   dadao_operands_regs_z,

   /* The regular "rega, regb << shift6" or "rega, imm12" */
   dadao_operands_rrs6_ri12,

   /* The regular "regb, regc", or "regb, imm6" */
   dadao_operands_rr_ri6,

   /* "Address"; only JMP.  Zero operands allowed unless GNU syntax.  */
   dadao_operands_jmp,

   /* "$X|X,$Y,$Z|Z": PUSHGO; like "3", but X can be expressed as an
      integer.  */
   dadao_operands_pushgo,

   /* Two registers or a register and a byte, like FLOT, possibly with
      rounding: "$X,$Z|Z" or "$X,ROUND_MODE,$Z|Z".  */
   dadao_operands_roundregs_z,

   /* "X,YZ", POP.  Unless GNU syntax, zero or one operand is allowed.  */
   dadao_operands_pop,

   /* Two registers, possibly with rounding: "$X,$Z" or
      "$X,ROUND_MODE,$Z".  */
   dadao_operands_roundregs,

   /* "XYZ", like SYNC.  */
   dadao_operands_sync,

   /* "X,$Y,$Z|Z", like SYNCD.  */
   dadao_operands_x_regs_z,

   /* "$X,Address, like GETA or branches.  */
   dadao_operands_regaddr,

   /* "$X|X,Address, like PUSHJ.  */
   dadao_operands_pushj,

   /* "$X,spec_reg"; GET.  */
   dadao_operands_get,

   /* "spec_reg,$Z|Z"; PUT.  */
   dadao_operands_put,

   /* Two registers, "$X,$Y".  */
   dadao_operands_set,

   /* "X,Y,Z"; like SWYM or TRAP.  Zero (or 1 if GNU syntax) to three
      operands, interpreted as 0; XYZ; X, YZ and X, Y, Z.  */
   dadao_operands_xyz_opt,

   /* Just "Z", like RESUME.  Unless GNU syntax, the operand can be omitted
      and will then be assumed zero.  */
   dadao_operands_resume,
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

#define PUSHGO_INSN_BYTE 0xbe
#define GO_INSN_BYTE 0x9e
#define SETL_INSN_BYTE 0xe3
#define INCML_INSN_BYTE 0xe6
#define INCMH_INSN_BYTE 0xe5
#define INCH_INSN_BYTE 0xe4
#define SWYM_INSN_BYTE 0xfd
#define JMP_INSN_BYTE 0xf0

/* Dadao bit-field definition:
 *   OP: 8-bit, [31..24]
 *   FA: 6-bit, [23..18]
 *   FB: 6-bit, [17..12]
 *   FC: 6-bit, [11..6]
 *   FD: 6-bit, [5..0]
 */
#define DDOP_CHECK_8_BIT(ddop_fx)								\
	do {											\
		if ((ddop_fx) < 0)								\
			as_bad_where(__FILE__, __LINE__, "negative");				\
		if ((ddop_fx) > 0xFF)								\
			as_bad_where(__FILE__, __LINE__, "bigger than 255");			\
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
