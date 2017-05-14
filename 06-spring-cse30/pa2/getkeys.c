/*
 * getKeys.c
 *
 * void getKeys( unsigned long keys[] );
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "mycrypt.h"

void
getKeys( unsigned long keys[] ) {

   char keyString[BUFSIZ] = { '\0' };
   char *keyPtr = NULL;
   unsigned long num = 0;
   int forever = 1;		/* Needed to make lint happy */
   int good1stKey = 0;		/* Set if the 1st key did parse correctly */

   /*
    * Just sit in a forever loop until we get two good keys or the user
    * exits with <ctrl-D> or <ctrl-Z>.
    */
   while ( forever ) {
      good1stKey = 0;

      (void) fprintf( devtty, "Enter the two 32-bit keys "
                      "[octal, decimal, or hex]\n"
                      "\tseparated by a space or tab: " );

      /* If user hits <ctrl-D> (Unix) or <ctrl>-Z (DOS), exit. */
      if ( fgets( keyString, sizeof( keyString ), devtty ) == NULL ) {
         (void) fprintf( devtty, "\n" );
         exit( 3 );
      }

      /*
       * Need to do this to flush the buffered input in /dev/tty from
       * previous fgets(). Otherwise we would get the input echoed back.
       * Would not need to do this if we read from stdin instead.
       */
      (void) fflush( devtty );

      /* Tokenize the input string into 2 keys. */
      keyPtr = strtok( keyString, WHITE_SPACE_CHARS );

      /* Didn't get the 1st key. */
      if ( keyPtr == NULL ) {
         (void) fprintf( devtty, "\n\tNo keys entered. "
                                 "You must enter 2 keys.\n\n" );

         continue;		/* Go around and try again. */
      }

      errno = 0;
      num = strToULong( keyPtr, 0 );

      if ( errno == 0 ) {
	 good1stKey = 1;	/* Got a good 1st key */
         keys[0] = num;
      }

      /* Now get the 2nd key from the same string */
      keyPtr = strtok( NULL, WHITE_SPACE_CHARS );

      /* Didn't get the 2nd key. */
      if ( keyPtr == NULL ) {
         (void) fprintf( devtty, "\n\tOnly 1 key entered. "
                                 "You must enter 2 keys.\n\n" );

         continue;           /* Go around again and try to get 2 keys. */
      }

      errno = 0;
      num = strToULong( keyPtr, 0 );

      if ( errno == 0 && good1stKey ) {    /* Both keys are good. */
         keys[1] = num;
         break;              /* Got 2 good keys so break out of forever loop */
      }

   } /* end while( forever ) */

}
