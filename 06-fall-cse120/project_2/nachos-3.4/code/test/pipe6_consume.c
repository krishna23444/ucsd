#include <syscall.h>

int main()
{
  char buffer[80];

  int numRead = Read(buffer, 3, ConsoleInput);

  if (numRead != 3)
    Exit(1);

  Exit(0);
}
