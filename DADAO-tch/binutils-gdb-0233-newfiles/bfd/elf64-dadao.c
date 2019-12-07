/*
 * DADAO-specific support for 64-bit ELF
 * Copyright (C) 2019-2033 Guan Xuetao (AT) Peking Univ.
 *
 * Contributed by:
 *   2019:
 *	Guan Xuetao <gxt@pku.edu.cn>
 */

#include "sysdep.h"
#include "bfd.h"
#include "libbfd.h"
#include "elf-bfd.h"
#include "elf/dadao.h"

static bfd_boolean
elf64_dadao_object_p (bfd * abfd)
{
  /* If the e_machine value is one of the unofficial ones, we convert
     it first and set e_flags accordingly for later consistency.  */
  if (elf_elfheader (abfd)->e_machine == E_DADAO_MACH_WUXING) {
      elf_elfheader (abfd)->e_machine = EM_DADAO;
      elf_elfheader (abfd)->e_flags &= ~EF_DADAO_SET_MACH (~0);
      elf_elfheader (abfd)->e_flags |= EF_DADAO_SET_MACH (E_DADAO_MACH_WUXING);
  } else if (elf_elfheader (abfd)->e_machine == E_DADAO_MACH_WUQING) {
      elf_elfheader (abfd)->e_machine = EM_DADAO;
      elf_elfheader (abfd)->e_flags &= ~EF_DADAO_SET_MACH (~0);
      elf_elfheader (abfd)->e_flags |= EF_DADAO_SET_MACH (E_DADAO_MACH_WUQING);
  } else if (elf_elfheader (abfd)->e_machine == E_DADAO_MACH_WUMING) {
      elf_elfheader (abfd)->e_machine = EM_DADAO;
      elf_elfheader (abfd)->e_flags &= ~EF_DADAO_SET_MACH (~0);
      elf_elfheader (abfd)->e_flags |= EF_DADAO_SET_MACH (E_DADAO_MACH_WUMING);
  }

  if (elf_elfheader (abfd)->e_machine == EM_DADAO) {
      int e_mach = EF_DADAO_MACH (elf_elfheader (abfd)->e_flags);

      switch (e_mach) {
	case E_DADAO_MACH_WUXING:
	case E_DADAO_MACH_WUQING:
	case E_DADAO_MACH_WUMING:
	  if (!bfd_default_set_arch_mach (abfd, bfd_arch_dadao, e_mach))
	    return FALSE;
	default:
	  break;
      }
  }

  return TRUE;
}

static bfd_boolean
elf64_dadao_section_from_shdr (bfd * abfd,
			     Elf_Internal_Shdr * hdr,
			     const char *name, int shindex)
{
  switch (hdr->sh_type) {
    case SHT_DADAO_INITREG:
    case SHT_DADAO_UDEBUG:
      return _bfd_elf_make_section_from_shdr (abfd, hdr, name, shindex);
    default:
      return FALSE;
  }
}

static bfd_reloc_status_type
elf64_dadao_reloc (bfd * abfd ATTRIBUTE_UNUSED,
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
elf64_dadao_info_to_howto (bfd * abfd ATTRIBUTE_UNUSED,
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
  cache_ptr->howto = &elf_dadao_howto_table[r_type];
  return TRUE;
}

static reloc_howto_type *
elf64_dadao_reloc_type_lookup (bfd * abfd ATTRIBUTE_UNUSED,
			     bfd_reloc_code_real_type code ATTRIBUTE_UNUSED)
{
  return NULL;
}

static reloc_howto_type *
elf64_dadao_reloc_name_lookup (bfd * abfd ATTRIBUTE_UNUSED,
			     const char *r_name ATTRIBUTE_UNUSED)
{
  return NULL;
}

/* We don't actually apply any relocations in this toolset
   so we make them all do nothing, but at least display useful
   names.
   Most of these are mainly used by the DADAO toolchain to resolve things
   before the final ELF file is created.  */
static reloc_howto_type elf_dadao_howto_table[] =
{
  HOWTO (R_DADAO_NOTYPE,		/* Type.  */
	 0,				/* Rightshift.  */
	 3,				/* Size.  */
	 0,				/* Bitsize.  */
	 FALSE,				/* PC_relative.  */
	 0,				/* Bitpos.  */
	 complain_overflow_dont,	/* Complain_on_overflow.  */
	 elf64_dadao_reloc,		/* Special_function.  */
	 "R_DADAO_NOTYPE",		/* Name.  */
	 FALSE,				/* Partial_inplace.  */
	 0,				/* Src_mask.  */
	 0,				/* Dst_mask.  */
	 FALSE),			/* PCrel_offset.  */
  HOWTO (R_DADAO_IMM12, 0, 3, 0, FALSE, 0,
	 complain_overflow_dont, elf64_dadao_reloc,
	 "R_DADAO_IMM12",
	 FALSE, 0, 0, FALSE),
  HOWTO (R_DADAO_IMM18, 0, 3, 0, FALSE, 0,
	 complain_overflow_dont, elf64_dadao_reloc,
	 "R_DADAO_IMM18",
	 FALSE, 0, 0, FALSE),
  HOWTO (R_DADAO_IMM24, 0, 3, 0, FALSE, 0,
	 complain_overflow_dont, elf64_dadao_reloc,
	 "R_DADAO_IMM24",
	 FALSE, 0, 0, FALSE)
};

#define ELF_ARCH				bfd_arch_dadao
#define ELF_MACHINE_CODE			EM_DADAO
#define ELF_MACHINE_ALT1			E_DADAO_MACH_WUXING
#define ELF_MAXPAGESIZE				0x1000
#define TARGET_BIG_NAME				"elf64-dadao"
#define TARGET_BIG_SYM				dadao_elf64_vec

#define elf_backend_object_p			elf64_dadao_object_p
#define elf_backend_section_from_shdr		elf64_dadao_section_from_shdr
#define elf_info_to_howto			elf64_dadao_info_to_howto
#define bfd_elf64_bfd_reloc_type_lookup		elf64_dadao_reloc_type_lookup
#define bfd_elf64_bfd_reloc_name_lookup		elf64_dadao_reloc_name_lookup

#include "elf64-target.h"
