/*
 * Filename: sum3.s
 * Author: Joe Jane Student
 * Userid: cs30xzz
 * Description: Example SPARC assembly routine to sum 3 arguments and
 *              return the sum.
 *              Called from main()
 * Date: Jan. 10, 2001
 * Sources of Help: List all the people, books, websites, etc. that you
 *                  used to help you write the code in this source file.
 */ 

        .global	sum3		! Declares the symbol to be globally visible so
				! we can call this function from other modules.

        .section ".text"	! The text segment begins here

/*
 * Function name: sum3()
 * Function prototype: int sum3( int a, int b, int c );
 * Description: Sums 3 int arguments and returns the sum.
 * Parameters: 
 *	arg 1: int a -- 1st number to sum.
 *	arg 2: int b -- 2nd number to sum.
 *	arg 3: int c -- 3rd number to sum.
 *
 * Side Effects: None.
 * Error Conditions: None
 * Return Value: Sum of the 3 arguments.
 *
 * Registers Used:
 *	%i0 - arg 1 -- 1st number to sum; also used to return the sum.
 *	%i1 - arg 2 -- 2nd number to sum.
 *	%i2 - arg 3 -- 3rd number to sum.
 *
 *	%l0 - scratch local reg. to store intermediate addition results
 */

sum3:
	save	%sp, -96, %sp	! Save caller's window; if different than -96
				! then comment on how that value was calculated.

	add	%i0, %i1, %l0	! Add 1st two args, store result in a local reg
				! %l0 = %i0 + %i1
	add	%l0, %i2, %l0	! Add 3rd arg to result above
				! %l0 = %l0 + %i2

	mov	%l0, %i0	! Return value goes in %i0

	ret			! Return from subroutine
	restore			! Restore caller's window; in "ret" delay slot

