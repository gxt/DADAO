/*
 * Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
 * Contributed by Guan Xuetao <gxt@pku.edu.cn>
 */
#ifndef __DADAO_ASM_ELF_H__
#define __DADAO_ASM_ELF_H__

#include <asm/ptrace.h>
#include <linux/elf-em.h>

#define ELF_NGREG		192	/* 64 * 3 */
#define ELF_NFPREG		64

typedef unsigned long elf_greg_t;
typedef unsigned long elf_fpreg_t;
typedef elf_greg_t elf_gregset_t[ELF_NGREG];
typedef elf_fpreg_t elf_fpregset_t[ELF_NFPREG];

#define R_DADAO_NONE		0
#define R_DADAO_CALL		28
#define R_DADAO_JUMP		29

#define ELF_CLASS		ELFCLASS64
#define ELF_DATA		ELFDATA2MSB
#define ELF_ARCH		EM_DADAO
#define ELF_PLATFORM		("dadao")
#define ELF_HWCAP		(0)
#define ELF_EXEC_PAGESIZE	PAGE_SIZE
#define ELF_ET_DYN_BASE		(TASK_SIZE / 8)

#define elf_check_arch(x)	((x)->e_machine == ELF_ARCH)

#endif /* __DADAO_ASM_ELF_H__ */
