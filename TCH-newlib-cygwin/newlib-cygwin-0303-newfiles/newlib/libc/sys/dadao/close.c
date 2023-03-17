
int close(int file) {
  int ret;
  __asm__ (
    "setzw   rd15, w0, 57  \n"
    "rd2rd   rd16, %1, 1  \n"
    "trap    cp0, 0       \n"
    "rd2rd   %0, rd31, 1  \n"
    : "=r" (ret)
    : "r" (file)
    : "memory"
  );
  return ret;
}

int _close(int file) {
  return close(file);
}
