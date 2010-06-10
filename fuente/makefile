
MAIN=../$(file)
SRC=$(file).cpp
SRCH=$(file).h
CXX=g++
CFLAGS= -O2 -lm -lpthread -lX11 -lfftw3
BIBLIOTECAS=-I../lib/

all: cpp

hache:
	${CXX} ${OPCIONES} ${SRCH} -o ${MAIN} ${CFLAGS} ${BIBLIOTECAS}

cpp:
	${CXX} ${OPCIONES} ${SRC} -o ${MAIN} ${CFLAGS} ${BIBLIOTECAS}
