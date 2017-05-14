#include <syscall.h>

int main()
{
  int id;
  
  id = Exec("test/pipe1_produce", 0, 0, 3);
  if (id == 0)
    Exit(1);

  Join(id);

  id = Exec("test/pipe1_consume", 0, 0, 4);
  if (id == 0)
    Exit(2);

  Exit(0);
}
