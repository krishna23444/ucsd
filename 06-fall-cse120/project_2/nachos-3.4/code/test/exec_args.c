/*
 * Execute a progam that executes the passed in parameter.
 *
 * Expected output:
 *  Thread spaceID 1 Exiting with value 0
 *  Thread spaceID 2 Exiting with value 0
 *  Machine halting!
 */
#include "syscall.h"

int main() {
  char* tmp = "test/halt";
  if (Exec("test/main_with_args", 1, &tmp, 0) == 0)
    Exit(1);
  Exit(0);
}
