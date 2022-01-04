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

struct dadao_dis_info {
	const struct dadao_opcode *ddis_majorp[256];
	const struct dadao_opcode **ddis_minorp[256];
	enum dadao_operand_type ddis_optype[256];
};

/* Initialize a target-specific array in INFO.  */
static struct dadao_dis_info *initialize_dadao_dis_info (void)
{
	struct dadao_dis_info *ddis_infop;
	const struct dadao_opcode *opcodep;
	const struct dadao_opcode **minor_opcodep;
	unsigned int i;

	ddis_infop = xmalloc (sizeof (struct dadao_dis_info));

	for (i=0; i<256; i++) {
		ddis_infop->ddis_majorp[i] = NULL;
		ddis_infop->ddis_minorp[i] = NULL;
		ddis_infop->ddis_optype[i] = dadao_operand_noop;
	}

	for (opcodep = dadao_opcodes; opcodep->name != NULL; opcodep++) {
		if (opcodep->type == dadao_type_pseudo)
			continue;

		i = opcodep->major_opcode;

		if (opcodep->op_fa == dadao_operand_op) {
			minor_opcodep = ddis_infop->ddis_minorp[i];
			if (minor_opcodep == NULL) {
				minor_opcodep = xcalloc (64, sizeof (struct dadao_opcode *));
				ddis_infop->ddis_minorp[i] = minor_opcodep;
				ddis_infop->ddis_optype[i] = dadao_operand_op;
			}
			minor_opcodep[opcodep->minor_opcode] = opcodep;
			continue;
		}

		if (opcodep->op_fb == dadao_operand_w16) {
			minor_opcodep = ddis_infop->ddis_minorp[i];
			if (minor_opcodep == NULL) {
				minor_opcodep = xcalloc (4, sizeof (struct dadao_opcode *));
				ddis_infop->ddis_minorp[i] = minor_opcodep;
				ddis_infop->ddis_optype[i] = dadao_operand_w16;
			}
			minor_opcodep[opcodep->minor_opcode] = opcodep;
			continue;
		}

		ddis_infop->ddis_majorp[i] = opcodep;
		ddis_infop->ddis_optype[i] = dadao_operand_none;
	}

	return ddis_infop;
}

#define	__DDIS_PRINT_REG(prefix, regnum)						\
	(*info->fprintf_func) (info->stream, "%s%d", prefix, (int)regnum)

#define	__DDIS_EXIT_FAIL()								\
	do {										\
		(*info->fprintf_func) (info->stream, "*unknown* (0x%08x)", insn);	\
		return 4;								\
	} while (0)

