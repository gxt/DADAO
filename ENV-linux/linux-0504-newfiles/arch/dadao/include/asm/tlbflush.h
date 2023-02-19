#ifndef __ASM_DADAO_TLBFLUSH_H
#define __ASM_DADAO_TLBFLUSH_H

#define flush_tlb()				BUG()
#define flush_tlb_all()				BUG()
#define flush_tlb_mm(mm)			BUG()
#define flush_tlb_page(vma, addr)		BUG()
#define flush_tlb_pgtables(mm, start, end)	BUG()
#define flush_tlb_kernel_range(start, end)	BUG()

static inline void local_flush_tlb_all(void)
{
	/* FIXME */
	__asm__ __volatile__ ("cpco cp0, 0" : : : "memory");
}

static inline void flush_tlb_range(struct vm_area_struct *vma,
		unsigned long start, unsigned long end)
{
	local_flush_tlb_all();
}

#endif /* __ASM_DADAO_TLBFLUSH_H */
