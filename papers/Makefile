
%.pdf: %.tex
	pdflatex $<
	#bibtex $<
	pdflatex $<
	pdflatex $<

clean:
	rm -f *.bbl *.aux* *.toc *.log *.lof *.log *.blg *.lot
