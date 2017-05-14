/*
 * Exec halt program
 *
 * Expected output:
 *  Thread spaceID 1 Exiting with value 0
 *  Machine halting!
 */
#include "syscall.h"

int main() {
  if (Exec("test/halt", 0, 0, 0) == 0)
    Exit(1);

  Exit(0);
}
