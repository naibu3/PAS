/*Implemente un programa en C que utilice colas de mensajes y comunique dos procesos,
de forma que cumpla los siguientes requisitos (puede utilizar como base el codigo
de los ficheros common.h, servidor.c y cliente.c que se le han proporcionado como
ejemplo):

    1. Hay un proceso cliente que enviara cadenas leidas desde teclado y las envia mediante
mensajes a un proceso servidor cada vez que pulsamos INTRO.
    2. El servidor recibira los mensajes y contara el numero de caracteres recibidos excep-
tuando el fin de cadena (un espacio en blanco se considerara un caracter). Tras esto, el
servidor mandara un mensaje al cliente, por otra cola distinta, con la cadena “Numero
de caracteres recibidos: X”, siendo X el numero de caracteres calculados.

Por tanto habra dos colas, ambas creadas por el servidor:

    a) Una cola servira para que el cliente le envie al servidor las cadenas de texto.
De esta cola leera el servidor para obtener dichas cadenas y analizarlas para contar
el numero de caracteres que tienen.
    b) Otra cola por la que el servidor enviara al cliente el numero de caracteres calcula-
dos en la cadena de texto recibida por la primera cola.
De esta segunda cola leera el cliente para mostrar el numero de caracteres calcu-
lados que le ha enviado el servidor.

Se han de tener en cuenta los siguientes items:

    - La cola de mensajes para el texto “Numero de caracteres recibidos: X”, enviados
desde el servidor al cliente, se creara y eliminara por parte del servidor (que siem-
pre es el primero en lanzarse) y la abrira el cliente.
    - Si el servidor tiene cualquier problema en su ejecucion debera mandar el mensaje
de salida, para forzar al cliente a parar.

    3. Asegurar que el nombre de las colas sea diferente para su ejecuci on en un sistema
compartido. Puede usar la idea de anexar el login al nombre de la cola.
    4. En el codigo de que se dispone en Moodle (ficheros common.h, servidor.c y cliente.c),
tanto el cliente como el servidor tienen incluidas unas funciones de log. Estas funciones
implementan un pequeño sistema de registro o log. Utilizandolas se registran en fiche-
ros de texto los mensajes que los programas van mostrando por pantalla (log-servidor.txt
y log-cliente.txt).
Por ejemplo, si queremos registrar en el cliente un mensaje simple, har ́ıamos la siguien-
te llamada:

    funcionLog("Error al abrir la cola del servidor");

Si quisi ́eramos registrar un mensaje mas complejo (por ejemplo, donde incluimos el
mensaje recibido a trav ́es de la cola), la llamada podr ́ıa hacerse del siguiente modo:

    char msgbuf[100];
    ...
    sprintf(msgbuf,"Recibido el mensaje: %s\n", buffer);
    funcionLog(msgbuf);

Utilice estas llamadas para dejar registro en fichero de texto de todos los mensajes que
se muestren por pantalla en la ejecucion del cliente y el servidor, incluidos los errores
que se imprimen por consola.
    5. El programa cliente capturara las señales SIGTERM y SIGINT para gestionar adecua-
damente el fin del programa servidor y de el mismo. Puede asociar estas señales con
una misma funcion que pare el programa.
Dicha funcion debera, en primer lugar, registrar la señal capturada (y su numero
entero) en el fichero de log del cliente.
El cliente, antes de salir, debera mandar a la cola correspondiente, un mensaje de
fin de sesion (que debe interpretar el servidor), que hara que el otro extremo deje
de esperar mensajes. Este mensaje tambien se registrara en los logs.
Se debera cerrar, en caso de que estuvieran abiertas, aquellas colas que se esten
utilizando y el fichero de log.*/

#ifndef COMMON_H_
#define COMMON_H_

#define MSG_QUEUE "/msg_queue"
#define NUM_QUEUE "/num_queue"
#define MAX_SIZE 1024
#define MSG_STOP "exit"

#endif /* #ifndef COMMON_H_ */
