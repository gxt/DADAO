#include "sys/time.h"
#include "time.h"


int _gettimeofday(struct timeval* tv, struct timezone* tz) {
  int ret;
  __asm__ (
    "setzwl  rd15, 169    \n"
    "rd2rd   rd16, %1, 0  \n"
    "rd2rd   rd17, %2, 0  \n"
    "trap    cp0, 0       \n"
    "rd2rd   %0, rd31, 0  \n"
    : "=r" (ret)
    : "r" (tv), "r" (tz)
    : "memory"
  );
  return ret;
}

int _settimeofday(const struct timeval *tv, const struct timezone *tz) {
  int ret;
  __asm__ (
    "setzwl  rd15, 170    \n"
    "rd2rd   rd16, %1, 0  \n"
    "rd2rd   rd17, %2, 0  \n"
    "trap    cp0, 0       \n"
    "rd2rd   %0, rd31, 0  \n"
    : "=r" (ret)
    : "r" (tv), "r" (tz)
    : "memory"
  );
  return ret;
}


int clock_gettime(clockid_t clk_id, struct timespec *tp) {
  int ret;
  __asm__ (
    "setzwl  rd15, 113    \n"
    "rd2rd   rd16, %1, 0  \n"
    "rd2rd   rd17, %2, 0  \n"
    "trap    cp0, 0       \n"
    "rd2rd   %0, rd31, 0  \n"
    : "=r" (ret)
    : "r" (clk_id), "r" (tp)
    : "memory"
  );
  return ret;
}

int clock_settime(clockid_t clk_id, const struct timespec *tp) {
  int ret;
  __asm__ (
    "setzwl  rd15, 112    \n"
    "rd2rd   rd16, %1, 0  \n"
    "rd2rd   rd17, %2, 0  \n"
    "trap    cp0, 0       \n"
    "rd2rd   %0, rd31, 0  \n"
    : "=r" (ret)
    : "r" (clk_id), "r" (tp)
    : "memory"
  );
  return ret;
}
