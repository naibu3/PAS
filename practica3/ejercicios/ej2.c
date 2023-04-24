/*Implemente un programa en C usando tuberias similar a los existentes en los ficheros
pipe.c y pipe2.c, pero en este caso que un proceso genere dos numeros aleatorios
flotantes y envie la suma de ellos al otro proceso para que este muestre su resultado.*/

#include <errno.h> //Control de errores
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //Para la funcion strerror(), que permite describir el valor de errno como cadena.
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int main(){

    // Para realizar el fork
    pid_t rf;
    int flag, status;
    // Para controlar los valores devueltos por las funciones (control de errores)
    int resultado;
    // Lo que vamos a leer y escribir de la tubería
    float numeroAleatorio;
    // Descriptores de los dos extremos de la tubería
    int fileDes[2];
    // Iterador
    int i = 0;

    // Creamos la tubería
    resultado = pipe(fileDes);
    if (resultado == -1) {
        printf("\nERROR al crear la tubería.\n");
        exit(1);
    }

    rf = fork();
    switch (rf) {
    case -1:        //ERROR_________________________________________________________

        printf("No se ha podido crear el proceso hijo...\n");
        exit(EXIT_FAILURE);

    case 0:     //HIJO______________________________________________________________

        printf("[HIJO]: Mi PID es %d y mi PPID es %d\n", getpid(), getppid());

        close(fileDes[1]);  //Cerramos el descriptor del padre

        // Recibimos la suma a través de la cola
        resultado = read(fileDes[0], &numeroAleatorio, sizeof(float));

        if (resultado != sizeof(float)) {
            printf("\n[HIJO]: ERROR al leer de la tubería...\n");
            exit(EXIT_FAILURE);
        }

        // Imprimimos el campo que viene en la tubería
        printf("[HIJO]: Leo la suma aleatoria %f de la tubería.\n", numeroAleatorio);

        // Cerrar el extremo que he usado
        printf("[HIJO]: Tubería cerrada ...\n");
        close(fileDes[0]);
        break;

    default:    //PADRE________________________________________________________________

        printf("[PADRE]: Mi PID es %d y el PID de mi hijo es %d \n", getpid(), rf);

        close(fileDes[0]);  //Cerramos el descriptor del hijo

        //GENERACION NUMERO ALEAT
        srand(time(NULL)); // Semilla números aleatorios
        /* Genera un float aleatorio entre 0 y 1
        float numeroAleatorio = (float)rand() / RAND_MAX;*/

        // Genera un float aleatorio en un rango específico
        float a = 1.0;  // Límite inferior del rango
        float b = 10.0; // Límite superior del rango
        float numeroAleatorio = a + ((float)rand() / RAND_MAX) * (b - a);
        
        printf("[PADRE]: Escribo el número aleatorio %f en la tubería...\n", numeroAleatorio);

        // Mandamos el mensaje
        resultado = write(fileDes[1], &numeroAleatorio, sizeof(float));

        if (resultado != sizeof(float)) {
            printf("\n[PADRE]: ERROR al escribir en la tubería...\n");
            exit(EXIT_FAILURE);
        }

        // Cerrar el extremo que he usado
        close(fileDes[1]);
        printf("[PADRE]: Tubería cerrada...\n");

        /*Espera del padre a los hijos*/
        while ((flag = wait(&status)) > 0) {
            if (WIFEXITED(status)) {
                printf("Proceso Padre, Hijo con PID %ld finalizado, status = %d\n", (long int)flag, WEXITSTATUS(status));
            } else if (WIFSIGNALED(status)) { // Para seniales como las de finalizar o matar
                printf("Proceso Padre, Hijo con PID %ld finalizado al recibir la señal %d\n", (long int)flag, WTERMSIG(status));
            }
        }
        if (flag == (pid_t)-1 && errno == ECHILD) {
            printf("Proceso Padre %d, no hay mas hijos que esperar. Valor de errno = %d, definido como: %s\n", getpid(), errno, strerror(errno));
        } else {
            printf("Error en la invocacion de wait o waitpid. Valor de errno = %d, definido como: %s\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
    exit(EXIT_SUCCESS);
}
