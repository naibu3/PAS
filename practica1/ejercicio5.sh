#!/bin/bash

#Crear un script que reciba como argumento un parametro que sera un directorio y un
#segundo argumento que sera un numero (numero en bytes). El script debe buscar todos
#los ficheros que esten alojados en la carpeta que se pasa como primer argumento cuyo
#tamano total en bytes sea mayor o igual que el numero pasado como segundo argumento.
#Para cada fichero debera mostrar:
#   1. El nombre del fichero sin la ruta
#   2. La fechar de creacion (legible)
#   3. Tamano en bytes
#   4. La cadena de permisos del fichero
#   5. Un 1 si el fichero es ejecutable y un 0 sino lo es

#TODO-> SE DEBEN MOSTRAR LOS ARCHIVOS POR ORDEN DE TAMAÑO

if [ $# -lt 2 ]; then
    echo "[!] More arguments expected!"
    echo "Usage: ./ejercicio5.sh <directory> <number>"
fi

directory=$1
num=$2

cd $directory

echo "NOMBRE F_CREAC TAMANIO PERMISOS EXECUTABLE"

for fich in *; do

    if [ -d $fich ]; then continue; fi

    tam=$(wc -c $fich | cut -d' ' -f1)

    if [ $tam -ge $num ]; then continue; fi

    creac=$(stat $fich --printf %w)
    perm=$(stat $fich --printf "%a %A")

    if [ -x $fich ]; then
        ejecut=1
    else
        ejecut=0
    fi

    echo "$(basename $fich) $creac $tam $perm $ejecut"
    
done