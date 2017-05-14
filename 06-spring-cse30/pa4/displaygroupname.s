/*
 * displayGroupName.s

#include <grp.h>

void
displayGroupName( const gid_t gid ) {

    struct group *gr;

    if ((gr = getgrgid( gid )) != NULL )
	(void) printf( "%-8s ", gr->gr_name );
    else
	(void) printf( "%-8ld ", gid );
}
 */

	.global	displayGroupName

	.section ".rodata"

fmt1:	.asciz "%-8s "
fmt2:	.asciz "%-8ld "

	.section ".text"

displayGroupName:
	save	%sp, -96, %sp

	mov	%i0, %o0
	call	getgrgid, 1
	nop

	cmp	%o0, 0
	be	else
	nop

	ld	[%o0], %o1
	set	fmt1, %o0
	call	printf, 2
	nop

	ret
	restore

else:
	set	fmt2, %o0
	mov	%i0, %o1
	call	printf,2
	nop

	ret
	restore

