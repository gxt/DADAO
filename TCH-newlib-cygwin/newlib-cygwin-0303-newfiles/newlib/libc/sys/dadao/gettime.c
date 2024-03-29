#include "sys/time.h"
#include "time.h"


int _gettimeofday(struct timeval* tv, struct timezone* tz) {
  int ret;
  __asm__ (
    "setzw   rd15, w0, 169 \n"
    "rd2rd   rd16, %1, 1  \n"
    "rd2rd   rd17, %2, 1  \n"
    "trap    cp0, 0       \n"
    "rd2rd   %0, rd31, 1  \n"
    : "=r" (ret)
    : "r" (tv), "r" (tz)
    : "memory"
  );
  return ret;
}

int _settimeofday(const struct timeval *tv, const struct timezone *tz) {
  int ret;
  __asm__ (
    "setzw   rd15, w0, 170 \n"
    "rd2rd   rd16, %1, 1  \n"
    "rd2rd   rd17, %2, 1  \n"
    "trap    cp0, 0       \n"
    "rd2rd   %0, rd31, 1  \n"
    : "=r" (ret)
    : "r" (tv), "r" (tz)
    : "memory"
  );
  return ret;
}


int clock_gettime(clockid_t clk_id, struct timespec *tp) {
  int ret;
  __asm__ (
    "setzw   rd15, w0, 113 \n"
    "rd2rd   rd16, %1, 1  \n"
    "rd2rd   rd17, %2, 1  \n"
    "trap    cp0, 0       \n"
    "rd2rd   %0, rd31, 1  \n"
    : "=r" (ret)
    : "r" (clk_id), "r" (tp)
    : "memory"
  );
  return ret;
}

int clock_settime(clockid_t clk_id, const struct timespec *tp) {
  int ret;
  __asm__ (
    "setzw   rd15, w0, 112 \n"
    "rd2rd   rd16, %1, 1  \n"
    "rd2rd   rd17, %2, 1  \n"
    "trap    cp0, 0       \n"
    "rd2rd   %0, rd31, 1  \n"
    : "=r" (ret)
    : "r" (clk_id), "r" (tp)
    : "memory"
  );
  return ret;
}
