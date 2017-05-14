#include <syscall.h>

int main()
{
  char* msg = " \0 \t\n\0 abc";
  int numWritten;

  numWritten = Write(msg, 8, ConsoleOutput);
  if (numWritten != 8)
    Exit(1);

  Exit(0);
}
