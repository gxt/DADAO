/*
 * Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
 * Contributed by Guan Xuetao <gxt@pku.edu.cn>
 */
#ifndef __DADAO_ASM_PGTABLE_BITS_H__
#define __DADAO_ASM_PGTABLE_BITS_H__

#define __PAGE_PRESENT		(1 << 0)
#define __PAGE_SUPERPAGE	(1 << 1)
#define __PAGE_YOUNG		(1 << 3)
#define __PAGE_DIRTY		(1 << 4)
#define __PAGE_EXEC		(1 << 5)
#define __PAGE_WRITE		(1 << 6)
#define __PAGE_READ		(1 << 7)

#endif /* __DADAO_ASM_PGTABLE_BITS_H__ */
