#include <stdio.h>
#include <stdlib.h>

extern void printChar( char );
extern int numOfDigits( long );

int main( int argc, char *argv[] ) {

   int row, col, i;

   long size = strtol( argv[1], NULL, 10 );
   long xChar = strtol( argv[2], NULL, 10);
   long fillChar = strtol( argv[3], NULL, 10);
   long borderChar = strtol( argv[4], NULL, 10 );

printf( "%d %d %d %d\n", size, xChar, fillChar, borderChar );

   for ( row = 0; row < numOfDigits(size); ++row ) {
      for ( col = 0; col < numOfDigits(size) * 2 + size; ++col )
	 printChar( (char) borderChar );
      printChar( '\n' );
   }

   for ( row = 0; row < size; ++row ) {

      for ( i = 0; i < numOfDigits(size); ++i )
         printChar( (char) borderChar );

      for ( col = 0; col < size; ++col ) {

         if ( row == col )
	    printChar( (char) xChar );
         else if ( row == (size - col - 1) )
	    printChar( (char) xChar );
         else
	    printChar( (char) fillChar );

      }

      for ( i = 0; i < numOfDigits(size); ++i )
         printChar( (char) borderChar );

      printChar( '\n' );
   }

   for ( row = 0; row < numOfDigits(size); ++row ) {
      for ( col = 0; col < numOfDigits(size) * 2 + size; ++col )
	 printChar( (char) borderChar );
      printChar( '\n' );
   }
}

