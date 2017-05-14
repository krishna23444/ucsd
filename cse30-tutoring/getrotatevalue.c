/*
 * getRotateValue.c
 *
 * int getRotateValue( void );
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "mycrypt.h"

int
getRotateValue( void ) {

   char buf[BUFSIZ] = { '\0' };
   char *newlinePtr = NULL;
   int rotateValue = 0;
   int forever = 1;			/* Needed to make lint happy */

   /*
    * Sit in a forever loop until we get a good rotation key or user exits
    */
   while ( forever ) {
     (void) fprintf( devtty, "Enter the rotation key [%d <-> +%d]: ",
                             MIN_ROTATE, MAX_ROTATE );

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

      /*
       * Get rid of the newline char.
       */
      if ( ( newlinePtr = strchr( buf, '\n' ) ) != NULL )
         *newlinePtr = '\0';

      /* 
       * Check to make sure they didn't just hit Enter without typing in
       * a rotation value.
       */
      if ( strlen( buf ) == 0 ) {
	 (void) fprintf( devtty, 
			 "\n\tNo rotation key entered; Try again.\n\n" );
	 continue;
      }

      errno = 0;
      rotateValue = strToULong( buf, 0 );

      if ( errno == 0 ) {       /* Check if it is in the range. */
         if ( checkRange( rotateValue, MIN_ROTATE, MAX_ROTATE ) != 0 ) {
            break;		/* Got a good rotate value! */
         }
         (void) fprintf( devtty,
                "\n\tRotation key must be within the range of [%d <-> %d]\n\n",
                MIN_ROTATE, MAX_ROTATE );
      }  /* if errno == 0 */

   } /* end while( forever ) */

   return rotateValue;
}
