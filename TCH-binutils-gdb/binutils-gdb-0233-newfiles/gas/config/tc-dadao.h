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

/* See gas/doc/internals.texi for explanation of these macros.  */
#define TARGET_FORMAT			"elf64-dadao"
#define TARGET_ARCH			bfd_arch_dadao
#define TARGET_BYTES_BIG_ENDIAN		1

extern void dadao_md_assemble (char *);
extern void dadao_md_begin (void);
extern void dadao_md_end (void);
extern void dadao_md_elf_section_change_hook (void);

#define md_assemble			dadao_md_assemble
#define md_begin			dadao_md_begin
#define md_end				dadao_md_end
#define md_elf_section_change_hook	dadao_md_elf_section_change_hook

extern void dadao_md_do_align (int, char *, int, int);
#define md_do_align(n, fill, len, max, label)	dadao_md_do_align (n, fill, len, max)

#define md_undefined_symbol(x) NULL
#define	md_operand(x)

#define md_number_to_chars number_to_chars_bigendian

#define WORKING_DOT_WORD

extern const struct relax_type dadao_relax_table[];
#define TC_GENERIC_RELAX_TABLE dadao_relax_table

#define tc_fix_adjustable(FIX)					\
 (((FIX)->fx_addsy == NULL					\
   || S_GET_SEGMENT ((FIX)->fx_addsy) != reg_section)		\
  && (FIX)->fx_r_type != BFD_RELOC_VTABLE_INHERIT		\
  && (FIX)->fx_r_type != BFD_RELOC_VTABLE_ENTRY)

/* Here's where we make all symbols global, when so requested.
   We must avoid doing that for expression symbols or section symbols,
   though.  */
#define tc_frob_symbol(sym, punt)				\
  do								\
    {								\
      if (S_GET_SEGMENT (sym) == reg_section)			\
	{							\
	  if (S_GET_NAME (sym)[0] != '$'			\
	      && S_GET_VALUE (sym) < 256)			\
	    {							\
		symbol_mark_used_in_reloc (sym);		\
	    }							\
	}							\
    }								\
  while (0)

/* No shared lib support, so we don't need to ensure externally
   visible symbols can be overridden.  */
#define EXTERN_FORCE_RELOC 0

/* When relaxing, we need to emit various relocs we otherwise wouldn't.  */
#define TC_FORCE_RELOCATION(fix) dadao_force_relocation (fix)
extern int dadao_force_relocation (struct fix *);

/* Call md_pcrel_from_section(), not md_pcrel_from().  */
#define MD_PCREL_FROM_SECTION(FIX, SEC) md_pcrel_from_section (FIX, SEC)
extern long md_pcrel_from_section (struct fix *, segT);

#define md_section_align(seg, size) (size)

#define LISTING_HEADER "GAS for DADAO"

/* The default of 4 means Bcc expansion looks like it's missing a line.  */
#define LISTING_LHS_CONT_LINES 5

extern fragS *dadao_opcode_frag;
#define TC_FRAG_TYPE fragS *
#define TC_FRAG_INIT(frag, max_bytes) (frag)->tc_frag_data = dadao_opcode_frag

/* Each insn is a tetrabyte (4 bytes) long, but if there are BYTE
   sequences sprinkled in, we can get unaligned DWARF2 offsets, so let's
   explicitly say one byte.  */
#define DWARF2_LINE_MIN_INSN_LENGTH 1

/* This target is buggy, and sets fix size too large.  */
#define TC_FX_SIZE_SLACK(FIX) 6

/* DADAO has global register symbols.  */
#define TC_GLOBAL_REGISTER_SYMBOL_OK

#endif /* TC_DADAO */
