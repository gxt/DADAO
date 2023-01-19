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
#include "dadao-dis.h"
#include "disassemble.h"
#include "libiberty.h"
#include "bfd.h"
#include "opintl.h"

static unsigned int analyze_operand (enum dadao_disassemble_type optype, unsigned int insn, int *cal_offset, int *illegal_insn)
{
        if (*illegal_insn == 1)
		return 0;
	unsigned int data = 0;
        switch (optype){
                case dadao_operand_none:
                        break;
                case dadao_operand_immu6:
                        data = insn & 0x3f;
                        break;
                case dadao_operand_immu12:
                        data = insn & 0xfff;
                        break;
                case dadao_operand_immu18:
                        data = insn & 0x3ffff;
                        break;
                case dadao_operand_ww:
                        data = (insn >> 16) & 0x3;
                        break;
                case dadao_operand_immu16:
                        data = insn & 0xffff;
                        break;
                case dadao_operand_imms12:
                        if (insn & 0x800)
                                data = 0xfffff000 | (insn & 0xfff);
                        else
                                data = insn & 0xfff;
                        break;
                case dadao_operand_imms18:
                        if (insn & 0x20000)
                                data = 0xfffc0000 | (insn & 0x3ffff);
                        else
                                data = insn & 0x3ffff;
                        break;
		case dadao_operand_offset12:
			*cal_offset = 12;
			break;
                case dadao_operand_offset18:
                        *cal_offset = 18;
                        break;
                case dadao_operand_offset24:
                        *cal_offset = 24;
                        break;
                case dadao_operand_ha:
                        data = (insn >> 18) & 0x3f;
                        break;
                case dadao_operand_hb:
                        data = (insn >> 12) & 0x3f;
                        break;
                case dadao_operand_hc:
                        data = (insn >> 6) & 0x3f;
                        break;
                case dadao_operand_hd:
                        data = insn & 0x3f;
                        break;
                default:
                        *illegal_insn = 1;
        }
        return data;
}

/* The main disassembly function.  */
int print_insn_dadao (bfd_vma memaddr, struct disassemble_info *info)
{
	unsigned char buffer[4];
	unsigned int insn;
	unsigned int ha, hb, hc, hd;
	struct dadao_disassemble *disassemble_dict;
	unsigned int match;
	int i;
	bfd_signed_vma offset;

	i = (*info->read_memory_func) (memaddr, buffer, 4, info);
	if (i != 0) {
		(*info->memory_error_func) (i, memaddr, info);
		return -1;
	}

	/* Present bytes in the order they are laid out in memory.  */
	info->display_endian = BFD_ENDIAN_LITTLE;
	info->insn_info_valid = 1;
	info->bytes_per_chunk = 4;
	info->branch_delay_insns = 0;
	info->target = 0;

	insn = bfd_getb32 (buffer);
	ha = (insn >> 10) & 0x3F;
	hb = (((insn >> 8) & 0x3) << 4) + ((insn >> 20) & 0xF);
	hc = (((insn >> 16) & 0xF) << 2) + (insn >> 30);
	hd = (insn >> 24) & 0x3F;
	insn = ((insn & 0xFF) << 24) + (ha << 18) + (hb << 12) + (hc << 6) + hd;

        for(disassemble_dict = dadao_disassemble_opcodes; disassemble_dict->disassemble_format != NULL; disassemble_dict++)
        {
                match = insn & disassemble_dict->opcode_mask;
                if (match != disassemble_dict->opcode_match)
                        continue;
                else
                {
                        int cal_offset = 0;
                        int illegal_insn = 0;
                        unsigned int op_1, op_2, op_3, op_4;
                        op_1 = analyze_operand(disassemble_dict->op1, insn, &cal_offset, &illegal_insn);
                        op_2 = analyze_operand(disassemble_dict->op2, insn, &cal_offset, &illegal_insn);
                        op_3 = analyze_operand(disassemble_dict->op3, insn, &cal_offset, &illegal_insn);
                        op_4 = analyze_operand(disassemble_dict->op4, insn, &cal_offset, &illegal_insn);
                        if (illegal_insn == 1){
                                (*info->fprintf_func) (info->stream, "*unknown* (0x%08x)", insn);
                                return 4;
                        }
			if (cal_offset == 12){
				offset = (insn & 0xfff) << 2;
				if (offset & 0x2000)
					offset -= 0x4000;
                                info->target = memaddr + offset;
                                (*info->fprintf_func) (info->stream, disassemble_dict->disassemble_format, op_1, op_2);
                                (*info->print_address_func) (memaddr + offset, info);
			}
                        if (cal_offset == 18){
                                offset = (insn & 0x3ffff) << 2;
                                if (offset & 0x80000)
                                        offset -= 0x100000;
                                info->target = memaddr + offset;
				(*info->fprintf_func) (info->stream, disassemble_dict->disassemble_format, op_1);
                                (*info->print_address_func) (memaddr + offset, info);
				return 4;
                        }
                        if (cal_offset == 24){
                                offset = (insn & 0xffffff) << 2;
                                if (offset & 0x2000000)
                                        offset -= 0x4000000;
                                info->target = memaddr + offset;
				(*info->fprintf_func) (info->stream, disassemble_dict->disassemble_format);
                                (*info->print_address_func) (memaddr + offset, info);
				return 4;
                        }
                        switch(disassemble_dict->operand_num){
                        case 1:
				(*info->fprintf_func) (info->stream, disassemble_dict->disassemble_format, op_1);
                                break;
                        case 2:
				(*info->fprintf_func) (info->stream, disassemble_dict->disassemble_format, op_1, op_2);
                                break;
                        case 3:
                                (*info->fprintf_func) (info->stream, disassemble_dict->disassemble_format, op_1, op_2, op_3);
                                break;
                        case 4:
                                (*info->fprintf_func) (info->stream, disassemble_dict->disassemble_format, op_1, op_2, op_3, op_4);
                                break;
                        }
                        break;
                }
        }
        return 4;
}
