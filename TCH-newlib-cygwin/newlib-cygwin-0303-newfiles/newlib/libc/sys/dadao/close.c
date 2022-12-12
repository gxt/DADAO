
int close(int file) {
  int ret;
  __asm__ (
    "setzw   rd15, w3, 57  \n"
    "rd2rd   rd16, %1, 0  \n"
    "trap    cp0, 0       \n"
    "rd2rd   %0, rd31, 0  \n"
    : "=r" (ret)
    : "r" (file)
    : "memory"
  );
  return ret;
}

int _close(int file) {
  return close(file);
}
