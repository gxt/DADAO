/*
 * Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
 * Contributed by Guan Xuetao <gxt@pku.edu.cn>
 */
#ifndef __DADAO_ASM_ARCH_MEMORY_H__
#define __DADAO_ASM_ARCH_MEMORY_H__

#include <linux/const.h>

/* the whole 48-bit address space, 47-bit for user address space */
#define __DD_MEMORY_TASK_SIZE		ULL(0x800000000000)

/* PAGE_OFFSET - the virtual address of the start of the kernel image */
#define __DD_MEMORY_PAGE_OFFSET		ULL(0xDADA00000000)
#define __DD_MEMORY_VMALLOC_START	(__DD_MEMORY_PAGE_OFFSET + SZ_2G)
#define __DD_MEMORY_VMALLOC_END		(__DD_MEMORY_PAGE_OFFSET + SZ_4G)

#endif /* __DADAO_ASM_ARCH_MEMORY_H__ */
