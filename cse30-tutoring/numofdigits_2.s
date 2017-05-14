/*
 * numOfDigits.s
 *
 * int numOfDigits( long num, int base );
 *
 * Counts the number of digits in num.
 *
 * If num is equal to zero, return 0.
 * Should work for negative numbers.
 *
 * %l0 - Counter for number of digits in num.
 */

MIN_BASE = 2
MAX_BASE = 36

	.global	numOfDigits

	.section ".text"

numOfDigits:
	save	%sp, -96, %sp

	/*
	 * Make sure the base argument value is within range.
	 */
	
	mov	%i1, %o0
	mov	MIN_BASE, %o1
	mov	MAX_BASE, %o2
	call	checkRange
	nop

	cmp	%o0, %g0	! if we got a bad base, return 0
	be	out_of_range
	nop

	clr	%l0

	ba	test
	nop

loop:
	inc	%l0		! count the # of base digits in num

	mov	%i0, %o0
	mov	%i1, %o1
	call	.div
	nop

	mov	%o0, %i0

test:
	cmp	%i0, %g0
	bne	loop
	nop

	mov	%l0, %i0

out_of_range:

	ret
	restore

