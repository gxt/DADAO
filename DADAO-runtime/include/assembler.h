#ifndef DADAO_ASSEMBLER_H
#define DADAO_ASSEMBLER_H

#ifdef	__ASSEMBLER__
#define CPRD_mstatus(regd)	cprd	cp0, cr12, cr0, regd
#define CPRD_misa(regd)		cprd	cp0, cr12, cr1, regd
#define CPRD_mepc(regd)		cprd	cp0, cr13, cr1, regd
#define CPRD_mcause(regd)	cprd	cp0, cr13, cr2, regd
#define CPRD_mcycle(regd)	cprd	cp0, cr44, cr0, regd
#define CPRD_minstret(regd)	cprd	cp0, cr44, cr2, regd
#define CPRD_mhartid(regd)	cprd	cp0, cr60, cr20, regd

#define CPWR_mstatus(regd)	cpwr	cp0, cr12, cr0, regd
#define CPWR_mideleg(regd)	cpwr	cp0, cr12, cr3, regd
#define CPWR_mie(regd)		cpwr	cp0, cr12, cr4, regd
#define CPWR_mtvec(regd)	cpwr	cp0, cr12, cr5, regd
#define CPWR_mepc(regd)		cpwr	cp0, cr13, cr1, regd
#endif

#endif /* DADAO_ASSEMBLER_H */
