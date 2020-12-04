/*
Rafael Díaz Medina A01024592
Edgar García A01031730
*/
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define TCP_PORT 8000
//!-------------------Debemos de ver como hacer que este cliente se comunique con los otros clientes
//!-----------Pienso que tal vez debamos de usar la función Listen de el servidor
//* Tipos de funciones

void gestorEstado(int signal);
void gestorNextEstado(int signal);

//* Variables Globales
int cliente;  
int nextPid;  
int estado;   
int estadoAnterior;

int main(int argc, const char * argv[]){
 
    struct sockaddr_in direccion;
    int pidToSend=getpid();
    int pidConvert=htonl(pidToSend);
    int estadoAnterior;
    int pidGet;
    ssize_t leidos;
    socklen_t escritos;
    sigset_t lasDos;

//?-------------------------------------------------------------------------------
//?-------------------------Control de señales Para el padre----------------------
//?-------------------------------------------------------------------------------
    sigemptyset(&lasDos);
    sigaddset(&lasDos, SIGUSR1);
    sigaddset(&lasDos, SIGINT);
    sigaddset(&lasDos, SIGTSTP);
    sigaddset(&lasDos,SIGALRM);
    if (signal(SIGTSTP, SIG_IGN) == SIG_ERR){
        printf("ERROR: No se pudo llamar al manejador\n");
    }
    else if (signal(SIGINT, SIG_IGN) == SIG_ERR){
        printf("ERROR: No se pudo llamar al manejador\n");
    } 
//?-------------------------------------------------------------------------------
//?---------------------------FIN CONTROL DE SEÑALES------------------------------
//?-------------------------------------------------------------------------------
    
    if (argc != 2){
        printf("Use: %s IP_Servidor \n", argv[0]);
        exit(-1);
    }
    
    int  receivedInt;//*Para el pid que recibimos
    // Crear el socket
    cliente = socket(PF_INET, SOCK_STREAM, 0);
    // Establecer conexión
    inet_aton(argv[1], &direccion.sin_addr);
    direccion.sin_port = htons(TCP_PORT);
    direccion.sin_family = AF_INET;
    
    escritos = connect(cliente, (struct sockaddr *) &direccion, sizeof(direccion));
    
    if (escritos == 0){
        printf("Conectado a %s:%d \n",
                inet_ntoa(direccion.sin_addr),
                ntohs(direccion.sin_port));

        write(cliente, &pidConvert, sizeof(pidConvert));
        leidos = read(cliente, &pidGet, sizeof(pidGet));
        nextPid=ntohl(pidGet);
//?-------------------------------------------------------------------------------
//?-------------------------Recibir señales de los semaforos----------------------
//?-------------------------------------------------------------------------------

        if (signal(SIGUSR1, gestorEstado) == SIG_ERR){
            printf("ERROR: No se pudo llamar al manejador\n");
        }
        else if (signal(SIGALRM, gestorNextEstado) == SIG_ERR){
            printf("ERROR: No se pudo llamar al manejador\n");
        } 
//?-------------------------------------------------------------------------------
//?---------------------------Fin señales de los semaforos------------------------
//?-------------------------------------------------------------------------------

        while ((leidos = read(cliente, & receivedInt, sizeof( receivedInt)))){
            if (ntohl( receivedInt) == 5){
                raise(SIGUSR1);
            } 
            else if (ntohl( receivedInt) == 2 && estado != 2){
                estadoAnterior=estado;
                estado = 2;
                printf("Entre al rojo");
                sigprocmask(SIG_BLOCK, &lasDos, NULL);
            } 
            else if (ntohl( receivedInt) == 3 && estado != 3){
                estadoAnterior=estado;
                estado = 3;
                printf("entre al intermitente \n");
                sigprocmask(SIG_BLOCK, &lasDos, NULL);
            }
            else if (ntohl( receivedInt) == 2 && estado == 2){//? Para la segunda vez
                estado=estadoAnterior;
                printf("Sali del Rojo");             
                sigprocmask(SIG_UNBLOCK, &lasDos, NULL);
                     
            } 
            else if (ntohl( receivedInt) == 3 && estado == 3){//? Para la segunda vez
                estado=estadoAnterior;
                printf("Sali del intermitente");
                sigprocmask(SIG_UNBLOCK, &lasDos, NULL);     
            }
        }
    }
    // Cerrar sockets
    close(cliente);
    return 0;
}
//* Enviamos el estado actual a nuestro server para que el lo imprima
void gestorEstado(int s){
    int time=4;
    int state=htonl(1);
    write(cliente, &state, sizeof(int));
    printf("Me cambie a verde soy el %d\n", getpid());
    alarm(time);
}

//* Enviamos el estado que sigue
void gestorNextEstado(int s){
    estado = 0;
    kill(nextPid, SIGUSR1);
}
