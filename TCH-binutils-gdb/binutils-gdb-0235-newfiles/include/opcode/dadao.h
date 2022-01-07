/*
 * Definitions for decoding the dadao opcode table.
 * Copyright (C) 2019-2033 Guan Xuetao (AT) Peking Univ.
 *
 * Contributed by:
 *   2019:
 *	Guan Xuetao <gxt@pku.edu.cn>
 */

/* The table is terminated with an entry with a NULL name.  */
struct dadao_reg_alias
{
	const char *name;
	unsigned int number;
};

extern const struct dadao_reg_alias dadao_reg_aliases[];

/* General indication of the type of instruction.  */
enum dadao_insn_type {
	dadao_type_normal,
	dadao_type_condbranch,
	dadao_type_branch,
	dadao_type_geta,
	dadao_type_dref,
	dadao_type_jsr,
	dadao_type_pseudo,
};

/* Type of operand an instruction takes. */
enum dadao_operand_type {
	dadao_operand_none = 0,
	dadao_operand_op,	/* minor-opcode */
	dadao_operand_rd,	/* rd reg class */
	dadao_operand_rb,	/* rb reg class */
	dadao_operand_rf,	/* rf reg class */
	dadao_operand_ra,	/* ra reg class */
	dadao_operand_i6,	/* shift left imm6 (unsigned) */
	dadao_operand_u12,	/* unsigned imm 12-bit */
	dadao_operand_u18,	/* unsigned imm 18-bit */
	dadao_operand_s12,	/* signed imm 12-bit */
	dadao_operand_s18,	/* signed imm 18-bit */
	dadao_operand_s24,	/* signed imm 24-bit */
	dadao_operand_w16,	/* setw/incw 16-bit word (unsigned) */
	dadao_operand_cp,	/* co-processor id */
	dadao_operand_cr,	/* co-processor reg number */
	dadao_operand_noop,	/* hard insn should not be here */
};

struct dadao_opcode {
	const char *name;
	int operands_num;
	unsigned int major_opcode;
	unsigned int minor_opcode;
	enum dadao_operand_type op_fa;
	enum dadao_operand_type op_fb;
	enum dadao_operand_type op_fc;
	enum dadao_operand_type op_fd;
	enum dadao_insn_type type;
};

/* Declare the actual tables.  */
extern const struct dadao_opcode dadao_opcodes[];

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

#define	DADAO_INSN_FT			((unsigned int)(0x2A << 24))
#define	DADAO_INSN_FO			((unsigned int)(0x2B << 24))

#define	DADAO_INSN_SETO			((unsigned int)(0x14 << 24))
#define	DADAO_INSN_SETZ			((unsigned int)(0x15 << 24))
#define	DADAO_INSN_OR			((unsigned int)(0x16 << 24))
#define	DADAO_INSN_ANDN			((unsigned int)(0x17 << 24))

#define	DADAO_INSN_GETA			((unsigned int)(0x18 << 24))
#define	DADAO_INSN_CALL			((unsigned int)(0xC8 << 24))
#define	DADAO_INSN_JUMP			((unsigned int)(0xD8 << 24))

/* Dadao bit-field definition:
 *   OP: 8-bit, [31..24]
 *   FA: 6-bit, [23..18]
 *   FB: 6-bit, [17..12]
 *   FC: 6-bit, [11..6]
 *   FD: 6-bit, [5..0]
 */
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

