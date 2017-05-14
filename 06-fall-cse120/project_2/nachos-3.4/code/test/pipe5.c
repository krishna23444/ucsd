/*
 * Pipeline breaks because writer finishes. Reader should finish reading
 * all the data in the buffer and then return on all following writes.
 * 
 * Expected Output:
 *  Thread spaceID 1 Exiting with value 0
 *  Thread spaceID 2 Exiting with value 0
 *  Thread spaceID 3 Exiting with value 0
 */
#include <syscall.h>

int main()
{
  int id;
  
  id = Exec("test/pipe5_produce", 0, 0, 2);
  if (id == 0)
    Exit(1);

  id = Exec("test/pipe5_consume", 0, 0, 4);
  if (id == 0)
    Exit(2);

  Exit(0);
}
