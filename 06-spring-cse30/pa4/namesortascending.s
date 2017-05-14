/*
 * nameSortAscending.s

#include <string.h>

#include "mydir.h"

int
nameSortAscending( const void *p1, const void *p2 ) {

    return strcmp( ((struct fileInfo *)p1)->name,
	           ((struct fileInfo *)p2)->name );
}

 */

	.global	nameSortAscending

	.section ".text"

nameSortAscending:

	save	%sp, -96, %sp

	mov	%i0, %o0
	mov	%i1, %o1
	call	strcmp, 2
	nop

	mov	%o0, %i0

	ret
	restore

