#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "pa3.h"

void
processFPToken( char *token, unsigned int mode ) {

   long num;
   float fpnum;
   char *endptr;
   char *dotptr;

   errno = 0;

   fpnum = (float) strtod( token, &endptr );

   if ( errno != 0 ) {
      (void) fprintf( stderr, "\t" );
      perror( token );
      /* (void) fprintf( stderr, "\n" ); */
      return;
   }

   if ( *endptr != '\0' ) {
      (void) fprintf( stderr, "\t%s is not a valid floating point "
                              "number.", token );
      (void) fprintf( stderr, "\n" );
      return;
   }
      
#ifdef NOTDEF
      {
         void *ptr = &fpnum;
         printf("%x\n", *(int *)ptr );
      }
#endif

   if ( mode & E_FLAG ) {
      dotptr = strchr( token, '.' );
      *dotptr = '\0';  /* Slam a '\0' to terminate int part of FP value */

      num = strtol( token, &endptr, 10 );

      /* Spell out in English the part to the left of the decimal point */
      if ( fpnum < 0 ) {
         (void) printf( "minus " );
         printEnglish( -num );
      } else 
         printEnglish( num );

      (void) printf( "dot " );

      /* Spell out the part to the right of the decimal point */

      /*
       * Handle any leading zeros in the fraction part since any leading
       * zeros in an int will be lost when we try to print it in English
       */

      while ( *++dotptr == '0' )
         (void) printf( "zero " );

      if ( *dotptr != '\0' ) {
         num = strtol( dotptr, &endptr, 10 );
         printEnglish( num );
      }

      (void) printf( "\n" );
   }

   if ( mode & B_FLAG ) {
      printFPBinary( fpnum );
      (void) printf( "\n" );
   }
}

