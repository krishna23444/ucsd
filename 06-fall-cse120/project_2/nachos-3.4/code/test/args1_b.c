/*
 * Tries to Exec a bad string address
 * 
 * Expected output:
 *  Thread spaceID 1 Exiting with value 0
 *  Thread spaceID 2 Exiting with value 0
 *  Thread spaceID 1 Exiting with value 0
 *  Thread spaceID 3 Exiting with value 1
 *  Thread spaceID 4 Exiting with value 0
 *  Thread spaceID 5 Exiting with value 1
 *  Thread spaceID 6 Exiting with value 0
 *  Thread spaceID 7 Exiting with value 1
 */
#include "syscall.h"

int main(int argc, char* argv[]) {
  int i;
  int id;
  for (i = 1; i < argc; i++) {
    id = Exec(argv[i], 0, 0, 1);
    if (id == 0)
      Exit(10 + i);
  }
  Exit(0); 
}
