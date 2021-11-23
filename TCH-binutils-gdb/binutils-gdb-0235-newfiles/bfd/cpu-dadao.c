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

const bfd_arch_info_type
bfd_dadao_arch =
 {
   64,				/* 64 bits in a machine word.  */
   64,				/* 64 bits in an address.  */
   8,				/* 8 bits in a byte.  */
   bfd_arch_dadao,		/* Architecture.  */
   0,				/* Machine number - 0 for now.  */
   /* Sorry, these are by custom and creeping assumption lower-case.  */
   "dadao",			/* Architecture name.  */
   "dadao",			/* Printable name.  */
   3,				/* Section align power.  */
   TRUE,			/* This is the default architecture.  */
   bfd_default_compatible,	/* Architecture comparison function.  */
   bfd_default_scan,		/* String to architecture conversion.  */
   bfd_arch_default_fill,	/* Default fill.  */
   NULL				/* Next in list.  */
};
