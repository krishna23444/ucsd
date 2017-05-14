/* 
 * Simple program to test exit system call.
 *
 * Expected output:
 *  Thread spaceID 1 Exiting with value 0
 */
#include "syscall.h"

int main() {
  Exit(0);
}
