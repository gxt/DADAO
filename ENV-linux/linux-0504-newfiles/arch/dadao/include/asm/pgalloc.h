#ifndef __ASM_DADAO_PGALLOC_H
#define __ASM_DADAO_PGALLOC_H

#include <linux/mm.h>

#include <asm-generic/pgalloc.h>

#define __pgd_populate(pgdp, pudp, prot)	set_pgd(pgdp, __pgd(__phys_to_pgd_val(pudp) | prot))
#define __pud_populate(pudp, pmdp, prot)	set_pud(pudp, __pud(__phys_to_pud_val(pmdp) | prot))
#define __pmd_populate(pmdp, ptep, prot)	set_pmd(pmdp, __pmd(__phys_to_pmd_val(ptep) | prot))

#define pgd_populate(mm, pgdp, pudp)		__pgd_populate(pgdp, __pa(pudp), PUD_TYPE_TABLE)
#define pud_populate(mm, pudp, pmdp)		__pud_populate(pudp, __pa(pmdp), PMD_TYPE_TABLE)
#define pmd_populate(mm, pmdp, ptep)		__pmd_populate(pmdp, page_to_phys(ptep), PMD_TYPE_TABLE)
#define pmd_populate_kernel(mm, pmdp, ptep)	__pmd_populate(pmdp, __pa(ptep), PMD_TYPE_TABLE)

#define pmd_pgtable(pmd)			pmd_page(pmd)

static inline pgd_t *pgd_alloc(struct mm_struct *mm)
{
	pgd_t *pgd;

	pgd = (pgd_t *)__get_free_page(GFP_KERNEL);

	return pgd;
}

static inline void pgd_free(struct mm_struct *mm, pgd_t *pgd)
{
	free_page((unsigned long)pgd);
}

static inline pud_t *pud_alloc_one(struct mm_struct *mm, unsigned long addr)
{
	return (pud_t *)__get_free_page(GFP_PGTABLE_USER);
}

static inline void pud_free(struct mm_struct *mm, pud_t *pudp)
{
	BUG_ON((unsigned long)pudp & (PAGE_SIZE-1));
	free_page((unsigned long)pudp);
}

static inline pmd_t *pmd_alloc_one(struct mm_struct *mm, unsigned long addr)
{
	gfp_t gfp = GFP_PGTABLE_USER;
	struct page *page;

	if (mm == &init_mm)
		gfp = GFP_PGTABLE_KERNEL;

	page = alloc_page(gfp);
	if (!page)
		return NULL;
	if (!pgtable_pmd_page_ctor(page)) {
		__free_page(page);
		return NULL;
	}
	return page_address(page);
}

static inline void pmd_free(struct mm_struct *mm, pmd_t *pmdp)
{
	BUG_ON((unsigned long)pmdp & (PAGE_SIZE-1));
	pgtable_pmd_page_dtor(virt_to_page(pmdp));
	free_page((unsigned long)pmdp);
}

#endif /* __ASM_DADAO_PGALLOC_H */
