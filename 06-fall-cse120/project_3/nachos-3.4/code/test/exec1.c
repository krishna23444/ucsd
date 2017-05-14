/*
 * Tries to Exec a bad string address
 * 
 * Expected output:
 *  Unable to openfile
 *  Thread spaceID 1 Exiting with value 1
 */
#include "syscall.h"

int main() {
  if (Exec((char*)4,0,0,0) == 0)
    Exit(1);
  Exit(0);
}
