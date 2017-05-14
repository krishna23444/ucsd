/*
 * void doUptime( struct utmpInfo *utmpTable, int entries );
 */

#include <stdio.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "mywho.h"

void
doUptime( struct utmpInfo *utmpTable, int entries ) {

    int i;
    int uptime, days, hours, mins;
    int numOfUsers = 0;
    char *ptr;
    time_t now = 0;
    time_t boottime = 0;
    char timeStr[24] = {'\0'};

    /*
     * Get and display current time
     */
    if ( utmpTable[0].type == BOOT_TIME ) {
	(void) time( &now );
        if ( strftime( timeStr, sizeof( timeStr ), "%l:%M%p",
                       localtime( &now ) ) == 0 ) {
            (void) fprintf( stderr, "Problems with strftime()\n" );
            exit( 1 );
        }

	/*
	 * strftime() formats the %p as AM and PM instead of am and pm
	 *
	 * This is to get the AM and PM to lower case
	 *
	 * There must be a better way to get am and pm in lower case
	 * Tried setlocale(). Will keep looking ...
	 */
	if ( ( ptr = strchr( timeStr, 'M' ) ) != NULL ) {
	    *(ptr-1) = tolower( *(ptr-1) );
	    *ptr = tolower( *ptr );
	}

        (void) printf( " %s  up ", timeStr );

        if ( utmpTable[0].type == BOOT_TIME ) {
            boottime = utmpTable[0].time;
            uptime = now - boottime;
	    days = uptime / 86400;
	    hours = (uptime - (days * 86400)) / 3600;
	    mins = (uptime - (days * 86400) - (hours * 3600)) / 60;

	    if ( days > 0 )
		(void) printf( "%d day(s), %2d:%02d, ", days, hours, mins );
	    else if ( hours > 0 )
		(void) printf( "%d hrs(s), %d min(s), ", hours, mins );
	    else 
		(void) printf( "%d min(s), ", mins );
        } else
	    (void) printf( "Couldn't get boot time " );

	for ( i = 0; i < entries; ++i )
	    if ( utmpTable[i].type == USER_PROCESS )
		++numOfUsers;
	    
	(void) printf( " %d user%s\n", numOfUsers, numOfUsers > 1 ? "s" : "" );
    }
}

