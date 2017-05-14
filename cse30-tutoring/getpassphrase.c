/*
 * getPassPhrase.c
 *
 * void getPassPhrase( char passPhrase[] );
 *           or
 * void getPassPhrase( char *passPhrase );
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mycrypt.h"

void
getPassPhrase( char passPhrase[] ) {

   char buf[BUFSIZ] = { '\0' };
   int slen = 0;

   do {
      (void) fprintf( devtty, "Enter the passphrase [at least %d chars]: ",
                      PASS_PHRASE_SIZE );

      /* If user hits <ctrl-D> (Unix) or <ctrl>-Z (DOS), exit. */
      if ( fgets( buf, sizeof( buf ), devtty ) == NULL ) {
         (void) fprintf( devtty, "\n" );
         exit( 3 );
      }

      /*
       * Need to do this to flush the buffered input in /dev/tty from
       * previous fgets(). Otherwise we would get the input echoed back.
       * Would not need to do this if we read from stdin instead.
       */
      (void) fflush( devtty );

      /* Make sure we got at least 8 chars. not including the newline char. */
      if ( (slen = strlen( buf )) < PASS_PHRASE_SIZE + 1 ) 
         (void) fprintf( devtty,"\n\tPassphrase must be at least %d chars long;"
                                " Try again.\n\n", PASS_PHRASE_SIZE );

   } while ( slen < PASS_PHRASE_SIZE + 1 );

   /*
    * Got a good pass phrase of at least 8 chars.
    * Copy the first 8 chars to our passPhrase memory location.
    */
   (void) strncpy( passPhrase, buf, PASS_PHRASE_SIZE );

}
