salida = presentacion
fuente = main
includes = conf/preconfig.tex conf/config.tex conf/packages.tex
basura = conf/preconfig.aux conf/config.aux conf/packages.aux $(fuente).aux $(fuente).log
basurabeamer = beamerconf.aux $(fuente).nav $(fuente).toc $(fuente).snm $(fuente).out

cc = latex
bib = bibtex
ccpdf = pdflatex
visor = evince

all : beamer
#	$(visor) $(salida).pdf

pdf : bibtex
	dvipdf $(out)
	rm $(salida).dvi

dvi : $(fuente).tex $(includes)
	$(cc) $(fuente).tex
	mv $(fuente).dvi $(salida).dvi

pdflatex : $(fuente).tex $(includes)
	$(ccpdf) $(fuente).tex
	mv $(fuente).pdf $(salida).pdf
	rm $(basura)

bibtex : dvi
	$(bib) $(fuente)
	$(cc) $(fuente).tex
	$(cc) $(fuente).tex
	mv $(fuente).dvi $(salida).dvi
	rm $(basura)

# para cuando hago make clean
clean:
	rm -f $(salida).pdf $(salida).dvi

beamer: pdflatex
	rm $(basurabeamer)