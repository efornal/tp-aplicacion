MAIN=../$(file)
SRC=$(file).cpp
SRCH=$(file).h
CXX=g++
CFLAGS= -O2 -lm -lpthread -lX11 -lfftw3 -lstdc++ -ggdb  ../lib/gnuplot_i.o
CFLAGS_DBG= -dbg -lm -lpthread -lX11 -lfftw3
BIBLIOTECAS=-I../lib/

all:
	${CXX} ${OPCIONES} ${SRC} -o ${MAIN} ${CFLAGS} ${BIBLIOTECAS}

debug:
	${CXX} ${OPCIONES} ${SRC} -o ${MAIN} ${CFLAGS_DBG} ${BIBLIOTECAS}
