
int close(int file) {
  int ret;
  __asm__ (
    "setzwl  rd15, 57     \n"
    "rd2rd   rd16, %1, 0  \n"
    "trap    cp0, 0       \n"
    "rd2rd   %0, rd31, 0  \n"
    : "=r" (ret)
    : "r" (file)
    : "memory"
  );
  return ret;
}
