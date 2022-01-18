
void _exit(int n) {
  __asm__ (
    "setzwl  rd15, 93     \n"
    "rd2rd   rd16, %0, 0  \n"
    "trap    cp0, 0       \n"
    : 
    : "r" (n)
    : "memory"
  );
}
