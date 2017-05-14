/* sort.c 
 *    Test program to sort a large number of integers.
 *
 *    Intention is to stress virtual memory system.
 *
 *    Ideally, we could read the unsorted array off of the file system,
 *    and store the result back to the file system!
 *
 * Expected Output:
 * Thread spaceID 1 Exiting with value 1023
 * No threads ready or runnable, and no pending interrupts.
 * Assuming the program completed.
 * Machine halting!
 * 
 * Ticks: total 17117598, idle 0, system 31370, user 17086228
 * Disk I/O: reads 0, writes 0
 * Console I/O: reads 0, writes 0
 * Paging: faults 782, in 782, out 665
 * Network I/O: packets received 0, sent 0
 * Physical memory size: 32 pages
 * 
 * Cleaning up...
 */
#include "syscall.h"

int A[1024];  /* size of physical memory; with code, we'll run out of space!*/

int main() {
  int i, j, tmp;

  /* first initialize the array, in reverse sorted order */
  for (i = 0; i < 1024; i++)    
    A[i] = 1024 - i;

  /* then sort! */
  for (i = 0; i < 1023; i++)
    for (j = i; j < (1023 - i); j++)
      if (A[j] > A[j + 1]) {  /* out of order -> need to swap ! */
        tmp = A[j];
        A[j] = A[j + 1];
        A[j + 1] = tmp;
      }
  Exit(A[0]);    /* and then we're done -- should be 0! */
}
