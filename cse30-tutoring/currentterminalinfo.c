#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "mywho.h"

void
currentTerminalInfo( struct utmpInfo *utmpTable, int entries ) {
    int i;
    char *ttyName;
    char idleStr[6];

    /*
     * Get the name of line to which stdin is associated.
     */
    if ( ( ttyName = ttyname( 0 ) ) == NULL ) 
	if ( ( ttyName = ttyname( 1 ) ) == NULL ) 
	   if ( ( ttyName = ttyname( 2 ) ) == NULL ) {
	        perror( "ttyname( 0, 1, and 2 )" );
	        (void) fprintf( stderr, 
			"Must be attached to terminal for '-c' option\n" );
	        exit( 1 );
            }

    /*
     * Skip over the leading /dev/ in the device name (line).
     */
    ttyName = ttyName + strlen( "/dev/" );

    /*
     * Search for this line in our utmpInfo table
     */
    for ( i = 0; i < entries; ++i ) {
	/*
	 * Find the entry for this device name associated with
	 * this active stdin.
	 */
	if ( utmpTable[i].type == USER_PROCESS &&
	     strcmp( utmpTable[i].line, ttyName ) == 0 ) {

	    calcIdleTime( utmpTable[i].idle, idleStr, sizeof(idleStr) );

/***********
            if ( strftime( timeStr, sizeof( timeStr ), "%b %e %H:%M",
			   localtime( &(utmpTable[i].time) ) ) == 0 ) {
		(void) fprintf( stderr, "Problems with strftime()\n" );
		exit( 1 );
	    }
 ***********/

            (void) printf( "%-8s %c %-12s %s\t(%s)\n", utmpTable[i].user, ' ',
	                   utmpTable[i].line, idleStr, utmpTable[i].host);
	    return;
        }
    }

    (void) printf( "Didn't find you on line %-12s\n", ttyName );

    return; 
}
