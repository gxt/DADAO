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

#define ELF_MACHINE_NAME "dadao"

#define ELF_MACHINE_NO_REL	1
#define ELF_MACHINE_NO_RELA	0

#define ELF_MACHINE_JMP_SLOT	R_DADAO_JUMP_SLOT

#define elf_machine_type_class(type)					\
	((type) == R_DADAO_JUMP_SLOT)

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
		       const ElfW(Rel) *reloc,
		       ElfW(Addr) *reloc_addr, ElfW(Addr) value)
{
  return *reloc_addr = value;
}

inline Elf64_Addr
__attribute__ ((always_inline))
elf_machine_rela (Elf64_Addr loadaddr, Elf64_Dyn *info[DT_NUM],
		  const Elf64_Rel *reloc, const Elf64_Sym *sym,
		  Elf64_Addr (*resolve) (const Elf64_Sym **ref,
					 Elf64_Addr reloc_addr,
					 int noplt))
{
	/* FIXME
  _dl_signal_error (0, "Elf64_Rela relocation requested -- unused on "
		    NULL, ELF_MACHINE_NAME);
	 */
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

inline void
__attribute__ ((always_inline))
elf_machine_lazy_rel (struct link_map *map,
		      Elf64_Addr l_addr, const Elf64_Rela *reloc,
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


/* Initial entry point code for the dynamic linker.
   The C function `_dl_start' is the real entry point;
   its return value is the user program's entry point.  */

#define RTLD_START #error need some startup code

#endif
