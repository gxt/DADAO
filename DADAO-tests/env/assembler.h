#ifndef DADAO_ASSEMBLER_H
#define DADAO_ASSEMBLER_H

// TODO: SHOULD redefine following regs, but now reuse risc-v csr module
#if 0
#define CSR_mstatus		0x300
#define CSR_misa		0x301
#define CSR_mideleg		0x303
#define CSR_mie			0x304
#define CSR_mtvec		0x305
#define CSR_mepc		0x341
#define CSR_mcause		0x342
#define CSR_mhartid		0xf14
#endif

#define CPRD_mstatus(regd)	cprd	cp0, cr12, cr0, regd
#define CPRD_misa(regd)		cprd	cp0, cr12, cr1, regd
#define CPRD_mepc(regd)		cprd	cp0, cr13, cr1, regd
#define CPRD_mcause(regd)	cprd	cp0, cr13, cr2, regd
#define CPRD_mhartid(regd)	cprd	cp0, cr60, cr20, regd

#define CPWR_mstatus(regd)	cpwr	cp0, cr12, cr0, regd
#define CPWR_mideleg(regd)	cpwr	cp0, cr12, cr3, regd
#define CPWR_mie(regd)		cpwr	cp0, cr12, cr4, regd
#define CPWR_mtvec(regd)	cpwr	cp0, cr12, cr5, regd
#define CPWR_mepc(regd)		cpwr	cp0, cr13, cr1, regd

#endif /* DADAO_ASSEMBLER_H */
