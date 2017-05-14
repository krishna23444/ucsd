/*
 * printEnglish.s
 * 
 * void printEnglish( long num );
 *
 */

	.global	printEnglish

	.section ".rodata"

fmt:	.asciz	"%s "

zero:	.asciz	"zero"
one:	.asciz	"one"
two:	.asciz	"two"
three:	.asciz	"three"
four:	.asciz	"four"
five:	.asciz	"five"
six:	.asciz	"six"
seven:	.asciz	"seven"
eight:	.asciz	"eight"
nine:	.asciz	"nine"

/*
 * char *words[] = { "zero", "one", "two", "three", "four", "five",
 *                   "six", "seven", "eight", "nine" };
 */
	.align 4
words:	
	.word zero, one, two, three, four, five, six, seven, eight, nine


	.section ".text"

	rt_digit_offset = -4

printEnglish:
	save	%sp, (-92 + rt_digit_offset) & -8, %sp

/***********
  Should do negative ("minus ") check here!!!!!
 ***********/

	mov	%i0, %o0
	call	.rem
	mov	10, %o1

	st	%o0, [%fp + rt_digit_offset]	! rt_digit = num % 10

	mov	%i0, %o0
	call	.div
	mov	10, %o1

	/*
         * result of (num / 10) is in %o0 and ready as param for next recursion
	 */

	cmp	%o0, 0
	be	no_recursion
	nop

	call	printEnglish, 1			! recurse
	nop

no_recursion:

	set	words, %l0			! words
	ld	[%fp + rt_digit_offset], %l1	! rt_digit
	sll	%l1, 2, %l1			! rt_digit * 4
	add	%l0, %l1, %o1			! word + rt_digit * 4
	ld	[%o1], %o1			! *(words + rt_digit * 4)
	set	fmt, %o0
	call	printf, 2
	nop

	ret
	restore

