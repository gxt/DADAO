
#include <linux/init.h>
#include <linux/linkage.h>
#include <asm/page.h>
#include <asm/pgtable.h>

unsigned long empty_zero_page[PAGE_SIZE / sizeof(unsigned long)] __page_aligned_bss;
EXPORT_SYMBOL(empty_zero_page);

pgd_t swapper_pg_dir[PTRS_PER_PGD] __page_aligned_bss;

void __init mem_init(void)
{
}
