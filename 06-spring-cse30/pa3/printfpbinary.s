/*
 * printFPBinary.s
 *
 * void printFPBinary( float f );
 *
 *  1 bit for sign
 *  8 bits of exponent
 * 23 bits of mantissa/fraction
 *
 * x xxxxxxxx xxxxxxxxxxxxxxxxxxxxxxx
 *
 * %l0 - bit mask that gets shifted down to test the bits for 0 or 1
 * %l1 - counter for outputting a space between the parts of the FP
 *
 */

	.global	printFPBinary

	.section	".rodata"

fmt:    .asciz  "%d"
fmt_sp: .asciz  " "

	.section	".text"

printFPBinary:

	save	%sp, (-92) & -8, %sp

	set     0x80000000, %l0         ! sign bit mask	

/* Can use these bit masks as one way to do this
   Can also use shifts left and right to isolate these parts of the FP
	set     0x7F800000, %l2         ! exponent mask	
	set     0x007FFFFF, %l3         ! mantissa mask	
*/

        btst    %l0, %i0 
        bne     sign_one
!       nop
        mov     1, %o1                  ! assume bit is a 1

        clr     %o1
sign_one:
        set     fmt, %o0
        call    printf, 2
        nop

        set     fmt_sp, %o0             ! output a space between sign bit and
        call    printf, 1		! exponent bits
        nop

        mov     8, %l1                  ! counter for outputing a space
					! after the 8 bits of exponent

        srl     %l0, 1, %l0		! shift mask bit down one position
					! to test the exponent bits

exp_loop:
        btst    %l0, %i0 
        bne     exp_one
!       nop
        mov     1, %o1                  ! assume bit is a 1

        clr     %o1
exp_one:
        set     fmt, %o0
        call    printf, 2
!       nop

        srl     %l0, 1, %l0		! shift mask bit down one position
					! we are walking down the exp. bits
        deccc   %l1
        bne     exp_loop
        nop

        set     fmt_sp, %o0             ! output a space between exponent and
        call    printf, 1		! mantissa/fraction bits
        nop

        mov     23, %l1                 ! counter for remaining mantissa bits

mantissa_loop:
        btst    %l0, %i0 
        bne     mantissa_one
!       nop
        mov     1, %o1                  ! assume bit is a 1

        clr     %o1
mantissa_one:
        set     fmt, %o0
        call    printf, 2
!       nop

        srl     %l0, 1, %l0		! shift mask bit down one position
					! we are walking down the mantissa bits
        deccc   %l1
        bne     mantissa_loop
        nop


	ret
	restore
