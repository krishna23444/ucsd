#include <syscall.h>

const int NUM_TO_READ = 33;

int main()
{
  char buf[100];
  
  int numRead = Read(buf, NUM_TO_READ, ConsoleInput);

  if (numRead != NUM_TO_READ)
    Exit(1);
  
  int numWritten = Write(buf, NUM_TO_READ, ConsoleOutput);
  
  if (numWritten != NUM_TO_READ)
    Exit(2);

  Exit(0);
}
