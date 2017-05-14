# =======================================
# D-Mem [0] = Starting address
# D-Mem [1] = # of elements in array
# D-Mem [2] = Return value, mean of a[]
#
# =======================================
# r0 	<->	start address
# r1	<->	for loop count
# r2	<->	current value to add
# r3	<->	number of elements
# ---------------------------------------
# k = array number of starting
# for (i = 0; i < 16; i++) {
# 	sum = a[k + i];
# }
# avg = sum / 16
# r = sum % 16
# if (r >= 8) {
# 	avg++;	
# }
# ---------------------------------------

0x00  000        ld      $r0, $zero      # r0 = start address = d[0]
0x00  001        ld      $r1, [$r0]      # load in the new add value

# ---------------------------------------
# Begin the unlooped-loop in order to
# optimize for the standard 16 numbers
# ---------------------------------------

0x00  002        add     $res, $r1       # res += r2
0x00  003        inch           	 # increment dsth iff overflow = 1
0x00  004        add     $one, $r0       # ++i
0x00  005        ld      $r1, [$r0]
0x00  006        add     $res, $r1
0x00  007        inch
0x00  008        add     $one, $r0
0x00  010        ld      $r1, [$r0]
0x00  011        add     $res, $r1
0x00  012        inch
0x00  013        add     $one, $r0
0x00  014        ld      $r1, [$r0]
0x00  015        add     $res, $r1
0x00  016        inch
0x00  017        add     $one, $r0
0x00  018        ld      $r1, [$r0]      # 5
0x00  019        add     $res, $r1
0x00  020        inch
0x00  021        add     $one, $r0
0x00  022        ld      $r1, [$r0]
0x00  023        add     $res, $r1
0x00  024        inch
0x00  025        add     $one, $r0
0x00  026        ld      $r1, [$r0]
0x00  027        add     $res, $r1
0x00  028        inch
0x00  029        add     $one, $r0
0x00  030        ld      $r1, [$r0]
0x00  031        add     $res, $r1
0x00  032        inch
0x00  033        add     $one, $r0
0x00  034        ld      $r1, [$r0]
0x00  035        add     $res, $r1
0x00  036        inch
0x00  037        add     $one, $r0
0x00  038        ld      $r1, [$r0]      # 10
0x00  039        add     $res, $r1
0x00  040        inch
0x00  041        add     $one, $r0
0x00  042        ld      $r1, [$r0]
0x00  043        add     $res, $r1
0x00  044        inch
0x00  045        add     $one, $r0
0x00  046        ld      $r1, [$r0]
0x00  047        add     $res, $r1
0x00  048        inch
0x00  049        add     $one, $r0
0x00  050        ld      $r1, [$r0]
0x00  051        add     $res, $r1
0x00  052        inch
0x00  053        add     $one, $r0
0x00  054        ld      $r1, [$r0]
0x00  055        add     $res, $r1
0x00  056        inch
0x00  057        add     $one, $r0
0x00  058        ld      $r1, [$r0]
0x00  059        add     $res, $r1
0x00  060        inch
0x00  061        add     $one, $r0
0x00  062        ld      $r1, [$r0]      # 16th one for static 16 numbers
0x00  063        add     $res, $r1
0x00  064        inch

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

0x00  065        srl4    $res            # res / 4
0x00  066        sll4    $three          # shift upper 4 bits left
0x00  067        add     $res, $three    # add upper to lower
# ---------------------------------------
# Deal with the remainder
# If the number is >= 8 (1xxx) round up
# shift right 3, and with 0x1 and add num
# ---------------------------------------
0x00  068        mov     $r2, $res       # r2 = res temporarily
0x00  069        srl3    $r2
0x00  070        and     $r2, $one       # if r2 >= 8, r2 == 1
0x00  071        add     $res, $r2       # res += r2 (if remainder, add 1)
0x00  072        add     $one, $r0       # d[1]
0x00  073        add     $one, $r0       # d[2]
0x00  074        st      [$r0], $res     # store averge in