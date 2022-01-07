/*
 * Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
 * Contributed by Guan Xuetao <gxt@pku.edu.cn>
 */
#ifndef _SYSDEPS_LINUX_DADAO_SYSDEP_H_
#define _SYSDEPS_LINUX_DADAO_SYSDEP_H_

#include <sysdeps/unix/sysv/linux/generic/sysdep.h>
#include <sysdeps/dadao/sysdep.h>

/* For Linux we can use the system call table in the header file unistd.h
   of the kernel.  But these symbols do not follow the SYS_* syntax
   so we have to redefine the `SYS_ify' macro here.  */
#undef	SYS_ify
#define	SYS_ify(syscall_name)		(__NR_##syscall_name)

#ifdef	__ASSEMBLER__

#define __DO_SYSCALL(syscall_name, args)			\
	trap	cp0, SYS_ify(syscall_name)

#define PSEUDO(name, syscall_name, args)			\
	.text;							\
	ENTRY (name);						\
	__DO_SYSCALL (syscall_name, args);
/* FIXME: error handler should be here */

#undef	PSEUDO_END
#define PSEUDO_END(name)					\
	END (name);

#define PSEUDO_NOERRNO(name, syscall_name, args)		\
	.text;							\
	ENTRY (name);						\
	__DO_SYSCALL (syscall_name, args);

#undef	PSEUDO_END_NOERRNO
#define PSEUDO_END_NOERRNO(name)				\
	END (name);

#define ret_NOERRNO		ret

#define	PSEUDO_ERRVAL		.error NOT support PSEUDO_ERRVAL
#define	ret_ERRVAL		ret

#else	/* __ASSEMBLER__ */

/* In order to get __set_errno() definition in INLINE_SYSCALL.  */
#include <errno.h>

/* Define a macro which expands into the inline wrapper code for a system call. */
#undef	INLINE_SYSCALL
#define	INLINE_SYSCALL(name, nr, args...)	({				\
	unsigned long __sys_res = INTERNAL_SYSCALL (name, , nr, args);		\
	if (__glibc_unlikely (INTERNAL_SYSCALL_ERROR_P (__sys_res, ))) {	\
		__set_errno (INTERNAL_SYSCALL_ERRNO (__sys_res, ));		\
		__sys_res = (unsigned long) -1;					\
	}									\
	(long) __sys_res; })

/* INTERNAL_SYSCALL_DECL - Allows us to setup some function static
 * value to use within the context of the syscall */
#define	INTERNAL_SYSCALL_DECL(err)	do { } while (0)

#define	INTERNAL_SYSCALL_RAW(name, err, nr, args...)		\
	({ unsigned long int _sys_result;			\
		{						\
		register int _a16 asm ("rd31");			\
		LOAD_ARGS_##nr (args)				\
		asm volatile (					\
			"# syscall " #name			\
			"trap	cp0, %1"			\
			: "=r" (_a16)				\
			: "i" (name) ASM_ARGS_##nr		\
			: "memory");				\
		_sys_result = _a16;				\
		}						\
	(long int) _sys_result; })

/* Similar to INLINE_SYSCALL but we don't set errno */
#define	INTERNAL_SYSCALL(name, err, nr, args...)		\
	INTERNAL_SYSCALL_RAW(SYS_ify(name), err, nr, args)

/* INTERNAL_SYSCALL_ERROR_P - Returns 0 if it wasn't an error, 1 otherwise
 * You are allowed to use the syscall result (val) and the DECL error
 * variable to determine what went wrong. */
#define INTERNAL_SYSCALL_ERROR_P(val, err)			\
	((unsigned long) (val) > -4096UL)

/* INTERNAL_SYSCALL_ERRNO - Munges the val/err pair into the error number.
 * In our case we just flip the sign. */
#define INTERNAL_SYSCALL_ERRNO(val, err)	(-(val))

#define LOAD_ARGS_0()
#define ASM_ARGS_0

#define LOAD_ARGS_1(a1)						\
	register long _a1 asm ("rd16") = (long) (a1);		\
	LOAD_ARGS_0 ()
#define ASM_ARGS_1		ASM_ARGS_0, "r" (_a1)

#define LOAD_ARGS_2(a1, a2)					\
	register long _a2 asm ("rd17") = (long) (a2);		\
	LOAD_ARGS_1 (a1)
#define ASM_ARGS_2		ASM_ARGS_1, "r" (_a2)

#define LOAD_ARGS_3(a1, a2, a3)					\
	register long _a3 asm ("rd18") = (long) (a3);		\
	LOAD_ARGS_2 (a1, a2)
#define ASM_ARGS_3		ASM_ARGS_2, "r" (_a3)

#define LOAD_ARGS_4(a1, a2, a3, a4)				\
	register long _a4 asm ("rd19") = (long) (a4);		\
	LOAD_ARGS_3 (a1, a2, a3)
#define ASM_ARGS_4		ASM_ARGS_3, "r" (_a4)

#define LOAD_ARGS_5(a1, a2, a3, a4, a5)				\
	register long _a5 asm ("rd20") = (long) (a5);		\
	LOAD_ARGS_4 (a1, a2, a3, a4)
#define ASM_ARGS_5		ASM_ARGS_4, "r" (_a5)

#define LOAD_ARGS_6(a1, a2, a3, a4, a5, a6)			\
	register long _a6 asm ("rd21") = (long) (a6);		\
	LOAD_ARGS_5 (a1, a2, a3, a4, a5)
#define ASM_ARGS_6		ASM_ARGS_5, "r" (_a6)

#define LOAD_ARGS_7(a1, a2, a3, a4, a5, a6, a7)			\
	register long _a7 asm ("rd22") = (long) (a7);		\
	LOAD_ARGS_6 (a1, a2, a3, a4, a5, a6)
#define ASM_ARGS_7		ASM_ARGS_6, "r" (_a7)

#endif	/* __ASSEMBLER__ */

#endif /* _SYSDEPS_LINUX_DADAO_SYSDEP_H_ */
