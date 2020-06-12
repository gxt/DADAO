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

#define FATAL_DEBUG						\
 do								\
   {								\
     opcodes_error_handler (_("internal: non-debugged code "	\
			      "(test-case missing): %s:%d"),	\
			    __FILE__, __LINE__);		\
     abort ();							\
   }								\
 while (0)

#define MAX_REG_NAME_LEN       (256 + 64)

struct dadao_dis_info
 {
   const char *reg_name[MAX_REG_NAME_LEN];

   /* Waste a little memory so we don't have to allocate each separately.
      We could have an array with static contents for these, but on the
      other hand, we don't have to.  */
   char basic_reg_name[MAX_REG_NAME_LEN][sizeof ("rg63")];
 };

/* Initialize a target-specific array in INFO.  */
static bfd_boolean
initialize_dadao_dis_info (struct disassemble_info *info)
{
  struct dadao_dis_info *minfop;
  long i;

  if (info->private_data != NULL)
    return TRUE;

  minfop = malloc (sizeof (struct dadao_dis_info));
  if (minfop == NULL)
    return FALSE;

  memset (minfop, 0, sizeof (*minfop));

  /* Fill in the rest with the canonical names.  */
  for (i = 0; i < 0x40; i++)
    if (minfop->reg_name[i] == NULL)
      {
	sprintf (minfop->basic_reg_name[i], "rg%ld", i);
	minfop->reg_name[i] = minfop->basic_reg_name[i];
      }
  for (i = 0x40; i < 0x80; i++)
    if (minfop->reg_name[i] == NULL)
      {
	sprintf (minfop->basic_reg_name[i], "rp%ld", i - 0x40);
	minfop->reg_name[i] = minfop->basic_reg_name[i];
      }
  for (i = 0x80; i < 0xC0; i++)
    if (minfop->reg_name[i] == NULL)
      {
	sprintf (minfop->basic_reg_name[i], "rf%ld", i - 0x80);
	minfop->reg_name[i] = minfop->basic_reg_name[i];
      }
  for (i = 0xC0; i < 0x100; i++)
    if (minfop->reg_name[i] == NULL)
      {
	sprintf (minfop->basic_reg_name[i], "rv%ld", i - 0xC0);
	minfop->reg_name[i] = minfop->basic_reg_name[i];
      }

  /* We assume it's actually a one-to-one mapping of number-to-name.  */
  for (i = 0; dadao_reg_aliases[i].name != NULL; i++)
    minfop->reg_name[dadao_reg_aliases[i].number] = dadao_reg_aliases[i].name;

  info->private_data = (void *) minfop;
  return TRUE;
}

/* A table indexed by the first byte is constructed as we disassemble each
   tetrabyte.  The contents is a pointer into dadao_insns reflecting the
   first found entry with matching major_opcode and minor_opcode. */
