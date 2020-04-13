#include <_ansi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "sys/syscall.h"
#include <errno.h>

int
chmod (const char *path, mode_t mode)
{
  UNIMPLEMENTED (("path: %s, mode: %lu", path, (unsigned long) mode));
  errno = ENOSYS;
  return -1;
}
