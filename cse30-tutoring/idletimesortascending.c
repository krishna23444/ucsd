#include "mywho.h"

int
idleTimeSortAscending( const void *p1, const void *p2 ) {

    return ((struct utmpInfo *)p1)->idle -
	   ((struct utmpInfo *)p2)->idle;
}