static const struct dadao_opcode *
get_opcode (unsigned long insn)
{
	static const struct dadao_opcode **opcodes = NULL;
	static const struct dadao_opcode **opcodes_3A = NULL;
	static const struct dadao_opcode **opcodes_3B = NULL;
	static const struct dadao_opcode **opcodes_3C = NULL;
	static const struct dadao_opcode **opcodes_3D = NULL;
	static const struct dadao_opcode **opcodes_3E = NULL;
	static const struct dadao_opcode **opcodes_DA = NULL;
	static const struct dadao_opcode **opcodes_DB = NULL;
	const struct dadao_opcode *opcodep = dadao_opcodes;
	unsigned int majop, minop;

	if (opcodes == NULL) {
		opcodes = xcalloc (256, sizeof (struct dadao_opcode *));
		opcodes_3A = xcalloc (4, sizeof (struct dadao_opcode *));
		opcodes_3B = xcalloc (4, sizeof (struct dadao_opcode *));
		opcodes_3C = xcalloc (4, sizeof (struct dadao_opcode *));
		opcodes_3D = xcalloc (4, sizeof (struct dadao_opcode *));
		opcodes_3E = xcalloc (64, sizeof (struct dadao_opcode *));
		opcodes_DA = xcalloc (64, sizeof (struct dadao_opcode *));
		opcodes_DB = xcalloc (64, sizeof (struct dadao_opcode *));
	}

	majop = insn >> 24;

	/* FIXME: Break out this into an initialization function.  */
	if (majop == 0x3A) {
		minop = (insn >> 16) & 0x3;
		opcodep = opcodes_3A[minop];
		if (opcodep != NULL)	return opcodep;

		/* Search through the table.  */
		for (opcodep = dadao_opcodes; opcodep->name != NULL; opcodep++) {
			if ((opcodep->major_opcode == 0x3A) && (opcodep->minor_opcode == minop)) {
				opcodes_3A[minop] = opcodep;
				return opcodep;
			}
		}

		return NULL;
	}

	if (majop == 0x3B) {
		minop = (insn >> 16) & 0x3;
		opcodep = opcodes_3B[minop];
		if (opcodep != NULL)	return opcodep;

		/* Search through the table.  */
		for (opcodep = dadao_opcodes; opcodep->name != NULL; opcodep++) {
			if ((opcodep->major_opcode == 0x3B) && (opcodep->minor_opcode == minop)) {
				opcodes_3B[minop] = opcodep;
				return opcodep;
			}
		}

		return NULL;
	}

	if (majop == 0x3C) {
		minop = (insn >> 16) & 0x3;
		opcodep = opcodes_3C[minop];
		if (opcodep != NULL)	return opcodep;

		/* Search through the table.  */
		for (opcodep = dadao_opcodes; opcodep->name != NULL; opcodep++) {
			if ((opcodep->major_opcode == 0x3C) && (opcodep->minor_opcode == minop)) {
				opcodes_3C[minop] = opcodep;
				return opcodep;
			}
		}

		return NULL;
	}

	if (majop == 0x3D) {
		minop = (insn >> 16) & 0x3;
		opcodep = opcodes_3D[minop];
		if (opcodep != NULL)	return opcodep;

		/* Search through the table.  */
		for (opcodep = dadao_opcodes; opcodep->name != NULL; opcodep++) {
			if ((opcodep->major_opcode == 0x3D) && (opcodep->minor_opcode == minop)) {
				opcodes_3D[minop] = opcodep;
				return opcodep;
			}
		}

		return NULL;
	}

	if ((majop == 0x3E) || (majop == 0x3F)) {
		minop = (insn & 0x00FC0000) >> 18;
		opcodep = opcodes_3E[minop];
		if (opcodep != NULL)	return opcodep;

		/* Search through the table.  */
		for (opcodep = dadao_opcodes; opcodep->name != NULL; opcodep++) {
			if ((opcodep->major_opcode == 0x3E) && (opcodep->minor_opcode == minop)) {
				opcodes_3E[minop] = opcodep;
				return opcodep;
			}
		}

		return NULL;
	}

	if (majop == 0xDA) {
		minop = (insn & 0x00FC0000) >> 18;
		opcodep = opcodes_DA[minop];
		if (opcodep != NULL)	return opcodep;

		/* Search through the table.  */
		for (opcodep = dadao_opcodes; opcodep->name != NULL; opcodep++) {
			if ((opcodep->major_opcode == 0xDA) && (opcodep->minor_opcode == minop)) {
				opcodes_DA[minop] = opcodep;
				return opcodep;
			}
		}

		return NULL;
	}

	if (majop == 0xDB) {
		minop = (insn & 0x00FC0000) >> 18;
		opcodep = opcodes_DB[minop];
		if (opcodep != NULL)	return opcodep;

		/* Search through the table.  */
		for (opcodep = dadao_opcodes; opcodep->name != NULL; opcodep++) {
			if ((opcodep->major_opcode == 0xDB) && (opcodep->minor_opcode == minop)) {
				opcodes_DB[minop] = opcodep;
				return opcodep;
			}
		}

		return NULL;
	}

	opcodep = opcodes[majop];
	if (opcodep != NULL)	return opcodep;

	for (opcodep = dadao_opcodes; opcodep->name != NULL; opcodep++) {
		if (opcodep->type == dadao_type_pseudo)
			continue;

		if (opcodep->major_opcode == majop) {
			opcodes[majop] = opcodep;
			return opcodep;
		}
		if ((opcodep->major_opcode == (majop & 0xFE)) && opcodep->double_modes) {
			opcodes[majop] = opcodep;
			return opcodep;
		}
	}

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

/* The main disassembly function.  */

int print_insn_dadao (bfd_vma memaddr, struct disassemble_info *info)
{
	unsigned char buffer[4];
	unsigned long insn;
	unsigned int fa, fb, fc, fd;
	const struct dadao_opcode *opcodep;
	int status;
	struct dadao_dis_info *minfop;
	bfd_signed_vma offset;
	unsigned int rb_off, rc_off;

	status = (*info->read_memory_func) (memaddr, buffer, 4, info);
	if (status != 0) {
		(*info->memory_error_func) (status, memaddr, info);
		return -1;
	}

	status = initialize_dadao_dis_info (info);
	if (status != TRUE)	return -1;

	minfop = (struct dadao_dis_info *) info->private_data;

	fa = buffer[1] >> 2;
	fb = ((buffer[1] & 0x3) << 4) | (buffer[2] >> 4);
	fc = ((buffer[2] & 0xf) << 2) | (buffer[3] >> 6);
	fd = buffer[3] & 0x3f;

	insn = bfd_getb32 (buffer);

	opcodep = get_opcode (insn);

	if (opcodep == NULL) {
		(*info->fprintf_func) (info->stream, "*unknown* (0x%08lx)", insn);
		return 4;
	}

	(*info->fprintf_func) (info->stream, "%s", opcodep->name);

	/* Present bytes in the order they are laid out in memory.  */
	info->display_endian = BFD_ENDIAN_BIG;

	info->insn_info_valid = 1;
	info->bytes_per_chunk = 4;
	info->branch_delay_insns = 0;
	info->target = 0;

	switch (opcodep->operands) {
	case dadao_operands_o000: /* nop / ret  */
		break;

	case dadao_operands_iiii_rrii: /* call / jump */
		if (insn & DADAO_INSN_ALTMODE) {
			offset = (fa << 20) | (fb << 14) | (fc << 8) | (fd << 2);
			if (offset & 0x2000000)		offset -= 0x4000000;	/* backward */

			info->target = memaddr + 4 + offset;

			(*info->fprintf_func) (info->stream, "\t");
			(*info->print_address_func) (memaddr + 4 + offset, info);
		} else {
			(*info->fprintf_func) (info->stream, "\t%s, %s, 0x%x",
				get_reg_name (minfop, fa), get_reg_name (minfop, fb), (fc << 6) | fd);
		}
		break;

	case dadao_operands_rjii:
		(*info->fprintf_func) (info->stream, "\t%s, 0x%x",
			get_reg_name (minfop, fa), ((fb & 0xF) << 12) | (fc << 6) | fd);
		break;

	case dadao_operands_oiii:
		(*info->fprintf_func) (info->stream, "\t0x%x", (fb << 12) | (fc << 6) | fd);
		break;

	case dadao_operands_orrr:
		(*info->fprintf_func) (info->stream, "\t%s, %s, %s",
			get_reg_name (minfop, fb), get_reg_name (minfop, fc), get_reg_name (minfop, fd));
		break;

	case dadao_operands_orri:
		(*info->fprintf_func) (info->stream, "\t%s, %s, %d",
			get_reg_name (minfop, fb), get_reg_name (minfop, fc), fd);
		break;

	case dadao_operands_orr0:
		rb_off = rc_off = 0;

		switch (opcodep->minor_opcode) {
		case 0x1:	rb_off = rc_off = 0x40;		break;
		case 0x2:	rb_off = rc_off = 0x80;		break;
		case 0x3:	rb_off = rc_off = 0xC0;		break;
		case 0x4:	rb_off = rc_off = 0x100;	break;
		case 0x11:	rb_off = 0x40;	break;
		case 0x12:	rb_off = 0x80;	break;
		case 0x13:	rb_off = 0xC0;	break;
		case 0x14:	rb_off = 0x100;	break;
		case 0x21:	rc_off = 0x40;	break;
		case 0x22:	rc_off = 0x80;	break;
		case 0x23:	rc_off = 0xC0;	break;
		case 0x24:	rc_off = 0x100;	break;
		default:	FATAL_DEBUG;
		}
		(*info->fprintf_func) (info->stream, "\t%s, %s",
			get_reg_name (minfop, fb + rb_off), get_reg_name (minfop, fc + rc_off));
		break;

	case dadao_operands_rrii_rrri:
		if (insn & DADAO_INSN_ALTMODE)
			(*info->fprintf_func) (info->stream, "\t%s, %s, %d",
				get_reg_name (minfop, fa), get_reg_name (minfop, fb), (fc << 6) | fd);
		else if (fd == 0)
			(*info->fprintf_func) (info->stream, "\t%s, %s, %s",
				get_reg_name (minfop, fa), get_reg_name (minfop, fb), get_reg_name (minfop, fc));
		else
			(*info->fprintf_func) (info->stream, "\t%s, %s, %s << %d",
				get_reg_name (minfop, fa), get_reg_name (minfop, fb), get_reg_name (minfop, fc), fd);
		break;

	case dadao_operands_orri_orrr:
		if (insn & DADAO_INSN_ALTMODE)
			(*info->fprintf_func) (info->stream, "\t%s, %s, %d",
				get_reg_name (minfop, fb), get_reg_name (minfop, fc), fd);
		else
			(*info->fprintf_func) (info->stream, "\t%s, %s, %s",
				get_reg_name (minfop, fb), get_reg_name (minfop, fc), get_reg_name (minfop, fd));
		break;

	case dadao_operands_orii_orrr:
		if (insn & DADAO_INSN_ALTMODE)
			(*info->fprintf_func) (info->stream, "\t%s, 0x%x", get_reg_name (minfop, fb), (fc << 6) | fd);
		else
			(*info->fprintf_func) (info->stream, "\t%s, %s, %s",
				get_reg_name (minfop, fb), get_reg_name (minfop, fc), get_reg_name (minfop, fd));
		break;

	case dadao_operands_riii: /* "ra, imm18".  */
		switch (opcodep->type) {
		case dadao_type_geta:
		case dadao_type_condbranch:
			offset = (fb << 14) | (fc << 8) | (fd << 2);
			if (offset & 0x80000)		offset -= 0x100000;	/* backward */

			info->target = memaddr + 4 + offset;

			(*info->fprintf_func) (info->stream, "\t%s, ", get_reg_name (minfop, fa));
			(*info->print_address_func) (memaddr + 4 + offset, info);
			break;

		default:
			FATAL_DEBUG;
		}
		break;

	default:
		(*info->fprintf_func) (info->stream, "*unknown operands type: %d for insn %08lx",
			opcodep->operands, insn);
	}

	return 4;
}
