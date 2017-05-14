/*
 * nameSortAscending.s

#include <string.h>

#include "mywho.h"

int
nameSortAscending( const void *p1, const void *p2 ) {

    return strcmp( ((struct utmpInfo *)p1)->user,
	           ((struct utmpInfo *)p2)->user );
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

