/*
 * Disassemble dadao instructions.
 * Copyright (C) 2019-2033 Guan Xuetao (AT) Peking Univ.
 *
 * Contributed by:
 *   2019:
 *	Guan Xuetao <gxt@pku.edu.cn>
 */
#include "sysdep.h"
#include <stdio.h>
#include "opcode/dadao.h"
#include "disassemble.h"
#include "libiberty.h"
#include "bfd.h"
#include "opintl.h"

#define BAD_CASE(x)						\
  do								\
   {								\
     opcodes_error_handler (_("bad case %d (%s) in %s:%d"),	\
			    x, #x, __FILE__, __LINE__);		\
     abort ();							\
   }								\
 while (0)

#define FATAL_DEBUG						\
 do								\
   {								\
     opcodes_error_handler (_("internal: non-debugged code "	\
			      "(test-case missing): %s:%d"),	\
			    __FILE__, __LINE__);		\
     abort ();							\
   }								\
 while (0)

#define ROUND_MODE(n)					\
 ((n) == 1 ? "ROUND_OFF" : (n) == 2 ? "ROUND_UP" :	\
  (n) == 3 ? "ROUND_DOWN" : (n) == 4 ? "ROUND_NEAR" :	\
  _("(unknown)"))

#define INSN_IMMEDIATE_BIT (IMM_OFFSET_BIT << 24)
#define INSN_BACKWARD_OFFSET_BIT (1 << 24)

#define MAX_REG_NAME_LEN       256
#define MAX_SPEC_REG_NAME_LEN  32
struct dadao_dis_info
 {
   const char *reg_name[MAX_REG_NAME_LEN];
   const char *spec_reg_name[MAX_SPEC_REG_NAME_LEN];

   /* Waste a little memory so we don't have to allocate each separately.
      We could have an array with static contents for these, but on the
      other hand, we don't have to.  */
   char basic_reg_name[MAX_REG_NAME_LEN][sizeof ("$255")];
 };

/* Initialize a target-specific array in INFO.  */

static bfd_boolean
initialize_dadao_dis_info (struct disassemble_info *info)
{
  struct dadao_dis_info *minfop = malloc (sizeof (struct dadao_dis_info));
  long i;

  if (minfop == NULL)
    return FALSE;

  memset (minfop, 0, sizeof (*minfop));

  /* Initialize register names from register symbols.  If there's no
     register section, then there are no register symbols.  */
  if ((info->section != NULL && info->section->owner != NULL)
      || (info->symbols != NULL
	  && info->symbols[0] != NULL
	  && bfd_asymbol_bfd (info->symbols[0]) != NULL))
    {
      bfd *abfd = info->section && info->section->owner != NULL
	? info->section->owner
	: bfd_asymbol_bfd (info->symbols[0]);
      asection *reg_section = bfd_get_section_by_name (abfd, "*REG*");

      if (reg_section != NULL)
	{
	  /* The returned symcount *does* include the ending NULL.  */
	  long symsize = bfd_get_symtab_upper_bound (abfd);
	  asymbol **syms = malloc (symsize);
	  long nsyms;

	  if (syms == NULL)
	    {
	      FATAL_DEBUG;
	      free (minfop);
	      return FALSE;
	    }
	  nsyms = bfd_canonicalize_symtab (abfd, syms);

	  /* We use the first name for a register.  If this is MMO, then
	     it's the name with the first sequence number, presumably the
	     first in the source.  */
	  for (i = 0; i < nsyms && syms[i] != NULL; i++)
	    {
	      if (syms[i]->section == reg_section
		  && syms[i]->value < MAX_REG_NAME_LEN
		  && minfop->reg_name[syms[i]->value] == NULL)
		minfop->reg_name[syms[i]->value] = syms[i]->name;
	    }
	}
    }

  /* Fill in the rest with the canonical names.  */
  for (i = 0; i < MAX_REG_NAME_LEN; i++)
    if (minfop->reg_name[i] == NULL)
      {
	sprintf (minfop->basic_reg_name[i], "$%ld", i);
	minfop->reg_name[i] = minfop->basic_reg_name[i];
      }

  /* We assume it's actually a one-to-one mapping of number-to-name.  */
  for (i = 0; dadao_spec_regs[i].name != NULL; i++)
    minfop->spec_reg_name[dadao_spec_regs[i].number] = dadao_spec_regs[i].name;

  info->private_data = (void *) minfop;
  return TRUE;
}

/* A table indexed by the first byte is constructed as we disassemble each
   tetrabyte.  The contents is a pointer into dadao_insns reflecting the
   first found entry with matching match-bits and lose-bits.  Further
   entries are considered one after one until the operand constraints
   match or the match-bits and lose-bits do not match.  Normally a
   "further entry" will just show that there was no other match.  */

static const struct dadao_opcode *
get_opcode (unsigned long insn, unsigned int fop, unsigned int fa)
{
  static const struct dadao_opcode **opcodes = NULL;
  static const struct dadao_opcode **opcodes_3E = NULL;
  static const struct dadao_opcode **opcodes_DA = NULL;
  static const struct dadao_opcode **opcodes_DB = NULL;
  const struct dadao_opcode *opcodep = dadao_opcodes;
  unsigned int opcode_part = (insn >> 24) & 255;

  if (opcodes == NULL) {
    opcodes = xcalloc (256, sizeof (struct dadao_opcode *));
    opcodes_3E = xcalloc (64, sizeof (struct dadao_opcode *));
    opcodes_DA = xcalloc (64, sizeof (struct dadao_opcode *));
    opcodes_DB = xcalloc (64, sizeof (struct dadao_opcode *));
  }

	/* FIXME: too ugly */
	if ((fop == 0x3E) || (fop == 0x3F)) {
		opcodep = opcodes_3E[fa];
		if (opcodep == NULL) {
			/* Search through the table.  */
			for (opcodep = dadao_opcodes; opcodep->name != NULL; opcodep++) {
				if ((opcodep->match == 0x3E000000) && (opcodep->fa_as_opcode == fa)) {
					opcodes_3E[fa] = opcodep;
					break;
				}
			}
		}
		goto get_opcode_found;
	}

	if (fop == 0xDA) {
		opcodep = opcodes_DA[fa];
		if (opcodep == NULL) {
			/* Search through the table.  */
			for (opcodep = dadao_opcodes; opcodep->name != NULL; opcodep++) {
				if ((opcodep->match == 0xDA000000) && (opcodep->fa_as_opcode == fa)) {
					opcodes_DA[fa] = opcodep;
					break;
				}
			}
		}
		goto get_opcode_found;
	}

	if (fop == 0xDB) {
		opcodep = opcodes_DB[fa];
		if (opcodep == NULL) {
			/* Search through the table.  */
			for (opcodep = dadao_opcodes; opcodep->name != NULL; opcodep++) {
				if ((opcodep->match == 0xDB000000) && (opcodep->fa_as_opcode == fa)) {
					opcodes_DB[fa] = opcodep;
					break;
				}
			}
		}
		goto get_opcode_found;
	}

  opcodep = opcodes[opcode_part];
  if (opcodep == NULL
      || (opcodep->match & insn) != opcodep->match
      || (opcodep->lose & insn) != 0)
    {
      /* Search through the table.  */
      for (opcodep = dadao_opcodes; opcodep->name != NULL; opcodep++)
	{
	  /* FIXME: Break out this into an initialization function.  */
	  if ((opcodep->match & (opcode_part << 24)) == opcode_part
	      && (opcodep->lose & (opcode_part << 24)) == 0)
	    opcodes[opcode_part] = opcodep;

	  if ((opcodep->match & insn) == opcodep->match
	      && (opcodep->lose & insn) == 0)
	    break;
	}
    }

get_opcode_found:

  if (opcodep->name == NULL)
    return NULL;

  /* Check constraints.  If they don't match, loop through the next opcode
     entries.  */
  do
    {
      switch (opcodep->operands)
	{
	  /* These have no restraint on what can be in the lower three
	     bytes.  */
	case dadao_operands_fa_op_fdfb_reg_fc_0_get:
	case dadao_operands_fa_op_fdfb_reg_fc_0_put:
	case dadao_operands_orrr:
	case dadao_operands_oiii:
	case dadao_operands_fa_op_fdfb_reg_fc_i6:
	case dadao_operands_fa_op_fdfb_reg_fc_reg_i6:
	case dadao_operands_fdfa_reg_fbc_rs6_i12:
	case dadao_operands_fdfa_reg_fbc_rs6_i12_or_sym:
	case dadao_operands_riii:
	case dadao_operands_riii_rrii:
	case dadao_operands_none:
	  return opcodep;

	default:
	  BAD_CASE (opcodep->operands);
	}

      opcodep++;
    }
  while ((opcodep->match & insn) == opcodep->match
	 && (opcodep->lose & insn) == 0);

  /* If we got here, we had no match.  */
  return NULL;
}

static inline const char *
get_reg_name (const struct dadao_dis_info * minfop, unsigned int x)
{
  if (x >= MAX_REG_NAME_LEN)
    return _("*illegal*");
  return minfop->reg_name[x];
}

static inline const char *
get_spec_reg_name (const struct dadao_dis_info * minfop, unsigned int x)
{
  if (x >= MAX_SPEC_REG_NAME_LEN)
    return _("*illegal*");
  return minfop->spec_reg_name[x];
}

/* The main disassembly function.  */

int
print_insn_dadao (bfd_vma memaddr, struct disassemble_info *info)
{
  unsigned char buffer[4];
  unsigned long insn;
  unsigned int x, y, z;
  unsigned int fop, fa, fb, fc, fbc, fd;
  const struct dadao_opcode *opcodep;
  int status = (*info->read_memory_func) (memaddr, buffer, 4, info);
  struct dadao_dis_info *minfop;

  if (status != 0)
    {
      (*info->memory_error_func) (status, memaddr, info);
      return -1;
    }

  /* FIXME: Is -1 suitable?  */
  if (info->private_data == NULL
      && ! initialize_dadao_dis_info (info))
    return -1;

  minfop = (struct dadao_dis_info *) info->private_data;
  x = buffer[1];
  y = buffer[2];
  z = buffer[3];

  fop = buffer[0];
  fa = buffer[1] >> 2;
  fb = ((buffer[1] & 0x3) << 4) | (buffer[2] >> 4);
  fc = ((buffer[2] & 0xf) << 2) | (buffer[3] >> 6);
  fbc = (fb << 6) | fc;
  fd = buffer[3] & 0x3f;

  insn = bfd_getb32 (buffer);

  opcodep = get_opcode (insn, fop, fa);

  if (opcodep == NULL)
    {
      (*info->fprintf_func) (info->stream, _("*unknown*"));
      return 4;
    }

  (*info->fprintf_func) (info->stream, "%s", opcodep->name);

  /* Present bytes in the order they are laid out in memory.  */
  info->display_endian = BFD_ENDIAN_BIG;

  info->insn_info_valid = 1;
  info->bytes_per_chunk = 4;
  info->branch_delay_insns = 0;
  info->target = 0;
  switch (opcodep->type)
    {
    case dadao_type_normal:
    case dadao_type_memaccess_block:
    case dadao_type_fd_eq_fb_op_fc:
    case dadao_type_fd_eq_fa_op_bc:
    case dadao_type_geta:
      info->insn_type = dis_nonbranch;
      break;

    case dadao_type_jmp:
    case dadao_type_branch:
      info->insn_type = dis_branch;
      break;

    case dadao_type_condbranch:
      info->insn_type = dis_condbranch;
      break;

    case dadao_type_memaccess_octa:
      info->insn_type = dis_dref;
      info->data_size = 8;
      break;

    case dadao_type_memaccess_tetra:
      info->insn_type = dis_dref;
      info->data_size = 4;
      break;

    case dadao_type_memaccess_wyde:
      info->insn_type = dis_dref;
      info->data_size = 2;
      break;

    case dadao_type_memaccess_byte:
      info->insn_type = dis_dref;
      info->data_size = 1;
      break;

    case dadao_type_jsr:
      info->insn_type = dis_jsr;
      break;

    default:
      BAD_CASE(opcodep->type);
    }

  switch (opcodep->operands)
    {
    case dadao_operands_orrr: /* regd, regb, regc */
      (*info->fprintf_func) (info->stream, "\t%s, %s, %s",
			     get_reg_name (minfop, fd),
			     get_reg_name (minfop, fb),
			     get_reg_name (minfop, fc));
      break;

    case dadao_operands_fdfa_reg_fbc_rs6_i12: /* The regular "rega, regb << shift6" or "rega, imm12" */
    case dadao_operands_fdfa_reg_fbc_rs6_i12_or_sym: /* The regular "rega, regb << shift6" or "rega, imm12" */
      if (insn & INSN_IMMEDIATE_BIT)
	(*info->fprintf_func) (info->stream, "\t%s, %s, %d",
			       get_reg_name (minfop, fd),
			       get_reg_name (minfop, fa), fbc);
      else if (fc == 0)
	  (*info->fprintf_func) (info->stream, "\t%s, %s, %s",
				get_reg_name (minfop, fd),
				get_reg_name (minfop, fa),
				get_reg_name (minfop, fb));
      else
	  (*info->fprintf_func) (info->stream, "\t%s, %s, %s << %d",
				get_reg_name (minfop, fd),
				get_reg_name (minfop, fa),
				get_reg_name (minfop, fb), fc);
      break;

    case dadao_operands_fa_op_fdfb_reg_fc_i6: /* The regular "regb, regc" or "regb, imm6" */
	(*info->fprintf_func) (info->stream, "\t%s, %s, %d",
			       get_reg_name (minfop, fd),
			       get_reg_name (minfop, fb), fc);
	break;

    case dadao_operands_fa_op_fdfb_reg_fc_reg_i6: /* The regular "regb, regc" or "regb, imm6" */
      if (insn & INSN_IMMEDIATE_BIT)
	(*info->fprintf_func) (info->stream, "\t%s, %s, %d",
			       get_reg_name (minfop, fd),
			       get_reg_name (minfop, fb), fc);
      else
	  (*info->fprintf_func) (info->stream, "\t%s, %s, %s",
				get_reg_name (minfop, fd),
				get_reg_name (minfop, fb),
				get_reg_name (minfop, fc));
      break;

    case dadao_operands_riii:
      /* Like jmp, GETA or branches - "$X,Address".  */
      {
	bfd_signed_vma offset = (y * 256 + z) * 4;

	if (insn & INSN_BACKWARD_OFFSET_BIT)
	  offset -= 65536 * 4;

	info->target = memaddr + offset;

	(*info->fprintf_func) (info->stream, "\t%s,", get_reg_name (minfop, x));
	(*info->print_address_func) (memaddr + offset, info);
      }
      break;

	case dadao_operands_riii_rrii:
		if (insn & INSN_IMMEDIATE_BIT) {
			bfd_signed_vma offset = (y * 256 + z) * 4;

			info->target = memaddr + offset;

			(*info->fprintf_func) (info->stream, "\t%s,", get_reg_name (minfop, fa));
			(*info->print_address_func) (memaddr + offset, info);
		} else
			(*info->fprintf_func) (info->stream, "\t%s, %s, %d",
				       get_reg_name (minfop, fa),
				       get_reg_name (minfop, fb), ((fc << 6) | fd));
		break;


	case dadao_operands_fa_op_fdfb_reg_fc_0_get:
		/* GET - "X,spec_reg".  */
		(*info->fprintf_func) (info->stream, "\t%s,%s",
				     get_reg_name (minfop, x),
				     get_spec_reg_name (minfop, z));
		break;

	case dadao_operands_fa_op_fdfb_reg_fc_0_put:
		/* PUT - "spec_reg,$Z|Z".  */
		(*info->fprintf_func) (info->stream, "\t%s,%s",
				     get_spec_reg_name (minfop, x),
				     get_reg_name (minfop, z));
		break;

	case dadao_operands_oiii: /* Like SWYM or TRAP - "imm18".  */
		(*info->fprintf_func) (info->stream, "\t%d", (fb << 12) | (fc << 6) | fd);
		break;

	case dadao_operands_none: /* nop / ret  */
		break;

    default:
      (*info->fprintf_func) (info->stream, _("*unknown operands type: %d*"),
			     opcodep->operands);
      break;
    }

  return 4;
}
