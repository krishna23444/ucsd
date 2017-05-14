/*
 * Exec a simple program a couple of times
 *
 * Expected output:
 *  Thread spaceID 1 Exiting with value 0
 *  Thread spaceID 2 Exiting with value 0
 *  Thread spaceID 3 Exiting with value 1
 *  Thread spaceID 4 Exiting with value 1
 *  Thread spaceID 5 Exiting with value 0
 */
#include "syscall.h"

int main() {   
  if (Exec("test/exit1", 0, 0, 0) == 0)
    Exit(1);

  if (Exec("test/exit2", 0, 0, 0) == 0)
    Exit(2);
 
  if (Exec("test/exit2", 0, 0, 0) == 0)
    Exit(3);
   
  if (Exec("test/exit1", 0, 0, 0) == 0)
    Exit(4);

  Exit(0);
}
