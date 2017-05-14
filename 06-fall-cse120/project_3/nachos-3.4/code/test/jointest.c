/*
 * Runs two joinable thread, calling join after each.
 * 
 * Expected output
 *  Thread spaceID 2 Exiting with value 0
 *  Thread spaceID 2 Exiting with value 1
 *  Thread spaceID 1 Exiting with value 400
 */
#include "syscall.h"

int main() {
  int newProc;
  newProc = Exec("test/exit1", 0, 0, 1);
  if (newProc == 0)
    Exit(100);
  if (Join(newProc) == -65535)
    Exit(150);
  newProc = Exec("test/exit2", 0,0,1);
  if (newProc == 0)
    Exit(200);
  if (Join(newProc) == -65535)
    Exit(250);
  Exit(400);
}
