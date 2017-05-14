/*
 * Filename: printHello.s
 * Author: Joe Jane Student
 * Userid: cs30xzz
 * Description: Example SPARC assembly routine to print out a greeting followed
 *		by a string (student's name) passed in as an argument
 *		to this function.
 *              Called from main()
 * Date: Jan. 10, 2001
 * Sources of Help: List all the people, books, websites, etc. that you
 *                  used to help you write the code in this source file.
 */ 

        .global	printHello	! Declares the symbol to be globally visible so
				! we can call this function from other modules.

        .section ".data"	! The data segment begins here

fmt:    			! The format string to printf()
	.asciz  "Hello, my name is: %s\n"


        .section ".text"	! The text segment begins here

/*
 * Function name: printHello()
 * Function prototype: void printHello( char *str );
 * Description: Prints to stdout a greeting string and the argument
 *              with a terminating newline via printf()
 * Parameters: 
 *	arg 1: char *str -- the string to print after the greeting
 *
 * Side Effects: Outputs greeting with string supplied as argument.
 * Error Conditions: None. [Arg 1 is not checked to ensure it is not NULL.]
 * Return Value: None
 *
 * Registers Used:
 *	%i0 - arg 1 -- the string (char *) passed in to this function
 *
 *	%o0 - param 1 to printf() -- format string
 *	%o1 - param 2 to printf() -- copy of arg 1 being passed in
 */

printHello:
	save	%sp, -96, %sp	! Save caller's window; if different than -96
				! then comment on how that value was calculated.

	set	fmt, %o0	! Parameter 1 to printf() goes in register %o0
	mov	%i0, %o1	! Parameter 2 to printf() goes in register %o1
	call	printf, 2	! Make function call specifying # of params
	nop			! Delay slot for call instruction

	ret			! Return from subroutine
	restore			! Restore caller's window; in "ret" delay slot

