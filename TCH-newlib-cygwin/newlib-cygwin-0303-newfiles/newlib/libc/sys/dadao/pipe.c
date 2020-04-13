#include <_ansi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "sys/syscall.h"
#include <errno.h>

int
pipe (int *fd)
{
  UNIMPLEMENTED (("fd: %p", fd));
  errno = ENOSYS;
  return -1;
}
