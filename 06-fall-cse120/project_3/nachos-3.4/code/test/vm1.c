/*
 * Program that only references some of the pages
 * 
 * Expected output:
 * Thread spaceID 1 Exiting with value 0
 * No threads ready or runnable, and no pending interrupts.
 * Assuming the program completed.
 * Machine halting!
 *
 * Ticks: total 225, idle 0, system 210, user 15
 * Disk I/O: reads 0, writes 0
 * Console I/O: reads 0, writes 0
 * Paging: faults 3, in 3, out 0
 * Network I/O: packets received 0, sent 0
 * Physical memory size: 32 pages
 * 
 * Cleaning up...
 */
#include "syscall.h"

int array[500000];

int main() {
  Exit(0);
}
