/*
 * displayPermissions.c
 */

#include <sys/stat.h>
#include <stdio.h>

void
displayPermissions( const mode_t mode ) {

    mode_t mask = 0700;	/* mask off 3 bits at a time starting with owner */
    int shift = 6;	/* shift the mask result over this number of bits */
    int i;

    for ( i = 0; i < 3; ++i ) {
        switch ( (mode & mask) >> shift ) {
	    case 0:
	        (void) printf( "---" );
	        break;

	    case 1:
	        (void) printf( "--x" );
	        break;

	    case 2:
	        (void) printf( "-w-" );
	        break;

	    case 3:
	        (void) printf( "-wx" );
	        break;

	    case 4:
	        (void) printf( "r--" );
	        break;

	    case 5:
	        (void) printf( "r-x" );
	        break;

	    case 6:
	        (void) printf( "rw-" );
	        break;

	    case 7:
	        (void) printf( "rwx" );
	        break;
        }
	shift -= 3;	/* reduce the shift count */
	mask >>= 3;	/* shift mask down to the group then the others bits */
    }
    (void) printf( " " );
}

