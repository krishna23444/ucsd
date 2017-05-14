/*
 * Tries to Exec a a program that does not exist
 * 
 * Expected output
 *  Unable to open file filedoesnotexist
 *  Thread spaceID 1 Exiting with value 1
 */
#include "syscall.h"

int main() {
  if (Exec("filedoesnotexist",0,0,0) == 0)
    Exit(1);
  Exit(0);
}
