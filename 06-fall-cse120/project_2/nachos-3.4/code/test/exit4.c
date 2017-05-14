/* 
 * Simple program to test exit system call.
 *
 * Expected output:
 *  Thread spaceID 1 Exiting with value 1
 */
#include "syscall.h"

int main() {
  Exit(1);
}
