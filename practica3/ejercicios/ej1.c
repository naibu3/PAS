/*El fichero de codigo de este ejercicio se llamara ((ej1.c)).

Implemente un programa que obtenga e imprima informacion sobre usuarios del sistema
(todos los campos de la estructura passwd) e informacion sobre grupos del sistema (GID y
nombre del grupo mediante la estructura group), segun las opciones recibidas por la linea de
argumentos.
- La opcion -u/--user se utilizara para indicar un usuario. Si le pasamos como argu-
mento un numero, lo interpretara como UID, en caso contrario como el nombre del
usuario. Se mostrara la informacion correspondiente a su estructura passwd.
- La opcion -g/--group se utilizara para indicar un grupo. Si le pasamos como argu-
mento un numero, lo interpretara como GID, en caso contrario como el nombre del
grupo. Se mostrara la informacion correspondiente a su estructura group.
- La opcion -a/--active sera equivalente a especificar--user con el usuario actual.
- La opcion -m/--maingroup modifica a --user o --active y hace que imprima la
informacion de su grupo principal (mismo formato que --group).
- Si se invoca al programa con la opcion -s o con --allgroups, se mostraran todos
los grupos del sistema, junto con su identificador. Para ello recorra el fichero corres-
pondiente (le permitira recordar como gestionar y buscar en cadenas) y luego vaya
extrayendo informacion como si se invocase la opcion --group. No muestre directa-
mente el contenido del fichero, recorralo y muestre la informacion por cada grupo que
haya.
- Invocar al programa sin ninguna opcion sera equivalente a activar las opciones --active
y --maingroup, es decir, se mostrara la informacion del usuario actual (con el mismo
formato que que -u/--user) y del grupo al que pertenece (con el mismo formato que
que -g/--group).
- Se creara una opcion de ayuda -h/--help para mostrar informacion sobre cada uno
de los usos del programa. Esa informacion tambien se mostrara cuando el usuario
cometa cualquier error en la invocacion del programa.

Tenga tambien en cuenta el siguiente control de errores:
- Asegurar que se pasan nombres e identificadores de usuarios o grupos validos que
existan en la m ́aquina.
- Asegurar que no se puedan pasar por linea de comandos opciones que sean incompa-
tibles. Las  ́unicas combinaciones posibles son:
• --help, junto con cualquiera (se ignoran las demas)
• vacio (equivalente a --active --maingroup)
• --user <uid o nombre de usuario>
• --user <uid o nombre de usuario> --maingroup
• --active
• --active --maingroup
• --group <gid o nombre de grupo>
• --allgroups
*/

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <grp.h>
#include <pwd.h>
#include <getopt.h>

//HEADERS__________________________________________________________________________
void help();
int userinfo(char* user);
void groupinfo(char* group);
bool esid(const char *var);
void allgroupinfo();

