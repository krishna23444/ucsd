/*
 * strToULong.s
 *
 *  Convert the token to a long unsigned int [lots of error checking] 
 *
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "pa1.h"

long
strToLong( char *str, int base ) {

    long num;
    char *endptr;
    char errorStr[BUFSIZ];

    errno = 0;

    num = strtol( str, &endptr, base );

    if ( errno != 0 ) {

        (void) snprintf( errorStr, BUFSIZ,
                         "\n\tConverting \"%s\" base \"%d\"", str, base);
        perror( errorStr );
        (void) fprintf( stderr, "\n" );
        return 0;               

    } else if (*endptr != '\0') {

        (void) fprintf( stderr, "\n\t\"%s\" is not an integer\n\n", str);
        errno = -1;             
        return 0;
    }

    return num;
}

 *
 */

/*
 *	%l5 = return value from strtoul()
 */

	endptr_offset = 4		! char *endptr; (4 bytes)
	BUFSIZ = 1024
	bufsiz_offset = 1028

	.global	strToULong

	.section ".data"

fmt0:	.asciz	"\n"
fmt1:	.asciz	"\n\tConverting \"%s\" base \"%d\""
fmt2:	.asciz	"\n\t\"%s\" is not an integer\n\n"

	.section ".text"

strToULong:

	save	%sp, -(92 + endptr_offset + BUFSIZ) & -8, %sp
	! allocate char *endptr and char errorStr[BUFSIZ] on stack

	set	errno, %l0
	st	%g0, [%l0]	! errno = 0;

	mov	%i0, %o0
	add	%fp, -endptr_offset, %o1	! &endptr
	mov	%i1, %o2
	call	strtoul, 3	! return value in our %o0
	nop

	mov	%o0, %l5	! save the return value num

	ld	[%l0], %l1	! get the value of errno after strtol()
	cmp	%l1, 0
	be	endptr_check	! errno == 0 so no error here
	nop

	/* errno error message block */

	add	%fp, -bufsiz_offset, %o0
	mov	BUFSIZ, %o1
	set	fmt1, %o2
	mov	%i0, %o3
	mov	%i1, %o4
	call	snprintf
	nop

	add	%fp, -bufsiz_offset, %o0
	call	perror, 1
	nop

        set     standardErr, %o0
        ld      [%o0], %o0
        set     fmt0, %o1
        call    fprintf, 3
        nop

	ba	end_error
!	nop
	ret

endptr_check:
	ld	[%fp + -endptr_offset], %l1	! address of endptr on the stack
	ldub	[%l1], %l2	! *endptr -- get the byte/char it references
	cmp	%l2, 0
	be	return_num	! *endptr == '\0' so no error here
	nop

	/* (*endptr != '\0') error message block */

        set     standardErr, %o0
        ld      [%o0], %o0
        set     fmt2, %o1
	mov	%i0, %o2
        call    fprintf, 3
        nop

	set	errno, %l0
	mov	-1, %l1
	st	%l1, [%l0]	! errno = -1;

	ba	end_error
!	nop
	ret
	
return_num:
	ret
	restore	%g0, %l5, %o0	! return num

end_error:
	restore	%g0, -1, %o0	! return -1

