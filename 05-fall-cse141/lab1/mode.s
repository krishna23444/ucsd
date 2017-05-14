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
0x00  000 START:  mov $zero, $r0        # start = 0 -- addr of D[0]

0x00  001         ld  $r1,   $r0        # end = a.length
0x00  002         ldn $r0,   $r0        # start = D[1]

0x00  003         add $r1,   $r0        # end = start + a.length

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
0x00  004         mov $r2,  $zero       # addr = 0
0x00  005         set3                  # tmp = 3
0x00  006         st  [$r3], $r2        # D[tmp] = 0
0x00  007         add $one, $r3         # tmp++
0x00  008         st  [$r3], $r2        # D[tmp] = 0
0x00  009         add $one, $r3         # tmp++
0x00  010         st  [$r3], $r2        # D[tmp] = 0
0x00  011         add $one, $r3         # tmp++
0x00  012         st  [$r3], $r2        # D[tmp] = 0
0x00  013         add $one, $r3         # tmp++
0x00  014         st  [$r3], $r2        # D[tmp] = 0
0x00  015         add $one, $r3         # tmp++
0x00  016         st  [$r3], $r2        # D[tmp] = 0
0x00  017         add $one, $r3         # tmp++
0x00  018         st  [$r3], $r2        # D[tmp] = 0
0x00  019         add $one, $r3         # tmp++
0x00  020         st  [$r3], $r2        # D[tmp] = 0

#---------------------------------------
# Negative logic test for frequency count gathering loop
#---------------------------------------
#   r0 <-> start
#   r1 <-> end
#---------------------------------------
#   if (start >= end)
#     __SKIP_LOOP__
#---------------------------------------
0x00  021         cmp $r0,   $r1        # if (start >= end)
0x00  022         bge CNTLPEND          #   goto CNT_LP_END

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
0x00  023 CNTLP:  ld  $r2,   [$r0]      # addr = D[start]
0x00  024         set3                  # tmp = 3
0x00  025         add $r2,   $r3        # addr += tmp
0x00  026         ld  $r3,   [$r2]      # tmp = D[addr]
0x00  027         add $one,  $r3        # tmp++
0x00  028         st  [$r2], $r3        # D[addr] = tmp
0x00  029         add $one,  $r0        # start++
0x00  030         cmp $r0,   $r1        # if (start < end)
0x00  031         blt CNTLP             #   goto CNTLP
0x00  032 CNTLPEND:

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
0x00  033         set3                  # tmp = 3
0x00  034         mov $r1,  $r3         # addr = tmp
0x00  035         ld  $r0,  [$r1]       # hcnt = D[addr]

0x00  036 D4CMP:  add $one, $r3         # tmp++
0x00  037         ld  $r2,  [$r3]       # currcnt = D[tmp]
0x00  038         cmp $r0,  $r2         # if (hcnt >= currcnt)
0x00  039         bge D5CMP             #   goto D5CMP
0x00  040         add $one, $r1         # addr++
0x00  041         mov $r0,  $r2         # hcnt = currcnt

0x00  042 D5CMP:  add $one, $r3         # tmp++
0x00  043         ld  $r2,  $[r3]       # currcnt = D[tmp]
0x00  044         cmp $r0,  $r2         # if (hcnt >= currcnt)
0x00  045         bge D6CMP             #   goto D6CMP
0x00  046         add $one, $r1         # addr++
0x00  047         mov $r0,  $r2         # hcnt = currcnt

0x00  048 D6CMP:  add $one, $r3         # tmp++
0x00  049         ld  $r2,  $[r3]       # currcnt = D[tmp]
0x00  050         cmp $r0,  $r2         # if (hcnt >= currcnt)
0x00  051         bge D7CMP             #   goto D7CMP
0x00  052         add $one, $r1         # addr++
0x00  053         mov $r0,  $r2         # hcnt = currcnt

0x00  054 D7CMP:  add $one, $r3         # tmp++
0x00  055         ld  $r2,  $[r3]       # currcnt = D[tmp]
0x00  056         cmp $r0,  $r2         # if (hcnt >= currcnt)
0x00  057         bge D8CMP             #   goto D8CMP
0x00  058         add $one, $r1         # addr++
0x00  059         mov $r0,  $r2         # hcnt = currcnt

0x00  060 D8CMP:  add $one, $r3         # tmp++
0x00  061         ld  $r2,  $[r3]       # currcnt = D[tmp]
0x00  062         cmp $r0,  $r2         # if (hcnt >= currcnt)
0x00  063         bge D9CMP             #   goto D9CMP
0x00  064         add $one, $r1         # addr++
0x00  065         mov $r0,  $r2         # hcnt = currcnt

0x00  066 D9CMP:  add $one, $r3         # tmp++
0x00  067         ld  $r2,  $[r3]       # currcnt = D[tmp]
0x00  068         cmp $r0,  $r2         # if (hcnt >= currcnt)
0x00  069         bge D10CMP            #   goto D10CMP
0x00  070         add $one, $r1         # addr++
0x00  071         mov $r0,  $r2         # hcnt = currcnt

0x00  072 D10CMP: add $one, $r3         # tmp++
0x00  073         ld  $r2,  $[r3]       # currcnt = D[tmp]
0x00  074         cmp $r0,  $r2         # if (hcnt >= currcnt)
0x00  075         bge FINISH            #   goto FINISH
0x00  076         add $one, $r1         # addr++
0x00  077         mov $r0,  $r2         # hcnt = currcnt

0x00  078 FINISH: mov $one,  $r1        # addr = 1
0x00  079         add $r1,   $one       # addr++
0x00  080         st  [$r1], $r0        # D[addr] = hcnt




