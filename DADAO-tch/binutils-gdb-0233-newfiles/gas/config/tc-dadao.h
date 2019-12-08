/*
 * tc-dadao.h -- Header file for tc-dadao.c.
 * Copyright (C) 2019-2033 Guan Xuetao (AT) Peking Univ.
 *
 * Contributed by:
 *   2019:
 *	Guan Xuetao <gxt@pku.edu.cn>
 */
#ifndef TC_DADAO
#define TC_DADAO

#ifndef TARGET_BYTES_BIG_ENDIAN
#define TARGET_BYTES_BIG_ENDIAN			1
#endif

#define WORKING_DOT_WORD			1

#define TARGET_FORMAT				"elf64-dadao"

#define TARGET_ARCH				bfd_arch_dadao

#define md_undefined_symbol(NAME)		(0)
#define md_section_align(SEGMENT, SIZE)		(SIZE)
#define md_estimate_size_before_relax(A, B)	(as_fatal (_("estimate size\n")), 0)
#define md_convert_frag(B, S, F)		as_fatal (_("convert_frag\n"))

/* If you define this macro, it should return the offset between the
   address of a PC relative fixup and the position from which the PC
   relative adjustment should be made.  On many processors, the base
   of a PC relative instruction is the next instruction, so this
   macro would return the length of an instruction.  */
#define MD_PCREL_FROM_SECTION(FIX, SEC)		md_pcrel_from (FIX)
extern long md_pcrel_from (struct fix *);

#endif /* TC_DADAO */
