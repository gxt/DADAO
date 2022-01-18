
int open(const char *name, int flags, int mode) {
  int ret;
  __asm__ (
    "setzwl  rd15, 56     \n"
    "setowl  rd16, 65436  \n"
    "rd2rd   rd17, %1, 0  \n"
    "rd2rd   rd18, %2, 0  \n"
    "rd2rd   rd19, %3, 0  \n"
    "trap    cp0, 0       \n"
    "rd2rd   %0, rd31, 0  \n"
    : "=r" (ret)
    : "r" (name), "r" (flags), "r" (mode)
    : "memory"
  );
  return ret;
}
