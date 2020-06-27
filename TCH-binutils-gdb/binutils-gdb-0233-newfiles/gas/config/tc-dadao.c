/*
 * tc-dadao.c -- Assemble code for dadao
 * Copyright (C) 2019-2033 Guan Xuetao (AT) Peking Univ.
 *
 * Contributed by:
 *   2019:
 *	Guan Xuetao <gxt@pku.edu.cn>
 */
#include "as.h"
#include <limits.h>
#include "subsegs.h"
#include "elf/dadao.h"
#include "opcode/dadao.h"
#include "safe-ctype.h"
#include "dwarf2dbg.h"

static int get_operands (char *, expressionS *);
static void dd_set_addr_offset(char *, offsetT, int, int);
static void dd_fill_nops (char *, int);

/* The fragS of the instruction being assembled.  Only valid from within
   md_assemble.  */
fragS *dadao_opcode_frag = NULL;

const char *md_shortopts = "x";

/* Should we automatically expand instructions into multiple insns in
   order to generate working code?  */
static int expand_op = 1;

struct option md_longopts[] =
 {
#define OPTION_NOEXPAND  (OPTION_MD_BASE)
   {"no-expand", no_argument, NULL, OPTION_NOEXPAND},
   {NULL, no_argument, NULL, 0}
 };

size_t md_longopts_size = sizeof (md_longopts);

static struct hash_control *dadao_opcode_hash;

/* For DADAO, we encode the relax_substateT:s (in e.g. fr_substate) as one
   bit length, and the relax-type shifted on top of that.  There seems to
   be no point in making the relaxation more fine-grained; the linker does
   that better and we might interfere by changing non-optimal relaxations
   into other insns that cannot be relaxed as easily.

   Groups for DADAO relaxing:

   1. GETA
      extra length: zero or three insns.

   2. BRCC
      extra length: zero or five insns.

   3. CALL
      extra length: zero or four insns.

   4. JUMP
      extra length: zero or four insns.
 */

#define STATE_GETA	(1)
#define STATE_BRCC	(2)
#define STATE_CALL	(3)
#define STATE_JUMP	(4)

/* No fine-grainedness here.  */
#define STATE_LENGTH_MASK	    (1)

#define STATE_ZERO		    (0)
#define STATE_MAX		    (1)

/* More descriptive name for convenience.  */
/* FIXME: We should start on something different, not MAX.  */
#define STATE_UNDF		    STATE_MAX

/* These displacements are relative to the address following the opcode
   word of the instruction.  The catch-all states have zero for "reach"
   and "next" entries.  */

#define RELAX_ENCODE_SHIFT 1
#define ENCODE_RELAX(what, length) (((what) << RELAX_ENCODE_SHIFT) + (length))

#define	DD_INSN_BYTES(n)	((n) * 4)

const relax_typeS dadao_relax_table[] = {
   /* Error sentinel (0, 0).  */
   {1,		1,		0,			0},

   /* Unused (0, 1).  */
   {1,		1,		0,			0},

   /* GETA (1, 0).  */
   {(1 << 18),	-(1 << 18),	0,			ENCODE_RELAX (STATE_GETA, STATE_MAX)},

   /* GETA (1, 1).  */
   {0,		0,		DD_INSN_BYTES(3),	0},

   /* BRCC (2, 0).  */
   {(1 << 20),	-(1 << 20),	0,			ENCODE_RELAX (STATE_BRCC, STATE_MAX)},

   /* BRCC (2, 1).  */
   {0,		0,		DD_INSN_BYTES(5),	0},

   /* CALL (3, 0).  */
   {(1 << 26),	-(1 << 26),	0,			ENCODE_RELAX (STATE_CALL, STATE_MAX)},

   /* CALL (3, 1).  */
   {0,		0,		DD_INSN_BYTES(4),	0},

   /* JUMP (4, 0).  */
   {(1 << 26),	-(1 << 26),	0,			ENCODE_RELAX (STATE_JUMP, STATE_MAX)},

   /* JUMP (4, 1).  */
   {0,		0,		DD_INSN_BYTES(4),	0},
};

const pseudo_typeS md_pseudo_table[] = {
	{"dd.b08", cons, 1},
	{"dd.w16", cons, 2},
	{"dd.t32", cons, 4},
	{"dd.o64", cons, 8},

	{NULL, 0, 0}
 };

const char comment_chars[] = "";
const char line_comment_chars[] = "#";
const char line_separator_chars[] = ";";

const char EXP_CHARS[] = "eE";

const char FLT_CHARS[] = "rf";

/* Fill in the offset-related part.  */
static void dd_set_addr_offset(char *opcodep, offsetT value, int bitcount, int is_insn)
{
	if (is_insn) {
		if ((value & 0x3) != 0)
			as_fatal("instruction address not align correctly");

		value /= 4;
	}

	if ((value > 0) && (value >= (1 << bitcount)))
		as_fatal("offset too large: 0x%llx", (unsigned long long)value);
	if ((value < 0) && ((-value) > (1 << bitcount)))
		as_fatal("offset too large: 0x%llx", (unsigned long long)value);

	switch (bitcount) {
	case 24:
		DDOP_SET_FA(opcodep, (value >> 18) & 0x3F);
		/* FALLTHROUGH */
	case 18:
		DDOP_SET_FB(opcodep, (value >> 12) & 0x3F);
		/* FALLTHROUGH */
	case 12:
		DDOP_SET_FC(opcodep, (value >> 6) & 0x3F);
		DDOP_SET_FD(opcodep, (value & 0x3F));
		break;
	default:
		as_fatal("offset bitcount(%d) not support", bitcount);
	}
}

