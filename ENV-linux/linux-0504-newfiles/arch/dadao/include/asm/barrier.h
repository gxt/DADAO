/*
 * Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
 * Contributed by Guan Xuetao <gxt@pku.edu.cn>
 */
#ifndef __DADAO_ASM_BARRIER_H__
#define __DADAO_ASM_BARRIER_H__

#ifndef __ASSEMBLY__

#define nop()		asm volatile ("swym	0")

#include <asm-generic/barrier.h>

#endif	/* __ASSEMBLY__ */

#endif /* __DADAO_ASM_BARRIER_H__ */
