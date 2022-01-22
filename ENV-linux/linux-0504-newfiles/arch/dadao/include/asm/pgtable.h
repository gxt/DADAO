/*
 * Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
 * Contributed by Guan Xuetao <gxt@pku.edu.cn>
 */
#ifndef __DADAO_ASM_PGTABLE_H__
#define __DADAO_ASM_PGTABLE_H__

#define __ARCH_USE_5LEVEL_HACK
#include <asm-generic/pgtable-nopmd.h>

#include <asm/pgtable-bits.h>
#include <asm/arch_memory.h>

#define	VMALLOC_START			__DD_MEMORY_VMALLOC_START
#define	VMALLOC_END			__DD_MEMORY_VMALLOC_END

#define PTRS_PER_PTE			1024
#define PTRS_PER_PGD			1024

#ifndef __ASSEMBLY__

extern pgd_t swapper_pg_dir[PTRS_PER_PGD];

#define set_pte(pteptr, pteval)		((*(pteptr)) = (pteval))

#define set_pte_at(mm, addr, pteptr, pteval)				\
	do {								\
		set_pte(pteptr, pteval);				\
	} while (0)

#define pte_clear(mm, addr, pteptr)	set_pte(pteptr, __pte(0))
#define pte_young(pte)			(pte_val(pte) & __PAGE_YOUNG)

static inline pte_t pte_wrprotect(pte_t pte)	{ pte_val(pte) &= ~__PAGE_WRITE; return pte; }
static inline pte_t pte_mkold(pte_t pte)	{ pte_val(pte) &= ~__PAGE_YOUNG; return pte; }
static inline pte_t pte_mkwrite(pte_t pte)	{ pte_val(pte) |=  __PAGE_WRITE; return pte; }

#define pmd_none(pmd)			(!pmd_val(pmd))
#define pmd_present(pmd)		(pmd_val(pmd) & __PAGE_PRESENT)
#define pmd_bad(pmd)			(1)	/* FIXME */
/*
 * ZERO_PAGE is a global shared page that is always zero: used
 * for zero-mapped memory areas etc..
 */
extern struct page *empty_zero_page;
#define ZERO_PAGE(vaddr)		(empty_zero_page)

#define PAGE_NONE			__pgprot(0)
#define PAGE_SHARED			__pgprot(0)
#define PAGE_KERNEL			__pgprot(0)

#define __swp_type(x)			(0)
#define __swp_offset(x)			(0)
#define __swp_entry(typ, off)		((swp_entry_t) { ((typ) | (off)) })
#define __swp_entry_to_pte(x)		((pte_t) { (x).val })
#define __pte_to_swp_entry(pte)		((swp_entry_t) { pte_val(pte) })

#define kern_addr_valid(addr)		(1)

#endif /* __ASSEMBLY__ */

#include <asm-generic/pgtable.h>

#endif /* __DADAO_ASM_PGTABLE_H__ */