/* Fill in NOP:s for the expanded part of GETA/JUMP/BRCC.  */
static void dd_fill_nops (char *opcodep, int n)
{
  int i;

  for (i = 0; i < n; i++)
    md_number_to_chars (opcodep + i * 4, DADAO_INSN_SWYM, 4);
}

/* Get up to four operands, filling them into the exp array.
   General idea and code stolen from the tic80 port.  */

static int
get_operands (char *s, expressionS *exp)
{
  char *p = s;
  int numexp = 0;
  int max_operands = 4;
  int nextchar = ',';

  while (nextchar == ',')
    {
      /* Skip leading whitespace */
      while (*p == ' ' || *p == '\t')
	p++;

      /* Check to see if we have any operands left to parse */
      if (*p == 0 || *p == '\n' || *p == '\r')
	{
	  break;
	}
      else if (numexp == max_operands)
	{
	  /* This seems more sane than saying "too many operands".  We'll
	     get here only if the trailing trash starts with a comma.  */
	  as_bad (_("too many operands"));
	  return 0;
	}

      /* Begin operand parsing at the current scan point.  */

      input_line_pointer = p;
      expression (&exp[numexp]);

      if (exp[numexp].X_op == O_illegal)
	{
	  as_bad (_("invalid operands"));
	}
      else if (exp[numexp].X_op == O_absent)
	{
	  as_bad (_("missing operand"));
	}

      numexp++;
      p = input_line_pointer;

      /* Skip leading whitespace */
      while (*p == ' ' || *p == '\t')
	p++;
      nextchar = *p++;
    }

  /* If we allow "naked" comments, ignore the rest of the line.  */
  if (nextchar != ',')
    {
      demand_empty_rest_of_line ();
      input_line_pointer--;
    }

  /* Mark the end of the valid operands with an illegal expression.  */
  exp[numexp].X_op = O_illegal;

  return (numexp);
}

/* Handle DADAO-specific option.  */

int
md_parse_option (int c, const char *arg ATTRIBUTE_UNUSED)
{
  switch (c)
    {
    case 'x':
      break;

    case OPTION_NOEXPAND:
      expand_op = 0;
      break;

    default:
      return 0;
    }

  return 1;
}

/* Display DADAO-specific help text.  */

void
md_show_usage (FILE * stream)
{
  fprintf (stream, _(" DADAO-specific command line options:\n"));
  fprintf (stream, _("\
  -no-expand              Do not expand GETA, branches or JUMP\n\
                          into multiple instructions.\n"));
  fprintf (stream, _("\
  -x                      Do not warn when an operand to GETA, a branch,\n\
                          or JUMP is not known to be within range.\n\
                          The linker will catch any errors. \n"));
}

/* Initialize GAS DADAO specifics.  */
void dadao_md_begin (void)
{
	int i;
	const struct dadao_opcode *opcode;
	char buf[5];

	dadao_opcode_hash = hash_new ();

	for (opcode = dadao_opcodes; opcode->name; opcode++)
		hash_insert (dadao_opcode_hash, opcode->name, (char *) opcode);

	for (i = 0; i < 64; i++) {
		sprintf (buf, "rg%d", i);
		symbol_table_insert (symbol_new (buf, reg_section, i, &zero_address_frag));

		sprintf (buf, "rp%d", i);
		symbol_table_insert (symbol_new (buf, reg_section, i + 64, &zero_address_frag));

		sprintf (buf, "rf%d", i);
		symbol_table_insert (symbol_new (buf, reg_section, i + 128, &zero_address_frag));

		sprintf (buf, "rv%d", i);
		symbol_table_insert (symbol_new (buf, reg_section, i + 192, &zero_address_frag));

		sprintf (buf, "rs%d", i);
		symbol_table_insert (symbol_new (buf, reg_section, i + 256, &zero_address_frag));
	}

	for (i = 0; dadao_reg_aliases[i].name != NULL; i++)
		symbol_table_insert (symbol_new (dadao_reg_aliases[i].name, reg_section,
				dadao_reg_aliases[i].number, &zero_address_frag));
}

