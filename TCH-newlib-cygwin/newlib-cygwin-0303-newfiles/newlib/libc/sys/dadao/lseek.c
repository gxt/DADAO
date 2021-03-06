
int lseek(int file, int ptr, int dir) {
  int ret;
  __asm__ (
    "setzwl  rd15, 62     \n"
    "rd2rd   rd16, %1, 0  \n"
    "rd2rd   rd17, %2, 0  \n"
    "rd2rd   rd18, %3, 0  \n"
    "trap    cp0, 0       \n"
    "rd2rd   %0, rd31, 0  \n"
    : "=r" (ret)
    : "r" (file), "r" (ptr), "r" (dir)
    : "memory"
  );
  return ret;
}

int _lseek(int file, int ptr, int dir) {
  return lseek(file, ptr, dir);
}
