MAIN=$(file)
SRC=$(file).cpp
CXX=g++
CFLAGS= -O2 -lm -lpthread -lX11
BIBLIOTECAS=-I./lib/
all: 
	${CXX} ${OPCIONES} ${SRC} -o ${MAIN} ${CFLAGS} ${BIBLIOTECAS}