/* Assemble one insn in STR.  */
void dadao_md_assemble (char *str)
{
  char *operands = str;
  char modified_char = 0;
  struct dadao_opcode *instruction;
  fragS *opc_fragP = NULL;

  /* Note that the struct frag member fr_literal in frags.h is char[], so
     I have to make this a plain char *.  */
  /* unsigned */ char *opcodep = NULL;

  expressionS exp[4];
  int n_operands = 0;
  int seto_flag = 0;
  unsigned int seto_w16 = 0;
  expressionS *exp_left;
  expressionS *exp_right;

	unsigned int regclass_offset_0;
	unsigned int regclass_offset_1;

	int flag_imm_signd = 0;

  /* Move to end of opcode.  */
  for (operands = str;
       is_part_of_name (*operands);
       ++operands)
    ;

  if (ISSPACE (*operands))
    {
      modified_char = *operands;
      *operands++ = '\0';
    }

  instruction = (struct dadao_opcode *) hash_find (dadao_opcode_hash, str);
  if (instruction == NULL)
    {
      as_bad (_("unknown opcode: `%s'"), str);

      return;
    }

  /* Put back the character after the opcode.  */
  if (modified_char != 0)
    operands[-1] = modified_char;

  input_line_pointer = operands;

  /* We assume that dadao_opcodes keeps having unique mnemonics for each
     opcode, so we don't have to iterate over more than one opcode; if the
     syntax does not match, then there's a syntax error.  */

    n_operands = get_operands (operands, exp);

  /* We also assume that the length of the instruction is at least 4, the
     size of an unexpanded instruction.  We need a self-contained frag
     since we want the relocation to point to the instruction, not the
     variant part.  */

  opcodep = frag_more (4);
  dadao_opcode_frag = opc_fragP = frag_now;
  frag_now->fr_opcode = opcodep;

  /* Mark start of insn for DWARF2 debug features.  */
  if (OUTPUT_FLAVOR == bfd_target_elf_flavour)
    dwarf2_emit_insn (4);

  md_number_to_chars (opcodep, (instruction->major_opcode) << 24, 4);

	/* Handle the rest.  */
	switch (instruction->operands) {
	case dadao_operands_ps_seto:	/* pseudo insn: seto ra, imm64 */
		if (n_operands != 2)
			DADAO_BAD_INSN("invalid operands to opcode");

		DDOP_EXP_MUST_BE_RG(exp[0]);

		if (exp[1].X_op != O_constant)
			DADAO_BAD_INSN("invalid operands to opcode");

		seto_flag = 0;

		seto_w16 = (exp[1].X_add_number >> 48) & 0xFFFF;
		if (seto_w16 != 0) {
			md_number_to_chars(opcodep, DADAO_INSN_SETW | DADAO_WYDE_WH | seto_w16, 4);
			DDOP_SET_FA(opcodep, exp[0].X_add_number);
			opcodep = frag_more (4);
			seto_flag = 1;
		}

		seto_w16 = (exp[1].X_add_number >> 32) & 0xFFFF;
		if (seto_w16 != 0) {
			if (seto_flag)
				md_number_to_chars(opcodep, DADAO_INSN_INCW | DADAO_WYDE_WJ | seto_w16, 4);
			else
				md_number_to_chars(opcodep, DADAO_INSN_SETW | DADAO_WYDE_WJ | seto_w16, 4);
			DDOP_SET_FA(opcodep, exp[0].X_add_number);
			opcodep = frag_more (4);
			seto_flag = 1;
		}

		seto_w16 = (exp[1].X_add_number >> 16) & 0xFFFF;
		if (seto_w16 != 0) {
			if (seto_flag)
				md_number_to_chars(opcodep, DADAO_INSN_INCW | DADAO_WYDE_WK | seto_w16, 4);
			else
				md_number_to_chars(opcodep, DADAO_INSN_SETW | DADAO_WYDE_WK | seto_w16, 4);
			DDOP_SET_FA(opcodep, exp[0].X_add_number);
			opcodep = frag_more (4);
			seto_flag = 1;
		}

		seto_w16 = (exp[1].X_add_number) & 0xFFFF;
		if (seto_flag)
			md_number_to_chars(opcodep, DADAO_INSN_INCW | DADAO_WYDE_WL | seto_w16, 4);
		else
			md_number_to_chars(opcodep, DADAO_INSN_SETW | DADAO_WYDE_WL | seto_w16, 4);
		DDOP_SET_FA(opcodep, exp[0].X_add_number);

		break;

	case dadao_operands_orr0: /* get.xx and put.xx */
		if (n_operands != 2)
			DADAO_BAD_INSN("invalid operands to opcode");

		regclass_offset_0 = 0x00;
		regclass_offset_1 = 0x00;

		switch (instruction->minor_opcode) {
		case 1:
			DDOP_EXP_MUST_BE_RP(exp[0]);
			DDOP_EXP_MUST_BE_RP(exp[1]);
			regclass_offset_0 = 0x40;
			regclass_offset_1 = 0x40;
			break;

		case 2:
			DDOP_EXP_MUST_BE_RF(exp[0]);
			DDOP_EXP_MUST_BE_RF(exp[1]);
			regclass_offset_0 = 0x80;
			regclass_offset_1 = 0x80;
			break;

		case 3:
			DDOP_EXP_MUST_BE_RV(exp[0]);
			DDOP_EXP_MUST_BE_RV(exp[1]);
			regclass_offset_0 = 0xC0;
			regclass_offset_1 = 0xC0;
			break;

		case 4:
			DDOP_EXP_MUST_BE_RS(exp[0]);
			DDOP_EXP_MUST_BE_RS(exp[1]);
			regclass_offset_0 = 0x100;
			regclass_offset_1 = 0x100;
			break;

		case 0x11:
			DDOP_EXP_MUST_BE_RP(exp[0]);
			DDOP_EXP_MUST_BE_RG(exp[1]);
			regclass_offset_0 = 0x40;
			break;

		case 0x12:
			DDOP_EXP_MUST_BE_RF(exp[0]);
			DDOP_EXP_MUST_BE_RG(exp[1]);
			regclass_offset_0 = 0x80;
			break;

		case 0x13:
			DDOP_EXP_MUST_BE_RV(exp[0]);
			DDOP_EXP_MUST_BE_RG(exp[1]);
			regclass_offset_0 = 0xC0;
			break;

		case 0x14:
			DDOP_EXP_MUST_BE_RS(exp[0]);
			DDOP_EXP_MUST_BE_RG(exp[1]);
			regclass_offset_0 = 0x100;
			break;

		case 0x21:
			DDOP_EXP_MUST_BE_RG(exp[0]);
			DDOP_EXP_MUST_BE_RP(exp[1]);
			regclass_offset_1 = 0x40;
			break;

		case 0x22:
			DDOP_EXP_MUST_BE_RG(exp[0]);
			DDOP_EXP_MUST_BE_RF(exp[1]);
			regclass_offset_1 = 0x80;
			break;

		case 0x23:
			DDOP_EXP_MUST_BE_RG(exp[0]);
			DDOP_EXP_MUST_BE_RV(exp[1]);
			regclass_offset_1 = 0xC0;
			break;

		case 0x24:
			DDOP_EXP_MUST_BE_RG(exp[0]);
			DDOP_EXP_MUST_BE_RS(exp[1]);
			regclass_offset_1 = 0x100;
			break;

		default:
			DADAO_BAD_INSN("invalid operands to opcode");
		}

		DDOP_SET_FA(opcodep, instruction->minor_opcode);
		DDOP_SET_FB(opcodep, exp[0].X_add_number - regclass_offset_0);
		DDOP_SET_FC(opcodep, exp[1].X_add_number - regclass_offset_1);
		DDOP_SET_FD(opcodep, 0);
		break;

	case dadao_operands_orrr: /* ONLY fp insns here: rb, rc, rd */
		if (n_operands != 3)
			DADAO_BAD_INSN("invalid operands to opcode");

		switch (instruction->type) {
		case dadao_type_regf:
			DDOP_EXP_MUST_BE_RF(exp[0]);
			break;

		case dadao_type_regf_fbrg:
			DDOP_EXP_MUST_BE_RG(exp[0]);
			break;

		default:
			DADAO_BAD_INSN("SHOULD NOT BE HERE");
		}

		DDOP_EXP_MUST_BE_RF(exp[1]);
		DDOP_EXP_MUST_BE_RF(exp[2]);

		DDOP_SET_FA(opcodep, instruction->minor_opcode);
		DDOP_SET_FB(opcodep, exp[0].X_add_number);
		DDOP_SET_FC(opcodep, exp[1].X_add_number);
		DDOP_SET_FD(opcodep, exp[2].X_add_number);
		break;


	case dadao_operands_rjii: /* ra, imm16  */
		if ((n_operands != 2) || (instruction->minor_opcode > 3))
			DADAO_BAD_INSN("invalid operands to opcode");

		DDOP_EXP_MUST_BE_RG(exp[0]);
		DDOP_EXP_MUST_BE_UIMM(exp[1], 16);

		DDOP_SET_FA(opcodep, exp[0].X_add_number);
		DDOP_SET_FB(opcodep, (exp[1].X_add_number >> 12) | (instruction->minor_opcode) << 4);
		DDOP_SET_FC(opcodep, (exp[1].X_add_number >> 6) & 0x3F);
		DDOP_SET_FD(opcodep, (exp[1].X_add_number & 0x3F));

		break;

	case dadao_operands_oiii: /* SWYM, TRIP, TRAP: one operands  */
		if (n_operands != 1)
			DADAO_BAD_INSN("invalid operands to opcode");

		DDOP_EXP_MUST_BE_UIMM(exp[0], 18);
		DDOP_SET_FA(opcodep, instruction->minor_opcode);
		DDOP_SET_FB(opcodep, (exp[0].X_add_number >> 12) & 63);
		DDOP_SET_FC(opcodep, (exp[0].X_add_number >> 6) & 63);
		DDOP_SET_FD(opcodep, (exp[0].X_add_number) & 63);

		break;

	case dadao_operands_orri: /* ONLY fp insns here: "rb, rc, imm6" */
		if (n_operands != 3)
			DADAO_BAD_INSN("invalid operands to opcode");

		switch (instruction->type) {
		case dadao_type_regf:
			DDOP_EXP_MUST_BE_RF(exp[0]);
			DDOP_EXP_MUST_BE_RF(exp[1]);
			break;

		case dadao_type_regf_fbrg:
			DDOP_EXP_MUST_BE_RG(exp[0]);
			DDOP_EXP_MUST_BE_RF(exp[1]);
			break;

		case dadao_type_regf_fcrg:
			DDOP_EXP_MUST_BE_RF(exp[0]);
			DDOP_EXP_MUST_BE_RG(exp[1]);
			break;

		default:
			DADAO_BAD_INSN("SHOULD NOT BE HERE");
		}

		DDOP_EXP_MUST_BE_UIMM(exp[2], 6);

		DDOP_SET_FA(opcodep, instruction->minor_opcode);
		DDOP_SET_FB(opcodep, exp[0].X_add_number);
		DDOP_SET_FC(opcodep, exp[1].X_add_number);
		DDOP_SET_FD(opcodep, exp[2].X_add_number);

		break;

	case dadao_operands_orri_orrr: /* "rb, rc, rd" or "rb, rc, imm6" */
		if (n_operands != 3)
			DADAO_BAD_INSN("invalid operands to opcode");

		DDOP_EXP_MUST_BE_RG(exp[0]);
		DDOP_EXP_MUST_BE_RG(exp[1]);

		DDOP_SET_FA(opcodep, instruction->minor_opcode);
		DDOP_SET_FB(opcodep, exp[0].X_add_number);
		DDOP_SET_FC(opcodep, exp[1].X_add_number);

		switch (exp[2].X_op) {
		case O_register: /* reg */
			DDOP_EXP_MUST_BE_RG(exp[2]);
			DDOP_SET_FD(opcodep, exp[2].X_add_number);
			break;

		case O_constant: /* imm6 */
			DDOP_SET_INSN_ALTMODE(opcodep);
			DDOP_EXP_MUST_BE_UIMM(exp[2], 6);
			DDOP_SET_FD(opcodep, exp[2].X_add_number);
			break;

		default:
			DADAO_BAD_INSN("invalid operands to opcode");
		}
		break;

	case dadao_operands_rrii_rrri:
		if (n_operands != 3)
			DADAO_BAD_INSN("invalid operands to opcode");

		switch (instruction->type) {
		case dadao_type_uimm:
			flag_imm_signd = 0;
			DDOP_EXP_MUST_BE_RG(exp[0]);
			DDOP_EXP_MUST_BE_RG(exp[1]);
			break;

		case dadao_type_simm:
			flag_imm_signd = 1;
			DDOP_EXP_MUST_BE_RG(exp[0]);
			DDOP_EXP_MUST_BE_RG(exp[1]);
			break;

		case dadao_type_regp:
		case dadao_type_regp_dref:
			flag_imm_signd = 1;
			DDOP_EXP_MUST_BE_RP(exp[0]);
			DDOP_EXP_MUST_BE_RP(exp[1]);
			break;

		case dadao_type_regf_dref:
			flag_imm_signd = 1;
			DDOP_EXP_MUST_BE_RF(exp[0]);
			DDOP_EXP_MUST_BE_RP(exp[1]);
			break;

		case dadao_type_dref:
			flag_imm_signd = 1;
			DDOP_EXP_MUST_BE_RG(exp[0]);
			DDOP_EXP_MUST_BE_RP(exp[1]);
			break;

		default:
			DADAO_BAD_INSN("SHOULD NOT BE HERE");
		}

		DDOP_SET_FA(opcodep, exp[0].X_add_number);
		DDOP_SET_FB(opcodep, exp[1].X_add_number);

		switch (exp[2].X_op) {
		case O_register: /* reg */
			DDOP_EXP_MUST_BE_RG(exp[2]);
			DDOP_SET_FC(opcodep, exp[2].X_add_number);
			DDOP_SET_FD(opcodep, 0);
			break;

		case O_left_shift: /* reg << shift6 */
			exp_left = symbol_get_value_expression (exp[2].X_add_symbol);
			exp_right = symbol_get_value_expression (exp[2].X_op_symbol);

			DDOP_EXP_MUST_BE_RG(exp_left[0]);
			DDOP_EXP_MUST_BE_UIMM(exp_right[0], 6);

			DDOP_SET_FC(opcodep, exp_left->X_add_number);
			DDOP_SET_FD(opcodep, exp_right->X_add_number);
			break;

		case O_constant: /* imm12 */
			DDOP_SET_INSN_ALTMODE(opcodep);

			if (flag_imm_signd)	DDOP_EXP_MUST_BE_SIMM(exp[2], 12);
			else			DDOP_EXP_MUST_BE_UIMM(exp[2], 12);

			DDOP_SET_FC(opcodep, (exp[2].X_add_number) >> 6);
			DDOP_SET_FD(opcodep, (exp[2].X_add_number) & 0x3F);
			break;

		default:
			DADAO_BAD_INSN("invalid operands to opcode");
		}
		break;

	case dadao_operands_rrrr:
		if (n_operands != 4)
			DADAO_BAD_INSN("invalid operands to opcode");

		DDOP_EXP_MUST_BE_RG(exp[0]);
		DDOP_EXP_MUST_BE_RG(exp[1]);
		DDOP_EXP_MUST_BE_RG(exp[2]);
		DDOP_EXP_MUST_BE_RG(exp[3]);

		DDOP_SET_FA(opcodep, exp[0].X_add_number);
		DDOP_SET_FB(opcodep, exp[1].X_add_number);
		DDOP_SET_FC(opcodep, exp[2].X_add_number);
		DDOP_SET_FD(opcodep, exp[3].X_add_number);
		break;

	case dadao_operands_riii: /* operand "ra, imm18" */
		if ((n_operands != 2) || (exp[1].X_op == O_register))
			DADAO_BAD_INSN("invalid operands to opcode");

		switch (instruction->type) {
		case dadao_type_condbranch:
			DDOP_EXP_MUST_BE_RG(exp[0]);
			DDOP_SET_FA(opcodep, exp[0].X_add_number);

			if (! expand_op)
				fix_new_exp (opc_fragP, opcodep - opc_fragP->fr_literal, 4, exp + 1, 1, BFD_RELOC_DADAO_BRCC);
			else
				frag_var (rs_machine_dependent, DD_INSN_BYTES(5), 0, ENCODE_RELAX (STATE_BRCC, STATE_UNDF),
					exp[1].X_add_symbol, exp[1].X_add_number, opcodep);
			break;

		case dadao_type_geta:
			DDOP_EXP_MUST_BE_RG(exp[0]);
			DDOP_SET_FA(opcodep, exp[0].X_add_number);

			if (! expand_op)
				fix_new_exp (opc_fragP, opcodep - opc_fragP->fr_literal, 4, exp + 1, 1, BFD_RELOC_DADAO_GETA);
			else
				frag_var (rs_machine_dependent, DD_INSN_BYTES(3), 0, ENCODE_RELAX (STATE_GETA, STATE_UNDF),
					exp[1].X_add_symbol, exp[1].X_add_number, opcodep);
			break;

		default:
			DADAO_BAD_INSN("SHOULD NOT BE HERE");
		}
		break;

	case dadao_operands_iiii_rrii: /* ONLY call or jump be here */
		if ((n_operands != 1) && (n_operands != 3))
			DADAO_BAD_INSN("invalid operands to opcode");

		if (n_operands == 1) {
			if (exp[0].X_op == O_register)
				DADAO_BAD_INSN("invalid operands to opcode");

			/* The last operand is imm24 whenever we see just two operands.  */
			DDOP_SET_INSN_ALTMODE(opcodep);

			switch (instruction->type) {
			case dadao_type_branch:
				if (! expand_op)
					fix_new_exp (opc_fragP, opcodep - opc_fragP->fr_literal, 4, exp + 1, 1, BFD_RELOC_DADAO_JUMP);
				else
					frag_var (rs_machine_dependent, DD_INSN_BYTES(4), 0, ENCODE_RELAX (STATE_JUMP, STATE_UNDF),
						exp[0].X_add_symbol, exp[0].X_add_number, opcodep);
				break;

			case dadao_type_jsr:
				if (! expand_op)
					fix_new_exp (opc_fragP, opcodep - opc_fragP->fr_literal, 4, exp + 1, 1, BFD_RELOC_DADAO_CALL);
				else
					frag_var (rs_machine_dependent, DD_INSN_BYTES(4), 0, ENCODE_RELAX (STATE_CALL, STATE_UNDF),
						exp[0].X_add_symbol, exp[0].X_add_number, opcodep);
				break;

			default:
				DADAO_BAD_INSN("SHOULD NOT BE HERE");
			}

		} else {	/* (n_operands == 3) */
			DDOP_EXP_MUST_BE_RG(exp[0]);
			DDOP_EXP_MUST_BE_RP(exp[1]);
			DDOP_EXP_MUST_BE_UIMM(exp[2], 12);

			DDOP_SET_FA(opcodep, exp[0].X_add_number);
			DDOP_SET_FB(opcodep, exp[1].X_add_number);
			DDOP_SET_FC(opcodep, ((exp[2].X_add_number) >> 6) & 0x3F);
			DDOP_SET_FD(opcodep, (exp[2].X_add_number) & 0x3F);
		}

		break;

	case dadao_operands_o000: /* nop / ret */
		if (n_operands != 0)
			DADAO_BAD_INSN("invalid operands to opcode");

		if (instruction->minor_opcode == 0x37) {	/* ret */
			DDOP_SET_FA(opcodep, instruction->minor_opcode);
			DDOP_SET_FB(opcodep, 0);
			DDOP_SET_FC(opcodep, 0);
			DDOP_SET_FD(opcodep, 0);
			break;
		}

		if (instruction->minor_opcode == 0x36) {	/* nop */
			md_number_to_chars(opcodep, 0xDADADADA, 4);
			break;
		}

		/* FALLTHROUGH.  */
	default:
		DADAO_BAD_INSN("unknown instruction operands");
	}
}

