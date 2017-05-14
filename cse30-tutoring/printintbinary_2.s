
	.global	printIntBinary

	.section ".rodata"

fmt:	.asciz	"%d"
fmt_sp:	.asciz	" "


	.section ".text"

printIntBinary:
	save	%sp, (-92) & -8, %sp

	set	0x80000000, %l0		! mask

	mov	4, %l1			! counter for outputing a space


loop:
	btst	%l0, %i0 
	bne	one
!	nop
	mov	1, %o1			! assume bit is a 1

	clr	%o1
one:
	set	fmt, %o0
	call	printf, 2
	nop

	deccc	%l1
	bne	no_space
	nop

	set	fmt_sp, %o0		! output a space every 4 bits
	call	printf, 1
	nop

	mov	4, %l1			! reset counter

no_space:
	srl	%l0, 1, %l0
	tst	%l0
	bne	loop
	nop

	ret
	restore

