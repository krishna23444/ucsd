/*
 * checkRange.s
 *
 * int checkRange( long value, long minRange, long maxRange );
 *
 * Returns 0 for error
 *         1 for OKAY
 *
 * if ( (value < minRange) || (value > maxRange) )
 *     return 0;
 * else
 *     return 1;
 *
 *	%i0 -- value to check
 *	%i1 -- minRange
 *	%i2 -- maxRange
 */

	.global	checkRange

	.section ".text"

checkRange:
	save	%sp, -96, %sp

	cmp	%i0, %i1	! value < minRange
	bl	error
	nop

	cmp	%i0, %i2	! value > maxRange
	bg	error
	nop

	mov	1, %i0		! return 1
	ret
	restore

error:
	clr	%i0		! return 0
	ret
	restore


