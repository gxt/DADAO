
void _exit(int n) {
  __asm__ (
    "setzw   rd15, w0, 93  \n"
    "rd2rd   rd16, %0, 1  \n"
    "trap    cp0, 0       \n"
    : 
    : "r" (n)
    : "memory"
  );
}
