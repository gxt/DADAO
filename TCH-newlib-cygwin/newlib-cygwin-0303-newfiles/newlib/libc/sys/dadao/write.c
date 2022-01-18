
int write(int file, char *ptr, int len) {
  int ret;
  __asm__ (
    "setzwl  rd15, 64     \n"
    "rd2rd   rd16, %1, 0  \n"
    "rd2rd   rd17, %2, 0  \n"
    "rd2rd   rd18, %3, 0  \n"
    "trap    cp0, 0       \n"
    "rd2rd   %0, rd31, 0  \n"
    : "=r" (ret)
    : "r" (file), "r" (ptr), "r" (len)
    : "memory"
  );
  return ret;
}
