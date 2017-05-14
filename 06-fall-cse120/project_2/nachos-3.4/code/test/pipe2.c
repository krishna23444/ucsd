/*
 * Parent should break pending pipelines if it finishes.
 *
 * Expected output:
 *  Thread spaceID 1 Exiting with value 0
 *  Thread spaceID 2 Exiting with value 1
 */
#include <syscall.h>

int main()
{
  int id;
  
  id = Exec("test/pipe1_produce", 0, 0, 2);
  if (id == 0)
    Exit(1);

  Exit(0);
}
