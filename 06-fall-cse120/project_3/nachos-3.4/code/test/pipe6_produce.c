#include <syscall.h>

int main()
{
  char* msg = "abdfgdfgcvtrytdjyhgdjhgjuydytsy\n";
  int numWritten;

  numWritten = Write(msg, 400, ConsoleOutput);
  if (numWritten != DEFAULT_BOUNDED_BUFFER_SIZE + 1)
    Exit(1);

  Exit(0);
}
