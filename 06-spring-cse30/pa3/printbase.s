/*
 * printBase.s
 * 
 * void printBase( long num, int base );
 *
 */

	.global	printBase

	.section ".rodata"

fmt:	.asciz	"%c"

errfmt:	.asciz	"\tprintBase: Invalid base %d; Valid range [%d - %d].\n\n"

/*
 * char *digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
 */

digits:	
	.asciz	"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"


	.section ".text"

	rt_digit_offset = -4
	MIN_BASE = 2
	MAX_BASE = 36

printBase:
	save	%sp, (-92 + rt_digit_offset) & -8, %sp

/******
  Should do Negative input check and octal/hex check here
 ******/

	/****
	  Call checkRange() !!!
	 ****/

	/* Check the base range */
	cmp	%i1, MIN_BASE
	bl	errorBase
	nop

	cmp	%i1, MAX_BASE
	ble	baseOK
	nop

	/* Print error and return */
errorBase:
	set	stdError, %o0
	ld	[%o0], %o0
	set	errfmt, %o1
	mov	%i1, %o2
	mov	MIN_BASE, %o3
	mov	MAX_BASE, %o4
	call	fprintf
	nop

	ret
	restore

	/* Base is OKAY */
baseOK:
	mov	%i0, %o0
	call	.rem
!	nop
	mov	%i1, %o1

	st	%o0, [%fp + rt_digit_offset]	! rt_digit = num % base

	mov	%i0, %o0
	call	.div
!	nop
	mov	%i1, %o1

	/*
         * result of (num/base) is in %o0 and ready as param for next recursion
	 */

	cmp	%o0, 0
	be	no_recursion
	nop

	call	printBase, 2
	nop

no_recursion:

	set	digits, %l0			! digits
	ld	[%fp + rt_digit_offset], %l1	! rt_digit
	add	%l0, %l1, %o1			! digits + rt_digit
	ldub	[%o1], %o1			! *(words + rt_digit)
	set	fmt, %o0
	call	printf, 2
	nop

	ret
	restore

