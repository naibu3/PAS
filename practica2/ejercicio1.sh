#!/bin/bash

#Crear un script que ejecute los comandos adecuados de grep que permitan realizar
#las siguientes tareas sobre el fichero de ejemplo peliculas.txt:

#   1. Mostrar aquellas lineas que contienen la duracion de la pelicula (uno o mas 
#       digitos al principio de la linea, un espacio y la secuencia “min”).
#   2. Mostrar los paises de las peliculas (sin mostrar la linea completa que contiene
#       dicha informacion). Suponemos que siempre aparecera rodeado de guiones,
#       p.ej: “-Estados Unidos-”.
#   3. Contar cuantas peliculas son del 2016 y cuantas del 2017.
#   4. Mostrar aquellas palabras que contengan la letra “d”,“l” o “t”, una vocal y
#       la misma letra (por ejemplo, “Universidad”, “expectativas”, “dedicarse”,etc).
#   5. Todas aquellas lineas que terminen con 3 puntos (“...”) y no empiecen por espacio,
#       utilizando el operador de repeticion {}.
#   6. Utilizar sed para mostrar entre comillas dobles las vocales con tilde (mayusculas
#       o minusculas).

echo "1) Linea con la duracion de las peliculas:"
grep -E '[0-9]+hr\ [0-9]+min' peliculas.txt

echo "2) Paises de las películas:"
grep -o '\-.*\-' peliculas.txt

echo "3) Hay $(grep -E -c '[0-9]+\/[0-9]+\/2016' peliculas.txt) peliculas del 2006 y $(grep -E -c '[0-9]+\/[0-9]+\/2017' peliculas.txt) del 2007."

echo "4) Palabras con 'd', 'l', o 't', una vocal y la misma letra:"
grep -E -o '[A-Za-z]*([dlt])[aeiou]\1[A-Za-z]*' peliculas.txt

echo "5) Lineas que terminen con 3 puntos (“...”) y no empiecen por espacio:"
grep -E '^[^ ].+\.{3}$' peliculas.txt

echo "6) Vocales con tilde:"
sed -e 's/[áéíóúÁÉÍÓÚ]/" "/g' peliculas.txt