/*
 * DADAO-specific support for 64-bit ELF
 * Copyright (C) 2019-2033 Guan Xuetao (AT) Peking Univ.
 *
 * codestyle: clangFormat
 * Contributed by:
 *   2019:
 *	Guan Xuetao <gxt@pku.edu.cn>
 *  Yang Chen <yang_chen@pku.edu.cn>
 * This file is part of BFD, the Binary File Descriptor library.
 */
#include "sysdep.h"
#include "bfd.h"
#include "libbfd.h"
#include "elf-bfd.h"
#include "elf/dadao.h"
#include "opcode/dadao.h"
#include "elf64-dadao.h"

#define MASK(n) ((1u << (n)) - 1)

#define MINUS_ONE (((bfd_vma)0) - 1)

#define MATCH(opcode, insn) ((opcode & 0xFF000000) == insn)

static int dd_fill_abs (unsigned int opcode,unsigned int reg, bfd_vma addr, bfd* abfd, void* datap) {
    int tmp = 0;
    unsigned int imm_w16_1, imm_w16_2, imm_w16_3, imm_w16_4;
    imm_w16_1 = (addr) & 0xFFFF;
    imm_w16_2 = (addr >> 16) & 0xFFFF;
    imm_w16_3 = (addr >> 32) & 0xFFFF;
    imm_w16_4 = (addr >> 48) & 0xFFFF;
    unsigned int insn_type;
    if(opcode == DADAO_INSN_SETZW_RB) {
        insn_type = DADAO_INSN_ORW_RB;
    }
    else if (opcode == DADAO_INSN_SETZW_RD) {
        insn_type = DADAO_INSN_ORW_RD;
    }
    else if (opcode == DADAO_INSN_SETOW_RD) {
        insn_type = DADAO_INSN_ANDNW_RD;
    }

    if (imm_w16_1) {
        bfd_put_32(abfd, opcode | reg | DADAO_WYDE_WL | imm_w16_1, (bfd_byte *)datap);
        tmp = 4;
    }
    if (imm_w16_2) {
        bfd_put_32(abfd, (tmp == 0 ? opcode : insn_type) | reg | DADAO_WYDE_WK | imm_w16_2, (bfd_byte *)datap + tmp);
        tmp = 4;
    }
    if (imm_w16_3) {
        bfd_put_32(abfd, (tmp == 0 ? opcode : insn_type) | reg | DADAO_WYDE_WJ | imm_w16_3, (bfd_byte *)datap + tmp);
        tmp = 4;
    }
    if (imm_w16_4) {
        bfd_put_32(abfd, (tmp == 0 ? opcode : insn_type) | reg | DADAO_WYDE_WH | imm_w16_4, (bfd_byte *)datap + tmp);
        tmp = 4;
    }
    /* If set 0 to reg, then just use opcode */
    if (!tmp) {
        bfd_put_32(abfd, opcode | reg | DADAO_WYDE_WL | imm_w16_1, (bfd_byte *)datap + tmp);
        tmp = 4;
    }
    return tmp;
}

/* Put these everywhere in new code.  */
#define FATAL_DEBUG                \
    _bfd_abort(__FILE__, __LINE__, \
               "Internal: Non-debugged code (test-case missing)")

