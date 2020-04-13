#include <_ansi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "sys/syscall.h"

void _exit (int n)
{
  __asm__ ("SET $255,%0\n\tTRAP 0,0,0"
	   : /* No outputs.  */
	   : "r" (n)
	   : "memory");
}
