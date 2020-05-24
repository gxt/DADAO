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
  RELOC_NUMBER (R_DADAO_GETA, 13)
  RELOC_NUMBER (R_DADAO_CBRANCH, 14)
  RELOC_NUMBER (R_DADAO_CALL, 15)
  RELOC_NUMBER (R_DADAO_JMP, 16)
  RELOC_NUMBER (R_DADAO_LDST, 17)

  /* A relative address such as in a GETA or a branch.  */
  RELOC_NUMBER (R_DADAO_ADDR19, 18)

  /* A relative address such as in a JMP (only).  */
  RELOC_NUMBER (R_DADAO_ADDR27, 19)

  /* A general register. */
  RELOC_NUMBER (R_DADAO_REG, 20)

  /* A global register and an offset, the global register (allocated at
     link time) contents plus the offset made equivalent to the relocation
     expression at link time.  The relocation must point at the Y field of
     an instruction.  */
  RELOC_NUMBER (R_DADAO_BASE_PLUS_OFFSET, 21)

END_RELOC_NUMBERS (R_DADAO_max)


/* Section Attributes.  */
/* A section containing necessary information for relaxation.  */
#define SHF_DADAO_CANRELAX	0x80000000

/* Symbol attributes.  */
/* A symbol with this section-index is a register.  */
#define SHN_REGISTER	SHN_LOPROC

/* This section holds contents for each initialized register, at VMA
   regno*8.  A symbol relative to this section will be transformed to an
   absolute symbol with the value corresponding to the register number at
   final link time.  A symbol with a value outside the inclusive range
   32*8 .. 254*8 is an error.  It is highly recommended to only use an
   upper bound of 253*8 or lower as specified in the (currently
   unspecified) ABI.  */
#define DADAO_REG_CONTENTS_SECTION_NAME ".DADAO.reg_contents"

/* At link time, a section by this name is created, expected to be
   included in DADAO_REG_CONTENTS_SECTION_NAME in the output.  */
#define DADAO_LD_ALLOCATED_REG_CONTENTS_SECTION_NAME \
 ".DADAO.reg_contents.linker_allocated"

/* This is a faked section holding symbols with SHN_REGISTER.  Don't
   confuse it with DADAO_REG_CONTENTS_SECTION_NAME; this one has no
   contents, just values.  It is an error for a value in this section to
   be outside the range 32..255 and it must never become an actual section
   in an object file.  */
#define DADAO_REG_SECTION_NAME "*REG*"

/* A section SECNAME is noted to start at "__.DADAO.start.SECNAME" by the
   presence of this symbol.  Currently only implemented for ".text"
   through the symbol "__.DADAO.start..text".  */
#define DADAO_LOC_SECTION_START_SYMBOL_PREFIX "__.DADAO.start."

/* This symbol is always a function.  */
#define DADAO_START_SYMBOL_NAME "Main"

#ifdef BFD_ARCH_SIZE
extern bfd_boolean _bfd_dadao_before_linker_allocation
  (bfd *, struct bfd_link_info *);
extern bfd_boolean _bfd_dadao_after_linker_allocation
  (bfd *, struct bfd_link_info *);
extern bfd_boolean _bfd_dadao_check_all_relocs
  (bfd *, struct bfd_link_info *);
#endif

#endif /* ELF_DADAO_H */
