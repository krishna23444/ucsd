DVITOPDF  := dvipdf
LATEX     := latex
VIEW      := open


FILES     := $(patsubst %.tex,%,$(wildcard *.tex))
AUX_FILES := $(FILES:%=%.aux)
DVI_FILES := $(FILES:%=%.dvi)
LOG_FILES := $(FILES:%=%.log)
PDF_FILES := $(FILES:%=%.pdf)


.PHONY: all clean new view
all: view

clean:
	-rm -f *~ .*.swp $(AUX_FILES) $(DVI_FILES) $(LOG_FILES) $(PDF_FILES)

new: clean all

view: $(PDF_FILES)
	$(VIEW) $^

%.dvi: %.tex
	$(LATEX) $<

%.pdf: %.dvi
	$(DVITOPDF) $<
	-rm -f $(<:%.dvi=%.{aux,dvi,log})
