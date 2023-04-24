/*Enviara cadenas leidas desde teclado y las envia mediante
mensajes a un proceso servidor cada vez que pulsamos INTRO*/


#include "common.h"
#include <errno.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

// Prototipo de funcion
void funcionLog(char *);
// Apuntador al fichero de log
FILE *fLog = NULL;

int main(int argc, char **argv) {

    char msgbuf[100];   //Para mensajes de log

    //CREACIóN DE LAS COLAS____________________________________________________________
    mqd_t mq_msg;  // Cola de envío (A)
    mqd_t mq_num;  // Cola de numeros (B)

    char buffer[MAX_SIZE];  // Buffer para intercambiar mensajes
    char msgQueue[100];  // Nombre para la cola A
    char numQueue[100];  // Nombre para la cola B

    // Abrir la cola del servidor. La cola CLIENT_QUEUE le servira en ejercicio resumen.
    // No es necesario crearla si se lanza primero el servidor, sino el programa no funciona.

    // Nombre para la cola. Al concatenar el login sera unica en un sistema compartido.
    sprintf(msgQueue, "%s-%s", MSG_QUEUE, getenv("USER"));
    printf("[Cliente]: El nombre de la cola A es: %s\n", msgQueue);
    //LOG
    sprintf(msgbuf,"[Cliente]: El nombre de la cola A es: %s\n", msgQueue);
    funcionLog(msgbuf);

    sprintf(numQueue, "%s-%s", NUM_QUEUE, getenv("USER"));
    printf("[Cliente]: El nombre de la cola B es: %s\n", numQueue);
    //LOG
    sprintf(msgbuf,"[Cliente]: El nombre de la cola B es: %s\n", numQueue);
    funcionLog(msgbuf);

    mq_msg = mq_open(msgQueue, O_WRONLY);
    mq_num = mq_open(numQueue, O_RDONLY);

    if (mq_msg == (mqd_t)-1) {
        perror("Error al abrir la cola A");
        funcionLog("Error al abrir la cola A");   //LOG
        exit(-1);
    }
    if (mq_num == (mqd_t)-1) {
        perror("Error al abrir la cola B");
        funcionLog("Error al abrir la cola B");   //LOG
        exit(-1);
    }

    printf("[Cliente]: El descriptor de la cola A es: %d\n", (int)mq_msg);
    //LOG
    sprintf(msgbuf,"[Cliente]: El descriptor de la cola A es: %d\n", (int)mq_msg);
    funcionLog(msgbuf);

    printf("[Cliente]: El descriptor de la cola B es: %d\n", (int)mq_num);
    //LOG
    sprintf(msgbuf,"[Cliente]: El descriptor de la cola B es: %d\n", (int)mq_num);
    funcionLog(msgbuf);

    //EJECUCIóN PPAL___________________________________________________________________
    //printf("Mandando mensajes al servidor (escribir \"%s\" para parar):\n", MSG_STOP);
    do {
        //ENVIO___________________________________
        printf("> ");

        /* Leer por teclado. Según la documentación, fgets lo hace de esta manera:
        It stops when either (n-1) characters are read, the newline character is read,
        or the end-of-file is reached, whichever comes first.
        Automáticamente fgets inserta el fin de cadena '\0'
        */
        fgets(buffer, MAX_SIZE, stdin);

        // Enviar y comprobar si el mensaje se manda
        if (mq_send(mq_msg, buffer, MAX_SIZE, 0) != 0) {
            perror("[Cliente] Error al enviar el mensaje");
            funcionLog("[Cliente] Error al enviar el mensaje"); //LOG
            exit(-1);
        }

        //RECEPCION________________________________
        ssize_t bytes_read; // Número de bytes leidos

        bytes_read = mq_receive(mq_num, buffer, MAX_SIZE, NULL);    // Recibir el mensaje
        
        if (bytes_read < 0) {   // Comprar que la recepción es correcta (bytes leidos no son negativos)
            perror("[CLIENTE] Error al recibir el mensaje");
            funcionLog("[Cliente] Error al enviar el mensaje"); //LOG
            exit(-1);
        }
        
        // buffer[bytes_read] = '\0';   // Cerrar la cadena
        
        printf("[CLIENTE] Recibido el mensaje: %s\n", buffer);
        //LOG
        sprintf(msgbuf,"[CLIENTE] Recibido el mensaje: %s\n", buffer);
        funcionLog(msgbuf);

    } while (strncmp(buffer, MSG_STOP, strlen(MSG_STOP)));  // Iterar hasta escribir el código de salida

    // Cerrar la cola del servidor
    if (mq_close(mq_msg) == (mqd_t)-1) {
        perror("Error al cerrar la cola A");
        funcionLog("Error al cerrar la cola A"); //LOG
        exit(-1);
    }
    if (mq_close(mq_num) == (mqd_t)-1) {
        perror("Error al cerrar la cola B");
        funcionLog("Error al cerrar la cola B"); //LOG
        exit(-1);
    }
    return 0;
}

// (Función auxiliar) Escritura de un log___________________________________________
void funcionLog(char *mensaje) {
    int resultado;
    char nombreFichero[100];
    char mensajeAEscribir[300];
    time_t t;

    // Abrir el fichero
    sprintf(nombreFichero, "log-cliente.txt");
    if (fLog == NULL) {
        fLog = fopen(nombreFichero, "at");
        if (fLog == NULL) {
            perror("Error abriendo el fichero de log");
            exit(1);
        }
    }

    // Obtener la hora actual
    t = time(NULL);
    struct tm *p = localtime(&t);
    strftime(mensajeAEscribir, 1000, "[%Y-%m-%d, %H:%M:%S]", p);

    // Vamos a incluir la hora y el mensaje que nos pasan
    sprintf(mensajeAEscribir, "%s ==> %s\n", mensajeAEscribir, mensaje);

    // Escribir finalmente en el fichero
    resultado = fputs(mensajeAEscribir, fLog);
    if (resultado < 0)
        perror("Error escribiendo en el fichero de log");

    fclose(fLog);
    fLog = NULL;
}
