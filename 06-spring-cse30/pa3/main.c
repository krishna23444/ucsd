#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "pa3.h"

/* Needed for assembly routines to use fprintf/stderr */
FILE *stdError = stderr;  

int
main( int argc, char *argv[] ) {

   char *endptr;
   char *iBaseStr = NULL;
   char *oBaseStr = "10";	/* Default output base = 10 (decimal) */
   char *inFileStr = NULL;
   char *str = NULL;
   char *token = NULL;
   char *baseToken = NULL;
   char *line = NULL;
   unsigned int mode = 0;
   unsigned int thisMode = 0;
   FILE *ifp = NULL;
   int ibase = 10;
   int obase = 10;
   int error = 0;
   char buf[BUFSIZ];
   char oBaseStrCopy[BUFSIZ];

   int c;

   if ( argc == 1 )
     mode |= H_FLAG;

   while ( (c = getopt( argc, argv, "ebhi:o:f:" )) != EOF ) {
      switch ( c ) {
         case 'e':
            mode |= E_FLAG;	/* Print numbers in english */
            break;
         case 'b':
            mode |= B_FLAG;	/* FP binary output */
            break;
         case 'h':
            mode |= H_FLAG;	/* Help / usage */
            break;
         case 'i':
            mode |= I_FLAG;	/* Input base */
	    iBaseStr = optarg;
            break;
         case 'o':
            mode |= O_FLAG;	/* Output base */
	    oBaseStr = optarg;
            break;
         case 'f':
            mode |= F_FLAG;	/* Input file vs. reading from stdin */
	    inFileStr = optarg;
            break;
         case '?':
	    mode |= H_FLAG;	/* Unknown option. Treat the same as Help. */
	    break;		/* Keep processing command line args. */
      }
   }

   /*
    * Check the arguments to the flags that take arguments.
    * Start with input base.
    */

   if ( (mode & I_FLAG) ) {
      errno = 0;

      ibase = strtol( iBaseStr, &endptr, 10 );

      if ( errno != 0 ) {
	 (void) fprintf( stderr, "\t" );
	 perror( iBaseStr );
	 error = 1;
      }

      else if ( *endptr != '\0' ) {
	 (void) fprintf( stderr, "\t%s (input base) is not an integer.\n",
				 iBaseStr );
	 error = 1;
      }

      else if ( !checkRange( ibase, MIN_BASE, MAX_BASE ) ) {
         (void) fprintf( stderr, "\t%d - invalid input base; "
				 "valid input base range [%d - %d]\n",
			         ibase, MIN_BASE, MAX_BASE );
	 error = 1;
      }
   }

   /*
    * Cycle thru the possible list of comma-separated output bases to
    * make sure they are all valid.
    */

   if ( (mode & O_FLAG) ) {
      /* Make a copy of output bases string because strtok() modifies it. */
      (void) strncpy( oBaseStrCopy, oBaseStr, sizeof(oBaseStrCopy) );
      str = oBaseStrCopy;
      while ( (token = strtok( str, "," )) != NULL ) { 
         errno = 0;

         obase = strtol( token, &endptr, 10 );

         if ( errno != 0 ) {
	    (void) fprintf( stderr, "\t" );
	    perror( token );
	    error = 1;
         }
   
         else if ( *endptr != '\0' ) {
	    (void) fprintf( stderr, "\t%s (output base) is not an integer.\n",
			    token );
	    error = 1;
         }

         else if ( !checkRange( obase, MIN_BASE, MAX_BASE ) ) {
            (void) fprintf( stderr, "\t%d - invalid output base; "
				    "valid output base range [%d - %d]\n",
			            obase, MIN_BASE, MAX_BASE );
	    error = 1;
         }

	 str = NULL;
      }
   }

   /*
    * Check the input file.
    */

   if ( (mode & F_FLAG) ) {

      ifp = fopen( inFileStr, "r" );

      if ( ifp == NULL ) {
	 (void) fprintf( stderr, "\t" );
	 perror( inFileStr );
	 error = 1;
      }

      /*
       * Check that there are no additional arguments after the options
       * if we are reading input from a file vs. the command line.
       */

      if ( optind != argc ) {
         (void) fprintf( stderr, "\tOnly one of -f input_file or numbers on "
	                         "the command line\n" );
	 error = 1;
      }
   }

   /*
    * Help mode or Error. Output a usage and exit.
    */ 

   if ( mode & H_FLAG ) {
      (void) fprintf( stderr, "Usage: %s [-ebh] "
                              "[-i input_base] [-o output_base(s)] "
                              "[-f input_file | numbers]\n"
			      "\t-e -- print number in english\n"
			      "\t-b -- print floating point number (if floating point) in binary\n"
                              "\t-h -- this help/usage message\n",
                              argv[0] );
      exit( 1 );
   }

   /*
    * Bail if we got an error anywhere above.
    */

   if ( error )
      exit( 1 );

   /***** DEAL WITH NESTED STRTOK CALLS ****/

/* Lots of common code here that could/should be redone. */

   if ( mode & F_FLAG ) {

   /*
    * Read input from specified file.
    */

      while ( (line = fgets( buf, sizeof(buf), ifp)) != NULL ) {
	 while ( (token = strtok( line, " \t\n" )) != NULL ) {
            (void) printf( "%s\n", token );

	    thisMode = mode;	/* Kludge! */
            if ( strchr( token, '.' ) == NULL ) { /* This is an int */
	       char *lasts;

	       /*
	        * Have to make a copy of the output bases string because
	        * strtok() modifies it.
	        */
	       (void) strncpy( oBaseStrCopy, oBaseStr, sizeof(oBaseStrCopy) );
	       str = oBaseStrCopy;

	       /*
		* Have to use strtok_r() because we are nested in an
		* outer loop with a strtok().
		*/
               while ( (baseToken = strtok_r( str, ",", &lasts )) != NULL ) { 
                  obase = strtol( baseToken, &endptr, 10 );

	          processIntToken( token, ibase, obase, thisMode );

		  if ( errno != 0 )
		     break;

	          str = NULL;
	          /* Turn off print english mode bit for subsequent bases. */
	          thisMode = thisMode & (~E_FLAG);	/* Kludge! */
	       }
	    } else	/* This is a FP num */
	       processFPToken( token, mode );

	    line = NULL;
	    (void) printf( "\n" );
	 }
      }
   } else {

   /*
    * Read input off the command line.
    */

      for ( ; optind < argc; ++optind ) {
         (void) printf( "%s\n", argv[optind] );

	 thisMode = mode;	/* Kludge! */
         if ( strchr( argv[optind], '.' ) == NULL ) { /* This is an int */
	    /*
	     * Have to make a copy of the output bases string because
	     * strtok() modifies it.
	     */
	    (void) strncpy( oBaseStrCopy, oBaseStr, sizeof(oBaseStrCopy) );
	    str = oBaseStrCopy;
            while ( (token = strtok( str, "," )) != NULL ) { 
               obase = strtol( token, &endptr, 10 );

	       processIntToken( argv[optind], ibase, obase, thisMode );

	       if ( errno != 0 )
		  break;

	       str = NULL;
	       /* Turn off print english mode bit for subsequent bases. */
	       thisMode = thisMode & (~E_FLAG);	/* Kludge! */
	    }
	 } else		/* This is a FP num */
	    processFPToken( argv[optind], mode );

	 (void) printf( "\n" );
      }
   }

   if ( mode & F_FLAG )
      (void) fclose( ifp );
   return(0);
}
