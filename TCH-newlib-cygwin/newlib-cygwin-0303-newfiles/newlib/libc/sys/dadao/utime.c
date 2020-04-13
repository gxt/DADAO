#include <_ansi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "sys/syscall.h"
#include <errno.h>

int
utime (path, times)
     const char *path;
     char *times;
{
  UNIMPLEMENTED (("path: %s, times: %p", path, times));
  errno = ENOSYS;
  return -1;
}
