MAIN=../$(file)
SRC=$(file).cpp
SRCH=$(file).h
CXX=g++
CFLAGS= -O2 -lm -lpthread -lX11 -lfftw3
CFLAGS_DBG= -dbg -lm -lpthread -lX11 -lfftw3
BIBLIOTECAS=-I../lib/

debug:
	${CXX} ${OPCIONES} ${SRC} -o ${MAIN} ${CFLAGS_DBG} ${BIBLIOTECAS}

all:
	${CXX} ${OPCIONES} ${SRC} -o ${MAIN} ${CFLAGS} ${BIBLIOTECAS}

