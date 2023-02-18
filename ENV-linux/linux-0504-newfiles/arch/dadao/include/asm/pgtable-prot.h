#ifndef __ASM_PGTABLE_PROT_H
#define __ASM_PGTABLE_PROT_H

#include <asm/pgtable-hwdef.h>

#include <linux/const.h>

/*
 * Software defined PTE bits definition.
 */
#define PTE_DIRTY		(_AT(pteval_t, 1) << 55)
#define PTE_SPECIAL		(_AT(pteval_t, 1) << 56)
#define PTE_PROT_NONE		(_AT(pteval_t, 1) << 58) /* only when !PTE_VALID */

#ifndef __ASSEMBLY__

#include <asm/pgtable-types.h>

#define PROT_DEFAULT		(PTE_TYPE_PAGE | PTE_AF | PTE_SHARED)
#define PROT_SECT_DEFAULT	(PMD_TYPE_SECT | PMD_SECT_AF | PMD_SECT_S)

#define PROT_NORMAL		(PROT_DEFAULT | PTE_PXN | PTE_UXN | PTE_WRITE)

#define PAGE_KERNEL		__pgprot(PROT_NORMAL)
#define PAGE_KERNEL_RO		__pgprot((PROT_NORMAL & ~PTE_WRITE) | PTE_RDONLY)
#define PAGE_KERNEL_ROX		__pgprot((PROT_NORMAL & ~(PTE_WRITE | PTE_PXN)) | PTE_RDONLY)
#define PAGE_KERNEL_EXEC	__pgprot(PROT_NORMAL & ~PTE_PXN)
#define PAGE_KERNEL_EXEC_CONT	__pgprot((PROT_NORMAL & ~PTE_PXN) | PTE_CONT)

#define PAGE_NONE		__pgprot(((PROT_DEFAULT) & ~PTE_VALID) | PTE_PROT_NONE | PTE_RDONLY | PTE_NG | PTE_PXN | PTE_UXN)
/* shared+writable pages are clean by default, hence PTE_RDONLY|PTE_WRITE */
#define PAGE_SHARED		__pgprot(PROT_DEFAULT | PTE_USER | PTE_RDONLY | PTE_NG | PTE_PXN | PTE_UXN | PTE_WRITE)
#define PAGE_SHARED_EXEC	__pgprot(PROT_DEFAULT | PTE_USER | PTE_RDONLY | PTE_NG | PTE_PXN | PTE_WRITE)
#define PAGE_READONLY		__pgprot(PROT_DEFAULT | PTE_USER | PTE_RDONLY | PTE_NG | PTE_PXN | PTE_UXN)
#define PAGE_READONLY_EXEC	__pgprot(PROT_DEFAULT | PTE_USER | PTE_RDONLY | PTE_NG | PTE_PXN)

#define __P000  PAGE_NONE
#define __P001  PAGE_READONLY
#define __P010  PAGE_READONLY
#define __P011  PAGE_READONLY
#define __P100  PAGE_READONLY_EXEC
#define __P101  PAGE_READONLY_EXEC
#define __P110  PAGE_READONLY_EXEC
#define __P111  PAGE_READONLY_EXEC

#define __S000  PAGE_NONE
#define __S001  PAGE_READONLY
#define __S010  PAGE_SHARED
#define __S011  PAGE_SHARED
#define __S100  PAGE_READONLY_EXEC
#define __S101  PAGE_READONLY_EXEC
#define __S110  PAGE_SHARED_EXEC
#define __S111  PAGE_SHARED_EXEC

#endif /* __ASSEMBLY__ */

#endif /* __ASM_PGTABLE_PROT_H */
