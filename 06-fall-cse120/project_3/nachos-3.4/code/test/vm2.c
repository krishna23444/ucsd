/*
 * Program that references all of the pages in memory
 * 
 * Expected output:
 * Thread spaceID 1 Exiting with value 0
 * No threads ready or runnable, and no pending interrupts.
 * Assuming the program completed.
 * Machine halting!
 * 
 * Ticks: total 10709698, idle 0, system 692370, user 10017328
 * Disk I/O: reads 0, writes 0
 * Console I/O: reads 0, writes 0
 * Paging: faults 17307, in 1681, out 16202
 * Network I/O: packets received 0, sent 0
 * Physical memory size: 32 pages
 * 
 * Cleaning up...
 */
#include "syscall.h"

int array[500000];

int main() {
  int i;
  for (i = 0; i < sizeof(array)/sizeof(array[0]); ++i)
    array[i] = i;
  Exit(0);
}
