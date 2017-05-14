#include <syscall.h>

const int NUM_TO_WRITE = 33;

int main()
{
  char* buf = "hello there buddy how are you doing?\n";

  int numWritten = Write(buf, NUM_TO_WRITE, ConsoleOutput);

  if (numWritten != NUM_TO_WRITE)
    Exit(1);

  Exit(0);
}
