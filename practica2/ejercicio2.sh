#!/bin/bash

#Realiza un script que reciba como argumento un fichero de texto que contendra una serie
#de direcciones IP de servidores DNS y realizara un ping a cada uno de ellos para
#comprobar la latencia media de los mismos. Ademas, se pasaran otros dos argumentos
#que indicaran el numero de pings realizados a cada IP y el timeout.
#Al final, se debera mostrar una lista de las direcciones y el tiempo medio de respuesta
#de cada uno ordenados de forma ascendente por el tiempo. Si alguna direccion no ha
#respondido en el tiempo indicado, se debera mostrar al final de la lista. Recuerda
#realizar los controles de errores oportunos.

if [ $# -lt 3 ]; then
    echo "[!] Not enough args, usage:"
    echo "./ejercicio2 <ip_file> <ping_number> <timeout_in_seconds>"
    exit
fi

dnr=()

for i in $(cat $1); do

    result=$(ping -W $3 -c $2 $i)

    if [[ $result == '1 packets transmitted, 0 received, 100% packet loss,' ]]; then
        echo "Este no responde"
    fi
    echo "$(echo $result | grep -E -o -e '[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}' | uniq) $(ping -W 1 -c 1 8.8.8.8 | grep -E -o -e '/[0-9].*/' | cut -d'/' -f2)"
done