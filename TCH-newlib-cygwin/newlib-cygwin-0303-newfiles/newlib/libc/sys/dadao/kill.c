#include <_ansi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "sys/syscall.h"
#include <errno.h>

_kill (n, m)
{
  if (n == 1)
    return TRAP1i (SYS_halt, 0);

  errno = EPERM;
  return -1;
}

