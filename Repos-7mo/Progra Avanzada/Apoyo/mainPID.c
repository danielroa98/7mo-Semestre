//
//  main.c
//  creando_procesos
//
//  Created by Vicente Cubells Nonell on 31/08/15.
//  Copyright (c) 2015 Vicente Cubells Nonell. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
int promedio(int a, int b);
int maximoArchivos(int *vector, int n);
void imprimirHistograma(int *vector, int *vector2, int n);

int main(int argc, char * argv[]) {

  char *cvalue = NULL;
  int c;

  int n=0;

  while ((c = getopt (argc, argv, "n:")) != -1)
        switch (c)
    {
        case 'n':
            cvalue = optarg;
            break;

        case '?':
            if (optopt == 'n')
                fprintf (stderr, "Opción -%c requiere un argumento.\n", optopt);
            else if (isprint(optopt))
                fprintf (stderr, "Opción desconocida '-%c'.\n", optopt);
            else
                fprintf (stderr,
                         "Opción desconocida '\\x%x'.\n",
                         optopt);
            return 1;
        default:
            abort ();
    }




  n=atoi(cvalue);

  int * vector = (int *) malloc(n * sizeof(int));
  int * vector2 = (int *) malloc(n * sizeof(int));
    /* Empieza el loop de creacion*/
    int * final = vector + n;

/*
    for (int *aux = vector; aux < final; ++aux) {

        printf("%d\n",*aux);

    }*/


    int *aux2=vector2;
    for (int *aux = vector; aux < final; ++aux){
      pid_t pid;
      int estado;


      int tubo[2];
      int tubo2[2];

      /* Crear un pipe */
      pipe(tubo);
      pipe(tubo2);
      pid = fork();

      /* De aquí en adelante lo ejecutan los dos procesos */
      if (pid == -1)
      {
          printf("Error al crear el proceso hijo \n");
          return -1;
      }
      else if (pid == 0)
      {
          // Estamos en el hijo
          /*printf("Estamos en el proceso hijo con PID = %d y su padre es PPID = %d \n", \
                 getpid(), getppid());*/
                 int prom=promedio(getpid(),getppid());
                 printf("Soy el proceso hijo con PID = %d y mi promedio es = %d\n",getpid(),prom);
                 //printf("Promedio hijo: %d\n",prom);

                 /*Poniendo valores en tubo*/
                 close (tubo[0]);
                 write(tubo[1],&prom,sizeof(int));

                 int id=getpid();
                 close (tubo2[0]);
                 write(tubo2[1],&id,sizeof(int));





          exit(0);
      }
      else {
          // Estamos en el padre
          /*printf("Estamos en el proceso padre con PID = %d y su padre es PPID = %d \n", \
                 getpid(), getppid());*/

          // Poner al padre a esperar que el hijo termine


          if (waitpid(pid, &estado, 0) != -1)
          {
            int a =WEXITSTATUS(estado);
            //printf("EXIT CODE: %d\n",a );
              if (WIFEXITED(estado)){
                //printf("Ya terminó el hijo con PID %d con valor de retorno %d \n", pid, WEXITSTATUS(estado));

                close(tubo[1]);
                int promedioexit=0;
                read(tubo[0],&promedioexit,sizeof(int));
                //printf("Promedio segun padre: %d\n",promedioexit);
                *aux=promedioexit;
                close(tubo2[1]);
                read (tubo2[0],aux2,sizeof(int));
                //printf("AUX2: %d\n",*aux2);
              }

          }

          //printf("Después del wait\n");

      }

      /* Lo van a imprimir los dos procesos */
      /*
      printf("Estamos en el proceso con PID = %d y su padre es PPID = %d \n", \
                     getpid(), getppid());*/

    aux2++;
    }


    aux2=vector2;

    imprimirHistograma(vector, vector2,n);
    free(vector);
    free(vector2);
    return 0;
}




int promedio(int a, int b){
  return((a+b)/2);
}


void imprimirHistograma(int *vector, int *vector2, int n){
  int max=maximoArchivos(vector, n);
  //printf("%d\n",max);
  double asteriscos=40.0;
  //int urna=getUrna(vector);


  int * final = vector + n;
  int *aux2=vector2;


  printf("%8s %8s %10s\n","PID hijo","Promedio","Histograma");
  for (int *aux = vector; aux < final; ++aux) {

    printf("%8d: %8d ",*aux2,*aux);
    //printf("    %f\n",(*aux*asteriscos)/max);
    //printf(" Printing ***");
    for (int i=0; i<(*aux*asteriscos)/max;++i){
      printf("*");
    }
    printf("\n");


    //printf("%d: %d\n",*aux,*aux2);
    aux2++;
  }



}


int maximoArchivos(int *vector,int n){
  int max=0;
  int *aux=vector;
  int *final=vector +n;

  for (aux=vector; aux < final; ++aux) {
      if(max<*aux){
        max=*aux;
        //printf("NEW MAX: %d\n",max);
      }
	}
  return max;
}
