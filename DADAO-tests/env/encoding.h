#ifndef DADAO_ENCODING_H
#define DADAO_ENCODING_H

#include "assembler.h"

// TODO: SHOULD redefine following regs, but now reuse risc-v csr module
#if 1
#define CSR_mstatus		0x300
#define CSR_misa		0x301
#define CSR_mideleg		0x303
#define CSR_mie			0x304
#define CSR_mtvec		0x305
#define CSR_mepc		0x341
#define CSR_mcause		0x342
#define CSR_mcycle		0xb00
#define CSR_minstret		0xb02
#define CSR_cycle		0xc00
#define CSR_time		0xc01
#define CSR_instret		0xc02
#define CSR_mhartid		0xf14
#endif

#ifndef __ASSEMBLER__
#ifdef __GNUC__

#define read_csr(reg) ({ unsigned long __tmp;						\
  switch (reg) {									\
    case CSR_mcycle:	asm volatile ("cprd cp0, cr44, cr0, %0": "=r"(__tmp)); break;	\
    case CSR_minstret:	asm volatile ("cprd cp0, cr44, cr2, %0": "=r"(__tmp)); break;	\
    case CSR_cycle:	asm volatile ("cprd cp0, cr48, cr0, %0": "=r"(__tmp)); break;	\
    case CSR_time:	asm volatile ("cprd cp0, cr48, cr1, %0": "=r"(__tmp)); break;	\
    case CSR_instret:	asm volatile ("cprd cp0, cr48, cr2, %0": "=r"(__tmp)); break;	\
  } __tmp; })

#define write_csr(reg, val) ({ \
  asm volatile ("csrw " #reg ", %0" :: "rK"(val)); })

#define rdtime()	read_csr(CSR_time)
#define rdcycle()	read_csr(CSR_cycle)
#define rdinstret()	read_csr(CSR_instret)

#endif
#endif

#endif /* DADAO_ENCODING_H */
