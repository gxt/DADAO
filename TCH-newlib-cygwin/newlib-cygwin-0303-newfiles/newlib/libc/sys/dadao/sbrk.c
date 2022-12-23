#include <sys/types.h>

static caddr_t brk(caddr_t addr) {
  int ret;
  __asm__ (
    "setzw   rd15, w0, 214 \n"
    "rd2rd   rd16, %1, 0  \n"
    "trap    cp0, 0       \n"
    "rd2rd   %0, rd31, 0  \n"
    : "=r" (ret)
    : "r" (addr)
    : "memory"
  );
  return ret;
}

static caddr_t _Sbrk_high = 0;

caddr_t _sbrk(size_t incr)
{
  if(_Sbrk_high == 0) {
    _Sbrk_high = brk(_Sbrk_high);
  }
  caddr_t prev_heap_end = _Sbrk_high;
  _Sbrk_high += incr;
  _Sbrk_high = brk(_Sbrk_high);
  return prev_heap_end;
}
