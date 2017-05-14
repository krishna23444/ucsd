/*
 * Program that demonstrates good locality
 * 
 * Expected output:
 * Thread spaceID 1 Exiting with value 0
 * No threads ready or runnable, and no pending interrupts.
 * Assuming the program completed.
 * Machine halting!
 * 
 * Ticks: total 11128, idle 0, system 410, user 10718
 * Disk I/O: reads 0, writes 0
 * Console I/O: reads 0, writes 0
 * Paging: faults 8, out 0
 * Network I/O: packets received 0, sent 0
 * Physical memory size: 32 pages
 * 
 * Cleaning up...
 */
 #include "syscall.h"

int arr[100000];

int main() {
  int i,j;
  for (j = 0; j < 10; ++j)
    for (i = 2000; i < 2050; ++i)
      arr[i] = j+i;
  Exit(0);
}
