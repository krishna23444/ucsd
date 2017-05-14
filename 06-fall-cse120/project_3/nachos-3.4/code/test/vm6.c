/*
 * Program that demonstrates Random locality
 * 
 * Expected output:
 * Ticks: total 13598891, idle 0, system 4486690, user 9112201
 * Disk I/O: reads 0, writes 0
 * Console I/O: reads 0, writes 0
 * Paging: faults 112165, out 98298
 * Network I/O: packets received 0, sent 0
 * Physical memory size: 32 pages
 * 
 * Cleaning up...
 * 
 */
 #include "syscall.h"
 
unsigned int seed;
int arr[100000];
void
irand_init()
{
  seed = 93186752;
}

/* a very poor random number generator */
unsigned int
irand(int l, int h)
{
  unsigned int a = 1588635695, m = 429496U, q = 2, r = 1117695901;
  unsigned int val;

  seed = a*(seed % q) - r*(seed / q);
  val = (seed / m) % (h - l) + l;
 
  return val;
}

int main() {
  int arr_size = sizeof(arr)/sizeof(arr[0]);
  int i;
  
  irand_init();
  for (i = 0; i < arr_size; i++) {
    arr[irand(0,arr_size)] = i;
  }
  Exit(0);
}
