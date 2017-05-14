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
0x00  001                 ld      $r0, $zero      # r0 = start address = d[0]
0x00  002                 ld      $r3, $one       # get length of set
0x00  003 inner:          ld      $r2, [$r0]      # load in the new add value   
0x00  004                 add     $dstl, $r2      # dstl += r2
0x00  005                 inch                    # increment dsth iff overflow = 1
0x00  006                 add     $one, $r1       # ++i
0x00  007                 cmp     $r1, $end
0x00  008                 blt     inner    
# ---------------------------------------
# r0	<->	start address
# r1	<->	0x0F (16-1 = 15)
# r2	<->
# r3	<->	number of elements
# ---------------------------------------
0x00  009                 mov     $r2, $dstl      # r2 = dstl temporarily
0x00  010                 sub     $r3, $one       # r3 (16) - 1 = 0x0F   
0x00  011                 mov     $r1, $dstl      # r1 = 0x0F
0x00  012                 mov     $dstl, $r2      # dstl = r2 (back to sum)
0x00  013                 and     $r2, $r1        # r2 is now remainder
0x00  014                 srl4    $dstl           # dstl / 4
0x00  015                 sll4    $dsth           # shift upper 4 bits left
0x00  016                 add     $dstl, $dsth    # add upper to lower     
0x00  017                 srl     $r3             # r3 = 8 (16/2)
0x00  018                 add     $one, $r3       # r3 = 9       
# ---------------------------------------
# r0	<->	start address
# r1	<->	0x0F (16-1 = 15)
# r2	<->	remainder
# r3	<->	9, to determine if you need to increment average
# ---------------------------------------
0x00  019                 cmp     $r2, $r3
0x00  020                 bge     skipsuminc
0x00  021                 add     $one, $dstl
0x00  022 skipsuminc:     add     $one, $r0       # d[1]
0x00  023                 add     $one, $r0       # d[2]
0x00  024                 st      [$r0], $dstl    # store averge in