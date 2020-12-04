/*
Rafael Díaz Medina A01024592
Edgar García A01031730
* Green
! Red
? blue
TODO: ORANGE xD 


TODO: Ya enviamos Rojo, nos falta quitarle uno a la primera vez

! Descarga una extensión que se llama color comments para que veas bonitos los comentarios xD
!Semaforos
*Primero que nada Crear un sevidor y cuatro clientes
? Atributos de el semaforo
* Verde 30 segundos, después le cede al siguiente semaforo al de la derecha.
* El semaforo enviara una señal SIGRUS1 para prender al otro semaforo.
* Si el proceso no recibe la señal se queda en rojo
* Cada que cambia de estado le manda su estado al administrador
? Si el Semaforo recibe un mensaje de el admin el mismo semaforo debe de almacenar su estado
? Los semaforos solo pueden responder a la señal SIGUSR1 que le envía otro semaforo directamente
! Administración
* La administración es el server y los 4 semaforos clientes
* El admin recibe y muestra el estado actual de cada semaforo siempre! hasta cuando ella lo manda
? La consola mandara un mensaje para que los semaforos esten intermitentes de 00:00 a 6:00 am
? Ctrl+Z envía el mensaje de ponerse en rojo a todos los semaforos, no importa que hagan
? Ctrl+C envía el mensaje de intermitente a todos los semaforos, no importa que hagan
? La consola solo responde a las señales Ctrl+C y Ctrl+Z
TODO- Para regresar los semaforos al estado en el que se encontraban o al inicio de los tiempos necesito
TODO- Volver a enviar el mismo mensaje
*/
/*
!Primero hacer el server y luego el cliente!
!Luego hacer la recepción de señales de cada estado, ctrl+c y ctrl+z
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

//* Tipos de funciones

void gestor(int);
void gestorDos(int);
void estadoActual(int);

//* Variables Globales
int semaforo;
int cliente[4];
int r;
int m;
//!---------------Main---------------
int main(int argc, const char * argv[]){
    sigset_t lasDos;
    
    struct sockaddr_in direccion;
    
    int pidReceived;
    int servidor;
    
    ssize_t leidos;
    socklen_t escritos;
    int continuar = 1;
    pid_t pid;
//?-------------------------------------------------------------------------------
//?-------------------------Control de señales Para el padre----------------------
//?-------------------------------------------------------------------------------
    sigemptyset(&lasDos);
    sigaddset(&lasDos, SIGINT);
    sigaddset(&lasDos, SIGTSTP);
    if (signal(SIGTSTP, gestorDos) == SIG_ERR){
        printf("ERROR: No se pudo llamar al manejador\n");
    }
    else if (signal(SIGINT, gestorDos) == SIG_ERR){
        printf("ERROR: No se pudo llamar al manejador\n");
    }
//?-------------------------------------------------------------------------------
//?---------------------------FIN CONTROL DE SEÑALES------------------------------
//?-------------------------------------------------------------------------------
//!-------------------------------------------------------------------------------
//!---------------------------Scockets, control de SOCKETS------------------------  
//!-------------------------------------------------------------------------------  
    
    if (argc != 2) {
        printf("Use: %s IP_Servidor \n", argv[0]);
        exit(-1);
    }
    
    // Crear el socket
    servidor = socket(PF_INET, SOCK_STREAM, 0);
    // Enlace con el socket
    inet_aton(argv[1], &direccion.sin_addr);
    direccion.sin_port = htons(TCP_PORT);
    direccion.sin_family = AF_INET;
    
    bind(servidor, (struct sockaddr *) &direccion, sizeof(direccion));
    
    // Escuhar
    listen(servidor, 4);
    
    escritos = sizeof(direccion);

    int * semaforos;
    semaforos=(int*)malloc(4*sizeof(int));
    int * pidSemaforos;
    pidSemaforos=(int*)malloc(4*sizeof(int));
    int * s=semaforos;
    int * i;
    int count=0;

    for (i = s; i<s+4; i++, count++) {

        cliente[count] = accept(servidor, (struct sockaddr *) &direccion, &escritos);
        
        printf("Aceptando conexiones en %s:%d \n",
               inet_ntoa(direccion.sin_addr),
               ntohs(direccion.sin_port));
        
        pid = fork();
        
        if (pid == 0) {
//?-------------------------------------------------------------------------------
//?-------------------------Control de señales Para los Hijos---------------------
//?-------------------------------------------------------------------------------
            if (signal(SIGTSTP, gestor) == SIG_ERR){
            printf("ERROR: No se pudo llamar al manejador\n");
            }
            else if (signal(SIGINT, gestor) == SIG_ERR){
                printf("ERROR: No se pudo llamar al manejador\n");
            }
//?-------------------------------------------------------------------------------
//?---------------------------FIN CONTROL DE SEÑALES------------------------------
//?-------------------------------------------------------------------------------
            
            semaforo=cliente[count];
            close(servidor);

            if (cliente[count] >= 0) {
                int pidReceivedConv;
                //Leer datos del socket
                while(leidos = read(semaforo, &pidReceivedConv, sizeof(pidReceivedConv))) {
                    estadoActual(count);
                    printf("Soy el %d", getpid());
                }
            }

            close(semaforo);
        }
        else {//? Cumple la función de comunicación entre padre e hijo
            pidSemaforos[count] = read(cliente[count], i, sizeof(i));
        }
    }

    if (pid > 0) {
        for (int j = 0; j < 4; j++) {
            if (j==3){

                write(cliente[j], &semaforos[0], sizeof(int));
            }
            else{
                write(cliente[j], &semaforos[j+1], sizeof(int));
            }
        }

        int convert= htonl(5);
        write(cliente[0], &convert, sizeof(convert));

        while (wait(NULL) != -1);
        
        // Cerrar sockets
        close(servidor);
        
    }
    //! Liberar memoria
    free(pidSemaforos);
    free(semaforos);
//!-------------------------------------------------------------------------------  
//!-------------------------------FIN DE SOCKETS----------------------------------
//!-------------------------------------------------------------------------------  
    return 0;
}
//* 😎 Con esta función mandamos el estado actual que tenemoos para imprimir los colores 😎
void estadoActual(int s) {
    printf("\n\t\t--------Calles Don Rafa y Edgar---------\n");
    for (int i=0; i<4; i++) {
        
        if (i == 0){
            for (int i = 0; i < 5; i++){
                printf("\t\t\t |\t\t|\n");
                if(i==4){
                    printf("\t\t\t |##############|\n");
                }
            }
            
            if(i==s){
                printf("\033[0;92m");
                printf("\t\t  Verde%d |\t",i);
                printf("\033[0m");
            }
            else{
                printf("\033[0;91m");
                printf("\t\t   Rojo%d |\t    ",i);
                printf("\033[0m");
            }
        }
        else if (i==1){
            if(i==s){
                printf("\033[0;92m");
                printf("    | Verde%d\n",i);
                printf("\033[0m");
            }
            else{
                printf("\033[0;91m");
                printf("\t| Rojo%d\n", i);
                printf("\033[0m");
            }
            printf("-------------------------|\t\t|-------------------------\n");
            for(int i=0; i<5;i++){
                printf("\t\t      #");
                printf("\t\t\t   #\n");
            }

            printf("-------------------------|\t\t|-------------------------\n");
        }
        else if(i==2){
            if(i==s){
                printf("\033[0;92m");
                printf("\t\t   Verde%d|\t",i);
                printf("\033[0m");
            }
            else{
                printf("\033[0;91m");
                printf("\t\t   Rojo%d |\t",i);
                printf("\033[0m");
            }   
        }
        else if(i==3){
            if(i==s){
                printf("\033[0;92m");
                printf("\t| Verde%d\n",i);
                printf("\033[0m");
            }
            else{
                printf("\033[0;91m");
                printf("\t| Rojo%d\n", i);
                printf("\033[0m");
            }  
            for (int i = 0; i < 5; i++){
                if(i==0){
                    printf("\t\t\t |##############|\n");
                }
                printf("\t\t\t |\t\t|\n");
            }
        }
    }
    printf("\n");
}
//* Aquí cuando mandamos estos mensjaes debemos de mandar la señal o algo que reinicie y bloquee a todos los clientes 😆
//* Aquí falta ponerle los colores adecuados a la calle Rafa 😆
void gestor(int s){
	if (s == SIGTSTP){
        int ctrlZ=htonl(2);
        write(semaforo, &ctrlZ, sizeof(ctrlZ));
    }
	else if(s==SIGINT){
        int ctrlC=htonl(3);
        write(semaforo, &ctrlC, sizeof(ctrlC));   
	}
}
void gestorDos(int s){
	if (s == SIGTSTP){
        if (r%2==0){
            printf("Acabo de enviar el mensaje ROJO Ctrl+Z a todos los semaforos\n");
            printf("\033[0;91m");
            for (int i = 0; i < 5; i++){
                printf("\t\t\t |\t\t|\n");
                if(i==4){
                    printf("\t\t\t |##############|\n");
                }
            }
            printf("\t\t    Rojo |\t    ");
            printf("\t| Rojo\n");
            printf("-------------------------|\t\t|-------------------------\n");
            for(int i=0; i<5;i++){
                printf("\t\t      #");
                printf("\t\t\t   #\n");
            }
            printf("-------------------------|\t\t|-------------------------\n");
            printf("\t\t    Rojo |\t");
            printf("\t| Rojo\n");
            for (int i = 0; i < 5; i++){
                if(i==0){
                    printf("\t\t\t |##############|\n");
                }
                printf("\t\t\t |\t\t|\n");
            }
            printf("\033[0m"); 
            printf("\n");
        }
        else{
            printf("Semaforos Don Rafa y Edgar regresaron a la normalidad\n");
        }
        r++;
    }
	else if(s==SIGINT){
         if (m%2==0){
            printf("Acabo de enviar el mensaje ROJO Ctrl+C a todos los semaforos\n");
            printf("\033[0;93m");
            for (int i = 0; i < 5; i++){
                printf("\t\t\t |\t\t|\n");
                if(i==4){
                    printf("\t\t\t |##############|\n");
                }
            }   
            printf("\t     Intermitent |\t    ");
            printf("\t| Intermitent\n");
            printf("-------------------------|\t\t|-------------------------\n");
            for(int i=0; i<5;i++){
                printf("\t\t      #");
                printf("\t\t\t   #\n");
            }
            printf("-------------------------|\t\t|-------------------------\n");
            printf("\t     Intermitent |\t    ");
            printf("\t| Intermitent\n");
            for (int i = 0; i < 5; i++){
                if(i==0){
                    printf("\t\t\t |##############|\n");
                }
                printf("\t\t\t |\t\t|\n");
            }
            printf("\033[0m"); 
        printf("\n");
        }
        else{
            printf("Semaforos Don Rafa y Edgar regresaron a la normalidad\n");
        }
        m++;
    }
}
