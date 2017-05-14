/*
 * displayX.s
 *
 * void displayX( long size, long xChar, long fillerChar, long borderChar );
 *
 * %i0 - size
 * %i1 - xChar
 * %i2 - fillerChar
 * %i3 - borderChar
 *
 * %l0 - row counter (also used for displaying top/bottom borders)
 * %l1 - col counter (also used for displaying top/bottom borders)
 * %l2 - (size - col - 1)
 * %l3 - numOfDigits(size, 10)
 * %l4 - numOfDigits(size, 10) * 2 + size -- length of top/bottom borders
 * %l5 - counter for displaying the border char on each row
 */

	NL = '\n'
	BASE = 10

	.global	displayX

	.section ".text"

displayX:
	save	%sp, -96, %sp

	/*
	 * First display the top border
	 */

	mov	%i0, %o0
	mov	BASE, %o1
	call	numOfDigits
	nop

	mov	%o0, %l3	! numOfDigits(size, 10)

	add	%l3, %l3, %l4	! numOfDigits(size, 10) * 2
	add	%i0, %l4, %l4	! numOfDigits(size, 10) * 2 + size


	clr	%l0		! top border row counter
	ba	top_border_outer_test
	nop

top_border_outer_loop:

	clr	%l1		! top border col counter
	ba	top_border_inner_test
	nop

top_border_inner_loop:
	mov	%i3, %o0
	call	printChar
	nop

	inc	%l1

top_border_inner_test:
	cmp	%l1, %l4
	bl	top_border_inner_loop
	nop

	mov	NL, %o0
	call	printChar
	nop

	inc	%l0

top_border_outer_test:
	cmp	%l0, %l3
	bl	top_border_outer_loop
	nop


	clr	%l0		! row = 0

	ba	outer_loop_test
	nop

outer_loop:

	/*
	 * Display the border char numOfDigits(size, 10) times.
	 */

	clr	%l5		! border char counter
	ba	row_border_test1
	nop

row_border_loop1:
	mov	%i3, %o0
	call	printChar
	nop

	inc	%l5

row_border_test1:
	cmp	%l5, %l3
	bl	row_border_loop1
	nop

	clr	%l1		! col = 0

	ba	inner_loop_test
	nop

inner_loop:

	cmp	%l0, %l1	! if ( row == col )
	bne	else_if
	nop

	mov	%i1, %o0
	call	printChar	! printChar( xChar );
	nop

	ba	end_if
	nop

else_if:

	sub	%i0, %l1, %l2	! (size - col - 1)
	dec	%l2

	cmp	%l0, %l2	! else if ( i == (size - j - 1) )
	bne	else
	nop

	mov	%i1, %o0	! printChar( xChar );
	call	printChar
	nop

	ba	end_if
	nop

else:				! else

	mov	%i2, %o0	! printChar( fillerChar );
	call	printChar
	nop

end_if:

	inc	%l1		! ++col

inner_loop_test:

	cmp	%l1, %i0	! col < size
	bl	inner_loop
	nop


	/*
	 * Display the border char numOfDigits(size, 10) times.
	 */

	clr	%l5		! border char counter
	ba	row_border_test2
	nop

row_border_loop2:
	mov	%i3, %o0
	call	printChar
	nop

	inc	%l5

row_border_test2:
	cmp	%l5, %l3
	bl	row_border_loop2
	nop

	mov	NL, %o0
	call	printChar	! printChar( '\n' )
	nop

	inc	%l0		! ++row

outer_loop_test:

	cmp	%l0, %i0	! row < size
	bl	outer_loop
	nop


	clr	%l0		! bottom border row counter
	ba	bottom_border_outer_test
	nop

bottom_border_outer_loop:

	clr	%l1		! bottom border col counter
	ba	bottom_border_inner_test
	nop

bottom_border_inner_loop:
	mov	%i3, %o0
	call	printChar
	nop

	inc	%l1

bottom_border_inner_test:
	cmp	%l1, %l4
	bl	bottom_border_inner_loop
	nop

	mov	NL, %o0
	call	printChar
	nop

	inc	%l0

bottom_border_outer_test:
	cmp	%l0, %l3
	bl	bottom_border_outer_loop
	nop


	ret
	restore

