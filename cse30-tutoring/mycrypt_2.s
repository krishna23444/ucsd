/*
 * mycrypt.s
 *
 * void mycrypt( FILE *inFile, unsigned long mask[], int rotateValue );
 *
 * Registers:
 *
 *   %i0 - inFile
 *   %i1 - mask[]
 *   %i2 - rotateValue
 *
 *   %l0 - length -- # of bytes read from fread()
 *   %l1 - i      -- main dual quad byte loop counter
 *   %l2 - ptr    -- pointer into the buffer
 *   %l3 - j      -- residual loop counter
 *   %l4 - (length - i)
 */

	BUFSIZ = 1024		! #define BUFSIZ  1024 (in stdio.h) 

	.global mycrypt

	.section ".data"
fmt:	.asciz	"Reading less than 8 bytes\n"

	.section ".text"

mycrypt:
	/*
	 * Local variable buf needs to be on the Stack. More practice
	 * for the students to access local variables on the Stack
	 * relative to the Frame Pointer. We could have allocated buf
	 * in the Data segment.
	 *
	 * char buf[BUFSIZ];
	 */
	save	%sp, -( 92 + BUFSIZ ) & -8, %sp

	ba	while_test
	nop

while_loop:
	/*
	 * Crypt two 4-byte chucks of buf at a time with mask[0] and mask[1]
	 * using XOR operation on those bytes.
	 *
	 * unsigned long *ptr = (unsigned long *) buf;
	 *
	 * for ( i = 0; (i + 8) <= length; i += 8 ) {
	 *    *ptr = *ptr ^ mask[0];
	 *    ++ptr;
	 *    *ptr = *ptr ^ mask[1];
	 *    ++ptr;
	 *    rotate( mask, rotateValue );
	 * }
	 */

	add	%fp, -BUFSIZ, %l2	! ptr -> buf

	clr	%l1			! i = 0
	ba	for_test_1
	nop

for_loop_1:
	ld	[%l2], %o0		! *ptr
	ld	[%i1], %o1		! mask[0]
	xor	%o0, %o1, %o0		! *ptr ^ mask[0]
	st	%o0, [%l2]		! *ptr = *ptr ^ mask[0];

	add	%l2, 4, %l2		! ++ptr;

	ld	[%l2], %o0		! *ptr
	ld	[%i1+4], %o1		! mask[1]
	xor	%o0, %o1, %o0		! *ptr ^ mask[1]
	st	%o0, [%l2]		! *ptr = *ptr ^ mask[1];

	add	%l2, 4, %l2		! ++ptr;

	mov	%i1, %o0		! mask
	mov	%i2, %o1		! rotateValue
	call	rotate			! rotate( mask, rotateValue );
	nop

	add	%l1, 8, %l1		! i += 8

for_test_1:
	add	%l1, 8, %o0		! (i + 8)
	cmp	%o0, %l0		! (i + 8) <= length
	ble	for_loop_1
	nop

	/*
	 * Now pick up any extra bytes left over (less than 8 of them).
	 */
	sub	%l0, %l1, %l4		! (length - i)

	clr	%l3			! j = 0
	ba	for_test_2
	nop

for_loop_2:
	ldub	[%l2+%l3], %o0		! *((unsigned char *)ptr + j)
	ldub	[%i1+%l3], %o1		! *((unsigned char *)mask + j)
	xor	%o0, %o1, %o0		! *(ptr+j) ^ *(mask+j)

	stb	%o0, [%l2+%l3]		! *(ptr+j) = *(ptr+j) ^ *(mask+j)

	inc	%l3			! ++j

for_test_2:
	cmp	%l3, %l4		! j < (length - i)
	bl	for_loop_2
	nop

	/*
	 * Now that we have all the bytes encrypted in buf, write them all
	 * out to stdout with fwrite().
	 *
	 * (void) fwrite( buf, sizeof( char ), length, stdout );
	 */
	add	%fp, -BUFSIZ, %o0	! buf
	mov	1, %o1			! sizeof( char )
	mov	%l0, %o2		! length
	set	standardOut, %o3
	ld	[%o3], %o3		! stdout
	call	fwrite
	nop
	
	/*
	 * And see if there is another block of bytes to read and encrypt.
	 */

while_test:
/*
 * while((length = fread( buf, sizeof( char ), sizeof( buf ), inFile )) != 0)
 */
	add	%fp, -BUFSIZ, %o0	! buf
	mov	1, %o1			! sizeof( char )
	mov	BUFSIZ, %o2		! sizeof( buf )
	mov	%i0, %o3		! inFile
	call	fread
	nop

	mov	%o0, %l0		! length (# of chars read)

	cmp	%l0, %g0		! while (length != 0)
	bne	while_loop
	nop

	ret
	restore

