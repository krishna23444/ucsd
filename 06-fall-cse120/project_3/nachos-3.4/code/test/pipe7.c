/* 
 * SUPER TEST!
 * Test execing several of the already-implemented tests.
 * 
 * Expected output:
 * Thread spaceID 1 Exiting with value 0
 * Thread spaceID 2 Exiting with value 0
 * Thread spaceID 3 Exiting with value 0
 * Thread spaceID 4 Exiting with value 0
 * Thread spaceID 10 Exiting with value 0
 * Thread spaceID 3 Exiting with value 0
 * Thread spaceID 2 Exiting with value 0
 * Thread spaceID 9 Exiting with value 0
 * Thread spaceID 1 Exiting with value 0
 * Thread spaceID 5 Exiting with value 0
 * Thread spaceID 6 Exiting with value 0
 * Thread spaceID 7 Exiting with value 0
 * Thread spaceID 8 Exiting with value 0
 */
#include <syscall.h>

int main()
{
  Exec("test/pipe4", 0, 0, 0);
  Exec("test/pipe5", 0, 0, 0);
  Exec("test/pipe6", 0, 0, 0);
  
  Exit(0);
}
