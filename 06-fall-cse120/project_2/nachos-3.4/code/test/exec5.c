/*
 * Exec a simple program.
 *
 * Expected output:
 *  Thread spaceID 1 Exiting with value 0
 *  Thread spaceID 2 Exiting with value 0
 */
#include "syscall.h"

int main() {
  if (Exec("test/exit1", 0, 0, 0) == 0)
    Exit(1);

  Exit(0);
}
