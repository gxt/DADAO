/*
 * Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
 * Contributed by Guan Xuetao <gxt@pku.edu.cn>
 */

/* This file contains a bit of information about the stack allocation
   of the processor.  */
#ifndef _SYSDEPS_DADAO_STACKINFO_H_
#define _SYSDEPS_DADAO_STACKINFO_H_

#include <elf.h>

/* On Dadao the stack grows down.  */
#define _STACK_GROWS_DOWN	1

/* Default to a non-executable stack. */
#define DEFAULT_STACK_PERMS	((PF_R) | (PF_W))

#endif	/* _SYSDEPS_DADAO_STACKINFO_H_ */
