#include <support.h>

void
initialise_board ()
{
  __asm__ volatile ("swym	0" : : : "memory");
}

void __attribute__ ((noinline)) __attribute__ ((externally_visible))
start_trigger ()
{
  __asm__ volatile ("swym	0" : : : "memory");
}

void __attribute__ ((noinline)) __attribute__ ((externally_visible))
stop_trigger ()
{
  __asm__ volatile ("swym	0" : : : "memory");
}
