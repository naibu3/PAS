#!/bin/bash

#Crea un script que muestre la siguiente informacion obtenida a partir del
#fichero /etc/group:
#   1. Grupos que contengan al menos 1 usuario ademas del usuario principal del grupo.
#   2. Grupos cuyo nombre empiece por u y acabe en s.
#   3. Grupos cuyo nombre contenga dos letras iguales seguidas.
#   4. Grupos que no contengan ningun usuario adicional.
#   5. Grupos con GID menor que 100.

echo "1) Grupos que contengan al menos 1 usuario ademas del usuario principal del grupo"
grep -E -v -e '.*:.*:.*:[A-Za-z]*$' /etc/group

echo "2) Grupos cuyo nombre empiece por u y acabe en s"
grep -E -e '^u.*s:' /etc/group

echo "3) Grupos cuyo nombre contenga dos letras iguales seguidas"
grep -E -e '^.*([A-Za-z])\1.*:' /etc/group

echo "4) Grupos que no contengan ningun usuario adicional"
grep -E -e '.*:.*:.*:[A-Za-z]*$' /etc/group

echo "5) Grupos con GID menor que 100"
grep -E -e '^.*:.*:[0-9]{1,2}:' /etc/group