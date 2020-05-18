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

/* Something to describe what we need to do with a fixup before output,
   for example assert something of what it became or make a relocation.  */

enum dadao_fixup_action
{
  dadao_fixup_byte,
  dadao_fixup_register,
  dadao_fixup_register_or_adjust_for_byte
};

static int get_spec_regno (char *);
static int get_operands (int, char *, expressionS *);
static int get_putget_operands (struct dadao_opcode *, char *, expressionS *);
static void s_greg (int);
static void dadao_greg_internal (char *);
static void dadao_set_geta_branch_offset (char *, offsetT);
static void dadao_set_jmp_offset (char *, offsetT);
static void dadao_fill_nops (char *, int);
static int cmp_greg_symbol_fixes (const void *, const void *);
static int cmp_greg_val_greg_symbol_fixes (const void *, const void *);

/* Copy the location of a frag to a fix.  */
#define COPY_FR_WHERE_TO_FX(FRAG, FIX)		\
 do						\
   {						\
     (FIX)->fx_file = (FRAG)->fr_file;		\
     (FIX)->fx_line = (FRAG)->fr_line;		\
   }						\
 while (0)

const char *md_shortopts = "x";

static bfd_vma lowest_text_loc = (bfd_vma) -1;
static int text_has_contents = 0;

/* The alignment of the previous instruction, and a boolean for whether we
   want to avoid aligning the next WYDE, TETRA, OCTA or insn.  */
static int last_alignment = 0;

static bfd_vma lowest_data_loc = (bfd_vma) -1;
static int data_has_contents = 0;

/* The fragS of the instruction being assembled.  Only valid from within
   md_assemble.  */
fragS *dadao_opcode_frag = NULL;

/* Raw GREGs as appearing in input.  These may be fewer than the number
   after relaxing.  */
static int n_of_raw_gregs = 0;
static struct
 {
   char *label;
   expressionS exp;
 } dadao_raw_gregs[MAX_GREGS];

static struct loc_assert_s
 {
   segT old_seg;
   symbolS *loc_sym;
   fragS *frag;
   struct loc_assert_s *next;
 } *loc_asserts = NULL;

/* Fixups for all unique GREG registers.  We store the fixups here in
   md_convert_frag, then we use the array to convert
   BFD_RELOC_DADAO_BASE_PLUS_OFFSET fixups in tc_gen_reloc.  The index is
   just a running number and is not supposed to be correlated to a
   register number.  */
static fixS *dadao_gregs[MAX_GREGS];
static int n_of_cooked_gregs = 0;

/* Pointing to the register section we use for output.  */
static asection *real_reg_section;

/* For each symbol; unknown or section symbol, we keep a list of GREG
   definitions sorted on increasing offset.  It seems no use keeping count
   to allocate less room than the maximum number of gregs when we've found
   one for a section or symbol.  */
struct dadao_symbol_gregs
 {
   int n_gregs;
   struct dadao_symbol_greg_fixes
   {
     fixS *fix;

     /* A signed type, since we may have GREGs pointing slightly before the
	contents of a section.  */
     offsetT offs;
   } greg_fixes[MAX_GREGS];
 };

/* Should we automatically expand instructions into multiple insns in
   order to generate working code?  */
static int expand_op = 1;

/* Should we warn when expanding operands?  FIXME: test-cases for when -x
   is absent.  */
static int warn_on_expansion = 1;

/* Should we merge non-zero GREG register definitions?  */
static int merge_gregs = 1;

/* Should we pass on undefined BFD_RELOC_DADAO_BASE_PLUS_OFFSET relocs
   (missing suitable GREG definitions) to the linker?  */
static int allocate_undefined_gregs_in_linker = 0;

/* Should we emit built-in symbols?  */
static int predefined_syms = 1;

/* Should we allow anything but the listed special register name
   (e.g. equated symbols)?  */
static int equated_spec_regs = 1;

