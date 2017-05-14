# use normal make for this Makefile
#
# Makefile for building user programs to run on top of Nachos
#
# Several things to be aware of:
#
#    Nachos assumes that the location of the program startup routine (the
# 	location the kernel jumps to when the program initially starts up)
#       is at location 0.  This means: start.o must be the first .o passed 
# 	to ld, in order for the routine "Start" to be loaded at location 0
#

# if you are cross-compiling, you need to point to the right executables
# and change the flags to ld and the build procedure for as

# UCSD OSTL (Linux)
# GCCDIR = /home/ostl/cs120f/public/mips-x86.linux-xgcc/
# UCSD uAPE (Sparc/Solaris)
# GCCDIR = /home/solaris/ieng9/cs120f/public/mips-sparc.solaris-xgcc/
GCCDIR = $(CSE120_GCC)

LDFLAGS = -T script -N
ASFLAGS = -mips2
CPPFLAGS = $(INCDIR)

# if you aren't cross-compiling:
# GCCDIR =
# LDFLAGS = -N -T 0
# ASFLAGS =
# CPPFLAGS = -P $(INCDIR)

PATH = $(GCCDIR):/lib:/usr/bin:/bin

CC = $(GCCDIR)gcc
AS = $(GCCDIR)as
LD = $(GCCDIR)ld

CPP = gcc -E
INCDIR =-I../userprog -I../threads
CFLAGS = -G 0 -c $(INCDIR)

all: halt shell  address_error matmult echo sort \
     jointest jointest2 jointest3 jointest4 jointest4_progB jointest5 join6 \
     join6_b \
     exec1 exec2 exec3 exec5 exec6 exec7 \
     args1 args1_b args2 args2_b \
     exit1 exit2 exit3 exit4 \
     pipe1 pipe1_produce pipe1_consume pipe2 pipe3 \
     pipe4 pipe4_produce pipe4_dual pipe4_consume \
     pipe5 pipe5_produce pipe5_consume \
     pipe6 pipe6_produce pipe6_consume pipe7 \
     vm1 vm2 vm3 vm4 vm5 vm6 \
     snake

start.o: start.s ../userprog/syscall.h
	$(CPP) $(CPPFLAGS) start.c > strt.s
	$(AS) $(ASFLAGS) -o start.o strt.s
	rm strt.s

halt.o: halt.c
	$(CC) $(CFLAGS) -c halt.c
halt: halt.o start.o
	$(LD) $(LDFLAGS) start.o halt.o -o halt.coff
	../bin/coff2noff halt.coff halt

shell.o: shell.c
	$(CC) $(CFLAGS) -c shell.c
shell: shell.o start.o
	$(LD) $(LDFLAGS) start.o shell.o -o shell.coff
	../bin/coff2noff shell.coff shell

sort.o: sort.c
	$(CC) $(CFLAGS) -c sort.c
sort: sort.o start.o
	$(LD) $(LDFLAGS) start.o sort.o -o sort.coff
	../bin/coff2noff sort.coff sort

matmult.o: matmult.c
	$(CC) $(CFLAGS) -c matmult.c
matmult: matmult.o start.o
	$(LD) $(LDFLAGS) start.o matmult.o -o matmult.coff
	../bin/coff2noff matmult.coff matmult
	
snake.o: snake.c
	$(CC) $(CFLAGS) -c snake.c
snake: snake.o start.o
	$(LD) $(LDFLAGS) start.o snake.o -o snake.coff
	../bin/coff2noff snake.coff snake

address_error.o: address_error.c
	$(CC) $(CFLAGS) -c address_error.c
address_error: address_error.o start.o
	$(LD) $(LDFLAGS) start.o address_error.o -o address_error.coff
	../bin/coff2noff address_error.coff address_error

exec7.o: exec7.c
	$(CC) $(CFLAGS) -c exec7.c
exec7: exec7.o start.o
	$(LD) $(LDFLAGS) start.o exec7.o -o exec7.coff
	../bin/coff2noff exec7.coff exec7

echo.o: echo.c
	$(CC) $(CFLAGS) -c echo.c
