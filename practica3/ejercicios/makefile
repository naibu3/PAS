all: ej1 ej2 servidor cliente

ej1: ej1.c
	gcc -o $@ $^

ej2: ej2.c
	gcc -o $@ $^

servidor: servidor.c
	gcc -o $@ $^ -lrt

cliente: cliente.c
	gcc -o $@ $^ -lrt