/* Set fragP->fr_var to the initial guess of the size of a relaxable insn
   and return it.  Sizes of other instructions are not known.  This
   function may be called multiple times.  */

int
md_estimate_size_before_relax (fragS *fragP, segT segment)
{
  int length;

#define HANDLE_RELAXABLE(state)						\
 case ENCODE_RELAX (state, STATE_UNDF):					\
   if (fragP->fr_symbol != NULL						\
       && S_GET_SEGMENT (fragP->fr_symbol) == segment			\
       && !S_IS_WEAK (fragP->fr_symbol))				\
     {									\
       /* The symbol lies in the same segment - a relaxable case.  */	\
       fragP->fr_subtype						\
	 = ENCODE_RELAX (state, STATE_ZERO);				\
     }

  switch (fragP->fr_subtype)
    {
      HANDLE_RELAXABLE (STATE_GETA);
	break;
      HANDLE_RELAXABLE (STATE_BRCC);
	break;
      HANDLE_RELAXABLE (STATE_JUMP);
	break;
      HANDLE_RELAXABLE (STATE_CALL);
	break;

    case ENCODE_RELAX (STATE_CALL, STATE_ZERO):
    case ENCODE_RELAX (STATE_GETA, STATE_ZERO):
    case ENCODE_RELAX (STATE_BRCC, STATE_ZERO):
    case ENCODE_RELAX (STATE_JUMP, STATE_ZERO):
      /* When relaxing a section for the second time, we don't need to do
	 anything except making sure that fr_var is set right.  */
      break;

    default:
      BAD_CASE (fragP->fr_subtype);
    }

  length = dadao_relax_table[fragP->fr_subtype].rlx_length;
  fragP->fr_var = length;

  return length;
}

