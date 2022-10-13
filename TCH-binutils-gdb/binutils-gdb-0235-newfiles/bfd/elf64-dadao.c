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

static bfd_reloc_status_type
dadao_elf_reloc(bfd *, arelent *, asymbol *, void *,
                asection *, bfd *, char **);
static bfd_reloc_status_type
dadao_final_link_relocate(reloc_howto_type *, asection *, bfd_byte *, bfd_vma,
                          bfd_signed_vma, bfd_vma, const char *, asection *,
                          char **);

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

	/* High 18 bits of 30-bit absolute address. */
	HOWTO(R_DADAO_HI18,		  /* type */
	      0,			  /* rightshift */
	      2,			  /* size */
	      32,			  /* bitsize */
	      FALSE,			  /* pc_relative */
	      0,			  /* bitpos */
	      complain_overflow_dont,	  /* complain_on_overflow */
	      dadao_elf_reloc,		  /* special_function */
	      "R_DADAO_HI18",		  /* name */
	      FALSE,			  /* partial_inplace */
	      ~0xffffff,		  /* src_mask */
	      0xffffff,			  /* dst_mask */
	      FALSE),			  /* pcrel_offset */

	/* Low 12 bits of 30-bit load or add. */
	HOWTO(R_DADAO_LO12,		  /* type */
	      0,			  /* rightshift */
	      2,			  /* size */
	      32,			  /* bitsize */
	      FALSE,			  /* pc_relative */
	      0,			  /* bitpos */
	      complain_overflow_dont,	  /* complaion_on_overflow */
	      dadao_elf_reloc,		  /* special_function */
	      "R_DADAO_LO12",		  /* name */
	      FALSE,			  /* partial_inplace */
	      ~0xffffff,		  /* src_mask */
	      0xffffff,			  /* dst_mask */
	      FALSE),			  /* pcrel_offset */
};

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
	{BFD_RELOC_DADAO_HI18, R_DADAO_HI18},
	{BFD_RELOC_DADAO_LO12, R_DADAO_LO12},
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

    case R_DADAO_HI18:
	value = (addr >> 12) - ((addr - value) >> 12);
	insn_origin = bfd_get_32(abfd, (bfd_byte *)datap);
	bfd_put_32(abfd, insn_origin | value , (bfd_byte *)datap);
	return bfd_reloc_ok;

    case R_DADAO_LO12:
	addr = ENCODE_ITYPE_IMM (addr);
	insn_origin = bfd_get_32(abfd, (bfd_byte *)datap);
	bfd_put_32(abfd, insn_origin | (addr >> 18) , (bfd_byte *)datap);
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
    case R_DADAO_HI18:
    case R_DADAO_LO12:
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

#include "elf64-target.h"