echo: echo.o start.o
	$(LD) $(LDFLAGS) start.o echo.o -o echo.coff
	../bin/coff2noff echo.coff echo
	
args2_b.o: args2_b.c
	$(CC) $(CFLAGS) -c args2_b.c
args2_b: args2_b.o start.o
	$(LD) $(LDFLAGS) start.o args2_b.o -o args2_b.coff
	../bin/coff2noff args2_b.coff args2_b

args2.o: args2.c
	$(CC) $(CFLAGS) -c args2.c
args2: args2.o start.o
	$(LD) $(LDFLAGS) start.o args2.o -o args2.coff
	../bin/coff2noff args2.coff args2
	
jointest.o: jointest.c
	$(CC) $(CFLAGS) -c jointest.c
jointest: jointest.o start.o
	$(LD) $(LDFLAGS) start.o jointest.o -o jointest.coff
	../bin/coff2noff jointest.coff jointest
	
jointest2.o: jointest2.c
	$(CC) $(CFLAGS) -c jointest2.c
jointest2: jointest2.o start.o
	$(LD) $(LDFLAGS) start.o jointest2.o -o jointest2.coff
	../bin/coff2noff jointest2.coff jointest2
	
jointest3.o: jointest3.c
	$(CC) $(CFLAGS) -c jointest3.c
jointest3: jointest3.o start.o
	$(LD) $(LDFLAGS) start.o jointest3.o -o jointest3.coff
	../bin/coff2noff jointest3.coff jointest3

jointest4.o: jointest4.c
	$(CC) $(CFLAGS) -c jointest4.c
jointest4: jointest4.o start.o
	$(LD) $(LDFLAGS) start.o jointest4.o -o jointest4.coff
	../bin/coff2noff jointest4.coff jointest4

jointest4_progB.o: jointest4_progB.c
	$(CC) $(CFLAGS) -c jointest4_progB.c
jointest4_progB: jointest4_progB.o start.o
	$(LD) $(LDFLAGS) start.o jointest4_progB.o -o jointest4_progB.coff
	../bin/coff2noff jointest4_progB.coff jointest4_progB

jointest5.o: jointest5.c
	$(CC) $(CFLAGS) -c jointest5.c
jointest5: jointest5.o start.o
	$(LD) $(LDFLAGS) start.o jointest5.o -o jointest5.coff
	../bin/coff2noff jointest5.coff jointest5
	
exec1.o: exec1.c
	$(CC) $(CFLAGS) -c exec1.c
exec1: exec1.o start.o
	$(LD) $(LDFLAGS) start.o exec1.o -o exec1.coff
	../bin/coff2noff exec1.coff exec1

exec2.o: exec2.c
	$(CC) $(CFLAGS) -c exec2.c
exec2: exec2.o start.o
	$(LD) $(LDFLAGS) start.o exec2.o -o exec2.coff
	../bin/coff2noff exec2.coff exec2

exec3.o: exec3.c
	$(CC) $(CFLAGS) -c exec3.c
exec3: exec3.o start.o
	$(LD) $(LDFLAGS) start.o exec3.o -o exec3.coff
	../bin/coff2noff exec3.coff exec3

vm1.o: vm1.c
	$(CC) $(CFLAGS) -c vm1.c
vm1: vm1.o start.o
	$(LD) $(LDFLAGS) start.o vm1.o -o vm1.coff
	../bin/coff2noff vm1.coff vm1

vm2.o: vm2.c
	$(CC) $(CFLAGS) -c vm2.c
vm2: vm2.o start.o
	$(LD) $(LDFLAGS) start.o vm2.o -o vm2.coff
	../bin/coff2noff vm2.coff vm2

vm3.o: vm3.c
	$(CC) $(CFLAGS) -c vm3.c
vm3: vm3.o start.o
	$(LD) $(LDFLAGS) start.o vm3.o -o vm3.coff
	../bin/coff2noff vm3.coff vm3

vm4.o: vm4.c
	$(CC) $(CFLAGS) -c vm4.c
vm4: vm4.o start.o
	$(LD) $(LDFLAGS) start.o vm4.o -o vm4.coff
	../bin/coff2noff vm4.coff vm4

