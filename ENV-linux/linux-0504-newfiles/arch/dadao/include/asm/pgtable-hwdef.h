/*
 * Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
 * Contributed by Guan Xuetao <gxt@pku.edu.cn>
 */
#ifndef __DADAO_ASM_PGTABLE_HWDEF_H__
#define __DADAO_ASM_PGTABLE_HWDEF_H__

/*
 * Size mapped by an entry at level n ( 0 <= n <= 3)
 * We map (PAGE_SHIFT - 3) at all translation levels and PAGE_SHIFT bits
 * in the final page. The maximum number of translation levels supported by
 * the architecture is 4. Hence, starting at at level n, we have further
 * ((4 - n) - 1) levels of translation excluding the offset within the page.
 * So, the total number of bits mapped by an entry at level n is :
 *
 *  ((4 - n) - 1) * (PAGE_SHIFT - 3) + PAGE_SHIFT
 *
 * Rearranging it a bit we get :
 *   (4 - n) * (PAGE_SHIFT - 3) + 3
 */
#define __PGTABLE_LVL_SHIFT(n)	((PAGE_SHIFT - 3) * (4 - (n)) + 3)

#define PTRS_PER_PGD		(1 << (PAGE_SHIFT - 3))
#define PTRS_PER_PUD		(1 << (PAGE_SHIFT - 3))
#define PTRS_PER_PMD		(1 << (PAGE_SHIFT - 3))
#define PTRS_PER_PTE		(1 << (PAGE_SHIFT - 3))

#define PGDIR_SHIFT		__PGTABLE_LVL_SHIFT(0)
#define PUD_SHIFT		__PGTABLE_LVL_SHIFT(1)
#define PMD_SHIFT		__PGTABLE_LVL_SHIFT(2)

#define PGDIR_SIZE		(_AC(1, UL) << PGDIR_SHIFT)
#define PUD_SIZE		(_AC(1, UL) << PUD_SHIFT)
#define PMD_SIZE		(_AC(1, UL) << PMD_SHIFT)

#define PGDIR_MASK		(~(PGDIR_SIZE - 1))
#define PUD_MASK		(~(PUD_SIZE - 1))
#define PMD_MASK		(~(PMD_SIZE - 1))

#define PUD_TABLE_BIT		(_AT(pudval_t, 1) << 1)
#define PMD_TABLE_BIT		(_AT(pmdval_t, 1) << 1)
#define PTE_TABLE_BIT		(_AT(pteval_t, 1) << 1)

#define PUD_TYPE_TABLE		(_AT(pudval_t, 3) << 0)
#define PMD_TYPE_TABLE		(_AT(pmdval_t, 3) << 0)

#define PTE_ADDR_MASK		(((_AT(pteval_t, 1) << (48 - PAGE_SHIFT)) - 1) << PAGE_SHIFT)

/* FIXME: arm64 ref; P2M (Page of 2M) -- PMD_SECT */
#define P2M_VALID		(_AT(pmdval_t, 1) << 0)
#define P2M_USER		(_AT(pmdval_t, 1) << 6)		/* AP[1] */
#define P2M_RDONLY		(_AT(pmdval_t, 1) << 7)		/* AP[2] */
#define P2M_S			(_AT(pmdval_t, 3) << 8)
#define P2M_AF			(_AT(pmdval_t, 1) << 10)
#define P2M_NG			(_AT(pmdval_t, 1) << 11)
#define P2M_CONT		(_AT(pmdval_t, 1) << 52)
#define P2M_PXN			(_AT(pmdval_t, 1) << 53)
#define P2M_UXN			(_AT(pmdval_t, 1) << 54)

#define PTE_VALID		(_AT(pteval_t, 1) << 0)
#define PTE_TYPE_MASK		(_AT(pteval_t, 3) << 0)
#define PTE_TYPE_PAGE		(_AT(pteval_t, 3) << 0)
#define PTE_USER		(_AT(pteval_t, 1) << 6)		/* AP[1] */
#define PTE_RDONLY		(_AT(pteval_t, 1) << 7)		/* AP[2] */
#define PTE_SHARED		(_AT(pteval_t, 3) << 8)		/* SH[1:0], inner shareable */
#define PTE_AF			(_AT(pteval_t, 1) << 10)	/* Access Flag */
#define PTE_NG			(_AT(pteval_t, 1) << 11)	/* nG */
#define PTE_WRITE		(_AT(pteval_t, 1) << 51)	/* Dirty Bit Management */
#define PTE_CONT		(_AT(pteval_t, 1) << 52)	/* Contiguous range */
#define PTE_PXN			(_AT(pteval_t, 1) << 53)	/* Privileged XN */
#define PTE_UXN			(_AT(pteval_t, 1) << 54)	/* User XN */

#endif /* __DADAO_ASM_PGTABLE_HWDEF_H__ */
