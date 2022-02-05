/*
 * DADAO ELF support for BFD.
 * Copyright (C) 2019-2033 Guan Xuetao (AT) Peking Univ.
 *
 * Contributed by:
 *   2019:
 *	Guan Xuetao <gxt@pku.edu.cn>
 */
#ifndef ELF_DADAO_H
#define ELF_DADAO_H

#include "elf/reloc-macros.h"

/* Relocations.  See the reloc table in bfd/elf64-dadao.c for details.  */
START_RELOC_NUMBERS (elf_dadao_reloc_type)
  RELOC_NUMBER (R_DADAO_NONE, 0)

  /* Standard absolute relocations.  */
  RELOC_NUMBER (R_DADAO_8, 1)
  RELOC_NUMBER (R_DADAO_16, 2)
  RELOC_NUMBER (R_DADAO_24, 3)
  RELOC_NUMBER (R_DADAO_32, 4)
  RELOC_NUMBER (R_DADAO_64, 5)

  /* Standard relative relocations.  */
  RELOC_NUMBER (R_DADAO_PC_8, 6)
  RELOC_NUMBER (R_DADAO_PC_16, 7)
  RELOC_NUMBER (R_DADAO_PC_24, 8)
  RELOC_NUMBER (R_DADAO_PC_32, 9)
  RELOC_NUMBER (R_DADAO_PC_64, 10)

  /* GNU extensions for C++ vtables.  */
  RELOC_NUMBER (R_DADAO_GNU_VTINHERIT, 11)
  RELOC_NUMBER (R_DADAO_GNU_VTENTRY, 12)

  /* DADAO specific instructions.  */
  RELOC_NUMBER (R_DADAO_ABS, 13)
  RELOC_NUMBER (R_DADAO_BRCC, 14)
  RELOC_NUMBER (R_DADAO_CALL, 15)
  RELOC_NUMBER (R_DADAO_JUMP, 16)

END_RELOC_NUMBERS (R_DADAO_max)


/* Section Attributes.  */
/* A section containing necessary information for relaxation.  */
#define SHF_DADAO_CANRELAX	0x80000000

/* This symbol is always a function.  */
#define DADAO_START_SYMBOL_NAME "Main"

#endif /* ELF_DADAO_H */
