/*
 * Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
 * Contributed by Guan Xuetao <gxt@pku.edu.cn>
 */
#ifndef _SYSDEPS_DADAO_SYSDEP_H_
#define _SYSDEPS_DADAO_SYSDEP_H_

#include <sysdeps/generic/sysdep.h>

#ifdef	__ASSEMBLER__

/* Syntactic details of assembler.  */
#define	ASM_SIZE_DIRECTIVE(name)	.size name,.-name

#ifdef	PROF
/* FIXME: add mcount support lator */
# define	CALL_MCOUNT		swym		/* Do nothing.  */
#else	/* PROF */
# define	CALL_MCOUNT				/* Do nothing.  */
#endif	/* PROF */

/* Define an entry point visible from C.  */
#define	ENTRY(name)						\
	.global	C_SYMBOL_NAME(name);				\
	.type	C_SYMBOL_NAME(name),%function;			\
	.align	4;						\
	C_LABEL(name)						\
	cfi_startproc;						\
	CALL_MCOUNT

#undef	END
#define	END(name)						\
	cfi_endproc;						\
	ASM_SIZE_DIRECTIVE(name)

#endif	/* __ASSEMBLER__ */

#endif /* _SYSDEPS_DADAO_SYSDEP_H_ */
