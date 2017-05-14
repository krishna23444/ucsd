/*
 * Tries to join a non-joinable thread
 * 
 * Expected output:
 *  Thread spaceID 1 Exiting with value 150
 *  Thread spaceID 2 Exiting with value 0
 */
#include "syscall.h"

int main() {
  int newProc;
  newProc = Exec("test/exit1", 0, 0, 0);
  if (newProc == 0)
    Exit(100);
  if (Join(newProc) == -65535)
    Exit(150);
  Exit(400);
}
