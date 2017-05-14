/*
 * timeSortDescending.s

#include "mywho.h"

int
timeSortDescending( const void *p1, const void *p2 ) {

    return ((struct utmpInfo *)p2)->time -
	   ((struct utmpInfo *)p1)->time;
}

 */

	.global	timeSortDescending

	.section ".text"

	/* leaf subroutine */
timeSortDescending:

	/*
	 * Get the time member offset from within a stuct utmpInfo as
	 * calculated in mywho.c using the offsetof() preprocessor macro
	 * and stored in a global variable timeOffset.
	 *
	 * Similar to the mechanism needed to communicate proprocessor
	 * info of stdin, stdout, stderr, to assembly modules.
	 *
	 * Could have made a C function wrapper around the offsetof() macro
	 * and called that from here, but then this could not be a leaf
	 * subroutine.
	 */
	set	timeOffset, %o4
	ld	[%o4], %o4

	ld	[%o0 + %o4], %o2
	ld	[%o1 + %o4], %o3
	sub	%o3, %o2, %o0

	retl
	nop

