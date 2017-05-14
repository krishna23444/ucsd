SRCS := $(wildcard *.v)
OBJS := $(SRCS:*.v=work/*)

all: $(OBJS)

clean:
	rm -rf $(OBJS)

new: clean alL

work/%: %.v
	vlog @<
