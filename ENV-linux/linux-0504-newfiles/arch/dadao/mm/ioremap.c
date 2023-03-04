#include <linux/export.h>
#include <linux/mm.h>
#include <linux/vmalloc.h>
#include <linux/io.h>

#include <asm/tlbflush.h>
#include <asm/pgalloc.h>

void __iomem *ioremap(phys_addr_t phys_addr, size_t size)
{
	return ((void __iomem *)phys_addr);
}
EXPORT_SYMBOL(ioremap);

void iounmap(volatile void __iomem *io_addr)
{
	unsigned long addr = (unsigned long)io_addr & PAGE_MASK;

	/*
	 * We could get an address outside vmalloc range in case
	 * of ioremap_cache() reusing a RAM mapping.
	 */
	if (is_vmalloc_addr((void *)addr))
		vunmap((void *)addr);
}
EXPORT_SYMBOL(iounmap);

void __iomem *ioremap_cache(phys_addr_t phys_addr, size_t size)
{
	return ((void __iomem *)phys_addr);
}
EXPORT_SYMBOL(ioremap_cache);
