
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mycrypt.h"

/*
 * Needed for getting stderr from <stdio.h> accessible in Assembly
 * (strToULong.s)
 */
FILE *standardErr = stderr;

/*
 * Needed for getting stdout from <stdio.h> accessible in Assembly
 * (mycrypt.s)
 */
FILE *standardOut = stdout;

/*
 * This is global so all C files can access /dev/tty without each having
 * to open/close it individually. Just makes things a little easier
 * and lets us talk about extern declaration in mycrypt.h vs. the real global
 * variable definition here.
 */
FILE *devtty = NULL;

int
main( int argc, char *argv[] ) {

   FILE *inFile = stdin;
   char passPhrase[PASS_PHRASE_SIZE] = { '\0' };
   unsigned long keys[2] = { 0, 0 };
   unsigned long mask[2] = { 0, 0 };
   int rotateValue = 0;

   /*
    * Only accept "program_name filename" or "program_name -"
    */
   if ( argc != 2 ) {
      (void) fprintf( stderr, "\n\tUsage: %s filename | -\n\n", argv[0] );
      exit( 1 );
   }

   /*
    * Got the right number of command line arguments.
    * Now set our input to be from stdin or the named file.
    *
    * Could have designed this such that if there was no second argument
    * (filename), then assume input from stdin. Lots of Unix utilities
    * use '-' as indicating the input is coming from stdin.
    */
   if ( strcmp( argv[1], "-" ) == 0 )
      inFile = stdin;	       /* Redundant, but helps make the code readable */
   else {
      inFile = fopen( argv[1], "rb" );
      if ( inFile == NULL ) {
         perror( argv[1] );
         exit( 2 );
      }
   }

   /*
    * Open the controlling terminal so we can read/write to/from it even
    * with stdin/stdout redirected.
    */
   devtty = fopen( "/dev/tty", "r+" );
   if ( devtty == NULL ) {
      perror( "/dev/tty" );
      exit( 2 );
   }

   /*
    * Get the pass phrase from the user.
    * Let getPassPhrase() exit the program if user wants to quit.
    */
   getPassPhrase( passPhrase );

   /*
    * Now get the keys to mask the pass phrase.
    *
    * Just sit in a forever loop until we get two good keys or the user
    * exits with <ctrl-D> or <ctrl-Z>.
    */
   getKeys( keys );

   /*
    * And finally get the rotate value [-63 <-> +63]
    */
   rotateValue = getRotateValue();

   /* Now XOR the pass phase with the keys */
   maskPassPhrase( keys, passPhrase, mask );

   /*
    * And then actually perform the encrypting/decrypting of the specified
    * input using the mask we just built and the rotation key.
    *
    * Symmetric encrypt/decrypt. The encrypted file run through this same
    * process with the same mask and rotation key used to encrypt it will
    * be decrypted into the original file.
    */
   mycrypt( inFile, mask, rotateValue );

   /*
    * Clean up after ourselves; good programming hygiene.
    */

   (void) fclose( inFile );
   (void) fclose( devtty );

   return 0;
}

