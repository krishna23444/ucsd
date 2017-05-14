/*
 * Access a memory location that we don't have rights to.
 *
 * Expected output:
 *  Thread 1: Address Error Exception type -900097
 *  Thread spaceID 1 Exiting with value 5
 */
#include "syscall.h"

int main() {
  int* p = 0xFFF243FF;
  if (*p == 1)
    Exit(1);
  Exit(0);
}
