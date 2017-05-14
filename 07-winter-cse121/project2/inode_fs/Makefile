CC = gcc
CFLAGS = -D_FILE_OFFSET_BITS=64 -DFUSE_USE_VERSION=25 \
				 -I/software/nonrdist/fuse/include 
LDFLAGS = -L/software/nonrdist/fuse/lib -Wl,-rpath=/software/nonrdist/fuse/lib \
					-lfuse -lm

BENCHMARK_SRC = ../benchmark/benchmark.c

MKFS = mkinodefs
FS = inodefs
PROGS = $(MKFS) $(FS)

all: $(PROGS)

$(MKFS): mkfs.c $(BENCHMARK_SRC)
	$(CC) $^ -lm -o $@

$(FS): Inode.c $(BENCHMARK_SRC)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

clean:	
	rm -f $(PROGS) *.o

new: clean all

bench: all
