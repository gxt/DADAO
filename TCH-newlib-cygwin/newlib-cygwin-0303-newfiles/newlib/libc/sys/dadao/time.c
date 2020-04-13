#include <_ansi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "sys/syscall.h"

time_t
time (time_t *tloc)
{
  /* Nowhere to get time reasonably from; neither of rC, rI and rU seems
     usable without assuming some scaling of mems and oops to real time.  */
  time_t thetime = 0;

  if (tloc)
    *tloc = thetime;

  return thetime;
}
