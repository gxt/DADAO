#ifndef __ASM_TLB_H
#define __ASM_TLB_H

#include <linux/pagemap.h>
#include <linux/swap.h>

static inline void __tlb_remove_table(void *_table)
{
	free_page_and_swap_cache((struct page *)_table);
}

#define tlb_flush tlb_flush
static void tlb_flush(struct mmu_gather *tlb);

#include <asm-generic/tlb.h>

static inline void tlb_flush(struct mmu_gather *tlb)
{
	struct vm_area_struct vma = TLB_FLUSH_VMA(tlb->mm, 0);
	bool last_level = !tlb->freed_tables;

	/*
	 * If we're tearing down the address space then we only care about
	 * invalidating the walk-cache, since the ASID allocator won't
	 * reallocate our ASID without invalidating the entire TLB.
	 */
	if (tlb->fullmm) {
		if (!last_level)
			flush_tlb_mm(tlb->mm);
		return;
	}

	flush_tlb_range(&vma, tlb->start, tlb->end);
}

static inline void __pte_free_tlb(struct mmu_gather *tlb, pgtable_t pte,
				  unsigned long addr)
{
	pgtable_pte_page_dtor(pte);
	tlb_remove_table(tlb, pte);
}

static inline void __pmd_free_tlb(struct mmu_gather *tlb, pmd_t *pmdp,
				  unsigned long addr)
{
	struct page *page = virt_to_page(pmdp);

	pgtable_pmd_page_dtor(page);
	tlb_remove_table(tlb, page);
}

static inline void __pud_free_tlb(struct mmu_gather *tlb, pud_t *pudp,
				  unsigned long addr)
{
	tlb_remove_table(tlb, virt_to_page(pudp));
}

#endif
