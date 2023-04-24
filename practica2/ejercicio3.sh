#!/bin/bash

#Desarrollar un script que reciba como argumento el nombre de un fichero de texto y
#extraiga cada palabra unica que aparezca en ese fichero. Cada palabra se debera
#mostrar en una linea diferente y deberan estar ordenadas por orden alfabetico.
#Ademas de la palabra, en cada linea se mostrara el numero de orden y el numero de
#veces que se repite la palabra en el fichero (sin distinguir mayusculas). Recuerda
#realizar los controles de errores oportunos.

if [ $# -lt 1 ]; then
    echo "[!]Not enough args, usage:\n./ejercicio3.sh <fichero>";
    exit;
fi

tr '[:space:]' '[\n*]' < $1 | sort | uniq -c