vm5.o: vm5.c
	$(CC) $(CFLAGS) -c vm5.c
vm5: vm5.o start.o
	$(LD) $(LDFLAGS) start.o vm5.o -o vm5.coff
	../bin/coff2noff vm5.coff vm5
	
vm6.o: vm6.c
	$(CC) $(CFLAGS) -c vm6.c
vm6: vm6.o start.o
	$(LD) $(LDFLAGS) start.o vm6.o -o vm6.coff
	../bin/coff2noff vm6.coff vm6

exec5.o: exec5.c
	$(CC) $(CFLAGS) -c exec5.c
exec5: exec5.o start.o
	$(LD) $(LDFLAGS) start.o exec5.o -o exec5.coff
	../bin/coff2noff exec5.coff exec5

exec6.o: exec6.c
	$(CC) $(CFLAGS) -c exec6.c
exec6: exec6.o start.o
	$(LD) $(LDFLAGS) start.o exec6.o -o exec6.coff
	../bin/coff2noff exec6.coff exec6

args1.o: args1.c
	$(CC) $(CFLAGS) -c args1.c
args1: args1.o start.o
	$(LD) $(LDFLAGS) start.o args1.o -o args1.coff
	../bin/coff2noff args1.coff args1

args1_b.o: args1_b.c
	$(CC) $(CFLAGS) -c args1_b.c
args1_b: args1_b.o start.o
	$(LD) $(LDFLAGS) start.o args1_b.o -o args1_b.coff
	../bin/coff2noff args1_b.coff args1_b

exit1.o: exit1.c
	$(CC) $(CFLAGS) -c exit1.c
exit1: exit1.o start.o
	$(LD) $(LDFLAGS) start.o exit1.o -o exit1.coff
	../bin/coff2noff exit1.coff exit1

exit2.o: exit2.c
	$(CC) $(CFLAGS) -c exit2.c
exit2: exit2.o start.o
	$(LD) $(LDFLAGS) start.o exit2.o -o exit2.coff
	../bin/coff2noff exit2.coff exit2
	
exit3.o: exit3.c
	$(CC) $(CFLAGS) -c exit3.c
exit3: exit3.o start.o
	$(LD) $(LDFLAGS) start.o exit3.o -o exit3.coff
	../bin/coff2noff exit3.coff exit3

exit4.o: exit4.c
	$(CC) $(CFLAGS) -c exit4.c
exit4: exit4.o start.o
	$(LD) $(LDFLAGS) start.o exit4.o -o exit4.coff
	../bin/coff2noff exit4.coff exit4

join6.o: join6.c
	$(CC) $(CFLAGS) -c join6.c
join6: join6.o start.o
	$(LD) $(LDFLAGS) start.o join6.o -o join6.coff
	../bin/coff2noff join6.coff join6

join6_b.o: join6_b.c
	$(CC) $(CFLAGS) -c join6_b.c
join6_b: join6_b.o start.o
	$(LD) $(LDFLAGS) start.o join6_b.o -o join6_b.coff
	../bin/coff2noff join6_b.coff join6_b

pipe1.o: pipe1.c
	$(CC) $(CFLAGS) -c pipe1.c
pipe1: pipe1.o start.o
	$(LD) $(LDFLAGS) start.o pipe1.o -o pipe1.coff
	../bin/coff2noff pipe1.coff pipe1

pipe1_produce.o: pipe1_produce.c
	$(CC) $(CFLAGS) -c pipe1_produce.c
pipe1_produce: pipe1_produce.o start.o
	$(LD) $(LDFLAGS) start.o pipe1_produce.o -o pipe1_produce.coff
	../bin/coff2noff pipe1_produce.coff pipe1_produce

pipe1_consume.o: pipe1_consume.c
	$(CC) $(CFLAGS) -c pipe1_consume.c
pipe1_consume: pipe1_consume.o start.o
	$(LD) $(LDFLAGS) start.o pipe1_consume.o -o pipe1_consume.coff
	../bin/coff2noff pipe1_consume.coff pipe1_consume

