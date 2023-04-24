#!/bin/bash

#Crea un script que acceda al fichero /etc/passwd y muestre:
#   1. Usuarios que pertenezcan a un grupo con GID igual a 46.
#   2. Usuarios cuyo nombre empiece y acabe con la misma letra.
#   3. Usuarios cuyo nombre no contenga la letra a/A.
#   4. Usuarios con UID de 4 dıgitos.
#   5. Usuarios con nombre entre 3 y 5 caracteres.

echo "Usuarios en un grupo con GID=46"
cat /etc/passwd | grep '.*:.*:.*:46:.*'

echo "Usuarios cuyo nombre empiece y acabe con la misma letra"
cat /etc/passwd | grep -E -e '^([A-Za-z])[A-Za-z]*\1:'

echo "Usuarios cuyo nombre no contenga la letra a/A"
cat /etc/passwd | grep -E -e '^[B-Zb-z]*:'

echo "Usuarios con UID de 4 digitos"
cat /etc/passwd | grep -E -e '^.*:.*:[0-9]{4,4}:'

echo "Usuarios con nombre entre 3 y 5 caracteres"
cat /etc/passwd | grep -E -e '^[A-Za-z]{3,5}:'