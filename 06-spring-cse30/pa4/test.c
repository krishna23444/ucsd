#include <stddef.h>

#include "mydir.h"

int main ( void ) {

	printf( "%d\n", offsetof( struct fileInfo, stbuf.st_mtime ) );

	return 0;
}
