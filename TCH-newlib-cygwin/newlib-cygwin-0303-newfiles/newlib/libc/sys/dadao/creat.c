#include <_ansi.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "sys/syscall.h"
#include <errno.h>

int
creat (const char *path,
       /* We can't set the mode, so we have to ignore it.  */
       mode_t mode)
{
  return open (path, O_CREAT|O_WRONLY|O_TRUNC);
}
