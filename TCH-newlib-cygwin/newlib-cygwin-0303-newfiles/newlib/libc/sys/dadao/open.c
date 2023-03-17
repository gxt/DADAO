
int open(const char *name, int flags, int mode) {
  int ret;
  __asm__ (
    "setzw   rd15, w0, 56  \n"
    "setow   rd16, w0, 65436  \n"
    "rd2rd   rd17, %1, 1  \n"
    "rd2rd   rd18, %2, 1  \n"
    "rd2rd   rd19, %3, 1  \n"
    "trap    cp0, 0       \n"
    "rd2rd   %0, rd31, 1  \n"
    : "=r" (ret)
    : "r" (name), "r" (flags), "r" (mode)
    : "memory"
  );
  return ret;
}

int _open(const char *name, int flags, int mode) {
  return open(name, flags, mode);
}
