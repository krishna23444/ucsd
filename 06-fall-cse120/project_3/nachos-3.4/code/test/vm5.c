/*
 * Program that demonstrates poor locality
 * 
 * Expected output:
 * Thread spaceID 1 Exiting with value 0
 * No threads ready or runnable, and no pending interrupts.
 * Assuming the program completed.
 * Machine halting!
 * 
 * Ticks: total 12481063, idle 0, system 8759370, user 3721693
 * Disk I/O: reads 0, writes 0
 * Console I/O: reads 0, writes 0
 * Paging: faults 218982, out 205203
 * Network I/O: packets received 0, sent 0
 * Physical memory size: 32 pages

 * 
 * Cleaning up...
 */
 #include "syscall.h"

int arr[100000];

int main() {
  int arr_size = sizeof(arr)/sizeof(arr[0]);
  int i,j;
  for (j = 0; j < 128; ++j)
    for (i = j; i < arr_size; i+=128) {
      arr[i] = j+i;
      arr[arr_size-i] = j+i;
    }
  Exit(0);
}
