/*
 * BFD library support routines for DADAO.
 * Copyright (C) 2019-2033 Guan Xuetao (AT) Peking Univ.
 *
 * Contributed by:
 *   2019:
 *	Guan Xuetao <gxt@pku.edu.cn>
 */
#include "sysdep.h"
#include "bfd.h"
#include "libbfd.h"

static const bfd_arch_info_type *
bfd_dadao_compatible (const bfd_arch_info_type *a, const bfd_arch_info_type *b)
{
  if (a->arch != b->arch)
    return NULL;

  if (a->mach != b->mach)
    return NULL;

  return a;
}

#define N(MACHINE, PRINT, DEFAULT, NEXT)			\
{								\
  64,	/* Bits in a word.  */					\
  64,	/* Bits in an address.  */				\
  8,	/* Bits in a byte.  */					\
  bfd_arch_dadao,						\
  MACHINE,							\
  "dadao",							\
  PRINT,							\
  3,								\
  DEFAULT,							\
  bfd_dadao_compatible,						\
  bfd_default_scan,						\
  bfd_arch_default_fill,					\
  NEXT								\
}

static const bfd_arch_info_type arch_info_struct[] =
{
  N (bfd_mach_dadao_wuqing, "DADAO-wuqing", FALSE, &arch_info_struct[1]),
  N (bfd_mach_dadao_wuming, "DADAO-wuming", FALSE, NULL)
};

const bfd_arch_info_type bfd_dadao_arch =
  N (bfd_mach_dadao_wuxing, "DADAO-wuxing", TRUE, &arch_info_struct[0]);