#define BAD_CASE(x)                \
    _bfd_abort(__FILE__, __LINE__, \
               "bad case for " #x)

#define ELF_DYNAMIC_INTERPRETER "/usr/lib/ld.so"

static bfd_reloc_status_type
dadao_elf_reloc(bfd *, arelent *, asymbol *, void *,
                asection *, bfd *, char **);
static bfd_reloc_status_type
dadao_final_link_relocate(reloc_howto_type *, asection *, bfd_byte *, bfd_vma,
                          bfd_signed_vma, bfd_vma, const char *, asection *,
                          char **);
/* Internal relocations used exclusively by the relaxation pass.  */

#define DADAO_ELF_WORD_BYTES 8

/* Reencode the imm field of add immediate.  */
static inline uint32_t
reencode_add_imm (uint32_t insn, uint32_t imm)
{
  return (insn & ~(MASK (12) << 10)) | ((imm & MASK (12)) << 10);
}

/* Reencode the IMM field of ADR.  */

uint32_t
_bfd_dadao_reencode_adr_imm (uint32_t insn, uint32_t imm)
{
  return (insn & ~((MASK (2) << 29) | (MASK (19) << 5)))
    | ((imm & MASK (2)) << 29) | ((imm & (MASK (19) << 2)) << 3);
}

/* Reencode the imm field of ld/st pos immediate.  */
static inline uint32_t
reencode_ldst_pos_imm (uint32_t insn, uint32_t imm)
{
  return (insn & ~(MASK (12) << 10)) | ((imm & MASK (12)) << 10);
}

/* DADAO ELF linker hash entry.  */

struct dadao_elf_link_hash_entry
{
  struct elf_link_hash_entry elf;

#define GOT_UNKNOWN     0
#define GOT_NORMAL      1
#define GOT_TLS_GD      2
#define GOT_TLS_IE      4
#define GOT_TLS_LE      8
  char tls_type;
};

#define dadao_elf_hash_entry(ent) \
  ((struct dadao_elf_link_hash_entry *)(ent))

struct _bfd_dadao_elf_obj_tdata
{
  struct elf_obj_tdata root;

  /* tls_type for each local got entry.  */
  char *local_got_tls_type;
};

#define _bfd_dadao_elf_tdata(abfd) \
  ((struct _bfd_dadao_elf_obj_tdata *) (abfd)->tdata.any)

#define _bfd_dadao_elf_local_got_tls_type(abfd) \
  (_bfd_dadao_elf_tdata (abfd)->local_got_tls_type)

#define _bfd_dadao_elf_tls_type(abfd, h, symndx)		\
  (*((h) != NULL ? &dadao_elf_hash_entry (h)->tls_type		\
     : &_bfd_dadao_elf_local_got_tls_type (abfd) [symndx]))

#define is_dadao_elf(bfd)				\
  (bfd_get_flavour (bfd) == bfd_target_elf_flavour	\
   && elf_tdata (bfd) != NULL				\
   && elf_object_id (bfd) == DADAO_ELF_DATA)

#define _bfd_riscv_elf_tdata(abfd) \
  ((struct _bfd_riscv_elf_obj_tdata *) (abfd)->tdata.any)

#define _bfd_dadao_elf_local_got_tls_type(abfd) \
  (_bfd_dadao_elf_tdata (abfd)->local_got_tls_type)

#include "elf/common.h"
#include "elf/internal.h"

struct dadao_elf_link_hash_table
{
  struct elf_link_hash_table elf;

  /* Short-cuts to get to dynamic linker sections.  */
  asection *sdyntdata;

  /* Small local sym to section mapping cache.  */
  struct sym_cache sym_cache;

  /* The max alignment of output sections.  */
  bfd_vma max_alignment;
};

/* Get the DADAO ELF linker hash table from a link_info structure.  */
#define dadao_elf_hash_table(p) \
  (elf_hash_table_id ((struct elf_link_hash_table *) ((p)->hash)) \
  == DADAO_ELF_DATA ? ((struct dadao_elf_link_hash_table *) ((p)->hash)) : NULL)

static void
dadao_elf_append_rela (bfd *abfd, asection *s, Elf_Internal_Rela *rel)
{
  const struct elf_backend_data *bed;
  bfd_byte *loc;

  bed = get_elf_backend_data (abfd);
  loc = s->contents + (s->reloc_count++ * bed->s->sizeof_rela);
  bed->s->swap_reloca_out (abfd, rel, loc);
}

/* PLT/GOT stuff.  */

#define PLT_HEADER_INSNS 8
#define PLT_ENTRY_INSNS 4
#define PLT_HEADER_SIZE (PLT_HEADER_INSNS * 4)
#define PLT_ENTRY_SIZE (PLT_ENTRY_INSNS * 4)

#define GOT_ENTRY_SIZE 8

#define GOTPLT_HEADER_SIZE (2 * GOT_ENTRY_SIZE)

#define sec_addr(sec) ((sec)->output_section->vma + (sec)->output_offset)

static bfd_vma
dadao_elf_got_plt_val (bfd_vma plt_index, struct bfd_link_info *info)
{
  return sec_addr (dadao_elf_hash_table (info)->elf.sgotplt)
	 + GOTPLT_HEADER_SIZE + (plt_index * GOT_ENTRY_SIZE);
}

/* DADAO ELF linker hash table.  */
struct elf_dadao_link_hash_table
{
  /* The main hash table.  */
  struct elf_link_hash_table root;

  /* Nonzero to force PIC branch veneers.  */
  int pic_veneer;

  /* Fix erratum 835769.  */
  int fix_erratum_835769;

  /* Don't apply link-time values for dynamic relocations.  */
  int no_apply_dynamic_relocs;

  /* The number of bytes in the initial entry in the PLT.  */
  bfd_size_type plt_header_size;

  /* The bytes of the initial PLT entry.  */
  const bfd_byte *plt0_entry;

  /* The number of bytes in the subsequent PLT entries.  */
  bfd_size_type plt_entry_size;

  /* The bytes of the subsequent PLT entry.  */
  const bfd_byte *plt_entry;

  /* Small local sym cache.  */
  struct sym_cache sym_cache;

  /* For convenience in allocate_dynrelocs.  */
  bfd *obfd;

  /* The amount of space used by the reserved portion of the sgotplt
     section, plus whatever space is used by the jump slots.  */
  bfd_vma sgotplt_jump_table_size;

  /* The stub hash table.  */
  struct bfd_hash_table stub_hash_table;

  /* Linker stub bfd.  */
  bfd *stub_bfd;

  /* Linker call-backs.  */
  asection *(*add_stub_section) (const char *, asection *);
  void (*layout_sections_again) (void);

  /* Array to keep track of which stub sections have been created, and
     information on stub grouping.  */
  struct map_stub
  {
    /* This is the section to which stubs in the group will be
       attached.  */
    asection *link_sec;
    /* The stub section.  */
    asection *stub_sec;
  } *stub_group;

  /* Assorted information used by elf64_dadao_size_stubs.  */
  unsigned int bfd_count;
  unsigned int top_index;
  asection **input_list;

  /* JUMP_SLOT relocs for variant PCS symbols may be present.  */
  int variant_pcs;

  /* The number of bytes in the PLT enty for the TLS descriptor.  */
  bfd_size_type tlsdesc_plt_entry_size;

  /* Used by local STT_GNU_IFUNC symbols.  */
  htab_t loc_hash_table;
  void * loc_hash_memory;
};

/* Create the .got section.  */

static bfd_boolean
dadao_elf_create_got_section (bfd *abfd, struct bfd_link_info *info)
{
  flagword flags;
  asection *s, *s_got;
  struct elf_link_hash_entry *h;
  const struct elf_backend_data *bed = get_elf_backend_data (abfd);
  struct elf_link_hash_table *htab = elf_hash_table (info);

  /* This function may be called more than once.  */
  if (htab->sgot != NULL)
    return TRUE;

  flags = bed->dynamic_sec_flags;

  s = bfd_make_section_anyway_with_flags (abfd,
                                          (bed->rela_plts_and_copies_p
                                           ? ".rela.got" : ".rel.got"),
                                          (bed->dynamic_sec_flags
                                           | SEC_READONLY));
  if (s == NULL
      || !bfd_set_section_alignment (s, bed->s->log_file_align))
    return FALSE;
  htab->srelgot = s;

  s = s_got = bfd_make_section_anyway_with_flags (abfd, ".got", flags);
  if (s == NULL
      || !bfd_set_section_alignment (s, bed->s->log_file_align))
    return FALSE;
  htab->sgot = s;

  /* The first bit of the global offset table is the header.  */
  s->size += bed->got_header_size;

  if (bed->want_got_plt)
    {
      s = bfd_make_section_anyway_with_flags (abfd, ".got.plt", flags);
      if (s == NULL
          || !bfd_set_section_alignment (s, bed->s->log_file_align))
        return FALSE;
      htab->sgotplt = s;

      /* Reserve room for the header.  */
      s->size += GOTPLT_HEADER_SIZE;
    }

  if (bed->want_got_sym)
    {
      /* Define the symbol _GLOBAL_OFFSET_TABLE_ at the start of the .got
         section.  We don't do this in the linker script because we don't want
         to define the symbol if we are not creating a global offset
         table.  */
      h = _bfd_elf_define_linkage_sym (abfd, info, s_got,
                                       "_GLOBAL_OFFSET_TABLE_");
      elf_hash_table (info)->hgot = h;
      if (h == NULL)
        return FALSE;
    }

  return TRUE;
}

/* Create .plt, .rela.plt, .got, .got.plt, .rela.got, .dynbss, and
   .rela.bss sections in DYNOBJ, and set up shortcuts to them in our
   hash table.  */

static bfd_boolean
dadao_elf_create_dynamic_sections (bfd *dynobj,
                                   struct bfd_link_info *info)
{
  struct dadao_elf_link_hash_table *htab;

  htab = dadao_elf_hash_table (info);
  BFD_ASSERT (htab != NULL);

  if (!dadao_elf_create_got_section (dynobj, info))
    return FALSE;

  if (!_bfd_elf_create_dynamic_sections (dynobj, info))
    return FALSE;

  if (!bfd_link_pic (info))
    {
      /* Technically, this section doesn't have contents.  It is used as the
         target of TLS copy relocs, to copy TLS data from shared libraries into
         the executable.  However, if we don't mark it as loadable, then it
         matches the IS_TBSS test in ldlang.c, and there is no run-time address
         space allocated for it even though it has SEC_ALLOC.  That test is
         correct for .tbss, but not correct for this section.  There is also
         a second problem that having a section with no contents can only work
         if it comes after all sections with contents in the same segment,
         but the linker script does not guarantee that.  This is just mixed in
         with other .tdata.* sections.  We can fix both problems by lying and
         saying that there are contents.  This section is expected to be small
         so this should not cause a significant extra program startup cost.  */
      htab->sdyntdata =
        bfd_make_section_anyway_with_flags (dynobj, ".tdata.dyn",
                                            (SEC_ALLOC | SEC_THREAD_LOCAL
                                             | SEC_LOAD | SEC_DATA
                                             | SEC_HAS_CONTENTS
                                             | SEC_LINKER_CREATED));
    }

  if (!htab->elf.splt || !htab->elf.srelplt || !htab->elf.sdynbss
      || (!bfd_link_pic (info) && (!htab->elf.srelbss || !htab->sdyntdata)))
    abort ();

  return TRUE;
}

/* Allocate space in .plt, .got and associated reloc sections for
   dynamic relocs.  */

static bfd_boolean
allocate_dynrelocs (struct elf_link_hash_entry *h, void *inf)
{
  struct bfd_link_info *info;
  struct dadao_elf_link_hash_table *htab;
  struct elf_dyn_relocs *p;

  if (h->root.type == bfd_link_hash_indirect)
    return TRUE;

  info = (struct bfd_link_info *) inf;
  htab = dadao_elf_hash_table (info);
  BFD_ASSERT (htab != NULL);

  if (htab->elf.dynamic_sections_created
      && h->plt.refcount > 0)
    {
      /* Make sure this symbol is output as a dynamic symbol.
         Undefined weak syms won't yet be marked as dynamic.  */
      if (h->dynindx == -1
          && !h->forced_local)
        {
          if (! bfd_elf_link_record_dynamic_symbol (info, h))
            return FALSE;
        }

      if (WILL_CALL_FINISH_DYNAMIC_SYMBOL (1, bfd_link_pic (info), h))
        {
          asection *s = htab->elf.splt;

          if (s->size == 0)
            s->size = PLT_HEADER_SIZE;

          h->plt.offset = s->size;

          /* Make room for this entry.  */
          s->size += PLT_ENTRY_SIZE;

          /* We also need to make an entry in the .got.plt section.  */
          htab->elf.sgotplt->size += GOT_ENTRY_SIZE;

          /* We also need to make an entry in the .rela.plt section.  */
          htab->elf.srelplt->size += sizeof (Elf64_External_Rela);

          /* If this symbol is not defined in a regular file, and we are
             not generating a shared library, then set the symbol to this
             location in the .plt.  This is required to make function
             pointers compare as equal between the normal executable and
             the shared library.  */
          if (! bfd_link_pic (info)
              && !h->def_regular)
            {
              h->root.u.def.section = s;
              h->root.u.def.value = h->plt.offset;
            }
        }
      else
        {
          h->plt.offset = (bfd_vma) -1;
          h->needs_plt = 0;
        }
    }
  else
    {
      h->plt.offset = (bfd_vma) -1;
      h->needs_plt = 0;
    }
  if (h->got.refcount > 0)
    {
      asection *s;
      bfd_boolean dyn;
      int tls_type = dadao_elf_hash_entry (h)->tls_type;

      /* Make sure this symbol is output as a dynamic symbol.
         Undefined weak syms won't yet be marked as dynamic.  */
      if (h->dynindx == -1
          && !h->forced_local)
        {
          if (! bfd_elf_link_record_dynamic_symbol (info, h))
            return FALSE;
        }

      s = htab->elf.sgot;
      h->got.offset = s->size;
      dyn = htab->elf.dynamic_sections_created;
      if (tls_type & (GOT_TLS_GD | GOT_TLS_IE))
        {
          /* TLS_GD needs two dynamic relocs and two GOT slots.  */
          if (tls_type & GOT_TLS_GD)
            {
              s->size += 2 * DADAO_ELF_WORD_BYTES;
              htab->elf.srelgot->size += 2 * sizeof (Elf64_External_Rela);
            }

          /* TLS_IE needs one dynamic reloc and one GOT slot.  */
          if (tls_type & GOT_TLS_IE)
            {
              s->size += DADAO_ELF_WORD_BYTES;
              htab->elf.srelgot->size += sizeof (Elf64_External_Rela);
            }
        }
      else
        {
          s->size += DADAO_ELF_WORD_BYTES;
          if (WILL_CALL_FINISH_DYNAMIC_SYMBOL (dyn, bfd_link_pic (info), h)
              && ! UNDEFWEAK_NO_DYNAMIC_RELOC (info, h))
            htab->elf.srelgot->size += sizeof (Elf64_External_Rela);
        }
    }
  else
    h->got.offset = (bfd_vma) -1;

  if (h->dyn_relocs == NULL)
    return TRUE;

  /* In the shared -Bsymbolic case, discard space allocated for
     dynamic pc-relative relocs against symbols which turn out to be
     defined in regular objects.  For the normal shared case, discard
     space for pc-relative relocs that have become local due to symbol
     visibility changes.  */

  if (bfd_link_pic (info))
    {
      if (SYMBOL_CALLS_LOCAL (info, h))
        {
          struct elf_dyn_relocs **pp;

          for (pp = &h->dyn_relocs; (p = *pp) != NULL; )
            {
              p->count -= p->pc_count;
              p->pc_count = 0;
              if (p->count == 0)
                *pp = p->next;
              else
                pp = &p->next;
            }
        }
      /* Also discard relocs on undefined weak syms with non-default
         visibility.  */
      if (h->dyn_relocs != NULL
          && h->root.type == bfd_link_hash_undefweak)
        {
          if (ELF_ST_VISIBILITY (h->other) != STV_DEFAULT
              || UNDEFWEAK_NO_DYNAMIC_RELOC (info, h))
            h->dyn_relocs = NULL;

          /* Make sure undefined weak symbols are output as a dynamic
             symbol in PIEs.  */
          else if (h->dynindx == -1
                   && !h->forced_local)
            {
              if (! bfd_elf_link_record_dynamic_symbol (info, h))
                return FALSE;
            }
        }
    }
  else
    {
      /* For the non-shared case, discard space for relocs against
         symbols which turn out to need copy relocs or are not
         dynamic.  */

      if (!h->non_got_ref
          && ((h->def_dynamic
               && !h->def_regular)
              || (htab->elf.dynamic_sections_created
                  && (h->root.type == bfd_link_hash_undefweak
                      || h->root.type == bfd_link_hash_undefined))))
        {
          /* Make sure this symbol is output as a dynamic symbol.
             Undefined weak syms won't yet be marked as dynamic.  */
          if (h->dynindx == -1
              && !h->forced_local)
            {
              if (! bfd_elf_link_record_dynamic_symbol (info, h))
                return FALSE;
            }

          /* If that succeeded, we know we'll be keeping all the
             relocs.  */
          if (h->dynindx != -1)
            goto keep;
        }

      h->dyn_relocs = NULL;

    keep: ;
    }

  /* Finally, allocate space.  */
  for (p = h->dyn_relocs; p != NULL; p = p->next)
    {
      asection *sreloc = elf_section_data (p->sec)->sreloc;
      sreloc->size += p->count * sizeof (Elf64_External_Rela);
    }

  return TRUE;
}

static bfd_boolean
dadao_elf_size_dynamic_sections (bfd *output_bfd, struct bfd_link_info *info)
{
  struct dadao_elf_link_hash_table *htab;
  bfd *dynobj;
  asection *s;
  bfd *ibfd;

  htab = dadao_elf_hash_table (info);
  BFD_ASSERT (htab != NULL);
  dynobj = htab->elf.dynobj;
  BFD_ASSERT (dynobj != NULL);

  if (elf_hash_table (info)->dynamic_sections_created)
    {
      /* Set the contents of the .interp section to the interpreter.  */
      if (bfd_link_executable (info) && !info->nointerp)
        {
          s = bfd_get_linker_section (dynobj, ".interp");
          BFD_ASSERT (s != NULL);
          s->size = sizeof ELF_DYNAMIC_INTERPRETER;
          s->contents = (unsigned char *) ELF_DYNAMIC_INTERPRETER;
        }
    }

  /* Set up .got offsets for local syms, and space for local dynamic
     relocs.  */
  for (ibfd = info->input_bfds; ibfd != NULL; ibfd = ibfd->link.next)
    {
      bfd_signed_vma *local_got;
      bfd_signed_vma *end_local_got;
      char *local_tls_type;
      bfd_size_type locsymcount;
      Elf_Internal_Shdr *symtab_hdr;
      asection *srel;

      if (! is_dadao_elf (ibfd))
        continue;

      for (s = ibfd->sections; s != NULL; s = s->next)
        {
          struct elf_dyn_relocs *p;

          for (p = elf_section_data (s)->local_dynrel; p != NULL; p = p->next)
            {
              if (!bfd_is_abs_section (p->sec)
                  && bfd_is_abs_section (p->sec->output_section))
                {
                  /* Input section has been discarded, either because
                     it is a copy of a linkonce section or due to
                     linker script /DISCARD/, so we'll be discarding
                     the relocs too.  */
                }
              else if (p->count != 0)
                {
                  srel = elf_section_data (p->sec)->sreloc;
                  srel->size += p->count * sizeof (Elf64_External_Rela);
                  if ((p->sec->output_section->flags & SEC_READONLY) != 0)
                    info->flags |= DF_TEXTREL;
                }
            }
        }

      local_got = elf_local_got_refcounts (ibfd);
      if (!local_got)
        continue;
      symtab_hdr = &elf_symtab_hdr (ibfd);
      locsymcount = symtab_hdr->sh_info;
      end_local_got = local_got + locsymcount;
      local_tls_type = _bfd_dadao_elf_local_got_tls_type (ibfd);
      s = htab->elf.sgot;
      srel = htab->elf.srelgot;
      for (; local_got < end_local_got; ++local_got, ++local_tls_type)
        {
          if (*local_got > 0)
            {
              *local_got = s->size;
              s->size += DADAO_ELF_WORD_BYTES;
              if (*local_tls_type & GOT_TLS_GD)
                s->size += DADAO_ELF_WORD_BYTES;
              if (bfd_link_pic (info)
                  || (*local_tls_type & (GOT_TLS_GD | GOT_TLS_IE)))
                srel->size += sizeof (Elf64_External_Rela);
            }
          else
            *local_got = (bfd_vma) -1;
        }
    }

  /* Allocate global sym .plt and .got entries, and space for global
     sym dynamic relocs.  */
  elf_link_hash_traverse (&htab->elf, allocate_dynrelocs, info);

  if (htab->elf.sgotplt)
    {
      struct elf_link_hash_entry *got;
      got = elf_link_hash_lookup (elf_hash_table (info),
                                  "_GLOBAL_OFFSET_TABLE_",
                                  FALSE, FALSE, FALSE);

      /* Don't allocate .got.plt section if there are no GOT nor PLT
         entries and there is no refeence to _GLOBAL_OFFSET_TABLE_.  */
      if ((got == NULL
           || !got->ref_regular_nonweak)
          && (htab->elf.sgotplt->size == GOTPLT_HEADER_SIZE)
          && (htab->elf.splt == NULL
              || htab->elf.splt->size == 0)
          && (htab->elf.sgot == NULL
              || (htab->elf.sgot->size
                  == get_elf_backend_data (output_bfd)->got_header_size)))
        htab->elf.sgotplt->size = 0;
    }

  /* The check_relocs and adjust_dynamic_symbol entry points have
     determined the sizes of the various dynamic sections.  Allocate
     memory for them.  */
  for (s = dynobj->sections; s != NULL; s = s->next)
    {
      if ((s->flags & SEC_LINKER_CREATED) == 0)
        continue;

      if (s == htab->elf.splt
          || s == htab->elf.sgot
          || s == htab->elf.sgotplt
          || s == htab->elf.sdynbss
          || s == htab->elf.sdynrelro
          || s == htab->sdyntdata)
        {
          /* Strip this section if we don't need it; see the
             comment below.  */
        }
      else if (strncmp (s->name, ".rela", 5) == 0)
        {
          if (s->size != 0)
            {
              /* We use the reloc_count field as a counter if we need
                 to copy relocs into the output file.  */
              s->reloc_count = 0;
            }
        }
      else
        {
          /* It's not one of our sections.  */
          continue;
        }
      if (s->size == 0)
        {
          /* If we don't need this section, strip it from the
             output file.  This is mostly to handle .rela.bss and
             .rela.plt.  We must create both sections in
             create_dynamic_sections, because they must be created
             before the linker maps input sections to output
             sections.  The linker does that before
             adjust_dynamic_symbol is called, and it is that
             function which decides whether anything needs to go
             into these sections.  */
          s->flags |= SEC_EXCLUDE;
          continue;
        }

      if ((s->flags & SEC_HAS_CONTENTS) == 0)
        continue;

      /* Allocate memory for the section contents.  Zero the memory
         for the benefit of .rela.plt, which has 4 unused entries
         at the beginning, and we don't want garbage.  */
      s->contents = (bfd_byte *) bfd_zalloc (dynobj, s->size);
      if (s->contents == NULL)
        return FALSE;
    }

  return _bfd_elf_add_dynamic_tags (output_bfd, info, TRUE);
}

/* Finish up dynamic symbol handling.  We set the contents of various
   dynamic sections here.  */

static bfd_boolean
dadao_elf_finish_dynamic_symbol (bfd *output_bfd,
                                 struct bfd_link_info *info,
                                 struct elf_link_hash_entry *h,
                                 Elf_Internal_Sym *sym)
{
  struct dadao_elf_link_hash_table *htab = dadao_elf_hash_table (info);
  const struct elf_backend_data *bed = get_elf_backend_data (output_bfd);

  if (h->plt.offset != (bfd_vma) -1)
    {
      /* We've decided to create a PLT entry for this symbol.  */
      bfd_byte *loc;
      bfd_vma i, header_address, plt_idx, got_address;
      uint32_t plt_entry[PLT_ENTRY_INSNS];
      Elf_Internal_Rela rela;

      BFD_ASSERT (h->dynindx != -1);

      /* Calculate the address of the PLT header.  */
      header_address = sec_addr (htab->elf.splt);

      /* Calculate the index of the entry.  */
      plt_idx = (h->plt.offset - PLT_HEADER_SIZE) / PLT_ENTRY_SIZE;

      /* Calculate the address of the .got.plt entry.  */
      got_address = dadao_elf_got_plt_val (plt_idx, info);

      /* Find out where the .plt entry should go.  */
      loc = htab->elf.splt->contents + h->plt.offset;

      for (i = 0; i < PLT_ENTRY_INSNS; i++)
        bfd_put_32 (output_bfd, plt_entry[i], loc + 4*i);

      /* Fill in the initial value of the .got.plt entry.  */
      loc = htab->elf.sgotplt->contents
            + (got_address - sec_addr (htab->elf.sgotplt));
      bfd_put_64 (output_bfd, sec_addr (htab->elf.splt), loc);

      /* Fill in the entry in the .rela.plt section.  */
      rela.r_offset = got_address;
      rela.r_addend = 0;
      rela.r_info = ELF64_R_INFO (h->dynindx, R_DADAO_JUMP);

      loc = htab->elf.srelplt->contents + plt_idx * sizeof (Elf64_External_Rela);
      bed->s->swap_reloca_out (output_bfd, &rela, loc);

      if (!h->def_regular)
        {
          /* Mark the symbol as undefined, rather than as defined in
             the .plt section.  Leave the value alone.  */
          sym->st_shndx = SHN_UNDEF;
          /* If the symbol is weak, we do need to clear the value.
             Otherwise, the PLT entry would provide a definition for
             the symbol even if the symbol wasn't defined anywhere,
             and so the symbol would never be NULL.  */
          if (!h->ref_regular_nonweak)
            sym->st_value = 0;
        }
    }
  if (h->got.offset != (bfd_vma) -1
      && !(dadao_elf_hash_entry (h)->tls_type & (GOT_TLS_GD | GOT_TLS_IE))
      && !UNDEFWEAK_NO_DYNAMIC_RELOC (info, h))
    {
      asection *sgot;
      asection *srela;
      Elf_Internal_Rela rela;

      /* This symbol has an entry in the GOT.  Set it up.  */

      sgot = htab->elf.sgot;
      srela = htab->elf.srelgot;
      BFD_ASSERT (sgot != NULL && srela != NULL);

      rela.r_offset = sec_addr (sgot) + (h->got.offset &~ (bfd_vma) 1);

      /* If this is a local symbol reference, we just want to emit a RELATIVE
         reloc.  This can happen if it is a -Bsymbolic link, or a pie link, or
         the symbol was forced to be local because of a version file.
         The entry in the global offset table will already have been
         initialized in the relocate_section function.  */
      if (bfd_link_pic (info)
          && SYMBOL_REFERENCES_LOCAL (info, h))
        {
          BFD_ASSERT((h->got.offset & 1) != 0);
          asection *sec = h->root.u.def.section;
          rela.r_info = ELF64_R_INFO (0, R_DADAO_RELATIVE);
          rela.r_addend = (h->root.u.def.value
                           + sec->output_section->vma
                           + sec->output_offset);
        }
      else
        {
          BFD_ASSERT((h->got.offset & 1) == 0);
          BFD_ASSERT (h->dynindx != -1);
          rela.r_info = ELF64_R_INFO (h->dynindx, R_DADAO_64);
          rela.r_addend = 0;
        }

      bfd_put_64 (output_bfd, 0,
                  sgot->contents + (h->got.offset & ~(bfd_vma) 1));
      dadao_elf_append_rela (output_bfd, srela, &rela);
    }

  if (h->needs_copy)
    {
      Elf_Internal_Rela rela;
      asection *s;

      /* This symbols needs a copy reloc.  Set it up.  */
      BFD_ASSERT (h->dynindx != -1);

      rela.r_offset = sec_addr (h->root.u.def.section) + h->root.u.def.value;
      rela.r_info = ELF64_R_INFO (h->dynindx, R_DADAO_COPY);
      rela.r_addend = 0;
      if (h->root.u.def.section == htab->elf.sdynrelro)
        s = htab->elf.sreldynrelro;
      else
        s = htab->elf.srelbss;
      dadao_elf_append_rela (output_bfd, s, &rela);
    }

  /* Mark some specially defined symbols as absolute.  */
  if (h == htab->elf.hdynamic
      || (h == htab->elf.hgot || h == htab->elf.hplt))
    sym->st_shndx = SHN_ABS;

  return TRUE;
}

/* Watch out: this currently needs to have elements with the same index as
   their R_DADAO_ number.  */
static reloc_howto_type elf_dadao_howto_table[] =
    {
        /* This reloc does nothing.  */
        HOWTO(R_DADAO_NONE,           /* type */
              0,                      /* rightshift */
              3,                      /* size (0 = byte, 1 = short, 2 = long) */
              0,                      /* bitsize */
              FALSE,                  /* pc_relative */
              0,                      /* bitpos */
              complain_overflow_dont, /* complain_on_overflow */
              bfd_elf_generic_reloc,  /* special_function */
              "R_DADAO_NONE",         /* name */
              FALSE,                  /* partial_inplace */
              0,                      /* src_mask */
              0,                      /* dst_mask */
              FALSE),                 /* pcrel_offset */

        /* An 8 bit absolute relocation.  */
        HOWTO(R_DADAO_8,                  /* type */
              0,                          /* rightshift */
              0,                          /* size (0 = byte, 1 = short, 2 = long) */
              8,                          /* bitsize */
              FALSE,                      /* pc_relative */
              0,                          /* bitpos */
              complain_overflow_bitfield, /* complain_on_overflow */
              bfd_elf_generic_reloc,      /* special_function */
              "R_DADAO_8",                /* name */
              FALSE,                      /* partial_inplace */
              0,                          /* src_mask */
              0xff,                       /* dst_mask */
              FALSE),                     /* pcrel_offset */

        /* An 16 bit absolute relocation.  */
        HOWTO(R_DADAO_16,                 /* type */
              0,                          /* rightshift */
              1,                          /* size (0 = byte, 1 = short, 2 = long) */
              16,                         /* bitsize */
              FALSE,                      /* pc_relative */
              0,                          /* bitpos */
              complain_overflow_bitfield, /* complain_on_overflow */
              bfd_elf_generic_reloc,      /* special_function */
              "R_DADAO_16",               /* name */
              FALSE,                      /* partial_inplace */
              0,                          /* src_mask */
              0xffff,                     /* dst_mask */
              FALSE),                     /* pcrel_offset */

        /* An 24 bit absolute relocation.  */
        HOWTO(R_DADAO_24,                 /* type */
              0,                          /* rightshift */
              2,                          /* size (0 = byte, 1 = short, 2 = long) */
              24,                         /* bitsize */
              FALSE,                      /* pc_relative */
              0,                          /* bitpos */
              complain_overflow_bitfield, /* complain_on_overflow */
              bfd_elf_generic_reloc,      /* special_function */
              "R_DADAO_24",               /* name */
              FALSE,                      /* partial_inplace */
              ~0xffffff,                  /* src_mask */
              0xffffff,                   /* dst_mask */
              FALSE),                     /* pcrel_offset */

        /* A 32 bit absolute relocation.  */
        HOWTO(R_DADAO_32,                 /* type */
              0,                          /* rightshift */
              2,                          /* size (0 = byte, 1 = short, 2 = long) */
              32,                         /* bitsize */
              FALSE,                      /* pc_relative */
              0,                          /* bitpos */
              complain_overflow_bitfield, /* complain_on_overflow */
              bfd_elf_generic_reloc,      /* special_function */
              "R_DADAO_32",               /* name */
              FALSE,                      /* partial_inplace */
              0,                          /* src_mask */
              0xffffffff,                 /* dst_mask */
              FALSE),                     /* pcrel_offset */

        /* 64 bit relocation.  */
        HOWTO(R_DADAO_64,                 /* type */
              0,                          /* rightshift */
              4,                          /* size (0 = byte, 1 = short, 2 = long) */
              64,                         /* bitsize */
              FALSE,                      /* pc_relative */
              0,                          /* bitpos */
              complain_overflow_bitfield, /* complain_on_overflow */
              bfd_elf_generic_reloc,      /* special_function */
              "R_DADAO_64",               /* name */
              FALSE,                      /* partial_inplace */
              0,                          /* src_mask */
              MINUS_ONE,                  /* dst_mask */
              FALSE),                     /* pcrel_offset */

        /* An 8 bit PC-relative relocation.  */
        HOWTO(R_DADAO_PC_8,               /* type */
              0,                          /* rightshift */
              0,                          /* size (0 = byte, 1 = short, 2 = long) */
              8,                          /* bitsize */
              TRUE,                       /* pc_relative */
              0,                          /* bitpos */
              complain_overflow_bitfield, /* complain_on_overflow */
              bfd_elf_generic_reloc,      /* special_function */
              "R_DADAO_PC_8",             /* name */
              FALSE,                      /* partial_inplace */
              0,                          /* src_mask */
              0xff,                       /* dst_mask */
              TRUE),                      /* pcrel_offset */

        /* An 16 bit PC-relative relocation.  */
        HOWTO(R_DADAO_PC_16,              /* type */
              0,                          /* rightshift */
              1,                          /* size (0 = byte, 1 = short, 2 = long) */
              16,                         /* bitsize */
              TRUE,                       /* pc_relative */
              0,                          /* bitpos */
              complain_overflow_bitfield, /* complain_on_overflow */
              bfd_elf_generic_reloc,      /* special_function */
              "R_DADAO_PC_16",            /* name */
              FALSE,                      /* partial_inplace */
              0,                          /* src_mask */
              0xffff,                     /* dst_mask */
              TRUE),                      /* pcrel_offset */

        /* An 24 bit PC-relative relocation.  */
        HOWTO(R_DADAO_PC_24,              /* type */
              0,                          /* rightshift */
              2,                          /* size (0 = byte, 1 = short, 2 = long) */
              24,                         /* bitsize */
              TRUE,                       /* pc_relative */
              0,                          /* bitpos */
              complain_overflow_bitfield, /* complain_on_overflow */
              bfd_elf_generic_reloc,      /* special_function */
              "R_DADAO_PC_24",            /* name */
              FALSE,                      /* partial_inplace */
              ~0xffffff,                  /* src_mask */
              0xffffff,                   /* dst_mask */
              TRUE),                      /* pcrel_offset */

        /* A 32 bit absolute PC-relative relocation.  */
        HOWTO(R_DADAO_PC_32,              /* type */
              0,                          /* rightshift */
              2,                          /* size (0 = byte, 1 = short, 2 = long) */
              32,                         /* bitsize */
              TRUE,                       /* pc_relative */
              0,                          /* bitpos */
              complain_overflow_bitfield, /* complain_on_overflow */
              bfd_elf_generic_reloc,      /* special_function */
              "R_DADAO_PC_32",            /* name */
              FALSE,                      /* partial_inplace */
              0,                          /* src_mask */
              0xffffffff,                 /* dst_mask */
              TRUE),                      /* pcrel_offset */

        /* 64 bit PC-relative relocation.  */
        HOWTO(R_DADAO_PC_64,              /* type */
              0,                          /* rightshift */
              4,                          /* size (0 = byte, 1 = short, 2 = long) */
              64,                         /* bitsize */
              TRUE,                       /* pc_relative */
              0,                          /* bitpos */
              complain_overflow_bitfield, /* complain_on_overflow */
              bfd_elf_generic_reloc,      /* special_function */
              "R_DADAO_PC_64",            /* name */
              FALSE,                      /* partial_inplace */
              0,                          /* src_mask */
              MINUS_ONE,                  /* dst_mask */
              TRUE),                      /* pcrel_offset */

        /* GNU extension to record C++ vtable hierarchy.  */
        HOWTO(R_DADAO_GNU_VTINHERIT,   /* type */
              0,                       /* rightshift */
              0,                       /* size (0 = byte, 1 = short, 2 = long) */
              0,                       /* bitsize */
              FALSE,                   /* pc_relative */
              0,                       /* bitpos */
              complain_overflow_dont,  /* complain_on_overflow */
              NULL,                    /* special_function */
              "R_DADAO_GNU_VTINHERIT", /* name */
              FALSE,                   /* partial_inplace */
              0,                       /* src_mask */
              0,                       /* dst_mask */
              TRUE),                   /* pcrel_offset */

        /* GNU extension to record C++ vtable member usage.  */
        HOWTO(R_DADAO_GNU_VTENTRY,          /* type */
              0,                            /* rightshift */
              0,                            /* size (0 = byte, 1 = short, 2 = long) */
              0,                            /* bitsize */
              FALSE,                        /* pc_relative */
              0,                            /* bitpos */
              complain_overflow_dont,       /* complain_on_overflow */
              _bfd_elf_rel_vtable_reloc_fn, /* special_function */
              "R_DADAO_GNU_VTENTRY",        /* name */
              FALSE,                        /* partial_inplace */
              0,                            /* src_mask */
              0,                            /* dst_mask */
              FALSE),                       /* pcrel_offset */

	    /* The ABS relocation is supposed to get any address that could
   		possibly be reached by the ABS instruction.  It can silently expand
   		to get a 64-bit operand, but will complain if any of the two least
   		significant bits are set.  The howto members reflect a simple ABS.  */
        HOWTO(R_DADAO_ABS,               /* type */
              0,                          /* rightshift */
              0,                          /* size (0 = byte, 1 = short, 2 = long) */
              16,                         /* bitsize */
              TRUE,                       /* pc_relative */
              0,                          /* bitpos */
              complain_overflow_bitfield, /* complain_on_overflow */
              dadao_elf_reloc,            /* special_function */
              "R_DADAO_ABS",             /* name */
              FALSE,                      /* partial_inplace */
              ~0x0000ffff,                /* src_mask */
              0x0000ffff,                 /* dst_mask */
              TRUE),                      /* pcrel_offset */

        /* The conditional branches are supposed to reach any (code) address.
		It can silently expand to a 64-bit operand, but will emit an error if
		any of the two least significant bits are set.  The howto members
		reflect a simple branch.  */
        HOWTO(R_DADAO_BRCC,               /* type */
              2,                          /* rightshift */
              2,                          /* size (0 = byte, 1 = short, 2 = long) */
              20,                         /* bitsize */
              TRUE,                       /* pc_relative */
              0,                          /* bitpos */
              complain_overflow_bitfield, /* complain_on_overflow */
              dadao_elf_reloc,            /* special_function */
              "R_DADAO_BRCC",             /* name */
              FALSE,                      /* partial_inplace */
              ~0x0003ffff,                /* src_mask */
              0x0003ffff,                 /* dst_mask */
              TRUE),                      /* pcrel_offset */

        /* A CALL is supposed to reach any (code) address.  By itself, it can
		reach +-64M; the expansion can reach all 64 bits.  Note that the 64M
		limit is soon reached if you link the program in wildly different
		memory segments.  The howto members reflect a trivial JUMP.  */
        HOWTO(R_DADAO_CALL,               /* type */
              2,                          /* rightshift */
              2,                          /* size (0 = byte, 1 = short, 2 = long) */
              26,                         /* bitsize */
              TRUE,                       /* pc_relative */
              0,                          /* bitpos */
              complain_overflow_bitfield, /* complain_on_overflow */
              dadao_elf_reloc,            /* special_function */
              "R_DADAO_CALL",             /* name */
              FALSE,                      /* partial_inplace */
              ~0xffffff,                  /* src_mask */
              0xffffff,                   /* dst_mask */
              TRUE),                      /* pcrel_offset */

        /* A JUMP is supposed to reach any (code) address.  By itself, it can
		reach +-64M; the expansion can reach all 64 bits.  Note that the 64M
		limit is soon reached if you link the program in wildly different
		memory segments.  The howto members reflect a trivial JUMP.  */
        HOWTO(R_DADAO_JUMP,               /* type */
              2,                          /* rightshift */
              2,                          /* size (0 = byte, 1 = short, 2 = long) */
              26,                         /* bitsize */
              TRUE,                       /* pc_relative */
              0,                          /* bitpos */
              complain_overflow_bitfield, /* complain_on_overflow */
              dadao_elf_reloc,            /* special_function */
              "R_DADAO_JUMP",             /* name */
              FALSE,                      /* partial_inplace */
              ~0xffffff,                  /* src_mask */
              0xffffff,                   /* dst_mask */
              TRUE),                      /* pcrel_offset */

        /* Relocation against a local symbol in a shared object.  */
        HOWTO (R_DADAO_RELATIVE,		/* type */
              0,				/* rightshift */
              2,				/* size */
              32,				/* bitsize */
              FALSE,				/* pc_relative */
              0,				/* bitpos */
              complain_overflow_dont,	/* complain_on_overflow */
              bfd_elf_generic_reloc,		/* special_function */
              "R_DADAO_RELATIVE",		/* name */
              FALSE,				/* partial_inplace */
              0,				/* src_mask */
              MINUS_ONE,			/* dst_mask */
              FALSE),			/* pcrel_offset */

        HOWTO (R_DADAO_COPY,			/* type */
              0,				/* rightshift */
              0,				/* this one is variable size */
              0,				/* bitsize */
              FALSE,				/* pc_relative */
              0,				/* bitpos */
              complain_overflow_bitfield,	/* complain_on_overflow */
              bfd_elf_generic_reloc,		/* special_function */
              "R_DADAO_COPY",		/* name */
              FALSE,				/* partial_inplace */
              0,				/* src_mask */
              0,				/* dst_mask */
              FALSE),			/* pcrel_offset */

	/* ADRP can reach +-512M; ((PG(S+A)-PG(P)) >> 12) & 0x1fffff */
	HOWTO(R_DADAO_ADRP,               /* type */
              12,                         /* rightshift */
              2,                          /* size (0 = byte, 1 = short, 2 = long) */
	      18,                         /* bitsize */
	      TRUE,                       /* pc_relative */
	      0,                          /* bitpos */
	      complain_overflow_bitfield, /* complain_on_overflow */
	      dadao_elf_reloc,            /* special_function */
	      "R_DADAO_ADRP",             /* name */
	      FALSE,                      /* partial_inplace */
	      0x1fffff,                   /* src_mask */
	      0x1fffff,                   /* dst_mask */
	      TRUE),                      /* pcrel_offset */
};

/* Look through the relocs for a section during the first phase.  */

static bfd_boolean
elf64_dadao_check_relocs (bfd *abfd, struct bfd_link_info *info,
			    asection *sec, const Elf_Internal_Rela *relocs)
{
  Elf_Internal_Shdr *symtab_hdr;
  struct elf_link_hash_entry **sym_hashes;
  const Elf_Internal_Rela *rel;
  const Elf_Internal_Rela *rel_end;
  asection *sreloc;

  struct elf_dadao_link_hash_table *htab;

  if (bfd_link_relocatable (info))
    return TRUE;

  BFD_ASSERT (is_dadao_elf (abfd));

  htab = elf_dadao_hash_table (info);
  sreloc = NULL;

  symtab_hdr = &elf_symtab_hdr (abfd);
  sym_hashes = elf_sym_hashes (abfd);

  rel_end = relocs + sec->reloc_count;
  for (rel = relocs; rel < rel_end; rel++)
    {
      struct elf_link_hash_entry *h;
      unsigned int r_symndx;
      unsigned int r_type;
      bfd_reloc_code_real_type bfd_r_type;
      Elf_Internal_Sym *isym;

      r_symndx = ELF64_R_SYM (rel->r_info);
      r_type = ELF64_R_TYPE (rel->r_info);

      if (r_symndx >= NUM_SHDR_ENTRIES (symtab_hdr))
	{
	  /* xgettext:c-format */
	  _bfd_error_handler (_("%pB: bad symbol index: %d"), abfd, r_symndx);
	  return FALSE;
	}

      if (r_symndx < symtab_hdr->sh_info)
	{
	  /* A local symbol.  */
	  isym = bfd_sym_from_r_symndx (&htab->sym_cache,
					abfd, r_symndx);
	  if (isym == NULL)
	    return FALSE;

	  /* Check relocation against local STT_GNU_IFUNC symbol.  */
	  if (ELF_ST_TYPE (isym->st_info) == STT_GNU_IFUNC)
	    {
	      h = elf64_dadao_get_local_sym_hash (htab, abfd, rel,
						    TRUE);
	      if (h == NULL)
		return FALSE;

	      /* Fake a STT_GNU_IFUNC symbol.  */
	      h->type = STT_GNU_IFUNC;
	      h->def_regular = 1;
	      h->ref_regular = 1;
	      h->forced_local = 1;
	      h->root.type = bfd_link_hash_defined;
	    }
	  else
	    h = NULL;
	}
      else
	{
	  h = sym_hashes[r_symndx - symtab_hdr->sh_info];
	  while (h->root.type == bfd_link_hash_indirect
		 || h->root.type == bfd_link_hash_warning)
	    h = (struct elf_link_hash_entry *) h->root.u.i.link;
	}

      /* Could be done earlier, if h were already available.  */
      bfd_r_type = dadao_tls_transition (abfd, info, r_type, h, r_symndx);

      if (h != NULL)
	{
	  /* If a relocation refers to _GLOBAL_OFFSET_TABLE_, create the .got.
	     This shows up in particular in an R_DADAO_PREL64 in large model
	     when calculating the pc-relative address to .got section which is
	     used to initialize the gp register.  */
	  if (h->root.root.string
	      && strcmp (h->root.root.string, "_GLOBAL_OFFSET_TABLE_") == 0)
	    {
	      if (htab->root.dynobj == NULL)
		htab->root.dynobj = abfd;

	      if (! dadao_elf_create_got_section (htab->root.dynobj, info))
		return FALSE;

	      BFD_ASSERT (h == htab->root.hgot);
	    }

	  /* Create the ifunc sections for static executables.  If we
	     never see an indirect function symbol nor we are building
	     a static executable, those sections will be empty and
	     won't appear in output.  */
	  switch (bfd_r_type)
	    {
	    default:
	      break;

	    case BFD_RELOC_DADAO_ADD_LO12:
	    case BFD_RELOC_DADAO_ADR_HI21_PCREL:
	      if (htab->root.dynobj == NULL)
		htab->root.dynobj = abfd;
	      if (!_bfd_elf_create_ifunc_sections (htab->root.dynobj, info))
		return FALSE;
	      break;
	    }
	  /* It is referenced by a non-shared object.  */
	  h->ref_regular = 1;
	}

      switch (bfd_r_type)
	{
	case BFD_RELOC_DADAO_ADD_LO12:
	case BFD_RELOC_DADAO_ADR_HI21_PCREL:
	case BFD_RELOC_DADAO_LDST64_LO12:
	  if (h == NULL || bfd_link_pic (info))
	    break;
	  /* Fall through.  */
	default:
	  break;
	}
    }

  return TRUE;
}

/* Return non-zero if the indicated VALUE has overflowed the maximum
   range expressible by a unsigned number with the indicated number of
   BITS.  */

static bfd_reloc_status_type
dadao_unsigned_overflow (bfd_vma value, unsigned int bits)
{
  bfd_vma lim;
  if (bits >= sizeof (bfd_vma) * 8)
    return bfd_reloc_ok;
  lim = (bfd_vma) 1 << bits;
  if (value >= lim)
    return bfd_reloc_overflow;
  return bfd_reloc_ok;
}

/* Return non-zero if the indicated VALUE has overflowed the maximum
   range expressible by an signed number with the indicated number of
   BITS.  */

static bfd_reloc_status_type
dadao_signed_overflow (bfd_vma value, unsigned int bits)
{
  bfd_signed_vma svalue = (bfd_signed_vma) value;
  bfd_signed_vma lim;

  if (bits >= sizeof (bfd_vma) * 8)
    return bfd_reloc_ok;
  lim = (bfd_signed_vma) 1 << (bits - 1);
  if (svalue < -lim || svalue >= lim)
    return bfd_reloc_overflow;
  return bfd_reloc_ok;
}

/* Insert the addend/value into the instruction or data object being
   relocated.  */
bfd_reloc_status_type
_bfd_dadao_elf_put_addend (bfd *abfd,
			     bfd_byte *address, bfd_reloc_code_real_type r_type,
			     reloc_howto_type *howto, bfd_signed_vma addend)
{
  bfd_reloc_status_type status = bfd_reloc_ok;
  bfd_signed_vma old_addend = addend;
  bfd_vma contents;
  int size;

  size = bfd_get_reloc_size (howto);
  switch (size)
    {
    case 0:
      return status;
    case 2:
      contents = bfd_get_16 (abfd, address);
      break;
    case 4:
      if (howto->src_mask != 0xffffffff)
	/* Must be 32-bit instruction, always little-endian.  */
	contents = bfd_getl32 (address);
      else
	/* Must be 32-bit data (endianness dependent).  */
	contents = bfd_get_32 (abfd, address);
      break;
    case 8:
      contents = bfd_get_64 (abfd, address);
      break;
    default:
      abort ();
    }

  switch (howto->complain_on_overflow)
    {
    case complain_overflow_dont:
      break;
    case complain_overflow_signed:
      status = dadao_signed_overflow (addend,
					howto->bitsize + howto->rightshift);
      break;
    case complain_overflow_unsigned:
      status = dadao_unsigned_overflow (addend,
					  howto->bitsize + howto->rightshift);
      break;
    case complain_overflow_bitfield:
    default:
      abort ();
    }

  addend >>= howto->rightshift;

  switch (r_type)
    {
    case BFD_RELOC_DADAO_ADR_HI21_PCREL:
      contents = _bfd_dadao_reencode_adr_imm (contents, addend);
      break;

    case BFD_RELOC_DADAO_ADD_LO12:
      /* Corresponds to: add rd, rn, #uimm12 to provide the low order
	 12 bits of the page offset following
	 BFD_RELOC_DADAO_ADR_HI21_PCREL which computes the
	 (pc-relative) page base.  */
      contents = reencode_add_imm (contents, addend);
      break;

    case BFD_RELOC_DADAO_LDST64_LO12:
      if (old_addend & ((1 << howto->rightshift) - 1))
	return bfd_reloc_overflow;
      /* Used for ldr*|str* rt, [rn, #uimm12] to provide the low order
	 12 bits address offset.  */
      contents = reencode_ldst_pos_imm (contents, addend);
      break;

      /* Group relocations to create high bits of a 16, 32, 48 or 64
	 bit signed data or abs address inline. Will change
	 instruction to MOVN or MOVZ depending on sign of calculated
	 value.  */

    default:
      /* Repack simple data */
      if (howto->dst_mask & (howto->dst_mask + 1))
	return bfd_reloc_notsupported;

      contents = ((contents & ~howto->dst_mask) | (addend & howto->dst_mask));
      break;
    }

  switch (size)
    {
    case 2:
      bfd_put_16 (abfd, contents, address);
      break;
    case 4:
      if (howto->dst_mask != 0xffffffff)
	/* must be 32-bit instruction, always little-endian */
	bfd_putl32 (contents, address);
      else
	/* must be 32-bit data (endianness dependent) */
	bfd_put_32 (abfd, contents, address);
      break;
    case 8:
      bfd_put_64 (abfd, contents, address);
      break;
    default:
      abort ();
    }

  return status;
}

/* Perform a relocation as part of a final link.  The input relocation type
   should be TLS relaxed.  */

static bfd_reloc_status_type
elf64_dadao_final_link_relocate (reloc_howto_type *howto,
				   bfd *input_bfd,
				   bfd *output_bfd,
				   asection *input_section,
				   bfd_byte *contents,
				   Elf_Internal_Rela *rel,
				   bfd_vma value,
				   struct bfd_link_info *info,
				   asection *sym_sec,
				   struct elf_link_hash_entry *h,
				   bfd_boolean *unresolved_reloc_p,
				   bfd_boolean save_addend,
				   bfd_vma *saved_addend,
				   Elf_Internal_Sym *sym)
{
  Elf_Internal_Shdr *symtab_hdr;
  unsigned int r_type = howto->type;
  bfd_reloc_code_real_type bfd_r_type
    = elf64_dadao_bfd_reloc_from_howto (howto);
  unsigned long r_symndx;
  bfd_byte *hit_data = contents + rel->r_offset;
  bfd_vma place, off, got_entry_addr = 0;
  bfd_signed_vma signed_addend;
  struct elf_dadao_link_hash_table *globals;
  bfd_boolean weak_undef_p;
  bfd_boolean relative_reloc;
  asection *base_got;
  bfd_vma orig_value = value;
  bfd_boolean resolved_to_zero;
  bfd_boolean abs_symbol_p;
  bfd_boolean via_plt_p;

  globals = elf_dadao_hash_table (info);

  symtab_hdr = &elf_symtab_hdr (input_bfd);

  BFD_ASSERT (is_dadao_elf (input_bfd));

  r_symndx = ELF64_R_SYM (rel->r_info);

  place = input_section->output_section->vma
    + input_section->output_offset + rel->r_offset;

  /* Get addend, accumulating the addend for consecutive relocs
     which refer to the same offset.  */
  signed_addend = saved_addend ? *saved_addend : 0;
  signed_addend += rel->r_addend;

  weak_undef_p = (h ? h->root.type == bfd_link_hash_undefweak
		  : bfd_is_und_section (sym_sec));
  abs_symbol_p = h != NULL && bfd_is_abs_symbol (&h->root);

  via_plt_p = (globals->root.splt != NULL && h != NULL
	       && h->plt.offset != (bfd_vma) - 1);

  /* Since STT_GNU_IFUNC symbol must go through PLT, we handle
     it here if it is defined in a non-shared object.  */
  if (h != NULL
      && h->type == STT_GNU_IFUNC
      && h->def_regular)
    {
      asection *plt;
      const char *name;
      bfd_vma addend = 0;

      if ((input_section->flags & SEC_ALLOC) == 0)
	{
	  /* If this is a SHT_NOTE section without SHF_ALLOC, treat
	     STT_GNU_IFUNC symbol as STT_FUNC.  */
	  if (elf_section_type (input_section) == SHT_NOTE)
	    goto skip_ifunc;

	  /* Dynamic relocs are not propagated for SEC_DEBUGGING
	     sections because such sections are not SEC_ALLOC and
	     thus ld.so will not process them.  */
	  if ((input_section->flags & SEC_DEBUGGING) != 0)
	    return bfd_reloc_ok;

	  if (h->root.root.string)
	    name = h->root.root.string;
	  else
	    name = bfd_elf_sym_name (input_bfd, symtab_hdr, sym, NULL);
	  _bfd_error_handler
	    /* xgettext:c-format */
	    (_("%pB(%pA+%#" PRIx64 "): "
	       "unresolvable %s relocation against symbol `%s'"),
	     input_bfd, input_section, (uint64_t) rel->r_offset,
	     howto->name, name);
	  bfd_set_error (bfd_error_bad_value);
	  return bfd_reloc_notsupported;
	}
      else if (h->plt.offset == (bfd_vma) -1)
	goto bad_ifunc_reloc;

      /* STT_GNU_IFUNC symbol must go through PLT.  */
      plt = globals->root.splt ? globals->root.splt : globals->root.iplt;
      value = (plt->output_section->vma + plt->output_offset + h->plt.offset);

      switch (bfd_r_type)
	{
	default:
	bad_ifunc_reloc:
	  if (h->root.root.string)
	    name = h->root.root.string;
	  else
	    name = bfd_elf_sym_name (input_bfd, symtab_hdr, sym,
				     NULL);
	  _bfd_error_handler
	    /* xgettext:c-format */
	    (_("%pB: relocation %s against STT_GNU_IFUNC "
	       "symbol `%s' isn't handled by %s"), input_bfd,
	     howto->name, name, __FUNCTION__);
	  bfd_set_error (bfd_error_bad_value);
	  return bfd_reloc_notsupported;

	case BFD_RELOC_DADAO_ADD_LO12:
	case BFD_RELOC_DADAO_ADR_HI21_PCREL:
	  break;
	}
    }

 skip_ifunc:
  resolved_to_zero = (h != NULL
		      && UNDEFWEAK_NO_DYNAMIC_RELOC (info, h));

  switch (bfd_r_type)
    {
    case BFD_RELOC_DADAO_ADR_HI21_PCREL:
      if (bfd_link_pic (info)
	  && (input_section->flags & SEC_ALLOC) != 0
	  && (input_section->flags & SEC_READONLY) != 0
	  && !SYMBOL_REFERENCES_LOCAL (info, h))
	{
	  int howto_index = bfd_r_type - BFD_RELOC_DADAO_RELOC_START;

	  _bfd_error_handler
	    /* xgettext:c-format */
	    (_("%pB: relocation %s against symbol `%s' which may bind "
	       "externally can not be used when making a shared object; "
	       "recompile with -fPIC"),
	     input_bfd, elf_dadao_howto_table[howto_index].name,
	     h->root.root.string);
	  bfd_set_error (bfd_error_bad_value);
	  return bfd_reloc_notsupported;
	}
      /* Fall through.  */

    case BFD_RELOC_DADAO_ADD_LO12:
    case BFD_RELOC_DADAO_LDST64_LO12:
      value = _bfd_dadao_elf_resolve_relocation (input_bfd, bfd_r_type,
						   place, value,
						   signed_addend, weak_undef_p);
      break;

    default:
      return bfd_reloc_notsupported;
    }

  if (saved_addend)
    *saved_addend = value;

  /* Only apply the final relocation in a sequence.  */
  if (save_addend)
    return bfd_reloc_continue;

  return _bfd_dadao_elf_put_addend (input_bfd, hit_data, bfd_r_type,
				      howto, value);
}

bfd_vma
_bfd_dadao_elf_resolve_relocation (bfd *input_bfd,
				     bfd_reloc_code_real_type r_type,
				     bfd_vma place, bfd_vma value,
				     bfd_vma addend, bfd_boolean weak_undef_p)
{
  bfd_boolean tls_reloc = TRUE;
  switch (r_type)
    {
    case BFD_RELOC_DADAO_ADR_HI21_PCREL:
      if (weak_undef_p)
	value = PG (place);
      value = PG (value + addend) - PG (place);
      break;

    case BFD_RELOC_DADAO_ADD_LO12:
    case BFD_RELOC_DADAO_LDST64_LO12:
      value = PG_OFFSET (value + addend);
      break;

    default:
      break;
    }

  return value;
}

/* Map BFD reloc types to DADAO ELF reloc types.  */

struct dadao_reloc_map
{
    bfd_reloc_code_real_type bfd_reloc_val;
    enum elf_dadao_reloc_type elf_reloc_val;
};

static const struct dadao_reloc_map dadao_reloc_map[] =
    {
        {BFD_RELOC_NONE, R_DADAO_NONE},
        {BFD_RELOC_8, R_DADAO_8},
        {BFD_RELOC_16, R_DADAO_16},
        {BFD_RELOC_24, R_DADAO_24},
        {BFD_RELOC_32, R_DADAO_32},
        {BFD_RELOC_64, R_DADAO_64},
        {BFD_RELOC_8_PCREL, R_DADAO_PC_8},
        {BFD_RELOC_16_PCREL, R_DADAO_PC_16},
        {BFD_RELOC_24_PCREL, R_DADAO_PC_24},
        {BFD_RELOC_32_PCREL, R_DADAO_PC_32},
        {BFD_RELOC_64_PCREL, R_DADAO_PC_64},
        {BFD_RELOC_VTABLE_INHERIT, R_DADAO_GNU_VTINHERIT},
        {BFD_RELOC_VTABLE_ENTRY, R_DADAO_GNU_VTENTRY},
        {BFD_RELOC_DADAO_ABS, R_DADAO_ABS},
        {BFD_RELOC_DADAO_BRCC, R_DADAO_BRCC},
        {BFD_RELOC_DADAO_CALL, R_DADAO_CALL},
        {BFD_RELOC_DADAO_JUMP, R_DADAO_JUMP},
	{BFD_RELOC_DADAO_ADRP, R_DADAO_ADRP},
};

static reloc_howto_type *
bfd_elf64_bfd_reloc_type_lookup(bfd *abfd ATTRIBUTE_UNUSED,
                                bfd_reloc_code_real_type code)
{
    unsigned int i;

    for (i = 0;
         i < sizeof(dadao_reloc_map) / sizeof(dadao_reloc_map[0]);
         i++)
    {
        if (dadao_reloc_map[i].bfd_reloc_val == code)
            return &elf_dadao_howto_table[dadao_reloc_map[i].elf_reloc_val];
    }

    return NULL;
}

static reloc_howto_type *
bfd_elf64_bfd_reloc_name_lookup(bfd *abfd ATTRIBUTE_UNUSED,
                                const char *r_name)
{
    unsigned int i;

    for (i = 0;
         i < sizeof(elf_dadao_howto_table) / sizeof(elf_dadao_howto_table[0]);
         i++)
        if (elf_dadao_howto_table[i].name != NULL && strcasecmp(elf_dadao_howto_table[i].name, r_name) == 0)
            return &elf_dadao_howto_table[i];

    return NULL;
}

/* This function performs the actual bitfiddling and sanity check for a
   final relocation.  Each relocation gets its *worst*-case expansion
   in size when it arrives here; any reduction in size should have been
   caught in linker relaxation earlier.  When we get here, the relocation
   looks like the smallest instruction with SWYM:s (nop:s) appended to the
   max size.  We fill in those nop:s.

	FIXME: ABS should be pc-relative, but expansion is absolute addr

   R_DADAO_ABS: (FIXME: Relaxation should break this up in 1, 2, 3 tetra)
    move $N,foo
   ->
    SETZWH $N, (foo >> 48) & 0xffff
    ORWJ $N, (foo >> 32) & 0xffff
    ORWK $N, (foo >> 16) & 0xffff
    ORWL $N, foo & 0xffff

   R_DADAO_BRCC: (FIXME: Relaxation should break this up, but
   condbranches needing relaxation might be rare enough to not be
   worthwhile.)
    Bcc $N,foo
   ->
    B~cc $N,.+16
    SETZWH $3, (foo >> 48) & 0xffff
    ORWJ $3, (foo >> 32) & 0xffff
    ORWK $3, (foo >> 16) & 0xffff
    ORWL $3, foo & 0xffff
    JUMP $3, $0, 0

   R_DADAO_CALL: (FIXME: Relaxation should break this up)
    Call foo
   ->
    SETZWH $3, (foo >> 48) & 0xffff
    ORWJ $3, (foo >> 32) & 0xffff
    ORWK $3, (foo >> 16) & 0xffff
    ORWL $3, foo & 0xffff
    CALL $3, $0, 0

   R_DADAO_JUMP: (FIXME: Relaxation...)
    JUMP foo
   ->
    SETZWH $3, (foo >> 48) & 0xffff
    ORWJ $3, (foo >> 32) & 0xffff
    ORWK $3, (foo >> 16) & 0xffff
    ORWL $3, foo & 0xffff
    JUMP $3, $0, 0
 */
static bfd_reloc_status_type
dadao_elf_perform_relocation(asection *isec, reloc_howto_type *howto,
                             void *datap, bfd_vma addr, bfd_vma value,
                             char **error_message ATTRIBUTE_UNUSED)
{
    bfd *abfd = isec->owner;
    bfd_reloc_status_type r;

    bfd_vma insn_origin;

    switch (howto->type)
    {
    case R_DADAO_ABS:
        insn_origin = bfd_get_32(abfd, (bfd_byte *)datap);

        r = bfd_check_overflow(complain_overflow_bitfield,
                               howto->bitsize, 0,
                               bfd_arch_bits_per_address(abfd),
                               addr);
        if (r == bfd_reloc_ok)
        {
            bfd_put_32(abfd, insn_origin | addr ,
                       (bfd_byte *)datap);
        }
        else
        {
            dd_fill_abs(insn_origin & 0xFF000000, insn_origin & 0x00FC0000, addr, abfd, datap);
        }

        return bfd_reloc_ok;

    case R_DADAO_BRCC:
        if ((value & 3) != 0)
            return bfd_reloc_notsupported;

        insn_origin = bfd_get_32(abfd, (bfd_byte *)datap);

        r = bfd_check_overflow(complain_overflow_bitfield,
                               howto->bitsize, 0,
                               bfd_arch_bits_per_address(abfd),
                               value);
        if (r == bfd_reloc_ok)
        {
            bfd_put_32(abfd, (insn_origin & 0xFF000000) | ((value >> 2) & 0x3FFFF),
                       (bfd_byte *)datap);

            return bfd_reloc_ok;
        }
        else
        {
            //wait to fix
            return bfd_reloc_notsupported;
        }

        return bfd_reloc_ok;

    case R_DADAO_CALL:
        if ((value & 3) != 0)
            return bfd_reloc_notsupported;

        insn_origin = bfd_get_32(abfd, (bfd_byte *)datap);

        r = bfd_check_overflow(complain_overflow_bitfield,
                               howto->bitsize, 0,
                               bfd_arch_bits_per_address(abfd),
                               value);
        if (r == bfd_reloc_ok)
        {
            bfd_put_32(abfd, DADAO_INSN_CALL_IIII | ((value >> 2) & 0xFFFFFF), (bfd_byte *)datap);
        }
        else
        {
            // wait to fix
            return bfd_reloc_notsupported;
        }

        return bfd_reloc_ok;

    case R_DADAO_JUMP:
        if ((value & 3) != 0)
            return bfd_reloc_notsupported;

        insn_origin = bfd_get_32(abfd, (bfd_byte *)datap);

        r = bfd_check_overflow(complain_overflow_bitfield,
                               howto->bitsize, 0,
                               bfd_arch_bits_per_address(abfd),
                               value);
        if (r == bfd_reloc_ok)
        {
            bfd_put_32(abfd, DADAO_INSN_JUMP_IIII | ((value >> 2) & 0xFFFFFF), (bfd_byte *)datap);
        }
        else
        {
            // wait to fix
            return bfd_reloc_notsupported;
        }

        return bfd_reloc_ok;

    case R_DADAO_ADRP:
	if((value & 3) != 0)
           return bfd_reloc_notsupported;

        insn_origin = bfd_get_32(abfd, (bfd_byte *)datap);

	r = bfd_check_overflow(complain_overflow_bitfield,
			       howto->bitsize, 0,
			       bfd_arch_bits_per_address(abfd),
			       value);
	if (r == bfd_reloc_ok)
	{
	    bfd_put_32(abfd, DADAO_INSN_ADRP_RIII | ((value >> 12) & 0x1FFFFF), (bfd_byte *)datap);
        }
	else
        {
	    //wait to fix
	    return bfd_reloc_notsupported;
	}

	return bfd_reloc_ok;
    
    default:
        BAD_CASE(howto->type);
    }

    /* SHOULD NOT BE HERE */
    return bfd_reloc_notsupported;
}

/* Set the howto pointer for an DADAO ELF reloc (type RELA).  */

static bfd_boolean
dadao_info_to_howto_rela(bfd *abfd,
                         arelent *cache_ptr,
                         Elf_Internal_Rela *dst)
{
    unsigned int r_type;

    r_type = ELF64_R_TYPE(dst->r_info);
    if (r_type >= (unsigned int)R_DADAO_max)
    {
        /* xgettext:c-format */
        _bfd_error_handler(_("%pB: unsupported relocation type %#x"),
                           abfd, r_type);
        bfd_set_error(bfd_error_bad_value);
        return FALSE;
    }
    cache_ptr->howto = &elf_dadao_howto_table[r_type];
    return TRUE;
}

/* Any DADAO-specific relocation gets here at assembly time or when linking
   to other formats (such as mmo); this is the relocation function from
   the reloc_table.  We don't get here for final pure ELF linking.  */

static bfd_reloc_status_type
dadao_elf_reloc(bfd *abfd,
                arelent *reloc_entry,
                asymbol *symbol,
                void *data,
                asection *input_section,
                bfd *output_bfd,
                char **error_message)
{
    bfd_vma relocation;
    bfd_reloc_status_type r;
    asection *reloc_target_output_section;
    bfd_reloc_status_type flag = bfd_reloc_ok;
    bfd_vma output_base = 0;

    r = bfd_elf_generic_reloc(abfd, reloc_entry, symbol, data,
                              input_section, output_bfd, error_message);

    /* If that was all that was needed (i.e. this isn't a final link, only
     some segment adjustments), we're done.  */
    if (r != bfd_reloc_continue)
        return r;

    if (bfd_is_und_section(symbol->section) && (symbol->flags & BSF_WEAK) == 0 && output_bfd == (bfd *)NULL)
        return bfd_reloc_undefined;

    /* Is the address of the relocation really within the section?  */
    if (reloc_entry->address > bfd_get_section_limit(abfd, input_section))
        return bfd_reloc_outofrange;

    /* Work out which section the relocation is targeted at and the
     initial relocation command value.  */

    /* Get symbol value.  (Common symbols are special.)  */
    if (bfd_is_com_section(symbol->section))
        relocation = 0;
    else
        relocation = symbol->value;

    reloc_target_output_section = bfd_asymbol_section(symbol)->output_section;

    /* Here the variable relocation holds the final address of the symbol we
     are relocating against, plus any addend.  */
    if (output_bfd)
        output_base = 0;
    else
        output_base = reloc_target_output_section->vma;

    relocation += output_base + symbol->section->output_offset;

    if (output_bfd != (bfd *)NULL)
    {
        /* Add in supplied addend.  */
        relocation += reloc_entry->addend;

        /* This is a partial relocation, and we want to apply the
	 relocation to the reloc entry rather than the raw data.
	 Modify the reloc inplace to reflect what we now know.  */
        reloc_entry->addend = relocation;
        reloc_entry->address += input_section->output_offset;
        return flag;
    }

    return dadao_final_link_relocate(reloc_entry->howto, input_section,
                                     data, reloc_entry->address,
                                     reloc_entry->addend, relocation,
                                     bfd_asymbol_name(symbol),
                                     reloc_target_output_section,
                                     error_message);
}

/* Relocate an DADAO ELF section.  Modified from elf32-fr30.c; look to it
   for guidance if you're thinking of copying this.  */

static bfd_boolean
dadao_elf_relocate_section(bfd *output_bfd ATTRIBUTE_UNUSED,
                           struct bfd_link_info *info,
                           bfd *input_bfd,
                           asection *input_section,
                           bfd_byte *contents,
                           Elf_Internal_Rela *relocs,
                           Elf_Internal_Sym *local_syms,
                           asection **local_sections)
{
    Elf_Internal_Shdr *symtab_hdr;
    struct elf_link_hash_entry **sym_hashes;
    Elf_Internal_Rela *rel;
    Elf_Internal_Rela *relend;

    symtab_hdr = &elf_tdata(input_bfd)->symtab_hdr;
    sym_hashes = elf_sym_hashes(input_bfd);
    relend = relocs + input_section->reloc_count;

    /* Zero the stub area before we start.  */
    if (input_section->rawsize != 0 && input_section->size > input_section->rawsize)
        memset(contents + input_section->rawsize, 0,
               input_section->size - input_section->rawsize);

    for (rel = relocs; rel < relend; rel++)
    {
        reloc_howto_type *howto;
        unsigned long r_symndx;
        Elf_Internal_Sym *sym;
        asection *sec;
        struct elf_link_hash_entry *h;
        bfd_vma relocation;
        bfd_reloc_status_type r;
        const char *name = NULL;
        int r_type;
        bfd_boolean undefined_signalled = FALSE;

        r_type = ELF64_R_TYPE(rel->r_info);

        if (r_type == R_DADAO_GNU_VTINHERIT || r_type == R_DADAO_GNU_VTENTRY)
            continue;

        r_symndx = ELF64_R_SYM(rel->r_info);

        howto = elf_dadao_howto_table + ELF64_R_TYPE(rel->r_info);
        h = NULL;
        sym = NULL;
        sec = NULL;

        if (r_symndx < symtab_hdr->sh_info)
        {
            sym = local_syms + r_symndx;
            sec = local_sections[r_symndx];
            relocation = _bfd_elf_rela_local_sym(output_bfd, sym, &sec, rel);

            name = bfd_elf_string_from_elf_section(input_bfd,
                                                   symtab_hdr->sh_link,
                                                   sym->st_name);
            if (name == NULL)
                name = bfd_section_name(sec);
        }
        else
        {
            bfd_boolean unresolved_reloc, ignored;

            RELOC_FOR_GLOBAL_SYMBOL(info, input_bfd, input_section, rel,
                                    r_symndx, symtab_hdr, sym_hashes,
                                    h, sec, relocation,
                                    unresolved_reloc, undefined_signalled,
                                    ignored);
            name = h->root.root.string;
        }

        if (sec != NULL && discarded_section(sec))
            RELOC_AGAINST_DISCARDED_SECTION(info, input_bfd, input_section,
                                            rel, 1, relend, howto, 0, contents);

        if (bfd_link_relocatable(info))
        {
            /* This is a relocatable link.  For most relocs we don't have to
	     change anything, unless the reloc is against a section
	     symbol, in which case we have to adjust according to where
	     the section symbol winds up in the output section.  */
            if (sym != NULL && ELF_ST_TYPE(sym->st_info) == STT_SECTION)
                rel->r_addend += sec->output_offset;

            continue;
        }

        r = dadao_final_link_relocate(howto, input_section,
                                      contents, rel->r_offset,
                                      rel->r_addend, relocation, name, sec, NULL);

        if (r != bfd_reloc_ok)
        {
            const char *msg = (const char *)NULL;

            switch (r)
            {
            case bfd_reloc_overflow:
                info->callbacks->reloc_overflow(info, (h ? &h->root : NULL), name, howto->name,
                                                (bfd_vma)0, input_bfd, input_section, rel->r_offset);
                break;

            case bfd_reloc_undefined:
                /* We may have sent this message above.  */
                if (!undefined_signalled)
                    info->callbacks->undefined_symbol(info, name, input_bfd, input_section, rel->r_offset, TRUE);
                undefined_signalled = TRUE;
                break;

            case bfd_reloc_outofrange:
                msg = _("internal error: out of range error");
                break;

            case bfd_reloc_notsupported:
                msg = _("internal error: unsupported relocation error");
                break;

            case bfd_reloc_dangerous:
                msg = _("internal error: dangerous relocation");
                break;

            default:
                msg = _("internal error: unknown error");
                break;
            }

            if (msg)
                (*info->callbacks->warning)(info, msg, name, input_bfd,
                                            input_section, rel->r_offset);
        }
    }

    return TRUE;
}

/* Perform a single relocation.  By default we use the standard BFD
   routines.  A few relocs we have to do ourselves.  */

static bfd_reloc_status_type
dadao_final_link_relocate(reloc_howto_type *howto, asection *input_section,
                          bfd_byte *contents, bfd_vma r_offset,
                          bfd_signed_vma r_addend, bfd_vma relocation,
                          const char *symname ATTRIBUTE_UNUSED,
                          asection *symsec ATTRIBUTE_UNUSED,
                          char **error_message)
{
    bfd_vma addr_abs;
    bfd_signed_vma addr_rel;

    bfd_reloc_status_type r = bfd_reloc_ok;

    switch (howto->type)
    {

    /* All these are PC-relative.  */
    case R_DADAO_ABS:
    case R_DADAO_BRCC:
    case R_DADAO_CALL:
    case R_DADAO_JUMP:
    case R_DADAO_ADRP:
        contents += r_offset - 4;

        addr_abs = relocation + (bfd_vma)r_addend;
        addr_rel = addr_abs - (input_section->output_section->vma + input_section->output_offset + r_offset) + 4;

        r = dadao_elf_perform_relocation(input_section, howto, contents,
                                         addr_abs, addr_rel, error_message);
        break;

    default:
        r = _bfd_final_link_relocate(howto, input_section->owner, input_section,
                                     contents, r_offset, relocation, r_addend);
    }

    return r;
}

/* Return the section that should be marked against GC for a given
   relocation.  */

static asection *
dadao_elf_gc_mark_hook(asection *sec,
                       struct bfd_link_info *info,
                       Elf_Internal_Rela *rel,
                       struct elf_link_hash_entry *h,
                       Elf_Internal_Sym *sym)
{
    if (h != NULL)
        switch (ELF64_R_TYPE(rel->r_info))
        {
        case R_DADAO_GNU_VTINHERIT:
        case R_DADAO_GNU_VTENTRY:
            return NULL;
        }

    return _bfd_elf_gc_mark_hook(sec, info, rel, h, sym);
}

/* Sort register relocs to come before expanding relocs.  */

static int
dadao_elf_sort_relocs(const void *p1, const void *p2)
{
    const Elf_Internal_Rela *r1 = (const Elf_Internal_Rela *)p1;
    const Elf_Internal_Rela *r2 = (const Elf_Internal_Rela *)p2;

    /* Sort primarily on r_offset & ~3, so relocs are done to consecutive
     insns.  */
    if ((r1->r_offset & ~(bfd_vma)3) > (r2->r_offset & ~(bfd_vma)3))
        return 1;
    else if ((r1->r_offset & ~(bfd_vma)3) < (r2->r_offset & ~(bfd_vma)3))
        return -1;

    /* Neither or both are register relocs.  Then sort on full offset.  */
    if (r1->r_offset > r2->r_offset)
        return 1;
    else if (r1->r_offset < r2->r_offset)
        return -1;
    return 0;
}

/* Look through the relocs for a section during the first phase.  */

static bfd_boolean
dadao_elf_check_relocs(bfd *abfd,
                       struct bfd_link_info *info,
                       asection *sec,
                       const Elf_Internal_Rela *relocs)
{
    Elf_Internal_Shdr *symtab_hdr;
    struct elf_link_hash_entry **sym_hashes;
    const Elf_Internal_Rela *rel;
    const Elf_Internal_Rela *rel_end;

    symtab_hdr = &elf_tdata(abfd)->symtab_hdr;
    sym_hashes = elf_sym_hashes(abfd);

    /* First we sort the relocs so that any register relocs come before
     expansion-relocs to the same insn.  FIXME: Not done for mmo.  */
    qsort((void *)relocs, sec->reloc_count, sizeof(Elf_Internal_Rela),
          dadao_elf_sort_relocs);

    if (bfd_link_relocatable(info))
        return TRUE;

    rel_end = relocs + sec->reloc_count;
    for (rel = relocs; rel < rel_end; rel++)
    {
        struct elf_link_hash_entry *h;
        unsigned long r_symndx;

        r_symndx = ELF64_R_SYM(rel->r_info);
        if (r_symndx < symtab_hdr->sh_info)
            h = NULL;
        else
        {
            h = sym_hashes[r_symndx - symtab_hdr->sh_info];
            while (h->root.type == bfd_link_hash_indirect || h->root.type == bfd_link_hash_warning)
                h = (struct elf_link_hash_entry *)h->root.u.i.link;
        }

        switch (ELF64_R_TYPE(rel->r_info))
        {
        /* This relocation describes the C++ object vtable hierarchy.
	   Reconstruct it for later use during GC.  */
        case R_DADAO_GNU_VTINHERIT:
            if (!bfd_elf_gc_record_vtinherit(abfd, sec, h, rel->r_offset))
                return FALSE;
            break;

        /* This relocation describes which C++ vtable entries are actually
	   used.  Record for later use during GC.  */
        case R_DADAO_GNU_VTENTRY:
            if (!bfd_elf_gc_record_vtentry(abfd, sec, h, rel->r_addend))
                return FALSE;
            break;
        }
    }

    return TRUE;
}

static bfd_boolean
dadao_elf_relax_delete_bytes (bfd *abfd,
                asection *sec, bfd_vma addr, int count)
{
    Elf_Internal_Shdr *symtab_hdr;
    unsigned int sec_shndx;
    bfd_byte *contents;
    Elf_Internal_Rela *irel, *irelend;
    Elf_Internal_Sym *isym;
    Elf_Internal_Sym *isymend;
    struct elf_link_hash_entry **sym_hashes;
    struct elf_link_hash_entry **end_hashes;
    struct elf_link_hash_entry **start_hashes;
    unsigned int symcount;

    sec_shndx = _bfd_elf_section_from_bfd_section (abfd, sec);
    contents = elf_section_data (sec)->this_hdr.contents;

    irel = elf_section_data (sec)->relocs;
    irelend = irel + sec->reloc_count;

    /* Actually delete the bytes */
    memmove (contents + addr, contents + addr + count,
        (size_t) (sec->size - addr - count));
    sec->size -= count;

    /* Adjust all the relocs */
    for (irel = elf_section_data (sec)->relocs; irel < irelend; irel++)
    {
        /* Get the new reloc addr */
        if (irel->r_offset > addr) {
            irel->r_offset -= count;
        }
    }
    BFD_ASSERT (addr % 2 == 0);
    BFD_ASSERT (count % 2 == 0);

    /* Adjust the local symbols defined in this section. */
    symtab_hdr = &elf_tdata(abfd)->symtab_hdr;
    isym = (Elf_Internal_Sym*) symtab_hdr->contents;
    for (isymend = isym + symtab_hdr->sh_info; isym < isymend; isym++)
    {
        if(isym->st_shndx == sec_shndx && isym->st_value > addr)
            isym->st_value -= count;
    }

    /* Now adjust the global symbols defined in this section. */
    symcount = (symtab_hdr->sh_size / sizeof(Elf64_External_Sym)
            - symtab_hdr->sh_info);
    sym_hashes = start_hashes = elf_sym_hashes (abfd);
    end_hashes = sym_hashes + symcount;

    for(; sym_hashes < end_hashes; sym_hashes++ )
    {
        struct elf_link_hash_entry *sym_hash = *sym_hashes;

        if((sym_hash->root.type == bfd_link_hash_defined
            || sym_hash->root.type == bfd_link_hash_defweak)
            && sym_hash->root.u.def.section == sec)
        {
            bfd_vma value = sym_hash->root.u.def.value;

            if (value > addr) {
                sym_hash->root.u.def.value -= count;
            }
        }
    }
    return TRUE;
}

static bfd_boolean
dadao_elf_relax_section (bfd *abfd, asection *sec,
		       struct bfd_link_info *link_info, bfd_boolean *again)
{
    Elf_Internal_Shdr *symtab_hdr;
    Elf_Internal_Rela *internal_relocs;
    Elf_Internal_Rela *irel, *irelend;
    bfd_byte *contents = NULL;
    Elf_Internal_Sym *isymbuf = NULL;

    /* Assume nothing changes.  */
    *again = FALSE;

    /* We don't have to do anything for a relocatable link, if this
        section does not have relocs, or if this is not a code
        section.  */
    if (bfd_link_relocatable (link_info)
        || (sec->flags & SEC_RELOC) == 0
        || sec->reloc_count == 0
        || (sec->flags & SEC_CODE) == 0)
        return TRUE;

    symtab_hdr = &elf_tdata (abfd)->symtab_hdr;

    /* Get a copy of the native relocations.  */
    internal_relocs = _bfd_elf_link_read_relocs (abfd, sec, NULL, NULL,
                                                link_info->keep_memory);
    if (internal_relocs == NULL)
        goto error_return;

    /* Walk through them looking for relaxing opportunities.  */
    irelend = internal_relocs + sec->reloc_count;
    int i = 0;
    for (irel = internal_relocs; irel < irelend; irel++)
    {
        unsigned long r_symndx = ELF64_R_SYM (irel->r_info);
        unsigned int r_type = ELF64_R_TYPE (irel->r_info);
        unsigned int opcode, nextopc;
        bfd_vma symval;
        bfd_vma pcrval;
        bfd_byte *ptr;
        
        /* The number of bytes to delete for relaxation and from where
        to delete these bytes starting at irel->r_offset. */
        int delcnt = 0;
        int deloff = 0;

        /* Get the section contents if we haven't done so already.  */
        if (contents == NULL)
        {
            /* Get cached copy if it exists.  */
            if (elf_section_data (sec)->this_hdr.contents != NULL)
            contents = elf_section_data (sec)->this_hdr.contents;
            /* Go get them off disk.  */
            else if (!bfd_malloc_and_get_section (abfd, sec, &contents))
            goto error_return;
        }

        ptr = contents + irel->r_offset - 4;

        /* Read this BFD's local symbols if we haven't done so already.  */
        if (isymbuf == NULL && symtab_hdr->sh_info != 0)
        {
            isymbuf = (Elf_Internal_Sym *) symtab_hdr->contents;
            if (isymbuf == NULL)
            isymbuf = bfd_elf_get_elf_syms (abfd, symtab_hdr,
                                            symtab_hdr->sh_info, 0,
                                            NULL, NULL, NULL);
            if (isymbuf == NULL)
            goto error_return;
        }

        /* Get the value of the symbol refferd to by the reloc. */
        if (r_symndx >= symtab_hdr->sh_info)
        {
            /* An external symbol.  */
            unsigned long indx;
            struct elf_link_hash_entry *h;

            indx = r_symndx - symtab_hdr->sh_info;
            h = elf_sym_hashes (abfd)[indx];
            BFD_ASSERT (h != NULL);

            if(h->root.type != bfd_link_hash_defined
            && h->root.type != bfd_link_hash_defweak){
                continue;
            }
            /* This appears to be a reference to an undefined
                symbol.  Just ignore it -- it will be caught by the
                regular reloc processing.  */

            symval = (h->root.u.def.value
                + h->root.u.def.section->output_section->vma
                + h->root.u.def.section->output_offset);
        }
        else {
            continue;
            Elf_Internal_Sym *isym;
            asection *sym_sec;
            
            switch (isym->st_shndx) {
                case SHN_UNDEF:
                    sym_sec = bfd_und_section_ptr;
                    break;
                case SHN_ABS:
                    sym_sec = bfd_abs_section_ptr;
                    break;
                case SHN_COMMON:
                    sym_sec = bfd_com_section_ptr;
                    break;
                default:
                    sym_sec = bfd_section_from_elf_index (abfd, isym->st_shndx);
                    break;
            }
            symval = (isym->st_value
                    + sym_sec->output_section->vma
                    + sym_sec->output_offset);
        }

        /* For simplicity of coding, we are going to modify the
        section contents, the section relocs, and the BFD symbol
        table.  We must tell the rest of the code not to free up this
        information.  It would be possible to instead create a table
        of changes which have to be made, as is done in coff-mips.c;
        that would be more work, but would require less memory when
        the linker is run.  */

        opcode = bfd_get_32 (abfd, ptr);
        nextopc = bfd_get_32 (abfd, ptr + 4);
        /* Only do relax when next insn is noop */
        if(!nextopc) {
            /* This is the pc-relative distance from the instruction the
            relocation is applied to, to the symbol referred.  */
            pcrval = symval - (sec->output_section->vma + sec->output_offset + irel->r_offset) + 4;

            if (r_type == R_DADAO_CALL && MATCH (opcode, DADAO_INSN_CALL_IIII))
            {
                /* Delete 16 bytes from irel->r_offset */
                delcnt = 16;
                deloff = 0;
            }
            if (r_type == R_DADAO_BRCC && ((opcode>>24) >= 0x28 && (opcode>>24) <= 0x2F))
            {
                /* Delete 20 bytes from irel->r_offset */
                delcnt = 20;
                deloff = 0;
            }
            if (r_type == R_DADAO_JUMP && MATCH (opcode, DADAO_INSN_JUMP_IIII))
            {
                delcnt = 16;
                deloff = 0;
            }
            if (r_type == R_DADAO_ADRP && MATCH (opcode, DADAO_INSN_ADRP_RIII))
            {
                delcnt = 16;
                deloff = 0;
            }
            if (r_type == R_DADAO_ABS && (MATCH (opcode, DADAO_INSN_SETZW_RD) 
                || MATCH (opcode, DADAO_INSN_SETOW_RD) || MATCH (opcode, DADAO_INSN_SETZW_RB)))
            {
                delcnt = 16 - dd_fill_abs(opcode & 0xFF000000,opcode & 0x00FC0000,symval,abfd,ptr);
                deloff = 0;
            }
        }
        else {
          /* FIXME: need to adjust the addr if the relocation has been relaxed. */
            delcnt = 0;
        }

        if (delcnt != 0) {
            /* Note that we've changed the relocs, section contents, etc. */
            elf_section_data (sec)->relocs = internal_relocs;
            elf_section_data (sec)->this_hdr.contents = contents;
            symtab_hdr->contents = (unsigned char *) isymbuf;

            /* Delete bytes depending on the delcnt and deloff. */
            if(!dadao_elf_relax_delete_bytes (abfd, sec, irel->r_offset + deloff, delcnt))
                goto error_return;
            
            /* That will change things, so we shold relax again.
                Note that this is not required, and it may be slow. */
            *again = TRUE;
        }
    }
    

    if (isymbuf != NULL
      && symtab_hdr->contents != (unsigned char *) isymbuf)
    {
      if (!link_info->keep_memory)
        free (isymbuf);
      else
       /* Cache the symbols for elf_link_input_bfd.  */
       symtab_hdr->contents = (unsigned char *) isymbuf;
    }

    if (contents != NULL
      && elf_section_data (sec)->this_hdr.contents != contents)
    {
      if (!link_info->keep_memory)
        free (contents);
      else
       /* Cache the section contents for elf_link_input_bfd.  */
       elf_section_data (sec)->this_hdr.contents = contents;
    }

    if (elf_section_data (sec)->relocs != internal_relocs)
    free (internal_relocs);

    return TRUE;

    error_return:
    if (symtab_hdr->contents != (unsigned char *) isymbuf)
        free (isymbuf);
    if (elf_section_data (sec)->this_hdr.contents != contents)
        free (contents);
    if (elf_section_data (sec)->relocs != internal_relocs)
        free (internal_relocs);

    return FALSE;
}


#define ELF_ARCH bfd_arch_dadao
#define ELF_MACHINE_CODE EM_DADAO

#define ELF_MAXPAGESIZE 0x1000

#define TARGET_BIG_SYM dadao_elf64_vec
#define TARGET_BIG_NAME "elf64-dadao"

#define elf_info_to_howto_rel NULL
#define elf_info_to_howto dadao_info_to_howto_rela
#define elf_backend_relocate_section dadao_elf_relocate_section
#define elf_backend_gc_mark_hook dadao_elf_gc_mark_hook

#define elf_backend_check_relocs dadao_elf_check_relocs
#define elf_backend_omit_section_dynsym _bfd_elf_omit_section_dynsym_all

#define bfd_elf64_bfd_copy_link_hash_symbol_type \
    _bfd_generic_copy_link_hash_symbol_type

#define elf_backend_may_use_rel_p 0
#define elf_backend_may_use_rela_p 1
#define elf_backend_default_use_rela_p 1

#define elf_backend_can_gc_sections 1

#define bfd_elf64_bfd_relax_section dadao_elf_relax_section

#define elf_backend_size_dynamic_sections	dadao_elf_size_dynamic_sections
#define elf_backend_create_dynamic_sections	dadao_elf_create_dynamic_sections
#define elf_backend_finish_dynamic_symbol	dadao_elf_finish_dynamic_symbol

#include "elf64-target.h"