/* The main disassembly function.  */
int print_insn_dadao (bfd_vma memaddr, struct disassemble_info *info)
{
	unsigned char buffer[4];
	unsigned int insn;
	unsigned int fa, fb, fc, fd;
	struct dadao_dis_info *ddis_infop;
	const struct dadao_opcode *opcodep;
	const struct dadao_opcode **minor_opcodep;
	int i;
	bfd_signed_vma offset;

	ddis_infop = (struct dadao_dis_info *) info->private_data;

	if (ddis_infop == NULL) {
		ddis_infop = initialize_dadao_dis_info ();
		info->private_data = (void *) ddis_infop;
	}

	i = (*info->read_memory_func) (memaddr, buffer, 4, info);
	if (i != 0) {
		(*info->memory_error_func) (i, memaddr, info);
		return -1;
	}

	/* Present bytes in the order they are laid out in memory.  */
	info->display_endian = BFD_ENDIAN_BIG;

	info->insn_info_valid = 1;
	info->bytes_per_chunk = 4;
	info->branch_delay_insns = 0;
	info->target = 0;

	insn = bfd_getb32 (buffer);

	fa = (insn >> 18) & 0x3F;
	fb = (insn >> 12) & 0x3F;
	fc = (insn >> 6) & 0x3F;
	fd = (insn) & 0x3F;

	switch (ddis_infop->ddis_optype[(insn >> 24)]) {
	case dadao_operand_none:
		opcodep = ddis_infop->ddis_majorp[(insn >> 24)];
		break;

	case dadao_operand_op:
		minor_opcodep = ddis_infop->ddis_minorp[(insn >> 24)];
		opcodep = minor_opcodep[fa];
		break;

	case dadao_operand_w16:
		minor_opcodep = ddis_infop->ddis_minorp[(insn >> 24)];
		opcodep = minor_opcodep[(fb >> 4)];
		break;

	default:
		opcodep = NULL;
	}

	if (opcodep == NULL)		__DDIS_EXIT_FAIL();

	if (opcodep->operands_num != 0)
		if (opcodep->name[0] == '_')
			(*info->fprintf_func) (info->stream, "%s\t", opcodep->name + 1);
		else
			(*info->fprintf_func) (info->stream, "%s\t", opcodep->name);
	else {
		/* ret / nop has no operands */
		(*info->fprintf_func) (info->stream, "%s", opcodep->name);
		return 4;
	}

	i = 0;

	switch (opcodep->op_fa) {
	case dadao_operand_rd:	__DDIS_PRINT_REG("rd", fa);	i++;	break;
	case dadao_operand_rb:	__DDIS_PRINT_REG("rb", fa);	i++;	break;
	case dadao_operand_rf:	__DDIS_PRINT_REG("rf", fa);	i++;	break;
	case dadao_operand_ra:	__DDIS_PRINT_REG("ra", fa);	i++;	break;
	case dadao_operand_cp:	__DDIS_PRINT_REG("cp", fa); i++;	break;

	case dadao_operand_s24: /* ONLY call or jump be here */
		offset = (insn & 0xFFFFFF) << 2;
		if (offset & 0x2000000)		offset -= 0x4000000;	/* backward */

		info->target = memaddr + 4 + offset;

		(*info->print_address_func) (memaddr + 4 + offset, info);
		return 4;

	case dadao_operand_op:
		break;

	default:
		__DDIS_EXIT_FAIL();
	}

	if (opcodep->op_fa != dadao_operand_op)
		(*info->fprintf_func) (info->stream, ", ");

	switch (opcodep->op_fb) {
	case dadao_operand_rd:	__DDIS_PRINT_REG("rd", fb);	i++;	break;
	case dadao_operand_rb:	__DDIS_PRINT_REG("rb", fb);	i++;	break;
	case dadao_operand_rf:	__DDIS_PRINT_REG("rf", fb);	i++;	break;
	case dadao_operand_ra:	__DDIS_PRINT_REG("ra", fb);	i++;	break;
	case dadao_operand_cr:	__DDIS_PRINT_REG("cr", fb); i++;	break;

	case dadao_operand_w16:
		(*info->fprintf_func) (info->stream, "0x%x", (insn & 0xFFFF));
		return 4;

	case dadao_operand_s18:
		if ((insn & 0xFF000000) == 0x18000000 || (insn & 0xFF000000) == 0x19000000) {		/* addrb riii / add riii */
			if (insn & 0x20000)
				(*info->fprintf_func) (info->stream, "%d", (int) (0xFFFC0000 | (insn & 0x3FFFF)));
			else
				(*info->fprintf_func) (info->stream, "%d", (int) (insn & 0x3FFFF));
			return 4;
		}

		offset = (insn & 0x3FFFF) << 2;
		if (offset & 0x80000)		offset -= 0x100000;	/* backward */

		info->target = memaddr + 4 + offset;

		(*info->print_address_func) (memaddr + 4 + offset, info);
		return 4;

	case dadao_operand_u18:
		(*info->fprintf_func) (info->stream, "%d", (int) (insn & 0x3FFFF));
		return 4;

	default:
		__DDIS_EXIT_FAIL();
	}

	(*info->fprintf_func) (info->stream, ", ");

	switch (opcodep->op_fc) {
	case dadao_operand_rd:	__DDIS_PRINT_REG("rd", fc);	i++;	break;
	case dadao_operand_rb:	__DDIS_PRINT_REG("rb", fc);	i++;	break;
	case dadao_operand_rf:	__DDIS_PRINT_REG("rf", fc);	i++;	break;
	case dadao_operand_ra:	__DDIS_PRINT_REG("ra", fc);	i++;	break;
	case dadao_operand_cr:	__DDIS_PRINT_REG("cr", fc); i++;	break;

	case dadao_operand_s12:
		if (insn & 0x800) {
			(*info->fprintf_func) (info->stream, "%d", (int) (0xFFFFF000 | (insn & 0xFFF)));
			return 4;
		}
		/* FALLTHROUGH */
	case dadao_operand_u12:
		(*info->fprintf_func) (info->stream, "%d", (int) (insn & 0xFFF));
		return 4;

	default:
		__DDIS_EXIT_FAIL();
	}

	switch (opcodep->op_fd) {
	case dadao_operand_rd:	__DDIS_PRINT_REG(", rd", fd);	break;
	case dadao_operand_rb:	__DDIS_PRINT_REG(", rb", fd);	break;
	case dadao_operand_rf:	__DDIS_PRINT_REG(", rf", fd);	break;
	case dadao_operand_ra:	__DDIS_PRINT_REG(", ra", fd);	break;
	case dadao_operand_cr:	__DDIS_PRINT_REG(", cr", fd);	break;

	case dadao_operand_i6:
		(*info->fprintf_func) (info->stream, ", %d", (int) (insn & 0x3F));
		break;

	case dadao_operand_none:
		break;

	default:
		__DDIS_EXIT_FAIL();
	}

	return 4;
}
