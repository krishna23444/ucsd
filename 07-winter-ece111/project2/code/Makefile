DOT_FILES = $(wildcard *.dot)
PS_FILES = $(DOT_FILES:%.dot=%.ps)
PNG_FILES = $(DOT_FILES:%.dot=%.png)

.PHONY: all clean new view

all: $(PS_FILES) $(PNG_FILES) view

clean:
	rm -f $(PS_FILES)

new: clean all

view: state_machine_v4.ps
	evince $< &


%.ps: %.dot
	dot -Tps2 $< -o $@

%.pdf: %.ps
	ps2pdf $<

%.png: %.dot
	dot -Tpng $< -o $@
