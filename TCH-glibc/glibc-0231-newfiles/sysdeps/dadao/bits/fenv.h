/*
 * Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
 * Contributed by Guan Xuetao <gxt@pku.edu.cn>
 */
#ifndef _FENV_H
# error "Never use <bits/fenv.h> directly; include <fenv.h> instead."
#endif

/* Define bits representing exceptions.  */
#define FE_INVALID		0x01
#define FE_DIVBYZERO		0x02
#define FE_OVERFLOW		0x04
#define FE_UNDERFLOW		0x08
#define FE_INEXACT		0x10

/* All supported exceptions.  */
#define FE_ALL_EXCEPT		(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW | FE_INEXACT)

/* Define bits representing rounding modes.  */
#define FE_TONEAREST		0x00
#define FE_UPWARD		0x01
#define FE_DOWNWARD		0x02
#define FE_TOWARDZERO		0x03

/* Type representing exception flags. */
typedef unsigned long fexcept_t;

/* Type representing floating-point environment.  */
typedef unsigned long fenv_t;

/* If the default argument is used we use this value.  */
#define FE_DFL_ENV	((const fenv_t *) -1l)

#if __GLIBC_USE (IEC_60559_BFP_EXT_C2X)
/* Type representing floating-point control modes.  */
typedef unsigned long femode_t;

/* Default floating-point control modes.  */
# define FE_DFL_MODE	((const femode_t *) -1L)
#endif
