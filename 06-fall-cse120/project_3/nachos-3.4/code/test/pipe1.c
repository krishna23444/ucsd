/* 
 * Fairly simple pipe with writer and reader
 * 
 * Expected output:
 *  Thread spaceID 1 Exiting with value 0
 *  Thread spaceID 2 Exiting with value 0
 *  a
 *  ab
 *  abcdefg
 *  Thread spaceID 3 Exiting with value 0
 */
#include <syscall.h>

int main()
{
  int id;
  
  id = Exec("test/pipe1_produce", 0, 0, 2);
  if (id == 0)
    Exit(1);

  id = Exec("test/pipe1_consume", 0, 0, 4);
  if (id == 0)
    Exit(2);

  Exit(0);
}
