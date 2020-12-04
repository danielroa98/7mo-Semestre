/*
Autores: Antonio Junco de Haas, Sergio Hernandez Castillo
Matrículas: A, A01025210
Descripción: Actividad 6 - Sockets y Señales

NOTA: Se trabajó junto con el equipo 9, el cual está compuesto por Daniel Roa
      y Miguel Monterrubio
*/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define TCP_PORT 8000

int semaforo_actual;
int semaforos[4];
int bufferes_semaforos[4];
ssize_t pids[4];

void todosEnRojo(int senial){
    char todosRojitos[] = "TodosRojosEh";

    for (int i = 0; i < 4; ++i){
        write(semaforos[i], &todosRojitos, sizeof(todosRojitos));
    }

    printf("Todos están en rojo.\n");
}

void todosEnAmarillo(int senial){
    char todosAmarillitos[] = "TodosAmarillosEh";

    for (int i = 0; i < 4; ++i){
        write(semaforos[i], &todosAmarillitos, sizeof(todosAmarillitos));
    }

    printf("Todos están en amarillo.\n");
}

void estadoSemaforoActual(int senial){
    printf("\n");

    for (int i = 0; i < 4; ++i){
        if (i == senial){
            printf("Semáforo %d está en verde.\n", i + 1);
        }

        else {
            printf("Semáforo %d está en rojo.\n", i + 1);
        }
    }
}

int main(int argc, const char * argv[]){
    struct sockaddr_in direccion;
    char buffer[1000];
    char inicio[] = "Empieza";
    int servidor;
    ssize_t leidos;
    ssize_t escritos; 
    pid_t pid;

    if (argc != 2){
        printf("Usa: %s IP_Servidor\n", argv[0]);
        exit(-1);
    }

    // Se crea el socket
    servidor = socket(PF_INET, SOCK_STREAM, 0);

    // Enlance con el socket
    inet_aton(argv[1], &direccion.sin_addr);
    direccion.sin_port = htons(TCP_PORT);
    direccion.sin_family = AF_INET;

    bind(servidor, (struct sockaddr *) &direccion, sizeof(direccion));

    // Escuchar 
    listen(servidor, 4); // 4 porque son 4 semaforos

    escritos = sizeof(direccion);

    for (int i = 0; i < 4; ++i){
        semaforos[i] = accept(servidor, (struct sockaddr *) &direccion, &escritos);
        printf("Aceptando conexiones en %s:%d\n", inet_ntoa(direccion.sin_addr), ntohs(direccion.sin_port));

        pid = fork();

        if (pid == 0){
            semaforo_actual = semaforos[i];
            
            if (signal(SIGTSTP, todosEnRojo) == SIG_ERR){
                printf("Hubo un error con el manejador rojo.\n");
            }

            if (signal(SIGINT, todosEnAmarillo) == SIG_ERR){
                printf("Hubo un error con el manejador amarillo.\n");
            }

            close(servidor);

            if (semaforo_actual >= 0){
                while(leidos = read(semaforos[i], &buffer, sizeof(buffer))){
                    estadoSemaforoActual(i);
                }
            }

            close(semaforo_actual);
        }

        else {
            pids[i] = read(semaforos[i], &bufferes_semaforos[i], sizeof(bufferes_semaforos[i]));
        }
    }

    if (pid > 0){
        for (int i = 0; i < 4; ++i){
            if (i == 3){
                write(semaforos[i], &bufferes_semaforos[0], pids[0]);
            }

            else {
                write(semaforos[i], &bufferes_semaforos[i + 1], pids[i + 1]);
            }
        }

        write(semaforos[0], &inicio, sizeof(inicio));

        while (wait(NULL) != -1);

        close(servidor);
    }

    return 0;
}