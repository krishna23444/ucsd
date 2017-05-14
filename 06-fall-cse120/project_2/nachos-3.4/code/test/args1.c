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

int main() {
  char *array[6];
  array[0] = "test/exit1";
  array[1] = "test/exit2";
  array[2] = "test/exit3";
  array[3] = "test/exit4";
  array[4] = "test/exit1";
  array[5] = "test/exit2";
  if (Exec("test/args1_b",6,array,0) == 0)
    Exit(1);
  Exit(0);
}
