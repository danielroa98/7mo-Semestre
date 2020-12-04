/*  Equipo 3:
    Andrés Barragán Salas               A01026567
    Carla Pérez Gavilán del Castillo    A01023033

    Actividad 6: Sockets y Señales - Semaforo
*/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

#define TCP_PORT 8000
#define TIEMPO 3

int cliente; //Conexión establecida por socket
int next_pid; //PID del semaforo siguiente
int estado;  //Estado actual del semaforo ([0]Rojo, [1]Verde, [2]Interrupción Rojo, [3]Interrupción Intermitente)

// Manejadores de señales
void stateChange(int signal);
void forwardStateChange(int signal);

// Declaracion de funciones
void reverse(char s[]);
void itoa(int n, char s[]);

int main(int argc, const char * argv[])
{
    struct sockaddr_in direccion;
    char buffer[1000];
    
    ssize_t leidos, escritos;
    
    if (argc != 2) {
        printf("Use: %s IP_Servidor \n", argv[0]);
        exit(-1);
    }
    
    // Crear el socket
    cliente = socket(PF_INET, SOCK_STREAM, 0);
    // Establecer conexión
    inet_aton(argv[1], &direccion.sin_addr);
    direccion.sin_port = htons(TCP_PORT);
    direccion.sin_family = AF_INET;
    
    escritos = connect(cliente, (struct sockaddr *) &direccion, sizeof(direccion));
    
    if (escritos == 0) {
        printf("Conectado a %s:%d \n",
               inet_ntoa(direccion.sin_addr),
               ntohs(direccion.sin_port));

        // Mandar el PID correspondiente a este semaforo y obtener el PID del siguiente semaforo 
        itoa(getpid(), buffer); 
        write(cliente, buffer, sizeof(int));
        leidos = read(cliente, &buffer, sizeof(buffer));
        next_pid = atoi(buffer);

        // Establecer señales a ser recibidas por el semáforo al obtener todos los parametros necesarios
        signal(SIGUSR1, stateChange);
        signal(SIGALRM, forwardStateChange);

        // Escuchar de la consola remota para recibir interrupciones y manejarlas
        while (leidos = read(cliente, &buffer, sizeof(buffer))) {
            if (strcmp(buffer, "START") == 0) {
                raise(SIGUSR1);
            } else if (strcmp(buffer, "STOP") == 0 && estado != 2) {
                estado = 2;
                // Bloquear señales SIGUSR1 y SIGALRM
            } else if (strcmp(buffer, "INTERMITENT") == 0 && estado != 3) {
                estado = 3;
                // Bloquear señales SIGUSR1 y SIGALRM
            }
            else {
                // Ranudar señales si se mandan por segunda vez
                // Usar señales pendientes (para captar el último alarm)
            }
        }
    }
    
    // Cerrar sockets
    close(cliente);
    
    return 0;
}

/*  Manejador de señales recibido para cambiar a estado VERDE y comenzar un temporizador para mantenerse en dicho estado
    @param signal: id de la señal recibida
*/
void stateChange(int signal) {
    estado = 1;
    char state[] = "VERDE"; 
    write(cliente, &state, sizeof(state));
    alarm(TIEMPO);
}

/*  Manejador de señales para indicar al siguiente semaforo que un cambio de estado debe ser realizado
    @param signal: id de señal recibida
*/
void forwardStateChange(int signal) {
    estado = 0;
    kill(next_pid, SIGUSR1);
}

/*  Invertir el orden de un string dado. 
    @param s: string a invertir
*/
void reverse(char s[]) {
    int i, j;
    char c;
    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

/*  Cambiar un numero entero dado a un string
    @param n: numero entero a cambiar
    @param s: buffer en el cual se almacenará el string resultante
*/
void itoa(int n, char s[]) {
    int i, sign;
    if ((sign = n) < 0)
        n = -n;
    i = 0;
    do {
        s[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}