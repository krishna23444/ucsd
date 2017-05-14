#include <syscall.h>

const int NUM_TO_READ = 14;

int main()
{
  char buffer[80];

  int numRead = Read(buffer, NUM_TO_READ, ConsoleInput);
  
  if (numRead != NUM_TO_READ)
    Exit(1);

  int numWritten = Write(buffer, NUM_TO_READ, ConsoleOutput);
  if (numWritten != NUM_TO_READ)
    Exit(2);

  Exit(0);
}
