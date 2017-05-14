# =======================================
# D-Mem 0 = Starting address
# D-Mem 1 = # of elements in array
# D-Mem 2 = Return value, mean of a[]
#
# =======================================
# r0 	<->	start address
# r1	<->	for loop count
# r2	<->	current value to add
# r3	<->	number of elements
# ---------------------------------------
# k = array number of starting
# for (i = 0; i < 16; i++) {
# 	sum = ak + i;
# }
# avg = sum / 16
# r = sum % 16
# if (r >= 8) {
# 	avg++;	
# }
# ---------------------------------------

		mov	$r0, $zero
                ld      $r0, $r0        # r0 = start address = d0
                ld      $r1, $r0        # load in the new add value
# ---------------------------------------
# Begin the unlooped-loop in order to
# optimize for the standard 16 numbers
# ---------------------------------------
                add     $res, $r1       # res += r2
                inch                    # increment three iff overflow = 1
                add     $one, $r0       # ++i
                ld      $r1, $r0
                add     $res, $r1
                inch
                add     $one, $r0
                ld      $r1, $r0
                add     $res, $r1
                inch
                add     $one, $r0
                ld      $r1, $r0
                add     $res, $r1
                inch
                add     $one, $r0
                ld      $r1, $r0        # 5
                add     $res, $r1
                inch
                add     $one, $r0
                ld      $r1, $r0
                add     $res, $r1
                inch
                add     $one, $r0
                ld      $r1, $r0
                add     $res, $r1
                inch
                add     $one, $r0
                ld      $r1, $r0
                add     $res, $r1
                inch
                add     $one, $r0
                ld      $r1, $r0
                add     $res, $r1
                inch
                add     $one, $r0
                ld      $r1, $r0        # 10
                add     $res, $r1
                inch
                add     $one, $r0
                ld      $r1, $r0
                add     $res, $r1
                inch
                add     $one, $r0
                ld      $r1, $r0
                add     $res, $r1
                inch
                add     $one, $r0
                ld      $r1, $r0
                add     $res, $r1
                inch
                add     $one, $r0
                ld      $r1, $r0
                add     $res, $r1
                inch
                add     $one, $r0
                ld      $r1, $r0
                add     $res, $r1
                inch
                add     $one, $r0
                ld      $r1, $r0        # 16th one for static 16 numbers
                add     $res, $r1
                inch
# ---------------------------------------
# Now shift lower portion 4 bits right
# shift upper portion 4 bits left
# add them together and will get the
# number divided by 16
# ---------------------------------------
# 0000 1010 0111 0101
# <---           --->
# 1010 0000 0000 0111
# Add them together to get
# 1010 0111 = the number divided by 16
# ---------------------------------------
                srl4    $res            # res / 4
                sll4    $three          # shift upper 4 bits left
		mov	$r0, $three	# move upper into a register
                add     $res, $r0       # add upper to lower
# ---------------------------------------
# Deal with the remainder
# If the number is >= 8 (1xxx) round up
# shift right 3, and with 0x1 and add num
# ---------------------------------------
                mov     $r2, $res       # r2 = res temporarily
                srl3    $r2
                and     $r2, $one       # if r2 >= 8, r2 == 1
                add     $res, $r2       # res += r2 (if remainder, add 1)
                add     $one, $r0       # d1
                add     $one, $r0       # d2
		mov	$r1, $res
                st      $r0, $r1        # store averge in
		hlt
