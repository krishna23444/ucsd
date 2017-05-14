#include "mywho.h"

int
idleTimeSortDescending( const void *p1, const void *p2 ) {

    return ((struct utmpInfo *)p2)->idle -
	   ((struct utmpInfo *)p1)->idle;
}
