/*
 * void calcIdleTime( time_t idleTime, char * idleString, int size );
 */

	now_Offset = 4

	.global	calcIdleTime

	.section ".data"

errStr:	.asciz	"calcIdleTime calling time()"

blank:	.asciz	"     "

dot:	.asciz	"  .  "

old:	.asciz	" old "

fmt:	.asciz	"%2d:%02d"

	.section ".text"

calcIdleTime:
	save	%sp, -(92 + now_Offset), %sp	! 4 bytes for time_t now

	mov	%i0, %l0
        ba	checkIdleTime
	nop

/* This is OLD - need to remove */
	st	%g0, [%fp-now_Offset]		! time_t now = 0;

	/*     OLD
	 *     if ( time( &now ) == (time_t) -1 ) {
	 *         perror( "calcIdleTime calling time()" );
	 *         (void) strncpy( idleString, "     ", size );
	 *         return;
	 *     }
	 */

	add	%fp, -now_Offset, %o0
	call	time
	nop

	cmp	%o0, -1
	bne	okay
	nop

	set	errStr, %o0
	call	perror
	nop

	mov	%i1, %o0
	set	blank, %o1
	mov	%i2, %o2
	call	strncpy
	nop

	ret
	restore

okay:
	/*
	 * idleTime = now - lastAccess;
	 */

	ld	[%fp-now_Offset], %l0
	sub	%l0, %i0, %l0			! %l0 hold idleTime


	!
	! if ( idleTime < 60 ) /* 1 minute */
	!     (void) strncpy( idleString, "  .  ", size );
	! else if ( idleTime < (24 * 60 * 60) ) /* 1 day */
	!     (void) snprintf( idleString, size, "%2d:%02d",
	!                      (int) (idleTime / (60 * 60)),
	!                      (int) ((idleTime % (60 * 60)) / 60) );
	! else
	!     (void) strncpy( idleString, " old ", size );
	!

checkIdleTime:
	cmp	%l0, 60
	bge	check_if_old
	nop

	mov	%i1, %o0
	set	dot, %o1
	mov	%i2, %o2
	call	strncpy
	nop

	ret
	restore

check_if_old:
	set	86400, %l1
	cmp	%l0, %l1		! 24*60*60
	bge	print_old
	nop

	mov	%l0, %o0
	mov	3600, %o1
	call	.div
	nop

	mov	%o0, %l1		! idleTime / (60 * 60)

	mov	%l0, %o0
	mov	3600, %o1
	call	.rem
	nop

	mov	60, %o1
	call	.div
	nop

	mov	%o0, %o4		! (idleTime % (60 * 60)) / 60)

	mov	%i1, %o0
	mov	%i2, %o1
	set	fmt, %o2
	mov	%l1, %o3
	call	snprintf
	nop

	ret
	restore

print_old:
	mov	%i1, %o0
	set	old, %o1
	mov	%i2, %o2
	call	strncpy
	nop

	ret
	restore

