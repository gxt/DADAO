/*
 * Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
 * Contributed by Guan Xuetao <gxt@pku.edu.cn>
 */
#ifndef	_LINK_H
# error "Never include <bits/link.h> directly; use <link.h> instead."
#endif

/* Registers for entry into PLT on dadao.  */
typedef struct La_dadao_regs
{
  uint64_t lr_reg [3];
  uint64_t lr_sp;
} La_dadao_regs;

/* Return values for calls from PLT on dadao.  */
typedef struct La_dadao_retval
{
  uint64_t lrv_reg [2];
} La_dadao_retval;


__BEGIN_DECLS

extern Elf64_Addr la_dadao_gnu_pltenter (Elf64_Sym *__sym, unsigned int __ndx,
					uintptr_t *__refcook,
					uintptr_t *__defcook,
					La_dadao_regs *__regs,
					unsigned int *__flags,
					const char *__symname,
					long int *__framesizep);
extern unsigned int la_dadao_gnu_pltexit (Elf64_Sym *__sym, unsigned int __ndx,
					 uintptr_t *__refcook,
					 uintptr_t *__defcook,
					 const La_dadao_regs *__inregs,
					 La_dadao_retval *__outregs,
					 const char *__symname);

__END_DECLS
