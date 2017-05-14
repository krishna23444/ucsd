/*
 * Demonstrate the page replacement policy and correct handling of dirty pages
 * 
 * Expected output:
 * Thread spaceID 1 Exiting with value 0
 * No threads ready or runnable, and no pending interrupts.
 * Assuming the program completed.
 * Machine halting!
 * 
 * Ticks: total 4282241, idle 0, system 275330, user 4006911
 * Disk I/O: reads 0, writes 0
 * Console I/O: reads 0, writes 0
 * Paging: faults 6881, in 3755, out 3348
 * Network I/O: packets received 0, sent 0
 * Physical memory size: 32 pages
 * 
 * Cleaning up...
 */
#include "syscall.h"

int array[100000];

int main() {
  int i;
  for (i = 0; i < sizeof(array)/sizeof(array[0]); ++i)
    array[i] = i;
  for (i = sizeof(array)/sizeof(array[0])-1; i >= 0; --i)
    if (array[i] != i) {
      Exit(i+1);
    }
  Exit(0);
}
