/*
 * Tries to Exec a bad string address
 * 
 * Expected output:
 *  Thread spaceID 3 Exiting with value 0
 *  Thread spaceID 3 Exiting with value 1
 *  Thread spaceID 3 Exiting with value 0
 *  Thread spaceID 3 Exiting with value 1
 *  Thread spaceID 3 Exiting with value 0
 *  Thread spaceID 3 Exiting with value 1
 *  Thread spaceID 2 Exiting with value 0
 *  Thread spaceID 1 Exiting with value 0
 */
#include "syscall.h"

int main(int argc, char* argv[]) {
  int i;
  int id;
  for (i = 1; i < argc; i++) {
    id = Exec(argv[i], 0, 0, 1);
    if (id == 0)
      Exit(10 + i);
    if (Join(id)==-65535)
      Exit(20+i);
  }
  Exit(0); 
}
