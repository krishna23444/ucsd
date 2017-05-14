# =============== Mode ==================
#
# =======================================
# D-Mem[0] = number of elements in a[]
# D-Mem[1] = starting address of a[], that is &a[0]
# D-Mem[2] = return value, mode of a[]
# D-Mem[3-10] = scratch memory locations
# D-Mem[D-Mem[1]] = a[0]
# .
# .
# D-Mem[D-Mem[1] + D-Mem[0] - 1] = a[n-1]
# =======================================


#--------------------------------------
# Initialize start & end variables
#--------------------------------------
#   r0 <-> start
#   r1 <-> end
#--------------------------------------
#   r0 = &a[0]
#   r1 = &a[n]
#--------------------------------------
0xA4  0  START:  mov $r0,   $zero      # start = 0 -- addr of D[0]

0xC4  1          ld  $r1,   $r0        # end = a.length
0xD0  2          ldn $r0,   $r0        # start = D[1]

0x24  3          add $r1,   $r0        # end = start + a.length

#---------------------------------------
# Clear memory addresses 3-11
# NOTE: the loop below has been unrolled
#---------------------------------------
#   r2 <-> 0
#   r3 <-> pointer
#---------------------------------------
#   for (tmp = 3; tmp < 11; ++tmp)
#     D[tmp] = 0;
#---------------------------------------
0xB4  4          mov $r2,  $zero      # addr = 0
0xBE  5          mov $r3,  $three     # tmp = 3
0xEE  6          st  $r3,  $r2        # D[tmp] = 0
0x37  7          add $one, $r3        # tmp++
0xEE  8          st  $r3,  $r2        # D[tmp] = 0
0x37  9          add $one, $r3        # tmp++
0xEE  10          st  $r3,  $r2        # D[tmp] = 0
0x37  11          add $one, $r3        # tmp++
0xEE  12          st  $r3,  $r2        # D[tmp] = 0
0x37  13          add $one, $r3        # tmp++
0xEE  14          st  $r3,  $r2        # D[tmp] = 0
0x37  15          add $one, $r3        # tmp++
0xEE  16          st  $r3,  $r2        # D[tmp] = 0
0x37  17          add $one, $r3        # tmp++
0xEE  18          st  $r3,  $r2        # D[tmp] = 0
0x37  19          add $one, $r3        # tmp++
0xEE  20          st  $r3,  $r2        # D[tmp] = 0

#---------------------------------------
# Negative logic test for frequency count gathering loop
#---------------------------------------
#   r0 <-> start
#   r1 <-> end
#---------------------------------------
#   if (start >= end)
#     __SKIP_LOOP__
#---------------------------------------
0x41  21          sub $r0,   $r1        # if (start >= end)
0x8A  22          bge CNTLPEND          #   goto CNT_LP_END

#---------------------------------------
# Accumulate and store the frequency for each digit (0-7) in the spare memory
# address. The count for 0x0 is stored in D[3], the count for 0x0 in D[4], etc
# until 0x7 which is stored in D[10].
#---------------------------------------
#   r0 <-> start
#   r1 <-> end
#   r2 <-> addr
#   r3 <-> tmp
#---------------------------------------
#   while (start < end) {
#     addr = D[start];
#     tmp = 3;
#     addr += tmp;       <<-- address of the frequency count for 'start' value
#     tmp = D[addr];
#     tmp++;
#     D[addr] = tmp2;
#     start++;
#   }
#---------------------------------------
0xC8  23  CNTLP:  ld  $r2,   $r0        # addr = D[start]
0xBE  24          mov $r3,   $three     # tmp = 3
0x2B  25          add $r2,   $r3        # addr += tmp
0xCE  26          ld  $r3,   $r2        # tmp = D[addr]
0x37  27          add $one,  $r3        # tmp++
0xEB  28          st  $r2,   $r3        # D[addr] = tmp
0x34  29          add $one,  $r0        # start++
0x41  30          sub $r0,   $r1        # if (start < end)
0x08  31          blt CNTLP             #   goto CNTLP

