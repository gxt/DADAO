
#ifndef _DADAO_LDSODEFS_H
#define _DADAO_LDSODEFS_H 1

#include <elf.h>

struct La_dadao_regs;
struct La_dadao_retval;

#define ARCH_PLTENTER_MEMBERS						\
	ElfW(Addr) (*dadao_gnu_pltenter) (ElfW(Sym) *,			\
					unsigned int,			\
                                        uintptr_t *,			\
                                        uintptr_t *,			\
					struct La_dadao_regs *,		\
					unsigned int *,			\
					const char *,			\
					long int *)

#define ARCH_PLTEXIT_MEMBERS						\
	ElfW(Addr) (*dadao_gnu_pltexit) (ElfW(Sym) *,			\
				       unsigned int,			\
				       uintptr_t *,			\
				       uintptr_t *,			\
				       const struct La_dadao_regs *,	\
				       struct La_dadao_retval *,	\
				       const char *)

#include_next <ldsodefs.h>

#endif
