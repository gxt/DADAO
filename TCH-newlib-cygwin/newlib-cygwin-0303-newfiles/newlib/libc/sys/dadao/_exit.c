#include <_ansi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "sys/syscall.h"

void _exit (int n)
{
  __asm__ ("orr	rg63, %0, rg0\n\ttrap	cp0, 0"
	   : /* No outputs.  */
	   : "r" (n)
	   : "memory");
}
