#ifndef __ASM_DADAO_TLBFLUSH_H
#define __ASM_DADAO_TLBFLUSH_H

#define flush_tlb()				BUG()
#define flush_tlb_all()				BUG()
#define flush_tlb_mm(mm)			BUG()
#define flush_tlb_page(vma, addr)		BUG()
#define flush_tlb_range(mm, start, end)		BUG()
#define flush_tlb_pgtables(mm, start, end)	BUG()
#define flush_tlb_kernel_range(start, end)	BUG()

#endif /* __ASM_DADAO_TLBFLUSH_H */
