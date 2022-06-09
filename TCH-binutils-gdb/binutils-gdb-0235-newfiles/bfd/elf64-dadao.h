extern bfd_vma
_bfd_dadao_elf_resolve_relocation (bfd *, bfd_reloc_code_real_type, bfd_vma,
				     bfd_vma, bfd_vma, bfd_boolean);

/* Take the PAGE component of an address or offset.  */
#define PG(x)	     ((x) & ~ (bfd_vma) 0xfff)
#define PG_OFFSET(x) ((x) &   (bfd_vma) 0xfff)
