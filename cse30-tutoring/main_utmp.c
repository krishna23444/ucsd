#include <errno.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <utmpx.h>

int
main( void ) {
    FILE *utmpFilePtr;
    struct utmpx utmpxBuf;
    char timeStr[32];

    errno = 0;
    utmpFilePtr = fopen( UTMPX_FILE, "r" );
    if ( utmpFilePtr == NULL ) {
	perror( UTMPX_FILE );
	exit( 1 );
    }

    while ( fread( &utmpxBuf, sizeof( utmpxBuf ), 1, utmpFilePtr ) == 1 ) {

        (void) fprintf( stdout, "%-8s %-12s %7ld\t%hd ", utmpxBuf.ut_name,
		        utmpxBuf.ut_line, utmpxBuf.ut_pid, utmpxBuf.ut_type );

        if ( strftime( timeStr, sizeof( timeStr ), "%b %e %H:%M",
	               localtime( &(utmpxBuf.ut_xtime) ) ) == 0 ) {
	    (void) fprintf( stderr, "Problems with strftime()\n" );
	    exit( 1 );
	}

        (void) fprintf( stdout, "%s\t%hd\t%s\n", timeStr, 
			utmpxBuf.ut_syslen, utmpxBuf.ut_host ); 
    }
    return 0; 
}
