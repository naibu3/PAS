#!/bin/bash

#Desarrolla un script que permita realizar una copia de seguridad de un determinado
#directorio y almacenarla en un fichero comprimido. El programa deber ́a recibir dos
#argumentos:
#    1. Directorio que se va a copiar.
#    2. Directorio donde se almacenara la copia comprimida.
#El nombre del fichero de copia resultante debera seguir el formato:
#    nombredirectoriooriginal_usuario_fecha.tar.gz,
#donde usuario, es el nombre del usuario que esta realizando la copia y fecha es
#la fecha en segundos desde el 1 de enero de 1970.
#Si el directorio de destino de la copia no existe, deberas crearlo.
#Al invocar al script, todos los ficheros de la carpeta donde se guarden los tar con
#una antiguedad mayor a 200 segundos, deberan ser borrados Ademas, deberas realizar
#los controles de errores que estimes oportunos.

if [ $# -lt 2 ]; then

    echo "[!] Error formato incorrecto. Uso:"
    echo "./ejercicio3.sh <dir_a_copiar> <dir_destino>"
    exit
fi

if [ ! -d $1 ]; then

    echo "No existe el directorio a copiar."
    exit
fi

if [ ! -d $2 ]; then

    echo "No existe el directorio donde se almacenará la copia, creandolo..."
    mkdir $2
fi


#Eliminar archivos de más de hace 20 segundos
echo "Se eliminarán los siguientes archivos:"
for fich in $(find -mmin +0,3333); do

    echo $fich
    #rm $fich
done

#Comprimir archivo
nombre="$1_$USER_$(date -u +%s)"
tar -czvf "$nombre.tar.gz" $1