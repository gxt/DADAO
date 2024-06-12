/*
 * Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
 * Contributed by Guan Xuetao <gxt@pku.edu.cn>
 */

/* Machine-dependent ELF dynamic relocation inline functions.  Stub version.  */
#ifndef dl_machine_h
#define dl_machine_h

#include <sysdep.h>
#include <tls.h>
#include <string.h>
#include <link.h>
#include <dl-irel.h>

#define ELF_MACHINE_NAME "dadao"

#define ELF_MACHINE_NO_REL	1
#define ELF_MACHINE_NO_RELA	0

#define ELF_MACHINE_JMP_SLOT	R_DADAO_JUMP_SLOT

#define elf_machine_type_class(type)					\
	( (( (type) == R_DADAO_JUMP_SLOT) * ELF_RTYPE_CLASS_PLT)	\
	| (( (type) == R_DADAO_COPY	) * ELF_RTYPE_CLASS_COPY))

/* Return nonzero iff ELF header is compatible with the running host.  */
static inline int
elf_machine_matches_host (const Elf64_Ehdr *ehdr)
{
  return ehdr->e_machine == EM_DADAO;
}

/* Return the link-time address of _DYNAMIC.  */
static inline Elf64_Addr
elf_machine_dynamic (void)
{
  extern const ElfW(Addr) _GLOBAL_OFFSET_TABLE_[] attribute_hidden;
  return _GLOBAL_OFFSET_TABLE_[0];
}

/* Return the run-time load address of the shared object.  */
static inline Elf64_Addr
elf_machine_load_address (void)
{
  extern ElfW(Dyn) _DYNAMIC[] attribute_hidden;
  return (ElfW(Addr)) &_DYNAMIC - elf_machine_dynamic ();
}

/* Fixup a PLT entry to bounce directly to the function at VALUE.  */
static inline ElfW(Addr)
elf_machine_fixup_plt (struct link_map *map, lookup_t t,
		       const ElfW(Sym) *refsym, const ElfW(Sym) *sym,
		       const ElfW(Rela) *reloc,
		       ElfW(Addr) *reloc_addr, ElfW(Addr) value)
{
  return *reloc_addr = value;
}

/* Return the final value of a plt relocation.  */
static inline ElfW(Addr)
elf_machine_plt_value (struct link_map *map,
		       const ElfW(Rela) *reloc,
		       ElfW(Addr) value)
{
  return value;
}

/* Set up the loaded object described by L so its unrelocated PLT
   entries will jump to the on-demand fixup code in dl-runtime.c.  */
static inline int
elf_machine_runtime_setup (struct link_map *l, int lazy, int profile)
{
  extern void _dl_runtime_resolve (Elf64_Word);

  if (lazy)
    {
      /* The GOT entries for functions in the PLT have not yet been filled
         in.  Their initial contents will arrange when called to push an
         offset into the .rel.plt section, push _GLOBAL_OFFSET_TABLE_[1],
         and then jump to _GLOBAL_OFFSET_TABLE[2].  */
      Elf64_Addr *got = (Elf64_Addr *) D_PTR (l, l_info[DT_PLTGOT]);
      got[1] = (Elf64_Addr) l;	/* Identify this shared object.  */

      /* This function will get called to fix up the GOT entry indicated by
         the offset on the stack, and then jump to the resolved address.  */
      got[2] = (Elf64_Addr) &_dl_runtime_resolve;
    }

  return lazy;
}

/* Initial entry point code for the dynamic linker.
   The C function `_dl_start' is the real entry point;
   its return value is the user program's entry point.  */

#define RTLD_START	asm ("			\
	.text					\n\
	.global	_start				\n\
	.type	_start, @function		\n\
_start:						\n\
	swym					\n\
	call	_dl_start			\n\
						\n\
	.global	_dl_start_user			\n\
	.type	_dl_start_user, @function	\n\
_dl_start_user:					\n\
	swym					\n\
	call	_dl_init			\n\
	swym					\n\
	jump	_dl_fini			\n\
");

/* Names of the architecture-specific auditing callback functions.  */
#define ARCH_LA_PLTENTER		dadao_gnu_pltenter
#define ARCH_LA_PLTEXIT			dadao_gnu_pltexit

#endif	/* dl_machine_h */

#ifdef	RESOLVE_MAP

inline void __attribute__ ((always_inline))
elf_machine_rela (struct link_map *map, const ElfW(Rela) *reloc,
		  const ElfW(Sym) *sym, const struct r_found_version *version,
		  void *const reloc_addr_arg, int skip_ifunc)
{ /* FIXME */
  ElfW(Addr) *const reloc_addr = reloc_addr_arg;
  const unsigned int r_type = ELFW (R_TYPE) (reloc->r_info);

  if (__builtin_expect (r_type == R_DADAO_RELATIVE, 0))
      *reloc_addr = map->l_addr + reloc->r_addend;
  else if (__builtin_expect (r_type == R_DADAO_NONE, 0))
      return;
  else
    {
      const ElfW(Sym) *const refsym = sym;
      struct link_map *sym_map = RESOLVE_MAP (&sym, version, r_type);
      ElfW(Addr) value = SYMBOL_ADDRESS (sym_map, sym, true);

      if (sym != NULL
	  && __glibc_unlikely (ELFW(ST_TYPE) (sym->st_info) == STT_GNU_IFUNC)
	  && __glibc_likely (sym->st_shndx != SHN_UNDEF)
	  && __glibc_likely (!skip_ifunc))
	value = elf_ifunc_invoke (value);

      switch (r_type)
	{
	case R_DADAO_COPY:
	  if (sym == NULL)
	      break;

	  if (sym->st_size > refsym->st_size
	      || (GLRO(dl_verbose) && sym->st_size < refsym->st_size))
	    {
	      const char *strtab;

	      strtab = (const void *) D_PTR (map, l_info[DT_STRTAB]);
	      _dl_error_printf ("\
%s: Symbol `%s' has different size in shared object, consider re-linking\n",
				RTLD_PROGNAME, strtab + refsym->st_name);
	    }
	  memcpy (reloc_addr_arg, (void *) value,
		  sym->st_size < refsym->st_size
		  ? sym->st_size : refsym->st_size);
	  break;

	case R_DADAO_RELATIVE:
	case R_DADAO_JUMP_SLOT:
	  *reloc_addr = value + reloc->r_addend;
	  break;

	default:
	  _dl_reloc_bad_type (map, r_type, 0);
	  break;
	}
    }
}

inline void __attribute__ ((always_inline))
elf_machine_lazy_rel (struct link_map *map,
		      ElfW(Addr) l_addr, const ElfW(Rela) *reloc,
		      int skip_ifunc)
{
  Elf64_Addr * const reloc_addr = (void *)(l_addr + reloc->r_offset);
  unsigned long int const r_type = ELF64_R_TYPE (reloc->r_info);

  if (r_type == R_DADAO_JUMP_SLOT)
    {
      /* Perform a RELATIVE reloc on the .got entry that transfers
	 to the .plt.  */
      *reloc_addr += l_addr;
    }
  else if (r_type == R_DADAO_NONE)
    return;
  else
    _dl_reloc_bad_type (map, r_type, 1);
}

inline void __attribute__ ((always_inline))
elf_machine_rela_relative (ElfW(Addr) l_addr,
			   const ElfW(Rela) *reloc,
			   void *const reloc_addr_arg)
{
  ElfW(Addr) *const reloc_addr = reloc_addr_arg;
  *reloc_addr = l_addr + reloc->r_addend;
}

#endif	/* RESOLVE_MAP */