/* Turn a string in input_line_pointer into a floating point constant of type
   type, and store the appropriate bytes in *litP.  The number of LITTLENUMS
   emitted is stored in *sizeP .  An error message is returned, or NULL on
   OK.  */

const char *
md_atof (int type, char *litP, int *sizeP)
{
  if (type == 'r')
    type = 'f';
  /* FIXME: Having 'f' in FLT_CHARS (and here) makes it
     problematic to also have a forward reference in an expression.
     The testsuite wants it, and it's customary.
     We'll deal with the real problems when they come; we share the
     problem with most other ports.  */
  return ieee_md_atof (type, litP, sizeP, TRUE);
}

/* Convert variable-sized frags into one or more fixups.  */

void
md_convert_frag (bfd *abfd ATTRIBUTE_UNUSED, segT sec ATTRIBUTE_UNUSED,
		 fragS *fragP)
{
  /* Pointer to first byte in variable-sized part of the frag.  */
  char *var_partp;

  /* Pointer to first opcode byte in frag.  */
  char *opcodep;

  /* Size in bytes of variable-sized part of frag.  */
  int var_part_size = 0;

  /* This is part of *fragP.  It contains all information about addresses
     and offsets to varying parts.  */
  symbolS *symbolP;
  unsigned long var_part_offset;

  /* This is the frag for the opcode.  It, rather than fragP, must be used
     when emitting a frag for the opcode.  */
  fixS *tmpfixP;

  /* Where, in file space, does addr point?  */
  bfd_vma target_address;
  bfd_vma opcode_address;

  know (fragP->fr_type == rs_machine_dependent);

  var_part_offset = fragP->fr_fix;
  var_partp = fragP->fr_literal + var_part_offset;
  opcodep = fragP->fr_opcode;

  symbolP = fragP->fr_symbol;

  target_address
    = ((symbolP ? S_GET_VALUE (symbolP) : 0) + fragP->fr_offset);

  /* The opcode that would be extended is the last four "fixed" bytes.  */
  opcode_address = fragP->fr_address + fragP->fr_fix;

	switch (fragP->fr_subtype) {
	case ENCODE_RELAX (STATE_CALL, STATE_ZERO):
	case ENCODE_RELAX (STATE_JUMP, STATE_ZERO):
		dd_set_addr_offset(opcodep, target_address - opcode_address, 24, 1);
		var_part_size = 0;
		break;

	case ENCODE_RELAX (STATE_GETA, STATE_ZERO):
		dd_set_addr_offset(opcodep, target_address - opcode_address, 18, 0);
		var_part_size = 0;
		break;

	case ENCODE_RELAX (STATE_BRCC, STATE_ZERO):
		dd_set_addr_offset(opcodep, target_address - opcode_address, 18, 1);
		var_part_size = 0;
		break;

#define HANDLE_MAX_RELOC(state, reloc)					\
  case ENCODE_RELAX (state, STATE_MAX):					\
    var_part_size							\
      = dadao_relax_table[ENCODE_RELAX (state, STATE_MAX)].rlx_length;	\
    dd_fill_nops (var_partp, var_part_size / 4);			\
    tmpfixP = fix_new (fragP, var_partp - fragP->fr_literal, 8,		\
		       fragP->fr_symbol, fragP->fr_offset, 1, reloc);	\
    tmpfixP->fx_file = fragP->fr_file;					\
    tmpfixP->fx_line = fragP->fr_line;					\
    break

      HANDLE_MAX_RELOC (STATE_GETA, BFD_RELOC_DADAO_GETA);
      HANDLE_MAX_RELOC (STATE_BRCC, BFD_RELOC_DADAO_BRCC);
      HANDLE_MAX_RELOC (STATE_CALL, BFD_RELOC_DADAO_CALL);
      HANDLE_MAX_RELOC (STATE_JUMP, BFD_RELOC_DADAO_JUMP);

    default:
      BAD_CASE (fragP->fr_subtype);
      break;
    }

  fragP->fr_fix += var_part_size;
  fragP->fr_var = 0;
}