#---------------------------------------
# Linear search through the frequencies in D[3] - D[10] for the maximum value.
# NOTE: the loop below has been unrolled
#---------------------------------------
#   r0 <-> hcnt     (D[addr] - highest count so far)
#   r1 <-> addr     (address with greatest count)
#   r2 <-> currcnt  (D[tmp] - current count being tested against)
#   r3 <-> tmp      (working address for loop)
#---------------------------------------
#   addr = 3;
#   for (tmp = 3; tmp < 11; ++tmp) 
#     if (D[addr] < D[tmp])
#       addr = tmp;
#---------------------------------------
0xBE  32  CNTLPEND: mov $r3,  $three    # tmp = 3
0xAB  33            mov $r1,  $r3       # addr = tmp
0xC1  34            ld  $r0,  $r1       # hcnt = D[addr]

0x37  35  D4CMP:  add $one, $r3         # tmp++
0xCB  36          ld  $r2,  $r3         # currcnt = D[tmp]
0x42  37          sub $r0,  $r2         # if (hcnt >= currcnt)
0x83  38          bge D5CMP             #   goto D5CMP
0xAB  39          mov $r1,  $r3         # addr = curraddr
0xA2  40          mov $r0,  $r2         # hcnt = currcnt

0x37  41  D5CMP:  add $one, $r3         # tmp++
0xCB  42          ld  $r2,  $r3         # currcnt = D[tmp]
0x42  43          sub $r0,  $r2         # if (hcnt >= currcnt)
0x83  44          bge D6CMP             #   goto D6CMP
0xAB  45          mov $r1,  $r3         # addr = curraddr
0xA2  46          mov $r0,  $r2         # hcnt = currcnt

0x37  47  D6CMP:  add $one, $r3         # tmp++
0xCB  48          ld  $r2,  $r3         # currcnt = D[tmp]
0x42  49          sub $r0,  $r2         # if (hcnt >= currcnt)
0x83  50          bge D7CMP             #   goto D7CMP
0xAB  51          mov $r1,  $r3         # addr = curraddr
0xA2  52          mov $r0,  $r2         # hcnt = currcnt

0x37  53  D7CMP:  add $one, $r3         # tmp++
0xCB  54          ld  $r2,  $r3         # currcnt = D[tmp]
0x42  55          sub $r0,  $r2         # if (hcnt >= currcnt)
0x83  56          bge D8CMP             #   goto D8CMP
0xAB  57          mov $r1,  $r3         # addr = curraddr
0xA2  58          mov $r0,  $r2         # hcnt = currcnt

0x37  59  D8CMP:  add $one, $r3         # tmp++
0xCB  60          ld  $r2,  $r3         # currcnt = D[tmp]
0x42  61          sub $r0,  $r2         # if (hcnt >= currcnt)
0x83  62          bge D9CMP             #   goto D9CMP
0xAB  63          mov $r1,  $r3         # addr = curraddr
0xA2  64          mov $r0,  $r2         # hcnt = currcnt

0x37  65  D9CMP:  add $one, $r3         # tmp++
0xCB  66          ld  $r2,  $r3         # currcnt = D[tmp]
0x42  67          sub $r0,  $r2         # if (hcnt >= currcnt)
0x83  68          bge D10CMP            #   goto D10CMP
0xAB  69          mov $r1,  $r3         # addr = curraddr
0xA2  70          mov $r0,  $r2         # hcnt = currcnt

0x37  71  D10CMP: add $one, $r3         # tmp++
0xCB  72          ld  $r2,  $r3         # currcnt = D[tmp]
0x42  73          sub $r0,  $r2         # if (hcnt >= currcnt)
0x83  74          bge FINISH            #   goto FINISH
0xAB  75          mov $r1,  $r3         # addr = curraddr
0xA2  76          mov $r0,  $r2         # hcnt = currcnt

0xBE  77  FINISH: mov $r3,  $three      # r3 = 3
0x47  78          sub $r1,  $r3         # res = addr - 3
0xAF  79          mov $r1,  $res        # r1 = res
0xB5  80          mov $r2,  $one        # r2 = 1
0x36  81          add $one, $r2         # r2++
0xE9  82          st  $r2,  $r1         # D[2] = mode
0x7D  83          hlt                   # end of program

