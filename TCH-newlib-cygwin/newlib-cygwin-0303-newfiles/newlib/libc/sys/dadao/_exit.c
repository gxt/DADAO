#include <_ansi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "sys/syscall.h"

void _exit (int n)
{
  __asm__ ("SET $63,%0\n\ttrap 0"
	   : /* No outputs.  */
	   : "r" (n)
	   : "memory");
}
