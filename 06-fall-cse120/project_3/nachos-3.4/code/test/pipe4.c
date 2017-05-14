/*
 * Expected output:
 * Thread spaceID 1 Exiting with value 0
 * Thread spaceID 2 Exiting with value 0
 * Thread spaceID 3 Exiting with value 0
 * Thread spaceID 4 Exiting with value 0
 * Thread spaceID 5 Exiting with value 0
 * Thread spaceID 6 Exiting with value 0
 * 
 */
#include <syscall.h>

int main()
{
  int id;
  
  id = Exec("test/pipe4_produce", 0, 0, 2);
  if (id == 0)
    Exit(1);

  id = Exec("test/pipe4_dual", 0, 0, 6);
  if (id == 0)
    Exit(2);

  id = Exec("test/pipe4_dual", 0, 0, 6);
  if (id == 0)
    Exit(3);

  id = Exec("test/pipe4_dual", 0, 0, 6);
  if (id == 0)
    Exit(4);

  id = Exec("test/pipe4_consume", 0, 0, 4);
  if (id == 0)
    Exit(5);

  Exit(0);
}
