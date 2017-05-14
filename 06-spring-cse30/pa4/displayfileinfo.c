/*
 * displayFileInfo.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <libgen.h>
#include <sys/mkdev.h>

#include "mydir.h"

void
displayFileInfo( struct fileInfo * const table, const int entries, 
		 const int displayMode ) {

    int i;
    char *timeStr;

    if ( displayMode & TFLAG )		/* sort by time */
	if ( displayMode & RFLAG )
	    qsort( table, entries, sizeof(struct fileInfo), mtimeSortAscending);
	else
	    qsort( table, entries, sizeof(struct fileInfo), mtimeSortDescending);
    else if ( displayMode & RFLAG ) 	/* sort by name */
	qsort( table, entries, sizeof(struct fileInfo), nameSortDescending) ;
    else
	qsort( table, entries, sizeof(struct fileInfo), nameSortAscending);

    for ( i = 0; i < entries; ++i ) {

	if ( (basename(table[i].name)[0] == '.') && !(displayMode & AFLAG) )
	    continue;			/* skip hidden files/dirs */
	
        if ( displayMode & LFLAG ) {	/* Long listing */
	    if ( S_ISDIR( table[i].stbuf.st_mode ) )
		(void) printf( "d" );
	    else if ( S_ISLNK( table[i].stbuf.st_mode ) )
		(void) printf( "l" );
	    else if ( S_ISBLK( table[i].stbuf.st_mode ) )
		(void) printf( "b" );
	    else if ( S_ISCHR( table[i].stbuf.st_mode ) )
		(void) printf( "c" );
	    else	/* Regular file */
		(void) printf( "-" );
	
	    displayPermissions( table[i].stbuf.st_mode );

	    (void) printf( "%3lu ", table[i].stbuf.st_nlink );

	    displayOwnerName( table[i].stbuf.st_uid );

	    displayGroupName( table[i].stbuf.st_gid );

	    if ( S_ISCHR( table[i].stbuf.st_mode ) ||
	         S_ISBLK( table[i].stbuf.st_mode ) )
		(void) printf( "%3ld,%3ld ", major(table[i].stbuf.st_rdev),
		                            minor(table[i].stbuf.st_rdev));
	    else
                (void) printf( "%7ld ", table[i].stbuf.st_size );

	    timeStr = ctime( &(table[i].stbuf.st_mtime) );
            *strrchr( timeStr, ':' ) = '\0';
	    (void) printf( "%s " , strchr( timeStr, ' ' ) + 1 );
	}

	(void) printf( "%s\n", table[i].name );
    }
}


