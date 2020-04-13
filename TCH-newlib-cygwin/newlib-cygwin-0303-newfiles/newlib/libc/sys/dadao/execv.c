#include <_ansi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "sys/syscall.h"
#include <errno.h>

int
execv (const char *path, char *const argv[])
{
  UNIMPLEMENTED (("path: %s, argv: %p", path, argv));
  errno = ENOSYS;
  return -1;
}
