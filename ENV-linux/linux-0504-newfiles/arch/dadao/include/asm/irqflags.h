#ifndef _ASM_DADAO_IRQFLAGS_H
#define _ASM_DADAO_IRQFLAGS_H

#include <asm/csr.h>

#define ARCH_IRQ_DISABLED	(PRIV_MODE | SR_SIE)
#define ARCH_IRQ_ENABLED	(PRIV_MODE)

/* read interrupt enabled status */
static inline unsigned long arch_local_save_flags(void)
{
	return cp0_read(CSR_SSTATUS);
}

/* set interrupt enabled status */
static inline void arch_local_irq_restore(unsigned long flags)
{
	cp0_write(CSR_SSTATUS, flags);
}

#include <asm-generic/irqflags.h>

#endif /* _ASM_DADAO_IRQFLAGS_H */
