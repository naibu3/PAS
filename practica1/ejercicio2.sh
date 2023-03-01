#!/bin/bash

#Desarrolla un script que permita configurar los permisos de los ficheros y
#subdirectorios de una determinada carpeta de la siguiente forma:
#    -El directorio y todos los subdirectorios deberan tener todos los
#    permisos para el usuario, lectura y ejecucion para el grupo y ninguno
#    para otros.
#    -Los archivos cuya extension sea .sh deberan recibir permisos de
#    ejecucion para el usuario.
#    -Los ficheros con extension .key deberan asegurarse, restringiendo los
#    permisos de manera que solo el usuario propietario pueda acceder a ellos.
#Ademas, al finalizar debe mostrar una lista ordenada alfabeticamente de
#los usuarios que hay logeados en el sistema en el instante en el que se
#ejecuta el script, eliminando los usuarios repetidos (Consulta los comandos
#who y uniq)

if [ $# -ne 1 ]; then

    echo "Numero de argumentos incorrectos. Uso: ./ejercicio2.sh <ruta_directorio>"
    exit
fi

parent=$1

echo "Cambiando permisos de la carpeta padre..."
chmod 777 $parent

function subdirmod(){

    cd $parent
    for child in $(ls); do

        if [ -d $child ]; then
            
            echo "Actualizando permisos del directorio $child"
            parent=$child
            subdirmod
        fi
    done

    for fich in $(find -maxdepth 1 -name '*.sh'); do
        echo "Actualizando permisos de $fich"
        chmod 000 $fich
        chmod u+x $fich
    done

    for fich in $(find -maxdepth 1 -name '*.key'); do
        echo "Actualizando permisos de $fich"
        chmod 000 $fich
        chmod o+rwx $fich
    done

    cd ..
}

subdirmod

echo "$(who | uniq)"