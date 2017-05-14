#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "pa3.h"

void
processIntToken( char *token, int ibase, int obase, unsigned int mode ) {

   long num;
   char *endptr;

   errno = 0;

   num = strtol( token, &endptr, ibase );

   if (errno != 0 ) {
      (void) fprintf( stderr, "\t" );
      perror( token );
      /* (void) fprintf( stderr, "\n" ); */
      return;
   }

   if ( *endptr != '\0' ) {
      (void) fprintf( stderr, "\t%s is not a valid integer in base %d.",
		      token, ibase );
      (void) fprintf( stderr, "\n" );
      errno = -1;
      return;
   }

   if ( mode & E_FLAG ) {
      if ( num < 0 ) {
         (void) printf( "minus " );
         printEnglish( -num );
      } else
         printEnglish( num );
      (void) printf( "\n" );
   }

   if ( mode & O_FLAG ) {
      if ( obase == 2 ) {
         printIntBinary( num ); 
      } else {
        if ( num < 0 ) {
           (void) printf( "-" );
           num = -num;
        }

        if ( obase == 8 ) {
           (void) printf( "0" );
        }

        if ( obase == 16 ) {
           (void) printf( "0x" );
        }

        printBase( num, obase );
      }
      (void) printf( "\n" );
   }
}
