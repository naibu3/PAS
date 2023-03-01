#!/bin/bash
# Script que genera un directorio con ficheros y subdirectorios.
#El script recibir ́a 4 argumentos:
#   1. Ruta del nuevo directorio que se va a crear.
#   2. Numero de subdirectorios que se crearan dentro del directorio principal.
#   3. Longitud minima de los nombres de los ficheros (sin extension) y subdirectorios.
#   4. Longitud maxima de los nombres de los ficheros (sin extension) y subdirectorios.
#Al ejecutarlo, debera crear un directorio principal en la ruta que se haya especificado
#en el primer parametro. Dentro de ese directorio, se deberan crear N subdirectorios
#(indicado por el segundo parametro) con nombres aleatorios y de longitud aleatoria
#entre el mınimo y el maximo especificados como argumentos. Por  ́ultimo, dentro de cada
#uno de estos subdirectorios, se crearan 4 ficheros (vacios) con nombres aleatorios y
#las extensiones .sh, .html, .key y .txt.


path=$1
subd_n=$2
min_len=$3
max_len=$4

# Recibe un minimo de caracteres y un maximo y genera un nombre aleatorio
function rand_name(){
    len=$[$1+$[$RANDOM%$[$2+1-$1]]] #LONGITUD ALEATORIA
    echo $(tr -dc A-Za-z0-9 < /dev/urandom | head -c $len); #NOMBRE ALEATORIO
}

if [ -e $path ]; then     # ENSURE DOESNT EXISTS

    option="n"
    read -t5 -p "[!] La ruta especificada ya existe, desea eliminarlo (y/n): " option

    if [ $option == "y" ]; then
        echo "Eliminando el directorio..."
        rm $path -r
    fi
fi

mkdir $path
cd $path

if [ $subd_n -gt 0 ]; then

    for i in $(seq $subd_n); do
        name=$(rand_name $min_len $max_len)
        mkdir $name
        cd $name
        touch "$(rand_name $min_len $max_len).key"
        touch "$(rand_name $min_len $max_len).sh"
        touch "$(rand_name $min_len $max_len).txt"
        touch "$(rand_name $min_len $max_len).html"
        cd ..
    done
fi
