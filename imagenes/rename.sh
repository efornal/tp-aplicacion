#!/bin/bash
# Ojo no esta generico de todo,.. uso especifico comando git..

if [ $# -lt 2 ]; then
  echo "uso: " $0 imagenes nuevo_nombre 1
  echo "salida: " $0 imagenes/*.JPG nuevo_nombre01.jpg
  exit
fi

path=$1
nombre=$2
cambiar=$3

cont=0;

for i in $path/*.JPG ; do
    if [ $cont -lt 9 ];then
    	cont=0` expr $cont + 1 `;
    else
    	cont=` expr $cont + 1 `;
    fi

    if [ $cambiar ];then
        git mv $i $path/$nombre$cont.jpg;
    else
        echo git mv $i $path/$nombre$cont.jpg
    fi
done

