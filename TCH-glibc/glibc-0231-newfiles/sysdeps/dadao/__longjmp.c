/*
 * Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
 * Contributed by Guan Xuetao <gxt@pku.edu.cn>
 */
#include <setjmp.h>
#include <stdlib.h>

/* Jump to the position specified by ENV, causing the
   setjmp call there to return VAL, or 1 if VAL is 0.  */
void
__longjmp (__jmp_buf env, int val)
{
#ifdef CHECK_SP
	CHECK_SP (env[0]);	/* FIXME: where is sp */
#endif

	asm volatile(
		""		/* FIXME: DO SOMETHING HERE */
		: /* No outputs.  */
		: /* need inputs. */
		: /* No point in clobbers.  */ );

	/* Avoid `volatile function does return' warnings.  */
	for (;;);
}
