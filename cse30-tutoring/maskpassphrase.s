/*
 * maskPassPhrase.s
 *
 * void maskPassPhrase( unsigned long keys[], char passPhrase[],
 *                      unsigned long mask[] );
 *
 * In C, we have to trick the compiler into thinking the second argument
 * is really a pointer to an unsigned long and grab 4 bytes to XOR.
 *
 * In assembly, we just grab as many bytes we want.
 *
 * Of course, we could do this byte-by-byte if we wanted to,
 * but this is more fun!
 *
 * mask[0] = keys[0] ^ *(unsigned long *)passPhrase;
 * mask[1] = keys[1] ^ *(unsigned long *)(passPhrase + 4);
 *
 * Could have done this in a loop, but this is more efficient
 * -- loop unrolling!
 *
 * Registers:
 *   %i0 - keys[]	(pointer to 1st word [0] of this array)
 *   %i0+4		(pointer to 2nd word [1] of this array)
 *
 *   %i1 - passPhrase[]	(pointer to 1st char [0] of this array)
 *   %i1+4		(pointer to 5th char [4] of this array)
 *
 *   %i2 - mask[]	(pointer to 1st word [0] of this array)
 *   %i2+4		(pointer to 2nd word [1] of this array)
 *
 *   %l0 - value of keys[0] or keys[1]
 *   %l1 - value of passPhrase[0-3] or passPhrase[4-7]
 *   %l2 - result of (keys[0] ^ passPhrase[0-3]) or (keys[1] ^ passPhrase[4-7])
 */

	.global	maskPassPhrase

	.section ".text"

maskPassPhrase:
	save	%sp, -96, %sp

	/*
	 * Perform the XOR mask on the first 4 bytes of the passPhrase
	 * and the first key bit pattern and save it in mask[0].
	 *
	 * mask[0] = keys[0] ^ *(unsigned long *)passPhrase;
	 */

	ld	[%i0], %l0		! keys[0]
	ld	[%i1], %l1		! passPhrase[0-3] -- 1st 4 bytes
	xor	%l0, %l1, %l2		! XOR of keys[0] and 1st 4 bytes
					!   of the passPhrase string
	
	st	%l2, [%i2]		! store XOR result into mask[0]

	/*
	 * Perform the XOR mask on the second 4 bytes of the passPhrase
	 * and the second key bit pattern and save it in mask[1].
	 *
	 * mask[1] = keys[1] ^ *(unsigned long *)(passPhrase + 4);
	 */


	ld	[%i0+4], %l0		! keys[1] -- 4 byte word
	ld	[%i1+4], %l1		! passPhrase[4-7] -- 2nd 4 bytes
	xor	%l0, %l1, %l2		! XOR of keys[1] and 2nd 4 bytes
					!   of the passPhrase string
	
	st	%l2, [%i2+4]		! store XOR result into mask[1]

	ret
	restore

