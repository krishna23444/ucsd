#include <stdio.h>
#include <stdlib.h>

extern void printChar( char );
extern int numOfDigits( long );
void displayX( long size, long xChar, long fillerChar, long borderChar );

int main( int argc, char *argv[] ) {

   long size = strtol( argv[1], NULL, 10 );
   long xChar = strtol( argv[2], NULL, 10);
   long fillChar = strtol( argv[3], NULL, 10);
   long borderChar = strtol( argv[4], NULL, 10 );

   displayX( size, xChar, fillChar, borderChar );
}

#define NL '\n'

void displayX( long size, long xChar, long fillerChar, long borderChar ) {

   int row, 		/* row counter of the main X pattern */
       col, 		/* column counter of the main X pattern */
       borderCnt;	/* counter for the border on either side of the X */

   /*
    * Display the top border.  Border depth is based on the number of
    * digits in the X pattern size.
    */

   for ( row = 0; row < numOfDigits(size); ++row ) {
      for ( col = 0; col < numOfDigits(size) * 2 + size; ++col )
	 printChar( borderChar );
      printChar( NL );
   }

   /*
    * Display the X pattern with border on either side of the X.
    */

   for ( row = 0; row < size; ++row ) {

      for ( borderCnt = 0; borderCnt < numOfDigits(size); ++borderCnt )
         printChar( borderChar );

      /*
       * The if( exp1 || exp2 ) statement below could also be structured as
       *
       *     if ( row == col )
       *        printChar( (char) xChar );
       *     else if ( row == (size - col - 1) )
       *        printChar( (char) xChar );
       *     else
       *        printChar( (char) fillChar );
       *
       * especially if we want the criss-cross pattern of the X to be
       * displayed with different characters.
       */

      for ( col = 0; col < size; ++col ) {
         if ( ( row == col ) || ( row == (size - col - 1) ) )
	    printChar( xChar );
         else
	    printChar( fillerChar );
      }

      for ( borderCnt = 0; borderCnt < numOfDigits(size); ++borderCnt )
         printChar( borderChar );

      printChar( NL );
   }

   /*
    * Display the bottom border.  Border depth is based on the number of
    * digits in the X pattern size.
    */

   for ( row = 0; row < numOfDigits(size); ++row ) {
      for ( col = 0; col < numOfDigits(size) * 2 + size; ++col )
	 printChar( borderChar );
      printChar( NL );
   }
}

