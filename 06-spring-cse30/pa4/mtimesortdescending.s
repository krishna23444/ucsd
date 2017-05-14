/*
 * mtimeSortDescending.s

#include "mydir.h"

int
mtimeSortDescending( const void *p1, const void *p2 ) {

    return ((struct fileInfo *)p2)->stbuf.st_mtime -
	   ((struct fileInfo *)p1)->stbuf.st_mtime;
}

 */

!	mtime_offset = 512 + 64
!	mtime_offset = mtimeOffset

	.global	mtimeSortDescending

	.section ".text"

	/* leaf subroutine */
mtimeSortDescending:

	set	mtimeOffset, %o5
	ld	[%o5], %o4			! mtime offset
	ld	[%o0 + %o4], %o2
	ld	[%o1 + %o4], %o3
	sub	%o3, %o2, %o0

	retl
	nop

