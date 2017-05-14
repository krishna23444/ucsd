#include "mywho.h"

int
timeSortAscending( const void *p1, const void *p2 ) {

    return ((struct utmpInfo *)p1)->time -
	   ((struct utmpInfo *)p2)->time;
}
