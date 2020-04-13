#include <_ansi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "sys/syscall.h"
#include <errno.h>

int
_close (int file)
{
  if ((unsigned int) file >= N_DADAO_FILEHANDLES
      || _DADAO_allocated_filehandle[file] == 0)
    {
      errno = EBADF;
      return -1;
    }

  _DADAO_allocated_filehandle[file] = 0;

  if (TRAP1f (SYS_Fclose, file) != 0)
    {
      errno = EIO;
      return -1;
    }
  return 0;
}
