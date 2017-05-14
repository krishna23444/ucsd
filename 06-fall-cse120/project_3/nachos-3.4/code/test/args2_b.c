/*
 * Execute a progam that executes the passed in parameter.
 *
 * Expected output:
 *  Thread spaceID 1 Exiting with value 0
 *  Thread spaceID 2 Exiting with value 0
 *  Machine halting!
 */
#include "syscall.h"

int main(int argc, char *argv[]) {
  if(Exec(argv[1], 0, 0, 0) == 0)
    Exit(1);
  Exit(0);
}
