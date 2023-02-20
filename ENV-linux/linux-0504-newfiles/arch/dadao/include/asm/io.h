#ifndef __ASM_IO_H
#define __ASM_IO_H

#include <linux/types.h>

#include <asm/byteorder.h>
#include <asm/barrier.h>
#include <asm/pgtable.h>

/*
 * I/O memory mapping functions.
 */
extern void __iomem *__ioremap(phys_addr_t phys_addr, size_t size, pgprot_t prot);
extern void iounmap(volatile void __iomem *addr);
extern void __iomem *ioremap_cache(phys_addr_t phys_addr, size_t size);

#include <asm-generic/io.h>

#endif	/* __ASM_IO_H */
