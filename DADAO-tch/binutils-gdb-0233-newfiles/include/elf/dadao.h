/*
 * DADAO ELF support for BFD.
 * Copyright (C) 2019-2033 Guan Xuetao (AT) Peking Univ.
 *
 * Contributed by:
 *   2019:
 *	Guan Xuetao <gxt@pku.edu.cn>
 */
#ifndef _ELF_DADAO_H
#define _ELF_DADAO_H

#include "bfd.h"
#include "elf/common.h"
#include "elf/reloc-macros.h"
#include "bfd_stdint.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define EF_DADAO_MACH(ef_dadao)        (((ef_dadao) >> 8) & 0xFFFF)
#define EF_DADAO_SET_MACH(dadao_fam)   (((dadao_fam) & 0xFFFF) << 8)

#define E_DADAO_MACH_WUXING	0x0100
#define E_DADAO_MACH_WUQING	0x0200
#define E_DADAO_MACH_WUMING	0x0400

/* DADAO Section types
   INITREG - A generic register initialisation section
   UDEBUG - Legacy-style debug data section.  */
#define SHT_DADAO_INITREG		(SHT_LOPROC + 2)
#define SHT_DADAO_UDEBUG		SHT_LOUSER

/* DADAO SECTION flags
     ELF-64 sh_flags is 64-bit, but there is no info on what the upper 32 bits
     are expected to be used for, it is not marked reserved either.
     We'll use them for DADAO-specific flags since we don't use ELF-32.

   INIT - Sections that are loaded and executed before the final text
	  microcode.  Non-code INIT sections are loaded first, then other
	  memory secions, then INIT2 sections, then INIT-code sections.
   INIT2 - Sections that are loaded before INIT-code sections, used for
	   transient configuration before executing INIT-code section
	   microcode. */

#define SHF_DADAO_INIT			0x80000000
#define SHF_DADAO_INIT2			0x40000000

/* Relocations.  */
START_RELOC_NUMBERS (elf_dadao_reloc_type)
    RELOC_NUMBER (R_DADAO_NOTYPE, 0)
    RELOC_NUMBER (R_DADAO_IMM12, 1)
    RELOC_NUMBER (R_DADAO_IMM18, 2)
    RELOC_NUMBER (R_DADAO_IMM24, 3)
END_RELOC_NUMBERS (R_DADAO_MAX)

#ifdef __cplusplus
}
#endif

#endif /* _ELF_DADAO_H */
