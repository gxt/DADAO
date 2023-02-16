/*
 * Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
 * Contributed by Guan Xuetao <gxt@pku.edu.cn>
 */
#ifndef __DADAO_ASM_PGTABLE_H__
#define __DADAO_ASM_PGTABLE_H__

#include <asm-generic/5level-fixup.h>

#include <asm/pgtable-hwdef.h>
#include <asm/arch_memory.h>

#define	VMALLOC_START			__DD_MEMORY_VMALLOC_START
#define	VMALLOC_END			__DD_MEMORY_VMALLOC_END

#ifndef __ASSEMBLY__

extern pgd_t swapper_pg_dir[PTRS_PER_PGD];

#define set_pte(pteptr, pteval)		((*(pteptr)) = (pteval))

#define set_pte_at(mm, addr, pteptr, pteval)				\
	do {								\
		set_pte(pteptr, pteval);				\
	} while (0)

#define pte_clear(mm, addr, pteptr)	set_pte(pteptr, __pte(0))
#define pte_young(pte)			(pte_val(pte) & __PAGE_YOUNG)
#define pte_unmap(pte)			do { } while (0)

#define pte_pfn(pte)			(pte_val(pte) >> PAGE_SHIFT)
#define pfn_pte(pfn,prot)		__pte(((phys_addr_t)(pfn) << PAGE_SHIFT) | pgprot_val(prot))

#define mk_pte(page,prot)		pfn_pte(page_to_pfn(page),prot)

static inline pte_t pte_wrprotect(pte_t pte)	{ pte_val(pte) &= ~__PAGE_WRITE; return pte; }
static inline pte_t pte_mkold(pte_t pte)	{ pte_val(pte) &= ~__PAGE_YOUNG; return pte; }
static inline pte_t pte_mkwrite(pte_t pte)	{ pte_val(pte) |=  __PAGE_WRITE; return pte; }
static inline pte_t pte_mkdirty(pte_t pte)	{ pte_val(pte) |=  __PAGE_DIRTY; return pte; }

#define pte_none(pte)			(!pte_val(pte))
#define pmd_none(pmd)			(!pmd_val(pmd))
#define pud_none(pud)			(!pud_val(pud))
#define pgd_none(pgd)			(!pgd_val(pgd))

#define pmd_bad(pmd)			(!(pmd_val(pmd) & PMD_TABLE_BIT))
#define pud_bad(pud)			(!(pud_val(pud) & PUD_TABLE_BIT))
#define pgd_bad(pgd)			(!(pgd_val(pgd) & 2))

#define pte_present(pte)		(pte_val(pte) & __PAGE_PRESENT)
#define pmd_present(pmd)		(pmd_val(pmd) & __PAGE_PRESENT)
#define pud_present(pud)		(pud_val(pud) & __PAGE_PRESENT)
#define pgd_present(pgd)		(pgd_val(pgd) & __PAGE_PRESENT)

#define pte_index(addr)			(((addr) >> PAGE_SHIFT)  & (PTRS_PER_PTE - 1))
#define pmd_index(addr)			(((addr) >> PMD_SHIFT)   & (PTRS_PER_PMD - 1))
#define pud_index(addr)			(((addr) >> PUD_SHIFT)   & (PTRS_PER_PUD - 1))
#define pgd_index(addr)			(((addr) >> PGDIR_SHIFT) & (PTRS_PER_PGD - 1))

#define pte_offset_kernel(dir, addr)	((pte_t *)__va((pmd_val(READ_ONCE(*(dir))) & PTE_ADDR_MASK) + pte_index(addr) * sizeof(pte_t)))
#define pte_offset_map(dir, addr)	pte_offset_kernel((dir), (addr))

#define pmd_offset(dir, addr)		((pmd_t *)__va((pud_val(READ_ONCE(*(dir))) & PTE_ADDR_MASK) + pmd_index(addr) * sizeof(pmd_t)))
#define pud_offset(dir, addr)		((pud_t *)__va((pgd_val(READ_ONCE(*(dir))) & PTE_ADDR_MASK) + pud_index(addr) * sizeof(pud_t)))
#define pgd_offset(mm, addr)		((pgd_t *)((mm)->pgd + pgd_index(addr)))

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
