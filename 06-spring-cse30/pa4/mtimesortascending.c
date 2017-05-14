#include "mydir.h"

int
mtimeSortAscending( const void *p1, const void *p2 ) {

    return ((struct fileInfo *)p1)->stbuf.st_mtime -
	   ((struct fileInfo *)p2)->stbuf.st_mtime;
}
