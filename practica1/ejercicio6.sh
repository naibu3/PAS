#!/bin/bash
#Desarrolla un script que simule la creacion de nuevos usuarios. La gestion de
#usuarios es una tarea muy comun para un administrador de sistemas. Sin embargo,
#en los servidores de la universidad no tenemos la posibilidad de crear o eliminar
#usuarios. Por ello, en este ejercicio se pretende crear un sistema de usuarios,
#muy sencillo, que simule el sistema utilizado en Linux.
#El sistema constara de un fichero (por ejemplo users.txt) que almacenara los usuarios
#existentes. Por otro lado, cada usuario tendra su home dentro de un directorio (por
#ejemplo dentro de ./homes). Ademas, habra un directorio que contendra los ficheros
#por defecto que se aniaden al home de un usuario al crearlo (por ejemplo ./skel).
#Dentro de nuestro script deberemos implementar una funcion crear usuario que se
#encargue de aniadir un nuevo usuario al sistema con el nombre indicado en su primer
#argumento. Al crearlo, lo aniadira al fichero de texto, le creara su home y metera
#los archivos por defecto que se encuentren en el directorio skel. Si se intenta crear
#un usuario que ya existe, no debera volver a crearlo.
#Una vez creada dicha funcion, el script debera llamarla utilizando como nombre el
#primer argumento con el que se invoque el script. Recuerda realizar los controles de
#errores oportunos.

function crear_usuario(){

    for user in users.txt; do
        if [ $1 == $user ]; then
            echo "User already exists"
            exit
        fi
    done
    
    echo $1 >> users.txt
    cd home
    mkdir $1
}

if [ $# -lt 1 ]; then
    echo "[!] Not enough arguments!"
    echo "Usage <username>"
    exit
fi

#CREAR DIRECTORIOS POR DEFECTO
if [ -e "/home" ]; then mkdir home; fi
if [ -e "users.txt" ]; then echo "root" > users.txt; fi

crear_usuario $1