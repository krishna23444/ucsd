#include <stdio.h>

#include "pa3.h"

FILE *stdError = stderr;

int
main( int argc, char *argv[] ) {

   int i;

   (void) printf( "printBaseTest\n\n" );

   for ( i = -1; i <= 38; ++i ) {
      printBase( 35, i );
      (void) printf( "\n" );
   }

   return 0;
}
