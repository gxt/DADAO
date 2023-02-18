/*
 * Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
 * Contributed by Guan Xuetao <gxt@pku.edu.cn>
 */
#ifndef __DADAO_ASM_PGTABLE_H__
#define __DADAO_ASM_PGTABLE_H__

#include <asm-generic/5level-fixup.h>

#include <asm/pgtable-hwdef.h>
#include <asm/pgtable-prot.h>
#include <asm/arch_memory.h>

#define	VMALLOC_START			__DD_MEMORY_VMALLOC_START
#define	VMALLOC_END			__DD_MEMORY_VMALLOC_END

#define FIRST_USER_ADDRESS		0UL

#ifndef __ASSEMBLY__

extern pgd_t swapper_pg_dir[PTRS_PER_PGD];

#define pgd_ERROR(pgd)			printk("%s:%d: bad pgd %016llx.\n", __FILE__, __LINE__, pgd_val(pgd))
#define pud_ERROR(pud)			printk("%s:%d: bad pud %016llx.\n", __FILE__, __LINE__, pud_val(pud))
#define pmd_ERROR(pmd)			printk("%s:%d: bad pmd %016llx.\n", __FILE__, __LINE__, pmd_val(pmd))

#define pgd_none(pgd)			(!pgd_val(pgd))
#define pud_none(pud)			(!pud_val(pud))
#define pmd_none(pmd)			(!pmd_val(pmd))
#define pte_none(pte)			(!pte_val(pte))

#define pgd_bad(pgd)			(!(pgd_val(pgd) & 2))
#define pud_bad(pud)			(!(pud_val(pud) & PUD_TABLE_BIT))
#define pmd_bad(pmd)			(!(pmd_val(pmd) & PMD_TABLE_BIT))

#define pgd_pte(pgd)			__pte(pgd_val(pgd))
#define pud_pte(pud)			__pte(pud_val(pud))
#define pmd_pte(pmd)			__pte(pmd_val(pmd))

#define pgd_present(pgd)		(pgd_val(pgd))
#define pud_present(pud)		pte_present(pud_pte(pud))
#define pmd_present(pmd)		pte_present(pmd_pte(pmd))
#define pte_present(pte)		(!!(pte_val(pte) & (PTE_VALID | PTE_PROT_NONE)))

#define pgd_index(addr)			(((addr) >> PGDIR_SHIFT) & (PTRS_PER_PGD - 1))
#define pud_index(addr)			(((addr) >> PUD_SHIFT)   & (PTRS_PER_PUD - 1))
#define pmd_index(addr)			(((addr) >> PMD_SHIFT)   & (PTRS_PER_PMD - 1))
#define pte_index(addr)			(((addr) >> PAGE_SHIFT)  & (PTRS_PER_PTE - 1))

#define pgd_clear(pgdp)			set_pgd(pgdp, __pgd(0))
#define pud_clear(pudp)			set_pud(pudp, __pud(0))
#define pmd_clear(pmdp)			set_pmd(pmdp, __pmd(0))
#define pte_clear(mm, addr, ptep)	set_pte(ptep, __pte(0))

#define __pgd_to_phys(pgd)		__pte_to_phys(pgd_pte(pgd))
#define __pud_to_phys(pud)		__pte_to_phys(pud_pte(pud))
#define __pmd_to_phys(pmd)		__pte_to_phys(pmd_pte(pmd))
#define __pte_to_phys(pte)		(pte_val(pte) & PTE_ADDR_MASK)

#define __phys_to_pgd_val(phys)		__phys_to_pte_val(phys)
#define __phys_to_pud_val(phys)		__phys_to_pte_val(phys)
#define __phys_to_pmd_val(phys)		__phys_to_pte_val(phys)
#define __phys_to_pte_val(phys)		(phys)

#define pgd_page(pgd)			pfn_to_page(__phys_to_pfn(__pgd_to_phys(pgd)))
#define pud_page(pud)			pfn_to_page(__phys_to_pfn(__pud_to_phys(pud)))
#define pmd_page(pmd)			pfn_to_page(__phys_to_pfn(__pmd_to_phys(pmd)))
#define pte_page(pte)			(pfn_to_page(pte_pfn(pte)))

