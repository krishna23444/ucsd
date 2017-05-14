/*
 * rotate.s
 *
 * void rotate( unsigned int mask[], int rotateCnt );
 *
 * This could probably be worked up some more to be cleaner.
 */

	MASK_LOWER_6 = 0x3F
	MASK_HIGH_BIT = 0x80000000
	MASK_LOW_BIT = 0x1

	.global	rotate

	.section ".text"

rotate:
	save	%sp, -96, %sp

	ld	[%i0], %l0		! mask[0]
	ld	[%i0+4], %l1		! mask[1]

	set	MASK_HIGH_BIT, %l2

	tst	%i1
	bneg	rotate_right
	nop

	and	MASK_LOWER_6, %i1, %i1

	ba	test1
	nop

loop1:
	and	%l2, %l0, %l3		! rotate in this bit at bit 0 %l1
	srl	%l3, 31, %l3

	and	%l2, %l1, %l4		! shift this bit into bit 0 %l0
	srl	%l4, 31, %l4

	sll	%l0, 1, %l0
	sll	%l1, 1, %l1
	or	%l0, %l4, %l0
	or	%l1, %l3, %l1

	dec	%i1

test1:
	cmp	%i1, 0
	bne	loop1
	nop

	ba	end
	nop

rotate_right:
	neg	%i1
	and	MASK_LOWER_6, %i1, %i1

	ba	test2
	nop

loop2:
	and	MASK_LOW_BIT, %l0, %l4	! shift this bit into bit 31 %l1
	sll	%l4, 31, %l4

	and	MASK_LOW_BIT, %l1, %l3	! rotate in this bit at bit 31 %l0
	sll	%l3, 31, %l3

	srl	%l0, 1, %l0
	srl	%l1, 1, %l1
	or	%l0, %l3, %l0
	or	%l1, %l4, %l1

	dec	%i1

test2:
	cmp	%i1, 0
	bne	loop2
	nop

end:
	st	%l0, [%i0]		! mask[0]
	st	%l1, [%i0+4]		! mask[1]

	ret
	restore

