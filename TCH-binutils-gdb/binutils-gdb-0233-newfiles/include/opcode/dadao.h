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
   dadao_type_pseudo,
   dadao_type_normal,
   dadao_type_fd_eq_fa_op_bc,
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

   /* "$X,Y,$Z|Z", like NEG and NEGU.  The Y field is optional, default 0.  */
   dadao_operands_neg,

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

   /* "$X,0"; SAVE.  */
   dadao_operands_save,

   /* "0,$Z"; UNSAVE. */
   dadao_operands_unsave,

   /* "X,Y,Z"; like SWYM or TRAP.  Zero (or 1 if GNU syntax) to three
      operands, interpreted as 0; XYZ; X, YZ and X, Y, Z.  */
   dadao_operands_xyz_opt,

   /* Just "Z", like RESUME.  Unless GNU syntax, the operand can be omitted
      and will then be assumed zero.  */
   dadao_operands_resume,

   /* These are specials to handle that pseudo-directives are specified
      like ordinary insns when being dadaoal-compatible.  They signify the
      specific pseudo-directive rather than the operands type.  */

   /* LOC.  */
   dadao_operands_loc,

   /* PREFIX.  */
   dadao_operands_prefix,

   /* BYTE.  */
   dadao_operands_byte,

   /* WYDE.  */
   dadao_operands_wyde,

   /* TETRA.  */
   dadao_operands_tetra,

   /* OCTA.  */
   dadao_operands_octa,

   /* LOCAL.  */
   dadao_operands_local,

   /* BSPEC.  */
   dadao_operands_bspec,

   /* ESPEC.  */
   dadao_operands_espec,
 };

struct dadao_opcode
 {
   const char *name;
   unsigned long match;
   unsigned long lose;
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

/* We can have 256 - 32 (local registers) - 1 ($255 is not allocatable)
   global registers.  */
#define MAX_GREGS 223
