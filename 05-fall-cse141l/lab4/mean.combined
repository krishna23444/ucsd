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

0x7E  0                  clrr                    # res (sum_high) = 0
0xB4  1                  mov     $r2, $zero      # r2 (sum_low) = 0
0xA4  2                  mov     $r0, $zero
0xC0  3                  ld      $r0, $r0        # r0 = start address = d0
# ---------------------------------------
# Begin the unlooped-loop in order to
# optimize for the standard 16 numbers
# ---------------------------------------
0xC4  4                  ld      $r1, $r0        # load first element to add (D[D[0]])
0x29  5                  add     $r2, $r1        # r2 += r1
0x7C  6                  inch                    # increment three iff overflow = 1
0x34  7                  add     $one, $r0       # ++i
0xC4  8                  ld      $r1, $r0        # load the next element and add (2)
0x29  9                  add     $r2, $r1        # r2 += r1 where r2 is the total
0x7C  10                  inch                    # increment the upper if needed
0x34  11                  add     $one, $r0       # add one to read the next element
0xC4  12                  ld      $r1, $r0        # load the next element and add (3)
0x29  13                  add     $r2, $r1        # r2 += r1 where r2 is the total
0x7C  14                  inch                    # increment the upper if needed
0x34  15                  add     $one, $r0       # add one to read the next element
0xC4  16                  ld      $r1, $r0        # load the next element and add (4)
0x29  17                  add     $r2, $r1        # r2 += r1 where r2 is the total
0x7C  18                  inch                    # increment the upper if needed
0x34  19                  add     $one, $r0       # add one to read the next element
0xC4  20                  ld      $r1, $r0        # load the next element and add (5)
0x29  21                  add     $r2, $r1        # r2 += r1 where r2 is the total
0x7C  22                  inch                    # increment the upper if needed
0x34  23                  add     $one, $r0       # add one to read the next element
0xC4  24                  ld      $r1, $r0        # load the next element and add (6)
0x29  25                  add     $r2, $r1        # r2 += r1 where r2 is the total
0x7C  26                  inch                    # increment the upper if needed
0x34  27                  add     $one, $r0       # add one to read the next element
0xC4  28                  ld      $r1, $r0        # load the next element and add (7)
0x29  29                  add     $r2, $r1        # r2 += r1 where r2 is the total
0x7C  30                  inch                    # increment the upper if needed
0x34  31                  add     $one, $r0       # add one to read the next element
0xC4  32                  ld      $r1, $r0        # load the next element and add (8)
0x29  33                  add     $r2, $r1        # r2 += r1 where r2 is the total
0x7C  34                  inch                    # increment the upper if needed
0x34  35                  add     $one, $r0       # add one to read the next element
0xC4  36                  ld      $r1, $r0        # load the next element and add (9)
0x29  37                  add     $r2, $r1        # r2 += r1 where r2 is the total
0x7C  38                  inch                    # increment the upper if needed
0x34  39                  add     $one, $r0       # add one to read the next element
0xC4  40                  ld      $r1, $r0        # load the next element and add (10)
0x29  41                  add     $r2, $r1        # r2 += r1 where r2 is the total
0x7C  42                  inch                    # increment the upper if needed
0x34  43                  add     $one, $r0       # add one to read the next element
0xC4  44                  ld      $r1, $r0        # load the next element and add (11)
0x29  45                  add     $r2, $r1        # r2 += r1 where r2 is the total
0x7C  46                  inch                    # increment the upper if needed
0x34  47                  add     $one, $r0       # add one to read the next element
0xC4  48                  ld      $r1, $r0        # load the next element and add (12)
0x29  49                  add     $r2, $r1        # r2 += r1 where r2 is the total
0x7C  50                  inch                    # increment the upper if needed
0x34  51                  add     $one, $r0       # add one to read the next element
0xC4  52                  ld      $r1, $r0        # load the next element and add (13)
0x29  53                  add     $r2, $r1        # r2 += r1 where r2 is the total
0x7C  54                  inch                    # increment the upper if needed
0x34  55                  add     $one, $r0       # add one to read the next element
0xC4  56                  ld      $r1, $r0        # load the next element and add (14)
0x29  57                  add     $r2, $r1        # r2 += r1 where r2 is the total
0x7C  58                  inch                    # increment the upper if needed
0x34  59                  add     $one, $r0       # add one to read the next element
0xC4  60                  ld      $r1, $r0        # load the next element and add (15)
0x29  61                  add     $r2, $r1        # r2 += r1 where r2 is the total
0x7C  62                  inch                    # increment the upper if needed
0x34  63                  add     $one, $r0       # add one to read the next element
0xC4  64                  ld      $r1, $r0        # load the next element and add (16)
0x29  65                  add     $r2, $r1        # r2 += r1 where r2 is the total
0x7C  66                  inch                    # increment the upper if needed
# ---------------------------------------
# Deal with the remainder
# If the number is >= 8 (1xxx) round up
# shift right 3, and with 0x1 and add num
# ---------------------------------------
0xBA  67                  mov     $r3, $r2        # r3 = r2 (total)
0x7B  68                  srl3    $r3             # r3 >>= 3 (shift it 3 to get top)
0x93  69                  and1    $r3             # r3 &= 0x1 (will be one if rem >=8)

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
0x62  70                  srl4    $r2             # r2 >>= 4
0x77  71                  sll4    $res            # shift upper 4 bits left
0xA7  72                  mov     $r0, $res       # move upper into a register
0x28  73                  add     $r2, $r0        # add upper to lower

0x2B  74                  add     $r2, $r3        # add remainder to sum

0xAD  75                  mov     $r1,  $one      # r1 = 1
0x35  76                  add     $one, $r1       # r1++
0xE6  77                  st      $r1,  $r2       # store averge to D[2]

0x7D  78                  hlt
