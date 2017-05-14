/*
 * Tries to run something that does not end with a null character
 * 
 * Expected output:
 *  Thread spaceID 1 Exiting with value 1
 */
#include "syscall.h"

char global[300];

int main() {
  int i =0;
  while (i <= 256) {
    global[i++] = 't';
    global[i++] = 'e';
    global[i++] = 's';
    global[i++] = 't';
    global[i++] = '/';
    global[i++] = '.';
    global[i++] = '.';
    global[i++] = '/';
  }
  global[i++] = 't';
  global[i++] = 'e';
  global[i++] = 's';
  global[i++] = 't';
  global[i++] = '/';
  global[i++] = 'h';
  global[i++] = 'a';
  global[i++] = 'l';
  global[i++] = 't';

  if (Exec(global, 0, 0, 0) == 0)
    Exit(1);
  Exit(0);
}