struct option md_longopts[] =
 {
#define OPTION_RELAX  (OPTION_MD_BASE)
#define OPTION_NOEXPAND  (OPTION_RELAX + 1)
#define OPTION_NOMERGEGREG  (OPTION_NOEXPAND + 1)
#define OPTION_NOSYMS  (OPTION_NOMERGEGREG + 1)
#define OPTION_FIXED_SPEC_REGS  (OPTION_NOSYMS + 1)
#define OPTION_LINKER_ALLOCATED_GREGS  (OPTION_FIXED_SPEC_REGS + 1)
   {"linkrelax", no_argument, NULL, OPTION_RELAX},
   {"no-expand", no_argument, NULL, OPTION_NOEXPAND},
   {"no-merge-gregs", no_argument, NULL, OPTION_NOMERGEGREG},
   {"no-predefined-syms", no_argument, NULL, OPTION_NOSYMS},
   {"fixed-special-register-names", no_argument, NULL,
    OPTION_FIXED_SPEC_REGS},
   {"linker-allocated-gregs", no_argument, NULL,
    OPTION_LINKER_ALLOCATED_GREGS},
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

   2. Bcc
      extra length: zero or five insns.

   4. JMP
      extra length: zero or four insns.

   5. GREG
      special handling, allocates a named global register unless another
      is within reach for all uses.
 */

#define STATE_GETA	(1)
#define STATE_BCC	(2)
#define STATE_JMP	(3)
#define STATE_GREG	(4)

/* No fine-grainedness here.  */
#define STATE_LENGTH_MASK	    (1)

#define STATE_ZERO		    (0)
#define STATE_MAX		    (1)

/* More descriptive name for convenience.  */
/* FIXME: We should start on something different, not MAX.  */
#define STATE_UNDF		    STATE_MAX

/* FIXME: For GREG, we must have other definitions; UNDF == MAX isn't
   appropriate; we need it the other way round.  This value together with
   fragP->tc_frag_data shows what state the frag is in: tc_frag_data
   non-NULL means 0, NULL means 8 bytes.  */
#define STATE_GREG_UNDF ENCODE_RELAX (STATE_GREG, STATE_ZERO)
#define STATE_GREG_DEF ENCODE_RELAX (STATE_GREG, STATE_MAX)

/* These displacements are relative to the address following the opcode
   word of the instruction.  The catch-all states have zero for "reach"
   and "next" entries.  */

#define GETA_0F (65536 * 4 - 8)
#define GETA_0B (-65536 * 4 - 4)

#define GETA_MAX_LEN 4 * 4
#define GETA_3F 0
#define GETA_3B 0

#define BCC_0F GETA_0F
#define BCC_0B GETA_0B

#define BCC_MAX_LEN 6 * 4
#define BCC_5F GETA_3F
#define BCC_5B GETA_3B

#define JMP_0F (65536 * 256 * 4 - 8)
#define JMP_0B (-65536 * 256 * 4 - 4)

#define JMP_MAX_LEN 5 * 4
#define JMP_4F 0
#define JMP_4B 0

#define RELAX_ENCODE_SHIFT 1
#define ENCODE_RELAX(what, length) (((what) << RELAX_ENCODE_SHIFT) + (length))

const relax_typeS dadao_relax_table[] =
 {
   /* Error sentinel (0, 0).  */
   {1,		1,		0,	0},

   /* Unused (0, 1).  */
   {1,		1,		0,	0},

   /* GETA (1, 0).  */
   {GETA_0F,	GETA_0B,	0,	ENCODE_RELAX (STATE_GETA, STATE_MAX)},

   /* GETA (1, 1).  */
   {GETA_3F,	GETA_3B,
		GETA_MAX_LEN - 4,	0},

   /* BCC (2, 0).  */
   {BCC_0F,	BCC_0B,		0,	ENCODE_RELAX (STATE_BCC, STATE_MAX)},

   /* BCC (2, 1).  */
   {BCC_5F,	BCC_5B,
		BCC_MAX_LEN - 4,	0},

   /* JMP (3, 0).  */
   {JMP_0F,	JMP_0B,		0,	ENCODE_RELAX (STATE_JMP, STATE_MAX)},

   /* JMP (3, 1).  */
   {JMP_4F,	JMP_4B,
		JMP_MAX_LEN - 4,	0},

   /* GREG (5, 0), (5, 1), though the table entry isn't used.  */
   {0, 0, 0, 0}, {0, 0, 0, 0},
};

const pseudo_typeS md_pseudo_table[] = {
	{"dd.byte", cons, 1},
	{"dd.wyde", cons, 2},
	{"dd.tetra", cons, 4},
	{"dd.octa", cons, 8},

   /* Support " .greg sym,expr" syntax.  */
   {"greg", s_greg, 0},

   {NULL, 0, 0}
 };

const char comment_chars[] = "";
const char line_comment_chars[] = "#";
const char line_separator_chars[] = ";";

const char EXP_CHARS[] = "eE";

const char FLT_CHARS[] = "rf";


/* Fill in the offset-related part of GETA or Bcc.  */

static void
dadao_set_geta_branch_offset (char *opcodep, offsetT value)
{
  if (value < 0)
    {
      value += 65536 * 4;
      opcodep[0] |= 1;
    }

  value /= 4;
  md_number_to_chars (opcodep + 2, value, 2);
}

/* Fill in the offset-related part of JMP.  */

static void
dadao_set_jmp_offset (char *opcodep, offsetT value)
{
  if (value < 0)
    {
      value += 65536 * 256 * 4;
      opcodep[0] |= 1;
    }

  value /= 4;
  md_number_to_chars (opcodep + 1, value, 3);
}

/* Fill in NOP:s for the expanded part of GETA/JMP/Bcc.  */

static void
dadao_fill_nops (char *opcodep, int n)
{
  int i;

  for (i = 0; i < n; i++)
    md_number_to_chars (opcodep + i * 4, 0xDADADADA, 4);
}

/* Get up to three operands, filling them into the exp array.
   General idea and code stolen from the tic80 port.  */

static int
get_operands (int max_operands, char *s, expressionS *exp)
{
  char *p = s;
  int numexp = 0;
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
	  as_bad (_("invalid operands"));
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

/* Get the value of a special register, or -1 if the name does not match
   one.  NAME is a null-terminated string.  */

static int
get_spec_regno (char *name)
{
  int i;

  if (name == NULL)
    return -1;

  /* Well, it's a short array and we'll most often just match the first
     entry, rJ.  */
  for (i = 0; dadao_spec_regs[i].name != NULL; i++)
    if (strcmp (name, dadao_spec_regs[i].name) == 0)
      return dadao_spec_regs[i].number;

  return -1;
}

/* For GET and PUT, parse the register names "manually", so we don't use
   user labels.  */
static int
get_putget_operands (struct dadao_opcode *insn, char *operands,
		     expressionS *exp)
{
  expressionS *expp_reg;
  expressionS *expp_sreg;
  char *sregp = NULL;
  char *sregend = operands;
  char *p = operands;
  char c = *sregend;
  int regno;

  /* Skip leading whitespace */
  while (*p == ' ' || *p == '\t')
    p++;

  input_line_pointer = p;

  /* Initialize both possible operands to error state, in case we never
     get further.  */
  exp[0].X_op = O_illegal;
  exp[1].X_op = O_illegal;

  if (insn->operands == dadao_operands_or0r_get)
    {
      expp_reg = &exp[0];
      expp_sreg = &exp[1];

      expression (expp_reg);

      p = input_line_pointer;

      /* Skip whitespace */
      while (*p == ' ' || *p == '\t')
	p++;

      if (*p == ',')
	{
	  p++;

	  /* Skip whitespace */
	  while (*p == ' ' || *p == '\t')
	    p++;
	  sregp = p;
	  input_line_pointer = sregp;
	  c = get_symbol_name (&sregp);
	  sregend = input_line_pointer;
	  if (c == '"')
	    ++ input_line_pointer;
	}
    }
  else
    {
      expp_sreg = &exp[0];
      expp_reg = &exp[1];

      c = get_symbol_name (&sregp);
      sregend = input_line_pointer;
      restore_line_pointer (c);
      p = input_line_pointer;

      /* Skip whitespace */
      while (*p == ' ' || *p == '\t')
	p++;

      if (*p == ',')
	{
	  p++;

	  /* Skip whitespace */
	  while (*p == ' ' || *p == '\t')
	    p++;

	  input_line_pointer = p;
	  expression (expp_reg);
	}
      *sregend = 0;
    }

  regno = get_spec_regno (sregp);
  *sregend = c;

  /* Let the caller issue errors; we've made sure the operands are
     invalid.  */
  if (expp_reg->X_op != O_illegal
      && expp_reg->X_op != O_absent
      && regno != -1)
    {
      expp_sreg->X_op = O_register;
      expp_sreg->X_add_number = regno + 256;
    }

  return 2;
}

/* Handle DADAO-specific option.  */

int
md_parse_option (int c, const char *arg ATTRIBUTE_UNUSED)
{
  switch (c)
    {
    case 'x':
      warn_on_expansion = 0;
      allocate_undefined_gregs_in_linker = 1;
      break;

    case OPTION_RELAX:
      linkrelax = 1;
      break;

    case OPTION_NOEXPAND:
      expand_op = 0;
      break;

    case OPTION_NOMERGEGREG:
      merge_gregs = 0;
      break;

    case OPTION_NOSYMS:
      predefined_syms = 0;
      equated_spec_regs = 0;
      break;

    case OPTION_FIXED_SPEC_REGS:
      equated_spec_regs = 0;
      break;

    case OPTION_LINKER_ALLOCATED_GREGS:
      allocate_undefined_gregs_in_linker = 1;
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
  -fixed-special-register-names\n\
                          Allow only the original special register names.\n"));
  fprintf (stream, _("\
  -relax                  Create linker relaxable code.\n"));
  fprintf (stream, _("\
  -no-predefined-syms     Do not provide dadaoal built-in constants.\n\
                          Implies -fixed-special-register-names.\n"));
  fprintf (stream, _("\
  -no-expand              Do not expand GETA, branches or JUMP\n\
                          into multiple instructions.\n"));
  fprintf (stream, _("\
  -no-merge-gregs         Do not merge GREG definitions with nearby values.\n"));
  fprintf (stream, _("\
  -linker-allocated-gregs If there's no suitable GREG definition for the\n\
                          operands of an instruction, let the linker resolve.\n"));
  fprintf (stream, _("\
  -x                      Do not warn when an operand to GETA, a branch,\n\
                          or JUMP is not known to be within range.\n\
                          The linker will catch any errors.  Implies\n\
                          -linker-allocated-gregs."));
}

/* Initialize GAS DADAO specifics.  */
void dadao_md_begin (void)
{
  int i;
  const struct dadao_opcode *opcode;

  dadao_opcode_hash = hash_new ();

  real_reg_section
    = bfd_make_section_old_way (stdoutput, DADAO_REG_SECTION_NAME);

  for (opcode = dadao_opcodes; opcode->name; opcode++)
    hash_insert (dadao_opcode_hash, opcode->name, (char *) opcode);

  /* We always insert the ordinary registers 0..255 as registers.  */
  for (i = 0; i < 256; i++)
    {
      char buf[5];

      /* Alternatively, we could diddle with '$' and the following number,
	 but keeping the registers as symbols helps keep parsing simple.  */
      sprintf (buf, "$%d", i);
      symbol_table_insert (symbol_new (buf, reg_section, i,
				       &zero_address_frag));
    }

  /* Insert dadaoal built-in names if allowed.  */
  if (predefined_syms)
    {
      for (i = 0; dadao_spec_regs[i].name != NULL; i++)
	symbol_table_insert (symbol_new (dadao_spec_regs[i].name,
					 reg_section,
					 dadao_spec_regs[i].number + 256,
					 &zero_address_frag));

      /* FIXME: Perhaps these should be recognized as specials; as field
	 names for those instructions.  */
      symbol_table_insert (symbol_new ("ROUND_CURRENT", reg_section, 512,
				       &zero_address_frag));
      symbol_table_insert (symbol_new ("ROUND_OFF", reg_section, 512 + 1,
				       &zero_address_frag));
      symbol_table_insert (symbol_new ("ROUND_UP", reg_section, 512 + 2,
				       &zero_address_frag));
      symbol_table_insert (symbol_new ("ROUND_DOWN", reg_section, 512 + 3,
				       &zero_address_frag));
      symbol_table_insert (symbol_new ("ROUND_NEAR", reg_section, 512 + 4,
				       &zero_address_frag));
    }
}

/* Assemble one insn in STR.  */
void dadao_md_assemble (char *str)
{
  char *operands = str;
  char modified_char = 0;
  struct dadao_opcode *instruction;
  fragS *opc_fragP = NULL;
  int max_operands = 3;

  /* Note that the struct frag member fr_literal in frags.h is char[], so
     I have to make this a plain char *.  */
  /* unsigned */ char *opcodep = NULL;

  expressionS exp[4];
  int n_operands = 0;
  expressionS *exp_left;
  expressionS *exp_right;

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

  if (now_seg == data_section)
    {
      if (lowest_data_loc != (bfd_vma) -1 && (lowest_data_loc & 3) != 0)
	{
	  if (data_has_contents)
	    as_bad (_("specified location wasn't TETRA-aligned"));

	  lowest_data_loc &= ~(bfd_vma) 3;
	  lowest_data_loc += 4;
	}

      data_has_contents = 1;
    }
  else if (now_seg == text_section)
    {
      if (lowest_text_loc != (bfd_vma) -1 && (lowest_text_loc & 3) != 0)
	{
	  if (text_has_contents)
	    as_bad (_("specified location wasn't TETRA-aligned"));

	  lowest_text_loc &= ~(bfd_vma) 3;
	  lowest_text_loc += 4;
	}

      text_has_contents = 1;
    }

  /* After a sequence of BYTEs or WYDEs, we need to get to instruction
     alignment.  For other pseudos, a ".p2align 2" is supposed to be
     inserted by the user.  */
  if (last_alignment < 2)
    {
      frag_align (2, 0, 0);
      record_alignment (now_seg, 2);
      last_alignment = 2;
    }

  /* We assume that dadao_opcodes keeps having unique mnemonics for each
     opcode, so we don't have to iterate over more than one opcode; if the
     syntax does not match, then there's a syntax error.  */

  /* Operands have little or no context and are all comma-separated; it is
     easier to parse each expression first.   */
  switch (instruction->operands)
    {
    case dadao_operands_o000:
      max_operands = 0;
      break;

	case dadao_operands_riii:
	case dadao_operands_iijr:
	case dadao_operands_or0r_get:
	case dadao_operands_or0r_put:
		max_operands = 2;
		break;

	case dadao_operands_orir:
	case dadao_operands_orir_orrr:
	case dadao_operands_orrr:
	case dadao_operands_riii_rrii:
		max_operands = 3;
		break;

	case dadao_operands_riir_rrir:
	case dadao_operands_riir_rrir_or_sym:
		max_operands = 3;
		break;

	case dadao_operands_oiii:
		max_operands = 1;
		break;

      /* The original 3 is fine for the rest.  */
    default:
      break;
    }

  /* If this is GET or PUT, and we don't do allow those names to be
     equated, we need to parse the names ourselves, so we don't pick up a
     user label instead of the special register.  */
  if (! equated_spec_regs
      && (instruction->operands == dadao_operands_or0r_get
	  || instruction->operands == dadao_operands_or0r_put))
    n_operands = get_putget_operands (instruction, operands, exp);
  else
    n_operands = get_operands (max_operands, operands, exp);

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

  md_number_to_chars (opcodep, instruction->match, 4);

  /* Handle the rest.  */
  switch (instruction->operands)
    {
	case dadao_operands_or0r_get:
		/* "$X,spec_reg"; GET.
		   Like with rounding modes, we demand that the special register or
		   symbol is already defined when we get here at the point of use.  */
		if (n_operands != 2)
			as_fatal (_("invalid operands to opcode %s: `%s'"), instruction->name, operands);

		DDOP_EXP_MUST_BE_REG(exp[0]);

		DDOP_SET_FA(opcodep, instruction->fa_as_opcode);
		DDOP_SET_FB(opcodep, exp[1].X_add_number - 256);
		DDOP_SET_FC(opcodep, 0);
		DDOP_SET_FD(opcodep, exp[0].X_add_number);
		break;

	case dadao_operands_or0r_put:
		/* "spec_reg,$Z"; PUT.  */
		if (n_operands != 2)
			as_fatal (_("invalid operands to opcode %s: `%s'"), instruction->name, operands);

		DDOP_EXP_MUST_BE_REG(exp[1]);
		DDOP_SET_FA(opcodep, instruction->fa_as_opcode);
		DDOP_SET_FB(opcodep, exp[1].X_add_number);
		DDOP_SET_FC(opcodep, 0);
		DDOP_SET_FD(opcodep, exp[0].X_add_number - 256);
		break;

	case dadao_operands_orrr: /* regd, regb, regc */
		if (n_operands != 3)
			as_fatal (_("invalid operands to opcode %s: `%s'"), instruction->name, operands);

		DDOP_EXP_MUST_BE_REG(exp[0]);
		DDOP_EXP_MUST_BE_REG(exp[1]);
		DDOP_EXP_MUST_BE_REG(exp[2]);

		DDOP_SET_FA(opcodep, instruction->fa_as_opcode);
		DDOP_SET_FB(opcodep, exp[1].X_add_number);
		DDOP_SET_FC(opcodep, exp[2].X_add_number);
		DDOP_SET_FD(opcodep, exp[0].X_add_number);
		break;


	case dadao_operands_iijr: /* regd, imm16  */
		if ((n_operands != 2) || (exp[1].X_op != O_constant))
			as_fatal (_("invalid operands to opcode %s: `%s'"), instruction->name, operands);

		DDOP_EXP_MUST_BE_REG(exp[0]);
		DDOP_CHECK_16_BIT(exp[1].X_add_number);
		DDOP_CHECK_2_BIT(instruction->fa_as_opcode);

		DDOP_SET_FD(opcodep, exp[0].X_add_number);
		DDOP_SET_FA(opcodep, (exp[1].X_add_number >> 10) & 63);
		DDOP_SET_FB(opcodep, (exp[1].X_add_number >> 4) & 63);
		DDOP_SET_FC(opcodep, (((exp[1].X_add_number & 0xF) << 2) | (instruction->fa_as_opcode)));

		break;

	case dadao_operands_oiii: /* SWYM, TRIP, TRAP: one operands  */
		if ((n_operands != 1) || (exp[0].X_op != O_constant))
			as_fatal (_("invalid operands to opcode %s: `%s'"), instruction->name, operands);

		DDOP_CHECK_18_BIT(exp[0].X_add_number);
		DDOP_SET_FA(opcodep, instruction->fa_as_opcode);
		DDOP_SET_FB(opcodep, (exp[0].X_add_number >> 12) & 63);
		DDOP_SET_FC(opcodep, (exp[0].X_add_number >> 6) & 63);
		DDOP_SET_FD(opcodep, (exp[0].X_add_number) & 63);

		break;

	case dadao_operands_orir: /* "regd, regb, imm6" */
		if ((n_operands != 3) || (exp[2].X_op != O_constant))
			as_fatal (_("invalid operands to opcode %s: `%s'"), instruction->name, operands);

		DDOP_EXP_MUST_BE_REG(exp[0]);
		DDOP_EXP_MUST_BE_REG(exp[1]);
		DDOP_CHECK_6_BIT(exp[2].X_add_number);

		DDOP_SET_FD(opcodep, exp[0].X_add_number);
		DDOP_SET_FA(opcodep, instruction->fa_as_opcode);
		DDOP_SET_FB(opcodep, exp[1].X_add_number);
		DDOP_SET_FC(opcodep, exp[2].X_add_number);

		break;

	case dadao_operands_orir_orrr: /* "regd, regb, regc" or "regd, regb, imm6" */
		if (n_operands != 3)
			as_fatal (_("invalid operands to opcode %s: `%s'"), instruction->name, operands);

		DDOP_EXP_MUST_BE_REG(exp[0]);
		DDOP_EXP_MUST_BE_REG(exp[1]);

		DDOP_SET_FD(opcodep, exp[0].X_add_number);
		DDOP_SET_FA(opcodep, instruction->fa_as_opcode);
		DDOP_SET_FB(opcodep, exp[1].X_add_number);

		switch (exp[2].X_op) {
		case O_register: /* regc */
			DDOP_CHECK_6_BIT(exp[2].X_add_number);
			DDOP_SET_FC(opcodep, exp[2].X_add_number);
			break;

		case O_constant: /* imm6 */
			DDOP_CHECK_6_BIT(exp[2].X_add_number);
			DDOP_SET_FC(opcodep, exp[2].X_add_number);

			opcodep[0] |= IMM_OFFSET_BIT;
			break;

		default:
			as_fatal (_("invalid operands to opcode %s: `%s'"), instruction->name, operands);
		}
		break;

	case dadao_operands_riir_rrir_or_sym: /* "regd, rega, regb << shift6" or "regd, rega, imm12" */
		if (n_operands == 2) {
			symbolS *sym;
			/* The last operand is immediate whenever we see just two operands.  */
			opcodep[0] |= IMM_OFFSET_BIT;

			/* Now, we could either have an implied "0" as the fbc operand, or
			   it could be the constant of a "base address plus offset".  It
			   depends on whether it is allowed; only memory operations, as
			   signified by instruction->type and "T" and "X" operand types,
			   and it depends on whether we find a register in the second
			   operand, exp[1].  */
			DDOP_EXP_MUST_BE_REG(exp[0]);

			if (exp[1].X_op == O_register)
				as_fatal (_("invalid operands to opcode %s: `%s'"), instruction->name, operands);

			/* To avoid getting a NULL add_symbol for constants and then
			   catching a SEGV in write_relocs since it doesn't handle
			   constants well for relocs other than PC-relative, we need to
			   pass expressions as symbols and use fix_new, not fix_new_exp.  */
			sym = make_expr_symbol (exp + 1);

			/* Mark the symbol as being OK for a reloc.  */
			symbol_get_bfdsym (sym)->flags |= BSF_KEEP;

			/* Now we know it can be a "base address plus offset".  Add
			   proper fixup types so we can handle this later, when we've
			   parsed everything.  */
			fix_new (opc_fragP, opcodep - opc_fragP->fr_literal + 2,
				8, sym, 0, 0, BFD_RELOC_DADAO_BASE_PLUS_OFFSET);
			break;
		}
		/* FALLTHROUGH.  */

	case dadao_operands_riir_rrir: /* "regd, rega, regb << shift6" or "regd, rega, imm12" */
		if (n_operands != 3)
			as_fatal (_("invalid operands to opcode %s: `%s'"), instruction->name, operands);

		DDOP_EXP_MUST_BE_REG(exp[0]);
		DDOP_EXP_MUST_BE_REG(exp[1]);

		DDOP_SET_FD(opcodep, exp[0].X_add_number);
		DDOP_SET_FA(opcodep, exp[1].X_add_number);

		switch (exp[2].X_op) {
		case O_register: /* regb */
			DDOP_CHECK_6_BIT(exp[2].X_add_number);
			DDOP_SET_FB(opcodep, exp[2].X_add_number);
			break;

		case O_left_shift: /* regb << shift6 */
			exp_left = symbol_get_value_expression (exp[2].X_add_symbol);
			exp_right = symbol_get_value_expression (exp[2].X_op_symbol);

			DDOP_EXP_MUST_BE_REG(exp_left[0]);
			DDOP_EXP_MUST_BE_UIMM6(exp_right[0]);

			DDOP_SET_FB(opcodep, exp_left->X_add_number);
			DDOP_SET_FC(opcodep, exp_right->X_add_number);
			break;

		case O_constant: /* imm12 */
			DDOP_EXP_MUST_BE_UIMM12(exp[2]);
			DDOP_SET_FB_FC(opcodep, exp[2].X_add_number);

			opcodep[0] |= IMM_OFFSET_BIT;
			break;

		default:
			as_fatal (_("invalid operands to opcode %s: `%s'"), instruction->name, operands);
		}
		break;

	case dadao_operands_riii_rrii: /* "rega, imm18" or "rega, regb, imm12" */
		if ((n_operands != 2) && (n_operands != 3))
			as_fatal (_("invalid operands to opcode %s: `%s'"), instruction->name, operands);

		if (n_operands == 2) {
			/* The last operand is imm18 whenever we see just two operands.  */
			opcodep[0] |= IMM_OFFSET_BIT;

			DDOP_EXP_MUST_BE_REG(exp[0]);
			DDOP_SET_FA(opcodep, exp[0].X_add_number);

			/* Add a frag for a JMP relaxation; we need room for max four extra instructions.
			   We don't do any work around here to check if we can determine the offset right away.  */
			if (! expand_op)
				fix_new_exp (opc_fragP, opcodep - opc_fragP->fr_literal, 4, exp + 1, 1, BFD_RELOC_DADAO_ADDR19);
			else
				frag_var (rs_machine_dependent, 4 * 4, 0, ENCODE_RELAX (STATE_JMP, STATE_UNDF),
					exp[1].X_add_symbol, exp[1].X_add_number, opcodep);
			break;
		}

		if (n_operands == 3) {
			symbolS *sym;

			DDOP_EXP_MUST_BE_REG(exp[0]);

			DDOP_SET_FA(opcodep, exp[0].X_add_number);

			/* Now, we could either have an implied "0" as the fbcd operand, or
			   it could be the constant of a "base address plus offset".  It
			   depends on whether it is allowed; only memory operations, as
			   signified by instruction->type and "T" and "X" operand types,
			   and it depends on whether we find a register in the second
			   operand, exp[1].  */

			if (exp[2].X_op == O_register)
				as_fatal (_("invalid operands to opcode %s: `%s'"), instruction->name, operands);

			/* To avoid getting a NULL add_symbol for constants and then
			   catching a SEGV in write_relocs since it doesn't handle
			   constants well for relocs other than PC-relative, we need to
			   pass expressions as symbols and use fix_new, not fix_new_exp.  */
			sym = make_expr_symbol (exp + 2);

			/* Mark the symbol as being OK for a reloc.  */
			symbol_get_bfdsym (sym)->flags |= BSF_KEEP;

			/* Now we know it can be a "base address plus offset".  Add
			   proper fixup types so we can handle this later, when we've
			   parsed everything.  */
			fix_new (opc_fragP, opcodep - opc_fragP->fr_literal + 2,
				8, sym, 0, 0, BFD_RELOC_DADAO_BASE_PLUS_OFFSET);
			break;
		}

		break;

	case dadao_operands_riii: /* "rega, imm18" */
		/* GETA/branch: Add a frag for relaxation.  We don't do any work
		   around here to check if we can determine the offset right away.  */
		if ((n_operands != 2) || (exp[1].X_op == O_register))
			as_fatal (_("invalid operands to opcode %s: `%s'"), instruction->name, operands);

		DDOP_EXP_MUST_BE_REG(exp[0]);
		DDOP_SET_FA(opcodep, exp[0].X_add_number);

		switch (instruction->type) {
		case dadao_type_condbranch:
			if (! expand_op)
				fix_new_exp (opc_fragP, opcodep - opc_fragP->fr_literal, 4, exp + 1, 1, BFD_RELOC_DADAO_ADDR19);
			else
				frag_var (rs_machine_dependent, BCC_MAX_LEN - 4, 0, ENCODE_RELAX (STATE_BCC, STATE_UNDF),
					exp[1].X_add_symbol, exp[1].X_add_number, opcodep);
			break;

		case dadao_type_jmp:
			if (exp[0].X_add_number != 0)
				as_fatal (_("non-zero reg operand NOT supported now: %s"), instruction->name);

			/* Add a frag for a JMP relaxation; we need room for max four extra instructions.
			   We don't do any work around here to check if we can determine the offset right away.  */
			if (! expand_op)
				fix_new_exp (opc_fragP, opcodep - opc_fragP->fr_literal, 4, exp + 1, 1, BFD_RELOC_DADAO_ADDR19);
			else
				frag_var (rs_machine_dependent, 4 * 4, 0, ENCODE_RELAX (STATE_JMP, STATE_UNDF),
					exp[1].X_add_symbol, exp[1].X_add_number, opcodep);
			break;

		case dadao_type_geta:
			if (! expand_op)
				fix_new_exp (opc_fragP, opcodep - opc_fragP->fr_literal, 4, exp + 1, 1, BFD_RELOC_DADAO_ADDR19);
			else
				frag_var (rs_machine_dependent, GETA_MAX_LEN - 4, 0, ENCODE_RELAX (STATE_GETA, STATE_UNDF),
					exp[1].X_add_symbol, exp[1].X_add_number, opcodep);
			break;

		default:
			as_fatal (_("FIXME: SHOULD NOT BE HERE: %s"), instruction->name);
		}

		break;

	case dadao_operands_o000: /* nop / ret */
		if (n_operands != 0)
			as_fatal (_("invalid operands to opcode %s: `%s'"), instruction->name, operands);

		if (instruction->fa_as_opcode == 0x01) {	/* ret */
			DDOP_SET_FA(opcodep, instruction->fa_as_opcode);
			DDOP_SET_FB(opcodep, 0);
			DDOP_SET_FC(opcodep, 0);
			DDOP_SET_FD(opcodep, 0);
			break;
		}

		/* ONLY for nop: 0xDADADADA */
		DDOP_SET_FA(opcodep, ((0xDADADA) >> 18) & 63);	/* FA is 0x36 */
		DDOP_SET_FB(opcodep, ((0xDADADA) >> 12) & 63);
		DDOP_SET_FC(opcodep, ((0xDADADA) >> 6) & 63);
		DDOP_SET_FD(opcodep, (0xDADADA) & 63);
		break;

    default:
      BAD_CASE (instruction->operands);
    }
}

/* The GREG pseudo.  At LABEL, we have the name of a symbol that we
   want to make a register symbol, and which should be initialized with
   the value in the expression at INPUT_LINE_POINTER (defaulting to 0).
   Either and (perhaps less meaningful) both may be missing. */

static void
dadao_greg_internal (char *label)
{
  expressionS *expP = &dadao_raw_gregs[n_of_raw_gregs].exp;
  segT section;

  /* Don't set the section to register contents section before the
     expression has been parsed; it may refer to the current position.  */
  section = expression (expP);

  /* FIXME: Check that no expression refers to the register contents
     section.  May need to be done in elf64-dadao.c.  */
  if (expP->X_op == O_absent)
    {
      /* Default to zero if the expression was absent.  */
      expP->X_op = O_constant;
      expP->X_add_number = 0;
      expP->X_unsigned = 0;
      expP->X_add_symbol = NULL;
      expP->X_op_symbol = NULL;
    }

  if (section == undefined_section)
    {
      /* This is an error or a LOC with an expression involving
	 forward references.  For the expression to be correctly
	 evaluated, we need to force a proper symbol; gas loses track
	 of the segment for "local symbols".  */
      if (expP->X_op == O_add)
	{
	  symbol_get_value_expression (expP->X_op_symbol);
	  symbol_get_value_expression (expP->X_add_symbol);
	}
      else
	{
	  gas_assert (expP->X_op == O_symbol);
	  symbol_get_value_expression (expP->X_add_symbol);
	}
    }

  dadao_raw_gregs[n_of_raw_gregs].label =label;

  if (n_of_raw_gregs == MAX_GREGS - 1)
    as_bad (_("too many GREG registers allocated (max %d)"), MAX_GREGS);
  else
    n_of_raw_gregs++;

  demand_empty_rest_of_line ();
}

/* The ".greg label,expr" worker.  */

static void
s_greg (int unused ATTRIBUTE_UNUSED)
{
  char *p;
  char c;

  /* This will skip over what can be a symbol and zero out the next
     character, which we assume is a ',' or other meaningful delimiter.
     What comes after that is the initializer expression for the
     register.  */
  c = get_symbol_name (&p);

  if (c == '"')
    c = * ++ input_line_pointer;

  if (! is_end_of_line[(unsigned char) c])
    input_line_pointer++;

  dadao_greg_internal (NULL);
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
     }									\
   break;

  switch (fragP->fr_subtype)
    {
      HANDLE_RELAXABLE (STATE_GETA);
      HANDLE_RELAXABLE (STATE_BCC);
      HANDLE_RELAXABLE (STATE_JMP);

    case ENCODE_RELAX (STATE_GETA, STATE_ZERO):
    case ENCODE_RELAX (STATE_BCC, STATE_ZERO):
    case ENCODE_RELAX (STATE_JMP, STATE_ZERO):
      /* When relaxing a section for the second time, we don't need to do
	 anything except making sure that fr_var is set right.  */
      break;

    case STATE_GREG_DEF:
      length = fragP->tc_frag_data != NULL ? 0 : 8;
      fragP->fr_var = length;

      /* Don't consult the relax_table; it isn't valid for this
	 relaxation.  */
      return length;
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
  fragS *opc_fragP = fragP->tc_frag_data;
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
  opcode_address = fragP->fr_address + fragP->fr_fix - 4;

  switch (fragP->fr_subtype)
    {
    case ENCODE_RELAX (STATE_GETA, STATE_ZERO):
    case ENCODE_RELAX (STATE_BCC, STATE_ZERO):
      dadao_set_geta_branch_offset (opcodep, target_address - opcode_address);
      if (linkrelax)
	{
	  tmpfixP
	    = fix_new (opc_fragP, opcodep - opc_fragP->fr_literal, 4,
		       fragP->fr_symbol, fragP->fr_offset, 1,
		       BFD_RELOC_DADAO_ADDR19);
	  COPY_FR_WHERE_TO_FX (fragP, tmpfixP);
	}
      var_part_size = 0;
      break;

    case ENCODE_RELAX (STATE_JMP, STATE_ZERO):
      dadao_set_jmp_offset (opcodep, target_address - opcode_address);
      if (linkrelax)
	{
	  tmpfixP
	    = fix_new (opc_fragP, opcodep - opc_fragP->fr_literal, 4,
		       fragP->fr_symbol, fragP->fr_offset, 1,
		       BFD_RELOC_DADAO_ADDR27);
	  COPY_FR_WHERE_TO_FX (fragP, tmpfixP);
	}
      var_part_size = 0;
      break;

    case STATE_GREG_DEF:
      if (fragP->tc_frag_data == NULL)
	{
	  /* We must initialize data that's supposed to be "fixed up" to
	     avoid emitting garbage, because md_apply_fix won't do
	     anything for undefined symbols.  */
	  md_number_to_chars (var_partp, 0, 8);
	  tmpfixP
	    = fix_new (fragP, var_partp - fragP->fr_literal, 8,
		       fragP->fr_symbol, fragP->fr_offset, 0, BFD_RELOC_64);
	  COPY_FR_WHERE_TO_FX (fragP, tmpfixP);
	  dadao_gregs[n_of_cooked_gregs++] = tmpfixP;
	  var_part_size = 8;
	}
      else
	var_part_size = 0;
      break;

#define HANDLE_MAX_RELOC(state, reloc)					\
  case ENCODE_RELAX (state, STATE_MAX):					\
    var_part_size							\
      = dadao_relax_table[ENCODE_RELAX (state, STATE_MAX)].rlx_length;	\
    dadao_fill_nops (var_partp, var_part_size / 4);			\
    if (warn_on_expansion)						\
      as_warn_where (fragP->fr_file, fragP->fr_line,			\
		     _("operand out of range, instruction expanded"));	\
    tmpfixP = fix_new (fragP, var_partp - fragP->fr_literal - 4, 8,	\
		       fragP->fr_symbol, fragP->fr_offset, 1, reloc);	\
    COPY_FR_WHERE_TO_FX (fragP, tmpfixP);				\
    break

      HANDLE_MAX_RELOC (STATE_GETA, BFD_RELOC_DADAO_GETA);
      HANDLE_MAX_RELOC (STATE_BCC, BFD_RELOC_DADAO_CBRANCH);
      HANDLE_MAX_RELOC (STATE_JMP, BFD_RELOC_DADAO_JMP);

    default:
      BAD_CASE (fragP->fr_subtype);
      break;
    }

  fragP->fr_fix += var_part_size;
  fragP->fr_var = 0;
}

/* Applies the desired value to the specified location.
   Also sets up addends for RELA type relocations.
   Stolen from tc-mcore.c.

   Note that this function isn't called when linkrelax != 0.  */

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
	      && symsec != absolute_section
	      && ((fixP->fx_r_type != BFD_RELOC_DADAO_REG
		   && fixP->fx_r_type != BFD_RELOC_DADAO_REG_OR_BYTE)
		  || symsec != reg_section))))
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

    case BFD_RELOC_DADAO_ADDR19:
      if (expand_op)
	{
	  /* This shouldn't happen.  */
	  BAD_CASE (fixP->fx_r_type);
	  break;
	}
      /* FALLTHROUGH.  */
    case BFD_RELOC_DADAO_GETA:
    case BFD_RELOC_DADAO_CBRANCH:
      /* If this fixup is out of range, punt to the linker to emit an
	 error.  This should only happen with -no-expand.  */
      if (val < -(((offsetT) 1 << 19)/2)
	  || val >= ((offsetT) 1 << 19)/2 - 1
	  || (val & 3) != 0)
	{
	  if (warn_on_expansion)
	    as_warn_where (fixP->fx_file, fixP->fx_line,
			   _("operand out of range"));
	  fixP->fx_done = 0;
	  val = 0;
	}
      dadao_set_geta_branch_offset (buf, val);
      break;

    case BFD_RELOC_DADAO_ADDR27:
      if (expand_op)
	{
	  /* This shouldn't happen.  */
	  BAD_CASE (fixP->fx_r_type);
	  break;
	}
      /* FALLTHROUGH.  */
    case BFD_RELOC_DADAO_JMP:
      /* If this fixup is out of range, punt to the linker to emit an
	 error.  This should only happen with -no-expand.  */
      if (val < -(((offsetT) 1 << 27)/2)
	  || val >= ((offsetT) 1 << 27)/2 - 1
	  || (val & 3) != 0)
	{
	  if (warn_on_expansion)
	    as_warn_where (fixP->fx_file, fixP->fx_line,
			   _("operand out of range"));
	  fixP->fx_done = 0;
	  val = 0;
	}
      dadao_set_jmp_offset (buf, val);
      break;

    case BFD_RELOC_DADAO_REG_OR_BYTE:
      if (fixP->fx_addsy != NULL
	  && (S_GET_SEGMENT (fixP->fx_addsy) != reg_section
	      || S_GET_VALUE (fixP->fx_addsy) > 255)
	  && S_GET_SEGMENT (fixP->fx_addsy) != absolute_section)
	{
	  as_bad_where (fixP->fx_file, fixP->fx_line,
			_("invalid operands"));
	  /* We don't want this "symbol" appearing in output, because
	     that will fail.  */
	  fixP->fx_done = 1;
	}

      buf[0] = val;

      /* If this reloc is for a Z field, we need to adjust
	 the opcode if we got a constant here.
	 FIXME: Can we make this more robust?  */

      if ((fixP->fx_where & 3) == 3
	  && (fixP->fx_addsy == NULL
	      || S_GET_SEGMENT (fixP->fx_addsy) == absolute_section))
	buf[-3] |= IMM_OFFSET_BIT;
      break;

    case BFD_RELOC_DADAO_REG:
      if (fixP->fx_addsy == NULL
	  || S_GET_SEGMENT (fixP->fx_addsy) != reg_section
	  || S_GET_VALUE (fixP->fx_addsy) > 255)
	{
	  as_bad_where (fixP->fx_file, fixP->fx_line,
			_("invalid operands"));
	  fixP->fx_done = 1;
	}

      *buf = val;
      break;

    case BFD_RELOC_DADAO_BASE_PLUS_OFFSET:
      /* These are never "fixed".  */
      fixP->fx_done = 0;
      return;

    case BFD_RELOC_DADAO_CBRANCH_J:
    case BFD_RELOC_DADAO_CBRANCH_1:
    case BFD_RELOC_DADAO_CBRANCH_2:
    case BFD_RELOC_DADAO_CBRANCH_3:
    case BFD_RELOC_DADAO_GETA_1:
    case BFD_RELOC_DADAO_GETA_2:
    case BFD_RELOC_DADAO_GETA_3:
    case BFD_RELOC_DADAO_JMP_1:
    case BFD_RELOC_DADAO_JMP_2:
    case BFD_RELOC_DADAO_JMP_3:
    default:
      BAD_CASE (fixP->fx_r_type);
      break;
    }

  if (fixP->fx_done)
    /* Make sure that for completed fixups we have the value around for
       use by e.g. dadao_frob_file.  */
    fixP->fx_offset = val;
}

