#include <syscall.h>

int main()
{
  char* msg = "abcdefg\n";
  int numWritten;

  numWritten = Write(msg, 1, ConsoleOutput);
  if (numWritten != 1)
    Exit(1);
  numWritten = Write("\n", 1, ConsoleOutput);
  if (numWritten != 1)
    Exit(2);
  numWritten = Write(msg, 2, ConsoleOutput);
  if (numWritten != 2)
    Exit(3);
  numWritten = Write("\n", 1, ConsoleOutput);
    if (numWritten != 1)
    Exit(4);
  numWritten = Write(msg, 9, ConsoleOutput);
  if (numWritten != 9)
    Exit(5);

  Exit(0);
}
