# Median
#--------------------------Sort---------------------

#
# r0 -- limit
# r1 -- working address
# r2 -- a[i]
# r3 -- a[i+1]
# D[2] -- flag, until the end, when result is stored
#
0xA4  0          mov  $r0,    $zero    # r0 = 0
0xC0  1          ld   $r0,    $r0      # limit = num

0xAD  2          mov  $r1,    $one     # r1 = 1
0x41  3          sub  $r0,    $r1      # limit--
0xA7  4          mov  $r0,    $res     # move from $res to $r0 (num-1)

0xC5  5          ld   $r1,    $r1      # ld start address of array (i=0)

0xB5  6          mov  $r2,    $one     # r2 = 1  
0x42  7          sub  $r0,    $r2      # if num of elements is 1, go to end of prog
0x85  8          bge  PRE_WHILE        # else goto while loop
0xC5  9          ld   $r1,    $r1      # ld value at median
0xBD  10          mov  $r3,    $one     # r3 = 1
0xFD  11          stn  $r3,    $r1      # store median at MEM[2]
0x7D  12          hlt                   # end program

0x21  13  PRE_WHILE: add  $r0,    $r1   # for counter purposes limit += start address

0xBD  14  WHILE:  mov  $r3,    $one     # set $r3 to 1
0xB4  15          mov  $r2,    $zero    # r2 = 0
0xFE  16          stn  $r3,    $r2      # D[2] = flag = false

0xC9  17  LOOP:   ld   $r2,    $r1      # a[i]
0xDD  18          ldn  $r3,    $r1      # a[i+1]

0x4E  19          sub  $r3,    $r2      # if a[i] < a[i+1]
0x86  20          bge  ENDIF            # skip if
0xE7  21          st   $r1,    $r3      # a[i] = a[i+1]
0xF6  22          stn  $r1,    $r2      # a[i+1] = old a[i]
0xBD  23          mov  $r3,    $one     # $r3 = 1
0xFC  24          stn  $r3,    $r0      # D[2] = flag = true
0x35  25          add  $one,   $r1      # i++

0x35  26  ENDIF:  add  $one,   $r1      # end for
0x44  27          sub  $r1,    $r0      # for(i=0; i<limit; i++)
0x0B  28          blt  LOOP             # continue for loop

0xAD  29          mov  $r1,    $one
0xC5  30          ld   $r1,    $r1      # ld start address of array (i=0)

0xBD  31          mov  $r3,    $one     # r3 = 1
0xDF  32          ldn  $r3,    $r3      # while (flag)
0x53  33          sub  $zero,  $r3      # if $zero < $r3 (flag)
0x14  34          blt  WHILE            # continue while

#----------------------End Sort------------------------
0xA4  35          mov  $r0,    $zero    # r0 = 0
0xD4  36          ldn  $r1,    $r0      # r1 = a = array start
0xC0  37          ld   $r0,    $r0      # r0 = n = number of elements

0x68  38          srl  $r0              # n = n/2

0x24  39          add  $r1,    $r0      # a += n

0xC5  40          ld   $r1,    $r1      # ld value at median
0xBD  41          mov  $r3,    $one     # r3 = 1
0xFD  42          stn  $r3,    $r1      # store median at MEM[2]
0x7D  43          hlt                   # end of program
