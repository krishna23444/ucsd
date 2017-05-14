# Median
#--------------------------Sort---------------------
        mov  $r0,    $zero    # r0 = 0
        ld   $r0,    $r0      # limit = num

        mov  $r1,    $one     # r1 = one
        sub  $r0,    $r1      # limit--
        mov  $r0,    $res     # move from $res to $r0 (num-1)

        mov  $r1,    $zero    # r1 = 0
        ldn  $r1,    $r1      # ld start address of array (i=0)

        mov  $r2,   $one      # r2 = 1  
        sub  $r0,   $r2       # if num of elements is 1, go to end of prog
        bge  WHILE            # else goto while loop
        add  $r0,    $r1      # for counter purposes limit += start address
        ld   $r1,    $r1      # ld value at median
        mov  $r3,    $one     # r3 = 1
        add  $one,   $r3      # r3 = 1+1
        st   $r3,    $r1      # store median at MEM[2]
        hlt                   # end program

WHILE:  add  $r0,    $r1      # for counter purposes limit += start address
        mov  $r3,    $three   # set $r3 to 3
        mov  $r2,    $zero    # r3 = 0
        st   $r3,    $r2      # flag = false

LOOP:   ld   $r2,    $r1      # a[i]
        ldn  $r3,    $r1      # a[i+1]

        sub  $r3,    $r2      # if a[i] < a[i+1]
        bge  ENDIF            # skip if
        st   $r1,    $r3      # a[i] = a[i+1]
        stn  $r1,    $r2      # a[i+1] = old a[i]
        mov  $r3,    $three   # set $r3 to 3
        st   $r3,    $r0      # flag = true
        add  $one,   $r1      # i++

ENDIF:  add  $one,   $r1      # end for
        sub  $r1,    $r0      # for(i=0; i<limit; i++)
        blt  LOOP             # continue for loop

        mov  $r1,    $zero
        ldn  $r1,    $r1      # ld start address of array (i=0)
        mov  $r2,    $zero    # r2 = 1 ALTERED
        sub  $r0,    $r2      # limit--
        mov  $r0,    $res     # move limit-- to limit

        mov  $r3,    $three   # r3 = 3
        ld   $r3,    $r3      # while(flag)
        sub  $zero,  $r3      # if $zero < $r3 (flag)
        blt  WHILE            # continue while

#----------------------End Sort------------------------
        mov  $r0,    $zero    # r0 = 0
        ld   $r0,    $r0      # n = number of elements
        mov  $r1,    $zero    # r1 = 0
        ldn  $r1,    $r1      # a = array start

        srl  $r0              # n = n/2

        add  $r1,    $r0      # a += n

        ld   $r1,    $r1      # ld value at median
        mov  $r3,    $one     # r3 = 1
        add  $one,   $r3      # r3 = 1+1
        st   $r3,    $r1      # store median at MEM[2]
        hlt                   # end of program

