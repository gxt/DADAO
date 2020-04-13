#include <_ansi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "sys/syscall.h"

int
_isatty (int fd)
{
  return fd == 0 || fd == 1 || fd == 2;
}
