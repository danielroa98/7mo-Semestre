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
 *  Ese es el programa de los semaforos, 
 *  responde a las señales enviadas por el código
 *  de consola.c
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

// Enteros importantes
int stoplight;
int nextSemaforo;
// Valor usado para identificar el estado actual del semáforo, Rojo = 0, Amarillo = 1 y Verde = 2
int luz;
int pastLight;

void reverse(char s[])
{
    int i, j;
    char c;

    for (i = 0, j = strlen(s) - 1; i < j; i++, j--){
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

// Función ITOA para convertir un int a un string
void itoa(int n, char s[])
{
    int i, sign;

    if ((sign = n) < 0){         // record sign
        n = -n;                 // make n positive
    }

    i = 0;
    do{                          // generate digits in reverse order
        s[i++] = n % 10 + '0'; // get next digit
    } while ((n /= 10) > 0);   // delete it 
    
    if (sign < 0){
        s[i++] = '-';
    }

    s[i] = '\0';
    reverse(s);
}

// Método para actualizar el color actual del semáforo a verde
void updateGo(){
    luz = 2;
    char avance[] = "verde";
    printf("Ya soy verde.\n");
    write(stoplight, &avance, sizeof(avance));
    alarm(30);
}

// Método para cambiar el color a rojo
void valarMorghulis(){
    luz = 0;
    printf("Regresé a rojo.\n");
    kill(nextSemaforo, SIGUSR1);
}

int main(int argc, const char *argv[])
{
    struct sockaddr_in direccion;
    char buffer[1000];
    size_t bufferSize = sizeof(buffer);

    pid_t pid = getpid();

    ssize_t leidos, escritores;
    sigset_t sigList;
    

    if (argc != 2)
    {
        printf("Use: %s IP_Servidor \n", argv[0]);
        exit(-1);
    }

    sigemptyset(&sigList);
    sigaddset(&sigList, SIGALRM);
    sigaddset(&sigList, SIGUSR1);

    // Crear el socket
    stoplight = socket(PF_INET, SOCK_STREAM, 0);

    // Establecer conexión
    inet_aton(argv[1], &direccion.sin_addr);
    direccion.sin_port = htons(TCP_PORT);
    direccion.sin_family = AF_INET;

    // Para establecer la conexión indicada
    escritores = connect(stoplight, (struct sockaddr *)&direccion, sizeof(direccion));

    if (escritores == 0)
    {
        printf("Conectado a la consola con la dirección de %s:%d \n", inet_ntoa(direccion.sin_addr), ntohs(direccion.sin_port));

        itoa(pid, buffer);
        write(stoplight, buffer, sizeof(int));

        leidos = read(stoplight, &buffer, bufferSize);
        nextSemaforo = atoi(buffer);

        if(signal(SIGUSR1, updateGo) == SIG_ERR){
            printf("No se pudo actualizar el estado del semáforo a verde.\n");
        }

        if(signal(SIGALRM, valarMorghulis) == SIG_ERR){
            printf("No se pudo cambiar el color del semáforo a rojo.\n");
        }

        // Escribir datos en el socket
        while (leidos = read(stoplight, &buffer, sizeof(buffer))){
            
            if(strcmp(buffer, "verde") == 0){
            
                raise(SIGUSR1);
            
            }
            else if (strcmp(buffer, "rojo") == 0 && luz != 0)
            {
                pastLight = luz;
                luz = 0;
                printf("Cambio manual a rojo. BLOQUEADO\n");
                sigprocmask(SIG_BLOCK, &sigList, NULL);
            }
            else if (strcmp(buffer, "amarillo") == 0 && luz != 1){
                pastLight = luz;
                luz = 1;
                printf("Cambio a manual a amarillo. BLOQUEADO\n");
                sigprocmask(SIG_BLOCK, &sigList, NULL);
            }
            else if(strcmp(buffer, "rojo") == 0 && luz == 0){
                luz = pastLight;
                printf("DESBLOQUEADO, ya no soy rojo\n");
                sigprocmask(SIG_UNBLOCK, &sigList, NULL);
            }
            else if(strcmp(buffer, "amarillo") == 0 && luz == 1){
                luz = pastLight;
                printf("DESBLOQUEADO, ya no soy amarillo\n");
                sigprocmask(SIG_UNBLOCK, &sigList, NULL);
            }
        }
        
    }

    // Cerrar sockets
    close(stoplight);

    return 0;
}
