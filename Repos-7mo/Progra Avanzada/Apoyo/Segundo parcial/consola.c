/*
  Equipo 14:
  - Octavio Garduza
  - Christian Dalma

  Actividad 6 | Sockets y Señales

  Consola

  Implementación de una consola de administración centralizada utilizando
  sockets y manejo de señales que:
   - Muestra el estado actual de cada semáforo
   - Si recibe Ctrl+Z establece todos los semáforos en amarillo
   - Si recibe Ctrl+C establece todos los semáforos en rojo

 Algunas partes de la actividad la realizamos en conjunto con el equipo de
 Sabrina Santana y Rubén Sánchez:

 - Manejo de señales de SIGTSTP y SIGINT.

 - Implementación de un socket de escritura y uno de lectura.
*/


#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define TCP_PORT_REC 9000 //Puerto de lectura
#define TCP_PORT_SND 8000 //Puerto de escritura

void ctrlc_handler(int);
void ctrlz_handler(int);

char buffer[1000];

int rec_client; //Socket de lectura
int snd_client; //Socket de escritura
int ctrlc_counter = 0;
int ctrlz_counter = 0;

pid_t current_pid;


/*
 * Function:  ctrlc_handler
 * --------------------
 *
 * Envía el código 2 [rojo] a todos los semáforos
 *
 * signum: ID de señal SIGINT
 *
*/
void ctrlc_handler(int signum){
  printf("\n");
  if(current_pid == 0){
    if(ctrlc_counter % 2 == 0){
      sprintf(buffer, "%d", 0);
    }
    else{
      sprintf(buffer, "%d", 2);
    }
    write(snd_client, &buffer, sizeof(buffer));
    ctrlc_counter++;
  }
}
/*
 * Function:  ctrlc_handler
 * --------------------
 *
 * Envía el código 1 [amarillo] a todos los semáforos
 *
 * signum: ID de señal SIGTSTP
 *
*/
void ctrlz_handler(int signum){
  printf("\n");
  if(current_pid == 0){
    if(ctrlz_counter % 2 == 0){
      sprintf(buffer, "%d", 1);
    }
    else{
      sprintf(buffer, "%d", 2);
    }
    write(snd_client, &buffer, sizeof(buffer));
    ctrlz_counter++;
  }
}

int main(int argc, const char * argv[]){

  struct sockaddr_in rec_address;
  struct sockaddr_in snd_address;

  int rec_socket;
  int snd_socket;
  int s_pid;

  int current_state;

  ssize_t size_data;

  socklen_t size_rec_socket;
  socklen_t size_snd_socket;

  int next = 1;

  signal(SIGTSTP, ctrlz_handler);
  signal(SIGINT, ctrlc_handler);

  if(argc != 2) {
    printf("Use: %s IP_Servidor \n", argv[0]);
    exit(-1);
  }

  //Crear sockets de lectura y escritura
  rec_socket = socket(PF_INET, SOCK_STREAM, 0);
  snd_socket = socket(PF_INET, SOCK_STREAM, 0);


  // Enlace con el ambos sockets
  inet_aton(argv[1], &rec_address.sin_addr);
  rec_address.sin_port = htons(TCP_PORT_REC);
  rec_address.sin_family = AF_INET;

  inet_aton(argv[1], &snd_address.sin_addr);
  snd_address.sin_port = htons(TCP_PORT_SND);
  snd_address.sin_family = AF_INET;

  size_rec_socket = sizeof(rec_address);
  size_snd_socket = sizeof(snd_address);

  bind(rec_socket, (struct sockaddr *) &rec_address, size_rec_socket);
  bind(snd_socket, (struct sockaddr *) &snd_address, size_snd_socket);

  // Escuhar ambos sockets
  listen(rec_socket, 10);
  listen(snd_socket, 10);

  //Aceptar conexiones de lectura
  while(next){

    rec_client = accept(rec_socket, (struct sockaddr *) &rec_address, &size_rec_socket); //regresa el file descriptor
    snd_client = accept(snd_socket, (struct sockaddr *) &snd_address, &size_snd_socket); //regresa el file descriptor

    printf("Aceptando conexiones en %s:%d \n", inet_ntoa(rec_address.sin_addr), ntohs(rec_address.sin_port));
    printf("Aceptando conexiones en %s:%d \n", inet_ntoa(snd_address.sin_addr), ntohs(snd_address.sin_port));

    current_pid = fork();

    if(current_pid == 0){
      next = 0;
    }
  }

  if(current_pid == 0){//Proceso hijo

    signal(SIGTSTP, ctrlz_handler);
    signal(SIGINT, ctrlc_handler);

    close(rec_socket);

    if(rec_client >= 0){
      read(rec_client, &buffer, sizeof(buffer));
      s_pid = atoi(buffer);
      while(size_data = read(rec_client, &buffer, sizeof(buffer))){ //Al recibir un mensaje del semáforo
        current_state = atoi(buffer);
        if(current_state == 0){
          printf("Semáforo: %d está en rojo (%d)\n", s_pid, current_state);
        }
        else if(current_state == 1){
          printf("Semáforo: %d está en amarillo (%d)\n", s_pid, current_state);
        }
        else if(current_state == 2){
          printf("Semáforo: %d está en verde (%d)\n", s_pid, current_state);
        }
      }
    }
    close(rec_client);
  }
  else if (current_pid > 0)
  {
    while (wait(NULL) != -1);
    close(rec_socket);
  }
  return 0;
}//Fin de main