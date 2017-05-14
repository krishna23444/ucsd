/*
 * Tries to Exec a program that cannot fit in memory
 * 
 * Expected output:
 *  Thread spaceID 1 Exiting with value 1
 */
#include "syscall.h"

int main() {
  if (Exec("test/exec4_huge",0,0,0) == 0)
    Exit(1);
  Exit(0);
}
