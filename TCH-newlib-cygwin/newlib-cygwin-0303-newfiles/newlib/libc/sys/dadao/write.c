#include <_ansi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "sys/syscall.h"
#include <errno.h>

int
_write ( int file,
	 char *ptr,
	 size_t len)
{
  long ret;

  if ((unsigned int) file >= 32 || _DADAO_allocated_filehandle[file] == 0)
    {
      errno = EBADF;
      return -1;
    }

  ret = TRAP3f (SYS_Fwrite, file, ptr, len);

  if (ret < 0)
    {
      errno = EIO;
      return -1;
    }

  return len;
}
