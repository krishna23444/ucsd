/*
 * Filename: pa0.h
 * Author: Joe Jane Student
 * Userid: cs30xzz
 * Description: Function prototypes and defines for PA0
 * Date: Jan. 10, 2001
 * Sources of Help: List all the people, books, websites, etc. that you
 *                  used to help you write the code in this source file.
 */

/* #defines used in PA0 */

/* User-defined types used in PA0 */

struct birthDateInfo {
   char *month;
   unsigned int day;
   unsigned int year;
};

/* Function prototypes for the C and assembly routines used in PA0 */

void printHello( char *string );

void printBirthDateInfo( struct birthDateInfo *bDayInfo );

int sum3( int a, int b, int c );

