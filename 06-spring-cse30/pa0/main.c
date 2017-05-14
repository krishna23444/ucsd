/*
 * Filename: main.c
 * Author: Joe Jane Student
 * Userid: cs30xzz
 * Description: Example C program for pa0.  It calls a couple of functions
 *		written in assembly and C as examples of function calls and
 *		parameter passing in C and SPARC assembly (also known as
 *		the language's and architecture's calling conventions).
 * Date: Jan. 10, 2001
 * Sources of Help: List all the people, books, websites, etc. that you
 *                  used to help you write the code in this source file.
 */ 

/*
 * Header files included here.
 * Std C Lib header files first, then local headers.
 */
#include <stdio.h>

#include "pa0.h"

/*
 * Function name: main()
 * Function prototype: int main( int argc, char *argv[] );
 *				or
 *                     int main( void ); if no command line processing.
 * Description: C main driver which calls C and SPARC assembly routines to
 *		print a greeting with the student's name and birthday and
 *		summing three ints.
 * Parameters: No command line args expected.
 * Side Effects: Outputs greeting with student's name, birthday, and the
 *		 sum of three ints.
 * Error Conditions: None.
 * Return Value: 0 indicating successful execution.
 */

int
main( void ) {

   char *name = "Joe Jane Student";	/* Your name to print */
   struct birthDateInfo bDayInfo;	/* Your birthday info */
   int x1 = 1,				/* Some values to sum */
       x2 = 2,
       x3 = 3;

   bDayInfo.day = 17;
   bDayInfo.year = 1978;
   bDayInfo.month = "December";

   /*
    * Print the greeting with your name
    */
   printHello( name );	

   /*
    * Print your birthday
    * printBirthDateInfo() takes a pointer to the birthDateInfo struct
    */
   printBirthDateInfo( &bDayInfo );	

   /*
    * Now sum the 3 local int variables and print the result
    */
   (void) printf( "The sum of %d, %d, and %d = %d\n", x1, x2, x3,
		  sum3( x1, x2, x3 ) );

   return( 0 );
}

