/*
 * buildFileInfoTable.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

#include "mydir.h"

int
buildFileInfoTable( const char *filename, struct fileInfo ** const tablePtr ) {

    struct fileInfo *table = NULL;
    struct fileInfo *newtable = NULL;
    struct stat stbuf;
    int entries = 0;
    DIR *dirPtr;
    struct dirent *direntPtr;
    char pathname[MAXNAMLEN];

    if ( lstat( filename, &stbuf ) == -1 ) {
	(void) fprintf( stderr, "lstat -- " );
	perror( filename );
	return 0;
    }

    if ( !S_ISDIR( stbuf.st_mode ) ) {	/* This filename is not a directory */

        table = (struct fileInfo *) malloc( sizeof( struct fileInfo ) );
	if ( table == NULL ) {
	    perror( "Trying to malloc a struct fileInfo (in buildFileInfoTable())" );
	    return 0;
	}
	(void) strncpy( table->name, filename, MAXNAMLEN-1 );
	table->name[MAXNAMLEN-1] = '\0';	/* Just to be safe */
	table->stbuf = stbuf;
	*tablePtr = table;
	return 1;

    } else {				/* This is a directory */

	(void) printf( "%s:\n", filename );

        dirPtr = opendir( filename );
	if ( dirPtr == NULL ) {
	    (void) fprintf( stderr, "opendir -- " );
	    perror( filename );
            return 0;
	}

	while ((direntPtr = readdir( dirPtr )) != NULL ) {

	    (void) snprintf( pathname, MAXNAMLEN, "%s/%s", filename,
	                     direntPtr->d_name );
	    if ( lstat( pathname, &stbuf ) == -1 ) {
		perror( pathname );
	    } else {
		newtable = (struct fileInfo *) realloc( table, 
				     sizeof( struct fileInfo) * (entries + 1) );
                if ( newtable == NULL ) {
		    perror( "Trying to realloc another struct fileInfo (in buildFileInfoTable())" );
		    (void) fprintf( stderr, "Only partial info will be displayed.\n" );
		    *tablePtr = table;
		    (void) closedir( dirPtr );
                    return entries;
                } else {
		    table = newtable;
		    (void) strncpy( table[entries].name, direntPtr->d_name, MAXNAMLEN-1 );
		    table[entries].name[MAXNAMLEN-1] = '\0'; /* Just to be safe */
		    table[entries].stbuf = stbuf;
		    ++entries;
		}
	    }
	} /* while */

	(void) closedir( dirPtr );
    }

    *tablePtr = table;
    return entries;
}
