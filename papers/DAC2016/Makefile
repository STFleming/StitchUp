
all: StitchUp_dac2016.pdf

%.pdf: %.tex
	pdflatex $<
	bibtex StitchUp_dac2016
	pdflatex $<
	pdflatex $<

clean:
	rm -f *.bbl *.aux* *.toc *.log *.lof *.log *.blg *.lot *.pdf