/* Applies the desired value to the specified location.
   Also sets up addends for RELA type relocations.
   Stolen from tc-mcore.c. */

void
md_apply_fix (fixS *fixP, valueT *valP, segT segment)
{
  char *buf  = fixP->fx_where + fixP->fx_frag->fr_literal;
  /* Note: use offsetT because it is signed, valueT is unsigned.  */
  offsetT val  = (offsetT) * valP;
  segT symsec
    = (fixP->fx_addsy == NULL
       ? absolute_section : S_GET_SEGMENT (fixP->fx_addsy));

  /* If the fix is relative to a symbol which is not defined, or, (if
     pcrel), not in the same segment as the fix, we cannot resolve it
     here.  */
  if (fixP->fx_addsy != NULL
      && (! S_IS_DEFINED (fixP->fx_addsy)
	  || S_IS_WEAK (fixP->fx_addsy)
	  || (fixP->fx_pcrel && symsec != segment)
	  || (! fixP->fx_pcrel
	      && symsec != absolute_section)))
    {
      fixP->fx_done = 0;
      return;
    }
  else if (fixP->fx_r_type == BFD_RELOC_VTABLE_INHERIT
	   || fixP->fx_r_type == BFD_RELOC_VTABLE_ENTRY)
    {
      /* These are never "fixed".  */
      fixP->fx_done = 0;
      return;
    }
  else
    /* We assume every other relocation is "fixed".  */
    fixP->fx_done = 1;

  switch (fixP->fx_r_type)
    {
    case BFD_RELOC_64:
    case BFD_RELOC_32:
    case BFD_RELOC_24:
    case BFD_RELOC_16:
    case BFD_RELOC_8:
    case BFD_RELOC_64_PCREL:
    case BFD_RELOC_32_PCREL:
    case BFD_RELOC_24_PCREL:
    case BFD_RELOC_16_PCREL:
    case BFD_RELOC_8_PCREL:
      md_number_to_chars (buf, val, fixP->fx_size);
      break;

	case BFD_RELOC_DADAO_GETA:
		dd_set_addr_offset(buf, val, 18, 0);
		break;

	case BFD_RELOC_DADAO_BRCC:
		dd_set_addr_offset(buf, val, 18, 1);
		break;

	case BFD_RELOC_DADAO_CALL:
	case BFD_RELOC_DADAO_JUMP:
		dd_set_addr_offset(buf, val, 24, 1);
		break;

    default:
      BAD_CASE (fixP->fx_r_type);
      break;
    }

  if (fixP->fx_done)
    /* Make sure that for completed fixups we have the value around for
       use by e.g. dadao_frob_file.  */
    fixP->fx_offset = val;
}

