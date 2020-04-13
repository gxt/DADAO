#include <_ansi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "sys/syscall.h"
#include <errno.h>

int
_wait (statusp)
     int *statusp;
{
  UNIMPLEMENTED (("statusp: %p", statusp));
  errno = ENOSYS;
  return -1;
}
