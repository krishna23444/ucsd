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

int main() {
  //ProgA = this
  int progB = Exec("test/jointest4_progB", 0, 0, 1);
  int progC = Exec("test/exit1", 0, 0, 1);
  if (progC == 0 || progB == 0)
    Exit(101);
  //Tries to join progC
  if (Join(progC) == -65535 )
    Exit(102);
  //Tries to join progB
  if (Join(progB) == -65535)
    Exit(103);
  Exit(100);
}
