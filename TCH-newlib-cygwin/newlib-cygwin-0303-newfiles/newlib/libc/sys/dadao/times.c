#include <_ansi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "sys/syscall.h"
#include "sys/times.h"

clock_t
_times (struct tms *buffer)
{
  memset (buffer, 0, sizeof (*buffer));
  return 0;
}
