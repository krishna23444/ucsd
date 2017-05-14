/*
 * Tries to join a non-existing thread
 * 
 * Expected output:
 *  Unable to open file filedoesnotexist
 *  Thread spaceID 1 Exiting with value 150
 */
#include "syscall.h"

int main() {
  int newProc;
  newProc = Exec("filedoesnotexist", 0, 0, 0);
  if (Join(newProc) == -65535)
    Exit(150);
  Exit(400);
}
