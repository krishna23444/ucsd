#include <syscall.h>

int main()
{
  char buffer[80];

  int numRead = Read(buffer, 400, ConsoleInput);

  if (numRead != 8)
    Exit(1);

  Exit(0);
}
