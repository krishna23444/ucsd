/*
 * strToLong.c
 *
 * long strToLong( char *str, int base );
 *
 * Convert the argument string to a long int as specified base.
 *
 * Check for errors:
 *    errno
 *    *endptr != '\0'
 *       Set errno to non-zero value to indicate an error
 *
 * Return the converted value.
 *
 * Calling routine needs to check the value of errno on return from
 * strToLong() to determine if returned value is valid.
 *
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "pa1.h"

long
strToLong( char *str, int base ) {

    long num;
    char *endptr;
    char errorStr[BUFSIZ];

    errno = 0;

    /* Try to convert to a long int */
    num = strtol( str, &endptr, base );

    /* Check errno first */
    if ( errno != 0 ) {

        (void) snprintf( errorStr, BUFSIZ,
                         "\n\tConverting \"%s\" base \"%d\"", str, base);
        perror( errorStr );
        return 0;		/* Errno already set */

    /* Then check *endptr */
    } else if (*endptr != '\0') {

        (void) fprintf( stderr, "\n\t\"%s\" is not an integer\n", str);
        errno = -1;		/* To indicate error */
        return 0;
    }

    return num;
}