/* A bsearch function for looking up a value against offsets for GREG
   definitions.  */

static int
cmp_greg_val_greg_symbol_fixes (const void *p1, const void *p2)
{
  offsetT val1 = *(offsetT *) p1;
  offsetT val2 = ((struct dadao_symbol_greg_fixes *) p2)->offs;

  if (val1 >= val2 && val1 < val2 + 255)
    return 0;

  if (val1 > val2)
    return 1;

  return -1;
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
	  /* Resolve this reloc now, as md_apply_fix would have done (not
	     called if -linkrelax).  There is no point in keeping a reloc
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
    case BFD_RELOC_DADAO_GETA_1:
    case BFD_RELOC_DADAO_GETA_2:
    case BFD_RELOC_DADAO_GETA_3:
    case BFD_RELOC_DADAO_CBRANCH:
    case BFD_RELOC_DADAO_CBRANCH_J:
    case BFD_RELOC_DADAO_CBRANCH_1:
    case BFD_RELOC_DADAO_CBRANCH_2:
    case BFD_RELOC_DADAO_CBRANCH_3:
    case BFD_RELOC_DADAO_JMP:
    case BFD_RELOC_DADAO_JMP_1:
    case BFD_RELOC_DADAO_JMP_2:
    case BFD_RELOC_DADAO_JMP_3:
    case BFD_RELOC_DADAO_ADDR19:
    case BFD_RELOC_DADAO_ADDR27:
      code = fixP->fx_r_type;
      break;

    case BFD_RELOC_DADAO_REG_OR_BYTE:
      /* If we have this kind of relocation to an unknown symbol or to the
	 register contents section (that is, to a register), then we can't
	 resolve the relocation here.  */
      if (addsy != NULL
	  && (bfd_is_und_section (addsec)
	      || strcmp (bfd_get_section_name (addsec->owner, addsec),
			 DADAO_REG_CONTENTS_SECTION_NAME) == 0))
	{
	  code = fixP->fx_r_type;
	  break;
	}

      /* If the relocation is not to the register section or to the
	 absolute section (a numeric value), then we have an error.  */
      if (addsy != NULL
	  && (S_GET_SEGMENT (addsy) != real_reg_section
	      || val > 255
	      || val < 0)
	  && ! bfd_is_abs_section (addsec))
	goto badop;

      /* Set the "immediate" bit of the insn if this relocation is to Z
	 field when the value is a numeric value, i.e. not a register.  */
      if ((fixP->fx_where & 3) == 3
	  && (addsy == NULL || bfd_is_abs_section (addsec)))
	buf[-3] |= IMM_OFFSET_BIT;

      buf[0] = val;
      return NULL;

    case BFD_RELOC_DADAO_BASE_PLUS_OFFSET:
      if (addsy != NULL
	  && strcmp (bfd_get_section_name (addsec->owner, addsec),
		     DADAO_REG_CONTENTS_SECTION_NAME) == 0)
	{
	  /* This changed into a register; the relocation is for the
	     register-contents section.  The constant part remains zero.  */
	  code = BFD_RELOC_DADAO_REG;
	  break;
	}

      /* If we've found out that this was indeed a register, then replace
	 with the register number.  The constant part is already zero.

	 If we encounter any other defined symbol, then we must find a
	 suitable register and emit a reloc.  */
      if (addsy == NULL || addsec != real_reg_section)
	{
	  struct dadao_symbol_gregs *gregs;
	  struct dadao_symbol_greg_fixes *fix;

	  if (S_IS_DEFINED (addsy)
	      && !bfd_is_com_section (addsec)
	      && !S_IS_WEAK (addsy))
	    {
	      if (! symbol_section_p (addsy) && ! bfd_is_abs_section (addsec))
		as_fatal (_("internal: BFD_RELOC_DADAO_BASE_PLUS_OFFSET not resolved to section"));

	      /* If this is an absolute symbol sufficiently near
		 lowest_data_loc, then we canonicalize on the data
		 section.  Note that val is signed here; we may subtract
		 lowest_data_loc which is unsigned.  Careful with those
		 comparisons.  */
	      if (lowest_data_loc != (bfd_vma) -1
		  && (bfd_vma) val + 256 > lowest_data_loc
		  && bfd_is_abs_section (addsec))
		{
		  val -= (offsetT) lowest_data_loc;
		  addsy = section_symbol (data_section);
		}
	      /* Likewise text section.  */
	      else if (lowest_text_loc != (bfd_vma) -1
		       && (bfd_vma) val + 256 > lowest_text_loc
		       && bfd_is_abs_section (addsec))
		{
		  val -= (offsetT) lowest_text_loc;
		  addsy = section_symbol (text_section);
		}
	    }

	  gregs = *symbol_get_tc (addsy);

	  /* If that symbol does not have any associated GREG definitions,
	     we can't do anything.  */
	  if (gregs == NULL
	      || (fix = bsearch (&val, gregs->greg_fixes, gregs->n_gregs,
				 sizeof (gregs->greg_fixes[0]),
				 cmp_greg_val_greg_symbol_fixes)) == NULL
	      /* The register must not point *after* the address we want.  */
	      || fix->offs > val
	      /* Neither must the register point more than 255 bytes
		 before the address we want.  */
	      || fix->offs + 255 < val)
	    {
	      /* We can either let the linker allocate GREGs
		 automatically, or emit an error.  */
	      if (allocate_undefined_gregs_in_linker)
		{
		  /* The values in baddsy and addend are right.  */
		  code = fixP->fx_r_type;
		  break;
		}
	      else
		as_bad_where (fixP->fx_file, fixP->fx_line,
			      _("no suitable GREG definition for operands"));
	      return NULL;
	    }
	  else
	    {
	      /* Transform the base-plus-offset reloc for the actual area
		 to a reloc for the register with the address of the area.
		 Put addend for register in Z operand.  */
	      buf[1] = val - fix->offs;
	      code = BFD_RELOC_DADAO_REG;
	      baddsy
		= (bfd_get_section_by_name (stdoutput,
					    DADAO_REG_CONTENTS_SECTION_NAME)
		   ->symbol);

	      addend = fix->fix->fx_frag->fr_address + fix->fix->fx_where;
	    }
	}
      else if (S_GET_VALUE (addsy) > 255)
	as_bad_where (fixP->fx_file, fixP->fx_line,
		      _("invalid operands"));
      else
	{
	  *buf = val;
	  return NULL;
	}
      break;

    case BFD_RELOC_DADAO_REG:
      if (addsy != NULL
	  && (bfd_is_und_section (addsec)
	      || strcmp (bfd_get_section_name (addsec->owner, addsec),
			 DADAO_REG_CONTENTS_SECTION_NAME) == 0))
	{
	  code = fixP->fx_r_type;
	  break;
	}

      if (addsy != NULL
	  && (addsec != real_reg_section
	      || val > 255
	      || val < 0)
	  && ! bfd_is_und_section (addsec))
	/* Drop through to error message.  */
	;
      else
	{
	  buf[0] = val;
	  return NULL;
	}
      /* FALLTHROUGH.  */

      /* The others are supposed to be handled by md_apply_fix.
	 FIXME: ... which isn't called when -linkrelax.  Move over
	 md_apply_fix code here for everything reasonable.  */
    badop:
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
  if (fixP->fx_r_type == BFD_RELOC_DADAO_BASE_PLUS_OFFSET)
    return 1;

  if (linkrelax)
    return 1;

  /* All our pcrel relocations are must-keep.  Note that md_apply_fix is
     called *after* this, and will handle getting rid of the presumed
     reloc; a relocation isn't *forced* other than to be handled by
     md_apply_fix (or tc_gen_reloc if linkrelax).  */
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

/* Adjust the symbol table.  We make reg_section relative to the real
   register section.  */

void
dadao_adjust_symtab (void)
{
  symbolS *sym;
  symbolS *regsec = section_symbol (reg_section);

  for (sym = symbol_rootP; sym != NULL; sym = symbol_next (sym))
    if (S_GET_SEGMENT (sym) == reg_section)
      {
	if (sym == regsec)
	  {
	    if (S_IS_EXTERNAL (sym) || symbol_used_in_reloc_p (sym))
	      abort ();
	    symbol_remove (sym, &symbol_rootP, &symbol_lastP);
	  }
	else
	  /* Change section to the *real* register section, so it gets
	     proper treatment when writing it out.  Only do this for
	     global symbols.  This also means we don't have to check for
	     $0..$255.  */
	  S_SET_SEGMENT (sym, real_reg_section);
      }
}

/* This is the expansion of md_relax_frag.  We go through the ordinary
   relax table function except when the frag is for a GREG.  Then we have
   to check whether there's another GREG by the same value that we can
   join with.  */
long dadao_md_relax_frag (segT seg, fragS *fragP, long stretch)
{
  switch (fragP->fr_subtype)
    {
      /* Growth for this type has been handled by dadao_md_end and
	 correctly estimated, so there's nothing more to do here.  */
    case STATE_GREG_DEF:
      return 0;

    default:
      return relax_frag (seg, fragP, stretch);

    case STATE_GREG_UNDF:
      BAD_CASE (fragP->fr_subtype);
    }

  as_fatal (_("internal: unexpected relax type %d:%d"),
	    fragP->fr_type, fragP->fr_subtype);
  return 0;
}

/* Various things we punt until all input is seen.  */
void dadao_md_end (void)
{
  fragS *fragP;
  asection *regsec;
  struct loc_assert_s *loc_assert;
  int i;

  /* The first frag of GREG:s going into the register contents section.  */
  fragS *dadao_reg_contents_frags = NULL;

  /* Emit the low LOC setting of .text.  */
  if (text_has_contents && lowest_text_loc != (bfd_vma) -1)
    {
      symbolS *symbolP;
      char locsymbol[sizeof (":") - 1
		    + sizeof (DADAO_LOC_SECTION_START_SYMBOL_PREFIX) - 1
		    + sizeof (".text")];

      /* An exercise in non-ISO-C-ness, this one.  */
      sprintf (locsymbol, ":%s%s", DADAO_LOC_SECTION_START_SYMBOL_PREFIX,
	       ".text");
      symbolP
	= symbol_new (locsymbol, absolute_section, lowest_text_loc,
		      &zero_address_frag);
      S_SET_EXTERNAL (symbolP);
    }

  /* Ditto .data.  */
  if (data_has_contents && lowest_data_loc != (bfd_vma) -1)
    {
      symbolS *symbolP;
      char locsymbol[sizeof (":") - 1
		     + sizeof (DADAO_LOC_SECTION_START_SYMBOL_PREFIX) - 1
		     + sizeof (".data")];

      sprintf (locsymbol, ":%s%s", DADAO_LOC_SECTION_START_SYMBOL_PREFIX,
	       ".data");
      symbolP
	= symbol_new (locsymbol, absolute_section, lowest_data_loc,
		      &zero_address_frag);
      S_SET_EXTERNAL (symbolP);
    }

  /* Check that we didn't LOC into the unknown, or rather that when it
     was unknown, we actually change sections.  */
  for (loc_assert = loc_asserts;
       loc_assert != NULL;
       loc_assert = loc_assert->next)
    {
      segT actual_seg;

      resolve_symbol_value (loc_assert->loc_sym);
      actual_seg = S_GET_SEGMENT (loc_assert->loc_sym);
      if (actual_seg != loc_assert->old_seg)
	{
	  const char *fnam;
	  unsigned int line;
	  int e_valid = expr_symbol_where (loc_assert->loc_sym, &fnam, &line);

	  gas_assert (e_valid == 1);
	  as_bad_where (fnam, line,
			_("LOC to section unknown or indeterminable "
			  "at first pass"));

	  /* Patch up the generic location data to avoid cascading
	     error messages from later passes.  (See original in
	     write.c:relax_segment.)  */
	  fragP = loc_assert->frag;
	  fragP->fr_type = rs_align;
	  fragP->fr_subtype = 0;
	  fragP->fr_offset = 0;
	  fragP->fr_fix = 0;
	}
    }

  if (n_of_raw_gregs != 0)
    {
      /* Emit GREGs.  They are collected in order of appearance, but must
	 be emitted in opposite order to both have section address regno*8
	 and the same allocation order (within a file) as dadaoal.  */
      segT this_segment = now_seg;
      subsegT this_subsegment = now_subseg;

      regsec = bfd_make_section_old_way (stdoutput,
					 DADAO_REG_CONTENTS_SECTION_NAME);
      subseg_set (regsec, 0);

      /* Finally emit the initialization-value.  Emit a variable frag, which
	 we'll fix in md_estimate_size_before_relax.  We set the initializer
	 for the tc_frag_data field to NULL, so we can use that field for
	 relaxation purposes.  */
      dadao_opcode_frag = NULL;

      frag_grow (0);
      dadao_reg_contents_frags = frag_now;

      for (i = n_of_raw_gregs - 1; i >= 0; i--)
	{
	  if (dadao_raw_gregs[i].label != NULL)
	    /* There's a symbol.  Let it refer to this location in the
	       register contents section.  The symbol must be globalized
	       separately.  */
	    colon (dadao_raw_gregs[i].label);

	  frag_var (rs_machine_dependent, 8, 0, STATE_GREG_UNDF,
		    make_expr_symbol (&dadao_raw_gregs[i].exp), 0, NULL);
	}

      subseg_set (this_segment, this_subsegment);
    }

  regsec = bfd_get_section_by_name (stdoutput, DADAO_REG_CONTENTS_SECTION_NAME);
  /* Mark the section symbol as being OK for a reloc.  */
  if (regsec != NULL)
    regsec->symbol->flags |= BSF_KEEP;

  /* Iterate over frags resulting from GREGs and move those that evidently
     have the same value together and point one to another.

     This works in time O(N^2) but since the upper bound for non-error use
     is 223, it's best to keep this simpler algorithm.  */
  for (fragP = dadao_reg_contents_frags; fragP != NULL; fragP = fragP->fr_next)
    {
      fragS **fpp;
      fragS *fp = NULL;
      fragS *osymfrag;
      offsetT osymval;
      expressionS *oexpP;
      symbolS *symbolP = fragP->fr_symbol;

      if (fragP->fr_type != rs_machine_dependent
	  || fragP->fr_subtype != STATE_GREG_UNDF)
	continue;

      /* Whatever the outcome, we will have this GREG judged merged or
	 non-merged.  Since the tc_frag_data is NULL at this point, we
	 default to non-merged.  */
      fragP->fr_subtype = STATE_GREG_DEF;

      /* If we're not supposed to merge GREG definitions, then just don't
	 look for equivalents.  */
      if (! merge_gregs)
	continue;

      osymval = (offsetT) S_GET_VALUE (symbolP);
      osymfrag = symbol_get_frag (symbolP);

      /* If the symbol isn't defined, we can't say that another symbol
	 equals this frag, then.  FIXME: We can look at the "deepest"
	 defined name; if a = c and b = c then obviously a == b.  */
      if (! S_IS_DEFINED (symbolP))
	continue;

      oexpP = symbol_get_value_expression (fragP->fr_symbol);

      /* If the initialization value is zero, then we must not merge them.  */
      if (oexpP->X_op == O_constant && osymval == 0)
	continue;

      /* Iterate through the frags downward this one.  If we find one that
	 has the same non-zero value, move it to after this one and point
	 to it as the equivalent.  */
      for (fpp = &fragP->fr_next; *fpp != NULL; fpp = &fpp[0]->fr_next)
	{
	  fp = *fpp;

	  if (fp->fr_type != rs_machine_dependent
	      || fp->fr_subtype != STATE_GREG_UNDF)
	    continue;

	  /* Calling S_GET_VALUE may simplify the symbol, changing from
	     expr_section etc. so call it first.  */
	  if ((offsetT) S_GET_VALUE (fp->fr_symbol) == osymval
	      && symbol_get_frag (fp->fr_symbol) == osymfrag)
	    {
	      /* Move the frag links so the one we found equivalent comes
		 after the current one, carefully considering that
		 sometimes fpp == &fragP->fr_next and the moves must be a
		 NOP then.  */
	      *fpp = fp->fr_next;
	      fp->fr_next = fragP->fr_next;
	      fragP->fr_next = fp;
	      break;
	    }
	}

      if (*fpp != NULL)
	fragP->tc_frag_data = fp;
    }
}

/* qsort function for dadao_symbol_gregs.  */

static int
cmp_greg_symbol_fixes (const void *parg, const void *qarg)
{
  const struct dadao_symbol_greg_fixes *p
    = (const struct dadao_symbol_greg_fixes *) parg;
  const struct dadao_symbol_greg_fixes *q
    = (const struct dadao_symbol_greg_fixes *) qarg;

  return p->offs > q->offs ? 1 : p->offs < q->offs ? -1 : 0;
}

/* Collect GREG definitions from dadao_gregs and hang them as lists sorted
   on increasing offsets onto each section symbol or undefined symbol.

   Also, remove the register convenience section so it doesn't get output
   as an ELF section.  */

void
dadao_frob_file (void)
{
  int i;
  struct dadao_symbol_gregs *all_greg_symbols[MAX_GREGS];
  int n_greg_symbols = 0;

  /* Collect all greg fixups and decorate each corresponding symbol with
     the greg fixups for it.  */
  for (i = 0; i < n_of_cooked_gregs; i++)
    {
      offsetT offs;
      symbolS *sym;
      struct dadao_symbol_gregs *gregs;
      fixS *fixP;

      fixP = dadao_gregs[i];
      know (fixP->fx_r_type == BFD_RELOC_64);

      /* This case isn't doable in general anyway, methinks.  */
      if (fixP->fx_subsy != NULL)
	{
	  as_bad_where (fixP->fx_file, fixP->fx_line,
			_("GREG expression too complicated"));
	  continue;
	}

      sym = fixP->fx_addsy;
      offs = (offsetT) fixP->fx_offset;

      /* If the symbol is defined, then it must be resolved to a section
	 symbol at this time, or else we don't know how to handle it.  */
      if (S_IS_DEFINED (sym)
	  && !bfd_is_com_section (S_GET_SEGMENT (sym))
	  && !S_IS_WEAK (sym))
	{
	  if (! symbol_section_p (sym)
	      && ! bfd_is_abs_section (S_GET_SEGMENT (sym)))
	    as_fatal (_("internal: GREG expression not resolved to section"));

	  offs += S_GET_VALUE (sym);
	}

      /* If this is an absolute symbol sufficiently near lowest_data_loc,
	 then we canonicalize on the data section.  Note that offs is
	 signed here; we may subtract lowest_data_loc which is unsigned.
	 Careful with those comparisons.  */
      if (lowest_data_loc != (bfd_vma) -1
	  && (bfd_vma) offs + 256 > lowest_data_loc
	  && bfd_is_abs_section (S_GET_SEGMENT (sym)))
	{
	  offs -= (offsetT) lowest_data_loc;
	  sym = section_symbol (data_section);
	}
      /* Likewise text section.  */
      else if (lowest_text_loc != (bfd_vma) -1
	       && (bfd_vma) offs + 256 > lowest_text_loc
	       && bfd_is_abs_section (S_GET_SEGMENT (sym)))
	{
	  offs -= (offsetT) lowest_text_loc;
	  sym = section_symbol (text_section);
	}

      gregs = *symbol_get_tc (sym);

      if (gregs == NULL)
	{
	  gregs = XNEW (struct dadao_symbol_gregs);
	  gregs->n_gregs = 0;
	  symbol_set_tc (sym, &gregs);
	  all_greg_symbols[n_greg_symbols++] = gregs;
	}

      gregs->greg_fixes[gregs->n_gregs].fix = fixP;
      gregs->greg_fixes[gregs->n_gregs++].offs = offs;
    }

  /* For each symbol having a GREG definition, sort those definitions on
     offset.  */
  for (i = 0; i < n_greg_symbols; i++)
    qsort (all_greg_symbols[i]->greg_fixes, all_greg_symbols[i]->n_gregs,
	   sizeof (all_greg_symbols[i]->greg_fixes[0]), cmp_greg_symbol_fixes);

  if (real_reg_section != NULL)
    {
      /* FIXME: Pass error state gracefully.  */
      if (bfd_get_section_flags (stdoutput, real_reg_section) & SEC_HAS_CONTENTS)
	as_fatal (_("register section has contents\n"));

      bfd_section_list_remove (stdoutput, real_reg_section);
      --stdoutput->section_count;
    }

}

/* Just get knowledge about alignment from the new section.  */
void dadao_md_elf_section_change_hook (void)
{
  last_alignment = bfd_get_section_alignment (now_seg->owner, now_seg);
}

/* The md_do_align worker.  At present, we just record an alignment to
   nullify the automatic alignment we do for WYDE, TETRA and OCTA, as gcc
   does not use the unaligned macros when attribute packed is used.
   Arguably this is a GCC bug.  */
void dadao_md_do_align (int n, char *fill ATTRIBUTE_UNUSED,
		  int len ATTRIBUTE_UNUSED, int max ATTRIBUTE_UNUSED)
{
  last_alignment = n;
}
