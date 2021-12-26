/*
 * Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
 * Contributed by Guan Xuetao <gxt@pku.edu.cn>
 */
#ifndef _SYSDEPS_DADAO_ATOMIC_MACHINE_H_
#define _SYSDEPS_DADAO_ATOMIC_MACHINE_H_

#include <stdint.h>

typedef int32_t		atomic32_t;
typedef int64_t		atomic64_t;

typedef uint32_t	uatomic32_t;
typedef uint64_t	uatomic64_t;

#define __HAVE_64B_ATOMICS		1
#define USE_ATOMIC_COMPILER_BUILTINS	0
#define ATOMIC_EXCHANGE_USES_CAS	0

#endif	/* _SYSDEPS_DADAO_ATOMIC_MACHINE_H_ */
