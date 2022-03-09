/*
 * Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
 * Contributed by Guan Xuetao <gxt@pku.edu.cn>
 */

#include <sysdep.h>
#define _ERRNO_H	1
#include <bits/errno.h>

/*
 * clone() is even more special than fork() as it mucks with stacks
 * and invokes a function in the right context after its all over.
 *
 * int clone(int (*fn)(void *arg), void *child_stack, int flags, void *arg,
 *	void *parent_tidptr, void *tls, void *child_tidptr)
 */

        .text
ENTRY(__clone)
	/* TODO: prepare args */
	move	rd15, SYS_ify(clone)
	trap	cp0, 0

	brz	rd31, thread_start
	brn	rd31, .Lsyscall_error
	RET
PSEUDO_END (__clone)

	.align 4
	.type thread_start, %function
thread_start:
	cfi_startproc

	/* TODO: Pick the function arg and execute.  */

	/* We are done, pass the return value through x0.  */
	move	rd15, SYS_ify(exit)
	trap	cp0, 0
	cfi_endproc
	.size thread_start, .-thread_start

libc_hidden_def (__clone)
weak_alias (__clone, clone)
