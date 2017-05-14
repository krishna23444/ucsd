#include <string.h>

#include "mydir.h"

int
nameSortDescending( const void *p1, const void *p2 ) {

    return strcmp( ((struct fileInfo *)p2)->name,
	           ((struct fileInfo *)p1)->name );
}
