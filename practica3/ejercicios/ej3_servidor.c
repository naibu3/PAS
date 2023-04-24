
#include "common.h"
#include <errno.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <signal.h>

// Prototipo de funcionn
void funcionLog(char *);
// Apuntador al fichero de log.  No se usa en este ejemplo, pero le servira en ejercicio resumen
FILE *fLog = NULL;

char msgbuf[100];   //Para mensajes de log

// flag que indica cuando hay que parar
int must_stop = 0;

void signal_handler(int sig) {
    printf("[Servidor] Se recibió la señal %d.\n", sig);
    //LOG
    sprintf(msgbuf,"[Servidor] Se recibió la señal %d.\n", sig);
    funcionLog(msgbuf);
    must_stop=1;
}

int main(int argc, char **argv) {

    //MANEJO DE SEÑALES_______________________________
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    //VARIABLES________________________________________________________________________
    int longitud;
    char long_msg[100];

    //CREACIóN DE LAS COLAS____________________________________________________________
    mqd_t mq_msg;  // Cola de envío (A)
    mqd_t mq_num;  // Cola de numeros (B)

    char buffer[MAX_SIZE];  // Buffer para intercambiar mensajes

    // Atributos de la cola (solo cuando se vayan a crear)
    struct mq_attr attr;

    // Inicializar los atributos de la cola
    attr.mq_maxmsg = 10;        // Maximo número de mensajes
    attr.mq_msgsize = MAX_SIZE; // Maximo tamaño de un mensaje
    
    char msgQueue[100];  // Nombre para la cola A
    char numQueue[100];  // Nombre para la cola B

    // Nombre para la cola. Al concatenar el login sera unica en un sistema compartido.
    sprintf(msgQueue, "%s-%s", MSG_QUEUE, getenv("USER"));
    printf("[Servidor]: El nombre de la cola A es: %s\n", msgQueue);
    //LOG
    sprintf(msgbuf,"[Servidor]: El nombre de la cola A es: %s\n", msgQueue);
    funcionLog(msgbuf);

    sprintf(numQueue, "%s-%s", NUM_QUEUE, getenv("USER"));
    printf("[Servidor]: El nombre de la cola B es: %s\n", numQueue);
    //LOG
    sprintf(msgbuf,"[Servidor]: El nombre de la cola B es: %s\n", numQueue);
    funcionLog(msgbuf);
    
    // Crear la cola de envio (A)
    mq_msg = mq_open(msgQueue, O_CREAT | O_RDONLY, 0644, &attr);
    // Crear la cola de envio (A)
    mq_num = mq_open(numQueue, O_CREAT | O_WRONLY, 0644, &attr);

    if (mq_msg == (mqd_t)-1) {
        perror("[Servidor] Error al abrir la cola A");
        funcionLog("[Servidor] Error al abrir la cola A");  //LOG
        exit(-1);
    }
    if (mq_num == (mqd_t)-1) {
        perror("[Servidor] Error al abrir la cola B");
        funcionLog("[Servidor] Error al abrir la cola B");  //LOG
        exit(-1);
    }

    printf("[Servidor]: El descriptor de la cola A es: %d\n", (int)mq_msg);
    //LOG
    sprintf(msgbuf,"[Servidor]: El descriptor de la cola A es: %d\n", (int)mq_msg);
    funcionLog(msgbuf);

    printf("[Servidor]: El descriptor de la cola B es: %d\n", (int)mq_num);
    //LOG
    sprintf(msgbuf,"[Servidor]: El descriptor de la cola B es: %d\n", (int)mq_num);
    funcionLog(msgbuf);
    
    do {
        //RECEPCION_____________________________________________
        ssize_t bytes_read; // Número de bytes leidos

        bytes_read = mq_receive(mq_msg, buffer, MAX_SIZE, NULL);    // Recibir el mensaje
        
        if (bytes_read < 0) {   // Comprar que la recepción es correcta (bytes leidos no son negativos)
            perror("[Servidor] Error al recibir el mensaje");
            funcionLog("[Servidor] Error al recibir el mensaje");   //LOG
            exit(-1);
        }
        
        // buffer[bytes_read] = '\0';   // Cerrar la cadena

        if (strncmp(buffer, MSG_STOP, strlen(MSG_STOP)) == 0){   // Comprobar el fin del bucle
            
            must_stop = 1;
        
        }else{
        
            printf("[Servidor] Recibido el mensaje: %s\n", buffer);
            //LOG
            sprintf(msgbuf,"[Servidor] Recibido el mensaje: %s\n", buffer);
            funcionLog(msgbuf);
        }
        longitud=strlen(buffer);
        sprintf(long_msg, "[Servidor] Numero de caracteres recibidos: %i", longitud-1);

        //RESPUESTA________________________________________________
        if (mq_send(mq_num, long_msg, MAX_SIZE, 0) != 0) {
            perror("[Servidor] Error al enviar el mensaje");
            funcionLog("[Servidor] Error al enviar el mensaje");   //LOG
            exit(-1);
        }

    } while (!must_stop); // Iterar hasta que llegue el código de salida

    if (mq_send(mq_num, MSG_STOP, MAX_SIZE, 0) != 0) {
        perror("[Servidor] Error al enviar el mensaje de salida");
        funcionLog("[Servidor] Error al enviar el mensaje de salida");   //LOG
        exit(-1);
    }

    // CIERRE Y ELIMINACIóN DE LAS COLAS_______________________________________________________________
    if (mq_close(mq_msg) == (mqd_t)-1) {
        perror("[Servidor] Error al cerrar la cola A");
        funcionLog("[Servidor] Error al cerrar la cola A");   //LOG
        exit(-1);
    }
    if (mq_close(mq_num) == (mqd_t)-1) {
        perror("[Servidor] Error al cerrar la cola B");
        funcionLog("[Servidor] Error al cerrar la cola B");   //LOG
        exit(-1);
    }

    // Eliminar la cola del servidor
    if (mq_unlink(msgQueue) == (mqd_t)-1) {
        perror("[Servidor] Error al eliminar la cola A");
        funcionLog("[Servidor] Error al eliminar la cola A");   //LOG
        exit(-1);
    }
    if (mq_unlink(numQueue) == (mqd_t)-1) {
        perror("[Servidor] Error al eliminar la cola B");
        funcionLog("[Servidor] Error al eliminar la cola B");   //LOG
        exit(-1);
    }

    return 0;
}

/* Función auxiliar, escritura de un log.
No se usa en este ejemplo, pero le puede servir para algun
ejercicio resumen */
void funcionLog(char *mensaje) {
    int resultado;
    char nombreFichero[100];
    char mensajeAEscribir[300];
    time_t t;

    // Abrir el fichero
    sprintf(nombreFichero, "log-servidor.txt");
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