#define set_pgd(pgdp, pgdv)		((*(pgdp)) = (pgdv))
#define set_pud(pudp, pudv)		((*(pudp)) = (pudv))
#define set_pmd(pmdp, pmdv)		((*(pmdp)) = (pmdv))
#define set_pte(ptep, ptev)		((*(ptep)) = (ptev))

#define set_pte_at(mm, addr, pteptr, pteval)				\
	do {								\
		set_pte(pteptr, pteval);				\
	} while (0)

#define pte_young(pte)			(!!(pte_val(pte) & PTE_AF))
#define pte_special(pte)		(!!(pte_val(pte) & PTE_SPECIAL))
#define pte_write(pte)			(!!(pte_val(pte) & PTE_WRITE))
#define pte_dirty(pte)			(!!(pte_val(pte) & PTE_DIRTY))
#define pte_unmap(pte)			do { } while (0)

#define pte_pfn(pte)			(pte_val(pte) >> PAGE_SHIFT)
#define pfn_pte(pfn,prot)		__pte(((phys_addr_t)(pfn) << PAGE_SHIFT) | pgprot_val(prot))

#define mk_pte(page,prot)		pfn_pte(page_to_pfn(page),prot)

#define pte_mkold(pte)			(__pte(pte_val(pte) & ~PTE_AF))
#define pte_mkyoung(pte)		(__pte(pte_val(pte) |  PTE_AF))
#define pte_wrprotect(pte)		(__pte(pte_val(pte) & ~PTE_WRITE))
#define pte_mkwrite(pte)		(__pte(pte_val(pte) |  PTE_WRITE))
#define pte_mkclean(pte)		(__pte(pte_val(pte) & ~PTE_DIRTY))
#define pte_mkdirty(pte)		(__pte(pte_val(pte) |  PTE_DIRTY))
#define pte_mkspecial(pte)		(__pte(pte_val(pte) |  PTE_SPECIAL))

#define pgd_offset(mm, addr)		((pgd_t *)((mm)->pgd + pgd_index(addr)))
#define pud_offset(dir, addr)		((pud_t *)__va((pgd_val(READ_ONCE(*(dir))) & PTE_ADDR_MASK) + pud_index(addr) * sizeof(pud_t)))
#define pmd_offset(dir, addr)		((pmd_t *)__va((pud_val(READ_ONCE(*(dir))) & PTE_ADDR_MASK) + pmd_index(addr) * sizeof(pmd_t)))
#define pte_offset_kernel(dir, addr)	((pte_t *)__va((pmd_val(READ_ONCE(*(dir))) & PTE_ADDR_MASK) + pte_index(addr) * sizeof(pte_t)))
#define pte_offset_map(dir, addr)	pte_offset_kernel((dir), (addr))

#define pgd_offset_k(addr)		pgd_offset(&init_mm, addr)

static inline pte_t pte_modify(pte_t pte, pgprot_t newprot)
{
	const pteval_t mask = PTE_USER | PTE_PXN | PTE_UXN | PTE_RDONLY |
			      PTE_PROT_NONE | PTE_VALID | PTE_WRITE;
	pte_val(pte) = (pte_val(pte) & ~mask) | (pgprot_val(newprot) & mask);
	return pte;
}

/*
 * ZERO_PAGE is a global shared page that is always zero: used
 * for zero-mapped memory areas etc..
 */
extern struct page *empty_zero_page;
#define ZERO_PAGE(vaddr)		(empty_zero_page)

#define __swp_type(x)			(0)
#define __swp_offset(x)			(0)
#define __swp_entry(typ, off)		((swp_entry_t) { ((typ) | (off)) })
#define __swp_entry_to_pte(x)		((pte_t) { (x).val })
#define __pte_to_swp_entry(pte)		((swp_entry_t) { pte_val(pte) })

#define kern_addr_valid(addr)		(1)

#define update_mmu_cache(vma, addr, ptep)	do { } while (0)

#endif /* __ASSEMBLY__ */

#include <asm-generic/pgtable.h>

#endif /* __DADAO_ASM_PGTABLE_H__ */
