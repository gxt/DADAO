#include <_ansi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "sys/syscall.h"
#include <errno.h>

int
_execve (const char *path, char *const argv[], char *const envp[])
{
  UNIMPLEMENTED (("path: %s, argv: %p, envp: %p", path, argv, envp));
  errno = ENOSYS;
  return -1;
}