pipe2.o: pipe2.c
	$(CC) $(CFLAGS) -c pipe2.c
pipe2: pipe2.o start.o
	$(LD) $(LDFLAGS) start.o pipe2.o -o pipe2.coff
	../bin/coff2noff pipe2.coff pipe2

pipe3.o: pipe3.c
	$(CC) $(CFLAGS) -c pipe3.c
pipe3: pipe3.o start.o
	$(LD) $(LDFLAGS) start.o pipe3.o -o pipe3.coff
	../bin/coff2noff pipe3.coff pipe3

pipe4.o: pipe4.c
	$(CC) $(CFLAGS) -c pipe4.c
pipe4: pipe4.o start.o
	$(LD) $(LDFLAGS) start.o pipe4.o -o pipe4.coff
	../bin/coff2noff pipe4.coff pipe4

pipe4_produce.o: pipe4_produce.c
	$(CC) $(CFLAGS) -c pipe4_produce.c
pipe4_produce: pipe4_produce.o start.o
	$(LD) $(LDFLAGS) start.o pipe4_produce.o -o pipe4_produce.coff
	../bin/coff2noff pipe4_produce.coff pipe4_produce

pipe4_dual.o: pipe4_dual.c
	$(CC) $(CFLAGS) -c pipe4_dual.c
pipe4_dual: pipe4_dual.o start.o
	$(LD) $(LDFLAGS) start.o pipe4_dual.o -o pipe4_dual.coff
	../bin/coff2noff pipe4_dual.coff pipe4_dual

pipe4_consume.o: pipe4_consume.c
	$(CC) $(CFLAGS) -c pipe4_consume.c
pipe4_consume: pipe4_consume.o start.o
	$(LD) $(LDFLAGS) start.o pipe4_consume.o -o pipe4_consume.coff
	../bin/coff2noff pipe4_consume.coff pipe4_consume

pipe5.o: pipe5.c
	$(CC) $(CFLAGS) -c pipe5.c
pipe5: pipe5.o start.o
	$(LD) $(LDFLAGS) start.o pipe5.o -o pipe5.coff
	../bin/coff2noff pipe5.coff pipe5

pipe5_produce.o: pipe5_produce.c
	$(CC) $(CFLAGS) -c pipe5_produce.c
pipe5_produce: pipe5_produce.o start.o
	$(LD) $(LDFLAGS) start.o pipe5_produce.o -o pipe5_produce.coff
	../bin/coff2noff pipe5_produce.coff pipe5_produce

pipe5_consume.o: pipe5_consume.c
	$(CC) $(CFLAGS) -c pipe5_consume.c
pipe5_consume: pipe5_consume.o start.o
	$(LD) $(LDFLAGS) start.o pipe5_consume.o -o pipe5_consume.coff
	../bin/coff2noff pipe5_consume.coff pipe5_consume

pipe6.o: pipe6.c
	$(CC) $(CFLAGS) -c pipe6.c
pipe6: pipe6.o start.o
	$(LD) $(LDFLAGS) start.o pipe6.o -o pipe6.coff
	../bin/coff2noff pipe6.coff pipe6

pipe6_produce.o: pipe6_produce.c
	$(CC) $(CFLAGS) -c pipe6_produce.c
pipe6_produce: pipe6_produce.o start.o
	$(LD) $(LDFLAGS) start.o pipe6_produce.o -o pipe6_produce.coff
	../bin/coff2noff pipe6_produce.coff pipe6_produce

pipe6_consume.o: pipe6_consume.c
	$(CC) $(CFLAGS) -c pipe6_consume.c
pipe6_consume: pipe6_consume.o start.o
	$(LD) $(LDFLAGS) start.o pipe6_consume.o -o pipe6_consume.coff
	../bin/coff2noff pipe6_consume.coff pipe6_consume

pipe7.o: pipe7.c
	$(CC) $(CFLAGS) -c pipe7.c
pipe7: pipe7.o start.o
	$(LD) $(LDFLAGS) start.o pipe7.o -o pipe7.coff
	../bin/coff2noff pipe7.coff pipe7