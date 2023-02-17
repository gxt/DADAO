#ifndef __ASM_DADAO_PGALLOC_H
#define __ASM_DADAO_PGALLOC_H

#include <linux/mm.h>

#include <asm-generic/pgalloc.h>

#define pgd_populate(mm, pgdp, pudp)		set_pgd(pgdp, __pgd(__pa(pudp) | __PAGE_PRESENT))
#define pud_populate(mm, pudp, pmdp)		set_pud(pudp, __pud(__pa(pmdp) | __PAGE_PRESENT))
#define pmd_populate(mm, pmdp, ptep)		set_pmd(pmdp, __pmd(__pa(ptep) | __PAGE_PRESENT))

#define pmd_pgtable(pmd)	pmd_page(pmd)

static inline pmd_t *pmd_alloc_one(struct mm_struct *mm, unsigned long addr)
{
	return (pmd_t *)__get_free_page(
		GFP_KERNEL | __GFP_RETRY_MAYFAIL | __GFP_ZERO);
}

static inline void pmd_free(struct mm_struct *mm, pmd_t *pmd)
{
	free_page((unsigned long)pmd);
}

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

#endif /* __ASM_DADAO_PGALLOC_H */
