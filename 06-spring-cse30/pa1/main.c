/* 
 * main.c
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "pa1.h"

int
main( int argc, char *argv[] ) {

  int badInput = 0;        /* Assume good input to begin with */
  int badSTL = 0;          /* To indicate if we got a strToLong() error */
  long xSize,              /* Size of the X pattern */
       xChar,              /* The character to display in the X pattern */
       fillerChar,         /* The character to display around the X pattern */
       borderChar;         /* The character to display as the border around
			      the X pattern. */
  char *usage = 
    "\nUsage: %s X_size X_char filler_char border_char\n"
    "    X_size      (must be within the range of [%d - %d])\n"
    "                (must be even)\n"
    "    X_char      (must be an ASCII value within the range "
                     "[%d - %d])\n"
    "    filler_char (must be an ASCII value within the range "
                     "[%d - %d])\n"
    "                (must be different than X_char)\n"
    "    border_char (must be an ASCII value within the range "
                     "[%d - %d])\n"
    "                (must be different than X_char)\n"
    "\n";

  /*
   * Make sure we have enough command line args
   */
  if ( argc != 5 ) {
    (void) fprintf( stderr, usage, argv[0], 
                    X_SIZE_MIN, X_SIZE_MAX,
                    ASCII_MIN, ASCII_MAX,
                    ASCII_MIN, ASCII_MAX,
                    ASCII_MIN, ASCII_MAX );
    return( 1 );  /* or exit( 1 ); either to indicate error/failure mode */
  }

  /*
   * Try to convert the command line args to long ints doing lots of
   * error checking and reporting (some in strToLong()).
   */

  /*
   * First get the X pattern size
   */
  errno = 0;

  xSize = strToLong( argv[1], BASE );

  if ( errno == 0 ) {                /* Okay -- got a good one */

    /*
     * Check the range of the X pattern size
     */
    if ( checkRange( xSize, X_SIZE_MIN, X_SIZE_MAX ) == 0 ) {
      (void) fprintf( stderr,
             "\n\tX_size(%ld) must be within the range of [%d - %d]\n",
             xSize, X_SIZE_MIN, X_SIZE_MAX );
      badInput = 1;
    }

  } else {                          /* Oops -- got a bad one */
    badInput = 1;
    badSTL = 1;
  }

  /*
   * If we have a valid X pattern size, make sure it is even.
   */
  if ( !badSTL ) {
    if ( !isEven( xSize ) ) {
      (void) fprintf( stderr, 
             "\n\tX_size(%ld) must be even\n", xSize );
      badInput = 1;
    }
  }  /* !badSTL */

  /*
   * Now get the X character used in the X pattern
   */
  badSTL = 0;                        /* Reset for checking ASCII chars */
  errno = 0;

  xChar = strToLong( argv[2], BASE );

  if ( errno == 0 ) {                /* Okay -- got a good one */

    /*
     * Check the range of the X char
     */
    if ( checkRange( xChar, ASCII_MIN, ASCII_MAX ) == 0 ) {
      (void) fprintf( stderr,
             "\n\tX_char(%ld) must be an ASCII code in the range [%d - %d]\n",
             xChar, ASCII_MIN, ASCII_MAX );
      badInput = 1;
    }
  } else {                          /* Oops -- got a bad one */
    badInput = 1;
    badSTL = 1;
  }


  /*
   *  Now get the filler char
   */
  badSTL = 0;                        /* Reset for checking ASCII chars */
  errno = 0;

  fillerChar = strToLong( argv[3], BASE );

  if ( errno == 0 ) {                /* Okay -- got a good one */

    /*
     * Check the range of the filler character
     */
    if ( checkRange( fillerChar, ASCII_MIN, ASCII_MAX ) == 0 ) {
      (void) fprintf( stderr,
             "\n\tfiller_char(%ld) must be an ASCII code in "
             "the range [%d - %d]\n", fillerChar, ASCII_MIN, ASCII_MAX );
      badInput = 1;
    }
  } else {                          /* Oops -- got a bad one */
    badInput = 1;
    badSTL = 1;
  }

  /*
   * Check that xChar != fillerChar
   */
  if ( !badSTL && (xChar == fillerChar) ) {
    (void) fprintf( stderr,
           "\n\tX_char(%ld) and filler_char(%ld) "
           "must be different\n", xChar, fillerChar );
    badInput = 1;
  }

  /*
   * Lastly, get the border char
   */
  badSTL = 0;                        /* Reset for checking ASCII chars */
  errno = 0;

  borderChar = strToLong( argv[4], BASE );

  if ( errno == 0 ) {                /* Okay -- got a good one */

    /*
     * Check the range of the border character
     */
    if ( checkRange( borderChar, ASCII_MIN, ASCII_MAX ) == 0 ) {
      (void) fprintf( stderr,
             "\n\tborder_char(%ld) must be an ASCII code in "
             "the range [%d - %d]\n", borderChar, ASCII_MIN, ASCII_MAX );
      badInput = 1;
    }
  } else {                          /* Oops -- got a bad one */
    badInput = 1;
    badSTL = 1;
  }

  /*
   * Check that xChar != borderChar
   */
  if ( !badSTL && (xChar == borderChar) ) {
    (void) fprintf( stderr,
           "\n\tX_char(%ld) and border_char(%ld) "
           "must be different\n", xChar, borderChar );
    badInput = 1;
  }

  /*
   * Got a bad input somewhere above and displayed some error messages.
   * Output another newline to help separate the errors from prompt and exit.
   */
  if ( badInput != 0 ) {
    (void) fprintf( stderr, "\n" );
    exit( 1 );
  }

  /*
   * Finally, display the pattern!
   */
  displayX( xSize, xChar, fillerChar, borderChar );

  return( 0 );
}

