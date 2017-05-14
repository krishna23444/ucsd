#include <stdio.h>
#include <errno.h>

int
main( void ) {

  if ( freopen( "/dev/tty", "r+", stdin ) != NULL ) {
     system( "./mycrypt - > foo1 < foo.in" );
  } else {
     perror( "freopen()" );
  }

  return 0;
}
