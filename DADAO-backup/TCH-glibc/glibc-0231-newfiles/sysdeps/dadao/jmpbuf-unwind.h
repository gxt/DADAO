/*
 * Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
 * Contributed by Guan Xuetao <gxt@pku.edu.cn>
 */
#include <setjmp.h>
#include <stdint.h>
#include <unwind.h>

/* Test if longjmp to JMPBUF would unwind the frame
   containing a local variable at ADDRESS.  */
#define _JMPBUF_UNWINDS(_jmpbuf, _address, _demangle)		\
  ((void *) (_address) < (void *) _demangle (_jmpbuf[0]))

#define _JMPBUF_CFA_UNWINDS_ADJ(_jmpbuf, _context, _adj)	\
  _JMPBUF_UNWINDS_ADJ (_jmpbuf, (void *) (uintptr_t) _Unwind_GetCFA (_context), _adj)

#define _JMPBUF_UNWINDS_ADJ(_jmpbuf, _address, _adj)		\
  ((uintptr_t) (_address) - (_adj) < (uintptr_t) (_jmpbuf[0]) - (_adj))

/* We use the normal longjmp for unwinding.  */
#define __libc_unwind_longjmp(buf, val) __libc_longjmp (buf, val)
