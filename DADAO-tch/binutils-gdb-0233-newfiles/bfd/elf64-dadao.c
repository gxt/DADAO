/*
 * DADAO-specific support for 64-bit ELF
 * Copyright (C) 2019-2033 Guan Xuetao (AT) Peking Univ.
 *
 * Contributed by:
 *   2019:
 *	Guan Xuetao <gxt@pku.edu.cn>
 *
 * This file is part of BFD, the Binary File Descriptor library.
 */

/* No specific ABI or "processor-specific supplement" defined.  */

#include "sysdep.h"
#include "bfd.h"
#include "libbfd.h"
#include "elf-bfd.h"
#include "elf/dadao.h"
#include "opcode/dadao.h"

static bfd_reloc_status_type
dadao_elf_reloc (bfd * abfd ATTRIBUTE_UNUSED,
		 arelent * reloc_entry,
		 asymbol * symbol,
		 void *data ATTRIBUTE_UNUSED,
		 asection * input_section,
		 bfd * output_bfd,
		 char **error_message ATTRIBUTE_UNUSED);

/* We don't actually apply any relocations in this toolset
   so we make them all do nothing, but at least display useful
   names.
   Most of these are mainly used by the DADAO toolchain to resolve things
   before the final ELF file is created.  */
static reloc_howto_type dadao_elf_howto_table[] =
{
  HOWTO (R_DADAO_NOTYPE,		/* Type.  */
	 0,				/* Rightshift.  */
	 3,				/* Size.  */
	 0,				/* Bitsize.  */
	 FALSE,				/* PC_relative.  */
	 0,				/* Bitpos.  */
	 complain_overflow_dont,	/* Complain_on_overflow.  */
	 dadao_elf_reloc,		/* Special_function.  */
	 "R_DADAO_NOTYPE",		/* Name.  */
	 FALSE,				/* Partial_inplace.  */
	 0,				/* Src_mask.  */
	 0,				/* Dst_mask.  */
	 FALSE),			/* PCrel_offset.  */
  HOWTO (R_DADAO_IMM12, 0, 3, 0, FALSE, 0,
	 complain_overflow_dont, dadao_elf_reloc,
	 "R_DADAO_IMM12",
	 FALSE, 0, 0, FALSE),
  HOWTO (R_DADAO_IMM18, 0, 3, 0, FALSE, 0,
	 complain_overflow_dont, dadao_elf_reloc,
	 "R_DADAO_IMM18",
	 FALSE, 0, 0, FALSE),
  HOWTO (R_DADAO_IMM24, 0, 3, 0, FALSE, 0,
	 complain_overflow_dont, dadao_elf_reloc,
	 "R_DADAO_IMM24",
	 FALSE, 0, 0, FALSE)
};

static bfd_reloc_status_type
dadao_elf_reloc (bfd * abfd ATTRIBUTE_UNUSED,
		 arelent * reloc_entry ATTRIBUTE_UNUSED,
		 asymbol * symbol ATTRIBUTE_UNUSED,
		 void *data ATTRIBUTE_UNUSED,
		 asection * input_section ATTRIBUTE_UNUSED,
		 bfd * output_bfd ATTRIBUTE_UNUSED,
		 char **error_message ATTRIBUTE_UNUSED)
{
  return bfd_reloc_ok;
}

static bfd_boolean
dadao_elf_info_to_howto (bfd * abfd ATTRIBUTE_UNUSED,
			 arelent * cache_ptr, Elf_Internal_Rela * dst)
{
  unsigned int r_type;

  r_type = ELF64_R_TYPE (dst->r_info);
  if (r_type >= R_DADAO_MAX) {
      /* xgettext:c-format */
      _bfd_error_handler (_("%pB: unsupported relocation type %#x"),
			  abfd, r_type);
      bfd_set_error (bfd_error_bad_value);
      return FALSE;
  }
  cache_ptr->howto = &dadao_elf_howto_table[r_type];
  return TRUE;
}

static reloc_howto_type *
dadao_elf_reloc_type_lookup (bfd * abfd ATTRIBUTE_UNUSED,
			     bfd_reloc_code_real_type code ATTRIBUTE_UNUSED)
{
  return NULL;
}

static reloc_howto_type *
dadao_elf_reloc_name_lookup (bfd * abfd ATTRIBUTE_UNUSED,
			     const char *r_name ATTRIBUTE_UNUSED)
{
  return NULL;
}

#define ELF_ARCH				bfd_arch_dadao
#define ELF_MACHINE_CODE			EM_DADAO
#define ELF_MACHINE_ALT1			E_DADAO_MACH_WUXING
#define ELF_MAXPAGESIZE				0x10000

#define TARGET_BIG_NAME				"elf64-dadao"
#define TARGET_BIG_SYM				dadao_elf64_vec

#define elf_info_to_howto_rel			NULL
#define elf_info_to_howto			dadao_elf_info_to_howto

#define bfd_elf64_bfd_reloc_type_lookup		dadao_elf_reloc_type_lookup
#define bfd_elf64_bfd_reloc_name_lookup		dadao_elf_reloc_name_lookup

#include "elf64-target.h"
