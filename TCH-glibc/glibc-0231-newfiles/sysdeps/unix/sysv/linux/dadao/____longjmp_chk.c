/*
 * Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
 * Contributed by Guan Xuetao <gxt@pku.edu.cn>
 */
#include <jmpbuf-offsets.h>
#include <sysdep.h>
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stackinfo.h>

#define __longjmp	____longjmp_chk

#ifdef _STACK_GROWS_DOWN
#define called_from(this, saved) ((this) < (saved))
#else
#define called_from(this, saved) ((this) > (saved))
#endif

#define CHECK_SP(sp)	do {						\
	void *this_frame = __builtin_frame_address (0);			\
	void *saved_frame = JB_FRAME_ADDRESS (env);			\
	INTERNAL_SYSCALL_DECL (err);					\
	stack_t ss;							\
									\
	/* If "env" is from a frame that called us, we're all set.  */	\
	if (called_from(this_frame, saved_frame))			\
		break;							\
	/* If we can't get the current stack state, give up. */		\
	if (INTERNAL_SYSCALL (sigaltstack, err, 2, NULL, &ss) != 0)	\
		break;							\
	/* If we we are executing on the alternate stack */		\
	if (ss.ss_flags == SS_ONSTACK					\
		&& (this_frame >= ss.ss_sp)				\
		&& (this_frame < (ss.ss_sp + ss.ss_size)))		\
		break;							\
									\
	__fortify_fail ("longjmp causes uninitialized stack frame");	\
	} while (0)

#include <__longjmp.c>
