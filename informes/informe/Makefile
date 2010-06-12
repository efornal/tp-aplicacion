# el nombre de archivo de salida, sin extensión:
out = tp
# nombre de los archivos fuente, sin/con extensión:
fuente = main
dep = conf/preconfig.tex conf/config.tex conf/packages.tex
# archivos auxiliares
aux = conf/preconfig.aux conf/config.aux conf/packages.aux
# 'compilador':
cc = latex
# BibTeX
bib = bibtex
# 'compilador' de pdf:
ccpdf = pdflatex
#-interaction=nonstopmode
# 'visor de pdf':
visor = evince

# todo, incluido el borrado de archivos auxiliares
all : pdf
	$(visor) $(out).pdf

# convierte de dvi a pdf:
pdf : bibtex
	dvipdf $(out)
	rm $(out).dvi

# archivo intermedio:
dvi : $(fuente).tex $(dep)
	$(cc) $(fuente).tex
	mv $(fuente).dvi $(out).dvi
#	rm $(fuente).aux $(aux)
#	rm $(fuente).log

# pdflatex:
pdflatex : $(fuente).tex $(dep)
	$(ccpdf) $(fuente).tex
	mv $(fuente).pdf $(out).pdf
	rm $(fuente).aux $(aux)
	rm $(fuente).log

bibtex : dvi
	$(bib) $(fuente)
	$(cc) $(fuente).tex
	$(cc) $(fuente).tex
	mv $(fuente).dvi $(out).dvi
	rm $(fuente).aux $(aux)
	rm $(fuente).log

# para cuando hago make clean
clean:
	rm -f $(out).pdf $(out).dvi $(fuente).log

