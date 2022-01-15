#include <_ansi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "sys/syscall.h"

void _exit (int n)
{
  __asm__ ("orr	rd63, %0, rd0\n\ttrap	cp0, 0"
	   : /* No outputs.  */
	   : "r" (n)
	   : "memory");
}
