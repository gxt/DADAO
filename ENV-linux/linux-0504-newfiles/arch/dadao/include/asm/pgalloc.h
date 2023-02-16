#ifndef __ASM_DADAO_PGALLOC_H
#define __ASM_DADAO_PGALLOC_H

#include <linux/mm.h>

#include <asm-generic/pgalloc.h>

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
