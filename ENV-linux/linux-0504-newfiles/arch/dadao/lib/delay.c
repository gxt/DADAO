#include <linux/delay.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/timex.h>

#define USECS_TO_CYCLES(time_usecs)			\
	xloops_to_cycles((time_usecs) * 0x10C7UL)

static inline unsigned long xloops_to_cycles(unsigned long xloops)
{
	return (xloops * loops_per_jiffy * HZ) >> 32;
}

void __delay(unsigned long cycles)
{
	cycles_t start = get_cycles();

	while ((get_cycles() - start) < cycles)
		cpu_relax();
}
EXPORT_SYMBOL(__delay);

inline void __const_udelay(unsigned long xloops)
{
	__delay(xloops_to_cycles(xloops));
}
EXPORT_SYMBOL(__const_udelay);

void __udelay(unsigned long usecs)
{
	__const_udelay(usecs * 0x10C7UL); /* 2**32 / 1000000 (rounded up) */
}
EXPORT_SYMBOL(__udelay);

void __ndelay(unsigned long nsecs)
{
	__const_udelay(nsecs * 0x5UL); /* 2**32 / 1000000000 (rounded up) */
}
EXPORT_SYMBOL(__ndelay);
