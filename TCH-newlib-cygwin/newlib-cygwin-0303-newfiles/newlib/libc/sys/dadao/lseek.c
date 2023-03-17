
int lseek(int file, int ptr, int dir) {
  int ret;
  __asm__ (
    "setzw   rd15, w0, 62  \n"
    "rd2rd   rd16, %1, 1  \n"
    "rd2rd   rd17, %2, 1  \n"
    "rd2rd   rd18, %3, 1  \n"
    "trap    cp0, 0       \n"
    "rd2rd   %0, rd31, 1  \n"
    : "=r" (ret)
    : "r" (file), "r" (ptr), "r" (dir)
    : "memory"
  );
  return ret;
}

int _lseek(int file, int ptr, int dir) {
  return lseek(file, ptr, dir);
}
