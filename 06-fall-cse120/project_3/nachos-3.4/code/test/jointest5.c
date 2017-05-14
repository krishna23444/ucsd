/*
 * Thread tries to join itself
 * 
 * Expected output:
 *  Thread spaceID 1 Exiting with value 1
 */
#include "syscall.h"

int main() {
  if (Join(1) == -65535)
    Exit(1);
  Exit(0);
}
