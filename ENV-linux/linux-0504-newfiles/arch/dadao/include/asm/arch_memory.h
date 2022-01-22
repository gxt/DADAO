/*
 * Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
 * Contributed by Guan Xuetao <gxt@pku.edu.cn>
 */
#ifndef __DADAO_ASM_ARCH_MEMORY_H__
#define __DADAO_ASM_ARCH_MEMORY_H__

/* PAGE_OFFSET - the virtual address of the start of the kernel image */
#define __DD_MEMORY_PAGE_OFFSET		UL(0xDADA00000000)
#define __DD_MEMORY_VMALLOC_START	(PAGE_OFFSET + SZ_2G)
#define __DD_MEMORY_VMALLOC_END		(PAGE_OFFSET + SZ_4G)

#endif /* __DADAO_ASM_ARCH_MEMORY_H__ */
