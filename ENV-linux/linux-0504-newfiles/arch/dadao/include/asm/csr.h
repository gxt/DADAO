#ifndef __ASM_DADAO_CSR_H
#define __ASM_DADAO_CSR_H

#include <linux/const.h>

#define PRIV_MODE	_AC(0xDADA0000, UL)

/* Status register flags */
#define SR_SIE		_AC(0x00000002, UL) /* Supervisor Interrupt Enable */
#define SR_SPIE		_AC(0x00000020, UL) /* Previous Supervisor IE */
#define SR_SPP		_AC(0x00000100, UL) /* Previously Supervisor */
#define SR_SUM		_AC(0x00040000, UL) /* Supervisor User Memory Access */

#define CSR_CYCLE		0xc00
#define CSR_TIME		0xc01
#define CSR_INSTRET		0xc02
#define CSR_SSTATUS		0x100
#define CSR_SIE			0x104
#define CSR_STVEC		0x105
#define CSR_SCOUNTEREN		0x106
#define CSR_SSCRATCH		0x140
#define CSR_SEPC		0x141
#define CSR_SCAUSE		0x142
#define CSR_STVAL		0x143
#define CSR_SIP			0x144
#define CSR_SATP		0x180
#define CSR_CYCLEH		0xc80
#define CSR_TIMEH		0xc81
#define CSR_INSTRETH		0xc82

#ifndef __ASSEMBLY__

#define cp0_read(csr)						\
({								\
	register unsigned long __v;				\
	__asm__ __volatile__ ("cprd cp0, cr%1, cr%2, %0 "	\
			: "=r" (__v)				\
			: "i" (csr >> 6), "i" (csr & 0x3f)	\
			: "memory");				\
	__v;							\
})

#define cp0_write(csr, val)					\
({								\
	unsigned long __v = (unsigned long)(val);		\
	__asm__ __volatile__ ("cpwr cp0, cr%1, cr%2, %0"	\
			: : "r" (__v)				\
			, "i" (csr >> 6), "i" (csr & 0x3f)	\
			: "memory");				\
})

#endif /* __ASSEMBLY__ */

#endif /* __ASM_DADAO_CSR_H */
