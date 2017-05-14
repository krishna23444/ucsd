/*
 * Tries to join a thread waiting to be joined by another
 * 
 * ProgA creates ProgB and ProgC
 * 
 * ProgA calls join on ProgC
 * ProgA yields
 * ProgB runs, calls join on ProgC, join fails because another process is
 *       is waiting on it, exit(202)
 * ProgC runs, exit(0)
 * ProgA runs, exit(100)
 *
 * Expected output:
 *  Thread spaceID 2 Exiting with value 202
 *  Thread spaceID 3 Exiting with value 0
 *  Thread spaceID 1 Exiting with value 100
 */
#include "syscall.h"

int main(int argc, char *argv[]) {
  if (Join(3) == -65535 )
    Exit(202);

  Exit(200);
}