/* Generate a machine-dependent relocation.  */

arelent *
tc_gen_reloc (asection *section ATTRIBUTE_UNUSED, fixS *fixP)
{
  bfd_signed_vma val
    = fixP->fx_offset
    + (fixP->fx_addsy != NULL
       && !S_IS_WEAK (fixP->fx_addsy)
       && !S_IS_COMMON (fixP->fx_addsy)
       ? S_GET_VALUE (fixP->fx_addsy) : 0);
  arelent *relP;
  bfd_reloc_code_real_type code = BFD_RELOC_NONE;
  char *buf  = fixP->fx_where + fixP->fx_frag->fr_literal;
  symbolS *addsy = fixP->fx_addsy;
  asection *addsec = addsy == NULL ? NULL : S_GET_SEGMENT (addsy);
  asymbol *baddsy = addsy != NULL ? symbol_get_bfdsym (addsy) : NULL;
  bfd_vma addend
    = val - (baddsy == NULL || S_IS_COMMON (addsy) || S_IS_WEAK (addsy)
	     ? 0 : bfd_asymbol_value (baddsy));

  /* FIXME: Range tests for all these.  */
  switch (fixP->fx_r_type)
    {
    case BFD_RELOC_64:
    case BFD_RELOC_32:
    case BFD_RELOC_24:
    case BFD_RELOC_16:
    case BFD_RELOC_8:
      code = fixP->fx_r_type;

      if (addsy == NULL || bfd_is_abs_section (addsec))
	{
	  /* Resolve this reloc now, as md_apply_fix would have done.
	     There is no point in keeping a reloc
	     to an absolute symbol.  No reloc that is subject to
	     relaxation must be to an absolute symbol; difference
	     involving symbols in a specific section must be signalled as
	     an error if the relaxing cannot be expressed; having a reloc
	     to the resolved (now absolute) value does not help.  */
	  md_number_to_chars (buf, val, fixP->fx_size);
	  return NULL;
	}
      break;

    case BFD_RELOC_64_PCREL:
    case BFD_RELOC_32_PCREL:
    case BFD_RELOC_24_PCREL:
    case BFD_RELOC_16_PCREL:
    case BFD_RELOC_8_PCREL:
    case BFD_RELOC_VTABLE_INHERIT:
    case BFD_RELOC_VTABLE_ENTRY:
    case BFD_RELOC_DADAO_GETA:
    case BFD_RELOC_DADAO_BRCC:
    case BFD_RELOC_DADAO_CALL:
    case BFD_RELOC_DADAO_JUMP:
      code = fixP->fx_r_type;
      break;

    default:
      as_bad_where
	(fixP->fx_file, fixP->fx_line,
	 _("operands were not reducible at assembly-time"));

      /* Unmark this symbol as used in a reloc, so we don't bump into a BFD
	 assert when trying to output reg_section.  FIXME: A gas bug.  */
      fixP->fx_addsy = NULL;
      return NULL;
    }

  relP = XNEW (arelent);
  gas_assert (relP != 0);
  relP->sym_ptr_ptr = XNEW (asymbol *);
  *relP->sym_ptr_ptr = baddsy;
  relP->address = fixP->fx_frag->fr_address + fixP->fx_where;

  relP->addend = addend;

  /* If this had been a.out, we would have had a kludge for weak symbols
     here.  */

  relP->howto = bfd_reloc_type_lookup (stdoutput, code);
  if (! relP->howto)
    {
      const char *name;

      name = S_GET_NAME (addsy);
      if (name == NULL)
	name = _("<unknown>");
      as_fatal (_("cannot generate relocation type for symbol %s, code %s"),
		name, bfd_get_reloc_code_name (code));
    }

  return relP;
}

/* See whether we need to force a relocation into the output file.
   This is used to force out switch and PC relative relocations when
   relaxing.  */

int
dadao_force_relocation (fixS *fixP)
{
  /* All our pcrel relocations are must-keep.  Note that md_apply_fix is
     called *after* this, and will handle getting rid of the presumed
     reloc; a relocation isn't *forced* other than to be handled by
     md_apply_fix.  */
  if (fixP->fx_pcrel)
    return 1;

  return generic_force_reloc (fixP);
}

/* The location from which a PC relative jump should be calculated,
   given a PC relative reloc.  */

long
md_pcrel_from_section (fixS *fixP, segT sec)
{
  if (fixP->fx_addsy != (symbolS *) NULL
      && (! S_IS_DEFINED (fixP->fx_addsy)
	  || S_GET_SEGMENT (fixP->fx_addsy) != sec))
    {
      /* The symbol is undefined (or is defined but not in this section).
	 Let the linker figure it out.  */
      return 0;
    }

  return (fixP->fx_frag->fr_address + fixP->fx_where);
}
