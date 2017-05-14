/*
 * displayOwnerName.c
 */

#include <pwd.h>

void
displayOwnerName( const uid_t uid ) {

    struct passwd *pw;

    if ((pw = getpwuid( uid )) != NULL )
	(void) printf( "%-8s ", pw->pw_name );
    else
	(void) printf( "%-8ld ", uid );
}

