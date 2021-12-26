/*
 * Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
 * Contributed by Guan Xuetao <gxt@pku.edu.cn>
 */
#ifndef _SYSDEPS_DADAO_BITS_SETJMP_H_
#define _SYSDEPS_DADAO_BITS_SETJMP_H_

#if !defined _SETJMP_H && !defined _PTHREAD_H
# error "Never include <bits/setjmp.h> directly; use <setjmp.h> instead."
#endif

#ifndef _ASM
__extension__ typedef unsigned long long __jmp_buf [64];
#endif

#endif /* _SYSDEPS_DADAO_BITS_SETJMP_H_ */
