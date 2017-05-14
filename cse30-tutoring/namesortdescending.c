#include <string.h>

#include "mywho.h"

int
nameSortDescending( const void *p1, const void *p2 ) {

    return strcmp( ((struct utmpInfo *)p2)->user,
	           ((struct utmpInfo *)p1)->user );
}
