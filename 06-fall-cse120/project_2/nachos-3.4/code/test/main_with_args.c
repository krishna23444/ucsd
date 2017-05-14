#include "syscall.h"

int main(int argc, char *argv[]) {
  if(Exec(argv[1], 0, 0, 0) == 0)
    Exit(1);
  Exit(0);
}
