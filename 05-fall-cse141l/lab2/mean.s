# =======================================
# mean.s for CSE 141L Lab 2 Fall 2005
# =======================================
# D-Mem 0 = Starting address
# D-Mem 1 = # of elements in array
# D-Mem 2 = Return value, mean of a[]
#
# =======================================
# r0   <->  memory address
# r1  <->  current value to add
# r2  <->  running sum, low byte
# r3  <->  remainder calculation
# res <-> running sum, high byte
# ---------------------------------------
# k = array number of starting
# for (i = 0; i < 16; i++) {
#   sum = ak + i;
# }
# avg = sum / 16
# r = sum % 16
# if (r >= 8) {
#   avg++;  
# }
# ---------------------------------------

                clrr                    # res (sum_high) = 0
                mov     $r2, $zero      # r2 (sum_low) = 0
                mov     $r0, $zero
                ld      $r0, $r0        # r0 = start address = d0
# ---------------------------------------
# Begin the unlooped-loop in order to
# optimize for the standard 16 numbers
# ---------------------------------------
                ld      $r1, $r0        # load first element to add (D[D[0]])
                add     $r2, $r1        # r2 += r1
                inch                    # increment three iff overflow = 1
                add     $one, $r0       # ++i
                ld      $r1, $r0        # load the next element and add (2)
                add     $r2, $r1        # r2 += r1 where r2 is the total
                inch                    # increment the upper if needed
                add     $one, $r0       # add one to read the next element
                ld      $r1, $r0        # load the next element and add (3)
                add     $r2, $r1        # r2 += r1 where r2 is the total
                inch                    # increment the upper if needed
                add     $one, $r0       # add one to read the next element
                ld      $r1, $r0        # load the next element and add (4)
                add     $r2, $r1        # r2 += r1 where r2 is the total
                inch                    # increment the upper if needed
                add     $one, $r0       # add one to read the next element
                ld      $r1, $r0        # load the next element and add (5)
                add     $r2, $r1        # r2 += r1 where r2 is the total
                inch                    # increment the upper if needed
                add     $one, $r0       # add one to read the next element
                ld      $r1, $r0        # load the next element and add (6)
                add     $r2, $r1        # r2 += r1 where r2 is the total
                inch                    # increment the upper if needed
                add     $one, $r0       # add one to read the next element
                ld      $r1, $r0        # load the next element and add (7)
                add     $r2, $r1        # r2 += r1 where r2 is the total
                inch                    # increment the upper if needed
                add     $one, $r0       # add one to read the next element
                ld      $r1, $r0        # load the next element and add (8)
                add     $r2, $r1        # r2 += r1 where r2 is the total
                inch                    # increment the upper if needed
                add     $one, $r0       # add one to read the next element
                ld      $r1, $r0        # load the next element and add (9)
                add     $r2, $r1        # r2 += r1 where r2 is the total
                inch                    # increment the upper if needed
                add     $one, $r0       # add one to read the next element
                ld      $r1, $r0        # load the next element and add (10)
                add     $r2, $r1        # r2 += r1 where r2 is the total
                inch                    # increment the upper if needed
                add     $one, $r0       # add one to read the next element
                ld      $r1, $r0        # load the next element and add (11)
                add     $r2, $r1        # r2 += r1 where r2 is the total
                inch                    # increment the upper if needed
                add     $one, $r0       # add one to read the next element
                ld      $r1, $r0        # load the next element and add (12)
                add     $r2, $r1        # r2 += r1 where r2 is the total
                inch                    # increment the upper if needed
                add     $one, $r0       # add one to read the next element
                ld      $r1, $r0        # load the next element and add (13)
                add     $r2, $r1        # r2 += r1 where r2 is the total
                inch                    # increment the upper if needed
                add     $one, $r0       # add one to read the next element
                ld      $r1, $r0        # load the next element and add (14)
                add     $r2, $r1        # r2 += r1 where r2 is the total
                inch                    # increment the upper if needed
                add     $one, $r0       # add one to read the next element
                ld      $r1, $r0        # load the next element and add (15)
                add     $r2, $r1        # r2 += r1 where r2 is the total
                inch                    # increment the upper if needed
                add     $one, $r0       # add one to read the next element
                ld      $r1, $r0        # load the next element and add (16)
                add     $r2, $r1        # r2 += r1 where r2 is the total
                inch                    # increment the upper if needed
# ---------------------------------------
# Deal with the remainder
# If the number is >= 8 (1xxx) round up
# shift right 3, and with 0x1 and add num
# ---------------------------------------
                mov     $r3, $r2        # r3 = r2 (total)
                srl3    $r3             # r3 >>= 3 (shift it 3 to get top)
                and1    $r3             # r3 &= 0x1 (will be one if rem >=8)

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
                srl4    $r2             # r2 >>= 4
                sll4    $res            # shift upper 4 bits left
                mov     $r0, $res       # move upper into a register
                add     $r2, $r0        # add upper to lower

                add     $r2, $r3        # add remainder to sum

                mov     $r1,  $one      # r1 = 1
                add     $one, $r1       # r1++
                st      $r1,  $r2       # store averge to D[2]

                hlt