//MAIN______________________________________________________________________________
int main(int argc, char **argv) {

    int c;
    int gid=-1;
    char* aux;
    const char* argstr="hu:ag:ms";
    char* user;
    char* group;

    bool u=false;
    bool a=false;
    bool g=false;
    bool m=false;
    bool s=false;

    if(argc==1){
        a=true;
        m=true;
    }

    //Opciones largas
    static struct option long_options[] = {
        {"help", no_argument, NULL, 'h'},
        {"user", required_argument, NULL, 'u'},
        {"group", required_argument, NULL, 'g'},
        {"active", no_argument, NULL, 'a'},
        {"maingroup", no_argument, NULL, 'm'},
        {"allgroups", no_argument, NULL, 's'},
        /* Necesario para indicar el final de las opciones */
        {0, 0, 0, 0}
    };

    //GESTION DE PARAMETROS__________________________________________________

    /* Deshabilitar la impresión de errores por defecto */
    /* opterr=0; */

    //SOLO OPCIONES CORTAS
    /*while ((c = getopt(argc, argv, argstr)) != -1) {

        //debug block
        //- optind: Indice del siguiente elemento ("-") a procesar del vector argv[]
        //- optarg: recoge el valor del argumento obligario de una opcion.
        //- optopt: recoge el valor de la opcion ("-") cuando es desconocida (?) o INCOMPLETA
        printf("optind: %d, optarg: %s, optopt: %c, opterr: %d\n\n", optind, optarg, optopt, opterr);

        switch (c) {
        case 'h':{   //-h
            printf("Help detectada\n");//debug
            help();
            exit(0);
            break;
        }
        case 'u':{   //-u
            printf("U detectada\n");//debug
            u=true;
            user=optarg;
            break;
        }
        case 'g':{   //-g
            printf("G detectada\n");//debug
            g=true;
            group=optarg;
            break;
        }
        case 'a':{   //-a
            printf("A detectada\n");//debug
            a=true;
            break;
        }
        case 'm':   //-m
            printf("M detectada\n");//debug
            m=true;
            break;

        case 's':   //-s
            printf("S detectada\n");//debug
            s=true;
            break;

        case '?':{   // Opcion no reconocida o INCOMPLETA (sin argumento)
            printf("Unknown detectada\n");//debug
            // Para el caso de que 'c' no tenga el argumento obligatorio.
            if (optopt == 'u'){

                fprintf(stderr,
                        "La opción %c requiere un argumento. Valor de opterr = "
                        "%d\n",
                        optopt, opterr);
            }
            // Se mira si el caracter es imprimible
            else if (isprint(optopt)){
                fprintf(stderr,
                        "Opción desconocida \"-%c\". Valor de opterr = %d\n",
                        optopt, opterr);
            }
            // Caracter no imprimible o especial
            else{
                fprintf(stderr, "Caracter `\\x%x'. Valor de opterr = %d\n",
                        optopt, opterr);
            }
            return 1; // Finaliza el programa
            break;
        }
        default:{
            if(argc=1){
                a=true;
                m=true;
            }
            break;
        }
        }
        
    }*/

    //OPCIONES LARGAS (Y CORTAS)
    while ((c = getopt_long(argc, argv, argstr, long_options, NULL)) != -1) {

        //debug block
        //- optind: Indice del siguiente elemento ("-") a procesar del vector argv[]
        //- optarg: recoge el valor del argumento obligario de una opcion.
        //- optopt: recoge el valor de la opcion ("-") cuando es desconocida (?) o INCOMPLETA
        printf("optind: %d, optarg: %s, optopt: %c, opterr: %d\n\n", optind, optarg, optopt, opterr);

        switch (c) {
        case 'h':{   //-h
            printf("Help detectada\n");//debug
            help();
            exit(0);
            break;
        }
        case 'u':{   //-u
            printf("U detectada\n");//debug
            u=true;
            user=optarg;
            break;
        }
        case 'g':{   //-g
            printf("G detectada\n");//debug
            g=true;
            group=optarg;
            break;
        }
        case 'a':{   //-a
            printf("A detectada\n");//debug
            a=true;
            break;
        }
        case 'm':   //-m
            printf("M detectada\n");//debug
            m=true;
            break;

        case 's':   //-s
            printf("S detectada\n");//debug
            s=true;
            break;

        case '?':{   // Opcion no reconocida o INCOMPLETA (sin argumento)
            printf("Unknown detectada\n");//debug
            // Para el caso de que 'c' no tenga el argumento obligatorio.
            if (optopt == 'u'){

                fprintf(stderr,
                        "La opción %c requiere un argumento. Valor de opterr = "
                        "%d\n",
                        optopt, opterr);
            }
            // Se mira si el caracter es imprimible
            else if (isprint(optopt)){
                fprintf(stderr,
                        "Opción desconocida \"-%c\". Valor de opterr = %d\n",
                        optopt, opterr);
            }
            // Caracter no imprimible o especial
            else{
                fprintf(stderr, "Caracter `\\x%x'. Valor de opterr = %d\n",
                        optopt, opterr);
            }
            return 1; // Finaliza el programa
            break;
        }
        default:{
            printf("Se ha producido algún error.");
            exit(EXIT_FAILURE);
        }}
    }

    //debug block
    printf("u->%i\n", u);
    printf("g->%i\n", g);
    printf("a->%i\n", a);
    printf("s->%i\n", s);
    printf("m->%i\n ", m);

    //EJECUCION PPAL________________________________________________________

    if(u){
        gid=userinfo(user);
    }
    if(a){
        user = getenv("USER");
        gid=userinfo(user);
    }
    if(g){
        groupinfo(group);
    }
    if(m){
        if(user==NULL){
            printf("Debes modificar la opción -u (USER) o -a (ACTIVE)\n");
            exit(1);
        }
        sprintf(aux, "%i", gid);
        groupinfo(aux);
    }
    if(s){
        allgroupinfo();
    }

    return 0;
}

