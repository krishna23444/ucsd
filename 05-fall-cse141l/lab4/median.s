# Median
#--------------------------Sort---------------------

#
# r0 -- limit
# r1 -- working address
# r2 -- a[i]
# r3 -- a[i+1]
# D[2] -- flag, until the end, when result is stored
#
        mov  $r0,    $zero    # r0 = 0
        ld   $r0,    $r0      # limit = num

        mov  $r1,    $one     # r1 = 1
        sub  $r0,    $r1      # limit--
        mov  $r0,    $res     # move from $res to $r0 (num-1)

        ld   $r1,    $r1      # ld start address of array (i=0)

        mov  $r2,    $one     # r2 = 1  
        sub  $r0,    $r2      # if num of elements is 1, go to end of prog
        bge  PRE_WHILE        # else goto while loop
        ld   $r1,    $r1      # ld value at median
        mov  $r3,    $one     # r3 = 1
        stn  $r3,    $r1      # store median at MEM[2]
        hlt                   # end program

PRE_WHILE: add  $r0,    $r1   # for counter purposes limit += start address

WHILE:  mov  $r3,    $one     # set $r3 to 1
        mov  $r2,    $zero    # r2 = 0
        stn  $r3,    $r2      # D[2] = flag = false

LOOP:   ld   $r2,    $r1      # a[i]
        ldn  $r3,    $r1      # a[i+1]

        sub  $r3,    $r2      # if a[i] < a[i+1]
        bge  ENDIF            # skip if
        st   $r1,    $r3      # a[i] = a[i+1]
        stn  $r1,    $r2      # a[i+1] = old a[i]
        mov  $r3,    $one     # $r3 = 1
        stn  $r3,    $r0      # D[2] = flag = true
        add  $one,   $r1      # i++

ENDIF:  add  $one,   $r1      # end for
        sub  $r1,    $r0      # for(i=0; i<limit; i++)
        blt  LOOP             # continue for loop

        mov  $r1,    $one
        ld   $r1,    $r1      # ld start address of array (i=0)

        mov  $r3,    $one     # r3 = 1
        ldn  $r3,    $r3      # while (flag)
        sub  $zero,  $r3      # if $zero < $r3 (flag)
        blt  WHILE            # continue while

#----------------------End Sort------------------------
        mov  $r0,    $zero    # r0 = 0
        ldn  $r1,    $r0      # r1 = a = array start
        ld   $r0,    $r0      # r0 = n = number of elements

        srl  $r0              # n = n/2

        add  $r1,    $r0      # a += n

        ld   $r1,    $r1      # ld value at median
        mov  $r3,    $one     # r3 = 1
        stn  $r3,    $r1      # store median at MEM[2]
        hlt                   # end of program
