#include <_ansi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "sys/syscall.h"
#include <errno.h>

int
_stat (const char *path, struct stat *st)

{
  UNIMPLEMENTED (("path: %s, st: %p", path, st));
  errno = ENOSYS;
  return -1;
}
