/*
 * Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
 * Contributed by Guan Xuetao <gxt@pku.edu.cn>
 */
#ifndef __DADAO_ASM_PAGE_H__
#define __DADAO_ASM_PAGE_H__

/* PAGE_SHIFT determines the page size */
#define PAGE_SHIFT		12
#define PAGE_SIZE		(_AC(1, UL) << PAGE_SHIFT)
#define PAGE_MASK		(~(PAGE_SIZE-1))

#include <asm/arch_memory.h>
#include <asm/pgtable-types.h>

/* PAGE_OFFSET - the virtual address of the start of the kernel image */
#define PAGE_OFFSET		__DD_MEMORY_PAGE_OFFSET

#ifndef __ASSEMBLY__

#define clear_page(page)			memset((page), 0, PAGE_SIZE)
#define copy_page(to,from)			memcpy((to), (from), PAGE_SIZE)

#define clear_user_page(page, vaddr, pg)	clear_page(page)
#define copy_user_page(to, from, vaddr, pg)	copy_page(to, from)


typedef struct page *pgtable_t;

#define __va(x)			((void *)((unsigned long)(x) + PAGE_OFFSET))
#define __pa(x)			((unsigned long)(x) - PAGE_OFFSET)

#define virt_to_pfn(kaddr)	(__pa(kaddr) >> PAGE_SHIFT)
#define pfn_to_virt(pfn)	__va((pfn) << PAGE_SHIFT)

#define virt_to_page(kaddr)	pfn_to_page(virt_to_pfn(kaddr))
#define page_to_virt(page)	pfn_to_virt(page_to_pfn(page))

#define page_to_phys(page)	((dma_addr_t)page_to_pfn(page) << PAGE_SHIFT)

#define pfn_valid(pfn)		((pfn) >= ARCH_PFN_OFFSET && ((pfn) - ARCH_PFN_OFFSET) < max_mapnr)

#define virt_addr_valid(kaddr)	(((void *)(kaddr) >= (void *)PAGE_OFFSET) && \
				((void *)(kaddr) < (void *)memory_end))

#endif /* __ASSEMBLY__ */

#define VM_DATA_DEFAULT_FLAGS	(VM_READ | VM_WRITE | VM_MAYREAD | VM_MAYWRITE | VM_MAYEXEC)

#include <asm-generic/memory_model.h>
#include <asm-generic/getorder.h>

#endif	/* __DADAO_ASM_PAGE_H__ */