//FUNCIONES__________________________________________________________________________

void help(){

    printf("Uso: ./ej1.c <args>\n");
    printf("Posibles parámetros:\n");
    printf("    -u/--user se utilizara para indicar un usuario.\n");
    printf("    -g/--group se utilizara para indicar un grupo\n");
    printf("    -a/--active sera equivalente a especificar--user con el usuario actual\n");
    printf("    -m/--maingroup modifica a --user o --active y hace que imprima la "
"informacion de su grupo principal\n");
    printf("    -s o con --allgroups, se mostraran todos los grupos del sistema, "
"junto con su identificador\n");
}

int userinfo(char* user){

    struct passwd *pw;
    char* uid;

    //Comprobar si se pasa el uid o el username
    if( esid(user) ){

        printf("User pasado por uid\n");
        if ((pw = getpwuid(atoi(uid))) == NULL){ //Si uid

            fprintf(stderr, "Fallo al obtener información de usuario.\n");
            exit(1);
        }
    }else{
        printf("User pasado por name\n");
        if ((pw = getpwnam(user)) == NULL){ //Si username
        
            fprintf(stderr, "Fallo al obtener información de usuario.\n");
            exit(1);
        }
    }
    
    printf("\nNombre: %s\n", pw->pw_gecos); // No es lo mismo el nombre de usuario asociado a un login que el propio login
    printf("Login: %s\n", pw->pw_name);
    printf("Password: %s\n", pw->pw_passwd);
    printf("UID: %i\n", pw->pw_uid);
    printf("Home: %s\n", pw->pw_dir);
    printf("Shell: %s\n", pw->pw_shell);
    printf("Número de grupo principal: %i\n\n", pw->pw_gid);

    return pw->pw_gid;
}

void groupinfo(char* group){

    struct group *gr;

    //Comprobar si se pasa el uid o el username
    if(esid(group)){

        if ((gr = getgrgid(atoi(group))) == NULL){ //Si gid

            fprintf(stderr, "Fallo al obtener información de usuario.\n");
            exit(1);
        }
    }else{
        
        if ((gr = getgrnam(group)) == NULL){ //Si username
        
            fprintf(stderr, "Fallo al obtener información de usuario.\n");
            exit(1);
        }
    }

    printf("Nombre: %s\n", gr->gr_name);
    printf("Password: %s\n", gr->gr_passwd);
    printf("Id: %i\n", gr->gr_gid);
    printf("Members: %s\n", gr->gr_mem);
}


void allgroupinfo(){

    struct group *gr;
    setgrent(); // Abrir el archivo /etc/group

    printf("Grupos del sistema:\n");

    while ((gr = getgrent()) != NULL) {
        printf("\nNombre: %s\n", gr->gr_name);
        printf("Password: %s\n", gr->gr_passwd);
        printf("Id: %i\n", gr->gr_gid);
        printf("Members: %s\n\n", gr->gr_mem);
    }

    endgrent(); // Cerrar el archivo /etc/group
}

bool esid(const char *var) {
    // Verificar si la variable contiene solo dígitos
    for (int i = 0; var[i] != '\0'; i++) {
        if (!isdigit(var[i])) return false;
    }
    return true;
}