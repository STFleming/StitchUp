
%.pdf: %.tex
	pdflatex $<
	bibtex date2016
	pdflatex $<
	pdflatex $<

clean:
	rm -f *.bbl *.aux* *.toc *.log *.lof *.log *.blg *.lot *.pdf
