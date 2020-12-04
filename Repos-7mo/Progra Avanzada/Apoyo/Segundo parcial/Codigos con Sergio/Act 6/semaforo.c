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
#include <sys/types.h>
#include <string.h>

#define TCP_PORT 8000

int semaforo;
int siguiente_semaforo;
int color; // Rojo = 0, Amarillo = 1, Verde = 2
int color_anterior; 

void reverse(char s[]){
    int i;
    int j;
    char c;
    
    for (i = 0, j = strlen(s) - 1; i < j; ++i, --j){
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

void itoa(int n, char s[]){
    int i;
    int sign;
    
    if ((sign = n) < 0){  
        n = -n;
    }          

    i = 0;

    do {       
        s[i++] = n % 10 + '0';   
    }
    
    while ((n /= 10) > 0);

    if (sign < 0){
        s[i++] = '-';
    }

    s[i] = '\0';

    reverse(s);
}

void verde(int senial){
    color = 2;
    char elColor[] = "Verde";
    write(semaforo, &elColor, sizeof(elColor));

    printf("Yo ahora estoy en verde.\n");

    alarm(3);
}

void elQueSigue(int senial){
    color = 0;
    kill(siguiente_semaforo, SIGUSR1);
}

int main(int argc, const char * argv[]){
    struct sockaddr_in direccion;
    char buffer[1000];
    ssize_t leidos;
    ssize_t escritos;
    pid_t pid = getpid();
    sigset_t listaDeSeniales;

    if (argc != 2){
        printf("Usa: %s IP_Servidor\n", argv[0]);
        exit(-1);
    }

    sigemptyset(&listaDeSeniales);
    sigaddset(&listaDeSeniales, SIGUSR1);
    sigaddset(&listaDeSeniales, SIGALRM);

    // Se crea el socket
    semaforo = socket(PF_INET, SOCK_STREAM, 0);

    // Enlance con el socket
    inet_aton(argv[1], &direccion.sin_addr);
    direccion.sin_port = htons(TCP_PORT);
    direccion.sin_family = AF_INET;

    escritos = connect(semaforo, (struct sockaddr *) &direccion, sizeof(direccion));

    if (escritos == 0){
        printf("Conectado a %s:%d\n", inet_ntoa(direccion.sin_addr), ntohs(direccion.sin_port));

        itoa(pid, buffer);

        write(semaforo, &buffer, sizeof(int)); //Si no jala, quita el ampersand
        leidos = read(semaforo, &buffer, sizeof(buffer));

        siguiente_semaforo = atoi(buffer);

        if (signal(SIGUSR1, verde) == SIG_ERR){
            printf("Hubo un error con el manejador verde.\n");
        }

        if (signal(SIGALRM, elQueSigue) == SIG_ERR){
            printf("Hubo un error con el manejador que pasa el color verde al siguiente semáforo.\n");
        }

        while(leidos = read(semaforo, &buffer, sizeof(buffer))){
            if (strcmp(buffer, "Empieza") == 0){
                raise(SIGUSR1);
            }

            else if ((strcmp(buffer, "TodosRojosEh") == 0) && (color != 0)){
                color_anterior = color;
                color = 0;
                printf("Yo ahora estoy en rojo.\n");
                sigprocmask(SIG_BLOCK, &listaDeSeniales, NULL);
            }

            else if ((strcmp(buffer, "TodosAmarillosEh") == 0) && (color != 1)){
                color_anterior = color;
                color = 1;
                printf("Yo ahora estoy en amarillo.\n");
                sigprocmask(SIG_BLOCK, &listaDeSeniales, NULL);
            }

            else if ((strcmp(buffer, "TodosRojosEh") == 0) && (color == 0)){
                color = color_anterior;
                printf("Ya no estoy en rojo.\n");
                sigprocmask(SIG_UNBLOCK, &listaDeSeniales, NULL);
            }

            else if ((strcmp(buffer, "TodosAmarillosEh") == 0) && (color == 1)){
                color = color_anterior;
                printf("Ya no estoy en amarillo.\n");
                sigprocmask(SIG_UNBLOCK, &listaDeSeniales, NULL);
            }
        }
    }

    close(semaforo);

    return 0;
}