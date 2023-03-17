#!/bin/bash
#Desarrolla un script que permita listar todos los ficheros de un directorio con
#una extension concreta sin mostrar los subdirectorios pero incluyendo los ficheros
#que estos puedan contener. El script recibira dos argumentos:
#    - El directorio 
#    - Y la extension.
#El nombre del fichero debera mostrarse sin su ruta, solo incluyendo el nombre.
#Ademas, se debera anadir un numero que indicara el orden de cada fichero y tambien otro 
#numero que indicara el numero de caracteres del mismo asi como el numero de veces que
#aparece un determinado caracter que se le solicita al usuario. Si el usuario no indica
#el caracter en 5 segundos, utiliza la letra “a”.

if [ $# -lt 2 ]; then

    echo "[!] Número de argumentos insuficiente"
    echo "Usage <directory> <extension>"
    exit
fi

function list_files(){
    
    cont=$3

    for fich in *; do

        if [ -d $fich ];then
        
            #echo "cd $fich" #debug
            cd $fich
            list_files $fich $2 $cont $4
            #echo "cd .." #debug
            cd ..
        else

            extension=$(echo $fich | rev | cut -d'.' -f1 | rev)

            if [[ $extension == $2 ]]; then
                echo "$(basename $fich) $extension  $cont   $(wc -m $fich | cut -d' ' -f1)  $(cat $fich | grep -o $4 | wc -l)"
                ((cont++))
            fi
        fi
    done
}

read -t5 -r -p "-> Introduce el caracter a contar: " character

if [ -n $character ]; then character='a'; fi

#echo $character #debug

echo "NOMBRE    EXTENSION   COUNT   N_CARACTERES    N_VECES_CARACTER"

cd $1
l_dir=$(pwd)

list_files $1 $2 0 $character

cd $l_dir