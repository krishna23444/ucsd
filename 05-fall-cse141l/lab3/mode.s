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
START:  mov $r0,   $zero      # start = 0 -- addr of D[0]

        ld  $r1,   $r0        # end = a.length
        ldn $r0,   $r0        # start = D[1]

        add $r1,   $r0        # end = start + a.length

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
        mov $r2,  $zero      # addr = 0
        mov $r3,  $three     # tmp = 3
        st  $r3,  $r2        # D[tmp] = 0
        add $one, $r3        # tmp++
        st  $r3,  $r2        # D[tmp] = 0
        add $one, $r3        # tmp++
        st  $r3,  $r2        # D[tmp] = 0
        add $one, $r3        # tmp++
        st  $r3,  $r2        # D[tmp] = 0
        add $one, $r3        # tmp++
        st  $r3,  $r2        # D[tmp] = 0
        add $one, $r3        # tmp++
        st  $r3,  $r2        # D[tmp] = 0
        add $one, $r3        # tmp++
        st  $r3,  $r2        # D[tmp] = 0
        add $one, $r3        # tmp++
        st  $r3,  $r2        # D[tmp] = 0

#---------------------------------------
# Negative logic test for frequency count gathering loop
#---------------------------------------
#   r0 <-> start
#   r1 <-> end
#---------------------------------------
#   if (start >= end)
#     __SKIP_LOOP__
#---------------------------------------
        sub $r0,   $r1        # if (start >= end)
        bge CNTLPEND          #   goto CNT_LP_END

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
CNTLP:  ld  $r2,   $r0        # addr = D[start]
        mov $r3,   $three     # tmp = 3
        add $r2,   $r3        # addr += tmp
        ld  $r3,   $r2        # tmp = D[addr]
        add $one,  $r3        # tmp++
        st  $r2,   $r3        # D[addr] = tmp
        add $one,  $r0        # start++
        sub $r0,   $r1        # if (start < end)
        blt CNTLP             #   goto CNTLP

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
CNTLPEND: mov $r3,  $three    # tmp = 3
          mov $r1,  $r3       # addr = tmp
          ld  $r0,  $r1       # hcnt = D[addr]

D4CMP:  add $one, $r3         # tmp++
        ld  $r2,  $r3         # currcnt = D[tmp]
        sub $r0,  $r2         # if (hcnt >= currcnt)
        bge D5CMP             #   goto D5CMP
        mov $r1,  $r3         # addr = curraddr
        mov $r0,  $r2         # hcnt = currcnt

D5CMP:  add $one, $r3         # tmp++
        ld  $r2,  $r3         # currcnt = D[tmp]
        sub $r0,  $r2         # if (hcnt >= currcnt)
        bge D6CMP             #   goto D6CMP
        mov $r1,  $r3         # addr = curraddr
        mov $r0,  $r2         # hcnt = currcnt

D6CMP:  add $one, $r3         # tmp++
        ld  $r2,  $r3         # currcnt = D[tmp]
        sub $r0,  $r2         # if (hcnt >= currcnt)
        bge D7CMP             #   goto D7CMP
        mov $r1,  $r3         # addr = curraddr
        mov $r0,  $r2         # hcnt = currcnt

D7CMP:  add $one, $r3         # tmp++
        ld  $r2,  $r3         # currcnt = D[tmp]
        sub $r0,  $r2         # if (hcnt >= currcnt)
        bge D8CMP             #   goto D8CMP
        mov $r1,  $r3         # addr = curraddr
        mov $r0,  $r2         # hcnt = currcnt

D8CMP:  add $one, $r3         # tmp++
        ld  $r2,  $r3         # currcnt = D[tmp]
        sub $r0,  $r2         # if (hcnt >= currcnt)
        bge D9CMP             #   goto D9CMP
        mov $r1,  $r3         # addr = curraddr
        mov $r0,  $r2         # hcnt = currcnt

D9CMP:  add $one, $r3         # tmp++
        ld  $r2,  $r3         # currcnt = D[tmp]
        sub $r0,  $r2         # if (hcnt >= currcnt)
        bge D10CMP            #   goto D10CMP
        mov $r1,  $r3         # addr = curraddr
        mov $r0,  $r2         # hcnt = currcnt

D10CMP: add $one, $r3         # tmp++
        ld  $r2,  $r3         # currcnt = D[tmp]
        sub $r0,  $r2         # if (hcnt >= currcnt)
        bge FINISH            #   goto FINISH
        mov $r1,  $r3         # addr = curraddr
        mov $r0,  $r2         # hcnt = currcnt

FINISH: mov $r3,  $three      # r3 = 3
        sub $r1,  $r3         # res = addr - 3
        mov $r1,  $res        # r1 = res
        mov $r2,  $one        # r2 = 1
        add $one, $r2         # r2++
        st  $r2,  $r1         # D[2] = mode
        hlt                   # end of program

