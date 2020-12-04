/*
 *  Actividad 6:
 *  Sockets y Señales
 * 
 *  Programación Avanzada
 *  Profesor: Vicente Cubells
 * 
 *  Este código fue elaborado por:
 *      Daniel Roa              -   A01021960
 *      Miguel Monterrubio      -   A01022153
 * 
 *  Este código fue hecho en conjunto con:
 *      Sergio Hernández
 *      Antonio Junco
 * 
 *  Ese es el programa de la consola de monitoreo, 
 *  le envía señales a los semaforos que se encuentran
 *  en el código de cruce.c
 */
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

// Port available for the console
#define TCP_PORT 8000

// Variable para indicar el semaforo que se está manejando
int semaforo_encendido;
int semaforos[4];
int bufferes[4];
ssize_t pids[4];
int greenGo = 0;

// Metodo para poner todos los semáforos en intermitentes (amarillo).
void cntrlCopy(int senial){
    char todosSlow[] = "amarillo";

    printf("Todos están en amarillo\n");

    for (int i = 0; i < 4; ++i)
    {
        write(semaforos[i], &todosSlow, sizeof(todosSlow));
    }
    
}

// Metodo para poner todos los semáforos en alto (rojo).
void cntrlUndo(int senial){
    char todosStop[] = "rojo";

    printf("Todos están en rojo\n");

    for (int i = 0; i < 4; ++i)
    {
        write(semaforos[i], &todosStop, sizeof(todosStop));
    }
    
}

void estadoSemaforoActual(int sign){

    printf("*---------ESTADO DE LOS SEMÁFOROS---------*\n");

    for (int cont = 0; cont < 4; ++cont){

        if(cont == sign && greenGo == 1){
        
            printf("El semáforo %d está en verde.\n", cont + 1);
        
        }else{
            printf("El semáforo %d está en rojo.\n", cont + 1);
        }
    }

    printf("*----------------------------------------*\n");
}

int main(int argc, const char *argv[])
{
    struct sockaddr_in direccion;
    char infoSemaforo[1000];

    int servidor;

    ssize_t leidos, escritos;

    // Estados del semaforo
    char inicioSemaforo[] = "verde";
    char intSemaforo[] = "amarillo";
    char pausaSemaforo[] = "rojo";

    pid_t pid;

    //Tamaños de las variables actuales
    size_t infoBufferSize = sizeof(infoSemaforo);
    size_t inicioSemaforoSize = sizeof(inicioSemaforo);
    size_t intSemaforoSize = sizeof(intSemaforo);
    size_t pausaSemaforoSize = sizeof(pausaSemaforo);

    if (signal(SIGINT, SIG_IGN) == SIG_ERR){
        printf("ERROR: No se pudo establecer conexión con el manejador de la señal.\n");
    }
    
    if(signal(SIGTSTP, SIG_IGN) == SIG_ERR){
        printf("ERROR: No se pudo establecer conexión con el manejador de la señal.\n");
    }

    if (argc != 2)
    {
        printf("Use: %s IP_Servidor \n", argv[0]);
        exit(-1);
    }

    // Crear el socket
    servidor = socket(PF_INET, SOCK_STREAM, 0);

    // Enlace con el socket
    inet_aton(argv[1], &direccion.sin_addr);
    direccion.sin_port = htons(TCP_PORT);
    direccion.sin_family = AF_INET;

    escritos = sizeof(direccion);
    bind(servidor, (struct sockaddr *)&direccion, escritos);

    // Escuhar especificamente para 4 semaforos
    listen(servidor, 4);

    // Aceptar conexiones

    for (int i = 0; i < 4; ++i){
        //Para almacenar la cantidad total de hosts
        semaforos[i] = accept(servidor, (struct sockaddr *)&direccion, &escritos);
        
        //Mensaje para confirmar a los usuarios que se puede establecer una conexión
        printf("Aceptando conexiones en %s:%d\n", inet_ntoa(direccion.sin_addr), ntohs(direccion.sin_port));

        pid = fork();

        if (pid == 0){
            semaforo_encendido = semaforos[i];

            // Funciones para poder convocar a los métodos que se encargan de las señales por manejarse
            if (signal(SIGINT, cntrlCopy) == SIG_ERR){
                printf("ERROR: No se pudo cambiar el estado de los semáforos a amarillo.\n");
            }

            if (signal(SIGTSTP, cntrlUndo) == SIG_ERR){
                printf("ERROR: No se pudo cambiar los semáforos a rojo.\n");
            }

            close(servidor);

            if(semaforo_encendido >= 0){
                while(leidos = read(semaforos[i], &infoSemaforo, sizeof(infoSemaforo))){
                    if(strcmp(infoSemaforo, "verde") == 0){
                        greenGo = 1;
                    }else{
                        greenGo = 0;
                    }

                    estadoSemaforoActual(i);
                }
            }
            close(semaforo_encendido);
        
        }else{

            pids[i] = read(semaforos[i], &bufferes[i], sizeof(bufferes[i]));

        }

    }
    
    if(pid > 0){

        for (int i = 0; i < 4; ++i){
            
            if(i == 3){
                write(semaforos[i], &bufferes[0], pids[0]);
            }
            else{
                write(semaforos[i], &bufferes[i+1], pids[i + 1]);
            }

        }
        write(semaforos[0], &inicioSemaforo, inicioSemaforoSize);

        while(wait(NULL) != -1);

        close(servidor);
        
    }

    return 0;
}
