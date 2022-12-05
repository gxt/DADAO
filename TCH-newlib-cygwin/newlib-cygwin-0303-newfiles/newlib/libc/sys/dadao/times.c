#include "sys/times.h"

clock_t _times(struct tms *buffer)
{
  clock_t ret;
  __asm__ (
    "setzw   rd15, 3, 153 \n"
    "rd2rd   rd16, %1, 0  \n"
    "trap    cp0, 0       \n"
    "rd2rd   %0, rd31, 0  \n"
    : "=r" (ret)
    : "r" (buffer)
    : "memory"
  );
  return ret;
}
