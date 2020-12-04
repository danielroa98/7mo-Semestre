/*
  Equipo 14:
  - Octavio Garduza
  - Christian Dalma

  Actividad 6 | Sockets y Señales

  Semáforo

  Implementación de un circuito de n semáforos utilizando
  sockets y manejo de señales donde cada uno:
   - Permanece en verde durante 30 segundos
   - Cede el turno al semáforo a su derecha utilizando SIGUSR1

 Algunas partes de la actividad la realizamos en conjunto con el equipo de
 Sabrina Santana y Rubén Sánchez:

 - Escribir los pids a un archivo, leer el archivo y guardarlos en un arreglo,
   buscar el pid del proceso siguiente al actual.

 - Implementación de un socket de escritura y uno de lectura.
*/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

void alarm_handler(int);
void sigusr1_handler(int);

void change_to_red();
void change_to_yellow();

#define TCP_PORT_SND 9000 //Puerto de escritura
#define TCP_PORT_REC 8000 //Puerto de lectura
#define S 4               //Cantidad de semáforos

int current_state = 0; //Color del semáforo [Rojo: 0, Amarillo: 1, Verde: 2]
int next_pid;
char buffer[1000];
int rec_socket; //Socket de lectura
int snd_socket; //Socket de escritura
int im_current;

/*
 * Function:  alarm_handler
 * --------------------
 *
 * Cambia el estado del semáforo a rojo, envía el nuevo estado a la consola y
 * envía un SIGUSR1 al proceso siguiente.
 *
 * signum: ID de señal SIGALRM
 *
 *
*/
void alarm_handler(int signum)
{
    printf("\nSemáforo: %d. Ha pasado 30 segundos en verde. Cambio de estado a [rojo]\n", getpid());
    printf("Cambio al semáforo de la derecha\n", getpid());
    im_current = 0;
    current_state = 0; //Rojo
    sprintf(buffer, "%d", current_state);
    printf("Actualizando estado de [rojo] en la consola\n");
    write(snd_socket, &buffer, sizeof(buffer));
    kill(next_pid, SIGUSR1);
}

/*
 * Function:  sigusr1_handler
 * --------------------
 *
 * Cambia el estado del semáforo a verde, envía el nuevo estado a la consola y
 * resetea la alarma a 30 segundos
 *
 * signum: ID de señal SIGUSR1
 *
 *
*/
void sigusr1_handler(int signum)
{
    printf("\nSemaforo: %d. Ha recibido un SIGURS1. Cambio de estado a [verde]\n", getpid());
    im_current = 1;
    current_state = 2; //Verde
    sprintf(buffer, "%d", current_state);
    printf("Actualizando estado [verde] en la consola\n");
    write(snd_socket, &buffer, sizeof(buffer));
    signal(SIGALRM, alarm_handler);
    alarm(30);
}

int main(int argc, const char *argv[])
{

    FILE *fp;

    struct sockaddr_in rec_address;
    struct sockaddr_in snd_address;

    ssize_t size_data;
    ssize_t size_rec_socket;
    ssize_t size_snd_socket;

    int current_pid;
    int index;

    char pid_elem[10];
    int pid_list[4];

    int next_color = 0;

    if (argc != 2)
    {
        printf("Use: %s IP_Servidor \n", argv[0]);
        exit(-1);
    }

    signal(SIGUSR1, sigusr1_handler);

    //Crear sockets de lectura y escritura
    rec_socket = socket(PF_INET, SOCK_STREAM, 0);
    snd_socket = socket(PF_INET, SOCK_STREAM, 0);

    //Enlace con ambos sockets
    inet_aton(argv[1], &rec_address.sin_addr);
    rec_address.sin_port = htons(TCP_PORT_REC);
    rec_address.sin_family = AF_INET;

    inet_aton(argv[1], &snd_address.sin_addr);
    snd_address.sin_port = htons(TCP_PORT_SND);
    snd_address.sin_family = AF_INET;

    //Establecer ambas conexiones.
    size_snd_socket = connect(snd_socket, (struct sockaddr *)&snd_address, sizeof(snd_address));
    sleep(2);
    size_rec_socket = connect(rec_socket, (struct sockaddr *)&rec_address, sizeof(rec_address));

    if (size_rec_socket == 0)
    { //Encontro server
        printf("\n\n");
        printf("Conectado lectura a %s:%d \n", inet_ntoa(rec_address.sin_addr), ntohs(rec_address.sin_port));
        printf("Conectado escritura a %s:%d \n", inet_ntoa(snd_address.sin_addr), ntohs(snd_address.sin_port));

        current_pid = getpid();
        sprintf(buffer, "%d", current_pid);

        fp = fopen("pid_db.txt", "a"); //Archivo que almacena pids
        fprintf(fp, "%s\n", buffer);
        fclose(fp);
        write(snd_socket, &buffer, sizeof(buffer));

        printf("\nCuando todos los semáforos hayan sido iniciados, presione cualquier tecla para comenzar\n");
        getchar();

        fp = fopen("pid_db.txt", "r");

        //Leer archivo y almacenar semáforos
        index = 0;
        while (feof(fp) == 0)
        {
            fgets(pid_elem, 10, fp);
            pid_list[index] = atoi(pid_elem);
            index++;
        }
        fclose(fp);

        //Buscar semáforo siguiente al actual
        for (index = 0; index < 4; index++)
        {
            if (pid_list[index] == current_pid && index != 3)
            {
                next_pid = pid_list[index + 1];
            }
            else if (pid_list[index] == current_pid)
            {
                next_pid = pid_list[0];
            }
        }
        printf("Semáforo %d está en línea\n", current_pid);
        printf("Semáforo a la derecha: %d\n", next_pid);

        if (current_pid == pid_list[0])
        { //Si es el semáforo inicial
            raise(SIGUSR1);
        }

        while (size_data = read(rec_socket, &buffer, sizeof(buffer)))
        { //Al recibir un mensaje de la consola
            next_color = atoi(buffer);
            if (next_color == 0)
            { //Todos a rojo
                change_to_red();
            }
            else if (next_color == 2)
            { //Todos a verde
                if (im_current)
                {
                    raise(SIGUSR1);
                }
            }
            else if (next_color == 1)
            { //Todos a amarillo
                change_to_yellow();
            }
        }
    }
    close(rec_socket);
    return 0;
}

void change_to_red()
{
    printf("\nHe recibido la señal de emergencia: [Todos a rojo]\n");
    current_state = 0; //Rojo
    sprintf(buffer, "%d", current_state);
    write(snd_socket, &buffer, sizeof(buffer));
    alarm(0);
}

void change_to_yellow()
{
    printf("\nHe recibido la señal de emergencia: [Todos a amarillo]\n");
    current_state = 1; //Amarillo
    sprintf(buffer, "%d", current_state);
    write(snd_socket, &buffer, sizeof(buffer));
    alarm(0);
}