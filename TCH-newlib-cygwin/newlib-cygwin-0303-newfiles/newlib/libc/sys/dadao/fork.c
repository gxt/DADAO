#include <_ansi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "sys/syscall.h"
#include <errno.h>

int
_fork ()
{
  UNIMPLEMENTED ((""));
  errno = ENOSYS;
  return -1;
}
