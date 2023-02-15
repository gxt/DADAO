/*
 * Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
 * Contributed by Guan Xuetao <gxt@pku.edu.cn>
 */
#ifndef __DADAO_ASM_ARCH_PGTABLE_H__
#define __DADAO_ASM_ARCH_PGTABLE_H__

#define PTRS_PER_PTE		(1 << 9)
#define PTRS_PER_PMD		(1 << 9)
#define PTRS_PER_PUD		(1 << 9)
#define PTRS_PER_PGD		(1 << 9)

#define PGDIR_SHIFT		39
#define PGDIR_SIZE		(_AC(1, UL) << PGDIR_SHIFT)
#define PGDIR_MASK		(~(PGDIR_SIZE - 1))

#define PUD_SHIFT		30
#define PUD_SIZE		(_AC(1, UL) << PUD_SHIFT)
#define PUD_MASK		(~(PUD_SIZE - 1))

#define PMD_SHIFT		21
#define PMD_SIZE		(_AC(1, UL) << PMD_SHIFT)
#define PMD_MASK		(~(PMD_SIZE - 1))

/* FIXME: copy from arm64 */
#define PUD_TABLE_BIT		(_AT(pudval_t, 1) << 1)
#define PMD_TABLE_BIT		(_AT(pmdval_t, 1) << 1)

#define PTE_ADDR_MASK		(((_AT(pteval_t, 1) << (48 - PAGE_SHIFT)) - 1) << PAGE_SHIFT)

#define __PAGE_PRESENT		(1 << 0)
#define __PAGE_SUPERPAGE	(1 << 1)
#define __PAGE_HUGEPAGE		(1 << 2)
#define __PAGE_YOUNG		(1 << 3)
#define __PAGE_DIRTY		(1 << 4)
#define __PAGE_EXEC		(1 << 5)
#define __PAGE_WRITE		(1 << 6)
#define __PAGE_READ		(1 << 7)

#endif /* __DADAO_ASM_ARCH_PGTABLE_H__ */
