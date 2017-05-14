/*
 * mydir.c
 */

#include "mydir.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

int mtimeOffset = offsetof( struct fileInfo, stbuf.st_mtime );
		  /*
		  offsetof( struct stat, st_mtime );
		  */

int
main( int argc, char *argv[] ) {

    int displayMode = 0;
    int entries = 0;
    struct fileInfo *table = NULL;
    int c;

    while ( (c = getopt( argc, argv, "alrt" )) != EOF )
	switch ( c ) {
	    case 'a':
		displayMode |= AFLAG;
		break;
	    case 'l':
		displayMode |= LFLAG;
		break;
	    case 'r':
		displayMode |= RFLAG;
		break;
	    case 't':
		displayMode |= TFLAG;
		break;
	    case '?':
		(void) fprintf( stderr, "Usage: %s [-alrt] [file ...]\n", argv[0] );
		exit( 1 );
	}

    if ( optind == argc ) {
	if ( (entries = buildFileInfoTable( ".", &table )) != 0 ) {
	    displayFileInfo( table, entries, displayMode );
	    free( table );
	}
    } else {
        for ( ; optind < argc; ++optind )
  	    if ( (entries = buildFileInfoTable( argv[optind], &table )) != 0 ) {
	        displayFileInfo( table, entries, displayMode );
	        free( table );
	    }
    }

    return 0;
}

