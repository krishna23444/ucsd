/*
 * printChar.s
 *
 * void printChar( char ch );
 */

	.global	printChar

	.section ".data"

fmt:	.asciz	"%c"

	.section ".text"

printChar:
	save	%sp, -96, %sp

	set	fmt, %o0
	mov	%i0, %o1
	call	printf
	nop

	ret
	restore

