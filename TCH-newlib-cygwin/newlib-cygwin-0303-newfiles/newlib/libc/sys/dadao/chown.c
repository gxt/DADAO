#include <_ansi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "sys/syscall.h"
#include <errno.h>

int
chown (const char *path, short owner, short group)
{
  UNIMPLEMENTED (("path: %s, owner: %d, group: %d", path, owner, group));
  errno = ENOSYS;
  return -1;
}
