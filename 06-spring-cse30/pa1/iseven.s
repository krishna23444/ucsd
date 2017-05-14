/*
 * isEven.s
 *
 * int isEven( long value );
 */
	.global	isEven

	.section ".text"

isEven:
	save	%sp, -96, %sp

	/*
	 * If the value in %i0 is even, bit 0 will be 0.
	 *
	 * Just mask off the lower bit, XOR with 0x1, and return the result.
	 *
	 * 0 means false
	 * 1 means true
	 */

	and	%i0, 1, %i0
	xor	%i0, 1, %i0

	ret
	restore

