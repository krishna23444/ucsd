/*
 * Tries to Exec a program that cannot fit in memory
 * 
 * Expected output:
 *  Thread spaceID 1 Exiting with value 1
 */
#include "syscall.h"

int array[5000000];

int main() {
  Exit(0);
}
