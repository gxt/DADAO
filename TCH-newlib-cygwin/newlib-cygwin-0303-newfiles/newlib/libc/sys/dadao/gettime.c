#include <_ansi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "sys/syscall.h"
#include "sys/time.h"
#include <errno.h>

int
_gettimeofday (struct timeval *tp, void *tzp)
{
  UNIMPLEMENTED (("timeval: %p, tzp: %p", tp, tzp));
  errno = ENOSYS;
  return -1;
}
